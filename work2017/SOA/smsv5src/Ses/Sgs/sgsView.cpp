// sgsView.cpp : CTonView クラスの動作の定義を行います。
//

#include "stdafx.h"

#include	"../../Object/GdiPlusEx.h"
#include	"../../Object/Vect.h"
#include	"../../Object/Data.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	DOWNUPLIMIT	4
#define	TIMERINTERVAL	100

//extern "C" {
	#include "../../ObjLib/Baselib.h"
	#include "../../ObjLib/Senvlib.h"
	#include "../../ObjLib/Scrnlib.h"
	#include "../../ObjLib/Pout.h"
//}

/////////////////////////////////////////////////////////////////////////////
// CTonView

IMPLEMENT_DYNCREATE(CTonView, CScrollView)

BEGIN_MESSAGE_MAP(CTonView, CScrollView)
	//{{AFX_MSG_MAP(CTonView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_KEYUP()
	ON_WM_SETCURSOR()
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_FILE_ENVSET, OnFileEnvset)
	ON_COMMAND(ID_OBJ_FIGU, OnObjFigu)
	ON_COMMAND(ID_STRG_INPUT, OnStrgInput)
	ON_COMMAND(ID_OBJ_MSUR, OnObjMsur)
	ON_UPDATE_COMMAND_UI(ID_OBJ_FIGU, OnUpdateObjFigu)
	ON_UPDATE_COMMAND_UI(ID_OBJ_MSUR, OnUpdateObjMsur)
	ON_UPDATE_COMMAND_UI(ID_STRG_INPUT, OnUpdateStrgInput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PREVIEW, OnUpdateViewPreview)
	ON_COMMAND(ID_LINE_CONERARC, OnLineConerarc)
	ON_COMMAND(ID_LINE_CONERSET, OnLineConerset)
	ON_COMMAND(ID_LINE_CRCL, OnLineCrcl)
	ON_COMMAND(ID_LINE_CURVEING, OnLineCurveing)
	ON_COMMAND(ID_LINE_CUT, OnLineCut)
	ON_COMMAND(ID_LINE_CUTJOIN, OnLineCutjoin)
	ON_COMMAND(ID_LINE_DEL, OnLineDel)
	ON_COMMAND(ID_LINE_JOIN, OnLineJoin)
	ON_COMMAND(ID_LINE_LINEING, OnLineLineing)
	ON_COMMAND(ID_LINE_MOVE, OnLineMove)
	ON_COMMAND(ID_OBJ_BMP, OnObjBmp)
	ON_COMMAND(ID_OBJ_WAKU, OnObjWaku)
	ON_COMMAND(ID_POINT_ADD, OnPointAdd)
	ON_COMMAND(ID_POINT_CHNGATR, OnPointChngatr)
	ON_COMMAND(ID_POINT_DEL, OnPointDel)
	ON_COMMAND(ID_POINT_DIV, OnPointDiv)
	ON_COMMAND(ID_POINT_LEGAL, OnPointLegal)
	ON_COMMAND(ID_POINT_MOVE, OnPointMove)
	ON_COMMAND(ID_SPCL_BITMAPING, OnSpclBitmaping)
	ON_COMMAND(ID_SPCL_OUTLINE, OnSpclOutline)
	ON_COMMAND(ID_SPCL_PARALLEL, OnSpclParallel)
	ON_COMMAND(ID_SPCL_PREPEND, OnSpclPrepend)
	ON_COMMAND(ID_SPCL_SHADOW, OnSpclShadow)
	ON_COMMAND(ID_SPCL_STRGSTRCT, OnSpclStrgstrct)
	ON_COMMAND(ID_TOOL_EXPORT, OnToolExport)
	ON_COMMAND(ID_TOOL_INPORT, OnToolInport)
	ON_COMMAND(ID_SLCT_AREA, OnSlctArea)
	ON_COMMAND(ID_SLCT_OBJ, OnSlctObj)
	ON_COMMAND(ID_VIEW_IMAGE, OnViewImage)
	ON_COMMAND(ID_ZOOM_ALL, OnZoomAll)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_TOOL_TRNS, OnToolTrns)
	ON_COMMAND(ID_VIEW_PREVIEW, OnViewPreview)
	ON_COMMAND(ID_TOOL_POINT, OnToolPoint)
	ON_COMMAND(ID_TOOL_LINE, OnToolLine)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_ALL, OnUpdateZoomAll)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_IN, OnUpdateZoomIn)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_SLCT_ALL, OnSlctAll)
	ON_COMMAND(ID_SLCT_CHNG, OnSlctChng)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_MVCOPY, OnEditMvcopy)
	ON_COMMAND(ID_TOOL_TRACE, OnToolTrace)
	ON_COMMAND(ID_CLR_SOLID, OnClrSolid)
	ON_COMMAND(ID_EDIT_MOVE, OnEditMove)
	ON_COMMAND(ID_FIGU_BOXS, OnFiguBoxs)
	ON_COMMAND(ID_FIGU_CRCL, OnFiguCrcl)
	ON_COMMAND(ID_FIGU_FREE, OnFiguFree)
	ON_COMMAND(ID_FIGU_LINE, OnFiguLine)
	ON_COMMAND(ID_FIGU_POINT, OnFiguPoint)
	ON_COMMAND(ID_FILE_ADDLOAD, OnFileAddload)
	ON_COMMAND(ID_FILE_EPSOUT, OnFileEpsout)
	ON_COMMAND(ID_FILE_PLOTER, OnFilePloter)
	ON_COMMAND(ID_LINE_ARC, OnLineArc)
	ON_COMMAND(ID_LINE_BEZE, OnLineBeze)
	ON_COMMAND(ID_LINE_HALFARC, OnLineHalfarc)
	ON_COMMAND(ID_MIRROR_CNTR, OnMirrorCntr)
	ON_COMMAND(ID_MIRROR_FREE, OnMirrorFree)
	ON_COMMAND(ID_MOJI_DEL, OnMojiDel)
	ON_COMMAND(ID_MOJI_INSEART, OnMojiInseart)
	ON_COMMAND(ID_MOJI_MOVE, OnMojiMove)
	ON_COMMAND(ID_MOJI_PLGNING, OnMojiPlgning)
	ON_COMMAND(ID_MOJI_ROUND, OnMojiRound)
	ON_COMMAND(ID_MOJI_SIZE, OnMojiSize)
	ON_COMMAND(ID_POINT_2POINT, OnPoint2point)
	ON_COMMAND(ID_POINT_ANYDEL, OnPointAnydel)
	ON_COMMAND(ID_POINT_ANYMOVE, OnPointAnymove)
	ON_COMMAND(ID_POINT_PICUP, OnPointPicup)
	ON_COMMAND(ID_POINT_SLOPLEGAL, OnPointSloplegal)
	ON_COMMAND(ID_POINT_XLEGAL, OnPointXlegal)
	ON_COMMAND(ID_POINT_YLEGAL, OnPointYlegal)
	ON_COMMAND(ID_PREV_PANE, OnPrevPane)
	ON_COMMAND(ID_ROUND_2POINT, OnRound2point)
	ON_COMMAND(ID_ROUND_FREE, OnRoundFree)
	ON_COMMAND(ID_ROUND_RAG, OnRoundRag)
	ON_COMMAND(ID_SIZE_2POINT, OnSize2point)
	ON_COMMAND(ID_SIZE_FREE, OnSizeFree)
	ON_COMMAND(ID_SIZE_TIME, OnSizeTime)
	ON_COMMAND(ID_SIZE_X, OnSizeX)
	ON_COMMAND(ID_SIZE_XY, OnSizeXy)
	ON_COMMAND(ID_SIZE_Y, OnSizeY)
	ON_COMMAND(ID_SLOP_FREE, OnSlopFree)
	ON_COMMAND(ID_SLOP_RAG, OnSlopRag)
	ON_COMMAND(ID_STRG_ORDER, OnStrgOrder)
	ON_COMMAND(ID_STRG_ROUND, OnStrgRound)
	ON_COMMAND(ID_STRG_SLOP, OnStrgSlop)
	ON_COMMAND(ID_TRNS_ARC, OnTrnsArc)
	ON_COMMAND(ID_TRNS_BOXS, OnTrnsBoxs)
	ON_COMMAND(ID_CLR_SEET_SET, OnClrSeetSet)
	ON_UPDATE_COMMAND_UI(ID_FIGU_BOXS, OnUpdateFiguBoxs)
	ON_UPDATE_COMMAND_UI(ID_FIGU_CRCL, OnUpdateFiguCrcl)
	ON_UPDATE_COMMAND_UI(ID_FIGU_FREE, OnUpdateFiguFree)
	ON_UPDATE_COMMAND_UI(ID_FIGU_LINE, OnUpdateFiguLine)
	ON_UPDATE_COMMAND_UI(ID_FIGU_POINT, OnUpdateFiguPoint)
	ON_UPDATE_COMMAND_UI(ID_LINE_MOVE, OnUpdateLineMove)
	ON_UPDATE_COMMAND_UI(ID_POINT_MOVE, OnUpdatePointMove)
	ON_UPDATE_COMMAND_UI(ID_SIZE_FREE, OnUpdateSizeFree)
	ON_COMMAND(ID_CLR_LINE, OnClrLine)
	ON_COMMAND(ID_CLR_GRADATION, OnClrGradation)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_ATR_CONER_KIND, OnAtrConerKind)
	ON_COMMAND(ID_VIEW_GRID, OnViewGrid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRID, OnUpdateViewGrid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_IMAGE, OnUpdateViewImage)
	ON_COMMAND(ID_WAKU_GRUP, OnWakuGrup)
	ON_COMMAND(ID_WAKU_CREATE, OnWakuCreate)
	ON_COMMAND(ID_WAKU_INPUT, OnWakuInput)
	ON_COMMAND(ID_WAKU_SAVE, OnWakuSave)
	ON_COMMAND(ID_WAKU_LOAD, OnWakuLoad)
	ON_COMMAND(ID_WAKU_EDIT, OnWakuEdit)
	ON_COMMAND(ID_WAKU_DELETE, OnWakuDelete)
	ON_COMMAND(ID_WAKU_SORT, OnWakuSort)
	ON_COMMAND(ID_WAKU_PAPER, OnWakuPaper)
	ON_COMMAND(ID_KAMON_LOAD, OnKamonLoad)
	ON_COMMAND(ID_ESKAMON_LOAD, OnEskamonLoad)
	ON_COMMAND(ID_ESKAMON_SAVE, OnEskamonSave)
	ON_COMMAND(ID_STRG_BOXSIN, OnStrgBoxsin)
	ON_COMMAND(ID_STRG_EDIT, OnStrgEdit)
	ON_COMMAND(ID_MOJI_SPACE, OnMojiSpace)
	ON_COMMAND(ID_VIEW_REDISP, OnViewRedisp)
	ON_COMMAND(ID_SORT_HEAD, OnSortHead)
	ON_COMMAND(ID_SORT_NEXT, OnSortNext)
	ON_COMMAND(ID_SORT_PREV, OnSortPrev)
	ON_COMMAND(ID_SORT_TAIL, OnSortTail)
	ON_COMMAND(ID_GRUP_SET, OnGrupSet)
	ON_COMMAND(ID_GRUP_RESET, OnGrupReset)
	ON_COMMAND(ID_LIST_SET, OnListSet)
	ON_COMMAND(ID_LIST_RESET, OnListReset)
	ON_COMMAND(ID_MSUR_XCREATE, OnMsurXcreate)
	ON_COMMAND(ID_MSUR_YCREATE, OnMsurYcreate)
	ON_COMMAND(ID_MSUR_ATRSET, OnMsurAtrset)
	ON_COMMAND(ID_HELP_MAIN, OnHelpMain)
	ON_COMMAND(ID_CLR_SET, OnClrSet)
	ON_COMMAND(ID_MOVE_RLTV, OnMoveRltv)
	ON_COMMAND(ID_MOVE_COOD, OnMoveCood)
	ON_COMMAND(ID_COPY_COOD, OnCopyCood)
	ON_COMMAND(ID_COPY_RLTV, OnCopyRltv)
	ON_COMMAND(ID_COPY_PICH, OnCopyPich)
	ON_COMMAND(ID_SUB_GRID, OnSubGrid)
	ON_COMMAND(ID_STRG_PROP, OnStrgProp)
	ON_COMMAND(ID_SUB_WAKU, OnSubWaku)
	ON_COMMAND(ID_SUB_ANYWAKU, OnSubAnywaku)
	ON_COMMAND(ID_FILE_PLTMOVE, OnFilePltmove)
	ON_COMMAND(ID_FIGU_SIZEBOXS, OnFiguSizeboxs)
	ON_COMMAND(ID_FIGU_SIZECRCL, OnFiguSizecrcl)
	ON_COMMAND(ID_MOJI_SVDEL, OnMojiSvdel)
	ON_COMMAND(ID_MOJI_SAVE, OnMojiSave)
	ON_COMMAND(ID_MOJI_LOAD, OnMojiLoad)
	ON_COMMAND(ID_FILE_DOTEDIT, OnFileDotedit)
	ON_COMMAND(ID_SUB_WAKUDEL, OnSubWakudel)
	ON_UPDATE_COMMAND_UI(ID_SLCT_OBJ, OnUpdateSlctObj)
	ON_UPDATE_COMMAND_UI(ID_LINE_ARC, OnUpdateLineArc)
	ON_UPDATE_COMMAND_UI(ID_LINE_BEZE, OnUpdateLineBeze)
	ON_UPDATE_COMMAND_UI(ID_LINE_CONERARC, OnUpdateLineConerarc)
	ON_UPDATE_COMMAND_UI(ID_LINE_CONERSET, OnUpdateLineConerset)
	ON_UPDATE_COMMAND_UI(ID_LINE_CRCL, OnUpdateLineCrcl)
	ON_UPDATE_COMMAND_UI(ID_LINE_CURVEING, OnUpdateLineCurveing)
	ON_UPDATE_COMMAND_UI(ID_LINE_CUT, OnUpdateLineCut)
	ON_UPDATE_COMMAND_UI(ID_LINE_CUTJOIN, OnUpdateLineCutjoin)
	ON_UPDATE_COMMAND_UI(ID_LINE_DEL, OnUpdateLineDel)
	ON_UPDATE_COMMAND_UI(ID_LINE_HALFARC, OnUpdateLineHalfarc)
	ON_UPDATE_COMMAND_UI(ID_LINE_JOIN, OnUpdateLineJoin)
	ON_UPDATE_COMMAND_UI(ID_LINE_LINEING, OnUpdateLineLineing)
	ON_UPDATE_COMMAND_UI(ID_POINT_2POINT, OnUpdatePoint2point)
	ON_UPDATE_COMMAND_UI(ID_POINT_ADD, OnUpdatePointAdd)
	ON_UPDATE_COMMAND_UI(ID_POINT_ANYDEL, OnUpdatePointAnydel)
	ON_UPDATE_COMMAND_UI(ID_POINT_ANYMOVE, OnUpdatePointAnymove)
	ON_UPDATE_COMMAND_UI(ID_POINT_CHNGATR, OnUpdatePointChngatr)
	ON_UPDATE_COMMAND_UI(ID_POINT_DEL, OnUpdatePointDel)
	ON_UPDATE_COMMAND_UI(ID_POINT_PICUP, OnUpdatePointPicup)
	ON_UPDATE_COMMAND_UI(ID_POINT_XLEGAL, OnUpdatePointXlegal)
	ON_UPDATE_COMMAND_UI(ID_POINT_YLEGAL, OnUpdatePointYlegal)
	ON_COMMAND(ID_BMP_ROUND, OnBmpRound)
	ON_COMMAND(ID_SPCL_OMITOUTLINE, OnSpclOmitoutline)
	ON_COMMAND(ID_FILE_ADDSAVE, OnFileAddsave)
	ON_COMMAND(ID_FILE_CLRPRINT, OnFileClrprint)
	ON_COMMAND(ID_BMP_LOAD, OnBmpLoad)
	ON_COMMAND(ID_BMP_TRNS, OnBmpTrns)
	ON_COMMAND(ID_SUB_GRID_ONOFF, OnSubGridOnoff)
	ON_UPDATE_COMMAND_UI(ID_SUB_GRID_ONOFF, OnUpdateSubGridOnoff)
	ON_COMMAND(ID_FILE_ADDENVLOAD, OnFileAddenvload)
	ON_COMMAND(ID_FILE_ADDDEL, OnFileAdddel)
	ON_WM_TIMER()
	ON_COMMAND(ID_CRDNT_TOP, OnCrdntTop)
	ON_COMMAND(ID_CRDNT_VCENTER, OnCrdntVcenter)
	ON_COMMAND(ID_CRDNT_BOTTOM, OnCrdntBottom)
	ON_COMMAND(ID_CRDNT_LEFT, OnCrdntLeft)
	ON_COMMAND(ID_CRDNT_HCENTER, OnCrdntHcenter)
	ON_COMMAND(ID_CRDNT_RIGHT, OnCrdntRight)
	ON_COMMAND(ID_MOVE_HCENTER, OnMoveHcenter)
	ON_COMMAND(ID_MOVE_VCENTER, OnMoveVcenter)
	ON_COMMAND(ID_MOVE_HVCENTER, OnMoveHvcenter)
	ON_COMMAND(ID_SIZE_HSAME, OnSizeHsame)
	ON_COMMAND(ID_SIZE_VSAME, OnSizeVsame)
	ON_COMMAND(ID_SIZE_HVSAME, OnSizeHvsame)
	ON_COMMAND(ID_SPACE_HEQUAL, OnSpaceHequal)
	ON_COMMAND(ID_SPACE_VEQUAL, OnSpaceVequal)
	ON_COMMAND(ID_SPCL_MARGE, OnSpclMarge)
	ON_COMMAND(ID_PART_LOAD, OnPartLoad)
	ON_COMMAND(ID_PART_SAVE, OnPartSave)
	ON_COMMAND(ID_SLINE_CUTJOIN, OnSlineCutjoin)
	ON_UPDATE_COMMAND_UI(ID_SLINE_CUTJOIN, OnUpdateSlineCutjoin)
	//}}AFX_MSG_MAP
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

