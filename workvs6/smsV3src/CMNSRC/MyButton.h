#if !defined(AFX_MYBUTTON_H__0D2F6505_1E16_11D3_9F64_0000E835797C__INCLUDED_)
#define AFX_MYBUTTON_H__0D2F6505_1E16_11D3_9F64_0000E835797C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
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
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MYBUTTON_H__0D2F6505_1E16_11D3_9F64_0000E835797C__INCLUDED_)
