#ifndef UAN_HARDWARE_H_
#define UAN_HARDWARE_H_

int Serialport1_init();
void *serialport1_Send_monitorPthread(void *ptr);
void serialport1_Send_monitorPthread_create();
void *serialport1_Recv_monitorPthread(void *ptr);
void serialport1_Recv_monitorPthread_create();
void timer_counter(int nSignal);
void TimerInit();
void PortSend(char* send,int sizecnt);
void PrintTime(unsigned int time);
int ListenChanel();
void WaitDelay(unsigned int waittime);
unsigned int CountBackoffTime(int backofftime);


#endif /* UAN_HARDWARE_H_ */
