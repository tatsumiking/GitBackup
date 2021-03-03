#if !defined(AFX_DLGWKGRP_H__5D2C5F24_36E7_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGWKGRP_H__5D2C5F24_36E7_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgWkGrp.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuGrup ダイアログ

class CDialogWakuGrup : public CDialog
{
// コンストラクション
public:
	CDialogWakuGrup(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	char	m_szGrupPath[FNAMEMAX];
	//{{AFX_DATA(CDialogWakuGrup)
	enum { IDD = IDD_WAKU_GRUP };
	CString	m_csWakuGrupPath;
	CString	m_csWakuGrupName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogWakuGrup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void CheckAddWakuGrup(LPSTR gruppath);
	char* CrDelFgets(char *buf, int max, FILE *fp);
	void InitWakuGrupCombo();

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWakuGrup)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeWkgrupname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGWKGRP_H__5D2C5F24_36E7_11D2_A0A2_444553540000__INCLUDED_)

