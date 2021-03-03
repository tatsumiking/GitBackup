#if !defined(DLGTXTIN_H)
#define DLGTXTIN_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgTxtIn.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogTextInput ダイアログ

class CDialogTextInput : public CMyDialog
{
// コンストラクション
public:
	CDialogTextInput(CWnd* pParent = NULL);   // 標準のコンストラクタ

public:
	void SetTitleText(LPSTR lpTitleText);
	void SetDialogTitle(LPSTR lpTitle);
// ダイアログ データ
public:

	LPSTR	m_lpTitle;
	LPSTR	m_lpTitleText;
	int		m_nIMEMode;	
	//{{AFX_DATA(CDialogTextInput)
	enum { IDD = IDD_TEXTINPUT };
	CStatic	m_stTitle;
	CMyEdit	m_edText;
	CString	m_csText;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogTextInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogTextInput)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(DLGTXTIN_H)
