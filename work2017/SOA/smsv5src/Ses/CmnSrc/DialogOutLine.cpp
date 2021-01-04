// DlgOutLn.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogOutLine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogOutLine ダイアログ


CDialogOutLine::CDialogOutLine(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogOutLine::IDD, pParent)
{
	m_lpTitle = _T("ふちどり");
	//{{AFX_DATA_INIT(CDialogOutLine)
	m_dOutLineMM = 0.0;
	m_nOutLineKind = 1;
	//}}AFX_DATA_INIT
}

void CDialogOutLine::SetTitle(LPTSTR lpTitle)
{
	m_lpTitle = lpTitle;
}

void CDialogOutLine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogOutLine)
	DDX_Text(pDX, IDC_ED_OUTLINEMM, m_dOutLineMM);
	DDX_Radio(pDX, IDC_RBTN_KADO, m_nOutLineKind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogOutLine, CDialog)
	//{{AFX_MSG_MAP(CDialogOutLine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogOutLine メッセージ ハンドラ

BOOL CDialogOutLine::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText(m_lpTitle);	
	
	// TODO: この位置に初期化の補足処理を追加してください
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
