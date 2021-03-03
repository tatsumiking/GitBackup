// wr32Dlg.h : ヘッダー ファイル
//

#if !defined(AFX_WR32DLG_H__5CDF5607_823E_11D3_BF1B_00A0B0086478__INCLUDED_)
#define AFX_WR32DLG_H__5CDF5607_823E_11D3_BF1B_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWr32Dlg ダイアログ

class CWr32Dlg : public CDialog
{
// 構築
public:
	CWr32Dlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

public:
	void InitExePath(LPSTR lpExePath);
	void LoadSeting();
	void CreateHead(LPSTR wakunum, LPSTR head);
	void ProgramExec();
	void GetProgranName(int no, char *lpprog);
	BOOL ConvertWroutRec();
	void SrchWakuName(LPSTR wakunum, LPSTR waku);
	int DivCsvData(LPSTR csvsp[], LPSTR csvbuf);
	int IsKanji(int hc);

public:
	int		m_nFirstFlag;
	char	m_szExePath[FNAMEMAX];
	char	m_szGrup[QRECMAX];
	char	m_szPaper[QRECMAX];
	char	m_szWakuPath[FNAMEMAX];

// ダイアログ データ
	//{{AFX_DATA(CWr32Dlg)
	enum { IDD = IDD_WR32_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CWr32Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CWr32Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_WR32DLG_H__5CDF5607_823E_11D3_BF1B_00A0B0086478__INCLUDED_)
