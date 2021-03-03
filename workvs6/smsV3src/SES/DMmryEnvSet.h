#if !defined(DMMRYENVSET_H_)
#define DMMRYENVSET_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMmryEnvSet.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMemoryEnvSet ダイアログ

class CDialogMemoryEnvSet : public CDialog
{
// コンストラクション
public:
	CDialogMemoryEnvSet(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CTonApp*	m_pcApp;
	CMainFrame*	m_pcWnd;
	//{{AFX_DATA(CDialogMemoryEnvSet)
	enum { IDD = IDD_MEMOENVSET };
	CString	m_csMemoTempPath;
	CString	m_csMemoBackPath;
	CString	m_csMemoPhotoPath;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogMemoryEnvSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	void GetSelectDir(LPSTR lpTitle, LPSTR lpPath);

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogMemoryEnvSet)
	afx_msg void OnBtnmemobackpath();
	afx_msg void OnBtnmemophotopath();
	afx_msg void OnBtnmemotemppath();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(DMMRYENVSET_H_)
