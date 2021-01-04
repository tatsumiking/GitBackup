
// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"

#include <stdio.h>
#include <conio.h>
#include <direct.h>
#include <io.h>
#include <stdlib.h>

#include "Ses.h"

#include "MainFrm.h"
#include "DialogTextInput.h"
#include "../UsbPrt.h"

//extern "C" {
	#include "../../ObjLib/Baselib.h"
	#include "../../ObjLib/Senvlib.h"
	#include "../../ObjLib/Scrnlib.h"
	#include "../../ObjLib/Curvelib.h"
	#include "../../ObjLib/Diblib.h"
	#include "../../ObjLib/Plgnfill.h"
	#include "../../ObjLib/Pout.h"
	#include "../../ObjLib/Figuenv.h"
	#include "../../ObjLib/Mojienv.h"
	#include "../../ObjLib/Wakuenv.h"
//}


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct {
	GLOBALHANDLE	hGmem;
	int				nCount;
}ENUMER;

static int m_nCount=0;
// 1000 * 60 * 10 
#define PROTECTTIMERINTERVAL 600000

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_WM_KEYUP()
	ON_WM_PAINT()
	ON_COMMAND(ID_WAKU_TATEYOKO, OnWakuTateyoko)
	ON_COMMAND(ID_WAKU_CONERKIND, OnWakuConerkind)
	ON_COMMAND(ID_WAKU_STARTKIND, OnWakuStartkind)
	ON_COMMAND(ID_FILE_SLCTPRINT, OnFileSlctprint)
	ON_COMMAND(ID_FILE_CENTERKIND, OnFileCenterkind)
	ON_COMMAND(ID_FILE_SAVEPAPER, OnFileSavepaper)
	ON_BN_CLICKED(ID_FILE_SETSPACE, OnFileSetspace)
	ON_BN_CLICKED(ID_WAKU_PAPERADD, OnWakuPaperadd)
	ON_COMMAND(ID_VIEW_STATUS_BAR, OnViewStatusBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateViewStatusBar)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_COMMAND_EX(ID_VIEW_TOOLBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateControlBarMenu)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケーター
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static CSize GetBaseUnits()
{
	return CSize(8, 16);
}

// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame() noexcept
{
	TCHAR	szExePath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];

	// TODO: この位置にメンバの初期化処理コードを追加してください。
	m_nFirstFlag = TRUE;
	m_hImc = NULL;
	m_dwClr = 0;
	m_nOptSlctObj = 0;
	//m_nOptChangeCurser = 0;
	m_nOptChangeCurser = 1;
	m_nEpsClrTrns = OFF;
	m_nStatusBar = OFF;
	// 初期化の位置を移動
	GdiplusStartup(&m_gdiToken, &m_gdiSI, NULL);

	InitExePath(szExePath);
	senvsetexepath(szExePath);
	GetWindowsDirectory(m_szEnvPath, FNAMEMAX);
	m_szEnvPath[2] = _T('\0');
	_tcscat_s(m_szEnvPath, FNAMEMAX, _T("\\UsesProgram"));
	_tmkdir(m_szEnvPath);
	_tcscat_s(m_szEnvPath, FNAMEMAX, _T("\\Itiban"));
	_tmkdir(m_szEnvPath);

	curveinit();
	senvinit();
	scrninit();
	_stprintf_s(fname, FNAMEMAX, _T("%s\\defenv\\clrtrns.env"), szExePath);
	mojienvinit();
	diblibinit();

	EnvLoad();

	m_pcFSF1Font = new CFSF1Font(0);
	m_pcWNTTFont = new CWinTTFont(0);
	m_pcESF0Font = new CESF0Font(0);
	m_pcESF1Font = new CESF1Font(0);
	m_pcFBU0Font = new CFBU0Font(0);
	m_pcWNTTFont->FSF1ClassSet(m_pcFSF1Font);
	m_pcESF0Font->FSF1ClassSet(m_pcFSF1Font);
	m_pcESF1Font->FSF1ClassSet(m_pcFSF1Font);
	m_pcFBU0Font->FSF1ClassSet(m_pcFSF1Font);
	m_pcWNTTFont->ParentSet(this);
	plgnfillinit();
	plgnfillareaset(-INTMAX, -INTMAX, INTMAX, INTMAX);
	senvinitscrnclr();
	DivDrawEnvLoad();
	PRINTDLG	sPrintDlg;
	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	pcApp->GetPrinterDeviceDefaults(&sPrintDlg);
}

CMainFrame::~CMainFrame()
{
	GdiplusShutdown(m_gdiToken);

	EndIme();
	senvendscrnclr();
	senvend();
	scrnend();
	plgnfillend();
	diblibend();
	EnvSave();
	mojienvflistclose();
	delete(m_pcFBU0Font);
	delete(m_pcESF1Font);
	delete(m_pcESF0Font);
	delete(m_pcWNTTFont);
	delete(m_pcFSF1Font);
}
void CMainFrame::InitExePath(LPTSTR szExePath)
{
	int		len;

	len = ::GetModuleFileName(AfxGetInstanceHandle( ), szExePath, FNAMEMAX);
	while(1){
		if(len <= 0){
			szExePath[0] = _T('\0');
			break;
		}
		if(szExePath[len] == _T(':') || szExePath[len] == _T('\\')){
			szExePath[len] = _T('\0');
			break;
		}
		len--;
	}
}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
#ifdef PROTECT 
	TCHAR	msgbuf[RECMAX];
	int		ret;
	int		id, nDay, nStat;

	CTonApp *pcApp = (CTonApp*)::AfxGetApp();
	ret = CheckProtectFile();
	if(ret == -1){
		ret = CheckProtectUsb();
		if(ret == -1){
			_stprintf_s(msgbuf, _T("USBプロテクトエラーです\n販売店にご連絡ください"));
			MessageBox(msgbuf, _T("筆耕システム 枠作成システム"), MB_OK);
		}
	}
	if(ret == -1){
		return -1;
	}
