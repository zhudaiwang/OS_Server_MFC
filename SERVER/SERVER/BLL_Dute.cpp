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
		EnterCriticalSection(&g_csDuteFIFO);		// �����ٽ���	ҵ����FIFO
		pstTmp = (P_DATA_T)FIFO_Pop(pstDuteFIFO);

		if ( pstTmp != NULL)
		{
			printf("-------------ҵ����FIFO----------------\n");
			printf("���յ��ĳ���Ϊ: pcTmp lenth = %d\n", pstTmp->nRecvBuffSize);
			printf("ҵ�����յ�  = \n");
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
		
		LeaveCriticalSection(&g_csDuteFIFO);		// �뿪�ٽ���	ҵ����FIFO
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
	EnterCriticalSection(&g_csClientLIST);		// �����ٽ���   �ͻ��˱�
	PLIST_NODE_T pstTmpNode = LST_FindNode(pstClientLIST, &unClientIPaddr, 4 , FIND_IP );
	if (pstTmpNode == NULL)																			//�޽ڵ�			
	{
		return 0;
	}
	else /* (�ҵ��˽ڵ�)*/
	{
		P_ClientAddr_T pstClientNode = (P_ClientAddr_T)pstTmpNode->pvData;        //pvData����ʼ��ַ������������ֻ�ǿ��Ը�ָ��λ�ø�����IPֵ��������
		P_NET_HEAD pstNetBag = BuildNetHead(PacketID, 0x00, NetCmd, ECHO_OK, 0);
		memcpy(pstClientNode->acSendBuffer , pstNetBag ,sizeof(NET_HEAD));//�������дһ������������ֵ���ǻظ�����Ϣ����ͷ����
		pstClientNode->nBufferLenth = sizeof(NET_HEAD);
		printf("ҵ�����ڣ�%d�ֽڵġ�ȷ�ϰ��� д�뷢������\n",pstClientNode->nBufferLenth );
	}		
	LeaveCriticalSection(&g_csClientLIST);		// �뿪�ٽ���	�ͻ��˱�
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


		//�ѽڵ�����ͨ��pstDuteFIFO_MFC ���͵��ⲿ��MFC�С�
		EnterCriticalSection(&g_csDuteFIFO_MFC);		// �����ٽ���	ҵ�����ⲿ��MFC FIFO

		if(pstTmp != NULL)
		{
			pstInMFC = (P_DATA_T)malloc(sizeof(DATA_T));
			memset(pstInMFC, 0, sizeof(DATA_T));
			memcpy(pstInMFC, pstTmp, sizeof(DATA_T));
			if(pstDuteFIFO_MFC != NULL)
			{
				FIFO_Push(pstDuteFIFO_MFC, pstInMFC); 
				printf("  �ڵ�ŵ�FIFO�� \n");
			}
		}


		
		LeaveCriticalSection(&g_csDuteFIFO_MFC);		// �뿪�ٽ���	ҵ�����ⲿ��MFC FIFO

		if ( pstTmp != 0)
		{
			printf("��FIFO�л�ȡ�Ľڵ�,�õ�ַΪ��0x%u \n", pstTmp);
		}
		

		////ҵ������������Ҫʵ������һ��������
		if(pstTmp != NULL)
		{
			P_NET_HEAD pstNetBag = (P_NET_HEAD)malloc(sizeof(NET_HEAD));
			memset(pstNetBag, 0x00, sizeof(NET_HEAD));
			memcpy(pstNetBag, pstTmp->acRecvBuff, sizeof(NET_HEAD));

			//���ȡ�����Ĳ���ͷ�����������ڵ㡣

			printf("pstNetBag->DirFlag = %d ", pstNetBag->DirFlag);
			if (pstNetBag->DirFlag == 0xaa)
			{
				switch (pstNetBag->NetCmd)
				{
				case 0x04: 
					sendSuccessMsg(pstTmp->unClientIPaddr ,pstNetBag->PacketID, pstNetBag->NetCmd);


					//putOutLowNum2Str(pstTmp->acRecvBuff[nIndex], pcOrigStr);


					printf("���յ����ǡ���������\n");
					break;	
				case 0x01: 
					sendSuccessMsg(pstTmp->unClientIPaddr ,pstNetBag->PacketID, pstNetBag->NetCmd);
					printf("���յ����ǡ����ݲɼ�������\n");
					
					
					mycord.acnOutNo = pstTmp->acRecvBuff;
					mycord.nOutStrIndex = pstTmp->nRecvBuffSize;
					mycord.OutToFile();

					//mycord.Code2Str(pcOrigStr);
					//mycord.OutToFile();

					free(pcOrigStr);
					break;
				default:
					printf("���յ����ǡ�δ֪����\n");
					break;
				}
			}
			else if (pstNetBag->DirFlag == 0x00)
			{
				if (pstNetBag->PacketID == 0)		//���û�а��壬�ǽ��ǿհ���������Ӧ�����
				{
					printf("���յ����ǡ����Ӱ���\n");  //��δ����߼������⣬Ӧ���ж��ǲ��ǿհ������ж������ġ�
				}
				else									//��Ϊ�������ǽ����ڷǿհ���ǰ���µġ�					
				{
					printf("���յ����ǡ�Ӧ�����\n"); 
				}
				
			}
			closesocket(pstTmp->unSocket);
				printf("--�Ͽ�����\n------------------------------\n\n\n");
				
			free(pstNetBag);
			
		}
		free(pstTmp);
	}

	

	return 0;
}

