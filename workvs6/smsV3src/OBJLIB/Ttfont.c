//
// Easy Scanner System Ver1.0
//
// ess  ttfont.c  TrueTypeÉtÉHÉìÉgä÷åWä÷êî
//
//  Copyright (c) 1998 by THEON

#include	<windows.h>
#include	<stdio.h>
#include	<math.h>

#include	"..\stdef.h"
#include	"dlldef.h"
#include	"fsf1font.h"

#include	"fsf1font.fnc"
#include	"ttfont.fnc"

#define	XMOVEDOT	0
#define YMOVEDOT	34

#define	FTTFMESH	256

int			m_nPointCount;
HANDLE		m_hGlyphBuf;
LPBYTE		m_lpGlyphBuf;
long		m_lGlyphLen;
char		m_sCrtFont[128];
DBL			m_dCrtx, m_dCrty;
DBL			m_dRsx, m_dRsy;
DBL			m_dBezeaddtime;
DBL			m_dXaddtime, m_dYaddtime;
DBL			m_dXaddmove, m_dYaddmove;

void ttfontInit()
{
	m_hGlyphBuf = GlobalAlloc(GHND, 0x0000FFFF);
	ttfontTrnsElementReset();
}

void ttfontEnd()
{
	GlobalFree(m_hGlyphBuf);
}

void ttfontFontNameSet(LPSTR pFont)
{
	strcpy(m_sCrtFont, pFont);
}

void ttfontBezeAddTimeSet(DBL time)
{
	m_dBezeaddtime = time;
}

void ttfontXYAddTimeSet(DBL xtime, DBL ytime)
{
	m_dXaddtime = xtime;
	m_dYaddtime = ytime;
}

void ttfontXYAddMoveSet(DBL xmove, DBL ymove)
{
	m_dXaddmove = xmove;
	m_dYaddmove = ymove;
}

void ttfontTrnsElementReset()
{
	m_dBezeaddtime = 1.3;
	m_dXaddtime = 1.0;
	m_dYaddtime = 1.0;
	m_dXaddmove = 0.0;
	m_dYaddmove = 0.0;
}

void ttfontSF1TrnsDataLoad(UINT sjis)
{
	HDC	hDC;

	hDC = GetDC(NULL);

	m_lpGlyphBuf = (BYTE FAR *)GlobalLock(m_hGlyphBuf);

	ttfontDataLoad(hDC, sjis, FTTBUFMAX);
	if(m_lGlyphLen <= 16){
		ttfontPointSetStart();
		ttfontPointSetEnd();
	}
	else{
		ttfontGlyphToFSF1Trns();
	}

	GlobalUnlock(m_hGlyphBuf);

	ReleaseDC(NULL, hDC);
}

