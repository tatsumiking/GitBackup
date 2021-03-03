
//
// Easy Scanner System
//
// object datafdib.cpp  ＢＭＰイメージ塗潰構造格納関数
//
// Copyright (c) 1998 by THEON

#include "stdafx.h"
#include <math.h>
#include "datafdib.h"

#define	DIVCALC	1
#define DIVDOTMAX	256
#define	ADDCNT	4

extern "C" {
	void Dispatch();
	char diblibcreatebmpfile(LPSTR fname);
	int	senvchckstopflag();
	int senvchckflag(int flag);
	int epsfstartblock();
	int epsfendblock();
	int pdrawchkclipmode();
	void pdrawclipmode(int flag);
	void plgnfillstart();
	void plgnfillrejionstart();
	void plgnfillrejionend();
	void plgnfilldraw(HDC PaintDC);
	int senvsetdrawmode(int mode);
	int	senvcheckdrawmode();
	void senvgetpenrgb(int pno, LPINT rval, LPINT gval, LPINT bval);
	int senvgetplgnlimit();
	void scrnsetpreviewpen(HDC PaintDC, int r, int g, int b, int thick);
	void scrnsetfigupen(HDC PaintDC);
	void scrnsetdelpen(HDC PaintDC);
	void scrnsetslctpen(HDC PaintDC);
	void scrnsetnoslctpen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	void scrnsetwinarea(int sx, int sy, int ex, int ey);
	void scrngetwinarea(int *sx, int *sy, int *ex, int *ey);
	void scrnsetdisprltvmove(DBL xmove, DBL ymove);
	void scrngetdisprltvmove(LPDBL xmove, LPDBL ymove);
	void scrnsetrltvtop(DBL rltvtopx, DBL rltvtopy);
	void scrngetrltvtop(LPDBL brltvtopx, LPDBL brltvtopy);
	void scrndxymmtorltv(DBL *x, DBL *y);
	void scrndlenmmtorltv(LPDBL thick);
	int scrnrltvareachk(DBL sx, DBL sy, DBL ex, DBL ey);
	void scrnrltvareaget(LPDBL sx, LPDBL sy, LPDBL ex, LPDBL ey);
	void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey);
}

IMPLEMENT_SERIAL(CDataFDib, CVect, 0)
CDataFDib::CDataFDib()
{
}

CDataFDib::CDataFDib(int nDummy)
:CVect(nDummy)
{
	m_pcDataTop = NULL;
	m_pcDibCData = NULL;
}

CDataFDib::~CDataFDib()
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

int CDataFDib::CheckID()
{
	return(DATAFDIBID);
}

void CDataFDib::Serialize(CArchive& ar)
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
		if(nID == DIBCDATAID || nID == GRADDATAID){
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

//p 内部データーが空の状態であれば削除しすべてが空のときTRUEを返す
int CDataFDib::CheckNullData()
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

CData* CDataFDib::GetData(int linkno)
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

int CDataFDib::SetData(CData * pcSetData)
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

void CDataFDib::GetDMiniMax(DBL *minx, DBL *miny, DBL *maxx, DBL *maxy)
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

void CDataFDib::RenewMiniMax()
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
		// 助長な処理がある変更関係のRenewは見直す必要あり
		pcData->RenewMiniMax();
		m_dMinix = min(pcData->m_dMinix, m_dMinix);
		m_dMiniy = min(pcData->m_dMiniy, m_dMiniy);
		m_dMaxx = max(pcData->m_dMaxx, m_dMaxx);
		m_dMaxy = max(pcData->m_dMaxy, m_dMaxy);
		pcData = pcData->GetNext();
	}
	m_pcDibCData->RenewMiniMax();
}

