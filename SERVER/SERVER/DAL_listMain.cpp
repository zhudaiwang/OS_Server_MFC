#include "stdafx.h"

//2013.2.27 晚
// 查找链表中指定条件的节点， 实现得很不好 ，其他都实现了.
#include "list.h"
#include "user.h"


int main_list()
{
	char *cC ="CC";
	int nIndex = 0;
	PLIST_HEADER_T list = Link_Init();

	PLIST_NODE_T pOneNode[5] = {0};
	PLIST_NODE_T pFindNode = {0};

	LST_Insert(list, 1, "CC", 3);
  	LST_Insert(list, 9, "hh", 3);
	LST_Insert(list, 9, "iiiii", 6);
	LST_Insert(list, 9, "nn", 3);
	LST_Insert(list, 9, "aa", 3);

	for (nIndex = 0; nIndex <5; nIndex++)
	{
		pOneNode[nIndex] = LST_GetAt(list,nIndex);
	}
	
	for (nIndex = 0; nIndex <5; nIndex++)
	{
		printf("pOneNode[%d] 的地址是%x，数据域是%s\n",nIndex,(char*)pOneNode[nIndex],pOneNode[nIndex]->pvData);
	}

	printf("未删除前链表个数 = %d \n", list->nNodeCount);
	pFindNode = LST_FindNode(list, &cC,strlen(cC), FIND_STR);//如果是查找字符串的话，就调用FIND_STR函数
	printf("找到的节点pFindNode的地址是%x\n",pFindNode);
	LST_DeleteByPos(list, 5);
	LST_DeleteByPos(list, 4);
	LST_DeleteByPos(list, 1);
	LST_DeleteByPos(list, 1);
	LST_DeleteByPos(list, 1);

	printf("删除后链表个数 = %d \n", list->nNodeCount);

	while(1);



}