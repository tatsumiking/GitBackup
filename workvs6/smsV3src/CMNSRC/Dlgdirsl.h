#if !defined(AFX_DLGDIRSL_H__83801882_1AA2_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGDIRSL_H__83801882_1AA2_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgDirSl.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogDirSlct ダイアログ

//C フォルダー選択ダイアログクラス
class CDialogDirSlct : public CDialog
{
// コンストラクション
public:
	void SetInitPath(LPSTR path);
	void DirlistUpdate();
	char m_szPathName[FNAMEMAX];
	CDialogDirSlct(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogDirSlct)
	enum { IDD = IDD_DIRSLCT };
	CString	m_csSlctDir;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogDirSlct)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogDirSlct)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDirlist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGDIRSL_H__83801882_1AA2_11D2_A0A2_444553540000__INCLUDED_)
