// DSetProtect.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DSetProtect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSetProtect ダイアログ


CDialogSetProtect::CDialogSetProtect(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetProtect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSetProtect)
	m_csPCID = _T("");
	m_csSetKey = _T("");
	m_csMsg = _T("");
	m_csMsg2 = _T("");
	//}}AFX_DATA_INIT
}


void CDialogSetProtect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSetProtect)
	DDX_Text(pDX, IDC_EDPCID, m_csPCID);
	DDX_Text(pDX, IDC_EDSETKEY, m_csSetKey);
	DDX_Text(pDX, IDC_STMSG, m_csMsg);
	DDX_Text(pDX, IDC_STMSG2, m_csMsg2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSetProtect, CDialog)
	//{{AFX_MSG_MAP(CDialogSetProtect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSetProtect メッセージ ハンドラ

