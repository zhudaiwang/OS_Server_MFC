#include "stdafx.h"
#include "DATA.h"

int main_fifo()
{

	int i = 0;
	DATA_T stData[5];
	P_FIFO_T fifo;


	stData[0].unSocket = 11;
	stData[1].unSocket = 12;
	stData[2].unSocket = 13;
	stData[3].unSocket = 14;
	stData[4].unSocket = 15;


	

	fifo =  FIFO_Creat(64, sizeof(stData[0]));

	
	printf("-----------------%d====\n",sizeof(stData[0]));

	
	FIFO_Push(fifo, &stData[0]);
	FIFO_Push(fifo, &stData[1]);
	FIFO_Push(fifo, &stData[2]);
	FIFO_Push(fifo, &stData[3]);
	FIFO_Push(fifo, &stData[4]);



	for (i = 4 ; i>0; i--)
	{
		printf("pcReadBuff = %d \n",*( FIFO_Pop(fifo)));
	}


	while(1);
}