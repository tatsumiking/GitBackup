#if !defined(AFX_DSLCTSTRG_H__C5959C43_2E32_4BA1_9DEA_F39909CE109D__INCLUDED_)
#define AFX_DSLCTSTRG_H__C5959C43_2E32_4BA1_9DEA_F39909CE109D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DSlctStrg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSlctStrg �_�C�A���O

class CDialogSlctStrg : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogSlctStrg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CTonView	*m_pcView;
	//{{AFX_DATA(CDialogSlctStrg)
	enum { IDD = IDD_SLCTSTRG };
	CListBox	m_lstStrg1;
	CListBox	m_lstStrg2;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogSlctStrg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void LoadStrg2List();
	void LoadStrg1List();

protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogSlctStrg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkLststrg1();
	afx_msg void OnDblclkLststrg2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DSLCTSTRG_H__C5959C43_2E32_4BA1_9DEA_F39909CE109D__INCLUDED_)
