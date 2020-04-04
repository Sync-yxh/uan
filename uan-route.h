#ifndef UAN_ROUTE_H_
#define UAN_ROUTE_H_

#include "uan-class.h"

int FindRouting(char* next);
bool Route_DataSend(RouteData data);
void* Route_SendControlThread(void* ptr);
void* Route_RecvControlThread(void* ptr);
void Route_SendControlThread_create();
void Route_RecvControlThread_create();

RouteData Route_Package(AppData ad);

#endif /* UAN_ROUTE_H_ */
