// sesView.cpp : CTonView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"

#include "io.h"

#include "CntrItem.h"
#include "SrvrItem.h"

#include "..\imgkit\Imagekit.h"

#include "DMmryEnvSet.h"
#include "Mente.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	DOWNUPLIMIT	4
#define	TIMERINTERVAL	100

extern "C" {
	void  Dispatch();
	int baseiskanji(UINT ch);
	int basedivcsvdata(LPSTR csvsp[], LPSTR csvbuf);
	void basechangesafix(LPSTR FileName, LPSTR Safix);
	LPSTR basedivstrg(LPSTR sp, LPSTR retsp, char far sparator);
	void scrnsetwintop(DBL topx, DBL topy);
	void scrnsetwinarea(int sx, int sy, int ex, int ey);
	void scrngetwinarea(int *winsx, int *winsy, int *winex, int *winey);
	void scrninitsizeelement(LPINT retwidth, LPINT rethight);
	void scrnresetzoomtime();
	void scrngetmmsize(LPDBL pdmmxsize, LPDBL pdmmysize);
	void scrnsetmmsize(DBL xsize, DBL ysize);
	void scrngetrltvsize(DBL *pdRXsize, DBL *pdRYsize);
	void scrnsetrltvtop(DBL xTop, DBL yTop);
	void scrngetrltvtop(LPDBL xTop, LPDBL yTop);
	void scrnsetrltvtopreverce(DBL xTop, DBL yTop, DBL xSize, DBL ySize);
	void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	void scrndxymmtorltv(LPDBL x, LPDBL y);
	void scrndxyrltvtomm(LPDBL x, LPDBL y);
	int scrngetcntrmoveflag();
	int scrngetstartmoveflag();
	void scrngetstartmovemm(LPDBL xmm, LPDBL ymm);
	void scrnsetrltvmove(DBL x, DBL y);
	void scrnsetdisprltvmove(DBL xmove, DBL ymove);
	DBL scrnsetxytime(DBL timex, DBL timey);
	void scrngetxytime(LPDBL timex, LPDBL timey);
	void scrndlenmmtorltv(LPDBL len);
	void scrnsetrltvelement();
	void scrnresetzoomtime();
	void scrnenvbasetimezoomup();
	void scrnenvbasetimezoomdown();
	void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	void scrnrltvareaget(DBL *sx, DBL *sy, DBL *ex, DBL *ey);
	int scrnrltvarealegal(LPDBL sx, LPDBL sy, LPDBL ex, LPDBL ey);
	int senvchckstopflag();
	int senvchckflag(int flag);
	void senvonflag(int flag);
	void senvoffflag(int flag);
	void senvgetbkclr(int *rval, int *gval, int *bval);
	void senvgetareaclr(int *rval, int *gval, int *bval);
	void senvgetgridclr(int *rval, int *gval, int *bval);
	void senvgetcntrclr(int *rval, int *gval, int *bval);
	void senvgetscalclr(int *rval, int *gval, int *bval);
	void senvgetasstclr(int *rval, int *gval, int *bval);
	void senvgetslctclr(int *rval, int *gval, int *bval);
	void senvgetfrclr(int *rval, int *gval, int *bval);
	void senvgetwakuclr(int *rval, int *gval, int *bval);
	void senvgetpgspclr(int *rval, int *gval, int *bval);

	int senvsetdrawmode(int mode);
	int	senvcheckdrawmode();
	int	senvcheckdrawgrid();
	void senvondrawgrid();
	void senvoffdrawgrid();
	int	senvcheckdrawimage();
	void senvondrawimage();
	void senvoffdrawimage();
	void senvgetexepath(LPSTR path);
	void senvgettmppath(LPSTR path);
	void senvsettmppath(LPSTR path);
	void senvgetbtppath(LPSTR path);
	void senvgetdatpath(LPSTR path);
	void senvgetareadotsize(int *ndot);
	void poutcntrkindget(int *cntrkind);
	void poutmmsizeget(DBL *xsize, DBL *ysize);
	void wakuenvgetpaperelement();
	void wakuenvgetfullpaperpath(LPSTR paperpath);
	void wakuenvloadoutputenv(LPSTR wakuloadfname);
	void wakuenvsetpapername(LPSTR path);
	void wakuenvgetpapername(LPSTR path);
}

/////////////////////////////////////////////////////////////////////////////
// CTonView

IMPLEMENT_DYNCREATE(CTonView, CScrollView)

BEGIN_MESSAGE_MAP(CTonView, CScrollView)
	//{{AFX_MSG_MAP(CTonView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_OLE_INSERT_NEW, OnInsertObject)
	ON_COMMAND(ID_CANCEL_EDIT_CNTR, OnCancelEditCntr)
	ON_COMMAND(ID_CANCEL_EDIT_SRVR, OnCancelEditSrvr)
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
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINK, OnUpdateEditPasteLink)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_SPECIAL, OnUpdateEditPasteSpecial)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_PASTE_SPECIAL, OnEditPasteSpecial)
	ON_COMMAND(ID_EDIT_PASTE_LINK, OnEditPasteLink)
	ON_COMMAND(ID_FILE_OLE_EXIT, OnFileOleExit)
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
	ON_COMMAND(ID_OBJ_RULE, OnObjRule)
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
	ON_COMMAND(ID_SCAN_SLCT, OnScanSlct)
	ON_COMMAND(ID_SCAN_INPUT, OnScanInput)
	ON_COMMAND(ID_TOOL_TRACE, OnToolTrace)
	ON_COMMAND(ID_CLR_SOLID, OnClrSolid)
	ON_COMMAND(ID_EDIT_MOVE, OnEditMove)
	ON_COMMAND(ID_FIGU_BOXS, OnFiguBoxs)
	ON_COMMAND(ID_FIGU_CRCL, OnFiguCrcl)
	ON_COMMAND(ID_FIGU_FREE, OnFiguFree)
	ON_COMMAND(ID_FIGU_LINE, OnFiguLine)
	ON_COMMAND(ID_FIGU_PLGN, OnFiguPlgn)
	ON_COMMAND(ID_FIGU_POINT, OnFiguPoint)
	ON_COMMAND(ID_FILE_ADDLOAD, OnFileAddload)
	ON_COMMAND(ID_FILE_ADDSAVE, OnFileAddsave)
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
	ON_UPDATE_COMMAND_UI(ID_FIGU_PLGN, OnUpdateFiguPlgn)
	ON_UPDATE_COMMAND_UI(ID_FIGU_POINT, OnUpdateFiguPoint)
	ON_UPDATE_COMMAND_UI(ID_LINE_MOVE, OnUpdateLineMove)
	ON_UPDATE_COMMAND_UI(ID_POINT_MOVE, OnUpdatePointMove)
	ON_UPDATE_COMMAND_UI(ID_SIZE_FREE, OnUpdateSizeFree)
	ON_COMMAND(ID_CLR_LINE, OnClrLine)
	ON_COMMAND(ID_CLR_GRADATION, OnClrGradation)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
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
	ON_COMMAND(ID_SUB_WAKUDEL, OnSubWakudel)
	ON_COMMAND(ID_ATR_CONER_KIND, OnAtrConerKind)
	ON_COMMAND(ID_SLOP_RL, OnSlopRl)
	ON_COMMAND(ID_SLOP_UD, OnSlopUd)
	ON_COMMAND(ID_FILE_CRCTSAVE, OnFileCrctsave)
	ON_COMMAND(ID_FILE_CRCTLOAD, OnFileCrctload)
	ON_COMMAND(ID_FILE_ADDDEL, OnFileAdddel)
	ON_COMMAND(ID_FILE_CRCTDEL, OnFileCrctdel)
	ON_COMMAND(ID_WAKU_INSORT, OnWakuInsort)
	ON_COMMAND(ID_SUB_GRID_ONOFF, OnSubGridOnoff)
	ON_UPDATE_COMMAND_UI(ID_SUB_GRID_ONOFF, OnUpdateSubGridOnoff)
	ON_COMMAND(ID_MOJI_FITING, OnMojiFiting)
	ON_COMMAND(ID_FILE_ADDENVLOAD, OnFileAddenvload)
	ON_COMMAND(ID_WAKU_CHANGE, OnWakuChange)
	ON_COMMAND(ID_BMP_ROUND, OnBmpRound)
	ON_WM_TIMER()
	ON_COMMAND(ID_BMP_SAVE, OnBmpSave)
	ON_COMMAND(ID_CLR_BACKSET, OnClrBackset)
	ON_COMMAND(ID_CLR_BACKRESET, OnClrBackreset)
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
	ON_COMMAND(ID_WAKU_APPLY, OnWakuApply)
	ON_COMMAND(ID_WAKU_PAPERDEL, OnWakuPaperdel)
	ON_COMMAND(ID_BMP_JPGSAVE, OnBmpJpgsave)
	ON_COMMAND(ID_MENTE_CPTR_FILE, OnMenteCptrFile)
	ON_COMMAND(ID_MENTE_CPTR_FOLDER, OnMenteCptrFolder)
	ON_COMMAND(ID_DBEDT_LOAD, OnDbedtLoad)
	ON_COMMAND(ID_DBEDT_DELETE, OnDbedtDelete)
	ON_UPDATE_COMMAND_UI(ID_DBEDT_DELETE, OnUpdateDbedtDelete)
	ON_COMMAND(ID_FILE_EPSSEND, OnFileEpssend)
	ON_COMMAND(ID_FILE_MPRNSEND, OnFileMprnsend)
	ON_COMMAND(ID_FILE_PRNSEND, OnFilePrnsend)
	ON_COMMAND(ID_FILE_MLTPRNFTP, OnFileMltprnftp)
	ON_COMMAND(ID_FILE_EMLTPRNFTP, OnFileEmltprnftp)
	ON_COMMAND(ID_BMP_LOAD, OnBmpLoad)
	ON_COMMAND(ID_BMP_CLIP, OnBmpClip)
	ON_COMMAND(ID_BMP_BKLOAD, OnBmpBkload)
	ON_COMMAND(ID_BMP_BKRESET, OnBmpBkreset)
	ON_COMMAND(ID_MOJI_FONTCHANGE, OnMojiFontchange)
	ON_COMMAND(ID_MEMO_WAKU_LOAD, OnMemoWakuLoad)
	ON_COMMAND(ID_MEMO_CALLPROG, OnMemoCallprog)
	ON_COMMAND(ID_MEMO_ENVSET, OnMemoEnvset)
	ON_COMMAND(ID_MENTE_WAKU_BACKUP, OnMenteWakuBackup)
	ON_COMMAND(ID_WAKU_INSEKIJI, OnWakuInsekiji)
	ON_WM_MOUSEWHEEL()
	ON_UPDATE_COMMAND_UI(ID_WAKU_INPUT, OnUpdateWakuInput)
	ON_UPDATE_COMMAND_UI(ID_WAKU_INSEKIJI, OnUpdateWakuInsekiji)
	//}}AFX_MSG_MAP
	// �W������R�}���h
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

CLIPFORMAT CTonView::m_cfObjectDescriptor = NULL;

/////////////////////////////////////////////////////////////////////////////
// CTonView �N���X�̍\�z/����

