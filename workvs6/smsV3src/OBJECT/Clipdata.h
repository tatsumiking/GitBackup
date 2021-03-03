#ifndef __CLIPDATA_H
#define __CLIPDATA_H

#include "vect.h"

class AFX_CLASS_EXPORT CClipData : public CVect
{
protected:
	CClipData();
	DECLARE_SERIAL(CClipData)
public:
	CClipData(int nDummy);
	~CClipData();
	virtual int CheckID();
	virtual void Serialize(CArchive& ar);
// データメンバー
public:
	CData	*m_pcClipTop;
	CData	*m_pcDataTop;
public:

public:
	virtual void OnDraw(CDC *pcDC);
	virtual void DeleteData(CData *pcDelData);
	virtual void InitElement();
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void RenewMiniMax();
	virtual void MoveTrns(DBL movex, DBL movey);
	virtual void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual void RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	virtual CData *SrchData(DBL xx, DBL yy, DBL *retlen);
	virtual CData* SrchNearPoint(DBL xx, DBL yy, DBL * retlen, int * retpp);
	virtual CData* SrchNearOnLinePoint(DBL xx, DBL yy, DBL * retlen, DBL * retx, DBL * rety, int * retpp);
	virtual void AtntionDraw(CDC *pcDC);
	virtual void NoactDraw(CDC *pcDC);
	virtual void DelDraw(CDC *pcDC);
	virtual void StockClrDraw(CDC *pcDC);
	virtual CData * CopyData(DBL movex, DBL movey);
	int AddData(CData *pcData);
	int AddClip(CData *pcData);
private:
};

#endif

