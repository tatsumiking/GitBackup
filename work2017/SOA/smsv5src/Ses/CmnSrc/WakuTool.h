
#pragma once


#include "FontCBox.h"	// ClassView によって追加されました。

// WakuTool.h : ヘッダー ファイル
//
/////////////////////////////////////////////////////////////////////////////
// CWakuTool ウィンドウ

class CWakuTool : public CToolBar
{
// コンストラクション
public:
	CWakuTool();
	virtual ~CWakuTool();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CWakuTool)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	CFontCBox m_cFontCBox;
	CEdit m_cEditStartX;
	CSpinButtonCtrl m_cSpinStartX;
	CEdit m_cEditStartY;
	CSpinButtonCtrl m_cSpinStartY;
	CEdit m_cEditWidth;
	CSpinButtonCtrl	m_cSpinWidth;
	CEdit m_cEditHight;
	CSpinButtonCtrl m_cSpinHight;
	CEdit m_cEditMojiCount;
	CSpinButtonCtrl m_cSpinMojiCount;

	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	BOOL SetToolItem(CSize textsize);
	void TateyokoChange();
	void TateYokoFlagBottonSet(int ftategaki);
	void CntrKindChange();
	void CntrKindBottnSet(int ncntrkind);
	void StartKindChange();
	void StartKindBottonSet(int nstartkind);
	void ConerKindChange();
	void ConerKindBottonSet(int nconerkind);

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CWakuTool)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

