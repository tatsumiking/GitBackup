
#pragma once

// DialogCrctdisp.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogCrctDisp �_�C�A���O

class CDialogCrctDisp : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogCrctDisp(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogCrctDisp)
	enum { IDD = IDD_CRCTDISP };
	CListBox	m_lstCrct;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogCrctDisp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogCrctDisp)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLstCrct();
	afx_msg void OnRecDel();
	afx_msg void OnRecUp();
	afx_msg void OnRecDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CTonView*	m_pcView;
	CTonDoc*	m_pcDoc;
	int m_nSlctIdx;
	TCHAR	szTitle[FNAMEMAX];
public:
	void InitLstCrct();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

