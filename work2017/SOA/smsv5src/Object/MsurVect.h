
#pragma once

#include "Data.h"
#include "Vect.h"

class AFX_CLASS_EXPORT CMsurVect : public CVect
{
protected:
	CMsurVect();
	DECLARE_SERIAL(CMsurVect)
public:
	virtual void Serialize(CArchive& ar);

private:

public:
	int	m_nType;
	TCHAR m_szMsurStrg[RECMAX];
	DBL	m_dFontSize;
	TCHAR m_szFontName[RECMAX];

public:
	CMsurVect(int nDummy);
	~CMsurVect();
public:
	virtual int CheckNullData();
	virtual int CheckID();
	virtual void SetType(int nType);
	virtual void GetType(int *nType);
	virtual void SetMsurStrg(LPTSTR spStrg);
	virtual void GetMsurStrg(LPTSTR spStrg);
	virtual void SetFontSize(DBL dFontSize);
	virtual void GetFontSize(LPDBL dFontSize);
	virtual void SetFontName(LPTSTR spFontName);
	virtual void GetFontName(LPTSTR spFontName);
	virtual void OnDraw(CDC *pcDC);
	virtual void CMsurVect::StockClrDraw(CDC *pcDC);
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual CData* CopyData(DBL movex, DBL movey);
	virtual void PlotOut(int nComID);
	virtual void AtntionDraw(CDC *pcDC);
	virtual void NoactDraw(CDC *pcDC);
	virtual void DelDraw(CDC *pcDC);
	virtual CData* SrchData(DBL xx, DBL yy, DBL *retlen);
	virtual CData* SrchNearPoint(DBL xx, DBL yy, DBL *retlen, int *retpp);
private:
	void DrawOutData(CDC *pcDC);
	void DrawOutXMsur(CDC *pcDC);
	void DrawOutYMsur(CDC *pcDC);
	void LineDrow(CDC *pcDC, DBL sx, DBL sy, DBL ex, DBL ey);
	void TextDrow(CDC *pcDC, DBL sx, DBL sy, DBL ex, DBL ey, LPTSTR strg, int len);
};