void ttfontDataLoad(HDC DC, UINT sjis, long checkmax)
{
	GLYPHMETRICS	gm;
	MAT2			mt;
	HFONT			hF, oldhF;
	char			msgbuf[QRECMAX];
	char			fontname[RECMAX];
#ifdef DBG
	char	dbgmsgbuf[RECMAX];
#endif

	mt.eM11.value=1; mt.eM11.fract=0; mt.eM12.value=0; mt.eM12.fract=0;
	mt.eM21.value=0; mt.eM21.fract=0; mt.eM22.value=1; mt.eM22.fract=0;

	if(0x0100 < sjis){
		if(m_fTateGaki == ON){
			sprintf(fontname, "@%s", m_sCrtFont);
		}
		else{
			sprintf(fontname, "%s", m_sCrtFont);
		}
		hF = CreateFont(FTTFMESH, FTTFMESH/2, 0, 0, 0, 0, 0, 0,
				//SHIFTJIS_CHARSET,OUT_TT_PRECIS,
				DEFAULT_CHARSET,OUT_TT_PRECIS,
				CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_DONTCARE,fontname);
	}
	else{
		hF = CreateFont(FTTFMESH, FTTFMESH/2, 0, 0, 0, 0, 0, 0,
				//ANSI_CHARSET,OUT_TT_PRECIS,
				DEFAULT_CHARSET,OUT_TT_PRECIS,
				CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_DONTCARE,m_sCrtFont);
	}

	oldhF = (HFONT)SelectObject(DC, hF);

	m_lGlyphLen = GetGlyphOutline(DC, sjis, GGO_NATIVE, 
				&gm, 0, NULL, &mt);

#ifdef DBG
	sprintf(dbgmsgbuf, "<%s> %x m_lGlyphLen %ld(0x%04x)", m_sCrtFont, sjis, m_lGlyphLen, m_lGlyphLen);
	MessageBox( NULL, dbgmsgbuf, NULL, MB_OK );
#endif

	if(m_lGlyphLen < 0 || checkmax <= m_lGlyphLen){
		sprintf(msgbuf, "<%s> 0x%04XÇÕïœä∑Ç≈Ç´Ç‹ÇπÇÒ", m_sCrtFont, sjis);
		MessageBox( NULL, msgbuf, NULL, MB_OK );
		m_lGlyphLen = 0;
		return;
	}

	m_lGlyphLen = GetGlyphOutline(DC, sjis, GGO_NATIVE, 
			&gm, (DWORD)checkmax, (void FAR *)m_lpGlyphBuf, &mt);

#ifdef DBG
	sprintf(dbgmsgbuf, "Boxs %d %d Org %d %d Inc %d %d", 
			gm.gmBlackBoxX,	gm.gmBlackBoxY, 
			gm.gmptGlyphOrigin.x, gm.gmptGlyphOrigin.y, 
			gm.gmCellIncX,	gm.gmCellIncY); 
	MessageBox( NULL, dbgmsgbuf, NULL, MB_OK );
#endif

	SelectObject(DC, oldhF);
	DeleteObject(hF);
}

void ttfontGlyphToFSF1Trns()
{
	LPTTPOLYGONHEADER	lppolyhead;
	LPTTPOLYCURVE		lppolycurve;
	LPPOINTFX		lpcood;
	int				idx, sidx;
	int				fxcnt;

	ttfontPointSetStart();
	idx = 0;
	while(1){
		if(idx >= m_lGlyphLen){
			break;
		}
		lppolyhead = (LPTTPOLYGONHEADER)&(m_lpGlyphBuf[idx]);
		ttfontMovetoSet(&(lppolyhead->pfxStart));
		sidx = idx;
		idx += sizeof(TTPOLYGONHEADER);
		while(1){
			lppolycurve = (LPTTPOLYCURVE)&(m_lpGlyphBuf[idx]);
			fxcnt = lppolycurve->cpfx;
			lpcood = lppolycurve->apfx;

			switch(lppolycurve->wType){
			case TT_PRIM_LINE:
				ttfontPolygonSet(lpcood, fxcnt);
				break;
			case TT_PRIM_QSPLINE:
				if(fxcnt == 2){
					ttfontConicSet(lpcood, fxcnt);
				}
				else if(fxcnt == 3){
					ttfontBezeSet(lpcood, fxcnt);
				}
				else{
					ttfontPolyCurveSet(lpcood, fxcnt);
				}
				break;
			}
			idx += (sizeof(short)*2 + sizeof(POINTFX)*(lppolycurve->cpfx));
			if((int)(lppolyhead->cb) <= (idx - sidx)){
				break;
			}
		}
		ttfontLastjoinSet();
	}
	ttfontPointSetEnd();
}

void ttfontPointSetStart()
{
	fsf1fontPointSetStart();
}

void ttfontPointSetEnd()
{
	fsf1fontPointSetEnd();
}

void ttfontMovetoSet(LPPOINTFX lpcood)
{
	DBL		dx, dy;

	m_dCrtx = lpcood->x.value;
	m_dCrty = lpcood->y.value;
	m_dRsx = m_dCrtx; m_dRsy = m_dCrty;
	dx = m_dCrtx; dy = m_dCrty;
	ttfontTTFcoodToSF1cood(&dx, &dy);
	fsf1fontMovePointSet(dx, dy);
}

void ttfontPolygonSet(LPPOINTFX lpcood, int count)
{
	int		idx;
	DBL		x1, y1, x2, y2;

	idx = 0;
	while(1){
		x1 = m_dCrtx;
		y1 = m_dCrty;
		x2 = lpcood[idx].x.value;
		y2 = lpcood[idx].y.value;
		m_dCrtx = x2;
		m_dCrty = y2;
		ttfontTTFcoodToSF1cood(&x1, &y1);
		ttfontTTFcoodToSF1cood(&x2, &y2);
		fsf1fontLinePointSet(x1, y1, x2, y2);
		idx++;
		if(idx >= count){
			break;
		}
	}
}

