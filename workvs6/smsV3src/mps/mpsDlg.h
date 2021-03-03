// mpsDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(MPSDLG_H)
#define MPSDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\LEAD10\lead.h"	// ClassView �ɂ���Ēǉ�����܂����B

/////////////////////////////////////////////////////////////////////////////
// CTonDlg �_�C�A���O

class CTonDlg : public CDialog
{
// �\�z
public:
	CTonDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^
	~CTonDlg();
// �_�C�A���O �f�[�^
public:
	CLead	*m_pcLead;		// LEAD�c�[���p�n���h��
    CLTDlgOCXCtl *m_pcLeadDlg;		// LEAD�c�[���_�C�A���O�p�n���h��
	char	m_szExePath[FNAMEMAX];
	char	m_szTmpPath[FNAMEMAX];
	char	m_szTitle[RECMAX];
	//{{AFX_DATA(CTonDlg)
	enum { IDD = IDD_MPS_DIALOG };
	CStatic	m_stImg;
	CStatic	m_stTop;
	CString	m_csDstYMM;
	CString	m_csDstXMM;
	CString	m_csSrcYDot;
	CString	m_csSrcXDot;
	CString	m_csFileName;
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void InitEnvPath();
	void InitExePath();
	void GetLegalRect(CRect *pcRect);
	void SelecttionBmp();
	int GetParentPath(LPSTR lpPath);
	void BmpCmpConvBytelen(LPSTR lpBmpFName, LPSTR lpCmpFName);
	void CmpBmpConvBytelen(LPSTR lpCmpFName, LPSTR lpBmpFName);
	BOOL IsKanji(UINT hc);
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTonDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnsend();
	afx_msg void OnBtnref();
	afx_msg void OnBtnend();
	afx_msg void OnBtnreload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(MPSDLG_H)
