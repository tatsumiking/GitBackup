
#pragma once

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

