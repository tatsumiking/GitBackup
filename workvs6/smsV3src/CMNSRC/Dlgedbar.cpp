// DialogEditBar.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"

#include "DlgEdBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogEditBar �_�C�A���O

extern "C" {
	void mojienvsetstring(LPSTR instrg);
	void mojienvgetstring(LPSTR instrg);
}

CDialogEditBar::CDialogEditBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CDialogEditBar)
	m_csEditStrg = _T("");
	//}}AFX_DATA_INIT
}

void CDialogEditBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEditBar)
	DDX_Text(pDX, IDC_EDITSTRG, m_csEditStrg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogEditBar, CDialogBar)
	//{{AFX_MSG_MAP(CDialogEditBar)
	ON_EN_SETFOCUS(IDC_EDITSTRG, OnSetfocusEditstrg)
	ON_EN_CHANGE(IDC_EDITSTRG, OnChangeEditstrg)
	ON_EN_KILLFOCUS(IDC_EDITSTRG, OnKillfocusEditstrg)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogEditBar ���b�Z�[�W �n���h��

void CDialogEditBar::InitItemElement()
{
	UpdateData(FALSE);
}

void CDialogEditBar::SetItemElement()
{
	char	szStr[RECMAX];

	mojienvgetstring(szStr);
	m_csEditStrg = _T(szStr);
	UpdateData(FALSE);
}

void CDialogEditBar::GetItemElement()
{
	UpdateData(TRUE);
	mojienvsetstring(m_csEditStrg.GetBuffer(RECMAX));
}

void CDialogEditBar::OnSetfocusEditstrg() 
{
	CEdit *pcEd = (CEdit*)GetDlgItem(IDC_EDITSTRG);
	m_hImc = ::ImmCreateContext();
	::ImmAssociateContext(pcEd->m_hWnd, m_hImc);
	::ImmSetOpenStatus(m_hImc, true);	
}

void CDialogEditBar::OnChangeEditstrg() 
{
	// TODO: ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A�R���g���[���́A lParam �}�X�N
	// ���ł̘_���a�� ENM_CHANGE �t���O�t���� EM_SETEVENTMASK
	// ���b�Z�[�W���R���g���[���֑��邽�߂� CDialogBar::OnInitDialog() �֐����I�[�o�[
	// ���C�h���Ȃ����肱�̒ʒm�𑗂�܂���B
	
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	
	UpdateData(TRUE);
}

void CDialogEditBar::OnKillfocusEditstrg() 
{
	CEdit* pcEd = (CEdit*)GetDlgItem(IDC_EDITSTRG);
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	::ImmAssociateContext(pcEd->m_hWnd, NULL);
	::ImmDestroyContext(m_hImc);
}

BOOL CDialogEditBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	return CDialogBar::OnSetCursor(pWnd, nHitTest, message);
}
