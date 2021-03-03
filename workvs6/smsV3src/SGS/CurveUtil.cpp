
/////////////////////////////////////////////////////////////////////////////
// CCrossLib

#include "stdafx.h"
#include "math.h"

#include "crosslib.h"

extern "C" {
	int curve3pcenterget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL FAR *x0, DBL FAR *y0);
	int curve3pragrget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0, DBL FAR *rag01, DBL FAR *rag03, DBL FAR *r);
	void curvenoragtocurverag(DBL *rag1, DBL *rag2);
	int curveragnoget(DBL rag);
	void curvebezecontrolpointget(DBL x1, DBL y1, DBL x2, DBL y2, DBL rag, DBL *xp, DBL *yp);
	void curvebezetpointget(dbezetype bz, DBL t, DBL *x, DBL *y);
	int curvecrosslineinareachk(dlinetype line1, dlinetype line2);
	int curvecrosslinelinexyget(dlinetype line1, dlinetype line2, DBL *x, DBL *y);
	int curvecrosslnlnxyget(dlinetype line1, dlinetype line2, DBL *x, DBL *y);
	void cruvearctobeze(darctype arc,  dbezetype *bz);
	int crossarclinecrs(darctype *ac1, dlinetype *ln2, DBL *xtbl, DBL *ytbl);
	int crossbezelinecrs(dbezetype *bz1, dlinetype *ln2, DBL *xtbl, DBL *ytbl);
	BOOL  pdrwatrarcchk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrbezechk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrcurvechk(int atr1);
	DBL   pdrwendragget(int atr1, int atr2, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL   pdrwstartragget(int atr0, int atr1, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2);
}

CCrossLib::CCrossLib(CScrollView* pcview)
{
	m_pcView = (CTonView*)pcview;
	m_pcDoc = (CTonDoc*)(m_pcView->GetDocument());
}

void CCmdLine::DivFiguElementPtrList()
{
	POSITION	pos;
	CVect		*pcVect;

	pos = m_cPtrList.GetHeadPosition();
	while(pos != NULL){
		pcVect = (CVect*)m_cPtrList.GetNext(pos);
		DivFiguElementVect(pcVect);
	}
}

void CCmdLine::DivFiguElementVect(CVect *pcVect)
{
	int pcnt, pp, np;
	dfigutype figu;
	CFigure *pcFg;

	pcnt = pcVect->GetPointCount();
	pp = 0;
	while(1){
		if(pcnt <= pp){
			break;
		}
		pcFg = new CFigure();
		kind = pcVect->GetFigu(pp, &np, &figu);
		if(pp == np){
			break;
		}
		if(kind == LINEKIND){
			pcFg->type = LINEKIND;
			pcFg->x1 = figu.x1;
			pcFg->y1 = figu.y1;
			pcFg->x2 = figu.x1;
			pcFg->y2 = figu.y1;
			pcFg->x3 = figu.x2;
			pcFg->y3 = figu.y2;
			pcFg->x4 = figu.x2;
			pcFg->y4 = figu.y2;
		}else if(kind == BEZEKIND){
			pcFg->type = BEZEKIND;
			pcFg->x1 = figu.x1;
			pcFg->y1 = figu.y1;
			pcFg->x2 = figu.x2;
			pcFg->y2 = figu.y2;
			pcFg->x3 = figu.x3;
			pcFg->y3 = figu.y3;
			pcFg->x4 = figu.x4;
			pcFg->y4 = figu.y4;
		}else if(kind == ARCKIND){

		}else if(kind == FREEKIND){

		}
		pp = np;
	}
	for(pp == 0; pp < pcnt; pp++){

	}

	POSITION	pos;
	CVect		*pcVect;
	pos = m_cPtrList.GetHeadPosition();
	while(pos != NULL){
		pcVect = (CVect*)m_cPtrList.GetNext(pos);
	}
}

void CCrossLib::GetCrossPointLinekind(dlinetype bln, CFigure *pcFg)
{
	CPoint pt;
	dlinetype ln;
	DBL		xx, yy;
	CFigure *pcFg2;

	ln.x1 = pcFg->x1; ln.y1 = pcFg->y1;
	ln.x2 = pcFg->x2; ln.y2 = pcFg->y2;
	len = hypot(ln.x2-ln.x1, ln.y2-ln.y1);
	ret = curvecrosslinelinexyget(bln, ln, &xx, &yy);
	if(ret == ON){
		len1 == hypot(ln.x1 - xx, ln.y1 - yy);
		len2 == hypot(ln.x2 - xx, ln.y2 - yy);
		if(len1 < thick){
			xx = ln.x1;
			yy = ln.y1;
		}
		pcFg->x1 = ln.x1; pcFg->y1 = ln.y1;
		pcFg->x2 = ln.x1; pcFg->y2 = ln.y1;
		pcFg->x3 = xx; pcFg->y3 = yy;
		pcFg->x4 = xx; pcFg->y4 = yy;
		pcFg2 = new CFigure();
		pcFg2->x1 = xx; pcFg2->y1 = yy;
		pcFg2->x2 = xx; pcFg2->y2 = yy;
		pcFg2->x3 = ln.x2; pcFg2->y3 = ln.y2;
		pcFg2->x4 = ln.x2; pcFg2->y4 = ln.y2;
	}
}

