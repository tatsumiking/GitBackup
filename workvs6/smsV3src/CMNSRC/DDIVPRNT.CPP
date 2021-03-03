// DDivPrnt.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DDivPrnt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDivPrint ダイアログ


CDialogDivPrint::CDialogDivPrint(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDivPrint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDivPrint)
	m_dNorisiro = 0.0;
	m_dPrintXSize = 0.0;
	m_dPrintYSize = 0.0;
	//}}AFX_DATA_INIT
	m_pcView = (CTonView*)pParent;
}


void CDialogDivPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDivPrint)
	DDX_Text(pDX, IDC_EDNORISIRO, m_dNorisiro);
	DDX_Text(pDX, IDC_EDPRNTXSIZE, m_dPrintXSize);
	DDX_Text(pDX, IDC_EDPRNTYSIZE, m_dPrintYSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogDivPrint, CDialog)
	//{{AFX_MSG_MAP(CDialogDivPrint)
	ON_BN_CLICKED(ID_DLGSET, OnDlgset)
	ON_BN_CLICKED(ID_DLGRESET, OnDlgreset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDivPrint メッセージ ハンドラ

void CDialogDivPrint::OnDlgset() 
{
	UpdateData(TRUE);
	m_pcView->CommandExec(DLGSETEVENT, 0, 0);
}

void CDialogDivPrint::OnDlgreset() 
{
	UpdateData(TRUE);
	m_pcView->CommandExec(DLGRESETEVENT, 0, 0);
}

void CDialogDivPrint::OnCancel() 
{
	UpdateData(TRUE);
	m_pcView->CommandExec(ENDEVENT, 0, 0);
}
