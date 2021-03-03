// DlgMain.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "io.h"
#include "DlgMain.h"
#include "DlgInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMERINTERVAL	1000
/////////////////////////////////////////////////////////////////////////////
// CDialogMain ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
}

CDialogMain::CDialogMain(CWnd* pcParent /*=NULL*/)
	: CDialog(CDialogMain::IDD, pcParent)
{
	m_pcView = (CTonView*)pcParent;

	//{{AFX_DATA_INIT(CDialogMain)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDialogMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDialogMain)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogMain, CDialog)
	//{{AFX_MSG_MAP(CDialogMain)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_INPUT, OnBtnInput)
	ON_BN_CLICKED(IDC_BTN_WAKUEDIT, OnBtnWakuedit)
	ON_BN_CLICKED(IDC_BTN_GAIJIEDIT, OnBtnGaijiedit)
	ON_BN_CLICKED(IDC_BTN_OPTION, OnBtnOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMain メッセージ ハンドラ

void CDialogMain::OnPaint() 
{
	CRect	rect;
	LPBYTE	lpDib, lpBits;
	LPBITMAPFILEHEADER lpbmf;
	LPBITMAPINFOHEADER lpbminfo;

	CPaintDC dc(this);

	GetWindowRect(&rect);
	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if(lpDib == NULL){
		return;
	}
	lpbmf = (LPBITMAPFILEHEADER)lpDib;
	lpbminfo = (LPBITMAPINFOHEADER)(lpDib + sizeof(BITMAPFILEHEADER));
	lpBits = (LPBYTE)(lpDib + lpbmf->bfOffBits);
	StretchDIBits(dc.m_hDC, 
					0, 0, rect.Width(), rect.Height(), 
					0, 0, m_nWidth, m_nHight, 
					lpBits, (LPBITMAPINFO)lpbminfo, DIB_RGB_COLORS, SRCCOPY);

	GlobalUnlock(m_hDib);

}

void CDialogMain::DibLoad()
{
	char	szExePath[FNAMEMAX];
	char	szFileName[FNAMEMAX];
	CFile	cpFile;
	long	lFileSize;
	LPBYTE	lpDib;
	LPBITMAPINFOHEADER lpbminfo;

	senvgetexepath(szExePath);
	sprintf(szFileName, "%s\\title.bmp", szExePath);
	if((cpFile.Open(szFileName, CFile::modeRead)) == FALSE){
		return;
	}
	lFileSize = cpFile.GetLength();
	m_hDib = GlobalAlloc(GHND, lFileSize);
	lpDib = (LPBYTE)GlobalLock(m_hDib);
	cpFile.ReadHuge(lpDib, lFileSize);
	lpbminfo = (LPBITMAPINFOHEADER)(lpDib + sizeof(BITMAPFILEHEADER));
	m_nWidth = lpbminfo->biWidth;
	m_nHight = lpbminfo->biHeight;
	GlobalUnlock(m_hDib);
	cpFile.Close();
}

BOOL CDialogMain::OnInitDialog() 
{
	HICON hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon,FALSE);

	
	CDialog::OnInitDialog();
	DibLoad();
	InitShowHideBtn();
	return TRUE;
}

void CDialogMain::OnBtnInput() 
{
	CDialogInput *pcDialogInput;
	pcDialogInput = new CDialogInput(m_pcView);
	pcDialogInput->DoModal();
	//SetFocus();
}

void CDialogMain::OnBtnWakuedit() 
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];

	senvgetexepath(exepath);
	sprintf(fname, "%s\\ses.exe", exepath);
	ProgExec(fname);
	SetFocus();
}

void CDialogMain::OnBtnGaijiedit() 
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];

	senvgetexepath(exepath);
	sprintf(fname, "%s\\sgs.exe", exepath);
	ProgExec(fname);
	SetFocus();
}

void CDialogMain::OnBtnOption() 
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];

	senvgetexepath(exepath);
	sprintf(fname, "%s\\layout.exe", exepath);
	ProgExec(fname);
	SetFocus();
}

void CDialogMain::ProgExec(LPSTR lpExexProg)
{
	static STARTUPINFO siStartInfo ;
	static PROCESS_INFORMATION piProcInfo ;

	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	CMainFrame *pcWnd = (CMainFrame*)pcApp->GetMainWnd();
	m_pcView->CommandExec(DLGSETEVENT, 0L, 0L);
	pcWnd->ShowWindow(SW_HIDE);
	ShowWindow(SW_HIDE);
	pcWnd->EnableWindow(FALSE);
	EnableWindow(FALSE);
	if(_access(lpExexProg, 0) == 0){
	    GetStartupInfo(&siStartInfo);
		if(CreateProcess(NULL, lpExexProg, NULL, NULL, 
		      FALSE, 0, 
		      NULL, NULL, &siStartInfo, 
			  &piProcInfo) == 0){
			return;
		}
		m_hProcess = piProcInfo.hProcess;
		ProgExitLoop();
		CloseHandle(piProcInfo.hThread) ;
		CloseHandle(piProcInfo.hProcess);
		m_hProcess = 0;
	}
	pcWnd->EnableWindow(TRUE);
	EnableWindow(TRUE);
	ShowWindow(SW_SHOW);
	pcWnd->ShowWindow(SW_SHOW);
}

void CDialogMain::ProgExitLoop()
{
	DWORD	dwExitCode;
	int		ret;

	while(1){
		ret = GetExitCodeProcess(m_hProcess, &dwExitCode);
		if(dwExitCode != STILL_ACTIVE){
			break;
		}
		Sleep(TIMERINTERVAL);
		//Sleep(1000);
	}
}

void CDialogMain::InitShowHideBtn()
{
	char	szExePath[FNAMEMAX];
	char	szFileName[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	CButton	*pcBtn;

	senvgetexepath(szExePath);
	sprintf(szFileName, "%s\\smsmenu.env", szExePath);
	fp = fopen(szFileName, "r");
	if(fp == NULL){
		pcBtn = (CButton*)GetDlgItem(IDC_BTN_INPUT);
		pcBtn->ShowWindow(SW_HIDE);
		pcBtn = (CButton*)GetDlgItem(IDC_BTN_OPTION);
		pcBtn->ShowWindow(SW_HIDE);
		return;
	}
	fgets(buf, RECMAX, fp);
	pcBtn = (CButton*)GetDlgItem(IDC_BTN_WAKUEDIT);
	if(atoi(buf) == 0){
		pcBtn->ShowWindow(SW_HIDE);
	}
	fgets(buf, RECMAX, fp);
	pcBtn = (CButton*)GetDlgItem(IDC_BTN_GAIJIEDIT);
	if(atoi(buf) == 0){
		pcBtn->ShowWindow(SW_HIDE);
	}
	fgets(buf, RECMAX, fp);
	pcBtn = (CButton*)GetDlgItem(IDC_BTN_INPUT);
	if(atoi(buf) == 0){
		pcBtn->ShowWindow(SW_HIDE);
	}
	fgets(buf, RECMAX, fp);
	pcBtn = (CButton*)GetDlgItem(IDC_BTN_OPTION);
	if(atoi(buf) == 0){
		pcBtn->ShowWindow(SW_HIDE);
	}
	fclose(fp);
}
