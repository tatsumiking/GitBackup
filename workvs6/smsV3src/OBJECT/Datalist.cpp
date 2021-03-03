
//
// Easy Scanner System
//
// object datalist.cpp  色単位データリンク構造格納関数
// （ベクトルデータのみをリンク、単一色）
// Copyright (c) 1998 by THEON

#include "stdafx.h"
#include <math.h>
#include "datalist.h"

#define	DIVCALC	1

extern "C" {
	void Dispatch();
	int	senvchckstopflag();
	int senvchckflag(int flag);
	int	senvgetcrtpenno();
	int epsfstartblock();
	int epsfendblock();
	int epsffillclrset(int rval, int gval, int bval);
	int epsfoutlineclrset(int rval, int gval, int bval);
	int epsfoutlinethick(DBL thick);
	int plgnfillerrorchk();
	void plgnfillerror(int flag);
	void plgnfillstart();
	void plgnfillrejionstart();
	void plgnfillrejionend();
	void plgnfilldraw(HDC PaintDC);
	int	senvcheckdrawmode();
	int senvgetclrmode();
	int	scrnbrushchek();
	void senvgetpenrgb(int pno, LPINT rval, LPINT gval, LPINT bval);
	void scrnsetclrbrushandpen(HDC PaintDC, COLORREF fillclr, COLORREF lineclr, int nthick);
	int senvgetplgnlimit();
	void scrnsetpreviewpen(HDC PaintDC, int r, int g, int b, int thick);
	void scrnsetfigupen(HDC PaintDC);
	void scrnsetdelpen(HDC PaintDC);
	void scrnsetslctpen(HDC PaintDC);
	void scrnsetnoslctpen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	void scrndxymmtorltv(DBL *x, DBL *y);
	void scrndlenmmtorltv(LPDBL thick);
	int poutpaintmodechk();
	int	poutpenoutinit(int pen);
	int	poutpenoutend();
}

IMPLEMENT_SERIAL(CDataList, CVect, 0)
CDataList::CDataList()
{
}

CDataList::CDataList(int nDummy)
:CVect(nDummy)
{
	m_nFillError = OFF;
	m_pcDataTop = NULL;
}

CDataList::~CDataList()
{
	CData*	pcData;
	CData*	nextData;
	int		idx;

	idx = 0;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		nextData = pcData->GetNext();
		delete(pcData);
		pcData = nextData;
	}
	return;
}

int CDataList::CheckID()
{
	return(DATALISTID);
}

void CDataList::Serialize(CArchive& ar)
{
	int		nID;

	if (ar.IsStoring())
	{
		if(m_pcDataTop != NULL){
			nID = m_pcDataTop->CheckID();
			ar << nID;
			m_pcDataTop->Serialize(ar);
		}
		else{
			nID = NOOPDATAID;
			ar << nID;
		}
		// ar << 
	}
	else
	{
		ar >> nID;
		if(nID != NOOPDATAID){
			m_pcDataTop = NewDataCreate(nID);
			m_pcDataTop->Serialize(ar);
		}
		else{
			m_pcDataTop = NULL;
		}
		// ar >>
	}
	CVect::Serialize(ar);	
	if (!(ar.IsStoring()))
	{
		RenewMiniMax();
	}
}

void CDataList::ResetVectLink()
{
	CData*	pcData;
	CData*	nextData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		nextData = pcData->GetNext();
		pcData->SetNext(NULL);
		pcData = nextData;
	}
	m_pcDataTop = NULL;
}

