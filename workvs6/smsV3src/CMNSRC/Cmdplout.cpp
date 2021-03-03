
/////////////////////////////////////////////////////////////////////////////
// CCmdPloter

#include "stdafx.h"
#include "math.h"

#pragma optimize( "", off )

#include "cmd_base.h"
#include "cmdplout.h"
#include "dlgplout.h"

extern "C" {
	void Dispatch();
	int curveperpendicular(DBL sx, DBL sy, DBL ex, DBL ey, DBL x, DBL y, DBL *cx, DBL *cy);
	void dispsethdc(HDC hDC);
	void dispinit();
	void dispend();
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
	void poutfilenoset(int fileno);
	void poutdatanoset(int datano);
	void poutmmsizeget(DBL *xsize, DBL *ysize);
	void poutmodeget(int *homemode, int *linemode, int *omitmode);
	void poutmodeset(int homemode, int linemode, int omitmode);
	void poutrpdladdset(DBL xmm, DBL ymm, int cntrflag);
	void poutoutareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	void poutelementinit();
	void poutbegin(int nComID);
	void poutend(int nComID);
	int poutpaintmodechk();
	void poutchkmove(DBL x, DBL y);	
	void poutpaintinit(int nComID);
	void poutpaintend(int nComID);
	void poutplotinit(int nComID);
	void poutplotend(int nComID);
	int pouthomemodecheck();
	void poutendhome();
	void poutpenup();
	void  poutline(int nComID, DBL sx, DBL sy, DBL ex, DBL ey);
	void  poutmove(int nComID, DBL dx, DBL dy);
	void  poutoutline(int nComID, DBL sx, DBL sy, DBL ex, DBL ey);
	void  poutrejionend(int nComID);
	void  poutrejionstart(int nComID);
	void plotdxymmtostep(LPDBL x, LPDBL y);
	void pouttypeget(int *type);
	int outlibOpen();
	void outlibPutString(char *buf);
	void outlibClose();
}

CCmdPloter::CCmdPloter(CScrollView* pcview)
:CCmdBase(pcview)
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	char	buf[HRECMAX];
	FILE	*fp;

	m_dNoriSiro = 30.0;
	m_nCheckFlag = FALSE;
	m_nAreaBoxsOutFlag = FALSE;
	m_nFileNo = 0;
	m_nDataNo = 0;
	senvgetexepath(exepath);
	sprintf(fname, "%s\\defenv\\cmdplout.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf", &m_dNoriSiro);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &m_nCheckFlag);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &m_nAreaBoxsOutFlag);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &m_nFileNo);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &m_nDataNo);
		fclose(fp);
	}
}

CCmdPloter::~CCmdPloter()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\defenv\\cmdplout.env", exepath);
	if((fp = fopen(fname, "wr")) != NULL){
		fprintf(fp, "%4.1lf\n", m_dNoriSiro);
		fprintf(fp, "%d\n", m_nCheckFlag);
		fprintf(fp, "%d\n", m_nAreaBoxsOutFlag);
		fprintf(fp, "%d\n", m_nFileNo);
		fprintf(fp, "%d\n", m_nDataNo);
		fclose(fp);
	}
}

