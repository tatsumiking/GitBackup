
#pragma once

// DlStrIns.h : ヘッダー ファイル
//
#include	"mydialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgInseart ダイアログ

class CDialogStrgInseart : public CMyDialog
{
// コンストラクション
public:
	CDialogStrgInseart(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogStrgInseart)
	enum { IDD = IDD_STRG_INSEART };
	CMyEdit	m_edInsStrg;
	BOOL	m_chkMode;
	CString	m_csInsStrg;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogStrgInseart)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogStrgInseart)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

