
/////////////////////////////////////////////////////////////////////////////
// CCrossLib

#include "stdafx.h"
#include "CrossLib.h"

//extern "C" {
#include "../../ObjLib/Curvelib.h"
#include "../../ObjLib/Crosslib.h"
#include "../../ObjLib/Pdrwlib.h"
//}

CCrossLib::CCrossLib(CScrollView* pcview)
{
	m_pcView = (CTonView*)pcview;
	m_pcDoc = (CTonDoc*)(m_pcView->GetDocument());
}

void CCrossLib::SetDataList(CTypedPtrList<CObList,CData*> *pList)
{
	m_pcPtrList = pList;
}

void CCrossLib::GetCrossPoint(dlinetype ln1, dcrostype * crs, int *crsmax, int limit)
{
	POSITION	pos;
	CData		*pcData;
	CVect		*pcVect;

	*crsmax = 0;
	pos = m_pcPtrList->GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_pcPtrList->GetNext(pos);
		if(pcData == NULL){
			break;
		}
		pcVect = (CVect*)pcData;
		if(pcVect->CheckFlag(REJIONFLAG) == ON){
			GetRejionCrossPoint(pcVect, ln1, crs, crsmax);
		}
		else{
			GetNoRejionCrossPoint(pcVect, ln1, crs, crsmax);
		}
		if((limit-16) < (*crsmax))
		{
			return;
		}
	}
}

void CCrossLib::GetCrossPointVect(CVect *pcVect, dlinetype ln1, dcrostype * crs, int *crsmax)
{
	if(pcVect->CheckFlag(REJIONFLAG) == ON){
		GetRejionCrossPoint(pcVect, ln1, crs, crsmax);
	}
	else{
		GetNoRejionCrossPoint(pcVect, ln1, crs, crsmax);
	}
}

void CCrossLib::GetRejionCrossPoint(CVect *pcVect, dlinetype ln1, dcrostype * crs, int *crsmax)
{
	int		scrsmax, ecrsmax, idx;
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		rag1, rag2;
	int		rag1no, rag2no;
	DBL		xp1, yp1, xp2, yp2;
	dlinetype	ln2;
	dbezetype	bz2;
	darctype	ac2;

	pcnt = pcVect->GetPointCount();
	if(pcnt <= 1){
		return;
	}
	scrsmax = *crsmax;
	m_pcVect = pcVect;
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
				ln2.x1 = x1; ln2.y1 = y1;
				ln2.x2 = x2; ln2.y2 = y2;
				GetCrossPointFigu(ln1, ln2, pp, crs, crsmax);
				ln2.x1 = x2; ln2.y1 = y2;
				ln2.x2 = x3; ln2.y2 = y3;
				GetCrossPointFigu(ln1, ln2, pp, crs, crsmax);
			}
			else if(p2 == 0){
				ln2.x1 = x1; ln2.y1 = y1;
				ln2.x2 = x2; ln2.y2 = y2;
				GetCrossPointFigu(ln1, ln2, pp, crs, crsmax);
			}
			else{
				bz2.x1 = x1; bz2.y1 = y1;
				bz2.x2 = x2; bz2.y2 = y2;
				bz2.x3 = x3; bz2.y3 = y3;
				bz2.x4 = x4; bz2.y4 = y4;
				GetCrossPointFigu(ln1, bz2, pp, crs, crsmax);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				ac2.x1 = x1; ac2.y1 = y1;
				ac2.x2 = x2; ac2.y2 = y2;
				ac2.x3 = x3; ac2.y3 = y3;
				GetCrossPointFigu(ln1, ac2, pp, crs, crsmax);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					ac2.x1 = x0; ac2.y1 = y0;
					ac2.x2 = x1; ac2.y2 = y1;
					ac2.x3 = x2; ac2.y3 = y2;
					GetCrossPointFigu(ln1, ac2, pp, crs, crsmax);
				}
				else{
					rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
					rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);
					if(rag1 == NORAG && rag2 == NORAG){
						ln2.x1 = x1; ln2.y1 = y1;
						ln2.x2 = x2; ln2.y2 = y2;
						GetCrossPointFigu(ln1, ln2, pp, crs, crsmax);
					}
					else{
						curvenoragtocurverag(&rag1, &rag2);
						rag1no = curveragnoget(rag1);
						rag2no = curveragnoget(rag2);
						curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
						curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
						bz2.x1 = x1; bz2.y1 = y1;
						bz2.x2 = xp1; bz2.y2 = yp1;
						bz2.x3 = xp2; bz2.y3 = yp2;
						bz2.x4 = x2; bz2.y4 = y2;
						GetCrossPointFigu(ln1, bz2, pp, crs, crsmax);
					}
				}
			}
			else{
				ln2.x1 = x1; ln2.y1 = y1;
				ln2.x2 = x2; ln2.y2 = y2;
				GetCrossPointFigu(ln1, ln2, pp, crs, crsmax);
			}
			pp += 1;
		}
	}
	ecrsmax = *crsmax;
	for(idx = scrsmax; idx < ecrsmax; idx++){
		crs[idx].pcData = pcVect;
	}
}

