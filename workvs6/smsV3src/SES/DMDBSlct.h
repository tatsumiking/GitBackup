#if !defined(DMDBSLCT_H)
#define DMDBSLCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMDBSlct.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMDBSelect �_�C�A���O

// [�N���X��] CDialogMDBSelect
// [�e�N���X] CDialog
// [�N���X�T�v] �f�[�^�x�[�X�e�[�u���A�N�[���G�I���_�C�A���O�N���X
class CDialogMDBSelect : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogMDBSelect(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CString	m_csDBFname;
	CDaoDatabase* m_pcDaoDatabase;
	CString	m_csTableName;
	//{{AFX_DATA(CDialogMDBSelect)
	enum { IDD = IDD_MDBSLCT };
	CListBox	m_lstName;
	int		m_nTBLorQRY;
	//}}AFX_DATA


// �I�[�o�[���C�h
public:
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogMDBSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void InitNameList();
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogMDBSelect)
	afx_msg void OnRdtable();
	afx_msg void OnRdquery();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(DMDBSLCT_H)
