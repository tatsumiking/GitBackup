
#pragma once

// DlgStrIn.h : ヘッダー ファイル
//

#include	"../CmnSrc/MyDialog.h"
#include	"../CmnSrc/MyButton.h"
#include	"../CmnSrc/MyEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgInput ダイアログ

class CDialogStrgInput : public CMyDialog
{
// コンストラクション
public:
	void SetFileToStrgText();
	void SampleTextOut();
	void InitFontList();
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void SetStrgTextToFile();
	CDialogStrgInput(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CDialogStrgInput();

	virtual void KeyIn(UINT nTCHAR, UINT nID);

	int		m_nFirstFlag;
	HGLOBAL hEditBuf;
	LPTSTR	lpEditBuf;
	CMyEdit* m_pcEdit;
	HIMC	m_hImc;
	CTonView*	m_pcView;

// ダイアログ データ
	//{{AFX_DATA(CDialogStrgInput)
	enum { IDD = IDD_STRG_INPUT };
	CMyButton	m_btnCancel;
	CMyButton	m_btnCreate;
	BOOL	m_fCheckGyaku;
	BOOL	m_fCheckTategaki;
	double	m_dHight;
	double	m_dMojiSpace;
	CString	m_csSmplText;
	CString	m_csFontName;
	double	m_dStartX;
	double	m_dStartY;
	double	m_dStrgLen;
	double	m_dStrgSlop;
	double	m_dStrgSpace;
	double	m_dWidth;
	CString	m_csStrgOrder;
	BOOL	m_fCheckMojiData;
	CString	m_stStrgSize;
	double	m_dTrnsLimit;
	BOOL	m_fBold;
	BOOL	m_fItric;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogStrgInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogStrgInput)
	afx_msg void OnDestroy();
	afx_msg void OnCreate();
	afx_msg void OnCkgyaku();
	afx_msg void OnSelchangeCbfontname();
	afx_msg void OnUpdateEdsmpltext();
	afx_msg void OnPaint();
	afx_msg void OnEditchangeCbfontname();
	virtual BOOL OnInitDialog();
	afx_msg void OnCkmojidata();
	afx_msg void OnEnd();
	afx_msg void OnCancel();
	afx_msg void OnInpend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

