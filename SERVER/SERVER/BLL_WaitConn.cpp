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
	//AcceptSocket是客户端IP和端口号。。
	//***********************************************

	SOCKET AcceptSocket; //AcceptSocket是客户端IP和端口号。。
	fd_set fdread;
	timeval tv;
	int nSize; 
	int nClientSize = sizeof(sockaddr);

	while(1)
	{

		FD_ZERO(&fdread);//初始化fd_set
		FD_SET(fdServer, &fdread);//分配套接字句柄到相应的fd_set


		tv.tv_sec = 5;//这里我们打算让select等待两秒后返回，避免被锁死，也避免马上返回
		tv.tv_usec = 0;

		select(0, &fdread, NULL, NULL, &tv);

		nSize = sizeof(server);
		if (FD_ISSET(fdServer, &fdread))//如果套接字句柄还在fd_set里，说明客户端已经有connect的请求发过来了，马上可以accept成功
		{
			AcceptSocket = accept(fdServer,( sockaddr*) &client, &nClientSize);
			
			unsigned int unClientIPaddr =  client.sin_addr.S_un.S_addr;
			
			//printf(" %u \n",unClientIPaddr);
			//printf("%u \n",client.sin_addr.S_un.S_addr);
			printf("-------------等待连接Conn----------------\n");
			printf("一个客户端连入：%s\n",inet_ntoa(client.sin_addr));
			OutToFile("一个客户端连入：");
			OutToFile(inet_ntoa(client.sin_addr));

#if 1

			/////////////////////////////////////////////////////
			//			ip 表
			///////////////////////////////////////////////////////
			//	提取socket，ip地址，寻找ip表，	*///              //注意进入临界区。出临界区。注意。  这个表被三个模块使用到。
			//     找到对应的ip，改掉对应的socket*/
			//	如果没有，就添加一个节点	·	*/								//添加到链表
			/************************************/

			if (pstClientLIST == NULL) //如果ip表 不存在
			{
				ClientLIST_Init();        //初始化ip表
			}

			EnterCriticalSection(&g_csClientLIST);		// 进入临界区   客户端表

			PLIST_NODE_T pstTmpNode = LST_FindNode(pstClientLIST, &unClientIPaddr, 4 , FIND_IP );
			

			if (pstTmpNode == NULL)																			//无节点			
			{
				P_ClientAddr_T pstClientNode = AddNewClientAddr(client.sin_addr.S_un.S_addr, inet_ntoa(client.sin_addr)); 				//生成节点
				LST_Insert(pstClientLIST, 1024, pstClientNode, sizeof(ClientAddr_T));					//添加节点
			}
			else /* (找到了节点)*/
			{
				P_ClientAddr_T pstClientNode = (P_ClientAddr_T)pstTmpNode->pvData;        /* 该位置的socket 换掉*/      //pvData是起始地址，我这样做，只是可以给指定位置赋给他IP值
				pstClientNode->unSocket = AcceptSocket;//20140404写到这里								//指定位置赋给他IP值
				printf("改变了这个IP对应的 SOCKET \n");
			}		
				
				
			
			LeaveCriticalSection(&g_csClientLIST);		// 离开临界区	客户端表		这时候是要等解锁了才执行以下的语句。用用同步。																	
			
																						
			///////////////////////////////////////////////////////
			//			接收 表
			////////////////////////////////////////////////////
			/************************************/
			/*						            */					//存socket 用于接收
			/*						            */					// 注意进入临界区。出临界区。注意。  FIFO 压入和读出的时候， FIFO,两个模块都会使用到。
			/*						            */					//添加到FIFO		
			/************************************/

			P_DATA_T pstRecvBufNode = AddNewRecvBuf(AcceptSocket, unClientIPaddr); //一共两种表，上面一种是发送表，现在这种是接收表。

/*进入临界区*/	
			EnterCriticalSection(&g_csConnFIFO);		// 进入临界区	等待链接队列 接收表
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
			LeaveCriticalSection(&g_csConnFIFO);		// 离开临界区	等待链接队列 接收表
			
/*出临界区*/

#endif

		//	break;
		}

		else//还没有客户端的connect请求，我们可以去做别的事，避免像没有用select方式的阻塞套接字程序被锁死的情况，如果没用select,当程序运行到accept的时候客户端恰好没有connect请求，那么程序就会被锁死，做不了任何事情
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
	//ret = recv(AcceptSocket,buffer,128,0);//这里同样可以用select，用法和上面一样

	//MessageBox(NULL, buffer, "recv", MB_ICONINFORMATION);
	//printf("recv: %s\n", buffer);




	WSACleanup();

	return 1;

}



