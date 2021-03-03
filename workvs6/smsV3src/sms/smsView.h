// smsView.h : CTonView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(SMSVIEW_H)
#define SMSVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"..\cmnsrc\cmd_base.h"

class CTonView : public CScrollView
{
protected: // シリアライズ機能のみから作成します。
	CTonView();
	DECLARE_DYNCREATE(CTonView)
public:
	virtual ~CTonView();
	CTonDoc* GetDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// オペレーション
public:
	int		m_nFirst;
	UINT	m_nTimerID;
	int		m_nKeyFlags;
	int		m_nBx, m_nBy;
	int		m_nDrawIn;
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTonView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL
// データ
public:
	//HIMC m_hImc;
	int		m_fAtntion;
	int		m_nBmpCnt;
	CCmdBase *m_pcCmdBase;
	// プリンター出力時に使用する変数
	int	m_nOutCount;
	int	m_nOrientation;
	UINT m_nDivPageLast;
	UINT m_nXDivPage;
	UINT m_nYDivPage;
	int	m_nDivCrtX, m_nDivCrtY;
	DBL	m_dPrnMMXSize;
	DBL	m_dPrnMMYSize;
	DBL	m_dDpiX;
	DBL	m_dDpiY;
	DBL	m_dMMXSize;
	DBL	m_dMMYSize;
	DBL m_dMoveMMX;
	DBL m_dMoveMMY;
	DBL m_dPrnAreaMMX;
	DBL m_dPrnAreaMMY;
	DBL	m_dPrnTimeX, m_dPrnTimeY;
	int m_nScaleTrns;

// インプリメンテーション
public:
	void AreaBackClrFill(CDC *pcDC);
	void ChangeFont(LPSTR lpFont);
	void DeletePrinterDC(CDC *pcDC);
	void CreatePrinterDC(CDC *pcDC, int cpycnt);
	void PlotOutMain(DBL areasx, DBL areasy, DBL areaex, DBL areaey);
	void PloterOutput(int cpycnt);
	void PrinterOutput(int cpycnt);
	void AllWakuSetString(LPSTR lpText);
	UINT CommandExec(UINT event, long x, long y);
	void FillWorkArea(CDC *pcDC);
	void InitArea(CRect rect);
	void PrintEnvLoad(LPSTR lpBinFile);
	void MouseSetCrt(DBL dstx, DBL dsty);
	void InitScrollElement();
	void RedrawFlagOn();
	void DrawWakuInRect(CDC *pcDC, CRect rect);

protected:

// 生成されたメッセージ マップ関数
protected:
	afx_msg LRESULT OnMyImeNotify(WPARAM dwCommand, LPARAM dwData);
	virtual void OnInitialUpdate();
	void CgAdjustCompositionWindow();
	//{{AFX_MSG(CTonView)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // smsView.cpp ファイルがデバッグ環境の時使用されます。
inline CTonDoc* CTonView::GetDocument()
   { return (CTonDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(SMSVIEW_H)
