#ifndef UAN_MAC_H_
#define UAN_MAC_H_

#include "uan-class.h"

#define random(x) (rand()%x)
#define BacktimeRange 10

void MAC_DataRecv(char input[]);
void* MAC_RecvControlThread(void *ptr);
MACData MAC_Package(char Macflag,char* MACSeq,char sou_ID,char next_ID,char des_ID, AppData ad);
bool MAC_DataSend(DataPkt dataPkt);
void* MAC_SendControlThread(void *ptr);     //MAC层发送控制线程
void MAC_SendControlThread_create();
void MAC_RecvControlThread_create();



#endif /* UAN_MAC_H_ */
