// DlStrIns.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "MyEdit.h"
#include "DialogStrgInseart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgInseart �_�C�A���O


CDialogStrgInseart::CDialogStrgInseart(CWnd* pParent /*=NULL*/)
	: CMyDialog(CDialogStrgInseart::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogStrgInseart)
	m_chkMode = FALSE;
	m_csInsStrg = _T("");
	//}}AFX_DATA_INIT
}


void CDialogStrgInseart::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogStrgInseart)
	DDX_Control(pDX, IDC_ED_INSSTRG, m_edInsStrg);
	DDX_Check(pDX, IDC_CHECK_INSMODE, m_chkMode);
	DDX_Text(pDX, IDC_ED_INSSTRG, m_csInsStrg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogStrgInseart, CMyDialog)
	//{{AFX_MSG_MAP(CDialogStrgInseart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgInseart ���b�Z�[�W �n���h��

BOOL CDialogStrgInseart::OnInitDialog() 
{
	CMyDialog::OnInitDialog();

	m_edInsStrg.SetIMEMode(ON);
	m_edInsStrg.SetFocus();

	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}
