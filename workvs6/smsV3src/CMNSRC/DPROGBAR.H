#if !defined(AFX_DPROGBAR_H__0A715303_265D_11D4_82D3_00A0B0086478__INCLUDED_)
#define AFX_DPROGBAR_H__0A715303_265D_11D4_82D3_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DProgBar.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogProgresBar ダイアログ

class CDialogProgresBar : public CDialog
{
// コンストラクション
public:
	CDialogProgresBar(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogProgresBar)
	enum { IDD = IDD_PROGBAR };
	CProgressCtrl	m_ProgBar;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogProgresBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogProgresBar)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DPROGBAR_H__0A715303_265D_11D4_82D3_00A0B0086478__INCLUDED_)
