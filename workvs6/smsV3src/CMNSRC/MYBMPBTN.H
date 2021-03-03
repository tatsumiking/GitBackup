#if !defined(AFX_MYBMPBTN_H__6FCE9D61_3775_11D4_9F64_0000E835797C__INCLUDED_)
#define AFX_MYBMPBTN_H__6FCE9D61_3775_11D4_9F64_0000E835797C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyBmpBtn.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMyBitmapButton ウィンドウ

class CMyBitmapButton : public CBitmapButton
{
// コンストラクション
public:
	CMyBitmapButton();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMyBitmapButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	int CheckCheck();
	void OnCheck();
	void OffCheck();
	void NonsetCheck();
	void ChangeCheck();
	virtual ~CMyBitmapButton();
	int		m_nCheck;
	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMyBitmapButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MYBMPBTN_H__6FCE9D61_3775_11D4_9F64_0000E835797C__INCLUDED_)
