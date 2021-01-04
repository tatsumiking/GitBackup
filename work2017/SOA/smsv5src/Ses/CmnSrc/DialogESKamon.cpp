// DlgESKmn.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogESKamon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogESKamon ダイアログ


CDialogESKamon::CDialogESKamon(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogESKamon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogESKamon)
	m_csName = _T("");
	m_csFullName = _T("");
	//}}AFX_DATA_INIT
}


void CDialogESKamon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogESKamon)
	DDX_Text(pDX, IDC_EDNAME, m_csName);
	DDX_Text(pDX, IDC_EDFULLNAME, m_csFullName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogESKamon, CDialog)
	//{{AFX_MSG_MAP(CDialogESKamon)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogESKamon メッセージ ハンドラ
