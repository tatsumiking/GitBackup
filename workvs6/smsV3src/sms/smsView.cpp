// smsView.cpp : CTonView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "Dlgmain.h"
#include "DlgOutput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
	void Dispatch();
	LPSTR basedivstrg(LPSTR sp, LPSTR retsp, char far sparator);
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
	DBL scrnsetxytime(DBL timex, DBL timey);
	void scrndlenmmtorltv(LPDBL len);
	void scrndxymmtorltv(LPDBL x, LPDBL y);
	void scrngetxytime(LPDBL timex, LPDBL timey);
	void scrnsetrltvelement();
	void scrnsetrltvtop(DBL xTop, DBL yTop);
	void scrngetrltvtop(LPDBL xTop, LPDBL yTop);
	void scrnsetrltvtopreverce(DBL xTop, DBL yTop, DBL xSize, DBL ySize);
	void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	void scrnrltvareaget(DBL *sx, DBL *sy, DBL *ex, DBL *ey);
	void scrnsetwinarea(int sx, int sy, int ex, int ey);
	void scrnsetdisprltvmove(DBL xmove, DBL ymove);
	void scrngetdisprltvmove(LPDBL xmove, LPDBL ymove);
	int scrngetcntrmoveflag();
	int scrngetstartmoveflag();
	void scrngetstartmovemm(LPDBL xmm, LPDBL ymm);
	void scrnsetrltvmove(DBL x, DBL y);
	int senvsetdrawmode(int mode);
	int	senvcheckdrawmode();
	int senvchckflag(int flag);
	void poutcntrkindget(int *kind);
	void poutrpdladdset(DBL xmm, DBL ymm, int cntrflag);
	void poutoutareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	void poutelementinit();
	void poutbegin(int nComID);
	void poutend(int nComID);
	void poutpaintinit(int nComID);
	void poutpaintend(int nComID);
	void poutplotinit(int nComID);
	void poutplotend(int nComID);
	int pouthomemodecheck();
	void poutendhome();
	void poutpenup();
	int outlibOpen();
	void outlibClose();
}

/////////////////////////////////////////////////////////////////////////////
// CTonView

#define	TIMERINTERVAL	100

IMPLEMENT_DYNCREATE(CTonView, CScrollView)

BEGIN_MESSAGE_MAP(CTonView, CScrollView)
	ON_MESSAGE(WM_IME_NOTIFY, OnMyImeNotify)
	//{{AFX_MSG_MAP(CTonView)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTonView クラスの構築/消滅

CTonView::CTonView()
{
	m_nFirst = TRUE;
	m_nOutCount = 1;
	// CG: 次の行は 「IME サポート」 コンポーネントにより追加されています。
	//m_hImc = ::ImmCreateContext();
	CgAdjustCompositionWindow();
	m_nBmpCnt = 0; 
	m_pcCmdBase = NULL;
}

CTonView::~CTonView()
{
	if(m_pcCmdBase != NULL){
		delete(m_pcCmdBase);
	}
	// CG: 次の行は 「IME サポート」コンポーネントにより追加されています。
	//::ImmAssociateContext(m_hWnd, NULL);
	//::ImmDestroyContext(m_hImc);
}

BOOL CTonView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTonView クラスの描画

void CTonView::OnDraw(CDC* pcDC)
{
	CData	*pcData;
	char	szTitle[RECMAX];
	CTonDoc* pcDoc = GetDocument();
	ASSERT_VALID(pcDoc);

	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	CMainFrame *pcWnd = (CMainFrame*)pcApp->GetMainWnd();
	if(m_nFirst == TRUE){
		m_nFirst = FALSE;
		strcpy(szTitle, "筆耕システム Ver1.0");
		pcDoc->SetTitle(szTitle);
		pcWnd->m_pcCrtView = this;
		InitScrollElement();
		m_nTimerID = SetTimer(1, TIMERINTERVAL, NULL);
	}

	AreaBackClrFill(pcDC);

	POSITION pos = pcDoc->m_pcObjDataList->GetHeadPosition();
	while (pos != NULL){
		pcData = pcDoc->m_pcObjDataList->GetNext(pos);
		pcData->OnDraw(pcDC);
	}

}

/////////////////////////////////////////////////////////////////////////////
// CTonView クラスの診断

