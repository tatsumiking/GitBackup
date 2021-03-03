// ah32Dlg.h : ヘッダー ファイル
//

#if !defined(AFX_AH32DLG_H__657DBB47_862F_11D3_9DFC_00A0B0086478__INCLUDED_)
#define AFX_AH32DLG_H__657DBB47_862F_11D3_9DFC_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAh32Dlg ダイアログ

class CAh32Dlg : public CDialog
{
// 構築
public:
	CAh32Dlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

public:
	void InitExePath(LPSTR lpExePath);
	void LoadSeting();
	int GetLastCharPos(char ch, LPSTR str);
	void ProgramExec(int no);
	void GetProgranName(int no, char *lpprog);
	BOOL ConvertAhoutRec();

public:
	void InitBtnName();
	CSysEnv	*m_pcSysEnv;
	CFuncLib	*m_pcFuncLib;
	int		m_nFirstFlag;
	char	m_szExePath[FNAMEMAX];
	char	m_szDataFname[FNAMEMAX];
	char	m_szGrup[QRECMAX];
	char	m_szPaper[QRECMAX];
	char	m_szWakuPath[FNAMEMAX];

// ダイアログ データ
	//{{AFX_DATA(CAh32Dlg)
	enum { IDD = IDD_AH32_DIALOG };
	CButton	m_btnOutput10;
	CButton	m_btnOutput9;
	CButton	m_btnOutput8;
	CButton	m_btnOutput7;
	CButton	m_btnOutput6;
	CButton	m_btnOutput5;
	CButton	m_btnOutput4;
	CButton	m_btnOutput3;
	CButton	m_btnOutput2;
	CButton	m_btnOutput1;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAh32Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CAh32Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnBtnExit();
	afx_msg void OnBtnInputedit();
	afx_msg void OnBtnOutput1();
	afx_msg void OnBtnOutput2();
	afx_msg void OnBtnOutput3();
	afx_msg void OnBtnOutput4();
	afx_msg void OnBtnOutput5();
	afx_msg void OnBtnOutput6();
	afx_msg void OnBtnOutput7();
	afx_msg void OnBtnOutput8();
	afx_msg void OnBtnOutput9();
	afx_msg void OnBtnOutput10();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_AH32DLG_H__657DBB47_862F_11D3_9DFC_00A0B0086478__INCLUDED_)