UINT CCmdPloter::ExecDivPlotOut(UINT event, DBL dx, DBL dy)
{
	CDialogPlotOut	*pDialogPlotOut;
	int homemode;
	int linemode;
	int omitmode;

	switch(event){
	case INITEVENT:
		pDialogPlotOut = new CDialogPlotOut(m_pcView);
		poutmodeget(&homemode, &linemode, &omitmode);
		pDialogPlotOut->m_fHomeStop = homemode;
		pDialogPlotOut->m_fLineOut = linemode;
		pDialogPlotOut->m_fOmitOut = omitmode;
		pDialogPlotOut->m_nFileNo = m_nFileNo;
		pDialogPlotOut->m_nDataNo = m_nDataNo;
		if(pDialogPlotOut->DoModal() == IDOK){
			homemode = pDialogPlotOut->m_fHomeStop;
			linemode = pDialogPlotOut->m_fLineOut;
			omitmode = pDialogPlotOut->m_fOmitOut;
			poutmodeset(homemode, linemode, omitmode);
			m_nFileNo = pDialogPlotOut->m_nFileNo;
			m_nDataNo = pDialogPlotOut->m_nDataNo;
		}
		else{
			return(ENDEVENT);
		}
		m_pcDialogDivPlotOut = new CDialogDivPlotOut(m_pcView);
		poutmmsizeget(&m_dPlotXsize, &m_dPlotYsize);
		m_pcDialogDivPlotOut->m_dPlotXSize = m_dPlotXsize; 
		m_pcDialogDivPlotOut->m_dPlotYSize = m_dPlotYsize; 
		m_pcDialogDivPlotOut->m_chkAreaBoxs = m_nAreaBoxsOutFlag;
		m_pcDialogDivPlotOut->m_chkStopChk = m_nCheckFlag;
		m_pcDialogDivPlotOut->m_dNorisiro = m_dNoriSiro;
		if(m_pcDialogDivPlotOut->Create(IDD_DIVPLOTER, m_pcView)){
			LocateDivPlotDlg();
		}
		senvoffflag(STOPFLAG);
		if(DivPlotAreaInit() == FALSE){
			m_dNoriSiro = 0;
			m_pcDialogDivPlotOut->m_dNorisiro = m_dNoriSiro;
			m_pcDialogDivPlotOut->UpdateData(FALSE);
			if(DivPlotAreaInit() == FALSE){
				AfxMessageBox("分割数が多すぎます");
				m_pcDialogDivPlotOut->DestroyWindow();
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
			m_nAreaBoxsOutFlag = m_pcDialogDivPlotOut->m_chkAreaBoxs;
			m_nCheckFlag = m_pcDialogDivPlotOut->m_chkStopChk;
			m_pcDialogDivPlotOut->DestroyWindow();
			DivPloterOutLoop();
			m_nMouseSetCount = 0;
			return(ENDEVENT);
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
			m_dPlotXsize = m_pcDialogDivPlotOut->m_dPlotXSize;
			m_dPlotYsize = m_pcDialogDivPlotOut->m_dPlotYSize;
			m_dNoriSiro = m_pcDialogDivPlotOut->m_dNorisiro;
			if(DivPlotAreaInit() == FALSE){
				m_dNoriSiro = 0;
				m_pcDialogDivPlotOut->m_dNorisiro = m_dNoriSiro;
				m_pcDialogDivPlotOut->UpdateData(FALSE);
				DivPlotAreaInit();
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
			m_pcDialogDivPlotOut->DestroyWindow();
			return(ENDEVENT);
		}
		break;
	}
	return(0);

}

UINT CCmdPloter::ExecPlotOut(UINT event, DBL dx, DBL dy)
{
	CDialogPlotOut	*pDialogPlotOut;
	int homemode;
	int linemode;
	int omitmode;
	DBL		xmm, ymm;
	DBL		areasx, areasy, areaex, areaey;

	if(event != INITEVENT){
		return(0);
	}
	senvoffflag(STOPFLAG);
	pDialogPlotOut = new CDialogPlotOut(m_pcView);
	poutmodeget(&homemode, &linemode, &omitmode);
	pDialogPlotOut->m_fHomeStop = homemode;
	pDialogPlotOut->m_fLineOut = linemode;
	pDialogPlotOut->m_fOmitOut = omitmode;
	pDialogPlotOut->m_nFileNo = m_nFileNo;
	pDialogPlotOut->m_nDataNo = m_nDataNo;
	if(pDialogPlotOut->DoModal() == IDOK){
		homemode = pDialogPlotOut->m_fHomeStop;
		linemode = pDialogPlotOut->m_fLineOut;
		omitmode = pDialogPlotOut->m_fOmitOut;
		m_nFileNo = pDialogPlotOut->m_nFileNo;
		m_nDataNo = pDialogPlotOut->m_nDataNo;
		poutmodeset(homemode, linemode, omitmode);
		poutfilenoset(m_nFileNo);
		poutdatanoset(m_nDataNo);
		scrngetmmsize(&xmm, &ymm);
		areasx = 0;
		areasy = 0;
		areaex = xmm;
		areaey = ymm;
		m_nAreaBoxsOutFlag = OFF;
		BeginDraw();
		DrawModeSET();
		dispsethdc(m_pcDC->m_hDC);
		dispinit();
		PlotOutMain(areasx, areasy, areaex, areaey);
		dispend();
		EndDraw();
	}
	return(0);
}

UINT CCmdPloter::ExecPlotMove(UINT event, DBL dx, DBL dy)
{
	int		ret;

	switch(event){
	case INITEVENT:
		ret = PltMoveOutInit();
		if(ret == FAIL){
			return(ENDEVENT);
		}
		m_nChkIdx = 0;
		break;
	case SETEVENT:
		BeginDraw();
		scrnsetslctpen(m_pcDC->m_hDC);
		m_pcSysEnv->ScrnDrawCheckPoint(m_pcDC, m_nChkIdx);
		m_pcSysEnv->ScrnSetCheckPoint(m_nChkIdx, dx, dy);
		m_pcSysEnv->ScrnDrawCheckPoint(m_pcDC, m_nChkIdx);
		scrnpenend(m_pcDC->m_hDC);
		EndDraw();
		PlotMoveChk(m_nChkIdx);
		m_nChkIdx = m_pcSysEnv->ScrnIncCheckPoint(m_nChkIdx);
		break;
	case RESETEVENT:
	case ENDEVENT:
		PltMoveOutEnd();
		return(ENDEVENT);
	}
	return(0);
}

int CCmdPloter::PltMoveOutInit()
{
	DBL		xmm, ymm;
	DBL		areasx, areasy, areaex, areaey;
	int		ret;
	int		type;

	m_nComID = 1;
	scrngetmmsize(&xmm, &ymm);
	areasx = 0;
	areasy = 0;
	areaex = xmm;
	areaey = ymm;
	poutoutareaset(areasx, areasy, areaex, areaey);
	pouttypeget(&type);
	ret = outlibOpen();
	if(ret == FAIL){
		AfxMessageBox("出力先が初期化できません", MB_OK, 0);
		return(FAIL);
	}
	poutelementinit();
	poutbegin(m_nComID);
	return(0);
}

void CCmdPloter::PlotMoveChk(int idx)
{
	DBL		dx, dy;

	m_pcSysEnv->ScrnGetCheckPoint(idx, &dx, &dy);
	plotdxymmtostep(&dx, &dy);
	poutchkmove(dx, dy);
}

void CCmdPloter::PltMoveOutEnd()
{
	int		type;

	pouttypeget(&type);
	if(type == CJ2HP || type == CJHP){
		poutchkmove(0.0, 0.0);
	}
	poutend(m_nComID);
	outlibClose();
}

void CCmdPloter::PlotOutMain(DBL areasx, DBL areasy, DBL areaex, DBL areaey)
{
	CTonDoc*	pcDoc;
	int	nComID;
	int	startflag;
	int	cntrflag;
	int	ret;
	CData* pcData;
	POSITION pos;
	DBL		xmm, ymm;
	int		type;
		
	((CTonView*)m_pcView)->m_nDrawIn = ON;
	nComID = 1;
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	startflag = scrngetstartmoveflag();
	cntrflag = scrngetcntrmoveflag();
	if(startflag == ON){
		scrngetstartmovemm(&xmm, &ymm);
	}
	else{
		xmm = 0;
		ymm = 0;
	}
	poutrpdladdset(xmm, ymm, cntrflag);
	poutoutareaset(areasx, areasy, areaex, areaey);
	pouttypeget(&type);
	ret = outlibOpen();
	if(ret == FAIL){
		AfxMessageBox("出力先が初期化できません", MB_OK, 0);
		((CTonView*)m_pcView)->m_nDrawIn = OFF;
		return;
	}
	poutelementinit();
	poutbegin(nComID);
	poutpaintinit(nComID);

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pcData = pcObjDataList->GetNext(pos);
		if(pcData == NULL){
			break;
		}
		if(pcData->CheckFlag(ACTIVEFLAG) == 	ON){
			pcData->PlotOut(nComID);
		}
		Dispatch();
		if(senvchckflag(STOPFLAG) == ON){
			poutpaintend(nComID);
			poutend(nComID);
			outlibClose();
			((CTonView*)m_pcView)->m_nDrawIn = OFF;
			return;
		}
	}

	poutpaintend(nComID);
	poutplotinit(nComID);

	pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pcData = pcObjDataList->GetNext(pos);
		if(pcData == NULL){
			break;
		}
		if(pcData->CheckFlag(ACTIVEFLAG) == 	ON){
			pcData->PlotOut(nComID);
		}
		Dispatch();
		if(senvchckflag(STOPFLAG) == ON){
			poutplotend(nComID);
			poutend(nComID);
			outlibClose();
			((CTonView*)m_pcView)->m_nDrawIn = OFF;
			return;
		}
	}

	scrngetmmsize(&xmm, &ymm);
	if(areasx == 0 && areasy == 0 && areaex == xmm && areaey == ymm){
		PlotOutTonnbo(nComID);
	}

	if(m_nAreaBoxsOutFlag == ON){
		PlotOutAreaBoxs(nComID, areasx, areasy, areaex, areaey);
	}

	if(pouthomemodecheck() == ON){
		poutendhome();
	}
	else{
		poutpenup();
	}
	poutplotend(nComID);
	poutend(nComID);
	outlibClose();
	((CTonView*)m_pcView)->m_nDrawIn = OFF;
}

