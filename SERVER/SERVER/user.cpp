
#include "stdafx.h"

#include "list.h"
#include "user.h"
#include "DATA.h"

//�ַ����Ƚϣ���ȷ���0��
int FIND_STR(void * CallBackArg,int nKeyLenth, PLIST_NODE_T pstNode)
{
	int	*pnTmp	= (int *) CallBackArg;
	char *pcStr	= (char*)*pnTmp;
	char *cNodeVel =(char*)(pstNode->pvData);
	return strncmp(pcStr,cNodeVel, nKeyLenth);
}


//�Ƚ������(ip��ַλ��)�Ƿ������Ĺؼ�����ͬ��
//���ip��ַҲ����pv������ָ��ַ��һ�οռ䡣
int FIND_IP(void * CallBackArg, int nKeyLenth, PLIST_NODE_T pstNode)
{
	int	*pnTmp	= (int *) CallBackArg;
	char *pcStr	= (char*)*pnTmp;
	unsigned int unAddr = (unsigned int )pcStr;
	int nResult = -1;

	P_ClientAddr_T pstNodeVel =(P_ClientAddr_T)(pstNode->pvData); //��ȡnode��ĵ�ַ
	unsigned int cNodeVel = pstNodeVel->unIpAddress;
	if (unAddr == cNodeVel)
	{
		return 0;
	}
	else
		return nResult;//
}