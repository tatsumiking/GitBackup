
#pragma once

// DlgImgSize.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogImageSize ダイアログ

class CDialogImageSize : public CDialog
{
// コンストラクション
public:
	CDialogImageSize(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogImageSize)
	enum { IDD = IDD_IMAGESIZE };
	BOOL	m_nNoWakuOut;
	double	m_dPrntBlkXStart;
	double	m_dPrntBlkYStart;
	double	m_dPrntBlkXSize;
	double	m_dPrntBlkYSize;
	double	m_dPrntBlkXSpace;
	double	m_dPrntBlkYSpace;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogImageSize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogImageSize)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

