// DialogSendDetails.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "es98.h"
#include "DialogSendDetails.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSendDetails ダイアログ


CDialogSendDetails::CDialogSendDetails(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSendDetails::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSendDetails)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDialogSendDetails::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSendDetails)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSendDetails, CDialog)
	//{{AFX_MSG_MAP(CDialogSendDetails)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSendDetails メッセージ ハンドラ
