//
// Easy Scanner System
//
// object rulevect.cpp  årê¸ä÷åWä÷êî
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include <math.h>
#include "rulevect.h"

extern "C" {
	int	senvchckstopflag();
	int	senvcheckdrawmode();
	void  scrndxymmtorltv(LPDBL x, LPDBL y);
	BOOL  pdrwatrarcchk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrbezechk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrcurvechk(int atr1);
	DBL   pdrwendragget(int atr1, int atr2, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL   pdrwstartragget(int atr0, int atr1, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2);
	void  pdrwmove(HDC PaintDC, DBL x1, DBL y1);
	void  pdrwline(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2);
	void  rulelibgetkind(int kindno, long *pattern, int *style, char *stylename);
	void  rulelibmove(HDC paintDC, DBL x1, DBL y1, long ptn, int style, DBL *sum);
	void  rulelibline(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, long ptn, int style, DBL *sum);
	void  rulelibarc(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, long ptn, int style, DBL *sum);
	void  rulelibbeze(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4, long ptn, int style, DBL *sum);
	void  rulelibfreecurve(HDC PaintDC, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, long ptn, int style, DBL *sum);
}

IMPLEMENT_SERIAL(CRuleVect, CObject, 3)
CRuleVect::CRuleVect()
{
	m_nStyle = 0;
}

CRuleVect::CRuleVect(int nDummy)
	: CVect(nDummy)
{
	m_nStyle = 0;
}

CRuleVect::~CRuleVect()
{
}

int CRuleVect::CheckID()
{
	return(RULEVECTID);
}

void CRuleVect::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nStyle;
	}
	else
	{
		ar >> m_nStyle;
	}
	CVect::Serialize(ar);
	if (!(ar.IsStoring()))
	{
		RenewMiniMax();
	}
}

//p ì‡ïîÉfÅ[É^Å[Ç™ãÛÇÃèÛë‘Ç≈Ç†ÇÍÇŒçÌèúÇµÇ∑Ç◊ÇƒÇ™ãÛÇÃÇ∆Ç´TRUEÇï‘Ç∑
int CRuleVect::CheckNullData()
{
	if(CVect::CheckNullData() == TRUE){
		return(TRUE);
	}
	return(FALSE);
}

void CRuleVect::GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	DBL		width;

	CVect::GetDMiniMax(minix, miniy, maxx, maxy);
	width = GetOutLineWidth();
	*minix = *minix - width;
	*miniy = *miniy - width;
	*maxx = *maxx + width;
	*maxy = *maxy + width;
}

void CRuleVect::SetStyle(int styleno)
{
	m_nStyle = styleno;
}

int CRuleVect::GetStyle()
{
	return(m_nStyle);
}

void CRuleVect::OnDraw(CDC *pcDC)
{
	int		mode;

	if(senvchckstopflag() == ON){
		return;
	}
	mode = senvcheckdrawmode(); 
	if(mode == PREVIEW
	|| mode == BLACKVIEW
	|| mode == PRINTVIEW
	|| mode == DIRCTVIEW
	|| mode == METAFVIEW){
		PlgnDraw(pcDC);
	}
	else{
		CVect::PlgnDraw(pcDC);
	}
}

void CRuleVect::OutlineDraw(CDC *pcDC)
{
	CVect::OutlineDraw(pcDC);
}


void CRuleVect::VectPlgnDraw(CDC *pcDC)
{
	CVect::PlgnDraw(pcDC);
}

void CRuleVect::PlgnDraw(CDC *pcDC)
{
	if(CheckFlag(REJIONFLAG) == ON){
		if(m_nPointCount >= 2){
			PlgnRejionDrawOut(pcDC);
		}
	}
	else{
		if(m_nPointCount >= 2){
			PlgnNoRejionDrawOut(pcDC);
		}
	}
}

