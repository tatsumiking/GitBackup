// prtctDlg.h : ヘッダー ファイル
//

#if !defined(AFX_PRTCTDLG_H__71320367_D9FE_11D3_9F64_0000E835797C__INCLUDED_)
#define AFX_PRTCTDLG_H__71320367_D9FE_11D3_9F64_0000E835797C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPrtctDlg ダイアログ

class CPrtctDlg : public CDialog
{
// 構築
public:
	void ObjectCopy();
	char m_szDriveName[QRECMAX];
	int m_nDriveNo;
	char m_szSetLabel[RECMAX];	
	char m_szDelLabel[RECMAX];	
	CPrtctDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	HANDLE	m_hDev;
	char	m_szExePath[FNAMEMAX];
	//{{AFX_DATA(CPrtctDlg)
	enum { IDD = IDD_PRTCT_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPrtctDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

protected:
	HICON m_hIcon;

// インプリメンテーション
protected:
	void InitExePath(LPSTR lpExePath);
	void ReadLabel(LPSTR buf);
	void WriteLabel(LPSTR buf);
	void SetDriveNo();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPrtctDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnprtctset();
	afx_msg void OnBtnprtctreset();
	afx_msg void OnBtnset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PRTCTDLG_H__71320367_D9FE_11D3_9F64_0000E835797C__INCLUDED_)
