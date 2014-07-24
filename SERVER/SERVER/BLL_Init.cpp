#include "DATA.h"
#include "stdafx.h"
#include "BLL_Init.h"


int BLL_InitCS()
{
	InitializeCriticalSection(&g_csClientLIST);
	InitializeCriticalSection(&g_csConnFIFO);
	InitializeCriticalSection(&g_csDuteFIFO);
	InitializeCriticalSection(&g_csRsLIST);
	
	InitializeCriticalSection(&g_csDuteFIFO_MFC);
	return 0;

}


int ConnFIFO_Init()
{
	pstConnFIFO = FIFO_Creat(20, sizeof(DATA_T));
	printf("pstConnFIFO->nNodeCount = %d",pstConnFIFO->nNodeCount);


	return 0;
}

int RsLIST_Init()
{
	pstRsLIST = Link_Init();
	return 0;
}

int DuteFIFO_Init()
{
	pstDuteFIFO = FIFO_Creat(20, sizeof(DATA_T));
	return 0;
}

int DuteFIFOMFC_Init()
{
	pstDuteFIFO_MFC = FIFO_Creat(20, sizeof(DATA_T));
	return 0;
}

int ClientLIST_Init()
{
	pstClientLIST = Link_Init();
	return 0;
}



unsigned int __stdcall BLL_InitFunc(void *)
{
	printf("BLL_InitFunc\n");
	int n= 0;
	int nIndex[6]= {-1, -1, -1, -1, -1 , -1};
	nIndex[0] = BLL_InitCS();
	nIndex[1] = ConnFIFO_Init();
	nIndex[2] = RsLIST_Init();
	nIndex[3] = DuteFIFO_Init();
	nIndex[4] = ClientLIST_Init();
	nIndex[5] =  DuteFIFOMFC_Init();

	for (n = 0; n < 6 ; n++)
	{
		printf("%d \n" ,nIndex[n]);

	}
	
	return 0;
}





