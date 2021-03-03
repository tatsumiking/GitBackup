#if !defined(AFX_DLGPTNLU_H__5D2C5F22_36E7_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGPTNLU_H__5D2C5F22_36E7_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgPtnLu.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnLineup ダイアログ

class CDialogPtnLineup : public CDialog
{
// コンストラクション
public:
	CDialogPtnLineup(CWnd* pParent = NULL);   // 標準のコンストラクタ

	int	m_nSlctMode;
// ダイアログ データ
	//{{AFX_DATA(CDialogPtnLineup)
	enum { IDD = IDD_PTN_LINEUP };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogPtnLineup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogPtnLineup)
	afx_msg void OnMode1();
	afx_msg void OnMode2();
	afx_msg void OnMode3();
	afx_msg void OnMode4();
	afx_msg void OnMode5();
	afx_msg void OnMode6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGPTNLU_H__5D2C5F22_36E7_11D2_A0A2_444553540000__INCLUDED_)

