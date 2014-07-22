#ifndef _DATA_H_
#define _DATA_H_




#include <windows.h>



#include "fifo.h"
#include "list.h"
#include "protocol.h"
#include "user.h"


#define DEBUG_SIG 0
#define NIFFULL_OK 1
#define NIFFULL_NO 0
#define RECV_LENTH 65535

extern P_FIFO_T pstConnFIFO;
extern PLIST_HEADER_T pstRsLIST;
extern P_FIFO_T pstDuteFIFO;
extern PLIST_HEADER_T pstClientLIST;



extern CRITICAL_SECTION g_csClientLIST ;

extern CRITICAL_SECTION g_csConnFIFO ;

extern CRITICAL_SECTION g_csDuteFIFO ;  //4
extern CRITICAL_SECTION g_csRsLIST ;    //3



typedef struct tagDATA_T  DATA_T;
typedef struct tagDATA_T *P_DATA_T;


 struct tagDATA_T
{
	SOCKET unSocket;
	unsigned int unClientIPaddr;
	char acRecvBuff[RECV_LENTH];
	int nRecvBuffSize;
	int nIfFULL;
	char acSendBuff[1024];
	int nSendBuffSize;
	int nState;
	int nPageNo;
	int nPageAmount;
};


/* 服务器接收到 客户端对应的ip表*/


/*
//客户端连上来了以后，将会把系统开出的是socket和ip还有一个缓冲区。 
//当客户端再次连上来的时候，会查找链表，如果没有该节点，则添加新
//节点，否则改变对应ip的socket值，ip就成了唯一的标示码，客户端连
//入，会去寻找ip表查找缓冲区，是否有内容要回复给客户端。
*/
typedef struct tagClientAddr_T
{
	SOCKET unSocket;

	unsigned int unIpAddress;
	unsigned int nBufferLenth;
	char acSendBuffer[256];
}ClientAddr_T,*P_ClientAddr_T;




P_ClientAddr_T AddNewClientAddr(unsigned int unClientAddr);

P_DATA_T  AddNewRecvBuf(SOCKET unAcceptSocket, unsigned int unClientIPaddr); 

void putOutLowNum(unsigned int nNum);

char* DeCordStrFULL(char* acRecvBuff, int nRecvBuffSize);


int DeCordIntoFile(char* acRecvBuff);

int putOutLowNum2Str(unsigned int nNum, char *pcOrigStr);



#endif  //DATA
