#if !defined(AFX_DLGSCRCL_H__6CC4A623_4DD7_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGSCRCL_H__6CC4A623_4DD7_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgSCrcl.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSizeCrcl ダイアログ

class CDialogSizeCrcl : public CDialog
{
// コンストラクション
public:
	CDialogSizeCrcl(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogSizeCrcl)
	enum { IDD = IDD_SIZE_CRCL };
	double	m_dCntrX;
	double	m_dCntrY;
	double	m_dHight;
	double	m_dWidth;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogSizeCrcl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogSizeCrcl)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGSCRCL_H__6CC4A623_4DD7_11D2_A0A2_444553540000__INCLUDED_)
