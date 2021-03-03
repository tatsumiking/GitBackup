#if !defined(DSETPROTECT_H)
#define DSETPROTECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DSetProtect.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSetProtect ダイアログ

class CDialogSetProtect : public CDialog
{
// コンストラクション
public:
	CDialogSetProtect(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogSetProtect)
	enum { IDD = IDD_SETPROTECT };
	CString	m_csPCID;
	CString	m_csSetKey;
	CString	m_csMsg;
	CString	m_csMsg2;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogSetProtect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogSetProtect)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(DSETPROTECT_H)