CData * CDataFDib::CopyData(DBL movex, DBL movey)
{
	CDataFDib*	pcnewData;
	CData*		pcData;
	CData*		pcnewSetData;
	char		newfname[FNAMEMAX];

	pcnewData = (CDataFDib*)NewDataCreate(DATAFDIBID);
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
	if(m_pcDibCData != NULL){
		pcnewData->m_pcDibCData = (CDibCData*)NewDataCreate(DIBCDATAID);
		diblibcreatebmpfile(newfname);
		CopyFile(m_pcDibCData->m_szBmpFname, newfname, FALSE);
		(pcnewData->m_pcDibCData)->SetBmpFname(newfname);
		(pcnewData->m_pcDibCData)->DibLoad();
		(pcnewData->m_pcDibCData)->m_dDstHight = m_pcDibCData->m_dDstHight;
		(pcnewData->m_pcDibCData)->m_dDstWidth = m_pcDibCData->m_dDstWidth;
		(pcnewData->m_pcDibCData)->m_dDstSx = m_pcDibCData->m_dDstSx + movex;
		(pcnewData->m_pcDibCData)->m_dDstSy = m_pcDibCData->m_dDstSy + movey;
	}	
	pcnewData->RenewMiniMax();
	// バックビットマップのコピーも必要
	return((CData*)pcnewData);
}

void CDataFDib::MoveTrns(DBL movex, DBL movey)
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
	if(m_pcDibCData != NULL){
		m_pcDibCData->MoveTrns(movex, movey);
	}
}

void CDataFDib::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
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
	if(m_pcDibCData != NULL){
		m_pcDibCData->SizeTrns(cntrx, cntry, timex, timey);
	}
}

void CDataFDib::SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
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
		DBL	minx, miny, maxx, maxy;
		GetDMiniMax(&minx, &miny, &maxx, &maxy);
		m_pcDibCData->SetDstElement(minx, miny, maxx-minx, maxy-miny);
	}
}

void CDataFDib::MirrorTrns(DBL a, DBL b, DBL t1, DBL t2)
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
		DBL	minx, miny, maxx, maxy;
		GetDMiniMax(&minx, &miny, &maxx, &maxy);
		m_pcDibCData->SetDstElement(minx, miny, maxx-minx, maxy-miny);
	}
}

void CDataFDib::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	CData*	pcData;
//	int		mode;

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

void CDataFDib::StrctBoxsTrns()
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
		DBL	minx, miny, maxx, maxy;
		GetDMiniMax(&minx, &miny, &maxx, &maxy);
		m_pcDibCData->SetDstElement(minx, miny, maxx-minx, maxy-miny);
	}
}

void CDataFDib::MtrxTrns()
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
		DBL	minx, miny, maxx, maxy;
		GetDMiniMax(&minx, &miny, &maxx, &maxy);
		m_pcDibCData->SetDstElement(minx, miny, maxx-minx, maxy-miny);
	}
}

CData* CDataFDib::SrchData(DBL xx, DBL yy, DBL *retlen)
{
	CData*	pcData;
	DBL		len;

	*retlen = INTMAX;
	pcData = m_pcDataTop;
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

CData* CDataFDib::SrchIdsData(int *nIDary, int nIDcnt, DBL xx, DBL yy, DBL *retlen)
{
	CData*	pcData;
	CData*	pctData;
	CData*	pcretData;
	DBL		len;
	int		i, nID;

	pcretData = NULL;
	*retlen = INTMAX;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}

		nID = pcData->CheckID();
		for(i = 0; i < nIDcnt; i++){
			if(nIDary[i] == nID){
				pctData = pcData->SrchData(xx, yy, &len);
				break;
			}
		}
		if(nIDcnt == i){
			pctData = pcData->SrchIdsData(nIDary, nIDcnt, xx, yy, &len);
		}
		if(*retlen > len){
			*retlen = len;
			pcretData = pctData;
		}
		pcData = pcData->GetNext();
	}
	return(pcretData);
}

