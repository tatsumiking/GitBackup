// smsoutView.cpp : CTonView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "io.h"
#include "CmdBase.h"
#include "DialogMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//extern "C" {
	#include "../../ObjLib/Baselib.h"
	#include "../../ObjLib/Outlib.h"
	#include "../../ObjLib/Plot.h"
	#include "../../ObjLib/Pout.h"
	#include "../../ObjLib/Scrnlib.h"
	#include "../../ObjLib/Senvlib.h"
//}

/////////////////////////////////////////////////////////////////////////////
// CTonView

#define	TIMERINTERVAL	100

IMPLEMENT_DYNCREATE(CTonView, CScrollView)

BEGIN_MESSAGE_MAP(CTonView, CScrollView)
	//{{AFX_MSG_MAP(CTonView)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTonView クラスの構築/消滅

CTonView::CTonView()
{
	TCHAR	szExePath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];

	m_nFirst = TRUE;
	m_nOutCount = 1;
	// CG: 次の行は 「IME サポート」 コンポーネントにより追加されています。
	m_hImc = ::ImmCreateContext();
	CgAdjustCompositionWindow();
	m_nBmpCnt = 0; 
	m_pcCmdBase = NULL;
	m_bDispImage = FALSE;
	baseDispatch();

	senvgetexepath(szExePath);
	_stprintf_s(szFileName, _T("%s\\wakuimage.txt"), szExePath);
    if((_taccess_s(szFileName, 0 )) == 0){
		m_bDispImage = TRUE;
	}
	m_bEpsonYoko = FALSE;
	_stprintf_s(szFileName, _T("%s\\epsonyoko.txt"), szExePath);
    if((_taccess_s(szFileName, 0 )) == 0){
		m_bEpsonYoko = TRUE;
	}
}