//p 内部データーが空の状態であれば削除しすべてが空のときTRUEを返す
int CDataList::CheckNullData()
{
	CTypedPtrList<CObList,CData*>     cLnkPtrList;
	CTypedPtrList<CObList,CData*>     cDelPtrList;
	POSITION	pos;
	CData	*pcData;

	if(m_pcDataTop == NULL){
		return(TRUE);
	}
	if(m_pcDataTop != NULL){
		pcData = m_pcDataTop;
		while(1){
			if(pcData == NULL){
				break;
			}
			if(pcData->CheckNullData() == TRUE){
				cDelPtrList.AddTail(pcData);
			}
			else{
				cLnkPtrList.AddTail(pcData);
			}
			pcData = pcData->GetNext();
		}

		pos = cDelPtrList.GetHeadPosition();
		while (pos != NULL)
		{
			pcData = cDelPtrList.GetNext(pos);
			if(pcData == NULL){
				break;
			}
			delete(pcData);
		}

		m_pcDataTop = NULL;
		pos = cLnkPtrList.GetHeadPosition();
		while (pos != NULL)
		{
			pcData = cLnkPtrList.GetNext(pos);
			if(pcData == NULL){
				break;
			}
			SetData(pcData);
		}
	}

	if(m_pcDataTop == NULL){
		return(TRUE);
	}
	return(FALSE);
}

CData* CDataList::GetData(int linkno)
{
	CData*	pcData;
	int		idx;

	idx = 0;
	pcData = m_pcDataTop;
	while(1){
		if(idx == linkno){
			return(pcData);
		}
		if(pcData == NULL){
			break;
		}
		pcData = pcData->GetNext();
		idx++;
	}
	return(NULL);
}

int CDataList::SetData(CData * pcSetData)
{
	CData*	pcData;
	CData*	pcPreData;
	int		idx;

	idx = 0;
	pcData = m_pcDataTop;
	if(pcData == NULL){
		m_pcDataTop = pcSetData;
		return(idx);
	}
	while(1){
		idx++;
		pcPreData = pcData;
		pcData = pcPreData->GetNext();
		if(pcData == NULL){
			pcPreData->SetNext(pcSetData);
			return(idx);
			break;
		}
	}
	return(0);
}

void CDataList::GetDMiniMax(DBL *minx, DBL *miny, DBL *maxx, DBL *maxy)
{
	CData*	pcData;
	DBL		tminx, tminy, tmaxx, tmaxy;

	(*minx) = INTMAX;
	(*miny) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->GetDMiniMax(&tminx, &tminy, &tmaxx, &tmaxy);
		(*minx) = min((*minx), tminx);
		(*miny) = min((*miny), tminy);
		(*maxx) = max((*maxx), tmaxx);
		(*maxy) = max((*maxy), tmaxy);
		pcData = pcData->GetNext();
	}
}

void CDataList::RenewMiniMax()
{
	CData*	pcData;

	m_dMinix = INTMAX;
	m_dMiniy = INTMAX;
	m_dMaxx = -INTMAX;
	m_dMaxy = -INTMAX;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->RenewMiniMax();
		m_dMinix = min(pcData->m_dMinix, m_dMinix);
		m_dMiniy = min(pcData->m_dMiniy, m_dMiniy);
		m_dMaxx = max(pcData->m_dMaxx, m_dMaxx);
		m_dMaxy = max(pcData->m_dMaxy, m_dMaxy);
		pcData = pcData->GetNext();
	}
}

CData * CDataList::CopyData(DBL movex, DBL movey)
{
	CDataList*	pcnewData;
	CData*		pcData;
	CData*		pcnewSetData;

	pcnewData = (CDataList*)NewDataCreate(DATALISTID);
	pcnewData->SetFillPen(m_nFillPen);
	pcnewData->SetOutLinePen(m_nOutLinePen);
	pcnewData->SetOutLineWidth(m_dOutLineWidth);

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcnewSetData = pcData->CopyData(movex, movey);
		pcnewData->SetData(pcnewSetData);
		pcData = pcData->GetNext();
	}
	pcnewData->RenewMiniMax();
	return((CData*)pcnewData);
}

void CDataList::MoveTrns(DBL movex, DBL movey)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->MoveTrns(movex, movey);
		pcData = pcData->GetNext();
	}
	RenewMiniMax();
}

void CDataList::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SizeTrns(cntrx, cntry, timex, timey);
		pcData = pcData->GetNext();
	}
	RenewMiniMax();
}

void CDataList::SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SlopTrns(cntrx, cntry, ddx, ddy);
		pcData = pcData->GetNext();
	}
	RenewMiniMax();
}

