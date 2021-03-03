#ifndef __BOLDLIB_H
#define __BOLDLIB_H

#include	"..\object\classdef.h"

class AFX_CLASS_EXPORT CBoldLib : public CObject
{
protected:
	CBoldLib();
	DECLARE_SERIAL(CBoldLib)
public:
	virtual void Serialize(CArchive& ar);
public:
	CBoldLib(int nDummy);
	~CBoldLib();

public:
	CDataList*	m_pcDataList;
	CVect*	m_pcVect;
	int		m_nPcnt;
	int		m_nXByte, m_nXSize, m_nYSize;
	LPBYTE	m_lpCusTbl;
	XYType	*m_lpXYTbl1, *m_lpXYTbl2;
	DBL		m_dFureLimit;
private:
	DBL		m_dRltvTopx, m_dRltvTopy;
public:
	void TrnsDataList(DBL sx, DBL sy, DBL xtime, DBL ytime);
	void BitMapOutLineSet(BYTE *lpBits, BYTE *lpTBits, int xbyte, int xsize, int ysize);
	void BitMapTrace(BYTE *lpBits, BYTE *lpTBits, int xbyte, int xsize, int ysize);
	void BitMapTarceSetPlgn(BYTE *lpBits, BYTE *lpTBits, int scus, int xbyte, int x, int y);
	void NextCusGet(BYTE *lpTBits, int xbyte, int *retx, int *rety, int bcus, int *retcus);
	int FirstCusGet(BYTE *lpBits, int xbyte, int x, int y, int *ncus, int *bcus);
	void CusAddGet(int cus, int *addx, int *addy);
	int CusLegal(int cus);
	void SetDataListPolygon(XYType *lpXYTbl1, XYType *lpXYTbl2, int points);
	void SetXYTblSetPlgn(BYTE *lpTBits, int xbyte, int idx, int tsx, int tsy);
	void SetPlgnEnd();
	void SetPlgnInit();
	int BitOnOffCheck(BYTE *lpBits, int xbyte, int x, int y);
	void BitOnSet(BYTE *lpBits, int xbyte, int x, int y);
	void BitOffSet(BYTE *lpBits, int xbyte, int x, int y);
	void BmpFileOut(LPSTR fname, BYTE far *lpBits, int xbyte, int xsize, int ysize);
	void PlgnPointSet(XYType *lpXYTbl1, int points, int tx, int ty);
	void PlgnLineToArcRejionSet(CVect *pcVect);
	int PlgnCurveChk(CVect *pcVect, int pp1, DBL *retx, DBL *rety, int *retpp);
	int PlgnCrclDastance(DBL tx, DBL ty, DBL x0, DBL y0, DBL r, DBL flimit);
};

#endif

