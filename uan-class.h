#ifndef UAN_CLASS_H_
#define UAN_CLASS_H_


//应用数据类型
class AppData
{
public:
	char Datalenth;
	char Data[100];
};

//路由数据类型
class RouteData
{
public:
	char flag;
	char seqNum;
	char srcID;
	AppData data;
};

//MAC发送结果
class MACResult
{
public:
	char flag;
	unsigned int TimeStamp;
	char seqNum;
	bool result;
};

//初始化报文
class InitPkt
{
public:
	char flag = 1;
	char seqNum;
	char srcID = m_ID;
	unsigned short Depth;

};

//状态报文
class StatePkt
{
public:
	char flag = 2;
	char seqNum;
	char srcID = m_ID;
	char bandwidth;
	char connect;
	unsigned short Depth;
};

class DataPkt
{
public:
	char flag = 3;
	char seqNum;
	char srcID;
	char relayID;
	char dstID;
	AppData data;
};

class State
{
public:
	int id;
    int connect;
    int bandwidth;

	bool operator < ( const State &s ) const; 
}

class Step
{
public:
    State state;
    bool action;
    std::vector<State> stateNextVec;
    double reward;
};

#endif /* UAN_CLASS_H_ */
