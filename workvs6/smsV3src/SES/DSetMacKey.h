#if !defined(AFX_DSETMACKEY_H)
#define AFX_DSETMACKEY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DSetMacKey.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSetMacKey ダイアログ

class CDialogSetMacKey : public CDialog
{
// コンストラクション
public:
	CDialogSetMacKey(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogSetMacKey)
	enum { IDD = IDD_SETMACKEY };
	CString	m_csChkKey;
	CString	m_csSubKey;
	CString	m_csMsg;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogSetMacKey)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogSetMacKey)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DSETMACKEY_H)
