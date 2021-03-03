#ifndef __METADATA_H
#define __METADATA_H

#include "data.h"

class AFX_CLASS_EXPORT CMetaData : public CData
{
protected:
	CMetaData();
	DECLARE_SERIAL(CMetaData)
public:
	CMetaData(int nDummy);
	~CMetaData();
	virtual int CheckID();
	virtual void Serialize(CArchive& ar);

// 関数
public:
	virtual void InitElement();
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void RenewMiniMax();
	virtual void MoveTrns(DBL movex, DBL movey);
	virtual void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual void RoundTrns(DBL cntrx, DBL cntry, DBL rag);
	virtual CData* SrchData(DBL xx, DBL yy, DBL *retlen);
	virtual void AtntionDraw(CDC *pcDC);
	virtual void StockClrDraw(CDC *pcDC);
	virtual void OnDraw(CDC *pcDC);
	virtual CData * CopyData(DBL movex, DBL movey);

	void GetDstElement(LPDBL dstsx, LPDBL dstsy, LPDBL dstwd, LPDBL dsthi);
	void SetDstElement(DBL dstsx, DBL dstsy, DBL dstwd, DBL dsthi);
	void GetSrcElement(LPDBL srcxdpi, LPDBL srcydpi, LPDBL srcwd, LPDBL srchi);
	void DrowAreaBoxsCross(CDC *pcDC);
// データメンバー
public:
	HENHMETAFILE	m_hEmf;
	DBL m_dDstSx, m_dDstSy;	//画面上のビットマップの開始位置mm
	DBL m_dDstWidth, m_dDstHeight;	//画面上のビットマップサイズmm
	DBL m_dSrcSx, m_dSrcSy;	// 実際のビットマップ位置(0,0)dot
	DBL m_dSrcWidth, m_dSrcHeight;	// 実際のビットマップサイズdot
	DBL	m_dSrcXDPI, m_dSrcYDPI;
	// 内部使用
public:
	virtual void LegalElement();

private:
};

#endif

