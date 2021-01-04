
#pragma once

// DWkBar3.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBar3 ダイアログ

class CTonApp;
class CMainFrame;
class CTonView;
class CTonDoc;

class CDialogWakuBar3 : public CDialogBar
{
// コンストラクション
public:
	CDialogWakuBar3(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CTonApp*	m_pcApp;
	CMainFrame*	m_pcWnd;
	CTonView*	m_pcView;
	int		m_nFirstFlag;
	HIMC	m_hImc;
	int		m_nShadowKind;
	COLORREF	m_clrShadow;
	DBL		m_dShadowDistance;
	DBL		m_dShadowRoundRag;
	//{{AFX_DATA(CDialogWakuBar3)
	enum { IDD = IDD_WAKUBAR3 };
	CButton	m_btnTateYoko;
	CString	m_csSizeX;
	CString	m_csSizeY;
	CString	m_csStartX;
	CString	m_csStartY;
	CBitmapButton	m_btnStart;
	CBitmapButton	m_btnConer;
	CString	m_csInText;
	BOOL	m_nShadow;
	//}}AFX_DATA

// オーバーライド
public:
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogWakuBar3)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void SetLocate();
	void GetLocate();
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void SetCrtView(CTonView * pcView);
	void TateyokoChange();
	void StartKindChange();
	void ConerKindChange();
protected:
	void TateYokoFlagBottonSet(int ftategaki);
	void StartKindBottonSet(int nstartkind);
	void ConerKindBottonSet(int nconerkind);
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWakuBar3)
	afx_msg void OnPaint();
	afx_msg void OnSetfocusEdintext();
	afx_msg void OnKillfocusEdintext();
	afx_msg void OnChkShadow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