CData* CDataFDib::SrchDataBoxs(DBL xx, DBL yy, DBL *retlen)
{
	CData*	pcData;
	DBL		len;

	*retlen = INTMAX;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SrchDataBoxs(xx, yy, &len);
		if(*retlen > len){
			*retlen = len;
		}
		pcData = pcData->GetNext();
	}
	return(this);
}

CData* CDataFDib::SrchIdsDataBoxs(int *nIDary, int nIDcnt, DBL xx, DBL yy, DBL *retlen)
{
	CData*	pcData;
	CData*	pctData;
	CData*	pcretData;
	DBL		len;
	int		i, nID;

	pcretData = NULL;
	*retlen = INTMAX;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}

		nID = pcData->CheckID();
		for(i = 0; i < nIDcnt; i++){
			if(nIDary[i] == nID){
				pctData = pcData->SrchDataBoxs(xx, yy, &len);
				break;
			}
		}
		if(nIDcnt == i){
			pctData = pcData->SrchIdsDataBoxs(nIDary, nIDcnt, xx, yy, &len);
		}
		if(*retlen > len){
			*retlen = len;
			pcretData = pctData;
		}
		pcData = pcData->GetNext();
	}
	return(pcretData);
}

CData* CDataFDib::SrchMoji(DBL xx, DBL yy, DBL *retlen, int *retlinkno)
{
	CData*	pcData;
	CData*	ptData;
	CData*	pretData;
	DBL		len;
	int		linkno;

	pretData = NULL;
	*retlen = INTMAX;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		ptData = pcData->SrchMoji(xx, yy, &len, &linkno);
		if(*retlen > len){
			*retlen = len;
			*retlinkno = linkno;
			pretData = ptData;
		}
		pcData = pcData->GetNext();
	}
	return(pretData);
}

void CDataFDib::FlagCheckMojiChange(int flag)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->FlagCheckMojiChange(flag);
		pcData = pcData->GetNext();
	}
}

void CDataFDib::PlotOut(int nComID)
{
	CData*	pcData;

	Dispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->PlotOut(nComID);
		pcData = pcData->GetNext();
		Dispatch();
		if(senvchckflag(STOPFLAG) == ON){
			return;
		}
	}
}

void CDataFDib::CheckFlagDraw(int flag, CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(pcData->CheckID() == BASEVECTID){
			((CVect*)pcData)->PlgnDraw(pcDC);
			plgnfillstart();
			plgnfillrejionstart();
			((CVect*)pcData)->FillDraw(pcDC);
			plgnfillrejionend();
			plgnfilldraw(pcDC->m_hDC);
		}
		else{
			pcData->CheckFlagDraw(flag, pcDC);
		}
		pcData = pcData->GetNext();
	}
}

void CDataFDib::AtntionDraw(CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(senvchckstopflag() == ON){
			break;
		}
		if(pcData->CheckID() == BASEVECTID){
			if(pcData->CheckFlag(ATNTION1FLAG) == ON){
				scrnsetslctpen(pcDC->m_hDC);
				((CVect*)pcData)->PlgnDraw(pcDC);
				scrnpenend(pcDC->m_hDC);
			}
		}
		else{
			pcData->AtntionDraw(pcDC);
		}
		pcData = pcData->GetNext();
	}
}

void CDataFDib::NoactDraw(CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(senvchckstopflag() == ON){
			break;
		}
		if(pcData->CheckID() == BASEVECTID){
			if(pcData->CheckFlag(ATNTION1FLAG) == ON){
				scrnsetnoslctpen(pcDC->m_hDC);
				((CVect*)pcData)->PlgnDraw(pcDC);
				scrnpenend(pcDC->m_hDC);
			}
		}
		else{
			pcData->NoactDraw(pcDC);
		}
		pcData = pcData->GetNext();
	}
}

