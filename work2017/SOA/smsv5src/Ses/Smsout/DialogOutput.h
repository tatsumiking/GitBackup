
#pragma once

// DlgOutput.h : ヘッダー ファイル
//

#include "CmdBase.h"
#include "CmdBaseBmp.h"
#include "CmdFtp.h"
#include "CmdEFtp.h"
/////////////////////////////////////////////////////////////////////////////
// CDialogOutput ダイアログ
#define	OUTLISTMAX	12
class CCrctWaku;

class CDialogOutput : public CDialog
{
// コンストラクション
public:
	CDialogOutput(CWnd* pParent = NULL);   // 標準のコンストラクタ
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

// ダイアログ データ
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


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogOutput)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
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
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

