#if !defined(AFX_FONTCBOX_H__3AF35843_BA88_11D1_8684_444553540000__INCLUDED_)
#define AFX_FONTCBOX_H__3AF35843_BA88_11D1_8684_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FontCBox.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CFontCBox ウィンドウ

class CFontCBox : public CComboBox
{
// コンストラクション
public:
	CFontCBox();

// アトリビュート
public:

// オペレーション
public:

// インプリメンテーション
public:
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void SetFontList();
	void LimitText(int nMaxChar);

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CFontCBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FONTCBOX_H__3AF35843_BA88_11D1_8684_444553540000__INCLUDED_)
