
#include "stdafx.h"

#include "list.h"
#include "user.h"
#include "DATA.h"

//字符串比较，相等返回0。
int FIND_STR(void * CallBackArg,int nKeyLenth, PLIST_NODE_T pstNode)
{
	int	*pnTmp	= (int *) CallBackArg;
	char *pcStr	= (char*)*pnTmp;
	char *cNodeVel =(char*)(pstNode->pvData);
	return strncmp(pcStr,cNodeVel, nKeyLenth);
}


//比较里面的(ip地址位置)是否和输入的关键字相同。
//这个ip地址也就是pv数据所指地址的一段空间。
int FIND_IP(void * CallBackArg, int nKeyLenth, PLIST_NODE_T pstNode)
{
	int	*pnTmp	= (int *) CallBackArg;
	char *pcStr	= (char*)*pnTmp;
	unsigned int unAddr = (unsigned int )pcStr;
	int nResult = -1;

	P_ClientAddr_T pstNodeVel =(P_ClientAddr_T)(pstNode->pvData); //获取node里的地址
	unsigned int cNodeVel = pstNodeVel->unIpAddress;
	if (unAddr == cNodeVel)
	{
		return 0;
	}
	else
		return nResult;//
}