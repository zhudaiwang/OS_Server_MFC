#include "stdafx.h"

//2013.2.27 ��
// ����������ָ�������Ľڵ㣬 ʵ�ֵúܲ��� ��������ʵ����.
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
		printf("pOneNode[%d] �ĵ�ַ��%x����������%s\n",nIndex,(char*)pOneNode[nIndex],pOneNode[nIndex]->pvData);
	}

	printf("δɾ��ǰ������� = %d \n", list->nNodeCount);
	pFindNode = LST_FindNode(list, &cC,strlen(cC), FIND_STR);//����ǲ����ַ����Ļ����͵���FIND_STR����
	printf("�ҵ��Ľڵ�pFindNode�ĵ�ַ��%x\n",pFindNode);
	LST_DeleteByPos(list, 5);
	LST_DeleteByPos(list, 4);
	LST_DeleteByPos(list, 1);
	LST_DeleteByPos(list, 1);
	LST_DeleteByPos(list, 1);

	printf("ɾ����������� = %d \n", list->nNodeCount);

	while(1);



}