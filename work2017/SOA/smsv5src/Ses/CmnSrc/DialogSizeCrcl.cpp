// DlgSCrcl.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "DialogSizeCrcl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSizeCrcl �_�C�A���O


CDialogSizeCrcl::CDialogSizeCrcl(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSizeCrcl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSizeCrcl)
	m_dCntrX = 0.0;
	m_dCntrY = 0.0;
	m_dHight = 0.0;
	m_dWidth = 0.0;
	//}}AFX_DATA_INIT
}


void CDialogSizeCrcl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSizeCrcl)
	DDX_Text(pDX, IDC_EDCX, m_dCntrX);
	DDX_Text(pDX, IDC_EDCY, m_dCntrY);
	DDX_Text(pDX, IDC_EDHIGHT, m_dHight);
	DDX_Text(pDX, IDC_EDWIDTH, m_dWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSizeCrcl, CDialog)
	//{{AFX_MSG_MAP(CDialogSizeCrcl)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSizeCrcl ���b�Z�[�W �n���h��
