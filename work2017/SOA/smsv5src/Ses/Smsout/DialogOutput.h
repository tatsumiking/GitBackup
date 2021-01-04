
#pragma once

// DlgOutput.h : �w�b�_�[ �t�@�C��
//

#include "CmdBase.h"
#include "CmdBaseBmp.h"
#include "CmdFtp.h"
#include "CmdEFtp.h"
/////////////////////////////////////////////////////////////////////////////
// CDialogOutput �_�C�A���O
#define	OUTLISTMAX	12
class CCrctWaku;

class CDialogOutput : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogOutput(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
public:
	int		m_nCrtTop;
	int		m_nFirst;
	CTonView	*m_pcView;
	CTonDoc		*m_pcDoc;
	COLORREF	m_clrSlct;
	CCmdFtp		*m_pcCmdFtp;
	CCmdEFtp	*m_pcCmdEFtp;
public:
	void EFTPSendEnd();
	void EFTPSendInit();
	void EFTPSendData();
	void FTPSendEnd();
	void FTPSendInit();
	void FTPSendData();
	void SaveOutCrctDat();
	void DeleteExtraSlash(LPTSTR lpBuf);
	void SetChangeElement();
	void InitChangeElement();
	void ResetElement();
	int CheckOutput();
	void OutputData(int cpycnt);
	void UpdateOutList();
	int LegalCrtTop(int crttop);
	void SetElement();
	void InitFontList();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogOutput)
	enum { IDD = IDD_OUTPUT };
	CButton	m_btnCmd13;
	CButton	m_chkDefPrinter;
	CStatic	m_stClr;
	CStatic	m_stTop;
	CComboBox	m_cmbFont;
	CString		m_csFont;
	BOOL		m_nInSort;
	CString	m_csSumCount;
	CString	m_csTitle;
	BOOL	m_nDefPrinter;
	BOOL	m_nTitleOut;
	//}}AFX_DATA
	CString	m_csOutNo[OUTLISTMAX];
	BOOL	m_nChkOut[OUTLISTMAX];
	CString	m_csPaper[OUTLISTMAX];
	CStatic	m_stOut[OUTLISTMAX];
	int		m_nOut[OUTLISTMAX];
	CString	m_csText[OUTLISTMAX];
	CString	m_csCpyCnt[OUTLISTMAX];


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogOutput)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogOutput)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtncmd1();
	afx_msg void OnBtncmd2();
	afx_msg void OnBtncmd3();
	afx_msg void OnBtncmd4();
	afx_msg void OnBtncmd5();
	afx_msg void OnBtncmd6();
	afx_msg void OnBtncmd7();
	afx_msg void OnBtncmd8();
	afx_msg void OnBtncmd9();
	afx_msg void OnBtncmd10();
	afx_msg void OnBtncmd11();
	afx_msg void OnBtncmd12();
	afx_msg void OnStclr();
	afx_msg void OnBtnnextpage();
	afx_msg void OnBtnprepage();
	afx_msg void OnPaint();
	afx_msg void OnChkdefprinter();
	afx_msg void OnBtnsizetrns();
	afx_msg void OnBtncmd13();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

