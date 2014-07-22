/******************************************************************************
* 版权所有 (C)2014, 朱代旺 
* 文件名称：
*    BLL_DataSR.c
* 当前版本：
*     1.0
* 内容摘要：
*     业务层，数据发收模块。 这个文件主要实现添加链接，发收数据，和转移节点。
* 历史纪录：
*     修改人		日期		版本号		        描述
*     朱代旺      2014.4.1       1.0
*******************************************************************************/

#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <string.h>

#include "DATA.h"
#include "stdafx.h"


#include "BLL_DataSR.h"
#include "BLL_Dute.h"
#include "BLL_Init.h"







int ADD_Client()
{
	char *pcTmp = NULL;
	P_DATA_T pstRecvBuf = NULL;
	do
	{
		EnterCriticalSection(&g_csConnFIFO);		// 进入临界区	等待链接队列 接收表
		if (pstConnFIFO->nNodeCount>0)
		{
			pcTmp = FIFO_Pop(pstConnFIFO);
			printf("-------------数据收发FIFO----------------\n");
			printf("有客户端连入，取出了一个“客户端接收节点”\n");
		}
		LeaveCriticalSection(&g_csConnFIFO);		// 离开临界区	等待链接队列 接收表
		Sleep(500);
	} while (pcTmp == NULL);

		if (pcTmp != NULL)
		{
			EnterCriticalSection(&g_csRsLIST);		// 进入临界区	发收表
			pstRecvBuf = (P_DATA_T)pcTmp;
			LST_Insert(pstRsLIST, 10, pstRecvBuf, sizeof(DATA_T));
			printf(" 将“客户端接收节点”，放入 发收表\n");
			LeaveCriticalSection(&g_csRsLIST);		// 离开临界区	发收表			
		}
	
	return 0;
	
}



