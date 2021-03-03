//
// Easy Scanner System
//
// object sptntext.cpp  線上文字列関係関数
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include <math.h>
#include "sptntext.h"

extern "C" {
	void scrndxymmtorltv(DBL *x, DBL *y);
	void scrndlenmmtorltv(DBL *len);
	void senvgetpenrgb(int pno, LPINT rval, LPINT gval, LPINT bval);
	int senvgetplgnlimit();
	void scrnsetpreviewpen(HDC PaintDC, int r, int g, int b, int thick);
	void scrnpenend(HDC PaintDC);
	int	senvchckstopflag();
	HBRUSH senvgetmainbrush();
	BOOL  pdrwatrarcchk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrbezechk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrcurvechk(int atr1);
	DBL   pdrwendragget(int atr1, int atr2, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL   pdrwstartragget(int atr0, int atr1, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2);
	int curve3pcenterget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL FAR *x0, DBL FAR *y0);
	int curve3pragrget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0, DBL FAR *rag01, DBL FAR *rag03, DBL FAR *r);
	void curvebezecontrolpointget(DBL x1, DBL y1, DBL x2, DBL y2, DBL rag, DBL *xp, DBL *yp);
	void curvenoragtocurverag(DBL *rag1, DBL *rag2);
	int curveperpendicular(DBL sx, DBL sy, DBL ex, DBL ey, DBL x, DBL y, DBL *cx, DBL *cy);
	int curveonlinechk(DBL x1, DBL y1, DBL x2, DBL y2, DBL x, DBL y);
	int senvcheckdrawmode();
	int senvgetclrmode();
}

IMPLEMENT_SERIAL(CSPtnText, CObject, 3)
CSPtnText::CSPtnText()
{
}

CSPtnText::CSPtnText(int nDummy)
	: CStrgText(nDummy)
{
	m_nSpaceKind = OFF;
	m_nRoundKind = ON;
	m_nSetMode = CNTRCNTR;
	m_dPitch = 1.0;
	m_dFlatPar = 100.0;
	m_nBold = OFF;
	m_nSlope = OFF;
	m_nOder = ODEREQUAL;
	m_pcFitingVect = NULL;
}

CSPtnText::~CSPtnText()
{
	if(m_pcFitingVect != NULL){
		delete(m_pcFitingVect);
	}
}

int CSPtnText::CheckID()
{
	return(SPTNTEXTID);
}

void CSPtnText::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nSpaceKind;
		ar << m_nRoundKind;
		ar << m_nSetMode;
		ar << m_dPitch;
		ar << m_dFlatPar;
		ar << m_nBold;
		ar << m_nSlope;
		ar << m_nOder;
	}
	else
	{
		ar >> m_nSpaceKind;
		ar >> m_nRoundKind;
		ar >> m_nSetMode;
		ar >> m_dPitch;
		ar >> m_dFlatPar;
		ar >> m_nBold;
		ar >> m_nSlope;
		ar >> m_nOder;
		m_pcFitingVect = (CVect*)NewDataCreate(BASEVECTID);
	}
	m_pcFitingVect->Serialize(ar);
	CStrgText::Serialize(ar);
	if (!(ar.IsStoring()))
	{
		RenewMiniMax();
	}
}

//p 内部データーが空の状態であれば削除しすべてが空のときTRUEを返す
int CSPtnText::CheckNullData()
{
	// 文字列を正規化する
	CStrgText::CheckNullData();
	if(m_pcFitingVect == NULL){
		return(TRUE);
	}
	if(m_pcFitingVect != NULL){
		if(m_pcFitingVect->CheckNullData() == TRUE){
			delete(m_pcFitingVect);
			m_pcFitingVect = NULL;
			return(TRUE);
		}
	}
	return(FALSE);
}

void CSPtnText::GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	DBL			tminix, tminiy, tmaxx, tmaxy;

	(*minix) = INTMAX;
	(*miniy) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	tminix = INTMAX;
	tminiy = INTMAX;
	tmaxx = -INTMAX;
	tmaxy = -INTMAX;
	CStrgText::GetDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
	(*minix) = min((*minix), tminix);
	(*miniy) = min((*miniy), tminiy);
	(*maxx) = max((*maxx), tmaxx);
	(*maxy) = max((*maxy), tmaxy);
	tminix = INTMAX;
	tminiy = INTMAX;
	tmaxx = -INTMAX;
	tmaxy = -INTMAX;
	m_pcFitingVect->GetDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
	(*minix) = min((*minix), tminix);
	(*miniy) = min((*miniy), tminiy);
	(*maxx) = max((*maxx), tmaxx);
	(*maxy) = max((*maxy), tmaxy);
}
void CSPtnText::GetStringDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	DBL			tminix, tminiy, tmaxx, tmaxy;

	(*minix) = INTMAX;
	(*miniy) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	tminix = INTMAX;
	tminiy = INTMAX;
	tmaxx = -INTMAX;
	tmaxy = -INTMAX;
	CStrgText::GetDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
	(*minix) = min((*minix), tminix);
	(*miniy) = min((*miniy), tminiy);
	(*maxx) = max((*maxx), tmaxx);
	(*maxy) = max((*maxy), tmaxy);
}

void CSPtnText::RenewMiniMax()
{
	DBL		minix, miniy, maxx, maxy;

	CStrgText::RenewMiniMax();
	m_pcFitingVect->RenewMiniMax();
	m_pcFitingVect->GetDMiniMax(&minix, &miniy, &maxx, &maxy);
	m_dMinix = min(minix, m_dMinix);
	m_dMiniy = min(miniy, m_dMiniy);
	m_dMaxx = max(maxx, m_dMaxx);
	m_dMaxy = max(maxy, m_dMaxy);
}

CData* CSPtnText::CopyData(DBL movex, DBL movey)
{
	CSPtnText *pcnewData;

	pcnewData = (CSPtnText*)NewDataCreate(SPTNTEXTID);
	CopyDataElement(pcnewData, movex, movey);
	pcnewData->RenewMiniMax();
	return((CData*)pcnewData);
}

