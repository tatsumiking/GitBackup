#if !defined(AFX_DIALOGSTRGARC_H__EC270A83_9192_11D2_940A_0000E835797C__INCLUDED_)
#define AFX_DIALOGSTRGARC_H__EC270A83_9192_11D2_940A_0000E835797C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DialogStrgArc.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgArc ダイアログ

class CDialogStrgArc : public CDialog
{
// コンストラクション
public:
	void SetItemElement();
	void GetItemElement();
	int m_nSetMode;
	CTonView	*m_pcView;
	CDialogStrgArc(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogStrgArc)
	enum { IDD = IDD_STRG_ARC };
	CStatic	m_stTop;
	CStatic	m_stSetMode3;
	CStatic	m_stSetMode2;
	CStatic	m_stSetMode1;
	BOOL	m_chkStrgEqual;
	BOOL	m_chkNoround;
	double	m_dERag;
	double	m_dSRag;
	double	m_dR;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogStrgArc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogStrgArc)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DIALOGSTRGARC_H__EC270A83_9192_11D2_940A_0000E835797C__INCLUDED_)
