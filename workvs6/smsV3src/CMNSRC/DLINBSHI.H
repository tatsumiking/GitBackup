#if !defined(AFX_DLINBSHI_H__DF2AE9E1_AD8A_11D3_9DFF_00A0B0086478__INCLUDED_)
#define AFX_DLINBSHI_H__DF2AE9E1_AD8A_11D3_9DFF_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlInBshi.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogInnumBoshi ダイアログ

class CDialogInnumBoshi : public CDialog
{
// コンストラクション
public:
	CDialogInnumBoshi(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	LPSTR m_lpTitle;
	LPSTR m_lpRetryText;
	LPSTR m_lpTitleText;
	LPSTR m_lpInfoText;
	LPSTR m_lpUnitText;
	CTonView*	m_pcView;
	int	m_nFirst;
	//{{AFX_DATA(CDialogInnumBoshi)
	enum { IDD = IDD_INNUMBOSHI };
	CButton	m_btnRetry;
	CEdit	m_edInputNum;
	CStatic	m_stUnit;
	CStatic	m_stTitle;
	CStatic	m_stInfo;
	double	m_dInputNum;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogInnumBoshi)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void SetRetryText(LPSTR lpRetryText);
	void SetInfoText(LPSTR lpInfoText);
	void SetTitleText(LPSTR lpTitleText);
	void SetUnitText(LPSTR lpUnitText);
	void SetDialogTitle(LPSTR lpTitle);

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogInnumBoshi)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnRetry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLINBSHI_H__DF2AE9E1_AD8A_11D3_9DFF_00A0B0086478__INCLUDED_)
