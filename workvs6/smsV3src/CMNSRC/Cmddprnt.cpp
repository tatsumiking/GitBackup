
/////////////////////////////////////////////////////////////////////////////
// CCmdDivPrint

#include "stdafx.h"
#include "math.h"

#include "cmd_base.h"
#include "cmddprnt.h"
#include "DlPrSize.h"
#include "dlgplout.h"

#define	OUTSPACEMM	0.0

extern "C" {
	void Dispatch();
	int curveperpendicular(DBL sx, DBL sy, DBL ex, DBL ey, DBL x, DBL y, DBL *cx, DBL *cy);
	void scrnsetslctdotpen(HDC PaintDC);
	void scrnsetslctpen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	void scrnsetwintop(DBL topx, DBL topy);
	void scrnsetrltvtop(DBL xTop, DBL yTop);
	void scrngetrltvtop(LPDBL xTop, LPDBL yTop);
	void senvgetexepath(LPSTR path);
	int senvchckflag(int flag);
	void senvgetdatpath(LPSTR path);
	void senvonflag(int flag);
	void senvoffflag(int flag);
	void scrnsetslctpen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	int scrngetcntrmoveflag();
	int scrngetstartmoveflag();
	void scrngetstartmovemm(LPDBL xmm, LPDBL ymm);
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
	void scrndxymmtorltv(LPDBL x, LPDBL y);
}

CCmdDivPrint::CCmdDivPrint(CScrollView* pcview)
:CCmdBase(pcview)
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	char	buf[HRECMAX];
	FILE	*fp;

	m_dPrintXsize = INTMAX;
	m_dPrintYsize = INTMAX;
	m_dNoriSiro = 0.0;
	m_nCheckFlag = FALSE;
	m_nAreaBoxsOutFlag = FALSE;
	senvgetexepath(exepath);
	sprintf(fname, "%s\\defenv\\cmddprnt.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf %lf", &m_dPrintXsize, &m_dPrintYsize);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf", &m_dNoriSiro);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &m_nCheckFlag);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &m_nAreaBoxsOutFlag);
		fclose(fp);
	}
}

CCmdDivPrint::~CCmdDivPrint()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\defenv\\cmddprnt.env", exepath);
	if((fp = fopen(fname, "wr")) != NULL){
		fprintf(fp, "%4.1lf %4.1lf\n", m_dPrintXsize, m_dPrintYsize);
		fprintf(fp, "%4.1lf\n", m_dNoriSiro);
		fprintf(fp, "%d\n", m_nCheckFlag);
		fprintf(fp, "%d\n", m_nAreaBoxsOutFlag);
		fclose(fp);
	}
}

UINT CCmdDivPrint::ExecDivPrint(UINT event, DBL dx, DBL dy)
{
	CDialogPrintSize *pcDialogPrintSize;
	CDC	cDC;
	DBL	dmmxsize, dmmysize;
	int		ret;
	HANDLE	hDevMode;
	CTonApp *pcApp;
	int		i;

	switch(event){
	case INITEVENT:
		pcApp = (CTonApp*)AfxGetApp();
		hDevMode = pcApp->GetDevMode();
		if(hDevMode == 0){
			CPrintDialog pd(TRUE);
			pd.m_pd.Flags |= PD_RETURNDEFAULT;
			pcApp->DoPrintDialog(&pd);
		}
		pcApp->CreatePrinterDC(cDC);
		m_dPrintMaxXsize = cDC.GetDeviceCaps(HORZSIZE);
		m_dPrintMaxYsize = cDC.GetDeviceCaps(VERTSIZE);
		cDC.DeleteDC();
		/*
		if(m_dPrintMaxXsize < m_dPrintXsize
		|| m_dPrintMaxYsize < m_dPrintYsize){
			m_dPrintXsize = m_dPrintMaxXsize;
			m_dPrintYsize = m_dPrintMaxYsize;
		}
		*/
		m_dPrintXsize = m_dPrintMaxXsize;
		m_dPrintYsize = m_dPrintMaxYsize;
		m_dNoriSiro = 0;
		hDevMode = pcApp->GetDevMode();
		scrngetmmsize(&dmmxsize, &dmmysize);
		if(m_dPrintMaxXsize < dmmxsize || m_dPrintMaxYsize < dmmysize){
			sprintf(m_szMsgBuf, "有効印刷領域が %4.1lfmm x %4.1lfmm と小さいですがサイズを変更しますか", m_dPrintXsize, m_dPrintYsize);
			pcDialogPrintSize = new CDialogPrintSize(m_pcView);
			pcDialogPrintSize->m_csSizeMsg = _T(m_szMsgBuf);
			ret = pcDialogPrintSize->DoModal();
			delete(pcDialogPrintSize);
			switch(ret){
			case IDOK:
				m_pcView->m_nScaleTrns = ON;
				ret = SETEVENT;
				break;
			case IDNO:
				m_pcView->m_nScaleTrns = OFF;
				ret = 0;
				break;
			case IDCANCEL:
				m_pcView->m_nScaleTrns = OFF;
				ret = SETEVENT;
				break;
			}
			if(ret == SETEVENT){
				m_pcView->m_nDivPageLast = 1;
				m_pcView->m_nDivMaxX = 1;
				m_pcView->m_nDivMaxY = 1;
				m_pcView->m_dDivSX[0] = 0;
				m_pcView->m_dDivSY[0] = dmmysize;
				m_pcView->m_dDivSX[1] = dmmxsize;
				m_pcView->m_dDivSY[1] = 0;
				m_pcDialogDivPrint = NULL;
				return(SETEVENT);
			}
		}

		m_pcDialogDivPrint = new CDialogDivPrint(m_pcView);
		m_pcDialogDivPrint->m_dPrintXSize = m_dPrintXsize; 
		m_pcDialogDivPrint->m_dPrintYSize = m_dPrintYsize; 
		m_pcDialogDivPrint->m_dNorisiro = m_dNoriSiro;
		if(m_pcDialogDivPrint->Create(IDD_DIVPRINT, m_pcView)){
			LocateDivPrintDlg();
		}
		senvoffflag(STOPFLAG);
		if(DivPrintAreaInit() == FALSE){
			m_dNoriSiro = 0;
			m_pcDialogDivPrint->m_dNorisiro = m_dNoriSiro;
			m_pcDialogDivPrint->UpdateData(FALSE);
			if(DivPrintAreaInit() == FALSE){
				AfxMessageBox("分割数が多すぎます");
				m_pcDialogDivPrint->DestroyWindow();
				return(ENDEVENT);
			}
		}
		SortAreaLine();
		DrawAreaLine();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			DrawAreaLine();
			SlctCrtAreaLine(dx, dy);
			DrawAreaLine();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			DrawAreaLine();
			SetCrtAreaLine(dx, dy);
			SortAreaLine();
			DrawAreaLine();
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			DrawAreaLine();
			SetCrtAreaLine(dx, dy);
			DrawAreaLine();
		}
		break;
	case REDRAWEVENT:
		/*
		((CTonView*)m_pcView)->m_pcDC->SetROP2(R2_XORPEN);
		DrawDivAreaLine(((CTonView*)m_pcView)->m_pcDC);
		((CTonView*)m_pcView)->m_pcDC->SetROP2(R2_COPYPEN);
		*/
		//DrawAreaLine();
		break;
	case DLGSETEVENT:
		if(m_nMouseSetCount == 0){
			m_nMouseSetCount = 2;
			DrawAreaLine();
			m_pcDialogDivPrint->DestroyWindow();
			for(i = 0; i <= DIVLINEMAX; i++){
				m_pcView->m_dDivSX[i] = m_tXdiv[i];
				m_pcView->m_dDivSY[i] = m_tYdiv[i];
			}
			m_pcView->m_nDivMaxX = m_nDivXCnt;
			m_pcView->m_nDivMaxY = m_nDivYCnt;
			m_pcView->m_nDivPageLast = m_nDivXCnt*m_nDivYCnt;

			m_nMouseSetCount = 0;
			return(SETEVENT);
		}
		else if(m_nMouseSetCount == 1){
			DrawAreaLine();
			ResetCrtAreaLine(dx, dy);
			DrawAreaLine();
			m_nMouseSetCount = 0;
		}
		break;
	case DLGRESETEVENT:
		if(m_nMouseSetCount == 0
		|| m_nMouseSetCount == 1){
			DrawAreaLine();
			m_dPrintXsize = m_pcDialogDivPrint->m_dPrintXSize;
			m_dPrintYsize = m_pcDialogDivPrint->m_dPrintYSize;
			m_dNoriSiro = m_pcDialogDivPrint->m_dNorisiro;
			if(DivPrintAreaInit() == FALSE){
				m_dNoriSiro = 0;
				m_pcDialogDivPrint->m_dNorisiro = m_dNoriSiro;
				m_pcDialogDivPrint->UpdateData(FALSE);
				DivPrintAreaInit();
			}
			SortAreaLine();
			DrawAreaLine();
			m_nMouseSetCount = 0;
		}
		break;
	case ENDEVENT:
		if(m_nMouseSetCount == 0
		|| m_nMouseSetCount == 1){
			DrawAreaLine();
			m_nMouseSetCount = 0;
			if(m_pcDialogDivPrint != NULL){
				m_pcDialogDivPrint->DestroyWindow();
			}
			return(ENDEVENT);
		}
		break;
	}
	return(0);

}

