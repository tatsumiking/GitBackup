#if !defined(AFX_DSLCTFTPADRS_H__65E67082_7F84_42C4_8B55_58255EA64A67__INCLUDED_)
#define AFX_DSLCTFTPADRS_H__65E67082_7F84_42C4_8B55_58255EA64A67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DSlctFtpAdrs.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSlctFtpAdrs �_�C�A���O

class CDialogSlctFtpAdrs : public CDialog
{
// �R���X�g���N�V����
public:
	void InitFileName();
	void InitCmbFtpFolder();
	CDialogSlctFtpAdrs(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogSlctFtpAdrs)
	enum { IDD = IDD_SLCTFTPADRS };
	CComboBox	m_cmbFtpFolder;
	CString	m_csFtpFolder;
	CString	m_csFileName;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogSlctFtpAdrs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogSlctFtpAdrs)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DSLCTFTPADRS_H__65E67082_7F84_42C4_8B55_58255EA64A67__INCLUDED_)
