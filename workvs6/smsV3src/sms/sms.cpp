// sms.cpp : �A�v���P�[�V�����p�N���X�̋@�\��`���s���܂��B
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
	// �W���̃t�@�C����{�h�L�������g �R�}���h
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTonApp �N���X�̍\�z

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
// �B��� CTonApp �I�u�W�F�N�g

CTonApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTonApp �N���X�̏�����

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
	Enable3dControls();		// ���L DLL �̒��� MFC ���g�p����ꍇ�ɂ͂������Ăяo���Ă��������B
#else
	Enable3dControlsStatic();	// MFC �ƐÓI�Ƀ����N���Ă���ꍇ�ɂ͂������Ăяo���Ă��������B
#endif

	// �ݒ肪�ۑ�����鉺�̃��W�X�g�� �L�[��ύX���܂��B
	// TODO: ���̕�������A��Ж��܂��͏����ȂǓK�؂Ȃ��̂�
	// �ύX���Ă��������B
	SetRegistryKey(_T("�M�k�V�X�e�� Ver1.00"));

	LoadStdProfileSettings(0);  // �W���� INI �t�@�C���̃I�v�V���������[�ނ��܂� (MRU ���܂�)

	// �A�v���P�[�V�����p�̃h�L�������g �e���v���[�g��o�^���܂��B�h�L�������g �e���v���[�g
	//  �̓h�L�������g�A�t���[�� �E�B���h�E�ƃr���[���������邽�߂ɋ@�\���܂��B

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_SMSTYPE,
		RUNTIME_CLASS(CTonDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CTonView));
	AddDocTemplate(pDocTemplate);

	// ���C�� MDI �t���[�� �E�B���h�E���쐬
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// DDE�Afile open �ȂǕW���̃V�F�� �R�}���h�̃R�}���h���C������͂��܂��B
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// �R�}���h���C���Ńf�B�X�p�b�` �R�}���h���w�肵�܂��B
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
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
// CTonApp ���b�Z�[�W �n���h��

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
	LPPRINTER_INFO_2	lpINFO2 = NULL;		//PRINT_INFO_2�\����
	DWORD				dwNeeded;			//PRINT_INFO_X�\���̂ɕK�v�Ȼ���
	DWORD				dwReturned;			//PRINT_INFO_X�\���̂̌�
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

