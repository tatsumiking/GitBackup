
/////////////////////////////////////////////////////////////////////////////
// CCurveUtil

#include "stdafx.h"
#include "math.h"

#include "curvutil.h"

extern "C" {
	DBL pdrwendragget(int atr1, int atr2, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL pdrwstartragget(int atr0, int atr1, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2);
	BOOL  pdrwatrbezechk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrarcchk(int atr1, int atr2, int atr3);
	int curve3pcenterget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL FAR *x0, DBL FAR *y0);
	int curve3pragrget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0, DBL FAR *rag01, DBL FAR *rag03, DBL FAR *r);
	void curvebezecontrolpointget(DBL x1, DBL y1, DBL x2, DBL y2, DBL rag, DBL *xp, DBL *yp);
	void curveonpointtocntrlpoint(dbezetype *dbz, DBL tp1, DBL tp2);
	void curvenoragtocurverag(DBL *rag1, DBL *rag2);
	DBL curveraglegal(DBL rag);
	void curvebezettdivget(dbezetype sbz, DBL st, DBL et, dbezetype *dbz);
	int crossbezettdiv(dbezetype *sbz, DBL tt, dbezetype *dbz1, dbezetype *dbz2);
	void scrndxymmtorltv(LPDBL x, LPDBL y);
}

CCurveUtil::CCurveUtil(CScrollView* pcView)
{
	m_pcView = (CTonView*)pcView;
}

void CCurveUtil::ConvertBezeDispath(CData *pcData)
{
	int		nID;

	nID = pcData->CheckID();
	switch(nID){
	case DATAGRUPID:
		ConvertBezeDataGrup((CDataGrup*)pcData);
		break;
	case DATALISTID:
		ConvertBezeDataList((CDataList*)pcData);
		break;
	case BASEVECTID:
		ConvertBezeVect((CVect*)pcData);
		break;
	}
}

void CCurveUtil::ConvertBezeDataGrup(CDataGrup *pcDataGrup)
{
	CData	*pcData;

	pcData = pcDataGrup->m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		ConvertBezeDispath(pcData);
		pcData = pcData->GetNext();
	}
}

void CCurveUtil::ConvertBezeDataList(CDataList *pcDataList)
{
	CData	*pcData;

	pcData = pcDataList->m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		ConvertBezeDispath(pcData);
		pcData = pcData->GetNext();
	}
}

void CCurveUtil::ConvertBezeVect(CVect *pcVect)
{
	int		pcnt, pp, dp;
	int		p0, p1, p2, p3, p4;
	CVect	*pcTempVect;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		rag1, rag2;
	DBL		xp1, yp1, xp2, yp2;
	dbezetype bz;

	pcnt = pcVect->GetPointCount();
	if(pcnt == 0){
		return;
	}
	StartArcLegal(pcVect);
	pcTempVect = new CVect(BASEVECTID);
	dp = 0;
	pcnt = pcVect->GetPointCount();
	for(pp = 0; pp < pcnt;){
		p0 = pcVect->PointLegal(pcnt, pp-1);
		p1 = pcVect->PointLegal(pcnt, pp+0);
		p2 = pcVect->PointLegal(pcnt, pp+1);
		p3 = pcVect->PointLegal(pcnt, pp+2);
		p4 = pcVect->PointLegal(pcnt, pp+3);
		pcVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		pcVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		pcVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		pcVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		pcVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				pcTempVect->SetAtrDXY(dp++, 0, x1, y1);
				pcTempVect->SetAtrDXY(dp++, 0, x2, y2);
			}
			else if(p2 == 0){
				pcTempVect->SetAtrDXY(dp++, 0, x1, y1);
			}
			else{
				bz.x1 = x1; bz.y1 = y1;
				bz.x2 = x2; bz.y2 = y2;
				bz.x3 = x3; bz.y3 = y3;
				bz.x4 = x4; bz.y4 = y4;
				dp = SetDivBeze(bz, pcTempVect, dp);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			dp = ConvertBezeArc(pcVect, pp, pp+2, pcTempVect, dp);
			// ŠÖ”“à‚Å‚a‚d‚y‚d‚ÌŠJŽn“_‚ÉPATN1ATR‚ðƒZƒbƒg
			pp += 2;
		}
		else if(atr1 & CURVATR){
			rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
			rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);
			if(rag1 == NORAG && rag2 == NORAG){
				pcTempVect->SetAtrDXY(dp++, 0, x1, y1);
			}
			else{
				curvenoragtocurverag(&rag1, &rag2);
				curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
				curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
				bz.x1 = x1; bz.y1 = y1;
				bz.x2 = xp1; bz.y2 = yp1;
				bz.x3 = xp2; bz.y3 = yp2;
				bz.x4 = x2; bz.y4 = y2;
				dp = SetDivBeze(bz, pcTempVect, dp);
			}
			pp += 1;
		}
		else{
			pcTempVect->SetAtrDXY(dp++, 0, x1, y1);
			pp += 1;
		}
	}
	for(pp = 0; pp < dp; pp++){
		pcTempVect->GetFAtrDXY(pp, &atr1, &x1, &y1);
		pcVect->SetFAtrDXY(pp, atr1, x1, y1);
	}
	pcVect->SetPointCount(dp);
	delete(pcTempVect);
}

