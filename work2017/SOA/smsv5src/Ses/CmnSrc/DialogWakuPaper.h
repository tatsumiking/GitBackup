
#pragma once

// DlgWkPpr.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuPaper ダイアログ
class CTonView;

class CDialogWakuPaper : public CDialog
{
// コンストラクション
public:
	CTonView	*m_pcView;

	CDialogWakuPaper(CWnd* pParent = NULL);   // 標準のコンストラクタ
	void CBInitPaperName();
	void GetPaperElement(LPTSTR srchname);
	void SavePaperElement(LPTSTR srchname);
	void DelPaperName(LPTSTR srchname);
	void BackupPaperName();

// ダイアログ データ
	//{{AFX_DATA(CDialogWakuPaper)
	enum { IDD = IDD_WAKU_PAPER };
	double	m_dPaperHight;
	double	m_dPaperSizeX;
	double	m_dPaperSizeY;
	double	m_dPaperSX;
	double	m_dPaperSY;
	double	m_dPaperWidth;
	CString	m_csPaperName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogWakuPaper)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWakuPaper)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangePapername();
	afx_msg void OnSave();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

