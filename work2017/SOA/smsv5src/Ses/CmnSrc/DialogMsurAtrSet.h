
#pragma once

// DlgMsAtr.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMsurAtrSet ダイアログ

class CDialogMsurAtrSet : public CDialog
{
// コンストラクション
public:
	CDialogMsurAtrSet(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogMsurAtrSet)
	enum { IDD = IDD_MSURATRSET };
	CString	m_csMsurStrg;
	double	m_dFontSize;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogMsurAtrSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogMsurAtrSet)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。