void CCurveUtil::StartArcLegal(CVect *pcVect)
{
	CVect	*pcTempVect;
	int		pcnt, pp, dp;
	int		p1, p2, p3;
	int		atr1, atr2, atr3;
	int		atr;
	DBL		x, y;
	DBL		tx, ty;

	pcnt = pcVect->GetPointCount();
	if(pcnt == 0){
		return;
	}
	pcTempVect = new CVect(BASEVECTID);
	pcTempVect->SetPointCount(pcnt);
	pp = 0;
	p1 = pcVect->PointLegal(pcnt, pp-1);
	p2 = pcVect->PointLegal(pcnt, pp);
	p3 = pcVect->PointLegal(pcnt, pp+1);
	pcVect->GetAtr(p1, &atr1);
	pcVect->GetAtr(p2, &atr2);
	pcVect->GetAtr(p3, &atr3);
	if(pdrwatrarcchk(atr1, atr2, atr3) == ON){

		for(pp = 1, dp = 0; pp < pcnt; pp++, dp++){
			pcVect->GetAtrDXY(pp, &atr, &x, &y);
			pcTempVect->SetAtrDXY(dp, atr, x, y);
		}
		pcVect->GetAtrDXY(0, &atr, &x, &y);
		pcTempVect->SetAtrDXY(dp, atr,  x, y);
	}
	else{
		for(pp = 0; pp < pcnt; pp++){
			pcVect->GetAtrDXY(pp, &atr, &x, &y);
			pcTempVect->SetAtrDXY(pp, atr, x, y);
		}
	}

	pcTempVect->GetAtrDXY(0, &atr, &tx, &ty);
	pcVect->SetAtrDXY(0, atr,  tx, ty);
	for(pp = 1, dp = 1; pp < pcnt; pp++, dp++){
		pcTempVect->GetAtrDXY(pp, &atr, &x, &y);
		if(tx == x && ty == y){
			dp--;
		}
		pcVect->SetAtrDXY(dp, atr, x, y);
		tx = x; ty = y;
	}
	pcTempVect->GetAtrDXY(0, &atr, &tx, &ty);
	if(tx == x && ty == y){
		dp--;
	}
	pcVect->SetPointCount(dp);
}

int CCurveUtil::ConvertBezeVectArea(CVect *pcVect, int sp, int ep, CVect *pcTempVect, int dp)
{
	int		pcnt, pp, atr1, atr2;
	DBL		x1, y1, x2, y2;
	int		idx, idxmax;
	int		sidx, eidx;
	DBL		subrag;

	idxmax = ep - sp;
	if(idxmax == 1){
		pcVect->GetAtrDXY(sp, &atr1, &x1, &y1);
		pcTempVect->SetAtrDXY(dp, 0, x1, y1);
		dp++;
		return(dp);
	}
	/*
	if(idxmax == 2){
		// ‚R“_‰~ŒÊ‚ÌŽž
		dp = ConvertBezeArc(pcVect, sp, ep, pcTempVect, dp);
		return(dp);
	}
	*/
	DBL		*len = new DBL[idxmax];
	DBL		*rag = new DBL[idxmax];

	pcnt = pcVect->GetPointCount();
	idx = 0;
	pcVect->GetAtrDXY(sp, &atr1, &x1, &y1);
	for(pp = sp+1; pp < ep; pp++){
		pcVect->GetAtrDXY(pp, &atr2, &x2, &y2);
		len[idx] = hypot(x2-x1, y2-y1);
		rag[idx] = atan2(y2-y1, x2-x1);
		idx++;
		x1 = x2; y1 = y2;
	}
	pp = pcVect->PointLegal(pcnt, ep);
	pcVect->GetAtrDXY(pp, &atr2, &x2, &y2);
	len[idx] = hypot(x2-x1, y2-y1);
	rag[idx] = atan2(y2-y1, x2-x1);

	sidx = 0;
	for(idx = 1; idx < idxmax; idx++){
		subrag = rag[sidx] - rag[idx];
		subrag = curveraglegal(subrag);
		if((len[idx-1] * 2.0 < len[idx] || len[idx-1] > len[idx] * 2.0)
		|| (len[sidx] * 2.0 < len[idx] || len[sidx] > len[idx] * 2.0)
		|| (subrag > M_PI_4 || -M_PI_4 > subrag)){
			eidx = idx+1;
			dp = ConvertBezeFreeArea(pcVect, sidx+sp, eidx+sp, pcTempVect, dp);
			sidx = eidx;
		}
	}
	if(sidx != idx){
		dp = ConvertBezeFreeArea(pcVect, sidx+sp, ep, pcTempVect, dp);
	}

	delete [] len;
	delete [] rag;

	return(dp);
}

