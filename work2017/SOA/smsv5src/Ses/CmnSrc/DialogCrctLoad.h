
#pragma once

// DlCrctLd.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogCrctLoad ダイアログ

class CDialogCrctLoad : public CDialog
{
// コンストラクション
public:
	CDialogCrctLoad(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogCrctLoad)
	enum { IDD = IDD_CRCTLOAD };
	CListBox	m_lstCrct;
	//}}AFX_DATA
	int		m_nSlctIdx;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogCrctLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogCrctLoad)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLstCrct();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