void CCmdPloter::PlotOutAreaBoxs(int nComID, DBL areasx, DBL areasy, DBL areaex, DBL areaey)
{
	DBL	sx, sy, ex, ey;

	sx = areasx;
	sy = areasy;
	ex = areaex;
	ey = areaey;
	plotdxymmtostep(&sx, &sy);
	plotdxymmtostep(&ex, &ey);
	poutrejionstart(nComID);
	poutmove(nComID, sx, sy);
	poutline(nComID, sx, sy, sx, ey);
	poutline(nComID, sx, ey, ex, ey);
	poutline(nComID, ex, ey, ex, sy);
	poutline(nComID, ex, sy, sx, sy);
	poutrejionend(nComID);

	// (areasx, areasy)
	sx = areasx+m_dNoriSiro;
	sy = areasy;
	ex = areasx+m_dNoriSiro;
	ey = areasy-m_dNoriSiro/2;
	PlotOutTomboLine(nComID, sx, sy, ex, ey);

	sx = areasx+m_dNoriSiro/2;
	sy = areasy-m_dNoriSiro;
	ex = areasx;
	ey = areasy-m_dNoriSiro;
	PlotOutTomboLine(nComID, sx, sy, ex, ey);

	// (areasx, areaey)
	sx = areasx+m_dNoriSiro;
	sy = areaey;
	ex = areasx+m_dNoriSiro;
	ey = areaey+m_dNoriSiro/2;
	PlotOutTomboLine(nComID, sx, sy, ex, ey);

	sx = areasx+m_dNoriSiro/2;
	sy = areaey+m_dNoriSiro;
	ex = areasx;
	ey = areaey+m_dNoriSiro;
	PlotOutTomboLine(nComID, sx, sy, ex, ey);

	// (areaex, areaey)
	sx = areaex-m_dNoriSiro;
	sy = areaey;
	ex = areaex-m_dNoriSiro;
	ey = areaey+m_dNoriSiro/2;
	PlotOutTomboLine(nComID, sx, sy, ex, ey);

	sx = areaex-m_dNoriSiro/2;
	sy = areaey+m_dNoriSiro;
	ex = areaex;
	ey = areaey+m_dNoriSiro;
	PlotOutTomboLine(nComID, sx, sy, ex, ey);

	// (areaex, areasy)
	sx = areaex-m_dNoriSiro;
	sy = areasy;
	ex = areaex-m_dNoriSiro;
	ey = areasy-m_dNoriSiro/2;
	PlotOutTomboLine(nComID, sx, sy, ex, ey);

	sx = areaex-m_dNoriSiro/2;
	sy = areasy-m_dNoriSiro;
	ex = areaex;
	ey = areasy-m_dNoriSiro;
	PlotOutTomboLine(nComID, sx, sy, ex, ey);
}