CTonView::~CTonView()
{
	if(m_pcCmdBase != NULL){
		delete(m_pcCmdBase);
	}
	// CG: 次の行は 「IME サポート」コンポーネントにより追加されています。
	::ImmAssociateContext(m_hWnd, NULL);
	::ImmDestroyContext(m_hImc);
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
	TCHAR	szTitle[RECMAX];
	CTonDoc* pcDoc = GetDocument();
	ASSERT_VALID(pcDoc);

	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	CMainFrame *pcWnd = (CMainFrame*)pcApp->GetMainWnd();
	if(m_nFirst == TRUE){
		m_nFirst = FALSE;
		pcDoc->SetCrtView(this);
		_tcscpy_s(szTitle, _T("筆耕システム Ver1.0"));
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
	WorkTonnboDraw(pcDC);

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
	m_pcDoc = (CTonDoc*)GetDocument();
	m_pcCmdBase = new CCmdBase(this);
	m_pcCmdBase->InitClass();
	m_pcDoc->InitSetList();
	CDialogMain *pcDialogMain = new CDialogMain(this);
	pcDialogMain->DoModal();
	delete(pcDialogMain);
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
	::ImmAssociateContext(m_hWnd, m_hImc);
}

void CTonView::CgAdjustCompositionWindow()
{
	// CG: この関数は 「IME サポート」 コンポーネントにより追加されています。
	// 現在のキャレット位置に変換ウィンドウを移動する必要がある場合に、この関数を呼び出してください。
	COMPOSITIONFORM cf;
	if (m_hImc)
	{
		cf.dwStyle = CFS_POINT;
		cf.ptCurrentPos = GetCaretPos();
		::ImmSetCompositionWindow(m_hImc, &cf);
	}
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

void CTonView::PrintEnvSave(LPTSTR printenvfname)
{
	PRINTDLG	m_pd;
	LPDEVMODE	lpDevMode;
	LPDEVNAMES	lpDevNames;
	FILE		*fp;
	HANDLE		hDevMode;
	HANDLE		hDevNames;
	CTonApp*	theApp;
	int			nwritemodesize;
	int			nwritenamessize;

	theApp = (CTonApp*)AfxGetApp();
	hDevMode = theApp->GetDevMode();
	hDevNames = theApp->GetDevNames();
	if(hDevMode == 0 || hDevNames == 0){
		memset( &m_pd, 0, sizeof( PRINTDLG ) );
		m_pd.lStructSize    = sizeof( PRINTDLG );
		m_pd.hwndOwner      = m_hWnd;
		m_pd.Flags          = PD_RETURNDEFAULT;
		m_pd.hDevMode       = NULL;
		m_pd.hDevNames      = NULL;
		::PrintDlg(&m_pd);
		hDevMode = m_pd.hDevMode;
		hDevNames = m_pd.hDevNames;
	}
	lpDevMode = (LPDEVMODE)::GlobalLock(hDevMode);
	lpDevNames = (LPDEVNAMES)::GlobalLock(hDevNames);
	nwritemodesize = GlobalSize(hDevMode);
	nwritenamessize = GlobalSize(hDevNames);
	if(lpDevMode == NULL && lpDevNames == NULL){
		return;
	}

	_tfopen_s(&fp, printenvfname, _T("wb"));
	if (fp != NULL) {
		fwrite(&nwritemodesize, sizeof(int), 1, fp);
		fwrite(lpDevMode, GlobalSize(hDevMode), 1, fp);
		fwrite(&nwritenamessize, sizeof(int), 1, fp);
		fwrite(lpDevNames, GlobalSize(hDevNames), 1, fp);
		fclose(fp);
	}

	::GlobalUnlock(hDevNames);
	::GlobalUnlock(hDevMode);

}

void CTonView::PrintEnvLoad(LPTSTR printenvfname)
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

	_tfopen_s(&fp, printenvfname, _T("rb"));
	if (fp == NULL) {
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
		// 用紙名サイズ等を設定
		hCrtDevMode = pcApp->GetDevMode();
		lpCrtDevMode = (LPDEVMODE)::GlobalLock(hCrtDevMode);
		lpLoadDevMode = (LPDEVMODE)::GlobalLock(hLoadDevMode);
		lpCrtDevMode->dmOrientation = lpLoadDevMode->dmOrientation;
		lpCrtDevMode->dmPaperSize = lpLoadDevMode->dmPaperSize;
		lpCrtDevMode->dmPaperLength = lpLoadDevMode->dmPaperLength;
		lpCrtDevMode->dmPaperWidth = lpLoadDevMode->dmPaperWidth;
		lpCrtDevMode->dmDefaultSource = lpLoadDevMode->dmDefaultSource;
		::GlobalUnlock(hLoadDevMode);
		::GlobalUnlock(hCrtDevMode);
		::GlobalFree(hLoadDevNames);
		::GlobalFree(hLoadDevMode);
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

void CTonView::PrintEnvSetElementLoad(LPTSTR printenvfname)
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
	int			ret;
	
	CTonApp *pcApp = (CTonApp*)AfxGetApp();

	_tfopen_s(&fp, printenvfname, _T("rb"));
	if (fp == NULL) {
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

	// 用紙名サイズ等を設定
	hCrtDevMode = pcApp->GetDevMode();
	lpCrtDevMode = (LPDEVMODE)::GlobalLock(hCrtDevMode);
	lpLoadDevMode = (LPDEVMODE)::GlobalLock(hLoadDevMode);
	lpCrtDevMode->dmOrientation = lpLoadDevMode->dmOrientation;
	lpCrtDevMode->dmPaperSize = lpLoadDevMode->dmPaperSize;
	lpCrtDevMode->dmPaperLength = lpLoadDevMode->dmPaperLength;
	lpCrtDevMode->dmPaperWidth = lpLoadDevMode->dmPaperWidth;
	lpCrtDevMode->dmDefaultSource = lpLoadDevMode->dmDefaultSource;
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
	rect.left = sx;
	rect.right = ex;
	rect.top = sy;
	rect.bottom = ey;
	pcDC->FillSolidRect(&rect, pcApp->m_clrBack);
}

UINT CTonView::CommandExec(UINT event, long x, long y)
{
	return(0);
}

void CTonView::AllWakuSetString(LPTSTR lpText)
{
	CSWakVect	*pcSWakVect;
	LPTSTR		sp;
	TCHAR		szSrcStr[RECMAX];
	TCHAR		szSetStr[RECMAX];

	m_pcCmdBase->SetDataListSelectID(SWAKVECTID, 0);
	POSITION pos = m_pcCmdBase->m_pcPtrList->GetHeadPosition();
	szSetStr[0] = '\0';
	if(lpText[0] == '\0'){
		lpText[1] = '\0';
	}
	sp = &(lpText[1]);
	while (pos != NULL)
	{
		pcSWakVect = (CSWakVect*)(m_pcCmdBase->m_pcPtrList->GetNext(pos));
		sp = basedivstrg(sp, szSetStr, '/');
		if(pcSWakVect->m_nFixFlag != ON){
			pcSWakVect->GetString(szSrcStr);
			if(_tcscmp(szSrcStr, szSetStr) != 0){
				pcSWakVect->SetString(szSetStr);
				if(szSetStr[0] != '\0'){
					pcSWakVect->StrgLineStruct();
					pcSWakVect->RenewMiniMax();
				}
			}
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
		strTitle.Format(_T("iti%03d"), m_nOutCount);
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
		AfxMessageBox(_T("出力先が初期化できません"), MB_OK, 0);
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
		baseDispatch();
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
		baseDispatch();
		if(senvchckflag(STOPFLAG) == ON){
			poutplotend(nComID);
			poutend(nComID);
			outlibClose();
			return;
		}
	}

	scrngetmmsize(&xmm, &ymm);
	if(areasx == 0 && areasy == 0 && areaex == xmm && areaey == ymm){
		PlotOutTonnbo(nComID);
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
	TCHAR	szDevice[QRECMAX];
	TCHAR	szDriver[QRECMAX];
	TCHAR	szOutput[QRECMAX];
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
	_tcscpy_s(szDevice, ((LPTSTR)(lpDevNames) + lpDevNames->wDeviceOffset));
	_tcscpy_s(szDriver, ((LPTSTR)(lpDevNames) + lpDevNames->wDriverOffset));
	_tcscpy_s(szOutput, ((LPTSTR)(lpDevNames) + lpDevNames->wOutputOffset));
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

void CTonView::ChangeFont(LPTSTR lpFont)
{
	CSWakVect	*pcSWakVect;
	TCHAR		szStr[RECMAX];

	m_pcCmdBase->SetDataListSelectID(SWAKVECTID, 0);
	POSITION pos = m_pcCmdBase->m_pcPtrList->GetHeadPosition();
	while (pos != NULL)
	{
		pcSWakVect = (CSWakVect*)(m_pcCmdBase->m_pcPtrList->GetNext(pos));
		pcSWakVect->GetString(szStr);
		pcSWakVect->SetFontName(lpFont);
		pcSWakVect->SetString(szStr);
		if(szStr[0] != '\0'){
			pcSWakVect->StrgLineStruct();
			pcSWakVect->RenewMiniMax();
		}
	}
}

void CTonView::DirectDraw(CDC *pcDC)
{
	CTonDoc* pcDoc = GetDocument();
	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	POSITION pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		CData* pcData = pcObjDataList->GetNext(pos);
		pcData->OnDraw(pcDC);
	}	
}

void CTonView::WorkSizeReset()
{
	CRect	rectClient;
	int		w, h;
	DBL		topx, topy;
	DBL		xtime, ytime;

	scrngetrltvtop(&topx, &topy);
	scrnsetrltvtop(0.0, 0.0);
	scrndxyrltvtomm(&topx, &topy);
	scrngetxytime(&xtime, &ytime);
	GetClientRect(&rectClient);
	h = rectClient.bottom - rectClient.top;
	w = rectClient.right - rectClient.left;
	scrnsetwinarea(rectClient.left, rectClient.top, rectClient.right, rectClient.bottom);
	scrninitsizeelement(&w, &h);
	SetScrollSizes( MM_TEXT, GetDocument()->GetDocSize());
	if(m_nFirst == FALSE){
		scrnsetxytime(xtime, ytime);
		scrnsetrltvtop(0.0, 0.0);
		scrnsetrltvelement();
		scrndxymmtorltv(&topx, &topy);
		scrnsetrltvtop(topx, topy);
		InitScrollElement();
	}
}

void CTonView::WorkAreaReset()
{
	scrnresetzoomtime();
	scrnsetrltvelement();
	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
	ScrollToPosition(GetDocument()->GetTopPoint());
	InitScrollElement();
}

void CTonView::WorkTonnboDraw(CDC *pcDC)
{
	CPen cPen, *pcOldPen;
	DBL		workxmm, workymm;
	DBL		sx, sy, ex, ey, cx, cy;

	CTonDoc* pcDoc = (CTonDoc*)GetDocument();
	if(pcDoc->m_nTonnbo == FALSE){
		return;
	}
	cPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pcOldPen = pcDC->SelectObject(&cPen);
	scrngetmmsize(&workxmm, &workymm);
	sx = pcDoc->m_dLeftSpace;
	sy = workymm - pcDoc->m_dTopSpace;
	ex = workxmm - pcDoc->m_dRightSpace;
	ey = pcDoc->m_dBottomSpace;
	cx = (sx+ex)/2.0;
	cy = (sy+ey)/2.0;
	WorkKadoTonnboDraw(pcDC, sx, sy, -1.0, -1.0);
	WorkKadoTonnboDraw(pcDC, sx, ey, -1.0, +1.0);
	WorkKadoTonnboDraw(pcDC, ex, ey, +1.0, +1.0);
	WorkKadoTonnboDraw(pcDC, ex, sy, +1.0, -1.0);
	WorkCntrTonnboDraw(pcDC, sx, cy, -1.0, 0.0);
	WorkCntrTonnboDraw(pcDC, ex, cy, +1.0, 0.0);
	WorkCntrTonnboDraw(pcDC, cx, sy, 0.0, -1.0);
	WorkCntrTonnboDraw(pcDC, cx, ey, 0.0, +1.0);
}

void CTonView::WorkKadoTonnboDraw(CDC *pcDC, DBL cx, DBL cy, DBL signex, DBL signey)
{
	DBL		dx, dy;

	dx = cx; dy = cy-12.0*signey;
	scrndxymmtorltv(&dx, &dy);
	pcDC->MoveTo((int)dx, (int)dy);
	dx = cx; dy = cy-3.0*signey;
	scrndxymmtorltv(&dx, &dy);
	pcDC->LineTo((int)dx, (int)dy);
	dx = cx+12.0*signex; dy = cy-3.0*signey;
	scrndxymmtorltv(&dx, &dy);
	pcDC->LineTo((int)dx, (int)dy);

	dx = cx+3.0*signex; dy = cy-12.0*signey;
	scrndxymmtorltv(&dx, &dy);
	pcDC->MoveTo((int)dx, (int)dy);
	dx = cx+3.0*signex; dy = cy;
	scrndxymmtorltv(&dx, &dy);
	pcDC->LineTo((int)dx, (int)dy);
	dx = cx+12.0*signex; dy = cy;
	scrndxymmtorltv(&dx, &dy);
	pcDC->LineTo((int)dx, (int)dy);
}

void CTonView::WorkCntrTonnboDraw(CDC *pcDC, DBL cx, DBL cy, DBL signex, DBL signey)
{
	DBL		dx, dy;

	if(signex != 0.0){
		dx = cx+5.0*signex; dy = cy-13.0;
		scrndxymmtorltv(&dx, &dy);
		pcDC->MoveTo((int)dx, (int)dy);
		dx = cx+5.0*signex; dy = cy+13.0;
		scrndxymmtorltv(&dx, &dy);
		pcDC->LineTo((int)dx, (int)dy);

		dx = cx+3.0*signex; dy = cy;
		scrndxymmtorltv(&dx, &dy);
		pcDC->MoveTo((int)dx, (int)dy);
		dx = cx+11.0*signex; dy = cy;
		scrndxymmtorltv(&dx, &dy);
		pcDC->LineTo((int)dx, (int)dy);
	}
	else{
		dx = cx+13.0; dy = cy-5.0*signey;
		scrndxymmtorltv(&dx, &dy);
		pcDC->MoveTo((int)dx, (int)dy);
		dx = cx-13.0; dy = cy-5.0*signey;
		scrndxymmtorltv(&dx, &dy);
		pcDC->LineTo((int)dx, (int)dy);

		dx = cx; dy = cy-3.0*signey;
		scrndxymmtorltv(&dx, &dy);
		pcDC->MoveTo((int)dx, (int)dy);
		dx = cx; dy = cy-11.0*signey;
		scrndxymmtorltv(&dx, &dy);
		pcDC->LineTo((int)dx, (int)dy);
	}
}

void CTonView::PlotOutTonnbo(int nComID)
{
	DBL		workxmm, workymm;
	DBL		sx, sy, ex, ey, cx, cy;

	CTonDoc *pcDoc = (CTonDoc*)(GetDocument());
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

void CTonView::PlotOutKadoTonnbo(int nComID, DBL cx, DBL cy, DBL signex, DBL signey)
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

void CTonView::PlotOutCntrTonnbo(int nComID, DBL cx, DBL cy, DBL signex, DBL signey)
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


