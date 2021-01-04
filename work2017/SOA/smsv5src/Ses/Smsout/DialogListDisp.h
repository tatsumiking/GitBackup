
#pragma once

// DlgLstDsp.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogListDisp ダイアログ
#define	CNTTBLMAX	50
#define CNTMINI		6
#define NOCLMSIZE	3

class CCrctWaku;

class CDialogListDisp : public CDialog
{
// コンストラクション
public:
	CDialogListDisp(CWnd* pParent = NULL);   // 標準のコンストラクタ
	
// ダイアログ データ
	//{{AFX_DATA(CDialogListDisp)
	enum { IDD = IDD_LISTDISP };
	CListBox	m_lstRecText;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogListDisp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

public:
	CTonView	*m_pcView;
	CTonDoc		*m_pcDoc;
	TCHAR	m_szTitle[QRECMAX];
	int		m_nInSort;
	int		m_nExitCall;
	DBL	m_dPrntTime;
	DBL	m_nPrntXSize;
	DBL	m_nPrntYSize;
	int	m_nCntTbl[CNTTBLMAX];
	// インプリメンテーション
public:
	void InitPrintEnv(CDC *pcDC);
	void PrintOutMain(CDC *pcDC);
	void InitRecTextList();
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogListDisp)
	afx_msg void OnBtnimagedisp();
	afx_msg void OnBtnprintout();
	afx_msg void OnBtnexit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

