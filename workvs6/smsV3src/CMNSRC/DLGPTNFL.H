#if !defined(AFX_DLGPTNFL_H__4176BEA4_4E35_11D2_99B2_004026171D2B__INCLUDED_)
#define AFX_DLGPTNFL_H__4176BEA4_4E35_11D2_99B2_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgPtnFL.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnFiguLine ダイアログ

class CDialogPtnFiguLine : public CDialog
{
// コンストラクション
public:
	CDialogPtnFiguLine(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogPtnFiguLine)
	enum { IDD = IDD_PTNFIGU_LINE };
	double	m_dSX;
	double	m_dSY;
	double	m_dEX;
	double	m_dEY;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogPtnFiguLine)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogPtnFiguLine)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGPTNFL_H__4176BEA4_4E35_11D2_99B2_004026171D2B__INCLUDED_)