void CSPtnText::CopyDataElement(CSPtnText *pcnewData, DBL movex, DBL movey)
{
	CVect* pcnewVect;

	pcnewVect = (CVect*)m_pcFitingVect->CopyData(movex, movey);
	pcnewData->SetFitingVect(pcnewVect);
	pcnewData->SetSpaceKind(m_nSpaceKind);
	pcnewData->SetRoundKind(m_nRoundKind);
	pcnewData->SetSetMode(m_nSetMode);
	pcnewData->SetPitch(m_dPitch);
	pcnewData->SetOderKind(m_nOder);

	CStrgText::CopyDataElement((CStrgText*)pcnewData, movex, movey);
}

void CSPtnText::AllFree()
{
	delete(m_pcFitingVect);
	CStrgText::AllFree();
}

void CSPtnText::OnDraw(CDC *pcDC)
{
	int		penno;
	int		rval, gval, bval;
	DBL		thick;
	int		nthick;
	int		nlimit;
	int		mode;

	nlimit = senvgetplgnlimit();
	if(senvchckstopflag() == ON){
		return;
	}
	if(CheckFlag(UNDERLINEFLAG) == ON){
		penno = GetOutLinePen();
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
		m_pcFitingVect->PlgnDraw(pcDC);
		scrnpenend(pcDC->m_hDC);
	}
	mode = senvcheckdrawmode(); 
	if(mode != PREVIEW
	&& mode != BLACKVIEW
	&& mode != PRINTVIEW
	&& mode != DIRCTVIEW
	&& mode != METAFVIEW){
		m_pcFitingVect->PlgnDraw(pcDC);
	}
	CStrgText::OnDraw(pcDC);
}

void CSPtnText::StockClrDraw(CDC *pcDC)
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL			cx, cy;

	if(senvgetmainbrush() != NULL){
		pos = m_cMojiTextList.GetHeadPosition();
		while(pos != NULL){
			pcMoji = m_cMojiTextList.GetNext(pos);
			if(senvchckstopflag() == ON){
				return;
			}
			pcMoji->FillClrDraw(pcDC);
		}
	}
	m_pcFitingVect->PlgnDraw(pcDC);
	GetStartXY(&cx, &cy);
	scrndxymmtorltv(&cx, &cy);
	BoxsDraw(pcDC, cx-2, cy-2, cx+2, cy+2);
}

void CSPtnText::OutlineDraw(CDC *pcDC)
{
	m_pcFitingVect->PlgnDraw(pcDC);
}

void CSPtnText::AtntionDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	m_pcFitingVect->AtntionDraw(pcDC);
}

void CSPtnText::NoactDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	m_pcFitingVect->NoactDraw(pcDC);
	CStrgText::NoactDraw(pcDC);
}

void CSPtnText::DelDraw(CDC *pcDC)
{
	m_pcFitingVect->DelDraw(pcDC);
	CStrgText::DelDraw(pcDC);
}

void CSPtnText::MoveTrns(DBL movex, DBL movey)
{
	m_pcFitingVect->MoveTrns(movex, movey);
	CStrgText::MoveTrns(movex, movey);
}

void CSPtnText::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	m_pcFitingVect->SizeTrns(cntrx, cntry, timex, timey);
	CStrgText::SizeTrns(cntrx, cntry, timex, timey);
}

void CSPtnText::SlopeTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
{
	m_pcFitingVect->SlopTrns(cntrx, cntry, ddx, ddy);
	CStrgText::SlopTrns(cntrx, cntry, ddx, ddy);
}

void CSPtnText::MirrorTrns(DBL a, DBL b, DBL t1, DBL t2)
{
	m_pcFitingVect->MirrorTrns(a, b, t1, t2);
	CStrgText::MirrorTrns(a, b, t1, t2);
}

void CSPtnText::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	m_pcFitingVect->RoundTrns(cntrx, cntry, tsin, tcos);
	CStrgText::RoundTrns(cntrx, cntry, tsin, tcos);
}

void CSPtnText::StrctBoxsTrns()
{
	m_pcFitingVect->StrctBoxsTrns();
	CStrgText::StrctBoxsTrns();
}

void CSPtnText::AllOnPointFlag(int bitflag)
{
	m_pcFitingVect->AllOnPointFlag(bitflag);
	CStrgText::AllOnPointFlag(bitflag);
}

void CSPtnText::AllOffPointFlag(int bitflag)
{
	m_pcFitingVect->AllOffPointFlag(bitflag);
	CStrgText::AllOffPointFlag(bitflag);
}

void CSPtnText::PointXLegal(DBL xx)
{
	m_pcFitingVect->PointXLegal(xx);
	CStrgText::PointXLegal(xx);
}

void CSPtnText::PointYLegal(DBL yy)
{
	m_pcFitingVect->PointYLegal(yy);
	CStrgText::PointYLegal(yy);
}

CData* CSPtnText::SrchNearPoint(DBL xx, DBL yy, DBL *retlen, int *retpp)
{
	CData*	ret;

	*retlen = INTMAX;
	ret = m_pcFitingVect->SrchNearPoint(xx, yy, retlen, retpp);
	return(ret);
}

CData* CSPtnText::SrchNearOnLinePoint(DBL xx, DBL yy, DBL *retlen, DBL *retx, DBL *rety, int *retpp)
{
	CData*	ret;

	*retlen = INTMAX;
	ret = m_pcFitingVect->SrchNearOnLinePoint(xx, yy, retlen, retx, rety, retpp);
	return(ret);
}

CVect* CSPtnText::GetFitingVect()
{
	return(m_pcFitingVect);
}

void CSPtnText::SetFitingVect(CVect* pcVect)
{
	m_pcFitingVect = pcVect;
	pcVect->SetFillPen(-1);
	pcVect->SetOutLinePen(-1);
	pcVect->SetOutLineWidth(0.0);
}

void CSPtnText::GetSpaceKind(int *flag)
{
	*flag = m_nSpaceKind;
}

void CSPtnText::SetSpaceKind(int flag)
{
	m_nSpaceKind = flag;
}

void CSPtnText::GetRoundKind(int *flag)
{
	*flag = m_nRoundKind;
}

