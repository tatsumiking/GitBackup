// DlgStLup.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogStrgLineup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	BMPXSIZE	150
#define BMPYSIZE	150
/////////////////////////////////////////////////////////////////////////////
// CDialogStrgLineup ダイアログ


CDialogStrgLineup::CDialogStrgLineup(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogStrgLineup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogStrgLineup)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDialogStrgLineup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogStrgLineup)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogStrgLineup, CDialog)
	//{{AFX_MSG_MAP(CDialogStrgLineup)
	ON_BN_CLICKED(IDC_MODE1, OnMode1)
	ON_BN_CLICKED(IDC_MODE2, OnMode2)
	ON_BN_CLICKED(IDC_MODE3, OnMode3)
	ON_BN_CLICKED(IDC_MODE4, OnMode4)
	ON_BN_CLICKED(IDC_MODE5, OnMode5)
	ON_BN_CLICKED(IDC_MODE6, OnMode6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgLineup メッセージ ハンドラ

void CDialogStrgLineup::OnMode1() 
{
	m_nSlctMode = LINEUPUP;
	CDialog::OnOK();
}

void CDialogStrgLineup::OnMode2() 
{
	m_nSlctMode = LINEUPUDCNTR;
	CDialog::OnOK();
}

void CDialogStrgLineup::OnMode3() 
{
	m_nSlctMode = LINEUPDOWN;
	CDialog::OnOK();
}

void CDialogStrgLineup::OnMode4() 
{
	m_nSlctMode = LINEUPLEFT;
	CDialog::OnOK();
}

void CDialogStrgLineup::OnMode5() 
{
	m_nSlctMode = LINEUPLRCNTR;
	CDialog::OnOK();
}

void CDialogStrgLineup::OnMode6() 
{
	m_nSlctMode = LINEUPRIGHT;
	CDialog::OnOK();
}
