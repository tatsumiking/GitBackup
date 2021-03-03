#if !defined(AFX_DIALOGTOOLOBJECTBAR_H__DB7CB941_CD8E_11D2_A711_00A0B0086478__INCLUDED_)
#define AFX_DIALOGTOOLOBJECTBAR_H__DB7CB941_CD8E_11D2_A711_00A0B0086478__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlTolBar.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogToolObjectBar ダイアログ

class CTonView;

class CDialogToolObjectBar : public CMyDialogBar
{
// コンストラクション
public:
	CTonView	*m_pcView;
	int	m_nFirst;
public:
	CDialogToolObjectBar(CWnd* pParent = NULL);   // 標準のコンストラクタ
public:
	void SetItemElement();
	void SetCrtView(CTonView * pcView);
	void SetButtonElement();

// ダイアログ データ
	//{{AFX_DATA(CDialogToolObjectBar)
	enum { IDD = IDD_TOOLOBJECTBAR };
	CBitmapButton	m_btnObjSelect;
	CBitmapButton	m_btnObjFigu;
	CBitmapButton	m_btnObjText;
	CBitmapButton	m_btnPaintExec;
	CBitmapButton	m_btnObjDelete;
	CBitmapButton	m_btnCrdntOneUp;
	CBitmapButton	m_btnFileInport;
	CBitmapButton	m_btnCrdntOneDown;
	CBitmapButton	m_btnZoomArea;
	CBitmapButton	m_btnFilePrint;
	CBitmapButton	m_btnClrPallet;
	CBitmapButton	m_btnFileEnvset;
	CBitmapButton	m_btnFileDelete;
	CBitmapButton	m_btnRedraw;
	CBitmapButton	m_btnSystemHelp;
	CBitmapButton	m_btnCrcldata1;
	CBitmapButton	m_btnCrcldata2;
	CBitmapButton	m_btnCrcldata3;
	CBitmapButton	m_btnRectdata4;
	CBitmapButton	m_btnRectdata5;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogToolObjectBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogToolObjectBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnObjSelect();
	afx_msg void OnObjFigu();
	afx_msg void OnObjText();
	afx_msg void OnPaintExec();
	afx_msg void OnObjDelete();
	afx_msg void OnCrdntOneup();
	afx_msg void OnFileInport();
	afx_msg void OnCrdntOnedown();
	afx_msg void OnZoomArea();
	afx_msg void OnFilePrint();
	afx_msg void OnClrPallet();
	afx_msg void OnFileEnvset();
	afx_msg void OnFileDelete();
	afx_msg void OnRedraw();
	afx_msg void OnSystemHelp();
	afx_msg void OnCrcldata1();
	afx_msg void OnCrcldata2();
	afx_msg void OnCrcldata3();
	afx_msg void OnRectdata4();
	afx_msg void OnRectdata5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DIALOGTOOLOBJECTBAR_H__DB7CB941_CD8E_11D2_A711_00A0B0086478__INCLUDED_)

