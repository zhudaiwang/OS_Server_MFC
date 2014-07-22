#ifndef _LIST_H_
#define _LIST_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


//实现一套通用的链表管理接口，该链表接口可以实现对任意数据类型的增，删，改，查四个接口。
typedef struct tagLIST_NODE_T
{
	void * pvData;
	struct tagLIST_NODE_T * pstNexter;
}LIST_NODE_T,*PLIST_NODE_T;

typedef struct tagLIST_HEADER_T
{
	int nNodeCount;
	PLIST_NODE_T pstFirstNode;
	PLIST_NODE_T pstLastNode;
}LIST_HEADER_T,*PLIST_HEADER_T;

// -----------INIT------------------------------------------------------------------


PLIST_HEADER_T Link_Init();



//---------------- LST_Insert--------------------------------------------------------
//将给出的数据插入到链表指定的位置上，如果链表节点总数小于这个位置，则插入到链表最末尾！
//插入完成后，返回插入节点的指针，插入失败，返回NULL。
PLIST_NODE_T LST_Insert(
						PLIST_HEADER_T pstList,
						int nInsertPos,
						void * pvData,
						int nDataSize);


// ---------------------------DELETE------------------------------------------------
// 
// 根据位置删除链表中的指定节点，删除成功，返回0，其他为删除失败！
int LST_DeleteByPos(
					PLIST_HEADER_T pstList,
					int nDeletePos);


//将指定节点从链表中删除，删除成功，返回0，其他为删除失败！
int LST_DeleteByNode(
					 PLIST_HEADER_T pstList,
					 PLIST_NODE_T pstNode);




// -------------------------LOOK-FOR--------------------------------------------------
// 
// 
// 查找链表中指定条件的节点，条件判断由回调函数FIND_CB_F来确定！
typedef int (*FIND_CB_F)(
						 void * CallBackArg, 
						 int nKeyLenth,
						 PLIST_NODE_T pstNode);



PLIST_NODE_T LST_FindNode(
						  PLIST_HEADER_T pstList,
						  void * pvKey, 
						  int nKeyLenth,
						  FIND_CB_F fFindCallBack);

PLIST_NODE_T  LST_GetAt(
						PLIST_HEADER_T pstList,
						int nNodeIndex);

int LST_GetCount(PLIST_HEADER_T pstList);


PLIST_NODE_T LST_OutNodeByPos(PLIST_HEADER_T pstList, int nDeletePos);// 代码开始先把指针移动到那个位置


int main_link();


#endif // _LIST_H_