void CDataList::MirrorTrns(DBL a, DBL b, DBL t1, DBL t2)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->MirrorTrns(a, b, t1, t2);
		pcData = pcData->GetNext();
	}
	RenewMiniMax();
}

void CDataList::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->RoundTrns(cntrx, cntry, tsin, tcos);
		pcData = pcData->GetNext();
	}
	RenewMiniMax();
}

void CDataList::StrctBoxsTrns()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->StrctBoxsTrns();
		pcData = pcData->GetNext();
	}
	RenewMiniMax();
}

void CDataList::MtrxTrns()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->MtrxTrns();
		pcData = pcData->GetNext();
	}
	RenewMiniMax();
}

CData* CDataList::SrchData(DBL xx, DBL yy, DBL *retlen)
{
	CData*	pcData;
	DBL		len;

	*retlen = INTMAX;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		len = INTMAX;
		pcData->SrchData(xx, yy, &len);
		if(*retlen > len){
			*retlen = len;
		}
		pcData = pcData->GetNext();
	}
	return(this);
}

CData* CDataList::SrchDataBoxs(DBL xx, DBL yy, DBL *retlen)
{
	CData*	pcData;
	DBL		len;

	*retlen = INTMAX;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		len = INTMAX;
		pcData->SrchDataBoxs(xx, yy, &len);
		if(*retlen > len){
			*retlen = len;
		}
		pcData = pcData->GetNext();
	}
	return(this);
}

void CDataList::PlotOut(int nComID)
{
	CData*	pcData;
	int		pno;

	Dispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}
 
	// ペン番号を設定
	pno = GetFillPen();
	if(poutpaintmodechk() == ON){
		if(pno != -1){
			poutpenoutinit(pno);
		}
	}

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(poutpaintmodechk() == ON){
			if(pcData->CheckFlag(REJIONFLAG) == ON && pno != -1){
				pcData->PlotOut(nComID);
			}
		}
		else{
			if(pcData->CheckFlag(REJIONFLAG) == OFF || pno == -1){
				pcData->PlotOut(nComID);
			}
		}

		pcData = pcData->GetNext();
		Dispatch();
		if(senvchckflag(STOPFLAG) == ON){
			return;
		}
	}

	if(poutpaintmodechk() == ON){
		if(pno != -1){
			poutpenoutend();
		}
	}
}

void CDataList::StockClrDraw(CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(pcData->CheckID() == RULEVECTID){
			((CRuleVect*)pcData)->VectPlgnDraw(pcDC);
		}
		else{
			pcData->PlgnDraw(pcDC);
		}
		pcData = pcData->GetNext();
	}
}

void CDataList::CheckFlagDraw(int flag, CDC *pcDC)
{
	CData*	pcData;
	int		penno;

	penno = GetFillPen();
	if(penno != -1
	&& m_nFillError == OFF){
		plgnfillerror(OFF);
		plgnfillstart();
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->PlgnDraw(pcDC);
		if(penno != -1
		&& m_nFillError == OFF){
			plgnfillrejionstart();
			pcData->FillDraw(pcDC);
			plgnfillrejionend();
		}
		pcData = pcData->GetNext();
	}
	if(penno != -1
	&& m_nFillError == OFF){
		if(plgnfillerrorchk() == OFF){
			plgnfilldraw(pcDC->m_hDC);
		}
		else{
			m_nFillError = ON;
		}
	}
}

