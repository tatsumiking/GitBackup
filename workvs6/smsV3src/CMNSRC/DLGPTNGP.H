#if !defined(AFX_DLGPTNGP_H__6CC4A624_4DD7_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGPTNGP_H__6CC4A624_4DD7_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgPtnGp.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnGrup ダイアログ

class CDialogPtnGrup : public CDialog
{
// コンストラクション
public:
	CDialogPtnGrup(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogPtnGrup)
	enum { IDD = IDD_PTN_GRUP };
	CComboBox	m_cbGrupName;
	CString	m_csGrupName;
	double	m_dPaperSizeX;
	double	m_dPaperSizeY;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogPtnGrup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogPtnGrup)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeGrupname();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGPTNGP_H__6CC4A624_4DD7_11D2_A0A2_444553540000__INCLUDED_)

