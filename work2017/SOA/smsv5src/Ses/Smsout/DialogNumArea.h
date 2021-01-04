
#pragma once

// DlgNumArea.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogNumArea ダイアログ

class CDialogNumArea : public CDialog
{
// コンストラクション
public:
	CDialogNumArea(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogNumArea)
	enum { IDD = IDD_INNUMAREA };
	CString	m_csENum;
	CString	m_csSNum;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogNumArea)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogNumArea)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