#endif
	lpCreateStruct->lpszClass = _T("筆耕システム 枠作成システム");
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_szBaseUnits = GetBaseUnits();

	if (!m_wndStatusBar.Create(this)
	||	!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // 作成に失敗
	}

	if(!CreateWakuBar1())
		return -1;
	if(!CreateWakuBar2())
		return -1;
	if(!CreateWakuBar3())
		return -1;
	if (!CreateClrBar())
		return -1;
	if(!CreateWakuBarSide())
		return -1;
	if (!m_wndLeftSideBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_LEFT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndLeftSideBar.LoadToolBar(IDR_LEFTSIDE))
	{
		TRACE0("Failed to create cdnttoolbar\n");
		return -1;      // 作成に失敗
	}
	if(m_nStatusBar == ON){
		m_wndStatusBar.ShowWindow(SW_SHOW);
	}else{
		m_wndStatusBar.ShowWindow(SW_HIDE);
	}
	m_wndWakuBar1.EnableDocking(CBRS_ALIGN_TOP);
	m_wndWakuBar2.EnableDocking(CBRS_ALIGN_TOP);
	m_wndWakuBar3.EnableDocking(CBRS_ALIGN_BOTTOM);
	m_wndWakuBarSide.EnableDocking(CBRS_ALIGN_RIGHT);
	m_wndLeftSideBar.EnableDocking(CBRS_ALIGN_LEFT);
	m_wndClrBar.EnableDocking(CBRS_ALIGN_BOTTOM);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndWakuBar1, AFX_IDW_DOCKBAR_TOP);
	DockControlBar(&m_wndWakuBar2, AFX_IDW_DOCKBAR_TOP);
	DockControlBar(&m_wndWakuBarSide, AFX_IDW_DOCKBAR_RIGHT);
	DockControlBar(&m_wndLeftSideBar, AFX_IDW_DOCKBAR_LEFT);
	DockControlBar(&m_wndWakuBar3, AFX_IDW_DOCKBAR_BOTTOM);
	DockControlBar(&m_wndClrBar, AFX_IDW_DOCKBAR_BOTTOM);

	InitIme();
	StartTimer();
	//CSplashWnd::ShowSplashScreen(this);
	return 0;
}
void CMainFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// cs.dwExStyle |= WS_EX_TOPMOST;
	cs.x = 0;
	cs.y = 200;
	cs.cx = 1024;
	cs.cy = 768;
	return CMDIFrameWnd::PreCreateWindow(cs);
}

// CMainFrame の診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG

// CMainFrame メッセージ ハンドラー

void CMainFrame::StartTimer()
{
	m_nTimerID = SetTimer(1, PROTECTTIMERINTERVAL, NULL);
}
void CMainFrame::OnTimer(UINT nIDEvent) 
{
	CMDIFrameWnd::OnTimer(nIDEvent);
#ifdef PROTECT 
	TCHAR	msgbuf[RECMAX];
	int		ret;

	ret = CheckProtectFile();
	if(ret == -1){
		ret = CheckProtectUsb();
		if(ret == -1){
			_stprintf_s(msgbuf, _T("USBプロテクトエラーです\n販売店にご連絡ください"));
			MessageBox(msgbuf, _T("筆耕システム 枠作成システム"), MB_OK);
		}
	}
	if(ret == -1){
		CTonApp *pcApp = (CTonApp*)::AfxGetApp();
		pcApp->AppExit();
	}
#endif
}

void CMainFrame::StatXYOut(DBL xabs, DBL yabs)
{
	TCHAR	xbuf[QRECMAX];
	TCHAR	ybuf[QRECMAX];
	TCHAR	msgbuf[QRECMAX];

	if(xabs > 10000 || yabs > 10000){
		_stprintf_s(xbuf, _T("%6.2lf"), xabs/1000);
		_stprintf_s(ybuf, _T("%6.2lf"), yabs/1000);
		_stprintf_s(msgbuf, _T("(%s,%s)M"), xbuf, ybuf);
	}
	else{
		_stprintf_s(xbuf, _T("%6.1lf"), xabs);
		_stprintf_s(ybuf, _T("%6.1lf"), yabs);
		_stprintf_s(msgbuf, _T("(%s,%s)mm"), xbuf, ybuf);
	}
	HelpTextOut(4, msgbuf);
}

void CMainFrame::HelpTextOut(int idx, LPTSTR msgbuf)
{
	m_wndStatusBar.SetPaneText(idx, msgbuf, TRUE);
}

BOOL CMainFrame::CreateClrBar()
{
	CSpinButtonCtrl* m_pSPB;

	if (!m_wndClrBar.Create(this, IDD_CLRBAR,
		CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
		ID_VIEW_CLRBAR))
	{
		TRACE0("Failed to create dialog bar m_wndClrBar\n");
		return FALSE;		// 作成に失敗
	}

	m_pSPB = (CSpinButtonCtrl*)(m_wndClrBar.GetDlgItem(IDC_SPTHICK));
	m_pSPB->SetRange(0, UD_MAXVAL);

	return TRUE;
}

BOOL CMainFrame::CreateWakuBar1()
{
	if (!m_wndWakuBar1.Create(this, IDD_WAKUBAR1,
		CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
		ID_VIEW_WAKUBAR1))
	{
		TRACE0("Failed to create dialog bar m_wndWakuBar1\n");
		return FALSE;		// 作成に失敗
	}
	return TRUE;
}

BOOL CMainFrame::CreateWakuBar2()
{
	if (!m_wndWakuBar2.Create(this, IDD_WAKUBAR2,
		CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
		ID_VIEW_WAKUBAR2))
	{
		TRACE0("Failed to create dialog bar m_wndWakuBar2\n");
		return FALSE;		// 作成に失敗
	}
	return TRUE;
}

BOOL CMainFrame::CreateWakuBar3()
{
	if (!m_wndWakuBar3.Create(this, IDD_WAKUBAR3,
		CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
		ID_VIEW_WAKUBAR3))
	{
		TRACE0("Failed to create dialog bar m_wndWakuBar3\n");
		return FALSE;		// 作成に失敗
	}
	return TRUE;
}

BOOL CMainFrame::CreateWakuBarSide()
{
	if (!m_wndWakuBarSide.Create(this, IDD_WAKUSIDE,
		CBRS_RIGHT | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
		ID_VIEW_WAKUSIDE))
	{
		TRACE0("Failed to create dialog bar m_wndWakuBar3\n");
		return FALSE;		// 作成に失敗
	}
	return TRUE;
}

void CMainFrame::InitItemElement()
{
	m_wndWakuBar1.InitItemElement();
	m_wndWakuBar2.InitItemElement();
	m_wndWakuBar3.InitItemElement();
	m_wndWakuBarSide.InitItemElement();
}

