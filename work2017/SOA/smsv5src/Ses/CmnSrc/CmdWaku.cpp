
/////////////////////////////////////////////////////////////////////////////
// CCmdWaku

#include "stdafx.h"
#include "winuser.h"
#include "io.h"

#include "CmdBase.h"
#include "CmdWaku.h"

#include "../../Object/Data.h"
#include "../../Object/Vect.h"
#include "../../Object/SWakVect.h"

#include "DialogWakuEdit.h"
#include "DialogWakuInput.h"
#include "DialogSlctStrg.h"
#include "MinMax.h"

#include "DialogWakuGrup.h"
#include "DialogWkPaperDel.h"
#include "DialogWakuPaper.h"
#include "DialogWakuSave.h"
#include "DialogWakuLoad.h"
#include "DialogWakuDelete.h"
#include "DialogWakuSort.h"
#include "DialogWakuInput.h"
#include "DialogSlctStrg.h"
#include "DialogTextInput.h"

#define	SCRNXYTIME	1.8

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Mojienv.h"
#include "../../ObjLib/Wakuenv.h"
//}

CCmdWaku::CCmdWaku(CScrollView* pcview)
:CCmdBase(pcview)
{
	
}

UINT CCmdWaku::ExecWakuGrupSet(UINT event, DBL dx, DBL dy)
{
	CDialogWakuGrup*	pDialogWakuGrup;
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	pDialogWakuGrup = new CDialogWakuGrup(m_pcView);
	ret = pDialogWakuGrup->DoModal();
	if(ret == IDOK){
		m_pcView->WorkSizeReset();
		//m_pcWnd->m_nCrtCommand = ID_WAKU_GRUP;
		m_pcWnd->InitItemElement();
	}
	delete(pDialogWakuGrup);
	return(ENDEVENT);
}

UINT CCmdWaku::ExecWakuPaperDel(UINT event, DBL dx, DBL dy)
{
	CDialogWkPaperDel*	pcDialogWkPaperDel;
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	pcDialogWkPaperDel = new CDialogWkPaperDel(m_pcView);
	ret = pcDialogWkPaperDel->DoModal();
	if(ret == IDOK){
		m_pcWnd->InitItemElement();
	}
	delete(pcDialogWkPaperDel);
	return(ENDEVENT);
}

UINT CCmdWaku::ExecWakuPaperSet(UINT event, DBL dx, DBL dy)
{
	CDialogWakuPaper*	pDialogWakuPaper;
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	pDialogWakuPaper = new CDialogWakuPaper(m_pcView);
	ret = pDialogWakuPaper->DoModal();
	if(ret == IDOK){
		m_pcView->WorkSizeReset();
	}
	delete(pDialogWakuPaper);
	return(ENDEVENT);
}

UINT CCmdWaku::ExecWakuSave(UINT event, DBL dx, DBL dy)
{
	CDialogWakuSave*	pDialogWakuSave;
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	pDialogWakuSave = new CDialogWakuSave(m_pcView);
	pDialogWakuSave->m_nSlctWaku = m_pcView->m_nSlctWaku;
	ret = pDialogWakuSave->DoModal();
	if(ret == IDOK){
		SaveCrtSlctWaku();
	}
	delete(pDialogWakuSave);
	return(ENDEVENT);
}

UINT CCmdWaku::ExecWakuLoad(UINT event, DBL dx, DBL dy)
{
	CDialogWakuLoad*	pDialogWakuLoad;
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	pDialogWakuLoad = new CDialogWakuLoad(m_pcView);
	ret = pDialogWakuLoad->DoModal();
	if(ret == IDOK){
		LoadCrtSlctWaku();
		SetToolBarElement();
	}
	delete(pDialogWakuLoad);
	return(ENDEVENT);
}

UINT CCmdWaku::ExecWakuDelete(UINT event, DBL dx, DBL dy)
{
	CDialogWakuDelete*	pDialogWakuDelete;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	pDialogWakuDelete = new CDialogWakuDelete(m_pcView);
	pDialogWakuDelete->DoModal();
	delete(pDialogWakuDelete);
	return(ENDEVENT);
}

