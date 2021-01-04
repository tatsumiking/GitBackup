// sgs.cpp : �A�v���P�[�V�����p�N���X�̋@�\��`���s���܂��B
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "direct.h"
#include <winspool.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//extern "C" {
	#include "../../ObjLib/Senvlib.h"
//}

/////////////////////////////////////////////////////////////////////////////
// CTonApp

BEGIN_MESSAGE_MAP(CTonApp, CWinApp)
	//{{AFX_MSG_MAP(CTonApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// �W���̃t�@�C����{�h�L�������g �R�}���h
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// �W���̈���Z�b�g�A�b�v �R�}���h
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTonApp �N���X�̍\�z

CTonApp::CTonApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif
	SetAppID(_T("Sgs.AppID.NoVersion"));

	DWORD dwVersion = ::GetVersion();
	m_bWin4 = (BYTE)dwVersion >= 4;
#ifndef _UNICODE
	m_bWin31 = (dwVersion > 0x80000000 && !m_bWin4);
#endif
	m_nDefFont = (m_bWin4) ? DEFAULT_GUI_FONT : ANSI_VAR_FONT;
	m_dcScreen.Attach(::GetDC(NULL));
	m_bLargeIcons = m_dcScreen.GetDeviceCaps(LOGPIXELSX) >= 120;
	m_pcSysEnv = new CSysEnv();
	m_pcCharLib = new CCharLib();

	m_hTempStr = ::GlobalAlloc(GHND, sizeof(TCHAR) * TEMPBUFMAX2);
	m_lpTempStr = (TCHAR*)::GlobalLock(m_hTempStr); 
}

