#if !defined(AFX_DIALOGESKAMON_H__C26419C4_177C_11D3_A712_00A0B0086478__INCLUDED_)
#define AFX_DIALOGESKAMON_H__C26419C4_177C_11D3_A712_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgESKmn.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogESKamon ダイアログ

class CDialogESKamon : public CDialog
{
// コンストラクション
public:
	CDialogESKamon(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogESKamon)
	enum { IDD = IDD_ESKAMON };
	CString	m_csName;
	CString	m_csFullName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogESKamon)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogESKamon)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DIALOGESKAMON_H__C26419C4_177C_11D3_A712_00A0B0086478__INCLUDED_)
