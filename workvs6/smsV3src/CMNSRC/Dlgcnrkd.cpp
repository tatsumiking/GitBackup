// DlgCnrKd.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "DlgCnrKd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogConerKind �_�C�A���O
extern "C" {
}

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
// CDialogConerKind ���b�Z�[�W �n���h��

void CDialogConerKind::SetDialogTitle(LPSTR lpTitle)
{
	m_lpTitle = lpTitle;
}

BOOL CDialogConerKind::OnInitDialog() 
{
	char	buf[RECMAX];

	CDialog::OnInitDialog();
	SetWindowText(m_lpTitle);
	sprintf(buf, "�x�_����         %d", LEFTDOWN);
	m_cbConerKind.AddString(buf);
	if(m_nConerKind == LEFTDOWN){
		m_csConerKind = _T(buf);
	}

	sprintf(buf, "�x�_����         %d", LEFTUP);
	m_cbConerKind.AddString(buf);
	if(m_nConerKind == LEFTUP){
		m_csConerKind = _T(buf);
	}

	sprintf(buf, "�x�_�E��         %d", RIGHTUP);
	m_cbConerKind.AddString(buf);
	if(m_nConerKind == RIGHTUP){
		m_csConerKind = _T(buf);
	}

	sprintf(buf, "�x�_�E��         %d", RIGHTDOWN);
	m_cbConerKind.AddString(buf);
	if(m_nConerKind == RIGHTDOWN){
		m_csConerKind = _T(buf);
	}

	sprintf(buf, "�x�_����         %d", CNTRCNTR);
	m_cbConerKind.AddString(buf);
	if(m_nConerKind == CNTRCNTR){
		m_csConerKind = _T(buf);
	}
	
	UpdateData(FALSE);
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDialogConerKind::OnOK() 
{
	char	buf[RECMAX];
	char	tmp[QRECMAX];

	UpdateData(TRUE);
	strcpy(buf, m_csConerKind.GetBuffer(RECMAX));	
	sscanf(buf, "%s %d", tmp, &m_nConerKind);
	CDialog::OnOK();
}
