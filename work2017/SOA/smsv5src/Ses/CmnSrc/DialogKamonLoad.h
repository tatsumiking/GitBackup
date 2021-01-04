
#pragma once

// DlgKmnLd.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogKamonLoad ダイアログ

#define	LISTMAX	10

//C 家紋集呼出ダイアログクラス
class CDialogKamonLoad : public CDialog
{
// コンストラクション
public:
	void KamnSelectDisp(int idx);
	void NameListInit();
	CFont* m_pcfontView;
	CFont* m_pcoldFont;
	CDialogKamonLoad(CWnd* pParent = NULL);   // 標準のコンストラクタ

public:
	TCHAR m_szKamonChar[16];
	TCHAR m_szKamonFont[RECMAX];

// ダイアログ データ
	//{{AFX_DATA(CDialogKamonLoad)
	enum { IDD = IDD_KAMON_LOAD };
	double	m_dKmnSize;
	CString	m_csSrchChar;
	CString	m_csSrchStrg;
	CString	m_csKmnView;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogKamonLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogKamonLoad)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeSrchChar();
	afx_msg void OnChangeSrchstrg();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeNamelist();
	afx_msg void OnDeltaposSpkmnsize(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

