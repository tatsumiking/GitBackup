
#pragma once

// DialogSizeList.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSizeList ダイアログ

class CDialogSizeList : public CDialog
{
// コンストラクション
public:
	CDialogSizeList(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogSizeList)
	enum { IDD = IDD_SIZE_LIST };
	double	m_dXSize;
	double	m_dYSize;
	int		m_nYCount;
	int		m_nXCount;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogSizeList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogSizeList)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