CTonView::CTonView()
{
	char	szExePath[FNAMEMAX];
	char	szFileName[FNAMEMAX];

	strcpy(m_szInfoEnv, "es98info.env");
	m_nFirst = TRUE;
	m_fWinExist = FALSE;
	m_nNextCommand = NOOP;
	m_nDrawIn = OFF;
	m_nRedrawFlag = OFF;
	m_nSublineDraw = ON;
	m_fAtntion = OFF;
	m_nBmpCnt = 0;
	m_fStrgInputMode = OFF;

	m_pCommand = new CCommand(this);
	m_cfObjectDescriptor = (CLIPFORMAT)
		::RegisterClipboardFormat(_T("Object Descriptor"));
	m_pSlctOle = NULL;

	m_nStockPoint = 0;
	m_szStockBuffer[m_nStockPoint] = '\0';

	SetFiguKind(ID_FIGU_BOXS);

	m_bDragMode = FALSE;
	m_ptDragPoint = CPoint(-1, -1);
	m_nDragMode = ON;
	//m_nDragMode = OFF;
	senvsetdrawmode(PREVIEW);

	m_nBaseXTop = 0;
	m_nBaseYTop = 0;

	m_hProcess = 0;

	m_nSlctWaku = -1;
	m_bDispImage = FALSE;
	senvgetexepath(szExePath);
	sprintf(szFileName, "%s\\wakuimage.txt", szExePath);
    if((_access(szFileName, 0 )) == 0){
		m_bDispImage = TRUE;
	}
	m_bEpsonYoko = FALSE;
	sprintf(szFileName, "%s\\epsonyoko.txt", szExePath);
    if((_access(szFileName, 0 )) == 0){
		m_bEpsonYoko = TRUE;
	}
}

CTonView::~CTonView()
{
	delete(m_pCommand);
	// CG: ���̍s�� �uIME �T�|�[�g�v�R���|�[�l���g�ɂ��ǉ�����Ă��܂��B
}

