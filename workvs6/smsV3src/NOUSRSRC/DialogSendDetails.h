#if !defined(AFX_DIALOGSENDDETAILS_H__D54DAFA1_A391_11D3_9DFF_00A0B0086478__INCLUDED_)
#define AFX_DIALOGSENDDETAILS_H__D54DAFA1_A391_11D3_9DFF_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSendDetails.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSendDetails ダイアログ

class CDialogSendDetails : public CDialog
{
// コンストラクション
public:
	CDialogSendDetails(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogSendDetails)
	enum { IDD = IDD_SENDDETAILS };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogSendDetails)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogSendDetails)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DIALOGSENDDETAILS_H__D54DAFA1_A391_11D3_9DFF_00A0B0086478__INCLUDED_)
