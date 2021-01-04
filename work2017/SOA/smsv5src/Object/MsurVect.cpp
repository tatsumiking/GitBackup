//
// Easy Scanner System
//
// object msurvect.cpp  ê°ñ@ê¸ä÷åWä÷êî
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"

#include "Data.h"
#include "Vect.h"
#include "DataList.h"
#include "MsurVect.h"

#include	"../ObjLib/Baselib.h"
#include	"../ObjLib/Senvlib.h"
#include	"../ObjLib/Scrnlib.h"

IMPLEMENT_SERIAL(CMsurVect, CObject, MSURVECTID)
CMsurVect::CMsurVect()
{
}

CMsurVect::CMsurVect(int nDummy)
	: CVect(nDummy)
{
	m_nType = XMSURTYPE;
	m_dFontSize = 5.0;
	_tcscpy_s(m_szFontName, _T("ÇlÇr ñæí©"));
	_tcscpy_s(m_szMsurStrg, _T(""));

	SetAtrDXY(0, 0, 0.0, 0.0);
	SetAtrDXY(1, 0, 0.0, 0.0);
	SetAtrDXY(2, 0, 0.0, 0.0);
	SetPointCount(3);
}

CMsurVect::~CMsurVect()
{
}

int CMsurVect::CheckID()
{
	return(MSURVECTID);
}

void CMsurVect::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nType;
		ar << CString(m_szMsurStrg);
		ar << m_dFontSize;
		ar << CString(m_szFontName);
		// ar <<
	}
	else
	{
		CString	csMsurStrg;
		CString	csFontName;

		ar >> m_nType;
		ar >> csMsurStrg;
		_tcscpy_s(m_szMsurStrg, csMsurStrg);
		ar >> m_dFontSize;
		ar >> csFontName;
		_tcscpy_s(m_szFontName, csFontName);
		// ar >>
	}
	CVect::Serialize(ar);
	if (!(ar.IsStoring()))
	{
		RenewMiniMax();
	}
}

//p ì‡ïîÉfÅ[É^Å[Ç™ãÛÇÃèÛë‘Ç≈Ç†ÇÍÇŒçÌèúÇµÇ∑Ç◊ÇƒÇ™ãÛÇÃÇ∆Ç´TRUEÇï‘Ç∑
int CMsurVect::CheckNullData()
{
	if(CVect::CheckNullData() == TRUE){
		return(TRUE);
	}
	return(FALSE);
}

void CMsurVect::SetType(int nType)
{
	m_nType	= nType;
}

void CMsurVect::GetType(int *nType)
{
	*nType = m_nType;
}

void CMsurVect::SetMsurStrg(LPTSTR spMsurStrg)
{
	_tcscpy_s(m_szMsurStrg, spMsurStrg);
}

void CMsurVect::GetMsurStrg(LPTSTR spMsurStrg)
{
	int len = _tcslen(m_szMsurStrg) + 1;
	_tcscpy_s(spMsurStrg, len, m_szMsurStrg);
}

void CMsurVect::SetFontSize(DBL dFontSize)
{
	m_dFontSize = dFontSize;
}

void CMsurVect::GetFontSize(LPDBL dFontSize)
{
	*dFontSize = m_dFontSize;
}

void CMsurVect::SetFontName(LPTSTR spFontName)
{
	_tcscpy_s(m_szFontName, spFontName);
}

void CMsurVect::GetFontName(LPTSTR spFontName)
{
	int len = _tcslen(m_szFontName) + 1;
	_tcscpy_s(spFontName, len, m_szFontName);
}


void CMsurVect::PlotOut(int nComID)
{
}

