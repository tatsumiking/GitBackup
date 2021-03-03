// MainFrm.cpp : CMainFrame クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "sms.h"
#include <direct.h>
#include <io.h>

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
	void senvsetoptslct(int mode);
	void senvseteditconerkind(int nkind);
	void senvgeteditconerkind(LPINT nkind);
	void senvsetwakuconerkind(int nkind);
	void senvgetwakuconerkind(LPINT nkind);
	void senvsetmojiconerkind(int nkind);
	void senvgetmojiconerkind(LPINT nkind);
	int senvchckflag(int flag);
	void senvonflag(int flag);
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
	void senvsetconerkind(int nkind);
	void senvgetconerkind(LPINT nkind);
	void poutgetparaminit(LPDBL lmtstep, LPDBL limitr, LPDBL smm, LPDBL emm);
	void poutsetparaminit(DBL lmtstep, DBL limitr, DBL smm, DBL emm);
	void poutsetdeepcut(DBL mm);
	void poutgetdeepcut(LPDBL mm);
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
	int	poutcheckdeepcutflag();
	void poutsetdeepcutflag(int flag);
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
	void mojienvgetconerkind(LPINT nconerkd);
	void mojienvsetconerkind(int nconerkd);
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
	void mojienvgettrnslimit(LPDBL dlimit);
	void mojienvsettrnslimit(DBL dlimit);
	void figuenvgetcntrkind(LPINT cntrkind);
	void figuenvsetcntrkind(int cntrkind);
	void figuenvgetplgncount(LPINT plgncount);
	void figuenvsetplgncount(int plgncount);
	void wakuenvsetkaisyaname(LPSTR name);
	void wakuenvsetgrupname(LPSTR name);
	void wakuenvsetpapername(LPSTR name);
	void wakuenvgetpapername(LPSTR name);
	void wakuenvgetpaperelement();
	void wakuenvgetpapersize(DBL *xmmsize, DBL *ymmsize);
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの構築/消滅

#define	PROTECT	1
//#define	NONPROTECT	1

CMainFrame::CMainFrame()
{
	char	szExePath[FNAMEMAX];
	char	fname[FNAMEMAX];

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

	m_nFirst = TRUE;

	PRINTDLG	sPrintDlg;
	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	pcApp->GetPrinterDeviceDefaults(&sPrintDlg);
}

CMainFrame::~CMainFrame()
{
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

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
#ifdef PROTECT 
	int		ret;
	char	msgbuf[RECMAX];
	int		id, nDay, nStat;

	ret = CheckProtectFile();
	if(ret == -1){
		ret = CheckProtectUsb();
		if(ret == -1){
			sprintf(msgbuf, "プロテクトエラーです\n販売店にご連絡ください");
			MessageBox(msgbuf, "筆耕システム 枠作成システム", MB_OK);
		}
	}

	if(ret == -1){
		return -1;
	}
#endif
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: この位置で CREATESTRUCT cs を修正して、Window クラスやスタイルを
	//       修正してください。

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		;
	cs.x = 100;
	cs.y = 100;
	cs.cx = 100;
	cs.cy = 100;
	cs.dwExStyle |= WS_EX_TOPMOST;
	return TRUE;
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
	senvgetexepath(exepath);
	sprintf(crctfname, "%s\\outcrect.dat", exepath);
	if((crctfp = fopen(crctfname, "w+")) != NULL){
		fclose(crctfp);
	}
}

void CMainFrame::EnvSave()
{
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
	sprintf(partpath, "%s\\part", exepath);
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

void CMainFrame::WakuEnvLoad()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	*sp;
	char	papername[HRECMAX];
	char	grupname[QRECMAX];
	char	gruppath[QRECMAX];
	int		cntrmoveflag;
	int		startmoveflag;
	DBL		startmovexmm, startmoveymm;
	DBL		xmmsize, ymmsize;

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
	sp = basecsvstrdivcpy(sp, gruppath);
	senvsetwakpath(gruppath);
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

	wakuenvgetpapersize(&xmmsize, &ymmsize);
	scrnsetmmsize(xmmsize, ymmsize);
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

void CMainFrame::HelpTextOut(int idx, LPSTR msg)
{

}

void CMainFrame::InitItemElement()
{

}

void CMainFrame::ShowItemElement()
{

}

void CMainFrame::HideItemElement()
{

}

void CMainFrame::SetItemElement()
{

}

void CMainFrame::GetItemElement()
{

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
void CMainFrame::OnPaint() 
{
	CPaintDC dc(this);
	if(m_nFirst == TRUE){
		m_nFirst = FALSE;
		//::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		//::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		//::SetWindowPos(this->m_hWnd, HWND_TOP, -100, -100, 0, 0, SWP_NOSIZE);
		::SetForegroundWindow(this->m_hWnd);
	}
}
