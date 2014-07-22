#include "stdafx.h"
#include "DecordStr.h"
#include <stdlib.h>
#include  <string.h> 
#include<stdio.h>

DecordStr::DecordStr()
{
}

DecordStr::DecordStr( char *pcOrigCode)
{
	int		nIndex;
	int		nLenth = strlen(pcOrigCode);
	//char	*
	char	acPreStr[7] = "0x";
	char	acLastStr[7] = "";
	char	acInputStr[1024 * NKBNO * 4] = {0};
	char	accOutStr[1024 * NKBNO][5] = {0};

	acnOutNo = (char *)malloc(1024*NKBNO);  //���������ٿռ�


	memset(acnOutNo, 0, 1024*NKBNO);
	memcpy(acInputStr, pcOrigCode, strlen(pcOrigCode));
	for (nIndex = 0, nOutStrIndex = 0; nIndex < nLenth; nIndex++)
	{
		if((acInputStr[nIndex] != ' ')&& (acInputStr[nIndex] >='0'))
		{
			strncpy(acLastStr, &acInputStr[nIndex], 2);
			strcpy(&acPreStr[2], acLastStr);
			nIndex++;
			strcpy(accOutStr[nOutStrIndex], acPreStr);
			sscanf(accOutStr[nOutStrIndex], "%x", &acnOutNo[nOutStrIndex]);
			memset(acPreStr, 0, 5);
			strcpy(acPreStr, "0x");
			nOutStrIndex++ ;
		}
	}

}


DecordStr::~DecordStr(void)
{
}


//////////////////////////
//		��ʼ���ӿ�
//////////////////////////

char * DecordStr::Code2Str( char *pcOrigCode)
{
	int		nIndex;
	int		nLenth = strlen(pcOrigCode);
	//char	*
	char	acPreStr[7] = "0x";
	char	acLastStr[7] = "";
	char	acInputStr[1024 * NKBNO * 4] = {0};
	char	accOutStr[1024 * NKBNO][5] = {0};

	acnOutNo = (char *)malloc(1024*NKBNO);  //���������ٿռ�


	memset(acnOutNo, 0, 1024*NKBNO);
	memcpy(acInputStr, pcOrigCode, strlen(pcOrigCode));
	for (nIndex = 0, nOutStrIndex = 0; nIndex < nLenth; nIndex++)
	{
		if((acInputStr[nIndex] != ' ')&& (acInputStr[nIndex] >='0'))
		{
			strncpy(acLastStr, &acInputStr[nIndex], 2);
			strcpy(&acPreStr[2], acLastStr);
			nIndex++;
			strcpy(accOutStr[nOutStrIndex], acPreStr);
			sscanf(accOutStr[nOutStrIndex], "%x", &acnOutNo[nOutStrIndex]);
			memset(acPreStr, 0, 5);
			strcpy(acPreStr, "0x");
			nOutStrIndex++ ;
		}
	}

	return acnOutNo;
}





//////////////////////////
//		������ļ�
//////////////////////////
void DecordStr::OutToFile()
{

	FILE *fp;  
	float num=11.22334;  
	char acSavePlace[32] = "F:\\�»���";
	char * pcThisDealNo = getThisDealNo_ForXINHUADU();
	strcat(acSavePlace, pcThisDealNo);
	strcat(acSavePlace, ".txt");
	fp=fopen(acSavePlace,"wb");  
	if (NULL==fp)  
	{  
		printf ("cannt open\n");  
		exit (1);            
	}  
	fwrite(acnOutNo,nOutStrIndex ,1,fp);//��ʱ�ı�������ǵ�ַ   
	fclose(fp);
}



//////////////////////////
//		СƱ������Ϣ
//////////////////////////
char * DecordStr::getThisDealNo()
{
	char *pcDealNo = getAfterStrAddr(acnOutNo, "����:");
	return  GetAnyThing(pcDealNo, 0, 14, 16);
}


char * DecordStr::getThisDealNo_ForXINHUADU()
{
	char *pcDealNo = getAfterStrAddr(acnOutNo, "����:");
	return  GetAnyThing(pcDealNo, 0, 8, 9);
}

char * DecordStr::getThisCashierNo()
{
	char *pcCashierNo = getAfterStrAddr(acnOutNo, "����:");
	return  GetAnyThing(pcCashierNo, 0, 4, 4);
}




int  DecordStr::getDealGoodsKinds()       //��Ʒ����Ŀ
{
	int nIndex= 0;
	int nLine = 0;
	int nContent =0;
	char *StrAddrStart = getGoodsAddr();
	char *StrAddrEnd = getGoodsAddrEnd();

	nContent = StrAddrEnd - StrAddrStart;

	for (nIndex = 0; nIndex < nContent; nIndex++)
	{
		if (StrAddrStart[nIndex] == 0x0a)
		{
			nLine++;
		}
	}
	return nLine/2;
}




