// DSetMacKey.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "DSetMacKey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSetMacKey �_�C�A���O


CDialogSetMacKey::CDialogSetMacKey(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetMacKey::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSetMacKey)
	m_csChkKey = _T("");
	m_csSubKey = _T("");
	m_csMsg = _T("");
	//}}AFX_DATA_INIT
}


void CDialogSetMacKey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSetMacKey)
	DDX_Text(pDX, IDC_EDCHKKEY, m_csChkKey);
	DDX_Text(pDX, IDC_EDSUBKEY, m_csSubKey);
	DDX_Text(pDX, IDC_STMSG, m_csMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSetMacKey, CDialog)
	//{{AFX_MSG_MAP(CDialogSetMacKey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSetMacKey ���b�Z�[�W �n���h��

void CDialogSetMacKey::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	
	CDialog::OnOK();
}