CLIPFORMAT CTonView::m_cfObjectDescriptor = NULL;

/////////////////////////////////////////////////////////////////////////////
// CTonView クラスの構築/消滅

CTonView::CTonView()
{
	_tcscpy_s(m_szInfoEnv, _T("es98info.env"));
	m_nFirst = TRUE;
	m_fWinExist = FALSE;
	m_nNextCommand = NOOP;
	m_nDrawIn = OFF;
	m_nRedrawFlag = OFF;
	m_fAtntion = OFF;
	m_nBmpCnt = 0;
	m_nBaseXTop = 0;
	m_nBaseYTop = 0;
	m_nSublineDraw = ON;

	m_pCommand = new CCommand(this);
	m_cfObjectDescriptor = (CLIPFORMAT)
		::RegisterClipboardFormat(_T("Object Descriptor"));

	m_nStockPoint = 0;
	m_szStockBuffer[m_nStockPoint] = '\0';

	SetFiguKind(ID_FIGU_BOXS);

	m_bDragMode = FALSE;
	m_ptDragPoint = CPoint(-1, -1);
	m_nDragMode = ON;
	//m_nDragMode = OFF;
	senvsetdrawmode(PREVIEW);

	m_bDispImage = FALSE;
	m_bEpsonYoko = FALSE;

	//CTonApp*	theApp;
	//HGLOBAL*	hndl;

	//theApp = (CTonApp*)AfxGetApp();
	//theApp->GetAppPrntH(hndl);
	m_nSlctWaku = -1;
}

CTonView::~CTonView()
{
	delete(m_pCommand);
	// CG: 次の行は 「IME サポート」コンポーネントにより追加されています。
}

BOOL CTonView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTonView クラスの描画

