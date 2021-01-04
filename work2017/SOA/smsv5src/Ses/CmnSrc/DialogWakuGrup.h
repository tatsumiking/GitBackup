
#pragma once

// DlgWkGrp.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuGrup ダイアログ

class CDialogWakuGrup : public CDialog
{
// コンストラクション
public:
	CDialogWakuGrup(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	TCHAR	m_szGrupPath[FNAMEMAX];
	//{{AFX_DATA(CDialogWakuGrup)
	enum { IDD = IDD_WAKU_GRUP };
	CString	m_csWakuGrupPath;
	CString	m_csWakuGrupName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogWakuGrup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void CheckAddWakuGrup(LPTSTR gruppath);
	void InitWakuGrupCombo();

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWakuGrup)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeWkgrupname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。


