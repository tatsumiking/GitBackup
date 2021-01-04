
#pragma once

// DWkBarSD.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBarSide �_�C�A���O

class CTonApp;
class CMainFrame;
class CTonView;
class CTonDoc;

class CDialogWakuBarSide : public CDialogBar
{
// �R���X�g���N�V����
public:
	CDialogWakuBarSide(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CTonApp*	m_pcApp;
	CMainFrame*	m_pcWnd;
	CTonView*	m_pcView;
	int		m_nFirstFlag;
	pltenvtype m_stPlotEnv;
	//{{AFX_DATA(CDialogWakuBarSide)
	enum { IDD = IDD_WAKUSIDE };
	CString	m_csPloter;
	CString	m_csPaperName;
	CString	m_csWorkXSize;
	CString	m_csWorkYSize;
	CBitmapButton m_btnCntrKind;
	//}}AFX_DATA

// �I�[�o�[���C�h
public:
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogWakuBarSide)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void ResetPlotElement();
	void InitCmbPaperName();
	void InitCmbPloter();
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void SetCrtView(CTonView * pcView);
	void SetPrint();
	void SetPaperSave();
	void SetPageSpace();
	void CenterkindChange();
protected:
	void CenterKindBottonSet(int cntrkind);
	void GetPaperName(DBL srcxmm, DBL srcymm, LPTSTR retpapername);
	void GetPaperSize(LPTSTR srcpapername, DBL *retxmm, DBL *retymm);
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogWakuBarSide)
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCmbpapername();
	afx_msg void OnSelchangeCmbploter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