void CTonView::OnDraw(CDC* pcDC)
{
	CGdiPlusEx *pcGpe;
	CRect	rectClip;
	CRect	rectData;
	DBL		dbrltvxtop;
	DBL		dbrltvytop;
	int		mode;
	int		noidx;

	mode = senvcheckdrawmode();
	if(m_nFirst == TRUE){
		m_nFirst = FALSE;
	}
	if(m_nDrawIn == ON){
		return;
	}

	m_nDrawIn = ON;


	senvchckstopflag();
	senvoffflag(STOPFLAG);

	scrngetrltvtop(&dbrltvxtop, &dbrltvytop);
	scrnsetwintop(dbrltvxtop, dbrltvytop);
	if(mode != PRINTVIEW){
		scrnsetrltvtop(0.0, 0.0);
	}

	CTonDoc* pcDoc = GetDocument();
	ASSERT_VALID(pcDoc);

	pcDC->GetClipBox(&rectClip);
	rectClip.InflateRect(1, 1);
	rectClip.NormalizeRect( );

	if(mode != PRINTVIEW){
		WorkAreaBackClrFill(pcDC);
		WorkAreaDraw(pcDC);
		CTonApp *theApp = (CTonApp*)AfxGetApp();
		CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
		if (pcSysEnv->ScrnCheckGrid() == ON) {
			pcDC->SetROP2(R2_XORPEN);
			WorkAreaGridDraw(pcDC);
			pcDC->SetROP2(R2_COPYPEN);
		}
		WorkAreaScale(pcDC, dbrltvxtop, dbrltvytop);
		WorkAreaCntrLineDraw(pcDC);
		WorkAreaSubLineDraw(pcDC);
		WorkGuideLine(pcDC);
	}

	pcGpe = new CGdiPlusEx();
	pcGpe->InitGraphics(pcDC);
	pcGpe->InitPath();
	if (mode == LINEVIEW || mode == POINTVIEW) {
		pcGpe->SetFillBrush(32, 0, 0, 0);
	}
	else {
		pcGpe->SetFillBrush(255, 0, 0, 0);
	}
	noidx = 1;

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	POSITION pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		CData* pcData = pcObjDataList->GetNext(pos);

		if(mode == PRINTVIEW){
			if (pcData->CheckFlag(ACTIVEFLAG) == ON) {
				pcData->GdiplusFillDraw(pcGpe);
			}
		}
		else {
			if (mode == LINEVIEW || mode == POINTVIEW) {
				if (pcData->CheckFlag(ACTIVEFLAG) == ON) {
					pcData->OnDraw(pcDC);
				}
			}
			pcData->GdiplusFillDraw(pcGpe);
		}
	}
	pcGpe->DrawFillPath();
	pcGpe->DeletePath();
	delete(pcGpe);

	if (mode == PREVIEW) {
		ActiveObjectOnDraw(pcDC);
		if (m_nRedrawFlag == ON) {
			m_pcDC = pcDC;
			CommandExec(REDRAWEVENT, 0, 0);
			m_pcDC = NULL;
			m_nRedrawFlag = OFF;
		}
	}
	if (mode != PRINTVIEW) {
		ActiveObjectBoxsDraw(pcDC);
	}

	scrnsetrltvtop(dbrltvxtop, dbrltvytop);
	scrnsetdisprltvmove(0, 0);
	m_nDrawIn = OFF;
}

void CTonView::AtntionDraw(CDC *pcDC)
{
	CTonDoc* pcDoc = GetDocument();
	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	POSITION pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		CData* pcData = pcObjDataList->GetNext(pos);
		pcData->AtntionDraw(pcDC);
	}
}

void CTonView::CheckFlagDraw(int flag, CDC *pcDC)
{
	CTonDoc* pcDoc = GetDocument();
	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	POSITION pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		CData* pcData = pcObjDataList->GetNext(pos);
		if(pcData->CheckFlag(flag) == ON){
			pcData->CheckFlagDraw(flag, pcDC);
		}
	}
}

void CTonView::CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick)
{
	CTonDoc* pcDoc = GetDocument();
	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	POSITION pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		CData* pcData = pcObjDataList->GetNext(pos);
		if(pcData->CheckFlag(flag) == ON){
			pcData->CheckFlagDrawSetClr(flag, pcDC, clr1, clr2, thick);
		}
	}
}

void CTonView::CheckFlagOutlineDraw(int flag, CDC *pcDC)
{
	CTonDoc* pcDoc = GetDocument();
	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	POSITION pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		CData* pcData = pcObjDataList->GetNext(pos);
		if(pcData->CheckFlag(flag) == ON){
			pcData->CheckFlagOutlineDraw(flag, pcDC);
		}
	}
}

void CTonView::ClipPathDraw(CDC *pcDC)
{

}

void CTonView::ActiveObjectBoxsDraw(CDC *pcDC)
{
	DBL		sx, sy, ex, ey;
	int		rval, gval, bval;
	int		ndot, nhdot;

	CTonDoc* pcDoc = GetDocument();
	pcDoc->GetDMiniMax(ACTIVEFLAG, &sx, &sy, &ex, &ey);
	senvgetslctclr(&rval, &gval, &bval);
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	senvgetareadotsize(&ndot);
	nhdot = ndot/2;
	sx = sx - ndot;
	sy = sy + ndot;
	ex = ex + ndot;
	ey = ey - ndot;

	CPen penStroke;
	if (!penStroke.CreatePen(PS_SOLID, 1, RGB(rval, gval, bval)))
		return;
	CPen* pOldPen = pcDC->SelectObject(&penStroke);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)sx, (int)ey);
	pcDC->LineTo((int)ex, (int)ey);
	pcDC->LineTo((int)ex, (int)sy);
	pcDC->LineTo((int)sx, (int)sy);
	pcDC->SelectObject(pOldPen);
}

void CTonView::WorkAreaBackClrFill(CDC *pcDC)
{
	int		rval, gval, bval;
	DBL		dxsize, dysize;
	int		winsx, winsy, winex, winey;

	scrngetwinarea(&winsx, &winsy, &winex, &winey);
	CRect *rect = new CRect();
	GetWindowRect(rect);
	senvgetbkclr(&rval, &gval, &bval);
	pcDC->SetBkColor(RGB(rval, gval, bval));
	dxsize = rect->right - rect->left;
	dysize = rect->bottom - rect->top;
	//scrngetrltvsize(&dxsize, &dysize);
	rect->top = (long)(winsy-dysize);
	rect->left = (long)(winsx-dxsize);
	rect->bottom = (long)(winsy + dysize*2);
	rect->right = (long)(winsx + dxsize*2);
	CBrush *pcBrush = new CBrush(RGB(rval, gval, bval));
	pcDC->FillRect(rect, pcBrush);
	delete pcBrush;
	delete rect;
}

void CTonView::ActiveObjectOnDraw(CDC *pcDC)
{
	CTonDoc* pcDoc = GetDocument();

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;

	POSITION pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		CData* pData = pcObjDataList->GetNext(pos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			pData->StockClrDraw(pcDC);
		}
	}
}

void CTonView::WorkAreaDraw(CDC* pcDC)
{
	DBL		mmxsize, mmysize;
	int		rval, gval, bval;

	senvgetareaclr(&rval, &gval, &bval);
	scrngetmmsize(&mmxsize, &mmysize);
	mmysize = 0;
	scrndxymmtorltv(&mmxsize, &mmysize);

	CPen penStroke;
	if (!penStroke.CreatePen(PS_SOLID, 1, RGB(rval, gval, bval)))
		return;
	CPen* pOldPen = pcDC->SelectObject(&penStroke);
	pcDC->MoveTo((int)0, (int)0);
	pcDC->LineTo((int)0, (int)mmysize);
	pcDC->LineTo((int)mmxsize, (int)mmysize);
	pcDC->LineTo((int)mmxsize, (int)0);
	pcDC->LineTo((int)0, (int)0);
	pcDC->SelectObject(pOldPen);
}

void CTonView::WorkAreaGridDraw(CDC* pcDC)
{
	DBL		dgrid;
	DBL		cntrx, cntry;
	DBL		len;
	DBL		mmxsize, mmysize;
	DBL		gridsx, gridsy;
	DBL		areasx, areasy;
	DBL		areaex, areaey;
	DBL		tx, ty;
	DBL		sx, sy, ex, ey;
	int		rval, gval, bval;
	CPen	penStroke;

	senvgetgridclr(&rval, &gval, &bval);
	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	pcSysEnv->ScrnGetGrid(&dgrid);
	pcSysEnv->ScrnGetCntrXY(&cntrx, &cntry);
	len = dgrid;
	scrndlenmmtorltv(&len);
	if(len < 8){
		return;
	}
	scrngetmmsize(&mmxsize, &mmysize);
	gridsx = cntrx - (int)(cntrx / dgrid) * dgrid;
	gridsy = cntry - (int)(cntry / dgrid) * dgrid;
	areasx = 0;
	areasy = 0;
	areaex = mmxsize;
	areaey = mmysize;
	if (!penStroke.CreatePen(PS_SOLID, 1, RGB(rval, gval, bval)))
		return;
	CPen* pOldPen = pcDC->SelectObject(&penStroke);
	for(tx = gridsx; tx < areaex; tx += dgrid){
		sy = areasy;
		ey = areaey;
		sx = tx;
		ex = tx;
		scrndxymmtorltv(&sx, &sy);
		scrndxymmtorltv(&ex, &ey);
		pcDC->MoveTo((int)sx, (int)sy);
		pcDC->LineTo((int)ex, (int)ey);
	}

	for(ty = gridsy; ty < areaey;  ty += dgrid){
		sx = areasx;
		ex = areaex;
		sy = ty;
		ey = ty;
		scrndxymmtorltv(&sx, &sy);
		scrndxymmtorltv(&ex, &ey);
		pcDC->MoveTo((int)sx, (int)sy);
		pcDC->LineTo((int)ex, (int)ey);
	}

	pcDC->SelectObject(pOldPen);
}

void CTonView::WorkAreaCntrLineDraw(CDC* pcDC)
{
	DBL		cntrx, cntry;
	DBL		mmxsize, mmysize;
	DBL		sx, sy, ex, ey;
	int		rval, gval, bval;
	CPen	penStroke;

	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	pcSysEnv->ScrnGetCntrXY(&cntrx, &cntry);
	scrngetmmsize(&mmxsize, &mmysize);
	senvgetcntrclr(&rval, &gval, &bval);
	if (!penStroke.CreatePen(PS_SOLID, 1, RGB(rval, gval, bval)))
		return;
	CPen* pOldPen = pcDC->SelectObject(&penStroke);
	sx = cntrx; ex = cntrx;
	sy = 0; ey = mmysize;
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)ey);

	sx = 0; ex = mmxsize;
	sy = cntry; ey = cntry;
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)ey);

	pcDC->SelectObject(pOldPen);
}

void CTonView::WorkAreaSubLineDraw(CDC* pcDC)
{
	DBL		cx, cy, rag;
	DBL		sx, sy, ex, ey;
	int		rval, gval, bval;
	CPen	penStroke;
	DBL		a, b;

	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	if(pcSysEnv->ScrnGetSubXY(&cx, &cy) == OFF){
		return;
	}
	pcSysEnv->ScrnGetSubRag(&rag);
	senvgetasstclr(&rval, &gval, &bval);
	if (!penStroke.CreatePen(PS_SOLID, 1, RGB(rval, gval, bval)))
		return;
	CPen* pOldPen = pcDC->SelectObject(&penStroke);
	scrndxymmtorltv(&cx, &cy);
	sx = cx;
	ex = cx;
	sy = -100;
	ey = 100;
	RoundTrns(sx, sy, cx, cy, rag, &sx, &sy);
	RoundTrns(ex, ey, cx, cy, rag, &ex, &ey);
	if(sx == ex){
		sy = 0; ey = 30000;
	}
	else if(sy == ey){
		sx = 0; ex = 30000;
	}
	else{
		a = (sy-ey) / (sx-ex);
		b = sy - a*sx;
		if(fabs(a) < 1){
			sx = 0;
			sy = a*sx+b;
			ex = 30000;
			ey = a*ex+b;
		}
		else{
			sy = 0;
			sx = (sy-b)/a;
			ey = 30000;
			ex = (ey-b)/a;
		}
	}
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)ey);
	sx = -100;
	ex = 100;
	sy = cy;
	ey = cy;
	RoundTrns(sx, sy, cx, cy, rag, &sx, &sy);
	RoundTrns(ex, ey, cx, cy, rag, &ex, &ey);
	if(sx == ex){
		sy = 0; ey = 30000;
	}
	else if(sy == ey){
		sx = 0; ex = 30000;
	}
	else{
		a = (sy-ey) / (sx-ex);
		b = sy - a*sx;
		if(fabs(a) < 1){
			sx = 0;
			sy = a*sx+b;
			ex = 30000;
			ey = a*ex+b;
		}
		else{
			sy = 0;
			sx = (sy-b)/a;
			ey = 30000;
			ex = (ey-b)/a;
		}
	}
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)ey);
	pcDC->SelectObject(pOldPen);
}

