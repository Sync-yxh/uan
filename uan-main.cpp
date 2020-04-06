#include <iostream>
#include "uan-variable.h"
#include "uan-class.h"
#include "uan-mac.h"
#include "uan-route.h"
#include "uan-hardware.h"
#include "uan-handle.h"

using namespace std;

int main() {
	// serial_fd1 = Serialport1_init();
	State x;
	x.id = 4;
	x.connect = 2;
	x.bandwidth = 0;
	QTable[x] = 0.5;
	x.id = 4;
	x.connect = 1;
	x.bandwidth = 0;
	QTable[x] = 1.0;
	x.id = 5;
	x.connect = 2;
	x.bandwidth = 0;
	QTable[x] = 0.8;
	QLStage = 3;

	serialport1_Recv_monitorPthread_create();
	serialport1_Send_monitorPthread_create();
	MAC_SendControlThread_create();
	MAC_RecvControlThread_create();
	Route_SendControlThread_create();
	Route_RecvControlThread_create();


	TimerInit();
	InitSys();

	while(true)
	{
	}
	return 0;
}
