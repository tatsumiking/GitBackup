
#pragma once

// DialogCrctdisp.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogCrctDisp ダイアログ

class CDialogCrctDisp : public CDialog
{
// コンストラクション
public:
	CDialogCrctDisp(CWnd* pParent = NULL);   // 標準のコンストラクタ
// ダイアログ データ
	//{{AFX_DATA(CDialogCrctDisp)
	enum { IDD = IDD_CRCTDISP };
	CListBox	m_lstCrct;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogCrctDisp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogCrctDisp)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLstCrct();
	afx_msg void OnRecDel();
	afx_msg void OnRecUp();
	afx_msg void OnRecDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CTonView*	m_pcView;
	CTonDoc*	m_pcDoc;
	int m_nSlctIdx;
	TCHAR	szTitle[FNAMEMAX];
public:
	void InitLstCrct();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