void CCmdPloter::PlotOutTomboLine(int nComID, DBL sx, DBL sy, DBL ex, DBL ey)
{
	plotdxymmtostep(&sx, &sy);
	plotdxymmtostep(&ex, &ey);
	poutrejionstart(nComID);
	poutmove(nComID, sx, sy);
	poutline(nComID, sx, sy, ex, ey);
	poutrejionend(nComID);
}

void CCmdPloter::LocateDivPlotDlg()
{
	CTonApp*	theApp;
	CMainFrame*	theWnd;
	CRect	prect;
	CRect	crect;
	int		x, y, cx, cy;

	m_pcView->GetWindowRect(&prect);
	m_pcDialogDivPlotOut->GetWindowRect(&crect);
	x = prect.right - crect.Width();
	y = prect.top;
	cx = crect.Width();
	cy = crect.Height();

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	m_pcDialogDivPlotOut->ShowWindow(SW_SHOW);
	::SetWindowPos(m_pcDialogDivPlotOut->m_hWnd, HWND_TOP, x, y, cx, cy, 0);
}

void CCmdPloter::DivPloterOutLoop()
{
	int		i, j, ret;
	DBL		sx, sy, ex, ey;

	m_tXdiv[m_nDivXCnt] = m_dDivEndX - m_dNoriSiro;
	m_tYdiv[m_nDivYCnt] = m_dDivTopY + m_dNoriSiro;
	for(i = 0; i < m_nDivYCnt; i++){
		sy = m_tYdiv[i]+m_dNoriSiro;
		ey = m_tYdiv[i+1]-m_dNoriSiro;
		for(j = 0; j < m_nDivXCnt; j++){
			sx = m_tXdiv[j]-m_dNoriSiro;
			ex = m_tXdiv[j+1]+m_dNoriSiro;

			BeginDraw();
			DrawModeXOR();
			DrawDivBoxs(m_pcDC, sx, sy, ex, ey);
			EndDraw();

			ret = IDOK;
			if(m_nCheckFlag == ON){
				ret = AfxMessageBox("ダイレクト出力しますか", MB_OKCANCEL);
			}
			if(ret == IDOK){
				PlotOutMain(sx, sy, ex, ey);
				if(senvchckflag(STOPFLAG) == ON){
					BeginDraw();
					DrawModeXOR();
					DrawDivBoxs(m_pcDC, sx, sy, ex, ey);
					EndDraw();
					return;
				}
			}

			BeginDraw();
			DrawModeXOR();
			DrawDivBoxs(m_pcDC, sx, sy, ex, ey);
			EndDraw();
		}
	}
}

