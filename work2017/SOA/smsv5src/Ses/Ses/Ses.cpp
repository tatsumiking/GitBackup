
// Ses.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"

#include <winspool.h>

#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "../../ObjLib/Baselib.h"

// CTonApp

BEGIN_MESSAGE_MAP(CTonApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CTonApp::OnAppAbout)
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 標準の印刷セットアップ コマンド
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CTonApp の構築

CTonApp::CTonApp() noexcept
{
	// 再起動マネージャーをサポートします
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// アプリケーションが共通言語ランタイム サポート (/clr) を使用して作成されている場合:
	//     1) この追加設定は、再起動マネージャー サポートが正常に機能するために必要です。
	//     2) 作成するには、プロジェクトに System.Windows.Forms への参照を追加する必要があります。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 下のアプリケーション ID 文字列を一意の ID 文字列で置換します。推奨される
	// 文字列の形式は CompanyName.ProductName.SubProduct.VersionInformation です
	SetAppID(_T("Ses.AppID.NoVersion"));

	m_nDefFont = (m_bWin4) ? DEFAULT_GUI_FONT : ANSI_VAR_FONT;
	m_dcScreen.Attach(::GetDC(NULL));
	m_bLargeIcons = m_dcScreen.GetDeviceCaps(LOGPIXELSX) >= 120;
	m_pcSysEnv = new CSysEnv();
	m_pcCharLib = new CCharLib();

	m_hTempStr = ::GlobalAlloc(GHND, sizeof(TCHAR) * TEMPBUFMAX2);
	m_lpTempStr = (TCHAR*)::GlobalLock(m_hTempStr); 

	m_hTempDevNames = 0;
	m_hTempDevMode = 0;

	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}
CTonApp::~CTonApp()
{
	::GlobalUnlock(m_hTempStr);
	::GlobalFree(m_hTempStr);
	delete(m_pcSysEnv);
	delete(m_pcCharLib);
}

// 唯一の CTonApp オブジェクト

CTonApp theApp;
// この ID はこのアプリケーションが統計的に一意なものとなるように生成されました。
// 変更して特定な ID を指定することもできます。

// {d3d8f7de-cbe0-4f55-9436-692cc1b04051}
static const CLSID clsid =
{0xd3d8f7de,0xcbe0,0x4f55,{0x94,0x36,0x69,0x2c,0xc1,0xb0,0x40,0x51}};


// CTonApp の初期化

BOOL CTonApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// OLE ライブラリを初期化します。
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit コントロールを使用するには AfxInitRichEdit2() が必要です
	// AfxInitRichEdit2();

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	m_szArgv1[0] = _T('\0');
	_tcscpy_s(m_szSavePath, _T("c:\\temp\\"));
	_tcscpy_s(m_szSaveName, _T("itiban.bmp"));
	m_clrBack = -1;

	SetRegistryKey(_T("Itiban Edit System"));
	LoadStdProfileSettings(4);  // 標準の INI ファイルのオプションをロードします (MRU を含む)


	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_SESTYPE,
		RUNTIME_CLASS(CTonDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CTonView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// メイン MDI フレーム ウィンドウを作成します。
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// 接尾辞が存在する場合にのみ DragAcceptFiles を呼び出します
	//  MDI アプリケーションでは、この呼び出しは、m_pMainWnd を設定した直後に発生しなければなりません。
	// ドラッグ/ドロップ オープンを許可します。
	m_pMainWnd->DragAcceptFiles();

	// DDE、file open など標準のシェル コマンドのコマンド ラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// DDE Execute open を使用可能にします。
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// コマンド ラインで指定されたディスパッチ コマンドです。アプリケーションが
	// /RegServer、/Register、/Unregserver または /Unregister で起動された場合、False を返します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CTonApp::ExitInstance()
{
	//TODO: 追加したリソースがある場合にはそれらも処理してください
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CTonApp メッセージ ハンドラー


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CTonApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CTonApp メッセージ ハンドラー
void CTonApp::OnFilePrintSetup()
{
	CWinApp::OnFilePrintSetup();
}

HANDLE CTonApp::GetDevMode()
{
	return(m_hDevMode);
}

HANDLE CTonApp::GetDevNames()
{
	return(m_hDevNames);
}

void CTonApp::AppExit()
{
	OnAppExit();
}

int CTonApp::CheckEnumPrinter(HANDLE hLoadDevNames)
{
	TCHAR	szLoadDevice[QRECMAX];
	TCHAR	szLoadDriver[QRECMAX];
	TCHAR	szLoadOutput[QRECMAX];
	TCHAR	szDevice[QRECMAX];
	TCHAR	szDriver[QRECMAX];
	TCHAR	szOutput[QRECMAX];
	LPPRINTER_INFO_2	lpINFO2 = NULL;		//PRINT_INFO_2構造体
	DWORD				dwNeeded;			//PRINT_INFO_X構造体に必要なｻｲｽﾞ
	DWORD				dwReturned;			//PRINT_INFO_X構造体の個数
	int					i, ret, len;
	CString				csStr;
	HANDLE				hPrinter = NULL;
	DWORD				nSize;
	LPDEVMODE			lpDevMode;

	CMainFrame *pcWnd = (CMainFrame*)GetMainWnd();

	if (hLoadDevNames == 0) {
		return(FALSE);
	}
	len = ::GlobalSize(hLoadDevNames);
	LPDEVNAMES lpDevNames = (LPDEVNAMES)::GlobalLock(hLoadDevNames);
	if (lpDevNames == NULL) {
		return(FALSE);
	}
	if (lpDevNames->wDeviceOffset >= len
		|| lpDevNames->wDriverOffset >= len
		|| lpDevNames->wOutputOffset >= len) {
		::GlobalUnlock(hLoadDevNames);
		return(FALSE);
	}
	_tcscpy_s(szLoadDevice, QRECMAX, (LPTSTR)(lpDevNames)+lpDevNames->wDeviceOffset);
	szLoadDevice[QRECMAX - 1] = _T('\0');
	_tcscpy_s(szLoadDriver, QRECMAX, (LPTSTR)(lpDevNames)+lpDevNames->wDriverOffset);
	szLoadDriver[QRECMAX - 1] = _T('\0');
	_tcscpy_s(szLoadOutput, QRECMAX, (LPTSTR)(lpDevNames)+lpDevNames->wOutputOffset);
	szLoadOutput[QRECMAX - 1] = _T('\0');
	::GlobalUnlock(hLoadDevNames);


	::EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, NULL, 0, &dwNeeded, &dwReturned);
	lpINFO2 = (LPPRINTER_INFO_2)new BYTE[dwNeeded];
	::EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, (LPBYTE)lpINFO2, dwNeeded, &dwNeeded, &dwReturned);
	ret = FALSE;
	// ロードしたプリンターと同じ名前があるか調べる
	for (i = 0; i < (int)dwReturned; i++) {
		if (_tcscmp(lpINFO2[i].pPrinterName, szLoadDevice) == 0) {
			ret = TRUE;
			break;
		}
	}
	if (lpINFO2 != NULL) {
		delete[]lpINFO2;
	}
	if (ret == FALSE) {
		return(FALSE);
	}

	try {

		_tcscpy_s(szDevice, QRECMAX, szLoadDevice);
		_tcscpy_s(szDriver, QRECMAX, szLoadDriver);
		if (::OpenPrinter(szDevice, &hPrinter, NULL) == FALSE) {
			return(FALSE);
		}
		
		::GetPrinter(hPrinter, 2, NULL, 0, &nSize);
		lpINFO2 = (LPPRINTER_INFO_2)new BYTE[nSize];;
		::GetPrinter(hPrinter, 2, (LPBYTE)lpINFO2, nSize, &nSize);
		_tcscpy_s(szOutput, lpINFO2->pPortName);
		_tcscpy_s(szDriver, lpINFO2->pDriverName);
		delete[] lpINFO2;

		ret = SetDevNamesElement(szDevice, szDriver, szOutput);
		
		if (ret == FALSE) {
			ClosePrinter(hPrinter);
			return(FALSE);
		}

		nSize = ::DocumentProperties(NULL, hPrinter, szDevice, NULL, NULL, 0);
		m_hTempDevMode = ::GlobalAlloc(GHND, nSize);
		lpDevMode = (LPDEVMODE)::GlobalLock(m_hTempDevMode);
		::DocumentProperties(NULL, hPrinter, szDevice, lpDevMode, NULL, DM_OUT_BUFFER);

		ClosePrinter(hPrinter);

		::GlobalUnlock(m_hTempDevMode);

		SelectPrinter(m_hTempDevNames, m_hTempDevMode, FALSE);
		
	}
	catch (CException* ex) {
		ex->Delete();
	}


	return(TRUE);
}

