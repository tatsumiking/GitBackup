
/////////////////////////////////////////////////////////////////////////////
// CCmdLine

#include "stdafx.h"
#include "math.h"

#include "CmdBase.h"
#include "CmdSLine.h"
#include "cfigure.h"

//extern "C" {
	#include "../../ObjLib/Curvelib.h"
	#include "../../ObjLib/Crosslib.h"
	#include "../../ObjLib/Pdrwlib.h"
	#include "../../ObjLib/Scrnlib.h"
	#include "../../ObjLib/Senvlib.h"
//}

CCmdSLine::CCmdSLine(CScrollView* pcview)
:CCmdBase(pcview)
{

}

UINT CCmdSLine::ExecCutJoin(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_dSX = dx;
			m_dSY = dy;
			m_dEX = dx;
			m_dEY = dy;
			BeginDraw();
			DrawSELine(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			SaveUndo();
			m_dEX = dx;
			m_dEY = dy;
			CutJoinMain();
			m_nMouseSetCount = 0;
			ScrnAllRedraw();
			return(ENDEVENT);
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawSELine(m_pcDC);
			m_dEX = dx;
			m_dEY = dy;
			DrawSELine(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawSELine(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		break;
	case DLGRESETEVENT:
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawSELine(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

//P íçñ⁄èÛë‘ÇÃê¸ï\é¶ä÷êî
void CCmdSLine::DrawSELine(CDC *pcDC)
{
	DBL		x1, y1;
	DBL		x2, y2;
	DBL		xsize, ysize;
	DBL		k, d;
	DBL		sx, sy;
	DBL		ex, ey;

	x1 = m_dSX;
	y1 = m_dSY;
	x2 = m_dEX;
	y2 = m_dEY;
	if(x1 == x2 && y1 == y2){
		return;
	}
	scrngetmmsize(&xsize, &ysize);
	if(fabs(x2-x1) < fabs(y2-y1)){
		if(EQUAL01(x1, x2)){
			sx = x1; sy = 0;
			ex = x2; ey = ysize;
		}else{
			curvecrosslinekdget(x1,y1, x2, y2, &k, &d);
			sy = 0;
			sx = -d / k;;
			ey = ysize;
			ex = (ey-d) / k;
		}
	}else{
		if(EQUAL01(y1, y2)){
			sx = 0; sy = y1;
			ex = xsize; ey = y2;
		}else{
			curvecrosslinekdget(x1,y1, x2, y2, &k, &d);
			sx = 0;
			sy = sx * k + d;
			ex = xsize;
			ey = ex * k + d;
		}
	}
	m_cutline.x1 = sx; m_cutline.y1 = sy;
	m_cutline.x2 = ex; m_cutline.y2 = ey;
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	scrnsetslctpen(pcDC->m_hDC);
	pcDC->MoveTo((long)sx, (long)sy);
	pcDC->LineTo((long)ex, (long)ey);
	scrnpenend(pcDC->m_hDC);
}

void CCmdSLine::CutJoinMain()
{
	POSITION	pos;
	CFigure		*pcFg;

	m_cFigureList.RemoveAll();
	m_cAddFigureList.RemoveAll();
	m_cAFigureList.RemoveAll();
	m_cBFigureList.RemoveAll();
	m_ncrs = 0;
	SetDataListSelectID(BASEVECTID, ACTIVEFLAG);
	DivFiguPtrList();
	m_pcDoc->DataActiveCls();
	pos = m_cFigureList.GetHeadPosition();
	while(pos != NULL){
		pcFg = (CFigure*)m_cFigureList.GetNext(pos);
		if(pcFg->type == LINEKIND){
			GetCrossPointLinekind(m_cutline, pcFg);
		}else{
			GetCrossPointBezekind(m_cutline, pcFg);
		}
	}

	pos = m_cAddFigureList.GetHeadPosition();
	while(pos != NULL){
		pcFg = (CFigure*)m_cAddFigureList.GetNext(pos);
		m_cFigureList.AddTail(pcFg);
	}

	DivideGroup(m_cutline);

	SortCrossSetFigure(m_crs, m_ncrs);

	m_pcCrtList = &m_cAFigureList;
	LinkFigure();

	m_pcCrtList = &m_cBFigureList;
	LinkFigure();
}
void CCmdSLine::LinkFigure()
{
	CDataList	*pcDataList;
	CVect *pcVect;
	CFigure *pcFg;
	
	pcDataList = (CDataList*)(m_pcDoc->NewData(DATALISTID));
	if(pcDataList == NULL){
		return;
	}
	while(1){
		if(m_pcCrtList->GetCount() == 0){
			break;
		}
		pcFg = (CFigure*)m_pcCrtList->GetHead();
		m_pcCrtList->RemoveHead();
		pcVect = TraceVect(pcFg);
		if(pcVect != NULL){
			pcDataList->SetData((CData*)pcVect);
		}
	}
	pcDataList->RenewMiniMax();
	m_pcDoc->DataAddTail((CData*)pcDataList);
}

CVect *CCmdSLine::TraceVect(CFigure *pcTopFg)
{
	CVect *pcVect;
	DBL x, y;
	int pp;
	CFigure *pcFg;
	
	pcVect = (CVect*)(m_pcDoc->NewData(BASEVECTID));
	pp = 0;
	pp = VectSetFigure(pcVect, pcTopFg, pp);
	x = pcTopFg->x4; y = pcTopFg->y4;
	while(1){
		if(pcTopFg->x1 == x && pcTopFg->y1 == y){
			break;
		}
		if(m_pcCrtList->GetCount() == 0){
			break;
		}
		pcFg = SharchNextFigure(x, y);
		if(pcFg == NULL){
			break;
		}
		RemoveListFigure(pcFg);
		if(x == pcFg->x1 && y == pcFg->y1){
			pp = VectSetFigure(pcVect, pcFg, pp);
			x = pcFg->x4; y = pcFg->y4;
		}else if(x == pcFg->x4 && y == pcFg->y4){
			pp = VectReverseSetFigure(pcVect, pcFg, pp);
			x = pcFg->x1; y = pcFg->y1;
		}
	}
	pcVect->SetPointCount(pp);
	return(pcVect);
}

void CCmdSLine::RemoveListFigure(CFigure *pcSrcFg)
{
	POSITION pos, delpos;
	CFigure *pcFg;

	pos = m_pcCrtList->GetHeadPosition();
	while(pos != NULL){
		delpos = pos;
		pcFg = (CFigure*)m_pcCrtList->GetNext(pos);
		if(pcSrcFg == pcFg){
			m_pcCrtList->RemoveAt(delpos);
			break;
		}
	}
}

int CCmdSLine::VectSetFigure(CVect *pcVect, CFigure *pcFg, int pp)
{
	if(pcFg->type == LINEKIND){
		pcVect->SetAtrDXY(pp++, 0, pcFg->x1, pcFg->y1); 
	}else if(pcFg->type == BEZEKIND){
		pcVect->SetAtrDXY(pp++, CURVATR|BEZEATR, pcFg->x1, pcFg->y1);
		pcVect->SetAtrDXY(pp++, BEZEATR, pcFg->x2, pcFg->y2);
		pcVect->SetAtrDXY(pp++, BEZEATR, pcFg->x3, pcFg->y3);
	}
	return(pp);
}
int CCmdSLine::VectReverseSetFigure(CVect *pcVect, CFigure *pcFg, int pp)
{
	if(pcFg->type == LINEKIND){
		pcVect->SetAtrDXY(pp++, 0, pcFg->x4, pcFg->y4); 
	}else if(pcFg->type == BEZEKIND){
		pcVect->SetAtrDXY(pp++, CURVATR|BEZEATR, pcFg->x4, pcFg->y4);
		pcVect->SetAtrDXY(pp++, BEZEATR, pcFg->x3, pcFg->y3);
		pcVect->SetAtrDXY(pp++, BEZEATR, pcFg->x2, pcFg->y2);
	}
	return(pp);
}
CFigure *CCmdSLine::SharchNextFigure(DBL x, DBL y)
{
	POSITION pos;
	CFigure *pcFg;
	
	pos = m_pcCrtList->GetHeadPosition();
	while(pos != NULL){
		pcFg = (CFigure*)m_pcCrtList->GetNext(pos);
		if(x == pcFg->x1 && y == pcFg->y1){
			return(pcFg);
		}else if(x == pcFg->x4 && y == pcFg->y4){
			return(pcFg);
		}
	}
	return(NULL);
}
void CCmdSLine::SortCrossSetFigure(crosstype * crs, int crsmax)
{
	int		i, j;
	crosstype tcrs;
	CFigure *pcFg;

	for(i = 0; i < crsmax; i++){
		for(j = i; j < crsmax; j++){
			if(crs[i].len > crs[j].len){
				tcrs = crs[i];
				crs[i] = crs[j];
				crs[j] = tcrs;
			}
		}
	}
	for(i = 0; i < crsmax; i += 2){
		pcFg = new CFigure();
		pcFg->type = LINEKIND;
		pcFg->x1 = crs[i].xx;
		pcFg->y1 = crs[i].yy;
		pcFg->x4 = crs[i+1].xx;
		pcFg->y4 = crs[i+1].yy;
		m_cAFigureList.AddTail(pcFg);
		
		pcFg = new CFigure();
		pcFg->type = LINEKIND;
		pcFg->x1 = crs[i].xx;
		pcFg->y1 = crs[i].yy;
		pcFg->x4 = crs[i+1].xx;
		pcFg->y4 = crs[i+1].yy;
		m_cBFigureList.AddTail(pcFg);
	}
}

void CCmdSLine::DivideGroup(dlinetype cutline)
{
	POSITION pos;
	CFigure *pcFg;
	int nKind;
	
	pos = m_cFigureList.GetHeadPosition();
	while(pos != NULL){
		pcFg = (CFigure*)m_cFigureList.GetNext(pos);
		nKind = CheckNKind(cutline, pcFg);
		if(nKind < 0){
			m_cAFigureList.AddTail(pcFg);
		}else if(nKind > 0){
			m_cBFigureList.AddTail(pcFg);
		}
	}
}
int CCmdSLine::CheckNKind(dlinetype ln, CFigure *pcFg)
{
	int nKind;

	nKind = curvenkindget(ln.x1, ln.y1, ln.x2, ln.y2, pcFg->x1, pcFg->y1);
	if(nKind < 0){
		return(-1);
	}else if(nKind > 0){
		return(1);
	}
	nKind = curvenkindget(ln.x1, ln.y1, ln.x2, ln.y2, pcFg->x4, pcFg->y4);
	return(nKind);
}
void CCmdSLine::GetCrossPointLinekind(dlinetype bln, CFigure *pcFg)
{
	dlinetype ln;
	int		ret;
	DBL		xx, yy;
	DBL		len1, len2;
	CFigure *pcFgPair;


	ln.x1 = pcFg->x1; ln.y1 = pcFg->y1;
	ln.x2 = pcFg->x4; ln.y2 = pcFg->y4;
	ret = curvecrosslinelinexyget(bln, ln, &xx, &yy);
	if(ret == ON){
		AddCrossPoint(xx, yy);
		len1 = hypot(ln.x1 - xx, ln.y1 - yy);
		len2 = hypot(ln.x2 - xx, ln.y2 - yy);
		if(len1 < 0.1){
			pcFgPair = SrchEPPairFigure(pcFg->x1, pcFg->y1);
			pcFg->x1 = xx;
			pcFg->y1 = yy;
			pcFgPair->x4 = xx;
			pcFgPair->y4 = yy;
			return;
		}
		if(len2 < 0.1){
			pcFgPair = SrchSPPairFigure(pcFg->x4, pcFg->y4);
			pcFg->x4 = xx;
			pcFg->y4 = yy;
			pcFgPair->x1 = xx;
			pcFgPair->y1 = yy;
			return;
		}
		pcFg->x1 = ln.x1; pcFg->y1 = ln.y1;
		pcFg->x2 = ln.x1; pcFg->y2 = ln.y1;
		pcFg->x3 = xx; pcFg->y3 = yy;
		pcFg->x4 = xx; pcFg->y4 = yy;

		pcFg = new CFigure();
		pcFg->type = LINEKIND;
		pcFg->x1 = xx; pcFg->y1 = yy;
		pcFg->x2 = xx; pcFg->y2 = yy;
		pcFg->x3 = ln.x2; pcFg->y3 = ln.y2;
		pcFg->x4 = ln.x2; pcFg->y4 = ln.y2;
		m_cAddFigureList.AddTail(pcFg);
	}
}

void CCmdSLine::GetCrossPointBezekind(dlinetype ln, CFigure *pcFg)
{
	dbezetype bz;
	DBL		xtbl[4], ytbl[4], ttbl[4];
	int		cnt;
	DBL		xx, yy;
	DBL		len1, len4;
	CFigure *pcFgPair;
	DBL		st, et;
	dbezetype sbz;

	bz.x1 = pcFg->x1; bz.y1 = pcFg->y1;
	bz.x2 = pcFg->x2; bz.y2 = pcFg->y2;
	bz.x3 = pcFg->x3; bz.y3 = pcFg->y3;
	bz.x4 = pcFg->x4; bz.y4 = pcFg->y4;
	cnt = crossbezelinecrsex(&bz, &ln, xtbl, ytbl, ttbl);
	if(cnt == 1 || cnt == 3){
		xx = xtbl[0]; yy = ytbl[0];
		AddCrossPoint(xx, yy);
		len1 = hypot(bz.x1 - xx, bz.y1 - yy);
		len4 = hypot(bz.x4 - xx, bz.y4 - yy);
		if(len1 < 0.1){
			pcFgPair = SrchEPPairFigure(pcFg->x1, pcFg->y1);
			pcFg->x1 = xx;
			pcFg->y1 = yy;
			if(pcFgPair != NULL){
				pcFgPair->x4 = xx;
				pcFgPair->y4 = yy;
			}
			return;
		}
		if(len4 < 0.1){
			pcFgPair = SrchSPPairFigure(pcFg->x4, pcFg->y4);
			pcFg->x4 = xx;
			pcFg->y4 = yy;
			if(pcFgPair != NULL){
				pcFgPair->x1 = xx;
				pcFgPair->y1 = yy;
			}
			return;
		}
		st = 0.0;
		et = ttbl[0];
		curvebezettdivget(bz, st, et, &sbz);
		pcFg->x1 = bz.x1; pcFg->y1 = bz.y1;
		pcFg->x2 = sbz.x2; pcFg->y2 = sbz.y2;
		pcFg->x3 = sbz.x3; pcFg->y3 = sbz.y3;
		pcFg->x4 = xx; pcFg->y4 = yy;

		pcFg = new CFigure();
		pcFg->type = BEZEKIND;
		st = ttbl[0];
		et = 1.0;
		curvebezettdivget(bz, st, et, &sbz);
		pcFg->x1 = xx; pcFg->y1 = yy;
		pcFg->x2 = sbz.x2; pcFg->y2 = sbz.y2;
		pcFg->x3 = sbz.x3; pcFg->y3 = sbz.y3;
		pcFg->x4 = bz.x4; pcFg->y4 = bz.y4;
		m_cAddFigureList.AddTail(pcFg);
	}
}

CFigure *CCmdSLine::SrchEPPairFigure(DBL x1, DBL y1)
{
	POSITION pos;
	CFigure *pcFg;

	pos = m_cFigureList.GetHeadPosition();
	while(pos != NULL){
		pcFg = (CFigure*)m_cFigureList.GetNext(pos);
		if(EQUAL(pcFg->x4, pcFg->y4)){
			return(pcFg);
		}
	}
	pos = m_cAddFigureList.GetHeadPosition();
	while(pos != NULL){
		pcFg = (CFigure*)m_cAddFigureList.GetNext(pos);
		if(EQUAL(pcFg->x4, pcFg->y4)){
			return(pcFg);
		}
	}
	return(NULL);
}

CFigure *CCmdSLine::SrchSPPairFigure(DBL x4, DBL y4)
{
	POSITION pos;
	CFigure *pcFg;

	pos = m_cFigureList.GetHeadPosition();
	while(pos != NULL){
		pcFg = (CFigure*)m_cFigureList.GetNext(pos);
		if(EQUAL(pcFg->x1, pcFg->y1)){
			return(pcFg);
		}
	}
	pos = m_cAddFigureList.GetHeadPosition();
	while(pos != NULL){
		pcFg = (CFigure*)m_cAddFigureList.GetNext(pos);
		if(EQUAL(pcFg->x1, pcFg->y1)){
			return(pcFg);
		}
	}
	return(NULL);
}

void CCmdSLine::AddCrossPoint(DBL xx, DBL yy)
{
	m_crs[m_ncrs].len = hypot(m_cutline.x1 - xx, m_cutline.y1 - yy);
	m_crs[m_ncrs].xx = xx;
	m_crs[m_ncrs].yy = yy;
	m_ncrs++;
}
void CCmdSLine::SortCrossPoint()
{
	int		i, j;
	crosstype tcrs;

	for(i = 0; i < m_ncrs; i++){
		for(j = i; j < m_ncrs; j++){
			if(m_crs[i].len > m_crs[j].len){
				tcrs = m_crs[i];
				m_crs[i] = m_crs[j];
				m_crs[j] = tcrs;
			}
		}
	}
}


// ï™äÑãyÇ—BEZEâª
void CCmdSLine::DivFiguPtrList()
{
	POSITION	pos;
	CVect		*pcVect;

	pos = m_cPtrList.GetHeadPosition();
	while(pos != NULL){
		pcVect = (CVect*)m_cPtrList.GetNext(pos);
		DivFiguElementVect(pcVect);
	}
}

void CCmdSLine::DivFiguElementVect(CVect *pcVect)
{
	int pcnt, pp, np;
	dfigutype figu;
	CFigure *pcFg;
	int kind;

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
			ConvertFGArcToBeze(&figu, pcFg);
		}else if(kind == FREEKIND){
			ConvertFGFreeToCFigure(&figu, pcFg);
		}
		m_cFigureList.AddTail(pcFg);
		pp = np;
	}
}

void CCmdSLine::ConvertFGArcToBeze(dfigutype *fg, CFigure *pcFg)
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

void CCmdSLine::ConvertFGFreeToCFigure(dfigutype *fg, CFigure *pcFg)
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

