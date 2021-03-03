// sgs.cpp : アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"
#include "direct.h"
#include <winspool.h>

#include "MainFrm.h"
#include "ChildFrm.h"
#include "IpFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
	void senvgetdatpath(LPSTR path);
}

/////////////////////////////////////////////////////////////////////////////
// CTonApp

BEGIN_MESSAGE_MAP(CTonApp, CWinApp)
	//{{AFX_MSG_MAP(CTonApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
	//}}AFX_MSG_MAP
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 標準の印刷セットアップ コマンド
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTonApp クラスの構築

CTonApp::CTonApp()
{
	DWORD dwVersion = ::GetVersion();
	m_bWin4 = (BYTE)dwVersion >= 4;
#ifndef _UNICODE
	m_bWin31 = (dwVersion > 0x80000000 && !m_bWin4);
#endif
	m_nDefFont = (m_bWin4) ? DEFAULT_GUI_FONT : ANSI_VAR_FONT;
	m_dcScreen.Attach(::GetDC(NULL));
	m_bLargeIcons = m_dcScreen.GetDeviceCaps(LOGPIXELSX) >= 120;
	m_pcSysEnv = new CSysEnv();

	m_hTempStr = ::GlobalAlloc(GHND, sizeof(char) * TEMPBUFMAX2);
	m_lpTempStr = (char*)::GlobalLock(m_hTempStr); 
}

CTonApp::~CTonApp()
{
	::GlobalUnlock(m_hTempStr);
	::GlobalFree(m_hTempStr);
	delete(m_pcSysEnv);
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CTonApp オブジェクト

CTonApp theApp;

// この ID はこのアプリケーションが統計的にユニークになるように生成されました。
// もし、特別な ID を指定したいならば、変更してもかまいません。

// {BD0E5764-DB2D-11D1-A09D-444553540000}
static const CLSID clsid =
{ 0xbd0e5764, 0xdb2d, 0x11d1, { 0xa0, 0x9d, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

/////////////////////////////////////////////////////////////////////////////
// CTonApp クラスの初期化

BOOL CTonApp::InitInstance()
{
	OSVERSIONINFO	osvi;
	long		lRet;
	LPCTSTR		lpSubKey;
	HKEY		hkResult;
	BOOL		bIsWindowsNT;
	int			usermode;

	usermode = ON;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&osvi);
	lpSubKey ="dummy";
	bIsWindowsNT =	(osvi.dwPlatformId == VER_PLATFORM_WIN32_NT);
	if(bIsWindowsNT){
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hkResult, NULL);
		if(lRet != 0){
			usermode = OFF;
		}
		else{
			RegCloseKey(hkResult);
		}
	}
	// OLE ライブラリの初期化
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();


#ifdef _AFXDLL
	Enable3dControls();			// 共有 DLL の中で MFC を使用する場合にはここを呼び出してください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクしている場合にはここを呼び出してください。
#endif
	m_szArgv1[0] = '\0';
	m_szSavePath[0] = '\0';
	m_szSaveName[0] = '\0';
	m_clrBack = -1;

	SetRegistryKey(_T("筆耕システム　外字作成部"));

	LoadStdProfileSettings();  // 標準の INI ファイルのオプションをローﾄﾞします (MRU を含む)

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_SGSTYPE,
		RUNTIME_CLASS(CTonDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CTonView));
	pDocTemplate->SetContainerInfo(IDR_SGSTYPE_CNTR_IP);
	pDocTemplate->SetServerInfo(
		IDR_SGSTYPE_SRVR_EMB, IDR_SGSTYPE_SRVR_IP,
		RUNTIME_CLASS(CInPlaceFrame));
	AddDocTemplate(pDocTemplate);

	m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);

	COleTemplateServer::RegisterAll();

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	m_pMainWnd->DragAcceptFiles();

	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		return TRUE;
	}

	if(usermode == ON){
		m_server.UpdateRegistry(OAT_INPLACE_SERVER);
	}
	COleObjectFactory::UpdateRegistryAll();

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard 仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
		// メッセージ ハンドラはありません。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// メッセージ ハンドラはありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CTonApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTonApp コマンド

BOOL CTonApp::ProcessMessageFilter(int code, LPMSG lpMsg) 
{
	return CWinApp::ProcessMessageFilter(code, lpMsg);
}


BOOL CTonApp::PreTranslateMessage(MSG* pMsg) 
{
	return CWinApp::PreTranslateMessage(pMsg);
}

void CTonApp::OnFilePrintSetup() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CPrintDialog pd(TRUE);
	DoPrintDialog(&pd);
	m_hDevMode = pd.m_pd.hDevMode;
	m_hDevNames = pd.m_pd.hDevNames;
}

void CTonApp::OnFileOpen() 
{
	char	datpath[FNAMEMAX];

	senvgetdatpath(datpath);
	SetCurrentDirectory(datpath);
	CWinApp::OnFileOpen();
}

HANDLE CTonApp::GetDevMode()
{
	return(m_hDevMode);
}

HANDLE CTonApp::GetDevNames()
{
	return(m_hDevNames);
}

