// MainFrm.cpp : CMainFrame クラスの動作の定義を行います。
//

#include "stdafx.h"

#include <stdio.h>
#include <conio.h>
#include <direct.h>
#include <io.h>
#include <stdlib.h>

//#include "Splash.h"

#include "MainFrm.h"
#include "..\UsbPrt.h"
#include "..\cmnsrc\Dlgtxtin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 1000 * 60 * 10 
#define PROTECTTIMERINTERVAL 600000

typedef struct{
    GLOBALHANDLE	hGmem;
	int				nCount;
}ENUMER;

extern "C" {
	void curveinit();
	int  baseiskanji(int far hc);
	LPSTR basecsvstrdivcpy(LPSTR sp, LPSTR str);
	int plgnfillinit();
	void plgnfillend();
	void plgnfillareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	void diblibinit();
	void diblibend();
	void dizargbtblinit(char *fname);
	void scrninit();
	void scrnend();
	void scrngetrltvsize(DBL *pdRXsize, DBL *pdRYsize);
	void scrngetmmsize(LPDBL pdmmxsize, LPDBL pdmmysize);
	void scrnsetmmsize(DBL Xsize, DBL Ysize);
	void scrnsetwindowsizeelement(DBL xsize, DBL ysize);
	void scrnsetcntrmoveflag(int flag);
	int scrngetcntrmoveflag();
	void scrnsetstartmoveflag(int flag);
	int scrngetstartmoveflag();
	void scrnsetstartmovemm(DBL xmm, DBL ymm);
	void scrngetstartmovemm(LPDBL xmm, LPDBL ymm);
	void senvinit();
	void senvend();
	void senvinitscrnclr();
	void senvendscrnclr();
	void senvsetcrtpltname(LPSTR pltname);
	void senvgetcrtpltname(LPSTR pltname);
	void senvsetexepath(LPSTR path);
	void senvgetexepath(LPSTR path);
	void senvgetwakpath(LPSTR path);
	void senvsetwakpath(LPSTR path);
	void senvgetkmnpath(LPSTR path);
	void senvsetkmnpath(LPSTR path);
	void senvgetdatpath(LPSTR path);
	void senvsetdatpath(LPSTR path);
	void senvgetoutpath(LPSTR path);
	void senvsetoutpath(LPSTR path);
	void senvgetpartpath(LPSTR path);
	void senvsetpartpath(LPSTR path);
	void senvgettmppath(LPSTR path);
	void senvsettmppath(LPSTR path);
	void senvgetbtppath(LPSTR path);
	void senvsetbtppath(LPSTR path);
	void senvsetdrawdivbeze(DBL rag, DBL mini, DBL max);
	void senvsetdrawdivarc(DBL rag, DBL mini, DBL max);
	void senvsetareadotsize(int dotsize);
	void senvgetareadotsize(int *dotsize);
	void senvsetpointdotsize(int dotsize);
	void senvgetpointdotsize(int *dotsize);
	void poutgetparaminit(LPDBL lmtstep, LPDBL limitr, LPDBL smm, LPDBL emm);
	void poutsetparaminit(DBL lmtstep, DBL limitr, DBL smm, DBL emm);
	void pouttypeset(int type);
	void pouttypeget(int *type);
	void poutmmsizeset(DBL xsize, DBL ysize);
	void poutmmsizeget(DBL *xsize, DBL *ysize);
	void poutstepset(DBL step);
	void poutstepget(DBL *step);
	void poutmmstartset(DBL sx, DBL sy);
	void poutmmstartget(DBL *sx, DBL *sy);
	void pouttimeset(DBL xtime, DBL ytime);
	void pouttimeget(DBL *xtime, DBL *ytime);
	void poutcntrkindset(int kind);
	void poutcntrkindget(int *kind);
	void poutportnameset(LPSTR port);
	void poutportnameget(LPSTR port);
	void poutploternameset(LPSTR pltname);
	void poutploternameget(LPSTR pltname);
	void poutarcflagset(int flag);
	void poutarcflagget(int *flag);
	void poutfillflagset(int flag);
	void poutfillflagget(int *flag);
	void poutmodeget(int *homemode, int *linemode, int *omitmode);
	void poutmodeset(int homemode, int linemode, int omitmode);
	void mojienvinit();
	void mojienvflistinit();
	void mojienvflistopen(LPSTR lpMode);
	void mojienvflistclose();
	void mojienvsetflist(int idx, int type, LPSTR sfname, LPSTR ffname);
	void mojienvgetflist(int idx, LPINT type, LPSTR sfname, LPSTR ffname);
	void mojienvgetfontpath(LPSTR fontpath);
	void mojienvsetfontpath(LPSTR fontpath);
	void mojienvgetfontname(LPSTR fontname);
	void mojienvsetfontname(LPSTR fontname);
	void mojienvgetesseisuno(LPINT esseisu);
	void mojienvsetesseisuno(int esseisu);
	void mojienvgetarceisuno(LPINT arceisu);
	void mojienvsetarceisuno(int arceisu);
	void mojienvgettategakiflag(LPINT tateflg);
	void mojienvsettategakiflag(int tateflg);
	void mojienvgetspacekind(LPINT spkflg);
	void mojienvsetspacekind(int spkflg);
	void senvsetconerkind(int nkind);
	void senvgetconerkind(LPINT nkind);
	void mojienvgetstartkind(LPINT nstartkd);
	void mojienvsetstartkind(int nstartkd);
	void mojienvsetsize(DBL dw, DBL dh);
	void mojienvsetspace(DBL dmsp, DBL dlsp);
	void mojienvsetstrlen(DBL dstrlen);
	void mojienvsetstart(DBL dsx, DBL dsy);
	void mojienvgetsize(LPDBL dw, LPDBL dh);
	void mojienvgetspace(LPDBL dmsp, LPDBL dlsp);
	void mojienvgetstrlen(LPDBL dstrlen);
	void mojienvgetstart(LPDBL dsx, LPDBL dsy);
	void figuenvgetcntrkind(LPINT cntrkind);
	void figuenvsetcntrkind(int cntrkind);
	void figuenvgetplgncount(LPINT plgncount);
	void figuenvsetplgncount(int plgncount);
	void wakuenvsetkaisyaname(LPSTR name);
	void wakuenvsetgrupname(LPSTR name);
	void wakuenvgetgrupname(LPSTR name);
	void wakuenvsetpapername(LPSTR name);
	void wakuenvgetpapername(LPSTR name);
	void wakuenvgetpaperelement();
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
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
	ID_SEPARATOR,           // ステータス ライン インジケータ
	ID_INDICATOR_TEXT1,
	ID_INDICATOR_TEXT1,
	ID_INDICATOR_TEXT3,
	ID_SEPARATOR,
};

