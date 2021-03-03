#if !defined(DSETSP_H_)
#define DSETSP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DSetSp.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSetSpace ダイアログ

class CDialogSetSpace : public CDialog
{
// コンストラクション
public:
	void SetCrtView(CTonView *pcView);
	CDialogSetSpace(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CTonView	*m_pcView;
	//{{AFX_DATA(CDialogSetSpace)
	enum { IDD = IDD_SETSPACE };
	CString	m_csTopSpace;
	CString	m_csBottomSpace;
	CString	m_csLeftSpace;
	CString	m_csRightSpace;
	BOOL	m_nTonnbo;
	//}}AFX_DATA


// オーバーライド
public:
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogSetSpace)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogSetSpace)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(DSETSP_H_)