#ifdef _DEBUG
void CTonView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTonView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTonDoc* CTonView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTonDoc)));
	return (CTonDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTonView クラスのメッセージ ハンドラ


void CTonView::OnTimer(UINT nIDEvent) 
{
	CScrollView::OnTimer(nIDEvent);
	KillTimer(m_nTimerID);
	m_pcCmdBase = new CCmdBase(this);
	m_pcCmdBase->InitClass();
	CDialogMain *pcDialogMain = new CDialogMain(this);
	CMainFrame *pcWnd = (CMainFrame*)AfxGetMainWnd();
	//pcWnd->ShowWindow(SW_HIDE);
	pcDialogMain->DoModal();
	delete(pcDialogMain);
	pcWnd->ShowWindow(SW_SHOW);
	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	pcApp->AppExit();
}

LRESULT CTonView::OnMyImeNotify(WPARAM dwCommand, LPARAM dwData)
{

	// CG: この関数は 「IME サポート」 コンポーネントにより追加されています。
	return DefWindowProc(WM_IME_NOTIFY, dwCommand, dwData);
}

void CTonView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: このビューのサイズの合計を計算します。
	sizeTotal.cx = sizeTotal.cy = 50;
	SetScrollSizes(MM_TEXT, sizeTotal);

	// CG: 次の行は 「IME サポート」 コンポーネントにより追加されています。
	//::ImmAssociateContext(m_hWnd, m_hImc);
}

void CTonView::CgAdjustCompositionWindow()
{
	// CG: この関数は 「IME サポート」 コンポーネントにより追加されています。
	// 現在のキャレット位置に変換ウィンドウを移動する必要がある場合に、この関数を呼び出してください。
/*
	COMPOSITIONFORM cf;
	if (m_hImc)
	{
		cf.dwStyle = CFS_POINT;
		cf.ptCurrentPos = GetCaretPos();
		::ImmSetCompositionWindow(m_hImc, &cf);
	}
*/
}

void CTonView::DrawWakuInRect(CDC *pcDC, CRect rect)
{
	CData	*pcData;

	InitArea(rect);
	FillWorkArea(pcDC);
	CTonDoc *pcDoc = GetDocument();
	POSITION pos = pcDoc->m_pcObjDataList->GetHeadPosition();
	while (pos != NULL){
		pcData = pcDoc->m_pcObjDataList->GetNext(pos);
		pcData->OnDraw(pcDC);
	}
}

void CTonView::RedrawFlagOn()
{

}

void CTonView::InitScrollElement()
{
	SetScrollSizes( MM_TEXT, GetDocument()->GetDocSize());
	ScrollToPosition(GetDocument()->GetTopPoint());
	Invalidate();
}

void CTonView::MouseSetCrt(DBL dstx, DBL dsty)
{

}

