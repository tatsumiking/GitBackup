// MainFrm.cpp : CMainFrame クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "smsout.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct{
    GLOBALHANDLE	hGmem;
	int				nCount;
}ENUMER;

//extern "C" {
	#include "../../ObjLib/Baselib.h"
	#include "../../ObjLib/Curvelib.h"
	#include "../../ObjLib/Diblib.h"
	#include "../../ObjLib/Figuenv.h"
	#include "../../ObjLib/Mojienv.h"
	#include "../../ObjLib/Plgnfill.h"
	#include "../../ObjLib/Pout.h"
	#include "../../ObjLib/Scrnlib.h"
	#include "../../ObjLib/Senvlib.h"
	#include "../../ObjLib/Wakuenv.h"
//}

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
static int m_nCount = 0;

CMainFrame::CMainFrame()
{
	TCHAR	szExePath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];

	// 初期化の位置を移動
	GdiplusStartup(&m_gdiToken, &m_gdiSI, NULL);

	// 初期化の位置を移動
	InitExePath(szExePath);
	senvsetexepath(szExePath);

	curveinit();
	senvinit();
	scrninit();
	wsprintf(fname, _T("%s\\defenv\\clrtrns.env"), szExePath);
	mojienvinit();
	diblibinit();

	EnvLoad();

	m_pcFSF1Font = new CFSF1Font(2);
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
	delete(m_pcFBU0Font);
	delete(m_pcESF1Font);
	delete(m_pcESF0Font);
	delete(m_pcWNTTFont);
	delete(m_pcFSF1Font);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
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
	cs.cx = 160;
	cs.cy = 160;
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
}

void CMainFrame::EnvSave()
{
}

void CMainFrame::ScrnEnvLoad()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	exedrive[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[FNAMEMAX];
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
	DBL		dpxtime, dpytime;
	TCHAR	datpath[FNAMEMAX];
	TCHAR	tmppath[FNAMEMAX];
	TCHAR	wakpath[FNAMEMAX];
	TCHAR	kmnpath[FNAMEMAX];
	TCHAR	outpath[FNAMEMAX];
	TCHAR	partpath[FNAMEMAX];
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
	_stprintf_s(partpath,_T("%s\\part"), exepath);
	_stprintf_s(tmppath, _T("%s\\temp"), exepath);
	_stprintf_s(btppath, _T("%s\\tbmp"), exepath);

	_stprintf_s(fname, _T("%s\\ses_sys.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if (fp != NULL) {
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%s"), m_szKaisyaName, HRECMAX);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%lf %lf"), &dmmx, &dmmy);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%d"), &ncntrkind);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%s"), sportname, QRECMAX);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%s %d %lf %lf %lf %lf %lf"),
			splotername, QRECMAX, &npcmdtype, 
			&dpxsize, &dpysize, &dpstep, 
			&dpsx, &dpsy);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%d %d %d %d %d"),
			&nparcflag, &npfillflag, 
			&nphomemode, &nplinemode, &npomitmode);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%lf"), &dclrlebel);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%s"), fntpath, FNAMEMAX);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%s"), datpath, FNAMEMAX);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%s"), outpath, FNAMEMAX);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%s"), kmnpath, FNAMEMAX);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%s"), wakpath, FNAMEMAX);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%s"), partpath, FNAMEMAX);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%s"), tmppath, FNAMEMAX);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
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
	senvsetpartpath(outpath);
	senvsetbtppath(btppath);
	mojienvsetfontpath(fntpath);
	senvsettmppath(tmppath);
}

void CMainFrame::ScrnAddEnvLoad()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[FNAMEMAX];
	TCHAR	pltname[FNAMEMAX];
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
	_stprintf_s(fname, _T("%s\\ses_sys2.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if (fp != NULL) {
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%s"), pltname, FNAMEMAX);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%d"), &cntrmoveflag);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%d"), &startmoveflag);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%lf %lf"), &startmovexmm, &startmoveymm);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%d"), &m_nTTEisuFlag);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%lf %lf %lf %lf"),
				&plotlimitstep, &plotlimitr,
				&plotstartspace, &plotendspace);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%d"), &nareadotsize);
		baselibCrDelFgets(buf, FNAMEMAX, fp);
		_stscanf_s(buf, _T("%d"), &npointdotsize);
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
	TCHAR	gruppath[QRECMAX];
	int		cntrmoveflag;
	int		startmoveflag;
	DBL		startmovexmm, startmoveymm;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\soainfo.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if (fp == NULL) {
		return;
	}

	cntrmoveflag = scrngetcntrmoveflag();
	startmoveflag = scrngetstartmoveflag();
	scrngetstartmovemm(&startmovexmm, &startmoveymm);

	if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;} // 会社名}
	if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;} // 書体位置}
	if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// グループ名}
	sp = buf;
	sp = basecsvstrdivcpy(sp, grupname);
	wakuenvsetgrupname(grupname);
	if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// グループ位置}
	sp = buf;
	sp = basecsvstrdivcpy(sp, gruppath);
	senvsetwakpath(gruppath);
	if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 標準書体}
	if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 外字プログラム}
	if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 枠作成プログラム}
	if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 出力機器名}
	if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 用紙名}
	sp = buf;
	sp = basecsvstrdivcpy(sp, papername);
	wakuenvsetpapername(papername);
	wakuenvgetpaperelement();
	if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 縦書き回転角度}
	if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 中央寄せフラグ}
	_stscanf_s(buf, _T("%d"), &cntrmoveflag);
	if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 開始位置フラグ}
	_stscanf_s(buf, _T("%d"), &startmoveflag);
	if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 横方向移動ＭＭ}
	_stscanf_s(buf, _T("%lf"), &startmovexmm);
	if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// 縦方向移動ＭＭ}
	_stscanf_s(buf, _T("%lf"), &startmoveymm);
	if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// グリッドサイズ
	if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){goto EndLabel;}	// ＧＤＩ変換倍率
	scrnsetcntrmoveflag(cntrmoveflag);
	scrnsetstartmoveflag(startmoveflag);
	scrnsetstartmovemm(startmovexmm, startmoveymm);
