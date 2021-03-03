// mpsDlg.h : ヘッダー ファイル
//

#if !defined(MPSDLG_H)
#define MPSDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\LEAD10\lead.h"	// ClassView によって追加されました。

/////////////////////////////////////////////////////////////////////////////
// CTonDlg ダイアログ

class CTonDlg : public CDialog
{
// 構築
public:
	CTonDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ
	~CTonDlg();
// ダイアログ データ
public:
	CLead	*m_pcLead;		// LEADツール用ハンドル
    CLTDlgOCXCtl *m_pcLeadDlg;		// LEADツールダイアログ用ハンドル
	char	m_szExePath[FNAMEMAX];
	char	m_szTmpPath[FNAMEMAX];
	char	m_szTitle[RECMAX];
	//{{AFX_DATA(CTonDlg)
	enum { IDD = IDD_MPS_DIALOG };
	CStatic	m_stImg;
	CStatic	m_stTop;
	CString	m_csDstYMM;
	CString	m_csDstXMM;
	CString	m_csSrcYDot;
	CString	m_csSrcXDot;
	CString	m_csFileName;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void InitEnvPath();
	void InitExePath();
	void GetLegalRect(CRect *pcRect);
	void SelecttionBmp();
	int GetParentPath(LPSTR lpPath);
	void BmpCmpConvBytelen(LPSTR lpBmpFName, LPSTR lpCmpFName);
	void CmpBmpConvBytelen(LPSTR lpCmpFName, LPSTR lpBmpFName);
	BOOL IsKanji(UINT hc);
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTonDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnsend();
	afx_msg void OnBtnref();
	afx_msg void OnBtnend();
	afx_msg void OnBtnreload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(MPSDLG_H)
