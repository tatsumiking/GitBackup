#if !defined(AFX_DIALOGEDITBAR_H__DFA05462_1488_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DIALOGEDITBAR_H__DFA05462_1488_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DialogEditBar.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogEditBar ダイアログ

//C 文字列変更バークラス
class CDialogEditBar : public CDialogBar
{
// コンストラクション
public:
	CDialogEditBar(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogEditBar)
	enum { IDD = IDD_EDITBAR };
	CString	m_csEditStrg;
	//}}AFX_DATA


// オーバーライド
	HIMC m_hImc;
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogEditBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogEditBar)
	afx_msg void OnSetfocusEditstrg();
	afx_msg void OnChangeEditstrg();
	afx_msg void OnKillfocusEditstrg();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DIALOGEDITBAR_H__DFA05462_1488_11D2_A0A2_444553540000__INCLUDED_)
