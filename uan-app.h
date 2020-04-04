#ifndef UAN_APP_H_
#define UAN_APP_H_

#include "uan-class.h"

void ScheduleData();
void ScheduleState();

void SendAppState(int nSignal);
void SendAppData(int nSignal);

#endif /* UAN_APP_H_ */
