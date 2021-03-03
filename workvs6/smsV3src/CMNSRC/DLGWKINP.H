#if !defined(AFX_DLGWKINP_H__5D2C5F25_36E7_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGWKINP_H__5D2C5F25_36E7_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgWkInp.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuInput ダイアログ

#define	INPUTEDITMAX	10

class CTonView;

class CDialogWakuInput : public CMyDialog
{
// コンストラクション
public:
	CDialogWakuInput(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogWakuInput)
	enum { IDD = IDD_WAKU_INPUT };
	CString	m_csCrctCnt;
	//}}AFX_DATA
	CStatic	m_stWkno[INPUTEDITMAX];
	CMyEdit	m_edText[INPUTEDITMAX];

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogWakuInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWakuInput)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDisp();
	afx_msg void OnCrct();
	afx_msg void OnLoad();
	afx_msg void OnOut();
	afx_msg void OnExit();
	afx_msg void OnPaint();
	afx_msg void OnPlotout();
	afx_msg void OnGaiji();
	afx_msg void OnCrctload();
	afx_msg void OnCrctsave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CTonView*	m_pcView;
	CTonDoc*	m_pcDoc;
	CCmdBase	*m_pcCmdBase;
	int		m_nCrtIdxTop;
	BOOL	m_fFirstFlag;
	int		m_nWakuMax;
	int		m_nNextFocus;
public:
	void SetString(LPSTR lpStr);
	virtual void KeyIn(UINT nChar, UINT nID);
	virtual void SelectEdit(UINT nID);
	void SaveRecString(int crctcnt, FILE *crctfp);
	void WakuSetFocus(int no);
	void WakuLoad(LPSTR wakufilename);
	void GetWakuMax();
	void SaveCrtWaku(int crctno, LPSTR paperpath, LPSTR wakuname);
	void SaveCrtWakuText();
	void LoadCrtWakuText();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGWKINP_H__5D2C5F25_36E7_11D2_A0A2_444553540000__INCLUDED_)