void CCrossLib::GetNoRejionCrossPoint(CVect *pcVect, dlinetype ln1, dcrostype * crs, int *crsmax)
{
	int		scrsmax, ecrsmax, idx;
	int		pcnt, pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		rag1, rag2;
	int		rag1no, rag2no;
	DBL		xp1, yp1, xp2, yp2;
	dlinetype	ln2;
	dbezetype	bz2;
	darctype	ac2;

	pcnt = pcVect->GetPointCount();
	if(pcnt <= 1){
		return;
	}
	scrsmax = *crsmax;
	m_pcVect = pcVect;
	for(pp = 0; pp < pcnt-1;){
		p0 = pcVect->PointLegal(pcnt, pp-1);
		p1 = pcVect->PointLegal(pcnt, pp+0);
		p2 = pcVect->PointLegal(pcnt, pp+1);
		p3 = pcVect->PointLegal(pcnt, pp+2);
		p4 = pcVect->PointLegal(pcnt, pp+3);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
		pcVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		pcVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		pcVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		pcVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		pcVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				ln2.x1 = x1; ln2.y1 = y1;
				ln2.x2 = x2; ln2.y2 = y2;
				GetCrossPointFigu(ln1, ln2, pp, crs, crsmax);
				ln2.x1 = x2; ln2.y1 = y2;
				ln2.x2 = x3; ln2.y2 = y3;
				GetCrossPointFigu(ln1, ln2, pp, crs, crsmax);
			}
			else if(p3 == 0){
				ln2.x1 = x1; ln2.y1 = y1;
				ln2.x2 = x2; ln2.y2 = y2;
				GetCrossPointFigu(ln1, ln2, pp, crs, crsmax);
			}
			else{
				bz2.x1 = x1; bz2.y1 = y1;
				bz2.x2 = x2; bz2.y2 = y2;
				bz2.x3 = x3; bz2.y3 = y3;
				bz2.x4 = x4; bz2.y4 = y4;
				GetCrossPointFigu(ln1, bz2, pp, crs, crsmax);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				ln2.x1 = x1; ln2.y1 = y1;
				ln2.x2 = x2; ln2.y2 = y2;
				GetCrossPointFigu(ln1, ln2, pp, crs, crsmax);
			}
			else{
				ac2.x1 = x1; ac2.y1 = y1;
				ac2.x2 = x2; ac2.y2 = y2;
				ac2.x3 = x3; ac2.y3 = y3;
				GetCrossPointFigu(ln1, ac2, pp, crs, crsmax);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
				rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);
				if(rag1 == NORAG && rag2 == NORAG){
					ln2.x1 = x1; ln2.y1 = y1;
					ln2.x2 = x2; ln2.y2 = y2;
					GetCrossPointFigu(ln1, ln2, pp, crs, crsmax);
				}
				else{
					curvenoragtocurverag(&rag1, &rag2);
					rag1no = curveragnoget(rag1);
					rag2no = curveragnoget(rag2);
					curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
					curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
					bz2.x1 = x1; bz2.y1 = y1;
					bz2.x2 = xp1; bz2.y2 = yp1;
					bz2.x3 = xp2; bz2.y3 = yp2;
					bz2.x4 = x2; bz2.y4 = y2;
					GetCrossPointFigu(ln1, bz2, pp, crs, crsmax);
				}
			}
			else{
				ln2.x1 = x1; ln2.y1 = y1;
				ln2.x2 = x2; ln2.y2 = y2;
				GetCrossPointFigu(ln1, ln2, pp, crs, crsmax);
			}
			pp += 1;
		}
	}
	ecrsmax = *crsmax;
	for(idx = scrsmax; idx < ecrsmax; idx++){
		crs[idx].pcData = pcVect;
	}
}

void CCrossLib::GetCrossPointFigu(dlinetype ln1, dlinetype ln2, int pp, dcrostype *crs, int *crsmax)
{
	DBL		xx, yy;
	int		ret;
	int		idx;

	idx = *crsmax;
	ret = curvecrosslinelinexyget(ln1, ln2, &xx, &yy);
	if(ret == ON){
		crs[idx].pp = pp;
		crs[idx].xx = xx;
		crs[idx].yy = yy;
		idx++;
	}
	*crsmax = idx;
}

void CCrossLib::GetCrossPointFigu(dlinetype ln1, darctype ac2, int pp, dcrostype *crs, int *crsmax)
{
	DBL		xtbl[3], ytbl[3];
	int		cnt, i;
	int		idx;
	dbezetype	bz2;

	idx = *crsmax;
	cruvearctobeze(ac2, &bz2);
	cnt = crossbezelinecrs(&bz2, &ln1, xtbl, ytbl);
	//cnt = crossarclinecrs(&ac2, &ln1, xtbl, ytbl);
	for(i = 0; i < cnt; i++){
		crs[idx].pp = pp;
		crs[idx].xx = xtbl[i];
		crs[idx].yy = ytbl[i];
		idx++;
	}
	*crsmax = idx;
}

void CCrossLib::GetCrossPointFigu(dlinetype ln1, dbezetype bz2, int pp, dcrostype *crs, int *crsmax)
{
	DBL		xtbl[4], ytbl[4];
	int		cnt, i;
	int		idx;

	idx = *crsmax;
	cnt = crossbezelinecrs(&bz2, &ln1, xtbl, ytbl);
	for(i = 0; i < cnt; i++){
		crs[idx].pp = pp;
		crs[idx].xx = xtbl[i];
		crs[idx].yy = ytbl[i];
		idx++;
	}
	*crsmax = idx;
}
