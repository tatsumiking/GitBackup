
/////////////////////////////////////////////////////////////////////////////
// CCmdKamonCrct

#include "stdafx.h"
#include "winuser.h"
#include "math.h"
#include "io.h"

#include "cmdkcrct.h"

#include	"DlgKmnLd.h"

#define	XMOVEDOT	0
#define YMOVEDOT	34

extern "C" {
	void baseaddsafix(LPSTR FileName, LPSTR Safix);
	void basechangesafix(LPSTR FileName, LPSTR Safix);
	void scrnsetdelpen(HDC PaintDC);
	void scrnsetslctpen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	void scrnsetrltvelement();
	void scrnresetzoomtime();
	void scrndxymmtorltv(LPDBL x, LPDBL y);
	void scrndxyrltvtomm(LPDBL x, LPDBL y);
	void scrndlenmmtorltv(LPDBL len);
	void scrngetrltvtop(LPDBL xTop, LPDBL yTop);
	void scrngetmmsize(LPDBL pdmmxsize, LPDBL pdmmysize);
	int DispatchWindows(HWND hWin);
}

CCmdKamonCrct::CCmdKamonCrct(CScrollView* pcview)
:CCmdBase(pcview)
{
	
}

UINT CCmdKamonCrct::ExecKamonLoad(UINT event, DBL dx, DBL dy)
{
	CDialogKamonLoad*	pDialogKamonLoad;
	int		ret;
	DBL		size;
	char	szkmnchar[QRECMAX];
	char	szkmnfont[RECMAX];

	if(event != INITEVENT){
		return(0);
	}
	pDialogKamonLoad = new CDialogKamonLoad(m_pcView);
	pDialogKamonLoad->m_dKmnSize = 100.0;
	ret = pDialogKamonLoad->DoModal();
	if(ret == IDOK){
		size = pDialogKamonLoad->m_dKmnSize;
		strcpy(szkmnchar, pDialogKamonLoad->m_szKamonChar);
		strcpy(szkmnfont, pDialogKamonLoad->m_szKamonFont);
		szkmnfont[0] = 'K';
		KamonLoadSetVect(size, szkmnfont, szkmnchar);
		ret = SETEVENT;
	}
	else{
		ret = ENDEVENT;
	}
	delete(pDialogKamonLoad);
	return(ret);
}

void CCmdKamonCrct::KamonLoadSetVect(DBL size, LPSTR fontname, LPSTR lpch)
{
	CTonDoc*	pcDoc;
	UINT		code;

	code = (*lpch) & 0x00ff;
	if(code < 0x30 || 0x7f < code){
		return;
	}

	m_dBezeaddtime = 1.3;
	m_dXaddtime = 1.0;
	m_dYaddtime = 1.0;
	m_dXaddmove = 0.0;
	m_dYaddmove = 0.0;

	m_dTrnsTime = size / FSF1MESH;

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->OffFlag(ACTIVEFLAG);
	m_pcWNTTFont = pcDoc->m_pcWNTTFont;
	m_hGlyphBuf = m_pcWNTTFont->m_hGlyphBuf;
	m_lpGlyphBuf = (BYTE FAR *)GlobalLock(m_hGlyphBuf);
	if(m_lpGlyphBuf == NULL){
		return;
	}
	m_pcWNTTFont->m_lpGlyphBuf = m_lpGlyphBuf;
	m_pcWNTTFont->FontNameSet(fontname);
	CDataList* pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	BeginDraw();
	m_pcWNTTFont->DataLoad(m_pcDC->m_hDC, code);
	EndDraw();
	m_lpGlyphBuf = m_pcWNTTFont->m_lpGlyphBuf;
	m_lGlyphLen = m_pcWNTTFont->m_lGlyphLen;

	TTGlyphToDataList(pcDataList);
	pcDataList->RenewMiniMax();

	if(m_lpGlyphBuf != NULL){
		GlobalUnlock(m_hGlyphBuf);
	}
	pcDoc->DataAddTail((CData*)pcDataList);
}

void CCmdKamonCrct::TTGlyphToDataList(CDataList *pcDataList)
{
	CVect	*pcVect;
	int		dp;
	LPTTPOLYGONHEADER	lppolyhead;
	LPTTPOLYCURVE		lppolycurve;
	LPPOINTFX		lpcood;
	int				idx, sidx;
	int				fxcnt;

	idx = 0;
	while(1){
		if(idx >= m_lGlyphLen){
			break;
		}
		pcVect = new CVect(0);
		dp = 0;
		lppolyhead = (LPTTPOLYGONHEADER)&(m_lpGlyphBuf[idx]);
		dp = MovetoSet(pcVect, dp, &(lppolyhead->pfxStart));
		sidx = idx;
		idx += sizeof(TTPOLYGONHEADER);
		while(1){
			lppolycurve = (LPTTPOLYCURVE)&(m_lpGlyphBuf[idx]);
			fxcnt = lppolycurve->cpfx;
			lpcood = lppolycurve->apfx;

			switch(lppolycurve->wType){
			case TT_PRIM_LINE:
				dp = PolygonSet(pcVect, dp, lpcood, fxcnt);
				break;
			case TT_PRIM_QSPLINE:
				if(fxcnt == 2){
					dp = ConicSet(pcVect, dp, lpcood, fxcnt);
				}
				else if(fxcnt == 3){
					dp = BezeSet(pcVect, dp, lpcood, fxcnt);
				}
				else{
					dp = PolyCurveSet(pcVect, dp, lpcood, fxcnt);
				}
				break;
			}
			idx += (sizeof(short)*2 + sizeof(POINTFX)*(lppolycurve->cpfx));
			if((int)(lppolyhead->cb) <= (idx - sidx)){
				break;
			}
		}
		LastjoinSet(pcVect, dp);
		pcDataList->SetData(pcVect);
	}
}

