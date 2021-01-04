
#pragma once

// DlgWkEdt.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuEdit �_�C�A���O

class CTonView;

class CDialogWakuEdit : public CMyDialog
{
// �R���X�g���N�V����
public:
	CDialogWakuEdit(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

public:
	HIMC	m_hImc;
	int		m_nTateYoko;
	CTonView*	m_pcView;
	int		m_nChangeEditID;

public:
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void InitFontList();
	void InitPlotName();
	void InitPaperName();
	void GetPaperSize(LPTSTR papername, DBL *mmxsize, DBL *mmysize);
	void CntrKindBottonSet(int cntrkind);

	virtual void KeyIn(UINT nTCHAR, UINT nID);

private:

public:
	void PlotElementGet(LPTSTR SrchPlotName);
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogWakuEdit)
	enum { IDD = IDD_WAKU_EDIT };
	CEdit	m_edMojiSpace;
	CBitmapButton	m_btnCntrKind;
	CMyEdit	m_edWorkYSize;
	CMyEdit	m_edWorkXSize;
	CMyEdit	m_edWidth;
	CMyEdit	m_edHight;
	CMyEdit	m_edText;
	CButton	m_btnTategaki;
	CComboBox	m_cmbPaperName;
	CComboBox	m_cmbPloter;
	CComboBox	m_cmbFont;
	double	m_dHight;
	CString	m_csText;
	double	m_dWidth;
	double	m_dWorkXSize;
	double	m_dWorkYSize;
	CString	m_csFont;
	CString	m_csPloter;
	CString	m_csPaperName;
	BOOL	m_nSpaceKind;
	double	m_dMojiSpace;
	BOOL	m_nYokoMoji;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogWakuEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogWakuEdit)
	afx_msg void OnBtntategaki();
	afx_msg void OnBtnoption();
	afx_msg void OnBtnpapersave();
	afx_msg void OnBtnprint();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbpapername();
	afx_msg void OnBtnset();
	afx_msg void OnBtnreset();
	afx_msg void OnChangeEdworkxsize();
	afx_msg void OnChangeEdworkysize();
	afx_msg void OnChangeEdhight();
	afx_msg void OnChangeEdwidth();
	afx_msg void OnChangeEdtext();
	afx_msg void OnCntrkind();
	afx_msg void OnSelchangeCmbploter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

