#if !defined(AFX_DDIVPRNT_H__1558B3C1_646B_11D4_A9A4_00605205F27C__INCLUDED_)
#define AFX_DDIVPRNT_H__1558B3C1_646B_11D4_A9A4_00605205F27C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DDivPrnt.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogDivPrint ダイアログ
class CTonView;

class CDialogDivPrint : public CDialog
{
// コンストラクション
public:
	CDialogDivPrint(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogDivPrint)
	enum { IDD = IDD_DIVPRINT };
	double	m_dNorisiro;
	double	m_dPrintXSize;
	double	m_dPrintYSize;
	//}}AFX_DATA

	CTonView*	m_pcView;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogDivPrint)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogDivPrint)
	afx_msg void OnDlgset();
	afx_msg void OnDlgreset();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DDIVPRNT_H__1558B3C1_646B_11D4_A9A4_00605205F27C__INCLUDED_)