void CCmdDivPrint::LocateDivPrintDlg()
{
	CTonApp*	theApp;
	CMainFrame*	theWnd;
	CRect	prect;
	CRect	crect;
	int		x, y, cx, cy;

	m_pcView->GetWindowRect(&prect);
	m_pcDialogDivPrint->GetWindowRect(&crect);
	x = prect.right - crect.Width();
	y = prect.top;
	cx = crect.Width();
	cy = crect.Height();

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	m_pcDialogDivPrint->ShowWindow(SW_SHOW);
	::SetWindowPos(m_pcDialogDivPrint->m_hWnd, HWND_TOP, x, y, cx, cy, 0);
}

BOOL CCmdDivPrint::DivPrintAreaInit()
{
	CTonDoc*	pcDoc;
	DBL		minix, miniy, maxx, maxy;
	int		wabsxsize, wabsysize;
	int		i;

	if(m_dPrintXsize <= m_dNoriSiro*2
	|| m_dPrintYsize <= m_dNoriSiro*2){
		return(FALSE);
	}
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	minix = 0; miniy = 0;
	scrngetmmsize(&maxx, &maxy);
	//pcDoc->GetDMiniMax(0, &minix, &miniy, &maxx, &maxy);
	//minix -= OUTSPACEMM; miniy -= OUTSPACEMM;
	//maxx += OUTSPACEMM; maxy += OUTSPACEMM;
	m_dDivBaseX = minix;
	m_dDivBaseY = maxy;
	m_dDivTopX = minix;
	m_dDivTopY = miniy;
	m_dDivEndX = maxx;
	m_dDivEndY = maxy;
	wabsxsize = (int)(m_dDivEndX - m_dDivTopX);
	wabsysize = (int)(m_dDivEndY - m_dDivTopY);
	m_nOneXsize = (int)(m_dPrintXsize - m_dNoriSiro*2);
	m_nOneYsize = (int)(m_dPrintYsize - m_dNoriSiro*2);
	if(m_nOneXsize == 0 || m_nOneYsize == 0){
		return(FALSE);
	}
	m_nDivXCnt = (wabsxsize + m_nOneXsize - 1) / m_nOneXsize;
	if(m_nDivXCnt <= 0){
		m_nDivXCnt = 1;
	}
	m_nDivYCnt = (wabsysize + m_nOneYsize - 1) / m_nOneYsize;
	if(m_nDivYCnt <= 0){
		m_nDivYCnt = 1;
	}
	if(m_nDivYCnt >= DIVLINEMAX
	|| m_nDivYCnt >= DIVLINEMAX){
		return(FALSE);
	}
	for(i = 0; i < m_nDivXCnt; i++){
		m_tXdiv[i] = m_dDivBaseX + i * m_nOneXsize;
	}
	for(i = 0; i < m_nDivYCnt; i++){
		m_tYdiv[i] = m_dDivBaseY - i * m_nOneYsize;
	}
	return(TRUE);
}

