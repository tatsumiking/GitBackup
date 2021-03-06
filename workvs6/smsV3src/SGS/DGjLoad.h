#if !defined(_DGJLOAD_H_)
#define _DGJLOAD_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DGjLoad.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogGaijiLoad ダイアログ

class CDialogGaijiLoad : public CDialog
{
// コンストラクション
public:
	CDialogGaijiLoad(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CMainFrame*	m_pcWnd;
	CTonView*	m_pcView;
	int	m_nFirstFlag;	
	int m_nCrtTop;
	int	m_nLstMax;
	int	m_nPageMax;
	int	m_nFirstKuten;
	int	m_nLastKuten;
	char	m_szFolderName[RECMAX];
	//{{AFX_DATA(CDialogGaijiLoad)
	enum { IDD = IDD_GAIJI_LOAD };
	CListBox	m_lstCode;
	CComboBox	m_cmbFont;
	CListBox	m_lstImg;
	CStatic	m_stTop;
	CStatic	m_stImg;
	CString	m_csCode;
	CString	m_csFont;
	int		m_nSortKind;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogGaijiLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void InitCmbFont();
	void BoxsDraw(CDC *pcDC, int sx, int sy, int ex, int ey);
	void CreatePartList();
	void InitListImageCode();
	void InitListKakusuSort();
	void InitListBusyuSort();
	void InitListCodeSort();
	void DrawPartImg(CDC *pcDC, CRect rect);
	void LegalRect(CRect *rect);

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogGaijiLoad)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBeforpage();
	afx_msg void OnNextpage();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeLstimg();
	afx_msg void OnBtnload();
	afx_msg void OnRdocodesort();
	afx_msg void OnRdobusyusort();
	afx_msg void OnRdokakususort();
	afx_msg void OnSelchangeCmbfont();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(_DGJLOAD_H_)
