#if !defined(AFX_DLGCNRKD_H__F8B40F02_322E_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGCNRKD_H__F8B40F02_322E_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgCnrKd.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogConerKind ダイアログ

//C 支点設定ダイアログクラス
class CDialogConerKind : public CDialog
{
// コンストラクション
public:
	CDialogConerKind(CWnd* pParent = NULL);   // 標準のコンストラクタ
	void SetDialogTitle(LPSTR lpTitle);

// ダイアログ データ
	//{{AFX_DATA(CDialogConerKind)
	enum { IDD = IDD_CONER_KIND };
	CComboBox	m_cbConerKind;
	CString	m_csConerKind;
	//}}AFX_DATA
	int		m_nConerKind;
	LPSTR	m_lpTitle;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogConerKind)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogConerKind)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGCNRKD_H__F8B40F02_322E_11D2_A0A2_444553540000__INCLUDED_)

