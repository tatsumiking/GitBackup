#if !defined(AFX_DLGLSTDSP_H__FDE97CA2_D7F3_4E11_ADC1_3CED96040A50__INCLUDED_)
#define AFX_DLGLSTDSP_H__FDE97CA2_D7F3_4E11_ADC1_3CED96040A50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLstDsp.h : ヘッダー ファイル
//

#include	"rdsoutcrct.h"
/////////////////////////////////////////////////////////////////////////////
// CDialogListDisp ダイアログ
#define	CNTTBLMAX	50
#define CNTMINI		6
#define NOCLMSIZE	3

class CDialogListDisp : public CDialog
{
// コンストラクション
public:
	CDialogListDisp(CWnd* pParent = NULL);   // 標準のコンストラクタ
	
// ダイアログ データ
	//{{AFX_DATA(CDialogListDisp)
	enum { IDD = IDD_LISTDISP };
	CListBox	m_lstRecText;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogListDisp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

public:
	CTonView	*m_pcView;
	CTonDoc		*m_pcDoc;
	char	m_szTitle[QRECMAX];
	int		m_nInSort;
	int		m_nExitCall;
	CDaoDatabase	m_dbOutcrct;
	CRdsOutCrct*	m_pcRdsOutCrct;
	DBL	m_dPrntTime;
	DBL	m_nPrntXSize;
	DBL	m_nPrntYSize;
	int	m_nCntTbl[CNTTBLMAX];
// インプリメンテーション
public:
	void InitSetDB();
	void InitPrintEnv(CDC *pcDC);
	void PrintOutMain(CDC *pcDC);
	void EndCrctDB();
	void InitCrctDB();
	void InitRecTextList();
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogListDisp)
	afx_msg void OnBtnimagedisp();
	afx_msg void OnBtnprintout();
	afx_msg void OnBtnexit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGLSTDSP_H__FDE97CA2_D7F3_4E11_ADC1_3CED96040A50__INCLUDED_)
