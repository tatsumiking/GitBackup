
#pragma once

// DSlctMAdrs.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSlctMailAdrs ダイアログ

class CDialogSlctMailAdrs : public CDialog
{
// コンストラクション
public:
	CDialogSlctMailAdrs(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CString		m_csAdrs;
	//{{AFX_DATA(CDialogSlctMailAdrs)
	enum { IDD = IDD_SLCTADRS };
	CComboBox	m_cmbMailAdrs;
	CString	m_csSubject;
	CString	m_csMailAdrs;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogSlctMailAdrs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void InitCmbMailAdrs();
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogSlctMailAdrs)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

