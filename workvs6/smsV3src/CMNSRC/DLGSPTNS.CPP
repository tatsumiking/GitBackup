// DlgSPtnS.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgSPtnS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSPtnSave ダイアログ
extern "C" {
	void ptnenvlistgetname(int slctidx, LPSTR ptnfname);
}

CDialogSPtnSave::CDialogSPtnSave(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSPtnSave::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSPtnSave)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDialogSPtnSave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSPtnSave)
	DDX_Control(pDX, IDC_LSTSPTN, m_lstSPtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSPtnSave, CDialog)
	//{{AFX_MSG_MAP(CDialogSPtnSave)
	ON_LBN_SELCHANGE(IDC_LSTSPTN, OnSelchangeLstsptn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSPtnSave メッセージ ハンドラ

BOOL CDialogSPtnSave::OnInitDialog() 
{
	int		idx;
	char	ptnname[32];

	CDialog::OnInitDialog();

	idx = 0;
	while(1){
		ptnenvlistgetname(idx, ptnname);
		if(ptnname[0] == '\0'){
			break;
		}
		m_lstSPtn.AddString(ptnname);
		idx++;
	}
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogSPtnSave::OnSelchangeLstsptn() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_nSlctidx = m_lstSPtn.GetCurSel();
	CDialog::OnOK();
}
