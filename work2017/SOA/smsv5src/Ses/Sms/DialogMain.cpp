// DlgMain.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "io.h"
#include "DialogMain.h"
#include "DialogInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMERINTERVAL	1000
/////////////////////////////////////////////////////////////////////////////
// CDialogMain ダイアログ
//extern "C" {
	#include "../../ObjLib/Senvlib.h"
	#include "../../ObjLib/Scrnlib.h"
//}

CDialogMain::CDialogMain(CWnd* pcParent /*=NULL*/)
	: CDialog(CDialogMain::IDD, pcParent)
{
	m_pcView = (CTonView*)pcParent;
	m_hDib = 0;

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
	ON_BN_CLICKED(IDC_BTN_INSTR, OnBtnInstr)
	ON_BN_CLICKED(IDC_BTN_KDEN, OnBtnKden)
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

	if (m_hDib == 0) {
		return;
	}
	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if(lpDib == NULL){
		return;
	}
	GetWindowRect(&rect);
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
	TCHAR	szExePath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];
	CFile	cpFile;
	long	lFileSize;
	LPBYTE	lpDib;
	LPBITMAPINFOHEADER lpbminfo;

	senvgetexepath(szExePath);
	_stprintf_s(szFileName, _T("%s\\title.bmp"), szExePath);
	if((cpFile.Open(szFileName, CFile::modeRead)) == FALSE){
		return;
	}
	lFileSize = cpFile.GetLength();
	m_hDib = GlobalAlloc(GHND, lFileSize);
	lpDib = (LPBYTE)GlobalLock(m_hDib);
	cpFile.Read(lpDib, lFileSize);
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
	SetFocus();
}

void CDialogMain::OnBtnWakuedit() 
{
	TCHAR	szExePath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];

	senvgetexepath(szExePath);
	_stprintf_s(szFileName, _T("%s\\ses.exe"), szExePath);
	ProgExec(szFileName);
	SetFocus();
}

void CDialogMain::OnBtnGaijiedit() 
{
	TCHAR	szExePath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];

	senvgetexepath(szExePath);
	_stprintf_s(szFileName, _T("%s\\sgs.exe"), szExePath);
	ProgExec(szFileName);
	SetFocus();
}

void CDialogMain::OnBtnInstr() 
{
	TCHAR	szExePath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];

	senvgetexepath(szExePath);
	_stprintf_s(szFileName, _T("%s\\instr.exe"), szExePath);
	ProgExec(szFileName);
	SetFocus();
}

void CDialogMain::OnBtnKden() 
{
	TCHAR	szExePath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];

	senvgetexepath(szExePath);
	_stprintf_s(szFileName, _T("%s\\kden.exe"), szExePath);
	ProgExec(szFileName);
	SetFocus();
}

void CDialogMain::OnBtnOption() 
{
	TCHAR	szExePath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];

	senvgetexepath(szExePath);
	_stprintf_s(szFileName, _T("%s\\layout.exe"), szExePath);
	ProgExec(szFileName);
	SetFocus();
}

void CDialogMain::ProgExec(LPTSTR lpExexProg)
{
	static STARTUPINFO siStartInfo ;
	static PROCESS_INFORMATION piProcInfo ;

	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	CMainFrame *pcWnd = (CMainFrame*)pcApp->GetMainWnd();
	pcWnd->ShowWindow(SW_HIDE);
	ShowWindow(SW_HIDE);
	pcWnd->EnableWindow(FALSE);
	EnableWindow(FALSE);
	if(_taccess_s(lpExexProg, 0) == 0){
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
	TCHAR	szExePath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	CButton	*pcBtn;

	senvgetexepath(szExePath);
	_stprintf_s(szFileName, _T("%s\\instr.exe"), szExePath);
	if(_taccess_s(szFileName, 0) != 0){
		pcBtn = (CButton*)GetDlgItem(IDC_BTN_INSTR);
		pcBtn->ShowWindow(SW_HIDE);
	}
	_stprintf_s(szFileName, _T("%s\\kden.exe"), szExePath);
	if(_taccess_s(szFileName, 0) != 0){
		pcBtn = (CButton*)GetDlgItem(IDC_BTN_KDEN);
		pcBtn->ShowWindow(SW_HIDE);
	}

	_stprintf_s(szFileName, _T("%s\\smsmenu.env"), szExePath);
	_tfopen_s(&fp, szFileName, _T("r"));
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
