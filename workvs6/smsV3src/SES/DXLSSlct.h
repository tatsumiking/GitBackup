#if !defined(DXLSSLCT_H)
#define DXLSSLCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DXLSSlct.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogXLSSelect ダイアログ
#include	"ExcelLib.h"

class CDialogXLSSelect : public CDialog
{
// コンストラクション
public:
	CDialogXLSSelect(CWnd* pParent = NULL);
// ダイアログ データ
public:
	CString	m_csFileName;
	CString	m_csSheetName;
	CExcelLib	m_cExcelLib;
	//{{AFX_DATA(CDialogXLSSelect)
	enum { IDD = IDD_XLSSLCT };
	CListBox	m_lstName;
	//}}AFX_DATA


// オーバーライド
public:
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogXLSSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void InitNameList();

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogXLSSelect)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(DXLSSLCT_H)