int CCmdKamonCrct::MovetoSet(CVect *pcVect, int dp, LPPOINTFX lpcood)
{
	DBL		dx, dy;

	m_dCrtx = lpcood->x.value;
	m_dCrty = lpcood->y.value;
	m_dRsx = m_dCrtx; m_dRsy = m_dCrty;
	dx = m_dCrtx; dy = m_dCrty;
	TrnsPoint(&dx, &dy);
	m_dVectCrtX = dx; m_dVectCrtY = dy;
	return(dp);
}

int CCmdKamonCrct::PolygonSet(CVect *pcVect, int dp, LPPOINTFX lpcood, WORD count)
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
		TrnsPoint(&x1, &y1);
		TrnsPoint(&x2, &y2);
		pcVect->SetAtrDXY(dp++, 0, x1, y1);
		m_dVectCrtX = x2; m_dVectCrtY = y2;
		idx++;
		if(idx >= count){
			break;
		}
	}
	return(dp);
}

int CCmdKamonCrct::ConicSet(CVect *pcVect, int dp, LPPOINTFX lpcood, WORD count)
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

		TrnsPoint(&x1, &y1);
		TrnsPoint(&x2, &y2);
		TrnsPoint(&x3, &y3);
		TrnsPoint(&x4, &y4);
		pcVect->SetAtrDXY(dp++, CURVATR|BEZEATR, x1, y1);
		pcVect->SetAtrDXY(dp++, BEZEATR, x2, y2);
		pcVect->SetAtrDXY(dp++, BEZEATR, x3, y3);
		m_dVectCrtX = x4; m_dVectCrtY = y4;
		idx += 2;
		if(idx >= count){
			break;
		}
	}
	return(dp);
}

int CCmdKamonCrct::BezeSet(CVect *pcVect, int dp, LPPOINTFX lpcood, WORD count)
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

		TrnsPoint(&x1, &y1);
		TrnsPoint(&x2, &y2);
		TrnsPoint(&x3, &y3);
		TrnsPoint(&x4, &y4);
		pcVect->SetAtrDXY(dp++, CURVATR|BEZEATR, x1, y1);
		pcVect->SetAtrDXY(dp++, BEZEATR, x2, y2);
		pcVect->SetAtrDXY(dp++, BEZEATR, x3, y3);
		m_dVectCrtX = x4; m_dVectCrtY = y4;

		idx += 3;
		if(idx >= count){
			break;
		}
	}
	return(dp);
}

int CCmdKamonCrct::PolyCurveSet(CVect *pcVect, int dp, LPPOINTFX lpcood, WORD count)
{
	WORD	idx;
	DBL		x1, y1, x2, y2, x3, y3;

	idx = 0;
	x1 = m_dCrtx;
	y1 = m_dCrty;
	x2 = lpcood[idx].x.value;
	y2 = lpcood[idx].y.value;
	idx++;
	pcVect->SetAtrDXY(dp++, CURVATR, m_dVectCrtX, m_dVectCrtX);
	while(1){
		x3 = lpcood[idx].x.value;
		y3 = lpcood[idx].y.value;
		dp = B_splinePointSet(pcVect, dp, x1, y1, x2, y2, x3, y3);
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
	TrnsPoint(&x3, &y3);
	m_dVectCrtX = x3; m_dVectCrtY = y3;
	return(dp);
}

int CCmdKamonCrct::B_splinePointSet(CVect *pcVect, int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL		t, t1, b1, b2, b3;
	DBL		tx, ty;

	TrnsPoint(&x1, &y1);
	TrnsPoint(&x2, &y2);
	TrnsPoint(&x3, &y3);
	t = 0.5;
	t1 = 1.0-t;
	b1 = t1*t1/2.0;
	b2 = t*t1+0.5;
	b3 = t*t/2.0;
	tx = b1*x1 + b2*x2 + b3*x3;
	ty = b1*y1 + b2*y2 + b3*y3;
	pcVect->SetAtrDXY(dp++, CURVATR|CONTATR, tx, ty);
	m_dVectCrtX = tx; m_dVectCrtY = ty;
	return(dp);
}

void CCmdKamonCrct::LastjoinSet(CVect *pcVect, int dp)
{
	DBL		xx, yy;

	xx = m_dRsx; yy = m_dRsy;
	TrnsPoint(&xx, &yy);
	pcVect->SetAtrDXY(dp++, 0, m_dVectCrtX, m_dVectCrtY);
	pcVect->SetAtrDXY(dp++, 0, xx, yy);
	pcVect->SetPointCount(dp);
}

void CCmdKamonCrct::TrnsPoint(DBL *retx, DBL *rety)
{
	DBL		tx, ty, dx, dy;

	tx = (*retx) * m_dXaddtime + m_dXaddmove + XMOVEDOT;
	ty = (*rety) * m_dYaddtime + m_dYaddmove + YMOVEDOT;
	dx = tx * (DBL)FSF1MESH / (DBL)FTTFMESH;
	dy = ty * (DBL)FSF1MESH / (DBL)FTTFMESH;
	dx = dx * m_dTrnsTime;
	dy = dy * m_dTrnsTime;
	*retx = dx;
	*rety = dy;
}