int CTonApp::SetDevNamesElement(LPTSTR lpDevice, LPTSTR lpDriver, LPTSTR lpOutput)
{
	LPDEVNAMES	lpDevNames;
	long		lSize;

	int		nDrvLen = _tcslen(lpDriver) + 1;
	int		nDevLen = _tcslen(lpDevice) + 1;
	int		nPortLen = _tcslen(lpOutput) + 1;

	lSize = sizeof(DEVNAMES) + (nDrvLen + nDevLen + nPortLen) * sizeof(TCHAR);

	m_hTempDevNames = ::GlobalAlloc(GHND, lSize);
	if (m_hTempDevNames == 0) {
		return(FALSE);
	}
	lpDevNames = (LPDEVNAMES)::GlobalLock(m_hTempDevNames);
	if (lpDevNames == NULL) {
		::GlobalFree(m_hTempDevNames);
		return(FALSE);
	}

	lpDevNames->wDriverOffset = (sizeof(DEVNAMES) / sizeof(TCHAR));
	lpDevNames->wDeviceOffset = lpDevNames->wDriverOffset + nDrvLen;
	lpDevNames->wOutputOffset = lpDevNames->wDeviceOffset + nDevLen;
	lpDevNames->wDefault = 0;
	LPTSTR lpPos = (LPTSTR)lpDevNames;
	_tcscpy_s(&(lpPos[lpDevNames->wDriverOffset]), nDrvLen, lpDriver);
	_tcscpy_s(&(lpPos[lpDevNames->wDeviceOffset]), nDevLen, lpDevice);
	_tcscpy_s(&(lpPos[lpDevNames->wOutputOffset]), nPortLen, lpOutput);

	::GlobalUnlock(m_hTempDevNames);

	return(TRUE);
}

void CTonApp::CreateWakuNameList(LPTSTR lpWakuPath)
{
	CFileFind	finder;
	BOOL	bWorking;
	TCHAR	szWakuListFile[FNAMEMAX];
	TCHAR	szSrchName[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	_stprintf_s(szWakuListFile, _T("%s\\wakuname.lst"), lpWakuPath);
	_tfopen_s(&fp, szWakuListFile, _T("w"));

	_stprintf_s(szSrchName, _T("%s\\*.wak"), lpWakuPath);
	bWorking = finder.FindFile(szSrchName);
	while (bWorking){
		bWorking = finder.FindNextFile();
		if(finder.IsDots() == FALSE && finder.IsDirectory() == FALSE){
			_tcscpy_s(szFileName, finder.GetFileName());
			_stprintf_s(buf, _T("\"%s\"\n"), szFileName);
			baselibFputs(buf, fp);
		}
		
	}
	finder.Close();

	fclose(fp);
}