BOOL CCmdPloter::DivPlotAreaInit()
{
	CTonDoc*	pcDoc;
	DBL		minix, miniy, maxx, maxy;
	int		wabsxsize, wabsysize;
	int		i;

	if(m_dPlotXsize <= m_dNoriSiro*2
	|| m_dPlotYsize <= m_dNoriSiro*2){
		return(FALSE);
	}
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	m_dDivBaseX = minix;
	m_dDivBaseY = maxy;
	m_dDivTopX = minix - m_dNoriSiro;
	m_dDivTopY = miniy - m_dNoriSiro;
	m_dDivEndX = maxx + m_dNoriSiro;
	m_dDivEndY = maxy + m_dNoriSiro;
	wabsxsize = (int)(m_dDivEndX - m_dDivTopX);
	wabsysize = (int)(m_dDivEndY - m_dDivTopY);
	m_nOneXsize = (int)(m_dPlotXsize - m_dNoriSiro*2);
	m_nOneYsize = (int)(m_dPlotYsize - m_dNoriSiro*2);
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

void CCmdPloter::DrawAreaLine()
{
	BeginDraw();
	DrawModeXOR();
	DrawDivAreaLine(m_pcDC);
	EndDraw();
}

void CCmdPloter::DrawDivAreaLine(CDC *pcDC)
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
			if(fabs(ex - sx) > (m_dPlotXsize+0.1)
			|| fabs(ey - sy) > (m_dPlotYsize+0.1)){
				DrawDivCrossLine(pcDC, sx, sy, ex, ey);
			}
		}
	}
}

void CCmdPloter::SlctCrtAreaLine(DBL xx, DBL yy)
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

void CCmdPloter::SortAreaLine()
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

void CCmdPloter::SetCrtAreaLine(DBL x, DBL y)
{
	if(m_nCrtMode == XMOVE){
		m_tXdiv[m_nCrtLine] = x;
	}
	else{
		m_tYdiv[m_nCrtLine] = y;
	}
}

void CCmdPloter::ResetCrtAreaLine(DBL x, DBL y)
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

void CCmdPloter::DrawDivBoxs(CDC *pcDC, DBL dsx, DBL dsy, DBL dex, DBL dey)
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

void CCmdPloter::DrawDivLine(CDC *pcDC, DBL dsx, DBL dsy, DBL dex, DBL dey)
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

void CCmdPloter::DrawDivDotLine(CDC *pcDC, DBL dsx, DBL dsy, DBL dex, DBL dey)
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

void CCmdPloter::DrawDivCrossLine(CDC *pcDC, DBL dsx, DBL dsy, DBL dex, DBL dey)
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

