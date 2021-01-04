
#pragma once

// ViewFDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CViewFileDialog ダイアログ

class CViewFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CViewFileDialog)

public:
	CViewFileDialog(BOOL bOpenFileDialog, // TRUE ならば FileOpen、 FALSE ならば FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);
	~CViewFileDialog();

public:
	int m_nImgSize;
	int m_nImgSY;
	int m_nImgSX;
	HANDLE	m_hDib;
	long	m_lDibSize;
	long	m_lImgFileSize;
	//{{AFX_DATA(CViewFileDialog)
	CStatic	m_stTop;
	CStatic	m_stImg;
	CComboBox	m_cmbGyousya;
	CComboBox	m_cmbKouji;
	CString	m_csGyousya;
	CString	m_csKouji;
	CString	m_csComent1;
	CString	m_csComent2;
	CString	m_csComent3;
	//}}AFX_DATA
	
public:
	void SetTitle(LPTSTR szTitle);
	void LoadCmbKouji();
	void LoadCmbGyousya();
	virtual void OnFileNameChange();
	void FileViewLoad(LPTSTR fname);
	void BitMapImageLoad(CArchive &ar);
	void BitMapImageDisp(CDC *dc);
	//{{AFX_VIRTUAL(CViewFileDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CViewFileDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnEditchangeCmbgyousya();
	afx_msg void OnEditchangeCmbkouji();
	afx_msg void OnChangeEdcoment1();
	afx_msg void OnChangeEdcoment2();
	afx_msg void OnChangeEdcoment3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

