// DlgNumArea.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "DialogNumArea.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogNumArea �_�C�A���O


CDialogNumArea::CDialogNumArea(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogNumArea::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogNumArea)
	m_csENum = _T("");
	m_csSNum = _T("");
	//}}AFX_DATA_INIT
}


void CDialogNumArea::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogNumArea)
	DDX_Text(pDX, IDC_EDENUM, m_csENum);
	DDX_Text(pDX, IDC_EDSNUM, m_csSNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogNumArea, CDialog)
	//{{AFX_MSG_MAP(CDialogNumArea)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogNumArea ���b�Z�[�W �n���h��

void CDialogNumArea::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	
	CDialog::OnOK();
}
