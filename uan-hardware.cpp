#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <pthread.h>

#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>

#include <errno.h>
#include <malloc.h>
#include <sys/ioctl.h>

#include <time.h>

#include <math.h>

#include "uan-hardware.h"
#include "uan-variable.h"
#include "uan-mac.h"
#include "uan-app.h"
#include "uan-route.h"

using namespace std;

void PrintTable();
void SendPacket();
void RecvPacket();

int Serialport1_init()
{
	int serial_fd;
	struct termios newopt,oldopt;
	serial_fd = open("/dev/ttySAC1",O_RDWR | O_NOCTTY |O_NONBLOCK );//
	if(serial_fd == -1)
	{
		perror("open error!");
		exit(0);
		//return -1;
	}
	else
	{
		cout<<"open port: "<<serial_fd<<endl;
	}
	if(tcgetattr(serial_fd, &oldopt) != 0)
	{
		perror("serial get error!");
		exit(0);
		//return -1;
	}

	newopt.c_cflag |= CLOCAL | CREAD;
	newopt.c_cflag &= ~CSIZE;
	newopt.c_cflag |= CS8;
	newopt.c_cflag &= ~PARENB;
	cfsetispeed(&newopt, B115200);
	cfsetospeed(&newopt, B115200);
	newopt.c_cflag &= ~CSTOPB;
	newopt.c_lflag &= ~(ICANON | ECHO | ECHOE);  //origin mode
	newopt.c_cc[VTIME] = 0;
	newopt.c_cc[VMIN] = 1;                       //at least 1 char to receive
	tcflush(serial_fd, TCIFLUSH);

	if(tcsetattr(serial_fd, TCSANOW ,&newopt) != 0)
	{
		perror("serial set error!");
		exit(0);
	}

	cout<<"serial port1 init complete."<<endl;

	return serial_fd;
}

void *serialport1_Send_monitorPthread(void *ptr)
{
	int writeresult;
	while(1)
	{
		if(SysPortState == SendState)
		{
			// tcflush(serial_fd1, TCOFLUSH);
			/**为了测试时串口端显示问题，添加换行，在真正代码中，需要去掉**/
			sendbuff[sendlen] = '\n';
			// writeresult = write(serial_fd1,sendbuff,(sendlen+1));
			writeresult = true;
			if(writeresult > 0)
			{
				PrintTime(SysTimeCnt);
				cout<<"send success:  ";
				for(int i=0;i<sendlen;i++)
					cout<<(unsigned int)(sendbuff[i] & 0x00FF)<<" ";
				cout<<endl;
			}
			SysPortState = RecvState;
		}
	}
	return 0;
}

void serialport1_Send_monitorPthread_create()
{
	printf("-----serialport1_Send_monitorPthread_create!----\n");
	pthread_t Pthread_1;
	pthread_create(&Pthread_1, NULL, serialport1_Send_monitorPthread, NULL);
}

void *serialport1_Recv_monitorPthread(void *ptr)
{
	// tcflush(serial_fd1, TCIFLUSH);
	while(1)
	{
		if(SysPortState == RecvState)
		{
			memset(readbuff,0,sizeof(char)*MAXLength);
			// readlen = read(serial_fd1,readbuff,MAXLength);
			string readstr;
			cin >> readstr;
			readlen = readstr.size();
			strcpy(readbuff,readstr.c_str());
			if(readlen > 0)
			{
				if(!strcmp(readbuff,"cmd1")){
					// PrintTable();
				}
				else if(!strcmp(readbuff,"cmd2")){
					// SendPacket();
				}
				else if(!strcmp(readbuff,"cmd3")){
					// RecvPacket();
				}
				else
				{
					PrintTime(SysTimeCnt);
					cout<<"recv success:  ";
					for(int i=0;i<readlen;i++){
						readbuff[i] -= 48;
						cout<<(unsigned int)(readbuff[i] & 0x00FF)<<" ";
					}
					cout<<endl;

					list<char> macrecv(readbuff,readbuff+readlen);
					pthread_mutex_lock(&mut);
					MACRecvBuff.push_back(macrecv);
					MACWaitRecv++;
					pthread_mutex_unlock(&mut);
				}
			}
		}
	}
	return 0;
}

void serialport1_Recv_monitorPthread_create()
{
	printf("-----serialport1_Recv_monitorPthread_create!----\n");
	pthread_t Pthread_2;
	pthread_create(&Pthread_2, NULL, serialport1_Recv_monitorPthread, NULL);
}

void PrintTime(unsigned int time)
{
	int second = time % 60;
	int minite = time / 60;
	int hour = minite / 60;
	minite = minite % 60;
	cout << "Time: "<<hour<<" hour "<<minite<<" min "<<second<<" sec   ";
}
int flag = 0;

void timer_counter(int nSignal)	//定时器计数函数
{
	pthread_mutex_lock(&mut);
	SysTimeCnt++;
	pthread_mutex_unlock(&mut);

	if(SysTimeCnt % 300 == 0){
		SendAppState();
		flag = 0;
		return;
	}
	if((SysTimeCnt % 300) == 9){
		UpdataQ();
		flag = 1;
	}
	if(SysTimeCnt % 5 == 0 && flag ==1){
		SendAppData();
	}
	if(SysTimeCnt >= 60000){
		SysTimeCnt = 0;
	}
}

void TimerInit()
{
	struct itimerval tick;
	signal(SIGALRM, timer_counter);				//接收到SIGALRM信号就计数
	memset(&tick, 0, sizeof(tick));             //tick清零
	tick.it_value.tv_sec = 1;                   //0秒后启动定时器
	tick.it_value.tv_usec = 0;
	tick.it_interval.tv_sec = 1;                //每隔1s发送一次SIGALRM信号
    tick.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &tick, NULL);        //ITIMER_REAL : 以系统真实的时间来计算，它送出SIGALRM信号
}

void PortSend(char* send,int sizecnt)
{
	sendlen = sizecnt;
	memcpy(sendbuff,send,sizecnt);

	SysPortState = SendState;
}

int ListenChanel()
{
	return true;
	if(readlen == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void WaitDelay(unsigned int waittime)
{
	unsigned int starttime = SysTimeCnt;
	while(SysTimeCnt - starttime < waittime){};
}

unsigned int CountBackoffTime(int backofftime)
{
	return (unsigned int)(pow(2,random(backofftime - 1)) * EtEDelay);
}

//void PrintTable()
//{
//	cout<<"Node ID        Hop           Valid           Path "<<endl;
//	for(auto iter = m_RouTable.begin();iter!=m_RouTable.end();iter++)
//	{
//		cout<<(int)iter->Des_ID<<"             "<<(int)iter->hop<<"             ";
//		cout<<iter->valid<<"             ";
//		for(int i = 0;i<iter->hop;i++)
//			cout<<(int)iter->Point[i]<<",";
//		cout<<endl;
//	}
//}

//void SendPacket()
//{
//	AppData ad;
//	ad.Datalenth = 2;
//	ad.Data[0] = 1;
//	ad.Data[1] = 1;
//	AppSend(8,ad);
//}

//void RecvPacket()
//{
//	AppData ad;
//	while(1)
//	{
//		bool notempty = AppRecv(&ad);
//		if(notempty)
//		{
//			cout<<"recv packet: ";
//			for(int i=0;i<ad.Datalenth;i++)
//				cout<<(int)ad.Data[i]<<" ";
//			cout<<endl;
//		}
//		else
//			break;
//	}
//}
