
#pragma once

// DWkPpAdd.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWkPaperAdd ダイアログ
class CTonView;

class CDialogWkPaperAdd : public CDialog
{
// コンストラクション
public:
	CDialogWkPaperAdd(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CTonView	*m_pcView;
	//{{AFX_DATA(CDialogWkPaperAdd)
	enum { IDD = IDD_WAKU_PAPERADD };
	CString	m_csPaperPath;
	CString	m_csPaperName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogWkPaperAdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	BOOL CheckFileName(LPTSTR lpFileName);
protected:
	void CBInitPaperName();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWkPaperAdd)
	afx_msg void OnSave();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangePapername();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