static CSize GetBaseUnits()
{
	/*
	CTonApp* theApp;
	CFont fnt;
	CFont *pFont;

	theApp = (CTonApp*)AfxGetApp();
  	fnt.Attach(GetStockObject(theApp->m_nDefFont));
	ASSERT(&fnt != NULL);
	ASSERT(fnt.GetSafeHandle() != NULL);

	pFont = theApp->m_dcScreen.SelectObject(&fnt);
	TEXTMETRIC tm;
	VERIFY(theApp->m_dcScreen.GetTextMetrics(&tm));
	theApp->m_dcScreen.SelectObject(pFont);

	return CSize(tm.tmAveCharWidth, tm.tmHeight);
	*/
	return CSize(8, 16);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの構築/消滅
#define	PROTECT	1
//#define	NONPROTECT	1

CMainFrame::CMainFrame()
{
	char	szExePath[FNAMEMAX];
	char	fname[FNAMEMAX];

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
	InitExePath(szExePath);
	senvsetexepath(szExePath);
	GetWindowsDirectory(m_szEnvPath, FNAMEMAX);
	m_szEnvPath[2] = '\0';
	strcat(m_szEnvPath, "\\UsesProgram");
	mkdir(m_szEnvPath);
	strcat(m_szEnvPath, "\\Itiban");
	mkdir(m_szEnvPath);

	curveinit();
	senvinit();
	scrninit();
	wsprintf(fname, "%s\\defenv\\clrtrns.env", szExePath);
	dizargbtblinit(fname);
	mojienvinit();
	diblibinit();

	EnvLoad();

	m_pcFSF1Font = new CFSF1Font(0);
	m_pcWNTTFont = new CWNTTFont(0);
	m_pcESF0Font = new CESF0Font(0);
	m_pcESF1Font = new CESF1Font(0);
	m_pcWNTTFont->FSF1ClassSet(m_pcFSF1Font);
	m_pcESF0Font->FSF1ClassSet(m_pcFSF1Font);
	m_pcESF1Font->FSF1ClassSet(m_pcFSF1Font);
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
	EndIme();
	senvendscrnclr();
	senvend();
	scrnend();
	plgnfillend();
	diblibend();
	EnvSave();
	mojienvflistclose();
	delete(m_pcESF1Font);
	delete(m_pcESF0Font);
	delete(m_pcWNTTFont);
	delete(m_pcFSF1Font);
}

void CMainFrame::InitExePath(LPSTR szExePath)
{
	int		len;

	len = ::GetModuleFileName(AfxGetInstanceHandle( ), szExePath, FNAMEMAX);
	while(1){
		if(len <= 0){
			szExePath[0] = '\0';
			break;
		}
		if(szExePath[len] == ':' || szExePath[len] == '\\'){
			szExePath[len] = '\0';
			break;
		}
		len--;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	char	msgbuf[RECMAX];
	int		ret;
	int		id, nDay, nStat;

#ifdef PROTECT 
	CTonApp *pcApp = (CTonApp*)::AfxGetApp();
	ret = CheckProtectFile();
	if(ret == -1){
		ret = CheckProtectUsb();
		if(ret == -1){
			sprintf(msgbuf, "USBプロテクトエラーです\n販売店にご連絡ください");
			MessageBox(msgbuf, "筆耕システム 枠作成システム", MB_OK);
		}
	}
	if(ret == -1){
		return -1;
	}
#endif
	lpCreateStruct->lpszClass = "筆耕システム 枠作成システム";
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

	if(!CreateClrBar())
		return -1;
	if(!CreateWakuBar1())
		return -1;
	if(!CreateWakuBar2())
		return -1;
	if(!CreateWakuBar3())
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
	m_wndClrBar.EnableDocking(CBRS_ALIGN_BOTTOM);
	m_wndWakuBar1.EnableDocking(CBRS_ALIGN_TOP);
	m_wndWakuBar2.EnableDocking(CBRS_ALIGN_TOP);
	m_wndWakuBar3.EnableDocking(CBRS_ALIGN_BOTTOM);
	m_wndWakuBarSide.EnableDocking(CBRS_ALIGN_RIGHT);
	m_wndLeftSideBar.EnableDocking(CBRS_ALIGN_LEFT);
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
	// TODO: この位置で CREATESTRUCT cs を修正して、Window クラスやスタイルを
	//       修正してください。

	//cs.x = 0;
	//cs.y = 0;
	//cs.cx = GetSystemMetrics(SM_CXSCREEN);
	//cs.cy = GetSystemMetrics(SM_CYSCREEN);
	//MDIMaximize(this); 
	cs.dwExStyle |= WS_EX_TOPMOST;

	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの診断

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

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メッセージ ハンドラ
void CMainFrame::StartTimer()
{
	m_nTimerID = SetTimer(1, PROTECTTIMERINTERVAL, NULL);
}
void CMainFrame::OnTimer(UINT nIDEvent) 
{
	char	msgbuf[RECMAX];
	int		ret;

	CMDIFrameWnd::OnTimer(nIDEvent);
#ifdef PROTECT 
	ret = CheckProtectFile();
	if(ret == -1){
		ret = CheckProtectUsb();
		if(ret == -1){
			sprintf(msgbuf, "USBプロテクトエラーです\n販売店にご連絡ください");
			MessageBox(msgbuf, "筆耕システム 枠作成システム", MB_OK);
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
	char	xbuf[QRECMAX];
	char	ybuf[QRECMAX];
	char	msgbuf[QRECMAX];

	if(xabs > 10000 || yabs > 10000){
		sprintf(xbuf, "%6.2lf", xabs/1000);
		sprintf(ybuf, "%6.2lf", yabs/1000);
		sprintf(msgbuf, "(%s,%s)M", xbuf, ybuf);
	}
	else{
		sprintf(xbuf, "%6.1lf", xabs);
		sprintf(ybuf, "%6.1lf", yabs);
		sprintf(msgbuf, "(%s,%s)mm", xbuf, ybuf);
	}
	HelpTextOut(4, msgbuf);
}

void CMainFrame::HelpTextOut(int idx, LPSTR msgbuf)
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

void CMainFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CMDIFrameWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnPaint() 
{
	CPaintDC dc(this);
	if(m_nFirstFlag == TRUE){
		m_nFirstFlag = FALSE;
		MDIMaximize(this);
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
	char	exepath[FNAMEMAX];
	char	crctfname[FNAMEMAX];
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
	sprintf(crctfname, "%s\\outcrect.dat", exepath);
	if((crctfp = fopen(crctfname, "w+")) != NULL){
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
	char	exepath[FNAMEMAX];
	char	exedrive[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[HRECMAX];
	DBL		dclrlebel;
	DBL		dmmx, dmmy;
	int		ncntrkind;
	char	sportname[QRECMAX];
	char	splotername[QRECMAX];
	int		npcmdtype;
	DBL		dpxsize, dpysize;
	DBL		dpstep;
	DBL		dpsx, dpsy;
	int		nparcflag, npfillflag;
	int		nphomemode, nplinemode, npomitmode;
	DBL		dpxtime, dpytime;
	char	datpath[FNAMEMAX];
	char	tmppath[FNAMEMAX];
	char	wakpath[FNAMEMAX];
	char	kmnpath[FNAMEMAX];
	char	partpath[FNAMEMAX];
	char	outpath[FNAMEMAX];
	char	btppath[FNAMEMAX];
	char	fntpath[FNAMEMAX];

	senvgetexepath(exepath);
	strcpy(exedrive, exepath);
	exedrive[2] = '\0';
	strcpy(m_szKaisyaName, "未設定");
	dmmx = 210; dmmy = 298;
	ncntrkind = LEFTDOWN;
	strcpy(splotername, "HPGL");
	npcmdtype = HPGL;
	dpxsize = 590.0; dpysize = 430.0;
	dpstep = 0.1;
	dpsx = -295; dpsy = -215;
	nparcflag = OFF; npfillflag = OFF;
	nphomemode = ON; nplinemode = ON; npomitmode = ON;
	dpxtime = 1.0; dpytime = 1.0;
	strcpy(sportname, "LPT1:");
	sprintf(fntpath, "%s\\moji", exepath);
	sprintf(datpath, "%s\\data", exepath);
	sprintf(outpath, "%s\\dout", exepath);
	sprintf(kmnpath, "%s\\kamon", exepath);
	sprintf(wakpath, "%s\\waku", exepath);
	sprintf(wakpath, "%s\\part", exepath);
	sprintf(tmppath, "%s\\temp", exepath);
	sprintf(btppath, "%s\\tbmp", exepath);

	sprintf(fname, "%s\\ses_sys.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", m_szKaisyaName);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf %lf", &dmmx, &dmmy);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &ncntrkind);
		CrDelFgets(sportname, HRECMAX, fp);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s %d %lf %lf %lf %lf %lf",
			splotername, &npcmdtype, 
			&dpxsize, &dpysize, &dpstep, 
			&dpsx, &dpsy);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d %d %d %d %d",
			&nparcflag, &npfillflag, 
			&nphomemode, &nplinemode, &npomitmode);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf", &dclrlebel);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", fntpath);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", datpath);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", outpath);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", kmnpath);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", wakpath);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", partpath);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", tmppath);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", btppath);
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
}

void CMainFrame::ScrnAddEnvLoad()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[HRECMAX];
	char	pltname[FNAMEMAX];
	int		cntrmoveflag;
	int		startmoveflag;
	DBL		startmovexmm, startmoveymm;
	DBL		plotlimitstep, plotlimitr;
	DBL		plotstartspace, plotendspace;
	int		nareadotsize;
	int		npointdotsize;

	cntrmoveflag = 0;
	startmoveflag = 0;
	startmovexmm = 0.0; startmoveymm = 0.0;
	m_nTTEisuFlag = ON;
	plotlimitstep = 30000.0;
	plotlimitr = 30000.0;
	plotstartspace = 0;
	plotendspace = 0;
	npointdotsize = 6;
	nareadotsize = 6;
	npointdotsize = 3;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\ses_sys2.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", pltname);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &cntrmoveflag);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &startmoveflag);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf %lf", &startmovexmm, &startmoveymm);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &m_nTTEisuFlag);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf %lf %lf %lf",
				&plotlimitstep, &plotlimitr,
				&plotstartspace, &plotendspace);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &nareadotsize);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &npointdotsize);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &m_nStatusBar);
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
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[HRECMAX];
	char	fontname[QRECMAX];
	int		esseisu, arceisu;
	int		ftategaki, fspacekind;
	DBL		dmmfwidth, dmmfhight;
	DBL		dmmmojispace, dmmstrgspace;
	DBL		dmmstringlen;
	DBL		dmmstartx, dmmstarty;
	int		nconerkind, nstartkind;

	strcpy(fontname, "ＭＳ 明朝");
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
	sprintf(fname, "%s\\ses_strg.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		CrDelFgets(fontname, HRECMAX, fp);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d %d", &esseisu, &arceisu);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d %d", &ftategaki, &fspacekind);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf %lf", &dmmfwidth, &dmmfhight);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf %lf", &dmmmojispace, &dmmstrgspace);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf", &dmmstringlen);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf %lf", &dmmstartx, &dmmstarty);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d %d", &nstartkind, &nconerkind);
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
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[HRECMAX];
	int		cntrkind, plgncount;

	cntrkind = CNTRCNTR;
	plgncount = 5;
	senvgetexepath(exepath);
	sprintf(fname, "%s\\ses_figu.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d\n", &cntrkind);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d\n", &plgncount);
		fclose(fp);
	}
	figuenvsetcntrkind(cntrkind);
	figuenvsetplgncount(plgncount);
}

