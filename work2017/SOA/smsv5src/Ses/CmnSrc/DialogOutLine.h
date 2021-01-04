
#pragma once

// DlgOutLn.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogOutLine ダイアログ

class CDialogOutLine : public CDialog
{
// コンストラクション
public:
	CDialogOutLine(CWnd* pParent = NULL);   // 標準のコンストラクタ
	void SetTitle(LPTSTR lpTitle);

	LPTSTR	m_lpTitle;
// ダイアログ データ
	//{{AFX_DATA(CDialogOutLine)
	enum { IDD = IDD_OUTLINE };
	double	m_dOutLineMM;
	int		m_nOutLineKind;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogOutLine)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogOutLine)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

