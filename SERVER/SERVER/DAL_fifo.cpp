
#include "stdafx.h"
#include "fifo.h"




P_FIFO_T FIFO_Creat(int nNodeMax, int nNodeSize)
{
	P_FIFO_T pstNewFifo = (P_FIFO_T)malloc(sizeof(FIFO_T));
	if (pstNewFifo == NULL)
	{
		return NULL;
	}
//	pstNewFifo->nNodeCount	= nNodeMax;
	pstNewFifo->nNodeMax	= nNodeMax;
	pstNewFifo->nNodeSize	= nNodeSize;
	pstNewFifo->nNodeCount	= 0;
	pstNewFifo->pvDataBuff	= (char *)malloc(nNodeMax * nNodeSize);
	pstNewFifo->pvRead		= pstNewFifo->pvDataBuff;
	pstNewFifo->pvWrite		= pstNewFifo->pvDataBuff;
	pstNewFifo->nReadTimes	= 0;
	pstNewFifo->nWriteTimes	= 0;

	return pstNewFifo;

}




void FIFO_Push(P_FIFO_T hFifo, void *pvBuff)
{
	P_FIFO_T pstFifo = hFifo;
	char *pcNewBuff = (char *)pvBuff;
	char *pcBuffEnd = (char *)pstFifo->pvDataBuff + pstFifo->nNodeSize * pstFifo->nNodeMax;

	if (pstFifo->pvWrite == pcBuffEnd)
	{
		pstFifo->pvWrite = pstFifo->pvDataBuff;
		pstFifo->nWriteTimes++;

		if ((pstFifo->pvWrite == pstFifo->pvRead) &&
			(pstFifo->nWriteTimes > pstFifo->nReadTimes))
		{
			if (pstFifo->pvRead == pcBuffEnd)
			{
				pstFifo->pvRead =  pstFifo->pvDataBuff;
			}
			else
				pstFifo->pvRead = (char *)pstFifo->pvRead + pstFifo->nNodeSize;
		}
	}
	memcpy(pstFifo->pvWrite, pcNewBuff, pstFifo->nNodeSize);
	pstFifo->pvWrite = (char *)pstFifo->pvWrite + pstFifo->nNodeSize;
	pstFifo->nNodeCount++;
}


char * FIFO_Pop(P_FIFO_T hFifo)
{
	if (hFifo->pvRead == hFifo->pvWrite &&hFifo->nReadTimes == hFifo->nWriteTimes)
	{
		return NULL;
	}
	P_FIFO_T pstFifo = hFifo;
	char *pcReadBuff = (char *)malloc(pstFifo->nNodeSize);
	memset(pcReadBuff, 0x00,pstFifo->nNodeSize);
	char *pcBuffEnd = (char *)pstFifo->pvDataBuff  + pstFifo->nNodeSize * pstFifo->nNodeMax;

	if ( pstFifo->pvRead == pcBuffEnd)
	{
		 pstFifo->pvRead = pstFifo->pvDataBuff;
		 pstFifo->nReadTimes++;
	}
	memcpy(pcReadBuff, pstFifo->pvRead, pstFifo->nNodeSize);
	pstFifo->pvRead =  (char *)pstFifo->pvRead + pstFifo->nNodeSize;
	pstFifo->nNodeCount--;
	return pcReadBuff;

}