void CMainFrame::SetItemElement()
{
	m_wndWakuBar1.SetItemElement();
	m_wndWakuBar2.SetItemElement();
	m_wndWakuBar3.SetItemElement();
	m_wndWakuBarSide.SetItemElement();
	m_wndWakuBar1.Invalidate(TRUE);
	m_wndWakuBar2.Invalidate(TRUE);
	m_wndWakuBar3.Invalidate(TRUE);
	m_wndWakuBarSide.Invalidate(TRUE);
}

void CMainFrame::SetSWakLocate()
{
	m_wndWakuBar3.SetLocate();
}

void CMainFrame::GetItemElement()
{
	m_wndWakuBar1.GetItemElement();
	m_wndWakuBar2.GetItemElement();
	m_wndWakuBar3.GetItemElement();
	m_wndWakuBarSide.GetItemElement();
}

void CMainFrame::ShowItemElement()
{
	//RecalcLayout();
}

void CMainFrame::HideItemElement()
{
	//RecalcLayout();
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
}

void CMainFrame::OnKeyUp(UINT nTCHAR, UINT nRepCnt, UINT nFlags) 
{
	CMDIFrameWnd::OnKeyUp(nTCHAR, nRepCnt, nFlags);
}

void CMainFrame::OnPaint() 
{
	CPaintDC dc(this);
	if(m_nFirstFlag == TRUE){
		m_nFirstFlag = FALSE;
		//MDIMaximize(this); // Windowの最大化

		//::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		//::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		::SetForegroundWindow(this->m_hWnd);
	}
}

void CMainFrame::ThisViewMax(CTonView *pcView)
{
	MDIMaximize(pcView->GetParentFrame()); 
}

void CMainFrame::SetCrtView(CTonView * pcView)
{
	m_pcCrtView = pcView;
	m_wndWakuBar1.SetCrtView(pcView);
	m_wndWakuBar2.SetCrtView(pcView);
	m_wndWakuBar3.SetCrtView(pcView);
	m_wndWakuBarSide.SetCrtView(pcView);
}

void CMainFrame::EnvLoad()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	crctfname[FNAMEMAX];
	FILE	*crctfp;

	ScrnEnvLoad();
	ScrnAddEnvLoad();
	MojiEnvLoad();
	FiguEnvLoad();
	FontEnvLoad();
	WakuEnvLoad();
	MemoryEnvLoad();
	MemoryEnvSave();
	senvgetexepath(exepath);
	_stprintf_s(crctfname, _T("%s\\outcrect.dat"), exepath);

	_tfopen_s(&crctfp, crctfname, _T("w,ccs=UNICODE"));
	if (crctfp != NULL) {
		fclose(crctfp);
	}
}

void CMainFrame::EnvSave()
{
	ScrnEnvSave();
	ScrnAddEnvSave();
	MojiEnvSave();
	FiguEnvSave();
	WakuEnvSave();
	MemoryEnvSave();
}

void CMainFrame::ScrnEnvLoad()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	exedrive[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	DBL		dclrlebel;
	DBL		dmmx, dmmy;
	int		ncntrkind;
	TCHAR	sportname[RECMAX];
	TCHAR	splotername[QRECMAX];
	int		npcmdtype;
	DBL		dpxsize, dpysize;
	DBL		dpstep;
	DBL		dpsx, dpsy;
	int		nparcflag, npfillflag;
	int		nphomemode, nplinemode, npomitmode;
	DBL		dpxtime, dpytime;
	TCHAR	datpath[FNAMEMAX];
	TCHAR	tmppath[FNAMEMAX];
	TCHAR	wakpath[FNAMEMAX];
	TCHAR	kmnpath[FNAMEMAX];
	TCHAR	partpath[FNAMEMAX];
	TCHAR	outpath[FNAMEMAX];
	TCHAR	btppath[FNAMEMAX];
	TCHAR	fntpath[FNAMEMAX];

	senvgetexepath(exepath);
	_tcscpy_s(exedrive, exepath);
	exedrive[2] = _T('\0');
	_tcscpy_s(m_szKaisyaName, _T("未設定"));
	dmmx = 210; dmmy = 298;
	ncntrkind = LEFTDOWN;
	_tcscpy_s(splotername, _T("HPGL"));
	npcmdtype = HPGL;
	dpxsize = 590.0; dpysize = 430.0;
	dpstep = 0.1;
	dpsx = -295; dpsy = -215;
	nparcflag = OFF; npfillflag = OFF;
	nphomemode = ON; nplinemode = ON; npomitmode = ON;
	dpxtime = 1.0; dpytime = 1.0;
	_tcscpy_s(sportname, _T("LPT1:"));
	_stprintf_s(fntpath, _T("%s\\moji"), exepath);
	_stprintf_s(datpath, _T("%s\\data"), exepath);
	_stprintf_s(outpath, _T("%s\\dout"), exepath);
	_stprintf_s(kmnpath, _T("%s\\kamon"), exepath);
	_stprintf_s(wakpath, _T("%s\\waku"), exepath);
	_stprintf_s(partpath, _T("%s\\part"), exepath);
	_stprintf_s(tmppath, _T("%s\\temp"), exepath);
	_stprintf_s(btppath, _T("%s\\tbmp"), exepath);

	_stprintf_s(fname, _T("%s\\ses_sys.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if (fp != NULL) {
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), m_szKaisyaName, FNAMEMAX);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf %lf"), &dmmx, &dmmy);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d"), &ncntrkind);
		baselibCrDelFgets(sportname, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s %d %lf %lf %lf %lf %lf"),
			splotername, QRECMAX, &npcmdtype,
			&dpxsize, &dpysize, &dpstep, 
			&dpsx, &dpsy);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d %d %d %d %d"),
			&nparcflag, &npfillflag, 
			&nphomemode, &nplinemode, &npomitmode);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf"), &dclrlebel);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), fntpath, FNAMEMAX);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), datpath, FNAMEMAX);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), outpath, FNAMEMAX);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), kmnpath, FNAMEMAX);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), wakpath, FNAMEMAX);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), partpath, FNAMEMAX);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), tmppath, FNAMEMAX);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), btppath, FNAMEMAX);
		fclose(fp);
	}
	//else{
		//CreateDirectory(exedrive, NULL);
		CreateDirectory(fntpath, NULL);
		CreateDirectory(datpath, NULL);
		CreateDirectory(outpath, NULL);
		CreateDirectory(kmnpath, NULL);
		CreateDirectory(wakpath, NULL);
		CreateDirectory(partpath, NULL);
		CreateDirectory(tmppath, NULL);
		CreateDirectory(btppath, NULL);
	//}
	scrnsetmmsize(dmmx, dmmy);
	poutcntrkindset(ncntrkind);
	poutportnameset(sportname);
	poutploternameset(splotername);
	pouttypeset(npcmdtype);
	poutmmsizeset(dpxsize, dpysize);
	poutstepset(dpstep);
	poutmmstartset(dpsx, dpsy);
	poutarcflagset(nparcflag);
	poutfillflagset(npfillflag);
	poutmodeset(nphomemode, nplinemode, npomitmode);
	senvsetdatpath(datpath);
	senvsetwakpath(wakpath);
	senvsetkmnpath(kmnpath);
	senvsetoutpath(outpath);
	senvsetpartpath(partpath);
	senvsetbtppath(btppath);
	mojienvsetfontpath(fntpath);
	senvsettmppath(tmppath);

	scrngetmmsize(&dmmx, &dmmy);
	poutcntrkindget(&ncntrkind);
	poutportnameget(sportname);
}

