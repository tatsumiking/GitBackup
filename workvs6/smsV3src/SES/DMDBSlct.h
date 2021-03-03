#if !defined(DMDBSLCT_H)
#define DMDBSLCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMDBSlct.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMDBSelect ダイアログ

// [クラス名] CDialogMDBSelect
// [親クラス] CDialog
// [クラス概要] データベーステーブル、クーリエ選択ダイアログクラス
class CDialogMDBSelect : public CDialog
{
// コンストラクション
public:
	CDialogMDBSelect(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CString	m_csDBFname;
	CDaoDatabase* m_pcDaoDatabase;
	CString	m_csTableName;
	//{{AFX_DATA(CDialogMDBSelect)
	enum { IDD = IDD_MDBSLCT };
	CListBox	m_lstName;
	int		m_nTBLorQRY;
	//}}AFX_DATA


// オーバーライド
public:
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogMDBSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void InitNameList();
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogMDBSelect)
	afx_msg void OnRdtable();
	afx_msg void OnRdquery();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(DMDBSLCT_H)