int CTonApp::CheckEnumPrinter(HANDLE hDevNames)
{
	char	szDevice[QRECMAX];
	char	szDriver[QRECMAX];
	char	szOutput[QRECMAX];
	LPPRINTER_INFO_2	lpINFO2 = NULL;		//PRINT_INFO_2構造体
	DWORD				dwNeeded;			//PRINT_INFO_X構造体に必要なｻｲｽﾞ
	DWORD				dwReturned;			//PRINT_INFO_X構造体の個数
	int					i, ret, len;
	CString				csStr;
	HANDLE				hPrinter = NULL;
	DWORD				nSize;
	LPDEVMODE			lpDevMode;

	CMainFrame *pcWnd = (CMainFrame*)GetMainWnd();

	if(hDevNames == 0){
		return(FALSE);
	}
	len = ::GlobalSize(hDevNames);
	LPDEVNAMES lpDevNames = (LPDEVNAMES)::GlobalLock(hDevNames);
	if(lpDevNames == NULL){
		return(FALSE);
	}
	if(lpDevNames->wDeviceOffset >= len
	|| lpDevNames->wDriverOffset >= len
	|| lpDevNames->wOutputOffset >= len){
		::GlobalUnlock(hDevNames);
		return(FALSE);
	}
	strncpy(szDevice, (LPSTR)(lpDevNames) + lpDevNames->wDeviceOffset, QRECMAX);
	szDevice[QRECMAX-1] = '\0';
	strncpy(szDriver, (LPSTR)(lpDevNames) + lpDevNames->wDriverOffset, QRECMAX);
	szDriver[QRECMAX-1] = '\0';
	strncpy(szOutput, (LPSTR)(lpDevNames) + lpDevNames->wOutputOffset, QRECMAX);
	szOutput[QRECMAX-1] = '\0';
	::GlobalUnlock(hDevNames);


	::EnumPrinters(PRINTER_ENUM_LOCAL,NULL,2,NULL,0,&dwNeeded,&dwReturned);
	lpINFO2 = (LPPRINTER_INFO_2)new BYTE[dwNeeded];
	::EnumPrinters(PRINTER_ENUM_LOCAL,NULL,2,(LPBYTE)lpINFO2,dwNeeded,&dwNeeded,&dwReturned);
	ret = FALSE;
	for(i = 0; i < (int)dwReturned; i++){
		if(strcmp(lpINFO2[i].pPrinterName, szDevice) == 0){
			ret = TRUE;
			break;
		}
	}
	if(lpINFO2 != NULL){
		delete []lpINFO2;
	}

	if(ret == TRUE){
		if(::OpenPrinter(szDevice, &hPrinter, NULL)){									::GetPrinter(hPrinter, 2, NULL, NULL, &nSize);
			::GetPrinter(hPrinter, 2, NULL, 0, &nSize);
			lpINFO2 = (LPPRINTER_INFO_2)new BYTE[nSize];;
			::GetPrinter(hPrinter, 2, (LPBYTE)lpINFO2, nSize, &nSize);
			strcpy(szOutput, lpINFO2->pPortName);
			strcpy(szDriver, lpINFO2->pDriverName);
			delete []lpINFO2;
			SetDevNamesElement(szDevice, szDriver, szOutput);

			nSize = ::DocumentProperties(NULL, hPrinter, szDevice, NULL, NULL, 0);
			if(m_hDevMode == 0){
				m_hDevMode = ::GlobalAlloc(GHND, nSize);
			}
			else{
				m_hDevMode = ::GlobalReAlloc(m_hDevMode, nSize, GHND);
			}
			lpDevMode = (LPDEVMODE)::GlobalLock(m_hDevMode);
			::DocumentProperties(NULL, hPrinter, szDevice, lpDevMode, NULL, DM_OUT_BUFFER);
			::GlobalUnlock(m_hDevMode);
		}
		else{
			return(FALSE);
		}
	}
	return(ret);
}

void CTonApp::SetDevNamesElement(LPSTR lpDevice, LPSTR lpDriver, LPSTR lpOutput)
{
	LPDEVNAMES	lpDevNames;
	long		lSize;

	int		nDrvLen = strlen(lpDriver) + 1;
	int		nDevLen = strlen(lpDevice) + 1;
	int		nPortLen = strlen(lpOutput) + 1;
	lSize = 8 + nDrvLen + nDevLen + nPortLen;
	if(m_hDevNames == 0){
		m_hDevNames = ::GlobalAlloc(GHND, lSize);
	}
	else{
		m_hDevNames = ::GlobalReAlloc(m_hDevNames, lSize, GHND);
	}
	lpDevNames = (LPDEVNAMES)::GlobalLock(m_hDevNames);
	lpDevNames->wDriverOffset = 8;
	lpDevNames->wDeviceOffset = lpDevNames->wDriverOffset + nDrvLen;
	lpDevNames->wOutputOffset = lpDevNames->wDeviceOffset + nDevLen;
	lpDevNames->wDefault = 0;
	LPSTR lpPos = (LPSTR)lpDevNames;
	strcpy(&(lpPos[lpDevNames->wDriverOffset]), lpDriver);
	strcpy(&(lpPos[lpDevNames->wDeviceOffset]), lpDevice);
	strcpy(&(lpPos[lpDevNames->wOutputOffset]), lpOutput);
	::GlobalUnlock(m_hDevNames);
}