void CTonView::PrintEnvLoad(LPSTR printenvfname)
{
	HANDLE		hCrtDevMode;
	LPDEVMODE	lpCrtDevMode;
	FILE		*fp;
	HANDLE		hLoadDevMode;
	HANDLE		hLoadDevNames;
	LPDEVMODE	lpLoadDevMode;
	LPDEVNAMES	lpLoadDevNames;
	int			nModeSize; 
	int			nNamesSize;
	int			nCrtModeSize;
	int			ret;
	
	CTonApp *pcApp = (CTonApp*)AfxGetApp();

	if((fp = fopen(printenvfname, "rb")) == NULL){
		return;
	}
	fseek(fp, 0L, 0);
	fread(&nModeSize, sizeof(int), 1, fp);
	hLoadDevMode = GlobalAlloc(GHND, nModeSize);
	lpLoadDevMode = (LPDEVMODE)::GlobalLock(hLoadDevMode);
	fread(lpLoadDevMode, nModeSize, 1, fp);

	fread(&nNamesSize, sizeof(int), 1, fp);
	hLoadDevNames = GlobalAlloc(GHND, nNamesSize);
	lpLoadDevNames = (LPDEVNAMES)::GlobalLock(hLoadDevNames);
	fread(lpLoadDevNames, nNamesSize, 1, fp);

	::GlobalUnlock(hLoadDevNames);
	::GlobalUnlock(hLoadDevMode);

	fclose(fp);

	ret = pcApp->CheckEnumPrinter(hLoadDevNames);
	if(ret == FALSE){
		hCrtDevMode = pcApp->GetDevMode();
		if(hCrtDevMode != 0){
			lpCrtDevMode = (LPDEVMODE)::GlobalLock(hCrtDevMode);
			lpLoadDevMode = (LPDEVMODE)::GlobalLock(hLoadDevMode);
			lpCrtDevMode->dmOrientation = lpLoadDevMode->dmOrientation;
			lpCrtDevMode->dmPaperSize = lpLoadDevMode->dmPaperSize;
			lpCrtDevMode->dmPaperLength = lpLoadDevMode->dmPaperLength;
			lpCrtDevMode->dmPaperWidth = lpLoadDevMode->dmPaperWidth;
			lpCrtDevMode->dmDefaultSource = lpLoadDevMode->dmDefaultSource;
			::GlobalUnlock(hLoadDevMode);
			::GlobalUnlock(hCrtDevMode);
		}
		return;
	}
	hCrtDevMode = pcApp->GetDevMode();
	nCrtModeSize = ::GlobalSize(hCrtDevMode);
	lpCrtDevMode = (LPDEVMODE)::GlobalLock(hCrtDevMode);
	lpLoadDevMode = (LPDEVMODE)::GlobalLock(hLoadDevMode);

	if(nCrtModeSize == nModeSize){
		memcpy(lpCrtDevMode, lpLoadDevMode, nModeSize);
	}
	else{
		lpCrtDevMode->dmOrientation = lpLoadDevMode->dmOrientation;
		lpCrtDevMode->dmPaperSize = lpLoadDevMode->dmPaperSize;
		lpCrtDevMode->dmPaperLength = lpLoadDevMode->dmPaperLength;
		lpCrtDevMode->dmPaperWidth = lpLoadDevMode->dmPaperWidth;
		lpCrtDevMode->dmDefaultSource = lpLoadDevMode->dmDefaultSource;
	}
	::GlobalUnlock(hLoadDevMode);
	::GlobalUnlock(hCrtDevMode);

	::GlobalFree(hLoadDevNames);
	::GlobalFree(hLoadDevMode);
}

void CTonView::InitArea(CRect rect)
{
	DBL		rltvxsize, rltvysize;
	DBL		mmxsize, mmysize;
	DBL		xtime, ytime, time;
	DBL		rltvsx, rltvsy;

	rltvxsize = rect.Width();
	rltvysize = rect.Height();
	rltvsx = rect.left;
	rltvsy = rect.top;
	scrngetmmsize(&mmxsize, &mmysize);
	xtime = (mmxsize / rltvxsize);
	ytime = (mmysize / rltvysize);
	if(xtime > ytime){
		time = xtime;
	}
	else{
		time = ytime;
	}
	scrnsetxytime(time, time);
	scrnsetrltvelement();
	scrnsetrltvtop(0, 0);
	scrnsetrltvmove(0, 0);
	scrnrltvareaset(rltvsx, rltvsy, rltvxsize+rltvsx, rltvysize+rltvsy);
	scrnsetwinarea((int)rltvsx, (int)rltvsy, (int)(rltvxsize+rltvsx), (int)(rltvysize+rltvsy));
	scrnsetdisprltvmove(rltvsx, rltvsy);
}

void CTonView::FillWorkArea(CDC *pcDC)
{
	DBL		xsize, ysize;
	DBL		sx, sy;
	DBL		ex, ey;
	CRect	rect;

	scrngetmmsize(&xsize, &ysize);
	sx = 0;
	sy = 0;
	ex = xsize;
	ey = ysize;
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	rect.left = (int)sx;
	rect.right = (int)ex;
	rect.top = (int)ey;
	rect.bottom = (int)sy; 
	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	if(pcApp->m_clrBack != -1){
		pcDC->FillSolidRect(&rect, pcApp->m_clrBack);
	}
	else{
		pcDC->FillSolidRect(&rect, RGB(255,255,255));
	}
}

void CTonView::AreaBackClrFill(CDC *pcDC)
{
	DBL		sx, sy, ex, ey;
	CRect	rect;

	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	if(pcApp->m_clrBack == -1){
		return;
	}
	scrnrltvareaget(&sx, &sy, &ex, &ey);
	rect.left = (long)sx;
	rect.right = (long)ex;
	rect.top = (long)sy;
	rect.bottom = (long)ey;
	pcDC->FillSolidRect(&rect, pcApp->m_clrBack);
}

UINT CTonView::CommandExec(UINT event, long x, long y)
{
	return(0);
}

