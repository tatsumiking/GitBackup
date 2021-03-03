#if !defined(AFX_MYFILEDL_H__0D2F6501_1E16_11D3_9F64_0000E835797C__INCLUDED_)
#define AFX_MYFILEDL_H__0D2F6501_1E16_11D3_9F64_0000E835797C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFileDl.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMyFileDialog ダイアログ

class AFX_CLASS_EXPORT CMyFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CMyFileDialog)

public:
	void SetBtnText(LPSTR msg);
	void SetTitle(LPSTR lpstr);
	void SetCrtPath(LPSTR lppath);
	CMyFileDialog(BOOL bOpenFileDialog, // TRUE ならば FileOpen、 FALSE ならば FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);
	
	LPSTR m_lpBtnMsg;
protected:
	//{{AFX_MSG(CMyFileDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MYFILEDL_H__0D2F6501_1E16_11D3_9F64_0000E835797C__INCLUDED_)
