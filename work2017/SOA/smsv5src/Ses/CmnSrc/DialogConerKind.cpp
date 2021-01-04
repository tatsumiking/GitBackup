// DlgCnrKd.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogConerKind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogConerKind ダイアログ

CDialogConerKind::CDialogConerKind(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogConerKind::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogConerKind)
	m_csConerKind = _T("");
	//}}AFX_DATA_INIT
}


void CDialogConerKind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogConerKind)
	DDX_Control(pDX, IDC_CBCONER_KIND, m_cbConerKind);
	DDX_CBString(pDX, IDC_CBCONER_KIND, m_csConerKind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogConerKind, CDialog)
	//{{AFX_MSG_MAP(CDialogConerKind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogConerKind メッセージ ハンドラ

void CDialogConerKind::SetDialogTitle(LPTSTR lpTitle)
{
	m_lpTitle = lpTitle;
}

BOOL CDialogConerKind::OnInitDialog() 
{
	TCHAR	buf[RECMAX];

	CDialog::OnInitDialog();
	SetWindowText(m_lpTitle);
	_stprintf_s(buf, _T("支点左下         %d"), LEFTDOWN);
	m_cbConerKind.AddString(buf);
	if(m_nConerKind == LEFTDOWN){
		m_csConerKind = buf;
	}

	_stprintf_s(buf, _T("支点左上         %d"), LEFTUP);
	m_cbConerKind.AddString(buf);
	if(m_nConerKind == LEFTUP){
		m_csConerKind = buf;
	}

	_stprintf_s(buf, _T("支点右上         %d"), RIGHTUP);
	m_cbConerKind.AddString(buf);
	if(m_nConerKind == RIGHTUP){
		m_csConerKind = buf;
	}

	_stprintf_s(buf, _T("支点右下         %d"), RIGHTDOWN);
	m_cbConerKind.AddString(buf);
	if(m_nConerKind == RIGHTDOWN){
		m_csConerKind = buf;
	}

	_stprintf_s(buf, _T("支点中央         %d"), CNTRCNTR);
	m_cbConerKind.AddString(buf);
	if(m_nConerKind == CNTRCNTR){
		m_csConerKind = buf;
	}
	
	UpdateData(FALSE);
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogConerKind::OnOK() 
{
	TCHAR	buf[RECMAX];
	TCHAR	tmp[QRECMAX];

	UpdateData(TRUE);
	_tcscpy_s(buf, m_csConerKind.GetBuffer(RECMAX));	
	_stscanf_s(buf, _T("%s %d"), tmp, QRECMAX, &m_nConerKind);
	CDialog::OnOK();
}
