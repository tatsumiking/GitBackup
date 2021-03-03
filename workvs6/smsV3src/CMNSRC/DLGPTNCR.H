#if !defined(AFX_DLGPTNCR_H__77C50202_4E6C_11D2_99B2_004026171D2B__INCLUDED_)
#define AFX_DLGPTNCR_H__77C50202_4E6C_11D2_99B2_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgPtnCr.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnCreate ダイアログ

class CTonView;

class CDialogPtnCreate : public CDialog
{
// コンストラクション
public:
	DBL CnvertUnitToMM(DBL unit);
	DBL CnvertMMToUnit(DBL mm);
	void UnitListInit();
	void OderListInit();
	void SetModeListInit();
	void FontListInit();
	CTonView	*m_pcView;
	CDialogPtnCreate(CWnd* pParent = NULL);   // 標準のコンストラクタ

public:
	void ElementGet();
	void ElementSet();
	UINT	m_nID;
	HIMC	m_hImc;

	char	m_szFontName[RECMAX];
	int		m_nOderNo;
	int		m_nUnitNo;
	DBL		m_dSizemm;
	int		m_nSetMode;
	// ダイアログ データ
	//{{AFX_DATA(CDialogPtnCreate)
	enum { IDD = IDD_PTN_CREATE };
	CEdit	m_edText;
	CComboBox	m_cbSetMode;
	CComboBox	m_cbUnit;
	CComboBox	m_cbOder;
	CComboBox	m_cbFont;
	BOOL	m_bTategaki;
	double	m_dFlatPar;
	double	m_dPitch;
	double	m_dSize;
	double	m_dSlopRag;
	CString	m_csText;
	double	m_dThickPar;
	BOOL	m_bRoundMode;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogPtnCreate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogPtnCreate)
	afx_msg void OnReset();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCbunit();
	afx_msg void OnSelchangeCboder();
	afx_msg void OnSelchangeCbfont();
	afx_msg void OnSelchangeCbsetmode();
	afx_msg void OnSetfocusEdtext();
	afx_msg void OnKillfocusEdtext();
	afx_msg void OnSetbtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGPTNCR_H__77C50202_4E6C_11D2_99B2_004026171D2B__INCLUDED_)