void CDataList::CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick)
{
	CData*	pcData;
	DBL		dthick;
	int		nthick;
	int		penno;

	penno = GetFillPen();
	dthick = GetOutLineWidth();
	if(clr1 != clr2){
		dthick = dthick - thick;
	}
	else{
		dthick = dthick + thick;
	}
	if(dthick > 0){
		scrndlenmmtorltv(&dthick);
		nthick = (int)dthick;
		scrnsetclrbrushandpen(pcDC->m_hDC, clr1, clr1, nthick);
	}
	else{
		dthick = fabs(dthick);
		scrndlenmmtorltv(&dthick);
		nthick = (int)dthick;
		scrnsetclrbrushandpen(pcDC->m_hDC, clr1, clr2, nthick);
	}

	if(penno != -1
	&& m_nFillError == OFF){
		plgnfillerror(OFF);
		plgnfillstart();
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->PlgnDraw(pcDC);
		if(penno != -1
		&& m_nFillError == OFF){
			plgnfillrejionstart();
			pcData->FillDraw(pcDC);
			plgnfillrejionend();
		}
		pcData = pcData->GetNext();
	}
	if(penno != -1
	&& m_nFillError == OFF){
		if(plgnfillerrorchk() == OFF){
			plgnfilldraw(pcDC->m_hDC);
		}
		else{
			m_nFillError = ON;
		}
	}
	scrnpenend(pcDC->m_hDC);
}

void CDataList::CheckFlagOutlineDraw(int flag, CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->PlgnDraw(pcDC);
		pcData = pcData->GetNext();
	}

	// 本当はバグ
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(pcData->CheckID() != BASEVECTID){
			pcData->CheckFlagOutlineDraw(flag, pcDC);
		}
		pcData = pcData->GetNext();
	}
}

void CDataList::AtntionDraw(CDC *pcDC)
{
	CData*	pcData;

	scrnsetslctpen(pcDC->m_hDC);
	StockClrDraw(pcDC);
	scrnpenend(pcDC->m_hDC);

	// 本当はバグ
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(senvchckstopflag() == ON){
			break;
		}
		if(pcData->CheckID() != BASEVECTID
		&& pcData->CheckID() != RULEVECTID){
			pcData->AtntionDraw(pcDC);
		}
		pcData = pcData->GetNext();
	}
}

void CDataList::NoactDraw(CDC *pcDC)
{
	CData*	pcData;

	scrnsetnoslctpen(pcDC->m_hDC);
	StockClrDraw(pcDC);
	scrnpenend(pcDC->m_hDC);

	// 本当はバグ
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(senvchckstopflag() == ON){
			break;
		}
		if(pcData->CheckID() != BASEVECTID
		&& pcData->CheckID() != RULEVECTID){
			pcData->NoactDraw(pcDC);
		}
		pcData = pcData->GetNext();
	}
}

void CDataList::DelDraw(CDC *pcDC)
{
	CData*	pcData;

	scrnsetdelpen(pcDC->m_hDC);
	StockClrDraw(pcDC);
	scrnpenend(pcDC->m_hDC);

	// 本当はバグ
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(pcData->CheckID() != BASEVECTID){
			pcData->DelDraw(pcDC);
		}
		pcData = pcData->GetNext();
	}
}

