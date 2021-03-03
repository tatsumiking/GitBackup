#if !defined(AFX_DLGWKATR_H__4DD4BA42_FBBE_11D1_A0A2_444553540000__INCLUDED_)
#define AFX_DLGWKATR_H__4DD4BA42_FBBE_11D1_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgWkAtr.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuAddAtr ダイアログ

class CDialogWakuAddAtr : public CDialog
{
// コンストラクション
public:
	CScrollView*	m_pcView;
	HIMC m_hImc;
	CDialogWakuAddAtr(CWnd* pParent = NULL);   // 標準のコンストラクタ
public:
	virtual ~CDialogWakuAddAtr();

// ダイアログ データ
	//{{AFX_DATA(CDialogWakuAddAtr)
	enum { IDD = IDD_WAKUADDATR };
	CString	m_csInputStrg;
	CString	m_csMatching;
	double	m_dTrns;
	BOOL	m_nChkFixed;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogWakuAddAtr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWakuAddAtr)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGWKATR_H__4DD4BA42_FBBE_11D1_A0A2_444553540000__INCLUDED_)

