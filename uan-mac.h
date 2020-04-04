#ifndef UAN_MAC_H_
#define UAN_MAC_H_

#include "uan-class.h"

#define random(x) (rand()%x)
#define BacktimeRange 10

void MAC_DataRecv(char input[]);
void* MAC_RecvControlThread(void *ptr);

bool MAC_DataSend(DataPkt dataPkt);
bool MAC_DataSend(InitPkt initPkt);
bool MAC_DataSend(StatePkt statePkt);

void* MAC_SendControlThread(void *ptr);     //MAC层发送控制线程
void MAC_SendControlThread_create();
void MAC_RecvControlThread_create();



#endif /* UAN_MAC_H_ */