void CRuleVect::PlgnRejionDrawOut(CDC *pcDC)
{
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	long	pattern;
	int		style;
	char	stylename[RECMAX];
	DBL		sumlen;

	if(m_nPointCount <= 1){
		return;
	}
	rulelibgetkind(m_nStyle, &pattern, &style, stylename);
	sumlen = 0;
	if(m_nPointCount == 2){
		GetAtrDXY(0, &atr1, &x1, &y1);
		GetAtrDXY(1, &atr2, &x2, &y2);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);
		rulelibmove(pcDC->m_hDC, x1, y1, pattern, style, &sumlen);
		rulelibline(pcDC->m_hDC, x1, y1, x2, y2, pattern, style, &sumlen);
		return;
	}
	p0 = m_nPointCount-1;
	p1 = 0;
	p2 = 1;
	GetAtrDXY(p0, &atr0, &x0, &y0);
	GetAtrDXY(p1, &atr1, &x1, &y1);
	GetAtrDXY(p2, &atr2, &x2, &y2);
	scrndxymmtorltv(&x0, &y0);
	scrndxymmtorltv(&x1, &y1);
	if(pdrwatrarcchk(atr0, atr1, atr2) == ON){
		rulelibmove(pcDC->m_hDC, x0, y0, pattern, style, &sumlen);
	}
	else{
		rulelibmove(pcDC->m_hDC, x1, y1, pattern, style, &sumlen);
	}

	for(pp = 0; pp < m_nPointCount;){
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
		scrndxymmtorltv(&x0, &y0);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);
		scrndxymmtorltv(&x3, &y3);
		scrndxymmtorltv(&x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				rulelibline(pcDC->m_hDC, x1, y1, x2, y2, pattern, style, &sumlen);
				rulelibline(pcDC->m_hDC, x2, y2, x3, y3, pattern, style, &sumlen);
			}
			else if(p2 == 0){
				rulelibline(pcDC->m_hDC, x1, y1, x2, y2, pattern, style, &sumlen);
			}
			else{
				rulelibbeze(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4, pattern, style, &sumlen);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				rulelibarc(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, pattern, style, &sumlen);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					rulelibarc(pcDC->m_hDC, x0, y0, x1, y1, x2, y2, pattern, style, &sumlen);
				}
				else{
					rulelibfreecurve(pcDC->m_hDC, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3, pattern, style, &sumlen);
				}
			}
			else{
				rulelibline(pcDC->m_hDC, x1, y1, x2, y2, pattern, style, &sumlen);
			}
			pp += 1;
		}
	}
}

void CRuleVect::PlgnNoRejionDrawOut(CDC *pcDC)
{
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	long	pattern;
	int		style;
	char	stylename[RECMAX];
	DBL		sumlen;

	if(m_nPointCount <= 1){
		PointDraw(pcDC, 0);
		return;
	}
	rulelibgetkind(m_nStyle, &pattern, &style, stylename);
	GetAtrDXY(m_nPointCount-1, &atr2, &x2, &y2);
	scrndxymmtorltv(&x2, &y2);
	pdrwmove(pcDC->m_hDC, x2, y2);
	pdrwline(pcDC->m_hDC, x2, y2, x2, y2);
	GetAtrDXY(0, &atr1, &x1, &y1);
	scrndxymmtorltv(&x1, &y1);
	pdrwmove(pcDC->m_hDC, x1, y1);
	pdrwline(pcDC->m_hDC, x1, y1, x1, y1);
	sumlen = hypot(x2 - x1, y2 - y1);
	if(sumlen < LIMITLINE){
		PointDraw(pcDC, 0);
		return;
	}
	sumlen = 0;
	rulelibmove(pcDC->m_hDC, x1, y1, pattern, style, &sumlen);

	for(pp = 0; pp < m_nPointCount-1;){
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
		scrndxymmtorltv(&x0, &y0);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);
		scrndxymmtorltv(&x3, &y3);
		scrndxymmtorltv(&x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				rulelibline(pcDC->m_hDC, x1, y1, x2, y2, pattern, style, &sumlen);
				rulelibline(pcDC->m_hDC, x2, y2, x3, y3, pattern, style, &sumlen);
			}
			else if(p3 == 0){
				rulelibline(pcDC->m_hDC, x1, y1, x2, y2, pattern, style, &sumlen);
			}
			else{
				rulelibbeze(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4, pattern, style, &sumlen);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				rulelibline(pcDC->m_hDC, x1, y1, x2, y2, pattern, style, &sumlen);
			}
			else{
				rulelibarc(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, pattern, style, &sumlen);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				rulelibfreecurve(pcDC->m_hDC, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3, pattern, style, &sumlen);
			}
			else{
				rulelibline(pcDC->m_hDC, x1, y1, x2, y2, pattern, style, &sumlen);
			}
			pp += 1;
		}
	}
}