void CDataList::OnDraw(CDC *pcDC)
{
	CData*	pcData;
	int		penno;
	int		rval, gval, bval;
	DBL		thick;
	int		nthick;
	int		nlimit;
	int		mode, crtno, penno1, penno2;

	nlimit = senvgetplgnlimit();
	mode = senvcheckdrawmode();
	crtno = senvgetcrtpenno();
	if(mode == PRINTVIEW && crtno != -1){
		penno1 = GetFillPen();
		penno2 = GetOutLinePen();
		if(penno1 != crtno && penno2 != crtno){
			return;
		}
	}

	if(mode == PRINTVIEW
	|| mode == PREVIEW){
		OnDrawOutPrint(pcDC);
		return;
	}

	penno = GetFillPen();
	if(penno != -1
	&& m_nFillError == OFF){
		plgnfillerror(OFF);
		mode = senvcheckdrawmode(); 
		if(mode == BLACKVIEW
		|| mode == DIRCTVIEW){
			if(penno == 0){
				rval = 0; gval = 0; bval = 0;
			}
			else{
				senvgetpenrgb(penno, &rval, &gval, &bval);
			}
			scrnsetpreviewpen(pcDC->m_hDC, rval, gval, bval, nlimit);
			pcData = m_pcDataTop;
			plgnfillstart();
			while(1){
				if(pcData == NULL){
					break;
				}
				if(senvchckstopflag() == ON){
					break;
				}
				plgnfillrejionstart();	
				pcData->FillDraw(pcDC);
				plgnfillrejionend();
				pcData = pcData->GetNext();
			}
			if(plgnfillerrorchk() == OFF){
				plgnfilldraw(pcDC->m_hDC);
			}
			else{
				m_nFillError = ON;
			}
			scrnpenend(pcDC->m_hDC);
		}
	}

	penno = GetOutLinePen();
	if(penno != -1){
		mode = senvcheckdrawmode(); 
		if(mode == BLACKVIEW
		|| mode == DIRCTVIEW){
			thick = GetOutLineWidth();
			if(thick == 0){
				return;
			}
			scrndlenmmtorltv(&thick);
			nthick = (int)thick;
			if(nthick < nlimit){
				nthick = nlimit;
			}
			if(penno == 0){
				rval = 0; gval = 0; bval = 0;
			}
			else{
				senvgetpenrgb(penno, &rval, &gval, &bval);
			}
			scrnsetpreviewpen(pcDC->m_hDC, rval, gval, bval, nthick);
		}
		else{
			scrnsetfigupen(pcDC->m_hDC);
		}
		pcData = m_pcDataTop;
		while(1){
			if(pcData == NULL){
				break;
			}
			if(senvchckstopflag() == ON){
				break;
			}
			pcData->PlgnDraw(pcDC);
			pcData = pcData->GetNext();
		}
		scrnpenend(pcDC->m_hDC);
	}
	/*
	// 本当はバグ
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(senvchckstopflag() == ON){
			break;
		}
		if(pcData->CheckID() != BASEVECTID){
			pcData->OnDraw(pcDC);
		}
		pcData = pcData->GetNext();
	}
	*/
}

void CDataList::OnDrawOutPrint(CDC *pcDC)
{
	CData*	pcData;
	int		penno, fillpenno;
	int		rval, gval, bval;
	COLORREF	fillclr;
	DBL		thick;
	int		nthick;
	int		mode;
	int		crtno;
	int		nlimit;

	nlimit = senvgetplgnlimit();
	crtno = senvgetcrtpenno();
	fillpenno = GetFillPen();
	if(fillpenno != -1
	&& (crtno == fillpenno || crtno == -1)
	&& m_nFillError == OFF){
		plgnfillerror(OFF);
		if(fillpenno == 0){
			rval = 0; gval = 0; bval = 0;
		}
		else{
			senvgetpenrgb(fillpenno, &rval, &gval, &bval);
		}
		if(senvgetclrmode() == BLACK){
			rval = 0;
			gval = 0;
			bval = 0;
		}
		fillclr = RGB(rval, gval, bval);
		scrnsetclrbrushandpen(pcDC->m_hDC, fillclr, -1, 0);
		pcData = (CData*)m_pcDataTop;
		plgnfillstart();
		while(1){
			if(pcData == NULL){
				break;
			}
			if(senvchckstopflag() == ON){
				break;
			}
			if(((CVect*)pcData)->CheckFlag(REJIONMASK) == ON){
				plgnfillrejionstart();	
				pcData->FillDraw(pcDC);
				plgnfillrejionend();
			}
			else{
				pcData->PlgnDraw(pcDC);
			}
			pcData = pcData->GetNext();
		}
		if(plgnfillerrorchk() == OFF){
			plgnfilldraw(pcDC->m_hDC);
		}
		else{
			m_nFillError = ON;
		}
		scrnpenend(pcDC->m_hDC);
	}

	penno = GetOutLinePen();
	thick = GetOutLineWidth();
	if(thick != 0 && penno != -1
	&& (crtno == penno || crtno == -1)){
		mode = senvcheckdrawmode(); 
		thick = GetOutLineWidth();
		scrndlenmmtorltv(&thick);
		nthick = (int)thick;
		if(nthick < nlimit){
			nthick = nlimit;
		}
		if(penno == 0){
			rval = 0; gval = 0; bval = 0;
		}
		else{
			senvgetpenrgb(penno, &rval, &gval, &bval);
		}
		if(senvgetclrmode() == BLACK){
			rval = 0;
			gval = 0;
			bval = 0;
		}
		scrnsetpreviewpen(pcDC->m_hDC, rval, gval, bval, nthick);
		pcData = m_pcDataTop;
		while(1){
			if(pcData == NULL){
				break;
			}
			if(senvchckstopflag() == ON){
				break;
			}
			pcData->PlgnDraw(pcDC);
			pcData = pcData->GetNext();
		}
		scrnpenend(pcDC->m_hDC);
	}
}

