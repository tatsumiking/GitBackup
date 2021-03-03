#if !defined(DLGGSLCT_H)
#define DLGGSLCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGSlct.h : ヘッダー ファイル
//
#define	GJTBLMAX	32
/////////////////////////////////////////////////////////////////////////////
// CDialogGaijiSlct ダイアログ

class CDialogGaijiSlct : public CDialog
{
// コンストラクション
public:
	CDialogGaijiSlct(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogGaijiSlct)
	enum { IDD = IDD_GAIJISLCT };
	CListBox	m_lstStr;
	CComboBox	m_cmbName;
	//}}AFX_DATA
	CString		m_csText[GJTBLMAX];

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogGaijiSlct)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

public:
	int		m_nSlct;
	char	m_szStr[QRECMAX];
// インプリメンテーション
public:
	void SetEndSlct(int idx);
	void UpdateEditTable(int nslct);
	void InitNameCmb();
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogGaijiSlct)
	afx_msg void OnSetfocusEdtext11();
	afx_msg void OnSetfocusEdtext12();
	afx_msg void OnSetfocusEdtext13();
	afx_msg void OnSetfocusEdtext14();
	afx_msg void OnSetfocusEdtext15();
	afx_msg void OnSetfocusEdtext16();
	afx_msg void OnSetfocusEdtext17();
	afx_msg void OnSetfocusEdtext18();
	afx_msg void OnSetfocusEdtext21();
	afx_msg void OnSetfocusEdtext22();
	afx_msg void OnSetfocusEdtext23();
	afx_msg void OnSetfocusEdtext24();
	afx_msg void OnSetfocusEdtext25();
	afx_msg void OnSetfocusEdtext26();
	afx_msg void OnSetfocusEdtext27();
	afx_msg void OnSetfocusEdtext28();
	afx_msg void OnSetfocusEdtext31();
	afx_msg void OnSetfocusEdtext32();
	afx_msg void OnSetfocusEdtext33();
	afx_msg void OnSetfocusEdtext34();
	afx_msg void OnSetfocusEdtext35();
	afx_msg void OnSetfocusEdtext36();
	afx_msg void OnSetfocusEdtext37();
	afx_msg void OnSetfocusEdtext38();
	afx_msg void OnSetfocusEdtext41();
	afx_msg void OnSetfocusEdtext42();
	afx_msg void OnSetfocusEdtext43();
	afx_msg void OnSetfocusEdtext44();
	afx_msg void OnSetfocusEdtext45();
	afx_msg void OnSetfocusEdtext46();
	afx_msg void OnSetfocusEdtext47();
	afx_msg void OnSetfocusEdtext48();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(DLGGSLCT_H)
