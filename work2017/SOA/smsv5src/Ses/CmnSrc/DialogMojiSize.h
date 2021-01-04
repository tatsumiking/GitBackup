
#pragma once

// DlgMjSiz.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiSize ダイアログ
class CTonView;

//C 文字サイズ変更ダイアログクラス
class CDialogMojiSize : public CDialog
{
// コンストラクション
public:
	void SetItemElement();
	void GetItemElement();
	CDialogMojiSize(CWnd* pParent = NULL);   // 標準のコンストラクタ

	CTonView*	m_pcView;
	CBitmapButton	m_btnSmall;
	CBitmapButton	m_btnBig;
// ダイアログ データ
	//{{AFX_DATA(CDialogMojiSize)
	enum { IDD = IDD_MOJI_SIZE };
	double	m_edStepmm;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogMojiSize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogMojiSize)
	virtual BOOL OnInitDialog();
	afx_msg void OnDlgReset();
	afx_msg void OnDlgSet();
	afx_msg void OnBtnSizebig();
	afx_msg void OnBtnSizesmall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

