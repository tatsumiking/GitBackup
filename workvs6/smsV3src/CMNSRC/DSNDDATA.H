#if !defined(AFX_DSNDDATA_H__1A473EA1_226E_11D3_A712_00A0B0086478__INCLUDED_)
#define AFX_DSNDDATA_H__1A473EA1_226E_11D3_A712_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DSndData.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSendData ダイアログ

//C ダイレクト出力設定ダイアログクラス
class CDialogSendData : public CDialog
{
// コンストラクション
public:
	void PaperDefLoad(LPSTR szCmdname);
	CDialogSendData(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogSendData)
	enum { IDD = IDD_SENDDATA };
	CSpinButtonCtrl	m_spYMMPaper;
	CSpinButtonCtrl	m_spYMMOut;
	CSpinButtonCtrl	m_spYMMNori;
	CSpinButtonCtrl	m_spXMMPaper;
	CSpinButtonCtrl	m_spXMMOut;
	CSpinButtonCtrl	m_spXMMNori;
	CSpinButtonCtrl	m_spCopy;
	CEdit	m_m_edYMMOut;
	CEdit	m_edXMMOut;
	CComboBox	m_cbCmdKind;
	BOOL	m_bChkX;
	BOOL	m_bChkY;
	double	m_dXMMOut;
	double	m_dYMMOut;
	double	m_dXMMPaper;
	double	m_dYMMPaper;
	double	m_dXMMNori;
	double	m_dYMMNori;
	int		m_nCopyCount;
	CString	m_csSendPath;
	CString	m_csSaverPath;
	//}}AFX_DATA

	int		m_nCmdKind;
	int		m_nXdpi;
	int		m_nYdpi;
	double	m_dStep;
	double	m_dRgnum;
	double	m_dGgnum;
	double	m_dBgnum;
	double	m_dRadd;
	double	m_dGadd;
	double	m_dBadd;
	double	m_dRcntrst;
	double	m_dGcntrst;
	double	m_dBcntrst;
	DBL		m_dSendTime;
	char	m_szCmdKind[RECMAX];
	char	m_szPaperName[RECMAX];
	int		m_nProcess;
	int		m_nFirst;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogSendData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogSendData)
	afx_msg void OnBtndetails();
	afx_msg void OnChangeEdxmmout();
	afx_msg void OnChangeEdymmout();
	afx_msg void OnSelchangeCbcmdkind();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBtnSetslct();
	afx_msg void OnBtnSvslct();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DIALOGDIRECTOUT_H__1A473EA1_226E_11D3_A712_00A0B0086478__INCLUDED_)
