#if !defined(_DGJSAVE_H_)
#define _DGJSAVE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DGjSave.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogGaijiSave ダイアログ

class CDialogGaijiSave : public CDialog
{
// コンストラクション
public:
	CDialogGaijiSave(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CMainFrame*	m_pcWnd;
	CTonView*	m_pcView;
	int	m_nFirstFlag;	
	int m_nCrtTop;
	int	m_nPageMax;
	int	m_nFirstKuten;
	int	m_nLastKuten;
	char	m_szFolderName[RECMAX];
	//{{AFX_DATA(CDialogGaijiSave)
	enum { IDD = IDD_GAIJI_SAVE };
	CComboBox	m_cmbBusyu;
	CComboBox	m_cmbKakusu;
	CComboBox	m_cmbFont;
	CListBox	m_lstImg;
	CStatic	m_stTop;
	CStatic	m_stImg;
	CString	m_csCode;
	CString	m_csBusyu;
	CString	m_csFont;
	CString	m_csKakusu;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogGaijiSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void InitCmbKakusu();
	void InitCmbBusyu();
	void InitCmbFont();
	void BoxsDraw(CDC *pcDC, int sx, int sy, int ex, int ey);
	void CreatePartList();
	void InitListImage();
	void DrawPartImg(CDC *pcDC, CRect rect);
	void LegalRect(CRect *rect);

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogGaijiSave)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBeforpage();
	afx_msg void OnNextpage();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeLstimg();
	afx_msg void OnSelchangeCmbfont();
	afx_msg void OnBtnsave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(_DGJSAVE_H_)
