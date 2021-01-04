
#pragma once

// DlgWkSlct.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuSlct ダイアログ

class CDialogWakuSlct : public CDialog
{
// コンストラクション
public:
	CDialogWakuSlct(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogWakuSlct)
	enum { IDD = IDD_WAKUSLCT };
	CListBox	m_lstWkName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogWakuSlct)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// データ
public:
	CTonView	*m_pcView;
	CTonDoc		*m_pcDoc;
	int		m_nWakuCnt;
	int		m_nSlct;
	int		m_nXSize, m_nYSize;
	int		m_nListMax;
	TCHAR	m_szSetStr[RECMAX8];
	CString	m_csWakuName;
// インプリメンテーション
public:
	int LeglSlctNo(int slct);
	void DrawSlctRect(CDC *pcDC, CRect rect);
	void InitWakuNameList();
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWakuSlct)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