void CMainFrame::WakuEnvLoad()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	*sp;
	char	papername[HRECMAX];
	char	grupname[QRECMAX];
	char	wakupath[QRECMAX];
	int		cntrmoveflag;
	int		startmoveflag;
	DBL		startmovexmm, startmoveymm;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\soainfo.env", exepath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}

	cntrmoveflag = scrngetcntrmoveflag();
	startmoveflag = scrngetstartmoveflag();
	scrngetstartmovemm(&startmovexmm, &startmoveymm);

	if(CrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;} // 会社名}
	if(CrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;} // 書体位置}
	if(CrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// グループ名}
	sp = buf;
	sp = basecsvstrdivcpy(sp, grupname);
	wakuenvsetgrupname(grupname);
	if(CrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// グループ位置}
	sp = buf;
	sp = basecsvstrdivcpy(sp, wakupath);
	senvsetwakpath(wakupath);
	if(CrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 標準書体}
	if(CrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 外字プログラム}
	if(CrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 枠作成プログラム}
	if(CrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 出力機器名}
	if(CrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 用紙名}
	sp = buf;
	sp = basecsvstrdivcpy(sp, papername);
	wakuenvsetpapername(papername);
	wakuenvgetpaperelement();
	if(CrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 縦書き回転角度}
	if(CrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 中央寄せフラグ}
	sscanf(buf, "%d", &cntrmoveflag);
	if(CrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 開始位置フラグ}
	sscanf(buf, "%d", &startmoveflag);
	if(CrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 横方向移動ＭＭ}
	sscanf(buf, "%lf", &startmovexmm);
	if(CrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 縦方向移動ＭＭ}
	sscanf(buf, "%lf", &startmoveymm);
	if(CrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// グリッドサイズ
	if(CrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// ＧＤＩ変換倍率
	scrnsetcntrmoveflag(cntrmoveflag);
	scrnsetstartmoveflag(startmoveflag);
	scrnsetstartmovemm(startmovexmm, startmoveymm);
EndLabel:
	fclose(fp);

}

void CMainFrame::WakuEnvSave()
{
	char	exepath[FNAMEMAX];
	char	ifname[FNAMEMAX];
	char	ofname[FNAMEMAX];
	FILE	*ifp;
	FILE	*ofp;
	char	buf[RECMAX];
	char	papername[HRECMAX];
	char	grupname[QRECMAX];
	char	wakupath[QRECMAX];
	int		cntrmoveflag;
	int		startmoveflag;
	DBL		startmovexmm, startmoveymm;

	senvgetexepath(exepath);
	sprintf(ofname, "%s\\soainfo.env", exepath);
	sprintf(ifname, "%s\\soainfo.bak", exepath);
	CopyFile(ofname, ifname, FALSE);

	if((ifp = fopen(ifname, "r")) == NULL){
		return;
	}
	if((ofp = fopen(ofname, "w")) == NULL){
		return;
	}
	fgets(buf, RECMAX, ifp);	// 会社名
	fprintf(ofp, "%s", buf);
	fgets(buf, RECMAX, ifp);	// 書体位置
	fprintf(ofp, "%s", buf);
	fgets(buf, RECMAX, ifp);	// グループ名
	wakuenvgetgrupname(grupname);
	fprintf(ofp, "\"%s\"\n", grupname);
	fgets(buf, RECMAX, ifp);	// グループ位置(枠位置)
	senvgetwakpath(wakupath);
	fprintf(ofp, "\"%s\"\n", wakupath);
	fgets(buf, RECMAX, ifp);	// 標準書体
	fprintf(ofp, "%s", buf);
	fgets(buf, RECMAX, ifp);	// 外字プログラム
	fprintf(ofp, "%s", buf);
	fgets(buf, RECMAX, ifp);	// 枠作成プログラム
	fprintf(ofp, "%s", buf);
	fgets(buf, RECMAX, ifp);	// 出力機器名
	fprintf(ofp, "%s", buf);
	fgets(buf, RECMAX, ifp);	// 用紙名
	wakuenvgetpapername(papername);
	fprintf(ofp, "\"%s\"\n", papername);
	fgets(buf, RECMAX, ifp);	// 縦書き回転角度
	fprintf(ofp, "%s", buf);
	fgets(buf, RECMAX, ifp);	// 中央寄せフラグ
	cntrmoveflag = scrngetcntrmoveflag();
	fprintf(ofp, "%d\n", cntrmoveflag);
	fgets(buf, RECMAX, ifp);	// 開始位置フラグ
	startmoveflag = scrngetstartmoveflag();
	fprintf(ofp, "%d\n", startmoveflag);
	scrngetstartmovemm(&startmovexmm, &startmoveymm);
	fgets(buf, RECMAX, ifp);	// 横方向移動mm
	fprintf(ofp, "%1.1lf\n", startmovexmm);
	fgets(buf, RECMAX, ifp);	// 縦方向移動mm
	fprintf(ofp, "%1.1lf\n", startmoveymm);
	fgets(buf, RECMAX, ifp);	// グリッドサイズ
	fprintf(ofp, "%s", buf);
	fgets(buf, RECMAX, ifp);	// ＧＤＩ変換倍率
	fprintf(ofp, "%s", buf);
	fclose(ofp);
	fclose(ifp);
}
void CMainFrame::MemoryEnvLoad()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];

	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	senvgetexepath(exepath);
	sprintf(pcApp->m_szBackImagePath, "%s\\backimage", exepath);
	sprintf(pcApp->m_szPhotoImagePath, "%s\\photoimage", exepath);
	sprintf(pcApp->m_szTempImagePath, "%s\\tempimage", exepath);

	sprintf(fname, "%s\\smm.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		CrDelFgets(buf, HRECMAX, fp); // コメントスキップ
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", pcApp->m_szBackImagePath);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", pcApp->m_szPhotoImagePath);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", pcApp->m_szTempImagePath);
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
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;

	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	senvgetexepath(exepath);
	sprintf(fname, "%s\\smm.env", exepath);
	if((fp = fopen(fname, "w")) != NULL){
		fprintf(fp, "//\n");
		fprintf(fp, "%s\n", pcApp->m_szBackImagePath);
		fprintf(fp, "%s\n", pcApp->m_szPhotoImagePath);
		fprintf(fp, "%s\n", pcApp->m_szTempImagePath);
		fclose(fp);
	}
}
void CMainFrame::DivDrawEnvLoad()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[HRECMAX];
	DBL		rag, mini, max;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\drawdiv.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf %lf %lf", &rag, &mini, &max);
		senvsetdrawdivbeze(rag, mini, max);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf %lf %lf", &rag, &mini, &max);
		senvsetdrawdivarc(rag, mini, max);
		fclose(fp);
	}
}

