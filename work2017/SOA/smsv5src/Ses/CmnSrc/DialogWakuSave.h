
#pragma once

// DlgWkSv.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuSave ダイアログ
class CTonView;

class CDialogWakuSave : public CDialog
{
// コンストラクション
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

	CDialogWakuSave(CWnd* pParent = NULL);   // 標準のコンストラクタ
	void WakuAllDraw(CDC *pcDC);
	void WakuDraw(CDC *pcDC, int sx, int sy, LPTSTR wakufname);
	void WakuImageDraw(CDC *pcDC, int sx, int sy, LPTSTR wakufname, long size);
	void WakuBoxsDraw(CDC *pcDC, DBL sx, DBL sy, LPTSTR wakufname);
	void TrnsTimeSet(DBL paperwidth, DBL paperhight);
	void BoxsDraw(CDC *pcDC, long sx, long sy, long ex, long ey);
	void GetWakuCountMax();
	void GetWakuSize(TCHAR *wenvfile, DBL *wakuxsize, DBL *wakuysize);

// ダイアログ データ
	//{{AFX_DATA(CDialogWakuSave)
	enum { IDD = IDD_WAKU_SAVE };
	CButton	m_btnNextPage;
	CListBox	m_lstWakuName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。


	//{{AFX_VIRTUAL(CDialogWakuSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWakuSave)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnBeforpage();
	afx_msg void OnNextpage();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangeLstwakuname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

