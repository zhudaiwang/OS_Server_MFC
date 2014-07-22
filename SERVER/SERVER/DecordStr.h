#pragma once



#define NKBNO (8)


#define ITEM_NAME		(0)
#define ITEM_BARCODE	(1)
#define ITEM_COUNT		(2)
#define ITEM_PRICE		(3)


extern int nOutStrIndex;


class DecordStr
{
public:
	char * acnOutNo;
	int nOutStrIndex;



public:
	DecordStr();
	DecordStr( char *pcOrigCode);
	~DecordStr(void);

	//////////////////////////
	//		��ʼ���ӿ�
	//////////////////////////
	char *Code2Str( char *pcOrigCode);


	//////////////////////////
	//		������ļ�
	//////////////////////////
	void OutToFile();




	//////////////////////////
	//		СƱ������Ϣ
	//////////////////////////

	char *getThisDealNo();


	char *getThisDealNo_ForXINHUADU();

	char *getThisCashierNo();


	int getDealGoodsKinds();				//��Ʒ����Ŀ

	char *getDealTotalNum();

	char *getDealTotalPrice();

	char *getRecvedMoney();

	char *getBackMoney();

	char *getDealTime();



	//////////////////////////
	//		СƱĳ��Ʒ������Ϣ
	//////////////////////////



	char *getGoodsName(unsigned int nItemNo );

	char* getOneGoodsCount(unsigned int nItemNo  );

	char* getGoodsUnitPrice(unsigned int nItemNo );

	char* getGoodsTotalPrice(unsigned int nItemNo );

	////////////////////////////////////
	//		�ײ�ӿ�
	////////////////////////////////

private:

	char * getPreStrAddr(char *pcOutStr, char *pcKeyWords );

	char * getAfterStrAddr(char *pcOutStr, char *pcKeyWords );

	char *getDealEndAddr();

	char* GetAnyThing(char *StrAddrStart, int nOffSetAddr, int nLenth, int nBufferLenth);

	char * getItemNoAddr(unsigned int nItemNo );

	char* getGoodsAddr();

	char* getGoodsAddrEnd();






};

