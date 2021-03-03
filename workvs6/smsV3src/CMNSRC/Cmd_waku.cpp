
/////////////////////////////////////////////////////////////////////////////
// CCmdWaku

#include "stdafx.h"
#include "winuser.h"
#include "math.h"
#include "io.h"

#include "cmd_waku.h"

#include	"DlgTxtIn.h"
#include	"DlgWkGrp.h"
#include	"DlgWkPpr.h"
#include	"DlgWkSv.h"
#include	"DlgWkLd.h"
#include	"DlgWkDel.h"
#include	"DlgWkSrt.h"
#include	"DlgWkAtr.h"
#include	"DWkPpDel.h"

#define	SCRNXYTIME	1.8

extern "C" {
	void baseaddsafix(LPSTR FileName, LPSTR Safix);
	void basechangesafix(LPSTR FileName, LPSTR Safix);
	void scrnsetdelpen(HDC PaintDC);
	void scrnsetslctpen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	void scrnsetrltvelement();
	void scrnresetzoomtime();
	void scrndxymmtorltv(LPDBL x, LPDBL y);
	void scrndxyrltvtomm(LPDBL x, LPDBL y);
	void scrndlenmmtorltv(LPDBL len);
	void scrngetrltvtop(LPDBL xTop, LPDBL yTop);
	void scrngetmmsize(LPDBL pdmmxsize, LPDBL pdmmysize);
	void mojienvgetfontname(LPSTR fontname);
	void mojienvsetfontname(LPSTR fontname);
	void mojienvgettategakiflag(LPINT tateflg);
	void mojienvsettategakiflag(int tateflg);
	void mojienvgetstart(LPDBL dstartx, LPDBL dstarty);
	void mojienvsetstart(DBL dstartx, DBL dstarty);
	void mojienvgetsize(LPDBL dfwidth, LPDBL dfhight);
	void mojienvsetsize(DBL dfwidth, DBL dfhight);
	void mojienvgetspace(LPDBL dmsp, LPDBL dlsp);
	void mojienvsetspace(DBL dmsp, DBL dlsp);
	void mojienvgetstrlen(LPDBL dstrlen);
	void mojienvsetstrlen(DBL dstrlen);
	void mojienvgetstring(LPSTR instrg);
	void mojienvsetstring(LPSTR instrg);
	void mojienvgetmojicount(LPDBL dmojicount);
	void mojienvsetmojicount(DBL dmojicount);
	void mojienvsetspacekind(int spkflg);
	void mojienvgetspacekind(LPINT spkflg);
	void mojienvsetyokomojiflag(int yokomoji);
	void mojienvgetyokomojiflag(LPINT yokomoji);
	void mojienvgetstartkind(LPINT nkind);
	void senvgetwakuconerkind(LPINT nkind);
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
	void wakuenvinit(LPSTR envfile);
	void wakuenvgetfixflag(int *flag);
	void wakuenvsetfixflag(int flag);
	void wakuenvgettrnspar(LPDBL par);
	void wakuenvsettrnspar(DBL par);
	void wakuenvgetmatching(LPSTR mstr);
	void wakuenvsetmatching(LPSTR mstr);
	void wakuenvgetpaperelement();
	void wakuenvgetpapersize(LPDBL paperwidth, LPDBL paperhight);
	void wakuenvgetfullpaperpath(LPSTR paperpath);
	void wakuenvgetselectwakuidx(int *idx);
	void wakuenvsaveoutputenv(LPSTR wakusavefname);
	void wakuenvloadoutputenv(LPSTR wakuloadfname);
	void wakuenvlistaddwakuname(int slctidx, LPSTR wakufname);
	void wakuenvlistgetwakuname(int slctidx, LPSTR wakufname);
	void wakuenvlistdelwakuname(int slctidx);
	int wakuenvlistsrchwakuname(LPSTR srchname);
	int DispatchWindows(HWND hWin);
}

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
			ScrnDataRedraw(m_pcSWakVect);
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
	char	instrg[RECMAX];

	mojienvgetstring(instrg);
	m_pcSWakVect->SetString(instrg);
	instrg[0] = '\0';
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
	DBL		dstrlen;
	DBL		sx, sy;

	mojienvgettategakiflag(&ftategaki);
	mojienvgetsize(&dwidth, &dhight);
	mojienvgetstrlen(&dstrlen);
	if(ftategaki == ON){
		dwidth = m_dEX - m_dSX;
		dhight = m_dEX - m_dSX;
		dstrlen = m_dSY - m_dEY;
	}
	else{
		dstrlen = m_dEX - m_dSX;
		dwidth = m_dSY - m_dEY;
		dhight = m_dSY - m_dEY;
	}
	mojienvsetstart(m_dSX, m_dSY);
	mojienvgetstart(&sx, &sy);
	mojienvsetsize(dwidth, dhight);
	mojienvsetstrlen(dstrlen);
}

