#if !defined(AFX_DLCLRBAR_H__0E4A8FC2_0DA8_11D2_8685_444553540000__INCLUDED_)
#define AFX_DLCLRBAR_H__0E4A8FC2_0DA8_11D2_8685_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlClrBar.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogClrBar ダイアログ

//#define XSPACE 22
//#define XSIZE 18
#define TOPSX 4
#define TOPSY 4
//#define XSPACE 36
//#define XSIZE 32
#define YSIZE 18
#define PLTMAX 50
#define MOVEPLT	10
#define XSIZESLCT	18
#define YSIZESLCT	18

class CTonView;

class CDialogClrBar : public CDialogBar
{
// コンストラクション
public:
	void SetOutLineClr();
	void SetFillClr();
	CDialogClrBar(CWnd* pParent = NULL);   // 標準のコンストラクタ

	CStatic	m_stTop;
	CBitmapButton	cPreBtn;
	CBitmapButton	cNextBtn;
	CBitmapButton	cPreBtn2;
	CBitmapButton	cNextBtn2;
	CBitmapButton	cNonClrBtn;
	CRect	m_rectPre;
	CRect	m_rectNext;
	CRect	m_rectPre2;
	CRect	m_rectNext2;
	CRect	m_rectNonClr;
	int m_nTopSx, m_nTopSy;
	int m_nFirstFlag;
	int	m_nCrtPltTopNo;
	int	m_nCrtFillPlt;
	int	m_nCrtLinePlt;
	DBL	m_dCrtThick;
	DBL	m_dXSpace;
	DBL	m_dXSize;

	void PltListInit();
	void SetItemElement();	
	void DrawSelectPaletBoxs(CPaintDC *dc);
// ダイアログ データ
	//{{AFX_DATA(CDialogClrBar)
	enum { IDD = IDD_CLRTOOLBAR };
	CString	m_csPltFilename;
	double	m_dLineThick;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogClrBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogClrBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNextbtn();
	afx_msg void OnPrebtn();
	afx_msg void OnNextbtn2();
	afx_msg void OnPrebtn2();
	afx_msg void OnSelchangePltcombo();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChangeThick();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDeltaposSpthick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLCLRBAR_H__0E4A8FC2_0DA8_11D2_8685_444553540000__INCLUDED_)