CFigure* CCurveUtil::GetCrossPointBezekind(dlinetype ln, CFigure *pcFg)
{
	dbezetype bz;
	DBL		xtbl[4], ytbl[4];
	int		cnt;

	bz.x1 = pcFg->x1; bz.y1 = pcFg->y1;
	bz.x2 = pcFg->x2; bz.y2 = pcFg->y2;
	bz.x3 = pcFg->x3; bz.y3 = pcFg->y3;
	bz.x4 = pcFg->x4; bz.y4 = pcFg->y4;
	cnt = crossbezelinecrsex(&bz, &ln, xtbl, ytbl, tttbl);
	if(cnt == 1 || cnt == 3){
		
	}
	return(NULL);
}


void CCurveUtil::ConvertFGArcToBeze(dfigutype *fg, CFigure *pcFg))
{
	int		stat;
	DBL		x0, y0;
	DBL		srag, erag, rag;
	DBL		dr;
	DBL		t1, t2;
	dbezetype bz;

	stat = curve3pcenterget(fg->x1, fg->y1, fg->x2, fg->y2, fg->x3, fg->y3, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(fg->x1, fg->y1, fg->x2, fg->y2, fg->x3, fg->y3, x0, y0, &srag, &erag, &dr);
	}
	if(stat == FAIL){
		pcFg->type = LINEKIND;
		pcFg->x1 = fg->x1;
		pcFg->y1 = fg->y1;
		pcFg->x2 = fg->x1;
		pcFg->y2 = fg->y1;
		pcFg->x3 = fg->x3;
		pcFg->y3 = fg->y3;
		pcFg->x4 = fg->x3;
		pcFg->y4 = fg->y3;
		return;
	}

	bz.x1 = fg->x1; bz.y1 = fg->y1;
	bz.x4 = fg->x3; bz.y4 = fg->y3;
	rag = (erag - srag) / 3.0 + srag;
	bz.x2 = cos(rag)*dr+x0;
	bz.y2 = sin(rag)*dr+y0;
	rag = (erag - srag) / 3.0 * 2.0 + srag;
	bz.x3 = cos(rag)*dr+x0;
	bz.y3 = sin(rag)*dr+y0;

	t1 = 1.0 / 3.0; t2 = 2.0 / 3.0;
	curveonpointtocntrlpoint(&bz, t1, t2);
	pcFg->type = BEZEKIND;
	pcFg->x1 = bz.x1;
	pcFg->y1 = bz.y1;
	pcFg->x2 = bz.x2;
	pcFg->y2 = bz.y2;
	pcFg->x3 = bz.x3;
	pcFg->y3 = bz.y3;
	pcFg->x4 = bz.x4;
	pcFg->y4 = bz.y4;
}

void CCurveUtil::ConvertFGFreeToCFigure(dfigutype *fg, CFigure *pcFg)
{
	DBL	rag1, rag2;

	rag1 = pdrwstartragget(fg->atr0, fg->atr1, fg->x0, fg->y0, fg->x1, fg->y1, fg->x2, fg->y2);
	rag2 = pdrwendragget(fg->atr1, fg->atr2, fg->x1, fg->y1, fg->x2, fg->y2, fg->x3, fg->y3);
	if(rag1 == NORAG && rag2 == NORAG){
		pcFg->type = LINEKIND;
		pcFg->x1 = fg->x1;
		pcFg->y1 = fg->y1;
		pcFg->x2 = fg->x1;
		pcFg->y2 = fg->y1;
		pcFg->x3 = fg->x2;
		pcFg->y3 = fg->y2;
		pcFg->x4 = fg->x2;
		pcFg->y4 = fg->y2;
		return;
	}
	pcFg->type = BEZEKIND;
	curvenoragtocurverag(&rag1, &rag2);
	pcFg->x1 = fg->x1; pcFg->y1 = fg->y1;
	pcFg->x4 = fg->x2; pcFg->y4 = fg->y2;
	curvebezecontrolpointget(fg->x1, fg->y1, fg->x2, fg->y2, rag1, &(pcFg->x2), &(pcFg->y2));
	curvebezecontrolpointget(fg->x2, fg->y2, fg->x1, fg->y1, rag2, &(pcFg->x3), &(pcFg->y3));
}

