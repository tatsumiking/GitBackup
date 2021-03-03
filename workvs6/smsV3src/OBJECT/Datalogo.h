
#ifndef __DATALOGO_H
#define __DATALOGO_H

#include "vect.h"
#include "dibcdata.h"

class AFX_CLASS_EXPORT CDataLogo : public CVect
{
protected:
	CDataLogo();
	DECLARE_SERIAL(CDataLogo)
public:
	virtual void Serialize(CArchive& ar);

public:
	CData* m_pcDataTop;
	CDibCData* m_pcDibCData;
	int		m_nFillError;
private:

public:
	CDataLogo(int nDummy);
	~CDataLogo();

public:
	int GetSumPointCount();
	void OnLogoDraw(CDC* pcDC);
	virtual void ResetVectLink();
	virtual void ReverceRejion();
	virtual int CheckNullData();
	virtual void OutLineOmit(DBL mm);
	virtual void SaveDxfFile();
	virtual void SaveEpsFile();
	virtual void OnDraw(CDC *pcDC);
	virtual void OutlineDraw(CDC *pcDC);
	virtual void SetDefFillClr();
	virtual void SetDefOutLineClr();
	virtual void PointPicup();
	virtual void AllOffPointFlag(int bitflag);
	virtual void AllOnPointFlag(int bitflag);
	virtual void PointXLegal(DBL xx);
	virtual void PointYLegal(DBL yy);
	void SetDibCData(CDibCData *pDibCData);
	virtual CData * CopyData(DBL movex, DBL movey);
	virtual int CheckFlag(int bitflag);
	virtual void SaveStrgStoreFile(FILE *fp, int id);
	virtual void LoadStrgStoreFile(FILE *fp, int id);
	virtual void DeleteData(CData * pData);
	virtual CData* SrchNearPoint(DBL xx, DBL yy, DBL * retlen, int * retpp);
	virtual CData* SrchNearOnLinePoint(DBL xx, DBL yy, DBL * retlen, DBL * retx, DBL * rety, int * retpp);
	virtual void SetAtnVectFSF1fBuf(CFSF1Font *pcFSF1Font);
	virtual void InAreaDataAtntionOn(DBL sx, DBL sy, DBL ex, DBL ey);
	virtual int SetData(CData *cpData);
	virtual CData* GetData(int linkno);
	virtual int CheckID();
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void RenewMiniMax();
	virtual void MoveTrns(DBL movex, DBL movey);
	virtual void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual void SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy);
	virtual void MirrorTrns(DBL a, DBL b, DBL t1, DBL t2);
	virtual void RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	virtual void StrctBoxsTrns();
	virtual void MtrxTrns();
	virtual CData* SrchData(DBL xx, DBL yy, DBL *retlen);
	virtual void OffFlag(int bitflag);
	virtual void OnFlag(int bitflag);
	virtual void ChngFlag(int bitflag);
	virtual void CheckFlagDraw(int flag, CDC *pcDC);
	virtual void CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick);
	virtual void CheckFlagOutlineDraw(int flag, CDC *pcDC);
	virtual void AtntionDraw(CDC *pcDC);
	virtual void NoactDraw(CDC *pcDC);
	virtual void DelDraw(CDC *pcDC);
	virtual void StockClrDraw(CDC *pcDC);
	virtual void PlotOut(int nComID);
	virtual CRect* GetBoundingRect();
};

#endif

