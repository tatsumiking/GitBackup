// DlgWkAtr.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "DialogWakuAddAtr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuAddAtr �_�C�A���O


CDialogWakuAddAtr::CDialogWakuAddAtr(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWakuAddAtr::IDD, pParent)
{
	m_pcView = (CScrollView*)pParent;
	//{{AFX_DATA_INIT(CDialogWakuAddAtr)
	m_csInputStrg = _T("");
	m_csMatching = _T("");
	m_dTrns = 0.0;
	//}}AFX_DATA_INIT
	// CG: ���̍s�� �uIME �T�|�[�g�v �R���|�[�l���g�ɂ��ǉ�����Ă��܂��B
	m_hImc = ::ImmCreateContext();
}

CDialogWakuAddAtr::~CDialogWakuAddAtr()
{
	// CG: ���̍s�� �uIME �T�|�[�g�v�R���|�[�l���g�ɂ��ǉ�����Ă��܂��B
	::ImmAssociateContext(m_hWnd, NULL);
	::ImmDestroyContext(m_hImc);
}

void CDialogWakuAddAtr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuAddAtr)
	DDX_Text(pDX, IDC_INSTRG, m_csInputStrg);
	DDX_Text(pDX, IDC_MATCHING, m_csMatching);
	DDX_Text(pDX, IDC_TRNS, m_dTrns);
	DDX_Check(pDX, IDC_CHKFIXED, m_nChkFixed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogWakuAddAtr, CDialog)
	//{{AFX_MSG_MAP(CDialogWakuAddAtr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuAddAtr ���b�Z�[�W �n���h��


BOOL CDialogWakuAddAtr::OnInitDialog() 
{
	CRect	rect;
	int		sx, sy, sizex, sizey;

	CDialog::OnInitDialog();

	GetWindowRect(&rect);
	sx = rect.left;
	sy = rect.top;
	sizex = rect.Width();
	sizey = rect.Height();
	::SetWindowPos(m_hWnd, HWND_TOPMOST, sx, sy, sizex, sizey, 0);
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	UpdateData(FALSE);
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDialogWakuAddAtr::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	UpdateData(TRUE);
	CDialog::OnOK();
}