void CMainFrame::ScrnAddEnvLoad()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	pltname[FNAMEMAX];
	int		cntrmoveflag;
	int		startmoveflag;
	DBL		startmovexmm, startmoveymm;
	DBL		plotlimitstep, plotlimitr;
	DBL		plotstartspace, plotendspace;
	int		nareadotsize;
	int		npointdotsize;

	_tcscpy_s(pltname, _T("penvalue.env"));
	cntrmoveflag = 0;
	startmoveflag = 0;
	startmovexmm = 0.0; startmoveymm = 0.0;
	m_nTTEisuFlag = ON;
	plotlimitstep = 30000.0;
	plotlimitr = 30000.0;
	plotstartspace = 0;
	plotendspace = 0;
	nareadotsize = 6;
	npointdotsize = 3;
	m_nStatusBar = ON;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\ses_sys2.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if (fp != NULL) {
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), pltname, FNAMEMAX);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d"), &cntrmoveflag);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d"), &startmoveflag);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf %lf"), &startmovexmm, &startmoveymm);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d"), &m_nTTEisuFlag);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf %lf %lf %lf"),
				&plotlimitstep, &plotlimitr,
				&plotstartspace, &plotendspace);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d"), &nareadotsize);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d"), &npointdotsize);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d"), &m_nStatusBar);
		fclose(fp);
	}
	senvsetcrtpltname(pltname);
	scrnsetcntrmoveflag(cntrmoveflag);
	scrnsetstartmoveflag(startmoveflag);
	scrnsetstartmovemm(startmovexmm, startmoveymm);
	poutsetparaminit(plotlimitstep, plotlimitr, plotstartspace, plotendspace);
	senvsetareadotsize(nareadotsize);
	senvsetpointdotsize(npointdotsize);
}

void CMainFrame::MojiEnvLoad()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	fontname[RECMAX];
	int		esseisu, arceisu;
	int		ftategaki, fspacekind;
	DBL		dmmfwidth, dmmfhight;
	DBL		dmmmojispace, dmmstrgspace;
	DBL		dmmstringlen;
	DBL		dmmstartx, dmmstarty;
	int		nconerkind, nstartkind;

	_tcscpy_s(fontname, _T("ＭＳ 明朝"));
	esseisu = -1;
	arceisu = -1;
	ftategaki = OFF;
	fspacekind = OFF;
	dmmfwidth = 100.0;
	dmmfhight = 100.0;
	dmmmojispace = 10.0;
	dmmstrgspace = 10.0;
	dmmstringlen = 500.0;
	dmmstartx = 100;
	dmmstarty = 100;
	nconerkind = LEFTUP;
	nstartkind = LEFTUP;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\ses_strg.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if (fp != NULL) {
		baselibCrDelFgets(fontname, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d %d"), &esseisu, &arceisu);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d %d"), &ftategaki, &fspacekind);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf %lf"), &dmmfwidth, &dmmfhight);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf %lf"), &dmmmojispace, &dmmstrgspace);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf"), &dmmstringlen);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf %lf"), &dmmstartx, &dmmstarty);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d %d"), &nstartkind, &nconerkind);
		fclose(fp);
	}
	mojienvsetfontname(fontname);
	mojienvsetesseisuno(esseisu);
	mojienvsetarceisuno(arceisu);
	mojienvsettategakiflag(ftategaki);
	mojienvsetspacekind(fspacekind);
	senvsetconerkind(nconerkind);
	mojienvsetstartkind(nstartkind);
	mojienvsetsize(dmmfwidth, dmmfhight);
	mojienvsetspace(dmmmojispace, dmmstrgspace);
	mojienvsetstrlen(dmmstringlen);
	mojienvsetstart(dmmstartx, dmmstarty);
}

void CMainFrame::FiguEnvLoad()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	int		cntrkind, plgncount;

	cntrkind = CNTRCNTR;
	plgncount = 5;
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\ses_figu.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if (fp != NULL) {
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d\n"), &cntrkind);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d\n"), &plgncount);
		fclose(fp);
	}
	figuenvsetcntrkind(cntrkind);
	figuenvsetplgncount(plgncount);
}

