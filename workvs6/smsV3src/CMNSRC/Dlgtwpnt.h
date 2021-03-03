#if !defined(AFX_DLGTWPNT_H__F8B40F03_322E_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGTWPNT_H__F8B40F03_322E_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgTwPnt.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialog2PointSize ダイアログ

//C ２点サイズダイアログクラス
class CDialog2PointSize : public CDialog
{
// コンストラクション
public:
	CDialog2PointSize(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialog2PointSize)
	enum { IDD = IDD_2POINTSIZETRNS };
	double	m_dXSize;
	double	m_dYSize;
	double	m_dLength;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialog2PointSize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialog2PointSize)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGTWPNT_H__F8B40F03_322E_11D2_A0A2_444553540000__INCLUDED_)

