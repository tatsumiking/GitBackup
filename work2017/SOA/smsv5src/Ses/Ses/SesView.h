
// SesView.h : CTonView クラスのインターフェイス
//

#pragma once

class CTonView : public CScrollView
{
protected: // シリアル化からのみ作成します。
	CTonView() noexcept;
	DECLARE_DYNCREATE(CTonView)
public:
	virtual ~CTonView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 属性
public:
	CTonDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
protected:
	virtual void OnInitialUpdate();
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pcDC, CPrintInfo* pcInfo);
	virtual void OnEndPrinting(CDC* pcDC, CPrintInfo* pcInfo);
	virtual void OnPrepareDC(CDC* pcDC, CPrintInfo* pInfo);

// 実装
public:
	UINT CommandExec(UINT event, long x, long y);

protected:

// 生成された、メッセージ割り当て関数
public:
	int m_nExitProcess;
	BOOL m_bDispImage; // TRUE イメージ表示　FALSE 枠のみ表示
	BOOL m_bEpsonYoko; // TRUE 印刷用横回転　FALSE 横長でもそのまま出力
	int	m_nCursorKind;
	int m_nSubCommand;
	int	m_nNextCommand;
	int m_nFiguKind;

	TCHAR m_szInfoEnv[RECMAX];
	int	m_nCrctIdx;
	int m_nCrtWaku;
	int m_nCrtGroup;
	int	m_nDrawIn;
	int	m_nSublineDraw;
	int	m_nFirst;
	int	m_nBx, m_nBy;	// １つ前のマウス位置
	int	m_nSetBx, m_nSetBy;	// ドラッグ時のDOWNマウス位置
	int m_nKeyFlags;
	int m_nDragFlag;
	int	m_fAtntion;
	int	m_nBmpCnt;
	int	m_nBaseXTop;
	int	m_nBaseYTop;
	int		m_nSlctWaku;
	CDC	*m_pcDC;

	COleDropTarget m_dropTarget;
	int	m_nBackDrawMode;		// 画面表示モードフラグのバックアップ
	int	m_fStrgInputMode;		// 文字列入力モード
	TCHAR	m_szStockStr[RECMAX];
	int m_nScaleTrns;
	UINT m_nDivPageLast;
	UINT m_nXDivPage;
	UINT m_nYDivPage;
	int	m_nDivCrtX, m_nDivCrtY;

public:
	void WorkTonnboDraw(CDC *pcDC);
	void WorkKadoTonnboDraw(CDC *pcDC, DBL cx, DBL cy, DBL signex, DBL signey);
	void WorkCntrTonnboDraw(CDC *pcDC, DBL cx, DBL cy, DBL signex, DBL signey);
	void PrintOutDC(CDC *pcDC);
	void SetString(LPTSTR LPTSTR);
	void WorkSpaceLineDraw(CDC *pcDC);
	void CallOnWakuApply();
	void PrintAreaBackClrFill(CDC *pcDC);
	void ExecSmsout();
	void ProgExitLoop();
	void PrintEnvLoad(LPTSTR printenvfname);
	void PrintEnvSave(LPTSTR printenvfname);
	void LoadCrtEnv(LPTSTR fname);
	void LoadCsvData(LPTSTR fname);
	void SetNextCommand(int cmd);
	int EndTimer();
	void StartTimer();
	int m_nRedrawFlag;
	void RedrawFlagOn();
	void ClipPathDraw(CDC *pcDC);
	void AtntionDraw(CDC *pcDC);
	void ActiveObjectOnDraw(CDC *pcDC);
	void ActiveObjectBoxsDraw(CDC *pcDC);
	void CheckFlagDraw(int flag, CDC *pcDC);
	void CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick);
	void CheckFlagOutlineDraw(int flag, CDC *pcDC);
	void PloterOutput();
	void PrinterOutput();
	void DrawSelectWaku(CDC *pcDC);
	void DrawSelectGroup(CDC *pcDC);
	int CheckSekijiGrup(CDataGrup *pcGrupVect);
	BOOL CheckKeyFlag(int nFlag);
	void WorkAreaCntrLineDraw(CDC* pcDC);
	void WorkAreaSubLineDraw(CDC* pcDC);
	void InitScrollElement();

	void MouseSetCrt(DBL cx, DBL cy);
	void DirectDraw(CDC* dc);

	void WorkSizeReset();
	void WorkAreaReset();
	UINT GetSubCommand();
	void MemoryProgExitLoop();


