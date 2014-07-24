#include "stdafx.h"
#include "DATA.h"

#include "DecordStr.h"
#include "stdafx.h"

#include "BLL_Dute.h"
#include "BLL_Init.h"


P_DATA_T DLL_GetNode()
{
	P_DATA_T pstTmp = NULL;

	if(pstDuteFIFO == NULL)
	{
		DuteFIFO_Init();
		//break;
	}
	if(pstDuteFIFO != NULL)
	{
		EnterCriticalSection(&g_csDuteFIFO);		// 进入临界区	业务处理FIFO
		pstTmp = (P_DATA_T)FIFO_Pop(pstDuteFIFO);

		if ( pstTmp != NULL)
		{
			printf("-------------业务处理FIFO----------------\n");
			printf("接收到的长度为: pcTmp lenth = %d\n", pstTmp->nRecvBuffSize);
			printf("业务处理收到  = \n");
			for (int nIndex = 0; nIndex< pstTmp->nRecvBuffSize; nIndex++)
			{
				
				putOutLowNum(pstTmp->acRecvBuff[nIndex]);
				//putOutLowNum2Str(pstTmp->acRecvBuff[nIndex], pcOrigStr);
				if ((nIndex + 1) %7  == 0)
				{
					printf("\n");
				}
			}
			printf("\n");
		}
		
		LeaveCriticalSection(&g_csDuteFIFO);		// 离开临界区	业务处理FIFO
	}
	return pstTmp;
}










P_NET_HEAD BuildNetHead(uint16 PacketID, uint8 DirFlag, uint8 NetCmd, NET_ECHO_VALUE EchoValue, uint16 DataLen)
{
	P_NET_HEAD pstNetBag = (P_NET_HEAD)malloc(sizeof(NET_HEAD));
	pstNetBag->PacketID = PacketID;
	pstNetBag->DirFlag = DirFlag;
	pstNetBag->NetCmd = NetCmd;
	pstNetBag->EchoValue = EchoValue;
	pstNetBag->DataLen = DataLen;
	return pstNetBag;
}




int sendSuccessMsg(unsigned int unClientIPaddr , uint16 PacketID,  uint8 NetCmd)
{
	EnterCriticalSection(&g_csClientLIST);		// 进入临界区   客户端表
	PLIST_NODE_T pstTmpNode = LST_FindNode(pstClientLIST, &unClientIPaddr, 4 , FIND_IP );
	if (pstTmpNode == NULL)																			//无节点			
	{
		return 0;
	}
	else /* (找到了节点)*/
	{
		P_ClientAddr_T pstClientNode = (P_ClientAddr_T)pstTmpNode->pvData;        //pvData是起始地址，我这样做，只是可以给指定位置赋给他IP值或发送内容
		P_NET_HEAD pstNetBag = BuildNetHead(PacketID, 0x00, NetCmd, ECHO_OK, 0);
		memcpy(pstClientNode->acSendBuffer , pstNetBag ,sizeof(NET_HEAD));//这里可以写一个函数，返回值就是回复的信息（包头）。
		pstClientNode->nBufferLenth = sizeof(NET_HEAD);
		printf("业务处理环节：%d字节的【确认包】 写入发缓冲区\n",pstClientNode->nBufferLenth );
	}		
	LeaveCriticalSection(&g_csClientLIST);		// 离开临界区	客户端表
	return 0;
}


unsigned int __stdcall BLL_DuteFunc(void *)
{	
	char*  pcOrigStr = NULL;
	DecordStr mycord;
	while(1)
	{
		P_DATA_T pstTmp = DLL_GetNode();
		P_DATA_T pstInMFC = NULL;


		//把节点内容通过pstDuteFIFO_MFC 传送到外部的MFC中。
		EnterCriticalSection(&g_csDuteFIFO_MFC);		// 进入临界区	业务处理外部的MFC FIFO

		if(pstTmp != NULL)
		{
			pstInMFC = (P_DATA_T)malloc(sizeof(DATA_T));
			memset(pstInMFC, 0, sizeof(DATA_T));
			memcpy(pstInMFC, pstTmp, sizeof(DATA_T));
			if(pstDuteFIFO_MFC != NULL)
			{
				FIFO_Push(pstDuteFIFO_MFC, pstInMFC); 
				printf("  节点放到FIFO里 \n");
			}
		}


		
		LeaveCriticalSection(&g_csDuteFIFO_MFC);		// 离开临界区	业务处理外部的MFC FIFO

		if ( pstTmp != 0)
		{
			printf("从FIFO中获取的节点,该地址为：0x%u \n", pstTmp);
		}
		

		////业务处理。，以下需要实现另外一个函数。
		if(pstTmp != NULL)
		{
			P_NET_HEAD pstNetBag = (P_NET_HEAD)malloc(sizeof(NET_HEAD));
			memset(pstNetBag, 0x00, sizeof(NET_HEAD));
			memcpy(pstNetBag, pstTmp->acRecvBuff, sizeof(NET_HEAD));

			//这个取出来的不是头，而是整个节点。

			printf("pstNetBag->DirFlag = %d ", pstNetBag->DirFlag);
			if (pstNetBag->DirFlag == 0xaa)
			{
				switch (pstNetBag->NetCmd)
				{
				case 0x04: 
					sendSuccessMsg(pstTmp->unClientIPaddr ,pstNetBag->PacketID, pstNetBag->NetCmd);


					//putOutLowNum2Str(pstTmp->acRecvBuff[nIndex], pcOrigStr);


					printf("接收到的是【心跳包】\n");
					break;	
				case 0x01: 
					sendSuccessMsg(pstTmp->unClientIPaddr ,pstNetBag->PacketID, pstNetBag->NetCmd);
					printf("接收到的是【数据采集器包】\n");
					
					
					mycord.acnOutNo = pstTmp->acRecvBuff;
					mycord.nOutStrIndex = pstTmp->nRecvBuffSize;
					mycord.OutToFile();

					//mycord.Code2Str(pcOrigStr);
					//mycord.OutToFile();

					free(pcOrigStr);
					break;
				default:
					printf("接收到的是【未知包】\n");
					break;
				}
			}
			else if (pstNetBag->DirFlag == 0x00)
			{
				if (pstNetBag->PacketID == 0)		//如果没有包体，那将是空包，而不是应答包。
				{
					printf("接收到的是【连接包】\n");  //这段代码逻辑有问题，应该判断是不是空包，再判断其他的。
				}
				else									//因为其他都是建立在非空包的前提下的。					
				{
					printf("接收到的是【应答包】\n"); 
				}
				
			}
			closesocket(pstTmp->unSocket);
				printf("--断开连接\n------------------------------\n\n\n");
				
			free(pstNetBag);
			
		}
		free(pstTmp);
	}

	

	return 0;
}