void CDataList::OutlineDraw(CDC *pcDC)
{
	CData*	pcData;
	int		penno;
	int		rval, gval, bval;
	DBL		thick;
	int		nthick;
	int		nlimit;

	nlimit = senvgetplgnlimit();
	penno = GetOutLinePen();
	if(penno == -1){
		return;
	}
	thick = GetOutLineWidth();
	scrndlenmmtorltv(&thick);
	nthick = (int)thick;
	if(nthick < nlimit){
		nthick = nlimit;
	}
	if(penno == 0){
		rval = 0; gval = 0; bval = 0;
	}
	else{
		senvgetpenrgb(penno, &rval, &gval, &bval);
	}
	if(senvgetclrmode() == BLACK){
		rval = 0;
		gval = 0;
		bval = 0;
	}
	scrnsetpreviewpen(pcDC->m_hDC, rval, gval, bval, nthick);

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->PlgnDraw(pcDC);
		pcData = pcData->GetNext();
	}
	scrnpenend(pcDC->m_hDC);

	// 本当はバグ
	/*
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(pcData->CheckID() != BASEVECTID){
			pcData->OutlineDraw(pcDC);
		}
		pcData = pcData->GetNext();
	}
	*/
}

CRect* CDataList::GetBoundingRect()
{
	DBL	dminx, dminy, dmaxx, dmaxy;
	int	nminx, nminy, nmaxx, nmaxy;

	GetDMiniMax(&dminx, &dminy, &dmaxx, &dmaxy);
	scrndxymmtorltv(&dminx, &dminy);
	scrndxymmtorltv(&dmaxx, &dmaxy);
	nminx = (int)dminx;
	nminy = (int)dminy;
	nmaxx = (int)dmaxx;
	nmaxy = (int)dmaxy;
	m_rectData.SetRect(nminx, nminy, nmaxx, nmaxy);
	return(&m_rectData);
}

void CDataList::InAreaDataAtntionOn(DBL sx, DBL sy, DBL ex, DBL ey)
{
	CData*	pcData;

	if(CheckFlag(ACTIVEFLAG) == OFF){
		return;
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->InAreaDataAtntionOn(sx, sy, ex, ey);
		pcData = pcData->GetNext();
	}
}

void CDataList::OffFlag(int bitflag)
{
	CData*	pcData;

	CVect::OffFlag(bitflag);
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->OffFlag(bitflag);
		pcData = pcData->GetNext();
	}
}

void CDataList::OnFlag(int bitflag)
{
	CData*	pcData;

	CVect::OnFlag(bitflag);
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->OnFlag(bitflag);
		pcData = pcData->GetNext();
	}
}

int CDataList::CheckFlag(int bitflag)
{
	return(CVect::CheckFlag(bitflag));
}

void CDataList::ChngFlag(int bitflag)
{
	CData*	pcData;

	CVect::ChngFlag(bitflag);
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->OnFlag(bitflag);
		pcData = pcData->GetNext();
	}
}

void CDataList::SetAtnVectFSF1fBuf(CFSF1Font * pcFSF1Font)
{
	CData*	pcData;

	if(CheckFlag(ACTIVEFLAG) == OFF){
		return;
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SetAtnVectFSF1fBuf(pcFSF1Font);
		pcData = pcData->GetNext();
	}
}

CData *CDataList::SrchNearOnLinePoint(DBL xx, DBL yy, DBL * retlen, DBL * retx, DBL * rety, int * retpp)
{
	CData*	pcData;
	CData*  retData;
	CData*  tpcData;
	DBL		len;
	DBL		x, y;
	int		pp;

	retData = NULL;
	*retlen = INTMAX;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		tpcData = pcData->SrchNearOnLinePoint(xx, yy, &len, &x, &y, &pp);
		if(*retlen > len){
			*retlen = len;
			*retx = x;
			*rety = y;
			*retpp = pp;
			retData = tpcData;
		}
		pcData = pcData->GetNext();
	}
	return(retData);
}

