#if !defined(AFX_DLGFCHNG_H__7F66C362_8745_11D2_940A_0000E835797C__INCLUDED_)
#define AFX_DLGFCHNG_H__7F66C362_8745_11D2_940A_0000E835797C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgFChng.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogFontChange ダイアログ
class CTonView;

//C 書体変更ダイアログクラス
class CDialogFontChange : public CDialog
{
// コンストラクション
public:
	CTonView*	m_pcView;

public:
	void FontCBInit();
	CDialogFontChange(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogFontChange)
	enum { IDD = IDD_FONTCHNG };
	CString	m_csFontName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。


	//{{AFX_VIRTUAL(CDialogFontChange)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogFontChange)
	virtual BOOL OnInitDialog();
	afx_msg void OnDlgreset();
	afx_msg void OnDlgset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGFCHNG_H__7F66C362_8745_11D2_940A_0000E835797C__INCLUDED_)
