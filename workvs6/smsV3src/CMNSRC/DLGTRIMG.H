#if !defined(AFX_DLGTRIMG_H__8C61DD42_2834_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGTRIMG_H__8C61DD42_2834_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgTrImg.h : ヘッダー ファイル
//
#include	"mydialog.h"
/////////////////////////////////////////////////////////////////////////////
// CDialogTraceImage ダイアログ

class CDialogTraceImage : public CMyDialog
{
// コンストラクション
public:
	CDialogTraceImage(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CDialogTraceImage();

	CTonView*	m_pcView;
	COLORREF	m_clrPicup;
	char		m_szBmpFname[FNAMEMAX];
	HANDLE		m_hDib;

public:
	void ChengRGBQUAD();
	void SetImage(LPSTR lpBmpFname);
	void GetImageAreaSize(LPDBL xsize, LPDBL ysize);
	void PicupClrDraw();
	virtual void KeyIn(UINT nChar, UINT nID);
	void SetItemElement();
	void InitItemElement();
	void SetClr(COLORREF picupClr);

// ダイアログ データ
	int		m_nFureLimit;
	//{{AFX_DATA(CDialogTraceImage)
	enum { IDD = IDD_TRACE_IMAGE };
	CSliderCtrl	m_slidAreaLimit;
	int		m_nAreaLimit;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogTraceImage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogTraceImage)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnChangeEdit1();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLineTrace();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGTRIMG_H__8C61DD42_2834_11D2_A0A2_444553540000__INCLUDED_)
