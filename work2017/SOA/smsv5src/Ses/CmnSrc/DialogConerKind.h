
#pragma once

// DlgCnrKd.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogConerKind ダイアログ

//C 支点設定ダイアログクラス
class CDialogConerKind : public CDialog
{
// コンストラクション
public:
	CDialogConerKind(CWnd* pParent = NULL);   // 標準のコンストラクタ
	void SetDialogTitle(LPTSTR lpTitle);

// ダイアログ データ
	//{{AFX_DATA(CDialogConerKind)
	enum { IDD = IDD_CONER_KIND };
	CComboBox	m_cbConerKind;
	CString	m_csConerKind;
	//}}AFX_DATA
	int		m_nConerKind;
	LPTSTR	m_lpTitle;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogConerKind)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogConerKind)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。


