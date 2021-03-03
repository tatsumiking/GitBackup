#if !defined(DWKBAR2_H)
#define DWKBAR2_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DWkBar2.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBar2 ダイアログ

class CDialogWakuBar2 : public CDialogBar
{
// コンストラクション
public:
	CDialogWakuBar2(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CTonApp*	m_pcApp;
	CTonView*	m_pcView;
	CTonDoc*	m_pcDoc;
	int		m_nFirstFlag;
	//{{AFX_DATA(CDialogWakuBar2)
	enum { IDD = IDD_WAKUBAR2 };
	CString	m_csFont;
	CString	m_csFlatPar;
	CString	m_csSpaceMM;
	CString	m_csOderKind;
	CString	m_csKanaPar;
	CString	m_csHanten;
	CString	m_csMojiCount;
	CString	m_csFildName;
	//}}AFX_DATA


// オーバーライド
public:
	//{{AFX_VIRTUAL(CDialogWakuBar2)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void SetCrtView(CTonView * pcView);
	void InitCmbFont();
	void InitCmbFildName();
	void InitCmbFlatPar();
	void InitCmbSpace();
	void InitCmbOderKind();
	void InitCmbKanaPar();
	void InitCmbHanten();
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWakuBar2)
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCmbfont();
	afx_msg void OnSelchangeCmbflatpar();
	afx_msg void OnSelchangeCmbhanten();
	afx_msg void OnSelchangeCmbkanapar();
	afx_msg void OnSelchangeCmboderkind();
	afx_msg void OnSelchangeCmbspacemm();
	afx_msg void OnBtnspacekind();
	afx_msg void OnChkfixed();
	afx_msg void OnChkyokogumi();
	afx_msg void OnSelchangeCmbfild();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(DWKBAR2_H)

