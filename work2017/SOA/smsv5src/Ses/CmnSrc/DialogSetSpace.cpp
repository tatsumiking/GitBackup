// DSetSp.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogSetSpace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSetSpace ダイアログ


CDialogSetSpace::CDialogSetSpace(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetSpace::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSetSpace)
	m_csTopSpace = _T("");
	m_csBottomSpace = _T("");
	m_csLeftSpace = _T("");
	m_csRightSpace = _T("");
	m_nTonnbo = FALSE;
	//}}AFX_DATA_INIT
}


void CDialogSetSpace::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSetSpace)
	DDX_Text(pDX, IDC_EDTOPSPACE, m_csTopSpace);
	DDX_Text(pDX, IDC_EDBOTTOMSPACE, m_csBottomSpace);
	DDX_Text(pDX, IDC_EDLEFSPACE, m_csLeftSpace);
	DDX_Text(pDX, IDC_EDRIGHTSPACE, m_csRightSpace);
	DDX_Check(pDX, IDC_CHKTONNBO, m_nTonnbo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSetSpace, CDialog)
	//{{AFX_MSG_MAP(CDialogSetSpace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSetSpace メッセージ ハンドラ

BOOL CDialogSetSpace::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CTonDoc *pcDoc = (CTonDoc*)m_pcView->GetDocument();

	m_csTopSpace.Format(_T("%1.1lf"), pcDoc->m_dTopSpace);
	m_csBottomSpace.Format(_T("%1.1lf"), pcDoc->m_dBottomSpace);
	m_csLeftSpace.Format(_T("%1.1lf"), pcDoc->m_dLeftSpace);
	m_csRightSpace.Format(_T("%1.1lf"), pcDoc->m_dRightSpace);
	m_nTonnbo = pcDoc->m_nTonnbo;
	UpdateData(FALSE);
	return TRUE;
}

void CDialogSetSpace::SetCrtView(CTonView *pcView)
{
	m_pcView = pcView;
}

void CDialogSetSpace::OnOK() 
{
	CTonDoc *pcDoc = (CTonDoc*)m_pcView->GetDocument();
	UpdateData(TRUE);
	pcDoc->m_dTopSpace = _ttof(m_csTopSpace.GetBuffer(QRECMAX));
	pcDoc->m_dBottomSpace = _ttof(m_csBottomSpace.GetBuffer(QRECMAX));
	pcDoc->m_dLeftSpace = _ttof(m_csLeftSpace.GetBuffer(QRECMAX));
	pcDoc->m_dRightSpace = _ttof(m_csRightSpace.GetBuffer(QRECMAX));
	pcDoc->m_nTonnbo = m_nTonnbo;
	CDialog::OnOK();
}

