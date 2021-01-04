
#pragma once

// 内部使用予定クラス宣言

#include "Plist.h"
#include "Data.h"

class AFX_CLASS_EXPORT CVect : public CData
{
private:
	CPList*	m_pPListTop;
public:
	int		m_nPointCount;
	int		m_nFillPen;
	int		m_nOutLinePen;
	DBL		m_dOutLineWidth;
public:

protected:
	CVect();
	DECLARE_SERIAL(CVect)
public:
	virtual void Serialize(CArchive& ar);
public:
	CVect(int nDummy);
	~CVect();
public:
	virtual int CheckNullData();
	virtual BOOL IsVectObject();
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void RenewMiniMax();
	virtual void MoveTrns(DBL movex, DBL movey);
	virtual void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual void SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy);
	virtual void MirrorTrns(DBL a, DBL b, DBL t1, DBL t2);
	virtual void RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	virtual void StrctBoxsTrns();
	virtual void MtrxTrns();
	virtual void ReverceRejion();
	virtual CData *SrchNearOnLinePoint(DBL xx, DBL yy, DBL *retlen, DBL *retx, DBL *rety, int *retpp);
	virtual CData *SrchNearPoint(DBL xx, DBL yy, DBL *retlen, int *retpp);
	virtual void AtntionDraw(CDC *pcDC);
	virtual void NoactDraw(CDC *pcDC);
	virtual void DelDraw(CDC *pcDC);
	virtual void PLDraw(CDC *pcDC);
	virtual void PlotOut(int nComID);
	virtual void GdiplusFillDraw(CGdiPlusEx *pcGpe);
	virtual void GdiplusPlgnDraw(CGdiPlusEx *pcGpe);
	virtual void GdiplusPointDraw(CGdiPlusEx *pcGpe, int atr, DBL dx, DBL dy);
	virtual void FillDraw(CDC *pcDC);
	virtual void PlgnDraw(CDC *pcDC);
	virtual void PointAreaDraw(CDC *pcDC, int pp1, int pp2);
	virtual void AreaFiguDraw(CDC *pcDC, int pp1, int pp2);
	virtual void PointDraw(CDC *pcDC, int pp);
	virtual int OneFiguDraw(CDC *pcDC, int pp);
public:

private:
	void GetNearLine(DBL xx, DBL yy, int p1, int p2, DBL *retlen, DBL *retx, DBL *rety, int *retpp);
	void GetNearBeze(DBL xx, DBL yy, int p1, int p2, int p3, int p4, DBL *retlen, DBL *retx, DBL *rety, int *retpp);
	void GetNearArc(DBL xx, DBL yy, int p1, int p2, int p3, DBL *retlen, DBL *retx, DBL *rety, int *retpp);
	void GetNearCurve(DBL xx, DBL yy, int p0, int p1, int p2, int p3, DBL *retlen, DBL *retx, DBL *rety, int *retpp);
	int GetNearOnBezePoint(DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4, DBL *retlen, DBL *retx, DBL *rety);
	int GetNearOnEnkoPoint(DBL xx, DBL yy, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL dr, DBL rag01, DBL rag02, DBL *retlen, DBL *retx, DBL *rety);
	int GetNearPoint(DBL x, DBL y);

	void RejionPlotOut(int nComID);
	void RejionPlotOutOmit(int nComID);
	void NoRejionPlotOut(int nComID);
	void NoRejionPlotOutOmit(int nComID);

	void FillRejionDrawOut(CDC *pcDC);
	void PointRejionDrawOut(CDC *pcDC);
	void PointNoRejionDrawOut(CDC *pcDC);
	void PlgnRejionDrawOut(CDC *pcDC);
	void PlgnNoRejionDrawOut(CDC *pcDC);

	void PlgnRejionConrDrawOut(CDC *pcDC, int nthick);
	void PlgnNoRejionConrDrawOut(CDC *pcDC, int nthick);

	int SaveEpsRejion();
	int SaveEpsNoRejion();
	int SaveDxfRejion();
	int SaveDxfNoRejion();

