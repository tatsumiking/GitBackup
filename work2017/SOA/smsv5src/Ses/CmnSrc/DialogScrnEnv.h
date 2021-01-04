
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDialogScrnEnv �_�C�A���O

class CDialogScrnEnv : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogScrnEnv(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CWnd* m_pcParent;
	//{{AFX_DATA(CDialogScrnEnv)
	enum { IDD = IDD_SCRNENV };
	CComboBox	m_cbPaperSize;
	CString	m_csKmonPath;
	CString	m_csCntrKind;
	CString	m_csDataPath;
	CString	m_csFontPath;
	CString	m_csPlotName;
	DBL		m_dPlotStep;
	DBL		m_dPlotSx;
	DBL		m_dPlotSy;
	DBL		m_dPlotXSize;
	DBL		m_dPlotYSize;
	CString	m_csOutDPath;
	CString	m_csPortName;
	CString	m_csTBMPPath;
	CString	m_csTempPath;
	CString	m_csWakuPath;
	DBL		m_dWorkXSize;
	DBL		m_dWorkYSize;
	CString	m_csDeepCut;
	CString	m_csAreaDot;
	CString	m_csPointDot;
	CString	m_csPartPath;
	//}}AFX_DATA
	pltenvtype m_stPlotEnv;
	int		m_nPlotType;
	int		m_fPlotArc;
	int		m_fPlotFill;

// �I�[�o�[���C�h
public:
	static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CDialogScrnEnv)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void CBInitPaperSize();
	void GetSelectDir(LPTSTR lpTitle, LPTSTR lpPath);
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogScrnEnv)
	afx_msg void OnSelchangePlotname();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnBtmp();
	afx_msg void OnBtnData();
	afx_msg void OnBtnFont();
	afx_msg void OnBtnKmn();
	afx_msg void OnBtnOut();
	afx_msg void OnBtnTemp();
	afx_msg void OnBtnWaku();
	afx_msg void OnPlotsize();
	afx_msg void OnSelchangePapersize();
	afx_msg void OnBtnPart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void PlotElementGet(LPTSTR PlotName);
	void CntrKindToStrg(int kind, TCHAR *buf);
	void CntrStrgToKind(int *kind, TCHAR *buf);
	void CBInitPortName();
	void CBInitPlotName();
	void CBInitCntrKind();
};

