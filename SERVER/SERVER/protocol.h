#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_




#define PACKET_DEFAULT			(0x01)
#define PACKET_REG				(0x02)


#define PACKET_CANCEL			(0x03)
#define PACKET_LOGIN			(0x04)

#define PACKET_EXIT				(0x05)

#define PACKET_UPDATE_ONE		(0x06)
#define PACKET_UPDATE_LIST		(0x07)

#define PACKET_HANDS			(0x08)

#define PACKET_VOICE_SEND		(0x09)
#define PACKET_VOICE_DOWN		(0x10)

#define PACKET_TEXT_SEND		(0x11)
#define PACKET_TEXT_DOWN		(0x12)


#define PACKET_SUCCESS			(0x13)
#define PACKET_FAILED			(0x14)



//1.1.	数据头格式

typedef struct tagPACKET_HEAD_T
{
	int				nProtocolNo;
	int				cCommand;
	short int		nPktAmount;
	short int		nPktNo;
	unsigned int	nCheckNo;
	int				nPktBodyLenth;

}PACKET_HEAD_T,*P_PACKET_HEAD_T;

//1.2 基本数据格式

//说明：数据文件或着数据包可以由多个以下表格定义的数据包连接组成。

typedef struct tagPACKET_T
{
	PACKET_HEAD_T stPackHead;
	//PACKET_BODY_UN unPackBody;

}PACKET_T,*P_PACKET_T;



#endif //_PROTOCOL_H_










