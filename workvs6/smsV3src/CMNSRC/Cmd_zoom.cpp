
/////////////////////////////////////////////////////////////////////////////
// CCmdZoom

#include "stdafx.h"
#include "math.h"

#include "cmd_base.h"
#include "cmd_zoom.h"

extern "C" {
	void scrnsetrltvelement();
	void scrnresetzoomtime();
	void scrnenvbasetimezoomup();
	void scrnenvbasetimezoomdown();
	void scrngetmmsize(LPDBL mmx, LPDBL mmy);
	void scrnsetwinarea(int sx, int sy, int ex, int ey);
	void scrninitsizeelement(LPINT retwidth, LPINT rethight);
	void scrnrltvareaget(DBL *sx, DBL *sy, DBL *ex, DBL *ey);
	void scrndlenrltvtomm(LPDBL dw);
	void scrndlenmmtorltv(LPDBL dw);
	void scrndxymmtorltv(LPDBL topx, LPDBL topy);
	void scrndxyrltvtomm(LPDBL x, LPDBL y);
	void scrnsetrltvtop(DBL topx, DBL topy);
	void scrngetrltvtop(LPDBL xTop, LPDBL yTop);
	DBL scrnsetxytime(DBL timex, DBL timey);
}

CCmdZoom::CCmdZoom(CScrollView* pcview)
:CCmdBase(pcview)
{
	//m_pcView = (CTonView*)pcview;
}

UINT CCmdZoom::ExecZoomAll()
{
	CRect	rectClient;
	int		w, h;

	m_pcView->GetClientRect(&rectClient);
	rectClient.top = rectClient.top+m_pcView->m_nBaseYTop;
	rectClient.left = rectClient.left+m_pcView->m_nBaseXTop;
	h = rectClient.bottom - rectClient.top;
	w = rectClient.right - rectClient.left;
	scrnsetwinarea(rectClient.left, rectClient.top, rectClient.right, rectClient.bottom);
	scrninitsizeelement(&w, &h);
	scrnresetzoomtime();
	scrnsetrltvelement();
	scrnsetrltvtop(0.0, 0.0);
	m_pcView->InitScrollElement();
	return(ENDEVENT);
}

UINT CCmdZoom::ExecZoomIn(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		m_pcView->m_nCursorKind = CUSKIND_ZOOMIN;
		::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_nMouseSetCount = 1;
			m_dSX = dx; m_dSY = dy;
			m_dEX = dx; m_dEY = dy;
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		else if(m_nMouseSetCount == 1){
			m_nMouseSetCount = 0;
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
			m_dEX = dx; m_dEY = dy;
			if(m_dEX == m_dSX || m_dEY == m_dSY){
				scrnenvbasetimezoomup();
				scrnsetrltvelement();
			}
			else{
				AreaInTimeLegal();
				scrnsetrltvelement();
			}
			dx = (m_dEX+m_dSX) / 2;
			dy = (m_dEY+m_dSY) / 2;
			((CTonView*)m_pcView)->InitScrollElement();
			ScrollAbsCntrSet(dx, dy);
			((CTonView*)m_pcView)->InitScrollElement();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			m_dEX = dx; m_dEY = dy;
			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 0){
			scrnenvbasetimezoomdown();
			scrnsetrltvelement();
			((CTonView*)m_pcView)->InitScrollElement();
			ScrollAbsCntrSet(dx, dy);
			((CTonView*)m_pcView)->InitScrollElement();
			//return(REDRAWEVENT);
		}
		else{
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		break;
	case ENDEVENT:
		m_pcView->m_nCursorKind = CUSKIND_ARROW;
		::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdZoom::ExecZoomOut(UINT event, DBL dx, DBL dy)
{
	CRect	rectClient;

	if(event == INITEVENT){
		m_pcView->GetClientRect(&rectClient);
		dx = (rectClient.right - rectClient.left)/2;
		dy = (rectClient.bottom - rectClient.top)/2;
		scrndxyrltvtomm(&dx, &dy);
		scrnenvbasetimezoomdown();
		scrnsetrltvelement();
		((CTonView*)m_pcView)->InitScrollElement();
		ScrollAbsCntrSet(dx, dy);
		((CTonView*)m_pcView)->InitScrollElement();
	}
	return(ENDEVENT);
}


void CCmdZoom::ScrollAbsCntrSet(DBL dx, DBL dy)
{
	CRect	rectClient;
	DBL		dw, dh;
	DBL		topx, topy;
	DBL		ex, ey;
	DBL		mmx, mmy;

	m_pcView->GetClientRect(&rectClient);

	// 2001.11.21 立身　ズーム時に指定がズレルを修正
	//dh = rectClient.bottom - rectClient.top - 16;
	//dw = rectClient.right - rectClient.left - 16;
	dh = rectClient.bottom - rectClient.top;
	dw = rectClient.right - rectClient.left;
	scrngetmmsize(&mmx, &mmy);
	scrndlenrltvtomm(&dw);
	scrndlenrltvtomm(&dh);
	dy = mmy - dy;
	topx = dx - dw/2;
	topy = dy - dh/2;
	if(topx < 0){
		topx = 0;
	}
	if(topy < 0){
		topy = 0;
	}
	ex = topx + dw;
	ey = topy + dh;
	if(mmx < ex){
		topx = mmx - dw;
	}
	if(topx < 0){
		topx = 0;
	}

	if(mmy < ey){
		topy = mmy - dh;
	}
	if(topy < 0){
		topy = 0;
	}

	scrnsetrltvtop(0.0, 0.0);
	scrndlenmmtorltv(&topx);
	scrndlenmmtorltv(&topy);
	scrnsetrltvtop(topx, topy);
sprintf(m_szMsgBuf, "%1.1lf %1.1lf\n", topx, topy);
TRACE(m_szMsgBuf);
}

void CCmdZoom::AreaInTimeLegal()
{
	CRect	rectClient;
	DBL		dw, dh;
	DBL		dabsw, dabsh;
	DBL		timex, timey;
	DBL		dmmxsize, dmmysize;
	DBL		rltvxsize, rltvysize;

	m_pcView->GetClientRect(&rectClient);

	dh = abs(rectClient.bottom - rectClient.top);
	dw = abs(rectClient.right - rectClient.left);
	dabsw = fabs(m_dEX - m_dSX);
	dabsh = fabs(m_dEY - m_dSY);

	scrngetmmsize(&dmmxsize, &dmmysize);
	timex = dabsw / dw;
	timey = dabsh / dh;
	if(timex > timey){
		rltvxsize = dmmxsize / timex;
		rltvysize = dmmysize / timex;
		if(rltvxsize >= AREAMAXLIMIT || rltvysize >= AREAMAXLIMIT){
			return;
		}
		scrnsetxytime(timex, timex);
	}
	else{
		rltvxsize = dmmxsize / timey;
		rltvysize = dmmysize / timey;
		if(rltvxsize >= AREAMAXLIMIT || rltvysize >= AREAMAXLIMIT){
			return;
		}
		scrnsetxytime(timey, timey);
	}
}

