// DProgBar.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DProgBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogProgresBar ダイアログ


CDialogProgresBar::CDialogProgresBar(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogProgresBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogProgresBar)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDialogProgresBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogProgresBar)
	DDX_Control(pDX, IDC_PROGRESSBAR, m_ProgBar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogProgresBar, CDialog)
	//{{AFX_MSG_MAP(CDialogProgresBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogProgresBar メッセージ ハンドラ

BOOL CDialogProgresBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ProgBar.SetRange(0, 1000);
	m_ProgBar.SetPos(0);
	return TRUE;
}
