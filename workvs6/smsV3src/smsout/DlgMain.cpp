// DlgMain.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgMain.h"
#include "DlgInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	return TRUE;
}

void CDialogMain::OnBtnInput() 
{
	CDialogInput *pcDialogInput;
	pcDialogInput = new CDialogInput(m_pcView);
	pcDialogInput->DoModal();
}

void CDialogMain::OnBtnWakuedit() 
{
	char	szExePath[FNAMEMAX];
	char	szProg[FNAMEMAX];

	senvgetexepath(szExePath);
	sprintf(szProg, "%s\\ses.exe", szExePath);
	ShellExecute(m_pcView->m_hWnd, NULL, szProg,
				NULL, NULL, SW_SHOWNORMAL);
}

void CDialogMain::OnBtnGaijiedit() 
{
	char	szExePath[FNAMEMAX];
	char	szProg[FNAMEMAX];

	senvgetexepath(szExePath);
	sprintf(szProg, "%s\\sgs.exe", szExePath);
	ShellExecute(m_pcView->m_hWnd, NULL, szProg,
				NULL, NULL, SW_SHOWNORMAL);
}

void CDialogMain::OnBtnOption() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
}
