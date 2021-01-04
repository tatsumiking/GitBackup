
#pragma once

// DialogDivPlotOut.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogDivPlotOut ダイアログ

class CTonView;

//C 分割プロッタ出力設定ダイアログクラス
class CDialogDivPlotOut : public CDialog
{
// コンストラクション
public:
	CDialogDivPlotOut(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogDivPlotOut)
	enum { IDD = IDD_DIVPLOTER };
	BOOL	m_chkAreaBoxs;
	BOOL	m_chkStopChk;
	double	m_dNorisiro;
	double	m_dPlotXSize;
	double	m_dPlotYSize;
	//}}AFX_DATA

	CTonView*	m_pcView;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogDivPlotOut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogDivPlotOut)
	afx_msg void OnDlgreset();
	afx_msg void OnDlgset();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

