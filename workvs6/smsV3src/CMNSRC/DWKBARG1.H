#if !defined(DWKBARG1_H)
#define DWKBARG1_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DWkBarG.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBarG1 ダイアログ

class CDialogWakuBarG1 : public CDialogBar
{
// コンストラクション
public:
	void SetItemElement();
	CDialogWakuBarG1(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogWakuBarG1)
	enum { IDD = IDD_WAKUBARG1 };
	CString	m_csWorkXSize;
	CString	m_csWorkYSize;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogWakuBarG1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWakuBarG1)
	afx_msg void OnChangeEdworkxsize();
	afx_msg void OnChangeEdworkysize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(DWKBARG1_H)
