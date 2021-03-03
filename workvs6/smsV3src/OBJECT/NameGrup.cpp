
//
// Easy Scanner System
//
// object data.cpp  データ構造のベースクラス関係関数
//
//  Copyright (c) 1998 by THEON

#include "StdAfx.h"
#include "math.h"
#include "namegrup.h"

#define	DIVCALC	1
#define	RLTVLIMIT 32

extern "C" {
	void Dispatch();
	int scrngetnamedisp();
	int	senvchckstopflag();
	int senvchckflag(int flag);
	int	senvgetcrtpenno();
	int epsfendblock();
	int epsffillclrset(int rval, int gval, int bval);
	int epsfoutlineclrset(int rval, int gval, int bval);
	int epsfoutlinethick(DBL thick);
	int plgnfillerrorchk();
	void plgnfillerror(int flag);
	int	senvcheckdrawmode();
	int senvgetclrmode();
	int	scrnbrushchek();
	void senvgetfrclr(int *rval, int *gval, int *bval);
	void senvgetpenrgb(int pno, LPINT rval, LPINT gval, LPINT bval);
	void scrnsetclrbrushandpen(HDC PaintDC, COLORREF fillclr, COLORREF lineclr, int nthick);
	void scrnsetfigupen(HDC PaintDC);
	void scrnsetnoslctpen(HDC PaintDC);
	void scrnsetfrpen(HDC PaintDC);
	void scrnsetdelpen(HDC PaintDC);
	void scrnsetslctpen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	void scrndxymmtorltv(DBL *x, DBL *y);
	void scrndlenmmtorltv(LPDBL thick);
	int   poutpaintmodechk();
}

IMPLEMENT_SERIAL(CNameGrup, CVect, 0)
CNameGrup::CNameGrup()
{
	m_pcDataTop = NULL;
	m_szName[0] = '\0';
	m_dFontSize = 10.0;
	m_dXSpace = 2.0;
	m_dYSpace = 2.0;
	m_dCntrSize = 0.0;
}

CNameGrup::CNameGrup(int nDummy)
:CVect(nDummy)
{
	m_pcDataTop = NULL;
	m_szName[0] = '\0';
	m_dFontSize = 10.0;
	m_dXSpace = 2.0;
	m_dYSpace = 2.0;
	m_dCntrSize = 0.0;
}

CNameGrup::~CNameGrup()
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

int CNameGrup::CheckID()
{
	return(NAMEGRUPID);
}

void CNameGrup::Serialize(CArchive& ar)
{
	CString	cstr;
	int		nID;

	if (ar.IsStoring())
	{
		cstr = _T(m_szName);
		ar << cstr;
		ar << m_dFontSize;
		ar << m_dXSpace;
		ar << m_dYSpace;
		ar << m_dCntrSize;
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
		ar >> cstr;
		strcpy(m_szName, cstr.GetBuffer(RECMAX));
		ar >> m_dFontSize;
		ar >> m_dXSpace;
		ar >> m_dYSpace;
		ar >> m_dCntrSize;
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

void CNameGrup::DeleteData(CData * delData)
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

int CNameGrup::CheckNullData()
{
	CTypedPtrList<CObList,CData*>     cLnkPtrList;
	CTypedPtrList<CObList,CData*>     cDelPtrList;
	POSITION	pos;
	CData	*pcData;

	if(m_pcDataTop == NULL){
		return(TRUE);
	}

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

	if(m_pcDataTop == NULL){
		return(TRUE);
	}

	return(FALSE);
}

void CNameGrup::SetName(LPSTR lpStr)
{
	strcpy(m_szName, lpStr);
}

void CNameGrup::GetName(LPSTR lpStr)
{
	strcpy(lpStr, m_szName);
}

void CNameGrup::SetFontSize(DBL size)
{
	m_dFontSize = size;
}

void CNameGrup::GetFontSize(LPDBL size)
{
	*size = m_dFontSize;
}

void CNameGrup::SetSpace(DBL xspace, DBL yspace)
{
	m_dXSpace = xspace;
	m_dYSpace = yspace;
}

void CNameGrup::GetSpace(LPDBL xspace, LPDBL yspace)
{
	*xspace = m_dXSpace;
	*yspace = m_dYSpace;
}

void CNameGrup::SetCntrSize(DBL size)
{
	m_dCntrSize = size;
}

void CNameGrup::GetCntrSize(LPDBL size)
{
	*size = m_dCntrSize;
}

CData* CNameGrup::GetData(int linkno)
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

int CNameGrup::SetData(CData * pcSetData)
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

void CNameGrup::GetDMiniMax(DBL *minx, DBL *miny, DBL *maxx, DBL *maxy)
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

void CNameGrup::RenewMiniMax()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->RenewMiniMax();
		pcData = pcData->GetNext();
	}
}

