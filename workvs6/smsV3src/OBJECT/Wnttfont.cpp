
//
// Easy Scanner System
//
// object wnttfont.cpp  ÇsÇíÇïÇÖÇsÇôÇêÇÖÉtÉHÉìÉgèàóùä÷êî
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include "wnttfont.h"

#define	NODBG		1

extern "C" {
	int mojienvcheckbold();
	int mojienvcheckitric();
}

IMPLEMENT_SERIAL(CWNTTFont, CObject, 3)
CWNTTFont::CWNTTFont()
{
}

CWNTTFont::CWNTTFont(int nDummy)
{
	m_lAllocGlyphLen = 0x0000FFFF;
	m_hGlyphBuf = GlobalAlloc(GHND, m_lAllocGlyphLen);
	TrnsElementReset();
}

CWNTTFont::~CWNTTFont()
{
	GlobalFree(m_hGlyphBuf);
}

void CWNTTFont::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// ar << 
	}
	else
	{
		// ar >>
	}
}

void CWNTTFont::FSF1ClassSet(CFSF1Font* pFSF1Font)
{
	m_pcFSF1Font = pFSF1Font;
}

void CWNTTFont::FontNameSet(LPSTR pFont)
{
	strcpy(m_sCrtFont, pFont);
}

void CWNTTFont::BezeAddTimeSet(DBL time)
{
	m_dBezeaddtime = time;
}

void CWNTTFont::TrnsElementReset()
{
	m_dBezeaddtime = 1.3;
	m_dXaddtime = 1.0;
	m_dYaddtime = 1.0;
	m_dXaddmove = 0.0;
	m_dYaddmove = 0.0;
}

void CWNTTFont::SetTateYoko(int flag)
{
	m_nTateYoko = flag;
}

void CWNTTFont::SF1TrnsDataLoad(UINT sjis)
{
	HDC	hDC;

	if(m_hGlyphBuf == 0){
		PointSetStart();
		PointSetEnd();
		return;
	}
	hDC = GetDC(m_pcParent->m_hWnd);

	m_lpGlyphBuf = (BYTE FAR *)GlobalLock(m_hGlyphBuf);
	if(m_lpGlyphBuf == NULL){
		PointSetStart();
		PointSetEnd();
		ReleaseDC(m_pcParent->m_hWnd, hDC);
		return;
	}
	DataLoad(hDC, sjis);
	if(m_lGlyphLen <= 16){
		PointSetStart();
		PointSetEnd();
	}
	else{
		GlyphToFSF1Trns();
	}
	if(m_lpGlyphBuf != NULL){
		GlobalUnlock(m_hGlyphBuf);
	}

	ReleaseDC(m_pcParent->m_hWnd, hDC);
}