int CCurveUtil::ConvertBezeArc(CVect *pcVect, int sp, int ep, CVect *pcTempVect, int dp)
{
	int	pcnt;
	int	p1, p2, p3;
	int	atr1, atr2, atr3;
	DBL	x1, y1, x2, y2, x3, y3;
	int	stat;
	DBL	x0, y0, srag, erag, dr;
	DBL	rag;
	DBL	t1, t2;
	dbezetype bz;

	pcnt = pcVect->GetPointCount();
	p1 = pcVect->PointLegal(pcnt, sp);
	p2 = pcVect->PointLegal(pcnt, sp+1);
	p3 = pcVect->PointLegal(pcnt, sp+2);
	pcVect->GetAtrDXY(p1, &atr1, &x1, &y1);
	pcVect->GetAtrDXY(p2, &atr2, &x2, &y2);
	pcVect->GetAtrDXY(p3, &atr3, &x3, &y3);
	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &srag, &erag, &dr);
	}
	if(stat == FAIL){
		pcTempVect->SetAtrDXY(dp, 0, x1, y1);
		dp++;
		return(dp);
	}

	bz.x1 = x1; bz.y1 = y1;
	bz.x4 = x3; bz.y4 = y3;
	rag = (erag - srag) / 3.0 + srag;
	bz.x2 = cos(rag)*dr+x0;
	bz.y2 = sin(rag)*dr+y0;
	rag = (erag - srag) / 3.0 * 2.0 + srag;
	bz.x3 = cos(rag)*dr+x0;
	bz.y3 = sin(rag)*dr+y0;

	t1 = 1.0 / 3.0; t2 = 1.0 / 3.0 * 2.0;
	curveonpointtocntrlpoint(&bz, t1, t2);
	dp = SetDivBeze(bz, pcTempVect, dp);
	return(dp);
}

int CCurveUtil::ConvertBezeFreeArea(CVect *pcVect, int sp, int ep, CVect *pcTempVect, int dp)
{
	int		tsp, tep;

	while(1){
		tsp = sp;
		tep = sp+3;
		if((ep - tep) < 3){
			tep = ep;
		}
		dp = ConvertBezeFreeCurve(pcVect, tsp, tep, pcTempVect, dp);
		if(tep == ep){
			break;
		}
		sp = tep;
	}
	return(dp);
}

int CCurveUtil::ConvertBezeFreeCurve(CVect *pcVect, int sp, int ep, CVect *pcTempVect, int dp)
{
	DBL		sumlen;
	DBL		len1, len2;
	DBL		t1, t2;
	dbezetype bz;
	int		pcnt, pp;

	pcnt = pcVect->GetPointCount();
	pp = pcVect->PointLegal(pcnt, sp);
	pcVect->GetDXY(pp, &(bz.x1), &(bz.y1));
	pp = pcVect->PointLegal(pcnt, ep);
	pcVect->GetDXY(pp, &(bz.x4), &(bz.y4));
	sumlen = GetFreeCurveAreaLength(pcVect, sp, ep);
	len1 = sumlen / 3.0;
	len2 = sumlen / 3.0 * 2.0;
	GetFreeCurveOnPoint(len1, &(bz.x2), &(bz.y2), pcVect, sp, ep);
	GetFreeCurveOnPoint(len2, &(bz.x3), &(bz.y3), pcVect, sp, ep);
	t1 = 1.0 / 3.0; t2 = 1.0 / 3.0 * 2.0;
	curveonpointtocntrlpoint(&bz, t1, t2); 
	pcTempVect->SetAtrDXY(dp, BEZEATR | CURVATR, bz.x1, bz.y1);
	dp++;
	pcTempVect->SetAtrDXY(dp, BEZEATR, bz.x2, bz.y2);
	dp++;
	pcTempVect->SetAtrDXY(dp, BEZEATR, bz.x3, bz.y3);
	dp++;
	return(dp);
}

