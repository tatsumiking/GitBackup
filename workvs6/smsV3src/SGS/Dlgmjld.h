#if !defined(AFX_DLGMJLD_H__FF78B4A2_FD1B_11D1_A0A2_444553540000__INCLUDED_)
#define AFX_DLGMJLD_H__FF78B4A2_FD1B_11D1_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgMjLd.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiLoad ダイアログ

//C 文字呼出ダイアログクラス
class CDialogMojiLoad : public CDialog
{
// コンストラクション
public:
	void SetDialogTitle(LPSTR lpTitle);
	void FontCBInit();
	CDialogMojiLoad(CWnd* pParent = NULL);   // 標準のコンストラクタ

	LPSTR m_lpTitle;

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

#endif // !defined(AFX_DLGMJLD_H__FF78B4A2_FD1B_11D1_A0A2_444553540000__INCLUDED_)
