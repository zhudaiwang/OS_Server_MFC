#include "stdafx.h"
#include "list.h"


PLIST_HEADER_T Link_Init()
{
	PLIST_HEADER_T pstTopNode =(PLIST_HEADER_T)malloc(sizeof(LIST_HEADER_T));

	if (pstTopNode == NULL)
	{
		return NULL;
	}
	pstTopNode->nNodeCount  = 0;
	pstTopNode->pstFirstNode = NULL;
	pstTopNode->pstLastNode = NULL;
	return pstTopNode;
}


PLIST_NODE_T LST_Insert(PLIST_HEADER_T pstList, int nInsertPos, void * pvData, int nDataSize)
{
	int i;
	PLIST_NODE_T pstNewNode = (PLIST_NODE_T)malloc(sizeof(LIST_NODE_T));
	PLIST_NODE_T pstPreNode = pstList->pstFirstNode;

	if (pstNewNode == NULL)
	{
		return NULL;
	}

	//pstNewNode->pvData = malloc(nDataSize);
	

	if (pstNewNode->pvData  == NULL)
	{
		return NULL;
	}
	pstNewNode->pvData = pvData;

	//memset(pstNewNode->pvData, 0x00, nDataSize);

	//memcpy(pstNewNode->pvData, pvData, nDataSize);

	pstNewNode->pstNexter = NULL;
	
	if (nInsertPos <= pstList->nNodeCount)     
	{
		if ((nInsertPos == 1)&&(pstList->nNodeCount !=0)) //插入链表头
		{
			pstNewNode->pstNexter = pstList->pstFirstNode;
			pstList->pstFirstNode->pstNexter = pstNewNode;
		}
		else
		{
			for(i = 0; i < nInsertPos; i++)       //正常插入
			{
				pstPreNode = pstPreNode->pstNexter;
			}
			pstPreNode->pstNexter = pstNewNode;
			pstList->nNodeCount++;
		}


	}

	else if (nInsertPos > pstList->nNodeCount)
	{
		if (pstList->nNodeCount == 0)          //链表为空 这是第一个节点
		{
			pstList->nNodeCount = 1;
			pstList->pstFirstNode = pstNewNode;
			pstList->pstLastNode  = pstNewNode;
		}
		else
		{
			while(pstPreNode->pstNexter != NULL)    //插入链表末尾
			{
				pstPreNode = pstPreNode->pstNexter;
			}
			pstPreNode->pstNexter = pstNewNode;
			pstList->nNodeCount++;
		}
	}
	return pstNewNode;
}


int LST_DeleteByPos(PLIST_HEADER_T pstList, int nDeletePos)// 代码开始先把指针移动到那个位置
{
	PLIST_NODE_T pstDelNode = (PLIST_NODE_T)malloc(sizeof(LIST_NODE_T));
	PLIST_NODE_T pstPreNode = pstDelNode;
	pstDelNode = pstList->pstFirstNode;
	if (pstDelNode == NULL)
	{
		printf("error");
		return -1;
	}
	if (nDeletePos > pstList->nNodeCount)
	{
		return -1;
	}
	else if (nDeletePos == 1)
	{
		if (pstList->nNodeCount == 1)          //删除链表唯一节点
		{
			pstList->nNodeCount   = 0;
			pstList->pstFirstNode = NULL;
			pstList->pstLastNode  = NULL;
			return -1;
		}
		else
		{
			pstList->pstFirstNode = pstList->pstFirstNode->pstNexter;//删除链表（排第一个）的节点
			free(pstDelNode);
			pstList->nNodeCount--;
		}
	}
	else if (nDeletePos < pstList->nNodeCount)		//常规删除节点
	{
		
		pstPreNode = LST_GetAt(pstList,(nDeletePos-1));
		pstDelNode = LST_GetAt(pstList,(nDeletePos));
		pstPreNode->pstNexter = pstDelNode->pstNexter;
		pstList->nNodeCount--;
		free(pstDelNode);
	}
	else if (nDeletePos == pstList->nNodeCount)	 //删除链表表尾（排最后一个）
	{
		pstPreNode = LST_GetAt(pstList,(nDeletePos-1));
		pstDelNode = LST_GetAt(pstList,(nDeletePos));
		pstList->pstLastNode = pstPreNode;
		pstPreNode->pstNexter = NULL;
		pstList->nNodeCount--;
		free(pstDelNode);
	}
		return 0;
}






