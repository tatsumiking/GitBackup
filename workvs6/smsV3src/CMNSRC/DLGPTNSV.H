#if !defined(AFX_DLGPTNSV_H__D45D8422_4F42_11D2_99B2_004026171D2B__INCLUDED_)
#define AFX_DLGPTNSV_H__D45D8422_4F42_11D2_99B2_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgPtnSv.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnSave ダイアログ
#define	SPTNSIZE	200
#define SPTNSPACE	24
//#define	SPTNSIZE	128
//#define	SPTNSIZE	64
//#define SPTNSPACE	16

class CDialogPtnSave : public CDialog
{
// コンストラクション
public:
	int m_nStartY;
	int m_nStartX;
	void ComboInitSetItem();
	void ListInitSetItem();
	int m_nFartFlag;
	int m_nCrtTop;
	int m_nDlgHight;
	int m_nDlgWidth;
	int m_nSPtnMax;
	int m_nSlctidx;
	CDialogPtnSave(CWnd* pParent = NULL);   // 標準のコンストラクタ
	void DrawImage(CDC *pcDC, LPSTR lpBmpFname, int ndsx, int ndsy, int ndw, int ndh);

// ダイアログ データ
	//{{AFX_DATA(CDialogPtnSave)
	enum { IDD = IDD_PTNSAVE };
	CButton	m_bntPrePage;
	CButton	m_bntNextPage;
	CStatic	m_stTop;
	CStatic	m_stFrame;
	CComboBox	m_cmbPtnGrup;
	CListBox	m_lstPtn;
	CString	m_csPtnGrup;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogPtnSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogPtnSave)
	afx_msg void OnSelchangeLstsptn();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCmbptngrup();
	afx_msg void OnBtnNextpg();
	afx_msg void OnBtnPrepg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGPTNSV_H__D45D8422_4F42_11D2_99B2_004026171D2B__INCLUDED_)
