#ifndef __NAMEGRUP_H
#define __NAMEGRUP_H

#include "vect.h"

class AFX_CLASS_EXPORT CNameGrup : public CVect
{
protected:
	CNameGrup();
	DECLARE_SERIAL(CNameGrup)
public:
	CNameGrup(int nDummy);
	~CNameGrup();
	virtual void Serialize(CArchive& ar);
public:
	CData*	m_pcDataTop;
	char	m_szName[RECMAX];
	DBL		m_dFontSize;
	DBL		m_dXSpace;
	DBL		m_dYSpace;
	DBL		m_dCntrSize;
public:

public:
	virtual int CheckID();
	virtual void DeleteData(CData *pData);
	virtual int CheckNullData();
	virtual void SetName(LPSTR lpStr);
	virtual void GetName(LPSTR lpStr);
	virtual void SetFontSize(DBL size);
	virtual void GetFontSize(LPDBL size);
	virtual void SetSpace(DBL xspace, DBL yspace);
	virtual void GetSpace(LPDBL xspace, LPDBL yspace);
	virtual void SetCntrSize(DBL size);
	virtual void GetCntrSize(LPDBL size);
	virtual CData* GetData(int idx);
	virtual int SetData(CData* pcData);
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void RenewMiniMax();
	virtual CData * CopyData(DBL movex, DBL movey);
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
	virtual CData* SrchMoji(DBL x, DBL y, DBL *len, int *linkno);
	virtual CData* SrchNearPoint(DBL xx, DBL yy, DBL * retlen, int * retpp);
	virtual void PlotOut(int nComID);
	virtual void CheckFlagDraw(int flag, CDC *pcDC);
	virtual void CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick);
	virtual void CheckFlagOutlineDraw(int flag, CDC *pcDC);
	virtual void AtntionDraw(CDC *pcDC);
	virtual void NoactDraw(CDC *pcDC);
	virtual void DelDraw(CDC *pcDC);
	virtual void OnDraw(CDC *pcDC);
	virtual void CntrDraw(CDC *pcDC);
	virtual void WakuDraw(CDC *pcDC);
	virtual void NameDraw(CDC *pcDC);
	virtual CRect* GetBoundingRect();
	virtual void InAreaDataAtntionOn(DBL sx, DBL sy, DBL ex, DBL ey);
	virtual void OnFlag(int bitflag);
	virtual void OffFlag(int bitflag);
	virtual int CheckFlag(int bitflag);
	virtual void SetDefOutLineClr();
	virtual void SetDefFillClr();
	virtual void SaveDxfFile();
	virtual void SaveEpsFile();
	virtual void SetUseClrPenno(int notbl[], LPINT cnt);
	virtual void DataChange();
	virtual void DataChangeSetDataGrup(CDataGrup *pcDataGrup);
	virtual void StrgFiting();
private:
};

#endif

