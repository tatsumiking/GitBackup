// DMmryEnvSet.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "ses.h"
#include "DMmryEnvSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogMemoryEnvSet ダイアログ


CDialogMemoryEnvSet::CDialogMemoryEnvSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMemoryEnvSet::IDD, pParent)
{
	m_pcApp = (CTonApp*)AfxGetApp();
	m_pcWnd = (CMainFrame*)m_pcApp->GetMainWnd();
	//{{AFX_DATA_INIT(CDialogMemoryEnvSet)
	m_csMemoTempPath = _T("");
	m_csMemoBackPath = _T("");
	m_csMemoPhotoPath = _T("");
	//}}AFX_DATA_INIT
}


void CDialogMemoryEnvSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMemoryEnvSet)
	DDX_Text(pDX, IDC_EDMEMOTEMPPATH, m_csMemoTempPath);
	DDX_Text(pDX, IDC_EDMEMOBACKPATH, m_csMemoBackPath);
	DDX_Text(pDX, IDC_EDMEMOPHOTOPATH, m_csMemoPhotoPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMemoryEnvSet, CDialog)
	//{{AFX_MSG_MAP(CDialogMemoryEnvSet)
	ON_BN_CLICKED(IDC_BTNMEMOBACKPATH, OnBtnmemobackpath)
	ON_BN_CLICKED(IDC_BTNMEMOPHOTOPATH, OnBtnmemophotopath)
	ON_BN_CLICKED(IDC_BTNMEMOTEMPPATH, OnBtnmemotemppath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMemoryEnvSet メッセージ ハンドラ
int CALLBACK CDialogMemoryEnvSet::BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if((BFFM_INITIALIZED == uMsg) && lpData){
		::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
	}
	return 0;
}

BOOL CDialogMemoryEnvSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_csMemoBackPath = _T(m_pcApp->m_szBackImagePath);
	m_csMemoPhotoPath = _T(m_pcApp->m_szPhotoImagePath);
	m_csMemoTempPath = _T(m_pcApp->m_szTempImagePath);
	UpdateData(FALSE);
	return TRUE;
}

void CDialogMemoryEnvSet::OnBtnmemobackpath() 
{
	char	szPath[RECMAX];

	UpdateData(TRUE);
	strcpy(szPath, m_csMemoBackPath.GetBuffer(FNAMEMAX));
	GetSelectDir("背景読込場所設定", szPath);
	if(szPath[0] != '\0'){
		m_csMemoBackPath = _T(szPath);
		UpdateData(FALSE);
	}
}

void CDialogMemoryEnvSet::OnBtnmemophotopath() 
{
	char	szPath[RECMAX];

	UpdateData(TRUE);
	strcpy(szPath, m_csMemoPhotoPath.GetBuffer(FNAMEMAX));
	GetSelectDir("写真読込場所設定", szPath);
	if(szPath[0] != '\0'){
		m_csMemoPhotoPath = _T(szPath);
		UpdateData(FALSE);
	}
}

void CDialogMemoryEnvSet::OnBtnmemotemppath() 
{
	char	szPath[RECMAX];

	UpdateData(TRUE);
	strcpy(szPath, m_csMemoTempPath.GetBuffer(FNAMEMAX));
	GetSelectDir("保存場所設定", szPath);
	if(szPath[0] != '\0'){
		m_csMemoTempPath = _T(szPath);
		UpdateData(FALSE);
	}
}

void CDialogMemoryEnvSet::OnOK() 
{
	strcpy(m_pcApp->m_szBackImagePath, m_csMemoBackPath.GetBuffer(FNAMEMAX));
	strcpy(m_pcApp->m_szPhotoImagePath, m_csMemoPhotoPath.GetBuffer(FNAMEMAX));
	strcpy(m_pcApp->m_szTempImagePath, m_csMemoTempPath.GetBuffer(FNAMEMAX));
	m_pcWnd->MemoryEnvSave();
	CDialog::OnOK();
}

void CDialogMemoryEnvSet::GetSelectDir(LPSTR lpTitle, LPSTR lpPath)
{
	BROWSEINFO	bInfo;
	LPITEMIDLIST	lpFoldPointer;

	memset(&bInfo, 0, sizeof(BROWSEINFO));
	bInfo.hwndOwner = this->m_hWnd;
	bInfo.pidlRoot = 0;
	bInfo.lpszTitle = lpTitle;
	bInfo.ulFlags = 1;
	bInfo.lpfn   = BrowseCallbackProc; // コールバーク関数
	bInfo.lParam = (LPARAM)(lpPath);       // 初期フォルダ
	lpFoldPointer = SHBrowseForFolder(&bInfo);
	SHGetPathFromIDList(lpFoldPointer, lpPath);
}