CTonApp::~CTonApp()
{
	::GlobalUnlock(m_hTempStr);
	::GlobalFree(m_hTempStr);
	delete(m_pcSysEnv);
	delete(m_pcCharLib);
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CTonApp �I�u�W�F�N�g

CTonApp theApp;

// ���� ID �͂��̃A�v���P�[�V���������v�I�Ƀ��j�[�N�ɂȂ�悤�ɐ�������܂����B
// �����A���ʂ� ID ���w�肵�����Ȃ�΁A�ύX���Ă����܂��܂���B

// {BD0E5764-DB2D-11D1-A09D-444553540000}
static const CLSID clsid =
{ 0xbd0e5764, 0xdb2d, 0x11d1, { 0xa0, 0x9d, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

/////////////////////////////////////////////////////////////////////////////
// CTonApp �N���X�̏�����

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
	lpSubKey = _T("dummy");
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
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	// OLE ���C�u�����̏�����
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);


#ifdef _AFXDLL
	Enable3dControls();			// ���L DLL �̒��� MFC ���g�p����ꍇ�ɂ͂������Ăяo���Ă��������B
#else
	Enable3dControlsStatic();	// MFC �ƐÓI�Ƀ����N���Ă���ꍇ�ɂ͂������Ăяo���Ă��������B
#endif
	m_szArgv1[0] = _T('\0');
	m_szSavePath[0] = _T('\0');
	m_szSaveName[0] = _T('\0');
	m_clrBack = -1;

	SetRegistryKey(_T("�M�k�V�X�e���@�O���쐬��"));

	LoadStdProfileSettings(4);  // �W���� INI �t�@�C���̃I�v�V���������[�ނ��܂� (MRU ���܂�)

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_SGSTYPE,
		RUNTIME_CLASS(CTonDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CTonView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	m_pMainWnd->DragAcceptFiles();

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}
int CTonApp::ExitInstance()
{
	AfxOleTerm(FALSE);
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ŏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard ���z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	//{{AFX_MSG(CAboutDlg)
		// ���b�Z�[�W �n���h���͂���܂���B
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
		// ���b�Z�[�W �n���h���͂���܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// �_�C�A���O�����s���邽�߂̃A�v���P�[�V���� �R�}���h
void CTonApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTonApp �R�}���h

void CTonApp::OnFilePrintSetup() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CPrintDialog pd(TRUE);
	DoPrintDialog(&pd);
	m_hDevMode = pd.m_pd.hDevMode;
	m_hDevNames = pd.m_pd.hDevNames;
}

void CTonApp::OnFileOpen() 
{
	TCHAR	datpath[FNAMEMAX];

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
	TCHAR	szDevice[QRECMAX];
	TCHAR	szDriver[QRECMAX];
	TCHAR	szOutput[QRECMAX];
	LPPRINTER_INFO_2	lpINFO2 = NULL;		//PRINT_INFO_2�\����
	DWORD				dwNeeded;			//PRINT_INFO_X�\���̂ɕK�v�Ȼ���
	DWORD				dwReturned;			//PRINT_INFO_X�\���̂̌�
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
	_tcsncpy(szDevice, (LPTSTR)(lpDevNames) + lpDevNames->wDeviceOffset, QRECMAX);
	szDevice[QRECMAX-1] = _T('\0');
	_tcsncpy(szDriver, (LPTSTR)(lpDevNames) + lpDevNames->wDriverOffset, QRECMAX);
	szDriver[QRECMAX-1] = _T('\0');
	_tcsncpy(szOutput, (LPTSTR)(lpDevNames) + lpDevNames->wOutputOffset, QRECMAX);
	szOutput[QRECMAX-1] = _T('\0');
	::GlobalUnlock(hDevNames);


	::EnumPrinters(PRINTER_ENUM_LOCAL,NULL,2,NULL,0,&dwNeeded,&dwReturned);
	lpINFO2 = (LPPRINTER_INFO_2)new BYTE[dwNeeded];
	::EnumPrinters(PRINTER_ENUM_LOCAL,NULL,2,(LPBYTE)lpINFO2,dwNeeded,&dwNeeded,&dwReturned);
	ret = FALSE;
	for(i = 0; i < (int)dwReturned; i++){
		if(_tcscmp(lpINFO2[i].pPrinterName, szDevice) == 0){
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
			_tcscpy_s(szOutput, lpINFO2->pPortName);
			_tcscpy_s(szDriver, lpINFO2->pDriverName);
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

void CTonApp::SetDevNamesElement(LPTSTR lpDevice, LPTSTR lpDriver, LPTSTR lpOutput)
{
	LPDEVNAMES	lpDevNames;
	long		lSize;

	int		nDrvLen = _tcslen(lpDriver) + 1;
	int		nDevLen = _tcslen(lpDevice) + 1;
	int		nPortLen = _tcslen(lpOutput) + 1;
	lSize = sizeof(DEVNAMES) + (nDrvLen + nDevLen + nPortLen) * sizeof(TCHAR);
	if(m_hDevNames == 0){
		m_hDevNames = ::GlobalAlloc(GHND, lSize);
	}
	else{
		m_hDevNames = ::GlobalReAlloc(m_hDevNames, lSize, GHND);
	}
	lpDevNames = (LPDEVNAMES)::GlobalLock(m_hDevNames);
	lpDevNames->wDriverOffset = sizeof(DEVNAMES);
	lpDevNames->wDeviceOffset = lpDevNames->wDriverOffset + nDrvLen;
	lpDevNames->wOutputOffset = lpDevNames->wDeviceOffset + nDevLen;
	lpDevNames->wDefault = 0;
	LPTSTR lpPos = (LPTSTR)lpDevNames;
	_tcscpy_s(&(lpPos[lpDevNames->wDriverOffset]), nDrvLen, lpDriver);
	_tcscpy_s(&(lpPos[lpDevNames->wDeviceOffset]), nDevLen, lpDevice);
	_tcscpy_s(&(lpPos[lpDevNames->wOutputOffset]), nPortLen, lpOutput);
	::GlobalUnlock(m_hDevNames);
}

