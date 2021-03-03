
#ifndef __STRGVECT_H
#define __STRGVECT_H

#include	"vect.h"
#include	"fsf1font.h"
#include	"wnttfont.h"
#include	"esf0font.h"
#include	"esf1font.h"

#include	"mojivect.h"

typedef struct {
	DBL	dSX, dSY;	// 文字列開始位置
	DBL	dW, dH;		// 文字サイズ
	DBL	dSL;		// 文字列長さ
	DBL	dM, dL;		// 文字間隔　行間隔
	DBL	dXLen, dYLen;	// 文字データのザイズ
	int	hcnt, zcnt;	// 半角文字の数　全角文字の数
	int	lasthan;	// 最後の文字が半角のとき
	DBL	sx, sy;
	DBL	space;
	DBL	width, height;
} SetingElementType;

class AFX_CLASS_EXPORT CStrgVect : public CVect
{
protected:
	CStrgVect();
	DECLARE_SERIAL(CStrgVect)
public:
	virtual void Serialize(CArchive& ar);

public:
	char		m_sFontName[FACEMAX];
	BYTE		m_nStrgFlag;
	BYTE		m_nYokoGumi;
	BYTE		m_nFlag3;
	BYTE		m_nFlag4;
	char		m_sString[STRGMAX];
	DBL			m_dCharCount;
	CMojiVect*	m_pcMojiTop;
	CRect		m_rectStrg;
	DBL			m_dTrns;	
	int		m_nShadowKind;		// 
	COLORREF	m_clrShadow;	// 影の色
	DBL		m_dShadowDistance;
	DBL		m_dShadowRoundRag;		// 影の位置

