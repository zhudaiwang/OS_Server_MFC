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
	//		初始化接口
	//////////////////////////
	char *Code2Str( char *pcOrigCode);


	//////////////////////////
	//		输出到文件
	//////////////////////////
	void OutToFile();




	//////////////////////////
	//		小票基本信息
	//////////////////////////

	char *getThisDealNo();


	char *getThisDealNo_ForXINHUADU();

	char *getThisCashierNo();


	int getDealGoodsKinds();				//商品总条目

	char *getDealTotalNum();

	char *getDealTotalPrice();

	char *getRecvedMoney();

	char *getBackMoney();

	char *getDealTime();



	//////////////////////////
	//		小票某商品基本信息
	//////////////////////////



	char *getGoodsName(unsigned int nItemNo );

	char* getOneGoodsCount(unsigned int nItemNo  );

	char* getGoodsUnitPrice(unsigned int nItemNo );

	char* getGoodsTotalPrice(unsigned int nItemNo );

	////////////////////////////////////
	//		底层接口
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

