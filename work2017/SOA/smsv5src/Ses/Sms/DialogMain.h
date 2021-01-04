
#pragma once

// DlgMain.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMain ダイアログ

class CDialogMain : public CDialog
{
// コンストラクション
public:
	CDialogMain(CWnd* pParent = NULL);   // 標準のコンストラクタ
// ダイアログ データ
	//{{AFX_DATA(CDialogMain)
	enum { IDD = IDD_MAIN };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogMain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// データー
public:
	CTonView	*m_pcView;	
	HANDLE	m_hDib;
	int		m_nWidth;
	int		m_nHight;

	int		m_nTimer;
	UINT	m_nTimerID;
	HANDLE	m_hProcess;

// インプリメンテーション
public:
	void InitShowHideBtn();
	void ProgExec(LPTSTR lpExexProg);
	void DibLoad();
	void ProgExitLoop();

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogMain)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnInput();
	afx_msg void OnBtnWakuedit();
	afx_msg void OnBtnGaijiedit();
	afx_msg void OnBtnOption();
	afx_msg void OnBtnInstr();
	afx_msg void OnBtnKden();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

