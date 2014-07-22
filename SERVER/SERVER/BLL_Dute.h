/******************************************************************************
* 版权所有 (C)2014, weibo.com @zhudaiwang 
* 文件名称：
*    BLL_Dute.h
* 当前版本：
*     1.0
* 内容摘要：
*     业务层，业务处理模块。
* 历史纪录：
*     修改人		日期		版本号		        描述
*     朱代旺      2014.4.10       1.0
*******************************************************************************/

#ifndef _BLL_DUTE_H_
#define _BLL_DUTE_H_

//20140411 问题出现          //ans 因为 uint8  uint16
#include "DATA.h"


#define ECHO_OK			0
#define ECHO_ERROR		1
#define ECHO_TIMEOUT	2


//应答值定义：
// typedef enum
// {
// 	ECHO_OK,					// 应答成功
// 	ECHO_ERROR,					// 应答失败
// 	ECHO_TIMEOUT				// 应答超时
// }NET_ECHO_VALUE;


typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned char NET_ECHO_VALUE;

// （表示方式说明：如”秒（1B：0~59）”, 1B表示占用1个字节，取值范围是0至59。）
// 
// 	网络包（发送包和应答包）的格式：网络包头(7Btyes)+数据(N Bytes)
// 
// 	网络包头定义：

#pragma pack (1) /*指定按1字节对齐*/      
typedef struct tagNET_HEAD
{
	uint16 PacketID;				// 网络包ID，0~65535
	uint8 DirFlag;					// 方向标志：发送包-0xAA 应答包-0x00
	uint8 NetCmd;				    // 网络命令
	NET_ECHO_VALUE EchoValue;	// 应答值,在DirFlag==0x00时需要设置
	uint16 DataLen;				// 数据长度
}NET_HEAD, *P_NET_HEAD;




///////////////////////////////////////////////////////////////
//	函 数 名 : DLL_GetNode
//	函数功能 : 取出节点
//	处理过程 : 从FIFO中找出节点，这个是接收到的完整的节点。
//	参数说明 : 无
//	返 回 值 : P_DATA_T结构体的起始地址，表示成功，返回NULL表示失败。
///////////////////////////////////////////////////////////////
P_DATA_T DLL_GetNode();





///////////////////////////////////////////////////////////////
//	函 数 名 : BuildNetHead
//	函数功能 : 生成一个数据包的包头
//	处理过程 : 
//	参数说明 :  uint16 PacketID;				// 网络包ID，0~65535
//				uint8 DirFlag;					// 方向标志：发送包-0xAA 应答包-0x00
//				uint8 NetCmd;				    // 网络命令
//				NET_ECHO_VALUE EchoValue;	// 应答值,在DirFlag==0x00时需要设置
//				uint16 DataLen;				// 数据长度
//	返 回 值 : P_NET_HEAD 包头，失败返回空指针
///////////////////////////////////////////////////////////////
P_NET_HEAD BuildNetHead(uint16 PacketID, uint8 DirFlag, uint8 NetCmd, NET_ECHO_VALUE EchoValue, uint16 DataLen);






///////////////////////////////////////////////////////////////
//	函 数 名 : sendSuccessMsg
//	函数功能 : “发送数据包”写入指定地址。
//	处理过程 : 
//	参数说明 :	unsigned int unClientIPaddr:  P_DATA_T内的客户端地址.
//				uint16 PacketID：			P_DATA_T内的recvbuf内的包ID.	
//	返 回 值 : 0 表示成功，非0表示失败。
///////////////////////////////////////////////////////////////
int sendSuccessMsg(unsigned int unClientIPaddr , uint16 PacketID,  uint8 NetCmd);



///////////////////////////////////////////////////////////////
//	函 数 名 : BLL_DuteFunc
//	函数功能 : 业务处理。
//	处理过程 : 取出节点，判断工作内容。大部分需要生成一个数据包的包头，“发送数据包”写入指定地址。
//	参数说明 : 无
//	返 回 值 : 0 表示成功，非0表示失败。
///////////////////////////////////////////////////////////////
unsigned int __stdcall BLL_DuteFunc(void *);


#endif //_BLL_DUTE_H_