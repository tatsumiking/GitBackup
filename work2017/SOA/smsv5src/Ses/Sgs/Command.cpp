/////////////////////////////////////////////////////////////////////////////
// CCommand

#include "stdafx.h"

#include	"DialogScrnEnv.h"
#include	"Command.h"

#include <dlgs.h>       // for standard control IDs for commdlg

static TCHAR addfilter[] = _T("筆耕システム Files (*.iti)|*.iti||");
static TCHAR addsafixs[] = _T("iti");

//extern "C" {
	#include "../../ObjLib/Senvlib.h"
	#include "../../ObjLib/Scrnlib.h"
//}

CCommand::CCommand(CScrollView* pcview)
{
	m_pcView = pcview;
	SetCrtCommand(NOOP);
	m_pcCmdSystem = new CCmdSystem(pcview);
	m_pcCmdFile = new CCmdFile(pcview);
	m_pcCmdSlct = new CCmdSlct(pcview);
	m_pcCmdEdit = new CCmdEdit(pcview);
	m_pcCmdStrg = new CCmdStrg(pcview);
	m_pcCmdMoji = new CCmdMoji(pcview);
	m_pcCmdPart = new CCmdPart(pcview);
	m_pcCmdStrgProp = new CCmdStrgProp(pcview);
	m_pcCmdSpclStrg = new CCmdSpclStrg(pcview);
	m_pcCmdKamonCrct = new CCmdKamonCrct(pcview);
	m_pcCmdWaku = new CCmdWaku(pcview);
	m_pcCmdFigu = new CCmdFigu(pcview);
	m_pcCmdZoom = new CCmdZoom(pcview);
	m_pcCmdTrns = new CCmdTrns(pcview);
	m_pcCmdPoint = new CCmdPoint(pcview);
	m_pcCmdLine = new CCmdLine(pcview);
	m_pcCmdSLine = new CCmdSLine(pcview);
	m_pcCmdSpcl = new CCmdSpcl(pcview);
	m_pcCmdDibC = new CCmdDibC(pcview);
	m_pcCmdClr = new CCmdClr(pcview);
	m_pcCmdAtr = new CCmdAtr(pcview);
	m_pcCmdSub = new CCmdSub(pcview);
	m_pcCmdCrdnt = new CCmdCrdnt(pcview);
	m_pcCmdPloter = new CCmdPloter(pcview);
	m_pcCmdKamon = new CCmdKamon(pcview);
	m_nFirstFlag = TRUE;
	m_nCrtCommand = NOOP;
}

CCommand::~CCommand()
{
	delete(m_pcCmdKamon);
	delete(m_pcCmdPloter);
	delete(m_pcCmdSpclStrg);
	delete(m_pcCmdCrdnt);
	delete(m_pcCmdSub);
	delete(m_pcCmdAtr);
	delete(m_pcCmdClr);
	delete(m_pcCmdDibC);
	delete(m_pcCmdSpcl);
	delete(m_pcCmdLine);
	delete(m_pcCmdSLine);
	delete(m_pcCmdPoint);
	delete(m_pcCmdTrns);
	delete(m_pcCmdZoom);
	delete(m_pcCmdFigu);
	delete(m_pcCmdKamonCrct);
	delete(m_pcCmdWaku);
	delete(m_pcCmdStrg);
	delete(m_pcCmdMoji);
	delete(m_pcCmdStrgProp);
	delete(m_pcCmdEdit);
	delete(m_pcCmdSlct);
	delete(m_pcCmdFile);
	delete(m_pcCmdSystem);
}

void CCommand::SetDocument(CDocument* pcDoc)
{
	m_pcDoc = pcDoc;
}

void CCommand::SetCrtCommand(UINT command)
{
	m_nCrtCommand = command;
}

UINT CCommand::GetCrtCommand()
{
	return(m_nCrtCommand);
}

void CCommand::SetSubCommand(int nSubCommand)
{
	CTonApp*	theApp;
	CMainFrame*	theWnd;

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	theWnd->m_nCrtCommand = nSubCommand;
	theWnd->SetCrtView((CTonView*)m_pcView);
	m_nSubCommand = nSubCommand;
	theWnd->ShowItemElement();
}

UINT CCommand::GetSubCommand()
{
	return(m_nSubCommand);	
}

