#include "stdafx.h"
#include <iostream>
#include <winsock2.h>
//#include <windows.h>

#include "DATA.h"
#include "BLL_WaitConn.h"
#include "BLL_Init.h"


#define InternetAddr "127.0.0.1"
#define iPort 8080

#pragma comment(lib, "ws2_32.lib")

using namespace std;


void OutToFile(char *pcSTR)
{

	FILE *fp;  
	char acSavePlace[32] = "D:\\";
	strcat(acSavePlace, pcSTR);
	strcat(acSavePlace, ".txt");
	fp=fopen(acSavePlace,"wb");  
	if (NULL==fp)  
	{  
		printf ("cannt open\n");  
		exit (1);            
	}  
	fclose(fp);
}



unsigned int __stdcall main_WaitConn(void *)
{

#if DEBUG_SIG
	printf("main_WaitConn  line 18\n");
#endif
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2), &wsa);

	SOCKET fdServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in server;
	struct sockaddr_in client;
	//client.sin_addr.S_un.S_addr = 0;
	server.sin_family = AF_INET;
	//server.sin_addr.s_addr = inet_addr(InternetAddr);
	server.sin_addr.s_addr = htonl(INADDR_ANY);	
	server.sin_port = htons(iPort);

	int ret = bind(fdServer, (sockaddr*)&server, sizeof(server));
	ret = listen(fdServer, 4);

	//****************************************************
	//AcceptSocket�ǿͻ���IP�Ͷ˿ںš���
	//***********************************************

	SOCKET AcceptSocket; //AcceptSocket�ǿͻ���IP�Ͷ˿ںš���
	fd_set fdread;
	timeval tv;
	int nSize; 
	int nClientSize = sizeof(sockaddr);

	while(1)
	{

		FD_ZERO(&fdread);//��ʼ��fd_set
		FD_SET(fdServer, &fdread);//�����׽��־������Ӧ��fd_set


		tv.tv_sec = 5;//�������Ǵ�����select�ȴ�����󷵻أ����ⱻ������Ҳ�������Ϸ���
		tv.tv_usec = 0;

		select(0, &fdread, NULL, NULL, &tv);

		nSize = sizeof(server);
		if (FD_ISSET(fdServer, &fdread))//����׽��־������fd_set�˵���ͻ����Ѿ���connect�����󷢹����ˣ����Ͽ���accept�ɹ�
		{
			AcceptSocket = accept(fdServer,( sockaddr*) &client, &nClientSize);
			
			unsigned int unClientIPaddr =  client.sin_addr.S_un.S_addr;
			
			//printf(" %u \n",unClientIPaddr);
			//printf("%u \n",client.sin_addr.S_un.S_addr);
			printf("-------------�ȴ�����Conn----------------\n");
			printf("һ���ͻ������룺%s\n",inet_ntoa(client.sin_addr));
			OutToFile("һ���ͻ������룺");
			OutToFile(inet_ntoa(client.sin_addr));

#if 1

			/////////////////////////////////////////////////////
			//			ip ��
			///////////////////////////////////////////////////////
			//	��ȡsocket��ip��ַ��Ѱ��ip��	*///              //ע������ٽ��������ٽ�����ע�⡣  ���������ģ��ʹ�õ���
			//     �ҵ���Ӧ��ip���ĵ���Ӧ��socket*/
			//	���û�У������һ���ڵ�	��	*/								//��ӵ�����
			/************************************/

			if (pstClientLIST == NULL) //���ip�� ������
			{
				ClientLIST_Init();        //��ʼ��ip��
			}

			EnterCriticalSection(&g_csClientLIST);		// �����ٽ���   �ͻ��˱�

			PLIST_NODE_T pstTmpNode = LST_FindNode(pstClientLIST, &unClientIPaddr, 4 , FIND_IP );
			

			if (pstTmpNode == NULL)																			//�޽ڵ�			
			{
				P_ClientAddr_T pstClientNode = AddNewClientAddr(client.sin_addr.S_un.S_addr); 				//���ɽڵ�
				LST_Insert(pstClientLIST, 1024, pstClientNode, sizeof(ClientAddr_T));					//��ӽڵ�
			}
			else /* (�ҵ��˽ڵ�)*/
			{
				P_ClientAddr_T pstClientNode = (P_ClientAddr_T)pstTmpNode->pvData;        /* ��λ�õ�socket ����*/      //pvData����ʼ��ַ������������ֻ�ǿ��Ը�ָ��λ�ø�����IPֵ
				pstClientNode->unSocket = AcceptSocket;//20140404д������								//ָ��λ�ø�����IPֵ
				printf("�ı������IP��Ӧ�� SOCKET \n");
			}		
				
				
			
			LeaveCriticalSection(&g_csClientLIST);		// �뿪�ٽ���	�ͻ��˱�		��ʱ����Ҫ�Ƚ����˲�ִ�����µ���䡣����ͬ����																	
			
																						
			///////////////////////////////////////////////////////
			//			���� ��
			////////////////////////////////////////////////////
			/************************************/
			/*						            */					//��socket ���ڽ���
			/*						            */					// ע������ٽ��������ٽ�����ע�⡣  FIFO ѹ��Ͷ�����ʱ�� FIFO,����ģ�鶼��ʹ�õ���
			/*						            */					//��ӵ�FIFO		
			/************************************/

			P_DATA_T pstRecvBufNode = AddNewRecvBuf(AcceptSocket, unClientIPaddr); //һ�����ֱ�����һ���Ƿ��ͱ����������ǽ��ձ�

/*�����ٽ���*/	
			EnterCriticalSection(&g_csConnFIFO);		// �����ٽ���	�ȴ����Ӷ��� ���ձ�
			if(pstConnFIFO == NULL)
			{
				ConnFIFO_Init();
				//break;
			}
			//printf("if(pstConnFIFO == NULL)\n"); 
			//printf("if(pstConnFIFO == %d)\n",pstConnFIFO); 
			if(pstConnFIFO != NULL)
			{
				FIFO_Push(pstConnFIFO, pstRecvBufNode); 
				//printf("\n\t ConnFIFO :line 130 FIFO_Push(pstConnFIFO, pstRecvBufNode); \n"); 
			}
			LeaveCriticalSection(&g_csConnFIFO);		// �뿪�ٽ���	�ȴ����Ӷ��� ���ձ�
			
/*���ٽ���*/

#endif

		//	break;
		}

		else//��û�пͻ��˵�connect�������ǿ���ȥ������£�������û����select��ʽ�������׽��ֳ�����������������û��select,���������е�accept��ʱ��ͻ���ǡ��û��connect������ô����ͻᱻ�������������κ�����
		{
			
			//::MessageBox(NULL, "waiting", "recv", MB_ICONINFORMATION);
			printf("waiting\n");
			OutToFile("waiting");

		}

		//





	}

	//char buffer[128];
	//ZeroMemory(buffer, 128);

	//::MessageBox(NULL, "CONNECTED", "recv", MB_ICONINFORMATION);
	//printf("waiting\n");
	//Sleep(10000);
	//ret = recv(AcceptSocket,buffer,128,0);//����ͬ��������select���÷�������һ��

	//MessageBox(NULL, buffer, "recv", MB_ICONINFORMATION);
	//printf("recv: %s\n", buffer);




	WSACleanup();

	return 1;

}



