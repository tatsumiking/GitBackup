
#pragma once

// DSlctFtpAdrs.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSlctFtpAdrs ダイアログ

class CDialogSlctFtpAdrs : public CDialog
{
// コンストラクション
public:
	void InitFileName();
	void InitCmbFtpFolder();
	CDialogSlctFtpAdrs(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogSlctFtpAdrs)
	enum { IDD = IDD_SLCTFTPADRS };
	CComboBox	m_cmbFtpFolder;
	CString	m_csFtpFolder;
	CString	m_csFileName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogSlctFtpAdrs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogSlctFtpAdrs)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

