#include <iostream>
#include <fstream>
#include <list>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>


#include "uan-variable.h"
#include "uan-class.h"
#include "uan-hardware.h"
#include "uan-route.h"
#include "uan-handle.h"

void SendAppData()
{
	RouteData Rdata;
	Rdata.flag = 3;
	Rdata.seqNum = dataSeqNum;
	Rdata.srcID = m_ID;
	dataSeqNum ++;
	AppData oridata;
	oridata.Datalenth = 5;
	for(int i=0;i<oridata.Datalenth;i++){
		oridata.Data[i] = i+1;
	}
	Rdata.data = oridata;
	pthread_mutex_lock(&mut);
	RouteWaitSend++;
	RouteSendBuff.push_back( Rdata );
	pthread_mutex_unlock(&mut);
}

void SendAppState()
{
	dataSeqNum = 0;
	StatePkt pkt;
	pkt.flag = 2;
	pkt.seqNum = stateSeqNum;
	pkt.srcID = m_ID;
	pkt.bandwidth = MACWaitSend;
	pkt.connect = ChildIDBuff.size();
	pkt.Depth = m_Depth;
	ToMAC(pkt);
	ChildIDBuff.clear();
	ParentIDBuff.clear();
	QstateVector.clear();
	pthread_mutex_lock(&mut);
	MACWaitSend = 0;
	MACSendBuff.clear();
	pthread_mutex_unlock(&mut);
}

// void ScheduleData()
// {
// 	struct itimerval tick;
// 	signal(SIGALRM, SendAppData);				//接收到SIGALRM信号就计数
// 	memset(&tick, 0, sizeof(tick));             //tick清零
// 	tick.it_value.tv_sec = 1;                   //1秒后启动定时器
// 	tick.it_value.tv_usec = 0;
// 	tick.it_interval.tv_sec = 2;                //每隔2s发送一次SIGALRM信号
//     tick.it_interval.tv_usec = 0;
// 	setitimer(ITIMER_REAL, &tick, NULL);        //ITIMER_REAL : 以系统真实的时间来计算，它送出SIGALRM信号
// }

// void ScheduleState()
// {
// 	struct itimerval tick;
// 	signal(SIGALRM, SendAppState);				//接收到SIGALRM信号就计数
// 	memset(&tick, 0, sizeof(tick));             //tick清零
// 	tick.it_value.tv_sec = 300;                   //5s后启动定时器
// 	tick.it_value.tv_usec = 0;
// 	tick.it_interval.tv_sec = 300;                //每隔5m发送一次SIGALRM信号
//     tick.it_interval.tv_usec = 0;
// 	setitimer(ITIMER_REAL, &tick, NULL);        //ITIMER_REAL : 以系统真实的时间来计算，它送出SIGALRM信号
// }