void CTonView::AllWakuSetString(LPSTR lpText)
{
	CSWakVect	*pcSWakVect;
	LPSTR		sp;
	char		szStr[RECMAX];

	m_pcCmdBase->SetDataListSelectID(SWAKVECTID, 0);
	POSITION pos = m_pcCmdBase->m_pcPtrList->GetHeadPosition();
	sp = &(lpText[1]);
	while (pos != NULL)
	{
		pcSWakVect = (CSWakVect*)(m_pcCmdBase->m_pcPtrList->GetNext(pos));
		sp = basedivstrg(sp, szStr, '/');
		if(pcSWakVect->m_nFixFlag != ON){
			pcSWakVect->SetString(szStr);
			pcSWakVect->StrgLineStruct();
			pcSWakVect->RenewMiniMax();
		}
	}
}

void CTonView::PrinterOutput(int cpycnt)
{
	CPrintInfo printInfo;
	CDC		cDC;
	CDC		*pcDC;
	DOCINFO docInfo;
	CString strTitle;
	int		nSPage, nEPage, nCrt;

	pcDC = &cDC;
	CreatePrinterDC(pcDC, cpycnt);
	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	printInfo.m_pPD->m_pd.hDC = pcDC->m_hDC;
	printInfo.m_pPD->m_pd.hDevMode = pcApp->GetDevMode();
	printInfo.m_pPD->m_pd.hDevNames = pcApp->GetDevNames();
	if (OnPreparePrinting(&printInfo))
	{
		strTitle.Format("iti%03d", m_nOutCount);
		m_nOutCount++;
		memset(&docInfo, 0, sizeof(DOCINFO));
		docInfo.cbSize = sizeof(DOCINFO);
		docInfo.lpszDocName = strTitle;

		OnBeginPrinting(pcDC, &printInfo);
		pcDC->StartDoc(&docInfo);
		nSPage = printInfo.GetMinPage();
		nEPage = printInfo.GetMaxPage();
		for(nCrt = nSPage; nCrt <= nEPage;  nCrt++){
			printInfo.m_nCurPage = nCrt;
			OnPrepareDC(pcDC, &printInfo);
			pcDC->StartPage();
			OnPrint(pcDC, &printInfo);
			pcDC->EndPage();
		}
		pcDC->EndDoc();
		OnEndPrinting(pcDC, &printInfo);
	}
	DeletePrinterDC(&cDC);
}

void CTonView::PloterOutput(int cpycnt)
{
	DBL		xmm, ymm;
	DBL		areasx, areasy, areaex, areaey;
	int		i;

	scrngetmmsize(&xmm, &ymm);
	areasx = 0;
	areasy = 0;
	areaex = xmm;
	areaey = ymm;
	for(i = 0; i < cpycnt; i++){
		PlotOutMain(areasx, areasy, areaex, areaey);
	}
}