PLIST_NODE_T LST_FindNode(PLIST_HEADER_T pstList, void * pvCallBackArg, int nKeyLenth, FIND_CB_F fFindCallBack)
{


	PLIST_NODE_T pstFindNode = NULL;
	pstFindNode = pstList->pstFirstNode;

	
	if (pstFindNode == NULL)
	{
		printf("list is empty\n");
		return NULL;
	}
	else if (pstFindNode != NULL)
	{
		while (0 != fFindCallBack(pvCallBackArg,nKeyLenth,pstFindNode) )
		{
			pstFindNode = pstFindNode->pstNexter;
			if (pstFindNode == NULL)
			{
				printf("查找客户端列表，没有符合的节点！\n");
				return NULL;
			}	
		}
		return pstFindNode;
	}
	
}


#if 0

PLIST_NODE_T LST_FindNode(PLIST_HEADER_T pstList, void * pvCallBackArg, int nKeyLenth, FIND_CB_F fFindCallBack)
{


	PLIST_NODE_T pstFindNode;
	pstFindNode = pstList->pstFirstNode;


	if (pstFindNode == NULL)
	{
		printf("list is empty\n");
		return NULL;
	}
	else if (pstFindNode != NULL)
	{
		while (0 != fFindCallBack(pvCallBackArg,nKeyLenth,pstFindNode) )
		{
			pstFindNode = pstFindNode->pstNexter;
			if (pstFindNode == NULL)
			{
				printf("查找客户端列表，没有符合的节点！\n");
				return NULL;
			}	
		}
		return pstFindNode;
	}

}


#endif





PLIST_NODE_T  LST_GetAt(PLIST_HEADER_T pstList, int nNodeIndex)
{
	int i;
	PLIST_NODE_T pstPreNode = (PLIST_NODE_T)malloc(sizeof(LIST_NODE_T));
	if (pstPreNode == NULL)
	{
		return NULL;
	}
	pstPreNode = pstList->pstFirstNode;
	if (pstList->nNodeCount > nNodeIndex)
	{
		for (i = 0; i < nNodeIndex; i++)
		{
			pstPreNode = pstPreNode->pstNexter;
		}
		return pstPreNode;
	}
	else 
		return NULL;
}




int LST_GetCount(PLIST_HEADER_T pstList)
{
	return pstList->nNodeCount;
}








PLIST_NODE_T LST_OutNodeByPos(PLIST_HEADER_T pstList, int nDeletePos)// 代码开始先把指针移动到那个位置
{
	PLIST_NODE_T pstDelNode = (PLIST_NODE_T)malloc(sizeof(LIST_NODE_T));
	PLIST_NODE_T pstPreNode = pstDelNode;
	pstDelNode = pstList->pstFirstNode;
	if (pstDelNode == NULL)
	{
		printf("error");
		return NULL;
	}
	if (nDeletePos > pstList->nNodeCount)
	{
		return NULL;
	}
	else if (nDeletePos == 1)
	{
		if (pstList->nNodeCount == 1)          //删除链表唯一节点
		{
			//free(pstDelNode);
			pstList->nNodeCount   = 0;
			pstList->pstFirstNode = NULL;
			pstList->pstLastNode  = NULL;
			return NULL;
		}
		else
		{
			pstList->pstFirstNode = pstList->pstFirstNode->pstNexter;//删除链表（排第一个）的节点
			//free(pstDelNode);
			pstList->nNodeCount--;
			
		}
	}
	else if (nDeletePos < pstList->nNodeCount)		//常规删除节点
	{

		pstPreNode = LST_GetAt(pstList,(nDeletePos-1));
		pstDelNode = LST_GetAt(pstList,(nDeletePos));
		pstPreNode->pstNexter = pstDelNode->pstNexter;
		pstList->nNodeCount--;
		//free(pstDelNode);
	}
	else if (nDeletePos == pstList->nNodeCount)	 //删除链表表尾（排最后一个）
	{
		pstPreNode = LST_GetAt(pstList,(nDeletePos-1));
		pstDelNode = LST_GetAt(pstList,(nDeletePos));
		pstList->pstLastNode = pstPreNode;
		pstPreNode->pstNexter = NULL;
		pstList->nNodeCount--;
		//free(pstDelNode);
	}
	return pstDelNode;
}
