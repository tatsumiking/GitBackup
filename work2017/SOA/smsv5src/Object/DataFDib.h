
#pragma once

#include "Data.h"
#include "Vect.h"
#include "DibCData.h"


class AFX_CLASS_EXPORT CDataFDib : public CVect
{
protected:
	CDataFDib();
	DECLARE_SERIAL(CDataFDib)
public:
	virtual void Serialize(CArchive& ar);

public:
	CData* m_pcDataTop;
	CDibCData* m_pcDibCData;
public:
	CDataFDib(int nDummy);
	~CDataFDib();

public:
	virtual void ReverceRejion();
	virtual int CheckNullData();
	virtual void SaveEpsFile();
	virtual void OnDraw(CDC *pcDC);
	void OnDrawMP(CDC *pcDC);
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
	virtual CData* SrchIdsData(int *nIDary, int nIDcnt, DBL xx, DBL yy, DBL *retlen);
	virtual CData* SrchDataBoxs(DBL xx, DBL yy, DBL *retlen);
	virtual CData* SrchIdsDataBoxs(int *nIDary, int nIDcnt, DBL xx, DBL yy, DBL *retlen);
	virtual CData* SrchMoji(DBL xx, DBL yy, DBL *retlen, int *retlinkno);
	virtual void OffFlag(int bitflag);
	virtual void OnFlag(int bitflag);
	virtual void ChngFlag(int bitflag);
	virtual void CheckFlagDraw(int flag, CDC *pcDC);
	virtual void AtntionDraw(CDC *pcDC);
	virtual void NoactDraw(CDC *pcDC);
	virtual void DelDraw(CDC *pcDC);
	virtual void StockClrDraw(CDC *pcDC);
	virtual void PlotOut(int nComID);
	virtual CRect* GetBoundingRect();
	virtual void FlagCheckMojiChange(int flag);
	virtual void StrgFiting();
};

