#if !defined(DSETPROTECT_H)
#define DSETPROTECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DSetProtect.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSetProtect �_�C�A���O

class CDialogSetProtect : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogSetProtect(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogSetProtect)
	enum { IDD = IDD_SETPROTECT };
	CString	m_csPCID;
	CString	m_csSetKey;
	CString	m_csMsg;
	CString	m_csMsg2;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogSetProtect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogSetProtect)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(DSETPROTECT_H)
