#include <iostream>
#include <fstream>
#include <list>
#include <string.h>

#include "uan-variable.h"
#include "uan-class.h"
#include "uan-hardware.h"
#include "uan-route.h"

void AppSend(char Des_ID,AppData inputAD)
{
	BundleData tempBD;
	tempBD.Bundleflag = 1;
	tempBD.Des_ID = Des_ID;
	tempBD.Sou_ID = m_ID;
	tempBD.bAD = inputAD;
	BundleSendBuff.push_back(tempBD);
	BundleWaitSend++;
}

bool AppRecv(AppData* outputAD)
{
	if(DataBuff.empty())
		return false;
	else
	{
		auto iter = DataBuff.begin();
		DataBuff.pop_front();
		*outputAD = *iter;
	}
	return true;
}
