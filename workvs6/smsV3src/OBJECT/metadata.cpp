//
// object metadata.cpp  メタファイル構造
//
//  Copyright (c) 2002 by THEON Ltd.

#include "StdAfx.h"
#include "math.h"
#include "io.h"

#include "metadata.h"

extern "C" {
	void scrnsetslctpen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	void scrndxymmtorltv(DBL *x, DBL *y);
}

IMPLEMENT_SERIAL(CMetaData, CObject, METADATAID)
CMetaData::CMetaData()
{
	m_dDstSx = 0; m_dDstSy = 0;
	m_dDstWidth = 0; m_dDstHeight = 0;
	m_dSrcSx = 0; m_dSrcSy = 0;
	m_dSrcWidth = 0; m_dSrcHeight = 0;
	m_dSrcXDPI = 72; m_dSrcYDPI = 72;
	m_hEmf = 0;
}

//p オブジェクトの生成
CMetaData::CMetaData(int nDummy)
:CData(nDummy)
{
	m_dDstSx = 0; m_dDstSy = 0;
	m_dDstWidth = 0; m_dDstHeight = 0;
	m_dSrcSx = 0; m_dSrcSy = 0;
	m_dSrcWidth = 0; m_dSrcHeight = 0;
	m_dSrcXDPI = 72; m_dSrcYDPI = 72;
	m_hEmf = 0;
}

//p オブジェクトの消滅
CMetaData::~CMetaData()
{
	if(m_hEmf != 0){
		::DeleteEnhMetaFile(m_hEmf);
		m_hEmf = 0;
	}
}

int CMetaData::CheckID()
{
	return(METADATAID);
}

void CMetaData::Serialize(CArchive& ar)
{
	LPBYTE				p;
	HGLOBAL				ph;
	UINT				s;

	if (ar.IsStoring())
	{
		ar << m_dDstSx;
		ar << m_dDstSy;
		ar << m_dDstWidth;
		ar << m_dDstHeight;

			s = ::GetEnhMetaFileBits(m_hEmf,NULL,NULL);
			ar << (DWORD)s;
			ph = ::GlobalAlloc(GHND,s);
			p = (LPBYTE)::GlobalLock(ph);
			::GetEnhMetaFileBits(m_hEmf,s,p);
			ar.Write(p,s);
			::GlobalUnlock(ph);
			::GlobalFree(ph);
	}
	else
	{
		ar >> m_dDstSx;
		ar >> m_dDstSy;
		ar >> m_dDstWidth;
		ar >> m_dDstHeight;

			ar >> (DWORD &)s;
			ph = ::GlobalAlloc(GHND,s);
			p = (LPBYTE)::GlobalLock(ph);
			ar.Read(p,s);
			m_hEmf = ::SetEnhMetaFileBits(s,p);
			::GlobalUnlock(ph);
			::GlobalFree(ph);
	}
	CData::Serialize(ar);
	if (!(ar.IsStoring()))
	{
		RenewMiniMax();
	}
}

void CMetaData::InitElement()
{
}

void CMetaData::GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	*minix = m_dMinix;
	*miniy = m_dMiniy;
	*maxx = m_dMaxx;
	*maxy = m_dMaxy;
}

void CMetaData::RenewMiniMax()
{
	DBL		sx, sy, ex, ey;

	sx = m_dDstSx;
	sy = m_dDstSy;
	ex = sx + m_dDstWidth;
	ey = sy + m_dDstHeight;
	m_dMinix = sx;
	m_dMiniy = sy;
	m_dMaxx = ex;
	m_dMaxy = ey;

	RenewMiniMaxPoint(sx, sy);
	RenewMiniMaxPoint(ex, ey);
}

void CMetaData::MoveTrns(DBL movex, DBL movey)
{
	m_dDstSx = m_dDstSx + movex;
	m_dDstSy = m_dDstSy + movey;
	RenewMiniMax();
}

void CMetaData::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	DBL		sx, sy, ex, ey;

	sx = m_dDstSx;
	sy = m_dDstSy;
	ex = sx + m_dDstWidth;
	ey = sy + m_dDstHeight;
	sx = (sx - cntrx) * timex + cntrx;
	sy = (sy - cntry) * timey + cntry;
	ex = (ex - cntrx) * timex + cntrx;
	ey = (ey - cntry) * timey + cntry;
	m_dDstSx = sx;
	m_dDstSy = sy;
	m_dDstWidth = ex - sx;
	m_dDstHeight = ey - sy;
	RenewMiniMax();
}

void CMetaData::RoundTrns(DBL cntrx, DBL cntry, DBL rag)
{

}