void CMainFrame::ScrnEnvSave()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	DBL		dclrlebel;
	DBL		dmmx, dmmy;
	int		ncntrkind;
	char	sportname[QRECMAX];
	char	splotername[QRECMAX];
	int		npcmdtype;
	DBL		dpxsize, dpysize;
	DBL		dpstep;
	DBL		dpsx, dpsy;
	int		nparcflag, npfillflag;
	int		nphomemode, nplinemode, npomitmode;
	char	datpath[FNAMEMAX];
	char	tmppath[FNAMEMAX];
	char	wakpath[FNAMEMAX];
	char	kmnpath[FNAMEMAX];
	char	outpath[FNAMEMAX];
	char	partpath[FNAMEMAX];
	char	btppath[FNAMEMAX];
	char	fntpath[FNAMEMAX];

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
	sprintf(fname, "%s\\ses_sys.env", exepath);
	if((fp = fopen(fname, "w")) != NULL){
		fprintf(fp, "%s\n", m_szKaisyaName);
		fprintf(fp, "%lf %lf\n", dmmx, dmmy);
		fprintf(fp, "%d\n", ncntrkind);
		fprintf(fp, "%s\n", sportname);
		fprintf(fp, "%s %d %lf %lf %lf %lf %lf\n",
				splotername, npcmdtype, 
				dpxsize, dpysize, dpstep, 
				dpsx, dpsy);
		fprintf(fp, "%d %d %d %d %d\n",
				nparcflag, npfillflag, 
				nphomemode, nplinemode, npomitmode);
		fprintf(fp, "%lf\n", dclrlebel);
		fprintf(fp, "%s\n", fntpath);
		fprintf(fp, "%s\n", datpath);
		fprintf(fp, "%s\n", outpath);
		fprintf(fp, "%s\n", kmnpath);
		fprintf(fp, "%s\n", wakpath);
		fprintf(fp, "%s\n", partpath);
		fprintf(fp, "%s\n", tmppath);
		fprintf(fp, "%s\n", btppath);
		fclose(fp);
	}
}