void CSPtnText::SetRoundKind(int flag)
{
	m_nRoundKind = flag;
}

void CSPtnText::GetSetMode(int *mode)
{
	*mode = m_nSetMode;
}

void CSPtnText::SetSetMode(int mode)
{
	m_nSetMode = mode;
}

void CSPtnText::GetPitch(LPDBL dpitch)
{
	*dpitch = m_dPitch;
}

void CSPtnText::SetPitch(DBL dpitch)
{
	m_dPitch = dpitch;
}

void CSPtnText::GetFlatPar(LPDBL dflatpar)
{
	*dflatpar = m_dFlatPar;
}

void CSPtnText::SetFlatPar(DBL dflatpar)
{
	m_dFlatPar = dflatpar;
}

void CSPtnText::GetOderKind(int *noder)
{
	*noder = m_nOder;
}

void CSPtnText::SetOderKind(int noder)
{
	m_nOder = noder;
}

void CSPtnText::StrgFiting()
{
	StrgLineStruct();
}

void CSPtnText::StrgLineStruct()
{
	DBL	dwidth, dhight;

	GetFontSize(&dwidth, &dhight);
	switch(m_nOder){
	case ODERLEFT:
		if(m_nSpaceKind == ON){
			SetLenStrgLineStructLeftData();
		}
		else{
			SetLenStrgLineStructLeftWaku();
		}
		break;
	case ODERCNTR:
		if(m_nSpaceKind == ON){
			SetLenStrgLineStructCntrData();
		}
		else{
			SetLenStrgLineStructCntrWaku();
		}
		break;
	case ODERRIGHT:
		if(m_nSpaceKind == ON){
			SetLenStrgLineStructRightData();
		}
		else{
			SetLenStrgLineStructRightWaku();
		}
		break;
	case ODEREQUAL:
	default:
		m_nOder = ODEREQUAL;
		if(m_nSpaceKind == ON){
			SetLenStrgLineStructSpace();
		}
		else{
			SetLenStrgLineStructEqual();
		}
		break;
	}
	StrgLineStructTrns();
}

void CSPtnText::SetLenStrgLineStructLeftData()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		dwidth, dhight;
	DBL		setlen, setspace;
	DBL		tsetlen;
	DBL		space;
	DBL		minix, miniy, maxx, maxy;
	UINT	code;

	m_dLinelen = GetVectLength();
	GetFontSize(&dwidth, &dhight);
	m_dHalffontsize = 0;
	m_dStartlen = 0;
	m_dTrnstime = 1;
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
		setspace = dhight * (m_dPitch - 1.0);
	}
	else{
		setspace = dwidth * (m_dPitch - 1.0);
	}
	setlen = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			tsetlen = setlen + (maxy - miniy) / 2;
		}
		else{
			tsetlen = setlen + (maxx - minix) / 2;
		}
		pcMoji->SetTT(tsetlen/m_dLinelen);
		pcMoji->GetCode(&code);
		if(code > 0x0100){
			space = setspace;
		}
		else{
			space = setspace/2;
		}
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			setlen = setlen + (maxy - miniy) + space;
		}
		else{
			setlen = setlen + (maxx - minix) + space;
		}
	}
}

void CSPtnText::SetLenStrgLineStructLeftWaku()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		dwidth, dhight;
	DBL		setlen, setspace;
	DBL		tsetlen;
	DBL		space;
	DBL		minix, miniy, maxx, maxy;
	UINT	code;

	m_dLinelen = GetVectLength();
	GetFontSize(&dwidth, &dhight);
	m_dHalffontsize = 0;
	m_dStartlen = 0;
	m_dTrnstime = 1;
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
		setspace = dhight * (m_dPitch - 1.0);
	}
	else{
		setspace = dwidth * (m_dPitch - 1.0);
	}
	setlen = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			tsetlen = setlen + (maxy - miniy) / 2;
		}
		else{
			tsetlen = setlen + (maxx - minix) / 2;
		}
		pcMoji->SetTT(tsetlen/m_dLinelen);
		pcMoji->GetCode(&code);
		if(code > 0x0100){
			space = setspace;
		}
		else{
			space = setspace/2;
		}
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			setlen = setlen + (maxy - miniy) + space;
		}
		else{
			setlen = setlen + (maxx - minix) + space;
		}
	}
}

void CSPtnText::SetLenStrgLineStructCntrData()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		dwidth, dhight;
	DBL		setlen, setspace;
	DBL		tsetlen;
	DBL		sumlen, sumspace, space;
	DBL		minix, miniy, maxx, maxy;
	int		zen, han;
	UINT	code;

	m_dLinelen = GetVectLength();
	sumlen = GetDataSumLength();
	GetFontSize(&dwidth, &dhight);
	m_dHalffontsize = 0;
	m_dStartlen = 0;
	m_dTrnstime = 1;
	GetSpaceMojiCount(&zen, &han);	
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
		setspace = dhight * (m_dPitch - 1.0);
		sumspace = zen * setspace + han * setspace / 2;
		m_dStartlen = (m_dLinelen - (sumlen + sumspace)) / 2;
	}
	else{
		setspace = dwidth * (m_dPitch - 1.0);
		sumspace = zen * setspace + han * setspace / 2;
		m_dStartlen = (m_dLinelen - (sumlen + sumspace)) / 2;
	}
	setlen = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			tsetlen = setlen + (maxy - miniy) / 2;
		}
		else{
			tsetlen = setlen + (maxx - minix) / 2;
		}
		pcMoji->SetTT(tsetlen/m_dLinelen);
		pcMoji->GetCode(&code);
		if(code > 0x0100){
			space = setspace;
		}
		else{
			space = setspace/2;
		}
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			setlen = setlen + (maxy - miniy) + space;
		}
		else{
			setlen = setlen + (maxx - minix) + space;
		}
	}
}

