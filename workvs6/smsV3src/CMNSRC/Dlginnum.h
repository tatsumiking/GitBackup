#if !defined(DLGINNUM_H)
#define DLGINNUM_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgInNum.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogInputNum ダイアログ
class CTonView;

//C 数値入力ダイアログクラス
class CDialogInputNum : public CDialog
{
// コンストラクション
public:
	void SetItemElement();
	void GetItemElement();
	void SetModal();
	void SetModaless();

	void SetTitleText(LPSTR lpTitleText);
	void SetUnitText(LPSTR lpUnitText);
	void SetDialogTitle(LPSTR lpTitle);
	CDialogInputNum(CWnd* pParent = NULL);   // 標準のコンストラクタ

	LPSTR m_lpTitle;
	LPSTR m_lpTitleText;
	LPSTR m_lpUnitText;
	CTonView*	m_pcView;
	int m_nMode;
	int	m_nFirst;
// ダイアログ データ
	//{{AFX_DATA(CDialogInputNum)
	enum { IDD = IDD_INNUM };
	CEdit	m_edInputNum;
	CButton	m_cbtnOK;
	CButton	m_cbtnCancel;
	CStatic	m_stUnit;
	CStatic	m_stTitle;
	double	m_dInputNum;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogInputNum)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogInputNum)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(DLGINNUM_H)

