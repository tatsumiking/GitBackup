#ifndef EXTMODE_H_
#define EXTMODE_H_

#pragma pack(2)

//文字列長定義
#define KEYLENGTH               24              // WORDバウンダリを推奨
#define PAPERSTRLEN             32              // caution!! max char 64 bytes.
#define PRODUCT_LENGTH  32

//ﾄﾞﾗｲﾊﾞﾀｲﾌﾟ定義
#define DT_PASSTHROUGH  0               // ﾊﾟｽｽﾙｰﾄﾞﾗｲﾊﾞ(専用ｱﾌﾟﾘ, ﾍﾞｸﾀ)
#define DT_RASTER               1       // 汎用ﾗｽﾀﾄﾞﾗｲﾊﾞ
#define DT_VECTOR               2       // 汎用ﾍﾞｸﾀﾄﾞﾗｲﾊﾞ

//用紙ﾘｽﾄ数定義
#define DRV_PAPERNUM    6                               //ﾄﾞﾗｲﾊﾞ保有用紙ﾘｽﾄ数
#define DRV_USERNUM             8                               //ﾕｰｻﾞ定義用紙ﾘｽﾄ数
#define APP_PAPERNUM    1                                       //ｱﾌﾟﾘ設定用紙ﾘｽﾄ数
#define EXPAPERLISTNUM  DRV_PAPERNUM + DRV_USERNUM + APP_PAPERNUM

typedef struct{
        TCHAR   Name[PAPERSTRLEN];
        short   Resource;
        SIZEL   Phys;
}PAPERLISTL,*LPPAPERLISTL;

typedef struct{
        CHAR    key[KEYLENGTH];                                 //ｷｰ文字列"MAX CO.,LTD"
        CHAR    szProductName[PRODUCT_LENGTH];  //製品名 Ex)MAX RP-950T
        WORD    nDriverType;                                    //ﾄﾞﾗｲﾊﾞ形式(DT_PASSTHROUGH,DT_RASTER)
        WORD    extFields;                                              //ﾊﾟﾗﾒｰﾀ指定ﾌﾗｸﾞ(ｱﾌﾟﾘ専用)
        WORD    extFAns;                                                //ﾊﾟﾗﾒｰﾀ指定ﾌﾗｸﾞ(ﾄﾞﾗｲﾊﾞ専用)
        PAPERLISTL      AppPaperList;                           //ﾌﾟﾘﾝﾀ設定ﾀﾞｲｱﾛｸﾞでのﾕｰｻﾞ設定用紙ｻｲｽﾞﾘｽﾄ
        PAPERLISTL      InitPaperList;                          //ｱﾌﾟﾘｹｰｼｮﾝ設定ﾚｲｱｳﾄﾘｽﾄ
        PAPERLISTL      DrvPaperList[EXPAPERLISTNUM];   //ﾄﾞﾗｲﾊﾞ保有用紙ﾘｽﾄ
        WORD    nReserved1;                                             //予約
        WORD    nReserved2;                                             //予約
}_EXTDMODE, FAR *LPEXTDMODE;

typedef	struct	_papersizelist{
	TCHAR	Name[64];
	SIZEL	Phys;
	WORD	nID;
}PAPERSIZELIST,*LPPAPERSIZELIST;

typedef	struct	_papersourcelist{
	TCHAR	Name[24];
	WORD	nID;
}PAPERSOURCELIST,*LPPAPERSOURCELIST;


#pragma pack()

#endif
