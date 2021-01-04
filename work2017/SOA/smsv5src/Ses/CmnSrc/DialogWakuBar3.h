
#pragma once

// DWkBar3.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBar3 �_�C�A���O

class CTonApp;
class CMainFrame;
class CTonView;
class CTonDoc;

class CDialogWakuBar3 : public CDialogBar
{
// �R���X�g���N�V����
public:
	CDialogWakuBar3(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CTonApp*	m_pcApp;
	CMainFrame*	m_pcWnd;
	CTonView*	m_pcView;
	int		m_nFirstFlag;
	HIMC	m_hImc;
	int		m_nShadowKind;
	COLORREF	m_clrShadow;
	DBL		m_dShadowDistance;
	DBL		m_dShadowRoundRag;
	//{{AFX_DATA(CDialogWakuBar3)
	enum { IDD = IDD_WAKUBAR3 };
	CButton	m_btnTateYoko;
	CString	m_csSizeX;
	CString	m_csSizeY;
	CString	m_csStartX;
	CString	m_csStartY;
	CBitmapButton	m_btnStart;
	CBitmapButton	m_btnConer;
	CString	m_csInText;
	BOOL	m_nShadow;
	//}}AFX_DATA

// �I�[�o�[���C�h
public:
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogWakuBar3)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void SetLocate();
	void GetLocate();
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void SetCrtView(CTonView * pcView);
	void TateyokoChange();
	void StartKindChange();
	void ConerKindChange();
protected:
	void TateYokoFlagBottonSet(int ftategaki);
	void StartKindBottonSet(int nstartkind);
	void ConerKindBottonSet(int nconerkind);
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogWakuBar3)
	afx_msg void OnPaint();
	afx_msg void OnSetfocusEdintext();
	afx_msg void OnKillfocusEdintext();
	afx_msg void OnChkShadow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