void CMsurVect::AtntionDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	scrnsetslctpen(pcDC->m_hDC);
	DrawOutData(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CMsurVect::NoactDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	scrnsetnoslctpen(pcDC->m_hDC);
	DrawOutData(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CMsurVect::DelDraw(CDC *pcDC)
{
	scrnsetdelpen(pcDC->m_hDC);
	DrawOutData(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CMsurVect::StockClrDraw(CDC *pcDC)
{
	DrawOutData(pcDC);
}

void CMsurVect::OnDraw(CDC *pcDC)
{
	int		mode;
	int		penno;
	int		rval, gval, bval;
	int		nlimit;

	nlimit = senvgetplgnlimit();
	if(senvchckstopflag() == ON){
		return;
	}
	penno = GetOutLinePen();
	if(penno == 0){
		rval = 0; gval = 0; bval = 0;
	}
	else{
		senvgetpenrgb(penno, &rval, &gval, &bval);
	}
	scrnsetpreviewpen(pcDC->m_hDC, rval, gval, bval, nlimit);
	mode = senvcheckdrawmode();
	DrawOutData(pcDC);
	scrnpenend(pcDC->m_hDC);
}

CData* CMsurVect::CopyData(DBL movex, DBL movey)
{
	CMsurVect *pcnewData;

	pcnewData = (CMsurVect*)NewDataCreate(MSURVECTID);

	pcnewData->SetType(m_nType);
	pcnewData->SetMsurStrg(m_szMsurStrg);
	pcnewData->SetFontSize(m_dFontSize);
	pcnewData->SetFontName(m_szFontName);

	pcnewData->RenewMiniMax();
	return((CData*)pcnewData);
}

void CMsurVect::GetDMiniMax(DBL * minix, DBL * miniy, DBL * maxx, DBL * maxy)
{
	CVect::RenewMiniMax();
	CVect::GetDMiniMax(minix, miniy, maxx, maxy);
}

void CMsurVect::DrawOutData(CDC *pcDC)
{
	switch(m_nType){
	case XMSURTYPE:
		DrawOutXMsur(pcDC);
		break;
	case YMSURTYPE:
		DrawOutYMsur(pcDC);
		break;
	}
}

void CMsurVect::DrawOutXMsur(CDC *pcDC)
{
	DBL		x1, y1, x2, y2, x3, y3;
	DBL		sx, sy, ex, ey;
	int		len;

	GetDXY(0, &x1, &y1);
	GetDXY(1, &x2, &y2);
	GetDXY(2, &x3, &y3);
	sy = y3 - m_dFontSize / 2.0;
	ey = y3 + m_dFontSize / 2.0;
	sx = x1; ex = x1;
	LineDrow(pcDC, sx, sy, ex, ey);
	sx = x2; ex = x2;
	LineDrow(pcDC, sx, sy, ex, ey);
	sx = x1; ex = x2;
	sy = y3; ey = y3;
	LineDrow(pcDC, sx, sy, ex, ey);
	len = _tcslen(m_szMsurStrg);
	if(x1 < x2){
		sx = (x1 + x2) / 2.0 - len * m_dFontSize / 4;
		ex = (x1 + x2) / 2.0 + len * m_dFontSize / 4;
		sy = y3; ey = y3;
	}
	else{
		sx = (x1 + x2) / 2.0 + len * m_dFontSize / 4;
		ex = (x1 + x2) / 2.0 - len * m_dFontSize / 4;
		sy = y3; ey = y3;
	}
	TextDrow(pcDC, sx, sy, ex, ey, m_szMsurStrg, len);
}

void CMsurVect::DrawOutYMsur(CDC *pcDC)
{
	DBL		x1, y1, x2, y2, x3, y3;
	DBL		sx, sy, ex, ey;
	int		len;

	GetDXY(0, &x1, &y1);
	GetDXY(1, &x2, &y2);
	GetDXY(2, &x3, &y3);

	sx = x3 - m_dFontSize / 2.0;
	ex = x3 + m_dFontSize / 2.0;
	sy = y1; ey = y1;
	LineDrow(pcDC, sx, sy, ex, ey);
	sy = y2; ey = y2;
	LineDrow(pcDC, sx, sy, ex, ey);
	sx = x3; ex = x3;
	sy = y1; ey = y2;
	LineDrow(pcDC, sx, sy, ex, ey);
	len = _tcslen(m_szMsurStrg);
	if(y1 < y2){
		sy = (y1 + y2) / 2.0 - len * m_dFontSize / 4;
		ey = (y1 + y2) / 2.0 + len * m_dFontSize / 4;
		sx = x3; ex = x3;
	}
	else{
		sy = (y1 + y2) / 2.0 + len * m_dFontSize / 4;
		ey = (y1 + y2) / 2.0 - len * m_dFontSize / 4;
		sx = x3; ex = x3;
	}
	TextDrow(pcDC, sx, sy, ex, ey, m_szMsurStrg, len);
}

void CMsurVect::LineDrow(CDC *pcDC, DBL sx, DBL sy, DBL ex, DBL ey)
{
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)ey);
}

void CMsurVect::TextDrow(CDC *pcDC, DBL sx, DBL sy, DBL ex, DBL ey, LPTSTR strg, int len)
{
	HFONT	hF, oldhF;
	DBL		dFW, dFH, rag;
	int		x, y;

	dFW = m_dFontSize / 2.0;
	dFH = m_dFontSize;
	scrndlenmmtorltv(&dFW);
	scrndlenmmtorltv(&dFH);
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	if(dFW < 1.0){
		dFW = 1.0;
	}
	if(dFH < 1.0){
		dFH = 1.0;
	}
	if(sy == ey){
		if(sx < ex){
			rag = 0.0;
			x = (int)sx; y = (int)(sy - dFH * 0.7);
		}
		else{
			rag = 1800.0;
			x = (int)sx; y = (int)(sy + dFH * 0.7);
		}
	}
	else{
		if(sy < ey){
			rag = 2700.0;
			y = (int)sy; x = (int)(sx + dFH * 0.7);
		}
		else{
			rag = 900.0;
			y = (int)sy; x = (int)(sx - dFH * 0.7);
		}
	}
	hF = CreateFont((int)dFH, (int)dFW, (int)rag, 0, 0, 0, 0, 0,
			SHIFTJIS_CHARSET,0,0,0,0,m_szFontName);
	oldhF = (HFONT)SelectObject(pcDC->m_hDC, hF);
	pcDC->SetBkMode(OPAQUE);
	TextOut(pcDC->m_hDC, x, y, strg, len);
	pcDC->SetBkMode(TRANSPARENT);
	hF = (HFONT)SelectObject(pcDC->m_hDC, oldhF);
	DeleteObject(hF);
}

CData* CMsurVect::SrchData(DBL xx, DBL yy, DBL *retlen)
{
	DBL		x1, y1, x2, y2, x3, y3;
	DBL		sx, sy, ex, ey;
	DBL		len, x, y;

	GetDXY(0, &x1, &y1);
	GetDXY(1, &x2, &y2);
	GetDXY(2, &x3, &y3);

	switch(m_nType){
	case XMSURTYPE:
		sx = x1; ex = x2;
		sy = y3; ey = y3;
		break;
	case YMSURTYPE:
		sx = x3; ex = x3;
		sy = y1; ey = y2;
		break;
	}
	len = hypot(sx - xx, sy - yy);
	*retlen = len;
	len = hypot(ex - xx, ey - yy);
	if((*retlen) > len){
		*retlen = len;
	}
	if(GetNearOnlinePoint(xx, yy, sx, sy, ex, ey, &len, &x, &y) == ON){
		if((*retlen) > len){
			*retlen = len;
		}
	}
	return(this);
}

CData* CMsurVect::SrchNearPoint(DBL xx, DBL yy, DBL *retlen, int *retpp)
{
	*retlen = INTMAX;
	*retpp = 0;
	return(this);
}
