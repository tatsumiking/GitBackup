#if !defined(AFX_MYSTATIC_H__CF492861_825B_11D4_A9A4_00605205F27C__INCLUDED_)
#define AFX_MYSTATIC_H__CF492861_825B_11D4_A9A4_00605205F27C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyStatic.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMyStatic ウィンドウ

class CMyStatic : public CStatic
{
// コンストラクション
public:
	CMyStatic();

// アトリビュート
public:
	int		m_nFlag;
// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMyStatic)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	int CheckFlag();
	void SetFlag(int flag);
	virtual ~CMyStatic();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMyStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MYSTATIC_H__CF492861_825B_11D4_A9A4_00605205F27C__INCLUDED_)
