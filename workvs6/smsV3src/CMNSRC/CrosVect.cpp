
/////////////////////////////////////////////////////////////////////////////
// CCrossVect

#include "stdafx.h"
#include "math.h"

#include "crosvect.h"
#include "..\Crosslib\Cross.h"

extern "C" {
	int curve3pcenterget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL FAR *x0, DBL FAR *y0);
	int curve3pragrget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0, DBL FAR *rag01, DBL FAR *rag03, DBL FAR *r);
	void curvenoragtocurverag(DBL *rag1, DBL *rag2);
	int curveragnoget(DBL rag);
	void curvebezecontrolpointget(DBL x1, DBL y1, DBL x2, DBL y2, DBL rag, DBL *xp, DBL *yp);
	void curvebezetpointget(dbezetype bz, DBL t, DBL *x, DBL *y);
	int curvecrosslineinareachk(dlinetype line1, dlinetype line2);
	int curvecrosslinelinexyget(dlinetype line1, dlinetype line2, DBL *x, DBL *y);
	BOOL  pdrwatrarcchk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrbezechk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrcurvechk(int atr1);
	DBL   pdrwendragget(int atr1, int atr2, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL   pdrwstartragget(int atr0, int atr1, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2);
}

CCrossVect::CCrossVect(CScrollView* pcview)
{
	m_pcView = (CTonView*)pcview;
	m_pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_dSpaceMM = 0;
}

void CCrossVect::SetSpaceMM(DBL dmm)
{
	m_dSpaceMM = dmm;
}

void CCrossVect::SetDataList(CTypedPtrList<CObList,CData*> *pList)
{
	m_pcPtrList = pList;
}

void CCrossVect::GetCrossPoint(dlinetype ln1, dcrostype * crs, int *crsmax)
{
	POSITION	pos;
	CData		*pcData;
	CVect		*pcVect;

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
	}
}

void CCrossVect::GetCrossPointVect(CVect *pcVect, dlinetype ln1, dcrostype * crs, int *crsmax)
{
	if(pcVect->CheckFlag(REJIONFLAG) == ON){
		GetRejionCrossPoint(pcVect, ln1, crs, crsmax);
	}
	else{
		GetNoRejionCrossPoint(pcVect, ln1, crs, crsmax);
	}
}

/////////////////////////////////////////////////////////////////////////////
//ベクトルデータの回転方向の調査。　返す値が０＝反時計回り、１＝時計回り
int CCrossVect::SearchVectRotateDirection(CVect *pcVect)
{
	DBL x_data, y_data, x_old, y_old, x_first, y_first, nz=0.0;

	int loop, interval;
	int point_number = (pcVect->GetPointCount());

	if(point_number>11)
	{
		interval = 2;
	}
	else
	{
		interval = 1;
	}

	pcVect->GetDXY(0, &x_first, &y_first);
	x_old = x_first;
	y_old = y_first;
	for(loop=interval; loop<point_number; loop+=interval)
	{
		pcVect->GetDXY(loop, &x_data, &y_data);
		nz = nz + (x_old-x_data)*(y_old+y_data);
		x_old = x_data;
		y_old = y_data;
	}
	nz = nz + (x_old-x_first)*(y_old+y_first);

	if(nz>=0.0)
	{
		return(0);
	}
	else
	{
		return(1);
	}
}

