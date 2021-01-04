
#pragma once

// DlgMjSv.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiSave ダイアログ

//C 文字保存ダイアログクラス
class CDialogMojiSave : public CDialog
{
// コンストラクション
public:
	void FontCBInit();
	CDialogMojiSave(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogMojiSave)
	enum { IDD = IDD_MOJISAVE };
	CString	m_csFontName;
	CString	m_csMojiCode;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogMojiSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogMojiSave)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

