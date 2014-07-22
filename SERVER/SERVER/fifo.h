#ifndef _FIFO_H_
#define _FIFO_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tagFIFO_T
{
	unsigned int nNodeSize;
	unsigned int nNodeMax;
	void *pvDataBuff;
	void *pvRead;
	void *pvWrite;
	unsigned int nNodeCount;
	unsigned int nReadTimes;
	unsigned int nWriteTimes;

}FIFO_T, *P_FIFO_T;

P_FIFO_T FIFO_Creat(int nNodeMax, int nNodeSize);

void FIFO_Push(P_FIFO_T hFifo, void *pvBuff);

char * FIFO_Pop(P_FIFO_T hFifo);


#endif  //_FIFO_H_