#ifndef _LIST_H_
#define _LIST_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


//ʵ��һ��ͨ�õ��������ӿڣ�������ӿڿ���ʵ�ֶ������������͵�����ɾ���ģ����ĸ��ӿڡ�
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
//�����������ݲ��뵽����ָ����λ���ϣ��������ڵ�����С�����λ�ã�����뵽������ĩβ��
//������ɺ󣬷��ز���ڵ��ָ�룬����ʧ�ܣ�����NULL��
PLIST_NODE_T LST_Insert(
						PLIST_HEADER_T pstList,
						int nInsertPos,
						void * pvData,
						int nDataSize);


// ---------------------------DELETE------------------------------------------------
// 
// ����λ��ɾ�������е�ָ���ڵ㣬ɾ���ɹ�������0������Ϊɾ��ʧ�ܣ�
int LST_DeleteByPos(
					PLIST_HEADER_T pstList,
					int nDeletePos);


//��ָ���ڵ��������ɾ����ɾ���ɹ�������0������Ϊɾ��ʧ�ܣ�
int LST_DeleteByNode(
					 PLIST_HEADER_T pstList,
					 PLIST_NODE_T pstNode);




// -------------------------LOOK-FOR--------------------------------------------------
// 
// 
// ����������ָ�������Ľڵ㣬�����ж��ɻص�����FIND_CB_F��ȷ����
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


PLIST_NODE_T LST_OutNodeByPos(PLIST_HEADER_T pstList, int nDeletePos);// ���뿪ʼ�Ȱ�ָ���ƶ����Ǹ�λ��


int main_link();


#endif // _LIST_H_


