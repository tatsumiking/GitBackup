#if !defined(AFX_DLGSTRVS_H__881F35E2_3DFE_11D2_99B2_004026171D2B__INCLUDED_)
#define AFX_DLGSTRVS_H__881F35E2_3DFE_11D2_99B2_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgStrVS.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgVStruct ダイアログ

class CTonView;

class CDialogStrgVStruct : public CDialog
{
// コンストラクション
public:
	void SetItemElement();
	void GetItemElement();
	int m_nSetMode;
	CTonView	*m_pcView;
	CDialogStrgVStruct(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogStrgVStruct)
	enum { IDD = IDD_STRG_VSTRUCT };
	CStatic	m_stTop;
	BOOL	m_chkStrgEqual;
	CStatic	m_stSetMode1;
	CStatic	m_stSetMode2;
	CStatic	m_stSetMode3;
	BOOL	m_chkNoround;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogStrgVStruct)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogStrgVStruct)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGSTRVS_H__881F35E2_3DFE_11D2_99B2_004026171D2B__INCLUDED_)
