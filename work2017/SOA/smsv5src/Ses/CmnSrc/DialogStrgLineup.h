
#pragma once

// DlgStLup.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgLineup ダイアログ

class CDialogStrgLineup : public CDialog
{
// コンストラクション
public:
	CDialogStrgLineup(CWnd* pParent = NULL);   // 標準のコンストラクタ

	int	m_nSlctMode;
// ダイアログ データ
	//{{AFX_DATA(CDialogStrgLineup)
	enum { IDD = IDD_STRG_LINEUP };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogStrgLineup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogStrgLineup)
	afx_msg void OnMode1();
	afx_msg void OnMode2();
	afx_msg void OnMode3();
	afx_msg void OnMode4();
	afx_msg void OnMode5();
	afx_msg void OnMode6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

