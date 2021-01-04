
// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"
#include "Util.h"

#include "MainFrm.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Mojienv.h"
#include "../../ObjLib/PlgnFill.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケーター
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame() noexcept
{
	TCHAR	szExePath[FNAMEMAX];

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

	InitExePath(szExePath);
	senvsetexepath(szExePath);
	EnvLoad();
	FontEnvLoad();

	scrninit();
	mojienvinit();
	plgnfillinit();

}

CMainFrame::~CMainFrame()
{
	EnvSave();
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

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("ツール バーの作成に失敗しました。\n");
		return -1;      // 作成できない場合
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("ステータス バーの作成に失敗しました。\n");
		return -1;      // 作成できない場合
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: ツール バーをドッキング可能にしない場合は、これらの 3 行を削除してください。
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return TRUE;
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

void CMainFrame::InitExePath(LPTSTR szExePath)
{
	int		len;

	len = ::GetModuleFileName(AfxGetInstanceHandle(), szExePath, FNAMEMAX);
	while (1) {
		if (len <= 0) {
			szExePath[0] = _T('\0');
			break;
		}
		if (szExePath[len] == _T(':') || szExePath[len] == _T('\\')) {
			szExePath[len] = _T('\0');
			break;
		}
		len--;
	}
}
void CMainFrame::EnvLoad()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	filename[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	fontpath[FNAMEMAX];


	senvgetexepath(exepath);
	_stprintf_s(fontpath, _T("%s\\moji"), exepath);
	_stprintf_s(filename, _T("%s\\smsutil.env"), exepath);
	_tfopen_s(&fp, filename, _T("r"));
	if (fp != NULL) {
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), fontpath, FNAMEMAX);
		fclose(fp);
	}
	mojienvsetfontpath(fontpath);
}
void CMainFrame::EnvSave()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	filename[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	fontpath[FNAMEMAX];


	senvgetexepath(exepath);
	mojienvgetfontpath(fontpath);

	_stprintf_s(filename, _T("%s\\smsutil.env"), exepath);
	_tfopen_s(&fp, filename, _T("w"));
	if (fp != NULL) {
		_stprintf_s(buf, _T("//\n"));
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%s\n"), fontpath);
		baselibFputs(buf, fp);
		fclose(fp);
	}
}
void CMainFrame::FontEnvLoad()
{
	FILE	*fp;
	TCHAR	exepath[FNAMEMAX];
	TCHAR	filename[FNAMEMAX];
	char	buf[RECMAX];
	int		len;
	int		fonttype;
	char	sSFName[HRECMAX];
	char	sFFName[HRECMAX];
	TCHAR	tSFName[HRECMAX];
	TCHAR	tFFName[HRECMAX];

	mojienvflistinit();
	mojienvflistopen(_T("wb"));
	mojienvflistclose();

	senvgetexepath(exepath);
	_stprintf_s(filename, _T("%s\\fontname.env"), exepath);
	_tfopen_s(&fp, filename, _T("r"));
	if (fp != NULL) {
		while (1) {
			if (fgets(buf, RECMAX, fp) == NULL) {
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
		}
		fclose(fp);
	}
}