void CCmdDivPrint::DrawAreaLine()
{
	BeginDraw();
	DrawModeXOR();
	DrawDivAreaLine(m_pcDC);
	EndDraw();
}

void CCmdDivPrint::DrawDivAreaLine(CDC *pcDC)
{
	int		i, j;
	DBL		sx, sy, ex, ey;

	sx = m_dDivTopX;
	ex = m_dDivEndX;
	sy = m_dDivTopY;
	ey = m_dDivEndY;
	DrawDivBoxs(pcDC, sx, sy, ex, ey);

	sy = m_dDivTopY;
	ey = m_dDivEndY;
	for(i = 1; i < m_nDivXCnt; i++){
		sx = m_tXdiv[i];
		ex = m_tXdiv[i];
		DrawDivLine(pcDC, sx, sy, ex, ey);
		sx = m_tXdiv[i]-m_dNoriSiro;
		ex = m_tXdiv[i]-m_dNoriSiro;
		DrawDivDotLine(pcDC, sx, sy, ex, ey);
		sx = m_tXdiv[i]+m_dNoriSiro;
		ex = m_tXdiv[i]+m_dNoriSiro;
		DrawDivDotLine(pcDC, sx, sy, ex, ey);
	}

	sx = m_dDivTopX;
	ex = m_dDivEndX;
	for(i = 1; i < m_nDivYCnt; i++){
		sy = m_tYdiv[i];
		ey = m_tYdiv[i];
		DrawDivLine(pcDC, sx, sy, ex, ey);
		sy = m_tYdiv[i]-m_dNoriSiro;
		ey = m_tYdiv[i]-m_dNoriSiro;
		DrawDivDotLine(pcDC, sx, sy, ex, ey);
		sy = m_tYdiv[i]+m_dNoriSiro;
		ey = m_tYdiv[i]+m_dNoriSiro;
		DrawDivDotLine(pcDC, sx, sy, ex, ey);
	}

	m_tXdiv[m_nDivXCnt] = m_dDivEndX - m_dNoriSiro;
	m_tYdiv[m_nDivYCnt] = m_dDivTopY + m_dNoriSiro;
	for(i = 0; i < m_nDivYCnt; i++){
		sy = m_tYdiv[i]+m_dNoriSiro;
		ey = m_tYdiv[i+1]-m_dNoriSiro;
		for(j = 0; j < m_nDivXCnt; j++){
			sx = m_tXdiv[j]-m_dNoriSiro;
			ex = m_tXdiv[j+1]+m_dNoriSiro;
			if(fabs(ex - sx) > (m_dPrintXsize+0.1)
			|| fabs(ey - sy) > (m_dPrintYsize+0.1)){
				DrawDivCrossLine(pcDC, sx, sy, ex, ey);
			}
		}
	}
}