void CTonView::OnBeginPrinting(CDC* pcDC, CPrintInfo* pcInfo) 
{
	DBL		xsize, ysize;
	DBL		xmm, ymm;
	DBL		xadd, yadd;
	DBL		dPrnStartSX, dPrnStartSY;
	DBL		dPHYPrnMMXSize, dPHYPrnMMYSize;
	int		cntrkind;
	
	LPDEVMODE lpDevMode = (LPDEVMODE)::GlobalLock(pcInfo->m_pPD->m_pd.hDevMode);
	m_nOrientation = lpDevMode->dmOrientation;
	::GlobalUnlock(pcInfo->m_pPD->m_pd.hDevMode);
	// TODO: 印刷前の特別な初期化処理を追加してください。
	senvsetdrawmode(PRINTVIEW);
	xsize = pcDC->GetDeviceCaps(PHYSICALWIDTH);
	ysize = pcDC->GetDeviceCaps(PHYSICALHEIGHT);
	m_dDpiX = pcDC->GetDeviceCaps(LOGPIXELSX);
	m_dDpiY = pcDC->GetDeviceCaps(LOGPIXELSY);
	dPHYPrnMMXSize = (int)(xsize / m_dDpiX * 25.4 + 0.5);
	dPHYPrnMMYSize = (int)(ysize / m_dDpiY * 25.4 + 0.5);

	m_dPrnMMXSize = pcDC->GetDeviceCaps(HORZSIZE);
	m_dPrnMMYSize = pcDC->GetDeviceCaps(VERTSIZE);
	dPrnStartSX = pcDC->GetDeviceCaps(PHYSICALOFFSETX);
	dPrnStartSY = pcDC->GetDeviceCaps(PHYSICALOFFSETY);
	scrngetmmsize(&m_dMMXSize, &m_dMMYSize);
	m_dPrnTimeX = (25.4 / m_dDpiX);
	m_dPrnTimeY = (25.4 / m_dDpiY);
	scrnsetxytime(m_dPrnTimeX, m_dPrnTimeY);
	scrnsetrltvelement();
	scrnsetdisprltvmove(0, 0);

	m_nXDivPage = 1;
	if(m_dMMXSize > dPHYPrnMMXSize){
		m_nXDivPage = (long)m_dMMXSize / (long)m_dPrnMMXSize;
		if((long)m_dMMXSize % (long)m_dPrnMMXSize){
			m_nXDivPage++;
		}
	}

	m_nYDivPage = 1;
	if(m_dMMYSize > dPHYPrnMMYSize){
		m_nYDivPage = (long)m_dMMYSize / (long)m_dPrnMMYSize;
		if((long)m_dMMYSize % (long)m_dPrnMMYSize){
			m_nYDivPage++;
		}
	}
	m_nDivPageLast = m_nXDivPage*m_nYDivPage;

	poutcntrkindget(&cntrkind);
	switch(cntrkind){
	case RIGHTUP:
	case LEFTUP:
		scrnsetrltvtop(0.0, 0.0);
		break;
	case LEFTDOWN:
	case RIGHTDOWN:
		xmm = m_dPrnMMXSize;
		ymm = m_dPrnMMYSize;
		scrnsetrltvtop(0.0, 0.0);
		scrndlenmmtorltv(&xmm);
		scrndlenmmtorltv(&ymm);
		scrnsetrltvtopreverce(0.0, 0.0, xmm, ymm);
		break;
	}

	xmm = 0; ymm = 0;
	if(scrngetcntrmoveflag() == ON){
		switch(cntrkind){
		case LEFTUP:
			xmm = (m_dPrnMMXSize - m_dMMXSize)/2.0;
			break;
		case RIGHTDOWN:
			xmm = -(m_dPrnMMXSize - m_dMMXSize)/2.0;
			break;
		case RIGHTUP:
			ymm = (m_dPrnMMYSize - m_dMMYSize)/2.0;
			break;
		case LEFTDOWN:
			ymm = -(m_dPrnMMYSize - m_dMMYSize)/2.0;
			break;
		}
	}
	if(scrngetstartmoveflag() == ON){
		scrngetstartmovemm(&xadd, &yadd);
		switch(cntrkind){
		case LEFTUP:
		case RIGHTDOWN:
			xmm = xmm + yadd;
			ymm = ymm - xadd;
			break;
		case RIGHTUP:
		case LEFTDOWN:
			xmm = xmm + xadd;
			ymm = ymm + yadd;
			break;
		}
	}
	scrndlenmmtorltv(&xmm);
	scrndlenmmtorltv(&ymm);
	switch(cntrkind){
	case LEFTUP:	// 左上
		if(scrngetcntrmoveflag() == ON){
			scrnsetrltvmove(xmm, ymm-dPrnStartSY);
		}
		else{
			scrnsetrltvmove(xmm-dPrnStartSX, ymm-dPrnStartSY);
		}
		break;
	case RIGHTUP:
		if(scrngetcntrmoveflag() == ON){
			scrnsetrltvmove(xmm-dPrnStartSX, ymm);
		}
		else{
			scrnsetrltvmove(xmm-dPrnStartSX, ymm-dPrnStartSY);
		}
		break;
	case RIGHTDOWN:
		if(scrngetcntrmoveflag() == ON){
			scrnsetrltvmove(xmm, ymm+dPrnStartSY);
		}
		else{
			scrnsetrltvmove(xmm+dPrnStartSX, ymm+dPrnStartSY);
		}
		break;
	case LEFTDOWN:
		if(scrngetcntrmoveflag() == ON){
			scrnsetrltvmove(xmm+dPrnStartSX, ymm);
		}
		else{
			scrnsetrltvmove(xmm+dPrnStartSX, ymm+dPrnStartSY);
		}
		break;
	}
	pcInfo->SetMinPage(1);
	pcInfo->SetMaxPage(m_nDivPageLast);
	if(m_nDivPageLast != 1){
		scrnsetrltvmove(-dPrnStartSX, -dPrnStartSY);
	}
	scrnrltvareaset(0, 0, INTMAX, INTMAX);
}