UINT CCmdWaku::ExecWakuSort(UINT event, DBL dx, DBL dy)
{
	CDialogWakuSort*	pDialogWakuSort;
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	pDialogWakuSort = new CDialogWakuSort(m_pcView);
	ret = pDialogWakuSort->DoModal();
	if(ret == IDOK){
	}
	delete(pDialogWakuSort);
	return(ENDEVENT);
}

UINT CCmdWaku::ExecWakuInput(UINT event, DBL dx, DBL dy)
{
	DBL			xsize, ysize;

	switch(event){
	case INITEVENT:
		InitClass();
		m_nDlg = SW_SHOW;
		m_nMouseSetCount = 0;
		SaveWakuString();
		scrngetmmsize(&xsize, &ysize);
		m_nDialogID = IDD_WAKU_INPUT;
		m_pcDialogWakuInput = new CDialogWakuInput(m_pcView);
		if(m_pcDialogWakuInput->Create(m_nDialogID, m_pcView)){
			LocateWakuStrDlg();
		}
		m_pcDialogSlctStrg = new CDialogSlctStrg(m_pcView);
		if(m_pcDialogSlctStrg->Create(IDD_SLCTSTRG, m_pcView)){
			m_pcDialogSlctStrg->ShowWindow(SW_SHOW);
		}

		break;
	case SETEVENT:
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:	// —vC³
		SaveWakuString();
		break;
	case RESETEVENT:
		break;
	case SETFOCUSEVENT:
		if(m_nDlg == SW_HIDE){
			m_pcDialogWakuInput->ShowWindow(SW_SHOW);
			m_nDlg = SW_SHOW;
		}
		break;
	case KILLFOCUSEVENT:
		break;
	case DLGSETEVENT:
		m_nDlg = SW_HIDE;
		break;
	case DLGRESETEVENT:
		SaveWakuString();
		break;
	case DLGDRAWEVENT:
		LoadWakuString();
		LocateWakuStrDlg();
		m_pcView->Invalidate();
		//return(REDRAWEVENT);
		break;
	case ENDEVENT:
		m_pcDialogWakuInput->DestroyWindow();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdWaku::ExecWakuInsort(UINT event, DBL dx, DBL dy)
{
	CData*	pcData;

	switch(event){
	case INITEVENT:
		InitClass();
		m_nSort = 0;
		break;
	case SETEVENT:
		m_nIDary[0] = SWAKVECTID;
		m_nIDcnt = 1;
		pcData = SrchIdsData(m_nIDary, m_nIDcnt, dx, dy);
		if(pcData != NULL){ 
			m_pcSWakVect = (CSWakVect*)pcData;
			m_pSortSWakVect[m_nSort] = m_pcSWakVect;
			m_nSort++;
			m_pcSWakVect->OffFlag(ACTIVEFLAG);
			ScrnDataRedraw((CData*)m_pcSWakVect);
			//return(REDRAWEVENT);
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		MoveWakuLink();
		ScrnAllRedraw();
		return(ENDEVENT);
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

void CCmdWaku::SWakVectStringReset()
{
	TCHAR	instrg[RECMAX];

	mojienvgetstring(instrg);
	m_pcSWakVect->SetString(instrg);
	instrg[0] = _T('\0');
	mojienvsetstring(instrg);
}

void CCmdWaku::DrawWakuBoxs(CDC *pcDC)
{
	DBL		workmmx, workmmy;
	DBL		sx, sy;
	DBL		ex, ey;
	DBL		cx, cy;
	DBL		x1, y1, x2, y2;

	sx = m_dSX;
	sy = m_dSY;
	ex = m_dEX;
	ey = m_dEY;

	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	cx = (sx + ex) / 2;
	cy = (sy + ey) / 2;

	scrngetmmsize(&workmmx, &workmmy);

	scrnsetslctpen(pcDC->m_hDC);
	switch(m_nStartKind){
	case CNTRCNTR:
		x1 = workmmx/2;
		y1 = workmmy/2;
		break;
	case LEFTUP:
		x1 = 0;
		y1 = workmmy;
		break;
	case LEFTDOWN:
		x1 = 0;
		y1 = 0;
		break;
	case RIGHTUP:
		x1 = workmmx;
		y1 = workmmy;
		break;
	case RIGHTDOWN:
		x1 = workmmx;
		y1 = 0;
		break;
	}

	switch(m_nConerKind){
	case CNTRCNTR:
		x2 = (sx+ex)/2;
		y2 = (sy+ey)/2;
		break;
	case LEFTUP:
		x2 = sx;
		y2 = sy;
		break;
	case LEFTDOWN:
		x2 = sx;
		y2 = ey;
		break;
	case RIGHTUP:
		x2 = ex;
		y2 = sy;
		break;
	case RIGHTDOWN:
		x2 = ex;
		y2 = ey;
		break;
	}

	scrndxymmtorltv(&x1, &y1);
	pcDC->MoveTo((long)x1, (long)y1);
	pcDC->LineTo((long)x2, (long)y2);

	DrawBoxs(pcDC, sx, sy, ex, ey);

	DrawBoxs(pcDC, sx-4, sy-4, sx+4, sy+4);
	DrawBoxs(pcDC, sx-4, ey-4, sx+4, ey+4);
	DrawBoxs(pcDC, ex-4, ey-4, ex+4, ey+4);
	DrawBoxs(pcDC, ex-4, sy-4, ex+4, sy+4);

	DrawBoxs(pcDC, sx-4, cy-4, sx+4, cy+4);
	DrawBoxs(pcDC, ex-4, cy-4, ex+4, cy+4);
	DrawBoxs(pcDC, cx-4, sy-4, cx+4, sy+4);
	DrawBoxs(pcDC, cx-4, ey-4, cx+4, ey+4);

	scrnpenend(pcDC->m_hDC);
}

DBL CCmdWaku::CntrKindInit(DBL dx, DBL dy)
{
	DBL		sx, sy;
	DBL		ex, ey;
	DBL		cx, cy;
	DBL		len, tlen;

	m_dBX = dx;
	m_dBY = dy;

	sx = m_dSX;
	sy = m_dSY;
	ex = m_dEX;
	ey = m_dEY;

	cx = (sx + ex) / 2;
	cy = (sy + ey) / 2;
	len = INTMAX;
	tlen = hypot(cx-dx, cy-dy);
	if(len > tlen){
		m_nCntrKind = CNTRCNTR;
		len = tlen;
	}
	tlen = hypot(sx-dx, ey-dy);
	if(len > tlen){
		m_nCntrKind = LEFTUP;
		len = tlen;
	}
	tlen = hypot(sx-dx, sy-dy);
	if(len > tlen){
		m_nCntrKind = LEFTDOWN;
		len = tlen;
	}
	tlen = hypot(ex-dx, sy-dy);
	if(len > tlen){
		m_nCntrKind = RIGHTDOWN;
		len = tlen;
	}
	tlen = hypot(ex-dx, ey-dy);
	if(len > tlen){
		m_nCntrKind = RIGHTUP;
		len = tlen;
	}
	tlen = hypot(sx-dx, cy-dy);
	if(len > tlen){
		m_nCntrKind = LEFTCNTR;
		len = tlen;
	}
	tlen = hypot(ex-dx, cy-dy);
	if(len > tlen){
		m_nCntrKind = RIGHTCNTR;
		len = tlen;
	}
	tlen = hypot(cx-dx, ey-dy);
	if(len > tlen){
		m_nCntrKind = CNTRUP;
		len = tlen;
	}
	tlen = hypot(cx-dx, sy-dy);
	if(len > tlen){
		m_nCntrKind = CNTRDOWN;
		len = tlen;
	}
	m_dCX = dx;
	m_dCY = dy;
	return(len);
}

void CCmdWaku::CntrKindMove(DBL dx, DBL dy)
{
	DBL		sx, sy;
	DBL		ex, ey;
	DBL		movex, movey;

	sx = m_dSX;
	sy = m_dSY;
	ex = m_dEX;
	ey = m_dEY;
	movex = dx - m_dCX;
	movey = dy - m_dCY;
	switch(m_nCntrKind){
	case LEFTUP:
		sx += movex;
		ey += movey;
		break;
	case LEFTDOWN:
		sx += movex;
		sy += movey;
		break;
	case LEFTCNTR:
		sx += movex;
		break;
	case RIGHTUP:
		ex += movex;
		ey += movey;
		break;
	case RIGHTDOWN:
		ex += movex;
		sy += movey;
		break;
	case RIGHTCNTR:
		ex += movex;
		break;
	case CNTRUP:
		ey += movey;
		break;
	case CNTRDOWN:
		sy += movey;
		break;
	case CNTRCNTR:
		sx += movex;
		sy += movey;
		ex += movex;
		ey += movey;
		break;
	}
	m_dSX = sx;
	m_dSY = sy;
	m_dEX = ex;
	m_dEY = ey;

	m_dCX = dx;
	m_dCY = dy;

	if(fabs(m_dEX - m_dSX) > fabs(m_dEY - m_dSY)){
		mojienvsettategakiflag(OFF);
	}
	else{
		mojienvsettategakiflag(ON);
	}

}

void CCmdWaku::WakuBoxsElementSet()
{
	int		ftategaki;
	DBL		dwidth, dhight;
	DBL		d_tcslen;
	DBL		sx, sy;

	mojienvgettategakiflag(&ftategaki);
	mojienvgetsize(&dwidth, &dhight);
	mojienvgetstrlen(&d_tcslen);
	if(ftategaki == ON){
		dwidth = m_dEX - m_dSX;
		dhight = m_dEX - m_dSX;
		d_tcslen = m_dSY - m_dEY;
	}
	else{
		d_tcslen = m_dEX - m_dSX;
		dwidth = m_dSY - m_dEY;
		dhight = m_dSY - m_dEY;
	}
	mojienvsetstart(m_dSX, m_dSY);
	mojienvgetstart(&sx, &sy);
	mojienvsetsize(dwidth, dhight);
	mojienvsetstrlen(d_tcslen);
}

void CCmdWaku::StdLineInit()
{
	DBL		dwidth, dhight;
	DBL		d_tcslen;
	DBL		dstartx, dstarty;
	DBL		sx, sy, ex, ey;
	DBL		mmxsize, mmysize;

	scrngetmmsize(&mmxsize, &mmysize);
	m_pcSWakVect->GetFontSize(&dwidth, &dhight);
	m_pcSWakVect->GetStrLen(&d_tcslen);
	m_pcSWakVect->GetStrStartXY(&dstartx, &dstarty);
	sx = dstartx;
	sy = dstarty;
	if(m_pcSWakVect->CheckTateYoko() == ON){
		ex = sx + dwidth;
		ey = sy - d_tcslen;
	}
	else{
		ex = sx + d_tcslen;
		ey = sy - dhight;
	}
	switch(m_nMode){
	case LINEUPUP:
		m_dSX = 0;
		m_dEX = mmxsize; 
		m_dSY = sy;
		m_dEY = sy;
		break;
	case LINEUPUDCNTR:
		m_dSX = 0;
		m_dEX = mmxsize; 
		m_dSY = (sy + ey) / 2.0;
		m_dEY = (sy + ey) / 2.0;
		break;
	case LINEUPDOWN:
		m_dSX = 0;
		m_dEX = mmxsize; 
		m_dSY = ey;
		m_dEY = ey;
		break;
	case LINEUPLEFT:
		m_dSX = sx;
		m_dEX = sx;
		m_dSY = 0;
		m_dEY = mmysize; 
		break;
	case LINEUPLRCNTR:
		m_dSX = (sx + ex) / 2.0;
		m_dEX = (sx + ex) / 2.0;
		m_dSY = 0;
		m_dEY = mmysize; 
		break;
	case LINEUPRIGHT:
		m_dSX = ex;
		m_dEX = ex;
		m_dSY = 0;
		m_dEY = mmysize; 
		break;
	}
}

void CCmdWaku::DrawStdLine(CDC *pcDC)
{
	DBL		sx, sy;
	DBL		ex, ey;

	sx = m_dSX; sy = m_dSY;
	ex = m_dEX; ey = m_dEY;

	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);

	scrnsetslctpen(pcDC->m_hDC);
	pcDC->MoveTo((long)sx, (long)sy);
	pcDC->LineTo((long)ex, (long)ey);

	scrnpenend(pcDC->m_hDC);
}

void CCmdWaku::MoveWakuData()
{
	DBL		movex, movey;
	DBL		dwidth, dhight;
	DBL		d_tcslen;
	DBL		dstartx, dstarty;
	DBL		sx, sy, ex, ey;

	m_pcSWakVect->GetFontSize(&dwidth, &dhight);
	m_pcSWakVect->GetStrLen(&d_tcslen);
	m_pcSWakVect->GetStrStartXY(&dstartx, &dstarty);
	sx = dstartx;
	sy = dstarty;
	if(m_pcSWakVect->CheckTateYoko() == ON){
		ex = sx + dwidth;
		ey = sy - d_tcslen;
	}
	else{
		ex = sx + d_tcslen;
		ey = sy - dhight;
	}
	switch(m_nMode){
	case LINEUPUP:
		movex = 0;
		movey = m_dSY - sy;
		break;
	case LINEUPUDCNTR:
		movex = 0;
		movey = m_dSY - (sy+ey)/2;
		break;
	case LINEUPDOWN:
		movex = 0;
		movey = m_dSY - ey;
		break;
	case LINEUPLEFT:
		movex = m_dSX - sx;
		movey = 0;
		break;
	case LINEUPLRCNTR:
		movex = m_dSX - (sx+ex)/2;
		movey = 0;
		break;
	case LINEUPRIGHT:
		movex = m_dSX - ex;
		movey = 0;
		break;
	}
	m_pcSWakVect->MoveTrns(movex, movey);	
}

void CCmdWaku::SetMode(int mode)
{
	m_nMode = mode;
}

void CCmdWaku::SetBoxsElement()
{
	DBL		dwidth, dhight;
	DBL		d_tcslen;
	DBL		dstartx, dstarty;
	TCHAR	fontname[RECMAX];
	TCHAR	instr[RECMAX];

	m_pcSWakVect->GetFontName(fontname);
	mojienvsetfontname(fontname);
	m_pcSWakVect->GetString(instr);
	mojienvsetstring(instr);
	m_pcSWakVect->GetFontSize(&dwidth, &dhight);
	m_pcSWakVect->GetStrLen(&d_tcslen);
	m_pcSWakVect->GetStrStartXY(&dstartx, &dstarty);
	m_dSX = dstartx;
	m_dSY = dstarty;
	if(m_pcSWakVect->CheckTateYoko() == ON){
		m_dEX = m_dSX + dwidth;
		m_dEY = m_dSY - d_tcslen;
	}
	else{
		m_dEX = m_dSX + d_tcslen;
		m_dEY = m_dSY - dhight;
	}
}

void CCmdWaku::LoadWakuString()
{
	m_pcDoc->LoadStringStoreFile(SWAKVECTID);
}

void CCmdWaku::SaveWakuString()
{
	m_pcDoc->SaveStringStoreFile(SWAKVECTID);
}

void CCmdWaku::LocateWakuStrDlg()
{
	CRect	prect;
	CRect	crect;
	int		x, y, cx, cy;

	m_pcView->GetWindowRect(&prect);
	m_pcDialogWakuInput->GetWindowRect(&crect);

	x = prect.right - crect.Width();
	y = prect.top;

	cx = crect.Width();
	cy = crect.Height();
	m_pcDialogWakuInput->ShowWindow(SW_SHOW);
	::SetWindowPos(m_pcDialogWakuInput->m_hWnd, HWND_TOP, x, y, cx, cy, 0);
}

void CCmdWaku::DelDrawWakuBoxs(CDC *pcDC)
{
	DBL		workmmx, workmmy;
	DBL		sx, sy;
	DBL		ex, ey;
	DBL		cx, cy;
	DBL		x1, y1, x2, y2;

	sx = m_dSX;
	sy = m_dSY;
	ex = m_dEX;
	ey = m_dEY;

	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	cx = (sx + ex) / 2;
	cy = (sy + ey) / 2;

	scrngetmmsize(&workmmx, &workmmy);

	scrnsetdelpen(pcDC->m_hDC);
	switch(m_nStartKind){
	case CNTRCNTR:
		x1 = workmmx/2;
		y1 = workmmy/2;
		break;
	case LEFTUP:
		x1 = 0;
		y1 = workmmy;
		break;
	case LEFTDOWN:
		x1 = 0;
		y1 = 0;
		break;
	case RIGHTUP:
		x1 = workmmx;
		y1 = workmmy;
		break;
	case RIGHTDOWN:
		x1 = workmmx;
		y1 = 0;
		break;
	}

	switch(m_nConerKind){
	case CNTRCNTR:
		x2 = (sx+ex)/2;
		y2 = (sy+ey)/2;
		break;
	case LEFTUP:
		x2 = sx;
		y2 = sy;
		break;
	case LEFTDOWN:
		x2 = sx;
		y2 = ey;
		break;
	case RIGHTUP:
		x2 = ex;
		y2 = sy;
		break;
	case RIGHTDOWN:
		x2 = ex;
		y2 = ey;
		break;
	}

	scrndxymmtorltv(&x1, &y1);
	pcDC->MoveTo((long)x1, (long)y1);
	pcDC->LineTo((long)x2, (long)y2);

	DrawBoxs(pcDC, sx, sy, ex, ey);

	DrawBoxs(pcDC, sx-4, sy-4, sx+4, sy+4);
	DrawBoxs(pcDC, sx-4, ey-4, sx+4, ey+4);
	DrawBoxs(pcDC, ex-4, ey-4, ex+4, ey+4);
	DrawBoxs(pcDC, ex-4, sy-4, ex+4, sy+4);

	DrawBoxs(pcDC, sx-4, cy-4, sx+4, cy+4);
	DrawBoxs(pcDC, ex-4, cy-4, ex+4, cy+4);
	DrawBoxs(pcDC, cx-4, sy-4, cx+4, sy+4);
	DrawBoxs(pcDC, cx-4, ey-4, cx+4, ey+4);

	scrnpenend(pcDC->m_hDC);
}

void CCmdWaku::MoveWakuLink()
{
	CData		*pcData;
	int			idx;
	POSITION	pos, prepos;
	int			nID;

	pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
	while(1){
		prepos = pos;
		pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
		nID = pcData->CheckID();
		if(nID == SWAKVECTID){
			break;
		}
	}
	pos = prepos;
	m_pcDoc->m_pcObjDataList->GetPrev(pos);
	prepos = pos;
	for(idx = m_nSort-1; idx >= 0; idx--){
		pcData = (CData*)m_pSortSWakVect[idx];
		pos = m_pcDoc->m_pcObjDataList->Find(pcData);
		if(pos == NULL){
			break;
		}
		m_pcDoc->m_pcObjDataList->RemoveAt(pos);
		if(prepos == NULL){
			m_pcDoc->m_pcObjDataList->AddHead(pcData);
		}
		else{
			m_pcDoc->m_pcObjDataList->InsertAfter(prepos, pcData);
		}
		pcData->OnFlag(ACTIVEFLAG);
	}
}

void CCmdWaku::InitWakuAreaLegal()
{
	DBL		xmm, ymm;

	scrngetmmsize(&xmm, &ymm);
	m_dSX = xmm / 3;
	m_dEX = xmm / 3 * 2;
	m_dEY = ymm / 3;
	m_dSY = ymm / 3 * 2;
	WakuBoxsElementSet();
}

void CCmdWaku::SaveCrtSlctWaku()
{
	CDialogTextInput*	pDialogTextInput;
	TCHAR	paperpath[FNAMEMAX];
	TCHAR	wakuname[FNAMEMAX];
	TCHAR	filename[FNAMEMAX];
	int		ret;
	int		srchidx, slctidx;

	wakuenvinit(m_pcView->m_szInfoEnv);
	wakuenvgetpaperelement();
	wakuenvgetfullpaperpath(paperpath);
	wakuenvgetselectwakuidx(&slctidx);

	wakuenvlistgetwakuname(slctidx, wakuname);

	pDialogTextInput = new CDialogTextInput(m_pcView);
	pDialogTextInput->m_nIMEMode = ON;
	pDialogTextInput->m_csText = wakuname;
INPUTWAKU:
	ret = pDialogTextInput->DoModal();
	if(ret == IDOK){
		_tcscpy_s(wakuname, pDialogTextInput->m_csText.GetBuffer(RECMAX));
		baseaddsafix(wakuname, _T("wak"));
		_stprintf_s(filename, _T("%s\\%s"), paperpath, wakuname);

		srchidx = wakuenvlistsrchwakuname(wakuname);
		if(srchidx != slctidx && srchidx != -1){
			ret = AfxMessageBox(_T("“¯ˆê–¼Ì‚Ì˜g‚ª‘¶Ý‚µ‚Ü‚·"), MB_OKCANCEL, -1);
			if(ret == IDOK){
				slctidx = srchidx;
				goto INPUTWAKU;
			}
			delete(pDialogTextInput);
			return;
		}

		m_pcDoc->WakuSave(filename);
		basechangesafix(filename, _T("env"));
		wakuenvsaveoutputenv(filename);
		basechangesafix(filename, _T("bin"));
		m_pcView->PrintEnvSave(filename);

		wakuenvlistaddwakuname(slctidx, wakuname);
	}
	delete(pDialogTextInput);
}

void CCmdWaku::LoadCrtSlctWaku()
{
	TCHAR	paperpath[FNAMEMAX];
	TCHAR	wakuname[FNAMEMAX];
	TCHAR	filename[FNAMEMAX];
	int		slctidx;
	int		cnt, ret;

	wakuenvgetfullpaperpath(paperpath);
	wakuenvgetselectwakuidx(&slctidx);
	if(slctidx < 0){
		return;
	}
	wakuenvlistgetwakuname(slctidx, wakuname);
	m_pcDoc->SetTitle(wakuname);
	_stprintf_s(filename, _T("%s\\%s"), paperpath, wakuname);
	m_pcDoc->m_nSaveWaku = OFF;
	cnt = m_pcDoc->m_pcObjDataList->GetCount();
	if(cnt != 0){
		ret = AfxMessageBox(_T("Œ»Ý‚Ì˜g‚ðíœ‚µ‚Ü‚·‚©"), MB_OKCANCEL);
		if(ret == IDOK){
			m_pcDoc->DataAllCls();
		}
		else{
			m_pcDoc->m_nSaveWaku = ON;
			m_pcDoc->OffFlag(ACTIVEFLAG);
		}
	}
	DBL		xsize, ysize;
	basechangesafix(filename, _T("env"));
    if((_taccess_s(filename, 0 )) == 0){
		wakuenvloadoutputenv(filename);
		scrngetmmsize(&xsize, &ysize);
	}
	m_pcView->WorkSizeReset();
	m_pcView->WorkAreaReset();
	scrngetmmsize(&xsize, &ysize);
	basechangesafix(filename, _T("wak"));
	m_pcDoc->WakuLoad(filename);
	// m_pcDoc->OnFlag(ACTIVEFLAG);
	scrngetmmsize(&xsize, &ysize);
	
	basechangesafix(filename, _T("bin"));
    if((_taccess_s(filename, 0 )) == 0){
		m_pcView->PrintEnvLoad(filename);
	}
	else{
		_stprintf_s(filename, _T("%s\\defprn.bin"), paperpath);
		m_pcView->PrintEnvLoad(filename);
	}
	
	scrngetmmsize(&xsize, &ysize);
	m_pcDoc->m_nSaveWaku = OFF;
}


