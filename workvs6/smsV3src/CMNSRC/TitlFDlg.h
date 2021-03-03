#if !defined(TITLFDLG_H)
#define TITLFDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TitlFileDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CTitlFileDialog ダイアログ

class CTitlFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CTitlFileDialog)

public:
	CTitlFileDialog(BOOL bOpenFileDialog, // TRUE ならば FileOpen、 FALSE ならば FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

	//{{AFX_DATA(CTitlFileDialog)
	CString	m_csComent1;
	//}}AFX_DATA

public:
	virtual void OnFileNameChange();
	void SetTitle(LPSTR szTitle);
	void FileTitlLoad(LPSTR fname);
	//{{AFX_VIRTUAL(CViewFileDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CTitlFileDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(TITLFDLG_H)
