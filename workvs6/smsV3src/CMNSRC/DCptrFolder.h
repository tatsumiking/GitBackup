#if !defined(DCPTRFOLDER_H)
#define DCPTRFOLDER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DCptrFolder.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogCptrFolder ダイアログ

class CDialogCptrFolder : public CDialog
{
// コンストラクション
public:
	CDialogCptrFolder(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	DBL		m_dXTime, m_dYTime;
	//{{AFX_DATA(CDialogCptrFolder)
	enum { IDD = IDD_CPTR_FOLDER };
	CString	m_csSrcFolder;
	CString	m_csDstFolder;
	CString	m_csSrcXSize;
	CString	m_csSrcYSize;
	CString	m_csDstXSize;
	CString	m_csDstYSize;
	//}}AFX_DATA


// オーバーライド
public:
	static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogCptrFolder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void InitElement();
	void GetSelectDir(LPSTR lpTitle, LPSTR lpPath);

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogCptrFolder)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpnsrcxsize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpnsrcysize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpndstxsize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpndstysize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtndstfolder();
	afx_msg void OnBtnsrcfolder();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(DCPTRFOLDER_H)
