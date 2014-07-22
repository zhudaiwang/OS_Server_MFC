#include "stdafx.h"

#include "DATA.h"






P_FIFO_T pstConnFIFO = NULL;
PLIST_HEADER_T pstRsLIST  = NULL;

P_FIFO_T pstDuteFIFO = NULL;

PLIST_HEADER_T pstClientLIST = NULL;





// 定义临界区对象
CRITICAL_SECTION g_csClientLIST; //2
CRITICAL_SECTION g_csConnFIFO ;  //1
CRITICAL_SECTION g_csDuteFIFO ;  //4
CRITICAL_SECTION g_csRsLIST ;    //3


#if 0

// 初始化临界区对象
InitializeCriticalSection(&cs); // 一般在构造函数中进行初始化

// 进入临界区
EnterCriticalSection(&cs);

// 离开临界区
LeaveCriticalSection(&cs);

// 临界区不用的时候，进行销毁释放占用资源
DeleteCriticalSection(&cs);


#endif





P_ClientAddr_T AddNewClientAddr(unsigned int unClientAddr,char* pcIPstr)
{
	P_ClientAddr_T pstClientNode = NULL;
	pstClientNode = (P_ClientAddr_T)malloc(sizeof(ClientAddr_T));
	memset(pstClientNode, 0x00, sizeof(ClientAddr_T));
	memcpy(pstClientNode->acIPstr, pcIPstr,15);
	pstClientNode->unIpAddress = unClientAddr;
	return pstClientNode;
}


P_DATA_T  AddNewRecvBuf(SOCKET unClientSocket, unsigned int unClientIPaddr)
{
	P_DATA_T pstNewBuf = (P_DATA_T)malloc(sizeof(DATA_T));
	memset(pstNewBuf, 0x00, sizeof(DATA_T)); 
	pstNewBuf->unSocket = unClientSocket;
	pstNewBuf->unClientIPaddr = unClientIPaddr;

	return pstNewBuf;
}



void putOutLowNum(unsigned int nNum)
{
	printf("%x", (nNum&0xF0)>>4);
	printf("%x ", nNum&0xF);
}




int putOutLowNum2Str(unsigned int nNum, char *pcOrigStr)
{
	sprintf(pcOrigStr, "%1x", (nNum&0xF0)>>4);
	sprintf(pcOrigStr, "%1x ", nNum&0xF);
	return strlen(pcOrigStr);
}

char* DeCordStrFULL(char* acRecvBuff, int nRecvBuffSize)
{
	int nIndex = 0;
	char *pcOrigStr = (char* )malloc(nRecvBuffSize *3);

	memset(pcOrigStr, 0x00, nRecvBuffSize *3);

	for (nIndex = 0; nIndex < nRecvBuffSize; nIndex++)
	{
		putOutLowNum2Str(acRecvBuff[nIndex], pcOrigStr);
	}

	return pcOrigStr;
}


int DeCordIntoFile(char *pcOrigStr)
{
	//1.解码
	//char * acnOutNo = Code2Str(pcOrigStr);
	//2.取出订单号
	//char * pcThisDealNo = getThisDealNo(acnOutNo);         //小票单号
	//3.写入文件
	//OutToFile(acnOutNo, pcThisDealNo); 
	return 0;
}