void CMainFrame::ScrnAddEnvSave()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	pltname[FNAMEMAX];
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
	sprintf(fname, "%s\\ses_sys2.env", exepath);
	if((fp = fopen(fname, "w")) != NULL){
		fprintf(fp, "%s\n", pltname);
		fprintf(fp, "%d\n", cntrmoveflag);
		fprintf(fp, "%d\n", startmoveflag);
		fprintf(fp, "%4.1lf %4.1lf\n", startmovexmm, startmoveymm);
		fprintf(fp, "%d\n", m_nTTEisuFlag);
		fprintf(fp, "%4.1lf %4.1lf %4.1lf %4.1lf\n", 
				plotlimitstep, plotlimitr, plotstartspace, plotendspace);
		fprintf(fp, "%d\n", nareadotsize);
		fprintf(fp, "%d\n", npointdotsize);
		fprintf(fp, "%d\n", m_nStatusBar);
		fclose(fp);
	}
}

void CMainFrame::MojiEnvSave()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	fontname[QRECMAX];
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
	sprintf(fname, "%s\\ses_strg.env", exepath);
	if((fp = fopen(fname, "w")) != NULL){
		fprintf(fp, "%s\n", fontname);
		fprintf(fp, "%d %d\n", esseisu, arceisu);
		fprintf(fp, "%d %d\n", ftategaki, fspacekind);
		fprintf(fp, "%lf %lf\n", dmmfwidth, dmmfhight);
		fprintf(fp, "%lf %lf\n", dmmmojispace, dmmstrgspace);
		fprintf(fp, "%lf\n", dmmstringlen);
		fprintf(fp, "%lf %lf\n", dmmstartx, dmmstarty);
		fprintf(fp, "%d %d\n", nstartkind, nconerkind);
		fclose(fp);
	}
}