BOOL CTonView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTonView �N���X�̕`��

void CTonView::OnDraw(CDC* pcDC)
{
	CTonApp *pcApp;
	CMainFrame*	pcWnd;
	CSysEnv *pcSysEnv;
	char	szFileName[FNAMEMAX];
	CRect	rectClip;
	CRect	rectData;
	DBL		dbrltvxtop;
	DBL		dbrltvytop;
	int		mode;
	int		noidx;

	pcApp = (CTonApp*)::AfxGetApp();
	pcWnd = (CMainFrame*)::AfxGetMainWnd();
	pcSysEnv = pcApp->m_pcSysEnv;
	mode = senvcheckdrawmode();
	if(m_nFirst == TRUE){
		m_nFirst = FALSE;
		if(pcApp->m_szArgv1[0] != '\0'){
			strcpy(szFileName, pcApp->m_szArgv1);
			LoadCsvData(szFileName);
			if(pcApp->m_nNoDispFlag == 1){
				SetSubCommand(ID_BMP_SAVE);
				CommandInit(ID_OBJ_BMP);
				StartTimer();
			}
		}
		pcWnd->SetItemElement();	
	}
	if(m_nDrawIn == ON){
		return;
	}

	m_nDrawIn = ON;

	senvchckstopflag();
	senvoffflag(STOPFLAG);

	scrngetrltvtop(&dbrltvxtop, &dbrltvytop);
TRACE("OnDraw scrngetrltvtop x%1.0lf y%1.0lf\n", dbrltvxtop, dbrltvytop);
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
		scrnsetdisprltvmove(0, 0);
		if(m_fAtntion == ON){
			m_fAtntion = OFF;
			PrintAreaBackClrFill(pcDC);
			ActiveObjectOnDraw(pcDC);
			m_nDrawIn = OFF;
			return;
		}
	}

	if(mode != PRINTVIEW){
		WorkAreaBackClrFill(pcDC);
	}

	if(pcApp->m_clrBack != -1){
		pcDC->FillSolidRect(&rectClip, pcApp->m_clrBack);
	}

	//OleObjectOnDraw(pcDC, pcDoc);

	noidx = 1;
	CTypedPtrList<CObList,CData*> *pcObjDataList = &(pcDoc->m_ObjDataList0);
	POSITION pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		CData* pcData = pcObjDataList->GetNext(pos);
		rectData = pcData->GetBoundingRect();
		rectData.InflateRect(1, 1);
		rectData.NormalizeRect( );
		if (rectData.IntersectRect(&rectData, &rectClip))
		{
			if(mode == PRINTVIEW){
				pcData->OnDraw(pcDC);
			}
			else{
				pcData->OnDraw(pcDC);
			}
		}
		if(senvchckflag(STOPFLAG) == ON){
			break;
		}
	}

	pcObjDataList = pcDoc->m_pcObjDataList;
	pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		CData* pcData = pcObjDataList->GetNext(pos);

		rectData = pcData->GetBoundingRect();
		rectData.InflateRect(1, 1);
		rectData.NormalizeRect( );
		if (rectData.IntersectRect(&rectData, &rectClip))
		{
			if(mode == PRINTVIEW){
				pcData->OnDraw(pcDC);
			}
			else{
				pcData->OnDraw(pcDC);
				if(GetSubCommand() == ID_WAKU_INSEKIJI){
					if(pcData->CheckID() == DATAGRUPID){
						pcData->DrawSekijiNo(pcDC);
					}
				}else{
					if(pcData->CheckID() == SWAKVECTID){
						pcData->DrawNo(pcDC, noidx);
						noidx++;
					}
				}
			}
		}
		if(senvchckflag(STOPFLAG) == ON){
			break;
		}
	}

	if(mode != PRINTVIEW){
		WorkAreaDraw(pcDC);
		if(pcSysEnv->ScrnCheckGrid() == ON){
			pcDC->SetROP2(R2_XORPEN);
			WorkAreaGridDraw(pcDC);
			pcDC->SetROP2(R2_COPYPEN);
		}
		WorkAreaScale(pcDC, dbrltvxtop, dbrltvytop);
		WorkAreaCntrLineDraw(pcDC);
		if(m_nSublineDraw == ON){
			WorkAreaSubLineDraw(pcDC);
		}
		WorkSpaceLineDraw(pcDC);
	}
	WorkTonnboDraw(pcDC);

	if(mode != PRINTVIEW){
		ActiveObjectBoxsDraw(pcDC);
		scrnsetrltvtop(dbrltvxtop, dbrltvytop);
TRACE("OnDraw scrnsetrltvtop x%1.0lf y%1.0lf\n", dbrltvxtop, dbrltvytop);
		if(mode == PREVIEW){
			//ActiveObjectOnDraw(pcDC);
			if(m_nRedrawFlag == ON){
				m_pcDC = pcDC;
				CommandExec(REDRAWEVENT, 0, 0);
				m_pcDC = NULL;
				m_nRedrawFlag = OFF;
			}
			if(m_nCrtWaku != -1){
				if(GetSubCommand() != ID_WAKU_INSEKIJI){
					DrawSelectWaku(pcDC);
				}
			}
		}
		else{
			//NoActiveObjectOnDraw(pcDC);
		}
	}

	scrnsetdisprltvmove(0, 0);
	m_nDrawIn = OFF;
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
	DBL		sx, sy, ex, ey, cx, cy;
	int		x1, y1;
	int		rval, gval, bval;
	int		ndot, ndot1;

	CTonDoc* pcDoc = GetDocument();
	pcDoc->GetDMiniMax(ACTIVEFLAG, &sx, &sy, &ex, &ey);
	if(sx > ex){
		return;
	}
	cx = (sx + ex) / 2.0;
	cy = (sy + ey) / 2.0;
	senvgetwakuclr(&rval, &gval, &bval);
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	scrndxymmtorltv(&cx, &cy);
	senvgetareadotsize(&ndot);
	ndot1 = ndot/2;

	// ����
	x1 = (int)sx - ndot1; y1 = (int)sy - ndot1;
	pcDC->FillSolidRect(x1, y1, ndot, ndot, RGB(rval, gval, bval));

	// ����
	x1 = (int)sx - ndot1; y1 = (int)ey - ndot1;
	pcDC->FillSolidRect(x1, y1, ndot, ndot, RGB(rval, gval, bval));

	// �E��
	x1 = (int)ex - ndot1; y1 = (int)ey - ndot1;
	pcDC->FillSolidRect(x1, y1, ndot, ndot, RGB(rval, gval, bval));

	// �E��
	x1 = (int)ex - ndot1; y1 = (int)sy - ndot1;
	pcDC->FillSolidRect(x1, y1, ndot, ndot, RGB(rval, gval, bval));

	// ������
	x1 = (int)sx - ndot1; y1 = (int)cy - ndot1;
	pcDC->FillSolidRect(x1, y1, ndot, ndot, RGB(rval, gval, bval));

	// �E����
	x1 = (int)ex - ndot1; y1 = (int)cy - ndot1;
	pcDC->FillSolidRect(x1, y1, ndot, ndot, RGB(rval, gval, bval));

	// ������
	x1 = (int)cx - ndot1; y1 = (int)sy - ndot1;
	pcDC->FillSolidRect(x1, y1, ndot, ndot, RGB(rval, gval, bval));

	// �㒆��
	x1 = (int)cx - ndot1; y1 = (int)ey - ndot1;
	pcDC->FillSolidRect(x1, y1, ndot, ndot, RGB(rval, gval, bval));
}

void CTonView::PrintAreaBackClrFill(CDC *pcDC)
{
	DBL		sx, sy, ex, ey;
	CRect	rect;

	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	if(pcApp->m_clrBack == -1){
		return;
	}
	scrnrltvareaget(&sx, &sy, &ex, &ey);
	rect.left = (long)sx;
	rect.right = (long)ex;
	rect.top = (long)sy;
	rect.bottom = (long)ey;
	pcDC->FillSolidRect(&rect, pcApp->m_clrBack);
}

void CTonView::WorkAreaBackClrFill(CDC *pcDC)
{
	int		rval, gval, bval;
	DBL		dxsize, dysize;
	int		winsx, winsy, winex, winey;
	CRect	rect;

	scrngetwinarea(&winsx, &winsy, &winex, &winey);
	GetWindowRect(&rect);
	senvgetbkclr(&rval, &gval, &bval);
	pcDC->SetBkColor(RGB(rval, gval, bval));
	dxsize = rect.right - rect.left;
	dysize = rect.bottom - rect.top;
	//scrngetrltvsize(&dxsize, &dysize);
	rect.top = (long)(winsy-dysize);
	rect.left = (long)(winsx-dxsize);
	rect.bottom = (long)(winsy + dysize*2);
	rect.right = (long)(winsx + dxsize*2);
	CBrush *pcBrush = new CBrush(RGB(rval, gval, bval));
	pcDC->FillRect(&rect, pcBrush);
	delete pcBrush;
}

void CTonView::ActiveObjectOnDraw(CDC *pcDC)
{
	CRect	rectClip;
	CRect	rectData;
	int		chk;

	CTonDoc* pcDoc = GetDocument();
	ASSERT_VALID(pcDoc);
	pcDC->GetClipBox(&rectClip);
	rectClip.InflateRect(1, 1);
	rectClip.NormalizeRect( );

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;

	POSITION pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		CData* pData = pcObjDataList->GetNext(pos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			rectData = pData->GetBoundingRect();
			rectData.NormalizeRect( );
			rectData.InflateRect(1, 1);
			if (rectData.IntersectRect(&rectData, &rectClip))
			{
				pData->StockClrDraw(pcDC);
			}
		}
		chk = ::GetAsyncKeyState(VK_ESCAPE);
		if(chk != 0){
			senvonflag(STOPFLAG);
		}
		if(senvchckflag(STOPFLAG) == ON){
			break;
		}
	}
}

void CTonView::OleObjectOnDraw(CDC *pcDC, CTonDoc *pcDoc)
{
	CTonCntrItem* pOleItem;
	CRect		brect;
	DBL			sx, sy, ex, ey;

  	POSITION pos = pcDoc->GetStartPosition();
	while (pos != NULL)
	{
		pOleItem = (CTonCntrItem*) pcDoc->GetNextClientItem(pos);
		if(senvcheckdrawmode() == PRINTVIEW){
			brect = pOleItem->m_rectPosition;
			sx = (pOleItem->m_rectPosition).left;
			ex = (pOleItem->m_rectPosition).right;
			sy = (pOleItem->m_rectPosition).top;
			ey = (pOleItem->m_rectPosition).bottom;
			sx = sx * m_dBackTime / m_dPrnTimeX;
			ex = ex * m_dBackTime / m_dPrnTimeX;
			sy = sy * m_dBackTime / m_dPrnTimeY;
			ey = ey * m_dBackTime / m_dPrnTimeY;
			//scrndxymmtorltv(&sx, &sy);
			//scrndxymmtorltv(&ex, &ey);
			(pOleItem->m_rectPosition).left = (long)sx;
			(pOleItem->m_rectPosition).right = (long)ex;
			(pOleItem->m_rectPosition).top = (long)sy;
			(pOleItem->m_rectPosition).bottom = (long)ey;
		}
		pOleItem->Draw(pcDC, pOleItem->m_rectPosition);
		if(senvcheckdrawmode() != PRINTVIEW){
			if (pOleItem == m_pSlctOle){
				CRectTracker tracker(m_pSlctOle->m_rectPosition,
					CRectTracker::resizeInside | CRectTracker::solidLine);
 				if (m_pSlctOle->GetItemState() == COleClientItem::openState
				||	m_pSlctOle->GetItemState() == COleClientItem::activeState)
					tracker.m_nStyle |= CRectTracker::hatchInside;
				tracker.Draw(pcDC);
			}
		}
		if(senvcheckdrawmode() == PRINTVIEW){
			pOleItem->m_rectPosition = brect;
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
	rval = 0xff ^ rval;
	gval = 0xff ^ gval;
	bval = 0xff ^ bval;
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
	penStroke.DeleteObject();
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
	char	cntbuf[RECMAX];
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
	if(!chFont.CreateFont(10, 5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, "�l�r ����")){
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
			sprintf(cntbuf, "%d", cnt*add);
			len = strlen(cntbuf);
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
			sprintf(cntbuf, "%d", cnt*add);
			len = strlen(cntbuf);
			pcDC->TextOut((int)sx, (int)sy, cntbuf, len);
		}
		cnt++;
	}

	pcDC->SelectObject(cpholdPen);
	pcDC->SelectObject(cpholdFont);
	chPen.DeleteObject();
	chFont.DeleteObject();
}

void CTonView::WorkSpaceLineDraw(CDC *pcDC)
{
	CPen	*pcOldPen;
	CPen	cPen;
	int		rval, gval, bval;
	DBL		workxmm, workymm;
	DBL		sx, sy, ex, ey;

	CTonDoc* pcDoc = (CTonDoc*)GetDocument();
	senvgetpgspclr(&rval, &gval, &bval);

	cPen.CreatePen(PS_DASHDOTDOT, 1, RGB(rval, gval, bval));
	pcOldPen = pcDC->SelectObject(&cPen);

	scrngetmmsize(&workxmm, &workymm);
	sx = pcDoc->m_dLeftSpace;
	ex = sx;
	sy = 0;
	ey = workymm;
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)ey);

	sx = workxmm - pcDoc->m_dRightSpace;
	ex = sx;
	sy = 0;
	ey = workymm;
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)ey);

	sx = 0;
	ex = workxmm;
	sy = pcDoc->m_dBottomSpace;
	ey = sy;
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)ey);

	sx = 0;
	ex = workxmm;
	sy = workymm - pcDoc->m_dTopSpace;
	ey = sy;
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	if(0 < sy){
		pcDC->MoveTo((int)sx, (int)sy);
		pcDC->LineTo((int)ex, (int)ey);
	}

	pcDC->SelectObject(pcOldPen);
	cPen.DeleteObject();
}

void CTonView::WorkTonnboDraw(CDC *pcDC)
{
	CPen cPen, *pcOldPen;
	DBL		workxmm, workymm;
	DBL		sx, sy, ex, ey, cx, cy;

	CTonDoc* pcDoc = (CTonDoc*)GetDocument();
	if(pcDoc->m_nTonnbo == FALSE){
		return;
	}
	cPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pcOldPen = pcDC->SelectObject(&cPen);
	scrngetmmsize(&workxmm, &workymm);
	sx = pcDoc->m_dLeftSpace;
	sy = workymm - pcDoc->m_dTopSpace;
	ex = workxmm - pcDoc->m_dRightSpace;
	ey = pcDoc->m_dBottomSpace;
	cx = (sx+ex)/2.0;
	cy = (sy+ey)/2.0;
	WorkKadoTonnboDraw(pcDC, sx, sy, -1.0, -1.0);
	WorkKadoTonnboDraw(pcDC, sx, ey, -1.0, +1.0);
	WorkKadoTonnboDraw(pcDC, ex, ey, +1.0, +1.0);
	WorkKadoTonnboDraw(pcDC, ex, sy, +1.0, -1.0);
	WorkCntrTonnboDraw(pcDC, sx, cy, -1.0, 0.0);
	WorkCntrTonnboDraw(pcDC, ex, cy, +1.0, 0.0);
	WorkCntrTonnboDraw(pcDC, cx, sy, 0.0, -1.0);
	WorkCntrTonnboDraw(pcDC, cx, ey, 0.0, +1.0);
}

void CTonView::WorkKadoTonnboDraw(CDC *pcDC, DBL cx, DBL cy, DBL signex, DBL signey)
{
	DBL		dx, dy;

	dx = cx; dy = cy-12.0*signey;
	scrndxymmtorltv(&dx, &dy);
	pcDC->MoveTo((int)dx, (int)dy);
	dx = cx; dy = cy-3.0*signey;
	scrndxymmtorltv(&dx, &dy);
	pcDC->LineTo((int)dx, (int)dy);
	dx = cx+12.0*signex; dy = cy-3.0*signey;
	scrndxymmtorltv(&dx, &dy);
	pcDC->LineTo((int)dx, (int)dy);

	dx = cx+3.0*signex; dy = cy-12.0*signey;
	scrndxymmtorltv(&dx, &dy);
	pcDC->MoveTo((int)dx, (int)dy);
	dx = cx+3.0*signex; dy = cy;
	scrndxymmtorltv(&dx, &dy);
	pcDC->LineTo((int)dx, (int)dy);
	dx = cx+12.0*signex; dy = cy;
	scrndxymmtorltv(&dx, &dy);
	pcDC->LineTo((int)dx, (int)dy);
}

void CTonView::WorkCntrTonnboDraw(CDC *pcDC, DBL cx, DBL cy, DBL signex, DBL signey)
{
	DBL		dx, dy;

	if(signex != 0.0){
		dx = cx+5.0*signex; dy = cy-13.0;
		scrndxymmtorltv(&dx, &dy);
		pcDC->MoveTo((int)dx, (int)dy);
		dx = cx+5.0*signex; dy = cy+13.0;
		scrndxymmtorltv(&dx, &dy);
		pcDC->LineTo((int)dx, (int)dy);

		dx = cx+3.0*signex; dy = cy;
		scrndxymmtorltv(&dx, &dy);
		pcDC->MoveTo((int)dx, (int)dy);
		dx = cx+11.0*signex; dy = cy;
		scrndxymmtorltv(&dx, &dy);
		pcDC->LineTo((int)dx, (int)dy);
	}
	else{
		dx = cx+13.0; dy = cy-5.0*signey;
		scrndxymmtorltv(&dx, &dy);
		pcDC->MoveTo((int)dx, (int)dy);
		dx = cx-13.0; dy = cy-5.0*signey;
		scrndxymmtorltv(&dx, &dy);
		pcDC->LineTo((int)dx, (int)dy);

		dx = cx; dy = cy-3.0*signey;
		scrndxymmtorltv(&dx, &dy);
		pcDC->MoveTo((int)dx, (int)dy);
		dx = cx; dy = cy-11.0*signey;
		scrndxymmtorltv(&dx, &dy);
		pcDC->LineTo((int)dx, (int)dy);
	}
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
	m_pSlctOle = NULL;

	CScrollView::OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CTonView �N���X�̈��
BOOL CTonView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �f�t�H���g�̈������
	return DoPreparePrinting(pInfo);
	//return TRUE;
}

void CTonView::OnBeginPrinting(CDC* pcDC, CPrintInfo* pcInfo)
{
	// TODO: ����O�̓��ʂȏ�����������ǉ����Ă��������B
	DBL		xsize, ysize;
	DBL		xmm, ymm;
	DBL		xadd, yadd;
	DBL		dPrnStartSX, dPrnStartSY;
	DBL		dPHYPrnMMXSize, dPHYPrnMMYSize;
	int		cntrkind;
	
	LPDEVMODE lpDevMode = (LPDEVMODE)::GlobalLock(pcInfo->m_pPD->m_pd.hDevMode);
	m_nOrientation = lpDevMode->dmOrientation;
	::GlobalUnlock(pcInfo->m_pPD->m_pd.hDevMode);
	// TODO: ����O�̓��ʂȏ�����������ǉ����Ă��������B
	m_nBackDrawMode = senvsetdrawmode(PRINTVIEW);
	xsize = pcDC->GetDeviceCaps(PHYSICALWIDTH);
	ysize = pcDC->GetDeviceCaps(PHYSICALHEIGHT);
	m_dDpiX = pcDC->GetDeviceCaps(LOGPIXELSX);
	m_dDpiY = pcDC->GetDeviceCaps(LOGPIXELSY);
	dPHYPrnMMXSize = (int)(xsize / m_dDpiX * 25.4 + 0.5);
	dPHYPrnMMYSize = (int)(ysize / m_dDpiY * 25.4 + 0.5);

	m_dPrnMMXSize = pcDC->GetDeviceCaps(HORZSIZE);
	m_dPrnMMYSize = pcDC->GetDeviceCaps(VERTSIZE);
	dPrnStartSX = pcDC->GetDeviceCaps(PHYSICALOFFSETX);
	dPrnStartSY = pcDC->GetDeviceCaps(PHYSICALOFFSETY);
	scrngetmmsize(&m_dMMXSize, &m_dMMYSize);
	m_dPrnTimeX = (25.4 / m_dDpiX);
	m_dPrnTimeY = (25.4 / m_dDpiY);
	m_dBackTime = scrnsetxytime(m_dPrnTimeX, m_dPrnTimeY);
	scrnsetrltvelement();

	m_nXDivPage = 1;
	if(m_dMMXSize > dPHYPrnMMXSize){
		m_nXDivPage = (long)m_dMMXSize / (long)m_dPrnMMXSize;
		if((long)m_dMMXSize % (long)m_dPrnMMXSize){
			m_nXDivPage++;
		}
	}

	m_nYDivPage = 1;
	if(m_dMMYSize > dPHYPrnMMYSize){
		m_nYDivPage = (long)m_dMMYSize / (long)m_dPrnMMYSize;
		if((long)m_dMMYSize % (long)m_dPrnMMYSize){
			m_nYDivPage++;
		}
	}
	m_nDivPageLast = m_nXDivPage*m_nYDivPage;

	DivPageCountSet(m_dMMXSize, m_dMMYSize, m_dPrnMMXSize, m_dPrnMMYSize);
	poutcntrkindget(&cntrkind);
	switch(cntrkind){
	case RIGHTUP:
	case LEFTUP:
		scrnsetrltvtop(0.0, 0.0);
		break;
	case LEFTDOWN:
	case RIGHTDOWN:
		xmm = m_dPrnMMXSize;
		ymm = m_dPrnMMYSize;
		//xmm = m_dMMXSize;
		//ymm = m_dMMYSize;
		scrnsetrltvtop(0.0, 0.0);
		scrndlenmmtorltv(&xmm);
		scrndlenmmtorltv(&ymm);
		scrnsetrltvtopreverce(0.0, 0.0, xmm, ymm);
		break;
	}

	xmm = 0; ymm = 0;
	if(scrngetcntrmoveflag() == ON){
		switch(cntrkind){
		case LEFTUP:
			xmm = (m_dPrnMMXSize - m_dMMXSize)/2.0;
			break;
		case RIGHTDOWN:
			xmm = -(m_dPrnMMXSize - m_dMMXSize)/2.0;
			break;
		case RIGHTUP:
			ymm = (m_dPrnMMYSize - m_dMMYSize)/2.0;
			break;
		case LEFTDOWN:
			ymm = -(m_dPrnMMYSize - m_dMMYSize)/2.0;
			break;
		}
	}
	if(scrngetstartmoveflag() == ON){
		scrngetstartmovemm(&xadd, &yadd);
		switch(cntrkind){
		case LEFTUP:
		case RIGHTDOWN:
			xmm = xmm + yadd;
			ymm = ymm - xadd;
			break;
		case RIGHTUP:
		case LEFTDOWN:
			xmm = xmm + xadd;
			ymm = ymm + yadd;
			break;
		}
	}
	scrndlenmmtorltv(&xmm);
	scrndlenmmtorltv(&ymm);
	switch(cntrkind){
	case LEFTUP:	// ����
		if(scrngetcntrmoveflag() == ON){
			scrnsetrltvmove(xmm, ymm-dPrnStartSY);
		}
		else{
			scrnsetrltvmove(xmm-dPrnStartSX, ymm-dPrnStartSY);
		}
		break;
	case RIGHTUP:
		if(scrngetcntrmoveflag() == ON){
			scrnsetrltvmove(xmm-dPrnStartSX, ymm);
		}
		else{
			scrnsetrltvmove(xmm-dPrnStartSX, ymm-dPrnStartSY);
		}
		break;
	case RIGHTDOWN:
		if(scrngetcntrmoveflag() == ON){
			scrnsetrltvmove(xmm, ymm+dPrnStartSY);
		}
		else{
			scrnsetrltvmove(xmm+dPrnStartSX, ymm+dPrnStartSY);
		}
		break;
	case LEFTDOWN:
		if(scrngetcntrmoveflag() == ON){
			scrnsetrltvmove(xmm+dPrnStartSX, ymm);
		}
		else{
			scrnsetrltvmove(xmm+dPrnStartSX, ymm+dPrnStartSY);
		}
		break;
	}
	pcInfo->SetMinPage(1);
	pcInfo->SetMaxPage(m_nDivPageLast);
	if(m_nDivPageLast != 1){
		scrnsetrltvmove(-dPrnStartSX, -dPrnStartSY);
	}
}

void CTonView::OnEndPrinting(CDC* /*pcDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �����̌㏈����ǉ����Ă��������B
	senvsetdrawmode(m_nBackDrawMode);
	scrnsetrltvmove(0, 0);
	scrnsetxytime(m_dBackTime, m_dBackTime);
	scrnsetrltvelement();
	scrnsetrltvtop(0.0, 0.0);
	scrnrltvareaset(0, 0, INTMAX, INTMAX);
	Invalidate();
}

void CTonView::OnPrepareDC(CDC* pcDC, CPrintInfo* pInfo) 
{
	DBL		sx, sy, ex, ey;
	int		mode;

	CScrollView::OnPrepareDC(pcDC, pInfo);
	mode = senvcheckdrawmode();
	if((mode == PRINTVIEW
	|| mode == DIRCTVIEW)
	&& pInfo != NULL){
		if(m_nDivPageLast != 1){
			if(m_nOrientation == DMORIENT_PORTRAIT){
				m_nDivCrtY = ((pInfo->m_nCurPage-1)) % m_nYDivPage;
				m_nDivCrtX = ((pInfo->m_nCurPage-1)) / m_nYDivPage;
			}
			else{
				m_nDivCrtX = ((pInfo->m_nCurPage-1)) % m_nXDivPage;
				m_nDivCrtY = ((pInfo->m_nCurPage-1)) / m_nXDivPage;
			}
			sx = m_nDivCrtX * m_dPrnMMXSize / m_dPrnTimeX;
			sy = m_nDivCrtY * m_dPrnMMYSize / m_dPrnTimeY;
			ex = m_dPrnMMXSize / m_dPrnTimeX;
			ey = m_dPrnMMYSize / m_dPrnTimeX;
			scrnsetrltvtop(sx, sy);
			scrnrltvareaset(-1000, -1000, ex+1000, ey+1000);
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
	if(lwx < (prnx + 20)){

	}
}

void CTonView::OnDestroy()
{
	// ���Ŏ��ɃA�C�e�����A�N�e�B�x�[�g���܂�; ����͕����r���[��
	// �g�p����Ă���ꍇ�ɂ͏d�v�ł��B
   CScrollView::OnDestroy();
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
}


/////////////////////////////////////////////////////////////////////////////
// OLE �N���C�A���g�̃T�|�[�g�ƃR�}���h

BOOL CTonView::IsSelected(const CObject* pcDocItem) const
{
	// CTonCntrItem �I�u�W�F�N�g�����ō\������Ă��鎞�ɂ�
	//  �ȉ��Ɏ������@�ŏ\���ł��B�قȂ�I��������n���h������Ƃ��ɂ�
	// ���̏ꏊ�����������Ă��������B

	// TODO: ���̊֐��͑I�����ꂽ OLE �N���C�A���g�̃A�C�e���ɑ΂���e�X�g���������܂��B
	return pcDocItem == m_pSlctOle;
}

void CTonView::OnInsertObject()
{
	// �V�K�� CTonCntrItem �I�u�W�F�N�g�̏��𓾂邽�߂�
	//  �W���I�ȃI�u�W�F�N�g�}���_�C�A���O �{�b�N�X���Ăяo���܂��B
	COleInsertDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();

	CTonCntrItem* pItem = NULL;
	TRY
	{
		// ���̃h�L�������g�ɐڑ����ꂽ�V�K�̃A�C�e�����쐬���܂��B
		CTonDoc* pcDoc = GetDocument();
		ASSERT_VALID(pcDoc);
		pItem = new CTonCntrItem(pcDoc);
		ASSERT_VALID(pItem);

		// �_�C�A���O �f�[�^����A�C�e�������������܂��B
		if (!dlg.CreateItem(pItem))
			AfxThrowMemoryException();  // ��O�������s���܂��B
		ASSERT_VALID(pItem);

		// �t�@�C������ł͂Ȃ��N���X ���X�g����쐬���ꂽ�A�C�e���ł��鎞
		//  �A�C�e����ҏW���邽�߂ɃT�[�o�[���N�����܂��B
		if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
			pItem->DoVerb(OLEIVERB_SHOW, this);

		ASSERT_VALID(pItem);

		// �b��I�ȃ��[�U�[ �C���^�[�t�F�C�X�̃f�U�C���Ƃ��čŌ�ɑ}�����ꂽ
		// �A�C�e����I�����Ă��܂��B

		// TODO: �A�v���P�[�V�����ɓK���ȑI������ɂ��킹�ď��������Ă��������B

		m_pSlctOle = pItem;   // �Ō�ɑ}�����ꂽ�A�C�e����I��
		pcDoc->UpdateAllViews(NULL);
	}
	CATCH(CException, e)
	{
		if (pItem != NULL)
		{
			ASSERT_VALID(pItem);
			pItem->Delete();
		}
		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH

	EndWaitCursor();
}

// �ȉ��Ɏ����R�}���h �n���h���͑g�ݍ��ݐ�ҏW�𒆎~���邽�߂̕W���I��
// �L�[�{�[�h ���[�U�[ �C���^�[�t�F�C�X��p�ӂ��Ă��܂��B�����ŃR���e�i
// �i�T�[�o�[�ł͂Ȃ��j�́A�f�B�A�N�e�B�x�[�V�����������N�����܂��B
void CTonView::OnCancelEditCntr()
{
	// ���̃r���[��ɂ���g�ݍ��ݐ�ŗL���ȃA�C�e������܂��B
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
	{
		pActiveItem->Close();
	}
	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
}

// �I�u�W�F�N�g���g�ݍ��ݐ�ŕҏW����͂��߂��Ƃ��ɂ� OnSetFocus �� OnSize �̓��ʂ�
// �n���h�����O�ɂ̓R���e�i���K�v�ł��B
void CTonView::OnSetFocus(CWnd* pcOldWnd)
{
	POINT MousePt;
	CMainFrame*	pcWnd;

	pcWnd = (CMainFrame*)::AfxGetMainWnd();
	CTonDoc *pcDoc = GetDocument();
	COleClientItem* pcActiveItem = pcDoc->GetInPlaceActiveItem(this);
	if (pcActiveItem != NULL &&
		pcActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// �����r���[�ɂ���A�C�e���ɂ̓t�H�[�J�X��ݒ肷��K�v������܂��B
		if (pcWnd != NULL)
		{
			pcWnd->SetFocus();   // ��{�N���X���Ăяo���Ă͂����܂���B
			return;
		}
	}
	pcDoc->SetCrtView(this);
	pcWnd->SetCrtView(this);
	::GetCursorPos(&MousePt);
	CommandExec(SETFOCUSEVENT, (long)(MousePt.x), (long)(MousePt.y));

	CScrollView::OnSetFocus(pcOldWnd);
}

void CTonView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
		pActiveItem->SetItemRects();
	WorkSizeReset();
}

/////////////////////////////////////////////////////////////////////////////
// OLE �T�[�o�[ �T�|�[�g

// �ȉ��̃R�}���h�n���h���͖��߂��ݕҏW�Z�b�V�������L�����Z������X�^���_�[�h �L�[
// �{�[�h ���[�U�[ �C���^�t�F�[�X��p�ӂ��܂��B
// �����ŁA�T�[�o�[ (�R���e�i�ł͂Ȃ�) �́A�f�B�A�N�e�B�x�[�V�����������N�����܂��B
void CTonView::OnCancelEditSrvr()
{
	GetDocument()->OnDeactivateUI(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CTonView �N���X�̐f�f

#ifdef _DEBUG
void CTonView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTonView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTonDoc* CTonView::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTonDoc)));
	return (CTonDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTonView �N���X�̃��b�Z�[�W �n���h��
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

BOOL CTonView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	CTonDoc* pcDoc = GetDocument();
	ASSERT_VALID(pcDoc);
	
	OnDragLeave();

	if (!GetSizeInfo(pDataObject, m_szObjectSize, m_szPreviousOffset))
		return FALSE;

	CClientDC dc(NULL);
	dc.HIMETRICtoDP(&m_szObjectSize);
	dc.HIMETRICtoDP(&m_szPreviousOffset);
	point -= m_szPreviousOffset;

	if ((dropEffect & DROPEFFECT_MOVE) && m_bDragMode && m_pSlctOle != NULL)
	{
		m_bDragMode = FALSE;
		if (m_pSlctOle->m_rectPosition.TopLeft() != point)
		{
			m_pSlctOle->m_rectPosition =
				CRect(point, m_pSlctOle->m_rectPosition.Size());
			pcDoc->SetModifiedFlag();
			pcDoc->UpdateAllViews(NULL);
		}
		m_ptDragPoint = CPoint(-1, -1);
		return TRUE;
	}
	else
	{
		BeginWaitCursor();
		CTonCntrItem* pItem = NULL;

		try
		{
			pItem = new CTonCntrItem(pcDoc);

			if (!pItem->CreateFromData(pDataObject))
				AfxThrowMemoryException();

			if (!GetSizeInfo(pDataObject, m_szObjectSize, m_szPreviousOffset))
				AfxThrowMemoryException();

			CClientDC dc(NULL);

			dc.HIMETRICtoDP(&m_szObjectSize);
			pItem->m_rectPosition = CRect(point, m_szObjectSize);
			m_pSlctOle = pItem;
			pcDoc->SetModifiedFlag();
			pcDoc->UpdateAllViews(NULL);

			EndWaitCursor();
			return TRUE;
		}

		catch(CException* pEx)
		{
			if (pItem != NULL)
				pItem->Delete();
			pEx->Delete();
			EndWaitCursor();
			MessageBox(_T("�n�k�d�I�u�W�F�N�g��}�����邱�Ƃ��ł��܂���ł���!"));
		}
	}

	EndWaitCursor();
	return FALSE;
}

int CTonView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (!m_dropTarget.Register(this))
		MessageBox(_T("Could not register as drag/drop target"));

	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CTonView::OleObjectOpen(UINT nFlags, CPoint point) 
{
	if (m_pSlctOle != NULL)
	{
		if (nFlags & MK_CONTROL)
			m_pSlctOle->DoVerb(OLEIVERB_OPEN, this);
		else
			m_pSlctOle->DoVerb(OLEIVERB_PRIMARY, this);
		return(TRUE);
	}
	else{
		return(FALSE);
	}
}

BOOL CTonView::OleObjectSelected(UINT nFlags, CPoint point)
{ 
	POSITION		pos;
	CTonCntrItem*	pItem;
	CTonDoc*		pcDoc = GetDocument();
	BOOL			bHit = FALSE;

	if (m_pSlctOle != NULL)
	{
		// �I������Ă����n�k�d�I�u�W�F�N�g���N���[�Y����
		m_pSlctOle->Close(OLECLOSE_SAVEIFDIRTY);
		m_pSlctOle = NULL;
		Invalidate();
	}

	// �n�k�d�I�u�W�F�N�g��I��
	pos = pcDoc->GetStartPosition();
	while (pos != NULL)
	{
		pItem = (CTonCntrItem*) pcDoc->GetNextItem(pos);
		if (pItem->m_rectPosition.PtInRect(point))
		{
			bHit = TRUE;
			m_pSlctOle = pItem;
			break;
		}
	}


	if(m_pSlctOle == NULL)
	{
		return(FALSE);
	}
	else{
		Invalidate();
		CRectTracker tracker(m_pSlctOle->m_rectPosition,
				CRectTracker::resizeInside | CRectTracker::solidLine);

		if (tracker.HitTest(point) == CRectTracker::hitMiddle)
		{
			CRect	rectScreenObject = m_pSlctOle->m_rectPosition;
			CPoint 	ptClickOffset(point.x - rectScreenObject.left,
				point.y - rectScreenObject.top);

			CRect rectAwake = rectScreenObject;
			rectAwake.InflateRect(1, 1);

			ClientToScreen(&rectScreenObject);
			ClientToScreen(&rectAwake);

			m_bDragMode = TRUE;
			DROPEFFECT dropResult = m_pSlctOle->DoDragDrop(rectScreenObject,
				ptClickOffset, TRUE, DROPEFFECT_COPY | DROPEFFECT_MOVE,
				&rectAwake);
			if (m_bDragMode == FALSE)
				return(TRUE);

			if (dropResult == DROPEFFECT_MOVE)
			{
				pItem->Delete();
				m_pSlctOle = NULL;
				pcDoc->SetModifiedFlag();
				pcDoc->UpdateAllViews(NULL);
			}
		}
		else if (tracker.Track(this, point))
		{
			m_pSlctOle->m_rectPosition = tracker.m_rect;
			pcDoc->SetModifiedFlag();
 			Invalidate();
		}
		return(TRUE);
	}
}

BOOL CTonView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_pSlctOle != NULL)
	{
		CRectTracker track;
		track.m_rect = m_pSlctOle->m_rectPosition;
		if (track.SetCursor(this, nHitTest))
			return TRUE;
	}
	switch(m_nCursorKind){
	case CUSKIND_APPSTARTING: //�W�����J�[�\������я��^�����v�J�[�\��
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_APPSTARTING));
		break;
	case CUSKIND_ARROW: //�W�����J�[�\��
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		break;
	case CUSKIND_CROSS: //�\���J�[�\�� 
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		break;
	case CUSKIND_IBEAM: //�A�C�r�[�� (�c��) �J�[�\�� 
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
		break;
	case CUSKIND_ICON: //���݂͎g���܂��� 
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ICON));
		break;
	case CUSKIND_NO: //�֎~�J�[�\�� 
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_NO));
		break;
	case CUSKIND_SIZE: //���݂͎g���܂��� 
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZE));
		break;
	case CUSKIND_SIZEALL: //4 �������J�[�\�� 
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
		break;
	case CUSKIND_SIZENESW: //�΂ߍ�������̗��������J�[�\�� 
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
		break;
	case CUSKIND_SIZENS: //�㉺���������J�[�\�� 
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		break;
	case CUSKIND_SIZENWSE: //�΂߉E������̗��������J�[�\�� 
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
		break;
	case CUSKIND_SIZEWE: //���E���������J�[�\�� 
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		break;
	case CUSKIND_UPARROW: //�����̖��J�[�\�� 
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_UPARROW));
		break;
	case CUSKIND_WAIT: //�����v�J�[�\�� 
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		break;
	case CUSKIND_HAND:
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CUS_HAND));
		break;
	case CUSKIND_ZOOMIN:
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CUS_ZOOMIN));
		break;
	case CUSKIND_ZOOMOUT:
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CUS_ZOOMOUT));
		break;
	case CUSKIND_ARROWPLUS:
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CUS_ARROWPLUS));
		break;
	default:
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		break;
	}
	return(TRUE);
}

// �\��t��
void CTonView::OnEditPaste() 
{
	CTonCntrItem*	pItem = NULL;
	CTonDoc*	pcDoc = GetDocument();
	ASSERT_VALID(pcDoc);

	if(pcDoc->CheckArchive() == TRUE){
		pcDoc->OffFlag(ACTIVEFLAG);
		pcDoc->LoadArchive();
		pcDoc->SetModifiedFlag();
		Invalidate();
	}
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SLCT_OBJ);
		CommandInit(ID_MAIN_SLCT);
	}

	return;	// �I������̂�������邽��

	if(!COleClientItem::CanPaste()){
		return;
	}

	COleDataObject	Obj;
	Obj.AttachClipboard();
	if (Obj.IsDataAvailable(CF_TEXT))
	{
		STGMEDIUM myStg;

		if (!Obj.GetData(CF_TEXT, &myStg))
			MessageBox(_T("Failed!"));
		else if (myStg.tymed != TYMED_HGLOBAL)
			MessageBox(_T("Unrecognized format"));
		else
		{
			LPTSTR lpstr = (LPTSTR) ::GlobalLock(myStg.hGlobal);
			MessageBox(lpstr);
			::ReleaseStgMedium(&myStg);
		} 
	}
	Obj.Detach();

	BeginWaitCursor();
	try
	{
		pItem = new CTonCntrItem(pcDoc);
		if (!pItem->CreateFromClipboard())
			AfxThrowMemoryException();	// any would do!

		pItem->UpdateLink();
		pItem->RefreshSize();

		m_pSlctOle = pItem;	// initially selected
	}
	catch (CException* pEx)
	{
		if (pItem != NULL)
		{
			m_pSlctOle = NULL;
			pItem->Delete();
			delete pItem;
		}
		pEx->Delete();
		MessageBox(_T("Could not perform paste!"));
	}
}

// �`����I�����ē\��t��
void CTonView::OnEditPasteSpecial() 
{
	COlePasteSpecialDialog dlg;

	dlg.AddStandardFormats();
	if (dlg.DoModal() != IDOK)
		return;

	CTonCntrItem* pItem = NULL;
	TRY
	{
		CTonDoc* pcDoc = GetDocument();

		pItem = new CTonCntrItem(pcDoc);
		if (!dlg.CreateItem(pItem))
		{
			TRACE0("���t���Ɏ��s���܂����B\n");
			AfxThrowMemoryException();
		}

		pItem->UpdateLink();
		pItem->RefreshSize();

		m_pSlctOle = pItem;
		pcDoc->SetModifiedFlag();
		pcDoc->UpdateAllViews(NULL, 0, pItem);
	}
	CATCH_ALL(e)
	{
		AfxMessageBox(IDP_FAILED_TO_CREATE);
		return;
	}
	END_CATCH_ALL
}