void CSPtnText::SetLenStrgLineStructCntrWaku()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		dwidth, dhight;
	DBL		setlen, setspace;
	DBL		tsetlen;
	DBL		sumlen, sumspace, space;
	DBL		minix, miniy, maxx, maxy;
	int		zen, han;
	UINT	code;

	m_dLinelen = GetVectLength();
	sumlen = GetWakuSumLength();
	GetFontSize(&dwidth, &dhight);
	m_dHalffontsize = 0;
	m_dStartlen = 0;
	m_dTrnstime = 1;
	GetSpaceMojiCount(&zen, &han);	
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
		setspace = dhight * (m_dPitch - 1.0);
		sumspace = zen * setspace + han * setspace / 2;
		m_dStartlen = (m_dLinelen - (sumlen + sumspace)) / 2;
	}
	else{
		setspace = dwidth * (m_dPitch - 1.0);
		sumspace = zen * setspace + han * setspace / 2;
		m_dStartlen = (m_dLinelen - (sumlen + sumspace)) / 2;
	}
	setlen = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			tsetlen = setlen + (maxy - miniy) / 2;
		}
		else{
			tsetlen = setlen + (maxx - minix) / 2;
		}
		pcMoji->SetTT(tsetlen/m_dLinelen);
		pcMoji->GetCode(&code);
		if(code > 0x0100){
			space = setspace;
		}
		else{
			space = setspace/2;
		}
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			setlen = setlen + (maxy - miniy) + space;
		}
		else{
			setlen = setlen + (maxx - minix) + space;
		}
	}
}

void CSPtnText::SetLenStrgLineStructRightData()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		dwidth, dhight;
	DBL		setlen, setspace;
	DBL		tsetlen;
	DBL		sumlen, sumspace, space;
	DBL		minix, miniy, maxx, maxy;
	int		zen, han;
	UINT	code;

	m_dLinelen = GetVectLength();
	sumlen = GetDataSumLength();
	GetFontSize(&dwidth, &dhight);
	m_dHalffontsize = 0;
	m_dStartlen = 0;
	m_dTrnstime = 1;
	GetSpaceMojiCount(&zen, &han);	
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
		setspace = dhight * (m_dPitch - 1.0);
		sumspace = zen * setspace + han * setspace / 2;
		m_dStartlen = m_dLinelen - (sumlen + sumspace);
	}
	else{
		setspace = dwidth * (m_dPitch - 1.0);
		sumspace = zen * setspace + han * setspace / 2;
		m_dStartlen = m_dLinelen - (sumlen + sumspace);
	}
	setlen = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			tsetlen = setlen + (maxy - miniy) / 2;
		}
		else{
			tsetlen = setlen + (maxx - minix) / 2;
		}
		pcMoji->SetTT(tsetlen/m_dLinelen);
		pcMoji->GetCode(&code);
		if(code > 0x0100){
			space = setspace;
		}
		else{
			space = setspace/2;
		}
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			setlen = setlen + (maxy - miniy) + space;
		}
		else{
			setlen = setlen + (maxx - minix) + space;
		}
	}
}

void CSPtnText::SetLenStrgLineStructRightWaku()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		dwidth, dhight;
	DBL		setlen, setspace;
	DBL		tsetlen;
	DBL		sumlen, sumspace, space;
	DBL		minix, miniy, maxx, maxy;
	int		zen, han;
	UINT	code;

	m_dLinelen = GetVectLength();
	sumlen = GetWakuSumLength();
	GetFontSize(&dwidth, &dhight);
	m_dHalffontsize = 0;
	m_dStartlen = 0;
	m_dTrnstime = 1;
	GetSpaceMojiCount(&zen, &han);	
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
		setspace = dhight * (m_dPitch - 1.0);
		sumspace = zen * setspace + han * setspace / 2;
		m_dStartlen = m_dLinelen - (sumlen + sumspace);
	}
	else{
		setspace = dwidth * (m_dPitch - 1.0);
		sumspace = zen * setspace + han * setspace / 2;
		m_dStartlen = m_dLinelen - (sumlen + sumspace);
	}
	setlen = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			tsetlen = setlen + (maxy - miniy) / 2;
		}
		else{
			tsetlen = setlen + (maxx - minix) / 2;
		}
		pcMoji->SetTT(tsetlen/m_dLinelen);
		pcMoji->GetCode(&code);
		if(code > 0x0100){
			space = setspace;
		}
		else{
			space = setspace/2;
		}
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			setlen = setlen + (maxy - miniy) + space;
		}
		else{
			setlen = setlen + (maxx - minix) + space;
		}
	}
}

void CSPtnText::SetLenStrgLineStructSpace()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		tmplinelen;
	DBL		dwidth, dhight;
	DBL		sminix, sminiy, smaxx, smaxy;
	DBL		mminix, mminiy, mmaxx, mmaxy;
	DBL		setlen;

	GetWakuDMiniMax(&sminix, &sminiy, &smaxx, &smaxy);
	GetFontSize(&dwidth, &dhight);
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
		m_dStrglen = smaxy - sminiy;
		m_dHalffontsize = dhight / 2;
	}	
	else{
		m_dStrglen = smaxx - sminix;
		m_dHalffontsize = dwidth / 2;
	}

	m_dLinelen = GetVectLength();
	if(m_pcFitingVect->CheckFlag(REJIONFLAG) == ON
	&& m_pcFitingVect->GetPointCount() != 2){
		m_dStrglen = m_dStrglen + m_dSize*m_dSpacePar/100.0;
		m_dStartlen = 0;
		m_dTrnstime = m_dLinelen / m_dStrglen;
	}
	else{
		//文字の中心までの長さをひく
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			m_dStrglen = m_dStrglen - dhight;
			tmplinelen = m_dLinelen - dhight;
		}
		else{
			m_dStrglen = m_dStrglen - dwidth;
			tmplinelen = m_dLinelen - dwidth;
		}
		m_dStartlen = 0;
		m_dTrnstime = tmplinelen / m_dStrglen;
	}

	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetWakuDMiniMax(&mminix, &mminiy, &mmaxx, &mmaxy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			setlen = (mmaxy + mminiy) / 2 - (sminiy + m_dHalffontsize);
		}
		else{
			setlen = (mmaxx + mminix) / 2 - (sminix + m_dHalffontsize);
		}
		pcMoji->SetTT(setlen/m_dLinelen);
	}
}

