
#pragma once

// DlEKmnSv.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogESKamonSave �_�C�A���O

class CDialogESKamonSave : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogESKamonSave(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	void InitKamonNameList();
	CTonApp*	m_pcApp;
	CTonView*	m_pcView;
	int	m_nDlgWidth;
	int	m_nDlgHight;
	int	m_nCrtTop;
	int m_nSelect;
	DBL	m_dTrnsTime;
	//{{AFX_DATA(CDialogESKamonSave)
	enum { IDD = IDD_ESKMN_SAVE };
	CListBox	m_lstKamonName;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogESKamonSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	void KamonAllDraw(CDC *pcDC);
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogESKamonSave)
	afx_msg void OnPaint();
	afx_msg void OnNextpage();
	afx_msg void OnBeforpage();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeKmnlist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B