CData * CNameGrup::CopyData(DBL movex, DBL movey)
{
	CNameGrup*	pcnewData;
	CData*		pcData;
	CData*		pcnewSetData;

	pcnewData = (CNameGrup*)NewDataCreate(NAMEGRUPID);
	pcnewData->SetName(m_szName);
	pcnewData->SetFontSize(m_dFontSize);
	pcnewData->SetSpace(m_dXSpace, m_dYSpace);
	pcnewData->SetCntrSize(m_dCntrSize);
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

void CNameGrup::MoveTrns(DBL movex, DBL movey)
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

void CNameGrup::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
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

void CNameGrup::SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
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

void CNameGrup::MirrorTrns(DBL a, DBL b, DBL t1, DBL t2)
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

void CNameGrup::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
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

void CNameGrup::StrctBoxsTrns()
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

void CNameGrup::MtrxTrns()
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

CData* CNameGrup::SrchData(DBL xx, DBL yy, DBL *retlen)
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

CData* CNameGrup::SrchIdsData(int *nIDary, int nIDcnt, DBL xx, DBL yy, DBL *retlen)
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
		pcData->SrchIdsData(nIDary, nIDcnt, xx, yy, &len);
		if(*retlen > len){
			*retlen = len;
		}
		pcData = pcData->GetNext();
	}
	return(this);
}

CData* CNameGrup::SrchDataBoxs(DBL xx, DBL yy, DBL *retlen)
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

CData* CNameGrup::SrchIdsDataBoxs(int *nIDary, int nIDcnt, DBL xx, DBL yy, DBL *retlen)
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
		pcData->SrchIdsDataBoxs(nIDary, nIDcnt, xx, yy, &len);
		if(*retlen > len){
			*retlen = len;
		}
		pcData = pcData->GetNext();
	}
	return(this);
}

CData* CNameGrup::SrchMoji(DBL xx, DBL yy, DBL *retlen, int *retlinkno)
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

CData* CNameGrup::SrchNearPoint(DBL xx, DBL yy, DBL * retlen, int * retpp)
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

void CNameGrup::PlotOut(int nComID)
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

void CNameGrup::CheckFlagDraw(int flag, CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->CheckFlagDraw(flag, pcDC);
		pcData = pcData->GetNext();
	}
}

void CNameGrup::CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->CheckFlagDrawSetClr(flag, pcDC, clr1, clr2, thick);
		pcData = pcData->GetNext();
	}
}

void CNameGrup::CheckFlagOutlineDraw(int flag, CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->CheckFlagOutlineDraw(flag, pcDC);
		pcData = pcData->GetNext();
	}
}

void CNameGrup::AtntionDraw(CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->AtntionDraw(pcDC);
		pcData = pcData->GetNext();
	}
}

void CNameGrup::NoactDraw(CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->NoactDraw(pcDC);
		pcData = pcData->GetNext();
	}
}

void CNameGrup::DelDraw(CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->DelDraw(pcDC);
		pcData = pcData->GetNext();
	}
}

void CNameGrup::OnDraw(CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->OnDraw(pcDC);
		pcData = pcData->GetNext();
	}
	if(scrngetnamedisp() == ON){
		CntrDraw(pcDC);
		WakuDraw(pcDC);
		NameDraw(pcDC);
	}
}

void CNameGrup::CntrDraw(CDC *pcDC)
{
	DBL		sx, sy;
	DBL		ex, ey;
	DBL		cx, cy;
	CPen	*pcPen;
	CPen	*pcoldPen;
	DBL		x1, y1, x2, y2;

	if(m_dCntrSize == 0.0){
		return;
	}
	pcPen = new CPen(PS_SOLID, 1, RGB(255, 0, 0));
	pcoldPen = pcDC->SelectObject(pcPen);

	GetDMiniMax(&sx, &sy, &ex, &ey);
	cx = (sx + ex) / 2.0;
	cy = (sy + ey) / 2.0;
	x1 = cx;
	x2 = cx;
	y1 = sy - m_dCntrSize * 2;
	y2 = y1 + m_dCntrSize;
	scrndxymmtorltv(&x1, &y1);
	scrndxymmtorltv(&x2, &y2);
	pcDC->MoveTo((int)x1, (int)y1);
	pcDC->LineTo((int)x2, (int)y2);

	y1 = ey + m_dCntrSize * 2;
	y2 = y1 - m_dCntrSize;
	scrndxymmtorltv(&x1, &y1);
	scrndxymmtorltv(&x2, &y2);
	pcDC->MoveTo((int)x1, (int)y1);
	pcDC->LineTo((int)x2, (int)y2);

	y1 = cy;
	y2 = cy;
	x1 = sx - m_dCntrSize * 2;
	x2 = x1 + m_dCntrSize;
	scrndxymmtorltv(&x1, &y1);
	scrndxymmtorltv(&x2, &y2);
	pcDC->MoveTo((int)x1, (int)y1);
	pcDC->LineTo((int)x2, (int)y2);

	x1 = ex + m_dCntrSize * 2;
	x2 = x1 - m_dCntrSize;
	scrndxymmtorltv(&x1, &y1);
	scrndxymmtorltv(&x2, &y2);
	pcDC->MoveTo((int)x1, (int)y1);
	pcDC->LineTo((int)x2, (int)y2);

	pcPen = pcDC->SelectObject(pcoldPen);
	delete(pcPen);
}