void CSPtnText::SetLenStrgLineStructEqual()
{
	POSITION	pos;
	CMojiText*	pcFirstMoji;
	CMojiText*	pcMoji;
	DBL	mminix, mminiy, mmaxx, mmaxy;
	DBL	dwd, dhi;
	DBL	setlen;
	DBL	tmplinelen;

	m_dLinelen = GetVectLength();
	m_dStrglen = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	pcFirstMoji = m_cMojiTextList.GetAt(pos);
	pcFirstMoji->GetWakuDMiniMax(&mminix, &mminiy, &mmaxx, &mmaxy);
	m_dHalffontsize = (mmaxx - mminix)/2;
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetFontSize(&dwd, &dhi);
		setlen = m_dStrglen;
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			m_dStrglen += dhi;
		}
		else{
			m_dStrglen += dwd;
		}
		pcMoji->SetTT(setlen/m_dLinelen);
	}

	if(m_pcFitingVect->CheckFlag(REJIONFLAG) == ON
	&& m_pcFitingVect->GetPointCount() != 2){
		m_dStartlen = 0;
		m_dTrnstime = m_dLinelen / m_dStrglen;
	}
	else{
		//文字の中心までの長さをひく
		pcFirstMoji->GetFontSize(&dwd, &dhi);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			m_dStrglen = m_dStrglen - dhi;
			tmplinelen = m_dLinelen - dhi;
		}
		else{
			m_dStrglen = m_dStrglen - dwd;
			tmplinelen = m_dLinelen - dwd;
		}
		m_dStartlen = 0;
		m_dTrnstime = tmplinelen / m_dStrglen;
	}
}

void CSPtnText::StrgLineStructTrns()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		tt;
	DBL		setlen;
	DBL		mminix, mminiy, mmaxx, mmaxy;
	DBL		cx, cy;
	DBL		movex, movey;
	DBL		tsin, tcos;
	DBL		rag, dx, dy;

	m_dLinelen = GetVectLength();
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetTT(&tt);
		setlen = tt * m_dLinelen;
		setlen = setlen*m_dTrnstime + m_dHalffontsize + m_dStartlen;
		if(m_pcFitingVect->CheckFlag(REJIONFLAG) == ON){
			if(m_dLinelen < setlen){
				setlen = setlen - m_dLinelen;
			}
		}
		pcMoji->GetWakuDMiniMax(&mminix, &mminiy, &mmaxx, &mmaxy);
		if(CheckTateYoko() == YOKOGAKI || CheckTateYoko() == YOKOOKI){
			switch(m_nSetMode){
			case LEFTDOWN:
				cx = (mminix + mmaxx) / 2;
				cy = mminiy;
				break;
			case CNTRCNTR:
				cx = (mminix + mmaxx) / 2;
				cy = (mminiy + mmaxy) / 2;
				break;
			case RIGHTUP:
				cx = (mminix + mmaxx) / 2;
				cy = mmaxy;
				break;
			}
		}
		else{
			switch(m_nSetMode){
			case LEFTDOWN:
				cx = mminix;
				cy = (mminiy + mmaxy) / 2;
				break;
			case CNTRCNTR:
				cx = (mminix + mmaxx) / 2;
				cy = (mminiy + mmaxy) / 2;
				break;
			case RIGHTUP:
				cx = mmaxx;
				cy = (mminiy + mmaxy) / 2;
				break;
			}
		}
		rag = GetVectPointRagAndCood(&setlen, &dx, &dy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			rag += M_PI_2;
		}
		movex = dx - cx;
		movey = dy - cy;
		pcMoji->MoveTrns(movex, movey);
		tsin = sin(rag);
		tcos = cos(rag);
		pcMoji->RoundTrns(dx, dy, tsin, tcos);
	}
}

DBL CSPtnText::GetVectLength()
{
	int		pcnt;
	DBL		x1, y1, x2, y2;
	DBL		sumlen;

	sumlen = 0;
	pcnt = m_pcFitingVect->GetPointCount();
	if(pcnt <= 1){
		return(sumlen);
	}
	if(pcnt == 2){
		m_pcFitingVect->GetDXY(0, &x1, &y1);
		m_pcFitingVect->GetDXY(1, &x2, &y2);
		sumlen = GetLineLength(x1, y1, x2, y2);
		return(sumlen);
	}
	if(m_pcFitingVect->CheckFlag(REJIONFLAG) == ON){
		sumlen = GetRejionLength();
	}
	else{
		sumlen = GetNorejionLength();
	}
	return(sumlen);
}

DBL CSPtnText::GetRejionLength()
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	pcnt = m_pcFitingVect->GetPointCount();
	for(pp = 0; pp < pcnt;){
		p0 = m_pcFitingVect->PointLegal(pcnt, pp-1);
		p1 = m_pcFitingVect->PointLegal(pcnt, pp+0);
		p2 = m_pcFitingVect->PointLegal(pcnt, pp+1);
		p3 = m_pcFitingVect->PointLegal(pcnt, pp+2);
		p4 = m_pcFitingVect->PointLegal(pcnt, pp+3);
		m_pcFitingVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		m_pcFitingVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		m_pcFitingVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		m_pcFitingVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		m_pcFitingVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				len = GetLineLength(x1, y1, x2, y2);
				sumlen += len;
				len = GetLineLength(x2, y2, x3, y3);
				sumlen += len;
			}
			else if(p2 == 0){
				len = GetLineLength(x1, y1, x2, y2);
				sumlen += len;
			}
			else{
				len = GetBezeLength(x1, y1, x2, y2, x3, y3, x4, y4);
				sumlen += len;
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				len = GetArcLength(x1, y1, x2, y2, x3, y3);
				sumlen += len;
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					len = GetArcLength(x1, y1, x2, y2, x3, y3);
					sumlen += len;
				}
				else{
					len = GetFreeCurveLength(atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
					sumlen += len;
				}
			}
			else{
				len = GetLineLength(x1, y1, x2, y2);
				sumlen += len;
			}
			pp += 1;
		}
	}
	return(sumlen);
}

