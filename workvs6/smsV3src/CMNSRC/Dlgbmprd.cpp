// DlgBmpRd.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "DlgBmpRd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogBmpRound �_�C�A���O


CDialogBmpRound::CDialogBmpRound(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogBmpRound::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogBmpRound)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CDialogBmpRound::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogBmpRound)
	DDX_Control(pDX, IDC_CBKIND, m_cbRoundKind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogBmpRound, CDialog)
	//{{AFX_MSG_MAP(CDialogBmpRound)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogBmpRound ���b�Z�[�W �n���h��

BOOL CDialogBmpRound::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_cbRoundKind.AddString("�P�W�O����]");
	m_cbRoundKind.AddString("���X�O����]");
	m_cbRoundKind.AddString("�E�X�O����]");
	m_cbRoundKind.AddString("�㉺���]");
	m_cbRoundKind.AddString("���E���]");
	m_cbRoundKind.SetCurSel(2);
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDialogBmpRound::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	m_nMode = m_cbRoundKind.GetCurSel();
	CDialog::OnOK();
}
