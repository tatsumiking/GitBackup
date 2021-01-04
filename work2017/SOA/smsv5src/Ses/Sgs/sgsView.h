// sgsView.h : CSgsView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#pragma once


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTonView : public CScrollView
{
protected: // シリアライズ機能のみから作成します。
	CTonView();
	DECLARE_DYNCREATE(CTonView)

// アトリビュート
public:
	CTonDoc* GetDocument();
	//   m_pSlctOle は現在の CTonCntrItem に対する選択を保持しています。
	//  多くのアプリケーションでこのようなメンバ変数は、複数の選択や CTonCntrItem
	//  オブジェクトではないオブジェクトの選択を十分に表現できません。
	//  この機構は選択機能を応用しようとするプログラマの理解を助けるた
	//  めに組み込まれています。

	// TODO: この選択方法をアプリケーションに適した方法に置き換えてください。
// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTonView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void OnPrepareDC(CDC* pcDC, CPrintInfo* pInfo = NULL);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pcDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pcDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CTonView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	UINT CommandExec(UINT event, long x, long y);

// 生成されたメッセージ マップ関数
public:
	BOOL m_bDispImage; // TRUE イメージ表示　FALSE 枠のみ表示
	BOOL m_bEpsonYoko; // TRUE 印刷用横回転　FALSE 横長でもそのまま出力
	TCHAR m_szInfoEnv[RECMAX];
	int	m_nCursorKind;
	int m_nCrtWaku;
	int m_nSubCommand;
	int	m_nNextCommand;
	int	m_nDrawIn;
	int	m_nFirst;
	int	m_nBx, m_nBy;	// １つ前のマウス位置
	int	m_nSetBx, m_nSetBy;	// ドラッグ時のDOWNマウス位置
	int m_nKeyFlags;
	int m_nDragFlag;
	int m_nDragMode;
	int	m_fAtntion;
	int	m_nBmpCnt;
	int	m_nCrctIdx;
	int	m_nBaseXTop;
	int	m_nBaseYTop;
	int	m_nSublineDraw;
	int	m_nSlctWaku;
	CDC	*m_pcDC;
	TCHAR	m_szStockStr[RECMAX];
	int	m_nBackDrawMode;		// 画面表示モードフラグのバックアップ
	int	m_fStrgInputMode;		// 文字列入力モード

public:
	void WorkSizeReset();
	void WorkAreaReset();
	UINT GetSubCommand();
public:
	void WorkGuideLine(CDC *pcDC);
	void SetString(LPTSTR lpStr);
	void CommandSetDef();
	void ExecSmsout();
	void SetNextCommand(int cmd);
	int EndTimer();
	void StartTimer();
	int m_nRedrawFlag;
	void RedrawFlagOn();
	void ClipPathDraw(CDC *pcDC);
	void AtntionDraw(CDC *pcDC);
	void CheckFlagDraw(int flag, CDC *pcDC);
	void CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick);
	void CheckFlagOutlineDraw(int flag, CDC *pcDC);
	void ActiveObjectOnDraw(CDC *pDC);
	void ActiveObjectBoxsDraw(CDC *pcDC);
	void PloterOutput();
	void PrinterOutput();
	void PrintEnvLoad(LPTSTR printenvfname);
	void PrintEnvSave(LPTSTR printenvfname);
	void DrawSelectWaku(CDC *pcDC);
	BOOL CheckKeyFlag(int nFlag);
	void WorkAreaCntrLineDraw(CDC* pcDC);
	void WorkAreaSubLineDraw(CDC* pcDC);
	void InitScrollElement();

	void DirectDraw(CDC *dc);
	void MouseSetCrt(DBL cx, DBL cy);

