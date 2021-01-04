
#pragma once

// dwkinskj.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuInSekiji �_�C�A���O
#define	INSEKIJIMAX	10

class CTonView;

class CDialogWakuInSekiji : public CMyDialog
{
// �R���X�g���N�V����
public:
	CDialogWakuInSekiji(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogWakuInSekiji)
	enum { IDD = IDD_WAKU_INSEKIJI };
	//}}AFX_DATA

	CDataExchange *m_pDE;
	CStatic	m_stWkno[INSEKIJIMAX];
	CButton	m_chkKtgk[INSEKIJIMAX];
	CMyEdit	m_edText[INSEKIJIMAX*3];

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogWakuInSekiji)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogWakuInSekiji)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnOut();
	afx_msg void OnDisp();
	afx_msg void OnGaiji();
	afx_msg void OnExit();
	afx_msg void OnPaint();
	afx_msg void OnPrev();
	afx_msg void OnNext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
public:
	CTonView*	m_pcView;
	CTonDoc*	m_pcDoc;
	CCmdBase	*m_pcCmdBase;
	int		m_nCrtIdxTop;
	BOOL	m_fFirstFlag;
	int		m_nGrupMax;
	int		m_nNextFocus;
public:
	void SetCmdBase(CCmdBase *pcCmdBae);
	void SetString(LPTSTR LPTSTR);
	virtual void KeyIn(UINT nTCHAR, UINT nID);
	virtual void SelectEdit(UINT nID);
	void WakuSetFocus(int no);
	void GetGrupMax();
	void SaveCrtGrupText();
	void LoadCrtGrupText();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

