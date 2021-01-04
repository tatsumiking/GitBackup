
#pragma once

// DlgWkSlct.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuSlct �_�C�A���O

class CDialogWakuSlct : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogWakuSlct(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogWakuSlct)
	enum { IDD = IDD_WAKUSLCT };
	CListBox	m_lstWkName;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogWakuSlct)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �f�[�^
public:
	CTonView	*m_pcView;
	CTonDoc		*m_pcDoc;
	int		m_nWakuCnt;
	int		m_nSlct;
	int		m_nXSize, m_nYSize;
	int		m_nListMax;
	TCHAR	m_szSetStr[RECMAX8];
	CString	m_csWakuName;
// �C���v�������e�[�V����
public:
	int LeglSlctNo(int slct);
	void DrawSlctRect(CDC *pcDC, CRect rect);
	void InitWakuNameList();
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogWakuSlct)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