DBL CSPtnText::GetNorejionLength()
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		len;
	DBL		sumlen;

	pcnt = m_pcFitingVect->GetPointCount();

	sumlen = 0;
	for(pp = 0; pp < pcnt-1;){
		p0 = m_pcFitingVect->PointLegal(pcnt, pp-1);
		p1 = m_pcFitingVect->PointLegal(pcnt, pp+0);
		p2 = m_pcFitingVect->PointLegal(pcnt, pp+1);
		p3 = m_pcFitingVect->PointLegal(pcnt, pp+2);
		p4 = m_pcFitingVect->PointLegal(pcnt, pp+3);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
		m_pcFitingVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		m_pcFitingVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		m_pcFitingVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		m_pcFitingVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		m_pcFitingVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				len = GetLineLength(x1, y1, x2, y2);
				sumlen += len;
				len = GetLineLength(x2, y2, x3, y3);
				sumlen += len;
			}
			else if(p3 == 0){
				len = GetLineLength(x1, y1, x2, y2);
				sumlen += len;
			}
			else{
				len = GetBezeLength(x1, y1, x2, y2, x3, y3, x4, y4);
				sumlen += len;
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				len = GetLineLength(x1, y1, x2, y2);
				sumlen += len;
			}
			else{
				len = GetArcLength(x1, y1, x2, y2, x3, y3);
				sumlen += len;
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				len = GetFreeCurveLength(atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				sumlen += len;
			}
			else{
				len = GetLineLength(x1, y1, x2, y2);
				sumlen += len;
			}
			pp += 1;
		}
	}
	return(sumlen);
}

DBL CSPtnText::GetBezeLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL		t, t1, b1, b2, b3, b4;
	DBL		sx, sy, ex, ey;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	sx = x1; sy = y1;
	for(t = 0.1; t < 1.0; t += 0.1){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		ex = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ey = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		len = GetLineLength(sx, sy, ex, ey);
		sumlen += len;
		sx = ex; sy = ey;
	}
	ex = x4; ey = y4;
	len = GetLineLength(sx, sy, ex, ey);
	len = hypot(sx-ex, sy-ey);
	sumlen += len;
	return(sumlen);
}

DBL CSPtnText::GetArcLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL		x0, y0, rag01, rag03, dr;
	int		stat;
	DBL		rag, steprag;
	DBL		sx, sy, ex, ey;
	int		count, idx;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat == FAIL){
		len = GetLineLength(x1, y1, x2, y2);
		sumlen += len;
		len = GetLineLength(x1, y1, x2, y2);
		sumlen += len;
		return(sumlen);
	}
	stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &rag01, &rag03, &dr);
	if(stat == FAIL){
		len = GetLineLength(x1, y1, x2, y2);
		sumlen += len;
		len = GetLineLength(x1, y1, x2, y2);
		sumlen += len;
		return(sumlen);
	}

	count = (int)(fabs(rag03 - rag01) / M_5RAG);
	if(count < 2){
		count = 2;
	}
	steprag = (rag03 - rag01) / (DBL)count;

	sx = x1; sy = y1;
	for(idx = 1, rag = rag01+steprag; idx < count; idx++, rag += steprag){
		ex = cos(rag)*dr+x0;
		ey = sin(rag)*dr+y0;
		len = GetLineLength(sx, sy, ex, ey);
		sumlen += len;
		sx = ex; sy = ey;
	}
	ex = x3; ey = y3;
	len = GetLineLength(sx, sy, ex, ey);
	sumlen += len;
	return(sumlen);
}

DBL CSPtnText::GetFreeCurveLength(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	rag1, rag2;
	DBL	xp1, yp1, xp2, yp2;
	DBL	len;

	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);

	if(rag1 == NORAG && rag2 == NORAG){
		len = GetLineLength(x1, y1, x2, y2);
	}
	else{
		curvenoragtocurverag(&rag1, &rag2);
		curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
		curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
		len = GetBezeLength(x1, y1, xp1, yp1, xp2, yp2, x2, y2);
	}
	return(len);
}

DBL CSPtnText::GetLineLength(DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		len;

	len = hypot(x1-x2, y1-y2);
	return(len);
}

// 点までの長さを求める
DBL CSPtnText::GetVectPointLength(DBL xx, DBL yy)
{
	int		pcnt;
	int		flag;
	DBL		x1, y1, x2, y2;
	DBL		sumlen;

	flag = OFF;
	sumlen = 0;
	pcnt = m_pcFitingVect->GetPointCount();
	if(pcnt <= 1){
		return(sumlen);
	}
	if(pcnt == 2){
		m_pcFitingVect->GetDXY(0, &x1, &y1);
		m_pcFitingVect->GetDXY(1, &x2, &y2);
		sumlen = GetLinePointLength(&flag, xx, yy, x1, y1, x2, y2);
		return(sumlen);
	}
	if(m_pcFitingVect->CheckFlag(REJIONFLAG) == ON){
		sumlen = GetRejionPointLength(&flag, xx, yy);
	}
	else{
		sumlen = GetNorejionPointLength(&flag, xx, yy);
	}
	return(sumlen);
}

DBL CSPtnText::GetRejionPointLength(int *flag, DBL xx, DBL yy)
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	pcnt = m_pcFitingVect->GetPointCount();
	for(pp = 0; pp < pcnt;){
		p0 = m_pcFitingVect->PointLegal(pcnt, pp-1);
		p1 = m_pcFitingVect->PointLegal(pcnt, pp+0);
		p2 = m_pcFitingVect->PointLegal(pcnt, pp+1);
		p3 = m_pcFitingVect->PointLegal(pcnt, pp+2);
		p4 = m_pcFitingVect->PointLegal(pcnt, pp+3);
		m_pcFitingVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		m_pcFitingVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		m_pcFitingVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		m_pcFitingVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		m_pcFitingVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
				len = GetLinePointLength(flag, xx, yy, x2, y2, x3, y3);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else if(p2 == 0){
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else{
				len = GetBezePointLength(flag, xx, yy, x1, y1, x2, y2, x3, y3, x4, y4);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				len = GetArcPointLength(flag, xx, yy, x1, y1, x2, y2, x3, y3);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					len = GetArcPointLength(flag, xx, yy, x1, y1, x2, y2, x3, y3);
					sumlen += len;
					if(*flag == ON){
						return(sumlen);
					}
				}
				else{
					len = GetFreeCurvePointLength(flag, xx, yy, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
					sumlen += len;
					if(*flag == ON){
						return(sumlen);
					}
				}
			}
			else{
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 1;
		}
	}
	return(sumlen);
}