DBL CCurveUtil::GetFreeCurveAreaLength(CVect* pcVect, int sp, int ep)
{
	int		pcnt, pp;
	int		p0, p1, p2, p3;
	int		atr0, atr1, atr2, atr3;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3;
	DBL		sumlen, len;

	sumlen = 0;
	pcnt = pcVect->GetPointCount();
	for(pp = sp; pp < ep; pp++){
		p0 = pcVect->PointLegal(pcnt, pp-1);
		p1 = pcVect->PointLegal(pcnt, pp+0);
		p2 = pcVect->PointLegal(pcnt, pp+1);
		p3 = pcVect->PointLegal(pcnt, pp+2);
		pcVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		pcVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		pcVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		pcVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		len = GetFreeCurveLength(atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
		sumlen += len;
	}
	return(sumlen);
}

DBL CCurveUtil::GetFreeCurveLength(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	rag1, rag2;
	DBL	xp1, yp1, xp2, yp2;
	DBL	len;

	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);

	if(rag1 == NORAG && rag2 == NORAG){
		len = hypot(x2-x1, y2-y1);
	}
	else{
		curvenoragtocurverag(&rag1, &rag2);
		curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
		curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
		len = GetBezeLength(x1, y1, xp1, yp1, xp2, yp2, x2, y2);
	}
	return(len);
}

DBL CCurveUtil::GetBezeLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL		t, t1, b1, b2, b3, b4;
	DBL		sx, sy, ex, ey;
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
		sumlen += hypot(ex-sx, ey-sy);
		sx = ex; sy = ey;
	}
	ex = x4; ey = y4;
	sumlen += hypot(ex-sx, ey-sy);
	return(sumlen);
}

void CCurveUtil::GetFreeCurveOnPoint(DBL setlen, DBL *retx, DBL *rety, CVect* pcVect, int sp, int ep)
{
	int		pcnt, pp;
	int		p0, p1, p2, p3;
	int		atr0, atr1, atr2, atr3;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3;
	DBL		dx, dy;

	pcnt = pcVect->GetPointCount();
	for(pp = sp; pp < ep; pp++){
		p0 = pcVect->PointLegal(pcnt, pp-1);
		p1 = pcVect->PointLegal(pcnt, pp+0);
		p2 = pcVect->PointLegal(pcnt, pp+1);
		p3 = pcVect->PointLegal(pcnt, pp+2);
		pcVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		pcVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		pcVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		pcVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		setlen = GetFreeCurveOnXY(setlen, &dx, &dy, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
		if(setlen <= 0){
			break;
		}
	}
	*retx = dx;
	*rety = dy;
}

DBL CCurveUtil::GetFreeCurveOnXY(DBL setlen, LPDBL dx, LPDBL dy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	rag1, rag2;
	DBL	xp1, yp1, xp2, yp2;

	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);

	if(rag1 == NORAG && rag2 == NORAG){
		setlen = GetLineOnXY(setlen, dx, dy, x1, y1, x2, y2);
		if(setlen <= 0){
			return(setlen);
		}
	}
	else{
		curvenoragtocurverag(&rag1, &rag2);
		curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
		curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
		setlen = GetBezeOnXY(setlen, dx, dy, x1, y1, xp1, yp1, xp2, yp2, x2, y2);
		if(setlen <= 0){
			return(setlen);
		}
	}
	return(setlen);
}

DBL CCurveUtil::GetBezeOnXY(DBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL		t, t1, b1, b2, b3, b4;
	DBL		sx, sy, ex, ey;
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
		setlen = GetLineOnXY(setlen, dx, dy, sx, sy, ex, ey);
		if(setlen <= 0){
			return(setlen);
		}
		sx = ex; sy = ey;
	}
	ex = x4; ey = y4;
	setlen = GetLineOnXY(setlen, dx, dy, sx, sy, ex, ey);
	return(setlen);
}

DBL CCurveUtil::GetLineOnXY(DBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		len;

	len = hypot(x1-x2, y1-y2);
	if(y2 == y1 && x2 == x1){
		(*dx) = x1; (*dy) = y1;
		return(setlen);
	}
	if(setlen <= len){
		(*dx) = (x2 - x1) * setlen / len + x1;
		(*dy) = (y2 - y1) * setlen / len + y1;
		setlen = 0;
	}
	else{
		setlen = setlen - len;
	}
	return(setlen);
}