void CTonView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	CTonDoc* pcDoc = GetDocument();
	if (m_pSlctOle == NULL && pcDoc->DataActiveCheck() == FALSE)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CTonView::OnEditCopy() 
{
	if (m_pSlctOle != NULL){
		ASSERT(m_pSlctOle != NULL);
		m_pSlctOle->CopyToClipboard();
	}
	CTonDoc* pcDoc = GetDocument();
	if(pcDoc->DataActiveCheck() == TRUE){
		pcDoc->SaveArchive();
	}
}

void CTonView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	CTonDoc* pcDoc = GetDocument();
	if (m_pSlctOle == NULL && pcDoc->DataActiveCheck() == FALSE)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CTonView::OnEditCut()
{
	if (m_pSlctOle != NULL)
	{
		ASSERT(m_pSlctOle != NULL);
		m_pSlctOle->CopyToClipboard();
		m_pSlctOle->Delete();
		m_pSlctOle = NULL;
		CTonDoc* pcDoc = GetDocument();
		pcDoc->SetModifiedFlag();
		Invalidate();
	}
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
	if (COleClientItem::CanPaste() || pcDoc->CheckArchive() == TRUE)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CTonView::OnEditPasteLink() 
{
	BeginWaitCursor();

	CTonDoc* pcDoc = GetDocument();
	CTonCntrItem* pItem = new CTonCntrItem(pcDoc);
											 
	if (!pItem->CreateLinkFromClipboard())
	{
		pItem->Delete();
		EndWaitCursor();
		MessageBox(_T("Could not create link from clipboard!"));
	}
	else
	{
		m_pSlctOle = pItem;
		pItem->RefreshSize();
		pcDoc->UpdateAllViews(NULL);
		EndWaitCursor();
	}
}

void CTonView::OnUpdateEditPasteLink(CCmdUI* pCmdUI) 
{
	if (COleClientItem::CanPasteLink())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CTonView::OnUpdateEditPasteSpecial(CCmdUI* pCmdUI) 
{
	if (COleClientItem::CanPasteLink() || COleClientItem::CanPaste())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CTonView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	BOOL	ret;

	m_nKeyFlags = nFlags;
	ret = OleObjectOpen(nFlags, point);
	if(ret == FALSE){
		m_nBx = point.x;
		m_nBy = point.y;
		CommandExec(DBLSETEVENT, point.x, point.y);
	}
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CTonView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	BOOL	ret;

	m_nKeyFlags = nFlags;
	ret = OleObjectSelected(nFlags, point);
	if(ret == FALSE){
		m_nSetBx = point.x;
		m_nSetBy = point.y;
		m_nBx = point.x;
		m_nBy = point.y;
		CommandExec(SETEVENT, point.x, point.y);
		m_nDragFlag = ON;
		CScrollView::OnLButtonDown(nFlags, point);
	}
}

void CTonView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	m_nKeyFlags = nFlags;
	if(m_nDragMode == ON && m_nDragFlag == ON){
		m_nDragFlag = OFF;
		m_nBx = point.x;
		m_nBy = point.y;
		if((point.x - DOWNUPLIMIT) < m_nSetBx &&  m_nSetBx < (point.x + DOWNUPLIMIT)
		&& (point.y - DOWNUPLIMIT) < m_nSetBy &&  m_nSetBy < (point.y + DOWNUPLIMIT)){
			point.x = m_nSetBx;
			point.y = m_nSetBy;
			CommandExec(SETEVENT2, point.x, point.y);
		}
		else{
			CommandExec(SETEVENT, point.x, point.y);
		}
	}
	CScrollView::OnLButtonUp(nFlags, point);
}

void CTonView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	BOOL	ret;
	int		nx, ny;
	CRect	rect;

	m_nKeyFlags = nFlags;
	ret = OleObjectOpen(nFlags, point);
	if(ret == FALSE){
		m_nBx = point.x;
		m_nBy = point.y;
		if(GetSubCommand() == ID_SLCT_OBJ){
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
	}
	CScrollView::OnRButtonDown(nFlags, point);
}

void CTonView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
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
		if(nFlags & MK_LBUTTON){
			CommandExec(MOVEEVENT, point.x, point.y);
		}
		else{
			CommandExec(UPMOVEEVENT, point.x, point.y);
		}
		m_nBx = point.x;
		m_nBy = point.y;
	}
	CScrollView::OnMouseMove(nFlags, point);
}

