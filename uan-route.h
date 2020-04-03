#ifndef UAN_ROUTE_H_
#define UAN_ROUTE_H_

#include "uan-class.h"

int FindRouting(char netaddr,char* next,bool way=true);
bool Route_DataSend(RouteData R1,char nextID);
void* Route_SendControlThread(void* ptr);
void* Route_RecvControlThread(void* ptr);
void Route_SendControlThread_create();
void Route_RecvControlThread_create();

RouteData Route_Package(char Rouflag,char sou_ID,char des_ID, AppData ad);



#endif /* UAN_ROUTE_H_ */
