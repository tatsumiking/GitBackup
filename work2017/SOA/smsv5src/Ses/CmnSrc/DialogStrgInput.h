
#pragma once

// DlgStrIn.h : �w�b�_�[ �t�@�C��
//

#include	"../CmnSrc/MyDialog.h"
#include	"../CmnSrc/MyButton.h"
#include	"../CmnSrc/MyEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgInput �_�C�A���O

class CDialogStrgInput : public CMyDialog
{
// �R���X�g���N�V����
public:
	void SetFileToStrgText();
	void SampleTextOut();
	void InitFontList();
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void SetStrgTextToFile();
	CDialogStrgInput(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	~CDialogStrgInput();

	virtual void KeyIn(UINT nTCHAR, UINT nID);

	int		m_nFirstFlag;
	HGLOBAL hEditBuf;
	LPTSTR	lpEditBuf;
	CMyEdit* m_pcEdit;
	HIMC	m_hImc;
	CTonView*	m_pcView;

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogStrgInput)
	enum { IDD = IDD_STRG_INPUT };
	CMyButton	m_btnCancel;
	CMyButton	m_btnCreate;
	BOOL	m_fCheckGyaku;
	BOOL	m_fCheckTategaki;
	double	m_dHight;
	double	m_dMojiSpace;
	CString	m_csSmplText;
	CString	m_csFontName;
	double	m_dStartX;
	double	m_dStartY;
	double	m_dStrgLen;
	double	m_dStrgSlop;
	double	m_dStrgSpace;
	double	m_dWidth;
	CString	m_csStrgOrder;
	BOOL	m_fCheckMojiData;
	CString	m_stStrgSize;
	double	m_dTrnsLimit;
	BOOL	m_fBold;
	BOOL	m_fItric;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogStrgInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogStrgInput)
	afx_msg void OnDestroy();
	afx_msg void OnCreate();
	afx_msg void OnCkgyaku();
	afx_msg void OnSelchangeCbfontname();
	afx_msg void OnUpdateEdsmpltext();
	afx_msg void OnPaint();
	afx_msg void OnEditchangeCbfontname();
	virtual BOOL OnInitDialog();
	afx_msg void OnCkmojidata();
	afx_msg void OnEnd();
	afx_msg void OnCancel();
	afx_msg void OnInpend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