void CCmdDivPrint::SlctCrtAreaLine(DBL xx, DBL yy)
{
	int		i;
	DBL		len, tlen;
	DBL		sx, sy, ex, ey;
	DBL		cx, cy;
	DBL		x, y;

	m_nCrtLine = -1;
	sx = m_dDivTopX - m_dNoriSiro;
	sy = m_dDivTopY - m_dNoriSiro;
	ex = m_dDivEndX + m_dNoriSiro;
	ey = m_dDivEndY + m_dNoriSiro;
	if(sx <= xx && xx <= ex && sy <= yy && yy <= ey){
		len = INTMAX;
		for(i = 1; i < m_nDivXCnt; i++){
			sy = m_dDivTopY-m_dNoriSiro;
			sx = m_tXdiv[i];
			ey = m_dDivEndY+m_dNoriSiro;
			ex = m_tXdiv[i];
			if(curveperpendicular(sx, sy, ex, ey, xx, yy, &x, &y) != FAIL){
				tlen = hypot(x - xx, y - yy);
				if(len > tlen){
					m_nCrtMode = XMOVE;
					m_nCrtLine = i;
					len = tlen;
				}
			}
		}
		for(i = 1; i < m_nDivYCnt; i++){
			sx = m_dDivTopX-m_dNoriSiro;
			sy = m_tYdiv[i];
			ex = m_dDivEndX+m_dNoriSiro;
			ey = m_tYdiv[i];
			if(curveperpendicular(sx, sy, ex, ey, xx, yy, &x, &y) != FAIL){
				tlen = hypot(x - xx, y - yy);
				if(len > tlen){
					m_nCrtMode = YMOVE;
					m_nCrtLine = i;
					len = tlen;
				}
			}
		}
	}

	if(m_nCrtLine == -1){
		if(xx < sx || ex < xx){
			m_nCrtMode = XMOVE;
			m_nCrtLine = 0;
		}
		else if(yy < sy || ey < yy){
			m_nCrtMode = YMOVE;
			m_nCrtLine = 0;
		}
		else{
			cx = (sx + ex)/2;
			cy = (sy + ey)/2;
			x = fabs(cx - xx);
			y = fabs(cy - yy);
			if(x < y){
				m_nCrtMode = YMOVE;
				m_nCrtLine = 0;
			}
			else{
				m_nCrtMode = XMOVE;
				m_nCrtLine = 0;
			}
		}
	}

	if(m_nCrtMode == XMOVE){
		if(m_nCrtLine == 0){
			m_nCrtLine = m_nDivXCnt;
			m_tXdiv[m_nCrtLine] = m_dDivBaseX;
			m_nBackX = -1;
			m_nDivXCnt++;
		}
		else{
			m_nBackX = (int)(m_tXdiv[m_nCrtLine]);
		}
	}
	else {
		if(m_nCrtLine == 0){
			m_nCrtLine = m_nDivYCnt;
			m_tYdiv[m_nCrtLine] = m_dDivBaseY;
			m_nDivYCnt++;
		}
		else{
			m_nBackY = (int)(m_tYdiv[m_nCrtLine]);
		}
	}
}

void CCmdDivPrint::SortAreaLine()
{
	int		i, j;
	DBL		tt;

	for(i = 0; i < m_nDivXCnt-1; i++){
		for(j = i+1; j < m_nDivXCnt; j++){
			if(m_tXdiv[j] < m_tXdiv[i]){
				tt = m_tXdiv[i];
				m_tXdiv[i] = m_tXdiv[j];
				m_tXdiv[j] = tt;
			}
		}
	}

	m_tXdiv[0] = m_dDivBaseX;
	for(i = 0, j = 1; i < m_nDivXCnt; i++){
		if(m_dDivBaseX < m_tXdiv[i]
		&& m_tXdiv[i] < (m_dDivEndX-m_dNoriSiro)
		&& m_tXdiv[j-1] != m_tXdiv[i]){
			m_tXdiv[j] = m_tXdiv[i];
			j++;
		}
	}
	m_nDivXCnt = j;

	for(i = 0; i < m_nDivYCnt-1; i++){
		for(j = i+1; j < m_nDivYCnt; j++){
			if(m_tYdiv[j] > m_tYdiv[i]){
				tt = m_tYdiv[i];
				m_tYdiv[i] = m_tYdiv[j];
				m_tYdiv[j] = tt;
			}
		}
	}

	m_tYdiv[0] = m_dDivBaseY;
	for(i = 0, j = 1; i < m_nDivYCnt; i++){
		if(m_dDivBaseY > m_tYdiv[i]
		&& m_tYdiv[i] > (m_dDivTopY+m_dNoriSiro)
		&& m_tYdiv[j-1] != m_tYdiv[i]){
			m_tYdiv[j] = m_tYdiv[i];
			j++;
		}
	}
	m_nDivYCnt = j;

}

