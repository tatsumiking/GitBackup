
//
// Easy Scanner System
//
// object datalogo.cpp  ロゴデータリンク構造格納関数
//
// Copyright (c) 1998 by THEON

#include "stdafx.h"

#include "Data.h"
#include "Vect.h"
#include "DibCData.h"
#include "DataLogo.h"

#include	"../ObjLib/Baselib.h"
#include	"../ObjLib/Senvlib.h"
#include	"../ObjLib/Scrnlib.h"
#include	"../ObjLib/Epsflib.h"
#include	"../ObjLib/Plgnfill.h"
#include	"../ObjLib/Pout.h"

#define	DIVCALC	1

IMPLEMENT_SERIAL(CDataLogo, CVect, 0)
CDataLogo::CDataLogo()
{
}

CDataLogo::CDataLogo(int nDummy)
:CVect(nDummy)
{
	m_nFillError = OFF;
	m_pcDataTop = NULL;
	m_pcDibCData = NULL;
}

CDataLogo::~CDataLogo()
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

	if(m_pcDibCData != NULL){
		delete(m_pcDibCData);
	}
	return;
}

int CDataLogo::CheckID()
{
	return(DATALOGOID);
}

void CDataLogo::Serialize(CArchive& ar)
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

		if(m_pcDibCData != NULL){
			nID = m_pcDibCData->CheckID();
			ar << nID;
			m_pcDibCData->Serialize(ar);
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

		ar >> nID;
		if(nID != NOOPDATAID){
			m_pcDibCData = (CDibCData*)NewDataCreate(nID);
			m_pcDibCData->Serialize(ar);
		}
		else{
			m_pcDibCData = NULL;
		}
	}
	CVect::Serialize(ar);
	if (!(ar.IsStoring()))
	{
		RenewMiniMax();
	}
}

void CDataLogo::ResetVectLink()
{
	CData*	pcData;
	CData*	nextData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		nextData = pcData->GetNext();
		pcData->ResetVectLink();
		pcData->SetNext(NULL);
		pcData = nextData;
	}
	m_pcDataTop = NULL;
}

//p 内部データーが空の状態であれば削除しすべてが空のときTRUEを返す
int CDataLogo::CheckNullData()
{
	CTypedPtrList<CObList,CData*>     cLnkPtrList;
	CTypedPtrList<CObList,CData*>     cDelPtrList;
	POSITION	pos;
	CData	*pcData;

	if(m_pcDibCData == NULL
	&& m_pcDataTop == NULL){
		return(TRUE);
	}
	if(m_pcDibCData != NULL){
		if(m_pcDibCData->CheckNullData() == TRUE){
			delete(m_pcDibCData);
			m_pcDibCData = NULL;
		}
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

	if(m_pcDibCData == NULL
	&& m_pcDataTop == NULL){
		return(TRUE);
	}
	return(FALSE);
}

CData* CDataLogo::GetData(int linkno)
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

int CDataLogo::SetData(CData * pcSetData)
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
		}
	}
	return(0);
}

void CDataLogo::GetDMiniMax(DBL *minx, DBL *miny, DBL *maxx, DBL *maxy)
{
	CData*	pcData;
	DBL		tminx, tminy, tmaxx, tmaxy;

	(*minx) = INTMAX;
	(*miny) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	pcData = m_pcDataTop;
	if(pcData == NULL){
		if(m_pcDibCData == NULL){
			return;
		}
		m_pcDibCData->GetDMiniMax(minx, miny, maxx, maxy);
		return;
	}
	while(1){
		if(pcData == NULL){
			break;
		}
		tminx = INTMAX; tminy = INTMAX;
		tmaxx = -INTMAX; tmaxy = -INTMAX;
		pcData->GetDMiniMax(&tminx, &tminy, &tmaxx, &tmaxy);
		(*minx) = min((*minx), tminx);
		(*miny) = min((*miny), tminy);
		(*maxx) = max((*maxx), tmaxx);
		(*maxy) = max((*maxy), tmaxy);
		pcData = pcData->GetNext();
	}
}

void CDataLogo::RenewMiniMax()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		// 助長な処理がある変更関係のRenewは見直す必要あり
		pcData->RenewMiniMax();
		pcData = pcData->GetNext();
	}
}

CData * CDataLogo::CopyData(DBL movex, DBL movey)
{
	CDataLogo*	pcnewData;
	CData*		pcData;
	CData*		pcnewSetData;

	pcnewData = (CDataLogo*)NewDataCreate(DATALOGOID);
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
	// バックビットマップのコピーも必要
	return((CData*)pcnewData);
}

