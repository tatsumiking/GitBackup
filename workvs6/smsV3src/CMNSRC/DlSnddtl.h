#if !defined(AFX_DLSNDDTL_H__A1EB3101_8385_11D2_80A6_004026171D2B__INCLUDED_)
#define AFX_DLSNDDTL_H__A1EB3101_8385_11D2_80A6_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlSndDtl.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSendDetails ダイアログ

//C ダイレクト出力詳細設定ダイアログクラス
class CDialogSendDetails : public CDialog
{
// コンストラクション
public:
	void PaperListInit();
	void PaperDefLoad();
	CDialogSendDetails(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogSendDetails)
	enum { IDD = IDD_SENDDETAILS };
	CSpinButtonCtrl	m_spRGNum;
	CSpinButtonCtrl	m_spRCntrst;
	CSpinButtonCtrl	m_spRAAdd;
	CSpinButtonCtrl	m_spGGNum;
	CSpinButtonCtrl	m_spGCntrst;
	CSpinButtonCtrl	m_spGAdd;
	CSpinButtonCtrl	m_spBGNum;
	CSpinButtonCtrl	m_spBCntrst;
	CSpinButtonCtrl	m_spBAdd;
	CComboBox	m_cbProcess;
	CComboBox	m_cbPaperkind;
	double	m_dRgnum;
	double	m_dGgnum;
	double	m_dBgnum;
	double	m_dRadd;
	double	m_dGadd;
	double	m_dBadd;
	double	m_dRcntrst;
	double	m_dGcntrst;
	double	m_dBcntrst;
	//}}AFX_DATA
	int		m_nProcess;
	char	m_szCmdKind[RECMAX];
	char	m_szPaperName[RECMAX];
	DBL		m_dSendTime;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogSendDetails)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogSendDetails)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCbpaperkind();
	afx_msg void OnSelchangeCbprocess();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLSNDDTL_H__A1EB3101_8385_11D2_80A6_004026171D2B__INCLUDED_)
