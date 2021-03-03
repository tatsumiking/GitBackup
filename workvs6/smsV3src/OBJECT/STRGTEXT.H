#ifndef __STRGTEXT_H
#define __STRGTEXT_H

#include	"datalist.h"

#include	"mojitext.h"

class AFX_CLASS_EXPORT CStrgText : public CVect
{
protected:
	CStrgText();
	DECLARE_SERIAL(CStrgText)
public:
	CStrgText(int nDummy);
	~CStrgText();
	virtual void Serialize(CArchive& ar);

public:
	CTypedPtrList<CObList, CMojiText*> m_cMojiTextList;	//リスト構造
	int		m_nStrgFlag;	//横書きYOKOGAKI 縦書きTATEGAKI 縦置きTATEOKI 横置きYOKOOKI　
	DBL		m_dStrlen;
	DBL		m_dLineSpace;
	// 文字を生成する際の標準状態
	char	m_szFontName[FACEMAX];	
	DBL		m_dSX, m_dSY;	//位置情報
	DBL		m_dSize;		//文字サイズ（横書き高さ、縦書き横幅）
	DBL		m_dSpacePar;	//文字間隔
	DBL		m_dFlatPar;		//平体率
	int		m_nBold;		// 太字 ON OFF
	int		m_nSlope;		// 斜形 ON OFF
	// 文字サイズを持たないデータがかな縮小でどんどん小さくなるバグ回避のため	
	DBL		m_dTKtknPar;
	DBL		m_dTHrgnPar;

public:
	virtual void ChangeFontIdx(LPSTR sFname, int idx);
	virtual void ChangeFont(LPSTR sFname);
	virtual void SetUseClrPenno(int notbl[], LPINT cnt);
	virtual int CheckID();
	virtual int CheckNullData();
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
	virtual void SetLineSpace(DBL space);
	virtual void GetLineSpace(LPDBL space);
	virtual void SetStartXY(DBL sx, DBL sy);
	virtual void GetStartXY(LPDBL sx, LPDBL sy);
	virtual void SetSize(DBL size);
	virtual void GetSize(LPDBL size);
	virtual void SetSpacePar(DBL par);
	virtual void GetSpacePar(LPDBL par);
	virtual void SetFlatPar(DBL par);
	virtual void GetFlatPar(LPDBL par);
	virtual void GetBold(LPINT flag);
	virtual void SetBold(int flag);
	virtual void GetSlope(LPINT flag);
	virtual void SetSlope(int flag);
	virtual void AllFree();
	virtual CData *GetIdxMoji(int nidx);
	virtual void CheckFlagDraw(int flag, CDC *pcDC);
	virtual void CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick);
	virtual void AtntionDraw(CDC *pcDC);
	virtual void AtntionMojiDraw(CDC *pcDC, int nidx);
	virtual void NoactDraw(CDC *pcDC);
	virtual void DelDraw(CDC *pcDC);
	virtual void OnDraw(CDC *pcDC);
	virtual void StockClrDraw(CDC *pcDC);
	virtual void OutlineDraw(CDC *pcDC);
	virtual void SetString(char *pstr);
	virtual void GetString(char *pstr);
	virtual int GetBeforeMojiIdx(int mojiidx);
	virtual int GetAfterMojiIdx(int mojiidx);
	virtual void MoveAfterMoji(int mojiidx, DBL movex, DBL movey);
	virtual void GetMojiWakuMiniMax(int mojiidx, DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void GetMojiMiniMax(int mojiidx, DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void GetDataDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void GetWakuDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual DBL GetDataSumLength();
	virtual DBL GetWakuSumLength();
	virtual void RenewMiniMax();
	virtual CData * CopyData(DBL movex, DBL movey);
	virtual void CopyDataElement(CStrgText *pcnewData, DBL movex, DBL movey);
	virtual void MoveTrns(DBL movex, DBL movey);
	virtual void IdxMoveTrns(int idx, DBL movex, DBL movey);
	virtual void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual void IdxSizeTrns(int idx, DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual void SlopeTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy);
	virtual void MirrorTrns(DBL a, DBL b, DBL t1, DBL t2);
	virtual void RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	virtual void IdxRoundTrns(int idx, DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	virtual void OnFlag(int bitflag);
	virtual void OffFlag(int bitflag);
	virtual void DataChange();
	virtual void DataChangeSetDataGrup(CDataGrup *pcDataGrup);
	virtual BOOL CheckDataChange();
	virtual void FreeIdxObject(int idx);
	virtual int CheckIdxFlag(int idx, int flag);
	virtual void OnIdxFlag(int idx, int flag);
	virtual void OffIdxFlag(int idx, int flag);
	virtual void BoxsDraw(CDC *pcDC, DBL sx, DBL sy, DBL ex, DBL ey);
	virtual CData* SrchData(DBL dx, DBL dy, DBL *retlen);
	virtual CData* SrchDataBoxs(DBL dx, DBL dy, DBL *retlen);
	virtual CData* SrchNearPoint(DBL xx, DBL yy, DBL *retlen, int *retpp);
	virtual CData* SrchNearOnLinePoint(DBL xx, DBL yy, DBL * retlen, DBL * retx, DBL * rety, int * retpp);
	virtual CData* SrchMoji(DBL x, DBL y, DBL *retlen, int *retmidx);
	virtual void SetDefFillClr();
	virtual void DeleteData(CData * delData);
	virtual void StrgFiting();
	void GetSpaceMojiCount(LPINT retzen, LPINT rethan);
	void GetCharCount(LPINT retzen, LPINT rethan);
	void StringElementInit();

private:
	void TategakiElementInit();
	void YokogakiElementInit();
	void StrgDataSumLenGet(DBL width, DBL hight, DBL *xlen, DBL *ylen);
};

#endif
