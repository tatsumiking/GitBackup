
#pragma once

// DlEKmnLd.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogESKamonLoad �_�C�A���O

class CDialogESKamonLoad : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogESKamonLoad(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

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
	//{{AFX_DATA(CDialogESKamonLoad)
	enum { IDD = IDD_ESKMN_LOAD };
	CListBox	m_lstKamonName;
	double	m_dKmnSize;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogESKamonLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	void KamonAllDraw(CDC *pcDC);
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogESKamonLoad)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnNextpage();
	afx_msg void OnBeforpage();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeKmnlist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