char * DecordStr::getDealTotalNum()
{
	char* pcDealEnd = getDealEndAddr();
	return GetAnyThing(pcDealEnd, 14, 3, 4);
}



char * DecordStr::getDealTotalPrice()  //СƱ��Ʒ�ܼۺϼƣ���ȡ�ģ�
{
	char* pcDealEnd = getDealEndAddr();
	return GetAnyThing(pcDealEnd, 25, 7, 8);
}





char * DecordStr::getRecvedMoney()  //ʵ��RMB
{
	char* pcDealEnd = getDealEndAddr();
	return GetAnyThing(pcDealEnd, 44, 5, 8);
}

char * DecordStr::getBackMoney()    //����
{
	char* pcDealEnd = getDealEndAddr();
	return GetAnyThing(pcDealEnd, 71, 6, 8);
}

char * DecordStr::getDealTime()       //СƱ��ӡʱ��
{
	char* pcDealEnd = getDealEndAddr();
	return GetAnyThing(pcDealEnd, 99, 18, 32);
}





//////////////////////////
//		ĳ��Ʒ������Ϣ
//////////////////////////

char * DecordStr::getGoodsName(unsigned int nItemNo)
{
	char *StrAddrStart = getItemNoAddr(nItemNo);
	 return GetAnyThing(StrAddrStart, 3, 24, 32);
}

char* DecordStr::getOneGoodsCount(unsigned int nItemNo)
{
	char *StrAddrStart = getItemNoAddr(nItemNo );
	return GetAnyThing(StrAddrStart, 46, 3, 4);

}
char* DecordStr::getGoodsUnitPrice(unsigned int nItemNo)
{
	char *StrAddrStart = getItemNoAddr(nItemNo);
	return GetAnyThing(StrAddrStart, 51, 5, 8);

}

char* DecordStr::getGoodsTotalPrice(unsigned int nItemNo )
{
	char *StrAddrStart = getItemNoAddr(nItemNo );
	return GetAnyThing(StrAddrStart, 57, 7, 8);

}



////////////////////////////////////
//		�ײ�ӿ�
////////////////////////////////


	char *  DecordStr::getItemNoAddr(unsigned int nItemNo )
	{
		char *StrAddrStart;
		int nIndex;
		int nOffSet = 0;
		int nDealGoodsKindsNo = getDealGoodsKinds();
		if (nItemNo > nDealGoodsKindsNo)
		{
			return NULL;
		}

		StrAddrStart = getAfterStrAddr(acnOutNo, "---");

		for (nIndex=0; nIndex<(nItemNo*2-1); nIndex++)
		{
			while (StrAddrStart[nOffSet] != 0x0a ) 
			{
				nOffSet++;
			}
			nOffSet++;
		}

		return &StrAddrStart[nOffSet];

	}


	char * DecordStr::getPreStrAddr(char *pcOutStr, char *pcKeyWords )
	{
		int nIndex = 0 ;
		for (;nIndex < NKBNO *1024; nIndex++)
		{
			if (!memcmp(&pcOutStr[nIndex], pcKeyWords, strlen(pcKeyWords)))
			{
				return &pcOutStr[nIndex];
			}
		}
		return NULL;
	}




	char * DecordStr::getAfterStrAddr(char *pcOutStr, char *pcKeyWords )
	{
		int nIndex = 0 ;
		for (;nIndex < NKBNO *1024; nIndex++)
		{
			while ( 0 != memcmp(&pcOutStr[nIndex], pcKeyWords, strlen(pcKeyWords)))
			{
				nIndex += 1;
			}
			while (!memcmp(&pcOutStr[nIndex], pcKeyWords, strlen(pcKeyWords)))
			{
				nIndex += 1;
			}
			nIndex +=  strlen(pcKeyWords)-1;
			return &pcOutStr[nIndex];
		}
		return NULL;
	}




	char* DecordStr::GetAnyThing(char *StrAddrStart, int nOffSetAddr, int nLenth, int nBufferLenth)
	{
		int nIndex = 0;
		char *pcResult = (char*)malloc(nBufferLenth);
		memset(pcResult, 0, nBufferLenth);
		for (nIndex = 0; nIndex < nLenth; nIndex++,nOffSetAddr++)
		{
			pcResult[nIndex] = StrAddrStart[nOffSetAddr];
		}
		return pcResult;
	}




	char * DecordStr::getDealEndAddr()
	{
		char *pcDealEnd = getGoodsAddrEnd();

		pcDealEnd = getAfterStrAddr(pcDealEnd, "---");

		return pcDealEnd+1;

	}


	char *  DecordStr::getGoodsAddr()
	{
		char *StrAddrStart = getAfterStrAddr(acnOutNo, "---");

		return (StrAddrStart+1);
	}


	char*  DecordStr::getGoodsAddrEnd()//
	{
		char *StrAddrStart = getGoodsAddr();

		int nOffSet = StrAddrStart - acnOutNo ;

		return getPreStrAddr(&acnOutNo[nOffSet], "---");

	}






