
#pragma once

// DlgShdw.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogShadow ダイアログ

class CDialogShadow : public CDialog
{
// コンストラクション
public:
	CDialogShadow(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CDialogShadow();

public:
	CDataList* m_pcDataList;
	CTonView *m_pcView;
	int	m_nFirstFlag;
	int	m_nKind;
	COLORREF	m_clrShadow;
	CFont	m_fntImg;
	DBL		m_dTime;
	DBL		m_dOrgSize;
// ダイアログ データ
	//{{AFX_DATA(CDialogShadow)
	enum { IDD = IDD_SHADOW };
	CSpinButtonCtrl	m_spRoundrag;
	CSpinButtonCtrl	m_spDistance;
	double	m_dDistance;
	double	m_dRoundRag;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogShadow)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void GetRltvRect(CRect *rect);
	void SampleDataOut(CDC *pcDC);

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogShadow)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCbshadowkind();
	virtual void OnOK();
	afx_msg void OnStclr();
	afx_msg void OnChangeEddistance();
	afx_msg void OnChangeEdroundrag();
	afx_msg void OnDeltaposSpdistance(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSproundrag(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