void CTonView::WorkAreaScale(CDC * pcDC, DBL rltvtopx, DBL rltvtopy)
{
	DBL		scalehight;
	DBL		basemm;
	DBL		rltvdot;
	int		cnt, add;
	DBL		mmxsize, mmysize;
	DBL		areasx, areasy;
	DBL		areaex, areaey;
	DBL		tx, ty;
	DBL		sx, sy, ex, ey;
	CPen	chPen;
	CFont	chFont;
	TCHAR	cntbuf[RECMAX];
	int		len;
	int		rval, gval, bval;

	senvgetscalclr(&rval, &gval, &bval);
	scalehight  = 4;
	for(basemm = 1.0; basemm <= 10000; basemm *= 10.0){
		rltvdot = basemm;
		scrndlenmmtorltv(&rltvdot);
		if(4 <= rltvdot){
			add = (int)basemm;
			break;
		}
		scalehight += 4;
	}
	scrngetmmsize(&mmxsize, &mmysize);
	areasx = 0;
	areasy = 0;
	areaex = mmxsize;
	areaey = mmysize;
	if (!chPen.CreatePen(PS_SOLID, 1, RGB(rval, gval, bval))){
		return;
	}
	if(!chFont.CreateFont(10, 5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, _T("ＭＳ 明朝"))){
		return;
	}
	CPen *cpholdPen = pcDC->SelectObject(&chPen);
	CFont *cpholdFont = pcDC->SelectObject(&chFont);
	pcDC->SetTextColor(RGB(rval, gval, bval));
	cnt = 0;
	for(tx = areasx; tx < areaex; tx += basemm){
		sy = areasy;
		ey = sy;
		sx = tx;
		ex = tx;
		scrndxymmtorltv(&sx, &sy);
		scrndxymmtorltv(&ex, &ey);
		sy = rltvtopy;
		ey = sy + scalehight;
		if((cnt % 5) == 0){
			ey = sy + scalehight + 2;
		}
		if((cnt % 10) == 0){
			ey = sy + scalehight + 4;
		}
		if((cnt % 50) == 0){
			ey = sy + scalehight + 6;
		}
		if((cnt % 100) == 0){
			ey = sy + scalehight + 8;
		}
		pcDC->MoveTo((int)sx, (int)sy);
		pcDC->LineTo((int)ex, (int)ey);
		cnt++;
	}

	cnt = 0;
	for(tx = areasx; tx < areaex; tx += basemm){
		sy = areasy;
		ey = sy;
		sx = tx;
		ex = tx;
		scrndxymmtorltv(&sx, &sy);
		scrndxymmtorltv(&ex, &ey);
		sy = rltvtopy;
		ey = sy + scalehight;
		if((cnt % 10) == 0){
			_stprintf_s(cntbuf, _T("%d"), cnt*add);
			len = _tcslen(cntbuf);
			pcDC->TextOut((int)sx, (int)sy, cntbuf, len);
		}
		cnt++;
	}

	cnt = 0;
	for(ty = areasy; ty < areaey;  ty += basemm){
		sx = areasx;
		ex = sx;
		sy = ty;
		ey = ty;
		scrndxymmtorltv(&sx, &sy);
		scrndxymmtorltv(&ex, &ey);
		sx = rltvtopx;
		ex = sx + scalehight;
		if((cnt % 5) == 0){
			ex = sx + scalehight + 2;
		}
		if((cnt % 10) == 0){
			ex = sx + scalehight + 4;
		}
		if((cnt % 50) == 0){
			ex = sx + scalehight + 6;
		}
		if((cnt % 100) == 0){
			ex = sx + scalehight + 8;
		}

		pcDC->MoveTo((int)sx, (int)sy);
		pcDC->LineTo((int)ex, (int)ey);
		cnt++;
	}

	cnt = 0;
	for(ty = areasy; ty < areaey;  ty += basemm){
		sx = areasx; ex = sx;
		sy = ty; ey = ty;
		scrndxymmtorltv(&sx, &sy);
		scrndxymmtorltv(&ex, &ey);
		sx = rltvtopx;
		ex = sx + scalehight;
		if((cnt % 10) == 0){
			_stprintf_s(cntbuf, _T("%d"), cnt*add);
			len = _tcslen(cntbuf);
			pcDC->TextOut((int)sx, (int)sy, cntbuf, len);
		}
		cnt++;
	}

	pcDC->SelectObject(cpholdPen);
	pcDC->SelectObject(cpholdFont);

}

void CTonView::OnInitialUpdate()
{
	CTonApp*	theApp;
	CMainFrame* theWnd;

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();

	theWnd->MDIMaximize(GetParentFrame()); 

	m_fWinExist = TRUE;
	//ShowWindow(SW_SHOWMINIMIZED);
	WorkSizeReset();
	scrnsetrltvtop(0, 0);
	scrnrltvareaset(0, 0, INTMAX, INTMAX);

	CScrollView::OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CTonView クラスの印刷
BOOL CTonView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// デフォルトの印刷準備
	return DoPreparePrinting(pInfo);
}

void CTonView::OnBeginPrinting(CDC* pcDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
	DBL		dPrnPageXSize, dPrnPageYSize;
	DBL		dmmxsize, dmmysize;
	DBL		dMoveX, dMoveY;
	
	// TODO: 印刷前の特別な初期化処理を追加してください。
	m_nBackDrawMode = senvsetdrawmode(PRINTVIEW);
	m_dPrnMMXSize = pcDC->GetDeviceCaps(HORZSIZE);
	m_dPrnMMYSize = pcDC->GetDeviceCaps(VERTSIZE);
	m_dDpiX = pcDC->GetDeviceCaps(LOGPIXELSX);
	m_dDpiY = pcDC->GetDeviceCaps(LOGPIXELSY);
	scrngetmmsize(&m_dMMXSize, &m_dMMYSize);
	if(m_nScaleTrns == ON){
		if(m_dPrnAreaMMX >= m_dMMXSize 
		&& m_dPrnAreaMMY >= m_dMMYSize){
			m_dPrnTimeX = (25.4 / m_dDpiX);
			m_dPrnTimeY = (25.4 / m_dDpiY);
		}
		else{
			dPrnPageXSize = m_dPrnAreaMMX / 25.4 * m_dDpiX;
			dPrnPageYSize = m_dPrnAreaMMY / 25.4 * m_dDpiY;
			scrngetmmsize(&dmmxsize, &dmmysize);
			m_dPrnTimeX = dmmxsize / dPrnPageXSize;
			m_dPrnTimeY = dmmysize / dPrnPageYSize;
			if(m_dPrnTimeX > m_dPrnTimeY){
				m_dPrnTimeY = m_dPrnTimeX * m_dDpiX / m_dDpiY;
			}
			else{
				m_dPrnTimeX = m_dPrnTimeY * m_dDpiY / m_dDpiX;
			}
		}

		m_nDivPageLast = 2;
		m_dBackTime = scrnsetxytime(m_dPrnTimeX, m_dPrnTimeY);
		dMoveX = m_dMoveMMX;
		dMoveY = m_dMoveMMY;
		scrndlenmmtorltv(&dMoveX);
		scrndlenmmtorltv(&dMoveY);
		scrnsetrltvtop(0.0, 0.0);
		scrnsetrltvmove(dMoveX, dMoveY);
	}
	else{
		m_dPrnTimeX = (25.4 / m_dDpiX);
		m_dPrnTimeY = (25.4 / m_dDpiY);
		m_dBackTime = scrnsetxytime(m_dPrnTimeX, m_dPrnTimeY);
		scrnsetrltvelement();
		DivPageCountSet(m_dMMXSize, m_dMMYSize, m_dPrnMMXSize, m_dPrnMMYSize);
		scrnsetrltvtop(0.0, 0.0);
		scrnsetrltvmove(0.0, 0.0);
	}
}