void CTonView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	senvsetdrawmode(PREVIEW);
}

void CTonView::OnPrepareDC(CDC* pcDC, CPrintInfo* pcInfo) 
{
	DBL		sx, sy, ex, ey;
	int		mode;

	CScrollView::OnPrepareDC(pcDC, pcInfo);
	mode = senvcheckdrawmode();
	if(mode == PRINTVIEW
	&& pcInfo != NULL){
		if(m_nDivPageLast != 1){
			if(m_nOrientation == DMORIENT_PORTRAIT){
				m_nDivCrtY = ((pcInfo->m_nCurPage-1)) % m_nYDivPage;
				m_nDivCrtX = ((pcInfo->m_nCurPage-1)) / m_nYDivPage;
			}
			else{
				m_nDivCrtX = ((pcInfo->m_nCurPage-1)) % m_nXDivPage;
				m_nDivCrtY = ((pcInfo->m_nCurPage-1)) / m_nXDivPage;
			}
			sx = m_nDivCrtX * m_dPrnMMXSize / m_dPrnTimeX;
			sy = m_nDivCrtY * m_dPrnMMYSize / m_dPrnTimeY;
			ex = m_dPrnMMXSize / m_dPrnTimeX;
			ey = m_dPrnMMYSize / m_dPrnTimeX;
			scrnsetrltvtop(sx, sy);
			scrnrltvareaset(-1000, -1000, ex+1000, ey+1000);
		}
	}
}

BOOL CTonView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	return CScrollView::OnPreparePrinting(pInfo);
}

void CTonView::PlotOutMain(DBL areasx, DBL areasy, DBL areaex, DBL areaey)
{
	CTonDoc*	pcDoc;
	int	nComID;
	int	startflag;
	int	cntrflag;
	int	ret;
	CData* pcData;
	POSITION pos;
	DBL		xmm, ymm;
		
	nComID = 1;
	pcDoc = (CTonDoc*)(GetDocument());
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
	ret = outlibOpen();
	if(ret == FAIL){
		AfxMessageBox("出力先が初期化できません", MB_OK, 0);
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
			return;
		}
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
}

void CTonView::CreatePrinterDC(CDC *pcDC, int cpycnt)
{
	HANDLE	hDevMode;
	HANDLE	hDevNames;
	DEVMODE		*lpDevMode;
	DEVNAMES	*lpDevNames;
	char	szDevice[QRECMAX];
	char	szDriver[QRECMAX];
	char	szOutput[QRECMAX];
	int		ret;

	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	hDevMode = pcApp->GetDevMode();
	hDevNames = pcApp->GetDevNames();
	if(hDevNames == 0){
		return;
	}
	lpDevNames = (DEVNAMES *)GlobalLock(hDevNames);
	if(lpDevNames == NULL){
		return;
	}
	strcpy(szDevice, ((LPSTR)(lpDevNames) + lpDevNames->wDeviceOffset));
	strcpy(szDriver, ((LPSTR)(lpDevNames) + lpDevNames->wDriverOffset));
	strcpy(szOutput, ((LPSTR)(lpDevNames) + lpDevNames->wOutputOffset));
	GlobalUnlock(hDevNames);

	lpDevMode = (DEVMODE *)GlobalLock(hDevMode);
	if(lpDevMode == NULL){
		return;
	}
	lpDevMode->dmCopies = cpycnt;
	ret = pcDC->CreateDC(szDriver, szDevice, szOutput, lpDevMode);
	GlobalUnlock(hDevMode);
	if(ret == 0){
		return;
	}
}

void CTonView::DeletePrinterDC(CDC *pcDC)
{
	pcDC->DeleteDC();
}

void CTonView::ChangeFont(LPSTR lpFont)
{
	CSWakVect	*pcSWakVect;
	char		szStr[RECMAX];

	m_pcCmdBase->SetDataListSelectID(SWAKVECTID, 0);
	POSITION pos = m_pcCmdBase->m_pcPtrList->GetHeadPosition();
	while (pos != NULL)
	{
		pcSWakVect = (CSWakVect*)(m_pcCmdBase->m_pcPtrList->GetNext(pos));
		pcSWakVect->GetString(szStr);
		pcSWakVect->SetFontName(lpFont);
		pcSWakVect->SetString(szStr);
	}
}

