
#pragma once

// DlgPlOut.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPlotOut ダイアログ

class CDialogPlotOut : public CDialog
{
// コンストラクション
public:
	void InitDataCmb();
	void InitFileCmb();
	CDialogPlotOut(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	int		m_nFileNo;
	int		m_nDataNo;
	//{{AFX_DATA(CDialogPlotOut)
	enum { IDD = IDD_PLOTOUT };
	CComboBox	m_cmbFile;
	CComboBox	m_cmbData;
	BOOL	m_fHomeStop;
	BOOL	m_fLineOut;
	BOOL	m_fOmitOut;
	CString	m_csCmbData;
	CString	m_csCmbFile;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogPlotOut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogPlotOut)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbdata();
	afx_msg void OnSelchangeCmbfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