void CDataLogo::MoveTrns(DBL movex, DBL movey)
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
	if(m_pcDibCData != NULL){
		m_pcDibCData->MoveTrns(movex, movey);
	}
}

void CDataLogo::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
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
	if(m_pcDibCData != NULL){
		m_pcDibCData->SizeTrns(cntrx, cntry, timex, timey);
	}
}

void CDataLogo::SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
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
	if(m_pcDibCData != NULL){
		// 
	}
}

void CDataLogo::MirrorTrns(DBL a, DBL b, DBL t1, DBL t2)
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
	if(m_pcDibCData != NULL){
		// 
	}
}

void CDataLogo::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
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
	if(m_pcDibCData != NULL){
	}
}

void CDataLogo::StrctBoxsTrns()
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
	if(m_pcDibCData != NULL){
	}
}

void CDataLogo::MtrxTrns()
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
	if(m_pcDibCData != NULL){
	}
}

CData* CDataLogo::SrchData(DBL xx, DBL yy, DBL *retlen)
{
	CData*	pcData;
	DBL		len;
	DBL		sx, sy, ex, ey;

	*retlen = INTMAX;
	pcData = m_pcDataTop;
	if(pcData == NULL){
		if(m_pcDibCData != NULL){
			m_pcDibCData->GetDMiniMax(&sx, &sy, &ex, &ey);
			len = hypot(xx-sx, yy-sy);
			if(*retlen > len){
				*retlen = len;
			}
			len = hypot(xx-ex, yy-sy);
			if(*retlen > len){
				*retlen = len;
			}
			len = hypot(xx-ex, yy-ey);
			if(*retlen > len){
				*retlen = len;
			}
			len = hypot(xx-sx, yy-ey);
			if(*retlen > len){
				*retlen = len;
			}
		}
		return(this);
	}
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SrchData(xx, yy, &len);
		if(*retlen > len){
			*retlen = len;
		}
		pcData = pcData->GetNext();
	}
	return(this);
}

void CDataLogo::PlotOut(int nComID)
{
	CData*	pcData;
	int		pno;

	baseDispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pno = GetFillPen();
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
		baseDispatch();
		if(senvchckflag(STOPFLAG) == ON){
			return;
		}
	}
}

void CDataLogo::StockClrDraw(CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		((CVect*)pcData)->PlgnDraw(pcDC);
		pcData = pcData->GetNext();
	}
}

void CDataLogo::CheckFlagDraw(int flag, CDC *pcDC)
{
	CData*	pcData;

	if(m_nFillError == OFF){
		plgnfillerror(OFF);
		plgnfillstart();
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		((CVect*)pcData)->PlgnDraw(pcDC);
		if(m_nFillError == OFF){
			plgnfillrejionstart();
			((CVect*)pcData)->FillDraw(pcDC);
			plgnfillrejionend();
		}
		pcData = pcData->GetNext();
	}
	if(plgnfillerrorchk() == OFF){
		plgnfilldraw(pcDC->m_hDC);
	}
	else{
		m_nFillError = ON;
	}
}

