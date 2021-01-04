
#pragma once

// DlgGaiji.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogGaiji ダイアログ
#define	GAIJIEDITMAX	40

class CDialogGaiji : public CMyDialog
{
// コンストラクション
public:
	CDialogGaiji(CWnd* pParent = NULL);   // 標準のコンストラクタ

public:
	CMyEdit	*m_pcEdit[GAIJIEDITMAX];
	TCHAR	m_szGaijiText[QRECMAX];
public:
	void IniTCHAREdit();
	void InitTitleCombo();
	void SetGaijiText(int x, int y);

// ダイアログ データ
	//{{AFX_DATA(CDialogGaiji)
	enum { IDD = IDD_GAIJI };
	CStatic	m_stTop;
	CComboBox	m_cmbTitle;
	CString	m_csTitle;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogGaiji)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogGaiji)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