void CMainFrame::FiguEnvSave()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	int		cntrkind, plgncount;

	figuenvgetcntrkind(&cntrkind);
	figuenvgetplgncount(&plgncount);

	senvgetexepath(exepath);
	sprintf(fname, "%s\\ses_figu.env", exepath);
	if((fp = fopen(fname, "w")) != NULL){
		fprintf(fp, "%d\n", cntrkind);
		fprintf(fp, "%d\n", plgncount);
		fclose(fp);
	}
}

char* CMainFrame::CrDelFgets(char *buf, int max, FILE *fp)
{
	char	*ret;
	int		len;

	ret = fgets(buf, max, fp);
	if(ret != NULL){
		len = strlen(buf);
		if(buf[len-1] == '\n'){
			buf[len-1] = '\0';
		}
	}
	return(ret);
}

void CMainFrame::FontEnvLoad()
{
	FILE	*fp;
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	int		len;
	int		fonttype;
	char	fontname[FNAMEMAX];
	char	accessname[FNAMEMAX];
	char	buf[HRECMAX];
	int		idx, eisuno;

	mojienvflistinit();
	mojienvflistopen("wb");
	idx = 0;
	senvgetexepath(exepath);
	sprintf(fname, "%s\\fontname.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		while(1){
			if(fgets(buf, HRECMAX, fp) == NULL){
				break;
			}
			len = strlen(buf);
			buf[len-1] = '\0';
			sscanf(buf, "%d %s", &fonttype, fontname);
			strcpy(accessname, &(buf[30]));
			mojienvsetflist(idx, fonttype, fontname, accessname);
			idx++;
		}
		fclose(fp);
	}
	mojienvgetesseisuno(&eisuno);
	if(eisuno != -1){
		fonttype = ES0FONT; 
		sprintf(fontname, "ＥＳ英数no%02d", eisuno);
		sprintf(accessname, "Ｅ英数%02d", eisuno);
		mojienvsetflist(idx, fonttype, fontname, accessname);
		idx++;
	}
	mojienvgetarceisuno(&eisuno);
	if(eisuno != -1){
		fonttype = ES0FONT; 
		sprintf(fontname, "ＡＲ英数%04d", eisuno);
		sprintf(accessname, "Ａ英数%04d", eisuno);
		mojienvsetflist(idx, fonttype, fontname, accessname);
		idx++;
	}
	TrueTypeFontListSet(idx);
	mojienvflistclose();
	mojienvflistopen("r+b");
}