UINT CCommand::Exec(UINT event, DBL dx, DBL dy)
{
	UINT	ret;
	CPoint	point;
	CTonView*	theView;
	CTonApp*	theApp;
	CSysEnv*	pcSysEnv;
	CMainFrame*	theWnd;
	TCHAR	msg[RECMAX];
	CDialogScrnEnv*	pDialogScrnEnv;

	//if(m_nCrtCommand == ID_MAIN_SLCT && m_nFirstFlag == TRUE){
	//	return(ENDEVENT);
	//}
	//m_nFirstFlag = FALSE;

	theView = (CTonView*)m_pcView;
	theApp = (CTonApp*)AfxGetApp();
	pcSysEnv = theApp->m_pcSysEnv;
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	scrndxyrltvtomm(&dx, &dy);
	if(pcSysEnv->ScrnCheckCntrlKey() == ON){
		pcSysEnv->ScrnAdjustGrid(&dx, &dy);				
	}
	theWnd->StatXYOut(dx, dy);

#ifdef _DEBUG
	if(event == INITEVENT){
		MEMORYSTATUS	ms;

		::GlobalMemoryStatus(&ms);
		_stprintf_s(msg, _T("%02ld％ 実メモリ0x%06lx/0x%06lx 仮想ファイル0x%08lx/0x%08lx 仮想メモリ0x%08lx/0x%08lx"),
			ms.dwMemoryLoad, 
			ms.dwAvailPhys, ms.dwTotalPhys,
			ms.dwAvailPageFile, ms.dwTotalPageFile,
			ms.dwAvailVirtual, ms.dwTotalVirtual);
		theWnd->HelpTextOut(0, msg);
	}
#endif
	if(event == INITEVENT){
		switch(m_nCrtCommand){
		case ID_FILE_ENVSET:
			pDialogScrnEnv = new CDialogScrnEnv(m_pcView);
			pDialogScrnEnv->DoModal();
			delete(pDialogScrnEnv);
			((CTonView*)m_pcView)->WorkSizeReset();
			((CTonView*)m_pcView)->WorkAreaReset();
			theWnd->m_nCrtCommand = ID_FILE_ENVSET;
			theWnd->InitItemElement();
			m_pcView->Invalidate();
			return(ENDEVENT);
		}
	}

	ret = 0;
	switch(m_nCrtCommand){
	case ID_MAIN_FILE:
		ret = ExecFile(event, dx, dy);
		break;
	case ID_MAIN_SLCT:
		ret = ExecSlct(event, dx, dy);
		break;
	case ID_MAIN_EDIT:
		ret = ExecEdit(event, dx, dy);
		break;
	case ID_MAIN_ZOOM:
		ret = ExecZoom(event, dx, dy);
		break;
	case ID_MAIN_TOOL:
		ret = ExecTool(event, dx, dy);
		break;
	case ID_OBJ_STRG:
		ret = ExecStrg(event, dx, dy);
		break;
	case ID_OBJ_WAKU:
		ret = ExecWaku(event, dx, dy);
		break;
	case ID_OBJ_KAMON:
		ret = ExecKamon(event, dx, dy);
		break;
	case ID_OBJ_FIGU:
		ret = ExecFigu(event, dx, dy);
		break;
	case ID_OBJ_BMP:
		ret = ExecDibC(event, dx, dy);
		break;
	case ID_TOOL_TRACE:
		ret = ExecTrace(event, dx, dy);
		break;
	case ID_TOOL_TRNS:
		ret = ExecTrns(event, dx, dy);
		break;
	case ID_TOOL_POINT:
		if(event == INITEVENT){
			m_nBackMode = senvsetdrawmode(POINTVIEW);
			if(m_nBackMode != POINTVIEW){
				m_pcView->Invalidate();
			}
		}
		ret = ExecPoint(event, dx, dy);
		break;
	case ID_TOOL_LINE:
		if(event == INITEVENT){
			m_nBackMode = senvsetdrawmode(POINTVIEW);
			if(m_nBackMode != POINTVIEW){
				m_pcView->Invalidate();
			}
		}
		ret = ExecLine(event, dx, dy);
		break;
	case ID_TOOL_SPCL:
		ret = ExecSpcl(event, dx, dy);
		break;
	case ID_MAIN_CRDNT:
		ret = ExecCrdnt(event, dx, dy);
		break;
	case ID_MAIN_ATR:
		ret = ExecAtr(event, dx, dy);
		break;
	case ID_MAIN_SUB:
		ret = ExecSub(event, dx, dy);
		break;
	case ID_ATR_CLR:
		ret = ExecAtrClr(event, dx, dy);
		break;
	default:
		ret = ENDEVENT;
		break;
	}
	if(ret == REDRAWEVENT){
		m_pcView->Invalidate();
	}
	return(ret);
}

