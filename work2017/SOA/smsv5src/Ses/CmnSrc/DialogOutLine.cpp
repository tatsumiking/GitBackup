// DlgOutLn.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "DialogOutLine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogOutLine �_�C�A���O


CDialogOutLine::CDialogOutLine(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogOutLine::IDD, pParent)
{
	m_lpTitle = _T("�ӂ��ǂ�");
	//{{AFX_DATA_INIT(CDialogOutLine)
	m_dOutLineMM = 0.0;
	m_nOutLineKind = 1;
	//}}AFX_DATA_INIT
}

void CDialogOutLine::SetTitle(LPTSTR lpTitle)
{
	m_lpTitle = lpTitle;
}

void CDialogOutLine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogOutLine)
	DDX_Text(pDX, IDC_ED_OUTLINEMM, m_dOutLineMM);
	DDX_Radio(pDX, IDC_RBTN_KADO, m_nOutLineKind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogOutLine, CDialog)
	//{{AFX_MSG_MAP(CDialogOutLine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogOutLine ���b�Z�[�W �n���h��

BOOL CDialogOutLine::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText(m_lpTitle);	
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}