void CMainFrame::TrueTypeFontListSet(int idx)
{
	CTonApp*	theApp;
	CString 	str;
	LOGFONT		lf;
	ENUMER		enumer;
	HDC			hDC;
	LPSTR		lpFontNT;
	int			i;
	int			fonttype;
	char		fontname[64];
	char		accessname[64];

	memset(&lf, 0, sizeof(LOGFONT));
	theApp = (CTonApp*)AfxGetApp();
	hDC = theApp->m_dcScreen.m_hDC;

	ASSERT(hDC != NULL);
	enumer.nCount = 0;
	enumer.hGmem = ::GlobalAlloc(GHND, 1L);
	lf.lfCharSet = SHIFTJIS_CHARSET;
	::EnumFontFamiliesEx(hDC, &lf,
		(FONTENUMPROC) EnumFamSiftjisCallBackEx, (LPARAM)&enumer, NULL);
	lpFontNT = (LPSTR)::GlobalLock(enumer.hGmem);
	for(i = 0 ;i < enumer.nCount ;i++){
		fonttype = TT2FONT;
		StrgSpaceDelCpy(fontname, lpFontNT);
		strcpy(accessname, lpFontNT);
		mojienvsetflist(idx, fonttype, fontname, accessname);
		idx++;
		lpFontNT += (LF_FACESIZE*2);
	}
	GlobalUnlock(enumer.hGmem);
	GlobalFree(enumer.hGmem);

	/* 英数書体の表示 */
	if(m_nTTEisuFlag == ON){
		enumer.nCount = 0;
		enumer.hGmem = ::GlobalAlloc(GHND, 1L);
		lf.lfCharSet = ANSI_CHARSET;
		::EnumFontFamiliesEx(hDC, &lf,
			(FONTENUMPROC) EnumFamAsciiCallBackEx, (LPARAM)&enumer, NULL);
		lpFontNT = (LPSTR)::GlobalLock(enumer.hGmem);
		for(i = 0 ;i < enumer.nCount ;i++){
			fonttype = TT3FONT;
			StrgSpaceDelCpy(fontname, lpFontNT);
			strcpy(accessname, lpFontNT);
			mojienvsetflist(idx, fonttype, fontname, accessname);
			idx++;
			lpFontNT += (LF_FACESIZE*2);
		}
		::GlobalUnlock(enumer.hGmem);
		::GlobalFree(enumer.hGmem);
	}
}