UINT CCommand::ExecFile(UINT event, DBL dx, DBL dy)
{
	UINT	ret;

	switch(m_nSubCommand){
	case ID_FILE_PLOTER:
		if(event != INITEVENT){
			return(0);
		}
		m_pcCmdPloter->ExecPlotOut(event, dx, dy);
		ret = ENDEVENT;
		break;
	case ID_FILE_PLTMOVE:
		ret = m_pcCmdPloter->ExecPlotMove(event, dx, dy);
		break;
	case ID_FILE_ADDENVLOAD:
		if(event == INITEVENT){
			m_pcCmdFile->SetFilter(addfilter, addsafixs);
		}
		ret = m_pcCmdFile->ExecAddEnvLoad(event, dx, dy);
		break;
	case ID_FILE_ADDLOAD:
		if(event == INITEVENT){
			m_pcCmdFile->SetFilter(addfilter, addsafixs);
		}
		ret = m_pcCmdFile->ExecAddLoad(event, dx, dy);
		break;
	case ID_FILE_ADDSAVE:
		if(event == INITEVENT){
			m_pcCmdFile->SetFilter(addfilter, addsafixs);
		}
		ret = m_pcCmdFile->ExecAddSave(event, dx, dy);
		break;
	case ID_FILE_ADDDEL:
		if(event == INITEVENT){
			m_pcCmdFile->SetFilter(addfilter, addsafixs);
		}
		ret = m_pcCmdFile->ExecAddDel(event, dx, dy);
		break;
	case ID_FILE_CLRPRINT:
		//ret = m_pcCmdPrint->ExecPrintOut(event, dx, dy);
		//break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);

}

UINT CCommand::ExecStrg(UINT event, DBL dx, DBL dy)
{
	UINT	ret;

	switch(m_nSubCommand){
	case ID_OBJ_STRG:
		ret = m_pcCmdStrg->ExecStrgCreate(event, dx, dy);
		break;
	case ID_STRG_EDIT:
		ret = m_pcCmdStrg->ExecStrgEdit(event, dx, dy);
		break;
	case ID_STRG_ORDER:
		ret = m_pcCmdStrg->ExecStrgOrder(event, dx, dy);
		break;
	case ID_STRG_ROUND:
		ret = m_pcCmdStrg->ExecStrgRound(event, dx, dy);
		break;
	case ID_STRG_SLOP:
		ret = m_pcCmdStrg->ExecStrgSlop(event, dx, dy);
		break;
	case ID_STRG_BOXSIN:
		ret = m_pcCmdStrg->ExecStrgBoxsin(event, dx, dy);
		break;
	//case ID_STRG_SETARC:
	//	ret = m_pcCmdStrgArc->ExecStrgSetArc(event, dx, dy);
	//	break;
	//case ID_STRG_FREEARC:
	//	ret = m_pcCmdStrgArc->ExecStrgFreeArc(event, dx, dy);
	//	break;
	case ID_STRG_PROP:
		ret = m_pcCmdStrgProp->ExecStrgProp(event, dx, dy);
		break;
	//case ID_STRG_SPACE:
	//	ret = m_pcCmdStrg->ExecStrgSpace(event, dx, dy);
	//	break;
	case ID_MOJI_DEL:
		ret = m_pcCmdStrg->ExecMojiDel(event, dx, dy);
		break;
	case ID_MOJI_INSEART:
		ret = m_pcCmdStrg->ExecMojiInseart(event, dx, dy);
		break;
	case ID_MOJI_MOVE:
		ret = m_pcCmdMoji->ExecMojiMove(event, dx, dy);
		break;
	//case ID_MOJI_FONTCHANGE:
	//	ret = m_pcCmdMoji->ExecFontChange(event, dx, dy);
	//	break;
	case ID_MOJI_PLGNING:
		ret = m_pcCmdStrg->ExecPlgning(event, dx, dy);
		break;
	case ID_MOJI_ROUND:
		ret = m_pcCmdMoji->ExecMojiRound(event, dx, dy);
		break;
	case ID_MOJI_SIZE:
		ret = m_pcCmdMoji->ExecMojiSize(event, dx, dy);
		break;
	case ID_MOJI_SPACE:
		ret = m_pcCmdStrg->ExecMojiSpace(event, dx, dy);
		break;
	case ID_MOJI_SAVE:
		ret = m_pcCmdMoji->ExecMojiSave(event, dx, dy);
		break;
	case ID_MOJI_LOAD:
		ret = m_pcCmdMoji->ExecMojiLoad(event, dx, dy);
		break;
	case ID_MOJI_SVDEL:
		ret = m_pcCmdMoji->ExecMojiSaveDel(event, dx, dy);
		break;
	case ID_PART_SAVE:
		ret = m_pcCmdPart->ExecPartSave(event, dx, dy);
		break;
	case ID_PART_LOAD:
		ret = m_pcCmdPart->ExecPartLoad(event, dx, dy);
		break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}

UINT CCommand::ExecFigu(UINT event, DBL dx, DBL dy)
{
	UINT	ret;
	CTonDoc*	theDoc;

	theDoc = (CTonDoc*)m_pcDoc;
	switch(m_nSubCommand){
	case ID_FIGU_POINT:
		ret = m_pcCmdFigu->ExecPoint(event, dx, dy);
		break;
	case ID_FIGU_LINE:
		ret = m_pcCmdFigu->ExecLine(event, dx, dy);
		break;
	case ID_FIGU_BOXS:
		ret = m_pcCmdFigu->ExecBoxs(event, dx, dy);
		break;
	case ID_FIGU_CRCL:
		ret = m_pcCmdFigu->ExecCrcl(event, dx, dy);
		break;
	case ID_FIGU_FREE:
		ret = m_pcCmdFigu->ExecFree(event, dx, dy);
		break;
	case ID_FIGU_SIZEBOXS:
		if(event == INITEVENT){
			theDoc->OffFlag(ACTIVEFLAG);			
		}
		ret = m_pcCmdFigu->ExecSizeBoxs(event, dx, dy);
		if(ret == ENDEVENT){
			m_nCrtCommand = ID_MAIN_EDIT;
			m_nSubCommand = ID_EDIT_MOVE;
			ret = REDRAWEVENT;
			Exec(INITEVENT, dx, dy);
		}
		break;
	case ID_FIGU_SIZECRCL:
		if(event == INITEVENT){
			theDoc->OffFlag(ACTIVEFLAG);			
		}
		ret = m_pcCmdFigu->ExecSizeCrcl(event, dx, dy);
		if(ret == ENDEVENT){
			m_nCrtCommand = ID_MAIN_EDIT;
			m_nSubCommand = ID_EDIT_MOVE;
			ret = REDRAWEVENT;
			Exec(INITEVENT, dx, dy);
		}
		break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}
UINT CCommand::ExecSlct(UINT event, DBL dx, DBL dy)
{
	UINT	ret;

	switch(m_nSubCommand){
	case ID_SLCT_OBJ:
		ret = m_pcCmdSlct->ExecAreaSlctMove(event, dx, dy);
		break;
	case ID_SLCT_AREA:
		ret = m_pcCmdSlct->ExecArea(event, dx, dy);
		break;
	case ID_SLCT_CHNG:
		ret = m_pcCmdSlct->ExecChng();
		break;
	case ID_SLCT_ALL:
		ret = m_pcCmdSlct->ExecAll();
		m_pcView->Invalidate();
		ret = ENDEVENT;
		break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}

UINT CCommand::ExecEdit(UINT event, DBL dx, DBL dy)
{
	UINT	ret;
	CTonDoc*	theDoc;

	theDoc = (CTonDoc*)m_pcDoc;
	switch(m_nSubCommand){
	case ID_EDIT_DELETE:
		m_pcCmdSub->ResetWakuData();
		ret = m_pcCmdEdit->ExecDelete();
		break;
	case ID_EDIT_MOVE:
		ret = m_pcCmdEdit->ExecMove(event, dx, dy);
		break;
	//case ID_EDIT_PMOVE:
	//	ret = m_pcCmdEdit->ExecPMove(event, dx, dy);
	//	break;
	case ID_EDIT_MVCOPY:
		ret = m_pcCmdEdit->ExecCopy(event, dx, dy);
		break;
	case ID_MOVE_RLTV:
		ret = m_pcCmdEdit->ExecMoveRltv(event, dx, dy);
		break;
	case ID_MOVE_COOD:
		ret = m_pcCmdEdit->ExecMoveCood(event, dx, dy);
		break;
	//case ID_MOVE_CNTR:
	//	ret = m_pcCmdEdit->ExecMoveCntr(event, dx, dy);
	//	break;
	case ID_COPY_RLTV:
		ret = m_pcCmdEdit->ExecCopyRltv(event, dx, dy);
		break;
	case ID_COPY_COOD:
		ret = m_pcCmdEdit->ExecCopyCood(event, dx, dy);
		break;
	case ID_COPY_PICH:
		ret = m_pcCmdEdit->ExecCopyPichNoMove(event, dx, dy);
		break;
	//case ID_COPY_RPICH:
	//	ret = m_pcCmdEdit->ExecCopyRPich(event, dx, dy);
	//	break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}

UINT CCommand::ExecZoom(UINT event, DBL dx, DBL dy)
{
	UINT	ret;

	switch(m_nSubCommand){
	case ID_ZOOM_IN:
		ret = m_pcCmdZoom->ExecZoomIn(event, dx, dy);
		break;
	case ID_ZOOM_OUT:
		ret = m_pcCmdZoom->ExecZoomOut(event, dx, dy);
		break;
	case ID_ZOOM_ALL:
		m_pcCmdZoom->ExecZoomAll();
		ret = ENDEVENT;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}

UINT CCommand::ExecPoint(UINT event, DBL dx, DBL dy)
{
	UINT	ret;

	switch(m_nSubCommand){
	case ID_POINT_MOVE:
		ret = m_pcCmdPoint->ExecPointMove(event, dx, dy);
		break;
	case ID_POINT_DEL:
		ret = m_pcCmdPoint->ExecPointDel(event, dx, dy);
		break;
	case ID_POINT_ADD:
		ret = m_pcCmdPoint->ExecPointAdd(event, dx, dy);
		break;
	case ID_POINT_ANYDEL:
		ret = m_pcCmdPoint->ExecAnyDel(event, dx, dy);
		break;
	case ID_POINT_ANYMOVE:
		ret = m_pcCmdPoint->ExecAnyMove(event, dx, dy);
		break;
	case ID_POINT_2POINT:
		ret = m_pcCmdPoint->Exec2Point(event, dx, dy);
		break;
	case ID_POINT_PICUP:
		ret = m_pcCmdPoint->ExecPicup(event, dx, dy);
		break;
	case ID_POINT_CHNGATR:
		ret = m_pcCmdPoint->ExecPointChngAtr(event, dx, dy);
		break;
	case ID_POINT_SLOPLEGAL:
		ret = m_pcCmdPoint->ExecSlopLegal(event, dx, dy);
		break;
	case ID_POINT_XLEGAL:
		ret = m_pcCmdPoint->ExecXLegal(event, dx, dy);
		break;
	case ID_POINT_YLEGAL:
		ret = m_pcCmdPoint->ExecYLegal(event, dx, dy);
		break;
	default:
		ret = ENDEVENT;
	}
	return(ret);
}

UINT CCommand::ExecLine(UINT event, DBL dx, DBL dy)
{
	UINT	ret;

	switch(m_nSubCommand){
	case ID_LINE_MOVE:
		ret = m_pcCmdLine->ExecLineMove(event, dx, dy);
		break;
	case ID_LINE_DEL:
		m_pcCmdSub->ResetWakuData();
		ret = m_pcCmdLine->ExecLineDel(event, dx, dy);
		break;
	case ID_LINE_LINEING:
		ret = m_pcCmdLine->ExecLineLineing(event, dx, dy);
		break;
	case ID_LINE_CURVEING:
		ret = m_pcCmdLine->ExecLineCurve(event, dx, dy);
		break;
	case ID_LINE_CUT:
		ret = m_pcCmdLine->ExecLineCut(event, dx, dy);
		break;
	case ID_LINE_JOIN:
		ret = m_pcCmdLine->ExecLineJoin(event, dx, dy);
		break;
	case ID_LINE_CUTJOIN:
		ret = m_pcCmdLine->ExecLineCutJoin(event, dx, dy);
		break;
	case ID_SLINE_CUTJOIN:
		ret = m_pcCmdSLine->ExecCutJoin(event, dx, dy);
		break;
	case ID_LINE_ARC:
		ret = m_pcCmdLine->ExecLineArc(event, dx, dy);
		break;
	case ID_LINE_BEZE:
		ret = m_pcCmdLine->ExecLineBeze(event, dx, dy);
		break;
	case ID_LINE_HALFARC:
		ret = m_pcCmdLine->ExecLineHalfArc(event, dx, dy);
		break;
	case ID_LINE_CONERSET:
		ret = m_pcCmdLine->ExecLineConerset(event, dx, dy);
		break;
	case ID_LINE_CONERARC:
		ret = m_pcCmdLine->ExecLineConerarc(event, dx, dy);
		break;
	case ID_LINE_CRCL:
		ret = m_pcCmdLine->ExecLineCrcl(event, dx, dy);
		break;
	//case ID_LINE_DIV:
	//	ret = m_pcCmdLine->ExecLineDiv(event, dx, dy);
	//	break;
	//case ID_SPCL_PARALLEL:
	//	ret = m_pcCmdSpclLine->ExecParallel(event, dx, dy);
	//	break;
	//case ID_SPCL_PREPEND:
	//	ret = m_pcCmdSpclLine->ExecPrepend(event, dx, dy);
	//	break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}

UINT CCommand::ExecTrns(UINT event, DBL dx, DBL dy)
{
	UINT	ret;

	switch(m_nSubCommand){
	case ID_MIRROR_CNTR:
		ret = m_pcCmdTrns->ExecTrnsMirrorConer(event, dx, dy);
		break;
	case ID_MIRROR_FREE:
		ret = m_pcCmdTrns->ExecTrnsMirr(event, dx, dy);
		break;
	case ID_ROUND_2POINT:
		ret = m_pcCmdTrns->ExecTrnsRound2Point(event, dx, dy);
		break;
	case ID_ROUND_RAG:
		ret = m_pcCmdTrns->ExecTrnsRoundRag(event, dx, dy);
		break;
	case ID_ROUND_FREE:
		ret = m_pcCmdTrns->ExecTrnsRound(event, dx, dy);
		break;
	case ID_SIZE_2POINT:
		ret = m_pcCmdTrns->ExecTrnsSize2Point(event, dx, dy);
		break;
	case ID_SIZE_TIME:
		ret = m_pcCmdTrns->ExecTrnsSizeTime(event, dx, dy);
		break;
	case ID_SIZE_X:
		ret = m_pcCmdTrns->ExecTrnsSizeX(event, dx, dy);
		break;
	case ID_SIZE_Y:
		ret = m_pcCmdTrns->ExecTrnsSizeY(event, dx, dy);
		break;
	case ID_SIZE_XY:
		ret = m_pcCmdTrns->ExecTrnsSizeXY(event, dx, dy);
		break;
	case ID_SIZE_FREE:
		ret = m_pcCmdTrns->ExecTrnsSize(event, dx, dy);
		break;
	//case ID_SIZE_DISP:
	//	ret = m_pcCmdTrns->ExecTrnsSizeDisp(event, dx, dy);
	//	break;
	case ID_SLOP_RAG:
	//case ID_SLOP_RL:
		ret = m_pcCmdTrns->ExecTrnsSlopRag(event, dx, dy);
		break;
	//case ID_SLOP_UD:
	//	ret = m_pcCmdTrns->ExecTrnsSlopRagUd(event, dx, dy);
	//	break;
	case ID_SLOP_FREE:
		ret = m_pcCmdTrns->ExecTrnsSlop(event, dx, dy);
		break;
	case ID_TRNS_ARC:
		ret = m_pcCmdTrns->ExecTrnsArc(event, dx, dy);
		break;
	case ID_TRNS_BOXS:
		ret = m_pcCmdTrns->ExecTrnsBoxs(event, dx, dy);
		break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}

UINT CCommand::ExecSpcl(UINT event, DBL x, DBL y)
{
	UINT	ret;
	CTonDoc *pcDoc;

	ret = 0;
	switch(m_nSubCommand){
	case ID_SPCL_SHADOW:
		if(event == INITEVENT){
			ret = m_pcCmdSpcl->ExecShadow(event, x, y);		
			m_pcView->Invalidate();
			return(ENDEVENT);
		}
		break;
	case ID_SPCL_OUTLINE:
		if(event == INITEVENT){
			ret = m_pcCmdSpcl->ExecOutLine(event, x, y);		
			m_pcView->Invalidate();
			return(ENDEVENT);
		}
		break;
	case ID_SPCL_MARGE:
		if(event == INITEVENT){
			pcDoc = (CTonDoc *)m_pcView->GetDocument();
			//ret = m_pcCmdCrdnt->ExecListSet(event, x, y);
			ret = m_pcCmdSpcl->ExecMarge(event, x, y);
			//ret = m_pcCmdCrdnt->ExecListReset(event, x, y);
			m_pcView->Invalidate();
			return(ENDEVENT);
		}
		break;
	//case ID_RULE_OUTLINE:
	//	if(event == INITEVENT){
	//		ret = m_pcCmdSpcl->ExecRuleOutLine(event, x, y);		
	//		m_pcView->Invalidate();
	//		return(ENDEVENT);
	//	}
	//	break;
	case ID_SPCL_STRGSTRCT:
		ret = m_pcCmdSpclStrg->ExecStrgLineStruct(event, x, y);
		break;
	//case ID_SPCL_AREA:
	//	ret = m_pcCmdSpcl->ExecAreaSlct(event, x, y);
	//	if(ret == ENDEVENT){
	//		m_pcView->Invalidate();
	//	}
	//	break;
	case ID_SPCL_OMITOUTLINE:
		if(event == INITEVENT){
			ret = m_pcCmdSpcl->ExecOutLineOmit(event, x, y);		
			m_pcView->Invalidate();
			return(ENDEVENT);
		}
		break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}

UINT CCommand::ExecDibC(UINT event, DBL x, DBL y)
{
	UINT	ret;

	switch(m_nSubCommand){
	case ID_BMP_LOAD:
		ret = m_pcCmdDibC->ExecBmpLoad(event, x, y);
		break;
	case ID_BMP_TRNS:
		ret = m_pcCmdDibC->ExecRound(event, x, y);
		break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}

UINT CCommand::ExecTrace(UINT event, DBL dx, DBL dy)
{
	UINT	ret;

	switch(m_nSubCommand){
	case ID_TOOL_TRACE:
		ret = m_pcCmdDibC->ExecTrace(event, dx, dy);
		break;
	case ID_FILE_DOTEDIT:
		ret = m_pcCmdDibC->ExecDotedit(event, dx, dy);
		break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}

UINT CCommand::ExecCrdnt(UINT event, DBL dx, DBL dy)
{
	UINT	ret;

	switch(m_nSubCommand){
	case ID_GRUP_SET:
		ret = m_pcCmdCrdnt->ExecGrupSet(event, dx, dy);
		break;
	case ID_GRUP_RESET:
		ret = m_pcCmdCrdnt->ExecGrupReset(event, dx, dy);
		break;
	case ID_LIST_SET:
		ret = m_pcCmdCrdnt->ExecListSet(event, dx, dy);
		break;
	case ID_LIST_RESET:
		ret = m_pcCmdCrdnt->ExecListReset(event, dx, dy);
		break;
	case ID_CLR_SET:
		ret = m_pcCmdClr->ExecClrSet(event, dx, dy);
		break;
	case ID_SORT_HEAD:
		ret = m_pcCmdCrdnt->ExecSortHead(event, dx, dy);
		break;
	case ID_SORT_TAIL:
		ret = m_pcCmdCrdnt->ExecSortTail(event, dx, dy);
		break;
	case ID_SORT_NEXT:
		ret = m_pcCmdCrdnt->ExecSortNext(event, dx, dy);
		break;
	case ID_SORT_PREV:
		ret = m_pcCmdCrdnt->ExecSortPrev(event, dx, dy);
		break;
	case ID_CRDNT_TOP:
		ret = m_pcCmdCrdnt->ExecCrdntTop(event, dx, dy);
		break;
	case ID_CRDNT_VCENTER:
		ret = m_pcCmdCrdnt->ExecCrdntVCenter(event, dx, dy);
		break;
	case ID_CRDNT_BOTTOM:
		ret = m_pcCmdCrdnt->ExecCrdntBottom(event, dx, dy);
		break;
	case ID_CRDNT_LEFT:
		ret = m_pcCmdCrdnt->ExecCrdntLeft(event, dx, dy);
		break;
	case ID_CRDNT_HCENTER:
		ret = m_pcCmdCrdnt->ExecCrdntHCenter(event, dx, dy);
		break;
	case ID_CRDNT_RIGHT:
		ret = m_pcCmdCrdnt->ExecCrdntRight(event, dx, dy);
		break;
	case ID_MOVE_HCENTER:
		ret = m_pcCmdCrdnt->ExecMoveHCenter(event, dx, dy);
		break;
	case ID_MOVE_VCENTER:
		ret = m_pcCmdCrdnt->ExecMoveVCenter(event, dx, dy);
		break;
	case ID_MOVE_HVCENTER:
		ret = m_pcCmdCrdnt->ExecMoveHVCenter(event, dx, dy);
		break;
	case ID_SIZE_HSAME:
		ret = m_pcCmdCrdnt->ExecSizeHSame(event, dx, dy);
		break;
	case ID_SIZE_VSAME:
		ret = m_pcCmdCrdnt->ExecSizeVSame(event, dx, dy);
		break;
	case ID_SIZE_HVSAME:
		ret = m_pcCmdCrdnt->ExecSizeHVSame(event, dx, dy);
		break;
	case ID_SPACE_HEQUAL:
		ret = m_pcCmdCrdnt->ExecSpaseHEqual(event, dx, dy);
		break;
	case ID_SPACE_VEQUAL:
		ret = m_pcCmdCrdnt->ExecSpaseVEqual(event, dx, dy);
		break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}

UINT CCommand::ExecAtrClr(UINT event, DBL dx, DBL dy)
{
	UINT	ret;

	switch(m_nSubCommand){
	case ID_CLR_GRADATION:
		ret = m_pcCmdClr->ExecGradation(event, dx, dy);
		break;
	case ID_CLR_SEET_SET:
		ret = m_pcCmdClr->Exec(event, dx, dy);
		break;
	case ID_CLR_SOLID:
		ret = m_pcCmdClr->Exec(event, dx, dy);
		break;
	case ID_CLR_LINE:
		ret = m_pcCmdClr->Exec(event, dx, dy);
		break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}

UINT CCommand::ExecAtr(UINT event, DBL dx, DBL dy)
{
	UINT	ret;

	switch(m_nSubCommand){
	case ID_ATR_CONER_KIND:
		ret = m_pcCmdAtr->ExecAtrConer(event, dx, dy);
		break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}

UINT CCommand::ExecTool(UINT event, DBL dx, DBL dy)
{
	UINT	ret;

	switch(m_nSubCommand){
	case ID_TOOL_INPORT:
		ret = m_pcCmdFile->ExecInport(event, dx, dy);
		break;
	case ID_TOOL_EXPORT:
		ret = m_pcCmdFile->ExecExport(event, dx, dy);
		break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}


UINT CCommand::ExecWaku(UINT event, DBL dx, DBL dy)
{
	UINT	ret;

	switch(m_nSubCommand){
	case ID_WAKU_GRUP:
		ret = m_pcCmdWaku->ExecWakuGrupSet(event, dx, dy);
		break;
	case ID_WAKU_PAPER:
		ret = m_pcCmdWaku->ExecWakuPaperSet(event, dx, dy);
		break;
	case ID_WAKU_INPUT:
		ret = m_pcCmdWaku->ExecWakuInput(event, dx, dy);
		break;
	case ID_WAKU_SAVE:
		ret = m_pcCmdWaku->ExecWakuSave(event, dx, dy);
		break;
	case ID_WAKU_LOAD:
		ret = m_pcCmdWaku->ExecWakuLoad(event, dx, dy);
		if(ret == ENDEVENT){
			m_pcView->ValidateRect(NULL); 
			m_pcView->Invalidate(TRUE);
		}
		break;
	case ID_WAKU_DELETE:
		ret = m_pcCmdWaku->ExecWakuDelete(event, dx, dy);
		break;
	case ID_WAKU_SORT:
		ret = m_pcCmdWaku->ExecWakuSort(event, dx, dy);
		break;
	//case ID_WAKU_INSORT:
	//	ret = m_pcCmdWaku->ExecWakuInsort(event, dx, dy);
	//	break;
	case ID_KAMON_LOAD:
		ret = m_pcCmdKamonCrct->ExecKamonLoad(event, dx, dy);
		if(ret == SETEVENT){
			ret = 0;
			m_nCrtCommand = ID_MAIN_EDIT;
			m_nSubCommand = ID_EDIT_MOVE;
			Exec(INITEVENT, dx, dy);
		}
		break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}

UINT CCommand::ExecKamon(UINT event, DBL dx, DBL dy)
{
	UINT	ret;

	switch(m_nSubCommand){
	case ID_ESKAMON_LOAD:
		ret = m_pcCmdKamon->ExecKamonLoad(event, dx, dy);
		break;
	case ID_ESKAMON_SAVE:
		ret = m_pcCmdKamon->ExecKamonSave(event, dx, dy);
		break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}

UINT CCommand::ExecSub(UINT event, DBL dx, DBL dy)
{
	UINT	ret;

	switch(m_nSubCommand){
	case ID_SUB_GRID:
		ret = m_pcCmdSub->ExecSubGrid(event, dx, dy);
		break;
	case ID_SUB_WAKU:
		ret = m_pcCmdSub->ExecSubWaku(event, dx, dy);
		break;
	case ID_SUB_WAKUDEL:
		ret = m_pcCmdSub->ExecSubWakuDel(event, dx, dy);
		break;
	case ID_SUB_ANYWAKU:
		ret = m_pcCmdSub->ExecSubAnyWaku(event, dx, dy);
		break;
	//case ID_ASST_CNTRLINE_FREE:
	//	ret = m_pcCmdSub->ExecCntrLineFree(event, dx, dy);
	//	break;
	//case ID_ASST_CNTRLINE_COOD:
	//	ret = m_pcCmdSub->ExecCntrLineCood(event, dx, dy);
	//	break;
	//case ID_ASST_CNTRLINE_RLTV:
	//	ret = m_pcCmdSub->ExecCntrLineRltv(event, dx, dy);
	//	break;
	//case ID_ASST_SUBLINE_FREE:
	//	ret = m_pcCmdSub->ExecSubLineFree(event, dx, dy);
	//	break;
	//case ID_ASST_SUBLINE_FREERAG:
	//	ret = m_pcCmdSub->ExecSubLineFreerag(event, dx, dy);
	//	break;
	//case ID_ASST_SUBLINE_COOD:
	//	ret = m_pcCmdSub->ExecSubLineCood(event, dx, dy);
	//	break;
	//case ID_ASST_SUBLINE_RLTV:
	//	ret = m_pcCmdSub->ExecSubLineRltv(event, dx, dy);
	//	break;
	//case ID_ASST_SUBLINE_RAG:
	//	ret = m_pcCmdSub->ExecSubLineRag(event, dx, dy);
	//	break;
	//case ID_ASST_SUBLINE_DEL:
	//	ret = m_pcCmdSub->ExecSubLineDel(event, dx, dy);
	//	break;
	//case ID_SCRNCLRSET:
	//	ret = m_pcCmdSub->ExecScrnClrSet(event, dx, dy);
	//	break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);

}

