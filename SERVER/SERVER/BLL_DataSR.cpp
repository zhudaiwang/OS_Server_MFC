/******************************************************************************
* ��Ȩ���� (C)2014, ����� 
* �ļ����ƣ�
*    BLL_DataSR.c
* ��ǰ�汾��
*     1.0
* ����ժҪ��
*     ҵ��㣬���ݷ���ģ�顣 ����ļ���Ҫʵ��������ӣ��������ݣ���ת�ƽڵ㡣
* ��ʷ��¼��
*     �޸���		����		�汾��		        ����
*     �����      2014.4.1       1.0
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
		EnterCriticalSection(&g_csConnFIFO);		// �����ٽ���	�ȴ����Ӷ��� ���ձ�
		if (pstConnFIFO->nNodeCount>0)
		{
			pcTmp = FIFO_Pop(pstConnFIFO);
			printf("-------------�����շ�FIFO----------------\n");
			printf("�пͻ������룬ȡ����һ�����ͻ��˽��սڵ㡱\n");
		}
		LeaveCriticalSection(&g_csConnFIFO);		// �뿪�ٽ���	�ȴ����Ӷ��� ���ձ�
		Sleep(500);
	} while (pcTmp == NULL);

		if (pcTmp != NULL)
		{
			EnterCriticalSection(&g_csRsLIST);		// �����ٽ���	���ձ�
			pstRecvBuf = (P_DATA_T)pcTmp;
			LST_Insert(pstRsLIST, 10, pstRecvBuf, sizeof(DATA_T));
			printf(" �����ͻ��˽��սڵ㡱������ ���ձ�\n");
			LeaveCriticalSection(&g_csRsLIST);		// �뿪�ٽ���	���ձ�			
		}
	
	return 0;
	
}



int SendRecvDATA()
{/*��ȡsocket��ĵ�ַ�����ԣ�socket��û�е�ַ�� ��ô�ڵ��Ҫ���һ��Ԫ�أ���ַ��Ԫ�أ�Ȼ����ȡip��*/
	/*����pstClientLIST���Ƿ��иýڵ㣬�нڵ㣬int���ݳ����Ƿ�Ϊ�㣬��ô���ݿ������������͡�*/ /*int ���ݳ����Ѿ���ӣ���data.h��*/
					/*�����ٿ��Ƕ����Ϣ���ͣ���ν����ǵ�������*/

	/*������Ϣ����ʱ20΢�룬�������ڵ㡣ѭ������������*/
	int nRecvLenth = 0;
	int nRSlistLenth = 0;
	int nIndex = 0;
	fd_set fRecv;
	timeval tv;

	EnterCriticalSection(&g_csRsLIST);		// �����ٽ���	���ձ�
	
	for (nIndex = 0; nIndex < pstRsLIST->nNodeCount; nIndex++)
	{
		/////////////////////////////////////////////
		// //��ȡ�ͻ��˵�ַ
		//////////////////////////////////////////////
		PLIST_NODE_T pstClientNodeTmp = LST_GetAt(pstRsLIST,nIndex);  

		if (pstClientNodeTmp == NULL)
		{
			printf(" �����շ��б�Ϊ�գ�û�пͻ�����\n");
			break;
		}
		P_DATA_T pstFindOutNode = (P_DATA_T)pstClientNodeTmp->pvData;
		unsigned int unClientIPaddr =  pstFindOutNode->unClientIPaddr;

		printf("--������Ϣ--\n");
		printf("��ʼѰ��Ŀ��ͻ��˵�ַ0x%x���鿴�Ƿ���������Ҫ����\n",  unClientIPaddr);
		/////////////////////////////////////////////
		// �������������ݿ�������
		//////////////////////////////////////////////
		EnterCriticalSection(&g_csClientLIST);		// �����ٽ���   �ͻ��˱�
		PLIST_NODE_T pstTmpNode = LST_FindNode(pstClientLIST, &unClientIPaddr, 4 , FIND_IP );  //�����ж��Ƿ������ݡ�
		if (pstTmpNode == NULL)
		{
			printf("û���ҵ��ͻ��˵�ַ0x%x\n",  unClientIPaddr);
		}
			

		P_ClientAddr_T pstClientNode = NULL;
		if (pstTmpNode != NULL)
		{
			printf(" �ҵ���Ŀ��ͻ��˵�ַ0x%x, ����ָ��Ϊ��%u\n",  unClientIPaddr, pstTmpNode);
			pstClientNode =(P_ClientAddr_T)pstTmpNode->pvData;
			pstFindOutNode->nSendBuffSize = pstClientNode->nBufferLenth;
			if(pstClientNode->nBufferLenth == 0)
			{
				printf("û��������Ҫ����\n");
			}
			else 
			{	
				printf("%d�ֽ����ݳɹ�������->���ͻ�����\n", pstClientNode->nBufferLenth);
				memcpy(pstFindOutNode->acSendBuff , pstClientNode->acSendBuffer,pstClientNode->nBufferLenth); ////20140409�硢��������
				
			/////////////////////////////////////////////
			// ������Ϣ��
			//////////////////////////////////////////////
				int nSendBufSize = send(pstFindOutNode->unSocket, pstFindOutNode->acSendBuff, pstFindOutNode->nSendBuffSize, 0);	
				printf("%d�ֽ������Ѿ�д���ں�׼������\n" ,nSendBufSize);
				printf("%d�ֽ�����Ϊ��",nSendBufSize);
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
		LeaveCriticalSection(&g_csClientLIST);		// �뿪�ٽ���	�ͻ��˱�
		/////////////////////////////////////////////
		// ������Ϣ��20΢��
		//////////////////////////////////////////////
		FD_ZERO(&fRecv);//��ʼ��fd_set
		FD_SET(pstFindOutNode->unSocket, &fRecv);//�����׽��־������Ӧ��fd_set


		tv.tv_sec = 3;//�������Ǵ�����select�ȴ�����󷵻أ����ⱻ������Ҳ�������Ϸ���
		tv.tv_usec = 20000;  //20,000 ΢�룬����20����

		select(0, &fRecv, NULL, NULL, &tv);
		
		printf("--������Ϣ--\n");
		if (FD_ISSET(pstFindOutNode->unSocket, &fRecv))
		{
			char acTmpBuf[RECV_LENTH] = {0};
			nRecvLenth = recv(pstFindOutNode->unSocket,acTmpBuf,RECV_LENTH,0);

			
			printf("���յ�%d�ֽ����ݣ� \n",nRecvLenth);
			//printf("���յ�������tmp�ַ���ΪacTmpBuf = %s\n ",acTmpBuf);

			if (nRecvLenth > 0)						//�ж� ������� С�����أ������������ݽ��ղŽ����ģ�Ϊʲôû�������յ���
			{
				memcpy(pstFindOutNode->acRecvBuff, acTmpBuf, nRecvLenth);       //ƴ�ӵ�ԭʼ�������ϡ� //20140416 ����Ҫ�ģ���Ϊƴ�ӵġ� ����һ�����λ����������͸ı��λ�á� ����������Ϊ�������ƶ���
				pstFindOutNode->nRecvBuffSize = nRecvLenth;                           //ע���������
				//MessageBox(NULL, buffer, "recv", MB_ICONINFORMATION);
				//printf("�յ��ַ��� %s \n ", pstFindOutNode->acRecvBuff);
// 				printf("ʮ�����Ʊ�ʾΪ:\n ");
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
			printf("��0.5����δ���յ�����\n");
		}
	}
	LeaveCriticalSection(&g_csRsLIST);		// �뿪�ٽ���	���ձ�


	////////////////////////////////////////////////////
	// ת�ƽڵ㣬�������Ҫ���Э�顣
	//////////////////////////////////////////////////////
	printf("--ת�ƽڵ�--\n");
	printf("�շ���Ϣ����Ľڵ����Ϊ %d ��\n", pstRsLIST->nNodeCount);
	EnterCriticalSection(&g_csRsLIST);		// �����ٽ���	���ձ�
	for (nIndex = 0; nIndex < pstRsLIST->nNodeCount; nIndex++)
	{
		/////////////////////////////////////////////
		// //��ȡ�ͻ��˽ڵ��ַ
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
		//���Э��
		pstHeadMsgTmp = (P_NET_HEAD)pstFindOutNode->acRecvBuff;       //������	

		
		if (pstHeadMsgTmp->DataLen == 0 )
		{
			
			if (pstHeadMsgTmp != NULL)
			{
				printf("��ȡ���ڵ㣬�������У���������Ϊ�� -->  ", nRecvLenth);     
				MoveClient(nIndex);		
			}
		}
		else 
		{		
			//unBodyLenth = unRecvLenth-7;										//�����Ƿ������Ƿ�Ҫ�������գ�����û�й�ע��
			//if (unBodyLenth == pstHeadMsgTmp->DataLen)
			//{
			//}
			if (pstHeadMsgTmp != NULL)
			{
				printf("��ȡ���ڵ㣬����������������%d�ֽ� ���д��ж������ԡ�-->  ", nRecvLenth);      //�д����ƣ�20140415�� //20140417δ���ơ�
				MoveClient(nIndex);		
			}
		}
		

	}



	LeaveCriticalSection(&g_csRsLIST);		// �뿪�ٽ���	���ձ�
	
	return 0;
}



int MoveClient(int nIndex)
{

		/////////////////////////////////////////////
		// //�Ƴ��ڵ�
		//////////////////////////////////////////////
		PLIST_NODE_T pstOutNode = LST_OutNodeByPos(pstRsLIST, nIndex);     //�Ƴ��ڵ�    //20140410 ��
		P_DATA_T pstDataNode =(P_DATA_T)pstOutNode->pvData;
		/*�����ٽ���*/	
		EnterCriticalSection(&g_csDuteFIFO);		// �����ٽ���	ҵ����FIFO


		if(pstDuteFIFO == NULL)
		{
			DuteFIFO_Init();
			printf("FIFO��û�нڵ㣬�ƶ�ʧ��\n");
			//break;
		}
		if(pstDuteFIFO != NULL)
		{
			FIFO_Push(pstDuteFIFO, pstDataNode); 
			printf("  �ڵ��Ƴ��ɹ� \n");
		}

		LeaveCriticalSection(&g_csDuteFIFO);		// �뿪�ٽ���	ҵ����FIFO


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