protected:
	CSize	m_szPreviousOffset;
	CSize	m_szObjectSize;
	CPoint	m_ptDragPoint;
	BOOL	m_bDragMode;
	BOOL GetSizeInfo(COleDataObject* pObject, CSize& szSize, CSize& szPlace);
	void RoundTrns(DBL srcx, DBL srcy, DBL x0, DBL y0, DBL rag, DBL *dstx, DBL *dsty);

	static CLIPFORMAT m_cfObjectDescriptor;
	//{{AFX_MSG(CTonView)
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnFileEnvset();
	afx_msg void OnObjFigu();
	afx_msg void OnStrgInput();
	afx_msg void OnObjMsur();
	afx_msg void OnUpdateObjFigu(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjMsur(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStrgInput(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPreview(CCmdUI* pCmdUI);
	afx_msg void OnLineConerarc();
	afx_msg void OnLineConerset();
	afx_msg void OnLineCrcl();
	afx_msg void OnLineCurveing();
	afx_msg void OnLineCut();
	afx_msg void OnLineCutjoin();
	afx_msg void OnLineDel();
	afx_msg void OnLineJoin();
	afx_msg void OnLineLineing();
	afx_msg void OnLineMove();
	afx_msg void OnObjBmp();
	afx_msg void OnObjWaku();
	afx_msg void OnPointAdd();
	afx_msg void OnPointChngatr();
	afx_msg void OnPointDel();
	afx_msg void OnPointDiv();
	afx_msg void OnPointLegal();
	afx_msg void OnPointMove();
	afx_msg void OnSpclBitmaping();
	afx_msg void OnSpclOutline();
	afx_msg void OnSpclParallel();
	afx_msg void OnSpclPrepend();
	afx_msg void OnSpclShadow();
	afx_msg void OnSpclStrgstrct();
	afx_msg void OnToolExport();
	afx_msg void OnToolInport();
	afx_msg void OnSlctArea();
	afx_msg void OnSlctObj();
	afx_msg void OnViewImage();
	afx_msg void OnZoomAll();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnToolTrns();
	afx_msg void OnViewPreview();
	afx_msg void OnToolPoint();
	afx_msg void OnToolLine();
	afx_msg void OnUpdateZoomAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSlctAll();
	afx_msg void OnSlctChng();
	afx_msg void OnEditDelete();
	afx_msg void OnEditRedo();
	afx_msg void OnEditUndo();
	afx_msg void OnEditMvcopy();
	afx_msg void OnToolTrace();
	afx_msg void OnClrSolid();
	afx_msg void OnEditMove();
	afx_msg void OnFiguBoxs();
	afx_msg void OnFiguCrcl();
	afx_msg void OnFiguFree();
	afx_msg void OnFiguLine();
	afx_msg void OnFiguPoint();
	afx_msg void OnFileAddload();
	afx_msg void OnFileEpsout();
	afx_msg void OnFilePloter();
	afx_msg void OnLineArc();
	afx_msg void OnLineBeze();
	afx_msg void OnLineHalfarc();
	afx_msg void OnMirrorCntr();
	afx_msg void OnMirrorFree();
	afx_msg void OnMojiDel();
	afx_msg void OnMojiInseart();
	afx_msg void OnMojiMove();
	afx_msg void OnMojiPlgning();
	afx_msg void OnMojiRound();
	afx_msg void OnMojiSize();
	afx_msg void OnPoint2point();
	afx_msg void OnPointAnydel();
	afx_msg void OnPointAnymove();
	afx_msg void OnPointPicup();
	afx_msg void OnPointSloplegal();
	afx_msg void OnPointXlegal();
	afx_msg void OnPointYlegal();
	afx_msg void OnPrevPane();
	afx_msg void OnRound2point();
	afx_msg void OnRoundFree();
	afx_msg void OnRoundRag();
	afx_msg void OnSize2point();
	afx_msg void OnSizeFree();
	afx_msg void OnSizeTime();
	afx_msg void OnSizeX();
	afx_msg void OnSizeXy();
	afx_msg void OnSizeY();
	afx_msg void OnSlopFree();
	afx_msg void OnSlopRag();
	afx_msg void OnStrgOrder();
	afx_msg void OnStrgRound();
	afx_msg void OnStrgSlop();
	afx_msg void OnTrnsArc();
	afx_msg void OnTrnsBoxs();
	afx_msg void OnClrSeetSet();
	afx_msg void OnUpdateFiguBoxs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFiguCrcl(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFiguFree(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFiguLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFiguPoint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineMove(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePointMove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSizeFree(CCmdUI* pCmdUI);
	afx_msg void OnClrLine();
	afx_msg void OnClrGradation();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnAtrConerKind();
	afx_msg void OnViewGrid();
	afx_msg void OnUpdateViewGrid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewImage(CCmdUI* pCmdUI);
	afx_msg void OnWakuGrup();
	afx_msg void OnWakuCreate();
	afx_msg void OnWakuInput();
	afx_msg void OnWakuSave();
	afx_msg void OnWakuLoad();
	afx_msg void OnWakuEdit();
	afx_msg void OnWakuDelete();
	afx_msg void OnWakuSort();
	afx_msg void OnWakuPaper();
	afx_msg void OnKamonLoad();
	afx_msg void OnEskamonLoad();
	afx_msg void OnEskamonSave();
	afx_msg void OnStrgBoxsin();
	afx_msg void OnStrgEdit();
	afx_msg void OnMojiSpace();
	afx_msg void OnViewRedisp();
	afx_msg void OnSortHead();
	afx_msg void OnSortNext();
	afx_msg void OnSortPrev();
	afx_msg void OnSortTail();
	afx_msg void OnGrupSet();
	afx_msg void OnGrupReset();
	afx_msg void OnListSet();
	afx_msg void OnListReset();
	afx_msg void OnMsurXcreate();
	afx_msg void OnMsurYcreate();
	afx_msg void OnMsurAtrset();
	afx_msg void OnHelpMain();
	afx_msg void OnClrSet();
	afx_msg void OnMoveRltv();
	afx_msg void OnMoveCood();
	afx_msg void OnCopyCood();
	afx_msg void OnCopyRltv();
	afx_msg void OnCopyPich();
	afx_msg void OnSubGrid();
	afx_msg void OnStrgProp();
	afx_msg void OnSubWaku();
	afx_msg void OnSubAnywaku();
	afx_msg void OnFilePltmove();
	afx_msg void OnFiguSizeboxs();
	afx_msg void OnFiguSizecrcl();
	afx_msg void OnMojiSvdel();
	afx_msg void OnMojiSave();
	afx_msg void OnMojiLoad();
	afx_msg void OnFileDotedit();
	afx_msg void OnSubWakudel();
	afx_msg void OnUpdateSlctObj(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineArc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineBeze(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineConerarc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineConerset(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineCrcl(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineCurveing(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineCutjoin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineDel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineHalfarc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineJoin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineLineing(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePoint2point(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePointAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePointAnydel(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePointAnymove(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePointChngatr(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePointDel(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePointPicup(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePointXlegal(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePointYlegal(CCmdUI* pCmdUI);
	afx_msg void OnBmpRound();
	afx_msg void OnSpclOmitoutline();
	afx_msg void OnFileAddsave();
	afx_msg void OnFileClrprint();
	afx_msg void OnBmpLoad();
	afx_msg void OnBmpTrns();
	afx_msg void OnSubGridOnoff();
	afx_msg void OnUpdateSubGridOnoff(CCmdUI* pCmdUI);
	afx_msg void OnFileAddenvload();
	afx_msg void OnFileAdddel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCrdntTop();
	afx_msg void OnCrdntVcenter();
	afx_msg void OnCrdntBottom();
	afx_msg void OnCrdntLeft();
	afx_msg void OnCrdntHcenter();
	afx_msg void OnCrdntRight();
	afx_msg void OnMoveHcenter();
	afx_msg void OnMoveVcenter();
	afx_msg void OnMoveHvcenter();
	afx_msg void OnSizeHsame();
	afx_msg void OnSizeVsame();
	afx_msg void OnSizeHvsame();
	afx_msg void OnSpaceHequal();
	afx_msg void OnSpaceVequal();
	afx_msg void OnSpclMarge();
	afx_msg void OnPartLoad();
	afx_msg void OnPartSave();
	afx_msg void OnSlineCutjoin();
	afx_msg void OnUpdateSlineCutjoin(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int		m_nTimer;
	UINT	m_nTimerID;
	int m_nStockPoint;
	TCHAR m_szStockBuffer[RECMAX*2];
	// プリンター出力時に使用する変数
	UINT m_nDivPageLast;
	UINT m_nXDivPage;
	UINT m_nYDivPage;
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
	DBL	m_dBackTime;
	DBL	m_dPrnTimeX, m_dPrnTimeY;
	int m_nScaleTrns;

	BOOL m_fWinExist;	// ウィンドウが生成されるまでFALSE
	CCommand* m_pCommand;
	void WorkAreaScale(CDC *pDC, DBL rltvtopx, DBL rltvtopy);
	void WorkAreaDraw(CDC* pDC);
	void WorkAreaGridDraw(CDC* pDC);
	void WorkAreaBackClrFill(CDC *pDC);
	UINT CommandExit();
	void SetSubCommand(int nSubCommand);
	UINT GetCrtCommand();
	UINT CommandInit(UINT Command);
	void DivPageCountSet(DBL workx, DBL worky, DBL prnx, DBL prny);
	void SetFiguKind(int nfigukind);
	int GetFiguKind();
};

#ifndef _DEBUG  // essView.cpp ファイルがデバッグ環境の時使用されます。
inline CTonDoc* CTonView::GetDocument()
   { return (CTonDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