void CMainFrame::WakuEnvLoad()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	*sp;
	TCHAR	papername[HRECMAX];
	TCHAR	grupname[QRECMAX];
	TCHAR	wakupath[FNAMEMAX];
	int		cntrmoveflag;
	int		startmoveflag;
	DBL		startmovexmm, startmoveymm;

	senvgetexepath(exepath);
	_tcscpy_s(grupname, QRECMAX, _T("waku"));
	_stprintf_s(wakupath, FNAMEMAX, _T("%s\\%s"), exepath, grupname);
	_tcscpy_s(papername, HRECMAX, _T("その他"));

	cntrmoveflag = scrngetcntrmoveflag();
	startmoveflag = scrngetstartmoveflag();
	scrngetstartmovemm(&startmovexmm, &startmoveymm);

	_stprintf_s(fname, _T("%s\\soainfo.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if (fp != NULL) {
		if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) { goto EndLabel; } // 会社名}
		if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) { goto EndLabel; } // 書体位置}
		if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) { goto EndLabel; }	// グループ名}
		sp = buf;
		sp = basecsvstrdivcpy(sp, grupname);
		if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) { goto EndLabel; }	// グループ位置}
		sp = buf;
		sp = basecsvstrdivcpy(sp, wakupath);
		if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) { goto EndLabel; }	// 標準書体}
		if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) { goto EndLabel; }	// 外字プログラム}
		if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) { goto EndLabel; }	// 枠作成プログラム}
		if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) { goto EndLabel; }	// 出力機器名}
		if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) { goto EndLabel; }	// 用紙名}
		sp = buf;
		sp = basecsvstrdivcpy(sp, papername);
		if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) { goto EndLabel; }	// 縦書き回転角度}
		if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) { goto EndLabel; }	// 中央寄せフラグ}
		_stscanf_s(buf, _T("%d"), &cntrmoveflag);
		if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) { goto EndLabel; }	// 開始位置フラグ}
		_stscanf_s(buf, _T("%d"), &startmoveflag);
		if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) { goto EndLabel; }	// 横方向移動ＭＭ}
		_stscanf_s(buf, _T("%lf"), &startmovexmm);
		if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) { goto EndLabel; }	// 縦方向移動ＭＭ}
		_stscanf_s(buf, _T("%lf"), &startmoveymm);
		if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) { goto EndLabel; }	// グリッドサイズ
		if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) { goto EndLabel; }	// ＧＤＩ変換倍率
EndLabel:
		fclose(fp);
	}
	if (_taccess_s(wakupath, 0)!= 0) {
		CreateFolderLoop(wakupath);
	}
	wakuenvsetgrupname(grupname);
	senvsetwakpath(wakupath);
	wakuenvsetpapername(papername);
	wakuenvgetpaperelement();
	scrnsetcntrmoveflag(cntrmoveflag);
	scrnsetstartmoveflag(startmoveflag);
	scrnsetstartmovemm(startmovexmm, startmoveymm);

}
void CMainFrame::CreateFolderLoop(LPTSTR lpPath)
{
	LPTSTR sp;
	TCHAR szPath[FNAMEMAX];
	TCHAR szFolder[FNAMEMAX];

	sp = lpPath;
	szPath[0] = _T('\0');
	while (true) {
		if (*sp == _T('\0')) {
			break;
		}
		sp = basedivstrg(sp, szFolder, _T('\\'));
		if (szPath[0] != _T('\0')) {
			_tcscat_s(szPath, _T("\\"));
		}
		_tcscat_s(szPath, szFolder);
		_tmkdir(szPath);
	}
}
void CMainFrame::WakuEnvSave()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	ifname[FNAMEMAX];
	TCHAR	ofname[FNAMEMAX];
	FILE	*ifp;
	FILE	*ofp;
	TCHAR	buf[RECMAX];
	TCHAR	papername[HRECMAX];
	TCHAR	grupname[QRECMAX];
	TCHAR	wakupath[QRECMAX];
	int		cntrmoveflag;
	int		startmoveflag;
	DBL		startmovexmm, startmoveymm;

	senvgetexepath(exepath);
	_stprintf_s(ofname, _T("%s\\soainfo.env"), exepath);
	_stprintf_s(ifname, _T("%s\\soainfo.bak"), exepath);
	CopyFile(ofname, ifname, FALSE);
	_tfopen_s(&ifp, ifname, _T("r"));
	if (ifp == NULL) {
		return;
	}
	_tfopen_s(&ofp, ofname, _T("w"));
	if (ofp == NULL) {
		fclose(ifp);
		return;
	}
	baselibCrDelFgets(buf, RECMAX, ifp);	// 会社名
	_stprintf_s(buf, _T("%s\n"), buf);
	baselibFputs(buf, ofp);
	baselibCrDelFgets(buf, RECMAX, ifp);	// 書体位置
	_stprintf_s(buf, _T("%s\n"), buf);
	baselibFputs(buf, ofp);
	baselibCrDelFgets(buf, RECMAX, ifp);	// グループ名
	wakuenvgetgrupname(grupname);
	_stprintf_s(buf, _T("\"%s\"\n"), grupname);
	baselibFputs(buf, ofp);
	baselibCrDelFgets(buf, RECMAX, ifp);	// グループ位置(枠位置)
	senvgetwakpath(wakupath);
	_stprintf_s(buf, _T("\"%s\"\n"), wakupath);
	baselibFputs(buf, ofp);
	baselibCrDelFgets(buf, RECMAX, ifp);	// 標準書体
	_stprintf_s(buf, _T("%s\n"), buf);
	baselibFputs(buf, ofp);
	baselibCrDelFgets(buf, RECMAX, ifp);	// 外字プログラム
	_stprintf_s(buf, _T("%s\n"), buf);
	baselibFputs(buf, ofp);
	baselibCrDelFgets(buf, RECMAX, ifp);	// 枠作成プログラム
	_stprintf_s(buf, _T("%s\n"), buf);
	baselibFputs(buf, ofp);
	baselibCrDelFgets(buf, RECMAX, ifp);	// 出力機器名
	_stprintf_s(buf, _T("%s\n"), buf);
	baselibFputs(buf, ofp);
	baselibCrDelFgets(buf, RECMAX, ifp);	// 用紙名
	wakuenvgetpapername(papername);
	_stprintf_s(buf, _T("\"%s\"\n"), papername);
	baselibFputs(buf, ofp);
	baselibCrDelFgets(buf, RECMAX, ifp);	// 縦書き回転角度
	_stprintf_s(buf, _T("%s\n"), buf);
	baselibFputs(buf, ofp);
	baselibCrDelFgets(buf, RECMAX, ifp);	// 中央寄せフラグ
	cntrmoveflag = scrngetcntrmoveflag();
	_stprintf_s(buf, _T("%d\n"), cntrmoveflag);
	baselibFputs(buf, ofp);
	baselibCrDelFgets(buf, RECMAX, ifp);	// 開始位置フラグ
	startmoveflag = scrngetstartmoveflag();
	_stprintf_s(buf, _T("%d\n"), startmoveflag);
	baselibFputs(buf, ofp);
	scrngetstartmovemm(&startmovexmm, &startmoveymm);
	baselibCrDelFgets(buf, RECMAX, ifp);	// 横方向移動mm
	_stprintf_s(buf, _T("%1.1lf\n"), startmovexmm);
	baselibFputs(buf, ofp);
	baselibCrDelFgets(buf, RECMAX, ifp);	// 縦方向移動mm
	_stprintf_s(buf, _T("%1.1lf\n"), startmoveymm);
	baselibFputs(buf, ofp);
	baselibCrDelFgets(buf, RECMAX, ifp);	// グリッドサイズ
	_stprintf_s(buf, _T("%s\n"), buf);
	baselibFputs(buf, ofp);
	baselibCrDelFgets(buf, RECMAX, ifp);	// ＧＤＩ変換倍率
	_stprintf_s(buf, _T("%s\n"), buf);
	baselibFputs(buf, ofp);
	fclose(ofp);
	fclose(ifp);
}
void CMainFrame::MemoryEnvLoad()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	senvgetexepath(exepath);
	_stprintf_s(pcApp->m_szBackImagePath, _T("%s\\backimage"), exepath);
	_stprintf_s(pcApp->m_szPhotoImagePath, _T("%s\\photoimage"), exepath);
	_stprintf_s(pcApp->m_szTempImagePath, _T("%s\\tempimage"), exepath);

	_stprintf_s(fname, _T("%s\\smm.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if (fp != NULL) {
		baselibCrDelFgets(buf, RECMAX, fp); // コメントスキップ
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), pcApp->m_szBackImagePath, FNAMEMAX);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), pcApp->m_szPhotoImagePath, FNAMEMAX);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), pcApp->m_szTempImagePath, FNAMEMAX);
		fclose(fp);
	}
	//else{
		CreateDirectory(pcApp->m_szBackImagePath, NULL);
		CreateDirectory(pcApp->m_szPhotoImagePath, NULL);
		CreateDirectory(pcApp->m_szTempImagePath, NULL);
	//}
}
void CMainFrame::MemoryEnvSave()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\smm.env"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	if (fp != NULL) {
		_stprintf_s(buf, _T("//\n"));
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%s\n"), pcApp->m_szBackImagePath);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%s\n"), pcApp->m_szPhotoImagePath);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%s\n"), pcApp->m_szTempImagePath);
		baselibFputs(buf, fp);
		fclose(fp);
	}
}
void CMainFrame::DivDrawEnvLoad()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	DBL		rag, mini, max;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\drawdiv.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if (fp != NULL) {
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf %lf %lf"), &rag, &mini, &max);
		senvsetdrawdivbeze(rag, mini, max);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf %lf %lf"), &rag, &mini, &max);
		senvsetdrawdivarc(rag, mini, max);
		fclose(fp);
	}
}

