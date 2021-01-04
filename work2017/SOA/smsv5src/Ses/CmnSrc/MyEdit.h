
#pragma once

// MyEdit.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CMyEdit �E�B���h�E

#include	"../CmnSrc/MyDialog.h"
#include	"../CmnSrc/MyDialogBar.h"

class CMyEdit : public CEdit
{
// �R���X�g���N�V����
public:
	CMyEdit();

	CMyDialog* m_pcMyDialog;
	CMyDialogBar* m_pcMyDialogBar;
	UINT	m_nID;
	int		m_nIMEMode;
	HIMC	m_hImc;
	HIMC	m_hOldImc;
	int		m_nFirstFlag;

	void IMEOn();
	void IMEOff();
// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMyEdit)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	//}}AFX_VIRTUAL
	void SetIMEMode(int flag);
	void SetItemID(UINT nID);
	void SetMyDialog(CMyDialog* pDlg);
	void SetMyDialogBar(CMyDialogBar* pDlgBar);

// �C���v�������e�[�V����
public:
	virtual ~CMyEdit();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CMyEdit)
	afx_msg void OnKeyDown(UINT nTCHAR, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nTCHAR, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

