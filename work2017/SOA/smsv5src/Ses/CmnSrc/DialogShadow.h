
#pragma once

// DlgShdw.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogShadow �_�C�A���O

class CDialogShadow : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogShadow(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	~CDialogShadow();

public:
	CDataList* m_pcDataList;
	CTonView *m_pcView;
	int	m_nFirstFlag;
	int	m_nKind;
	COLORREF	m_clrShadow;
	CFont	m_fntImg;
	DBL		m_dTime;
	DBL		m_dOrgSize;
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogShadow)
	enum { IDD = IDD_SHADOW };
	CSpinButtonCtrl	m_spRoundrag;
	CSpinButtonCtrl	m_spDistance;
	double	m_dDistance;
	double	m_dRoundRag;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogShadow)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void GetRltvRect(CRect *rect);
	void SampleDataOut(CDC *pcDC);

protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogShadow)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCbshadowkind();
	virtual void OnOK();
	afx_msg void OnStclr();
	afx_msg void OnChangeEddistance();
	afx_msg void OnChangeEdroundrag();
	afx_msg void OnDeltaposSpdistance(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSproundrag(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

