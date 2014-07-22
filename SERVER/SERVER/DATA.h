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


/* ���������յ� �ͻ��˶�Ӧ��ip��*/


/*
//�ͻ������������Ժ󣬽����ϵͳ��������socket��ip����һ���������� 
//���ͻ����ٴ���������ʱ�򣬻�����������û�иýڵ㣬�������
//�ڵ㣬����ı��Ӧip��socketֵ��ip�ͳ���Ψһ�ı�ʾ�룬�ͻ�����
//�룬��ȥѰ��ip����һ��������Ƿ�������Ҫ�ظ����ͻ��ˡ�
*/
typedef struct tagClientAddr_T
{
	SOCKET unSocket;
	char acIPstr[32];
	unsigned int unIpAddress;
	unsigned int nBufferLenth;
	char acSendBuffer[256];
}ClientAddr_T,*P_ClientAddr_T;




P_ClientAddr_T AddNewClientAddr(unsigned int unClientAddr , char* pcIPstr);

P_DATA_T  AddNewRecvBuf(SOCKET unAcceptSocket, unsigned int unClientIPaddr); 

void putOutLowNum(unsigned int nNum);

char* DeCordStrFULL(char* acRecvBuff, int nRecvBuffSize);


int DeCordIntoFile(char* acRecvBuff);

int putOutLowNum2Str(unsigned int nNum, char *pcOrigStr);



#endif  //DATA