void CNameGrup::WakuDraw(CDC *pcDC)
{
	int		rval, gval, bval;
	DBL		sx, sy;
	DBL		ex, ey;
	CPen	*pcPen;
	CPen	*pcoldPen;

	senvgetfrclr(&rval, &gval, &bval);
	pcPen = new CPen(PS_SOLID, 1, RGB(rval, gval, bval));
	pcoldPen = pcDC->SelectObject(pcPen);

	GetDMiniMax(&sx, &sy, &ex, &ey);
	if(m_dCntrSize != 0){
		sx -= (m_dCntrSize*2);
		sy -= (m_dCntrSize*2);
		sx += (m_dCntrSize*2);
		sy += (m_dCntrSize*2);
	}
	sx -= m_dXSpace;
	sy -= m_dYSpace;
	ex += m_dXSpace;
	ey += m_dYSpace;
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)sx, (int)ey);
	pcDC->LineTo((int)ex, (int)ey);
	pcDC->LineTo((int)ex, (int)sy);
	pcDC->LineTo((int)sx, (int)sy);

	pcPen = pcDC->SelectObject(pcoldPen);
	delete(pcPen);
}

void CNameGrup::NameDraw(CDC *pcDC)
{
	DBL		sx, sy;
	DBL		ex, ey;
	CFont	cFont;
	CFont	*pcOldFont;
	DBL		size;
	int		wd, hi;

	if(pcDC != NULL){
		size = m_dFontSize;
		scrndlenmmtorltv(&size);
		wd = (int)size; hi = (int)(size/2.0);
		cFont.CreateFont(wd,hi,2700,0,0,0,0,0,SHIFTJIS_CHARSET,0,0,0,0,"@ＭＳ 明朝");
		pcOldFont = pcDC->SelectObject(&cFont);
		if(CheckFlag(ACTIVEFLAG) == ON){
			scrnsetfrpen(pcDC->m_hDC);
			pcDC->SetTextColor(RGB(255, 0, 0));
		}
		else{
			scrnsetnoslctpen(pcDC->m_hDC);
		}
		GetDMiniMax(&sx, &sy, &ex, &ey);
		if(m_dCntrSize != 0){
			sx -= (m_dCntrSize*2);
			sy -= (m_dCntrSize*2);
			sx += (m_dCntrSize*2);
			sy += (m_dCntrSize*2);
		}
		sx -= m_dXSpace;
		sy -= m_dYSpace;
		ex += m_dXSpace;
		ey += m_dYSpace;
		scrndxymmtorltv(&sx, &sy);
		scrndxymmtorltv(&ex, &ey);
		::TextOut(pcDC->m_hDC,(int)ex-wd, (int)ey,  m_szName, strlen(m_szName));
		scrnpenend(pcDC->m_hDC);
		pcDC->SelectObject(&pcOldFont);
	}
}

CRect* CNameGrup::GetBoundingRect()
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

void CNameGrup::InAreaDataAtntionOn(DBL sx, DBL sy, DBL ex, DBL ey)
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

void CNameGrup::OffFlag(int bitflag)
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

void CNameGrup::OnFlag(int bitflag)
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

int CNameGrup::CheckFlag(int bitflag)
{
	return(CVect::CheckFlag(bitflag));
}

void CNameGrup::SetDefOutLineClr()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SetDefOutLineClr();
		pcData = pcData->GetNext();
	}
}

void CNameGrup::SetDefFillClr()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SetDefFillClr();
		pcData = pcData->GetNext();
	}
}

void CNameGrup::SaveDxfFile()
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

void CNameGrup::SaveEpsFile()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SaveEpsFile();
		pcData = pcData->GetNext();
	}
}

void CNameGrup::SetUseClrPenno(int notbl[], LPINT cnt)
{
	int		penno, i;

	penno = GetOutLinePen();
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

void CNameGrup::DataChange()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->DataChange();
		pcData = pcData->GetNext();
	}
}

void CNameGrup::DataChangeSetDataGrup(CDataGrup *pcDataGrup)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->DataChangeSetDataGrup(pcDataGrup);
		pcData = pcData->GetNext();
	}
}

void CNameGrup::StrgFiting()
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