protected:
	void RoundTrns(DBL srcx, DBL srcy, DBL x0, DBL y0, DBL rag, DBL *dstx, DBL *dsty);

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nTCHAR, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nTCHAR, UINT nRepCnt, UINT nFlags);
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
	afx_msg void OnScanSlct();
	afx_msg void OnScanInput();
	afx_msg void OnToolTrace();
	afx_msg void OnClrSolid();
	afx_msg void OnEditMove();
	afx_msg void OnFiguBoxs();
	afx_msg void OnFiguCrcl();
	afx_msg void OnFiguFree();
	afx_msg void OnFiguLine();
	afx_msg void OnFiguPlgn();
	afx_msg void OnFiguPoint();
	afx_msg void OnFileAddload();
	afx_msg void OnFileAddsave();
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
	afx_msg void OnUpdateFiguPlgn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFiguPoint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineMove(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePointMove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSizeFree(CCmdUI* pCmdUI);
	afx_msg void OnClrLine();
	afx_msg void OnClrGradation();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
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
	afx_msg void OnSubWakudel();
	afx_msg void OnAtrConerKind();
	afx_msg void OnSlopRl();
	afx_msg void OnSlopUd();
	afx_msg void OnFileCrctsave();
	afx_msg void OnFileCrctload();
	afx_msg void OnFileAdddel();
	afx_msg void OnFileCrctdel();
	afx_msg void OnWakuInsort();
	afx_msg void OnSubGridOnoff();
	afx_msg void OnUpdateSubGridOnoff(CCmdUI* pCmdUI);
	afx_msg void OnMojiFiting();
	afx_msg void OnFileAddenvload();
	afx_msg void OnWakuChange();
	afx_msg void OnBmpRound();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBmpSave();
	afx_msg void OnClrBackset();
	afx_msg void OnClrBackreset();
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
	afx_msg void OnWakuApply();
	afx_msg void OnWakuPaperdel();
	afx_msg void OnBmpJpgsave();
	afx_msg void OnMenteCptrFile();
	afx_msg void OnMenteCptrFolder();
	afx_msg void OnDbedtLoad();
	afx_msg void OnDbedtDelete();
	afx_msg void OnUpdateDbedtDelete(CCmdUI* pCmdUI);
	afx_msg void OnFileEpssend();
	afx_msg void OnFileMprnsend();
	afx_msg void OnFilePrnsend();
	afx_msg void OnFileMltprnftp();
	afx_msg void OnFileEmltprnftp();
	afx_msg void OnBmpLoad();
	afx_msg void OnBmpClip();
	afx_msg void OnBmpBkload();
	afx_msg void OnBmpBkreset();
	afx_msg void OnMojiFontchange();
	afx_msg void OnMemoWakuLoad();
	afx_msg void OnMemoCallprog();
	afx_msg void OnMemoEnvset();
	afx_msg void OnMenteWakuBackup();
	afx_msg void OnWakuInsekiji();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnUpdateWakuInput(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWakuInsekiji(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
private:
	int		m_nTimer;
	UINT	m_nTimerID;
	HANDLE	m_hProcess;
	int m_nStockPoint;
	TCHAR m_szStockBuffer[RECMAX*2];
	// プリンター出力時に使用する変数
	int	m_nOrientation;
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

	BOOL m_fWinExist;	// ウィンドウが生成されるまでFALSE
	CCommand* m_pCommand;
	void WorkAreaScale(CDC *pcDC, DBL rltvtopx, DBL rltvtopy);
	void WorkAreaDraw(CDC* pcDC);
	void WorkAreaGridDraw(CDC* pcDC);
	void WorkAreaBackClrFill(CDC *pcDC);
	UINT CommandExit();
	void SetSubCommand(int nSubCommand);
	UINT GetCrtCommand();
	UINT CommandInit(UINT Command);
	void DivPageCountSet(DBL workx, DBL worky, DBL prnx, DBL prny);
	void SetFiguKind(int nfigukind);
};

#ifndef _DEBUG  // SesView.cpp のデバッグ バージョン
inline CTonDoc* CTonView::GetDocument() const
   { return reinterpret_cast<CTonDoc*>(m_pDocument); }
#endif

