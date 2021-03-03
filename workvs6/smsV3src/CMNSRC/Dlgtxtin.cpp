// DlgTxtIn.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "DlgTxtIn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogTextInput ダイアログ


CDialogTextInput::CDialogTextInput(CWnd* pParent /*=NULL*/)
	: CMyDialog(CDialogTextInput::IDD, pParent)
{
	m_lpTitle = NULL;
	m_lpTitleText = NULL;
	m_nIMEMode = OFF;
	//{{AFX_DATA_INIT(CDialogTextInput)
	m_csText = _T("");
	//}}AFX_DATA_INIT
}

void CDialogTextInput::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTextInput)
	DDX_Control(pDX, IDC_STTITLE, m_stTitle);
	DDX_Control(pDX, IDC_EDNAME, m_edText);
	DDX_Text(pDX, IDC_EDNAME, m_csText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTextInput, CMyDialog)
	//{{AFX_MSG_MAP(CDialogTextInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTextInput メッセージ ハンドラ

void CDialogTextInput::SetTitleText(LPSTR lpTitleText)
{
	m_lpTitleText = lpTitleText;
}

void CDialogTextInput::SetDialogTitle(LPSTR lpTitle)
{
	m_lpTitle = lpTitle;
}

BOOL CDialogTextInput::OnInitDialog() 
{
	CMyDialog::OnInitDialog();

	if(m_lpTitle != NULL){
		SetWindowText(m_lpTitle);
	}
	if(m_lpTitleText != NULL){
		m_stTitle.SetWindowText(m_lpTitleText);
	}
	if(m_nIMEMode == ON){
		m_edText.SetIMEMode(ON);
	}
	m_edText.SetFocus();

	return TRUE;
}

void CDialogTextInput::OnOK() 
{
	if(m_nIMEMode == ON){
		m_edText.SetIMEMode(OFF);
	}
	CMyDialog::OnOK();
}
