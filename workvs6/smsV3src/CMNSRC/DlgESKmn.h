#if !defined(AFX_DIALOGESKAMON_H__C26419C4_177C_11D3_A712_00A0B0086478__INCLUDED_)
#define AFX_DIALOGESKAMON_H__C26419C4_177C_11D3_A712_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgESKmn.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogESKamon �_�C�A���O

class CDialogESKamon : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogESKamon(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogESKamon)
	enum { IDD = IDD_ESKAMON };
	CString	m_csName;
	CString	m_csFullName;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogESKamon)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogESKamon)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DIALOGESKAMON_H__C26419C4_177C_11D3_A712_00A0B0086478__INCLUDED_)
