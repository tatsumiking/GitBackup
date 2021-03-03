#if !defined(AFX_DSLCTFTPADRS_H__65E67082_7F84_42C4_8B55_58255EA64A67__INCLUDED_)
#define AFX_DSLCTFTPADRS_H__65E67082_7F84_42C4_8B55_58255EA64A67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DSlctFtpAdrs.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSlctFtpAdrs ダイアログ

class CDialogSlctFtpAdrs : public CDialog
{
// コンストラクション
public:
	void InitFileName();
	void InitCmbFtpFolder();
	CDialogSlctFtpAdrs(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogSlctFtpAdrs)
	enum { IDD = IDD_SLCTFTPADRS };
	CComboBox	m_cmbFtpFolder;
	CString	m_csFtpFolder;
	CString	m_csFileName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogSlctFtpAdrs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogSlctFtpAdrs)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DSLCTFTPADRS_H__65E67082_7F84_42C4_8B55_58255EA64A67__INCLUDED_)