int SendRecvDATA()
{/*提取socket里的地址，不对，socket里没有地址。 那么节点就要添加一个元素，地址的元素，然后提取ip。*/
	/*查找pstClientLIST，是否有该节点，有节点，int数据长度是否不为零，那么数据拷贝出来，发送。*/ /*int 数据长度已经添加，在data.h里*/
					/*后期再考虑多个信息发送，这次仅考虑单个发送*/

	/*接收信息，超时20微秒，跳过本节点。循环遍历本链表。*/
	int nRecvLenth = 0;
	int nRSlistLenth = 0;
	int nIndex = 0;
	fd_set fRecv;
	timeval tv;

	EnterCriticalSection(&g_csRsLIST);		// 进入临界区	发收表
	
	for (nIndex = 0; nIndex < pstRsLIST->nNodeCount; nIndex++)
	{
		/////////////////////////////////////////////
		// //获取客户端地址
		//////////////////////////////////////////////
		PLIST_NODE_T pstClientNodeTmp = LST_GetAt(pstRsLIST,nIndex);  

		if (pstClientNodeTmp == NULL)
		{
			printf(" 数据收发列表为空，没有客户连接\n");
			break;
		}
		P_DATA_T pstFindOutNode = (P_DATA_T)pstClientNodeTmp->pvData;
		unsigned int unClientIPaddr =  pstFindOutNode->unClientIPaddr;

		printf("--发送信息--\n");
		printf("开始寻找目标客户端地址0x%x，查看是否有数据需要发送\n",  unClientIPaddr);
		/////////////////////////////////////////////
		// 发缓冲区的内容拷贝出来
		//////////////////////////////////////////////
		EnterCriticalSection(&g_csClientLIST);		// 进入临界区   客户端表
		PLIST_NODE_T pstTmpNode = LST_FindNode(pstClientLIST, &unClientIPaddr, 4 , FIND_IP );  //还有判断是否有内容。
		if (pstTmpNode == NULL)
		{
			printf("没有找到客户端地址0x%x\n",  unClientIPaddr);
		}
			

		P_ClientAddr_T pstClientNode = NULL;
		if (pstTmpNode != NULL)
		{
			printf(" 找到了目标客户端地址0x%x, 他的指针为：%u\n",  unClientIPaddr, pstTmpNode);
			pstClientNode =(P_ClientAddr_T)pstTmpNode->pvData;
			pstFindOutNode->nSendBuffSize = pstClientNode->nBufferLenth;
			if(pstClientNode->nBufferLenth == 0)
			{
				printf("没有数据需要发送\n");
			}
			else 
			{	
				printf("%d字节数据成功拷贝到->发送缓冲区\n", pstClientNode->nBufferLenth);
				memcpy(pstFindOutNode->acSendBuff , pstClientNode->acSendBuffer,pstClientNode->nBufferLenth); ////20140409午、、有问题
				
			/////////////////////////////////////////////
			// 发送信息，
			//////////////////////////////////////////////
				int nSendBufSize = send(pstFindOutNode->unSocket, pstFindOutNode->acSendBuff, pstFindOutNode->nSendBuffSize, 0);	
				printf("%d字节数据已经写入内核准备发送\n" ,nSendBufSize);
				printf("%d字节数据为：",nSendBufSize);
				for (int nIndex = 0; nIndex< nSendBufSize; nIndex++)
				{

					putOutLowNum(pstFindOutNode->acSendBuff[nIndex]);
					if ((nIndex + 1) %7  == 0)
					{
						printf("\n");
					}
				}
				memset(pstClientNode->acSendBuffer, 0x00, pstClientNode->nBufferLenth);
				 pstClientNode->nBufferLenth = 0;
			}
		
		}
		LeaveCriticalSection(&g_csClientLIST);		// 离开临界区	客户端表
		/////////////////////////////////////////////
		// 接收信息，20微秒
		//////////////////////////////////////////////
		FD_ZERO(&fRecv);//初始化fd_set
		FD_SET(pstFindOutNode->unSocket, &fRecv);//分配套接字句柄到相应的fd_set


		tv.tv_sec = 3;//这里我们打算让select等待两秒后返回，避免被锁死，也避免马上返回
		tv.tv_usec = 20000;  //20,000 微秒，就是20毫秒

		select(0, &fRecv, NULL, NULL, &tv);
		
		printf("--接收信息--\n");
		if (FD_ISSET(pstFindOutNode->unSocket, &fRecv))
		{
			char acTmpBuf[RECV_LENTH] = {0};
			nRecvLenth = recv(pstFindOutNode->unSocket,acTmpBuf,RECV_LENTH,0);

			
			printf("接收到%d字节数据， \n",nRecvLenth);
			//printf("接收到的数据tmp字符串为acTmpBuf = %s\n ",acTmpBuf);

			if (nRecvLenth > 0)						//判断 如果数据 小于零呢，明明是有数据接收才进来的，为什么没有数据收到？
			{
				memcpy(pstFindOutNode->acRecvBuff, acTmpBuf, nRecvLenth);       //拼接到原始的数据上。 //20140416 这里要改，改为拼接的。 设置一个标记位。如果满，就改标记位置。 后面如果标记为满，就移动。
				pstFindOutNode->nRecvBuffSize = nRecvLenth;                           //注意这里，这里
				//MessageBox(NULL, buffer, "recv", MB_ICONINFORMATION);
				//printf("收到字符串 %s \n ", pstFindOutNode->acRecvBuff);
// 				printf("十六进制表示为:\n ");
// 				for (int nRecvIndex = 0; nRecvIndex < nRecvLenth; nRecvIndex++)
// 				{
// 					putOutLowNum(pstFindOutNode->acRecvBuff[nRecvIndex]);
// 					if ((nRecvIndex + 1) %7  == 0)
// 					{
// 						printf("\n ");
// 					}
// 				}
// 				printf("\n");
			}
			
		}
		else
		{
			printf("在0.5秒内未接收到数据\n");
		}
	}
	LeaveCriticalSection(&g_csRsLIST);		// 离开临界区	发收表


	////////////////////////////////////////////////////
	// 转移节点，这里就需要配合协议。
	//////////////////////////////////////////////////////
	printf("--转移节点--\n");
	printf("收发信息链表的节点个数为 %d 个\n", pstRsLIST->nNodeCount);
	EnterCriticalSection(&g_csRsLIST);		// 进入临界区	发收表
	for (nIndex = 0; nIndex < pstRsLIST->nNodeCount; nIndex++)
	{
		/////////////////////////////////////////////
		// //获取客户端节点地址
		//////////////////////////////////////////////
		
		PLIST_NODE_T pstTmp = LST_GetAt(pstRsLIST,nIndex);  
		
		P_NET_HEAD pstHeadMsgTmp = NULL;
		
		if (pstTmp == NULL)
		{
			printf("if (pstTmp == NULL)\n");
			break;
		}
		
		P_DATA_T pstFindOutNode = (P_DATA_T)pstTmp->pvData;
		
		
		unsigned short unRecvLenth =  nRecvLenth;
		unsigned short unBodyLenth = 0;
		//配合协议
		pstHeadMsgTmp = (P_NET_HEAD)pstFindOutNode->acRecvBuff;       //心跳包	

		
		if (pstHeadMsgTmp->DataLen == 0 )
		{
			
			if (pstHeadMsgTmp != NULL)
			{
				printf("获取到节点，缓冲区中，包体数据为空 -->  ", nRecvLenth);     
				MoveClient(nIndex);		
			}
		}
		else 
		{		
			//unBodyLenth = unRecvLenth-7;										//数据是否满，是否要继续接收，这里没有关注。
			//if (unBodyLenth == pstHeadMsgTmp->DataLen)
			//{
			//}
			if (pstHeadMsgTmp != NULL)
			{
				printf("获取到节点，缓冲区包体数据有%d字节 “有待判断完整性”-->  ", nRecvLenth);      //有待改善，20140415晚 //20140417未完善。
				MoveClient(nIndex);		
			}
		}
		

	}



	LeaveCriticalSection(&g_csRsLIST);		// 离开临界区	发收表
	
	return 0;
}



int MoveClient(int nIndex)
{

		/////////////////////////////////////////////
		// //移出节点
		//////////////////////////////////////////////
		PLIST_NODE_T pstOutNode = LST_OutNodeByPos(pstRsLIST, nIndex);     //移出节点    //20140410 午
		P_DATA_T pstDataNode =(P_DATA_T)pstOutNode->pvData;
		/*进入临界区*/	
		EnterCriticalSection(&g_csDuteFIFO);		// 进入临界区	业务处理FIFO


		if(pstDuteFIFO == NULL)
		{
			DuteFIFO_Init();
			printf("FIFO内没有节点，移动失败\n");
			//break;
		}
		if(pstDuteFIFO != NULL)
		{
			FIFO_Push(pstDuteFIFO, pstDataNode); 
			printf("  节点移出成功 \n");
		}

		LeaveCriticalSection(&g_csDuteFIFO);		// 离开临界区	业务处理FIFO


	return 0;
}




unsigned int __stdcall BLL_DataSRFunc(void *)
{
#if DEBUG_SIG
	printf("BLL_DataSRFunc 200\n");
#endif
	while (1)
	{
		ADD_Client();
		
		SendRecvDATA();
	}
}
