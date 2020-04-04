#ifndef UAN_HANDLE_H_
#define UAN_HANDLE_H_

void ToMAC(InitPkt init);
void ToMAC(StatePkt state);
void HandleRou(char[]);                        //路由处理函数
void InitSys();                              //初始化建立地址函数


#endif /* UAN_HANDLE_H_ */