void CMainFrame::ScrnEnvSave()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	DBL		dclrlebel;
	DBL		dmmx, dmmy;
	int		ncntrkind;
	TCHAR	sportname[QRECMAX];
	TCHAR	splotername[QRECMAX];
	int		npcmdtype;
	DBL		dpxsize, dpysize;
	DBL		dpstep;
	DBL		dpsx, dpsy;
	int		nparcflag, npfillflag;
	int		nphomemode, nplinemode, npomitmode;
	TCHAR	datpath[FNAMEMAX];
	TCHAR	tmppath[FNAMEMAX];
	TCHAR	wakpath[FNAMEMAX];
	TCHAR	kmnpath[FNAMEMAX];
	TCHAR	outpath[FNAMEMAX];
	TCHAR	partpath[FNAMEMAX];
	TCHAR	btppath[FNAMEMAX];
	TCHAR	fntpath[FNAMEMAX];

	scrngetmmsize(&dmmx, &dmmy);
	poutcntrkindget(&ncntrkind);
	poutportnameget(sportname);
	poutploternameget(splotername);
	pouttypeget(&npcmdtype);
	poutmmsizeget(&dpxsize, &dpysize);
	poutstepget(&dpstep);
	poutmmstartget(&dpsx, &dpsy);
	poutarcflagget(&nparcflag);
	poutfillflagget(&npfillflag);
	poutmodeget(&nphomemode, &nplinemode, &npomitmode);
	senvgetdatpath(datpath);
	senvgetwakpath(wakpath);
	senvgetkmnpath(kmnpath);
	senvgetoutpath(outpath);
	senvgetpartpath(partpath);
	senvgetbtppath(btppath);
	mojienvgetfontpath(fntpath);
	senvgettmppath(tmppath);
	dclrlebel = 0;
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\ses_sys.env"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	if (fp != NULL) {
		_stprintf_s(buf, _T("%s\n"), m_szKaisyaName);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%lf %lf\n"), dmmx, dmmy);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d\n"), ncntrkind);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%s\n"), sportname);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%s %d %lf %lf %lf %lf %lf\n"),
				splotername, npcmdtype, 
				dpxsize, dpysize, dpstep, 
				dpsx, dpsy);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d %d %d %d %d\n"),
				nparcflag, npfillflag, 
				nphomemode, nplinemode, npomitmode);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%lf\n"), dclrlebel);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%s\n"), fntpath);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%s\n"), datpath);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%s\n"), outpath);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%s\n"), kmnpath);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%s\n"), wakpath);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%s\n"), partpath);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%s\n"), tmppath);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%s\n"), btppath);
		baselibFputs(buf, fp);
		fclose(fp);
	}
}

void CMainFrame::ScrnAddEnvSave()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	pltname[FNAMEMAX];
	int		cntrmoveflag;
	int		startmoveflag;
	DBL		startmovexmm, startmoveymm;
	DBL		plotlimitstep, plotlimitr;
	DBL		plotstartspace, plotendspace;
	int		nareadotsize;
	int		npointdotsize;

	senvgetcrtpltname(pltname);
	cntrmoveflag = scrngetcntrmoveflag();
	startmoveflag = scrngetstartmoveflag();
	scrngetstartmovemm(&startmovexmm, &startmoveymm);
	poutgetparaminit(&plotlimitstep, &plotlimitr, &plotstartspace, &plotendspace);
	senvgetareadotsize(&nareadotsize);
	senvgetpointdotsize(&npointdotsize);

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\ses_sys2.env"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	if (fp != NULL) {
		_stprintf_s(buf, _T("%s\n"), pltname);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d\n"), cntrmoveflag);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d\n"), startmoveflag);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%4.1lf %4.1lf\n"), startmovexmm, startmoveymm);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d\n"), m_nTTEisuFlag);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%4.1lf %4.1lf %4.1lf %4.1lf\n"),
				plotlimitstep, plotlimitr, plotstartspace, plotendspace);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d\n"), nareadotsize);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d\n"), npointdotsize);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d\n"), m_nStatusBar);
		baselibFputs(buf, fp);
		fclose(fp);
	}
}

