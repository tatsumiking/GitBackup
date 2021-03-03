#if !defined(AFX_DLEPSKLD_H__BE5BBF61_32F0_11D3_A712_00A0B0086478__INCLUDED_)
#define AFX_DLEPSKLD_H__BE5BBF61_32F0_11D3_A712_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlepskld.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogEpsKamonLoad ダイアログ

#define	LISTMAX	10

class CDialogEpsKamonLoad : public CDialog
{
// コンストラクション
public:
	CDialogEpsKamonLoad(CWnd* pParent = NULL);   // 標準のコンストラクタ
public:
	void KamnSelectDisp(int idx);
	void NameListInit();
	void EpsDispLoad(CDC *pcCDC, LPSTR epsname);
	void SkipEpsHead(FILE *fp);
	void DispEpsLoadLoop(FILE *fp, CDC *pcCDC);
	void BezeDraw(CDC *pcCDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	void DispTimeTrns(LPDBL xx, LPDBL yy);
	int NumChk(char buf[], int len);
	BOOL ReadString(FILE *fp, char *buffer, int max);

public:
	char m_szFileName[FNAMEMAX];	
	DBL	m_dDispSx;
	DBL	m_dDispSy;
	DBL	m_dDispSize;
	DBL	m_dTime;
// ダイアログ データ
	//{{AFX_DATA(CDialogEpsKamonLoad)
	enum { IDD = IDD_EPSKAMON_LOAD };
	CEdit	m_edKmnView;
	CListBox	m_lstName;
	double m_dKmnSize;
	CString	m_csSrchChar;
	CString	m_csSrchStrg;
	CString	m_csKmnView;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogEpsKamonLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogEpsKamonLoad)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeSrchchar();
	afx_msg void OnChangeSrchstrg();
	afx_msg void OnSelchangeNamelist();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLEPSKLD_H__BE5BBF61_32F0_11D3_A712_00A0B0086478__INCLUDED_)