void ttfontConicSet(LPPOINTFX lpcood, int count)
{
	int		idx;
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

		ttfontTTFcoodToSF1cood(&x1, &y1);
		ttfontTTFcoodToSF1cood(&x2, &y2);
		ttfontTTFcoodToSF1cood(&x3, &y3);
		ttfontTTFcoodToSF1cood(&x4, &y4);
		fsf1fontBezePointSet(x1, y1, x2, y2, x3, y3, x4, y4);

		idx += 2;
		if(idx >= count){
			break;
		}
	}
}

void ttfontBezeSet(LPPOINTFX lpcood, int count)
{
	int		idx;
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

		ttfontTTFcoodToSF1cood(&x1, &y1);
		ttfontTTFcoodToSF1cood(&x2, &y2);
		ttfontTTFcoodToSF1cood(&x3, &y3);
		ttfontTTFcoodToSF1cood(&x4, &y4);
		fsf1fontBezePointSet(x1, y1, x2, y2, x3, y3, x4, y4);

		idx += 3;
		if(idx >= count){
			break;
		}
	}
}

void ttfontPolyCurveSet(LPPOINTFX lpcood, int count)
{
	int		idx;
	DBL		x1, y1, x2, y2, x3, y3;

	idx = 0;
	x1 = m_dCrtx;
	y1 = m_dCrty;
	x2 = lpcood[idx].x.value;
	y2 = lpcood[idx].y.value;
	idx++;
	fsf1fontCurveToStartPointSet();
	while(1){
		x3 = lpcood[idx].x.value;
		y3 = lpcood[idx].y.value;
		ttfontB_splinePointSet(x1, y1, x2, y2, x3, y3);
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
	ttfontTTFcoodToSF1cood(&x3, &y3);
	fsf1fontCurveToEndPointSet(x3, y3);
}

void ttfontB_splinePointSet(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL		t, t1, b1, b2, b3;
	DBL		tx, ty;

	ttfontTTFcoodToSF1cood(&x1, &y1);
	ttfontTTFcoodToSF1cood(&x2, &y2);
	ttfontTTFcoodToSF1cood(&x3, &y3);
	t = 0.5;
	t1 = 1.0-t;
	b1 = t1*t1/2.0;
	b2 = t*t1+0.5;
	b3 = t*t/2.0;
	tx = b1*x1 + b2*x2 + b3*x3;
	ty = b1*y1 + b2*y2 + b3*y3;
	fsf1fontCurveToPointSet(tx, ty);
}

void ttfontLastjoinSet()
{
	DBL		x1, y1, x2, y2;

	x1 = m_dCrtx; y1 = m_dCrty;
	x2 = m_dRsx; y2 = m_dRsy;
	ttfontTTFcoodToSF1cood(&x1, &y1);
	ttfontTTFcoodToSF1cood(&x2, &y2);
	if(m_dCrtx == m_dRsx && m_dCrty == m_dRsy){
		fsf1fontRejionSet();
	}
	else{
		fsf1fontLinePointSet(x1, y1, x2, y2);
		fsf1fontRejionSet();
		// fsf1fontNoRejionSet();
	}
}

void ttfontTTFcoodToSF1cood(DBL *retx, DBL *rety)
{
	DBL		tx, ty, dx, dy;

	tx = (*retx) * m_dXaddtime + m_dXaddmove + XMOVEDOT;
	ty = (*rety) * m_dYaddtime + m_dYaddmove + YMOVEDOT;
	if(m_fTateGaki == ON){
		dx = ty * (DBL)FSF1MESH / (DBL)FTTFMESH;
		dy = tx * (DBL)FSF1MESH / (DBL)FTTFMESH;
	}
	else{
		dx = tx  * (DBL)FSF1MESH / (DBL)FTTFMESH;
		dy = ((DBL)FTTFMESH - ty) * (DBL)FSF1MESH / (DBL)FTTFMESH;
	}
	*retx = dx;
	*rety = dy;
}

