// sms.cpp : アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"
#include <winspool.h>
#include "sms.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
}
/////////////////////////////////////////////////////////////////////////////
// CTonApp

BEGIN_MESSAGE_MAP(CTonApp, CWinApp)
	//{{AFX_MSG_MAP(CTonApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	//}}AFX_MSG_MAP
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTonApp クラスの構築

CTonApp::CTonApp()
{
	m_dcScreen.Attach(::GetDC(NULL));
	m_pcSysEnv = new CSysEnv();
	m_clrBack = 0xffffffff;
}

CTonApp::~CTonApp()
{
	delete(m_pcSysEnv);
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CTonApp オブジェクト

CTonApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTonApp クラスの初期化

BOOL CTonApp::InitInstance()
{
	OSVERSIONINFO	osvi;

	AfxGetModuleState()->m_dwVersion = 0x0601;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&osvi);
	m_nMajor = osvi.dwMajorVersion;
	m_nMinor = osvi.dwMinorVersion;

	AfxEnableControlContainer();
#ifdef _AFXDLL
	Enable3dControls();		// 共有 DLL の中で MFC を使用する場合にはここを呼び出してください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクしている場合にはここを呼び出してください。
#endif

	// 設定が保存される下のレジストリ キーを変更します。
	// TODO: この文字列を、会社名または所属など適切なものに
	// 変更してください。
	SetRegistryKey(_T("筆耕システム Ver1.00"));

	LoadStdProfileSettings(0);  // 標準の INI ファイルのオプションをローﾄﾞします (MRU を含む)

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_SMSTYPE,
		RUNTIME_CLASS(CTonDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CTonView));
	AddDocTemplate(pDocTemplate);

	// メイン MDI フレーム ウィンドウを作成
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// コマンドラインでディスパッチ コマンドを指定します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
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
// CTonApp メッセージ ハンドラ

void CTonApp::AppExit()
{
	OnAppExit();
}

void CTonApp::OnAppExit() 
{
	CWinApp::OnAppExit();
}

HANDLE CTonApp::GetDevMode()
{
	return(m_hDevMode);
}

HANDLE CTonApp::GetDevNames()
{
	return(m_hDevNames);
}

void CTonApp::SetupPrinter()
{
	CWinApp::OnFilePrintSetup();
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

