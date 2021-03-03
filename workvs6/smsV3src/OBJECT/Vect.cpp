
//
// Easy Scanner System
//
// object vect.cpp  ベクトルデータ関係関数
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include "math.h"

#include "fsf1font.h"
#include "vect.h"

#define	DIVCALC	1

extern "C" {
	void Dispatch();
	int	senvchckstopflag();
	int senvchckflag(int flag);
	int senvcheckrejionmode();
	int basestrgcntget(char far *strg);
	void fdxflinesave(DBL x1, DBL y1, DBL x2, DBL y2);
	void fdxfarcsave(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void fdxfbezesave(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	void fdxffreecurvesave(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	int epsfstartrejion();
	int epsfstartnorejion();
	int epsfendnorejion();
	int epsfendrejion();
	int epsflinesave(DBL x1, DBL y1, DBL x2, DBL y2);
	int epsfarcsave(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	int epsfbezesave(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	int epsffreecurvesave(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void curveroundtrns(DBL srcx, DBL srcy, DBL x0, DBL y0, DBL rag, DBL *dstx, DBL *dsty);
	int curve3pcenterget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL FAR *x0, DBL FAR *y0);
	int curve3pragrget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0, DBL FAR *rag01, DBL FAR *rag03, DBL FAR *r);
	void curvebezecontrolpointget(DBL x1, DBL y1, DBL x2, DBL y2, DBL rag, DBL *xp, DBL *yp);
	int curvecenterget(DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL *x0, DBL *y0);
	int curvedivpointget(DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL rag2, DBL *x12, DBL *y12);
	BOOL curvedivragchk(DBL rag1, DBL rag2);
	void curvenoragtocurverag(DBL *rag1, DBL *rag2);
	int curveragnoget(DBL rag);
	int curveragrget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x0, DBL y0, DBL *rag01, DBL *rag02, DBL *r);
	void curvesymxytrns(DBL a, DBL b, DBL t1, DBL t2, DBL *x, DBL *y);
	int curvecrosslineinareachk(dlinetype line1, dlinetype line2);
	int curvecrosslinelinexyget(dlinetype line1, dlinetype line2, DBL *x, DBL *y);
	int curveonlinechk(DBL x1, DBL y1, DBL x2, DBL y2, DBL x, DBL y);
	void curvebezetpointget(dbezetype bz, DBL t, DBL *x, DBL *y);
	int curvenkindget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void mathmtrxtrns(DBL *x, DBL *y);
	BOOL  pdrwatrarcchk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrbezechk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrcurvechk(int atr1);
	void  pdrwmove(HDC PaintDC, DBL x1, DBL y1);
	void  pdrwline(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2);
	void  pdrwarc(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void  pdrwbeze(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	void  pdrwbezecntrl(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	void  pdrwfreecurve(HDC PaintDC, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL   pdrwendragget(int atr1, int atr2, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL   pdrwstartragget(int atr0, int atr1, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2);
	int pdrwreverseatrget(int atr1, int atr2);
	BOOL pdrwatrcurvecontchk(int atr1);

	void  plgnfillmove(HDC PaintDC, DBL x1, DBL y1);
	void  plgnfillline(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2);
	void  plgnfillarc(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void  plgnfillbeze(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	void  plgnfillfreecurve(HDC PaintDC,int atr0,int atr1,int atr2,DBL x0,DBL y0,DBL x1,DBL y1,DBL x2,DBL y2,DBL x3,DBL y3);
	void  poutroundkindset(int kind);
	int   poutomitmodecheck();
	void poutsetoverlapflag(int flag);
	void poutgetdeepcut(LPDBL mm);
	int poutcheckoverlapend();
	void  poutarc(int nComID, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	int   poutareachk(DBL sx, DBL sy, DBL ex, DBL ey);
	void  poutbeze(int nComID, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	int   poutfillflagchk();
	void  poutfreecurve(int nComID, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void  poutline(int nComID, DBL sx, DBL sy, DBL ex, DBL ey);
	void  poutmove(int nComID, DBL dx, DBL dy);
	void  poutoutline(int nComID, DBL sx, DBL sy, DBL ex, DBL ey);
	void  poutrejionend(int nComID);
	void  poutrejionstart(int nComID);
	int   poutpaintmodechk();
	int poutchkdeepcut(DBL x0,DBL y0,DBL x1,DBL y1,DBL x2,DBL y2);
	void poutstartdeepcut(DBL x1, DBL y1, DBL x2, DBL y2);
	void poutenddeepcut(DBL x1, DBL y1, DBL x2, DBL y2);
	void  scrndxymmtorltv(LPDBL x, LPDBL y);
	void  plotdxymmtostep(LPDBL x, LPDBL y);
	int strcttrnsboxspoint(DBL *x, DBL *y);
	int strcttrnsarcpoint(DBL *x, DBL *y);
	void scrnsetdelpen(HDC PaintDC);
	void scrnsetfigupen(HDC PaintDC);
	void scrnsetslctpen(HDC PaintDC);
	void scrnsetnoslctpen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	int senvcheckverjion();
	HPEN	senvgetsubpen();
	HPEN	senvgetlinepen();
	HPEN	senvgetmainpen();
	int	senvcheckdrawmode();
	int	senvcheckdrawpoint();
	int	senvcheckdrawbeze();
	int senvgetcrtfillpenno();
	int senvgetcrtlinepenno();
	DBL senvgetcrtlinethick();
	HBRUSH senvgetconrbrush(int *nthick);
	HPEN senvgetconrpen();
	int	senvcheckconrmode();
	void senvgetpointdotsize(int *ndot);
	void senvsetpointdotsize(int ndot);
	void senvgetpenrgb(int pno, LPINT rval, LPINT gval, LPINT bval);
	int senvgetpenno(int rval, int gval, int bval);
}

IMPLEMENT_SERIAL(CVect, CObject, 3)
CVect::CVect()
{
}

CVect::CVect(int nDummy)
:CData(nDummy)
{
	m_pPListTop = new CPList(nDummy);
	m_nFillPen =  senvgetcrtfillpenno();
	m_nOutLinePen = senvgetcrtlinepenno();
	m_dOutLineWidth = senvgetcrtlinethick();
	m_nPointCount = 0;	
}

CVect::~CVect()
{
	CPList*	pPList;
	CPList*	nextpPList;

	pPList = m_pPListTop;
	if(pPList == NULL){
		return;
	}
	while(1){
		nextpPList = pPList->GetNext();
		delete(pPList);
		if(nextpPList == NULL){
			break;
		}
		pPList = nextpPList;
	}
}

int CVect::CheckID()
{
	return(BASEVECTID);
}

void CVect::Serialize(CArchive& ar)
{
	int		pp;
	int		atr;
	DBL		dx, dy;
	int		r, g, b;
	COLORREF	clr;

	if (ar.IsStoring()){
		ar << m_nPointCount;
		if(m_nFillPen != -1){
			if(m_nFillPen == 0){
				r = 0; g = 0; b = 0;
			}else{
				senvgetpenrgb(m_nFillPen, &r, &g, &b);
			}
			clr = RGB(r, g, b) | 0x01000000;
			ar << clr;
		}else{
			ar << m_nFillPen;
		}

		if(m_nOutLinePen != -1){
			if(m_nOutLinePen == 0){
				r = 0; g = 0; b = 0;
			}else{
				senvgetpenrgb(m_nOutLinePen, &r, &g, &b);
			}
			clr = RGB(r, g, b) | 0x01000000;
			ar << clr;
		}else{
			ar << m_nOutLinePen;
		}

		ar << m_dOutLineWidth;
		for(pp = 0; pp < m_nPointCount; pp++){
			GetAtrDXY(pp, &atr, &dx, &dy);
			ar << atr;
			ar << dx;
			ar << dy;
		}
	}else{
		ar >> m_nPointCount;
		ar >> m_nFillPen;
		if(m_nFillPen != -1
		&& m_nFillPen & 0x01000000){
			clr = m_nFillPen & 0x00ffffff;
			r = GetRValue(clr); 
			g = GetGValue(clr); 
			b = GetBValue(clr);
			m_nFillPen = senvgetpenno(r, g, b);
		}
		ar >> m_nOutLinePen;
		if(m_nOutLinePen != -1
		&& m_nOutLinePen & 0x01000000){
			clr = m_nOutLinePen & 0x00ffffff;
			r = GetRValue(clr); 
			g = GetGValue(clr); 
			b = GetBValue(clr);
			m_nOutLinePen = senvgetpenno(r, g, b);
		}
		ar >> m_dOutLineWidth;
		for(pp = 0; pp < m_nPointCount; pp++){
			ar >> atr;
			ar >> dx;
			ar >> dy;
			if(dx < -1000000 || 1000000 < dx){
				dx = 0;
			}
			if(dy < -1000000 || 1000000 < dy){
				dy = 0;
			}
			SetAtrDXY(pp, atr, dx, dy);
		}
		// ar >>
	}

	CData::Serialize(ar);
	if (!(ar.IsStoring()))
	{
		RenewMiniMax();
	}
}

//p 内部データーが空の状態であれば削除しすべてが空のときTRUEを返す
int CVect::CheckNullData()
{
	if(m_nPointCount == 0){
		return(TRUE);
	}
	return(FALSE);
}

BOOL CVect::IsVectObject()
{
	return(TRUE);
}

void CVect::GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	*minix = m_dMinix;
	*miniy = m_dMiniy;
	*maxx = m_dMaxx;
	*maxy = m_dMaxy;
}

void CVect::RenewMiniMax()
{
	int		pp;
	int		p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, x1, x2, x3, x4;
	DBL		y0, y1, y2, y3, y4;
	DBL		rag1, rag2;


	m_dMinix = INTMAX;
	m_dMiniy = INTMAX;
	m_dMaxx = -INTMAX;
	m_dMaxy = -INTMAX;
	if(m_nPointCount == 1){
		GetDXY(0, &x1, &y1);
		m_dMinix = min(x1, m_dMinix);
		m_dMiniy = min(y1, m_dMiniy);
		m_dMaxx = max(x1, m_dMaxx);
		m_dMaxy = max(y1, m_dMaxy);
		return;
	}
	if(m_nPointCount == 2){
		GetDXY(0, &x1, &y1);
		GetDXY(1, &x2, &y2);
		m_dMinix = min(x1, m_dMinix);
		m_dMinix = min(x2, m_dMinix);
		m_dMiniy = min(y1, m_dMiniy);
		m_dMiniy = min(y2, m_dMiniy);
		m_dMaxx = max(x1, m_dMaxx);
		m_dMaxx = max(x2, m_dMaxx);
		m_dMaxy = max(y1, m_dMaxy);
		m_dMaxy = max(y2, m_dMaxy);
		return;
	}
	for(pp = 0; pp < m_nPointCount;){
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				RenewMiniMaxLine(x1, y1, x2, y2);
				RenewMiniMaxLine(x2, y2, x3, y3);
			}
			else if(p2 == 0){
				RenewMiniMaxLine(x1, y1, x2, y2);
			}
			else{
				RenewMiniMaxBeze(x1, y1, x2, y2, x3, y3, x4, y4);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				RenewMiniMaxArc(x1, y1, x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == OFF){
				RenewMiniMaxLine(x1, y1, x2, y2);
			}
			else{
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					RenewMiniMaxArc(x0, y0, x1, y1, x2, y2);
				}
				else{
					rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
					rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);
					curvenoragtocurverag(&rag1, &rag2);
					x4 = x2; y4 = y2;
					curvebezecontrolpointget(x1, y1, x4, y4, rag1, &x2, &y2);
					curvebezecontrolpointget(x4, y4, x1, y1, rag2, &x3, &y3);
					RenewMiniMaxBeze(x1, y1, x2, y2, x3, y3, x4, y4);
				}
			}
			pp += 1;
		}
	}
}

CData * CVect::CopyData(DBL movex, DBL movey)
{
	CVect*	pcnewData;
	int		pp;
	int		atr;
	DBL		x, y;

	pcnewData = (CVect*)NewDataCreate(BASEVECTID);
	for(pp = 0; pp < m_nPointCount; pp++){
		GetAtrDXY(pp, &atr, &x, &y);
		pcnewData->SetAtrDXY(pp, atr, x+movex, y+movey);
	}
	pcnewData->SetPointCount(m_nPointCount);
	pcnewData->SetFillPen(m_nFillPen);
	pcnewData->SetOutLinePen(m_nOutLinePen);
	pcnewData->SetOutLineWidth(m_dOutLineWidth);
	if(CheckFlag(REJIONFLAG) == OFF){
		pcnewData->OffFlag(REJIONFLAG);
	}
	pcnewData->RenewMiniMax();
	return((CData*)pcnewData);
}

void CVect::MoveTrns(DBL movex, DBL movey)
{
	int		pp;
	DBL		x, y;

	for(pp = 0; pp < m_nPointCount; pp++){
		GetDXY(pp, &x, &y);
		x = x + movex;
		y = y + movey;
		SetDXY(pp, x, y);
	}
}

void CVect::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	int		pp;
	DBL		x, y;

	for(pp = 0; pp < m_nPointCount; pp++){
		GetDXY(pp, &x, &y);
		x = (x - cntrx) * timex + cntrx;
		y = (y - cntry) * timey + cntry;
		x = floor(x*100) / 100;
		y = floor(y*100) / 100;
		SetDXY(pp, x, y);
	}
}

void CVect::SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
{
	int		pp;
	DBL		x, y, xx, yy;

	for(pp = 0; pp < m_nPointCount; pp++){
		GetDXY(pp, &x, &y);
		x = x - cntrx;
		y = y - cntry;
		xx = x + y * ddx;
		yy = x * ddy + y;
		xx = floor(xx*100) / 100;
		yy = floor(yy*100) / 100;
		x = xx + cntrx;
		y = yy + cntry;
		SetDXY(pp, x, y);
	}
}

void CVect::MirrorTrns(DBL a, DBL b, DBL t1, DBL t2)
{
	int		pp;
	DBL		x, y;

	for(pp = 0; pp < m_nPointCount; pp++){
		GetDXY(pp, &x, &y);
		curvesymxytrns(a, b, t1, t2, &x, &y);
		x = floor(x*100) / 100;
		y = floor(y*100) / 100;
		SetDXY(pp, x, y);
	}
}

void CVect::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	int		pp;
	DBL		x, y, xx, yy;

	for(pp = 0; pp < m_nPointCount; pp++){
		GetDXY(pp, &x, &y);
		x = x - cntrx;
		y = y - cntry;
		xx = x * tcos - y * tsin;
		yy = x * tsin + y * tcos;
		xx = floor(xx*100) / 100;
		yy = floor(yy*100) / 100;
		x = xx + cntrx;
		y = yy + cntry;
		SetDXY(pp, x, y);
	}
}

void CVect::StrctBoxsTrns()
{
	int		pp;
	DBL		x, y;

	for(pp = 0; pp < m_nPointCount; pp++){
		GetDXY(pp, &x, &y);
		if(strcttrnsboxspoint(&x, &y) != FAIL){
			x = floor(x*100) / 100;
			y = floor(y*100) / 100;
			SetDXY(pp, x, y);
		}
	}
}

void CVect::MtrxTrns()
{
	int		pp;
	DBL		x, y;

	for(pp = 0; pp < m_nPointCount; pp++){
		GetDXY(pp, &x, &y);
		mathmtrxtrns(&x, &y);
		x = floor(x*100) / 100;
		y = floor(y*100) / 100;
		SetDXY(pp, x, y);
	}
}

void CVect::ReverceRejion()
{
	int		nDummy;
	CVect*	pcVect;
	int		pp, sp, dp;
	int		atr1, atr2;
	DBL		x1, y1, x2, y2;

	nDummy = 0;
	pcVect = new CVect(nDummy);
	for(pp = 0; pp < m_nPointCount; pp++){
		GetAtrDXY(pp, &atr1, &x1, &y1);
		pcVect->SetAtrDXY(pp, atr1, x1, y1);
	}

	if(CheckFlag(REJIONFLAG) == ON){
		pcVect->GetAtrDXY(0, &atr1, &x1, &y1);
		for(sp = m_nPointCount-1, dp = 0; sp >= 0; sp--, dp++){
			pcVect->GetAtrDXY(sp, &atr2, &x2, &y2);
			atr1 = pdrwreverseatrget(atr1, atr2);
			SetAtrDXY(dp, atr1, x1, y1);
			atr1 = atr2;
			x1 = x2;
			y1 = y2;
		}
	}
	else{
		pcVect->GetAtrDXY(m_nPointCount-1, &atr1, &x1, &y1);
		for(sp = m_nPointCount-2, dp = 0; sp >= 0; sp--, dp++){
			pcVect->GetAtrDXY(sp, &atr2, &x2, &y2);
			atr1 = pdrwreverseatrget(atr1, atr2);
			SetAtrDXY(dp, atr1, x1, y1);
			atr1 = atr2;
			x1 = x2;
			y1 = y2;
		}
		SetAtrDXY(dp, atr1, x1, y1);
	}
	delete(pcVect);
}

CData* CVect::SrchNearOnLinePoint(DBL xx, DBL yy, DBL *retlen, DBL *retx, DBL *rety, int *retpp)
{
	int		anspp, pp;
	int		p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3;
	DBL		ansx, ansy;
	DBL		anslen;

	anslen = INTMAX;
	anspp = -1;
	for(pp = 0; pp < m_nPointCount;){
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		GetAtr(p0, &atr0);
		GetAtr(p1, &atr1);
		GetAtr(p2, &atr2);
		GetAtr(p3, &atr3);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				GetNearLine(xx, yy, p1, p2, &anslen, &ansx, &ansy, &anspp);
				GetNearLine(xx, yy, p2, p3, &anslen, &ansx, &ansy, &anspp);
			}
			else if(p2 == 0){
				GetNearLine(xx, yy, p1, p2, &anslen, &ansx, &ansy, &anspp);
			}
			else{
				GetNearBeze(xx, yy, p1, p2, p3, p4, &anslen, &ansx, &ansy, &anspp);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				GetNearArc(xx, yy, p1, p2, p3, &anslen, &ansx, &ansy, &anspp);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == OFF){
				GetNearLine(xx, yy, p1, p2, &anslen, &ansx, &ansy, &anspp);
			}
			else{
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					GetNearArc(xx, yy, p0, p1, p2, &anslen, &ansx, &ansy, &anspp);
				}
				else{
					GetNearCurve(xx, yy, p0, p1, p2, p3, &anslen, &ansx, &ansy, &anspp);
				}
			}
			pp += 1;
		}
	}
	*retlen = anslen;
	*retx = ansx;
	*rety = ansy;
	*retpp = anspp;
	return(this);
}

CData* CVect::SrchData(DBL xx, DBL yy, DBL * retlen)
{
	int		mode;
	DBL		len;
	DBL		x, y;
	int		pp;

	*retlen = INTMAX;
	mode = senvcheckdrawmode(); 
	if(mode == PREVIEW){
		if(SrchInArea(xx, yy) == TRUE){
			len = 0;
		}
		else{
			SrchNearOnLinePoint(xx, yy, &len, &x, &y, &pp);
			if(CheckFlag(REJIONFLAG) == OFF){
				len /= 4.0;
			}
		}
	}
	else{
		SrchNearOnLinePoint(xx, yy, &len, &x, &y, &pp);
	}
	if(*retlen > len){
		*retlen = len;
	}
	return(this);
}

CData* CVect::SrchDataBoxs(DBL xx, DBL yy, DBL * retlen)
{
	DBL		len;
	DBL		x, y;
	int		pp;

	if(CheckPointIn(xx, yy) == TRUE){
		*retlen = 0;
	}
	else{
		SrchNearOnLinePoint(xx, yy, &len, &x, &y, &pp);
		*retlen = len;
	}
	//*retlen = GetPreLengthBoxs(xx, yy, m_dMinix, m_dMiniy, m_dMaxx, m_dMaxy);
	return(this);
}

CData* CVect::SrchNearPoint(DBL xx, DBL yy, DBL *retlen, int *retpp)
{
	int		anspp, pp;
	DBL		anslen, tlen;
	DBL		x, y;

	anslen = INTMAX;
	anspp = -1;
	for(pp = 0; pp < m_nPointCount; pp++){
		GetDXY(pp, &x, &y);
		tlen = hypot(x - xx, y - yy);
		if(tlen < anslen && !NEAR0(tlen)){
			anslen = tlen;
			anspp = pp;
		}
	}
	*retlen = anslen;
	*retpp = anspp;

	return(this);
}

// プロッター出力関係コマンド
void CVect::PlotOut(int nComID)
{
	int		ret;
	DBL		sx, sy, ex, ey;

	Dispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}
	
	sx = m_dMinix;
	sy = m_dMiniy;
	ex = m_dMaxx;
	ey = m_dMaxy;
	plotdxymmtostep(&sx, &sy);
	plotdxymmtostep(&ex, &ey);
	ret = poutareachk(sx, sy, ex, ey);
	if(ret == OFF){
		return;
	}
	SetRoundKind();
	poutsetoverlapflag(OFF);
	if(CheckFlag(REJIONFLAG) == ON){
		poutrejionstart(nComID);
		if(poutomitmodecheck() == ON){
			poutsetoverlapflag(OFF);
			RejionPlotOutOmit(nComID);
			poutsetoverlapflag(ON);
			RejionPlotOutOmit(nComID);
			poutsetoverlapflag(OFF);
		}
		else{
			poutsetoverlapflag(OFF);
			RejionPlotOut(nComID);
			poutsetoverlapflag(ON);
			RejionPlotOut(nComID);
			poutsetoverlapflag(OFF);
		}		
		poutrejionend(nComID);
	}
	else{
		poutrejionstart(nComID);
		if(poutomitmodecheck() == ON){
			NoRejionPlotOutOmit(nComID);
		}
		else{
			NoRejionPlotOut(nComID);
		}
		poutrejionend(nComID);
	}
}

void CVect::AtntionDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	scrnsetslctpen(pcDC->m_hDC);
	PlgnDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CVect::NoactDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	scrnsetnoslctpen(pcDC->m_hDC);
	PlgnDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CVect::DelDraw(CDC *pcDC)
{
	scrnsetdelpen(pcDC->m_hDC);
	PlgnDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CVect::PLDraw(CDC *pcDC)
{
	scrnsetfigupen(pcDC->m_hDC);
	PlgnDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CVect::OnDraw(CDC *pcDC)
{
	int		mode;
	// この関数は呼ばれないはず
	if(senvchckstopflag() == ON){
		return;
	}
	mode = senvcheckdrawmode(); 
	if(mode == PREVIEW
	|| mode == BLACKVIEW
	|| mode == PRINTVIEW
	|| mode == DIRCTVIEW
	|| mode == METAFVIEW){
		FillDraw(pcDC);
	}
	else{
		PlgnDraw(pcDC);
	}
}

void CVect::OutlineDraw(CDC *pcDC)
{
	PlgnDraw(pcDC);
}

void CVect::FillDraw(CDC *pcDC)
{
	if(CheckFlag(REJIONFLAG) == OFF){
		return;
	}
	FillRejionDrawOut(pcDC);
}

void CVect::PlgnDraw(CDC *pcDC)
{
	HPEN	pen, oldpen;
	HBRUSH	brush, oldbrush;
	int		nthick;

	if(CheckFlag(REJIONFLAG) == ON){
		//if(m_nPointCount >= 1){
			PlgnRejionDrawOut(pcDC);
		//}
		if(CheckFlag(ACTIVEFLAG) == ON){
			pen = senvgetsubpen();
			if(pen != NULL){
				PointRejionDrawOut(pcDC);
			}
		}
		brush = senvgetconrbrush(&nthick);
		if(senvcheckconrmode() == ON
		&& brush != NULL && nthick >= 3){
			pen = senvgetconrpen();
			oldpen = (HPEN)SelectObject(pcDC->m_hDC, pen);
			oldbrush = (HBRUSH)SelectObject(pcDC->m_hDC, brush);
			PlgnRejionConrDrawOut(pcDC, nthick);
			SelectObject(pcDC->m_hDC, oldbrush);
			SelectObject(pcDC->m_hDC, oldpen);
		}
	}
	else{
		//if(m_nPointCount >= 1){
			PlgnNoRejionDrawOut(pcDC);
		//}
		if(CheckFlag(ACTIVEFLAG) == ON){
			pen = senvgetsubpen();
			if(pen != NULL){
				PointNoRejionDrawOut(pcDC);
			}
		}
		brush = senvgetconrbrush(&nthick);
		if(senvcheckconrmode() == ON
		&& brush != NULL && nthick >= 3){
			pen = senvgetconrpen();
			oldpen = (HPEN)SelectObject(pcDC->m_hDC, pen);
			oldbrush = (HBRUSH)SelectObject(pcDC->m_hDC, brush);
			PlgnNoRejionConrDrawOut(pcDC, nthick);
			SelectObject(pcDC->m_hDC, oldbrush);
			SelectObject(pcDC->m_hDC, oldpen);
		}
	}
}

void CVect::PointAreaDraw(CDC *pcDC, int pp1, int pp2)
{
	int		pp;
	int		ndot;

	if(m_nPointCount == 0
	|| m_nPointCount == 1){
		return;
	}

	senvgetpointdotsize(&ndot);
	senvsetpointdotsize(ndot+2);
	if(pp2 == -1){
		if(pp1 != -1){
			PointDraw(pcDC, pp1);
		}
		senvsetpointdotsize(ndot);
		return;
	}
	if(m_nPointCount == 2){
		PointDraw(pcDC, 0);
		PointDraw(pcDC, 1);
		senvsetpointdotsize(ndot);
		return;
	}
	if(pp1 == pp2){
		for(pp = 0; pp < m_nPointCount; pp++){
			PointDraw(pcDC, pp);
		}
	}
	else if(pp1 < pp2){
		for(pp = pp1; pp <= pp2; pp++){
			PointDraw(pcDC, pp);
		}
	}
	else{ // if(pp1 > pp2){
		for(pp = pp1; pp < m_nPointCount; pp++){
			PointDraw(pcDC, pp);
		}
		for(pp = 0; pp <= pp2; pp++){
			PointDraw(pcDC, pp);
		}
	}
	senvsetpointdotsize(ndot);
}

void CVect::AreaFiguDraw(CDC *pcDC, int pp1, int pp2)
{
	int		pp;
	int		atr0, atr1, atr2;
	DBL		x0, y0, x1, y1, x2, y2;

	if(m_nPointCount == 0
	|| m_nPointCount == 1){
		return;
	}

	if(pp2 == -1){
		if(pp1 != -1){
			PointDraw(pcDC, pp1);
		}
		return;
	}
	if(m_nPointCount == 2 && pp1 == 0 && pp2 == 1){
		GetAtrDXY(0, &atr1, &x1, &y1);
		GetAtrDXY(1, &atr2, &x2, &y2);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);

		pdrwmove(pcDC->m_hDC, x1, y1);
		pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
		return;
	}
	if(pp1 < pp2){
		GetAtrDXY(pp1, &atr1, &x1, &y1);
		scrndxymmtorltv(&x1, &y1);
		pdrwmove(pcDC->m_hDC, x1, y1);
		for(pp = pp1; pp < pp2;){
			pp = OneFiguDraw(pcDC, pp);
		}
	}
	else{
		GetAtrDXY(pp1, &atr1, &x1, &y1);
		scrndxymmtorltv(&x1, &y1);
		pdrwmove(pcDC->m_hDC, x1, y1);
		for(pp = pp1; pp < m_nPointCount;){
			pp = OneFiguDraw(pcDC, pp);
		}

		GetAtrDXY(m_nPointCount-1, &atr0, &x0, &y0);
		GetAtrDXY(0, &atr1, &x1, &y1);
		GetAtrDXY(1, &atr2, &x2, &y2);
		if(CheckFlag(REJIONFLAG) == ON
		&& pdrwatrarcchk(atr0, atr1, atr2) == ON){
			scrndxymmtorltv(&x0, &y0);
			pdrwmove(pcDC->m_hDC, x0, y0);
		}
		else{
			scrndxymmtorltv(&x1, &y1);
			pdrwmove(pcDC->m_hDC, x1, y1);
		}
		for(pp = 0; pp < pp2;){
			pp = OneFiguDraw(pcDC, pp);
		}
	}
}

void CVect::PointDraw(CDC *pcDC, int pp)
{
	int		p2, atr;
	DBL		dx, dy;
	DBL		x1, y1, x2, y2, x3, y3;
	DBL		len, rag;
	int		x, y;
	int		ndot, nhdot;
	int		sx, sy, ex, ey;

	if(senvcheckdrawpoint() == OFF){
		return;
	}
	GetAtrDXY(pp, &atr, &dx, &dy);
	if(atr == BEZEATR && senvcheckdrawbeze() == OFF){
		return;
	}
	scrndxymmtorltv(&dx, &dy);
	x = (int)dx; y = (int)dy;
	senvgetpointdotsize(&ndot);
	if(senvcheckrejionmode() == ON && pp <= 1){
		x1 = dx; y1 = dy;
		p2 = PointLegal(m_nPointCount, pp+1);
		GetAtrDXY(p2, &atr, &x2, &y2);
		scrndxymmtorltv(&x2, &y2);
		len = hypot(x2-x1, y2-y1);
		x3 = (x2-x1) * (DBL)(ndot*2) / len + x1;
		y3 = (y2-y1) * (DBL)(ndot*2) / len + y1;
		x2 = x3;
		y2 = y3;
		rag = M_PI * 3 / 4;
		curveroundtrns(x2, y2, x1, y1, rag, &x2, &y2);
		curveroundtrns(x3, y3, x1, y1, -rag, &x3, &y3);
		sx = (int)x1; sy = (int)y1;
		ex = (int)x2; ey = (int)y2;
		pcDC->MoveTo(ex, ey);
		pcDC->LineTo(sx, sy);
		ex = (int)x3; ey = (int)y3;
		pcDC->LineTo(ex, ey);
	}
	nhdot = ndot/2;
	sx = x - nhdot;
	sy = y - nhdot;
	ex = sx + ndot;
	ey = sy + ndot;
	pcDC->MoveTo(sx, sy);
	pcDC->LineTo(sx, ey);
	pcDC->LineTo(ex, ey);
	pcDC->LineTo(ex, sy);
	pcDC->LineTo(sx, sy);
	if(atr != BEZEATR){
		pcDC->MoveTo(sx, sy);
		pcDC->LineTo(ex, ey);
		pcDC->MoveTo(ex, sy);
		pcDC->LineTo(sx, ey);
	}
}

int CVect::OneFiguDraw(CDC *pcDC, int pp)
{
	int		p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;

	p0 = PointLegal(m_nPointCount, pp-1);
	p1 = PointLegal(m_nPointCount, pp+0);
	p2 = PointLegal(m_nPointCount, pp+1);
	p3 = PointLegal(m_nPointCount, pp+2);
	p4 = PointLegal(m_nPointCount, pp+3);
	if(CheckFlag(REJIONFLAG) == OFF){
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
	}
	GetAtrDXY(p0, &atr0, &x0, &y0);
	GetAtrDXY(p1, &atr1, &x1, &y1);
	GetAtrDXY(p2, &atr2, &x2, &y2);
	GetAtrDXY(p3, &atr3, &x3, &y3);
	GetAtrDXY(p4, &atr4, &x4, &y4);
	scrndxymmtorltv(&x0, &y0);
	scrndxymmtorltv(&x1, &y1);
	scrndxymmtorltv(&x2, &y2);
	scrndxymmtorltv(&x3, &y3);
	scrndxymmtorltv(&x4, &y4);
	if(CheckFlag(REJIONFLAG) == ON){
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				pdrwmove(pcDC->m_hDC, x1, y1);
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
				pdrwline(pcDC->m_hDC, x2, y2, x3, y3);
			}
			else if(p2 == 0){
				pdrwmove(pcDC->m_hDC, x1, y1);
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			else{
				pdrwmove(pcDC->m_hDC, x1, y1);
				pdrwbeze(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
				if(CheckFlag(ACTIVEFLAG) == ON && senvcheckdrawbeze() == ON){
					pdrwbezecntrl(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				pdrwmove(pcDC->m_hDC, x1, y1);
				pdrwarc(pcDC->m_hDC, x1, y1, x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == OFF){
				pdrwmove(pcDC->m_hDC, x1, y1);
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			else{
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					pdrwmove(pcDC->m_hDC, x0, y0);
					pdrwarc(pcDC->m_hDC, x0, y0, x1, y1, x2, y2);
				}
				else{
					pdrwmove(pcDC->m_hDC, x1, y1);
					pdrwfreecurve(pcDC->m_hDC, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				}
			}
			pp += 1;
		}
	}
	else{
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				pdrwmove(pcDC->m_hDC, x1, y1);
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
				pdrwline(pcDC->m_hDC, x2, y2, x3, y3);
			}
			else if(p3 == 0){
				pdrwmove(pcDC->m_hDC, x1, y1);
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			else{
				pdrwmove(pcDC->m_hDC, x1, y1);
				pdrwbeze(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
				if(CheckFlag(ACTIVEFLAG) == ON && senvcheckdrawbeze() == ON){
					pdrwbezecntrl(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				pdrwmove(pcDC->m_hDC, x1, y1);
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			else{
				pdrwmove(pcDC->m_hDC, x1, y1);
				pdrwarc(pcDC->m_hDC, x1, y1, x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == OFF){
				pdrwmove(pcDC->m_hDC, x1, y1);
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			else{
				pdrwmove(pcDC->m_hDC, x1, y1);
				pdrwfreecurve(pcDC->m_hDC, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
			}
			pp += 1;
		}
	}
	return(pp);
}

// 以下　private:

void CVect::GetNearLine(DBL xx, DBL yy, int p1, int p2, DBL *retlen, DBL *retx, DBL *rety, int *retpp)
{
	DBL	x1, y1, x2, y2;
	DBL	len;
	DBL	x, y;

	GetDXY(p1, &x1, &y1);
	GetDXY(p2, &x2, &y2);
	len = hypot(x1 - xx, y1 - yy);
	if((*retlen) > len){
		*retlen = len;
		*retx = x1;
		*rety = y1;
		*retpp = p1;
	}
	len = hypot(x2 - xx, y2 - yy);
	if((*retlen) > len){
		*retlen = len;
		*retx = x2;
		*rety = y2;
		*retpp = p2;
	}
	if(GetNearOnlinePoint(xx, yy, x1, y1, x2, y2, &len, &x, &y) == ON){
		if((*retlen) > len){
			*retlen = len;
			*retx = x;
			*rety = y;
			*retpp = p1;
		}
	}
}

void CVect::GetNearBeze(DBL xx, DBL yy, int p1, int p2, int p3, int p4, DBL *retlen, DBL *retx, DBL *rety, int *retpp)
{
	DBL	x1, y1, x2, y2, x3, y3, x4, y4;
	DBL	len;
	DBL	x, y;

	GetDXY(p1, &x1, &y1);
	GetDXY(p2, &x2, &y2);
	GetDXY(p3, &x3, &y3);
	GetDXY(p4, &x4, &y4);
	len = hypot(x1 - xx, y1 - yy);
	if((*retlen) > len){
		*retlen = len;
		*retx = x1;
		*rety = y1;
		*retpp = p1;
	}
	len = hypot(x4 - xx, y4 - yy);
	if((*retlen) > len){
		*retlen = len;
		*retx = x4;
		*rety = y4;
		*retpp = p4;
	}
	if(GetNearOnBezePoint(xx, yy, x1, y1, x2, y2, x3, y3, x4, y4, &len, &x, &y) == ON){
		if((*retlen) > len){
			*retlen = len;
			*retx = x;
			*rety = y;
			*retpp = p1;
		}
	}
}

void CVect::GetNearArc(DBL xx, DBL yy, int p1, int p2, int p3, DBL *retlen, DBL *retx, DBL *rety, int *retpp)
{
	DBL		x1, y1, x2, y2, x3, y3;
	DBL		len;
	DBL		x, y;
	int		stat;
	DBL		x0, y0, rag01, rag03, dr;

	GetDXY(p1, &x1, &y1);
	GetDXY(p2, &x2, &y2);
	GetDXY(p3, &x3, &y3);
	len = hypot(x1 - xx, y1 - yy);
	if((*retlen) > len){
		*retlen = len;
		*retx = x1;
		*rety = y1;
		*retpp = p1;
	}
	len = hypot(x3 - xx, y3 - yy);
	if((*retlen) > len){
		*retlen = len;
		*retx = x3;
		*rety = y3;
		*retpp = p3;
	}
	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat == FAIL){
		if(GetNearOnlinePoint(xx, yy, x1, y1, x3, y3, &len, &x, &y) == ON){
			if((*retlen) > len){
				*retlen = len;
				*retx = x;
				*rety = y;
				*retpp = p1;
			}
		}
		return;
	}
	stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &rag01, &rag03, &dr);
	if(stat == FAIL){
		if(GetNearOnlinePoint(xx, yy, x1, y1, x3, y3, &len, &x, &y) == ON){
			if((*retlen) > len){
				*retlen = len;
				*retx = x;
				*rety = y;
				*retpp = p1;
			}
		}
		return;
	}
	if(GetNearOnEnkoPoint(xx, yy, x0, y0, x1, y1, x3, y3, dr, rag01, rag03, &len, &x, &y) == ON){
		if((*retlen) > len){
			*retlen = len;
			*retx = x;
			*rety = y;
			*retpp = p1;
		}
	}
}

void CVect::GetNearCurve(DBL xx, DBL yy, int p0, int p1, int p2, int p3, DBL *retlen, DBL *retx, DBL *rety, int *retpp)
{
	int	atr0, atr1, atr2, atr3;
	DBL	x0, y0, x1, y1, x2, y2, x3, y3;
	DBL	len;
	DBL	x, y;
	DBL	rag1, rag2;
	DBL	xp1, yp1, xp2, yp2;

	GetAtrDXY(p0, &atr0, &x0, &y0);
	GetAtrDXY(p1, &atr1, &x1, &y1);
	GetAtrDXY(p2, &atr2, &x2, &y2);
	GetAtrDXY(p3, &atr3, &x3, &y3);
	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);
	curvenoragtocurverag(&rag1, &rag2);
	curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
	curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
	if(GetNearOnBezePoint(xx, yy, x1, y1, xp1, yp1, xp2, yp2, x2, y2, &len, &x, &y) == ON){
		if((*retlen) > len){
			*retlen = len;
			*retx = x;
			*rety = y;
			*retpp = p1;
		}
	}
}

int CVect::GetNearOnBezePoint(DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4, DBL *retlen, DBL *retx, DBL *rety)
{
	DBL	len;
	DBL	x, y;
	DBL	t, t1, b1, b2, b3, b4;
	DBL	sx, sy, ex, ey;

	sx = x1; sy = y1;
	for(t = 0.1; t < 1.0; t += 0.1){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		ex = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ey = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		if(GetNearOnlinePoint(xx, yy, sx, sy, ex, ey, &len, &x, &y) == ON){
			*retlen = len;
			*retx = x;
			*rety = y;
			return(ON);
		}
		sx = ex; sy = ey;
	}
	ex = x4; ey = y4;
	if(GetNearOnlinePoint(xx, yy, sx, sy, ex, ey, &len, &x, &y) == ON){
		*retlen = len;
		*retx = x;
		*rety = y;
		return(ON);
	}
	return(OFF);
}

int CVect::GetNearOnEnkoPoint(DBL xx, DBL yy, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL dr, DBL srag, DBL erag, DBL *retlen, DBL *retx, DBL *rety)
{
	DBL		rag, degree5, steprag;
	DBL		sx, sy, ex, ey;
	DBL		len;
	DBL		x, y;
	int		count, idx;

	degree5 = M_PI / 36.0;
	count = (int)(fabs(erag - srag) / degree5);
	if(count < 5){
		count = 5;
	}
	steprag = (erag - srag) / (DBL)count;

	sx = x1; sy = y1;
	for(idx = 1, rag = srag+steprag; idx < count; idx++, rag += steprag){
		ex = cos(rag)*dr+x0;
		ey = sin(rag)*dr+y0;
		if(GetNearOnlinePoint(xx, yy, sx, sy, ex, ey, &len, &x, &y) == ON){
			*retlen = len;
			*retx = x;
			*rety = y;
			return(ON);
		}
		sx = ex; sy = ey;
	}
	ex = x2; ey = y2;
	if(GetNearOnlinePoint(xx, yy, sx, sy, ex, ey, &len, &x, &y) == ON){
		*retlen = len;
		*retx = x;
		*rety = y;
		return(ON);
	}
	return(OFF);
}

int CVect::GetNearPoint(DBL x0, DBL y0)
{
	int		pp, tpp;
	DBL		x, y;
	DBL		len, tlen;

	len = INTMAX;
	pp = 0;
	for(tpp = 0; tpp < m_nPointCount; tpp++){
		GetDXY(tpp, &x, &y);
		tlen = hypot(x0 - x, y0 - y);
		if(tlen < len){
			len = tlen;
			pp = tpp;
		}
	}
	return(pp);
}

// プロッター出力関係コマンド
void CVect::RejionPlotOut(int nComID)
{
	int		pp, p0, p1, p2, p3, p4, p5;
	int		atr0, atr1, atr2, atr3, atr4, atr5;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;

	if(m_nPointCount == 0
	|| m_nPointCount == 1){
		return;
	}
	if(m_nPointCount == 2){
		GetAtrDXY(0, &atr0, &x0, &y0);
		GetAtrDXY(1, &atr1, &x1, &y1);
		plotdxymmtostep(&x0, &y0);
		plotdxymmtostep(&x1, &y1);
		poutmove(nComID, x0, y0);
		poutline(nComID, x0, y0, x1, y1);
		return;
	}

	p0 = m_nPointCount-1;
	p1 = 0;
	p2 = 1;
	GetAtrDXY(p0, &atr0, &x0, &y0);
	GetAtrDXY(p1, &atr1, &x1, &y1);
	GetAtrDXY(p2, &atr2, &x2, &y2);
	plotdxymmtostep(&x0, &y0);
	plotdxymmtostep(&x1, &y1);
	if(poutcheckoverlapend() == OFF){
		if(pdrwatrarcchk(atr0, atr1, atr2) == ON){
			poutmove(nComID, x0, y0);
		}
		else{
			poutmove(nComID, x1, y1);
		}
	}

	for(pp = 0; pp < m_nPointCount;){
		Dispatch();
		if(senvchckflag(STOPFLAG) == ON){
			return;
		}
		if(poutcheckoverlapend() == ON){
			return;
		}
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		p5 = PointLegal(m_nPointCount, pp+4);
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
		GetAtrDXY(p5, &atr5, &x5, &y5);
		plotdxymmtostep(&x0, &y0);
		plotdxymmtostep(&x1, &y1);
		plotdxymmtostep(&x2, &y2);
		plotdxymmtostep(&x3, &y3);
		plotdxymmtostep(&x4, &y4);
		plotdxymmtostep(&x5, &y5);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				poutline(nComID, x1, y1, x2, y2);
				poutline(nComID, x2, y2, x3, y3);
			}
			else if(p2 == 0){
				poutline(nComID, x1, y1, x2, y2);
			}
			else{
				if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
					poutstartdeepcut(x1, y1, x2, y2);
				}
				poutbeze(nComID, x1, y1, x2, y2, x3, y3, x4, y4);
				if(poutchkdeepcut(x3, y3, x4, y4, x5, y5) == ON){
					poutenddeepcut(x3, y3, x4, y4);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
					poutstartdeepcut(x1, y1, x2, y2);
				}
				poutarc(nComID, x1, y1, x2, y2, x3, y3);
				if(poutchkdeepcut(x2, y2, x3, y3, x4, y4) == ON){
					poutenddeepcut(x2, y2, x3, y3);
				}
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					poutarc(nComID, x0, y0, x1, y1, x2, y2);
				}
				else{
					if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
						poutstartdeepcut(x1, y1, x2, y2);
					}
					poutfreecurve(nComID, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				}
			}
			else{
				if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
					poutstartdeepcut(x1, y1, x2, y2);
				}
				poutline(nComID, x1, y1, x2, y2);
			}
			if(poutchkdeepcut(x1, y1, x2, y2, x3, y3) == ON){
				poutenddeepcut(x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
}

void CVect::RejionPlotOutOmit(int nComID)
{
	int		pp, p0, p1, p2, p3, p4, p5;
	int		atr0, atr1, atr2, atr3, atr4, atr5;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;

	if(m_nPointCount <= 1){
		return;
	}
	if(m_nPointCount == 2){
		GetAtrDXY(0, &atr0, &x0, &y0);
		GetAtrDXY(1, &atr1, &x1, &y1);
		plotdxymmtostep(&x0, &y0);
		plotdxymmtostep(&x1, &y1);
		poutmove(nComID, x0, y0);
		poutline(nComID, x0, y0, x1, y1);
		return;
	}

	if(poutcheckoverlapend() == OFF){
		GetAtrDXY(0, &atr0, &x0, &y0);
		plotdxymmtostep(&x0, &y0);
		poutmove(nComID, x0, y0);
	}
	for(pp = 0; pp < m_nPointCount;){
		Dispatch();
		if(senvchckflag(STOPFLAG) == ON){
			return;
		}
		if(poutcheckoverlapend() == ON){
			return;
		}
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		p5 = PointLegal(m_nPointCount, pp+4);
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
		GetAtrDXY(p5, &atr5, &x5, &y5);
		plotdxymmtostep(&x0, &y0);
		plotdxymmtostep(&x1, &y1);
		plotdxymmtostep(&x2, &y2);
		plotdxymmtostep(&x3, &y3);
		plotdxymmtostep(&x4, &y4);
		plotdxymmtostep(&x5, &y5);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				poutline(nComID, x1, y1, x2, y2);
				poutline(nComID, x2, y2, x3, y3);
			}
			else if(p3 == 0){
				poutline(nComID, x1, y1, x2, y2);
			}
			else{
				if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
					poutstartdeepcut(x1, y1, x2, y2);
				}
				poutline(nComID, x1, y1, x4, y4);
				if(poutchkdeepcut(x1, y1, x4, y4, x5, y5) == ON){
					poutenddeepcut(x1, y1, x4, y4);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				poutline(nComID, x1, y1, x2, y2);
			}
			else{
				if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
					poutstartdeepcut(x1, y1, x2, y2);
				}
				poutline(nComID, x1, y1, x2, y2);
				poutline(nComID, x2, y2, x3, y3);
				if(poutchkdeepcut(x2, y2, x3, y3, x4, y4) == ON){
					poutenddeepcut(x2, y2, x3, y3);
				}
			}
			pp += 2;
		}
		else{
			if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
				poutstartdeepcut(x1, y1, x2, y2);
			}
			poutline(nComID, x1, y1, x2, y2);
			if(poutchkdeepcut(x1, y1, x2, y2, x3, y3) == ON){
				poutenddeepcut(x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
}

void CVect::NoRejionPlotOut(int nComID)
{
	int		pp, p0, p1, p2, p3, p4, p5;
	int		atr0, atr1, atr2, atr3, atr4, atr5;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;

	if(m_nPointCount <= 1){
		return;
	}
	if(m_nPointCount == 2){
		GetAtrDXY(0, &atr0, &x0, &y0);
		GetAtrDXY(1, &atr1, &x1, &y1);
		plotdxymmtostep(&x0, &y0);
		plotdxymmtostep(&x1, &y1);
		poutmove(nComID, x0, y0);
		poutline(nComID, x0, y0, x1, y1);
		return;
	}

	GetAtrDXY(0, &atr0, &x0, &y0);
	plotdxymmtostep(&x0, &y0);
	poutmove(nComID, x0, y0);

	for(pp = 0; pp < m_nPointCount-1;){
		Dispatch();
		if(senvchckflag(STOPFLAG) == ON){
			return;
		}
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		p5 = PointLegal(m_nPointCount, pp+4);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
		GetAtrDXY(p5, &atr5, &x5, &y5);
		plotdxymmtostep(&x0, &y0);
		plotdxymmtostep(&x1, &y1);
		plotdxymmtostep(&x2, &y2);
		plotdxymmtostep(&x3, &y3);
		plotdxymmtostep(&x4, &y4);
		plotdxymmtostep(&x5, &y5);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				poutline(nComID, x1, y1, x2, y2);
				poutline(nComID, x2, y2, x3, y3);
			}
			else if(p3 == 0){
				poutline(nComID, x1, y1, x2, y2);
			}
			else{
				if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
					poutstartdeepcut(x1, y1, x2, y2);
				}
				poutbeze(nComID, x1, y1, x2, y2, x3, y3, x4, y4);
				if(poutchkdeepcut(x3, y3, x4, y4, x5, y5) == ON){
					poutenddeepcut(x3, y3, x4, y4);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				poutline(nComID, x1, y1, x2, y2);
			}
			else{
				if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
					poutstartdeepcut(x1, y1, x2, y2);
				}
				poutarc(nComID, x1, y1, x2, y2, x3, y3);
				if(poutchkdeepcut(x2, y2, x3, y3, x4, y4) == ON){
					poutenddeepcut(x2, y2, x3, y3);
				}
			}
			pp += 2;
		}
		else{
			if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
				poutstartdeepcut(x1, y1, x2, y2);
			}
			if(pdrwatrcurvechk(atr1) == ON){
				poutfreecurve(nComID, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
			}
			else{
				poutline(nComID, x1, y1, x2, y2);
			}
			if(poutchkdeepcut(x1, y1, x2, y2, x3, y3) == ON){
				poutenddeepcut(x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
}

void CVect::NoRejionPlotOutOmit(int nComID)
{
	int		pp, p0, p1, p2, p3, p4, p5;
	int		atr0, atr1, atr2, atr3, atr4, atr5;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;

	if(m_nPointCount <= 1){
		return;
	}
	if(m_nPointCount == 2){
		GetAtrDXY(0, &atr0, &x0, &y0);
		GetAtrDXY(1, &atr1, &x1, &y1);
		plotdxymmtostep(&x0, &y0);
		plotdxymmtostep(&x1, &y1);
		poutmove(nComID, x0, y0);
		poutline(nComID, x0, y0, x1, y1);
		return;
	}

	GetAtrDXY(0, &atr0, &x0, &y0);
	plotdxymmtostep(&x0, &y0);
	poutmove(nComID, x0, y0);

	for(pp = 0; pp < m_nPointCount-1;){
		Dispatch();
		if(senvchckflag(STOPFLAG) == ON){
			return;
		}
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		p5 = PointLegal(m_nPointCount, pp+4);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
		GetAtrDXY(p5, &atr5, &x5, &y5);
		plotdxymmtostep(&x0, &y0);
		plotdxymmtostep(&x1, &y1);
		plotdxymmtostep(&x2, &y2);
		plotdxymmtostep(&x3, &y3);
		plotdxymmtostep(&x4, &y4);
		plotdxymmtostep(&x5, &y5);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				poutline(nComID, x1, y1, x2, y2);
				poutline(nComID, x2, y2, x3, y3);
			}
			else if(p3 == 0){
				poutline(nComID, x1, y1, x2, y2);
			}
			else{
				if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
					poutstartdeepcut(x1, y1, x2, y2);
				}
				poutline(nComID, x1, y1, x4, y4);
				if(poutchkdeepcut(x1, y1, x4, y4, x5, y5) == ON){
					poutenddeepcut(x1, y1, x4, y4);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
				poutstartdeepcut(x1, y1, x2, y2);
			}
			poutline(nComID, x1, y1, x2, y2);
			poutline(nComID, x2, y2, x3, y3);
			if(poutchkdeepcut(x2, y2, x3, y3, x4, y4) == ON){
				poutenddeepcut(x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
				poutstartdeepcut(x1, y1, x2, y2);
			}
			poutline(nComID, x1, y1, x2, y2);
			if(poutchkdeepcut(x1, y1, x2, y2, x3, y3) == ON){
				poutenddeepcut(x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
}

void CVect::FillRejionDrawOut(CDC *pcDC)
{
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;

	if(m_nPointCount <= 0){
		return;
	}

	if(m_nPointCount == 1){
		GetAtrDXY(0, &atr1, &x1, &y1);
		scrndxymmtorltv(&x1, &y1);

		plgnfillmove(pcDC->m_hDC, x1, y1);
		plgnfillline(pcDC->m_hDC, x1, y1, x1+1, y1);
		plgnfillline(pcDC->m_hDC, x1+1, y1, x1+1, y1+1);
		plgnfillline(pcDC->m_hDC, x1+1, y1+1, x1, y1+1);
		plgnfillline(pcDC->m_hDC, x1, y1+1, x1, y1);
		return;
	}
	if(m_nPointCount == 2){
		GetAtrDXY(0, &atr1, &x1, &y1);
		GetAtrDXY(1, &atr2, &x2, &y2);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);

		plgnfillmove(pcDC->m_hDC, x1, y1);
		plgnfillline(pcDC->m_hDC, x1, y1, x2, y2);
		plgnfillline(pcDC->m_hDC, x2, y2, x1, y1);
		return;
	}

	p0 = m_nPointCount-1;
	p1 = 0;
	p2 = 1;
	GetAtrDXY(p0, &atr0, &x0, &y0);
	GetAtrDXY(p1, &atr1, &x1, &y1);
	GetAtrDXY(p2, &atr2, &x2, &y2);
	scrndxymmtorltv(&x0, &y0);
	scrndxymmtorltv(&x1, &y1);
	if(pdrwatrarcchk(atr0, atr1, atr2) == ON){
		plgnfillmove(pcDC->m_hDC, x0, y0);
	}
	else{
		plgnfillmove(pcDC->m_hDC, x1, y1);
	}

	for(pp = 0; pp < m_nPointCount;){
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
		scrndxymmtorltv(&x0, &y0);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);
		scrndxymmtorltv(&x3, &y3);
		scrndxymmtorltv(&x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				plgnfillline(pcDC->m_hDC, x1, y1, x2, y2);
				plgnfillline(pcDC->m_hDC, x2, y2, x3, y3);
			}
			else if(p2 == 0){
				plgnfillline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			else{
				plgnfillbeze(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				plgnfillarc(pcDC->m_hDC, x1, y1, x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					plgnfillarc(pcDC->m_hDC, x0, y0, x1, y1, x2, y2);
				}
				else{
					plgnfillfreecurve(pcDC->m_hDC, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				}
			}
			else{
				plgnfillline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
}

void CVect::PointRejionDrawOut(CDC *pcDC)
{
	int		pp;
	int		atr;
	HPEN	pen;

	for(pp = 0; pp < m_nPointCount; pp++){
		GetAtr(pp, &atr);
		if((atr & CONTATR) == 0){
			pen = senvgetlinepen();
			SelectObject(pcDC->m_hDC, pen);
		}
		else{
			pen = senvgetmainpen();
			SelectObject(pcDC->m_hDC, pen);
		}
		PointDraw(pcDC, pp);
	}
}

void CVect::PointNoRejionDrawOut(CDC *pcDC)
{
	int		pp;
	int		atr;
	HPEN	pen;

	for(pp = 0; pp < m_nPointCount; pp++){
		GetAtr(pp, &atr);
		if((atr & CONTATR) == 0){
			pen = senvgetlinepen();
			SelectObject(pcDC->m_hDC, pen);
		}
		else{
			pen = senvgetmainpen();
			SelectObject(pcDC->m_hDC, pen);
		}
		PointDraw(pcDC, pp);
	}
}

void CVect::PlgnRejionDrawOut(CDC *pcDC)
{
	HPEN	pen;
	int		mode;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;

	if(m_nPointCount <= 0){
		return;
	}
	if(m_nPointCount == 1){
		GetAtrDXY(0, &atr1, &x1, &y1);
		scrndxymmtorltv(&x1, &y1);

		pen = senvgetlinepen();
		SelectObject(pcDC->m_hDC, pen);

		pdrwmove(pcDC->m_hDC, x1, y1);
		pdrwline(pcDC->m_hDC, x1, y1, x1+1, y1);
		pdrwline(pcDC->m_hDC, x1+1, y1, x1+1, y1+1);
		pdrwline(pcDC->m_hDC, x1+1, y1+1, x1, y1+1);
		pdrwline(pcDC->m_hDC, x1, y1+1, x1, y1);
		return;
	}
	if(m_nPointCount == 2){
		GetAtrDXY(0, &atr1, &x1, &y1);
		GetAtrDXY(1, &atr2, &x2, &y2);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);

		pen = senvgetlinepen();
		SelectObject(pcDC->m_hDC, pen);

		pdrwmove(pcDC->m_hDC, x1, y1);
		pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
		return;
	}
	p0 = m_nPointCount-1;
	p1 = 0;
	p2 = 1;
	GetAtrDXY(p0, &atr0, &x0, &y0);
	GetAtrDXY(p1, &atr1, &x1, &y1);
	GetAtrDXY(p2, &atr2, &x2, &y2);
	scrndxymmtorltv(&x0, &y0);
	scrndxymmtorltv(&x1, &y1);
	scrndxymmtorltv(&x2, &y2);
	mode = senvcheckdrawmode();
	if(mode != PRINTVIEW
	&& mode != DIRCTVIEW
	&& mode != METAFVIEW){
		if(CheckFlag(VCUTFLAG) == ON){

			pen = senvgetlinepen();
			SelectObject(pcDC->m_hDC, pen);

			pdrwmove(pcDC->m_hDC, x1-8, y1+8);
			pdrwline(pcDC->m_hDC, x1-8, y1+8, x1+8, y1-8);
			pdrwmove(pcDC->m_hDC, x1+8, y1+8);
			pdrwline(pcDC->m_hDC, x1+8, y1+8, x1-8, y1-8);
			pdrwmove(pcDC->m_hDC, x2-4, y2+8);
			pdrwline(pcDC->m_hDC, x2-4, y2+4, x2+4, y2-4);
			pdrwmove(pcDC->m_hDC, x2+4, y2+4);
			pdrwline(pcDC->m_hDC, x2+4, y2+4, x2-4, y2-4);
		}
	}
	if(pdrwatrarcchk(atr0, atr1, atr2) == ON){
		pdrwmove(pcDC->m_hDC, x0, y0);
	}
	else{
		pdrwmove(pcDC->m_hDC, x1, y1);
	}

	for(pp = 0; pp < m_nPointCount;){
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
		scrndxymmtorltv(&x0, &y0);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);
		scrndxymmtorltv(&x3, &y3);
		scrndxymmtorltv(&x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				pen = senvgetlinepen();
				SelectObject(pcDC->m_hDC, pen);

				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
				pdrwline(pcDC->m_hDC, x2, y2, x3, y3);
			}
			else if(p2 == 0){
				pen = senvgetlinepen();
				SelectObject(pcDC->m_hDC, pen);
	
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			else{
				pen = senvgetmainpen();
				SelectObject(pcDC->m_hDC, pen);

				pdrwbeze(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
				if(CheckFlag(ACTIVEFLAG) == ON && senvcheckdrawbeze() == ON){
					pdrwbezecntrl(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			pen = senvgetmainpen();
			SelectObject(pcDC->m_hDC, pen);

			if(p2 != 0){
				pdrwarc(pcDC->m_hDC, x1, y1, x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				pen = senvgetmainpen();
				SelectObject(pcDC->m_hDC, pen);

				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					pdrwarc(pcDC->m_hDC, x0, y0, x1, y1, x2, y2);
				}
				else{
					pdrwfreecurve(pcDC->m_hDC, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				}
			}
			else{
				pen = senvgetlinepen();
				SelectObject(pcDC->m_hDC, pen);

				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
}

void CVect::PlgnNoRejionDrawOut(CDC *pcDC)
{
	HPEN	pen;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;

	if(m_nPointCount <= 0){
		return;
	}
	if(m_nPointCount == 1){
		GetAtrDXY(0, &atr1, &x1, &y1);
		scrndxymmtorltv(&x1, &y1);

		pen = senvgetlinepen();
		SelectObject(pcDC->m_hDC, pen);

		pdrwmove(pcDC->m_hDC, x1, y1);
		pdrwline(pcDC->m_hDC, x1, y1, x1+1, y1);
		pdrwline(pcDC->m_hDC, x1+1, y1, x1+1, y1+1);
		pdrwline(pcDC->m_hDC, x1+1, y1+1, x1, y1+1);
		pdrwline(pcDC->m_hDC, x1, y1+1, x1, y1);
		return;
	}

	GetAtrDXY(0, &atr1, &x1, &y1);
	scrndxymmtorltv(&x1, &y1);
	pdrwmove(pcDC->m_hDC, x1, y1);

	for(pp = 0; pp < m_nPointCount-1;){
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
		scrndxymmtorltv(&x0, &y0);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);
		scrndxymmtorltv(&x3, &y3);
		scrndxymmtorltv(&x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){

				pen = senvgetlinepen();
				SelectObject(pcDC->m_hDC, pen);

				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
				pdrwline(pcDC->m_hDC, x2, y2, x3, y3);
			}
			else if(p3 == 0){

				pen = senvgetlinepen();
				SelectObject(pcDC->m_hDC, pen);

				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			else{

				pen = senvgetmainpen();
				SelectObject(pcDC->m_hDC, pen);

				pdrwbeze(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
				if(CheckFlag(ACTIVEFLAG) == ON && senvcheckdrawbeze() == ON){
					pdrwbezecntrl(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){

				pen = senvgetlinepen();
				SelectObject(pcDC->m_hDC, pen);

				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			else{

				pen = senvgetmainpen();
				SelectObject(pcDC->m_hDC, pen);

				pdrwarc(pcDC->m_hDC, x1, y1, x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){

				pen = senvgetmainpen();
				SelectObject(pcDC->m_hDC, pen);

				pdrwfreecurve(pcDC->m_hDC, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
			}
			else{

				pen = senvgetlinepen();
				SelectObject(pcDC->m_hDC, pen);

				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
}

void CVect::PlgnRejionConrDrawOut(CDC *pcDC, int nthick)
{
	int		pp, p1, p2, p3;
	int		atr1, atr2, atr3;
	DBL		x1, y1, x2, y2, x3, y3;
	DBL		x21, y21, x23, y23, dx, dy;
	DBL		xt, yt;
	DBL		rag21, rag23;
	DBL		dthick;
	POINT		pArry[4];
	int			wPolyCount[2];
	int			nRejionCount;


	if(m_nPointCount <= 2){
		return;
	}
	for(pp = 1; pp < m_nPointCount;){
		p1 = PointLegal(m_nPointCount, pp-1);
		p2 = PointLegal(m_nPointCount, pp+0);
		p3 = PointLegal(m_nPointCount, pp+1);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);
		scrndxymmtorltv(&x3, &y3);
		pp += 1;
		if((x1 == x2 && y1 == y2)
		|| (x2 == x3 && y2 == y3)){
			continue;
		}
		if((atr2 & CONTATR) != 0){
			continue;
		}
		if(atr2 & BEZEATR){
			continue;
		}
		dthick = nthick / 2;
		GetRoundRag(x1, y1, x2, y2, &rag21);
		SetRoundPoint(x2, y2, rag21, 0.0, dthick, &x21,  &y21);
		GetRoundRag(x2, y2, x3, y3, &rag23);
		SetRoundPoint(x2, y2, rag23, 0.0, dthick, &x23, &y23);
		GetConerPoint(x21, y21, rag21, x23, y23, rag23, &dx, &dy);
		SetRoundPoint(x3, y3, rag23, 0.0, dthick, &xt, &yt);
		if(curveonlinechk(x23, y23, xt, yt, dx, dy) == OFF){
			pArry[0].x = (long)x21; pArry[0].y = (long)y21;
			pArry[1].x = (long)x23; pArry[1].y = (long)y23;
			pArry[2].x = (long)dx; pArry[2].y = (long)dy;
			wPolyCount[0] = 3;
			nRejionCount = 1;
			PolyPolygon(pcDC->m_hDC, pArry, wPolyCount, nRejionCount);
			// 三角形を塗りつぶし
		}
		dthick = -nthick / 2;
		GetRoundRag(x1, y1, x2, y2, &rag21);
		SetRoundPoint(x2, y2, rag21, 0.0, dthick, &x21,  &y21);
		GetRoundRag(x2, y2, x3, y3, &rag23);
		SetRoundPoint(x2, y2, rag23, 0.0, dthick, &x23, &y23);
		GetConerPoint(x21, y21, rag21, x23, y23, rag23, &dx, &dy);
		SetRoundPoint(x3, y3, rag23, 0.0, dthick, &xt, &yt);
		if(curveonlinechk(x23, y23, xt, yt, dx, dy) == OFF){
			pArry[0].x = (long)x21; pArry[0].y = (long)y21;
			pArry[1].x = (long)x23; pArry[1].y = (long)y23;
			pArry[2].x = (long)dx; pArry[2].y = (long)dy;
			wPolyCount[0] = 3;
			nRejionCount = 1;
			PolyPolygon(pcDC->m_hDC, pArry, wPolyCount, nRejionCount);
			// 三角形を塗りつぶし
		}
	}
}

void CVect::PlgnNoRejionConrDrawOut(CDC *pcDC, int nthick)
{
	int		pp, p1, p2, p3;
	int		atr1, atr2, atr3;
	DBL		x1, y1, x2, y2, x3, y3;
	DBL		x21, y21, x23, y23, dx, dy;
	DBL		xt, yt;
	DBL		rag21, rag23;
	DBL		dthick;
	POINT		pArry[4];
	int			wPolyCount[2];
	int			nRejionCount;


	if(m_nPointCount <= 2){
		return;
	}
	for(pp = 0; pp < m_nPointCount-1;){
		p1 = PointLegal(m_nPointCount, pp-1);
		p2 = PointLegal(m_nPointCount, pp+0);
		p3 = PointLegal(m_nPointCount, pp+1);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);
		scrndxymmtorltv(&x3, &y3);
		pp += 1;
		if((x1 == x2 && y1 == y2)
		|| (x2 == x3 && y2 == y3)){
			continue;
		}
		if((atr2 & CONTATR) != 0){
			continue;
		}
		if(atr2 & BEZEATR){
			continue;
		}
		dthick = nthick / 2;
		GetRoundRag(x1, y1, x2, y2, &rag21);
		SetRoundPoint(x2, y2, rag21, 0.0, dthick, &x21,  &y21);
		GetRoundRag(x2, y2, x3, y3, &rag23);
		SetRoundPoint(x2, y2, rag23, 0.0, dthick, &x23, &y23);
		GetConerPoint(x21, y21, rag21, x23, y23, rag23, &dx, &dy);
		SetRoundPoint(x3, y3, rag23, 0.0, dthick, &xt, &yt);
		if(curveonlinechk(x23, y23, xt, yt, dx, dy) == OFF){
			pArry[0].x = (long)x21; pArry[0].y = (long)y21;
			pArry[1].x = (long)x23; pArry[1].y = (long)y23;
			pArry[2].x = (long)dx; pArry[2].y = (long)dy;
			wPolyCount[0] = 3;
			nRejionCount = 1;
			PolyPolygon(pcDC->m_hDC, pArry, wPolyCount, nRejionCount);
			// 三角形を塗りつぶし
		}
		dthick = -nthick / 2;
		GetRoundRag(x1, y1, x2, y2, &rag21);
		SetRoundPoint(x2, y2, rag21, 0.0, dthick, &x21,  &y21);
		GetRoundRag(x2, y2, x3, y3, &rag23);
		SetRoundPoint(x2, y2, rag23, 0.0, dthick, &x23, &y23);
		GetConerPoint(x21, y21, rag21, x23, y23, rag23, &dx, &dy);
		SetRoundPoint(x3, y3, rag23, 0.0, dthick, &xt, &yt);
		if(curveonlinechk(x23, y23, xt, yt, dx, dy) == OFF){
			pArry[0].x = (long)x21; pArry[0].y = (long)y21;
			pArry[1].x = (long)x23; pArry[1].y = (long)y23;
			pArry[2].x = (long)dx; pArry[2].y = (long)dy;
			wPolyCount[0] = 3;
			nRejionCount = 1;
			PolyPolygon(pcDC->m_hDC, pArry, wPolyCount, nRejionCount);
			// 三角形を塗りつぶし
		}
	}
}

// 長さを取得する関数
DBL CVect::GetLength()
{
	DBL		sumlen;
	DBL		x1, y1;
	DBL		x2, y2;

	sumlen = 0;
	if(m_nPointCount <= 1){
		return(sumlen);
	}
	if(m_nPointCount == 2){
		GetDXY(0, &x1, &y1);
		GetDXY(1, &x2, &y2);
		sumlen = GetLineLength(x1, y1, x2, y2);
		return(sumlen);
	}
	if(CheckFlag(REJIONFLAG) == ON){
		sumlen = GetRejionLength();
	}
	else{
		sumlen = GetNorejionLength();
	}
	return(sumlen);
}

DBL CVect::GetAreaLength(int p1, int p2)
{
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	if(p1 < p2){
		sumlen = Getp1p2AreaLength(p1, p2);
	}
	else{
		len = Getp1p2AreaLength(p1, m_nPointCount);
		sumlen += len;
		len = Getp1p2AreaLength(0, p2);
		sumlen += len;
	}
	return(sumlen);
}

DBL CVect::Getp1p2AreaLength(int areap1, int areap2)
{
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	if(areap1 == areap2){
		return(sumlen);
	}
	for(pp = areap1; pp < areap2;){
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
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

// 閉図形の線分の長さを求める関数
DBL CVect::GetRejionLength()
{
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	for(pp = 0; pp < m_nPointCount;){
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
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

DBL CVect::GetNorejionLength()
{
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	for(pp = 0; pp < m_nPointCount-1;){
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
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

DBL CVect::GetBezeLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
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

DBL CVect::GetArcLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

DBL CVect::GetFreeCurveLength(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

DBL CVect::GetLineLength(DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		len;

	len = hypot(x1-x2, y1-y2);
	return(len);
}

int CVect::GetFiguKind(int pp)
{
	int		p1, p2, p3, p4;
	int		atr1, atr2, atr3;

	p1 = PointLegal(m_nPointCount, pp+0);
	p2 = PointLegal(m_nPointCount, pp+1);
	p3 = PointLegal(m_nPointCount, pp+2);
	p4 = PointLegal(m_nPointCount, pp+3);
	GetAtr(p1, &atr1);
	GetAtr(p2, &atr2);
	GetAtr(p3, &atr3);
	if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
		if(p3 == 0){
			return(LINEKIND);
		}
		else if(p2 == 0){
			return(LINEKIND);
		}
		else{
			return(BEZEKIND);
		}
	}
	else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
		if(p2 == 0){
			return(LINEKIND);
		}
		else{
			return(ARCKIND);
		}
	}
	else{
		if(pdrwatrcurvechk(atr1) == ON){
			return(FCBEZEKIND);
		}
		else{
			return(LINEKIND);
		}
	}
}

int CVect::NextPointGet(int pp)
{
	int		p1, p2, p3, p4;
	int		atr1, atr2, atr3;

	p1 = PointLegal(m_nPointCount, pp+0);
	p2 = PointLegal(m_nPointCount, pp+1);
	p3 = PointLegal(m_nPointCount, pp+2);
	p4 = PointLegal(m_nPointCount, pp+3);
	GetAtr(p1, &atr1);
	GetAtr(p2, &atr2);
	GetAtr(p3, &atr3);
	if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
		if(p3 == 0){
			return(pp+1);
		}
		else if(p2 == 0){
			return(pp+1);
		}
		else{
			return(pp+3);
		}
	}
	else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
		if(p2 == 0){
			return(pp+1);
		}
		else{
			return(pp+2);
		}
	}
	else{
		return(pp+1);
	}
}

int CVect::BeforePointGet(int pp)
{
	int		p1, p2, p3, p4;
	int		atr1, atr2, atr3, atr4;

	p1 = PointLegal(m_nPointCount, pp+0);
	p2 = PointLegal(m_nPointCount, pp-1);
	p3 = PointLegal(m_nPointCount, pp-2);
	p4 = PointLegal(m_nPointCount, pp-3);
	GetAtr(p1, &atr1);
	GetAtr(p2, &atr2);
	GetAtr(p3, &atr3);
	GetAtr(p4, &atr4);
	if(pdrwatrbezechk(atr4, atr3, atr2) == ON){
		if(p2 == 0 || p3 == 0){
			return(0);
		}
		else{
			return(p4);
		}
	}
	else if(pdrwatrarcchk(atr3, atr2, atr1) == ON){
		return(p3);
	}
	else{
		return(p2);
	}
}

void CVect::GetFreeCurveBeze(int pp, dbezetype *bz)
{
	int		p0, p1, p2, p3;
	int		atr0, atr1, atr2, atr3;
	DBL		x0, x1, x2, x3;
	DBL		y0, y1, y2, y3;
	DBL	rag1, rag2;
	DBL	xp1, yp1, xp2, yp2;

	p0 = PointLegal(m_nPointCount, pp-1);
	p1 = PointLegal(m_nPointCount, pp+0);
	p2 = PointLegal(m_nPointCount, pp+1);
	p3 = PointLegal(m_nPointCount, pp+2);
	GetAtrDXY(p0, &atr0, &x0, &y0);
	GetAtrDXY(p1, &atr1, &x1, &y1);
	GetAtrDXY(p2, &atr2, &x2, &y2);
	GetAtrDXY(p3, &atr3, &x3, &y3);
	
	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);

	if(rag1 == NORAG && rag2 == NORAG){
		bz->x1 = x1; bz->y1 = y1;
		bz->x2 = x1; bz->y2 = y1;
		bz->x3 = x2; bz->y3 = y2;
		bz->x4 = x2; bz->y4 = y2;
	}
	else{
		curvenoragtocurverag(&rag1, &rag2);
		curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
		curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
		bz->x1 = x1; bz->y1 = y1;
		bz->x2 = xp1; bz->y2 = yp1;
		bz->x3 = xp2; bz->y3 = yp2;
		bz->x4 = x2; bz->y4 = y2;
	}
}

// 低レベル関数
int CVect::PointLegal(int nPmax, int pp)
{
	if(CheckFlag(REJIONFLAG) == ON){
		pp %= nPmax;
		pp += nPmax;
		pp %= nPmax;
	}
	else{
		if(nPmax <= pp){
			pp = nPmax-1;
		}
		if(0 >= pp){
			pp = 0;
		}
	}
	return(pp);
}

int CVect::GetPointCount()
{
	return(m_nPointCount);
}

void CVect::SetPointCount(int nCnt)
{
	m_nPointCount = nCnt;
}

int CVect::GetFillPen()
{
	return(m_nFillPen);
}

void CVect::SetFillPen(int npen)
{
	m_nFillPen = npen;
}

int CVect::GetOutLinePen()
{
	return(m_nOutLinePen);
}

void CVect::SetOutLinePen(int npen)
{
	m_nOutLinePen = npen;
}

DBL CVect::GetOutLineWidth()
{
	return(m_dOutLineWidth);
}

void CVect::SetOutLineWidth(DBL dw)
{
	m_dOutLineWidth = dw;
}

void CVect::AllOffPointFlag(int bit)
{
	int		pp;

	for(pp = 0; pp < m_nPointCount; pp++){
		OffPointFlag(pp, bit);
	}
}

void CVect::AllOnPointFlag(int bit)
{
	int		pp;

	for(pp = 0; pp < m_nPointCount; pp++){
		OnPointFlag(pp, bit);
	}
}

int CVect::CheckPointFlag(int idx, int bit)
{
	CPList*	pPList;
	int	tidx, atr;

	pPList = GetPList(idx, &tidx);
	pPList->GetAtr(tidx, &atr);
	if(atr & bit){
		return(ON);
	}
	return(OFF);
}

void CVect::OnPointFlag(int idx, int bit)
{
	CPList*	pPList;
	int	tidx, atr;

	pPList = GetPList(idx, &tidx);
	pPList->GetAtr(tidx, &atr);
	atr |= bit;
	pPList->SetAtr(tidx, atr);
}

void CVect::OffPointFlag(int idx, int bit)
{
	CPList*	pPList;
	int	tidx, atr;

	pPList = GetPList(idx, &tidx);
	pPList->GetAtr(tidx, &atr);
	atr &= ~bit;
	pPList->SetAtr(tidx, atr);
}

void CVect::GetAtr(int idx, int *atr)
{
	CPList*	pPList;
	int	tidx;

	pPList = GetPList(idx, &tidx);
	pPList->GetAtr(tidx, atr);
}

void CVect::SetAtr(int idx, int atr)
{
	CPList*	pPList;
	int	tidx;

	pPList = GetPList(idx, &tidx);
	pPList->SetAtr(tidx, atr);
}

void CVect::GetAtrDXY(int idx, int *atr, DBL *x, DBL *y)
{
	GetFAtrDXY(idx, atr, x, y);
	*atr &= PATRMASK;
}

void CVect::SetAtrDXY(int idx, int atr, DBL x, DBL y)
{
	atr &= PATRMASK;
	SetFAtrDXY(idx, atr, x, y);
}

void CVect::GetFAtrDXY(int idx, int *atr, DBL *x, DBL *y)
{
	CPList*	pPList;
	int	tidx;

	pPList = GetPList(idx, &tidx);
	pPList->GetAtrDXY(tidx, atr, x, y);
}

void CVect::SetFAtrDXY(int idx, int atr, DBL x, DBL y)
{
	CPList*	pPList;
	int	tidx;

	pPList = GetPList(idx, &tidx);
	pPList->SetAtrDXY(tidx, atr, x, y);
}

void CVect::GetDXY(int idx, DBL *x, DBL *y)
{
	CPList*	pPList;
	int	tidx;

	pPList = GetPList(idx, &tidx);
	pPList->GetDXY(tidx, x, y);
}

void CVect::SetDXY(int idx, DBL x, DBL y)
{
	CPList*	pPList;
	int	tidx;

	pPList = GetPList(idx, &tidx);
	pPList->SetDXY(tidx, x, y);
}

CPList* CVect::GetPList(int idx, int *tidx)
{
	CPList*	pPList;
	int	cnt, orgidx;

	*tidx = 0;
	orgidx = idx;
	pPList = m_pPListTop;
	while(1){
		cnt = pPList->GetCount();
		if(0 <= idx && idx < cnt){
			*tidx = idx;
			return(pPList);
		}
		pPList = pPList->GetNext();
		idx -= cnt;
		if(pPList == NULL){
			break;
		}
	}

	PListAddLink(idx);

	idx = orgidx;
	pPList = m_pPListTop;
	while(1){
		cnt = pPList->GetCount();
		if(0 <= idx && idx < cnt){
			*tidx = idx;
			return(pPList);
		}
		pPList = pPList->GetNext();
		idx -= cnt;
		if(pPList == NULL){
			break;
		}
	}
	// ここより先はエラー（システムダウンを避けるための処理）
	*tidx = 0;
	return(m_pPListTop);
}

void CVect::PListAddLink(int idx)
{
	int		nDummy;
	int		i, cnt;
	int		blockcnt;
	CPList*	bfpPList;
	CPList*	pPList;

	nDummy = 0;
	pPList = m_pPListTop;
	cnt = pPList->GetBlockCount();
	while(1){
		bfpPList = pPList;
		pPList = pPList->GetNext();
		if(pPList == NULL){
			break;
		}
	}
	blockcnt = idx / cnt + 1;
	for(i = 0; i < blockcnt; i++){
		pPList = new CPList(nDummy);
		if(pPList == NULL){
			break;
		}
		bfpPList->SetNext(pPList);
		bfpPList = pPList;
	}
}

void CVect::WakuSave(FILE * fp)
{

}

void CVect::InAreaDataAtntionOn(DBL sx, DBL sy, DBL ex, DBL ey)
{
	DBL	dminx, dminy, dmaxx, dmaxy;

	GetDMiniMax(&dminx, &dminy, &dmaxx, &dmaxy);
	if(sx < dminx && dmaxx < ex	
	&& sy < dminy && dmaxy < ey){
		OnFlag(ATNTION1FLAG);
	}
}

void CVect::SetAtnVectFSF1fBuf(CFSF1Font * pcFSF1Font)
{
	int		pp;
	int		atr;
	DBL		dx, dy;

	if(CheckFlag(ATNTION1FLAG) == OFF){
		return;
	}
	for(pp = 0; pp < m_nPointCount; pp++){
		GetAtrDXY(pp, &atr, &dx, &dy);
		pcFSF1Font->FBufScaleTrns(dx, dy, &dx, &dy);
		pcFSF1Font->AtrDXYCrtSet(atr, dx, dy);
		pcFSF1Font->CrtPointInc();
	}
	if(CheckFlag(REJIONFLAG) == ON){
		pcFSF1Font->RejionSet();
	}
	else{
		pcFSF1Font->NoRejionSet();
	}
}

void CVect::PointYLegal(DBL xx)
{
	int		pp;
	DBL		x, y;

	for(pp = 0; pp < m_nPointCount; pp++){
		if(CheckPointFlag(pp, PATN0ATR) == ON){
			GetDXY(pp, &x, &y);
			SetDXY(pp, xx, y);
		}
	}
	RenewMiniMax();
}

void CVect::PointXLegal(DBL yy)
{
	int		pp;
	DBL		x, y;

	for(pp = 0; pp < m_nPointCount; pp++){
		if(CheckPointFlag(pp, PATN0ATR) == ON){
			GetDXY(pp, &x, &y);
			SetDXY(pp, x, yy);
		}
	}
	RenewMiniMax();
}

void CVect::PointPicup()
{
	int		pp, dp;
	int		atr;
	DBL		x, y;
	CVect	*pcVect;

	pcVect = new CVect(BASEVECTID);
	dp = 0;
	for(pp = 0; pp < m_nPointCount; pp++){
		if(CheckPointFlag(pp, PATN0ATR) == ON){
			GetAtrDXY(pp, &atr, &x, &y);
			pcVect->SetAtrDXY(dp, atr, x, y);
			dp++;
		}
	}

	if(2 < dp){
		for(pp = 0; pp < dp; pp++){
			pcVect->GetAtrDXY(pp, &atr, &x, &y);
			SetAtrDXY(pp, atr, x, y);
		}
		m_nPointCount = dp;
	}
	delete(pcVect);
	RenewMiniMax();
}

void CVect::SetDefOutLineClr()
{
	int		penno;
	DBL		width;

	penno = senvgetcrtlinepenno();
	SetOutLinePen(penno);
	width = senvgetcrtlinethick();
	SetOutLineWidth(width);
}

void CVect::SetDefFillClr()
{
	int		penno;

	penno =  senvgetcrtfillpenno();
	SetFillPen(penno);
}

void CVect::SaveDxfFile()
{
	if(CheckFlag(REJIONFLAG) == ON){
		SaveDxfRejion();
	}
	else{
		SaveDxfNoRejion();
	}
}

int CVect::SaveDxfRejion()
{
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;

	if(m_nPointCount == 0
	|| m_nPointCount == 1){
		return(0);
	}
	if(m_nPointCount == 2){
		GetAtrDXY(0, &atr0, &x0, &y0);
		GetAtrDXY(1, &atr1, &x1, &y1);
		fdxflinesave(x0, y0, x1, y1);
		return(0);
	}

	GetAtrDXY(0, &atr0, &x0, &y0);
	fdxflinesave(x0, y0, x0, y0);
	for(pp = 0; pp < m_nPointCount;){
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				fdxflinesave(x1, y1, x2, y2);
				fdxflinesave(x2, y2, x3, y3);
			}
			else if(p2 == 0){
				fdxflinesave(x1, y1, x2, y2);
			}
			else{
				fdxfbezesave(x1, y1, x2, y2, x3, y3, x4, y4);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				fdxfarcsave(x1, y1, x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					fdxfarcsave(x0, y0, x1, y1, x2, y2);
				}
				else{
					fdxffreecurvesave(atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				}
			}
			else{
				fdxflinesave(x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
	return(0);
}

int CVect::SaveDxfNoRejion()
{
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;

	if(m_nPointCount <= 1){
		return(0);
	}
	if(m_nPointCount == 2){
		GetAtrDXY(0, &atr0, &x0, &y0);
		GetAtrDXY(1, &atr1, &x1, &y1);
		fdxflinesave(x0, y0, x1, y1);
		return(0);
	}

	GetAtrDXY(0, &atr0, &x0, &y0);
	fdxflinesave(x0, y0, x0, y0);
	for(pp = 0; pp < m_nPointCount-1;){
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				fdxflinesave(x1, y1, x2, y2);
				fdxflinesave(x2, y2, x3, y3);
			}
			else if(p3 == 0){
				fdxflinesave(x1, y1, x2, y2);
			}
			else{
				fdxfbezesave(x1, y1, x2, y2, x3, y3, x4, y4);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				fdxflinesave(x1, y1, x2, y2);
			}
			else{
				fdxfarcsave(x1, y1, x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				fdxffreecurvesave(atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
			}
			else{
				fdxflinesave(x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
	return(0);
}

void CVect::SaveEpsFile()
{
	if(CheckFlag(REJIONFLAG) == ON){
		SaveEpsRejion();
	}
	else{
		SaveEpsNoRejion();
	}
}

int CVect::SaveEpsRejion()
{
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;

	if(m_nPointCount == 0
	|| m_nPointCount == 1){
		return(0);
	}
	if(m_nPointCount == 2){
		epsfstartnorejion();
		GetAtrDXY(0, &atr0, &x0, &y0);
		GetAtrDXY(1, &atr1, &x1, &y1);
		epsflinesave(x0, y0, x1, y1);
		epsfendnorejion();
		return(0);
	}

	epsfstartrejion();
	for(pp = 0; pp < m_nPointCount;){
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				epsflinesave(x1, y1, x2, y2);
				epsflinesave(x2, y2, x3, y3);
			}
			else if(p2 == 0){
				epsflinesave(x1, y1, x2, y2);
			}
			else{
				epsfbezesave(x1, y1, x2, y2, x3, y3, x4, y4);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				epsfarcsave(x1, y1, x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					epsfarcsave(x0, y0, x1, y1, x2, y2);
				}
				else{
					epsffreecurvesave(atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				}
			}
			else{
				epsflinesave(x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
	epsfendrejion();
	return(0);
}

int CVect::SaveEpsNoRejion()
{
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;

	if(m_nPointCount <= 1){
		return(0);
	}
	if(m_nPointCount == 2){
		epsfstartnorejion();
		GetAtrDXY(0, &atr0, &x0, &y0);
		GetAtrDXY(1, &atr1, &x1, &y1);
		epsflinesave(x0, y0, x1, y1);
		epsfendnorejion();
		return(0);
	}

	epsfstartnorejion();
	for(pp = 0; pp < m_nPointCount-1;){
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
		GetAtrDXY(p0, &atr0, &x0, &y0);
		GetAtrDXY(p1, &atr1, &x1, &y1);
		GetAtrDXY(p2, &atr2, &x2, &y2);
		GetAtrDXY(p3, &atr3, &x3, &y3);
		GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				epsflinesave(x1, y1, x2, y2);
				epsflinesave(x2, y2, x3, y3);
			}
			else if(p3 == 0){
				epsflinesave(x1, y1, x2, y2);
			}
			else{
				epsfbezesave(x1, y1, x2, y2, x3, y3, x4, y4);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				epsflinesave(x1, y1, x2, y2);
			}
			else{
				epsfarcsave(x1, y1, x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				epsffreecurvesave(atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
			}
			else{
				epsflinesave(x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
	epsfendnorejion();
	return(0);
}

void CVect::OutLineOmit(DBL mm)
{
	int		pp, p0, p1, p2;
	int		atr1;
	int		dp;
	int		pcnt;
	DBL		dx, dy;
	CVect	*pcDstVect;

	if(m_nPointCount <= 2){
		return;
	}
	dp = 0;
	pcDstVect = new CVect(BASEVECTID);
	for(pp = 0; pp < m_nPointCount;){
		p0 = PointLegal(m_nPointCount, pp-1);
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
			GetAtr(p1, &atr1);
			ThreePointCalc(mm, p0, p1, p2, &dx, &dy);
			if((atr1&PATRMASK) != 0){
				PointCurvecontLegal(mm, p1, &dx, &dy);
			}
			pcDstVect->SetAtrDXY(dp, atr1, dx, dy);
			dp++;
			pp += 1;
		/*
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		GetAtr(p1, &atr1);
		GetAtr(p2, &atr2);
		GetAtr(p3, &atr3);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			ThreePointCalc(mm, p0, p1, p2, &dx, &dy);
   	    	pcDstVect->SetAtrDXY(dp, atr1, dx, dy);
    	    dp++;
			ThreePointCalc(mm, p1, p2, p3, &dx, &dy);
   	    	pcDstVect->SetAtrDXY(dp, atr2, dx, dy);
    	    dp++;
			ThreePointCalc(mm, p2, p3, p4, &dx, &dy);
   	    	pcDstVect->SetAtrDXY(dp, atr3, dx, dy);
    	    dp++;
			pp += 3;
		}
		else{
			ThreePointCalc(mm, p0, p1, p2, &dx, &dy);
			if(pdrwatrcurvecontchk(atr1) == ON){
				PointCurvecontLegal(mm, p1, &dx, &dy);
			}
			pcDstVect->SetAtrDXY(dp, atr1, dx, dy);
			dp++;
			pp += 1;
		}
		*/
	}
	pcnt = dp;
	pcDstVect->SetPointCount(pcnt);
	for(pp = 0; pp < pcnt; pp++){
		pcDstVect->GetAtrDXY(pp, &atr1, &dx, &dy);
		SetAtrDXY(pp, atr1, dx, dy);
	}
	m_nPointCount = pcnt;
	RenewMiniMax();
	delete(pcDstVect);
}

void CVect::ThreePointCalc(DBL mm, int p0, int p1, int p2, DBL *dx, DBL *dy)
{
	int		atr0, atr1, atr2;
	DBL 	sx0, sy0, sx1, sy1, sx2, sy2;
	DBL		dx1, dy1;

	GetAtrDXY(p0, &atr0, &sx0, &sy0);
	GetAtrDXY(p1, &atr1, &sx1, &sy1);
	GetAtrDXY(p2, &atr2, &sx2, &sy2);
	TreePoint(atr1, mm, sx0, sy0, sx1, sy1, sx2, sy2, &dx1, &dy1);
	*dx = dx1;
	*dy = dy1;
}

void CVect::TreePoint(int atr, DBL mm, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL *x, DBL *y)
{
	DBL	x21, y21, x23, y23, dx, dy;
	DBL	rag21, rag23;

	GetRoundRag(x1, y1, x2, y2, &rag21);
	SetRoundPoint(x2, y2, rag21, 0.0, mm, &x21,  &y21);
	GetRoundRag(x2, y2, x3, y3, &rag23);
	SetRoundPoint(x2, y2, rag23, 0.0, mm, &x23, &y23);
	GetConerPoint(x21, y21, rag21, x23, y23, rag23, &dx, &dy);
	//LegalLength(atr, mm, x2, y2, &dx, &dy);
	*x = dx; *y = dy;
}

void CVect::GetRoundRag(DBL x0, DBL y0, DBL x1, DBL y1, DBL *rag)
{
	if(x0 == x1 && y0 == y1){
		*rag = 0.0;
	}
	else{
		*rag = atan2(y1-y0, x1-x0);
	}
}

void CVect::SetRoundPoint(DBL x0, DBL y0, DBL rag, DBL x1, DBL y1, DBL *x, DBL *y)
{
	DBL	tcos, tsin;

	tcos = cos(rag);
	tsin = sin(rag);
	*x = x1 * tcos - y1 * tsin + x0;
	*y = y1 * tcos + x1 * tsin + y0;
}

void CVect::GetConerPoint(DBL x1, DBL y1, DBL rag1, DBL x2, DBL y2, DBL rag2, DBL *x, DBL *y)
{
	DBL	a1, a2, b1, b2;

	a1 = tan(rag1);
	a2 = tan(rag2);
	if(a1 > 100000.0 || a1 < -100000.0){
		b1 = x1;
		if(a2 > 100000.0 || a2 < -100000.0){
			*x = (x1 + x2) / 2.0;
			*y = (y1 + y2) / 2.0;
			return;
		}
		b2 = y2 - a2*x2;
		b1 = x1;
		*x = b1;
		*y = a2 * b1 + b2;
		return;
	}
	if(a2 > 100000.0 || a2 < -100000.0){
		b1 = y1 - a1*x1;
		b2 = x2;
		*x = b2;
		*y = a1 * b2 + b1;
		return;
	}
	b1 = y1 - a1*x1;
	b2 = y2 - a2*x2;
	if(a1 == a2){
		*x = (x1 + x2) / 2.0;
		*y = (y1 + y2) / 2.0;
	}
	else{
		*x = (b1 - b2) / (a2 - a1);
		*y = (b1*a2 - b2*a1)/(a2-a1);
	}
	if((*x) < -50000.0 || 50000.0 < (*x)
	|| (*y) < -50000.0 || 50000.0 < (*y)){
		*x = (x1 + x2) / 2.0;
		*y = (y1 + y2) / 2.0;
	}
}

void CVect::LegalLength(int atr, double mm, double x0, double y0, double *dx, double *dy)
{
	double	x, y;
	double	len;

	x = (*dx) - x0; y = (*dy) - y0;
	len = hypot(x, y);
	if(len == 0){
		return;
	}
	mm = fabs(mm);
	if(atr & CONTATR){
		*dx = ((x * mm) / len) + x0;
		*dy = ((y * mm) / len) + y0;
	}
    /*
	else if(len > fabs(dot * 2.0)){
		*dx = ((x * (mm * 2.0)) / len) + x0;
		*dy = ((y * (mm * 2.0)) / len) + y0;
	}
    */
}

void CVect::PointCurvecontLegal(DBL mm, int pp, DBL *retx, DBL *rety)
{
	DBL		dx, dy, xx, yy;
	DBL		len, subx, suby;

	dx = *retx; dy = *rety;
	GetDXY(pp, &xx, &yy);
	subx = dx - xx;
	suby = dy - yy;
	len = hypot(subx, suby);
	*retx = subx * fabs(mm) / len + xx;
	*rety = suby * fabs(mm) / len + yy;
}

void CVect::RenewMiniMaxLine(DBL x1, DBL y1, DBL x2, DBL y2)
{
	m_dMinix = min(x1, m_dMinix);
	m_dMinix = min(x2, m_dMinix);
	m_dMiniy = min(y1, m_dMiniy);
	m_dMiniy = min(y2, m_dMiniy);
	m_dMaxx = max(x1, m_dMaxx);
	m_dMaxx = max(x2, m_dMaxx);
	m_dMaxy = max(y1, m_dMaxy);
	m_dMaxy = max(y2, m_dMaxy);
}

void CVect::RenewMiniMaxArc(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	int		stat;
	DBL		x0, y0;
	DBL		srag, erag, dr;
	DBL		rag, degree5, steprag;
	DBL		sx, sy, ex, ey;
	int		count, idx;

	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat == FAIL){
		RenewMiniMaxLine(x1, y1, x2, y2);
		RenewMiniMaxLine(x2, y2, x3, y3);
		return;
	}
	stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &srag, &erag, &dr);
	if(stat == FAIL){
		RenewMiniMaxLine(x1, y1, x2, y2);
		RenewMiniMaxLine(x2, y2, x3, y3);
		return;
	}

	degree5 = M_PI / 36.0;
	count = (int)(fabs(erag - srag) / degree5);
	if(count < 5){
		count = 5;
	}
	steprag = (erag - srag) / (DBL)count;

	sx = x1; sy = y1;
	for(idx = 1, rag = srag+steprag; idx < count; idx++, rag += steprag){
		ex = cos(rag)*dr+x0;
		ey = sin(rag)*dr+y0;
		RenewMiniMaxLine(sx, sy, ex, ey);
		sx = ex; sy = ey;
	}
}

void CVect::RenewMiniMaxBeze(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL	t, t1, b1, b2, b3, b4;
	DBL	sx, sy, ex, ey;

	sx = x1; sy = y1;
	for(t = 0.1; t < 1.0; t += 0.1){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		ex = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ey = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		RenewMiniMaxLine(sx, sy, ex, ey);
		sx = ex; sy = ey;
	}
	ex = x4; ey = y4;
	RenewMiniMaxLine(sx, sy, ex, ey);
}

BOOL CVect::CheckPointIn(DBL xx, DBL yy)
{
	DBL		sumrag, srag, erag;
	int		pp, p1, p2, p3, p4;
	int		atr1, atr2, atr3;

	if(CheckFlag(REJIONFLAG) == OFF){
		return(FALSE);	
	}
	sumrag = 0;
	if(CheckPointInGetRag(xx, yy, 0, &srag) == FALSE){
		return(TRUE);
	}
	for(pp = 0; pp < m_nPointCount;){
		p1 = PointLegal(m_nPointCount, pp+0);
		p2 = PointLegal(m_nPointCount, pp+1);
		p3 = PointLegal(m_nPointCount, pp+2);
		p4 = PointLegal(m_nPointCount, pp+3);
		GetAtr(p1, &atr1);
		GetAtr(p2, &atr2);
		GetAtr(p3, &atr3);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				if(CheckPointInGetSumrag(xx, yy, p2, &srag, &erag, &sumrag) == FALSE){
					return(TRUE);
				}
				if(CheckPointInGetSumrag(xx, yy, p3, &srag, &erag, &sumrag) == FALSE){
					return(TRUE);
				}
			}
			else if(p2 == 0){
				if(CheckPointInGetSumrag(xx, yy, p2, &srag, &erag, &sumrag) == FALSE){
					return(TRUE);
				}
			}
			else{
				if(CheckPointInBezeGetSumrag(xx, yy, p1, p2, p3, p4, &srag, &erag, &sumrag) == FALSE){
					return(TRUE);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(CheckPointInGetSumrag(xx, yy, p2, &srag, &erag, &sumrag) == FALSE){
				return(TRUE);
			}
			if(CheckPointInGetSumrag(xx, yy, p3, &srag, &erag, &sumrag) == FALSE){
				return(TRUE);
			}
			pp += 2;
		}
		else{
			if(CheckPointInGetSumrag(xx, yy, p2, &srag, &erag, &sumrag) == FALSE){
				return(TRUE);
			}
			pp += 1;
		}
	}
	if(fabs(sumrag) > 0.2){
		return(TRUE);
	}
	else{
		return(FALSE);
	}
}

BOOL CVect::CheckPointInGetRag(DBL xx, DBL yy, int pp, LPDBL rag)
{
	DBL		x, y;

	GetDXY(pp, &x, &y);
	if(xx == x && yy == y){
		return(FALSE);
	}
	*rag = atan2(y-yy, x-xx);
	if(*rag < 0){
		(*rag) += (M_PI*2);
	}
	return(TRUE);
}

BOOL CVect::CheckPointInGetSumrag(DBL xx, DBL yy, int pp, LPDBL srag, LPDBL erag, LPDBL sumrag)
{
	if(CheckPointInGetRag(xx, yy, pp, erag) == FALSE){
		return(FALSE);
	}
	CheckPointInAddSumRag(srag, erag, sumrag);
	return(TRUE);
}

BOOL CVect::CheckPointInBezeGetSumrag(DBL xx, DBL yy, int p1, int p2, int p3, int p4, LPDBL srag, LPDBL erag, LPDBL sumrag)
{
	DBL	x1, y1, x2, y2, x3, y3, x4, y4;
	DBL	t, t1, b1, b2, b3, b4;
	DBL	x, y;

	GetDXY(p1, &x1, &y1);
	GetDXY(p2, &x2, &y2);
	GetDXY(p3, &x3, &y3);
	GetDXY(p4, &x4, &y4);
	for(t = 0.2; t < 1.0; t += 0.2){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		x = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		y = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		if(xx == x && yy == y){
			return(FALSE);
		}
		(*erag) = atan2(y-yy, x-xx);
		CheckPointInAddSumRag(srag, erag, sumrag);
	}
	x = x4; y = y4;
	if(xx == x && yy == y){
		return(FALSE);
	}
	(*erag) = atan2(y-yy, x-xx);
	CheckPointInAddSumRag(srag, erag, sumrag);
	return(TRUE);
}

void CVect::CheckPointInAddSumRag(LPDBL srag, LPDBL erag, LPDBL sumrag)
{
	DBL		subrag;

	subrag = (*erag) - (*srag);
	if(subrag < M_PI){
		subrag += M_PI*2;
	}
	if(subrag > M_PI){
		subrag -= M_PI*2;
	}
	//CheckRag(*srag, *erag, *sumrag);
	(*sumrag) += subrag;
	//CheckRag(*srag, *erag, *sumrag);
	(*srag) = (*erag);
}

void CVect::CheckRag(DBL srag, DBL erag, DBL sumrag)
{
	DBL	rag1, rag2, rag3;

	rag1 = srag / M_1RAG;
	rag2 = erag / M_1RAG;
	rag3 = sumrag / M_1RAG;
}

int CVect::SrchInArea(DBL dx, DBL dy)
{
	int		pcnt, pp, np, tp, atr;
	DBL		x1, y1, x2, y2;
	dbezetype	bz;
	DBL		subrag;
	DBL		sumrag;

	sumrag = 0;
	pcnt = GetPointCount();
	GetAtrDXY(0, &atr, &x1, &y1);
	for(pp = 0; pp < pcnt;){
		np = NextPointGet(pp);
		if(pp + 3 == np){
			tp = PointLegal(pcnt, pp);
			GetAtrDXY(tp, &atr, &bz.x1, &bz.y1);
			tp = PointLegal(pcnt, pp+1);
			GetAtrDXY(tp, &atr, &bz.x2, &bz.y2);
			tp = PointLegal(pcnt, pp+2);
			GetAtrDXY(tp, &atr, &bz.x3, &bz.y3);
			tp = PointLegal(pcnt, pp+3);
			GetAtrDXY(tp, &atr, &bz.x4, &bz.y4);
			curvebezetpointget(bz, 0.0, &x2, &y2);
			subrag = GetSubRag(dx, dy, x1, y1, x2, y2);
			if(NORAG == subrag){
				return(TRUE);
			}
			sumrag += subrag;
			x1 = x2; y1 = y2;
			curvebezetpointget(bz, 0.3, &x2, &y2);
			subrag = GetSubRag(dx, dy, x1, y1, x2, y2);
			if(NORAG == subrag){
				return(TRUE);
			}
			sumrag += subrag;
			x1 = x2; y1 = y2;
			curvebezetpointget(bz, 0.7, &x2, &y2);
			subrag = GetSubRag(dx, dy, x1, y1, x2, y2);
			if(NORAG == subrag){
				return(TRUE);
			}
			sumrag += subrag;
			x1 = x2; y1 = y2;
		}
		else if(pp + 2 == np){
			tp = PointLegal(pcnt, pp);
			GetAtrDXY(tp, &atr, &x2, &y2);
			subrag = GetSubRag(dx, dy, x1, y1, x2, y2);
			if(NORAG == subrag){
				return(TRUE);
			}
			sumrag += subrag;
			x1 = x2; y1 = y2;
			tp = PointLegal(pcnt, pp+1);
			GetAtrDXY(tp, &atr, &x2, &y2);
			subrag = GetSubRag(dx, dy, x1, y1, x2, y2);
			if(NORAG == subrag){
				return(TRUE);
			}
			sumrag += subrag;
			x1 = x2; y1 = y2;
		}
		else{
			tp = PointLegal(pcnt, pp);
			GetAtrDXY(tp, &atr, &x2, &y2);
			subrag = GetSubRag(dx, dy, x1, y1, x2, y2);
			if(NORAG == subrag){
				return(TRUE);
			}
			sumrag += subrag;
			x1 = x2; y1 = y2;
		}
		pp = np;
	}
	GetAtrDXY(0, &atr, &x2, &y2);
	subrag = GetSubRag(dx, dy, x1, y1, x2, y2);
	if(NORAG == subrag){
		return(TRUE);
	}
	sumrag += subrag;

	if(M_PI < sumrag || -M_PI > sumrag){
		return(TRUE);
	}
	return(FALSE);
}

DBL CVect::GetSubRag(DBL dx, DBL dy, DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		rag1, rag2, subrag;
	int		nrag1, nrag2, nsubrag;

	if(dx == x1 && dy == y1){
		return(NORAG);
	}
	if(dx == x2 && dy == y2){
		return(NORAG);
	}
	if(x1 == x2 && y1 == y2){
		return(0);
	}
	rag1 = atan2(y1-dy, x1-dx);
	rag2 = atan2(y2-dy, x2-dx);
	subrag = rag1 - rag2;
	if(M_PI*2 < subrag){
		subrag -= M_PI*2;
	}
	if(-M_PI*2 > subrag){
		subrag += M_PI*2;
	}
	if(M_PI < subrag){
		subrag -= M_PI*2;
	}
	if(-M_PI > subrag){
		subrag += M_PI*2;
	}
	nrag1 = (int)(rag1 / M_1RAG);
	nrag2 = (int)(rag2 / M_1RAG);
	nsubrag = (int)(subrag / M_1RAG);
	return(subrag);
}

void CVect::SetRoundKind()
{
	int		p0, p1, p2, p3;
	int		atr0, atr1, atr2, atr3;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3;
	int		kind;

	p0 = 0;
	p1 = 1;
	p2 = 2;
	p3 = m_nPointCount-1;

	GetAtrDXY(p1, &atr1, &x1, &y1);
	GetAtrDXY(p3, &atr3, &x3, &y3);
	if(!(x1 == x3 && y1 == y3)){
		poutroundkindset(0);
	}
	GetAtrDXY(p0, &atr0, &x0, &y0);
	GetAtrDXY(p1, &atr1, &x1, &y1);
	GetAtrDXY(p2, &atr2, &x2, &y2);

	plotdxymmtostep(&x0, &y0);
	plotdxymmtostep(&x1, &y1);
	plotdxymmtostep(&x2, &y2);
	kind = curvenkindget(x1, y1, x2, y2, x0, y0);
	poutroundkindset(kind);
}

void CVect::CnvBezeToLine(int divcnt)
{
	int		pcnt, pp, np;
	int		kind;
	dfigutype	fg;
	dbezetype	bz;

	pcnt = GetPointCount();
	if(CheckFlag(REJIONFLAG) == OFF || pcnt == 2){
		pcnt = pcnt-1;
	}
	for(pp = 0; pp < pcnt;){
		kind = GetFigu(pp, &np, &fg);
		switch(kind){
		case LINEKIND:
			break;
		case BEZEKIND:
			bz.x1 = fg.x1; bz.y1 = fg.y1;
			bz.x2 = fg.x2; bz.y2 = fg.y2;
			bz.x3 = fg.x3; bz.y3 = fg.y3;
			bz.x4 = fg.x4; bz.y4 = fg.y4;
			curvebezetpointget(bz, 0.3333, &(fg.x2), &(fg.y2));
			curvebezetpointget(bz, 0.6666, &(fg.x3), &(fg.y3));
			SetAtrDXY(pp, 0, fg.x1, fg.y1);
			SetAtrDXY(pp+1, 0, fg.x2, fg.y2);
			SetAtrDXY(pp+2, 0, fg.x3, fg.y3);
			break;
		case ARCKIND:
			SetAtrDXY(pp, 0, fg.x1, fg.y1);
			SetAtrDXY(pp+1, 0, fg.x2, fg.y2);
			break;
		case FREEKIND:
			SetAtrDXY(pp, 0, fg.x1, fg.y1);
			break;
		}
		pp = np;
	}
}

int CVect::GetFigu(int pp, int *np, dfigutype *figu)
{
	int		p0, p1, p2, p3, p4;

	p0 = PointLegal(m_nPointCount, pp-1);
	p1 = PointLegal(m_nPointCount, pp+0);
	p2 = PointLegal(m_nPointCount, pp+1);
	p3 = PointLegal(m_nPointCount, pp+2);
	p4 = PointLegal(m_nPointCount, pp+3);
	GetAtrDXY(p0, &(figu->atr0), &(figu->x0), &(figu->y0));
	GetAtrDXY(p1, &(figu->atr1), &(figu->x1), &(figu->y1));
	GetAtrDXY(p2, &(figu->atr2), &(figu->x2), &(figu->y2));
	GetAtrDXY(p3, &(figu->atr3), &(figu->x3), &(figu->y3));
	GetAtrDXY(p4, &(figu->atr4), &(figu->x4), &(figu->y4));
	if(pdrwatrbezechk(figu->atr1, figu->atr2, figu->atr3) == ON){
		if(CheckFlag(REJIONFLAG) == ON){
			if(p2 == 0 || p3 == 0){
				*np = pp+1;
				return(LINEKIND);
			}
		}
		else{
			if(p3 == 0 || p4 == 0){
				*np = pp+1;
				return(LINEKIND);
			}
		}
		*np = pp+3;
		return(BEZEKIND);
	}
	else if(pdrwatrarcchk(figu->atr1, figu->atr2, figu->atr3) == ON){
		if(CheckFlag(REJIONFLAG) == ON){
			if(p2 == 0){
				*np = pp+1;
				return(LINEKIND);
			}
		}
		else{
			if(p3 == 0){
				*np = pp+1;
				return(LINEKIND);
			}
		}
		*np = pp+2;
		return(ARCKIND);
	}
	else if(pdrwatrcurvechk(figu->atr1) == ON){
		*np = pp+1;
		return(FREEKIND);
	}
	else{
		*np = pp+1;
		return(LINEKIND);
	}
}
