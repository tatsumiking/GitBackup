// MainFrm.cpp : CMainFrame クラスの動作の定義を行います。
//

#include "stdafx.h"

#include <stdio.h>
#include <conio.h>
#include <direct.h>
#include <stdlib.h>

#include "MainFrm.h"
#include "..\UsbPrt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct{
    GLOBALHANDLE	hGmem;
	int				nCount;
}ENUMER;

extern "C" {
	void curveinit();
	int  baseiskanji(int far hc);
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
	void mojienvflistopen(LPSTR lpMode);
	void mojienvflistclose();
	void mojienvflistinit();
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

//#define	PROTECT	1
#define	NONPROTECT	1

CMainFrame::CMainFrame()
{
	char	szExePath[FNAMEMAX];
	char	fname[FNAMEMAX];

	// TODO: この位置にメンバの初期化処理コードを追加してください。
	m_nFirstFlag = TRUE;
	m_dwClr = 0;
	m_nOptSlctObj = 0;
	m_nOptChangeCurser = 0;
	m_nEpsClrTrns = OFF;


	// 初期化の位置を移動
	InitExePath(szExePath);
	senvsetexepath(szExePath);

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
	m_pcARCFont = new CARCFont(0);
	m_pcWNTTFont->FSF1ClassSet(m_pcFSF1Font);
	m_pcESF0Font->FSF1ClassSet(m_pcFSF1Font);
	m_pcESF1Font->FSF1ClassSet(m_pcFSF1Font);
	m_pcARCFont->FSF1ClassSet(m_pcFSF1Font);
	m_pcWNTTFont->ParentSet(this);
	plgnfillinit();
	plgnfillareaset(-INTMAX, -INTMAX, INTMAX, INTMAX);
	senvinitscrnclr();
	DivDrawEnvLoad();
}

CMainFrame::~CMainFrame()
{
	senvendscrnclr();
	senvend();
	scrnend();
	plgnfillend();
	diblibend();
	//EnvSave();
	mojienvflistclose();
	delete(m_pcARCFont);
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
	lpCreateStruct->lpszClass = "筆耕システム　外字作成部";
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

	if(!CreateWakuBarG1())
		return -1;

	if(!CreateWakuBarG2())
		return -1;

	if (!m_wndLeftSideBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_LEFT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndLeftSideBar.LoadToolBar(IDR_LEFTSIDE))
	{
		TRACE0("Failed to create cdnttoolbar\n");
		return -1;      // 作成に失敗
	}

	m_wndWakuBarG1.EnableDocking(CBRS_ALIGN_TOP);
	m_wndWakuBarG2.EnableDocking(CBRS_ALIGN_BOTTOM);
	m_wndLeftSideBar.EnableDocking(CBRS_ALIGN_LEFT);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndWakuBarG1, AFX_IDW_DOCKBAR_TOP);
	DockControlBar(&m_wndWakuBarG2, AFX_IDW_DOCKBAR_BOTTOM);
	
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

BOOL CMainFrame::CreateWakuBarG1()
{
	if (!m_wndWakuBarG1.Create(this, IDD_WAKUBARG1,
		CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
		ID_VIEW_WAKUBAR3))
	{
		TRACE0("Failed to create dialog bar m_wndWakuBarG1\n");
		return FALSE;		// 作成に失敗
	}
	return TRUE;
}

BOOL CMainFrame::CreateWakuBarG2()
{
	if (!m_wndWakuBarG2.Create(this, IDD_WAKUBARG2,
		CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
		ID_VIEW_WAKUBAR3))
	{
		TRACE0("Failed to create dialog bar m_wndWakuBarG2\n");
		return FALSE;		// 作成に失敗
	}
	return TRUE;
}

void CMainFrame::InitItemElement()
{
}

void CMainFrame::SetItemElement()
{
	m_wndWakuBarG1.SetItemElement();
}

void CMainFrame::GetItemElement()
{
}

void CMainFrame::ShowItemElement()
{
}

void CMainFrame::HideItemElement()
{
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
}

void CMainFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
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
		//::SetForegroundWindow(this->m_hWnd);
	}
}

void CMainFrame::ThisViewMax(CTonView *pcView)
{
	MDIMaximize(pcView->GetParentFrame()); 
}

void CMainFrame::SetCrtView(CTonView * pcView)
{
	m_pcCrtView = pcView;
	SetItemElement();
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
	senvgetexepath(exepath);
	sprintf(crctfname, "%s\\outcrect.dat", exepath);
	if((crctfp = fopen(crctfname, "w+")) == NULL){
		fclose(crctfp);
	}
}

void CMainFrame::EnvSave()
{
	ScrnEnvSave();
	ScrnAddEnvSave();
	MojiEnvSave();
	FiguEnvSave();
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
	char	outpath[FNAMEMAX];
	char	partpath[FNAMEMAX];
	char	btppath[FNAMEMAX];
	char	fntpath[FNAMEMAX];

	senvgetexepath(exepath);
	strcpy(exedrive, exepath);
	exedrive[2] = '\0';
	strcpy(m_szKaisyaName, "未設定");
	dmmx = 300; dmmy = 300;
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
	//strcat(exedrive, "\\es98use");
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
		sscanf(buf, "%lf %lf", &m_dBackMMX, &m_dBackMMY);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &ncntrkind);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", sportname);
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
	CreateDirectory(fntpath, NULL);
	CreateDirectory(datpath, NULL);
	CreateDirectory(outpath, NULL);
	CreateDirectory(kmnpath, NULL);
	CreateDirectory(wakpath, NULL);
	CreateDirectory(partpath, NULL);
	CreateDirectory(tmppath, NULL);
	CreateDirectory(btppath, NULL);
	dmmx = 300; dmmy = 300;
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
	char	sportname[RECMAX];
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
		fprintf(fp, "%lf %lf\n", m_dBackMMX, m_dBackMMY);
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
}
void CMainFrame::SetSWakLocate()
{

}