void CTonView::OnEndPrinting(CDC* /*pcDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
	senvsetdrawmode(m_nBackDrawMode);
	scrnsetrltvmove(0, 0);
	scrnsetxytime(m_dBackTime, m_dBackTime);
	scrnsetrltvelement();
	scrnsetrltvtop(0.0, 0.0);
	scrnrltvareaset(0, 0, INTMAX, INTMAX);
	Invalidate();
}

void CTonView::OnDestroy()
{
	// 消滅時にアイテムを非アクティベートします; これは分割ビューが
	// 使用されている場合には重要です。
   CScrollView::OnDestroy();
}
// オブジェクトが組み込み先で編集されはじめたときには OnSetFocus と OnSize の特別な
// ハンドリングにはコンテナが必要です。
void CTonView::OnSetFocus(CWnd* pOldWnd)
{
	POINT MousePt;
	CTonApp*	theApp;
	CMainFrame*	theWnd;

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	theWnd->SetCrtView(this);
	::GetCursorPos(&MousePt);
	CommandExec(SETFOCUSEVENT, (long)(MousePt.x), (long)(MousePt.y));

	CScrollView::OnSetFocus(pOldWnd);
}

void CTonView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
	WorkSizeReset();
}

/////////////////////////////////////////////////////////////////////////////
// CTonView クラスの診断

#ifdef _DEBUG
void CTonView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTonView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTonDoc* CTonView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTonDoc)));
	return (CTonDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTonView クラスのメッセージ ハンドラ
DROPEFFECT CTonView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	if (!GetSizeInfo(pDataObject, m_szObjectSize, m_szPreviousOffset))
		return DROPEFFECT_NONE;

	CClientDC dc(NULL);
	dc.HIMETRICtoDP(&m_szObjectSize);
	dc.HIMETRICtoDP(&m_szPreviousOffset);

	return OnDragOver(pDataObject, dwKeyState, point);
}

BOOL CTonView::GetSizeInfo(COleDataObject* pObject, CSize& szSize, CSize& szPlace)
{
	HGLOBAL hDesc = pObject->GetGlobalData(m_cfObjectDescriptor);
	if (hDesc == NULL)
		return FALSE;

	LPOBJECTDESCRIPTOR pDesc = (LPOBJECTDESCRIPTOR) ::GlobalLock(hDesc);
	if (pDesc == NULL)
		return FALSE;

	szSize.cx = (int) pDesc->sizel.cx;
	szSize.cy = (int) pDesc->sizel.cy;

	szPlace.cx = (int) pDesc->pointl.x;
	szPlace.cy = (int) pDesc->pointl.y;

	::GlobalUnlock(hDesc);
	::GlobalFree(hDesc);
	return TRUE;
}

void CTonView::OnDragLeave() 
{
	CClientDC dc(this);
	dc.DrawFocusRect(CRect(m_ptDragPoint, m_szObjectSize));
}

DROPEFFECT CTonView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	point -= m_szPreviousOffset;

	DROPEFFECT retval;
	if (dwKeyState & MK_CONTROL)
		retval = DROPEFFECT_COPY;
	else
		retval = DROPEFFECT_MOVE;

	if (point == m_ptDragPoint)
		return retval;

	CClientDC dc(this);
	dc.DrawFocusRect(CRect(m_ptDragPoint, m_szObjectSize));
	m_ptDragPoint = point;
	dc.DrawFocusRect(CRect(m_ptDragPoint, m_szObjectSize));
	return retval;
}
int CTonView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CTonView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

// 貼り付け
void CTonView::OnEditPaste() 
{
	CTonDoc*	pcDoc = GetDocument();
	ASSERT_VALID(pcDoc);

	if(pcDoc->CheckArchive() == TRUE){
		pcDoc->LoadArchive();
		pcDoc->SetModifiedFlag();
		Invalidate();
	}
	return;	// 終了するのを回避するため
}

void CTonView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	CTonDoc* pcDoc = GetDocument();
	if (pcDoc->DataActiveCheck() == FALSE)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CTonView::OnEditCopy() 
{
	CTonDoc* pcDoc = GetDocument();
	if(pcDoc->DataActiveCheck() == TRUE){
		pcDoc->SaveArchive();
	}
}

void CTonView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	CTonDoc* pcDoc = GetDocument();
	if (pcDoc->DataActiveCheck() == FALSE)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CTonView::OnEditCut() 
{
	CTonDoc* pcDoc = GetDocument();
	if(pcDoc->DataActiveCheck() == TRUE){
		pcDoc->SaveArchive();
		pcDoc->DataActiveCls();
		pcDoc->SetModifiedFlag();
		Invalidate();
	}
}
void CTonView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	CTonDoc* pcDoc = GetDocument();
	if (pcDoc->CheckArchive() == TRUE)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}
void CTonView::MouseSetCrt(DBL cx, DBL cy)
{
	m_nSetBx = (int)cx;
	m_nSetBy = (int)cy;
}
void CTonView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	BOOL	ret;

	m_nKeyFlags = nFlags;
	m_nBx = point.x;
	m_nBy = point.y;
	CommandExec(DBLSETEVENT, point.x, point.y);
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CTonView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	BOOL	ret;

	m_nKeyFlags = nFlags;
	m_nSetBx = point.x;
	m_nSetBy = point.y;
	m_nBx = point.x;
	m_nBy = point.y;
	CommandExec(SETEVENT, point.x, point.y);
	m_nDragFlag = ON;
	CScrollView::OnLButtonDown(nFlags, point);
}

void CTonView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_nKeyFlags = nFlags;
	if(m_nDragMode == ON && m_nDragFlag == ON){
		m_nDragFlag = OFF;
		m_nBx = point.x;
		m_nBy = point.y;
		CommandExec(SETEVENT2, point.x, point.y);
	}
	CScrollView::OnLButtonUp(nFlags, point);
}

void CTonView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	BOOL	ret;
	int		nx, ny;
	CRect	rect;

	m_nKeyFlags = nFlags;
		m_nBx = point.x;
		m_nBy = point.y;
		if(GetCrtCommand() == ID_MAIN_SLCT){
			CMenu menu;
			VERIFY(menu.LoadMenu(IDR_POPUPMENU));
			CMenu* pPopup = menu.GetSubMenu(0);
			ASSERT(pPopup != NULL);
			GetWindowRect(&rect);
			nx = point.x + rect.left;
			ny = point.y + rect.top;
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, nx, ny, GetParentFrame());
		}
		else{
			CommandExec(RESETEVENT, point.x, point.y);
		}
	CScrollView::OnRButtonDown(nFlags, point);
}

void CTonView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	m_nKeyFlags = nFlags;
	/*
	if(m_nDragMode == ON && m_nDragFlag == ON){
		m_nDragFlag = OFF;
		m_nBx = point.x;
		m_nBy = point.y;
		CommandExec(RESETEVENT, point.x, point.y);
	}
	*/
	CScrollView::OnRButtonUp(nFlags, point);
}

void CTonView::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_nKeyFlags = nFlags;
	if(point.x != m_nBx || point.y != m_nBy){
		//if(m_nDragMode == OFF){
			CommandExec(MOVEEVENT, point.x, point.y);
		//}
		//else if(m_nDragFlag == ON){ // m_nDragMode is ON
		//	CommandExec(MOVEEVENT, point.x, point.y);
		//}
		m_nBx = point.x;
		m_nBy = point.y;
	}
	CScrollView::OnMouseMove(nFlags, point);
}

void CTonView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CTonApp *theApp;
	CSysEnv *pcSysEnv;

	// 日本語変換前のコードが取得できる
	switch(nChar){
	case VK_RETURN:
		CommandExec(SETEVENT, (long)(m_nBx), (long)(m_nBy));
		break;
	case VK_ESCAPE:
		CommandExit();
		break;
	case VK_CONTROL:
		theApp = (CTonApp*)AfxGetApp();
		pcSysEnv = theApp->m_pcSysEnv;
		pcSysEnv->ScrnSetCntrlKey(OFF);
		break;
	case VK_SHIFT:
		theApp = (CTonApp*)AfxGetApp();
		pcSysEnv = theApp->m_pcSysEnv;
		pcSysEnv->ScrnSetShiftKey(OFF);
		break;
	}
	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CTonView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	POINT MousePt;
	CTonApp *theApp;
	CSysEnv *pcSysEnv;

	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	if(m_fWinExist == FALSE){
		CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	switch(nChar){
	case VK_CONTROL:
		theApp = (CTonApp*)AfxGetApp();
		pcSysEnv = theApp->m_pcSysEnv;
		pcSysEnv->ScrnSetCntrlKey(ON);
		break;
	case VK_SHIFT:
		theApp = (CTonApp*)AfxGetApp();
		pcSysEnv = theApp->m_pcSysEnv;
		pcSysEnv->ScrnSetShiftKey(ON);
		break;
	case VK_NUMPAD0:
	case '0':
		CommandExec(RESETEVENT, (long)(m_nBx), (long)(m_nBy));
		break;
	case VK_RETURN:
		CommandExec(SETEVENT, (long)(m_nBx), (long)(m_nBy));
		break;
	case VK_NUMPAD5:
	case '5':
		CommandExec(SETEVENT, (long)(m_nBx), (long)(m_nBy));
		break;
	case VK_NUMPAD1:
	case '1':
		::GetCursorPos(&MousePt);
		MousePt.x -= 1;
		MousePt.y += 1;
		::SetCursorPos(MousePt.x, MousePt.y);
		break;
	case VK_NUMPAD2:
	case '2':
		::GetCursorPos(&MousePt);
		MousePt.y += 1;
		::SetCursorPos(MousePt.x, MousePt.y);
		break;
	case VK_NUMPAD3:
	case '3':
		::GetCursorPos(&MousePt);
		MousePt.x += 1;
		MousePt.y += 1;
		::SetCursorPos(MousePt.x, MousePt.y);
		break;
	case VK_NUMPAD4:
	case '4':
		::GetCursorPos(&MousePt);
		MousePt.x -= 1;
		::SetCursorPos(MousePt.x, MousePt.y);
		break;
	case VK_NUMPAD6:
	case '6':
		::GetCursorPos(&MousePt);
		MousePt.x += 1;
		::SetCursorPos(MousePt.x, MousePt.y);
		break;
	case VK_NUMPAD7:
	case '7':
		::GetCursorPos(&MousePt);
		MousePt.x -= 1;
		MousePt.y -= 1;
		::SetCursorPos(MousePt.x, MousePt.y);
		break;
	case VK_NUMPAD8:
	case '8':
		::GetCursorPos(&MousePt);
		MousePt.y -= 1;
		::SetCursorPos(MousePt.x, MousePt.y);
		break;
	case VK_NUMPAD9:
	case '9':
		::GetCursorPos(&MousePt);
		MousePt.x += 1;
		MousePt.y -= 1;
		::SetCursorPos(MousePt.x, MousePt.y);
		break;
	case VK_LEFT:
		::GetCursorPos(&MousePt);
		MousePt.x -= 8;
		::SetCursorPos(MousePt.x, MousePt.y);
		break;
	case VK_UP:
		::GetCursorPos(&MousePt);
		MousePt.y -= 8;
		::SetCursorPos(MousePt.x, MousePt.y);
		break;
	case VK_RIGHT:
		::GetCursorPos(&MousePt);
		MousePt.x += 8;
		::SetCursorPos(MousePt.x, MousePt.y);
		break;
	case VK_DOWN:
		::GetCursorPos(&MousePt);
		MousePt.y += 8;
		::SetCursorPos(MousePt.x, MousePt.y);
		break;
	case VK_ESCAPE:
		CommandExit();
		break;
	}
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

UINT CTonView::CommandExit()
{
	UINT	ret;

	if(m_fWinExist == FALSE){
		return(0);
	}
	ret = CommandExec(ENDEVENT, 0L, 0L);
	return(ret);
}

UINT CTonView::GetCrtCommand()
{
	return(m_pCommand->GetCrtCommand());
}

UINT CTonView::GetSubCommand()
{
	return(m_pCommand->GetSubCommand());
}

void CTonView::SetSubCommand(int SubCommand)
{
	m_pCommand->SetSubCommand(SubCommand);
}

void CTonView::SetNextCommand(int cmd)
{
	m_nNextCommand = cmd;
}

UINT CTonView::CommandInit(UINT Command)
{
	UINT	ret;

	if(m_fWinExist == FALSE){
		return(0);
	}
	CTonDoc* pcDoc = GetDocument();
	m_pCommand->SetDocument(pcDoc);
	m_pCommand->SetCrtCommand(Command);
	ret = CommandExec(INITEVENT, 0L, 0L);
	return(ret);
}

UINT CTonView::CommandExec(UINT event, long x, long y)
{
	UINT	ret;

	if(m_fWinExist == FALSE){
		return(0);
	}
	
	ret = m_pCommand->Exec(event, x, y);
	if(ret == ENDEVENT || event == ENDEVENT){
		CommandSetDef();
	}
	return(ret);
}

void CTonView::WorkSizeReset()
{
	CRect	rectClient;
	int		w, h;
	DBL		topx, topy;
	DBL		xtime, ytime;

	if(m_pCommand->GetCrtCommand() == ID_MAIN_ZOOM){
		return;
	}
	if(m_fWinExist == TRUE){
		scrngetrltvtop(&topx, &topy);
		scrnsetrltvtop(0.0, 0.0);
		scrndxyrltvtomm(&topx, &topy);
		scrngetxytime(&xtime, &ytime);
		GetClientRect(&rectClient);
		h = rectClient.bottom - rectClient.top;
		w = rectClient.right - rectClient.left;
		scrnsetwinarea(rectClient.left, rectClient.top, rectClient.right, rectClient.bottom);
		scrninitsizeelement(&w, &h);
		SetScrollSizes( MM_TEXT, GetDocument()->GetDocSize());
		if(m_nFirst == FALSE){
			scrnsetxytime(xtime, ytime);
			scrnsetrltvtop(0.0, 0.0);
			scrnsetrltvelement();
			scrndxymmtorltv(&topx, &topy);
			scrnsetrltvtop(topx, topy);
			InitScrollElement();
		}
	}
}

void CTonView::WorkAreaReset()
{
	scrnresetzoomtime();
	scrnsetrltvelement();
	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
	ScrollToPosition(GetDocument()->GetTopPoint());
	InitScrollElement();
}

void CTonView::OnPrepareDC(CDC* pcDC, CPrintInfo* pInfo) 
{
	UINT	page, crtx, crty;
	DBL		prnx, prny;
	DBL		sx, sy, ex, ey;

	CScrollView::OnPrepareDC(pcDC, pInfo);
	if(senvcheckdrawmode() == PRINTVIEW
	&& pInfo != NULL){
		if(pInfo->m_nCurPage > m_nDivPageLast){
			pInfo->m_bContinuePrinting = FALSE;
		}

		if(m_nScaleTrns == ON){
			scrnsetrltvtop(0.0, 0.0);
			scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
		}
		else{
			page = pInfo->m_nCurPage - 1;
			crtx = page % m_nXDivPage;
			crty = page / m_nXDivPage;
			scrnsetrltvtop(0.0, 0.0);
			prnx = m_dPrnMMXSize * crtx;
			prny = m_dPrnMMYSize * crty;
			scrndxymmtorltv(&prnx, &prny);
			sx = m_dPrnMMXSize * crtx;
			sy = m_dPrnMMYSize * crty;
			ex = sx + m_dPrnMMXSize;
			ey = sy + m_dPrnMMYSize;
			scrndxymmtorltv(&sx, &sy);
			scrndxymmtorltv(&ex, &ey);
			//scrnrltvareaset(sx, ey, ex, sy);
			scrnsetrltvtop(0.0, 0.0);
			scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
		}
	}
}

void CTonView::DivPageCountSet(DBL workx, DBL worky, DBL prnx, DBL prny)
{
	long	lwx, lwy;
	long	lpx, lpy;

	lwx = (long)workx;
	lwy = (long)worky;
	lpx = (long)prnx;
	lpy = (long)prny;
	m_nXDivPage = lwx / lpx;
	if(lwx % lpx){
		m_nXDivPage++;
	}
	m_nYDivPage = lwy / lpy;
	if(lwy % lpy){
		m_nYDivPage++;
	}
	m_nDivPageLast = m_nXDivPage*m_nYDivPage;
}

//ここまでは各システム共通

void CTonView::OnUpdateObjFigu(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCrtCommand() == ID_OBJ_FIGU);
}