void CTonView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CTonApp *theApp;
	CSysEnv *pcSysEnv;

	// ���{��ϊ��O�̃R�[�h���擾�ł���
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

	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

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
		if(GetSubCommand() == ID_FILE_PLOTER){
			if(AfxMessageBox("��ۯ�����~���Ă�낵���ł���", MB_OKCANCEL, -1) == IDOK){
				senvonflag(STOPFLAG);
			}
		}
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

void CTonView::SetNextCommand(int cmd)
{
	m_nNextCommand = cmd;
}

UINT CTonView::CommandExec(UINT event, long x, long y)
{
	UINT	ret;
	DBL		dx, dy;

	if(m_fWinExist == FALSE){
		return(0);
	}
	
	dx = x;
	dy = y;
	ret = m_pCommand->Exec(event, dx, dy);

	if(ret == ENDEVENT || event == ENDEVENT){
		if(m_pCommand->GetSubCommand() != ID_SLCT_OBJ){
			m_pCommand->SetCrtCommand(ID_MAIN_SLCT);
			m_pCommand->SetSubCommand(ID_SLCT_OBJ);
			m_pCommand->Exec(INITEVENT, 0.0, 0.0);
		}
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

//�����܂ł͊e�V�X�e������

void CTonView::OnFileOleExit() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(m_pSlctOle != NULL){
		CTonCntrItem* pItem = (CTonCntrItem*) m_pSlctOle;
		pItem->Close(OLECLOSE_SAVEIFDIRTY);
		m_pSlctOle = NULL;
	}
}

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

// stan �֐����s��
void CTonView::OnFileEnvset() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		CommandInit(ID_FILE_ENVSET);
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

void CTonView::OnObjRule() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_OBJ_RULE);
		CommandInit(ID_OBJ_RULE);
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

