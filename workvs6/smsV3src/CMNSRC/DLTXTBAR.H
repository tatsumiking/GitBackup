#if !defined(AFX_DIALOGTEXTINBAR_H__EB898821_CD81_11D2_A711_00A0B0086478__INCLUDED_)
#define AFX_DIALOGTEXTINBAR_H__EB898821_CD81_11D2_A711_00A0B0086478__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DialogTextInBar.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogTextInEnv ダイアログ

class CTonView;

class CDialogTextInBar : public CMyDialogBar
{
// コンストラクション
public:
	CTonView	*m_pcView;
	int m_nFirst;
public:
	CDialogTextInBar(CWnd* pParent = NULL);   // 標準のコンストラクタ
public:
	void SetCrtView(CTonView * pcView);

// ダイアログ データ
	//{{AFX_DATA(CDialogTextInBar)
	enum { IDD = IDD_TEXTINBAR };
	CComboBox	m_cbFontSize;
	CComboBox	m_cbFontName;
	CBitmapButton	m_btnTextTategaki;
	CBitmapButton	m_btnTextUnderLine;
	CBitmapButton	m_btnTextRight;
	CBitmapButton	m_btnTextLeft;
	CBitmapButton	m_btnTextItric;
	CBitmapButton	m_btnTextEqul;
	CBitmapButton	m_btnTextCntr;
	CBitmapButton	m_btnTextBold;
	CString	m_csFontName;
	CString	m_csFontSize;
	double	m_dStrgLen;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogTextInBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	DBL	m_dFontSize;
public:
	void SetItemElement();
	void GetItemElement();
	void SetButtonElement();
	void InitFontSize();
	void InitFontList();
protected:
	void GetCmbFontName();
	void GetCmbFontSize();
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogTextInBar)
	afx_msg void OnPaint();
	afx_msg void OnBtnBold();
	afx_msg void OnBtnItric();
	afx_msg void OnBtnUnderl();
	afx_msg void OnBtnLeft();
	afx_msg void OnBtnCntr();
	afx_msg void OnBtnRight();
	afx_msg void OnBtnEqul();
	afx_msg void OnBtnTategaki();
	afx_msg void OnChangeEdstrglen();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbFontname();
	afx_msg void OnSelchangeCmbFontsize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DIALOGTEXTINBAR_H__EB898821_CD81_11D2_A711_00A0B0086478__INCLUDED_)