	int			m_nFontType;
public:
	virtual void ChangeFont(LPSTR sFname);
	virtual void SetUseClrPenno(int notbl[], LPINT cnt);
	int CheckOneMojiTrns();
	virtual void ReverceRejion();
	void SaveEpsFile();
	virtual void DeleteData(CData * delData);
	virtual int CheckNullData();
	virtual CData *SrchNearOnLinePoint(DBL xx, DBL yy, DBL *retlen, DBL *retx, DBL *rety, int *retpp);
	virtual CData* SrchNearPoint(DBL xx, DBL yy, DBL *retlen, int *retpp);
	void SetData(CMojiVect* pcMojiVect);
	virtual void GetSpaceMojiCount(LPINT zen, LPINT han);
	virtual void OnDraw(CDC *pcDC);
	virtual void OutlineDraw(CDC *pcDC);
	virtual void PointPicup();
	virtual void AllOffPointFlag(int bitflag);
	virtual void AllOnPointFlag(int bitflag);
	virtual void PointXLegal(DBL xx);
	virtual void PointYLegal(DBL yy);
	virtual CData * CopyData(DBL movex, DBL movey);
	void SetDataList(CDataList *pcDataList);
	virtual CData* SrchData(DBL dx, DBL dy, DBL *retlen);
	virtual CData* SrchDataBoxs(DBL dx, DBL dy, DBL *retlen);
	virtual int CheckID();
	virtual void WakuSave(FILE *fp);
	CStrgVect(int nDummy);
	~CStrgVect();
	int SetMojiLink(CMojiVect *pcSetMoji);
	virtual CRect* GetBoundingRect();
	virtual	void SetMaxCharCount(DBL cnt);
	virtual DBL	GetMaxCharCount();
	virtual void SetYokoMojiFlag(int flag);
	virtual int CheckYokoMojiFlag();
	virtual void SetSpaceKind(int flag);
	virtual int CheckSpaceKind();
	virtual void SetTateYoko(int flag);
	virtual int CheckTateYoko();
	virtual void SetFontName(LPSTR sFname);
	virtual void GetFontName(LPSTR sFname);
	virtual void SetFontSize(DBL dFWsize, DBL dFHsize);
	virtual void GetFontSize(DBL *dFWsize, DBL *dFHsize);
	virtual void SetSpaceEnv(DBL dMSpace, DBL dLSpace);
	virtual void GetSpaceEnv(DBL *dMSpace, DBL *dLSpace);
	virtual void SetStrStartXY(DBL dSx, DBL dSy);
	virtual void GetStrStartXY(DBL *dSx, DBL *dSy);
	virtual void SetStrLen(DBL dStrLen);
	virtual void GetStrLen(DBL *dStrLen);
	virtual void GetTrnsPar(LPDBL par);
	virtual void SetTrnsPar(DBL par);
	virtual void AllFree();
	virtual void AllMojiFree();
	virtual CData *GetIdxMoji(int nidx);
	virtual void CheckFlagDraw(int flag, CDC *pcDC);
	virtual void CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick);
	virtual void AtntionDraw(CDC *pcDC);
	virtual void AtntionMojiDraw(CDC *pcDC, int nidx);
	virtual void NoactDraw(CDC *pcDC);
	virtual void DelDraw(CDC *pcDC);
	virtual void SetString(char *pstr);
	virtual void GetString(char *pstr);
	virtual void SetStringUnLoad(char *pstr);
	virtual void PlotOut(int nID);
	virtual int GetBeforeMojiIdx(int mojiidx);
	virtual int GetAfterMojiIdx(int mojiidx);
	virtual void MoveAfterMoji(int mojiidx, DBL movex, DBL movey);
	virtual void GetMojiWakuMiniMax(int mojiidx, DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void GetMojiMiniMax(int mojiidx, DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual CData* SrchMoji(DBL x, DBL y, DBL *retlen, int *retmidx);
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void GetDataDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void GetWakuDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void GetDataSumLen(DBL *xsumlen, DBL *ysumlen);
	virtual void GetWakuSumLen(DBL *xsumlen, DBL *ysumlen);
	virtual void RenewMiniMax();
	virtual void MoveTrns(DBL movex, DBL movey);
	virtual void IdxMoveTrns(int idx, DBL movex, DBL movey);
	virtual void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual void IdxSizeTrns(int idx, DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual void SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy);
	virtual void MirrorTrns(DBL a, DBL b, DBL t1, DBL t2);
	virtual void RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	virtual void IdxRoundTrns(int idx, DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	virtual void StrctBoxsTrns();
	virtual void SetOutLinePen(int nPen);
	virtual void SetOutLineWidth(DBL dThick);
	virtual void SetFillPen(int nPen);
	virtual void SetDefOutLineClr();
	virtual void SetDefFillClr();

	virtual void OnFlag(int bitflag);
	virtual void OffFlag(int bitflag);
	virtual void DataChange();
	virtual void DataChangeSetDataGrup(CDataGrup *pcDataGrup);
	virtual BOOL CheckDataChange();
	virtual void SetIdxOutLinePen(int idx, int nPen);
	virtual void SetIdxFillPen(int idx, int nPen);
	virtual int GetIdxOutLinePen(int idx);
	virtual int GetIdxFillPen(int idx);
	virtual void FreeIdxObject(int idx);
	virtual int CheckIdxFlag(int idx, int flag);
	virtual void OnIdxFlag(int idx, int flag);
	virtual void OffIdxFlag(int idx, int flag);
	virtual void FlagCheckMojiChange(int flag);
	virtual void StrgFiting();
	void StringBoxsDraw(CDC *pcDC);
	void StockClrDraw(CDC *pcDC);
	void BoxsDraw(CDC *pcDC, DBL sx, DBL sy, DBL ex, DBL ey);
	int FontSelect(LPSTR sFontName, int fTateGaki);
	void StringElementInit();
private:
	void TategakiElementInitWaku();
	void TategakiElementInitData();
	void TategakiEqualGetElementWaku(SetingElementType *SET);
	void TategakiEqualGetElementData(SetingElementType *SET);
	void YokogakiElementInitWaku();
	void YokogakiElementInitData();
	void YokogakiEqualGetElementWaku(SetingElementType *SET);
	void YokogakiEqualGetElementData(SetingElementType *SET);
	void YokoMojiElementInitWaku(SetingElementType *SrcSET, CMojiVect *pcSMojiVect, CMojiVect *pcEMojiVect);
	void YokoMojiElementInitData(SetingElementType *SrcSET, CMojiVect *pcSMojiVect, CMojiVect *pcEMojiVect);
	void StrgDataSumLenGet(DBL width, DBL hight, DBL *xlen, DBL *ylen);
	void AreaDataSumLenGet(CMojiVect *pcSMojiVect, CMojiVect *pcEMojiVect, DBL width, DBL hight, DBL *xlen, DBL *ylen);
	void GetCharCount(int *hcnt, int *zcnt);
	void GetAreaCharCount(CMojiVect *pcSMojiVect, CMojiVect *pcEMojiVect, int *hcnt, int *zcnt);
	BOOL IsLastHankaku();
	BOOL IsAreaLastHankaku(CMojiVect *pcSMoji, CMojiVect *pcEMoji);
};

#endif