void CWNTTFont::DataLoad(HDC hDC, UINT sjis)
{
	GLYPHMETRICS	gm;
	MAT2	mt;
	HFONT	hF, oldhF;
	int		bold;
	DWORD	itric;
	char	msgbuf[QRECMAX];
	DBL		time;
	int		sizeh, sizew;
#ifdef DBG
	char	dbgmsgbuf[RECMAX];
#endif

	mt.eM11.value=1; mt.eM11.fract=0; mt.eM12.value=0; mt.eM12.fract=0;
	mt.eM21.value=0; mt.eM21.fract=0; mt.eM22.value=1; mt.eM22.fract=0;

	if(mojienvcheckbold() == ON){
		bold = FW_BOLD;
	}
	else{
		bold = 0;
	}
	if(mojienvcheckitric() == ON){
		itric = TRUE;
	}
	else{
		itric = FALSE;
	}

	sizeh = FTTFMESH;
	sizew = FTTFMESH/2;
	if(0x0100 < sjis){
		hF = CreateFont(sizeh, sizew, 0, 0, bold, itric, 0, 0,
				//SHIFTJIS_CHARSET,OUT_TT_PRECIS,
				DEFAULT_CHARSET,OUT_TT_PRECIS,
				CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
				//FIXED_PITCH | FF_DONTCARE, m_sCrtFont);
				DEFAULT_PITCH | FF_DONTCARE,m_sCrtFont);
	}
	else{
		hF = CreateFont(sizeh, sizew, 0, 0, bold, itric, 0, 0,
				//ANSI_CHARSET,OUT_TT_PRECIS,
				DEFAULT_CHARSET,OUT_TT_PRECIS,
				CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
				//FIXED_PITCH | FF_DONTCARE, m_sCrtFont);
				DEFAULT_PITCH | FF_DONTCARE,m_sCrtFont);
	}

	oldhF = (HFONT)SelectObject(hDC, hF);

	TEXTMETRIC mtr;
	GetTextMetrics(hDC, &mtr);
	//if(mtr.tmMaxCharWidth < 255){
	// 2002.07.25 èºà‰â∆ñ‰ÇÃÇΩÇﬂÇ…î˜í≤êÆ
	if(strncmp(m_sCrtFont, "Kamon", 5) == 0){
		time = 2.09;
		m_dXaddmove = -MKAMONADDX;
		m_dYaddmove = mtr.tmDescent;
	}
	else if(sjis == 0x0020){
		time = 1.0;
		m_dXaddmove = 0;
		m_dYaddmove = mtr.tmDescent;
	}
	else{
		time = (DBL)(mtr.tmHeight) / (DBL)(mtr.tmMaxCharWidth);
		m_dXaddmove = 0;
		m_dYaddmove = mtr.tmDescent;
	}
	SelectObject(hDC, oldhF);
	DeleteObject(hF);

	sizew = (int)((DBL)sizew * time);
	if(0x0100 < sjis){
		hF = CreateFont(sizeh, sizew, 0, 0, bold, itric, 0, 0,
				//SHIFTJIS_CHARSET,OUT_TT_PRECIS,
				DEFAULT_CHARSET,OUT_TT_PRECIS,
				CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
				//FIXED_PITCH | FF_DONTCARE, m_sCrtFont);
				DEFAULT_PITCH | FF_DONTCARE,m_sCrtFont);
	}
	else{
		hF = CreateFont(sizeh, sizew, 0, 0, bold, itric, 0, 0,
				//ANSI_CHARSET,OUT_TT_PRECIS,
				DEFAULT_CHARSET,OUT_TT_PRECIS,
				CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
				//FIXED_PITCH | FF_DONTCARE, m_sCrtFont);
				DEFAULT_PITCH | FF_DONTCARE,m_sCrtFont);
	}

	oldhF = (HFONT)SelectObject(hDC, hF);

	m_lGlyphLen = GetGlyphOutline(hDC, sjis, GGO_NATIVE, 
				&gm, 0, NULL, &mt);
	if(m_lGlyphLen == -1){
		m_lGlyphLen = 0;
		SelectObject(hDC, oldhF);
		DeleteObject(hF);
		return;
	}
	if(m_lGlyphLen > m_lAllocGlyphLen){
		GlobalUnlock(m_hGlyphBuf);
		m_hGlyphBuf = GlobalReAlloc(m_hGlyphBuf, m_lGlyphLen, GHND);
		if(m_hGlyphBuf == 0){
			m_lGlyphLen = 0;
			SelectObject(hDC, oldhF);
			DeleteObject(hF);
			return;
		}
		m_lAllocGlyphLen = m_lGlyphLen;
		m_lpGlyphBuf = (BYTE FAR *)GlobalLock(m_hGlyphBuf);
		if(m_lpGlyphBuf == NULL){
			m_lGlyphLen = 0;
			SelectObject(hDC, oldhF);
			DeleteObject(hF);
			return;
		}
	}

#ifdef DBG
	sprintf(dbgmsgbuf, "<%s> %x m_lGlyphLen %ld(0x%04x)", m_sCrtFont, sjis, m_lGlyphLen, m_lGlyphLen);
	hmsgdisp(dbgmsgbuf);
#endif

	if(m_lGlyphLen < 0){
		sprintf(msgbuf, "<%s> 0x%04XÇÕïœä∑Ç≈Ç´Ç‹ÇπÇÒ", m_sCrtFont, sjis);
		MessageBox( NULL, msgbuf, NULL, MB_OK );
		m_lGlyphLen = 0;
		SelectObject(hDC, oldhF);
		DeleteObject(hF);
		return;
	}

	m_lGlyphLen = GetGlyphOutline(hDC, sjis, GGO_NATIVE, 
			&gm, m_lGlyphLen, (void FAR *)m_lpGlyphBuf, &mt);

#ifdef DBG
	sprintf(dbgmsgbuf, "Boxs %d %d Org %d %d Inc %d %d", 
			gm.gmBlackBoxX,	gm.gmBlackBoxY, 
			gm.gmptGlyphOrigin.x, gm.gmptGlyphOrigin.y, 
			gm.gmCellIncX,	gm.gmCellIncY); 
	MessageBox( NULL, dbgmsgbuf, NULL, MB_OK );
#endif

	SelectObject(hDC, oldhF);
	DeleteObject(hF);
}