void CCmdPloter::PlotOutTonnbo(int nComID)
{
	DBL		workxmm, workymm;
	DBL		sx, sy, ex, ey, cx, cy;

	CTonDoc *pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	if(pcDoc->m_nTonnbo == FALSE){
		return;
	}
	scrngetmmsize(&workxmm, &workymm);
	sx = pcDoc->m_dLeftSpace;
	sy = workymm - pcDoc->m_dTopSpace;
	ex = workxmm - pcDoc->m_dRightSpace;
	ey = pcDoc->m_dBottomSpace;
	cx = (sx+ex)/2.0;
	cy = (sy+ey)/2.0;
	PlotOutKadoTonnbo(nComID, sx, sy, -1.0, -1.0);
	PlotOutKadoTonnbo(nComID, sx, ey, -1.0, +1.0);
	PlotOutKadoTonnbo(nComID, ex, ey, +1.0, +1.0);
	PlotOutKadoTonnbo(nComID, ex, sy, +1.0, -1.0);
	PlotOutCntrTonnbo(nComID, sx, cy, -1.0, 0.0);
	PlotOutCntrTonnbo(nComID, ex, cy, +1.0, 0.0);
	PlotOutCntrTonnbo(nComID, cx, sy, 0.0, -1.0);
	PlotOutCntrTonnbo(nComID, cx, ey, 0.0, +1.0);
}

void CCmdPloter::PlotOutKadoTonnbo(int nComID, DBL cx, DBL cy, DBL signex, DBL signey)
{
	DBL		x1, x2, x3, y1, y2, y3;

	x1 = cx; y1 = cy-12.0*signey;
	x2 = cx; y2 = cy-3.0*signey;
	x3 = cx+12.0*signex; y3 = cy-3.0*signey;
	plotdxymmtostep(&x1, &y1);
	plotdxymmtostep(&x2, &y2);
	plotdxymmtostep(&x3, &y3);
	poutrejionstart(nComID);
	poutmove(nComID, x1, y1);
	poutline(nComID, x1, y1, x2, y2);
	poutline(nComID, x2, y2, x3, y3);
	poutrejionend(nComID);

	x1 = cx+3.0*signex; y1 = cy-12.0*signey;
	x2 = cx+3.0*signex; y2 = cy;
	x3 = cx+12.0*signex; y3 = cy;
	plotdxymmtostep(&x1, &y1);
	plotdxymmtostep(&x2, &y2);
	plotdxymmtostep(&x3, &y3);
	poutrejionstart(nComID);
	poutmove(nComID, x1, y1);
	poutline(nComID, x1, y1, x2, y2);
	poutline(nComID, x2, y2, x3, y3);
	poutrejionend(nComID);
}

void CCmdPloter::PlotOutCntrTonnbo(int nComID, DBL cx, DBL cy, DBL signex, DBL signey)
{
	DBL		x1, x2, x3, y1, y2, y3;

	if(signex != 0.0){
		x1 = cx+5.0*signex; y1 = cy-13.0;
		x2 = cx+5.0*signex; y2 = cy+13.0;
		plotdxymmtostep(&x1, &y1);
		plotdxymmtostep(&x2, &y2);
		poutrejionstart(nComID);
		poutmove(nComID, x1, y1);
		poutline(nComID, x1, y1, x2, y2);
		poutrejionend(nComID);

		x1 = cx+3.0*signex; y1 = cy;
		x2 = cx+11.0*signex; y2 = cy;
		plotdxymmtostep(&x1, &y1);
		plotdxymmtostep(&x2, &y2);
		poutrejionstart(nComID);
		poutmove(nComID, x1, y1);
		poutline(nComID, x1, y1, x2, y2);
		poutrejionend(nComID);
	}
	else{
		x1 = cx+13.0; y1 = cy-5.0*signey;
		x2 = cx-13.0; y2 = cy-5.0*signey;
		plotdxymmtostep(&x1, &y1);
		plotdxymmtostep(&x2, &y2);
		poutrejionstart(nComID);
		poutmove(nComID, x1, y1);
		poutline(nComID, x1, y1, x2, y2);
		poutrejionend(nComID);

		x1 = cx; y1 = cy-3.0*signey;
		x2 = cx; y2 = cy-11.0*signey;
		plotdxymmtostep(&x1, &y1);
		plotdxymmtostep(&x2, &y2);
		poutrejionstart(nComID);
		poutmove(nComID, x1, y1);
		poutline(nComID, x1, y1, x2, y2);
		poutrejionend(nComID);
	}
}

