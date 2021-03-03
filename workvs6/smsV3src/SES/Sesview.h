// sesView.h : CTonView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SESVIEW_H__A4B45ED0_417C_11D2_99B2_004026171D2B__INCLUDED_)
#define AFX_SESVIEW_H__A4B45ED0_417C_11D2_99B2_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTonCntrItem;

class CTonView : public CScrollView
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CTonView();
	DECLARE_DYNCREATE(CTonView)

// �A�g���r���[�g
public:
	CTonDoc* GetDocument();
	//   m_pSlctOle �͌��݂� CTonCntrItem �ɑ΂���I����ێ����Ă��܂��B
	//  �����̃A�v���P�[�V�����ł��̂悤�ȃ����o�ϐ��́A�����̑I���� CTonCntrItem
	//  �I�u�W�F�N�g�ł͂Ȃ��I�u�W�F�N�g�̑I�����\���ɕ\���ł��܂���B
	//  ���̋@�\�͑I���@�\�����p���悤�Ƃ���v���O���}�̗����������邽
	//  �߂ɑg�ݍ��܂�Ă��܂��B

	// TODO: ���̑I����@���A�v���P�[�V�����ɓK�������@�ɒu�������Ă��������B
// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTonView)
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void OnPrepareDC(CDC* pcDC, CPrintInfo* pInfo = NULL);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pcDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pcDC, CPrintInfo* pInfo);
	virtual BOOL IsSelected(const CObject* pcDocItem) const;// OLE �R���e�i �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CTonView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	UINT CommandExec(UINT event, long x, long y);

// �������ꂽ���b�Z�[�W �}�b�v�֐�
public:
	BOOL m_bDispImage; // TRUE �C���[�W�\���@FALSE �g�̂ݕ\��
	BOOL m_bEpsonYoko; // TRUE ����p����]�@FALSE �����ł����̂܂܏o��
	int	m_nCursorKind;
	int m_nSubCommand;
	int	m_nNextCommand;
	int m_nFiguKind;

	char m_szInfoEnv[RECMAX];
	int	m_nCrctIdx;
	int m_nCrtWaku;
	int	m_nDrawIn;
	int	m_nSublineDraw;
	int	m_nFirst;
	int	m_nBx, m_nBy;	// �P�O�̃}�E�X�ʒu
	int	m_nSetBx, m_nSetBy;	// �h���b�O����DOWN�}�E�X�ʒu
	int m_nKeyFlags;
	int m_nDragFlag;
	int m_nDragMode;
	int	m_fAtntion;
	int	m_nBmpCnt;
	int	m_nBaseXTop;
	int	m_nBaseYTop;
	CSize	m_szPreviousOffset;
	CSize	m_szObjectSize;
	CPoint	m_ptDragPoint;
	BOOL	m_bDragMode;
	int		m_nSlctWaku;
	CDC	*m_pcDC;

	CTonCntrItem* m_pSlctOle;
	COleDropTarget m_dropTarget;
	int	m_nBackDrawMode;		// ��ʕ\�����[�h�t���O�̃o�b�N�A�b�v
	int	m_fStrgInputMode;		// ��������̓��[�h
	char	m_szStockStr[RECMAX];

public:
	void WorkTonnboDraw(CDC *pcDC);
	void WorkKadoTonnboDraw(CDC *pcDC, DBL cx, DBL cy, DBL signex, DBL signey);
	void WorkCntrTonnboDraw(CDC *pcDC, DBL cx, DBL cy, DBL signex, DBL signey);
	void PrintOutDC(CDC *pcDC);
	void SetString(LPSTR lpStr);
	void WorkSpaceLineDraw(CDC *pcDC);
	void CallOnWakuApply();
	void PrintAreaBackClrFill(CDC *pcDC);
	void ExecSmsout();
	void ProgExitLoop();
	void LoadCrtEnv(LPSTR fname);
	void LoadCsvData(LPSTR fname);
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
	void PrintEnvLoad(LPSTR printenvfname);
	void PrintEnvSave(LPSTR printenvfname);
	void DrawSelectWaku(CDC *pcDC);
	BOOL CheckKeyFlag(int nFlag);
	void WorkAreaCntrLineDraw(CDC* pcDC);
	void WorkAreaSubLineDraw(CDC* pcDC);
	void InitScrollElement();

	void MouseSetCrt(DBL cx, DBL cy);
	void DirectDraw(CDC* dc);

	BOOL GetStockString(LPSTR buf);
	void StrgInputModeOn();
	void StrgInputModeOff();
	void WorkSizeReset();
	void WorkAreaReset();
	UINT GetSubCommand();
	void MemoryProgExitLoop();
protected:
	BOOL GetSizeInfo(COleDataObject* pObject, CSize& szSize, CSize& szPlace);
	void RoundTrns(DBL srcx, DBL srcy, DBL x0, DBL y0, DBL rag, DBL *dstx, DBL *dsty);

	static CLIPFORMAT m_cfObjectDescriptor;
	//{{AFX_MSG(CTonView)
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInsertObject();
	afx_msg void OnCancelEditCntr();
	afx_msg void OnCancelEditSrvr();
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
	afx_msg void OnUpdateEditPasteLink(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPasteSpecial(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnEditPasteSpecial();
	afx_msg void OnEditPasteLink();
	afx_msg void OnFileOleExit();
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
	afx_msg void OnObjRule();
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int		m_nTimer;
	UINT	m_nTimerID;
	HANDLE	m_hProcess;
	int		m_nExitProcess;
	int m_nStockPoint;
	char m_szStockBuffer[RECMAX*2];
	// �v�����^�[�o�͎��Ɏg�p����ϐ�
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
	DBL	m_dBackTime;
	DBL	m_dPrnTimeX, m_dPrnTimeY;
	int m_nScaleTrns;

	BOOL m_fWinExist;	// �E�B���h�E�����������܂�FALSE
	CCommand* m_pCommand;
	void WorkAreaScale(CDC *pcDC, DBL rltvtopx, DBL rltvtopy);
	void WorkAreaDraw(CDC* pcDC);
	void WorkAreaGridDraw(CDC* pcDC);
	void WorkAreaBackClrFill(CDC *pcDC);
	void OleObjectOnDraw(CDC *pcDC, CTonDoc *pcDoc);
	BOOL OleObjectOpen(UINT nFlags, CPoint point); 
	BOOL OleObjectSelected(UINT nFlags, CPoint point);
	UINT CommandExit();
	void SetSubCommand(int nSubCommand);
	UINT GetCrtCommand();
	UINT CommandInit(UINT Command);
	void DivPageCountSet(DBL workx, DBL worky, DBL prnx, DBL prny);
	void SetFiguKind(int nfigukind);
	int GetFiguKind();
};

#ifndef _DEBUG  // essView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline CTonDoc* CTonView::GetDocument()
   { return (CTonDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SESVIEW_H__A4B45ED0_417C_11D2_99B2_004026171D2B__INCLUDED_)
