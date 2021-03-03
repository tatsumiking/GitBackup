#if !defined(AFX_DLGPTNFE_H__4176BEA2_4E35_11D2_99B2_004026171D2B__INCLUDED_)
#define AFX_DLGPTNFE_H__4176BEA2_4E35_11D2_99B2_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgPtnFE.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnFiguEArc ダイアログ

class CDialogPtnFiguEArc : public CDialog
{
// コンストラクション
public:
	CDialogPtnFiguEArc(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogPtnFiguEArc)
	enum { IDD = IDD_PTNFIGU_EARC };
	double	m_dCX;
	double	m_dCY;
	double	m_dWidth;
	double	m_dHight;
	double	m_dSRag;
	double	m_dERag;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogPtnFiguEArc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogPtnFiguEArc)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGPTNFE_H__4176BEA2_4E35_11D2_99B2_004026171D2B__INCLUDED_)