void CDataFDib::DelDraw(CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(pcData->CheckID() == BASEVECTID){
			scrnsetdelpen(pcDC->m_hDC);
			((CVect*)pcData)->PlgnDraw(pcDC);
			scrnpenend(pcDC->m_hDC);
		}
		else{
			pcData->DelDraw(pcDC);
		}
		pcData = pcData->GetNext();
	}
}

void CDataFDib::StockClrDraw(CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(senvchckstopflag() == ON){
			break;
		}
		if(pcData->CheckID() == BASEVECTID){
			if(pcData->CheckFlag(ATNTION1FLAG) == ON){
				((CVect*)pcData)->PlgnDraw(pcDC);
			}
		}
		else{
			pcData->StockClrDraw(pcDC);
		}
		pcData = pcData->GetNext();
	}
}

void CDataFDib::OutlineDraw(CDC *pcDC)
{
	CData*	pcData;
	int		penno;
	int		rval, gval, bval;
	DBL		thick;
	int		nthick;
	int		nlimit;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(pcData->CheckID() == BASEVECTID){
			penno = GetOutLinePen();
			if(penno == -1){
				return;
			}
			thick = GetOutLineWidth();
			scrndlenmmtorltv(&thick);
			nthick = (int)thick;
			nlimit = senvgetplgnlimit();
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
			((CVect*)pcData)->PlgnDraw(pcDC);
			scrnpenend(pcDC->m_hDC);
		}
		else{
			pcData->OutlineDraw(pcDC);
		}
		pcData = pcData->GetNext();
	}
}

void CDataFDib::OnDraw(CDC *pcDC)
{
	DBL		minx, miny, maxx, maxy;
	DBL		scrnsx, scrnex, scrnsy, scrney;
	int		sx, sy, ex, ey;
	CData	*pcData;
	int		flag;

	if(m_pcDibCData != NULL){
		m_pcDibCData->GetDMiniMax(&minx, &miny, &maxx, &maxy);
	}
	else{
		GetDMiniMax(&minx, &miny, &maxx, &maxy);
	}
	DBL	dsx, dsy, dex, dey;
	GetDMiniMax(&dsx, &dsy, &dex, &dey);
	minx -= 1; miny -= 1;
	maxx += 1; maxy += 1;
	scrndxymmtorltv(&minx, &miny);
	scrndxymmtorltv(&maxx, &maxy);
	scrnrltvareaget(&scrnsx, &scrnsy, &scrnex, &scrney);
	if(scrnsx > minx)	minx = scrnsx;
	if(scrnex < minx)	minx = scrnex;
	if(scrnsx > maxx)	maxx = scrnsx;
	if(scrnex < maxx)	maxx = scrnex;
	if(scrnsy > miny)	miny = scrnsy;
	if(scrney < miny)	miny = scrney;
	if(scrnsy > maxy)	maxy = scrnsy;
	if(scrney < maxy)	maxy = scrney;

	sx = (int)minx-3; sy = (int)miny+3;
	ex = (int)maxx+3; ey = (int)maxy-3;
	flag = OFF;
	if(pdrawchkclipmode() == OFF){
		//　コメントになっていた
		pcDC->SelectClipRgn(NULL);
		pcDC->BeginPath();
		pcDC->MoveTo(sx, sy);
		pcDC->LineTo(sx, ey);
		pcDC->LineTo(ex, ey);
		pcDC->LineTo(ex, sy);
		pcDC->LineTo(sx, sy);
		pcDC->EndPath();
		pcDC->SelectClipPath(RGN_XOR);
		pdrawclipmode(ON);
		flag = ON;
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(senvchckstopflag() == ON){
			break;
		}
		pcDC->BeginPath();
		pcData->OutlineDraw(pcDC); 
		pcDC->EndPath();
		pcDC->SelectClipPath(RGN_XOR);
		pcData = pcData->GetNext();
	}
	if(flag == ON){
		pdrawclipmode(OFF);
		if(m_pcDibCData != NULL){
			m_pcDibCData->FillDraw(pcDC);
		}
		//　コメントになっていた
		pcDC->SelectClipRgn(NULL);
	}
}