void CWNTTFont::GlyphToFSF1Trns()
{
	LPTTPOLYGONHEADER	lppolyhead;
	LPTTPOLYCURVE		lppolycurve;
	LPPOINTFX		lpcood;
	int				idx, sidx;
	int				fxcnt;

	PointSetStart();
	idx = 0;
	while(1){
		if(idx >= m_lGlyphLen){
			break;
		}
		lppolyhead = (LPTTPOLYGONHEADER)&(m_lpGlyphBuf[idx]);
		MovetoSet(&(lppolyhead->pfxStart));
		sidx = idx;
		idx += sizeof(TTPOLYGONHEADER);
		while(1){
			lppolycurve = (LPTTPOLYCURVE)&(m_lpGlyphBuf[idx]);
			fxcnt = lppolycurve->cpfx;
			lpcood = lppolycurve->apfx;

			switch(lppolycurve->wType){
			case TT_PRIM_LINE:
				PolygonSet(lpcood, fxcnt);
				break;
			case TT_PRIM_QSPLINE:
				if(fxcnt == 2){
					ConicSet(lpcood, fxcnt);
				}
				else if(fxcnt == 3){
					BezeSet(lpcood, fxcnt);
				}
				else{
					PolyCurveSet(lpcood, fxcnt);
				}
				break;
			}
			idx += (sizeof(short)*2 + sizeof(POINTFX)*(lppolycurve->cpfx));
			if((int)(lppolyhead->cb) <= (idx - sidx)){
				break;
			}
		}
		LastjoinSet();
	}
	PointSetEnd();
}

void CWNTTFont::PointSetStart()
{
	m_pcFSF1Font->PointSetStart();
}

void CWNTTFont::PointSetEnd()
{
	m_pcFSF1Font->PointSetEnd();
}

void CWNTTFont::MovetoSet(LPPOINTFX lpcood)
{
	DBL		dx, dy;

	m_dCrtx = lpcood->x.value;
	m_dCrty = lpcood->y.value;
	m_dRsx = m_dCrtx; m_dRsy = m_dCrty;
	dx = m_dCrtx; dy = m_dCrty;
	TTFcoodToSF1cood(&dx, &dy);
	m_pcFSF1Font->MovePointSet(dx, dy);
}

void CWNTTFont::PolygonSet(LPPOINTFX lpcood, WORD count)
{
	WORD	idx;
	DBL		x1, y1, x2, y2;

	idx = 0;
	while(1){
		x1 = m_dCrtx;
		y1 = m_dCrty;
		x2 = lpcood[idx].x.value;
		y2 = lpcood[idx].y.value;
		m_dCrtx = x2;
		m_dCrty = y2;
		TTFcoodToSF1cood(&x1, &y1);
		TTFcoodToSF1cood(&x2, &y2);
		m_pcFSF1Font->LinePointSet(x1, y1, x2, y2);
		idx++;
		if(idx >= count){
			break;
		}
	}
}

void CWNTTFont::ConicSet(LPPOINTFX lpcood, WORD count)
{
	WORD	idx;
	double	x1, y1, x2, y2, x3, y3, x4, y4;
	double	xt, yt;

	idx = 0;
	while(1){
		x1 = m_dCrtx; y1 = m_dCrty;
		xt = lpcood[idx].x.value;
		yt = lpcood[idx].y.value;
		x4 = lpcood[idx+1].x.value;
		y4 = lpcood[idx+1].y.value;
		x2 = (xt - x1) * 4.0 / 6.0 + x1;
		y2 = (yt - y1) * 4.0 / 6.0 + y1;
		x3 = (xt - x4) * 4.0 / 6.0 + x4;
		y3 = (yt - y4) * 4.0 / 6.0 + y4;
		m_dCrtx = x4; m_dCrty = y4;

		TTFcoodToSF1cood(&x1, &y1);
		TTFcoodToSF1cood(&x2, &y2);
		TTFcoodToSF1cood(&x3, &y3);
		TTFcoodToSF1cood(&x4, &y4);
		m_pcFSF1Font->BezePointSet(x1, y1, x2, y2, x3, y3, x4, y4);

		idx += 2;
		if(idx >= count){
			break;
		}
	}

}

void CWNTTFont::BezeSet(LPPOINTFX lpcood, WORD count)
{
	WORD	idx;
	double	x1, y1, x2, y2, x3, y3, x4, y4;

	idx = 0;
	while(1){
		x1 = m_dCrtx; y1 = m_dCrty;
		x2 = lpcood[idx].x.value;
		y2 = lpcood[idx].y.value;
		x3 = lpcood[idx+1].x.value;
		y3 = lpcood[idx+1].y.value;
		x4 = lpcood[idx+2].x.value;
		y4 = lpcood[idx+2].y.value;
		x2 = (x2 - x1) * m_dBezeaddtime + x1;
		y2 = (y2 - y1) * m_dBezeaddtime + y1;
		x3 = (x3 - x4) * m_dBezeaddtime + x4;
		y3 = (y3 - y4) * m_dBezeaddtime + y4;
		m_dCrtx = x4; m_dCrty = y4;

		TTFcoodToSF1cood(&x1, &y1);
		TTFcoodToSF1cood(&x2, &y2);
		TTFcoodToSF1cood(&x3, &y3);
		TTFcoodToSF1cood(&x4, &y4);
		m_pcFSF1Font->BezePointSet(x1, y1, x2, y2, x3, y3, x4, y4);

		idx += 3;
		if(idx >= count){
			break;
		}
	}
}