void CCmdDivPrint::SetCrtAreaLine(DBL x, DBL y)
{
	if(m_nCrtMode == XMOVE){
		m_tXdiv[m_nCrtLine] = x;
	}
	else{
		m_tYdiv[m_nCrtLine] = y;
	}
}

void CCmdDivPrint::ResetCrtAreaLine(DBL x, DBL y)
{
	if(m_nCrtMode == XMOVE){
		if(m_nBackX == -1){
			m_nDivXCnt--;
		}
		else{
			m_tXdiv[m_nCrtLine] = m_nBackX;
		}
	}
	else{
		if(m_nBackY == -1){
			m_nDivYCnt--;
		}
		else{
			m_tYdiv[m_nCrtLine] = m_nBackY;
		}
	}
}

void CCmdDivPrint::DrawDivBoxs(CDC *pcDC, DBL dsx, DBL dsy, DBL dex, DBL dey)
{
	int		sx, sy, ex, ey;

	scrnsetslctpen(pcDC->m_hDC);

	scrndxymmtorltv(&dsx, &dsy);
	scrndxymmtorltv(&dex, &dey);
	sx = (int)dsx; sy = (int)dsy;
	ex = (int)dex; ey = (int)dey;
	pcDC->MoveTo(sx, sy);
 	pcDC->LineTo(sx, ey);
 	pcDC->LineTo(ex, ey);
 	pcDC->LineTo(ex, sy);
 	pcDC->LineTo(sx, sy);

	scrnpenend(pcDC->m_hDC);
}

void CCmdDivPrint::DrawDivLine(CDC *pcDC, DBL dsx, DBL dsy, DBL dex, DBL dey)
{
	int		sx, sy, ex, ey;

	scrnsetslctpen(pcDC->m_hDC);

	scrndxymmtorltv(&dsx, &dsy);
	scrndxymmtorltv(&dex, &dey);
	sx = (int)dsx; sy = (int)dsy;
	ex = (int)dex; ey = (int)dey;
	pcDC->MoveTo(sx, sy);
	pcDC->LineTo(ex, ey);

	scrnpenend(pcDC->m_hDC);
}

void CCmdDivPrint::DrawDivDotLine(CDC *pcDC, DBL dsx, DBL dsy, DBL dex, DBL dey)
{
	int		sx, sy, ex, ey;

	scrnsetslctdotpen(pcDC->m_hDC);

	scrndxymmtorltv(&dsx, &dsy);
	scrndxymmtorltv(&dex, &dey);
	sx = (int)dsx; sy = (int)dsy;
	ex = (int)dex; ey = (int)dey;
	pcDC->MoveTo(sx, sy);
	pcDC->LineTo(ex, ey);

	scrnpenend(pcDC->m_hDC);
}

void CCmdDivPrint::DrawDivCrossLine(CDC *pcDC, DBL dsx, DBL dsy, DBL dex, DBL dey)
{
	int		sx, sy, ex, ey;

	scrnsetslctpen(pcDC->m_hDC);

	scrndxymmtorltv(&dsx, &dsy);
	scrndxymmtorltv(&dex, &dey);
	sx = (int)dsx; sy = (int)dsy;
	ex = (int)dex; ey = (int)dey;
	pcDC->MoveTo(sx, sy);
 	pcDC->LineTo(ex, ey);
 	pcDC->MoveTo(ex, sy);
 	pcDC->LineTo(sx, ey);

	scrnpenend(pcDC->m_hDC);
}

