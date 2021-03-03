// DlgPtnLu.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgPtnLu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	BMPXSIZE	150
#define BMPYSIZE	150
/////////////////////////////////////////////////////////////////////////////
// CDialogPtnLineup ダイアログ


CDialogPtnLineup::CDialogPtnLineup(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPtnLineup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPtnLineup)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDialogPtnLineup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPtnLineup)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPtnLineup, CDialog)
	//{{AFX_MSG_MAP(CDialogPtnLineup)
	ON_BN_CLICKED(IDC_MODE1, OnMode1)
	ON_BN_CLICKED(IDC_MODE2, OnMode2)
	ON_BN_CLICKED(IDC_MODE3, OnMode3)
	ON_BN_CLICKED(IDC_MODE4, OnMode4)
	ON_BN_CLICKED(IDC_MODE5, OnMode5)
	ON_BN_CLICKED(IDC_MODE6, OnMode6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnLineup メッセージ ハンドラ


void CDialogPtnLineup::OnMode1() 
{
	m_nSlctMode = LINEUPUP;
	CDialog::OnOK();
}

void CDialogPtnLineup::OnMode2() 
{
	m_nSlctMode = LINEUPUDCNTR;
	CDialog::OnOK();
}

void CDialogPtnLineup::OnMode3() 
{
	m_nSlctMode = LINEUPDOWN;
	CDialog::OnOK();
}

void CDialogPtnLineup::OnMode4() 
{
	m_nSlctMode = LINEUPLEFT;
	CDialog::OnOK();
}

void CDialogPtnLineup::OnMode5() 
{
	m_nSlctMode = LINEUPLRCNTR;
	CDialog::OnOK();
}

void CDialogPtnLineup::OnMode6() 
{
	m_nSlctMode = LINEUPRIGHT;
	CDialog::OnOK();
}