void CWNTTFont::PolyCurveSet(LPPOINTFX lpcood, WORD count)
{
	WORD	idx;
	DBL		x1, y1, x2, y2, x3, y3;

	idx = 0;
	x1 = m_dCrtx;
	y1 = m_dCrty;
	x2 = lpcood[idx].x.value;
	y2 = lpcood[idx].y.value;
	idx++;
	m_pcFSF1Font->CurveToStartPointSet();
	while(1){
		x3 = lpcood[idx].x.value;
		y3 = lpcood[idx].y.value;
		B_splinePointSet(x1, y1, x2, y2, x3, y3);
		idx++;
		x1 = x2;
		y1 = y2;
		x2 = x3;
		y2 = y3;
		if(idx >= count){
			break;
		}
	}
	m_dCrtx = x3;
	m_dCrty = y3;
	TTFcoodToSF1cood(&x3, &y3);
	m_pcFSF1Font->CurveToEndPointSet(x3, y3);
}

void CWNTTFont::B_splinePointSet(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL		t, t1, b1, b2, b3;
	DBL		tx, ty;

	TTFcoodToSF1cood(&x1, &y1);
	TTFcoodToSF1cood(&x2, &y2);
	TTFcoodToSF1cood(&x3, &y3);
	t = 0.5;
	t1 = 1.0-t;
	b1 = t1*t1/2.0;
	b2 = t*t1+0.5;
	b3 = t*t/2.0;
	tx = b1*x1 + b2*x2 + b3*x3;
	ty = b1*y1 + b2*y2 + b3*y3;
	m_pcFSF1Font->CurveToPointSet(tx, ty);
}

void CWNTTFont::LastjoinSet()
{
	DBL		x1, y1, x2, y2;

	x1 = m_dCrtx; y1 = m_dCrty;
	x2 = m_dRsx; y2 = m_dRsy;
	TTFcoodToSF1cood(&x1, &y1);
	TTFcoodToSF1cood(&x2, &y2);
	if(m_dCrtx == m_dRsx && m_dCrty == m_dRsy){
		m_pcFSF1Font->RejionSet();
	}
	else{
		m_pcFSF1Font->LinePointSet(x1, y1, x2, y2);
		m_pcFSF1Font->RejionSet();
		// m_pcFSF1Font->NoRejionSet();
	}
}

void CWNTTFont::TTFcoodToSF1cood(DBL *retx, DBL *rety)
{
	DBL		tx, ty, dx, dy;

	tx = (*retx) * m_dXaddtime + m_dXaddmove;
	ty = (*rety) * m_dYaddtime + m_dYaddmove;
	if(m_nTateYoko == YOKOGAKI){
		dx = tx  * (DBL)FSF1MESH / (DBL)FTTFMESH;
		dy = ((DBL)FTTFMESH - ty) * (DBL)FSF1MESH / (DBL)FTTFMESH;
	}
	else if(m_nTateYoko == TATEGAKI){
		dx = ty * (DBL)FSF1MESH / (DBL)FTTFMESH;
		dy = tx * (DBL)FSF1MESH / (DBL)FTTFMESH;
	}
	else if(m_nTateYoko == YOKOOKI){
		dx = ty * (DBL)FSF1MESH / (DBL)FTTFMESH;
		dy = tx * (DBL)FSF1MESH / (DBL)FTTFMESH;
		//dx = tx  * (DBL)FSF1MESH / (DBL)FTTFMESH;
		//dy = ((DBL)FTTFMESH - ty) * (DBL)FSF1MESH / (DBL)FTTFMESH;
	}
	else if(m_nTateYoko == TATEOKI){
		dx = tx  * (DBL)FSF1MESH / (DBL)FTTFMESH;
		dy = ((DBL)FTTFMESH - ty) * (DBL)FSF1MESH / (DBL)FTTFMESH;
		//dx = ((DBL)FTTFMESH - ty) * (DBL)FSF1MESH / (DBL)FTTFMESH;
		//dy = ((DBL)FTTFMESH - tx) * (DBL)FSF1MESH / (DBL)FTTFMESH;
	}
	*retx = dx;
	*rety = dy;
}

void CWNTTFont::ParentSet(CWnd *pParent)
{
 	m_pcParent = pParent;
}

