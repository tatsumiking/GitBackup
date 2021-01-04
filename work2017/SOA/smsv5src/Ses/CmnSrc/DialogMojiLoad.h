
#pragma once

// DlgMjLd.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiLoad ダイアログ

//C 文字呼出ダイアログクラス
class CDialogMojiLoad : public CDialog
{
// コンストラクション
public:
	void SetDialogTitle(LPTSTR lpTitle);
	void FontCBInit();
	CDialogMojiLoad(CWnd* pParent = NULL);   // 標準のコンストラクタ

	LPTSTR m_lpTitle;

// ダイアログ データ
	//{{AFX_DATA(CDialogMojiLoad)
	enum { IDD = IDD_MOJILOAD };
	CString	m_csFontName;
	CString	m_csMojiCode;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogMojiLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogMojiLoad)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

