// MyDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDialog ダイアログ


CMyDialog::CMyDialog(LPCTSTR lpszTemplateName, CWnd* pParent /*=NULL*/)
	: CDialog(lpszTemplateName, pParent)
{
}

CMyDialog::CMyDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
}


void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDialog)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
	//{{AFX_MSG_MAP(CMyDialog)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDialog メッセージ ハンドラ

void CMyDialog::KeyIn(UINT nChar, UINT nID)
{

}

void CMyDialog::SelectEdit(UINT nID)
{

}