void CDataLogo::CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick)
{
	CData*	pcData;
	int		nthick;

	scrndlenmmtorltv(&thick);
	nthick = (int)thick;
	scrnsetclrbrushandpen(pcDC->m_hDC, clr1, clr2, nthick);

	if(m_nFillError == OFF){
		plgnfillerror(OFF);
		plgnfillstart();
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		((CVect*)pcData)->PlgnDraw(pcDC);
		if(m_nFillError == OFF){
			plgnfillrejionstart();
			((CVect*)pcData)->FillDraw(pcDC);
			plgnfillrejionend();
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

void CDataLogo::CheckFlagOutlineDraw(int flag, CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		((CVect*)pcData)->PlgnDraw(pcDC);
		pcData = pcData->GetNext();
	}
}

void CDataLogo::AtntionDraw(CDC *pcDC)
{

	scrnsetslctpen(pcDC->m_hDC);
	StockClrDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CDataLogo::NoactDraw(CDC *pcDC)
{

	scrnsetnoslctpen(pcDC->m_hDC);
	StockClrDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CDataLogo::DelDraw(CDC *pcDC)
{
	scrnsetdelpen(pcDC->m_hDC);
	StockClrDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}
void CDataLogo::OnDraw(CDC *pcDC)
{
	CGdiPlusEx *pcGpe;
	int		mode;
	CData*	pcData;
	int		penno;
	int		rval, gval, bval;
	DBL		dThick;
	int		nLimit, nThick;

	pcGpe = new CGdiPlusEx();
	pcGpe->InitGraphics(pcDC);

	mode = senvcheckdrawmode();
	if (mode == PREVIEW
	|| mode == BLACKVIEW
	|| mode == PRINTVIEW
	|| mode == DIRCTVIEW
	|| mode == METAFVIEW) {
		pcGpe->InitPath();
		pcData = m_pcDataTop;
		while (1) {
			if (pcData == NULL) {
				break;
			}
			if (senvchckstopflag() == ON) {
				break;
			}
			pcData->GdiplusFillDraw(pcGpe);
			pcData = pcData->GetNext();
		}

		penno = GetFillPen();
		if (penno != -1) {
			if (penno == 0) {
				rval = 0; gval = 0; bval = 0;
			}
			else {
				senvgetpenrgb(penno, &rval, &gval, &bval);
			}
			pcGpe->SetFillBrush(rval, gval, bval);
			pcGpe->DrawFillPath();
		}

		penno = GetOutLinePen();
		if (penno != -1) {
			if (penno == 0) {
				rval = 0; gval = 0; bval = 0;
			}
			else {
				senvgetpenrgb(penno, &rval, &gval, &bval);
			}
			nLimit = senvgetplgnlimit();
			dThick = GetOutLineWidth();
			scrndlenmmtorltv(&dThick);
			nThick = (int)dThick;
			if (nThick < nLimit) {
				nThick = nLimit;
			}
			pcGpe->SetPen(rval, gval, bval, nThick);
			pcGpe->DrawPath();
		}
	}
	else {
		if (m_pcDibCData != NULL) {
			senvgetbkclr(&rval, &gval, &bval);
			m_pcDibCData->SetRGBQuadData(0, rval, gval, bval);
			senvgetimageclr(&rval, &gval, &bval);
			m_pcDibCData->SetRGBQuadData(1, rval, gval, bval);
			m_pcDibCData->OnDraw(pcDC);
		}
		pcData = m_pcDataTop;
		while (1) {
			if (pcData == NULL) {
				break;
			}
			if (senvchckstopflag() == ON) {
				break;
			}
			pcData->GdiplusPlgnDraw(pcGpe);
			pcData = pcData->GetNext();
		}
	}
	delete(pcGpe);
}
CRect* CDataLogo::GetBoundingRect()
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

void CDataLogo::InAreaDataAtntionOn(DBL sx, DBL sy, DBL ex, DBL ey)
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

void CDataLogo::OffFlag(int bitflag)
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

void CDataLogo::OnFlag(int bitflag)
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

int CDataLogo::CheckFlag(int bitflag)
{
	return(CVect::CheckFlag(bitflag));
}

void CDataLogo::ChngFlag(int bitflag)
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

void CDataLogo::SetAtnVectFSF1fBuf(CFSF1Font * pcFSF1Font)
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

CData *CDataLogo::SrchNearOnLinePoint(DBL xx, DBL yy, DBL * retlen, DBL * retx, DBL * rety, int * retpp)
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

CData* CDataLogo::SrchNearPoint(DBL xx, DBL yy, DBL * retlen, int * retpp)
{
	CData*	pcData;
	CData*  retData;
	CData*  tpcData;
	DBL		len;
	int		pp;

	retData = NULL;
	*retlen = INTMAX;
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

void CDataLogo::DeleteData(CData * delData)
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

void CDataLogo::SaveStrgStoreFile(FILE * fp, int id)
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

void CDataLogo::LoadStrgStoreFile(FILE * fp, int id)
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

void CDataLogo::SetDibCData(CDibCData * pDibCData)
{
	if(m_pcDibCData != NULL){
		delete(m_pcDibCData);
	}
	m_pcDibCData = pDibCData;
}

void CDataLogo::PointYLegal(DBL yy)
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

void CDataLogo::PointXLegal(DBL xx)
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

void CDataLogo::AllOnPointFlag(int bitflag)
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

void CDataLogo::AllOffPointFlag(int bitflag)
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

void CDataLogo::PointPicup()
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

void CDataLogo::SetDefOutLineClr()
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

void CDataLogo::SetDefFillClr()
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

void CDataLogo::SaveDxfFile()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SaveDxfFile();
		pcData = pcData->GetNext();
	}
}

void CDataLogo::SaveEpsFile()
{
	CData*	pcData;
	int		pcnt;

	pcData = m_pcDataTop;
	if(pcData == NULL){
		return;
	}
	pcnt = GetSumPointCount();
	if(pcnt == 0){
		return;
	}
	epsfstartblock();
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

int CDataLogo::GetSumPointCount()
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

void CDataLogo::OutLineOmit(DBL mm)
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

void CDataLogo::ReverceRejion()
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