EndLabel:
	fclose(fp);

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
void CMainFrame::FontEnvLoad()
{
	FILE	*fp;
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	char	buf[RECMAX];
	int		len;
	int		fonttype;
	char	sSFName[QRECMAX];
	char	sFFName[QRECMAX];
	TCHAR	tSFName[QRECMAX];
	TCHAR	tFFName[QRECMAX];
	int		idx, eisuno;

	mojienvflistinit();
	mojienvflistopen(_T("wb"));
	mojienvflistclose();
	m_nCount = 0;
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\fontname.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if (fp != NULL) {
		while (1) {
			if (fgets(buf, RECMAX, fp) == NULL) {
				break;
			}
			len = strlen(buf);
			buf[len - 1] = '\0';
			sscanf_s(buf, "%d %s", &fonttype, sSFName, QRECMAX);
			strcpy_s(sFFName, QRECMAX, &(buf[30]));
			baselibCharStrToTCHARStr(sSFName, tSFName, QRECMAX);
			baselibCharStrToTCHARStr(sFFName, tFFName, QRECMAX);
			mojienvflistopen(_T("r+b"));
			mojienvsetflist(fonttype, tSFName, tFFName);
			mojienvflistclose();
			m_nCount++;
		}
		fclose(fp);
	}
	mojienvgetesseisuno(&eisuno);
	if (eisuno != -1) {
		fonttype = ES0FONT;
		sprintf_s(sSFName, QRECMAX, "ＥＳ英数no%02d", eisuno);
		sprintf_s(sFFName, QRECMAX, "Ｅ英数%02d", eisuno);
		baselibCharStrToTCHARStr(sSFName, tSFName, QRECMAX);
		baselibCharStrToTCHARStr(sFFName, tFFName, QRECMAX);
		mojienvflistopen(_T("r+b"));
		mojienvsetflist(fonttype, tSFName, tFFName);
		mojienvflistclose();
		m_nCount++;
	}
	mojienvgetarceisuno(&eisuno);
	if (eisuno != -1) {
		fonttype = ES0FONT;
		sprintf_s(sSFName, QRECMAX, "ＡＲ英数%04d", eisuno);
		sprintf_s(sFFName, QRECMAX, "Ａ英数%04d", eisuno);
		baselibCharStrToTCHARStr(sSFName, tSFName, QRECMAX);
		baselibCharStrToTCHARStr(sFFName, tFFName, QRECMAX);
		mojienvflistopen(_T("r+b"));
		mojienvsetflist(fonttype, tSFName, tFFName);
		mojienvflistclose();
		m_nCount++;
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
		fonttype = TT1FONT;
		lpTFName = (LPTSTR)(&(lpnlf->elfFullName[1]));
		len = _tcslen(lpTFName) + 1;
		_tcscpy_s(tSFName, len, lpTFName);
		baselibSpaceDelete(tSFName);
		mojienvflistopen(_T("r+b"));
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
		fonttype = TT2FONT;
		lpTFName = (LPTSTR)(lpnlf->elfFullName);
		len = _tcslen(lpTFName) + 1;
		_tcscpy_s(tSFName, len, lpTFName);
		baselibSpaceDelete(tSFName);
		mojienvflistopen(_T("r+b"));
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

	if (RASTER_FONTTYPE < nFontType && lpnlf->elfFullName[0] != '@') {
		if (lpnlf->elfLogFont.lfCharSet == ANSI_CHARSET) {
			fonttype = TT3FONT;
			lpTFName = (LPTSTR)(lpnlf->elfFullName);
			len = _tcslen(lpTFName) + 1;
			_tcscpy_s(tSFName, len, lpTFName);
			baselibSpaceDelete(tSFName);
			mojienvflistopen(_T("r+b"));
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

void CMainFrame::HelpTextOut(int idx, LPTSTR msg)
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
