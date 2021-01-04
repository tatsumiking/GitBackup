
#pragma once

// DWkPpDel.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWkPaperDel ダイアログ

class CDialogWkPaperDel : public CDialog
{
// コンストラクション
public:
	CDialogWkPaperDel(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CTonView	*m_pcView;
	//{{AFX_DATA(CDialogWkPaperDel)
	enum { IDD = IDD_WAKU_PAPERDEL };
	CString	m_csPaperPath;
	CString	m_csPaperName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogWkPaperDel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void CBInitPaperName();
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWkPaperDel)
	afx_msg void OnDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangePapername();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

