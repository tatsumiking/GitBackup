
#pragma once

// DialogBmpRound.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogBmpRound ダイアログ

//C ＢＭＰ回転ダイアログクラス
class CDialogBmpRound : public CDialog
{
// コンストラクション
public:
	int m_nMode;
	CDialogBmpRound(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogBmpRound)
	enum { IDD = IDD_BMP_ROUND };
	CComboBox	m_cbRoundKind;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogBmpRound)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogBmpRound)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

