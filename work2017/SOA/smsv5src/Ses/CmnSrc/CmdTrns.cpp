
/////////////////////////////////////////////////////////////////////////////
// CCmdTrns

#include "stdafx.h"

#include "../../Object/DibCData.h"

#include "CmdBase.h"
#include "CmdTrns.h"

#include "DialogInputNum.h"
#include "DialogInputNumPair.h"
#include "Dialog2PointSize.h"

//extern "C" {
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Curvelib.h"
#include "../../ObjLib/Strcttrn.h"
#include "../../ObjLib/Dltlib.h"
//}

CCmdTrns::CCmdTrns(CScrollView* pcview)
:CCmdBase(pcview)
{
	
}

UINT CCmdTrns::ExecTrnsMirr(UINT event, DBL dx, DBL dy)
{
	CTonDoc *pcDoc;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_dSX = dx;
			m_dSY = dy;
			m_dEX = dx;
			m_dEY = dy;
			BeginDraw();
			DrawAtnLine(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			SaveUndo();
			BeginDraw();
			DrawAtnLine(m_pcDC);
			m_dEX = dx;
			m_dEY = dy;
			EndDraw();
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
			MainMirrTrns();
			m_nMouseSetCount = 0;
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnRedraw();
			
			//return(REDRAWEVENT);
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnLine(m_pcDC);
			m_dEX = dx;
			m_dEY = dy;
			DrawAtnLine(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnLine(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		break;
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnLine(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdTrns::ExecTrnsRound(UINT event, DBL dx, DBL dy)
{
	CTonDoc *pcDoc;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_dSX = dx;
			m_dSY = dy;
			SetRoundLine(dx, dy);
			BeginDraw();
			DrawRoundLine(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			SaveUndo();
			BeginDraw();
			DrawRoundLine(m_pcDC);
			SetRoundLine(dx, dy);
			EndDraw();
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
			MainRoundTrns();
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnRedraw();
			m_nMouseSetCount = 0;
			//return(REDRAWEVENT);
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawRoundLine(m_pcDC);
			SetRoundLine(dx, dy);
			DrawRoundLine(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawRoundLine(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		break;
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawRoundLine(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdTrns::ExecTrnsSizeDisp(UINT event, DBL dx, DBL dy)
{

	if(event != INITEVENT){
		return(0);
	}
	InitDataAreaBoxs();
	DispSize();
	return(ENDEVENT);
}

UINT CCmdTrns::ExecTrns2PointSizeDisp(UINT event, DBL dx, DBL dy)
{
	CVect*	retvect;
	int		retpp;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		m_dSX = 0; m_dSY = 0;
		m_dEX = 0; m_dEY = 0;
		DrawSlctBoxs(m_dSX, m_dSY);
		DrawSlctBoxs(m_dEX, m_dEY);
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			retvect = (CVect*)SrchDataPoint(dx, dy, &retpp);
			if(retvect != NULL){
				DrawSlctBoxs(m_dSX, m_dSY);
				DrawSlctBoxs(m_dEX, m_dEY);
				retvect->GetDXY(retpp, &m_dSX, &m_dSY);
				DrawSlctBoxs(m_dSX, m_dSY);
				m_nMouseSetCount = 1;
			}
		}
		else if(m_nMouseSetCount == 1){
			retvect = (CVect*)SrchDataPoint(dx, dy, &retpp);
			if(retvect != NULL){
				retvect->GetDXY(retpp, &m_dEX, &m_dEY);
				DrawSlctBoxs(m_dEX, m_dEY);
				m_nMouseSetCount = 0;
				Disp2PointSize();
			}
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdTrns::ExecTrnsSize(UINT event, DBL dx, DBL dy)
{
	CTonDoc *pcDoc;

	switch(event){
	case INITEVENT:
		InitDataAreaBoxs();
		DispSize();
		BeginDraw();
		DrawAreaBoxs(m_pcDC);
		EndDraw();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			CntrKindInit(dx, dy);
			DrawAreaBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			CntrKindMove(dx, dy);
			DrawAreaBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			CntrKindMove(dx, dy);
			DrawAreaBoxs(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 0){
			SaveUndo();
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			EndDraw();
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
			MainSizeTrns();
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnRedraw();
			m_nMouseSetCount = 0;
			return(ENDEVENT);
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			CntrKindMove(m_dBX, m_dBY);
			DrawAreaBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		break;
	case ENDEVENT:
		m_nMouseSetCount = 0;
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}


UINT CCmdTrns::ExecTrnsRound2Point(UINT event, DBL dx, DBL dy)
{
	CTonDoc *pcDoc;
	CVect*	retvect;
	int		retpp;
	DBL		xx, yy;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			retvect = (CVect*)SrchDataPoint(dx, dy, &retpp);
			if(retvect != NULL){
				retvect->GetDXY(retpp, &m_dSX, &m_dSY);

				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				xx = m_dSX; yy = m_dSY;
				scrndxymmtorltv(&xx, &yy);
				DrawBoxs(m_pcDC, xx-4, yy-4, xx+4, yy+4);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 1;
			}
		}
		else if(m_nMouseSetCount == 1){
			retvect = (CVect*)SrchDataPoint(dx, dy, &retpp);
			if(retvect != NULL){
				retvect->GetDXY(retpp, &m_dEX, &m_dEY);
				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				xx = m_dEX; yy = m_dEY;
				scrndxymmtorltv(&xx, &yy);
				DrawBoxs(m_pcDC, xx-4, yy-4, xx+4, yy+4);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 2;
			}
		}
		else{
			SaveUndo();
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
			Main2PointRoundTrns();
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnRedraw();
			m_nMouseSetCount = 0;
			//return(REDRAWEVENT);
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			xx = m_dSX; yy = m_dSY;
			scrndxymmtorltv(&xx, &yy);
			DrawBoxs(m_pcDC, xx-4, yy-4, xx+4, yy+4);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			xx = m_dEX; yy = m_dEY;
			scrndxymmtorltv(&xx, &yy);
			DrawBoxs(m_pcDC, xx-4, yy-4, xx+4, yy+4);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdTrns::ExecTrnsSize2Point(UINT event, DBL dx, DBL dy)
{
	CTonDoc *pcDoc;
	CVect*	retvect;
	int		retpp;
	DBL		xx, yy;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			retvect = (CVect*)SrchDataPoint(dx, dy, &retpp);
			if(retvect != NULL){
				retvect->GetDXY(retpp, &m_dSX, &m_dSY);
				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				xx = m_dSX; yy = m_dSY;
				scrndxymmtorltv(&xx, &yy);
				DrawBoxs(m_pcDC, xx-4, yy-4, xx+4, yy+4);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 1;
			}
		}
		else if(m_nMouseSetCount == 1){
			retvect = (CVect*)SrchDataPoint(dx, dy, &retpp);
			if(retvect != NULL){
				retvect->GetDXY(retpp, &m_dEX, &m_dEY);
				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				xx = m_dEX; yy = m_dEY;
				scrndxymmtorltv(&xx, &yy);
				DrawBoxs(m_pcDC, xx-4, yy-4, xx+4, yy+4);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 2;
			}
		}
		else{
			SaveUndo();
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
			Main2PointSizeTrns();
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnRedraw();
			m_nMouseSetCount = 0;
			//return(REDRAWEVENT);
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			xx = m_dSX; yy = m_dSY;
			scrndxymmtorltv(&xx, &yy);
			DrawBoxs(m_pcDC, xx-4, yy-4, xx+4, yy+4);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			xx = m_dEX; yy = m_dEY;
			scrndxymmtorltv(&xx, &yy);
			DrawBoxs(m_pcDC, xx-4, yy-4, xx+4, yy+4);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}


UINT CCmdTrns::ExecTrnsSlop(UINT event, DBL dx, DBL dy)
{
	CTonDoc *pcDoc;

	switch(event){
	case INITEVENT:
		InitDataAreaBoxs();
		BeginDraw();
		DrawSlopBoxs(m_pcDC);
		EndDraw();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			BeginDraw();
			DrawSlopBoxs(m_pcDC);
			SlopCntrKindInit(dx, dy);
			DrawSlopBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawSlopBoxs(m_pcDC);
			SlopCntrKindMove(dx, dy);
			DrawSlopBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawSlopBoxs(m_pcDC);
			SlopCntrKindMove(dx, dy);
			DrawSlopBoxs(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 0){
			SaveUndo();
			BeginDraw();
			DrawSlopBoxs(m_pcDC);
			EndDraw();
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
			MainSlopTrns();
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnRedraw();
			m_nMouseSetCount = 0;
			return(ENDEVENT);
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawSlopBoxs(m_pcDC);
			SlopCntrKindMove(m_dBX, m_dBY);
			DrawSlopBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		break;
	case ENDEVENT:
		BeginDraw();
		DrawSlopBoxs(m_pcDC);
		EndDraw();
		m_nMouseSetCount = 0;
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdTrns::ExecTrnsSizeTime(UINT event, DBL dx, DBL dy)
{
/*	CTonDoc*	pcDoc;
	CDialogInputNum	*pDialogInputNum;
	DBL		xtime, ytime;
	DBL		cx, cy;
	DBL		minix, miniy, maxx, maxy;

	if(event != INITEVENT){
		return(0);
	}
	DispSize();
	xtime = 1.0;
	ytime = 1.0;
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle("倍率変更(横拡大率設定)");
	pDialogInputNum->SetUnitText("倍");
	pDialogInputNum->SetTitleText("横倍率");
	pDialogInputNum->m_dInputNum = 1.0;
	if(pDialogInputNum->DoModal() == IDOK){
		xtime = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle("倍率変更(縦拡大率設定)");
	pDialogInputNum->SetUnitText("倍");
	pDialogInputNum->SetTitleText("縦倍率");
	pDialogInputNum->m_dInputNum = 1.0;
	if(pDialogInputNum->DoModal() == IDOK){
		ytime = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	SaveUndo();
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	GetDataCenter(minix, miniy, maxx, maxy, &cx, &cy);
	pcDoc->SizeTrns(cx, cy, xtime, ytime);
	ScrnAreaRedraw(minix, miniy, maxx, maxy);
	ScrnRedraw();
	return(ENDEVENT);
*/

	CTonDoc*	pcDoc;
	CDialogInputNumPair	*pDialogInputNumPair;
	DBL		xtime, ytime;
	DBL		cx, cy;
	DBL		minix, miniy, maxx, maxy;

	if(event != INITEVENT){
		return(0);
	}
	DispSize();
	xtime = 1.0;
	ytime = 1.0;

	pDialogInputNumPair = new CDialogInputNumPair(m_pcView);
	pDialogInputNumPair->SetDialogTitle(_T("倍率変更"));
	pDialogInputNumPair->SetUnitText(_T("倍"), _T("倍"));
	pDialogInputNumPair->SetTitleText(_T("横倍率"), _T("縦倍率"));
	pDialogInputNumPair->m_dInputNumX = xtime;
	pDialogInputNumPair->m_dInputNumY = ytime;
	if(pDialogInputNumPair->DoModal() == IDOK){
		xtime = pDialogInputNumPair->m_dInputNumX;
		ytime = pDialogInputNumPair->m_dInputNumY;
		delete(pDialogInputNumPair);
	}
	else{
		delete(pDialogInputNumPair);
		return(ENDEVENT);
	}

	SaveUndo();
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	GetDataCenter(minix, miniy, maxx, maxy, &cx, &cy);
	pcDoc->SizeTrns(cx, cy, xtime, ytime);
	ScrnAreaRedraw(minix, miniy, maxx, maxy);
	ScrnRedraw();
	return(ENDEVENT);
}

UINT CCmdTrns::ExecTrnsMirrorConer(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	DBL		cx, cy;
	DBL		minix, miniy, maxx, maxy;
	DBL		sx, sy, ex, ey;
	DBL		a, b, t1, t2;

	if(event != INITEVENT){
		return(0);
	}

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	GetDataCenter(minix, miniy, maxx, maxy, &cx, &cy);
	sx = cx;
	sy = miniy;
	ex = cx;
	ey = maxy;
	if(sx == ex){
		a = 100000.0;
		b = sx;
	}
	else{
		a = (ey - sy) / (ex - sx);
		b = ey - a*ex;
	}
	t1 = a * a;
	t2 = 2.0 * a;
	SaveUndo();
	pcDoc->MirrTrns(a, b, t1, t2);
	ScrnAreaRedraw(minix, miniy, maxx, maxy);
	ScrnRedraw();
	return(ENDEVENT);
}

UINT CCmdTrns::ExecTrnsRoundRag(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CDialogInputNum	*pDialogInputNum;
	DBL		rag;
	DBL		tsin, tcos;
	DBL		cx, cy;
	DBL		minix, miniy, maxx, maxy;

	if(event != INITEVENT){
		return(0);
	}
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("角度指定回転変更"));
	pDialogInputNum->SetUnitText(_T("度"));
	pDialogInputNum->SetTitleText(_T("回転角度"));
	pDialogInputNum->m_dInputNum = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		rag = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	SaveUndo();
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	GetDataCenter(minix, miniy, maxx, maxy, &cx, &cy);
	tsin = sin(-rag*M_1RAG);
	tcos = cos(-rag*M_1RAG);
	RoundTrnsData(cx, cy, tsin, tcos);
	ScrnAreaRedraw(minix, miniy, maxx, maxy);
	ScrnRedraw();
	return(ENDEVENT);
}

UINT CCmdTrns::ExecTrnsSizeX(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CDialogInputNum	*pDialogInputNum;
	DBL		xtime, ytime;
	DBL		cx, cy;
	DBL		minix, miniy, maxx, maxy;
	DBL		xsize;

	if(event != INITEVENT){
		return(0);
	}

	DispSize();
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	GetDataCenter(minix, miniy, maxx, maxy, &cx, &cy);

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("横サイズ変更(横サイズ指定)"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("横サイズ"));
	xsize = maxx-minix;
	pDialogInputNum->m_dInputNum = FLOOR100(xsize);
	if(pDialogInputNum->DoModal() == IDOK){
		xsize = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	SaveUndo();
	xtime = xsize / (maxx - minix);
	ytime = xtime;
	pcDoc->SizeTrns(cx, cy, xtime, ytime);
	ScrnAreaRedraw(minix, miniy, maxx, maxy);
	ScrnRedraw();
	return(ENDEVENT);
}

UINT CCmdTrns::ExecTrnsSizeY(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CDialogInputNum	*pDialogInputNum;
	DBL		ysize;
	DBL		xtime, ytime;
	DBL		cx, cy;
	DBL		minix, miniy, maxx, maxy;

	if(event != INITEVENT){
		return(0);
	}

	DispSize();
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	GetDataCenter(minix, miniy, maxx, maxy, &cx, &cy);

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("縦サイズ変更(縦サイズ指定)"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("縦サイズ"));
	ysize = maxy-miniy;
	pDialogInputNum->m_dInputNum = FLOOR100(ysize);
	if(pDialogInputNum->DoModal() == IDOK){
		ysize = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	SaveUndo();
	ytime = ysize / (maxy - miniy);
	xtime = ytime;
	pcDoc->SizeTrns(cx, cy, xtime, ytime);
	ScrnAreaRedraw(minix, miniy, maxx, maxy);
	ScrnRedraw();
	return(ENDEVENT);
}

UINT CCmdTrns::ExecTrnsSizeXY(UINT event, DBL dx, DBL dy)
{
/*	CTonDoc*	pcDoc;
	CDialogInputNum	*pDialogInputNum;
	DBL		xsize, ysize;
	DBL		xtime, ytime;
	DBL		cx, cy;
	DBL		minix, miniy, maxx, maxy;

	if(event != INITEVENT){
		return(0);
	}

	DispSize();
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	GetDataCenter(minix, miniy, maxx, maxy, &cx, &cy);

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("寸法サイズ変更(横サイズ指定)"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("横サイズ"));
	xsize = maxx-minix;
	pDialogInputNum->m_dInputNum = FLOOR100(xsize);
	if(pDialogInputNum->DoModal() == IDOK){
		xsize = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("寸法サイズ変更(縦サイズ指定)"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("縦サイズ"));
	ysize = maxy-miniy;
	pDialogInputNum->m_dInputNum = FLOOR100(ysize);
	if(pDialogInputNum->DoModal() == IDOK){
		ysize = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	SaveUndo();
	xtime = xsize / (maxx - minix);
	ytime = ysize / (maxy - miniy);
	pcDoc->SizeTrns(cx, cy, xtime, ytime);
	ScrnAreaRedraw(minix, miniy, maxx, maxy);
	ScrnRedraw();
	return(ENDEVENT);
*/

	CTonDoc*	pcDoc;
	CDialogInputNumPair	*pDialogInputNumPair;
	DBL		xsize, ysize;
	DBL		xtime, ytime;
	DBL		cx, cy;
	DBL		minix, miniy, maxx, maxy;

	if(event != INITEVENT){
		return(0);
	}
	DispSize();
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	GetDataCenter(minix, miniy, maxx, maxy, &cx, &cy);

	pDialogInputNumPair = new CDialogInputNumPair(m_pcView);
	pDialogInputNumPair->SetDialogTitle(_T("寸法サイズ変更"));
	pDialogInputNumPair->SetUnitText(_T("mm"), _T("mm"));
	pDialogInputNumPair->SetTitleText(_T("横サイズ"), _T("縦サイズ"));
	xsize = maxx-minix;
	ysize = maxy-miniy;
	pDialogInputNumPair->m_dInputNumX = FLOOR100(xsize);
	pDialogInputNumPair->m_dInputNumY = FLOOR100(ysize);
	if(pDialogInputNumPair->DoModal() == IDOK){
		xsize = pDialogInputNumPair->m_dInputNumX;
		ysize = pDialogInputNumPair->m_dInputNumY;
		delete(pDialogInputNumPair);
	}
	else{
		delete(pDialogInputNumPair);
		return(ENDEVENT);
	}

	SaveUndo();
	xtime = xsize / (maxx - minix);
	ytime = ysize / (maxy - miniy);
	pcDoc->SizeTrns(cx, cy, xtime, ytime);
	ScrnAreaRedraw(minix, miniy, maxx, maxy);
	ScrnRedraw();
	return(ENDEVENT);
}

UINT CCmdTrns::ExecTrnsSlopRag(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CDialogInputNum	*pDialogInputNum;
	DBL		rag;
	DBL		ddx, ddy;
	DBL		cx, cy;
	DBL		minix, miniy, maxx, maxy;

	if(event != INITEVENT){
		return(0);
	}
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("角度指定斜形変更"));
	pDialogInputNum->SetUnitText(_T("度"));
	pDialogInputNum->SetTitleText(_T("斜形角度"));
	pDialogInputNum->m_dInputNum = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		rag = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	SaveUndo();
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	GetDataCenter(minix, miniy, maxx, maxy, &cx, &cy);
	ddx = tan(rag*M_1RAG);
	ddy = 0;
	pcDoc->SlopTrns(cx, cy, ddx, ddy);
	ScrnAreaRedraw(minix, miniy, maxx, maxy);
	ScrnRedraw();
	return(ENDEVENT);
}

UINT CCmdTrns::ExecTrnsSlopRagUd(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CDialogInputNum	*pDialogInputNum;
	DBL		rag;
	DBL		ddx, ddy;
	DBL		cx, cy;
	DBL		minix, miniy, maxx, maxy;

	if(event != INITEVENT){
		return(0);
	}
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("角度指定上下斜形変更"));
	pDialogInputNum->SetUnitText(_T("度"));
	pDialogInputNum->SetTitleText(_T("斜形角度"));
	pDialogInputNum->m_dInputNum = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		rag = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	SaveUndo();
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	GetDataCenter(minix, miniy, maxx, maxy, &cx, &cy);
	ddx = 0;
	ddy = tan(rag*M_1RAG);
	pcDoc->SlopTrns(cx, cy, ddx, ddy);
	ScrnAreaRedraw(minix, miniy, maxx, maxy);
	ScrnRedraw();
	return(ENDEVENT);
}

UINT CCmdTrns::ExecTrnsArc(UINT event, DBL dx, DBL dy)
{
	DBL		retlen;
	int		retpp;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		InitCreateArcTrnsVect();
		BeginDraw();
		m_pcVect->AtntionDraw(m_pcDC);
		EndDraw();
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			retlen = INTMAX;
			m_pcVect->SrchNearPoint(dx, dy, &retlen, &retpp);
			m_nPP = retpp;
			m_pcVect->OnFlag(ATNTION1FLAG);
			m_pcVect->GetDXY(m_nPP, &m_dBX, &m_dBY);
			m_nMouseSetCount = 1;
		}
		else{
			BeginDraw();
			m_pcVect->AtntionDraw(m_pcDC);
			m_pcVect->SetDXY(m_nPP, dx, dy);
			m_pcVect->AtntionDraw(m_pcDC);
			EndDraw();
			m_pcVect->OffFlag(ATNTION1FLAG);
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcVect->AtntionDraw(m_pcDC);
			m_pcVect->SetDXY(m_nPP, dx, dy);
			m_pcVect->AtntionDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:	// 要修正
		CDC	*pcDC;
		int	mode;

		pcDC = ((CTonView*)m_pcView)->m_pcDC;
		if(pcDC != NULL){
			mode = pcDC->GetROP2();
			pcDC->SetROP2(R2_XORPEN);
			m_pcVect->AtntionDraw(pcDC);
			pcDC->SetROP2(mode);
		}
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 0){
			SaveUndo();
			m_nMouseSetCount = 2;
			ArcTrnsVectMain();
			delete(m_pcVect);
			m_nMouseSetCount = 0;
			ScrnAllRedraw();
			return(ENDEVENT);
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcVect->AtntionDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
			m_pcVect->OffFlag(ATNTION1FLAG);
		}
		break;
	case ENDEVENT:
		BeginDraw();
		m_pcVect->AtntionDraw(m_pcDC);
		EndDraw();
		m_nMouseSetCount = 0;
		delete(m_pcVect);
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdTrns::ExecTrnsBoxs(UINT event, DBL dx, DBL dy)
{
	DBL		retlen;
	int		retpp;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		InitCreateBoxsTrnsVect();
		BeginDraw();
		m_pcVect->AtntionDraw(m_pcDC);
		EndDraw();
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			retlen = INTMAX;
			m_pcVect->SrchNearPoint(dx, dy, &retlen, &retpp);
			m_nPP = retpp;
			m_pcVect->OnFlag(ATNTION1FLAG);
			m_pcVect->GetDXY(m_nPP, &m_dBX, &m_dBY);
			m_nMouseSetCount = 1;
		}
		else{
			BeginDraw();
			m_pcVect->AtntionDraw(m_pcDC);
			m_pcVect->SetDXY(m_nPP, dx, dy);
			m_pcVect->AtntionDraw(m_pcDC);
			EndDraw();
			m_pcVect->OffFlag(ATNTION1FLAG);
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcVect->AtntionDraw(m_pcDC);
			m_pcVect->SetDXY(m_nPP, dx, dy);
			m_pcVect->AtntionDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:	// 要修正
		CDC	*pcDC;
		int	mode;

		pcDC = ((CTonView*)m_pcView)->m_pcDC;
		if(pcDC != NULL){
			mode = pcDC->GetROP2();
			pcDC->SetROP2(R2_XORPEN);
			m_pcVect->AtntionDraw(pcDC);
			pcDC->SetROP2(mode);
		}
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 0){
			SaveUndo();
			m_nMouseSetCount = 2;
			BoxsTrnsVectMain();
			delete(m_pcVect);
			m_nMouseSetCount = 0;
			ScrnAllRedraw();
			return(ENDEVENT);
		}
		else if(m_nMouseSetCount == 1){
			m_nMouseSetCount = 0;
			m_pcVect->OffFlag(ATNTION1FLAG);
		}
		break;
	case ENDEVENT:
		BeginDraw();
		m_pcVect->AtntionDraw(m_pcDC);
		EndDraw();
		m_nMouseSetCount = 0;
		m_pcVect->OffFlag(ATNTION1FLAG);
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

// 内部関数
void CCmdTrns::MainMirrTrns()
{
	CTonDoc*	pcDoc;
	DBL	sx, sy, ex, ey;
	DBL	a, b, t1, t2;

	sx = m_dSX;
	sy = m_dSY;
	ex = m_dEX;
	ey = m_dEY;
	if(sx == ex){
		a = 100000.0;
		b = sx;
	}
	else{
		a = (ey - sy) / (ex - sx);
		b = ey - a*ex;
	}
	t1 = a * a;
	t2 = 2.0 * a;
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->MirrTrns(a, b, t1, t2);
}

void CCmdTrns::MainRoundTrns()
{
	DBL		rag;
	DBL		tsin, tcos;
	DBL		cx, cy;

	if(m_dSX == m_dCX && m_dSY == m_dCY){
		return;
	}
	rag = atan2(m_dCY - m_dSY, m_dCX - m_dSX);
	tsin = sin(rag);
	tcos = cos(rag);
	cx = m_dSX;
	cy = m_dSY;
	RoundTrnsData(cx, cy, tsin, tcos);
}

void CCmdTrns::RoundTrnsData(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	CTonDoc*	pcDoc;
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->RoundTrns(cntrx, cntry, tsin, tcos);
	RoundAllDibC(cntrx, cntry, tsin, tcos);
}

void CCmdTrns::RoundAllDibC(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	POSITION	pos;
	CDibCData	*pcDibCData;

	SetDataListSelectID(DIBCDATAID, ACTIVEFLAG);
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcDibCData = (CDibCData*)(m_cPtrList.GetNext(pos));
		RoundDibC(pcDibCData, cntrx, cntry, tsin, tcos);
	}
}

void CCmdTrns::RoundDibC(CDibCData *pcDibCData, DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	int		mode;
	DBL		sx, sy, ex, ey;
	DBL		size;

	if(0 < tsin && 0.7 > tcos && tcos > -0.7){
		mode = 2;
	}
	else if(0 > tsin && 0.7 > tcos && tcos > -0.7){
		mode = 3;
	}
	else if(0 > tcos && 0.7 > tsin && tsin > -0.7){
		mode = 1;
	}
	else if(0 < tcos && 0.7 > tsin && tsin > -0.7){
		mode = 0;
	}
	if(mode != 0){
		sx = pcDibCData->m_dDstSx;
		sy = pcDibCData->m_dDstSy;
		ex = sx + pcDibCData->m_dDstWidth;
		ey = sy + pcDibCData->m_dDstHight;

		if(mode == 1 || mode == 2){
			size  = pcDibCData->m_dDstWidth;
			pcDibCData->m_dDstWidth = pcDibCData->m_dDstHight;
			pcDibCData->m_dDstHight = size;
		}
		pcDibCData->SetTrnsMode(mode);
		pcDibCData->FileTrnsSave();
		pcDibCData->FileLoad();

		if(mode == 1 || mode == 2){
			sx = sx - cntrx;
			sy = sy - cntry;
			pcDibCData->m_dDstSx = sy + cntrx;
			pcDibCData->m_dDstSy = sx + cntry;
			pcDibCData->RenewMiniMax();
		}
	}
}

void CCmdTrns::MainSizeTrns()
{
	CTonDoc*	pcDoc;
	DBL		movex, movey;
	DBL		cx, cy;
	DBL		timex, timey;

	movex = m_dSX - m_dBSX;
	movey = m_dSY - m_dBSY;
	cx = m_dSX;
	cy = m_dSY;
	timex = (m_dEX - m_dSX) / (m_dBEX - m_dBSX);
	timey = (m_dEY - m_dSY) / (m_dBEY - m_dBSY);
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->MoveTrns(movex, movey);
	pcDoc->SizeTrns(cx, cy, timex, timey);
}

void CCmdTrns::MainSlopTrns()
{
	CTonDoc*	pcDoc;
	DBL		dx, dy;
	DBL		cx, cy;
	DBL		ddx, ddy;

	dx = m_dCX; dy = m_dCY;
	GetSlopElement(dx, dy, &cx, &cy, &ddx, &ddy);
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->SlopTrns(cx, cy, ddx, ddy);
}

void CCmdTrns::SetRoundLine(DBL dx, DBL dy)
{
	DBL		len;

	m_dCX = dx;
	m_dCY = dy;
	len = hypot(m_dCX - m_dSX, m_dCY - m_dSY);
	m_dEX = m_dSX + len;
	m_dEY = m_dSY;
}

void CCmdTrns::DrawRoundLine(CDC *pcDC)
{
	DBL		sx, sy;
	DBL		ex, ey;
	DBL		cx, cy;

	sx = m_dSX;
	sy = m_dSY;
	ex = m_dEX;
	ey = m_dEY;
	cx = m_dCX;
	cy = m_dCY;

	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	scrndxymmtorltv(&cx, &cy);

	scrnsetslctpen(pcDC->m_hDC);
	pcDC->MoveTo((long)sx, (long)sy);
	pcDC->LineTo((long)ex, (long)ey);
	pcDC->MoveTo((long)sx, (long)sy);
	pcDC->LineTo((long)cx, (long)cy);

	scrnpenend(pcDC->m_hDC);
}

DBL CCmdTrns::SlopCntrKindInit(DBL dx, DBL dy)
{
	DBL		x1, y1;
	DBL		x2, y2;
	DBL		x3, y3;
	DBL		x4, y4;
	DBL		len, tlen;

	m_dBX = dx;
	m_dBY = dy;

	m_dX1 = m_dSX; m_dY1 = m_dSY;
	m_dX2 = m_dSX; m_dY2 = m_dEY;
	m_dX3 = m_dEX; m_dY3 = m_dEY;
	m_dX4 = m_dEX; m_dY4 = m_dSY;
	x1 = (m_dX1+m_dX2)/2; y1 = (m_dY1+m_dY2)/2;
	x2 = (m_dX2+m_dX3)/2; y2 = (m_dY2+m_dY3)/2;
	x3 = (m_dX3+m_dX4)/2; y3 = (m_dY3+m_dY4)/2;
	x4 = (m_dX4+m_dX1)/2; y4 = (m_dY4+m_dY1)/2;

	len = INTMAX;
	tlen = hypot(x1-dx, y1-dy);
	if(len > tlen){
		m_nCntrKind = LEFTCNTR;
		len = tlen;
	}
	tlen = hypot(x2-dx, y2-dy);
	if(len > tlen){
		m_nCntrKind = CNTRDOWN;
		len = tlen;
	}
	tlen = hypot(x3-dx, y3-dy);
	if(len > tlen){
		m_nCntrKind = RIGHTCNTR;
		len = tlen;
	}
	tlen = hypot(x4-dx, y4-dy);
	if(len > tlen){
		m_nCntrKind = CNTRUP;
		len = tlen;
	}
	m_dCX = dx;
	m_dCY = dy;
	return(len);
}

void CCmdTrns::SlopCntrKindMove(DBL dx, DBL dy)
{
	DBL		x1, y1;
	DBL		x2, y2;
	DBL		x3, y3;
	DBL		x4, y4;
	DBL		cx, cy;
	DBL		ddx, ddy;


	GetSlopElement(dx, dy, &cx, &cy, &ddx, &ddy);

	x1 = m_dSX; y1 = m_dSY;
	x2 = m_dSX; y2 = m_dEY;
	x3 = m_dEX; y3 = m_dEY;
	x4 = m_dEX; y4 = m_dSY;

	SlopTrnsPoint(&x1, &y1, cx, cy, ddx, ddy);
	SlopTrnsPoint(&x2, &y2, cx, cy, ddx, ddy);
	SlopTrnsPoint(&x3, &y3, cx, cy, ddx, ddy);
	SlopTrnsPoint(&x4, &y4, cx, cy, ddx, ddy);

	m_dX1 = x1; m_dY1 = y1;
	m_dX2 = x2; m_dY2 = y2;
	m_dX3 = x3; m_dY3 = y3;
	m_dX4 = x4; m_dY4 = y4;

	m_dCX = dx; m_dCY = dy;
}

void CCmdTrns::SlopTrnsPoint(LPDBL dx, LPDBL dy, DBL cx, DBL cy, DBL ddx, DBL ddy)
{
	DBL		x, y;
	DBL		xx, yy;

	x = (*dx);
	y = (*dy);
	x = x - cx;
	y = y - cy;
	xx = x + y * ddx;
	yy = x * ddy + y;
	x = xx + cx;
	y = yy + cy;
	(*dx) = x;
	(*dy) = y;
}

void CCmdTrns::DrawSlopBoxs(CDC *pcDC)
{
	DBL		x1, y1;
	DBL		x2, y2;
	DBL		x3, y3;
	DBL		x4, y4;
	long	x, y;

	x1 = m_dX1; y1 = m_dY1;
	x2 = m_dX2; y2 = m_dY2;
	x3 = m_dX3; y3 = m_dY3;
	x4 = m_dX4; y4 = m_dY4;

	scrndxymmtorltv(&x1, &y1);
	scrndxymmtorltv(&x2, &y2);
	scrndxymmtorltv(&x3, &y3);
	scrndxymmtorltv(&x4, &y4);

	scrnsetslctpen(pcDC->m_hDC);

	x = (int)x1; y = (int)y1;
	pcDC->MoveTo(x, y);
	x = (int)x2; y = (int)y2;
 	pcDC->LineTo(x, y);
	x = (int)x3; y = (int)y3;
 	pcDC->LineTo(x, y);
	x = (int)x4; y = (int)y4;
 	pcDC->LineTo(x, y);
	x = (int)x1; y = (int)y1;
 	pcDC->LineTo(x, y);

	x1 = (m_dX1+m_dX2)/2; y1 = (m_dY1+m_dY2)/2;
	x2 = (m_dX2+m_dX3)/2; y2 = (m_dY2+m_dY3)/2;
	x3 = (m_dX3+m_dX4)/2; y3 = (m_dY3+m_dY4)/2;
	x4 = (m_dX4+m_dX1)/2; y4 = (m_dY4+m_dY1)/2;

	scrndxymmtorltv(&x1, &y1);
	scrndxymmtorltv(&x2, &y2);
	scrndxymmtorltv(&x3, &y3);
	scrndxymmtorltv(&x4, &y4);

	DrawBoxs(pcDC, x1-4, y1-4, x1+4, y1+4);
	DrawBoxs(pcDC, x2-4, y2-4, x2+4, y2+4);
	DrawBoxs(pcDC, x3-4, y3-4, x3+4, y3+4);
	DrawBoxs(pcDC, x4-4, y4-4, x4+4, y4+4);

	scrnpenend(pcDC->m_hDC);
}


void CCmdTrns::GetSlopElement(DBL dx, DBL dy, LPDBL cx, LPDBL cy, LPDBL ddx, LPDBL ddy)
{
	DBL		x1, y1;
	DBL		x2, y2;
	DBL		x3, y3;
	DBL		x4, y4;
	DBL		ragx, ragy;

	m_dX1 = m_dSX; m_dY1 = m_dSY;
	m_dX2 = m_dSX; m_dY2 = m_dEY;
	m_dX3 = m_dEX; m_dY3 = m_dEY;
	m_dX4 = m_dEX; m_dY4 = m_dSY;
	x1 = (m_dX1+m_dX2)/2; y1 = (m_dY1+m_dY2)/2;
	x2 = (m_dX2+m_dX3)/2; y2 = (m_dY2+m_dY3)/2;
	x3 = (m_dX3+m_dX4)/2; y3 = (m_dY3+m_dY4)/2;
	x4 = (m_dX4+m_dX1)/2; y4 = (m_dY4+m_dY1)/2;

	*ddx = 0; *ddy = 0;
	switch(m_nCntrKind){
	case LEFTCNTR:
		*cx = x3; *cy = y3;
		ragy = atan2(dy-y3, dx-x3);
		*ddy = tan(ragy);
		break;
	case CNTRDOWN:
		*cx = x4; *cy = y4;
		ragx = atan2(dy-y4, dx-x4)-M_PI_2;
		*ddx = -tan(ragx);
		break;
	case RIGHTCNTR:
		*cx = x1; *cy = y1;
		ragy = atan2(dy-y1, dx-x1);
		*ddy = tan(ragy);
		break;
	case CNTRUP:
		*cx = x2; *cy = y2;
		ragx = atan2(dy-y2, dx-x2)-M_PI_2;
		*ddx = -tan(ragx);
		break;
	}
}

void CCmdTrns::Main2PointSizeTrns()
{
	DBL		xlen, ylen, len;
	CDialog2PointSize*	pDialog2PointSize;
	DBL		time;
	DBL		cx, cy;
	CTonDoc*	pcDoc;

	len = hypot(m_dEX - m_dSX, m_dEY - m_dSY);
	xlen = fabs(m_dEX - m_dSX);
	ylen = fabs(m_dEY - m_dSY);

	pDialog2PointSize = new CDialog2PointSize(m_pcView);
	pDialog2PointSize->m_dLength = FLOOR100(len);
	pDialog2PointSize->m_dXSize = FLOOR100(xlen);
	pDialog2PointSize->m_dYSize = FLOOR100(ylen);
	if(pDialog2PointSize->DoModal() == IDOK){
		xlen = FLOOR100(xlen);
		ylen = FLOOR100(ylen);
		len = FLOOR100(len);
		if(pDialog2PointSize->m_dXSize != xlen){
			time = pDialog2PointSize->m_dXSize / xlen;
		}
		if(pDialog2PointSize->m_dYSize != ylen){
			time = pDialog2PointSize->m_dYSize / ylen;
		}
		if(pDialog2PointSize->m_dLength != len){
			time = pDialog2PointSize->m_dLength / len;
		}
		cx = m_dSX;
		cy = m_dSY;
		pcDoc = (CTonDoc*)(m_pcView->GetDocument());
		pcDoc->SizeTrns(cx, cy, time, time);
	}
	delete(pDialog2PointSize);
}

void CCmdTrns::Main2PointRoundTrns()
{
	DBL		rag;
	DBL		tsin, tcos;
	DBL		cx, cy;

	if(m_dSX == m_dEX && m_dSY == m_dEY){
		return;
	}
	rag = atan2(m_dEY - m_dSY, m_dEX - m_dSX);
	tsin = sin(-rag);
	tcos = cos(-rag);
	cx = m_dSX;
	cy = m_dSY;
	RoundTrnsData(cx, cy, tsin, tcos);
}

void CCmdTrns::InitCreateBoxsTrnsVect()
{
	CTonDoc*	pcDoc;
	DBL		sx, sy, ex, ey;
	DBL		cx, cy;
	int		pp;
	DBL		dx[10], dy[10];

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &sx, &sy, &ex, &ey);
	cx = (sx + ex) / 2;
	cy = (sy + ey) / 2;
	m_pcVect = new CVect(BASEVECTID);
	m_pcVect->SetAtrDXY(0, 0, sx, sy);
	m_pcVect->SetAtrDXY(1, 0, ex, sy);
	m_pcVect->SetAtrDXY(2, 0, ex, ey);
	m_pcVect->SetAtrDXY(3, 0, sx, ey);
	m_pcVect->SetPointCount(4);
	for(pp = 0; pp < 4; pp++){
		m_pcVect->GetDXY(pp, &(dx[pp]), &(dy[pp]));
	}
	strctboxsbasepointset(dx, dy);
}

void CCmdTrns::BoxsTrnsVectMain()
{
	CTonDoc*	pcDoc;
	int		pp;
	DBL		dx[10], dy[10];

	for(pp = 0; pp < 4; pp++){
		m_pcVect->GetDXY(pp, &(dx[pp]), &(dy[pp]));
	}
	strctboxstrnspointset(dx, dy);
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->StrctBoxsTrns();
}

void CCmdTrns::InitCreateArcTrnsVect()
{
	CTonDoc*	pcDoc;
	DBL		sx, sy, ex, ey;
	DBL		cx, cy;
	int		pp;
	DBL		dx[10], dy[10];

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &sx, &sy, &ex, &ey);
	m_dSX = sx; m_dSY = sy;
	m_dEX = ex; m_dEY = ey;
	cx = (sx + ex) / 2;
	cy = (sy + ey) / 2;
	m_pcVect = new CVect(BASEVECTID);
	m_pcVect->SetAtrDXY(0, CURVATR, sx, sy);
	m_pcVect->SetAtrDXY(1, CURVATR|CONTATR, cx, sy);
	m_pcVect->SetAtrDXY(2, CURVATR, ex, sy);
	m_pcVect->SetAtrDXY(3, CURVATR|CONTATR, ex, cy);
	m_pcVect->SetAtrDXY(4, CURVATR, ex, ey);
	m_pcVect->SetAtrDXY(5, CURVATR|CONTATR, cx, ey);
	m_pcVect->SetAtrDXY(6, CURVATR, sx, ey);
	m_pcVect->SetAtrDXY(7, CURVATR|CONTATR, sx, cy);
	m_pcVect->SetPointCount(8);
	for(pp = 0; pp < 8; pp++){
		m_pcVect->GetDXY(pp, &(dx[pp]), &(dy[pp]));
	}
}

void CCmdTrns::ArcTrnsVectMain()
{
	POSITION	pos;
	int		pp;
	DBL		stdx[10], stdy[10];
	int		cnt;
	CData	*pcData;

	for(pp = 0; pp < 8; pp++){
		m_pcVect->GetDXY(pp, &(stdx[pp]), &(stdy[pp]));
	}

	cnt = SetDataListSelectID(STRGVECTID, ACTIVEFLAG);
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_cPtrList.GetNext(pos);
		if(pcData == NULL){
			break;
		}
		((CStrgVect*)pcData)->DataChange();
	}

	cnt = SetDataListSelectID(BASEVECTID, ACTIVEFLAG);
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_cPtrList.GetNext(pos);
		if(pcData == NULL){
			break;
		}
		ArcTrnsVect((CVect*)pcData, stdx, stdy);
	}
}

void CCmdTrns::ArcTrnsVect(CVect *pcVect, DBL stdx[], DBL stdy[])
{
	int		pcnt, pp, dp;
	int		atr, atr1, atr2, atr3;
	DBL		x1, y1, x2, y2, x3, y3;
	DBL		dx, dy;
	CVect	*pctmpVect;

	pctmpVect = new CVect(BASEVECTID);
	pcnt = pcVect->GetPointCount();
	// 直線を３点円弧にする処理
	pcVect->GetAtrDXY(0, &atr1, &x1, &y1);
	for(pp = 1, dp = 0; pp < pcnt; pp++){
		pcVect->GetAtrDXY(pp, &atr2, &x2, &y2);
		if((atr1 & ~CONTATR) == 0){
			atr1 |= CURVATR;
			pctmpVect->SetAtrDXY(dp, atr1, x1, y1);
			dp++;
			atr3 = CURVATR | CONTATR;
			x3 = (x1 + x2) / 2.0;
			y3 = (y1 + y2) / 2.0;
			pctmpVect->SetAtrDXY(dp, atr3, x3, y3);
			dp++;
		}
		else{
			pctmpVect->SetAtrDXY(dp, atr1, x1, y1);
			dp++;
		}
		atr1 = atr2;
		x1 = x2;
		y1 = y2;
	}
	if(pcVect->CheckFlag(REJIONFLAG) == ON){
		pcVect->GetAtrDXY(0, &atr2, &x2, &y2);
		if((atr1 & ~CONTATR) == 0){
			atr1 |= CURVATR;
			pctmpVect->SetAtrDXY(dp, atr1, x1, y1);
			dp++;
			atr3 = CURVATR | CONTATR;
			x3 = (x1 + x2) / 2.0;
			y3 = (y1 + y2) / 2.0;
			pctmpVect->SetAtrDXY(dp, atr3, x3, y3);
			dp++;
		}
		else{
			pctmpVect->SetAtrDXY(dp, atr1, x1, y1);
			dp++;
		}
	}
	else{
		pctmpVect->SetAtrDXY(dp, atr1, x1, y1);
		dp++;
	}

	pcnt = dp;
	for(pp = 0; pp < pcnt; pp++){
		pctmpVect->GetAtrDXY(pp, &atr, &dx, &dy);
		ArcTrnsXY(stdx, stdy, &dx, &dy);
		pcVect->SetAtrDXY(pp, atr, dx, dy);
	}
	delete(pctmpVect);
	pcVect->SetPointCount(pcnt);
	pcVect->RenewMiniMax();
}

void CCmdTrns::ArcTrnsXY(DBL stdx[], DBL stdy[], LPDBL dx, LPDBL dy)
{
/////////////////////////////////////////////////////////////////////////////
//	P(xORy)(u,0)=p(xORy)uw[0]、P(xORy)(1,w)=p(xORy)uw[1]
//	P(xORy)(u,1)=p(xORy)uw[2]、P(xORy)(0,w)=p(xORy)uw[3]
/////////////////////////////////////////////////////////////////////////////
	DBL		hi, wd;
	DBL		x1, y1, x2, y2, x3, y3;
	DBL		px[2][2], py[2][2], pxuw[4], pyuw[4];
	DBL		tempM[3];

	wd = ((*dx) - m_dSX) / (m_dEX - m_dSX);
	hi = ((*dy) - m_dSY) / (m_dEY - m_dSY);

	px[0][0] = stdx[0];
	px[1][0] = stdx[2];
	px[1][1] = stdx[4];
	px[0][1] = stdx[6];

	py[0][0] = stdy[0];
	py[1][0] = stdy[2];
	py[1][1] = stdy[4];
	py[0][1] = stdy[6];

	x1 = stdx[0]; y1 = stdy[0];
	x2 = stdx[1]; y2 = stdy[1];
	x3 = stdx[2]; y3 = stdy[2];
	ArcTrnsGetPUW(x1, y1, x2, y2, x3, y3, wd, &(pxuw[0]), &(pyuw[0]));
	x1 = stdx[4]; y1 = stdy[4];
	x2 = stdx[5]; y2 = stdy[5];
	x3 = stdx[6]; y3 = stdy[6];
	ArcTrnsGetPUW(x1, y1, x2, y2, x3, y3, 1.0-wd, &(pxuw[2]), &(pyuw[2]));
	x1 = stdx[2]; y1 = stdy[2];
	x2 = stdx[3]; y2 = stdy[3];
	x3 = stdx[4]; y3 = stdy[4];
	ArcTrnsGetPUW(x1, y1, x2, y2, x3, y3, hi, &(pxuw[1]), &(pyuw[1]));
	x1 = stdx[6]; y1 = stdy[6];
	x2 = stdx[7]; y2 = stdy[7];
	x3 = stdx[0]; y3 = stdy[0];
	ArcTrnsGetPUW(x1, y1, x2, y2, x3, y3, 1.0-hi, &(pxuw[3]), &(pyuw[3]));

	/////////////////////////////////////////////////////////////////////////////
	////X座標の計算
	tempM[0] = ((-1.0)*px[0][0]*(1.0-hi)) + ((-1.0)*px[0][1]*hi) + (pxuw[3]);
	tempM[1] = ((-1.0)*px[1][0]*(1.0-hi)) + ((-1.0)*px[1][1]*hi) + (pxuw[1]);
	tempM[2] = (pxuw[0]*(1.0-hi)) + (pxuw[2]*hi);
	*dx = (tempM[0]*(1.0-wd)) + tempM[1]*wd + tempM[2];

	/////////////////////////////////////////////////////////////////////////////
	////Y座標の計算
	tempM[0] = ((-1.0)*py[0][0]*(1.0-hi)) + ((-1.0)*py[0][1]*hi) + (pyuw[3]);
	tempM[1] = ((-1.0)*py[1][0]*(1.0-hi)) + ((-1.0)*py[1][1]*hi) + (pyuw[1]);
	tempM[2] = (pyuw[0]*(1.0-hi)) + (pyuw[2]*hi);
	*dy = (tempM[0]*(1.0-wd)) + tempM[1]*wd + tempM[2];
}

void CCmdTrns::ArcTrnsGetPUW(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL dd, LPDBL xuw, LPDBL yuw)
{
	DBL srag, erag, dr, ox, oy, rag;
	int	stat1, stat2;

	stat1 = curve3pcenterget(x1, y1, x2, y2, x3, y3, &ox, &oy);
	if(stat1 != FAIL){
		stat2 = curve3pragrget(x1, y1, x2, y2, x3, y3, ox, oy, &srag, &erag, &dr);
	}
	if(stat1 == FAIL || stat2 == FAIL){
		*xuw = ((x3-x1) * dd) + x1;
		*yuw = ((y3-y1) * dd) + y1;
	}
	else{
		rag = ((erag-srag) * dd)+srag;
		*xuw = cos(rag)*dr+ox;
		*yuw = sin(rag)*dr+oy;
	}
}

void CCmdTrns::DispSize()
{
	CTonApp*	theApp;
	CMainFrame*	theWnd;
	TCHAR		msg[RECMAX];

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	_stprintf_s(msg, _T("横%4.1lfmm 縦%4.1lfmm"), m_dEX-m_dSX, m_dEY - m_dSY);
	theWnd->HelpTextOut(2, msg);
}

void CCmdTrns::Disp2PointSize()
{
	CTonApp*	theApp;
	CMainFrame*	theWnd;
	TCHAR		msg[RECMAX];
	DBL			len;

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	len = hypot(m_dEX-m_dSX, m_dEY-m_dSY);
	_stprintf_s(msg, _T(" 長%4.1lf x%4.1lf y%4.1lf"), len, m_dEX-m_dSX, m_dEY-m_dSY);
	theWnd->HelpTextOut(2, msg);
}

void CCmdTrns::DrawSlctBoxs(DBL dx, DBL dy)
{
	DBL		xx, yy;

	BeginDraw();
	scrnsetpointpen(m_pcDC->m_hDC);
	xx = dx; yy = dy;
	scrndxymmtorltv(&xx, &yy);
	DrawBoxs(m_pcDC, xx-4, yy-4, xx+4, yy+4);
	scrnpenend(m_pcDC->m_hDC);
	EndDraw();
}

DBL CCmdTrns::CntrKindInit(DBL dx, DBL dy)
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

	if((ex - sx) != 0 && (ey - sy) != 0){
		m_dTimexpy = 1.0;
	}
	else{
		m_dTimexpy = (ex - sx) / (ey - sy);
	}

	cx = (sx + ex) / 2;
	cy = (sy + ey) / 2;
	len = INTMAX;
	tlen = hypot(cx-dx, cy-dy);
	if(len > tlen){
		m_nCntrKind = CNTRCNTR;
		len = tlen;
	}
	tlen = hypot(sx-dx, sy-dy);
	if(len > tlen){
		m_nCntrKind = LEFTUP;
		len = tlen;
	}
	tlen = hypot(sx-dx, ey-dy);
	if(len > tlen){
		m_nCntrKind = LEFTDOWN;
		len = tlen;
	}
	tlen = hypot(ex-dx, ey-dy);
	if(len > tlen){
		m_nCntrKind = RIGHTDOWN;
		len = tlen;
	}
	tlen = hypot(ex-dx, sy-dy);
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
	tlen = hypot(cx-dx, sy-dy);
	if(len > tlen){
		m_nCntrKind = CNTRUP;
		len = tlen;
	}
	tlen = hypot(cx-dx, ey-dy);
	if(len > tlen){
		m_nCntrKind = CNTRDOWN;
		len = tlen;
	}
	m_dCX = dx;
	m_dCY = dy;
	return(len);
}

void CCmdTrns::CntrKindMove(DBL dx, DBL dy)
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
		sy += movey;
		break;
	case LEFTDOWN:
		sx += movex;
		ey += movey;
		break;
	case LEFTCNTR:
		sx += movex;
		break;
	case RIGHTUP:
		ex += movex;
		sy += movey;
		break;
	case RIGHTDOWN:
		ex += movex;
		ey += movey;
		break;
	case RIGHTCNTR:
		ex += movex;
		break;
	case CNTRUP:
		sy += movey;
		break;
	case CNTRDOWN:
		ey += movey;
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
}

