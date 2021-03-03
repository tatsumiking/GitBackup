// DlgBmpRd.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgBmpRd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogBmpRound ダイアログ


CDialogBmpRound::CDialogBmpRound(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogBmpRound::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogBmpRound)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDialogBmpRound::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogBmpRound)
	DDX_Control(pDX, IDC_CBKIND, m_cbRoundKind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogBmpRound, CDialog)
	//{{AFX_MSG_MAP(CDialogBmpRound)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogBmpRound メッセージ ハンドラ

BOOL CDialogBmpRound::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_cbRoundKind.AddString("１８０°回転");
	m_cbRoundKind.AddString("左９０°回転");
	m_cbRoundKind.AddString("右９０°回転");
	m_cbRoundKind.AddString("上下反転");
	m_cbRoundKind.AddString("左右反転");
	m_cbRoundKind.SetCurSel(2);
	// TODO: この位置に初期化の補足処理を追加してください
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogBmpRound::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	m_nMode = m_cbRoundKind.GetCurSel();
	CDialog::OnOK();
}