void CMainFrame::MojiEnvSave()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	fontname[QRECMAX];
	TCHAR	buf[RECMAX];
	int		esseisu, arceisu;
	int		ftategaki, fspacekind;
	DBL		dmmfwidth, dmmfhight;
	DBL		dmmmojispace, dmmstrgspace;
	DBL		dmmstringlen;
	DBL		dmmstartx, dmmstarty;
	int		nconerkind, nstartkind;

	mojienvgetfontname(fontname);
	mojienvgetesseisuno(&esseisu);
	mojienvgetarceisuno(&arceisu);
	mojienvgettategakiflag(&ftategaki);
	mojienvgetspacekind(&fspacekind);
	senvgetconerkind(&nconerkind);
	mojienvgetstartkind(&nstartkind);
	mojienvgetsize(&dmmfwidth, &dmmfhight);
	mojienvgetspace(&dmmmojispace, &dmmstrgspace);
	mojienvgetstrlen(&dmmstringlen);
	mojienvgetstart(&dmmstartx, &dmmstarty);

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\ses_strg.env"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	if (fp != NULL) {
		_stprintf_s(buf, _T("%s\n"), fontname);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d %d\n"), esseisu, arceisu);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d %d\n"), ftategaki, fspacekind);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%lf %lf\n"), dmmfwidth, dmmfhight);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%lf %lf\n"), dmmmojispace, dmmstrgspace);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%lf\n"), dmmstringlen);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%lf %lf\n"), dmmstartx, dmmstarty);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d %d\n"), nstartkind, nconerkind);
		baselibFputs(buf, fp);
		fclose(fp);
	}
}

void CMainFrame::FiguEnvSave()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	int		cntrkind, plgncount;
	TCHAR	buf[RECMAX];

	figuenvgetcntrkind(&cntrkind);
	figuenvgetplgncount(&plgncount);

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\ses_figu.env"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	if (fp != NULL) {
		_stprintf_s(buf, _T("%d\n"), cntrkind);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d\n"), plgncount);
		baselibFputs(buf, fp);
		fclose(fp);
	}
}
void CMainFrame::FontEnvLoad()
{
	FILE	*fp;
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	char	buf[RECMAX];
	int		len;
	int		fonttype;
	char	sSFName[HRECMAX];
	char	sFFName[HRECMAX];
	TCHAR	tSFName[HRECMAX];
	TCHAR	tFFName[HRECMAX];
	int		eisuno;

	mojienvflistinit();
	mojienvflistopen(_T("wb"));
	mojienvflistclose();
	m_nFontSetIdx = 0;
	senvgetexepath(exepath);
	_stprintf_s(fname,_T("%s\\fontname.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if (fp != NULL) {
		while(1){
			if(fgets(buf, RECMAX, fp) == NULL){
				break;
			}
			len = strlen(buf);
			buf[len - 1] = '\0';
			sscanf_s(buf, "%d %s\n", &fonttype, sSFName, HRECMAX);
			strcpy_s(sFFName, HRECMAX, &(buf[30]));
			baselibCharStrToTCHARStr(sSFName, tSFName, HRECMAX);
			baselibCharStrToTCHARStr(sFFName, tFFName, HRECMAX);
			mojienvflistopen(_T("r+b"));
			mojienvsetflist(fonttype, tSFName, tFFName);
			mojienvflistclose();
			m_nFontSetIdx++;
		}
		fclose(fp);
	}
	mojienvgetesseisuno(&eisuno);
	if(eisuno != -1){
		fonttype = ES0FONT; 
		sprintf_s(sSFName, HRECMAX, "ＥＳ英数no%02d", eisuno);
		sprintf_s(sFFName, HRECMAX, "Ｅ英数%02d", eisuno);
		baselibCharStrToTCHARStr(sSFName, tSFName, HRECMAX);
		baselibCharStrToTCHARStr(sFFName, tFFName, HRECMAX);
		mojienvflistopen(_T("r+b"));
		mojienvsetflist(fonttype, tSFName, tFFName);
		mojienvflistclose();
		m_nFontSetIdx++;
	}
	mojienvgetarceisuno(&eisuno);
	if(eisuno != -1){
		fonttype = ES0FONT; 
		sprintf_s(sSFName, HRECMAX, "ＡＲ英数%04d", eisuno);
		sprintf_s(sFFName, HRECMAX, "Ａ英数%04d", eisuno);
		baselibCharStrToTCHARStr(sSFName, tSFName, HRECMAX);
		baselibCharStrToTCHARStr(sFFName, tFFName, HRECMAX);
		mojienvflistopen(_T("r+b"));
		mojienvsetflist(fonttype, tSFName, tFFName);
		mojienvflistclose();
		m_nFontSetIdx++;
	}
	TrueTypeFontListSet();
}

void CMainFrame::TrueTypeFontListSet()
{
	CTonApp*	theApp;
	CString 	str;
	LOGFONT		lf;
	HDC			hDC;

	memset(&lf, 0, sizeof(LOGFONT));
	theApp = (CTonApp*)AfxGetApp();
	hDC = theApp->m_dcScreen.m_hDC;

	ASSERT(hDC != NULL);
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfFaceName[0] = _T('\0');
	::EnumFontFamiliesEx(hDC, &lf,
		(FONTENUMPROC)EnumFamTategakiCallBackEx, NULL, NULL);
	::EnumFontFamiliesEx(hDC, &lf,
		(FONTENUMPROC)EnumFamOpenTypeCallBackEx, NULL, NULL);

}
BOOL CALLBACK AFX_EXPORT CMainFrame::EnumFamTategakiCallBackEx(ENUMLOGFONTEX* lpnlf,
	NEWTEXTMETRICEX* lpntm, int nFontType, LPVOID lpAdrs)
{
	int			fonttype;
	LPTSTR		lpTFName;
	int			len;
	TCHAR		tSFName[QRECMAX];

	if (RASTER_FONTTYPE < nFontType && lpnlf->elfFullName[0] == '@') {
		mojienvflistopen(_T("r+b"));
		fonttype = TT1FONT;
		lpTFName = (LPTSTR)(&(lpnlf->elfFullName[1]));
		len = _tcslen(lpTFName) + 1;
		_tcscpy_s(tSFName, len, lpTFName);
		baselibSpaceDelete(tSFName);
		mojienvsetflist(fonttype, tSFName, lpTFName);
		mojienvflistclose();
		m_nCount++;
	}
	return(TRUE);
}
BOOL CALLBACK AFX_EXPORT CMainFrame::EnumFamOpenTypeCallBackEx(ENUMLOGFONTEX* lpnlf,
	NEWTEXTMETRICEX* /*lpntm*/, int nFontType, LPVOID lpAdrs)
{
	int			fonttype;
	LPTSTR		lpTFName;
	int			len;
	TCHAR		tSFName[QRECMAX];

	if (RASTER_FONTTYPE < nFontType  && lpnlf->elfFullName[0] != '@') {
		mojienvflistopen(_T("r+b"));
		fonttype = TT2FONT;
		lpTFName = (LPTSTR)(lpnlf->elfFullName);
		len = _tcslen(lpTFName) + 1;
		_tcscpy_s(tSFName, len, lpTFName);
		baselibSpaceDelete(tSFName);
		mojienvsetflist(fonttype, tSFName, lpTFName);
		mojienvflistclose();
		m_nCount++;
	}
	return(TRUE);
}
BOOL CALLBACK AFX_EXPORT CMainFrame::EnumFamAsciiCallBackEx(ENUMLOGFONTEX* lpnlf,
	 NEWTEXTMETRICEX* /*lpntm*/, int nFontType, LPVOID lpAdrs)
{
	int			fonttype;
	LPTSTR		lpTFName;
	int			len;
	TCHAR		tSFName[QRECMAX];

	if(RASTER_FONTTYPE < nFontType && lpnlf->elfFullName[0] != '@'){
		if(lpnlf->elfLogFont.lfCharSet == ANSI_CHARSET){
			mojienvflistopen(_T("r+b"));
			fonttype = TT3FONT;
			lpTFName = (LPTSTR)(lpnlf->elfFullName);
			len = _tcslen(lpTFName) + 1;
			_tcscpy_s(tSFName, len, lpTFName);
			baselibSpaceDelete(tSFName);
			mojienvsetflist(fonttype, tSFName, lpTFName);
			mojienvflistclose();
			m_nCount++;
		}
	}
	return(TRUE);
}
CTonView* CMainFrame::GetCrtView()
{
	return(m_pcCrtView);
}

int CMainFrame::CheckProtectFile()
{
	TCHAR	fname[FNAMEMAX];
	TCHAR	szWinDir[FNAMEMAX];
	FILE	*fp;

	::GetSystemDirectory(szWinDir, FNAMEMAX);
	_stprintf_s(fname, _T("%s\\winsleep.sys"), szWinDir);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		_stprintf_s(fname,_T("%s\\winsleep.drv"), szWinDir);
		_tfopen_s(&fp, fname, _T("r"));
		if(fp == NULL){
			return -1;
		}
	}
	fclose(fp);
	return(0);
}

