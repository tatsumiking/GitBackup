#if !defined(DALGWKSRT_H)
#define DALGWKSRT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgWkSrt.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuSort ダイアログ
class CTonView;

class CDialogWakuSort : public CDialog
{
// コンストラクション
public:
	CTonView*	m_pcView;
	int	m_nFirstFlag;	
	int m_nMouseSetCount;
	int	m_nDlgWidth;
	int	m_nDlgHight;
	int	m_nImgTopX;
	int	m_nImgTopY;
	int m_nWakuMax;
	int	m_nOneLineCount;
	int m_nCrtTop;
	int m_nSlctWaku1;
	int m_nSlctWaku2;
	DBL m_dTrnsTime;
	HANDLE		m_hDib;   // 画像表用バッファハンドル
	ULONG		m_nDibSize;   // 画像表用バッファサイズ

	CDialogWakuSort(CWnd* pParent = NULL);   // 標準のコンストラクタ
	void WakuAllDraw(CDC *pcDC);
	void WakuDraw(CDC *pcDC, DBL sx, DBL sy, LPSTR wakufname);
	void WakuImageDraw(CDC *pcDC, DBL sx, DBL sy, LPSTR wakufname, long size);
	void WakuBoxsDraw(CDC *pcDC, DBL sx, DBL sy, LPSTR wakufname);
	void TrnsTimeSet(DBL paperwidth, DBL paperhight);
	void BoxsDraw(CDC *pcDC, long sx, long sy, long ex, long ey);
	void GetWakuCountMax();
	void GetWakuSize(char *wenvfile, DBL *wakuxsize, DBL *wakuysize);

// ダイアログ データ
	//{{AFX_DATA(CDialogWakuSort)
	enum { IDD = IDD_WAKU_SORT };
	CButton	m_btnNextPage;
	CListBox	m_lstWakuName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogWakuSort)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogWakuSort)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnBeforpage();
	afx_msg void OnNextpage();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangeLstwakuname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(DALGWKSRT_H)
