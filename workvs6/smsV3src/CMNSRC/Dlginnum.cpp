// DlgInNum.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "DlgInNum.h"

#define MODAL		0
#define	MODALESS	1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogInputNum �_�C�A���O


CDialogInputNum::CDialogInputNum(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogInputNum::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogInputNum)
	m_dInputNum = 0.0;
	//}}AFX_DATA_INIT
	m_nMode = MODAL;
	m_pcView = (CTonView*)pParent;
}


void CDialogInputNum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogInputNum)
	DDX_Control(pDX, IDC_EDNUM, m_edInputNum);
	DDX_Control(pDX, IDOK, m_cbtnOK);
	DDX_Control(pDX, IDCANCEL, m_cbtnCancel);
	DDX_Control(pDX, IDC_STUNIT, m_stUnit);
	DDX_Control(pDX, IDC_STTITLE, m_stTitle);
	DDX_Text(pDX, IDC_EDNUM, m_dInputNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogInputNum, CDialog)
	//{{AFX_MSG_MAP(CDialogInputNum)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogInputNum ���b�Z�[�W �n���h��

void CDialogInputNum::SetDialogTitle(LPSTR lpTitle)
{
	m_lpTitle = lpTitle;
}

void CDialogInputNum::SetUnitText(LPSTR lpUnitText)
{
	m_lpUnitText = lpUnitText;
}

void CDialogInputNum::SetTitleText(LPSTR lpTitleText)
{
	m_lpTitleText = lpTitleText;
}

BOOL CDialogInputNum::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText(m_lpTitle);
	m_stUnit.SetWindowText(m_lpUnitText);
	m_stTitle.SetWindowText(m_lpTitleText);
	if(m_nMode == MODALESS){
		m_cbtnOK.ShowWindow(SW_HIDE);
		//SetDlgItemText(IDCANCEL, "���f");
	}
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDialogInputNum::SetModaless()
{
	m_nMode = MODALESS;
}

void CDialogInputNum::SetModal()
{
	m_nMode = MODAL;
}

void CDialogInputNum::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
	if(m_nMode == MODAL){
		CDialog::OnCancel();
	}
	else{
		m_pcView->CommandExec(DLGRESETEVENT, 0, 0);
	}
}

void CDialogInputNum::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	if(m_nMode == MODAL){
		CDialog::OnOK();
	}
	else{
		m_pcView->CommandExec(DLGSETEVENT, 0, 0);
	}
}

void CDialogInputNum::GetItemElement()
{
	UpdateData(TRUE);
}

void CDialogInputNum::SetItemElement()
{
	UpdateData(FALSE);
}

void CDialogInputNum::OnPaint() 
{
	CPaintDC dc(this);	

	if(m_nFirst == TRUE){
		m_nFirst = FALSE;
		m_edInputNum.SetSel(0, -1);
		m_edInputNum.SetFocus();
	}
}