int CMainFrame::CheckProtectUsb()
{
#ifdef _DEBUG
		return(0);
#else
	long	nID, nRet;
	HMODULE		hModule;
	LPUSBPRTCHK	fncUsbPrtCheck;

	hModule = LoadLibrary(_T("UsbPrt.dll"));
	if(hModule == 0){
		return(-1);
	}
	fncUsbPrtCheck = (LPUSBPRTCHK)GetProcAddress(hModule, "Check");
	nID = 0x05;
	nRet = fncUsbPrtCheck(nID);
	FreeLibrary(hModule);
	if(nRet == nID){
		return(0);
	}else{
		return(-1);
	}
#endif
}

void CMainFrame::OnWakuTateyoko()
{
	int	ftategaki;

	mojienvgettategakiflag(&ftategaki);
	if(ftategaki == -1){
		return;
	}
	m_wndWakuBar3.TateyokoChange();
	if(m_pcCrtView != NULL){
		m_pcCrtView->CallOnWakuApply();
	}
}

void CMainFrame::OnWakuConerkind() 
{
	m_wndWakuBar3.ConerKindChange();
}

void CMainFrame::OnWakuStartkind() 
{
	m_wndWakuBar3.StartKindChange();
}

void CMainFrame::OnFileSlctprint()
{
	m_wndWakuBarSide.SetPrint();
}

void CMainFrame::OnFileSavepaper()
{
	m_wndWakuBarSide.SetPaperSave();
}

void CMainFrame::OnFileCenterkind()
{
	m_wndWakuBarSide.CenterkindChange();
}

void CMainFrame::OnFileSetspace() 
{
	m_wndWakuBarSide.SetPageSpace();
	if(m_pcCrtView != NULL){
		m_pcCrtView->Invalidate(TRUE);
	}
}

void CMainFrame::InitIme()
{
}

void CMainFrame::EndIme()
{
}

void CMainFrame::OnIme()
{
	m_hImc = ::ImmCreateContext();
	if(m_hImc == NULL){
		return;
	}
	m_hOldImc = ::ImmAssociateContext(m_hWnd, m_hImc);
	::ImmSetOpenStatus(m_hImc, true);	
}

void CMainFrame::OffIme()
{
	::ImmSetOpenStatus(m_hImc, false);	
	if(m_hImc != NULL){
		::ImmAssociateContext(m_hWnd, m_hOldImc);
		::ImmDestroyContext(m_hImc);
	}
}


void CMainFrame::OnWakuPaperadd() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
}

void CMainFrame::OnViewStatusBar() 
{
	if(m_wndStatusBar.IsWindowVisible()){
		m_wndStatusBar.ShowWindow(SW_HIDE);
		m_nStatusBar = OFF;
	}
	else{
		m_wndStatusBar.ShowWindow(SW_SHOW);
		m_nStatusBar = ON;
	}
	RecalcLayout();
}

void CMainFrame::OnUpdateViewStatusBar(CCmdUI* pCmdUI) 
{
	if(pCmdUI->m_nID == ID_VIEW_STATUS_BAR){
		if(m_wndStatusBar.IsWindowVisible()){
			pCmdUI->SetCheck(1);
		}
		else{
			pCmdUI->SetCheck(0);
		}
	}
}

