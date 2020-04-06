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
