#if !defined(_DPRTSAVE_H_)
#define _DPRTSAVE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DPrtSave.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPartSave ダイアログ

class CDialogPartSave : public CDialog
{
// コンストラクション
public:
	CDialogPartSave(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CMainFrame*	m_pcWnd;
	CTonView*	m_pcView;
	int	m_nFirstFlag;	
	int m_nCrtTop;
	int	m_nLstMax;
	int	m_nPageMax;
	char	m_szName[RECMAX];
	//{{AFX_DATA(CDialogPartSave)
	enum { IDD = IDD_PART_SAVE };
	CListBox	m_lstImg;
	CStatic	m_stTop;
	CStatic	m_stImg;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogPartSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void BoxsDraw(CDC *pcDC, int sx, int sy, int ex, int ey);
	void CreatePartList();
	void InitListImage();
	void DrawPartImg(CDC *pcDC, CRect rect);
	void LegalRect(CRect *rect);

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogPartSave)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBeforpage();
	afx_msg void OnNextpage();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeLstimg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(_DPRTSAVE_H_)
