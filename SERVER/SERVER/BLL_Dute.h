/******************************************************************************
* ��Ȩ���� (C)2014, weibo.com @zhudaiwang 
* �ļ����ƣ�
*    BLL_Dute.h
* ��ǰ�汾��
*     1.0
* ����ժҪ��
*     ҵ��㣬ҵ����ģ�顣
* ��ʷ��¼��
*     �޸���		����		�汾��		        ����
*     �����      2014.4.10       1.0
*******************************************************************************/

#ifndef _BLL_DUTE_H_
#define _BLL_DUTE_H_

//20140411 �������          //ans ��Ϊ uint8  uint16
#include "DATA.h"


#define ECHO_OK			0
#define ECHO_ERROR		1
#define ECHO_TIMEOUT	2


//Ӧ��ֵ���壺
// typedef enum
// {
// 	ECHO_OK,					// Ӧ��ɹ�
// 	ECHO_ERROR,					// Ӧ��ʧ��
// 	ECHO_TIMEOUT				// Ӧ��ʱ
// }NET_ECHO_VALUE;


typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned char NET_ECHO_VALUE;

// ����ʾ��ʽ˵�����硱�루1B��0~59����, 1B��ʾռ��1���ֽڣ�ȡֵ��Χ��0��59����
// 
// 	����������Ͱ���Ӧ������ĸ�ʽ�������ͷ(7Btyes)+����(N Bytes)
// 
// 	�����ͷ���壺

#pragma pack (1) /*ָ����1�ֽڶ���*/      
typedef struct tagNET_HEAD
{
	uint16 PacketID;				// �����ID��0~65535
	uint8 DirFlag;					// �����־�����Ͱ�-0xAA Ӧ���-0x00
	uint8 NetCmd;				    // ��������
	NET_ECHO_VALUE EchoValue;	// Ӧ��ֵ,��DirFlag==0x00ʱ��Ҫ����
	uint16 DataLen;				// ���ݳ���
}NET_HEAD, *P_NET_HEAD;




///////////////////////////////////////////////////////////////
//	�� �� �� : DLL_GetNode
//	�������� : ȡ���ڵ�
//	������� : ��FIFO���ҳ��ڵ㣬����ǽ��յ��������Ľڵ㡣
//	����˵�� : ��
//	�� �� ֵ : P_DATA_T�ṹ�����ʼ��ַ����ʾ�ɹ�������NULL��ʾʧ�ܡ�
///////////////////////////////////////////////////////////////
P_DATA_T DLL_GetNode();





///////////////////////////////////////////////////////////////
//	�� �� �� : BuildNetHead
//	�������� : ����һ�����ݰ��İ�ͷ
//	������� : 
//	����˵�� :  uint16 PacketID;				// �����ID��0~65535
//				uint8 DirFlag;					// �����־�����Ͱ�-0xAA Ӧ���-0x00
//				uint8 NetCmd;				    // ��������
//				NET_ECHO_VALUE EchoValue;	// Ӧ��ֵ,��DirFlag==0x00ʱ��Ҫ����
//				uint16 DataLen;				// ���ݳ���
//	�� �� ֵ : P_NET_HEAD ��ͷ��ʧ�ܷ��ؿ�ָ��
///////////////////////////////////////////////////////////////
P_NET_HEAD BuildNetHead(uint16 PacketID, uint8 DirFlag, uint8 NetCmd, NET_ECHO_VALUE EchoValue, uint16 DataLen);






///////////////////////////////////////////////////////////////
//	�� �� �� : sendSuccessMsg
//	�������� : ���������ݰ���д��ָ����ַ��
//	������� : 
//	����˵�� :	unsigned int unClientIPaddr:  P_DATA_T�ڵĿͻ��˵�ַ.
//				uint16 PacketID��			P_DATA_T�ڵ�recvbuf�ڵİ�ID.	
//	�� �� ֵ : 0 ��ʾ�ɹ�����0��ʾʧ�ܡ�
///////////////////////////////////////////////////////////////
int sendSuccessMsg(unsigned int unClientIPaddr , uint16 PacketID,  uint8 NetCmd);



///////////////////////////////////////////////////////////////
//	�� �� �� : BLL_DuteFunc
//	�������� : ҵ����
//	������� : ȡ���ڵ㣬�жϹ������ݡ��󲿷���Ҫ����һ�����ݰ��İ�ͷ�����������ݰ���д��ָ����ַ��
//	����˵�� : ��
//	�� �� ֵ : 0 ��ʾ�ɹ�����0��ʾʧ�ܡ�
///////////////////////////////////////////////////////////////
unsigned int __stdcall BLL_DuteFunc(void *);


#endif //_BLL_DUTE_H_