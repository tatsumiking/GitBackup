
#pragma once

#include "Data.h"
#include "Vect.h"
#include "DataList.h"
#include "BoldLib.h"

class AFX_CLASS_EXPORT CMojiVect : public CVect
{
protected:
	CMojiVect();
	DECLARE_SERIAL(CMojiVect)
public:
	CDataList	*m_pcDataList;
	CRect* GetBoundingRect();
	virtual void Serialize(CArchive& ar);
	CMojiVect(int nDummy);
	CBoldLib	*m_pcBoldLib;
	~CMojiVect();

public:
	int			m_fAtrFlag;
	CMojiVect*	m_pcMojiNext;
	int			m_hFno;
	CRect		m_rectMoji;
	int			m_nZenKaku;
	int			m_nCode;
	int			m_nBaseCode;
	DBL			m_dLenFromStrgTop;

public:
	void Shadow1Draw(CDC *pcDC, COLORREF clr, int nCnt, DBL dAddX, DBL dAddY);
	void Shadow2Draw(CDC *pcDC, COLORREF clr, int nCnt, DBL dAddX, DBL dAddY);
	int CheckSpecialSpace(DBL *time);
	int IsNumCode();
	int ChangeIBMCode(int sjis);
	virtual void SetUseClrPenno(int notbl[], LPINT cnt);
	void GetDataSEP(LPDBL sp, LPDBL ep);
	void BoldTrns(DBL par);
	int CheckOneMojiTrns();
	void SaveEpsFile();
	virtual int CheckID();
	virtual int CheckNullData();
	virtual void SetNextMoji(CMojiVect *pMojiVect);
	virtual CMojiVect* GetNextMoji();
	virtual void GetWakuDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void GetDataDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual CData* SrchData(DBL x, DBL y, DBL *retlen);
	virtual CData* SrchDataBoxs(DBL x, DBL y, DBL *retlen);
	virtual CData* CopyData(DBL movex, DBL movey);
	virtual void DataChange();
	virtual void StockClrDraw(CDC *pcDC);
	virtual void CheckFlagDraw(int flag, CDC *pcDC);
	virtual void CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick);
	void DrawMojiWaku(CDC *pcDC);
	virtual void OnDraw(CDC *pcDC);
	virtual void GdiplusFillDraw(CGdiPlusEx *pcGpe);
	virtual void OutlineDraw(CDC *pcDC);
	virtual void SetOutLinePen(int nPen);
	virtual void SetOutLineWidth(DBL dThick);
	virtual void SetFillPen(int nPen);
	virtual void SetDefOutLineClr();
	virtual void SetDefFillClr();

	virtual void GetFontBoxs(DBL x[], DBL y[]);
	virtual void SetFontSize(DBL dFWsize, DBL dFHsize);
	virtual void GetFontSize(DBL *dFWsize, DBL *dFHsize);
	virtual void SetStartXY(DBL dSx, DBL dSy);
	virtual void GetStartXY(DBL *dSx, DBL *dSy);
	virtual void SetMoveXY(DBL movex, DBL movey);
	virtual void GetMoveXY(LPDBL movex, LPDBL movey);
	virtual void SetTimeXY(DBL movex, DBL movey);
	virtual void GetTimeXY(LPDBL movex, LPDBL movey);
	DBL GetLenFromStrgTop();
	void SetLenFromStrgTop(DBL setlen);
	virtual void MoveTrns(DBL movex, DBL movey);
	virtual void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual BOOL IsZenKaku();

	virtual void ReverceRejion();
	virtual void DeleteData(CData * delData);
	virtual CData* SrchNearOnLinePoint(DBL xx, DBL yy, DBL *retlen, DBL *retx, DBL *rety, int *retpp);
	virtual CData* SrchNearPoint(DBL xx, DBL yy, DBL *retlen, int *retpp);
	void SetTateYoko(int flag);
	int	CheckTateYoko();
	void DrawModeSet(int flag);
	int CheckDrawMode();
	virtual void OffFlag(int bitflag);
	virtual void OnFlag(int bitflag);
	virtual void PointPicup();
	virtual void AllOffPointFlag(int bitflag);
	virtual void AllOnPointFlag(int bitflag);
	virtual void PointXLegal(DBL xx);
	virtual void PointYLegal(DBL yy);
	virtual void FontLoad(int nFontType);
	virtual void PlotOut(int nID);
	virtual void RenewMiniMax();
	virtual void SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy);
	virtual void MirrorTrns(DBL a, DBL b, DBL t1, DBL t2);
	virtual void RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	virtual void StrctBoxsTrns();
	void SetVect(LPBYTE bufsp, int pcnt, CVect *pcVect);

	void GetPrePostTable(DBL wd, DBL hi, int pre[], int post[]);
private:
};