void CTonView::OnSpclParallel() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SPCL_PARALLEL);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnSpclPrepend() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SPCL_PREPEND);
		CommandInit(ID_TOOL_LINE);
	}
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
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	//SendMessage(WM_WINDOWPOSCHANGED, 0, (long)m_hWnd);
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_TOOL_EXPORT);
		CommandInit(ID_MAIN_TOOL);
	}
}

void CTonView::OnToolInport() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_TOOL_INPORT);
		CommandInit(ID_MAIN_TOOL);
	}
}

void CTonView::OnSlctArea() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SLCT_AREA);
		CommandInit(ID_MAIN_SLCT);
	}
}

void CTonView::OnSlctObj() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SLCT_OBJ);
		CommandInit(ID_MAIN_SLCT);
	}
}

void CTonView::OnZoomAll() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
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
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_TRNS_SIZE);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnViewPreview() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(senvcheckdrawmode() == PREVIEW){
		senvsetdrawmode(POINTVIEW);
	}
	else{
		senvsetdrawmode(PREVIEW);
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
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	if(senvcheckdrawmode() == PREVIEW){
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
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(GetSubCommand() == ID_ZOOM_ALL);
}

void CTonView::OnUpdateZoomIn(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(GetSubCommand() == ID_ZOOM_IN);
}

void CTonView::OnUpdateFiguBoxs(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(GetSubCommand() == ID_FIGU_BOXS);
}

void CTonView::OnUpdateFiguCrcl(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(GetSubCommand() == ID_FIGU_CRCL);
}

void CTonView::OnUpdateFiguFree(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(GetSubCommand() == ID_FIGU_FREE);
}

void CTonView::OnUpdateFiguLine(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(GetSubCommand() == ID_FIGU_LINE);
}

void CTonView::OnUpdateFiguPlgn(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(GetSubCommand() == ID_FIGU_PLGN);
}

void CTonView::OnUpdateFiguPoint(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(GetSubCommand() == ID_FIGU_POINT);
}

void CTonView::OnUpdateLineMove(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(GetSubCommand() == ID_LINE_MOVE);
}

void CTonView::OnUpdatePointMove(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(GetSubCommand() == ID_POINT_MOVE);
}

void CTonView::OnUpdateSizeFree(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
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
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
	point = GetScrollPosition();
	dx = point.x;
	dy = point.y;
	scrnsetrltvtop(dx, dy);
	TRACE("OnHScroll scrnsetrltvtop x%d y%d\n", point.x, point.y);
	Invalidate();
}

void CTonView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CPoint	point;
	DBL	dx, dy;
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
	point = GetScrollPosition();
	dx = point.x;
	dy = point.y;
	TRACE("OnVScroll scrnsetrltvtop x%d y%d\n", point.x, point.y);
	scrnsetrltvtop(dx, dy);
	Invalidate();
}
BOOL CTonView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	int		ret;
	CPoint	point;
	DBL		dx, dy;

	ret = CScrollView::OnMouseWheel(nFlags, zDelta, pt);
	point = GetScrollPosition();
	dx = point.x;
	dy = point.y;
	scrnsetrltvtop(dx, dy);
	return(ret);
}

BOOL CTonView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	dwStyle |= WS_MAXIMIZE;
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CTonView::OnToolTrace() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_TOOL_TRACE);
		CommandInit(ID_TOOL_TRACE);
	}
}

void CTonView::OnSlctAll() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SLCT_ALL);
		CommandInit(ID_MAIN_SLCT);
	}
}

void CTonView::OnSlctChng() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SLCT_CHNG);
		CommandInit(ID_MAIN_SLCT);
	}
}

void CTonView::OnEditDelete() 
{
	if(m_fStrgInputMode == ON){
		return;
	}
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_EDIT_DELETE);
		CommandInit(ID_MAIN_EDIT);
	}
}

void CTonView::OnEditRedo() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CTonDoc* pcDoc = GetDocument();
	if(CommandExit() == (UINT)ENDEVENT){
		pcDoc->LoadRedo();
		Invalidate();
	}	
}

void CTonView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	CTonDoc* pcDoc = GetDocument();
	if (pcDoc->CheckRedo() == TRUE)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
	
}
void CTonView::OnEditUndo() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CTonDoc* pcDoc = GetDocument();
	if(CommandExit() == (UINT)ENDEVENT){
		if(pcDoc->CheckRedo() == FALSE){
			pcDoc->SaveRedo();
		}
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
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_EDIT_MVCOPY);
		CommandInit(ID_MAIN_EDIT);
	}
}

void CTonView::OnScanSlct() 
{
	HMODULE hDSMDLL = NULL;

	if(CommandExit() == (UINT)ENDEVENT){
		ImgKitScanSelect(m_hWnd);
	}
}

void CTonView::OnScanInput() 
{
	char	szBtpPath[FNAMEMAX+2];
	char	szFname[FNAMEMAX+2];
	char	szBmpFname[FNAMEMAX+2];
	char	szBasename[FNAMEMAX+2];
	CTonDoc		*pcDoc;
	CDibCData	*pcDibCData;
	int		cnt, ret;
	char	cntstr[16];
	char	cntstr2[16];
	DBL		mmxsize, mmysize;

	if(CommandExit() == (UINT)ENDEVENT){
		senvgetbtppath(szBtpPath);
		sprintf(szFname, "%s\\twain", szBtpPath);
		ImgKitScanMulti(m_hWnd, szFname);
		cnt = 0;
		while(1){
			sprintf(cntstr, "%03d", cnt);
			sprintf(cntstr2, "%03d", m_nBmpCnt);
			m_nBmpCnt++;
			strcpy(szBasename, szFname);
			sprintf(szFname, "%s.%s", szBasename, cntstr);
			sprintf(szBmpFname, "%s%s.bmp", szBasename, cntstr2);
			unlink(szBmpFname);
			ret = rename(szFname, szBmpFname);
			if(ret != 0){
				break;
			}
			pcDoc = (CTonDoc*)(GetDocument());
			pcDibCData = (CDibCData*)(pcDoc->NewData(DIBCDATAID));
			senvgetbtppath(szBtpPath);
			pcDibCData->SetBmpFname(szBmpFname);
			if(pcDibCData->DibLoad() == FAIL){
				return;
			}
			scrngetmmsize(&mmxsize, &mmysize);
			pcDibCData->SetDstElement(0, 0, mmxsize, mmysize);
			pcDibCData->RenewMiniMax();
			pcDoc->DataAddTail((CData*)pcDibCData);
		}
		Invalidate();
	}
}

void CTonView::OnClrGradation() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CLR_GRADATION);
		CommandInit(ID_ATR_CLR);
	}
}

void CTonView::OnClrLine() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CLR_LINE);
		CommandInit(ID_ATR_CLR);
	}
}

void CTonView::OnClrSolid() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CLR_SOLID);
		CommandInit(ID_ATR_CLR);
	}
}

void CTonView::OnEditMove() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_EDIT_MOVE);
		CommandInit(ID_MAIN_EDIT);
	}
}

