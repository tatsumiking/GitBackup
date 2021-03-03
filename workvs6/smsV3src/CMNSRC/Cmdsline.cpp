
/////////////////////////////////////////////////////////////////////////////
// CCmdSpclLine

#include "stdafx.h"
#include "math.h"

#include "cmd_base.h"
#include "cmdsline.h"

extern "C" {
	void scrndxymmtorltv(LPDBL x, LPDBL y);
	void scrnsetslctpen(HDC PaintDC);
	void scrnsetpreviewpen(HDC PaintDC, int r, int g, int b, int nthick);
	void scrnpenend(HDC PaintDC);
	int curvecrosslnlnxyget(dlinetype line1, dlinetype line2, DBL *x, DBL *y);
	int curveperpendicular(DBL sx, DBL sy, DBL ex, DBL ey, DBL x, DBL y, DBL *cx, DBL *cy);
	int curvenkindget(DBL sx, DBL sy, DBL ex, DBL ey, DBL x, DBL y);
}

CCmdSpclLine::CCmdSpclLine(CScrollView* pcview)
:CCmdBase(pcview)
{
}

UINT CCmdSpclLine::ExecParallel(UINT event, DBL dx, DBL dy)
{
	DBL		retx, rety;
	int		retpp;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		HlpMsgDsp("基準線を指定してください");
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_pcVect = (CVect*)SrchDataOnLinePoint(dx, dy, &retx, &rety, &retpp);
			if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
				m_nPP = retpp;
				BeginDraw();
				scrnsetslctpen(m_pcDC->m_hDC);
				m_pcVect->OneFiguDraw(m_pcDC, m_nPP);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 1;
				HlpMsgDsp("平行にする線を指定してください");
			}
		}
		else if(m_nMouseSetCount == 1){
			m_pcVect2 = (CVect*)SrchDataOnLinePoint(dx, dy, &retx, &rety, &retpp);
			if(m_pcVect2 != NULL && m_pcVect2->CheckID() == BASEVECTID){
				m_nPP2 = retpp;
				BeginDraw();
				scrnsetslctpen(m_pcDC->m_hDC);
				m_pcVect2->OneFiguDraw(m_pcDC, m_nPP2);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 2;
				HlpMsgDsp("支点を指定してください");
			}
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			scrnsetslctpen(m_pcDC->m_hDC);
			m_pcVect->OneFiguDraw(m_pcDC, m_nPP);
			m_pcVect2->OneFiguDraw(m_pcDC, m_nPP2);
			scrnpenend(m_pcDC->m_hDC);
			m_pcVect2->PLDraw(m_pcDC);
			EndDraw();

			SaveUndo();
			ParallelMain(dx, dy);

			BeginDraw();
			m_pcVect2->PLDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
			HlpMsgDsp("基準線を指定してください");
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdSpclLine::ExecPrepend(UINT event, DBL dx, DBL dy)
{
	DBL		retx, rety;
	int		retpp;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		HlpMsgDsp("基準線を指定してください");
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_pcVect = (CVect*)SrchDataOnLinePoint(dx, dy, &retx, &rety, &retpp);
			if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
				m_nPP = retpp;
				BeginDraw();
				scrnsetslctpen(m_pcDC->m_hDC);
				m_pcVect->OneFiguDraw(m_pcDC, m_nPP);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 1;
				HlpMsgDsp("垂直にする線を指定してください");
			}
		}
		else if(m_nMouseSetCount == 1){
			m_pcVect2 = (CVect*)SrchDataOnLinePoint(dx, dy, &retx, &rety, &retpp);
			if(m_pcVect2 != NULL && m_pcVect2->CheckID() == BASEVECTID){
				m_nPP2 = retpp;
				BeginDraw();
				scrnsetslctpen(m_pcDC->m_hDC);
				m_pcVect2->OneFiguDraw(m_pcDC, m_nPP2);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 2;
				HlpMsgDsp("支点を指定してください");
			}
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			scrnsetslctpen(m_pcDC->m_hDC);
			m_pcVect->OneFiguDraw(m_pcDC, m_nPP);
			scrnpenend(m_pcDC->m_hDC);
			m_pcVect2->PLDraw(m_pcDC);
			EndDraw();

			SaveUndo();
			PrependMain(dx, dy);

			BeginDraw();
			m_pcVect2->PLDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
			HlpMsgDsp("基準線を指定してください");
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

void CCmdSpclLine::ParallelMain(DBL dx, DBL dy)
{
	int		pcnt, pp;
	DBL		sx, sy, ex, ey;
	DBL		xx, yy;
	DBL		mvx, mvy;
	dlinetype	ln1, ln2;

	pcnt = m_pcVect->GetPointCount();
	pp = m_nPP;
	m_pcVect->GetDXY(pp, &sx, &sy);
	pp = m_pcVect->PointLegal(pcnt, pp+1);
	m_pcVect->GetDXY(pp, &ex, &ey);

	curveperpendicular(sx, sy, ex, ey, dx, dy, &xx, &yy);
	mvx = dx - xx; mvy = dy - yy;
	sx += mvx; sy +=mvy;
	ex += mvx; ey +=mvy;
	ln1.x1 = sx; ln1.y1 = sy;
	ln1.x2 = ex; ln1.y2 = ey;

	pcnt = m_pcVect2->GetPointCount();
	pp = m_pcVect2->PointLegal(pcnt, m_nPP2-1);
	m_pcVect2->GetDXY(pp, &(ln2.x1), &(ln2.y1));
	pp = m_pcVect2->PointLegal(pcnt, m_nPP2);
	m_pcVect2->GetDXY(pp, &(ln2.x2), &(ln2.y2));
	curvecrosslnlnxyget(ln1, ln2, &xx, &yy);

	pp = m_pcVect2->PointLegal(pcnt, m_nPP2);
	m_pcVect2->SetDXY(pp, xx, yy);

	pp = m_pcVect2->PointLegal(pcnt, m_nPP2+1);
	m_pcVect2->GetDXY(pp, &(ln2.x1), &(ln2.y1));
	pp = m_pcVect2->PointLegal(pcnt, m_nPP2+2);
	m_pcVect2->GetDXY(pp, &(ln2.x2), &(ln2.y2));
	curvecrosslnlnxyget(ln1, ln2, &xx, &yy);

	pp = m_pcVect2->PointLegal(pcnt, m_nPP2+1);
	m_pcVect2->SetDXY(pp, xx, yy);
}

void CCmdSpclLine::PrependMain(DBL dx, DBL dy)
{
	int		pcnt, pp;
	DBL		sx, sy, ex, ey;
	DBL		xx, yy;
	int		ret;
	dlinetype	ln1, ln2;

	pcnt = m_pcVect->GetPointCount();
	pp = m_nPP;
	m_pcVect->GetDXY(pp, &sx, &sy);
	pp = m_pcVect->PointLegal(pcnt, pp+1);
	m_pcVect->GetDXY(pp, &ex, &ey);
	ret = curveperpendicular(sx, sy, ex, ey, dx, dy, &xx, &yy);
	if(ret == FAIL || (dx == xx && dy == yy)){
		sx += 100.0; sy += 100.0;
		ex += 100.0; ey += 100.0;
		ret = curveperpendicular(sx, sy, ex, ey, dx, dy, &xx, &yy);
	}
	ln1.x1 = dx; ln1.y1 = dy;
	ln1.x2 = xx; ln1.y2 = yy;
	
	pcnt = m_pcVect2->GetPointCount();
	pp = m_pcVect2->PointLegal(pcnt, m_nPP2-1);
	m_pcVect2->GetDXY(pp, &(ln2.x1), &(ln2.y1));
	pp = m_pcVect2->PointLegal(pcnt, m_nPP2);
	m_pcVect2->GetDXY(pp, &(ln2.x2), &(ln2.y2));
	curvecrosslnlnxyget(ln1, ln2, &xx, &yy);

	pp = m_pcVect2->PointLegal(pcnt, m_nPP2);
	m_pcVect2->SetDXY(pp, xx, yy);

	pp = m_pcVect2->PointLegal(pcnt, m_nPP2+1);
	m_pcVect2->GetDXY(pp, &(ln2.x1), &(ln2.y1));
	pp = m_pcVect2->PointLegal(pcnt, m_nPP2+2);
	m_pcVect2->GetDXY(pp, &(ln2.x2), &(ln2.y2));
	curvecrosslnlnxyget(ln1, ln2, &xx, &yy);

	pp = m_pcVect2->PointLegal(pcnt, m_nPP2+1);
	m_pcVect2->SetDXY(pp, xx, yy);
}

void CCmdSpclLine::DrawLine(DBL sx, DBL sy, DBL ex, DBL ey)
{
	BeginDraw();
	scrnsetpreviewpen(m_pcDC->m_hDC, 255, 0, 0, 2);
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	m_pcDC->MoveTo((int)sx, (int)sy);
	m_pcDC->LineTo((int)ex, (int)ey);
	scrnpenend(m_pcDC->m_hDC);
	EndDraw();
}
