
#pragma once

// DlEKmnSv.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogESKamonSave ダイアログ

class CDialogESKamonSave : public CDialog
{
// コンストラクション
public:
	CDialogESKamonSave(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	void InitKamonNameList();
	CTonApp*	m_pcApp;
	CTonView*	m_pcView;
	int	m_nDlgWidth;
	int	m_nDlgHight;
	int	m_nCrtTop;
	int m_nSelect;
	DBL	m_dTrnsTime;
	//{{AFX_DATA(CDialogESKamonSave)
	enum { IDD = IDD_ESKMN_SAVE };
	CListBox	m_lstKamonName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogESKamonSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	void KamonAllDraw(CDC *pcDC);
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogESKamonSave)
	afx_msg void OnPaint();
	afx_msg void OnNextpage();
	afx_msg void OnBeforpage();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeKmnlist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

