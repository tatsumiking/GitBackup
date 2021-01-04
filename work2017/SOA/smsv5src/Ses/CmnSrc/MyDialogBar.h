
#pragma once

// MyDlgBar.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMyDialogBar ダイアログ

class CMyDialogBar : public CDialogBar
{
// コンストラクション
public:
	virtual void SelectEdit(UINT nID);
	virtual void KeyIn(UINT nTCHAR, UINT nID);
	CMyDialogBar(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CMyDialogBar)
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CMyDialogBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMyDialogBar)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