void CTonView::OnFiguBoxs() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	SetFiguKind(ID_FIGU_BOXS);
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FIGU_BOXS);
		CommandInit(ID_OBJ_FIGU);
	}
}

void CTonView::OnFiguCrcl() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	SetFiguKind(ID_FIGU_CRCL);
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FIGU_CRCL);
		CommandInit(ID_OBJ_FIGU);
	}
}

void CTonView::OnFiguFree() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	SetFiguKind(ID_FIGU_FREE);
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FIGU_FREE);
		CommandInit(ID_OBJ_FIGU);
	}
}

void CTonView::OnFiguLine() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FIGU_LINE);
		CommandInit(ID_OBJ_FIGU);
	}
}

void CTonView::OnFiguPlgn() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	SetFiguKind(ID_FIGU_PLGN);
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FIGU_PLGN);
		CommandInit(ID_OBJ_FIGU);
	}
}

void CTonView::OnFiguPoint() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	SetFiguKind(ID_FIGU_POINT);
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FIGU_POINT);
		CommandInit(ID_OBJ_FIGU);
	}
}

void CTonView::OnFiguSizeboxs() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FIGU_SIZEBOXS);
		CommandInit(ID_OBJ_FIGU);
	}
}

void CTonView::OnFiguSizecrcl() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
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
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
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

void CTonView::OnFileEpssend() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_EPSSEND);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::OnFileMprnsend() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_MPRNSEND);
		CommandInit(ID_MAIN_FILE);
	}
}


void CTonView::OnFileMltprnftp() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_MLTPRNFTP);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::OnFileEmltprnftp() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_EMLTPRNFTP);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::OnFilePrnsend() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_PRNSEND);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::OnFilePltmove() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_PLTMOVE);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::OnLineArc() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_ARC);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnLineBeze() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_BEZE);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnLineHalfarc() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LINE_HALFARC);
		CommandInit(ID_TOOL_LINE);
	}
}

void CTonView::OnMirrorCntr() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MIRROR_CNTR);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnMirrorFree() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MIRROR_FREE);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnMojiDel() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_DEL);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiInseart() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_INSEART);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiMove() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_MOVE);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiFiting() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_FITING);
		CommandInit(ID_OBJ_STRG);
	}
}
void CTonView::OnMojiFontchange() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_FONTCHANGE);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiPlgning() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_PLGNING);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiRound() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_ROUND);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiSize() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_SIZE);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiSpace() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_SPACE);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiSvdel() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_SVDEL);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiLoad() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_LOAD);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnMojiSave() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MOJI_SAVE);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnPoint2point() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_2POINT);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnPointAnydel() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_ANYDEL);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnPointAnymove() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_ANYMOVE);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnPointPicup() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
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
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_XLEGAL);
		CommandInit(ID_TOOL_POINT);
	}
	
}

void CTonView::OnPointYlegal() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_POINT_YLEGAL);
		CommandInit(ID_TOOL_POINT);
	}
}

void CTonView::OnPrevPane() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
}

void CTonView::OnRound2point() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_ROUND_2POINT);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnRoundFree() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_ROUND_FREE);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnRoundRag() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_ROUND_RAG);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSize2point() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SIZE_2POINT);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSizeFree() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SIZE_FREE);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSizeTime() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SIZE_TIME);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSizeX() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SIZE_X);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSizeXy() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SIZE_XY);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSizeY() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SIZE_Y);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSlopFree() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SLOP_FREE);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSlopRag() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SLOP_RAG);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnStrgInput() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_OBJ_STRG);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnStrgOrder() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_STRG_ORDER);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnStrgRound() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_STRG_ROUND);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnStrgSlop() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_STRG_SLOP);
		CommandInit(ID_OBJ_STRG);
	}
}

void CTonView::OnStrgBoxsin() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
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
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_TRNS_ARC);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnTrnsBoxs() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_TRNS_BOXS);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnClrSeetSet() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CLR_SEET_SET);
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

void CTonView::OnWakuPaperdel() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_WAKU_PAPERDEL);
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
void CTonView::OnWakuInsekiji() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_WAKU_INSEKIJI);
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

void CTonView::OnWakuChange() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_WAKU_CHANGE);
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
	CData* pcData;
	POSITION pos;

	CTonDoc* pcDoc = GetDocument();
	noidx = 0;

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pcData = pcObjDataList->GetNext(pos);

		if(pcData->CheckID() == SWAKVECTID){
			if(noidx == m_nCrtWaku){
				pcData->SelectDraw(pcDC);
			}
			else{
				pcData->NoSelectDraw(pcDC);
			}
			noidx++;
		}
	}
}

void CTonView::PrintEnvSave(LPSTR printenvfname)
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

	if((fp = fopen(printenvfname, "wb")) != NULL){
		fwrite(&nwritemodesize, sizeof(int), 1, fp);
		fwrite(lpDevMode, GlobalSize(hDevMode), 1, fp);
		fwrite(&nwritenamessize, sizeof(int), 1, fp);
		fwrite(lpDevNames, GlobalSize(hDevNames), 1, fp);
		fclose(fp);
	}

	::GlobalUnlock(hDevNames);
	::GlobalUnlock(hDevMode);

}

void CTonView::PrintEnvLoad(LPSTR printenvfname)
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

	if((fp = fopen(printenvfname, "rb")) == NULL){
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
		hCrtDevMode = pcApp->GetDevMode();
		if(hCrtDevMode != 0){
			lpCrtDevMode = (LPDEVMODE)::GlobalLock(hCrtDevMode);
			lpLoadDevMode = (LPDEVMODE)::GlobalLock(hLoadDevMode);
			lpCrtDevMode->dmOrientation = lpLoadDevMode->dmOrientation;
			lpCrtDevMode->dmPaperSize = lpLoadDevMode->dmPaperSize;
			lpCrtDevMode->dmPaperLength = lpLoadDevMode->dmPaperLength;
			lpCrtDevMode->dmPaperWidth = lpLoadDevMode->dmPaperWidth;
			lpCrtDevMode->dmDefaultSource = lpLoadDevMode->dmDefaultSource;
			::GlobalUnlock(hLoadDevMode);
			::GlobalUnlock(hCrtDevMode);
		}
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
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SORT_HEAD);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnSortTail() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SORT_TAIL);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnSortPrev() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SORT_PREV);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnSortNext() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SORT_NEXT);
		CommandInit(ID_MAIN_CRDNT);
	}
}


void CTonView::OnGrupSet() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_GRUP_SET);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnGrupReset() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_GRUP_RESET);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnListSet() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LIST_SET);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnListReset() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_LIST_RESET);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnClrSet() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CLR_SET);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnClrBackset() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CLR_BACKSET);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnClrBackreset() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_CLR_BACKRESET);
		CommandInit(ID_MAIN_CRDNT);
	}
}

void CTonView::OnMsurXcreate() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MSUR_XCREATE);
		CommandInit(ID_OBJ_MSUR);
	}
}

void CTonView::OnMsurYcreate() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MSUR_YCREATE);
		CommandInit(ID_OBJ_MSUR);
	}
}

void CTonView::OnMsurAtrset() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MSUR_ATRSET);
		CommandInit(ID_OBJ_MSUR);
	}
}

void CTonView::OnHelpMain() 
{
	char	execpath[FNAMEMAX];
	char	szPdfFileName[RECMAX];

	senvgetexepath(execpath);
	sprintf(szPdfFileName, "%s\\�}�j���A��.pdf", execpath);
	ShellExecute(m_hWnd, NULL, szPdfFileName, NULL, NULL, SW_SHOWNORMAL);
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
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
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

void CTonView::OnAtrConerKind() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_ATR_CONER_KIND);
		CommandInit(ID_MAIN_ATR);
	}
}

// �V�X�e���ʒ�`�t�@�C��

void CTonView::OnFileCrctsave() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_CRCTSAVE);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::OnFileCrctload() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_CRCTLOAD);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::OnFileAdddel() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_ADDDEL);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::OnFileCrctdel() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_CRCTDEL);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::PrinterOutput()
{
	OnFilePrint();
}

void CTonView::PloterOutput()
{
	OnFilePloter();
}

void CTonView::OnSlopRl() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SLOP_RL);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnSlopUd() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_SLOP_UD);
		CommandInit(ID_TOOL_TRNS);
	}
}

void CTonView::OnWakuInsort() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_WAKU_INSORT);
		CommandInit(ID_OBJ_WAKU);
	}
}

void CTonView::SetFiguKind(int nfigukind)
{
	m_nFiguKind = nfigukind;
}

void CTonView::OnObjFigu()
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(m_nFiguKind);
		CommandInit(ID_OBJ_FIGU);
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

void CTonView::OnFilePloter() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_FILE_PLOTER);
		CommandInit(ID_MAIN_FILE);
	}
}

void CTonView::OnBmpRound() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_BMP_ROUND);
		CommandInit(ID_OBJ_BMP);
	}
}

void CTonView::OnBmpSave() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_BMP_SAVE);
		CommandInit(ID_OBJ_BMP);
	}
}

void CTonView::OnBmpJpgsave() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_BMP_JPGSAVE);
		CommandInit(ID_OBJ_BMP);
	}
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

void CTonView::MouseSetCrt(DBL cx, DBL cy)
{
	m_nSetBx = (int)cx;
	m_nSetBy = (int)cy;
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
	DWORD	dwExitCode;
	int		ret;

	m_nTimer++;
	CScrollView::OnTimer(nIDEvent);
	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	if(pcApp->m_nNoDispFlag == 1){
		KillTimer(m_nTimerID);
		pcApp->AppExit();
		return;
	}
	if(m_hProcess != 0){
		KillTimer(m_nTimerID);
		ret = GetExitCodeProcess(m_hProcess, &dwExitCode);
		if(dwExitCode != STILL_ACTIVE){
			m_nExitProcess = TRUE;
			return;
		}
		m_nTimerID = SetTimer(1, TIMERINTERVAL, NULL);
		m_nTimer = 0;
	}
}

int CTonView::EndTimer()
{
	if(m_nTimerID != 0){
		KillTimer(m_nTimerID);
		m_nTimerID = 0;
	}
	return(m_nTimer);
}