public:
	DBL GetLength();
	DBL GetAreaLength(int p1, int p2);
	DBL Getp1p2AreaLength(int p1, int p2);
	DBL GetRejionLength();
	DBL GetNorejionLength();
	DBL GetBezeLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	DBL GetArcLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetFreeCurveLength(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetLineLength(DBL x1, DBL y1, DBL x2, DBL y2);
	int GetFiguKind(int pp);
	int NextPointGet(int pp);
	int BeforePointGet(int pp);
	void GetFreeCurveBeze(int pp, dbezetype *bz);

public:
	virtual void CnvBezeToLine(int divcnt);
	int GetFigu(int pp, int *np, dfigutype *figu);
	void SetRoundKind();
	DBL GetSubRag(DBL dx, DBL dy, DBL x1, DBL y1, DBL x2, DBL y2);
	int SrchInArea(DBL dx, DBL dy);
	void CheckPointInAddSumRag(LPDBL srag, LPDBL erag, LPDBL sumrag);
	void CheckRag(DBL srag, DBL erag, DBL sumrag);
	BOOL CheckPointInBezeGetSumrag(DBL xx, DBL yy, int p1, int p2, int p3, int p4, LPDBL srag, LPDBL erag, LPDBL sumrag);
	BOOL CheckPointInGetSumrag(DBL xx, DBL yy, int pp, LPDBL srag, LPDBL erag, LPDBL sumrag);
	BOOL CheckPointInGetRag(DBL xx, DBL yy, int pp, LPDBL rag);
	BOOL CheckPointIn(DBL xx, DBL yy);
	virtual void OutLineOmit(DBL mm);
	virtual void SaveDxfFile();
	virtual void SaveEpsFile();
	virtual void SetDefFillClr();
	virtual void SetDefOutLineClr();
	virtual void PointPicup();
	void virtual PointXLegal(DBL xx);
	virtual void PointYLegal(DBL yy);
	virtual CData * CopyData(DBL movex, DBL movey);
	virtual void SetAtnVectFSF1fBuf(CFSF1Font *pcFSF1Font);
	int CheckID();
	virtual void InAreaDataAtntionOn(DBL sx, DBL sy, DBL ex, DBL ey);
	virtual CData* SrchData(DBL xx, DBL yy, DBL *retlen);
	virtual CData* SrchDataBoxs(DBL xx, DBL yy, DBL *retlen);
	virtual void WakuSave(FILE* fp);
	virtual int PointLegal(int nPmax, int pp);
	virtual int GetPointCount();
	virtual void SetPointCount(int nCnt);
	virtual int GetFillPen();
	virtual void SetFillPen(int npen);
	virtual void GetFillRGB(LPINT rval, LPINT gval, LPINT bval);
	virtual int GetOutLinePen();
	virtual void SetOutLinePen(int npen);
	virtual DBL GetOutLineWidth();
	virtual void SetOutLineWidth(DBL dw);
	virtual void GetOutLineRGBW(LPINT rval, LPINT gval, LPINT bval, LPINT nthick);
	virtual void AllOffPointFlag(int bit);
	virtual void AllOnPointFlag(int bit);
	virtual int CheckPointFlag(int idx, int bit);
	virtual void OnPointFlag(int idx, int bit);
	virtual void OffPointFlag(int idx, int bit);
	virtual void GetAtr(int idx, int *atr);
	virtual void SetAtr(int idx, int atr);
	virtual void GetAtrDXY(int idx, int *atr, DBL *x, DBL *y);
	virtual void SetAtrDXY(int idx, int atr, DBL x, DBL y);
	virtual void GetFAtrDXY(int idx, int *atr, DBL *x, DBL *y);
	virtual void SetFAtrDXY(int idx, int atr, DBL x, DBL y);
	virtual void GetDXY(int idx, DBL *x, DBL *y);
	virtual void SetDXY(int idx, DBL x, DBL y);
	void ThreePointCalc(DBL mm, int p0, int p1, int p2, DBL *dx, DBL *dy);
	void PointCurvecontLegal(DBL mm, int pp, DBL *retx, DBL *rety);
private:
	CPList* GetPList(int idx, int *tidx);
	void PListAddLink(int idx);
	void TreePoint(int atr, DBL mm, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL *x, DBL *y);
	void GetRoundRag(DBL x0, DBL y0, DBL x1, DBL y1, DBL *rag);
	void SetRoundPoint(DBL x0, DBL y0, DBL rag, DBL x1, DBL y1, DBL *x, DBL *y);
	void GetConerPoint(DBL x1, DBL y1, DBL rag1, DBL x2, DBL y2, DBL rag2, DBL *x, DBL *y);
	void LegalLength(int atr, double dot, double x0, double y0, double *dx, double *dy);
	void RenewMiniMaxLine(DBL x1, DBL y1, DBL x2, DBL y2);
	void RenewMiniMaxArc(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void RenewMiniMaxBeze(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
};