int CCurveUtil::SetFigu(CVect *pcVect, int pp, dbezetype *bz, dlinetype *ln)
{
	int		pcnt, p1, p2, p3, p4;
	int		atr1, atr2, atr3, atr4;
	DBL		x1, x2, x3, x4;
	DBL		y1, y2, y3, y4;
	int		kind;

	pcnt = pcVect->GetPointCount();
	p1 = pcVect->PointLegal(pcnt, pp+0);
	p2 = pcVect->PointLegal(pcnt, pp+1);
	p3 = pcVect->PointLegal(pcnt, pp+2);
	p4 = pcVect->PointLegal(pcnt, pp+3);
	pcVect->GetAtrDXY(p1, &atr1, &x1, &y1);
	pcVect->GetAtrDXY(p2, &atr2, &x2, &y2);
	pcVect->GetAtrDXY(p3, &atr3, &x3, &y3);
	pcVect->GetAtrDXY(p4, &atr4, &x4, &y4);
	if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
		bz->x1 = x1; bz->y1 = y1;
		bz->x2 = x2; bz->y2 = y2;
		bz->x3 = x3; bz->y3 = y3;
		bz->x4 = x4; bz->y4 = y4;
		kind = BEZEKIND;
	}
	else{
		ln->x1 = x1; ln->y1 = y1;
		ln->x2 = x2; ln->y2 = y2;
		kind = LINEKIND;
	}
	return(kind);
}

int CCurveUtil::CheckBezeOverLimit(dbezetype bz)
{
	DBL		rag1, rag2, subrag;

	return(FALSE);

	if((bz.x1 == bz.x2 && bz.y1 == bz.y2)
	|| (bz.x3 == bz.x4 && bz.y3 == bz.y4)){
		return(FALSE);
	}
	rag1 = atan2(bz.y2-bz.y1, bz.x2-bz.x1);
	rag2 = atan2(bz.y4-bz.y3, bz.x4-bz.x3);
	subrag = rag2 - rag1;
	if(subrag > M_PI){
		subrag -= M_PI*2;
	}
	if(subrag < -M_PI){
		subrag += M_PI*2;
	}

	if(fabs(subrag) > M_PI_4){
		return(TRUE);
	}
	return(FALSE);
}

int CCurveUtil::SetDivBeze(dbezetype bz, CVect *pcVect, int dp)
{
	DBL		rag1, rag2, rag3;
	DBL		sub12, sub23, sub;
	int		div, i;
	DBL		step, st, et;
	dbezetype dbz;

	if((bz.x1 == bz.x2 && bz.y1 == bz.y2)
	|| (bz.x1 == bz.x4 && bz.y1 == bz.y4)
	|| (bz.x3 == bz.x4 && bz.y3 == bz.y4)){
		pcVect->SetAtrDXY(dp++, BEZEATR|CURVATR, bz.x1, bz.y1);
		pcVect->SetAtrDXY(dp++, BEZEATR, bz.x2, bz.y2);
		pcVect->SetAtrDXY(dp++, BEZEATR, bz.x3, bz.y3);
		return(dp);
	}
	rag1 = atan2(bz.y2 - bz.y1, bz.x2 - bz.x1);
	rag2 = atan2(bz.y4 - bz.y1, bz.x4 - bz.x1);
	rag3 = atan2(bz.y4 - bz.y3, bz.x4 - bz.x3);
	sub12 = rag1 - rag2;
	sub12 = curveraglegal(sub12);
	sub23 = rag2 - rag3;
	sub23 = curveraglegal(sub23);
	sub = fabs(sub12) + fabs(sub23);
	//div = sub / M_PI_2 + 1;		// ‚X‚O“x‚ÅŠ„‚é
	div = (int)(sub / (M_PI / 3.0)) + 1;		// ‚U‚O“x‚ÅŠ„‚é
	step = 1.0 / (DBL)div;
	for(i = 0; i < div; i++){
		st = step * (DBL)i;
		et = step * (DBL)(i+1);
		curvebezettdivget(bz, st, et, &dbz);
		pcVect->SetAtrDXY(dp++, BEZEATR|CURVATR, dbz.x1, dbz.y1);
		pcVect->SetAtrDXY(dp++, BEZEATR, dbz.x2, dbz.y2);
		pcVect->SetAtrDXY(dp++, BEZEATR, dbz.x3, dbz.y3);
	}
	return(dp);
}
