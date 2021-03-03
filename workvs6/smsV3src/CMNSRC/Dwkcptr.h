#if !defined(DWKCPTR_H)
#define DWKCPTR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DWkCptr.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuCopyTrns ダイアログ
class CTonView;

class CDialogWakuCopyTrns : public CDialog
{
// コンストラクション
public:
	CDialogWakuCopyTrns(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CTonView*	m_pcView;
	int	m_nFirstFlag;	
	int	m_nDlgWidth;
	int	m_nDlgHight;
	int	m_nImgTopX;
	int	m_nImgTopY;
	int m_nWakuMax;
	int m_nCrtTop;
	int m_nSlctWaku;
	int	m_nOneLineCount;
	DBL m_dTrnsTime;
	HANDLE		m_hDib;   // 画像表用バッファハンドル
	ULONG		m_nDibSize;   // 画像表用バッファサイズ

	//{{AFX_DATA(CDialogWakuCopyTrns)
	enum { IDD = IDD_WAKU_CPTR };
	CButton	m_btnNextPage;
	CListBox	m_lstWakuName;
	CString	m_csDstFolder;
	CString	m_csDstXSize;
	CString	m_csDstYSize;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogWakuCopyTrns)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	void GetSelectDir(LPSTR lpTitle, LPSTR lpPath);
	void AllResetSelect();
	void WakuAllDraw(CDC *pcDC);
	void WakuDraw(CDC *pcDC, DBL sx, DBL sy, LPSTR wakufname);
	void WakuImageDraw(CDC *pcDC, DBL sx, DBL sy, LPSTR wakufname, long size);
	void WakuBoxsDraw(CDC *pcDC, DBL sx, DBL sy, LPSTR wakufname);
	void TrnsTimeSet(DBL paperwidth, DBL paperhight);
	void BoxsDraw(CDC *pcDC, long sx, long sy, long ex, long ey);
	void GetWakuCountMax();
	void GetWakuSize(char *wenvfile, DBL *wakuxsize, DBL *wakuysize);

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWakuCopyTrns)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBeforpage();
	afx_msg void OnNextpage();
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangeLstwakuname();
	afx_msg void OnBtndstfolder();
	afx_msg void OnDeltaposSpndstxsize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpndstysize(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(DWKCPTR_H)


