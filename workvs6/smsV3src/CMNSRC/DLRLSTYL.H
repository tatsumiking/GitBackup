#if !defined(AFX_DLRLSTYL_H__EFF6BA61_B795_11D2_B60B_00A0B0086478__INCLUDED_)
#define AFX_DLRLSTYL_H__EFF6BA61_B795_11D2_B60B_00A0B0086478__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlRlStyl.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogRuleStyle ダイアログ

class CDialogRuleStyle : public CDialog
{
// コンストラクション
public:
	void GetItemElement();
	void InitStyleList();
	CDialogRuleStyle(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogRuleStyle)
	enum { IDD = IDD_RULESTYLE };
	CComboBox	m_cbStyle;
	CString	m_csStyle;
	//}}AFX_DATA

	int	m_nStyle;
	CTonView*	m_pcView;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogRuleStyle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogRuleStyle)
	afx_msg void OnReset();
	afx_msg void OnSelchangeCbstyle();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLRLSTYL_H__EFF6BA61_B795_11D2_B60B_00A0B0086478__INCLUDED_)