CData* CDataList::SrchNearPoint(DBL xx, DBL yy, DBL * retlen, int * retpp)
{
	CData*	pcData;
	CData*  retData;
	CData*  tpcData;
	DBL		len;
	int		pp;

	*retlen = INTMAX;
	*retpp = 0;
	retData = NULL;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		tpcData = pcData->SrchNearPoint(xx, yy, &len, &pp);
		if(*retlen > len){
			*retlen = len;
			*retpp = pp;
			retData = tpcData;
		}
		pcData = pcData->GetNext();
	}
	return(retData);
}

void CDataList::DeleteData(CData * delData)
{
	CData*	crtData;
	CData*  preData;

	crtData = m_pcDataTop;
	if(crtData == NULL){
		return;
	}
	if(crtData == delData){
		crtData = crtData->GetNext();
		m_pcDataTop = crtData;
		return;
	}
	else{
		crtData->DeleteData(delData);
	}
	while(1){
		preData = crtData;
		crtData = crtData->GetNext();
		if(crtData == NULL){
			break;
		}
		if(crtData == delData){
			crtData = crtData->GetNext();
			preData->SetNext(crtData);
			break;
		}
		else{
			crtData->DeleteData(delData);
		}
	}
}

void CDataList::FlagOnDelData(int flag)
{
	CData*	crtData;
	CData*  preData;
	CData*  nextData;

	crtData = m_pcDataTop;
	if(crtData == NULL){
		return;
	}
	if(crtData->CheckFlag(flag) == ON){
		nextData = crtData->GetNext();
		m_pcDataTop = nextData;
		delete(crtData);
		return;
	}
	else{
		crtData->FlagOnDelData(flag);
	}
	while(1){
		preData = crtData;
		crtData = crtData->GetNext();
		if(crtData == NULL){
			break;
		}
		if(crtData->CheckFlag(flag) == ON){
			nextData = crtData->GetNext();
			preData->SetNext(nextData);
			delete(crtData);
			crtData = preData;
		}
		else{
			crtData->FlagOnDelData(flag);
		}
	}
}

void CDataList::SaveStrgStoreFile(FILE * fp, int id)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SaveStrgStoreFile(fp, id);
		pcData = pcData->GetNext();
	}
}

void CDataList::LoadStrgStoreFile(FILE * fp, int id)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->LoadStrgStoreFile(fp, id);
		pcData = pcData->GetNext();
	}
}


void CDataList::PointYLegal(DBL yy)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->PointYLegal(yy);
		pcData = pcData->GetNext();
	}
}

void CDataList::PointXLegal(DBL xx)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->PointXLegal(xx);
		pcData = pcData->GetNext();
	}
}

void CDataList::AllOnPointFlag(int bitflag)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->AllOnPointFlag(bitflag);
		pcData = pcData->GetNext();
	}
}

void CDataList::AllOffPointFlag(int bitflag)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->AllOffPointFlag(bitflag);
		pcData = pcData->GetNext();
	}
}

void CDataList::PointPicup()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->PointPicup();
		pcData = pcData->GetNext();
	}
}

void CDataList::SetDefOutLineClr()
{
	CData*	pcData;

	CVect::SetDefOutLineClr();
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SetDefOutLineClr();
		pcData = pcData->GetNext();
	}
}

void CDataList::SetDefFillClr()
{
	CData*	pcData;

	CVect::SetDefFillClr();
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SetDefFillClr();
		pcData = pcData->GetNext();
	}
}