void CTonView::LoadCsvData(LPSTR fname)
{
	FILE	*fp;
	char	buf[RECMAX8+16];
	int		len;
	LPSTR	csvsp[RECMAX];
	char	datpath[FNAMEMAX];
	char	paperpath[FNAMEMAX];
	char	filename[FNAMEMAX];
	CData	*pcData;
	CSWakVect	*pcSWakVect;
	char	szStr[RECMAX];
	LPSTR	sp;
	DBL		xsize, ysize;

	fp = fopen(fname, "r");
	if(fp == NULL){
		return;
	}
	fgets(buf, RECMAX8, fp);
	len = strlen(buf);
	buf[len-1] = '\0';
	fclose(fp);

	CTonDoc* pcDoc = GetDocument();
	pcDoc->DataAllCls();

	len = strlen(buf);
	if(buf[len-1] == '\n'){
		buf[len-1] = '\0';
	}
	basedivcsvdata(csvsp, buf);
	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	strcpy(pcApp->m_szSavePath, csvsp[16]);
	strcpy(pcApp->m_szSaveName, csvsp[6]);
	pcApp->m_nNoDispFlag = atoi(csvsp[18]);
	if(csvsp[19][0] != '\0'){
		strcpy(pcApp->m_szBackBmpName, csvsp[19]);
	}
	else{
		pcApp->m_szBackBmpName[0] = '\0';
	}
	if(strcmp(csvsp[17], "��") == 0){
		senvgetdatpath(datpath);
		sprintf(filename, "%s\\%s", datpath, pcApp->m_szSaveName);
		basechangesafix(filename, "iti");

		CFile file(filename, CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		pcDoc->Serialize(ar);
		ar.Close();
		file.Close();
		basechangesafix(filename, "env");
		LoadCrtEnv(filename);

		basechangesafix(filename, "bin");
	    if((_access(filename, 0 )) == 0){
			PrintEnvLoad(filename);
		}
		else{
			sprintf(filename, "%s\\defprn.bin", datpath);
			PrintEnvLoad(filename);
		}
		return;
	}

	if(pcApp->m_szBackBmpName[0] != '\0'
	&& access(pcApp->m_szBackBmpName, 0) == 0){
		scrngetmmsize(&xsize, &ysize);
		CDibCData*	pcDibCData = (CDibCData*)(pcDoc->NewData(DIBCDATAID));
		if(pcDibCData == NULL){
			return;
		}
		pcDibCData->SetBmpFname(pcApp->m_szBackBmpName);
		pcDibCData->DibLoad();
		pcDibCData->SetDstElement(0.0, 0.0, xsize, ysize);
		pcDibCData->RenewMiniMax();
		pcDoc->DataAddTail(pcDibCData);
	}

	wakuenvgetfullpaperpath(paperpath);
	sprintf(filename, "%s\\%s.wak", paperpath, csvsp[14]);

	basechangesafix(filename, "env");
    if((_access(filename, 0 )) == 0){
		wakuenvloadoutputenv(filename);
	}
	WorkSizeReset();
	WorkAreaReset();
	basechangesafix(filename, "wak");
	pcDoc->WakuLoad(filename);
	pcDoc->OnFlag(ACTIVEFLAG);
	basechangesafix(filename, "bin");
    if((_access(filename, 0 )) == 0){
		PrintEnvLoad(filename);
	}
	else{
		sprintf(filename, "%s\\defprn.bin", paperpath);
		PrintEnvLoad(filename);
	}

	sp = csvsp[15];
	POSITION pos = pcDoc->m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pcData = pcDoc->m_pcObjDataList->GetNext(pos);
		if(pcData->CheckID() == SWAKVECTID){
			pcSWakVect = (CSWakVect*)pcData;
			sp = basedivstrg(sp, szStr, '/');
			if(pcSWakVect->m_nFixFlag != ON){
				pcSWakVect->SetString(szStr);
				pcSWakVect->RenewMiniMax();
				if(szStr[0] != '\0'){
					pcSWakVect->StrgLineStruct();
					pcSWakVect->RenewMiniMax();
				}
			}
		}
	}
}

void CTonView::LoadCrtEnv(LPSTR fname)
{
	FILE	*fp;
	char	buf[RECMAX];
	DBL		xmmsize, ymmsize;

	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%lf %lf\n", &xmmsize, &ymmsize);
	scrnsetmmsize(xmmsize, ymmsize);
	fclose(fp);
	WorkSizeReset();
	WorkAreaReset();
}

void CTonView::ProgExitLoop()
{
	DWORD	dwExitCode;
	int		ret;

	while(1){
		ret = GetExitCodeProcess(m_hProcess, &dwExitCode);
		if(dwExitCode != STILL_ACTIVE){
			break;
		}
		Sleep(TIMERINTERVAL);
	}
}

void CTonView::ExecSmsout()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	static STARTUPINFO siStartInfo ;
	static PROCESS_INFORMATION piProcInfo ;

	senvgetexepath(exepath);
	//chdir(exepath);
	sprintf(fname, "%s\\smsout.exe", exepath);
	if(_access(fname, 0) == 0){
	    GetStartupInfo(&siStartInfo);
		if(CreateProcess(NULL, fname, NULL, NULL, 
		      FALSE, 0, 
		      NULL, NULL, &siStartInfo, 
			  &piProcInfo) == 0){
			return;
		}
		m_hProcess = piProcInfo.hProcess;
		ProgExitLoop();
		CloseHandle(piProcInfo.hThread) ;
		CloseHandle(piProcInfo.hProcess);
		m_hProcess = 0;
	}
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

void CTonView::CallOnWakuApply()
{
	OnWakuApply();
}

void CTonView::OnWakuApply() 
{
	CommandExec(DLGSETEVENT, 0, 0);	
}

void CTonView::OnMenteCptrFile() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MENTE_CPTR_FILE);
		CommandInit(ID_MAIN_MENTE);
	}
}

void CTonView::OnMenteCptrFolder() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_MENTE_CPTR_FOLDER);
		CommandInit(ID_MAIN_MENTE);
	}
}

void CTonView::SetString(LPSTR lpStr)
{
	strcpy(m_szStockStr, lpStr);
	CommandExec(USER1EVENT, 0, 0);
}

void CTonView::OnDbedtLoad() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_DBEDT_LOAD);
		CommandInit(ID_MAIN_DBEDT);
	}
}

void CTonView::OnDbedtDelete() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_DBEDT_DELETE);
		CommandInit(ID_MAIN_DBEDT);
	}
}

void CTonView::OnUpdateDbedtDelete(CCmdUI* pCmdUI) 
{
	CTonDoc* pcDoc = GetDocument();
	if(pcDoc->m_nDBType == 0){
		pCmdUI->Enable(FALSE);
	}else{
		pCmdUI->Enable(TRUE);
	}
}

void CTonView::PrintOutDC(CDC *pcDC)
{
	CPrintInfo printInfo;

	CTonApp *pcApp = (CTonApp*)::AfxGetApp();
	CTonDoc	*pcDoc = GetDocument();
	pcDC->m_bPrinting = TRUE;
	printInfo.m_rectDraw.SetRect(0, 0,
		pcDC->GetDeviceCaps(HORZRES),
		pcDC->GetDeviceCaps(VERTRES));
	pcDC->DPtoLP(&printInfo.m_rectDraw);
	printInfo.m_pPD->m_pd.hDevMode = pcApp->GetDevMode();
	OnBeginPrinting(pcDC, &printInfo);

	DOCINFO docInfo;
	memset(&docInfo, 0, sizeof(DOCINFO));
	docInfo.cbSize = sizeof(DOCINFO);
	CString csStr = pcDoc->GetTitle();
	docInfo.lpszDocName = csStr;

	pcDC->StartDoc(&docInfo);
	pcDC->StartPage();

	OnDraw(pcDC);

	pcDC->EndPage();
	pcDC->EndDoc();

	OnEndPrinting(pcDC, &printInfo);

}

void CTonView::OnBmpLoad() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_BMP_LOAD);
		CommandInit(ID_OBJ_BMP);
	}
}

void CTonView::OnBmpClip() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_BMP_CLIP);
		CommandInit(ID_OBJ_BMP);
	}
}

void CTonView::OnBmpBkload() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_BMP_BKLOAD);
		CommandInit(ID_OBJ_BMP);
	}
}

void CTonView::OnBmpBkreset() 
{
	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_BMP_BKRESET);
		CommandInit(ID_OBJ_BMP);
	}
}

void CTonView::OnMemoWakuLoad() 
{
	char	szBackPapaerName[QRECMAX];
	char	szBackPapaerXSize, szBackPapaerYSize;

	if(CommandExit() == (UINT)ENDEVENT){
		SetSubCommand(ID_WAKU_LOAD);
		wakuenvgetpapername(szBackPapaerName);
		wakuenvsetpapername("�v���o�p�l��");
		wakuenvgetpaperelement();
		CommandInit(ID_OBJ_WAKU);
		wakuenvsetpapername(szBackPapaerName);
		wakuenvgetpaperelement();
	}
}

PROCESS_INFORMATION m_piProcInfo;	// �v���Z�X���X�e�[�^�X�e�[�u��

void CTonView::OnMemoCallprog() 
{
	char	execpath[FNAMEMAX];
	char	exeprogfile[FNAMEMAX];
	char	execommand[FNAMEMAX];
	char	szVectFileName[FNAMEMAX];
	char	szBmpFileName[FNAMEMAX];
	STARTUPINFO siStartInfo;	// �v���Z�X������ԃe�[�u��
	CDibCData *pcDibCData;
	DBL xsize, ysize;

	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	CTonDoc	*pcDoc = (CTonDoc*)GetDocument();
	sprintf(szVectFileName, "%s\\memory.vct", pcApp->m_szTempImagePath);
	pcDoc->SaveVectFile(szVectFileName);
	scrngetmmsize(&xsize, &ysize);
	senvgetexepath(execpath);
	sprintf(exeprogfile, "%s\\smm.exe", execpath);
	if(_access(exeprogfile, 0) == 0){
		sprintf(szBmpFileName, "%s\\memory.bmp", pcApp->m_szTempImagePath);
		unlink(szBmpFileName);
		m_piProcInfo.hProcess = 0;
		sprintf(execommand, "%s %0.1lf,%0.1lf,", exeprogfile, xsize, ysize);
	    GetStartupInfo(&(siStartInfo));
		if(CreateProcess(NULL, execommand, NULL, NULL, 
		      FALSE, 0, 
		      NULL, NULL, &siStartInfo, 
			  &m_piProcInfo) == 0){
			return;
		}
		MemoryProgExitLoop();
		CloseHandle(m_piProcInfo.hThread) ;
		CloseHandle(m_piProcInfo.hProcess);
		m_hProcess = 0;
		if(_access(szBmpFileName, 0) == 0){
			pcDibCData = (CDibCData*)(pcDoc->NewData(DIBCDATAID));
			if(pcDibCData == NULL){
				return;
			}
			pcDibCData->SetBmpFname(szBmpFileName);
			pcDibCData->DibLoad();
			pcDibCData->RenewMiniMax();
			pcDibCData->SetDstElement(0.0, 0.0, xsize, ysize);
			pcDibCData->RenewMiniMax();
			pcDoc->DataAddTail0(pcDibCData);
			Invalidate(TRUE);
		}
	}
}
void CTonView::MemoryProgExitLoop()
{
	DWORD	dwExitCode;
	int		ret;

	while(1){
		ret = GetExitCodeProcess(m_piProcInfo.hProcess, &dwExitCode);
		if(dwExitCode != STILL_ACTIVE){
			break;
		}
		Sleep(TIMERINTERVAL);
	}
}
void CTonView::OnMemoEnvset() 
{
	CDialogMemoryEnvSet *pcDialogMemoryEnvSet;
	pcDialogMemoryEnvSet = new CDialogMemoryEnvSet(this);
	pcDialogMemoryEnvSet->DoModal();
	delete(pcDialogMemoryEnvSet);
}

void CTonView::OnMenteWakuBackup() 
{
	CMente *pcCMente;

	pcCMente = new CMente;
	delete(pcCMente);
	::AfxMessageBox("�g�̃o�b�N�A�b�v���I�����܂���", MB_OK);
}

void CTonView::OnUpdateWakuInput(CCmdUI* pCmdUI) 
{
}

void CTonView::OnUpdateWakuInsekiji(CCmdUI* pCmdUI) 
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];

	senvgetexepath(exepath);
	sprintf(fname, "%s\\sekiji.env", exepath);
	if (access(fname, 0) != 0)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}
