
#pragma once

// DlgChkOut.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogCheckOutput ダイアログ

class CDialogCheckOutput : public CDialog
{
// コンストラクション
public:
	CDialogCheckOutput(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogCheckOutput)
	enum { IDD = IDD_CHECKOUTPUT };
	CStatic	m_stTop;
	CStatic	m_stImg;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogCheckOutput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

public:
	CTonView	*m_pcView;
public:
// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogCheckOutput)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

