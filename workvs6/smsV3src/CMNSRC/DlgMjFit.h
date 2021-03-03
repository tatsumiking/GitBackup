#if !defined(AFX_DIALOGMOJIFITING_H__12EB70E1_19AE_11D3_A712_00A0B0086478__INCLUDED_)
#define AFX_DIALOGMOJIFITING_H__12EB70E1_19AE_11D3_A712_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMjFit.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiFiting ダイアログ

class CDialogMojiFiting : public CDialog
{
// コンストラクション
public:
	void SetItemElement();
	void GetItemElement();
	CDialogMojiFiting(CWnd* pParent = NULL);   // 標準のコンストラクタ

	CTonView*	m_pcView;
	CBitmapButton	m_btnBig;
	CBitmapButton	m_btnSmall;
	CBitmapButton	m_btnRLBig;
	CBitmapButton	m_btnRLSmall;
	CBitmapButton	m_btnUDSmall;
	CBitmapButton	m_btnUDBig;
	CBitmapButton	m_btnUpMove;
	CBitmapButton	m_btnDownMove;
	CBitmapButton	m_btnLeftMove;
	CBitmapButton	m_btnRightMove;
// ダイアログ データ
	//{{AFX_DATA(CDialogMojiFiting)
	enum { IDD = IDD_MOJI_FITING };
	double	m_dStep;
	double	m_dXSize;
	double	m_dYSize;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogMojiFiting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogMojiFiting)
	afx_msg void OnBtnBig();
	afx_msg void OnBtnSmall();
	afx_msg void OnBtnRLBig();
	afx_msg void OnBtnRLSmall();
	afx_msg void OnBtnUDBig();
	afx_msg void OnBtnUDSmall();
	afx_msg void OnBtnUpMove();
	afx_msg void OnBtnDownMove();
	afx_msg void OnBtnLeftMove();
	afx_msg void OnBtnRightMove();
	afx_msg void OnReset();
	afx_msg void OnSet();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpstep(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DIALOGMOJIFITING_H__12EB70E1_19AE_11D3_A712_00A0B0086478__INCLUDED_)
