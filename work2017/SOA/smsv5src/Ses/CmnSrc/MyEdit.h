
#pragma once

// MyEdit.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMyEdit ウィンドウ

#include	"../CmnSrc/MyDialog.h"
#include	"../CmnSrc/MyDialogBar.h"

class CMyEdit : public CEdit
{
// コンストラクション
public:
	CMyEdit();

	CMyDialog* m_pcMyDialog;
	CMyDialogBar* m_pcMyDialogBar;
	UINT	m_nID;
	int		m_nIMEMode;
	HIMC	m_hImc;
	HIMC	m_hOldImc;
	int		m_nFirstFlag;

	void IMEOn();
	void IMEOff();
// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMyEdit)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	//}}AFX_VIRTUAL
	void SetIMEMode(int flag);
	void SetItemID(UINT nID);
	void SetMyDialog(CMyDialog* pDlg);
	void SetMyDialogBar(CMyDialogBar* pDlgBar);

// インプリメンテーション
public:
	virtual ~CMyEdit();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMyEdit)
	afx_msg void OnKeyDown(UINT nTCHAR, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nTCHAR, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

