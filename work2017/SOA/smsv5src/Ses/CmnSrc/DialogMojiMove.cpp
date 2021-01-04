// DlgMjMov.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "DialogMojiMove.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiMove �_�C�A���O


CDialogMojiMove::CDialogMojiMove(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMojiMove::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMojiMove)
	m_edStepmm = 0.0;
	//}}AFX_DATA_INIT
	m_pcView = (CTonView*)pParent;
	m_btnLeft.LoadBitmaps(IDB_MOVELEFT1, IDB_MOVELEFT2, IDB_MOVELEFT3);
	m_btnRight.LoadBitmaps(IDB_MOVERIGHT1, IDB_MOVERIGHT2, IDB_MOVERIGHT3);
	m_btnUp.LoadBitmaps(IDB_MOVEUP1, IDB_MOVEUP2, IDB_MOVEUP3);
	m_btnDown.LoadBitmaps(IDB_MOVEDOWN1, IDB_MOVEDOWN2, IDB_MOVEDOWN3);
}


void CDialogMojiMove::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMojiMove)
	DDX_Text(pDX, IDC_ED_STEP, m_edStepmm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMojiMove, CDialog)
	//{{AFX_MSG_MAP(CDialogMojiMove)
	ON_BN_CLICKED(IDC_DLGRESET, OnDlgReset)
	ON_BN_CLICKED(IDC_DLGSET, OnDlgSet)
	ON_BN_CLICKED(IDC_BTN_MOVEDOWN, OnBtnMovedown)
	ON_BN_CLICKED(IDC_BTN_MOVELEFT, OnBtnMoveleft)
	ON_BN_CLICKED(IDC_BTN_MOVERIGHT, OnBtnMoveright)
	ON_BN_CLICKED(IDC_BTN_MOVEUP, OnBtnMoveup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiMove ���b�Z�[�W �n���h��

BOOL CDialogMojiMove::OnInitDialog() 
{
	CDialog::OnInitDialog();

	VERIFY(m_btnLeft.SubclassDlgItem(IDC_BTN_MOVELEFT, this));
	VERIFY(m_btnRight.SubclassDlgItem(IDC_BTN_MOVERIGHT, this));
	VERIFY(m_btnUp.SubclassDlgItem(IDC_BTN_MOVEUP, this));
	VERIFY(m_btnDown.SubclassDlgItem(IDC_BTN_MOVEDOWN, this));
	m_btnLeft.SizeToContent();
	m_btnRight.SizeToContent();
	m_btnUp.SizeToContent();
	m_btnDown.SizeToContent();
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDialogMojiMove::OnDlgReset() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	m_pcView->CommandExec(DLGRESETEVENT, 0, 0);
}

void CDialogMojiMove::OnDlgSet() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	m_pcView->CommandExec(DLGSETEVENT, 0, 0);
}

void CDialogMojiMove::GetItemElement()
{
	UpdateData(TRUE);
}

void CDialogMojiMove::SetItemElement()
{
	UpdateData(FALSE);
}

void CDialogMojiMove::OnBtnMoveleft() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	m_pcView->CommandExec(USER1EVENT, 0, 0);
}

void CDialogMojiMove::OnBtnMoveright() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	m_pcView->CommandExec(USER2EVENT, 0, 0);
}

void CDialogMojiMove::OnBtnMoveup() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	m_pcView->CommandExec(USER3EVENT, 0, 0);
}

void CDialogMojiMove::OnBtnMovedown() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	m_pcView->CommandExec(USER4EVENT, 0, 0);
}

