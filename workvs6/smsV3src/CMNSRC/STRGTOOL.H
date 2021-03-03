#if !defined(AFX_STRGTOOL_H__3AF35842_BA88_11D1_8684_444553540000__INCLUDED_)
#define AFX_STRGTOOL_H__3AF35842_BA88_11D1_8684_444553540000__INCLUDED_

#include "FontCBox.h"	// ClassView によって追加されました。

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// StrgTool.h : ヘッダー ファイル
//
/////////////////////////////////////////////////////////////////////////////
// CStrgTool ウィンドウ

class CStrgTool : public CToolBar
{
// コンストラクション
public:
	CStrgTool();
	virtual ~CStrgTool();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CStrgTool)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	CFontCBox m_cFontCBox;
	CEdit m_cEditFontSize;
	CSpinButtonCtrl	m_cSpinFontSize;
	CEdit m_cEditFlatPar;
	CSpinButtonCtrl m_cSpinFlatPar;
	CEdit m_cEditMojiSpace;
	CSpinButtonCtrl m_cSpinMojiSpace;
	CEdit m_cEditStrgSpace;
	CSpinButtonCtrl m_cSpinStrgSpace;
	CEdit m_cEditStrgSize;
	CSpinButtonCtrl m_cSpinStrgSize;

	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	BOOL SetToolItem(CSize textsize);
	void TateyokoChange();
	void SpacekindChange();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CStrgTool)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STRGTOOL_H__3AF35842_BA88_11D1_8684_444553540000__INCLUDED_)
