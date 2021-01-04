
#pragma once

// DlgSBoxs.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSizeBoxs ダイアログ

class CDialogSizeBoxs : public CDialog
{
// コンストラクション
public:
	CDialogSizeBoxs(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogSizeBoxs)
	enum { IDD = IDD_SIZE_BOXS };
	double	m_dCntrX;
	double	m_dCntrY;
	double	m_dHight;
	double	m_dWidth;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogSizeBoxs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogSizeBoxs)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

