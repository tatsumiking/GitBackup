#if !defined(AFX_DLMWKINP_H__5D2C5F25_36E7_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLMWKINP_H__5D2C5F25_36E7_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlmWkInp.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuInput ダイアログ

class CTonView;

class CDialogWakuInput : public CMyDialog
{
// コンストラクション
public:
	CDialogWakuInput(CWnd* pParent = NULL);   // 標準のコンストラクタ

public:
	CTonView*	m_pcView;
	CMyEdit*	m_pcEdit[10];	
	BOOL m_fFirstFlag;
	int m_nWakuMax;
	int m_nNextFocus;
	int	m_nCrtIdxTop;

public:
	virtual void KeyIn(UINT nChar, UINT nID);
	virtual void SelectEdit(UINT nID);
	void WakuSetFocus(int no);
	void GetWakuMax();
	void SaveCrtWakuText();
	void LoadCrtWakuText();

// ダイアログ データ
	//{{AFX_DATA(CDialogWakuInput)
	enum { IDD = IDD_WAKU_INPUT };
	CButton	m_btnDisp;
	//}}AFX_DATA


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
	afx_msg void OnExit();
	afx_msg void OnPaint();
	afx_msg void OnSetbtn();
	afx_msg void OnNextpage();
	afx_msg void OnBeforpage();
	afx_msg void OnCrctout();
	afx_msg void OnCrctsave();
	afx_msg void OnCrctdel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLMWKINP_H__5D2C5F25_36E7_11D2_A0A2_444553540000__INCLUDED_)
