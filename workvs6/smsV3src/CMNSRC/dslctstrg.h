#if !defined(AFX_DSLCTSTRG_H__C5959C43_2E32_4BA1_9DEA_F39909CE109D__INCLUDED_)
#define AFX_DSLCTSTRG_H__C5959C43_2E32_4BA1_9DEA_F39909CE109D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DSlctStrg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSlctStrg ダイアログ

class CDialogSlctStrg : public CDialog
{
// コンストラクション
public:
	CDialogSlctStrg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CTonView	*m_pcView;
	//{{AFX_DATA(CDialogSlctStrg)
	enum { IDD = IDD_SLCTSTRG };
	CListBox	m_lstStrg1;
	CListBox	m_lstStrg2;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogSlctStrg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void LoadStrg2List();
	void LoadStrg1List();

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogSlctStrg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkLststrg1();
	afx_msg void OnDblclkLststrg2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DSLCTSTRG_H__C5959C43_2E32_4BA1_9DEA_F39909CE109D__INCLUDED_)
