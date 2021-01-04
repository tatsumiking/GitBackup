
#pragma once

// DSizeTrns.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSizeTrns ダイアログ

class CDialogSizeTrns : public CDialog
{
// コンストラクション
public:
	CDialogSizeTrns(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CTonView	*m_pcView;
	CTonDoc		*m_pcDoc;
	//{{AFX_DATA(CDialogSizeTrns)
	enum { IDD = IDD_SIZETRNS };
	CButton		m_btnProperty;
	CComboBox	m_cmbPlotName;
	CString	m_csDstXSize;
	CString	m_csDstYSize;
	CString	m_csSrcXSize;
	CString	m_csSrcYSize;
	CString	m_csPlotName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogSizeTrns)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void SetBtnEnable();
	void InitLoadFirstWakuData();
	void InitCmbPlotName();

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogSizeTrns)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbplotname();
	afx_msg void OnDeltaposSpndstxsize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpndstysize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpnsrcxsize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpnsrcysize(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnBtnproperty();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

