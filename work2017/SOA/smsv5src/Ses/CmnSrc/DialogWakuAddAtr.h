
#pragma once

// DlgWkAtr.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuAddAtr �_�C�A���O

class CDialogWakuAddAtr : public CDialog
{
// �R���X�g���N�V����
public:
	CScrollView*	m_pcView;
	HIMC m_hImc;
	CDialogWakuAddAtr(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
public:
	virtual ~CDialogWakuAddAtr();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogWakuAddAtr)
	enum { IDD = IDD_WAKUADDATR };
	CString	m_csInputStrg;
	CString	m_csMatching;
	double	m_dTrns;
	BOOL	m_nChkFixed;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogWakuAddAtr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogWakuAddAtr)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

