#if !defined(DLINPAIR_H)
#define DLINPAIR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlInPair.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogInputNumPair ダイアログ
class CTonView;

//C ペア（ＸＹなど）数値入力ダイアログクラス
class CDialogInputNumPair : public CDialog
{
// コンストラクション
public:
	void SetItemElement();
	void GetItemElement();
	void SetModal();
	void SetModaless();

	void SetTitleText(LPSTR lpTitleTextX, LPSTR lpTitleTextY);
	void SetUnitText(LPSTR lpUnitTextX, LPSTR lpUnitTextY);
	void SetDialogTitle(LPSTR lpTitle);
	CDialogInputNumPair(CWnd* pParent = NULL);   // 標準のコンストラクタ

	LPSTR m_lpTitle;
	LPSTR m_lpTitleTextX;
	LPSTR m_lpTitleTextY;
	LPSTR m_lpUnitTextX;
	LPSTR m_lpUnitTextY;
	CTonView*	m_pcView;
	int m_nMode;
	int	m_nFirst;
// ダイアログ データ
	//{{AFX_DATA(CDialogInputNumPair)
	enum { IDD = IDD_INNUMPAIR };
	CEdit	m_edInputNumX;
	CEdit	m_edInputNumY;
	CButton	m_cbtnOK;
	CButton	m_cbtnCancel;
	CStatic	m_stUnitX;
	CStatic	m_stUnitY;
	CStatic	m_stTitleX;
	CStatic	m_stTitleY;
	double	m_dInputNumX;
	double	m_dInputNumY;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogInputNumPair)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogInputNumPair)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(DLINPAIR_H)

