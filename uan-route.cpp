#include <iostream>
#include <fstream>
#include <list>
#include <string.h>

#include "uan-variable.h"
#include "uan-class.h"
#include "uan-mac.h"
#include "uan-hardware.h"

using namespace std;

State OldState;
char action = 255;

int FindRouting(char& next)
{
	int FindResult = 1;
	next = action;
	return FindResult;
}

double CalcMaxNextQ(State nodeState,vector<State> stateVec)
{
    double maxQ = INT_MIN;
    bool found = false;
    for(vector<State>::iterator iter = stateVec.begin(); iter != stateVec.end(); iter++)
    {
        State one = *iter;
        if(one.id == nodeState.id){
            maxQ = QTable[one];
            found = true;
        }
    }
    if(found == false || maxQ == INT_MIN){
        maxQ = 0;
    }

    return maxQ;
}

void UpdataQ()
{
	if(QLStage == 1)
	{
		OldState = QstateVector[0];
		action = OldState.id;
		QLStage = 2;
	}	
	else if(QLStage == 2)
	{
		Step data;
		data.state = OldState;
		data.reward = 1;
		data.stateNextVec = QstateVector;
		if(QTable.find(data.state) == QTable.end()){
    		QTable[data.state] = 0;
    	}
    	double oldQ = QTable[data.state];
    	double maxNextQ = CalcMaxNextQ(data.state,data.stateNextVec);

    	double newQ = oldQ + alpha*(data.reward + gamma*maxNextQ - oldQ);
    	QTable[data.state] = newQ;

		if(QstateVec.size()==1){
			action = QstateVec[0].id;
		}
		else if(QstateVec.size()==0){
			action = char(255);
		}
		else{
			double maxQ1 = INT_MIN;
			State maxS1;
			for(std::vector<State>::iterator iter = QstateVec.begin(); iter != QstateVec.end(); iter++)
			{
				State one = *iter;
				if(QTable.find(one) != QTable.end()){
					if(QTable[one] >= maxQ1){
						maxQ1 = QTable[one];
						maxS1 = one;
					}
				}
			}
			if(maxQ1 > INT_MIN){
				action = maxS1.id;
			}
			else{
				action = char(255);
			}
		}
	}
}

bool Route_DataSend(RouteData data)
{
	char nextID;
	if(!FindRouting(nextID)){
		pthread_mutex_lock(&mut);
		RouteSendBuff.pop_front();
		RouteWaitSend--;
		pthread_mutex_unlock(&mut);
		return false;
	}
	DataPkt pkt;
	pkt.seqNum = data.seqNum;
	pkt.srcID = data.srcID;
	pkt.relayID = m_ID;
	pkt.dstID = nextID;
	pkt.data = data.data;
	pthread_mutex_lock(&mut);
    MACSendBuff.push_back(pkt);
    RouteSendBuff.pop_front();
	MACWaitSend++;
	RouteWaitSend--;
	pthread_mutex_unlock(&mut);
	return true;
}

void* Route_SendControlThread(void* ptr)                       //路由层发送控制函数
{
	fstream f_routeresult;
	while(1)
	{
		if(RouteSendBuff.empty()!=true && RouteWaitSend>0)  //判断缓存是否为空及发送标志位
		{
			auto routedata=RouteSendBuff.begin();           //获取首位
			Route_DataSend(*routedata);
		}
    }
	return 0;
}

void* Route_RecvControlThread(void* ptr)                      //路由层接收控制函数
{
    while(1)
	{
	    if(RouteRecvBuff.empty()!=true && RouteWaitRecv>0)
		{
	  	    auto routerecviter =RouteRecvBuff.begin(); //取其中第1个元素
			pthread_mutex_lock(&mut);
			RouteWaitRecv--;
			RouteWaitSend++;
			RouteSendBuff.push_back( *routerecviter );
			RouteRecvBuff.pop_front();
			pthread_mutex_unlock(&mut);
		}
	}
    return 0;
}

RouteData Route_Package(AppData ad)
{
	RouteData rd;

	rd.flag = 3;
	rd.seqNum = dataSeqNum;
	rd.srcID = m_ID;
	rd.data = ad;

	return rd;
}

void Route_SendControlThread_create()
{
	printf("-----Route_SendControlThread_create!----\n");
	pthread_t Pthread_routes;
	pthread_create(&Pthread_routes, NULL, Route_SendControlThread, NULL);
}
void Route_RecvControlThread_create()
{
	printf("-----Route_RecvControlThread_create!----\n");
	pthread_t Pthread_router;
	pthread_create(&Pthread_router, NULL, Route_RecvControlThread, NULL);
}

