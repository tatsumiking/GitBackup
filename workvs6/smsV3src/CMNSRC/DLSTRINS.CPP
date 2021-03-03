// DlStrIns.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MyEdit.h"
#include "DlStrIns.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgInseart ダイアログ


CDialogStrgInseart::CDialogStrgInseart(CWnd* pParent /*=NULL*/)
	: CMyDialog(CDialogStrgInseart::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogStrgInseart)
	m_chkMode = FALSE;
	m_csInsStrg = _T("");
	//}}AFX_DATA_INIT
}


void CDialogStrgInseart::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogStrgInseart)
	DDX_Control(pDX, IDC_ED_INSSTRG, m_edInsStrg);
	DDX_Check(pDX, IDC_CHECK_INSMODE, m_chkMode);
	DDX_Text(pDX, IDC_ED_INSSTRG, m_csInsStrg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogStrgInseart, CMyDialog)
	//{{AFX_MSG_MAP(CDialogStrgInseart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgInseart メッセージ ハンドラ

BOOL CDialogStrgInseart::OnInitDialog() 
{
	CMyDialog::OnInitDialog();

	m_edInsStrg.SetIMEMode(ON);
	m_edInsStrg.SetFocus();

	// TODO: この位置に初期化の補足処理を追加してください

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

