
#pragma once

// DWkBarSD.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBarSide ダイアログ

class CTonApp;
class CMainFrame;
class CTonView;
class CTonDoc;

class CDialogWakuBarSide : public CDialogBar
{
// コンストラクション
public:
	CDialogWakuBarSide(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CTonApp*	m_pcApp;
	CMainFrame*	m_pcWnd;
	CTonView*	m_pcView;
	int		m_nFirstFlag;
	pltenvtype m_stPlotEnv;
	//{{AFX_DATA(CDialogWakuBarSide)
	enum { IDD = IDD_WAKUSIDE };
	CString	m_csPloter;
	CString	m_csPaperName;
	CString	m_csWorkXSize;
	CString	m_csWorkYSize;
	CBitmapButton m_btnCntrKind;
	//}}AFX_DATA

// オーバーライド
public:
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogWakuBarSide)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void ResetPlotElement();
	void InitCmbPaperName();
	void InitCmbPloter();
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void SetCrtView(CTonView * pcView);
	void SetPrint();
	void SetPaperSave();
	void SetPageSpace();
	void CenterkindChange();
protected:
	void CenterKindBottonSet(int cntrkind);
	void GetPaperName(DBL srcxmm, DBL srcymm, LPTSTR retpapername);
	void GetPaperSize(LPTSTR srcpapername, DBL *retxmm, DBL *retymm);
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWakuBarSide)
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCmbpapername();
	afx_msg void OnSelchangeCmbploter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

