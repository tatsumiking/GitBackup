// DlgPtnFL.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgPtnFL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnFiguLine ダイアログ


CDialogPtnFiguLine::CDialogPtnFiguLine(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPtnFiguLine::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPtnFiguLine)
	m_dSX = 0.0;
	m_dSY = 0.0;
	m_dEX = 0.0;
	m_dEY = 0.0;
	//}}AFX_DATA_INIT
}


void CDialogPtnFiguLine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPtnFiguLine)
	DDX_Text(pDX, IDC_EDSX, m_dSX);
	DDX_Text(pDX, IDC_EDSY, m_dSY);
	DDX_Text(pDX, IDC_EDEX, m_dEX);
	DDX_Text(pDX, IDC_EDEY, m_dEY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPtnFiguLine, CDialog)
	//{{AFX_MSG_MAP(CDialogPtnFiguLine)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnFiguLine メッセージ ハンドラ