void CDataFDib::OnDrawMP(CDC *pcDC)
{
	DBL		minx, miny, maxx, maxy;
	DBL		scrnsx, scrnex, scrnsy, scrney;
	int		sx, sy, ex, ey;
	CData	*pcData;

	if(m_pcDibCData != NULL){
		m_pcDibCData->GetDMiniMax(&minx, &miny, &maxx, &maxy);
	}
	else{
		GetDMiniMax(&minx, &miny, &maxx, &maxy);
	}
	minx -= 1; miny -= 1;
	maxx += 1; maxy += 1;
	scrndxymmtorltv(&minx, &miny);
	scrndxymmtorltv(&maxx, &maxy);
	scrnrltvareaget(&scrnsx, &scrnsy, &scrnex, &scrney);
	if(scrnsx > minx)	minx = scrnsx;
	if(scrnex < minx)	minx = scrnex;
	if(scrnsx > maxx)	maxx = scrnsx;
	if(scrnex < maxx)	maxx = scrnex;
	if(scrnsy > miny)	miny = scrnsy;
	if(scrney < miny)	miny = scrney;
	if(scrnsy > maxy)	maxy = scrnsy;
	if(scrney < maxy)	maxy = scrney;

	sx = (int)minx-1; sy = (int)miny-1;
	ex = (int)maxx+1; ey = (int)maxy+1;

	pdrawclipmode(ON);
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(senvchckstopflag() == ON){
			break;
		}
		pcDC->BeginPath();
		pcData->OutlineDraw(pcDC); 
		pcDC->EndPath();
		pcDC->SelectClipPath(RGN_AND);
		pcData = pcData->GetNext();
	}
	pdrawclipmode(OFF);
	if(m_pcDibCData != NULL){
		m_pcDibCData->FillDraw(pcDC);
	}
}

CRect* CDataFDib::GetBoundingRect()
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

void CDataFDib::InAreaDataAtntionOn(DBL sx, DBL sy, DBL ex, DBL ey)
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

void CDataFDib::OffFlag(int bitflag)
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

void CDataFDib::OnFlag(int bitflag)
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

int CDataFDib::CheckFlag(int bitflag)
{
	return(CVect::CheckFlag(bitflag));
}

void CDataFDib::ChngFlag(int bitflag)
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

void CDataFDib::SetAtnVectFSF1fBuf(CFSF1Font * pcFSF1Font)
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

CData *CDataFDib::SrchNearOnLinePoint(DBL xx, DBL yy, DBL * retlen, DBL * retx, DBL * rety, int * retpp)
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

CData* CDataFDib::SrchNearPoint(DBL xx, DBL yy, DBL * retlen, int * retpp)
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

void CDataFDib::DeleteData(CData * delData)
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

void CDataFDib::SaveStrgStoreFile(FILE * fp, int id)
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

void CDataFDib::LoadStrgStoreFile(FILE * fp, int id)
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

void CDataFDib::SetDibCData(CDibCData * pDibCData)
{
	if(m_pcDibCData != NULL){
		delete(m_pcDibCData);
	}
	m_pcDibCData = pDibCData;
}

void CDataFDib::PointYLegal(DBL yy)
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

void CDataFDib::PointXLegal(DBL xx)
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

void CDataFDib::AllOnPointFlag(int bitflag)
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

void CDataFDib::AllOffPointFlag(int bitflag)
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

void CDataFDib::PointPicup()
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

void CDataFDib::SetDefOutLineClr()
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

void CDataFDib::SetDefFillClr()
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

void CDataFDib::SaveEpsFile()
{
	CData*	pcData;

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

void CDataFDib::ReverceRejion()
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

void CDataFDib::StrgFiting()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->StrgFiting();
		pcData = pcData->GetNext();
	}
}


