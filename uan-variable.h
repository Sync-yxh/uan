#ifndef UAN_VARIABLE_H_
#define UAN_VARIABLE_H_

#include <iostream>
#include <list>
#include <map>
#include <string.h>
#include <vector>

using namespace std;

#define MAXLength 200       //串口长度
#define SendState 1         //串口状态
#define RecvState 2			//串口状态
#define RecordCNT   1
#define EtEDelay 20

typedef unsigned short uint16;

extern pthread_mutex_t mut;  // 互斥锁
extern char m_ID;       //自身硬件地址
extern uint16 m_Depth;

extern unsigned int SysTimeCnt;       //系统时间计时器

extern char stateSeqNum;              
extern char dataSeqNum;                

extern int MACWaitRecv;       //MAC等待接收标志
extern int MACWaitSend;       //MAC等待发送标志
extern int RouteWaitSend;    //路由等待发送标志
extern int RouteWaitRecv;     //路由等待接收标志

#include "uan-class.h"

extern vector<MACResult> MACResultArray;         //MAC发送结果记录数组

extern list< list<char> > MACRecvBuff;               //MAC接收缓存
extern list<DataPkt> MACSendBuff;                  //MAC发送缓存
extern list<RouteData> RouteRecvBuff;              //路由接收缓存
extern list<RouteData> RouteSendBuff;              //路由发送缓存
extern list<AppData> DataRecvBuff;

extern vector<State>  QstateVector;             //状态组
extern map<State, double> QTable;               //Q值表

extern vector<char> ParentIDBuff;
extern vector<char> ChildIDBuff;

extern int SysPortState;
extern char readbuff[MAXLength];
extern char sendbuff[MAXLength];
extern int serial_fd1;
extern int readlen;
extern int sendlen;

extern int Backtime;

extern int QLStage;

#endif /* UAN_VARIABLE_H_ */
