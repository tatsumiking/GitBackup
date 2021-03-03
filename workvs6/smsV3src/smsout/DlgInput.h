#if !defined(DLGINPUT_H)
#define DLGINPUT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInput.h : ヘッダー ファイル
//
#include	"..\cmnsrc\imehndl.h"
#include	"..\cmnsrc\cmd_base.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogInput ダイアログ
#define	WAKUMAX		300
#define INWAKUMAX	10

class CDialogInput : public CDialog
{
// コンストラクション
public:
	CDialogInput(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogInput)
	enum { IDD = IDD_INPUT };
	CStatic	m_stImage;
	CStatic	m_stTop;
	CComboBox	m_cmbPaper;
	CListBox	m_lstWkSlct;
	CString	m_csPaperName;
	//}}AFX_DATA
	CString	m_csWkno[INWAKUMAX];
	CString	m_csWakuStr[INWAKUMAX];
	CEdit	m_edWakuStr[INWAKUMAX];


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogInput)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// データー
public:
	CTonView	*m_pcView;
	CTonDoc		*m_pcDoc;
	int		m_nFirst;
	int		m_nNewCrct;
	int		m_nCrtTop;
	int		m_nSlctIdx;
	char	m_szWakuText[WAKUMAX+1][RECMAX];
	CImeHndl	cImeHndl;
	int		m_nCrtWakuMax;
	char	m_szCrtWakuName[FNAMEMAX];
	CCmdBase *m_pcCmdBase;
// インプリメンテーション
public:
	void CallImageDisp();
	int LegalCrtTop(int crttop);
	void SaveCrctData();
	void SlctCrtWaku();
	void GetLastWakuNo();
	void AllWakuSetString();
	void SetWakuString(int top);
	void ResetWakuString();
	int CheckSameWaku();
	void MoveNextEdit();
	void KillFocusEdit();
	void SetFocusEdit();
	void UpdateInWakuString(int num);
	void InitCmbPaper();
	void InitLstWkSlct();

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogInput)
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
	afx_msg void OnSelchangeCmbpaper();
	afx_msg void OnSelchangeLstwkslct();
	afx_msg void OnSetfocusEdwakustr1();
	afx_msg void OnSetfocusEdwakustr2();
	afx_msg void OnSetfocusEdwakustr3();
	afx_msg void OnSetfocusEdwakustr4();
	afx_msg void OnSetfocusEdwakustr5();
	afx_msg void OnSetfocusEdwakustr6();
	afx_msg void OnSetfocusEdwakustr7();
	afx_msg void OnSetfocusEdwakustr8();
	afx_msg void OnSetfocusEdwakustr9();
	afx_msg void OnSetfocusEdwakustr10();
	afx_msg void OnKillfocusEdwakustr1();
	afx_msg void OnKillfocusEdwakustr2();
	afx_msg void OnKillfocusEdwakustr3();
	afx_msg void OnKillfocusEdwakustr4();
	afx_msg void OnKillfocusEdwakustr5();
	afx_msg void OnKillfocusEdwakustr6();
	afx_msg void OnKillfocusEdwakustr7();
	afx_msg void OnKillfocusEdwakustr8();
	afx_msg void OnKillfocusEdwakustr9();
	afx_msg void OnKillfocusEdwakustr10();
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(DLGINPUT_H)
