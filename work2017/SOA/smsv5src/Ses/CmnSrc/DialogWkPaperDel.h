
#pragma once

// DWkPpDel.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWkPaperDel �_�C�A���O

class CDialogWkPaperDel : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogWkPaperDel(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CTonView	*m_pcView;
	//{{AFX_DATA(CDialogWkPaperDel)
	enum { IDD = IDD_WAKU_PAPERDEL };
	CString	m_csPaperPath;
	CString	m_csPaperName;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogWkPaperDel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void CBInitPaperName();
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogWkPaperDel)
	afx_msg void OnDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangePapername();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