DBL CSPtnText::GetNorejionPointLength(int *flag, DBL xx, DBL yy)
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	pcnt = m_pcFitingVect->GetPointCount();
	for(pp = 0; pp < pcnt-1;){
		p0 = m_pcFitingVect->PointLegal(pcnt, pp-1);
		p1 = m_pcFitingVect->PointLegal(pcnt, pp+0);
		p2 = m_pcFitingVect->PointLegal(pcnt, pp+1);
		p3 = m_pcFitingVect->PointLegal(pcnt, pp+2);
		p4 = m_pcFitingVect->PointLegal(pcnt, pp+3);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
		m_pcFitingVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		m_pcFitingVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		m_pcFitingVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		m_pcFitingVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		m_pcFitingVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
				len = GetLinePointLength(flag, xx, yy, x2, y2, x3, y3);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else if(p3 == 0){
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else{
				len = GetBezePointLength(flag, xx, yy, x1, y1, x2, y2, x3, y3, x4, y4);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else{
				len = GetArcPointLength(flag, xx, yy, x1, y1, x2, y2, x3, y3);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				len = GetFreeCurvePointLength(flag, xx, yy, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else{
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 1;
		}
	}
	return(sumlen);
}

DBL CSPtnText::GetBezePointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL		t, t1, b1, b2, b3, b4;
	DBL		sx, sy, ex, ey;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	sx = x1; sy = y1;
	for(t = 0.1; t < 1.0; t += 0.1){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		ex = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ey = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		len = GetLinePointLength(flag, xx, yy, sx, sy, ex, ey);
		sumlen += len;
		if(*flag == ON){
			return(sumlen);
		}
		sx = ex; sy = ey;
	}
	ex = x4; ey = y4;
	len = GetLinePointLength(flag, xx, yy, sx, sy, ex, ey);
	sumlen += len;
	return(sumlen);
}

DBL CSPtnText::GetArcPointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL		x0, y0, rag01, rag03, dr;
	int		stat;
	DBL		rag, steprag;
	DBL		sx, sy, ex, ey;
	int		count, idx;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat == FAIL){
		len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
		sumlen += len;
		if(*flag == ON){
			return(sumlen);
		}
		len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
		sumlen += len;
		return(sumlen);
	}
	stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &rag01, &rag03, &dr);
	if(stat == FAIL){
		len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
		sumlen += len;
		if(*flag == ON){
			return(sumlen);
		}
		len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
		sumlen += len;
		return(sumlen);
	}

	count = (int)(fabs(rag03 - rag01) / M_5RAG);
	if(count < 2){
		count = 2;
	}
	steprag = (rag03 - rag01) / (DBL)count;

	sx = x1; sy = y1;
	for(idx = 1, rag = rag01+steprag; idx < count; idx++, rag += steprag){
		ex = cos(rag)*dr+x0;
		ey = sin(rag)*dr+y0;
		len = GetLinePointLength(flag, xx, yy, sx, sy, ex, ey);
		sumlen += len;
		if(*flag == ON){
			return(sumlen);
		}
		sx = ex; sy = ey;
	}
	ex = x3; ey = y3;
	len = GetLinePointLength(flag, xx, yy, sx, sy, ex, ey);
	sumlen += len;
	return(sumlen);
}

DBL CSPtnText::GetFreeCurvePointLength(int *flag, DBL xx, DBL yy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	rag1, rag2;
	DBL	xp1, yp1, xp2, yp2;
	DBL	len;

	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);

	if(rag1 == NORAG && rag2 == NORAG){
		len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
	}
	else{
		curvenoragtocurverag(&rag1, &rag2);
		curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
		curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
		len = GetBezePointLength(flag, xx, yy, x1, y1, xp1, yp1, xp2, yp2, x2, y2);
	}
	return(len);
}

DBL CSPtnText::GetLinePointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		len;
	DBL		cx, cy;
	int		ret;
	DBL		tlen;

	*flag = OFF;
	len = hypot(x1-x2, y1-y2);
	ret = curveperpendicular(x1, y1, x2, y2, xx, yy, &cx, &cy);
	if(ret != FAIL){
		ret = curveonlinechk(x1, y1, x2, y2, cx, cy);
		if(ret == ON){
			tlen = hypot(xx - cx, yy - cy);
			if(tlen < 1.0){
				*flag = ON;
				len = hypot(x1-cx, y1-cy);
			}
		}
	}
	return(len);
}

// 開始点からの距離が０になった角度と位置を求める
DBL CSPtnText::GetVectPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy)
{
	int		pcnt;
	DBL		rag;

	rag = 0;
	pcnt = m_pcFitingVect->GetPointCount();
	if(pcnt <= 1){
		return(rag);
	}
	if(pcnt == 2){
		rag = GetNorejionPointRagAndCood(setlen, dx, dy);
		return(rag);
	}
	if(m_pcFitingVect->CheckFlag(REJIONFLAG) == ON){
		rag = GetRejionPointRagAndCood(setlen, dx, dy);
	}
	else{
		rag = GetNorejionPointRagAndCood(setlen, dx, dy);
	}
	return(rag);
}

