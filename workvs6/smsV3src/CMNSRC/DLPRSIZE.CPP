// DialogPrintSize.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlPrSize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPrintSize ダイアログ


CDialogPrintSize::CDialogPrintSize(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPrintSize::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPrintSize)
	m_csSizeMsg = _T("");
	//}}AFX_DATA_INIT
}


void CDialogPrintSize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPrintSize)
	DDX_Text(pDX, IDC_SIZEMSG, m_csSizeMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPrintSize, CDialog)
	//{{AFX_MSG_MAP(CDialogPrintSize)
	ON_BN_CLICKED(IDNO, OnNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPrintSize メッセージ ハンドラ

void CDialogPrintSize::OnNo() 
{
	CDialog::EndDialog(IDNO);
}
