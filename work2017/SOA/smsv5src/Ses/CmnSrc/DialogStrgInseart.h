
#pragma once

// DlStrIns.h : �w�b�_�[ �t�@�C��
//
#include	"mydialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgInseart �_�C�A���O

class CDialogStrgInseart : public CMyDialog
{
// �R���X�g���N�V����
public:
	CDialogStrgInseart(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogStrgInseart)
	enum { IDD = IDD_STRG_INSEART };
	CMyEdit	m_edInsStrg;
	BOOL	m_chkMode;
	CString	m_csInsStrg;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogStrgInseart)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogStrgInseart)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

