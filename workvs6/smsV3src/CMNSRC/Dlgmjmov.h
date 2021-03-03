#if !defined(AFX_DLGMJMOV_H__4DDF98A2_3B99_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGMJMOV_H__4DDF98A2_3B99_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgMjMov.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiMove ダイアログ
class CTonView;

//C 文字移動ダイアログクラス
class CDialogMojiMove : public CDialog
{
// コンストラクション
public:
	void SetItemElement();
	void GetItemElement();
	CDialogMojiMove(CWnd* pParent = NULL);   // 標準のコンストラクタ

	CTonView*	m_pcView;
	CBitmapButton	m_btnLeft;
	CBitmapButton	m_btnRight;
	CBitmapButton	m_btnUp;
	CBitmapButton	m_btnDown;
// ダイアログ データ
	//{{AFX_DATA(CDialogMojiMove)
	enum { IDD = IDD_MOJI_MOVE };
	double	m_edStepmm;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogMojiMove)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogMojiMove)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnMovedown();
	afx_msg void OnBtnMoveleft();
	afx_msg void OnBtnMoveright();
	afx_msg void OnBtnMoveup();
	afx_msg void OnDlgReset();
	afx_msg void OnDlgSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGMJMOV_H__4DDF98A2_3B99_11D2_A0A2_444553540000__INCLUDED_)
