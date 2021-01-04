// DlgSBoxs.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogSizeBoxs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSizeBoxs ダイアログ


CDialogSizeBoxs::CDialogSizeBoxs(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSizeBoxs::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSizeBoxs)
	m_dCntrX = 0.0;
	m_dCntrY = 0.0;
	m_dHight = 0.0;
	m_dWidth = 0.0;
	//}}AFX_DATA_INIT
}


void CDialogSizeBoxs::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSizeBoxs)
	DDX_Text(pDX, IDC_EDCX, m_dCntrX);
	DDX_Text(pDX, IDC_EDCY, m_dCntrY);
	DDX_Text(pDX, IDC_EDHIGHT, m_dHight);
	DDX_Text(pDX, IDC_EDWIDTH, m_dWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSizeBoxs, CDialog)
	//{{AFX_MSG_MAP(CDialogSizeBoxs)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSizeBoxs メッセージ ハンドラ