void CCrossVect::GetRejionCrossPoint(CVect *pcVect, dlinetype ln1, dcrostype * crs, int *crsmax)
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		rag1, rag2;
	int		rag1no, rag2no, result, loop;
	DBL		xp1, yp1, xp2, yp2;
	dlinetype	ln2;
	dbezetype	bz2;
	darctype	ac2;
	CCross CrossRutine;
	double aTempT0[10];
	double aTempT1[10];
	double aTempX[10];
	double aTempY[10];

	pcnt = pcVect->GetPointCount();
	
	if(pcnt <= 1){
		return;
	}
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
				CrossRutine.Reset(ln1, ln2);
				result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
				for(loop=0; loop<result; ++loop)
				{
					(crs[(*crsmax)+loop].xx) = aTempX[loop];
					(crs[(*crsmax)+loop].yy) = aTempY[loop];
					(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
					(crs[(*crsmax)+loop].pp) = pp;
				}
				(*crsmax) += result;

				ln2.x1 = x2; ln2.y1 = y2;
				ln2.x2 = x3; ln2.y2 = y3;
				CrossRutine.Reset(ln1, ln2);
				result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
				for(loop=0; loop<result; ++loop)
				{
					(crs[(*crsmax)+loop].xx) = aTempX[loop];
					(crs[(*crsmax)+loop].yy) = aTempY[loop];
					(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
					(crs[(*crsmax)+loop].pp) = pp;
				}
				(*crsmax) += result;
			}
			else if(p2 == 0){
				ln2.x1 = x1; ln2.y1 = y1;
				ln2.x2 = x2; ln2.y2 = y2;
				CrossRutine.Reset(ln1, ln2);
				result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
				for(loop=0; loop<result; ++loop)
				{
					(crs[(*crsmax)+loop].xx) = aTempX[loop];
					(crs[(*crsmax)+loop].yy) = aTempY[loop];
					(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
					(crs[(*crsmax)+loop].pp) = pp;
				}
				(*crsmax) += result;
			}
			else{
				bz2.x1 = x1; bz2.y1 = y1;
				bz2.x2 = x2; bz2.y2 = y2;
				bz2.x3 = x3; bz2.y3 = y3;
				bz2.x4 = x4; bz2.y4 = y4;
				CrossRutine.Reset(ln1, bz2);
				result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
				for(loop=0; loop<result; ++loop)
				{
					(crs[(*crsmax)+loop].xx) = aTempX[loop];
					(crs[(*crsmax)+loop].yy) = aTempY[loop];
					(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
					(crs[(*crsmax)+loop].pp) = pp;
				}
				(*crsmax) += result;
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				ac2.x1 = x1; ac2.y1 = y1;
				ac2.x2 = x2; ac2.y2 = y2;
				ac2.x3 = x3; ac2.y3 = y3;
				CrossRutine.Reset(ln1, ac2);
				result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
				for(loop=0; loop<result; ++loop)
				{
					(crs[(*crsmax)+loop].xx) = aTempX[loop];
					(crs[(*crsmax)+loop].yy) = aTempY[loop];
					(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
					(crs[(*crsmax)+loop].pp) = pp;
				}
				(*crsmax) += result;
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					ac2.x1 = x0; ac2.y1 = y0;
					ac2.x2 = x1; ac2.y2 = y1;
					ac2.x3 = x2; ac2.y3 = y2;
					CrossRutine.Reset(ln1, ac2);
					result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
					for(loop=0; loop<result; ++loop)
					{
						(crs[(*crsmax)+loop].xx) = aTempX[loop];
						(crs[(*crsmax)+loop].yy) = aTempY[loop];
						(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
						(crs[(*crsmax)+loop].pp) = pp;
					}
					(*crsmax) += result;
				}
				else{
					rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
					rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);
					if(rag1 == NORAG && rag2 == NORAG){
						ln2.x1 = x1; ln2.y1 = y1;
						ln2.x2 = x2; ln2.y2 = y2;
						CrossRutine.Reset(ln1, ln2);
						result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
						for(loop=0; loop<result; ++loop)
						{
							(crs[(*crsmax)+loop].xx) = aTempX[loop];
							(crs[(*crsmax)+loop].yy) = aTempY[loop];
							(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
							(crs[(*crsmax)+loop].pp) = pp;
						}
						(*crsmax) += result;
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
						CrossRutine.Reset(ln1, bz2);
						result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
						for(loop=0; loop<result; ++loop)
						{
							(crs[(*crsmax)+loop].xx) = aTempX[loop];
							(crs[(*crsmax)+loop].yy) = aTempY[loop];
							(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
							(crs[(*crsmax)+loop].pp) = pp;
						}
						(*crsmax) += result;
					}
				}
			}
			else{
				ln2.x1 = x1; ln2.y1 = y1;
				ln2.x2 = x2; ln2.y2 = y2;
				CrossRutine.Reset(ln1, ln2);
				result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
				for(loop=0; loop<result; ++loop)
				{
					(crs[(*crsmax)+loop].xx) = aTempX[loop];
					(crs[(*crsmax)+loop].yy) = aTempY[loop];
					(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
					(crs[(*crsmax)+loop].pp) = pp;
				}
				(*crsmax) += result;
			}
			pp += 1;
		}
	}
}

void CCrossVect::GetNoRejionCrossPoint(CVect *pcVect, dlinetype ln1, dcrostype * crs, int *crsmax)
{
	int		pcnt, pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		rag1, rag2;
	int		rag1no, rag2no, result, loop;
	DBL		xp1, yp1, xp2, yp2;
	dlinetype	ln2;
	dbezetype	bz2;
	darctype	ac2;
	CCross CrossRutine;
	double aTempT0[10];
	double aTempT1[10];
	double aTempX[10];
	double aTempY[10];

	pcnt = pcVect->GetPointCount();

	if(pcnt <= 1){
		return;
	}
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
				CrossRutine.Reset(ln1, ln2);
				result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
				for(loop=0; loop<result; ++loop)
				{
					(crs[(*crsmax)+loop].xx) = aTempX[loop];
					(crs[(*crsmax)+loop].yy) = aTempY[loop];
					(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
					(crs[(*crsmax)+loop].pp) = pp;
				}
				(*crsmax) += result;

				ln2.x1 = x2; ln2.y1 = y2;
				ln2.x2 = x3; ln2.y2 = y3;
				CrossRutine.Reset(ln1, ln2);
				result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
				for(loop=0; loop<result; ++loop)
				{
					(crs[(*crsmax)+loop].xx) = aTempX[loop];
					(crs[(*crsmax)+loop].yy) = aTempY[loop];
					(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
					(crs[(*crsmax)+loop].pp) = pp;
				}
				(*crsmax) += result;
			}
			else if(p3 == 0){
				ln2.x1 = x1; ln2.y1 = y1;
				ln2.x2 = x2; ln2.y2 = y2;
				CrossRutine.Reset(ln1, ln2);
				result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
				for(loop=0; loop<result; ++loop)
				{
					(crs[(*crsmax)+loop].xx) = aTempX[loop];
					(crs[(*crsmax)+loop].yy) = aTempY[loop];
					(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
					(crs[(*crsmax)+loop].pp) = pp;
				}
				(*crsmax) += result;
			}
			else{
				bz2.x1 = x1; bz2.y1 = y1;
				bz2.x2 = x2; bz2.y2 = y2;
				bz2.x3 = x3; bz2.y3 = y3;
				bz2.x4 = x4; bz2.y4 = y4;
				CrossRutine.Reset(ln1, bz2);
				result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
				for(loop=0; loop<result; ++loop)
				{
					(crs[(*crsmax)+loop].xx) = aTempX[loop];
					(crs[(*crsmax)+loop].yy) = aTempY[loop];
					(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
					(crs[(*crsmax)+loop].pp) = pp;
				}
				(*crsmax) += result;
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				ln2.x1 = x1; ln2.y1 = y1;
				ln2.x2 = x2; ln2.y2 = y2;
				CrossRutine.Reset(ln1, ln2);
				result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
				for(loop=0; loop<result; ++loop)
				{
					(crs[(*crsmax)+loop].xx) = aTempX[loop];
					(crs[(*crsmax)+loop].yy) = aTempY[loop];
					(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
					(crs[(*crsmax)+loop].pp) = pp;
				}
				(*crsmax) += result;
			}
			else{
				ac2.x1 = x1; ac2.y1 = y1;
				ac2.x2 = x2; ac2.y2 = y2;
				ac2.x3 = x3; ac2.y3 = y3;
				CrossRutine.Reset(ln1, ac2);
				result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
				for(loop=0; loop<result; ++loop)
				{
					(crs[(*crsmax)+loop].xx) = aTempX[loop];
					(crs[(*crsmax)+loop].yy) = aTempY[loop];
					(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
					(crs[(*crsmax)+loop].pp) = pp;
				}
				(*crsmax) += result;
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
					CrossRutine.Reset(ln1, ln2);
					result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
					for(loop=0; loop<result; ++loop)
					{
						(crs[(*crsmax)+loop].xx) = aTempX[loop];
						(crs[(*crsmax)+loop].yy) = aTempY[loop];
						(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
						(crs[(*crsmax)+loop].pp) = pp;
					}
					(*crsmax) += result;
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
						CrossRutine.Reset(ln1, bz2);
						result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
						for(loop=0; loop<result; ++loop)
						{
							(crs[(*crsmax)+loop].xx) = aTempX[loop];
							(crs[(*crsmax)+loop].yy) = aTempY[loop];
							(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
							(crs[(*crsmax)+loop].pp) = pp;
						}
						(*crsmax) += result;
				}
			}
			else{
				ln2.x1 = x1; ln2.y1 = y1;
				ln2.x2 = x2; ln2.y2 = y2;
				CrossRutine.Reset(ln1, ln2);
				result = CrossRutine.SearchPoint(aTempX, aTempY, aTempT0, aTempT1);
				for(loop=0; loop<result; ++loop)
				{
					(crs[(*crsmax)+loop].xx) = aTempX[loop];
					(crs[(*crsmax)+loop].yy) = aTempY[loop];
					(crs[(*crsmax)+loop].pcData) = (CData *)pcVect;
					(crs[(*crsmax)+loop].pp) = pp;
				}
				(*crsmax) += result;
			}
			pp += 1;
		}
	}
}

