
#pragma once

// MyButton.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMyButton ウィンドウ

class CMyButton : public CButton
{
// コンストラクション
public:
	CMyButton();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMyButton)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMyButton();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMyButton)
	afx_msg void OnKeyDown(UINT nTCHAR, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nTCHAR, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