void CTonView::OnUpdateObjMsur(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCrtCommand() == ID_OBJ_MSUR);
}

void CTonView::OnUpdateStrgInput(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_STRG_INPUT);
}

void CTonView::SetFiguKind(int nfigukind)
{
}

int CTonView::GetFiguKind()
{
	return(ID_FIGU_BOXS);
}

// stan 関数実行部
void CTonView::OnFileEnvset() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		CommandInit(ID_FILE_ENVSET);
	}
}

void CTonView::OnObjFigu()
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(GetFiguKind());
		CommandInit(ID_OBJ_FIGU);
	}
}

void CTonView::OnObjMsur() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		CommandInit(ID_OBJ_MSUR);
	}
}

void CTonView::OnLineConerarc() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_CONERARC);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnLineConerset() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_CONERSET);
		CommandInit(ID_TOOL_LINE);
	}
	
}

void CTonView::OnLineCrcl() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_CRCL);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnLineCurveing() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_CURVEING);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnLineCut() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_CUT);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnLineCutjoin() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_CUTJOIN);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnLineDel() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_DEL);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnLineJoin() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_JOIN);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnLineLineing() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_LINEING);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnLineMove() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_MOVE);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnObjBmp() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_OBJ_BMP);
		CommandInit(ID_OBJ_BMP);
	}
}

void CTonView::OnBmpRound() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_BMP_ROUND);
		CommandInit(ID_OBJ_BMP);
	}
	
}

void CTonView::OnObjWaku() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_WAKU_CREATE);
		CommandInit(ID_OBJ_WAKU);
	}
}

void CTonView::OnPointAdd() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_ADD);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnPointChngatr() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_CHNGATR);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnPointDel() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_DEL);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnPointDiv() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_DIV);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnPointLegal() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_LEGAL);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnPointMove() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_MOVE);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnSpclBitmaping() 
{
}

void CTonView::OnSpclOutline() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SPCL_OUTLINE);
		CommandInit(ID_TOOL_SPCL);
	}
}

void CTonView::OnSpclMarge() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SPCL_MARGE);
		CommandInit(ID_TOOL_SPCL);
	}
}

void CTonView::OnSpclParallel() 
{
}

void CTonView::OnSpclPrepend() 
{
}

void CTonView::OnSpclShadow() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SPCL_SHADOW);
		CommandInit(ID_TOOL_SPCL);
	}
}

void CTonView::OnSpclStrgstrct() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SPCL_STRGSTRCT);
		CommandInit(ID_TOOL_SPCL);
	}
}

void CTonView::OnToolExport() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_TOOL_EXPORT);
		CommandInit(ID_MAIN_TOOL);
	}
}

void CTonView::OnToolInport() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_TOOL_INPORT);
		CommandInit(ID_MAIN_TOOL);
	}
}

void CTonView::OnSlctArea() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SLCT_AREA);
		CommandInit(ID_MAIN_SLCT);
	}
}

void CTonView::OnSlctObj() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SLCT_OBJ);
		CommandInit(ID_MAIN_SLCT);
	}
}

void CTonView::OnZoomAll() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_ZOOM_ALL);
		CommandInit(ID_MAIN_ZOOM);
	}
}

void CTonView::OnZoomIn() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_ZOOM_IN);
		CommandInit(ID_MAIN_ZOOM);
	}
}

void CTonView::OnZoomOut() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_ZOOM_OUT);
		CommandInit(ID_MAIN_ZOOM);
	}
}

void CTonView::OnToolTrns() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_TRNS_SIZE);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnViewPreview() 
{
	if(senvcheckdrawmode() == PREVIEW){
		senvsetdrawmode(POINTVIEW);
	}
	else{
		senvsetdrawmode(PREVIEW);
	}
	Invalidate();
}

void CTonView::OnViewGrid() 
{
	if(senvcheckdrawgrid() == ON){
		senvoffdrawgrid();
	}
	else{
		senvondrawgrid();
	}
	Invalidate();
}

void CTonView::OnViewImage() 
{
	if(senvcheckdrawimage() == ON){
		senvoffdrawimage();
	}
	else{
		senvondrawimage();
	}
	Invalidate();
}

void CTonView::OnUpdateViewPreview(CCmdUI* pCmdUI) 
{
	if(senvcheckdrawmode() == PREVIEW){
		pCmdUI->SetCheck(1);
	}
	else{
		pCmdUI->SetCheck(0);
	}
}

void CTonView::OnUpdateViewGrid(CCmdUI* pCmdUI) 
{
	if(senvcheckdrawgrid() == ON){
		pCmdUI->SetCheck(1);
	}
	else{
		pCmdUI->SetCheck(0);
	}
}

void CTonView::OnUpdateViewImage(CCmdUI* pCmdUI) 
{
	if(senvcheckdrawimage() == ON){
		pCmdUI->SetCheck(1);
	}
	else{
		pCmdUI->SetCheck(0);
	}
}

void CTonView::OnUpdateZoomAll(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_ZOOM_ALL);
}

void CTonView::OnUpdateZoomIn(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_ZOOM_IN);
}

void CTonView::OnUpdateSlctObj(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_SLCT_OBJ);
}

void CTonView::OnUpdateFiguBoxs(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_FIGU_BOXS);
}

void CTonView::OnUpdateFiguCrcl(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_FIGU_CRCL);
}

void CTonView::OnUpdateFiguFree(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_FIGU_FREE);
}

void CTonView::OnUpdateFiguLine(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_FIGU_LINE);
}

void CTonView::OnUpdateFiguPoint(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_FIGU_POINT);
}

void CTonView::OnUpdateLineMove(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_LINE_MOVE);
}

void CTonView::OnUpdatePointMove(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_POINT_MOVE);
}

void CTonView::OnUpdateSizeFree(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_SIZE_FREE);

}
BOOL CTonView::CheckKeyFlag(int nflag)
{
	if(m_nKeyFlags & nflag){
		return(TRUE);
	}
	return(FALSE);
}

void CTonView::InitScrollElement()
{
	SetScrollSizes( MM_TEXT, GetDocument()->GetDocSize());
	ScrollToPosition(GetDocument()->GetTopPoint());
	Invalidate();
}

void CTonView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CPoint	point;
	DBL	dx, dy;
	
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
	point = GetScrollPosition();
	dx = point.x;
	dy = point.y;
	scrnsetrltvtop(dx, dy);
	Invalidate();
}

void CTonView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CPoint	point;
	DBL	dx, dy;
	
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
	point = GetScrollPosition();
	dx = point.x;
	dy = point.y;
	scrnsetrltvtop(dx, dy);
	Invalidate();
}

BOOL CTonView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	dwStyle |= WS_MAXIMIZE;
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CTonView::OnToolTrace() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_TOOL_TRACE);
		CommandInit(ID_TOOL_TRACE);
	}
}

void CTonView::OnToolPoint() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_MOVE);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnToolLine() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_MOVE);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnSlctAll() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SLCT_ALL);
		CommandInit(ID_MAIN_SLCT);
	}
}

void CTonView::OnSlctChng() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SLCT_CHNG);
		CommandInit(ID_MAIN_SLCT);
	}
}

