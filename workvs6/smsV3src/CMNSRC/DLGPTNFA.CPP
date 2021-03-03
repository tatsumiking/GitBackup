// DlgPtnFA.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgPtnFA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnFiguArc ダイアログ


CDialogPtnFiguArc::CDialogPtnFiguArc(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPtnFiguArc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPtnFiguArc)
	m_dCX = 0.0;
	m_dCY = 0.0;
	m_dR = 0.0;
	m_dSRag = 0.0;
	m_dERag = 0.0;
	//}}AFX_DATA_INIT
}


void CDialogPtnFiguArc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPtnFiguArc)
	DDX_Text(pDX, IDC_EDCX, m_dCX);
	DDX_Text(pDX, IDC_EDCY, m_dCY);
	DDX_Text(pDX, IDC_EDR, m_dR);
	DDX_Text(pDX, IDC_EDSRAG, m_dSRag);
	DDX_Text(pDX, IDC_EDERAG, m_dERag);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPtnFiguArc, CDialog)
	//{{AFX_MSG_MAP(CDialogPtnFiguArc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnFiguArc メッセージ ハンドラ
