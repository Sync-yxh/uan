#include <iostream>
#include <fstream>
#include <list>
#include <string.h>
#include <stdlib.h>

#include "uan-variable.h"
#include "uan-class.h"
#include "uan-mac.h"
#include "uan-handle.h"
#include "uan-hardware.h"

using namespace std;

void MAC_DataRecv(char input[])
{
	DataPkt dataPkt;
	dataPkt.flag = input[0];
	dataPkt.seqNum = input[1];
	dataPkt.srcID = input[2];
	dataPkt.relayID = input[3];
	dataPkt.dstID = input[4];
	dataPkt.data.Datalenth = input[5];
	sizecnt = 6;
	for (int i = 0; i < dataPkt.data.Datalenth; i++)
	{
		arr[6 + i] = dataPkt.data.Data[i];
		sizecnt ++;
	}
	if(dataPkt.flag == 3)
	{
		if(dataPkt.dstID == m_ID){
			RouteData rd;
			rd.flag = dataPkt.flag;
			rd.seqNum = dataPkt.seqNum;
			rd.srcID = dataPkt.srcID;
			rd.data = dataPkt.data;
			pthread_mutex_lock(&mut);
			RouteRecvBuff.push_back(rd);    //存入路由接收缓存
			RouteWaitRecv++;
			pthread_mutex_unlock(&mut);
		}
	}
}

void* MAC_RecvControlThread(void *ptr)
{
	while(1)
	{
		pthread_mutex_lock(&mut);
		bool localrecvflag = (!MACRecvBuff.empty()) && (MACWaitRecv > 0);   //判断接收缓存是否为空，接收等待标志是否大于0
		pthread_mutex_unlock(&mut);
		if (localrecvflag)   //有缓存数据接收
		{
			auto macrecv = MACRecvBuff.begin();        //从接收缓存中读取数据
			int list_char_length = macrecv->size();    //得到list<char>的长度
			char *input = new char[list_char_length];  //新建相对应长度input数组
			int char_i = 0;
			for (auto i = macrecv->begin(); i != macrecv->end(); i++)  //赋值input数组中的char元素
			{
				input[char_i] = *i;
				char_i ++ ;
			}

			if (input[0] == 1 || input[0] == 2){
				HandleRou(input);
			}
			else if (input[0] == 3){
				MAC_DataRecv(input);
			}
			else{
				cout<<"报文 flag = "<< input[0] <<" , 无法正确解析"<< endl;
			}
			delete[] input;

			pthread_mutex_lock(&mut);
			MACRecvBuff.pop_front();                 //删除已经读取的数据
			MACWaitRecv--;                           //MAC接收等待标志减一
			pthread_mutex_unlock(&mut);
		}
	}
	return 0;
}

bool MAC_DataSend(DataPkt dataPkt)
{
	char *arr = new char[108];
	int sizecnt = 0;
	memset(input,0,108*sizeof(char));
	bool DataSendResult = false;
	arr[0] = dataPkt.flag;
	arr[1] = dataPkt.seqNum;
	arr[2] = dataPkt.srcID;
	arr[3] = dataPkt.relayID;
	arr[4] = dataPkt.dstID;
	arr[5] = dataPkt.data.Datalenth;
	sizecnt = 6;
	for (int i = 0; i < dataPkt.data.Datalenth; i++)
	{
		arr[6 + i] = Mac_Data.mAD.Data[i];
		sizecnt ++;
	}
	PortSend(arr,sizecnt);
	delete[]arr;
	DataSendResult = true;
    return DataSendResult;  //返回发送是否成功的结果
}

bool MAC_DataSend(InitPkt initPkt)
{
	char *arr = new char[108];
	int sizecnt = 0;
	memset(input,0,108*sizeof(char));
	bool SendResult = false;
	arr[0] = initPkt.flag;
	arr[1] = initPkt.seqNum;
	arr[2] = initPkt.srcID;
	arr[3] = (char)(initPkt.Depth >> 8);
	arr[4] = (char)(initPkt.Depth & 0x0F);
	sizecnt = 5;
	PortSend(arr,sizecnt);
	delete[]arr;
	SendResult = true;
    return SendResult;  //返回发送是否成功的结果
}

bool MAC_DataSend(StatePkt statePkt)
{
	char *arr = new char[108];
	int sizecnt = 0;
	memset(input,0,108*sizeof(char));
	bool SendResult = false;
	arr[0] = statePkt.flag;
	arr[1] = statePkt.seqNum;
	arr[2] = statePkt.srcID;
	arr[3] = statePkt.bandwidth;
	arr[4] = statePkt.connect;
	arr[5] = (char)(statePkt.Depth >> 8);
	arr[6] = (char)(statePkt.Depth & 0x0F);
	sizecnt = 7;
	PortSend(arr,sizecnt);
	delete[]arr;
	SendResult = true;
    return SendResult;  //返回发送是否成功的结果
}

void* MAC_SendControlThread(void *ptr)     //MAC层发送控制线程
{
	int Backtime = 0;
	while (true)
	{
		if (!MACSendBuff.empty() && MACWaitSend > 0)
		{
			bool result = true;  //用以判断发送是否成功
			while (!ListenChanel())
			{
				WaitDelay(CountBackoffTime(Backtime++));  //等待信道空闲
			}
			Backtime = 0;

			auto iter = MACSendBuff.begin();

			result = MAC_DataSend(*iter);

			pthread_mutex_lock(&mut);
			MACSendBuff.pop_front();
			MACWaitSend--;
			pthread_mutex_unlock(&mut);

			MACResult MR;
			MR.flag = (*iter).flag;
			MR.TimeStamp = SysTimeCnt;
			MR.seqNum = (*iter).seqNum;
			MR.result = result;

			//MACResultArray记录
			MACResultArray.push_bakc(MR);

			if (MACResultCount.size() == RecordCNT)
			{
				fstream outfile;
				outfile.open("MACResult.txt", ios::in | ios::out | ios::app);
				for (int i = 0; i < RecordCNT; i++)
				{
					outfile.width(6);
					outfile.flags(ios::right);
					outfile << (int)MACResultArray[i].flag;
					outfile.width(6);
					outfile.flags(ios::right);
					outfile << MACResultArray[i].TimeStamp;
					outfile.width(6);
					outfile.flags(ios::right);
					outfile<< (int) MACResultArray[i].seqNum;
					outfile.width(6);
					outfile.flags(ios::right);
					outfile<< MACResultArray[i].result << endl;
				}
				MACResultArray.clear();  //清空数组
				outfile.close();
			}
		}
	}
	return 0;
}

void MAC_SendControlThread_create()
{
	printf("-----MAC_SendControlThread_create!----\n");
	pthread_t Pthread_macs;
	pthread_create(&Pthread_macs, NULL, MAC_SendControlThread, NULL);
}
void MAC_RecvControlThread_create()
{
	printf("-----MAC_RecvControlThread_create!----\n");
	pthread_t Pthread_macr;
	pthread_create(&Pthread_macr, NULL, MAC_RecvControlThread, NULL);
}
