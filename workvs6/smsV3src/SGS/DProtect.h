//{{AFX_INCLUDES()
#include "..\protekit\prtctl.h"
//}}AFX_INCLUDES
#if !defined(DPROTECT_H_)
#define DPROTECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DProtect.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogProtect ダイアログ

class CDialogProtect : public CDialog
{
// コンストラクション
public:
	CDialogProtect(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	int		m_nDelProtect;
	char	m_szPassword[QRECMAX];
	int		m_nStat;
	int		m_nDays;

	//{{AFX_DATA(CDialogProtect)
	enum { IDD = IDD_PROTECT };
	CPrtCtl	m_prtCtl;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogProtect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogProtect)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(DPROTECT_H)