void CDataList::SaveDxfFile()
{
	CData*	pcData;
	int		penno;
	int		rval, gval, bval;

	penno = GetFillPen();
	if(penno != -1){
		if(penno == 0){
			rval = 0; gval = 0; bval = 0;
		}
		else{
			senvgetpenrgb(penno, &rval, &gval, &bval);
		}
	}
	penno = GetOutLinePen();
	if(penno != -1){
		if(penno == 0){
			rval = 0; gval = 0; bval = 0;
		}
		else{
			senvgetpenrgb(penno, &rval, &gval, &bval);
		}
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SaveDxfFile();
		pcData = pcData->GetNext();
	}
}

void CDataList::SaveEpsFile()
{
	CData*	pcData;
	int		pcnt;
	int		penno;
	int		rval, gval, bval;
	DBL		thick;

	
	pcData = m_pcDataTop;
	if(pcData == NULL){
		return;
	}
	pcnt = GetSumPointCount();
	if(pcnt == 0){
		return;
	}
	epsfstartblock();
	penno = GetFillPen();
	if(penno != -1){
		if(penno == 0){
			rval = 0; gval = 0; bval = 0;
		}
		else{
			senvgetpenrgb(penno, &rval, &gval, &bval);
		}
		epsffillclrset(rval, gval, bval);
	}
	else{
		rval = -1; gval = -1; bval = -1;
		epsffillclrset(rval, gval, bval);
	}
	penno = GetOutLinePen();
	if(penno != -1){
		if(penno == 0){
			rval = 0; gval = 0; bval = 0;
		}
		else{
			senvgetpenrgb(penno, &rval, &gval, &bval);
		}
		epsfoutlineclrset(rval, gval, bval);
		thick = GetOutLineWidth();
		epsfoutlinethick(thick);
	}
	else{
		rval = -1; gval = -1; bval = -1;
		epsfoutlineclrset(rval, gval, bval);
		thick = 0;
		epsfoutlinethick(thick);
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SaveEpsFile();
		pcData = pcData->GetNext();
	}
	epsfendblock();
}

int CDataList::GetSumPointCount()
{
	CVect	*pcVect;
	int		cnt, sum;

	pcVect = (CVect *)m_pcDataTop;
	sum = 0;
	while(1){
		if(pcVect == NULL){
			break;
		}
		cnt = pcVect->GetPointCount();
		sum += cnt;
		pcVect = (CVect*)pcVect->GetNext();
	}
	return(cnt);
}

void CDataList::OutLineOmit(DBL mm)
{
	CData*	pcData;
	CData*	pcnewData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(pcData->CheckID() == BASEVECTID){
			pcData->OnFlag(COPYFLAG);	
		}
		else{
			pcData->OutLineOmit(mm);
		}
		pcData = pcData->GetNext();
	}

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(pcData->CheckFlag(COPYFLAG) == ON){
			pcData->OffFlag(COPYFLAG);
			pcnewData = pcData->CopyData(0.0, 0.0);
			pcnewData->OutLineOmit(mm);
			SetData(pcnewData);
		}
		pcData = pcData->GetNext();
	}

	RenewMiniMax();
}

void CDataList::LinkCopy(CDataList * pcSrcList)
{
	CData*	pcData;

	pcData = pcSrcList->m_pcDataTop;
	SetData(pcData);
}

void CDataList::ReverceRejion()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->ReverceRejion();	
		pcData = pcData->GetNext();
	}
}

void CDataList::SetUseClrPenno(int notbl[], LPINT cnt)
{
	int		penno, i;

	penno = GetOutLinePen();
	if(penno != -1){
		if(penno == 0){
			SetOutLinePen(1);
			penno = 1;
		}
		for(i = 0; i < *cnt; i++){
			if(notbl[i] == penno){
				return;
			}
		}
		notbl[i] = penno;
		i++;
		*cnt = i;
	}
	penno = GetFillPen();
	if(penno != -1){
		if(penno == 0){
			SetFillPen(1);
			penno = 1;
		}
		for(i = 0; i < *cnt; i++){
			if(notbl[i] == penno){
				return;
			}
		}
		notbl[i] = penno;
		i++;
		*cnt = i;
	}
}

void CDataList::CnvBezeToLine(int divcnt)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->CnvBezeToLine(divcnt);
		pcData = pcData->GetNext();
	}
}