void CTonView::OnEditDelete() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_EDIT_DELETE);
		CommandInit(ID_MAIN_EDIT);
	}
}

void CTonView::OnEditRedo() 
{
	CTonDoc* pcDoc = GetDocument();
	if(CommandExit() == (UINT)ENDEVENT){
		pcDoc->LoadRedo();
		Invalidate();
	}	
}

void CTonView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	CTonDoc* pcDoc = GetDocument();
	if (pcDoc->CheckRedo() == TRUE)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
	
}

void CTonView::OnEditUndo() 
{
	CTonDoc* pcDoc = GetDocument();
	if(CommandExit() == (UINT)ENDEVENT){
		pcDoc->LoadUndo();
		Invalidate();
	}	
}

void CTonView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	CTonDoc* pcDoc = GetDocument();
	if (pcDoc->CheckUndo() == TRUE)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
	
}

void CTonView::OnEditMvcopy() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_EDIT_MVCOPY);
		CommandInit(ID_MAIN_EDIT);
	}
}
void CTonView::OnFileDotedit() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_DOTEDIT);
		CommandInit(ID_TOOL_TRACE);
	}
}

void CTonView::OnClrGradation() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CLR_GRADATION);
		CommandInit(ID_ATR_CLR);
	}
}

void CTonView::OnClrLine() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CLR_LINE);
		CommandInit(ID_ATR_CLR);
	}
}

void CTonView::OnClrSolid() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CLR_SOLID);
		CommandInit(ID_ATR_CLR);
	}
}

void CTonView::OnEditMove() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_EDIT_MOVE);
		CommandInit(ID_MAIN_EDIT);
	}
}

void CTonView::OnFiguBoxs() 
{
	SetFiguKind(ID_FIGU_BOXS);
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FIGU_BOXS);
		CommandInit(ID_OBJ_FIGU);
	}
}

void CTonView::OnFiguCrcl() 
{
	SetFiguKind(ID_FIGU_CRCL);
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FIGU_CRCL);
		CommandInit(ID_OBJ_FIGU);
	}
}

void CTonView::OnFiguFree() 
{
	SetFiguKind(ID_FIGU_FREE);
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FIGU_FREE);
		CommandInit(ID_OBJ_FIGU);
	}
}

void CTonView::OnFiguLine() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FIGU_LINE);
		CommandInit(ID_OBJ_FIGU);
	}
}

void CTonView::OnFiguPoint() 
{
	SetFiguKind(ID_FIGU_POINT);
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FIGU_POINT);
		CommandInit(ID_OBJ_FIGU);
	}
}

void CTonView::OnFiguSizeboxs() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FIGU_SIZEBOXS);
		CommandInit(ID_OBJ_FIGU);
	}
}

void CTonView::OnFiguSizecrcl() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FIGU_SIZECRCL);
		CommandInit(ID_OBJ_FIGU);
	}
}

void CTonView::OnFileAddenvload() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_ADDENVLOAD);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::OnFileAddload() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_ADDLOAD);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::OnFileAddsave() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_ADDSAVE);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::OnFileEpsout() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_EPSOUT);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::OnFileClrprint() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_CLRPRINT);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::OnFilePloter() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_PLOTER);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::OnFilePltmove() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_PLTMOVE);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::OnLineArc() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_ARC);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnLineBeze() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_BEZE);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnLineHalfarc() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_HALFARC);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnMirrorCntr() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MIRROR_CNTR);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnMirrorFree() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MIRROR_FREE);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnMojiDel() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_DEL);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiInseart() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_INSEART);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiMove() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_MOVE);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiPlgning() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_PLGNING);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiRound() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_ROUND);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiSize() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_SIZE);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiSpace() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_SPACE);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiSvdel() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_SVDEL);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiLoad() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_LOAD);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiSave() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_SAVE);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnPartLoad() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_PART_LOAD);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnPartSave() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_PART_SAVE);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnPoint2point() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_2POINT);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnPointAnydel() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_ANYDEL);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnPointAnymove() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_ANYMOVE);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnPointPicup() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_PICUP);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnPointSloplegal() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_SLOPLEGAL);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnPointXlegal() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_XLEGAL);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnPointYlegal() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_YLEGAL);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnPrevPane() 
{
}

void CTonView::OnRound2point() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_ROUND_2POINT);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnRoundFree() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_ROUND_FREE);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnRoundRag() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_ROUND_RAG);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSize2point() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SIZE_2POINT);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSizeFree() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SIZE_FREE);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSizeTime() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SIZE_TIME);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSizeX() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SIZE_X);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSizeXy() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SIZE_XY);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSizeY() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SIZE_Y);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSlopFree() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SLOP_FREE);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSlopRag() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SLOP_RAG);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnStrgInput() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_OBJ_STRG);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnStrgOrder() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_STRG_ORDER);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnStrgRound() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_STRG_ROUND);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnStrgSlop() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_STRG_SLOP);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnStrgBoxsin() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_STRG_BOXSIN);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnStrgEdit() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_STRG_EDIT);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnStrgProp() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_STRG_PROP);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnTrnsArc() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_TRNS_ARC);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnTrnsBoxs() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_TRNS_BOXS);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnClrSeetSet() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CLR_SEET_SET);
		CommandInit(ID_MAIN_ATR);
	}
}

void CTonView::OnAtrConerKind() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_ATR_CONER_KIND);
		CommandInit(ID_MAIN_ATR);
	}
}

void CTonView::OnWakuGrup() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_WAKU_GRUP);
		CommandInit(ID_OBJ_WAKU);
	}
}

void CTonView::OnWakuPaper() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_WAKU_PAPER);
		CommandInit(ID_OBJ_WAKU);
	}
}

void CTonView::OnWakuCreate() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_WAKU_CREATE);
		CommandInit(ID_OBJ_WAKU);
	}
}

void CTonView::OnWakuInput() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_WAKU_INPUT);
		CommandInit(ID_OBJ_WAKU);
	}
}

void CTonView::OnWakuSave() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_WAKU_SAVE);
		CommandInit(ID_OBJ_WAKU);
	}
}

void CTonView::OnWakuLoad() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_WAKU_LOAD);
		CommandInit(ID_OBJ_WAKU);
	}
}

void CTonView::OnWakuEdit() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_WAKU_EDIT);
		CommandInit(ID_OBJ_WAKU);
	}
}

void CTonView::OnWakuDelete() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_WAKU_DELETE);
		CommandInit(ID_OBJ_WAKU);
	}
}

void CTonView::OnWakuSort() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_WAKU_SORT);
		CommandInit(ID_OBJ_WAKU);
	}
}

void CTonView::OnKamonLoad() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_KAMON_LOAD);
		CommandInit(ID_OBJ_WAKU);
	}
}

void CTonView::OnEskamonLoad() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_ESKAMON_LOAD);
		CommandInit(ID_OBJ_KAMON);
	}
}

void CTonView::OnEskamonSave() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_ESKAMON_SAVE);
		CommandInit(ID_OBJ_KAMON);
	}
}

void CTonView::DrawSelectWaku(CDC *pcDC)
{
	int		noidx;
	CData* pData;

	CTonDoc* pcDoc = GetDocument();
	noidx = 0;

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	POSITION pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = pcObjDataList->GetNext(pos);

		if(pData->CheckID() == SWAKVECTID){
			if(noidx == m_nCrtWaku){
				pData->SelectDraw(pcDC);
			}
			else{
				pData->NoSelectDraw(pcDC);
			}
			noidx++;
		}
	}
}

void CTonView::PrintEnvSave(LPTSTR printenvfname)
{
	PRINTDLG	m_pd;
	LPDEVMODE	lpDevMode;
	LPDEVNAMES	lpDevNames;
	FILE		*fp;
	HANDLE		hDevMode;
	HANDLE		hDevNames;
	CTonApp*	theApp;
	int			nwritemodesize;
	int			nwritenamessize;

	theApp = (CTonApp*)AfxGetApp();
	hDevMode = theApp->GetDevMode();
	hDevNames = theApp->GetDevNames();
	if(hDevMode == 0 || hDevNames == 0){
		memset( &m_pd, 0, sizeof( PRINTDLG ) );
		m_pd.lStructSize    = sizeof( PRINTDLG );
		m_pd.hwndOwner      = m_hWnd;
		m_pd.Flags          = PD_RETURNDEFAULT;
		m_pd.hDevMode       = NULL;
		m_pd.hDevNames      = NULL;
		::PrintDlg(&m_pd);
		hDevMode = m_pd.hDevMode;
		hDevNames = m_pd.hDevNames;
	}
	lpDevMode = (LPDEVMODE)::GlobalLock(hDevMode);
	lpDevNames = (LPDEVNAMES)::GlobalLock(hDevNames);
	nwritemodesize = GlobalSize(hDevMode);
	nwritenamessize = GlobalSize(hDevNames);
	if(lpDevMode == NULL && lpDevNames == NULL){
		return;
	}

	_tfopen_s(&fp, printenvfname, _T("wb"));
	if (fp != NULL) {
		fwrite(&nwritemodesize, sizeof(int), 1, fp);
		fwrite(lpDevMode, GlobalSize(hDevMode), 1, fp);
		fwrite(&nwritenamessize, sizeof(int), 1, fp);
		fwrite(lpDevNames, GlobalSize(hDevNames), 1, fp);
		fclose(fp);
	}

	::GlobalUnlock(hDevNames);
	::GlobalUnlock(hDevMode);

}

void CTonView::PrintEnvLoad(LPTSTR printenvfname)
{
	HANDLE		hCrtDevMode;
	LPDEVMODE	lpCrtDevMode;
	FILE		*fp;
	HANDLE		hLoadDevMode;
	HANDLE		hLoadDevNames;
	LPDEVMODE	lpLoadDevMode;
	LPDEVNAMES	lpLoadDevNames;
	int			nModeSize; 
	int			nNamesSize;
	int			nCrtModeSize;
	int			ret;
	
	CTonApp *pcApp = (CTonApp*)AfxGetApp();

	_tfopen_s(&fp, printenvfname, _T("rb"));
	if (fp == NULL) {
		return;
	}
	fseek(fp, 0L, 0);
	fread(&nModeSize, sizeof(int), 1, fp);
	hLoadDevMode = GlobalAlloc(GHND, nModeSize);
	lpLoadDevMode = (LPDEVMODE)::GlobalLock(hLoadDevMode);
	fread(lpLoadDevMode, nModeSize, 1, fp);

	fread(&nNamesSize, sizeof(int), 1, fp);
	hLoadDevNames = GlobalAlloc(GHND, nNamesSize);
	lpLoadDevNames = (LPDEVNAMES)::GlobalLock(hLoadDevNames);
	fread(lpLoadDevNames, nNamesSize, 1, fp);

	::GlobalUnlock(hLoadDevNames);
	::GlobalUnlock(hLoadDevMode);

	fclose(fp);

	ret = pcApp->CheckEnumPrinter(hLoadDevNames);
	if(ret == FALSE){
		return;
	}
	hCrtDevMode = pcApp->GetDevMode();
	nCrtModeSize = ::GlobalSize(hCrtDevMode);
	lpCrtDevMode = (LPDEVMODE)::GlobalLock(hCrtDevMode);
	lpLoadDevMode = (LPDEVMODE)::GlobalLock(hLoadDevMode);

	if(nCrtModeSize == nModeSize){
		memcpy(lpCrtDevMode, lpLoadDevMode, nModeSize);
	}
	else{
		lpCrtDevMode->dmOrientation = lpLoadDevMode->dmOrientation;
		lpCrtDevMode->dmPaperSize = lpLoadDevMode->dmPaperSize;
		lpCrtDevMode->dmPaperLength = lpLoadDevMode->dmPaperLength;
		lpCrtDevMode->dmPaperWidth = lpLoadDevMode->dmPaperWidth;
		lpCrtDevMode->dmDefaultSource = lpLoadDevMode->dmDefaultSource;
	}
	::GlobalUnlock(hLoadDevMode);
	::GlobalUnlock(hCrtDevMode);

	::GlobalFree(hLoadDevNames);
	::GlobalFree(hLoadDevMode);
}

