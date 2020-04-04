#include <iostream>
#include <fstream>
#include <list>
#include <string.h>
#include "uan-variable.h"
#include "uan-class.h"
#include "uan-app.h"

using namespace std;


void ToMAC(InitPkt init)
{
	MAC_DataSend(init);
}
void ToMAC(StatePkt state)
{
	MAC_DataSend(state);
}

void HandleRou(char input[])
{
	switch (input[0])
	{
	case 1:             //初始化报文
	{	
		InitPkt initpkt;                    // 为了代码可读性，先复原报文，再建立路由
		initpkt.seqNum =  input[1];
		initpkt.srcID = input[2];
		initpkt.Depth = uint16(input[3] << 8 & input[4]);

		if(m_Depth < initpkt.Depth){
			ChildIDBuff.push_back(initpkt.srcID)
		}
		else{
			ParentIDBuff.push_back(initpkt.srcID)
		}
	}break;
	case 2:                //状态报文
	{
		StatePkt statePkt;
		statePkt.seqNum = input[1];
		statePkt.srcID = input[2];
		statePkt.bandwidth = input[3];
		statePkt.connect = input[4];
		statePkt.Depth = uint16(input[5] << 8 & input[6]);
		if(m_Depth < initpkt.Depth){
			ChildIDBuff.push_back(initpkt.srcID)
		}
		else{
			ParentIDBuff.push_back(initpkt.srcID)
		}
	}break;
	default:
		break;
	}
}

void SetFlie()
{
	fstream outfile;
	outfile.open("MACResult.txt", ios::in | ios::out | ios::app);
	outfile.width(6);
	outfile.flags(ios::right);
	outfile << "flag";
	outfile.width(6);
	outfile.flags(ios::right);
	outfile << "Time";
	outfile.width(6);
	outfile.flags(ios::right);
	outfile<< "Seq ";
	outfile.width(6);
	outfile.flags(ios::right);
	outfile<< "Result" << endl;
	outfile.close();

	outfile.open("RouteResult.txt", ios::in | ios::out | ios::app);
	outfile.width(6);
	outfile.flags(ios::right);
	outfile << "flag";
	outfile.width(6);
	outfile.flags(ios::right);
	outfile << "Time";
	outfile.width(6);
	outfile.flags(ios::right);
	outfile<< "Seq ";
	outfile.width(6);
	outfile.flags(ios::right);
	outfile<< "Result" << endl;
	outfile.close();

	outfile.open("BundleResult.txt", ios::in | ios::out | ios::app);
	outfile.width(6);
	outfile.flags(ios::right);
	outfile << "Time";
	outfile.width(6);
	outfile.flags(ios::right);
	outfile<< "Seq ";
	outfile.width(6);
	outfile.flags(ios::right);
	outfile<< "Result" << endl;
	outfile.close();
}

void InitSys()
{
	SetFlie();
	InitPkt pkt;
	pkt.seqNum = stateSeqNum;
	pkt.srcID = m_ID;
	pkt.Depth = m_Depth;

	ToMAC(tempIB);
	stateSeqNum ++;

	ScheduleState();
	ScheduleData();
}
