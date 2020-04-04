#include "uan-variable.h"

pthread_mutex_t mut;  // 互斥锁
char m_ID = 7;       //自身硬件地址
uint16 m_Depth = 1000;

unsigned int SysTimeCnt=0;       //系统时间计时器

char stateSeqNum = 0;                //状态报文序列号
char dataSeqNum = 0;             //信息报文序列号

int MACWaitRecv = 0;       //MAC等待接收标志
int MACWaitSend = 0;       //MAC等待发送标志
int RouteWaitSend = 0;    //路由等待发送标志
int RouteWaitRecv = 0;     //路由等待接收标志

vector<MACResult> MACResultArray;         //MAC发送结果记录数组

list< list<char> > MACRecvBuff;               //MAC接收缓存
list<DataPkt> MACSendBuff;                  //MAC发送缓存
list<RouteData> RouteRecvBuff;              //路由接收缓存
list<RouteData> RouteSendBuff;              //路由发送缓存
list<AppData> DataBuff;


vector<char> ParentIDBuff;
vector<char> ChildIDBuff;

int SysPortState = RecvState;     //  系统状态
char readbuff[MAXLength];
char sendbuff[MAXLength];
int serial_fd1;
int readlen = 0;
int sendlen = 0;

