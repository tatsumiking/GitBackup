//{{AFX_INCLUDES()
#include "..\protekit\prtctl.h"
//}}AFX_INCLUDES
#if !defined(DPROTECT_H_)
#define DPROTECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DProtect.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogProtect �_�C�A���O

class CDialogProtect : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogProtect(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	int		m_nDelProtect;
	char	m_szPassword[QRECMAX];
	int		m_nStat;
	int		m_nDays;

	//{{AFX_DATA(CDialogProtect)
	enum { IDD = IDD_PROTECT };
	CPrtCtl	m_prtCtl;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogProtect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogProtect)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(DPROTECT_H)
