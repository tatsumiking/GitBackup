// DlgPtnFE.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgPtnFE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnFiguEArc ダイアログ


CDialogPtnFiguEArc::CDialogPtnFiguEArc(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPtnFiguEArc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPtnFiguEArc)
	m_dCX = 0.0;
	m_dCY = 0.0;
	m_dWidth = 0.0;
	m_dHight = 0.0;
	m_dSRag = 0.0;
	m_dERag = 0.0;
	//}}AFX_DATA_INIT
}


void CDialogPtnFiguEArc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPtnFiguEArc)
	DDX_Text(pDX, IDC_EDCX, m_dCX);
	DDX_Text(pDX, IDC_EDCY, m_dCY);
	DDX_Text(pDX, IDC_EDWIDTH, m_dWidth);
	DDX_Text(pDX, IDC_EDHIGHT, m_dHight);
	DDX_Text(pDX, IDC_EDSRAG, m_dSRag);
	DDX_Text(pDX, IDC_EDERAG, m_dERag);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPtnFiguEArc, CDialog)
	//{{AFX_MSG_MAP(CDialogPtnFiguEArc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnFiguEArc メッセージ ハンドラ
