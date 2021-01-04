
#pragma once

// DWkBar1.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBar1 ダイアログ
class CTonView;

class CDialogWakuBar1 : public CDialogBar
{
// コンストラクション
public:
	CDialogWakuBar1(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CTonApp*	m_pcApp;
	CTonView*	m_pcView;
	int		m_nFirstFlag;
	//{{AFX_DATA(CDialogWakuBar1)
	enum { IDD = IDD_WAKUBAR1 };
	CComboBox	m_cmbPaperName;
	CString		m_csPaperName;
	//}}AFX_DATA


// オーバーライド
public:
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogWakuBar1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void InitWakuPaperCombo();
	void SetCrtView(CTonView * pcView);
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWakuBar1)
	afx_msg void OnSelchangeCmbpaper();
	afx_msg void OnPaint();
	afx_msg void OnWakuPaperAdd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

