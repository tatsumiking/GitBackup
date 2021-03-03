// DlgDirSt.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "DlgDirSl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDirSlct �_�C�A���O


CDialogDirSlct::CDialogDirSlct(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDirSlct::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDirSlct)
	m_csSlctDir = _T("");
	//}}AFX_DATA_INIT
}

void CDialogDirSlct::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDirSlct)
	DDX_Text(pDX, IDC_STSLCTNAME, m_csSlctDir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogDirSlct, CDialog)
	//{{AFX_MSG_MAP(CDialogDirSlct)
	ON_LBN_SELCHANGE(IDC_DIRLIST, OnSelchangeDirlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDirSlct ���b�Z�[�W �n���h��

BOOL CDialogDirSlct::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	
	DirlistUpdate();
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDialogDirSlct::OnSelchangeDirlist() 
{
	DlgDirSelect(m_szPathName, IDC_DIRLIST);
	DirlistUpdate();
}

void CDialogDirSlct::DirlistUpdate()
{
	char	szSrchPath[RECMAX];

	sprintf(szSrchPath, "%s\\*.*", m_szPathName);
	DlgDirList(szSrchPath, IDC_DIRLIST, IDC_STSLCTNAME, 0xC010);
}

void CDialogDirSlct::SetInitPath(LPSTR path)
{
	strcpy(m_szPathName, path);
}