DBL CSPtnText::GetRejionPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy)
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		rag;

	rag = 0;
	pcnt = m_pcFitingVect->GetPointCount();
	for(pp = 0; pp < pcnt;){
		p0 = m_pcFitingVect->PointLegal(pcnt, pp-1);
		p1 = m_pcFitingVect->PointLegal(pcnt, pp+0);
		p2 = m_pcFitingVect->PointLegal(pcnt, pp+1);
		p3 = m_pcFitingVect->PointLegal(pcnt, pp+2);
		p4 = m_pcFitingVect->PointLegal(pcnt, pp+3);
		m_pcFitingVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		m_pcFitingVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		m_pcFitingVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		m_pcFitingVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		m_pcFitingVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
				rag = GetLinePointRagAndCood(setlen, dx, dy, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else if(p2 == 0){
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else{
				rag = GetBezePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2, x3, y3, x4, y4);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				rag = GetArcPointRagAndCood(setlen, dx, dy, x1, y1, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					rag = GetArcPointRagAndCood(setlen, dx, dy, x1, y1, x2, y2, x3, y3);
					if((*setlen) <= 0){
						return(rag);
					}
				}
				else{
					rag = GetFreeCurvePointRagAndCood(setlen, dx, dy, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
					if((*setlen) <= 0){
						return(rag);
					}
				}
			}
			else{
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 1;
		}
	}
	return(rag);
}

DBL CSPtnText::GetNorejionPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy)
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		rag, len;

	rag = 0;
	pcnt = m_pcFitingVect->GetPointCount();
	for(pp = 0; pp < pcnt-1;){
		p0 = m_pcFitingVect->PointLegal(pcnt, pp-1);
		p1 = m_pcFitingVect->PointLegal(pcnt, pp+0);
		p2 = m_pcFitingVect->PointLegal(pcnt, pp+1);
		p3 = m_pcFitingVect->PointLegal(pcnt, pp+2);
		p4 = m_pcFitingVect->PointLegal(pcnt, pp+3);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
		m_pcFitingVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		m_pcFitingVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		m_pcFitingVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		m_pcFitingVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		m_pcFitingVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
				rag = GetLinePointRagAndCood(setlen, dx, dy, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else if(p3 == 0){
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else{
				rag = GetBezePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2, x3, y3, x4, y4);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else{
				rag = GetArcPointRagAndCood(setlen, dx, dy, x1, y1, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				rag = GetFreeCurvePointRagAndCood(setlen, dx, dy, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else{
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 1;
		}
	}
	// 最後の線分を延長したところの座標を求める
	if(0 < *setlen){
		x1 = m_dLastX1; y1 = m_dLastY1;
		x2 = m_dLastX2; y2 = m_dLastY2;
		len = hypot(x1-x2, y1-y2);
		(*dx) = (x2 - x1) * ((*setlen)+len) / len + x1;
		(*dy) = (y2 - y1) * ((*setlen)+len) / len + y1;
	}
	return(rag);
}

DBL CSPtnText::GetBezePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL		t, t1, b1, b2, b3, b4;
	DBL		sx, sy, ex, ey;
	DBL		len;
	DBL		rag;

	rag = 0;
	sx = x1; sy = y1;
	for(t = 0.1; t < 1.0; t += 0.1){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		ex = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ey = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		rag = GetLinePointRagAndCood(setlen, dx, dy, sx, sy, ex, ey);
		if((*setlen) <= 0){
			return(rag);
		}
		sx = ex; sy = ey;
	}
	ex = x4; ey = y4;
	rag = GetLinePointRagAndCood(setlen, dx, dy, sx, sy, ex, ey);
	if((*setlen) <= 0){
		return(rag);
	}
	len = hypot(sx-ex, sy-ey);
	return(rag);
}

DBL CSPtnText::GetArcPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL		x0, y0, rag01, rag03, dr;
	int		stat;
	DBL		rag, steprag;
	DBL		sx, sy, ex, ey;
	int		count, idx;
	DBL		retrag;

	rag = 0;
	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat == FAIL){
		rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
		rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
		return(rag);
	}
	stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &rag01, &rag03, &dr);
	if(stat == FAIL){
		rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
		rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
		return(rag);
	}

	count = (int)(fabs(rag03 - rag01) / M_5RAG);
	if(count < 2){
		count = 2;
	}
	steprag = (rag03 - rag01) / (DBL)count;

	sx = x1; sy = y1;
	for(idx = 1, rag = rag01+steprag; idx < count; idx++, rag += steprag){
		ex = cos(rag)*dr+x0;
		ey = sin(rag)*dr+y0;
		retrag = GetLinePointRagAndCood(setlen, dx, dy, sx, sy, ex, ey);
		if((*setlen) <= 0){
			return(retrag);
		}
		sx = ex; sy = ey;
	}
	ex = x3; ey = y3;
	retrag = GetLinePointRagAndCood(setlen, dx, dy, sx, sy, ex, ey);
	if((*setlen) <= 0){
		return(retrag);
	}
	return(retrag);
}

DBL CSPtnText::GetFreeCurvePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	rag;
	DBL	rag1, rag2;
	DBL	xp1, yp1, xp2, yp2;

	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);

	if(rag1 == NORAG && rag2 == NORAG){
		rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
	}
	else{
		curvenoragtocurverag(&rag1, &rag2);
		curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
		curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
		rag = GetBezePointRagAndCood(setlen, dx, dy, x1, y1, xp1, yp1, xp2, yp2, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
	}
	return(rag);
}

DBL CSPtnText::GetLinePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		len;
	DBL		rag;

	len = hypot(x1-x2, y1-y2);
	if(y2 == y1 && x2 == x1){
		rag = 0;
	}
	else{
		rag = atan2(y2-y1, x2-x1);
	}
	if((*setlen) <= len){
		(*dx) = (x2 - x1) * (*setlen) / len + x1;
		(*dy) = (y2 - y1) * (*setlen) / len + y1;
		(*setlen) = 0;
	}
	else{
		(*setlen) = (*setlen) - len;
	}
	m_dLastX1 = x1; m_dLastY1 = y1;
	m_dLastX2 = x2; m_dLastY2 = y2;
	return(rag);
}

void CSPtnText::DeleteData(CData *delData)
{
	CVect	*pcVect;

	if(m_pcFitingVect == delData){
		pcVect = (CVect*)(m_pcFitingVect->CopyData(0, 0));
		m_pcFitingVect = pcVect;
		return;
	}
	CStrgText::DeleteData(delData);
}

