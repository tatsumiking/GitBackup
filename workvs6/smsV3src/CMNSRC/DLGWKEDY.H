#if !defined(AFX_DIALOGWAKUEDIY_H)
#define AFX_DIALOGWAKUEDIY_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgWkEdy.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuEdiy ダイアログ

class CTonView;

class CDialogWakuEdiy : public CMyDialog
{
// コンストラクション
public:
	CDialogWakuEdiy(CWnd* pParent = NULL);   // 標準のコンストラクタ

public:
	HIMC	m_hImc;
	int		m_nTateYoko;
	CTonView*	m_pcView;
	int		m_nChangeEditID;

public:
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void InitFontList();
	void InitPlotName();
	void InitPaperName();
	void GetPaperSize(LPSTR papername, DBL *mmxsize, DBL *mmysize);
	void CntrKindBottonSet(int cntrkind);

	virtual void KeyIn(UINT nChar, UINT nID);

private:
	LPSTR CrDelFgets(char * buf, int max, FILE * fp);

public:
	void PlotElementGet(LPSTR SrchPlotName);
// ダイアログ データ
	//{{AFX_DATA(CDialogWakuEdiy)
	enum { IDD = IDD_WAKU_EDIY };
	CEdit	m_edMojiSpace;
	CBitmapButton	m_btnCntrKind;
	CMyEdit	m_edWorkYSize;
	CMyEdit	m_edWorkXSize;
	CMyEdit	m_edWidth;
	CMyEdit	m_edHight;
	CMyEdit	m_edText;
	CButton	m_btnTategaki;
	CComboBox	m_cmbPaperName;
	CComboBox	m_cmbPloter;
	CComboBox	m_cmbFont;
	double	m_dHight;
	CString	m_csText;
	double	m_dWidth;
	double	m_dWorkXSize;
	double	m_dWorkYSize;
	CString	m_csFont;
	CString	m_csPloter;
	CString	m_csPaperName;
	BOOL	m_nSpaceKind;
	double	m_dMojiSpace;
	BOOL	m_nYokoMoji;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogWakuEdiy)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWakuEdiy)
	afx_msg void OnBtntategaki();
	afx_msg void OnBtnoption();
	afx_msg void OnBtnpapersave();
	afx_msg void OnBtnprint();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbpapername();
	afx_msg void OnBtnset();
	afx_msg void OnBtnreset();
	afx_msg void OnChangeEdworkxsize();
	afx_msg void OnChangeEdworkysize();
	afx_msg void OnChangeEdhight();
	afx_msg void OnChangeEdwidth();
	afx_msg void OnChangeEdtext();
	afx_msg void OnCntrkind();
	afx_msg void OnSelchangeCmbploter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DIALOGWAKUEDIY_H)