void CTonView::OnViewRedisp() 
{
	Invalidate();
}

void CTonView::OnSortHead() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SORT_HEAD);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnSortTail() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SORT_TAIL);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnSortPrev() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SORT_PREV);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnSortNext() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SORT_NEXT);
		CommandInit(ID_MAIN_CRDNT);
	}
}


void CTonView::OnGrupSet() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_GRUP_SET);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnGrupReset() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_GRUP_RESET);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnListSet() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LIST_SET);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnListReset() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LIST_RESET);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnClrSet() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CLR_SET);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnMsurXcreate() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MSUR_XCREATE);
		CommandInit(ID_OBJ_MSUR);
	}
}

void CTonView::OnMsurYcreate() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MSUR_YCREATE);
		CommandInit(ID_OBJ_MSUR);
	}
}

void CTonView::OnMsurAtrset() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MSUR_ATRSET);
		CommandInit(ID_OBJ_MSUR);
	}
}

void CTonView::OnHelpMain() 
{
	TCHAR	execpath[FNAMEMAX];
	TCHAR	execcmd[RECMAX];
	char	sExeccmd[RECMAX];

	senvgetexepath(execpath);
	_stprintf_s(execcmd, _T("Winhelp %s\\es98.hlp"), execpath);
	baselibTCHARStrToCharStr(execcmd, sExeccmd, RECMAX);
	::WinExec(sExeccmd, SW_SHOW);
}

void CTonView::OnMoveRltv() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOVE_RLTV);
		CommandInit(ID_MAIN_EDIT);
	}
}

void CTonView::OnMoveCood() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOVE_COOD);
		CommandInit(ID_MAIN_EDIT);
	}
}

void CTonView::OnCopyCood() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_COPY_COOD);
		CommandInit(ID_MAIN_EDIT);
	}
}

void CTonView::OnCopyRltv() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_COPY_RLTV);
		CommandInit(ID_MAIN_EDIT);
	}
}

void CTonView::OnCopyPich() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_COPY_PICH);
		CommandInit(ID_MAIN_EDIT);
	}
}

void CTonView::OnSubGrid() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SUB_GRID);
		CommandInit(ID_MAIN_SUB);
	}
}

void CTonView::OnSubGridOnoff() 
{
	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	if(pcSysEnv->ScrnCheckGrid() == ON){
		pcSysEnv->ScrnOnOffGrid(OFF);
	}
	else{
		pcSysEnv->ScrnOnOffGrid(ON);
	}
	Invalidate();
}

void CTonView::OnSubWaku() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SUB_WAKU);
		CommandInit(ID_MAIN_SUB);
	}
}

void CTonView::OnSubAnywaku() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SUB_ANYWAKU);
		CommandInit(ID_MAIN_SUB);
	}
}

void CTonView::OnSubWakudel() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SUB_WAKUDEL);
		CommandInit(ID_MAIN_SUB);
	}
}

void CTonView::OnUpdateLineArc(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_LINE_ARC);
}

void CTonView::OnUpdateLineBeze(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_LINE_BEZE);
	
}

void CTonView::OnUpdateLineConerarc(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_LINE_CONERARC);
}

void CTonView::OnUpdateLineConerset(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_LINE_CONERSET);
}

void CTonView::OnUpdateLineCrcl(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_LINE_CRCL);
}

void CTonView::OnUpdateLineCurveing(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_LINE_CURVEING);
}

void CTonView::OnUpdateLineCut(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_LINE_CUT);
}

void CTonView::OnUpdateLineCutjoin(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_LINE_CUTJOIN);
}

void CTonView::OnUpdateLineDel(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_LINE_DEL);
}

void CTonView::OnUpdateLineHalfarc(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_LINE_HALFARC);
}

void CTonView::OnUpdateLineJoin(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_LINE_JOIN);
}

void CTonView::OnUpdateLineLineing(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_LINE_LINEING);
}

void CTonView::OnUpdatePoint2point(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_POINT_2POINT);
}

void CTonView::OnUpdatePointAdd(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_POINT_ADD);
}

void CTonView::OnUpdatePointAnydel(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_POINT_ANYDEL);
}

void CTonView::OnUpdatePointAnymove(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_POINT_ANYMOVE);
}

void CTonView::OnUpdatePointChngatr(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_POINT_CHNGATR);
}

void CTonView::OnUpdatePointDel(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_POINT_DEL);
}

void CTonView::OnUpdatePointPicup(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_POINT_PICUP);
}

void CTonView::OnUpdatePointXlegal(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_POINT_XLEGAL);
}

void CTonView::OnUpdatePointYlegal(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetSubCommand() == ID_POINT_YLEGAL);
}

void CTonView::OnSpclOmitoutline() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SPCL_OMITOUTLINE);
		CommandInit(ID_TOOL_SPCL);
	}
}

void CTonView::OnBmpLoad() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_BMP_LOAD);
		CommandInit(ID_OBJ_BMP);
	}
}

void CTonView::OnBmpTrns() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_BMP_TRNS);
		CommandInit(ID_OBJ_BMP);
	}
}

void CTonView::PrinterOutput()
{
	
}

void CTonView::PloterOutput()
{

}

void CTonView::RoundTrns(DBL srcx, DBL srcy, DBL x0, DBL y0, DBL rag, DBL *dstx, DBL *dsty)
{
	DBL	tx, ty, tcos, tsin;

	tx = srcx - x0; ty = srcy - y0;
	tsin = sin(rag);
	tcos = cos(rag);
	*dstx = tx * tcos - ty * tsin + x0;
	*dsty = tx * tsin + ty * tcos + y0;
}

void CTonView::RedrawFlagOn()
{
	m_nRedrawFlag = ON;
}

void CTonView::DirectDraw(CDC *pcDC)
{
	CTonDoc* pcDoc = GetDocument();
	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	POSITION pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		CData* pcData = pcObjDataList->GetNext(pos);
		pcData->OnDraw(pcDC);
	}	
}

void CTonView::OnUpdateSubGridOnoff(CCmdUI* pCmdUI) 
{
	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	if(pcSysEnv->ScrnCheckGrid() == ON){
		pCmdUI->SetCheck(1);
	}
	else{
		pCmdUI->SetCheck(0);
	}
}

void CTonView::OnFileAdddel() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_ADDDEL);
		CommandInit(ID_MAIN_FILE);
	}
}


void CTonView::StartTimer()
{
	if(m_nTimerID != 0){
		KillTimer(m_nTimerID);
	}
	m_nTimerID = SetTimer(1, TIMERINTERVAL, NULL);
	m_nTimer = 0;
}

void CTonView::OnTimer(UINT nIDEvent) 
{
	m_nTimer++;
	CScrollView::OnTimer(nIDEvent);
}

int CTonView::EndTimer()
{
	if(m_nTimerID != 0){
		KillTimer(m_nTimerID);
		m_nTimerID = 0;
	}
	return(m_nTimer);
}

void CTonView::ExecSmsout()
{

}


void CTonView::OnCrdntTop() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CRDNT_TOP);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnCrdntVcenter() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CRDNT_VCENTER);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnCrdntBottom() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CRDNT_BOTTOM);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnCrdntLeft() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CRDNT_LEFT);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnCrdntHcenter() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CRDNT_HCENTER);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnCrdntRight() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CRDNT_RIGHT);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnMoveHcenter() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOVE_HCENTER);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnMoveVcenter() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOVE_VCENTER);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnMoveHvcenter() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOVE_HVCENTER);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnSizeHsame() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SIZE_HSAME);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnSizeVsame() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SIZE_VSAME);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnSizeHvsame() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SIZE_HVSAME);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnSpaceHequal() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SPACE_HEQUAL);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnSpaceVequal() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SPACE_VEQUAL);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::CommandSetDef()
{
	CTonApp*	pcApp;
	CMainFrame* pcWnd;

	m_pCommand->SetCrtCommand(ID_MAIN_SLCT);
	m_pCommand->SetSubCommand(ID_SLCT_OBJ);
	m_pCommand->Exec(INITEVENT, 0.0, 0.0);
	pcApp = (CTonApp*)AfxGetApp();
	pcWnd = (CMainFrame*)pcApp->GetMainWnd();
	pcWnd->ShowItemElement();
	pcWnd->SetItemElement();
}

void CTonView::SetString(LPTSTR lpStr)
{
	_tcscpy_s(m_szStockStr, lpStr);
	CommandExec(USER1EVENT, 0, 0);
}

void CTonView::WorkGuideLine(CDC *pcDC)
{
	CPen	penLine;
	CPen	*pcOldPen;
	DBL		mmxsize, mmysize;
	DBL		sx, sy, ex, ey;

	penLine.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	pcOldPen = pcDC->SelectObject(&penLine);
	scrngetmmsize(&mmxsize, &mmysize);

	sx = 0;	ex = mmxsize;
	sy = MOJISIZE; ey = MOJISIZE;
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)ey);

	sx = 0;	ex = mmxsize;
	sy = MOJISIZE*2; ey = MOJISIZE*2;
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)ey);

	sx = MOJISIZE; ex = MOJISIZE;
	sy = 0; ey = mmysize;
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)ey);

	sx = MOJISIZE*2; ex = MOJISIZE*2;
	sy = 0; ey = mmysize;
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)ey);

	pcDC->SelectObject(pcOldPen);
	penLine.DeleteObject();
}

void CTonView::OnSlineCutjoin() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SLINE_CUTJOIN);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnUpdateSlineCutjoin(CCmdUI* pCmdUI) 
{
}
