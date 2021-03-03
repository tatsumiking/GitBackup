#if !defined(AFX_DLGCHKOUT_H__258BD15F_1A21_4F82_94AD_AF16E8B958AF__INCLUDED_)
#define AFX_DLGCHKOUT_H__258BD15F_1A21_4F82_94AD_AF16E8B958AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgChkOut.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogCheckOutput ダイアログ

class CDialogCheckOutput : public CDialog
{
// コンストラクション
public:
	CDialogCheckOutput(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogCheckOutput)
	enum { IDD = IDD_CHECKOUTPUT };
	CStatic	m_stTop;
	CStatic	m_stImg;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogCheckOutput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

public:
	CTonView	*m_pcView;
public:
// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogCheckOutput)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGCHKOUT_H__258BD15F_1A21_4F82_94AD_AF16E8B958AF__INCLUDED_)
