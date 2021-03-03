// MyDlgBar.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MyDlgBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDialogBar ダイアログ


CMyDialogBar::CMyDialogBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
}

void CMyDialogBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDialogBar)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyDialogBar, CDialogBar)
	//{{AFX_MSG_MAP(CMyDialogBar)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDialogBar メッセージ ハンドラ

void CMyDialogBar::KeyIn(UINT nChar, UINT nID)
{

}

void CMyDialogBar::SelectEdit(UINT nID)
{

}