void CCmdWaku::StdLineInit()
{
	DBL		dwidth, dhight;
	DBL		dstrlen;
	DBL		dstartx, dstarty;
	DBL		sx, sy, ex, ey;
	DBL		mmxsize, mmysize;

	scrngetmmsize(&mmxsize, &mmysize);
	m_pcSWakVect->GetFontSize(&dwidth, &dhight);
	m_pcSWakVect->GetStrLen(&dstrlen);
	m_pcSWakVect->GetStrStartXY(&dstartx, &dstarty);
	sx = dstartx;
	sy = dstarty;
	if(m_pcSWakVect->CheckTateYoko() == ON){
		ex = sx + dwidth;
		ey = sy - dstrlen;
	}
	else{
		ex = sx + dstrlen;
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
	DBL		dstrlen;
	DBL		dstartx, dstarty;
	DBL		sx, sy, ex, ey;

	m_pcSWakVect->GetFontSize(&dwidth, &dhight);
	m_pcSWakVect->GetStrLen(&dstrlen);
	m_pcSWakVect->GetStrStartXY(&dstartx, &dstarty);
	sx = dstartx;
	sy = dstarty;
	if(m_pcSWakVect->CheckTateYoko() == ON){
		ex = sx + dwidth;
		ey = sy - dstrlen;
	}
	else{
		ex = sx + dstrlen;
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
	DBL		dstrlen;
	DBL		dstartx, dstarty;
	char	fontname[RECMAX];
	char	instr[RECMAX];

	m_pcSWakVect->GetFontName(fontname);
	mojienvsetfontname(fontname);
	m_pcSWakVect->GetString(instr);
	mojienvsetstring(instr);
	m_pcSWakVect->GetFontSize(&dwidth, &dhight);
	m_pcSWakVect->GetStrLen(&dstrlen);
	m_pcSWakVect->GetStrStartXY(&dstartx, &dstarty);
	m_dSX = dstartx;
	m_dSY = dstarty;
	if(m_pcSWakVect->CheckTateYoko() == ON){
		m_dEX = m_dSX + dwidth;
		m_dEY = m_dSY - dstrlen;
	}
	else{
		m_dEX = m_dSX + dstrlen;
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
		pcData = m_pSortSWakVect[idx];
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
	char	paperpath[FNAMEMAX];
	char	wakuname[FNAMEMAX];
	char	filename[FNAMEMAX];
	int		ret;
	int		srchidx, slctidx;

	wakuenvinit(m_pcView->m_szInfoEnv);
	wakuenvgetpaperelement();
	wakuenvgetfullpaperpath(paperpath);
	wakuenvgetselectwakuidx(&slctidx);

	wakuenvlistgetwakuname(slctidx, wakuname);

	pDialogTextInput = new CDialogTextInput(m_pcView);
	pDialogTextInput->m_nIMEMode = ON;
	pDialogTextInput->m_csText = _T(wakuname);
INPUTWAKU:
	ret = pDialogTextInput->DoModal();
	if(ret == IDOK){
		strcpy(wakuname, pDialogTextInput->m_csText.GetBuffer(RECMAX));
		baseaddsafix(wakuname, "wak");
		sprintf(filename, "%s\\%s", paperpath, wakuname);

		srchidx = wakuenvlistsrchwakuname(wakuname);
		if(srchidx != slctidx && srchidx != -1){
			ret = AfxMessageBox("“¯ˆê–¼Ì‚Ì˜g‚ª‘¶Ý‚µ‚Ü‚·", MB_OKCANCEL, -1);
			if(ret == IDOK){
				slctidx = srchidx;
				goto INPUTWAKU;
			}
			delete(pDialogTextInput);
			return;
		}

		m_pcDoc->WakuSave(filename);
		basechangesafix(filename, "env");
		wakuenvsaveoutputenv(filename);
		basechangesafix(filename, "bin");
		m_pcView->PrintEnvSave(filename);

		wakuenvlistaddwakuname(slctidx, wakuname);
	}
	delete(pDialogTextInput);
}

void CCmdWaku::LoadCrtSlctWaku()
{
	char	paperpath[FNAMEMAX];
	char	wakuname[FNAMEMAX];
	char	filename[FNAMEMAX];
	int		slctidx;
	int		cnt, ret;

	wakuenvgetfullpaperpath(paperpath);
	wakuenvgetselectwakuidx(&slctidx);
	if(slctidx < 0){
		return;
	}
	wakuenvlistgetwakuname(slctidx, wakuname);
	m_pcDoc->SetTitle(wakuname);
	sprintf(filename, "%s\\%s", paperpath, wakuname);
	m_pcDoc->m_nSaveWaku = OFF;
	cnt = m_pcDoc->m_pcObjDataList->GetCount();
	if(cnt != 0){
		ret = AfxMessageBox("Œ»Ý‚Ì˜g‚ðíœ‚µ‚Ü‚·‚©", MB_OKCANCEL);
		if(ret == IDOK){
			m_pcDoc->DataAllCls();
		}
		else{
			m_pcDoc->m_nSaveWaku = ON;
			m_pcDoc->OffFlag(ACTIVEFLAG);
		}
	}
	DBL		xsize, ysize;
	basechangesafix(filename, "env");
    if((_access(filename, 0 )) == 0){
		wakuenvloadoutputenv(filename);
		scrngetmmsize(&xsize, &ysize);
	}
	m_pcView->WorkSizeReset();
	m_pcView->WorkAreaReset();
	scrngetmmsize(&xsize, &ysize);
	basechangesafix(filename, "wak");
	m_pcDoc->WakuLoad(filename);
	// m_pcDoc->OnFlag(ACTIVEFLAG);
	scrngetmmsize(&xsize, &ysize);

	basechangesafix(filename, "bin");
    if((_access(filename, 0 )) == 0){
		m_pcView->PrintEnvLoad(filename);
	}
	else{
		sprintf(filename, "%s\\defprn.bin", paperpath);
		m_pcView->PrintEnvLoad(filename);
	}

	scrngetmmsize(&xsize, &ysize);
	m_pcDoc->m_nSaveWaku = OFF;
}


