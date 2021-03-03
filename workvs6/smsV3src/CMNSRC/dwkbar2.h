#if !defined(DWKBAR2_H)
#define DWKBAR2_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DWkBar2.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBar2 �_�C�A���O

class CDialogWakuBar2 : public CDialogBar
{
// �R���X�g���N�V����
public:
	CDialogWakuBar2(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CTonApp*	m_pcApp;
	CTonView*	m_pcView;
	CTonDoc*	m_pcDoc;
	int		m_nFirstFlag;
	//{{AFX_DATA(CDialogWakuBar2)
	enum { IDD = IDD_WAKUBAR2 };
	CString	m_csFont;
	CString	m_csFlatPar;
	CString	m_csSpaceMM;
	CString	m_csOderKind;
	CString	m_csKanaPar;
	CString	m_csHanten;
	CString	m_csMojiCount;
	CString	m_csFildName;
	//}}AFX_DATA


// �I�[�o�[���C�h
public:
	//{{AFX_VIRTUAL(CDialogWakuBar2)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void SetCrtView(CTonView * pcView);
	void InitCmbFont();
	void InitCmbFildName();
	void InitCmbFlatPar();
	void InitCmbSpace();
	void InitCmbOderKind();
	void InitCmbKanaPar();
	void InitCmbHanten();
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogWakuBar2)
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCmbfont();
	afx_msg void OnSelchangeCmbflatpar();
	afx_msg void OnSelchangeCmbhanten();
	afx_msg void OnSelchangeCmbkanapar();
	afx_msg void OnSelchangeCmboderkind();
	afx_msg void OnSelchangeCmbspacemm();
	afx_msg void OnBtnspacekind();
	afx_msg void OnChkfixed();
	afx_msg void OnChkyokogumi();
	afx_msg void OnSelchangeCmbfild();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(DWKBAR2_H)

