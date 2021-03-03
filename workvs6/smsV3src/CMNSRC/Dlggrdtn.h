#if !defined(AFX_DLGGRDTN_H__6A47F062_2CE8_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGGRDTN_H__6A47F062_2CE8_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgGrdtn.h : ヘッダー ファイル
//
#include	"mydialog.h"
/////////////////////////////////////////////////////////////////////////////
// CDialogGradation ダイアログ

#define	MARKMAX	32
#define PTNMAX	10

//C グラデーション設定ダイアログクラス
class CDialogGradation : public CMyDialog
{
// コンストラクション
public:
	void PltCopy(LPRGBQUAD lpclrgraptn);
	void GraPtnLoad(int ptn);
	void ClrTableReset();
	void SelectMarkDraw(CDC *pcdc, int mark);
	void Create256Dib();
	void SetGraBarPtn();

	
	CTonView*	m_pcView;
	HANDLE	m_hDibGraPtn[PTNMAX];
	int		m_nPtnSx[PTNMAX];
	int		m_nPtnSy[PTNMAX];
	int		m_nBitCount[PTNMAX];
	int		m_nPtnMax;

	COLORREF	m_clrSet[MARKMAX];
	int			m_nSetx[MARKMAX];

	int		m_nCrtSelect;
	int		m_nSelectMark;
	int		m_nTopx, m_nTopy;
	int		m_nXSpace, m_nYSpace;
	int		m_nBarSx, m_nBarEx;
	int		m_nBarSy;

	long	m_sizeDib;
	HANDLE	m_hDib;

	CDialogGradation(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CDialogGradation();
// ダイアログ データ
	//{{AFX_DATA(CDialogGradation)
	enum { IDD = IDD_GRADATION };
	CStatic	m_stTop;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogGradation)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogGradation)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGGRDTN_H__6A47F062_2CE8_11D2_A0A2_444553540000__INCLUDED_)
