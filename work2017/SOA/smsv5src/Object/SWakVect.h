
#pragma once

#include "Data.h"
#include "Vect.h"
#include "StrgVect.h"

class AFX_CLASS_EXPORT CSWakVect : public CStrgVect
{
protected:
	DBL GetTableDataSize(CMojiVect *pcMojiTbl[], int nCntMax);
	CSWakVect();
	DECLARE_SERIAL(CSWakVect)
public:
	virtual void Serialize(CArchive& ar);

public:
	int		m_nWakuNo;
	CVect	*m_pcFitingVect;// 線上配置基準線
	int		m_nSpaceKind;	// 文字枠(SPACEWAKU), データ(SPACEDATA), プロポーショナル(SPACEPROP)
	DBL		m_dFlatPar;		// 長体、平体（文字サイズに対する割合）
	DBL		m_dSpacePar;	// 文字間隔
	DBL		m_dKanaPar;		// ひらがな縮小パーセント
	DBL		m_dThickPar;	// 太字パーセント（文字サイズに対する割合）
	DBL		m_dSlopRag;		// 斜形角度
	int		m_nOder;		// 均等(ODEREQUAL)、中央(ODERCNTR)、左上(ODERLEFT)、右下(ODERRIGHT)
	int		m_nNouse;
	int		m_nFixFlag;
	int		m_nMirror;
	TCHAR	m_szFildName[RECMAX];	// Ver3で追加
public:
	CSWakVect(int nDummy);
	~CSWakVect();
public:
	int GetMojiCount();
	void GetFildName(LPTSTR lpFildName);
	void SetFildName(LPTSTR lpFildName);
	DBL GetWakuSumLength();
	DBL GetDataSumLength();
	DBL GetBody(CMojiVect *pcMojiVect);
	DBL GetSpace(CMojiVect *pcMojiVect);
	DBL GetBody();
	DBL GetSumSpace();
	void StrgSlopTrns();
	DBL GetMiniSpace(int pretbl[], int posttbl[]);
	DBL GetPropStrglen();
	virtual CData* SrchData(DBL dx, DBL dy, DBL *retlen);
	virtual CData* SrchDataBoxs(DBL dx, DBL dy, DBL * retlen);
	virtual void DeleteData(CData * delData);
	virtual int CheckNullData();
	void CopyString(CSWakVect *pcnewVect, DBL movex, DBL movey);
	virtual void SaveStrgStoreFile(FILE *fp, int id);
	virtual void LoadStrgStoreFile(FILE *fp, int id);
	virtual int CheckID();
	virtual void AllOffPointFlag(int bitflag);
	virtual void AllOnPointFlag(int bitflag);
	virtual void PointXLegal(DBL xx);
	virtual void PointYLegal(DBL yy);
	virtual CData* CopyData(DBL movex, DBL movey);
	virtual void AllFree();
	virtual void OnDraw(CDC *pcDC);
	virtual void NoSelectDraw(CDC *pcDC);
	virtual void SelectDraw(CDC *pcDC);
	void WakuBoxsDraw(CDC *pcDC);
	virtual void NoSelectDraw(CDC *pcDC, int no);
	virtual void SelectDraw(CDC *pcDC, int no);
	virtual int DrawNo(CDC *pcDC, int noidx);
	virtual void OutlineDraw(CDC *pcDC);
	virtual void AtntionDraw(CDC *pcDC);
	virtual void NoactDraw(CDC *pcDC);
	virtual void DelDraw(CDC *pcDC);
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void GetStringDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void RenewMiniMax();
	virtual void MoveTrns(DBL movex, DBL movey);
	virtual void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual void SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy);
	virtual void MirrorTrns(DBL a, DBL b, DBL t1, DBL t2);
	virtual void RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	virtual void StrctBoxsTrns();
	CData* SrchNearPoint(DBL xx, DBL yy, DBL *retlen, int *retpp);
	virtual CVect* GetFitingVect();
	virtual void WakuSave(FILE *fp);
	virtual void WakuLoad(FILE *fp, LPTSTR buf);
	virtual void WakuCnvLoad(FILE *fp, LPTSTR buf);
	void SetFitingVect(CVect* pcVect);
	void GetKanaPar(LPDBL dpar);
	void SetKanaPar(DBL dpar);
	void GetSpacePar(LPDBL dspace);
	void SetSpacePar(DBL dspace);
	void GetFlatPar(LPDBL dpar);
	void SetFlatPar(DBL dpar);
	void GetThickPar(LPDBL dpar);
	void SetThickPar(DBL dpar);
	void GetSlopRag(LPDBL drag);
	void SetSlopRag(DBL drag);
	void GetOderKind(int *nkind);
	void SetOderKind(int nkind);
	void GetSpaceKind(int *nkind);
	void SetSpaceKind(int nkind);
	void SetString(TCHAR *pstr);
	void StringElementInit();
	virtual void StrgFiting();
	void StrgLineStruct();
	void GetFitingBoxs(LPDBL sx, LPDBL sy, LPDBL ex, LPDBL ey);
private:
	void SetLenStrgLineStructLeftData();
	void SetLenStrgLineStructLeftWaku();
	void SetLenStrgLineStructCntrData();
	void SetLenStrgLineStructCntrWaku();
	void SetLenStrgLineStructRightData();
	void SetLenStrgLineStructRightWaku();
	void SetLenStrgLineStructEqualData();
	void SetLenStrgLineStructEqualWaku();
	void GetTopLastSpace(LPDBL top, LPDBL last);
	void StrgLineStructTrns();
	void YokoGumiStructTrnsWaku(CMojiVect *pcMojiTbl[], int nCnt);
	void YokoGumiStructTrnsData(CMojiVect *pcMojiTbl[], int nCnt);

	DBL GetVectLength();
	DBL GetRejionLength();
	DBL GetNorejionLength();
	DBL GetBezeLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	DBL GetArcLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetFreeCurveLength(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetLineLength(DBL x1, DBL y1, DBL x2, DBL y2);

	DBL GetVectPointLength(DBL xx, DBL yy);
	DBL GetRejionPointLength(int *flag, DBL xx, DBL yy);
	DBL GetNorejionPointLength(int *flag, DBL xx, DBL yy);
	DBL GetBezePointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	DBL GetArcPointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetFreeCurvePointLength(int *flag, DBL xx, DBL yy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetLinePointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2);

	DBL GetVectPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy);
	DBL GetRejionPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy);
	DBL GetNorejionPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy);
	DBL GetBezePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	DBL GetArcPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetFreeCurvePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetLinePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2);

private:
	DBL	m_dLinelen;
	DBL	m_dLastX1;
	DBL	m_dLastY1;
	DBL	m_dLastX2;
	DBL	m_dLastY2;
};

