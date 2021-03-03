#if !defined(AFX_DIALOGPRINTSIZE_H__FD6B7A61_FCC2_11D2_A712_00A0B0086478__INCLUDED_)
#define AFX_DIALOGPRINTSIZE_H__FD6B7A61_FCC2_11D2_A712_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogPrintSize.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPrintSize ダイアログ

class CDialogPrintSize : public CDialog
{
// コンストラクション
public:
	CDialogPrintSize(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogPrintSize)
	enum { IDD = IDD_PRINTSIZE };
	CString	m_csSizeMsg;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogPrintSize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogPrintSize)
	afx_msg void OnNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DIALOGPRINTSIZE_H__FD6B7A61_FCC2_11D2_A712_00A0B0086478__INCLUDED_)
