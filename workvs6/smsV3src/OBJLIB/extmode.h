#ifndef EXTMODE_H_
#define EXTMODE_H_

#pragma pack(2)

//�����񒷒�`
#define KEYLENGTH               24              // WORD�o�E���_���𐄏�
#define PAPERSTRLEN             32              // caution!! max char 64 bytes.
#define PRODUCT_LENGTH  32

//��ײ�����ߒ�`
#define DT_PASSTHROUGH  0               // �߽�ٰ��ײ��(��p����, �޸�)
#define DT_RASTER               1       // �ėp׽���ײ��
#define DT_VECTOR               2       // �ėp�޸���ײ��

//�p��ؽĐ���`
#define DRV_PAPERNUM    6                               //��ײ�ޕۗL�p��ؽĐ�
#define DRV_USERNUM             8                               //հ�ޒ�`�p��ؽĐ�
#define APP_PAPERNUM    1                                       //���ؐݒ�p��ؽĐ�
#define EXPAPERLISTNUM  DRV_PAPERNUM + DRV_USERNUM + APP_PAPERNUM

typedef struct{
        TCHAR   Name[PAPERSTRLEN];
        short   Resource;
        SIZEL   Phys;
}PAPERLISTL,*LPPAPERLISTL;

typedef struct{
        CHAR    key[KEYLENGTH];                                 //��������"MAX CO.,LTD"
        CHAR    szProductName[PRODUCT_LENGTH];  //���i�� Ex)MAX RP-950T
        WORD    nDriverType;                                    //��ײ�ތ`��(DT_PASSTHROUGH,DT_RASTER)
        WORD    extFields;                                              //���Ұ��w���׸�(���ؐ�p)
        WORD    extFAns;                                                //���Ұ��w���׸�(��ײ�ސ�p)
        PAPERLISTL      AppPaperList;                           //������ݒ��޲�۸ނł�հ�ސݒ�p������ؽ�
        PAPERLISTL      InitPaperList;                          //���ع���ݐݒ�ڲ���ؽ�
        PAPERLISTL      DrvPaperList[EXPAPERLISTNUM];   //��ײ�ޕۗL�p��ؽ�
        WORD    nReserved1;                                             //�\��
        WORD    nReserved2;                                             //�\��
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