void CMainFrame::StrgSpaceDelCpy(LPSTR dststrg, LPSTR srcstrg)
{
	int		hc, lc;
	int		i, j;

	i = 0;
	j = 0;
	while(1){
		hc = srcstrg[i] & 0x00ff;
		if(hc == '\0'){
			dststrg[j] = '\0';
			break;
		}
		if(baseiskanji(hc) == ON){
			lc = srcstrg[i] & 0x00ff;
			if(hc == 0x0080 && lc == 0x0040){
				i += 2;
			}
			else{
				dststrg[j] = srcstrg[i];
				dststrg[j+1] = srcstrg[i+1];
				i += 2;
				j += 2;
			}
		}
		else{
			if(hc == ' '){
				i += 1;
			}
			else{
				dststrg[j] = srcstrg[i];
				i += 1;
				j += 1;
			}
		}
	}
}

BOOL CALLBACK AFX_EXPORT CMainFrame::EnumFamSiftjisCallBackEx(ENUMLOGFONTEX* lpnlf,
	 NEWTEXTMETRICEX* /*lpntm*/, int nFontType, LPVOID lpAdrs)
{
	ENUMER FAR *penumer;
	LPSTR       lpFontData, lpNextName;
	DWORD		dwSize;
	int			len;

	penumer = (ENUMER FAR *)lpAdrs;
	if(nFontType & TRUETYPE_FONTTYPE && lpnlf->elfFullName[0] != '@'){
		if(lpnlf->elfLogFont.lfCharSet == SHIFTJIS_CHARSET){
			dwSize = (DWORD)(LF_FACESIZE*2) * (penumer->nCount + 1);
			if(!GlobalReAlloc(penumer->hGmem, dwSize, GMEM_MOVEABLE)){
				return(FALSE);
			}
			lpFontData = (LPSTR)::GlobalLock(penumer->hGmem);
			lpNextName = lpFontData + ((penumer->nCount) * LF_FACESIZE*2);
			len = strlen((LPSTR)lpnlf->elfFullName);
			lstrcpy(lpNextName, (LPSTR)lpnlf->elfFullName);
			lpNextName[len] = '\0';
			lpNextName[len+1] = '\0';
			GlobalUnlock(penumer->hGmem);
			penumer->nCount++;
		}
	}
	return(TRUE);
}

BOOL CALLBACK AFX_EXPORT CMainFrame::EnumFamAsciiCallBackEx(ENUMLOGFONTEX* lpnlf,
	 NEWTEXTMETRICEX* /*lpntm*/, int nFontType, LPVOID lpAdrs)
{
	ENUMER FAR *penumer;
	LPSTR       lpFontData, lpNextName;
	DWORD		dwSize;
	int			len;

	penumer = (ENUMER FAR *)lpAdrs;
	if(nFontType & TRUETYPE_FONTTYPE && lpnlf->elfFullName[0] != '@'){
		if(lpnlf->elfLogFont.lfCharSet == ANSI_CHARSET){
			dwSize = (DWORD)(LF_FACESIZE*2) * (penumer->nCount + 1);
			if(!GlobalReAlloc(penumer->hGmem, dwSize, GMEM_MOVEABLE)){
				return(FALSE);
			}
			lpFontData = (LPSTR)::GlobalLock(penumer->hGmem);
			lpNextName = lpFontData + ((penumer->nCount) * LF_FACESIZE*2);
			len = strlen((LPSTR)lpnlf->elfFullName);
			lstrcpy(lpNextName, (LPSTR)lpnlf->elfFullName);
			lpNextName[len] = '\0';
			lpNextName[len+1] = '\0';
			GlobalUnlock(penumer->hGmem);
			penumer->nCount++;
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
	char	fname[FNAMEMAX];
	char	szWinDir[FNAMEMAX];
	FILE	*fp;

	::GetSystemDirectory(szWinDir, FNAMEMAX);
	sprintf(fname, "%s\\winsleep.sys", szWinDir);
	fp = fopen(fname, "r");
	if(fp == NULL){
		sprintf(fname, "%s\\winsleep.drv", szWinDir);
		fp = fopen(fname, "r");
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

	hModule = LoadLibrary("UsbPrt.dll");
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

