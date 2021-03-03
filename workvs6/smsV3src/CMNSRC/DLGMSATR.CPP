// DlgMsAtr.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgMsAtr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogMsurAtrSet ダイアログ


CDialogMsurAtrSet::CDialogMsurAtrSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMsurAtrSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMsurAtrSet)
	m_csMsurStrg = _T("");
	m_dFontSize = 0.0;
	//}}AFX_DATA_INIT
}


void CDialogMsurAtrSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMsurAtrSet)
	DDX_Text(pDX, IDC_MSUR_STRG, m_csMsurStrg);
	DDX_Text(pDX, IDC_MSUR_FONTSIZE, m_dFontSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMsurAtrSet, CDialog)
	//{{AFX_MSG_MAP(CDialogMsurAtrSet)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMsurAtrSet メッセージ ハンドラ