CData *CMetaData::SrchData(DBL xx, DBL yy, DBL *retlen)
{
	DBL		minix, miniy, maxx, maxy;
	DBL		x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		sumrag;
	dfigutype	fg;
	DBL		tlen, tx, ty;

	GetDMiniMax(&minix, &miniy, &maxx, &maxy);
	sumrag = 0;
	x1 = minix; y1 = miniy;
	x2 = minix; y2 = maxy;
	x3 = maxx; y3 = maxy;
	x4 = maxx; y4 = miniy;
	AddSubRag(&sumrag, xx, yy, x1, y1, x2, y2);
	AddSubRag(&sumrag, xx, yy, x2, y2, x3, y3);
	AddSubRag(&sumrag, xx, yy, x3, y3, x4, y4);
	AddSubRag(&sumrag, xx, yy, x4, y4, x1, y1);
	if(M_PI < sumrag || -M_PI > sumrag){
		*retlen = GetSrchNearMM();
		return(this);
	}
	*retlen = INTMAX;
	fg.x1 = x1; fg.y1 = y1;
	fg.x2 = x2; fg.y2 = y2;
	SrchNearOnPointLine(fg, xx, yy, &tlen, &tx, &ty);
	if(tlen < *retlen){
		*retlen = tlen;
	}
	fg.x1 = x2; fg.y1 = y2;
	fg.x2 = x3; fg.y2 = y3;
	SrchNearOnPointLine(fg, xx, yy, &tlen, &tx, &ty);
	if(tlen < *retlen){
		*retlen = tlen;
	}
	fg.x1 = x3; fg.y1 = y3;
	fg.x2 = x4; fg.y2 = y4;
	SrchNearOnPointLine(fg, xx, yy, &tlen, &tx, &ty);
	if(tlen < *retlen){
		*retlen = tlen;
	}
	fg.x1 = x4; fg.y1 = y4;
	fg.x2 = x1; fg.y2 = y1;
	SrchNearOnPointLine(fg, xx, yy, &tlen, &tx, &ty);
	if(tlen < *retlen){
		*retlen = tlen;
	}
	return(this);
}

void CMetaData::AtntionDraw(CDC *pcDC)
{
	scrnsetslctpen(pcDC->m_hDC);
	StockClrDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CMetaData::StockClrDraw(CDC *pcDC)
{
	DrowAreaBoxsCross(pcDC);
}

void CMetaData::OnDraw(CDC *pcDC)
{
	DBL		dsx, dsy, dex, dey;
	DBL		dw, dh;
	CRect	rect;

	//DBL		minx, miny, maxx, maxy;

	//GetDMiniMax(&minx, &miny, &maxx, &maxy);
	GetDstElement(&dsx, &dsy, &dw, &dh);
	dsx = dsx;
	dsy = dsy;
	dex = dsx + dw;
	dey = dsy + dh;
	scrndxymmtorltv(&dsx, &dsy);
	scrndxymmtorltv(&dex, &dey);
	rect.left = (long)dsx; rect.top = (long)dey;
	rect.right = (long)dex; rect.bottom = (long)dsy;
	pcDC->PlayMetaFile(m_hEmf, rect);
}

void CMetaData::DrowAreaBoxsCross(CDC *pcDC)
{
	DBL		dsx, dsy, dw, dh;
	DBL		dex, dey;
	int		nsx, nsy, nex, ney;

	GetDstElement(&dsx, &dsy, &dw, &dh);
	dsx = dsx;
	dsy = dsy;
	dex = dsx + dw;
	dey = dsy + dh;
	scrndxymmtorltv(&dsx, &dsy);
	scrndxymmtorltv(&dex, &dey);
	nsx = (int)dsx - 1; nsy = (int)dsy - 1;
	nex = (int)dex + 1; ney = (int)dey + 1;
	pcDC->MoveTo(nsx, nsy);
	pcDC->LineTo(nsx, ney);
	pcDC->LineTo(nex, ney);
	pcDC->LineTo(nex, nsy);
	pcDC->LineTo(nsx, nsy);

	pcDC->MoveTo(nsx, nsy);
	pcDC->LineTo(nex, ney);
	pcDC->MoveTo(nsx, ney);
	pcDC->LineTo(nex, nsy);
}

CData * CMetaData::CopyData(DBL movex, DBL movey)
{
	CMetaData*	pcnewData;
	LPBYTE		p;
	HGLOBAL		ph;
	UINT		s;

	pcnewData = new CMetaData(METADATAID);
	pcnewData->m_dDstHeight = m_dDstHeight;
	pcnewData->m_dDstWidth = m_dDstWidth;
	pcnewData->m_dDstSx = m_dDstSx + movex;
	pcnewData->m_dDstSy = m_dDstSy + movey;

	s = ::GetEnhMetaFileBits(m_hEmf,NULL,NULL);
	ph = ::GlobalAlloc(GHND,s);
	p = (LPBYTE)::GlobalLock(ph);
	::GetEnhMetaFileBits(m_hEmf,s,p);
	pcnewData->m_hEmf = ::SetEnhMetaFileBits(s,p);
	::GlobalUnlock(ph);
	::GlobalFree(ph);

	pcnewData->RenewMiniMax();
	return((CData*)pcnewData);
}

void CMetaData::GetDstElement(LPDBL dstsx, LPDBL dstsy, LPDBL dstwd, LPDBL dsthi)
{
	*dstsx = m_dDstSx;
	*dstsy = m_dDstSy;
	*dstwd = m_dDstWidth;
	*dsthi = m_dDstHeight;
}

void CMetaData::SetDstElement(DBL dstsx, DBL dstsy, DBL dstwd, DBL dsthi)
{
	m_dDstSx = dstsx;
	m_dDstSy = dstsy;
	m_dDstWidth = dstwd;
	m_dDstHeight = dsthi;
}

void CMetaData::GetSrcElement(LPDBL srcxdpi, LPDBL srcydpi, LPDBL srcwd, LPDBL srchi)
{
	*srcxdpi = m_dSrcXDPI;
	*srcydpi = m_dSrcYDPI;
	*srcwd = m_dSrcWidth;
	*srchi = m_dSrcHeight;
}


void CMetaData::LegalElement()
{

}
