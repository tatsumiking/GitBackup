// DlgDirSt.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogDirSlct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDirSlct ダイアログ


CDialogDirSlct::CDialogDirSlct(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDirSlct::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDirSlct)
	m_csSlctDir = _T("");
	//}}AFX_DATA_INIT
}

void CDialogDirSlct::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDirSlct)
	DDX_Text(pDX, IDC_STSLCTNAME, m_csSlctDir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogDirSlct, CDialog)
	//{{AFX_MSG_MAP(CDialogDirSlct)
	ON_LBN_SELCHANGE(IDC_DIRLIST, OnSelchangeDirlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDirSlct メッセージ ハンドラ

BOOL CDialogDirSlct::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	
	DirlistUpdate();
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogDirSlct::OnSelchangeDirlist() 
{
	DlgDirSelect(m_szPathName, FNAMEMAX, IDC_DIRLIST);
	DirlistUpdate();
}

void CDialogDirSlct::DirlistUpdate()
{
	TCHAR	szSrchPath[RECMAX];

	_stprintf_s(szSrchPath, _T("%s\\*.*"), m_szPathName);
	DlgDirList(szSrchPath, IDC_DIRLIST, IDC_STSLCTNAME, 0xC010);
}

void CDialogDirSlct::SetInitPath(LPTSTR path)
{
	_tcscpy_s(m_szPathName, path);
}
