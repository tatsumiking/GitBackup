
/////////////////////////////////////////////////////////////////////////////
// CCmdFigu

#include "stdafx.h"
#include "math.h"

#include "cmd_rule.h"

extern "C" {
	void scrndxymmtorltv(LPDBL x, LPDBL y);
	void scrndlenmmtorltv(LPDBL len);
	void figuenvgetcntrkind(LPINT cntrkind);
	void figuenvgetplgncount(LPINT plgncount);
}

CCmdRule::CCmdRule(CScrollView* pcview)
	:CCmdBase(pcview)
{
}

UINT CCmdRule::ExecPoint(UINT event, DBL dx, DBL dy)
{
	return(ENDEVENT);
}

UINT CCmdRule::ExecLine(UINT event, DBL dx, DBL dy)
{
	int		nStyle;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		m_pcDialogRuleStyle = new CDialogRuleStyle(m_pcView);
		if(m_pcDialogRuleStyle->Create(IDD_RULESTYLE, m_pcView)){
			LocateRightFitDialog(m_pcDialogRuleStyle);
		}
		m_nMouseSetCount = 1;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 1){
			SaveUndo();
			CreateLine(dx, dy);
			if(m_pcDataList == NULL){
				break;
			}
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 2;
		}
		else if(m_nMouseSetCount == 2){
			m_pcDialogRuleStyle->GetItemElement();
			nStyle = m_pcDialogRuleStyle->m_nStyle;
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			SetLineEndPoint(dx, dy);
			SetStyle(nStyle);
			m_pcDataList->RenewMiniMax();
			DrawModeSET();
			m_pcDataList->OnDraw(m_pcDC);
			EndDraw();
			AddLinkDoc(m_pcDataList);
			m_nMouseSetCount = 1;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 2){
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			SetLineEndPoint(dx, dy);
			m_pcDataList->AtntionDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case DLGRESETEVENT:
	case RESETEVENT:
	case ENDEVENT:
		if(m_nMouseSetCount == 2){
			delete(m_pcDataList);
		}
		if(m_nMouseSetCount != 0){
			m_pcDialogRuleStyle->DestroyWindow();
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdRule::ExecBoxs(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			SaveUndo();
			CreateBoxs(dx, dy);
			if(m_pcDataList == NULL){
				break;
			}
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			SetBoxsEndPoint(dx, dy);
			m_pcDataList->RenewMiniMax();
			DrawModeSET();
			m_pcDataList->OnDraw(m_pcDC);
			EndDraw();
			AddLinkDoc(m_pcDataList);
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			SetBoxsEndPoint(dx, dy);
			m_pcDataList->AtntionDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			delete(m_pcDataList);
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);

}

UINT CCmdRule::ExecCrcl(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			SaveUndo();
			CreateCrcl(dx, dy);
			if(m_pcDataList == NULL){
				break;
			}
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			SetCrclEndPoint(dx, dy);
			m_pcDataList->RenewMiniMax();
			DrawModeSET();
			m_pcDataList->OnDraw(m_pcDC);
			EndDraw();
			AddLinkDoc(m_pcDataList);
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			SetCrclEndPoint(dx, dy);
			m_pcDataList->AtntionDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			delete(m_pcDataList);
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);

}

UINT CCmdRule::ExecPlgn(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			SaveUndo();
			GetToolBarElement();
			CreatePlgn(dx, dy);
			if(m_pcDataList == NULL){
				break;
			}
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			GetToolBarElement();
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			SetPlgnEndPoint(dx, dy);
			m_pcDataList->RenewMiniMax();
			DrawModeSET();
			m_pcDataList->OnDraw(m_pcDC);
			EndDraw();
			AddLinkDoc(m_pcDataList);
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			GetToolBarElement();
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			SetPlgnEndPoint(dx, dy);
			m_pcDataList->AtntionDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			delete(m_pcDataList);
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);

}

UINT CCmdRule::ExecFree(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			SaveUndo();
			CreateFree(dx, dy);
			if(m_pcDataList == NULL){
				break;
			}
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else{
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			SetFreePoint(m_nMouseSetCount, dx, dy);
			m_pcDataList->AtntionDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount++;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount >= 1){
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			SetFreePoint(m_nMouseSetCount, dx, dy);
			m_pcDataList->AtntionDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount >= 1){
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			SetFreeEndPoint(m_nMouseSetCount);
			m_pcDataList->RenewMiniMax();
			DrawModeSET();
			m_pcDataList->OnDraw(m_pcDC);
			EndDraw();
			AddLinkDoc(m_pcDataList);
			return(ENDEVENT);
		}
		break;
	case ENDEVENT:
		if(m_nMouseSetCount >= 1){
			delete(m_pcDataList);
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);

}

void CCmdRule::CreateLine(DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CRuleVect*		pcRuleVect;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(m_pcDataList == NULL){
		return;
	}
	pcRuleVect = new CRuleVect(RULEVECTID);
	m_pcDataList->SetData((CData*)pcRuleVect);
	pcRuleVect->OffFlag(REJIONFLAG);
	pcRuleVect->OnFlag(ATNTION1FLAG);
	pcRuleVect->SetPointCount(2);
	pcRuleVect->SetAtrDXY(0, 0, dx, dy);
	pcRuleVect->SetAtrDXY(1, 0, dx, dy);
}

void CCmdRule::SetLineEndPoint(DBL dx, DBL dy)
{
	CRuleVect*		pcRuleVect;
	DBL		sx, sy;

	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	pcRuleVect = (CRuleVect*)(m_pcDataList->GetData(0));
	if(m_pcView->m_nKeyFlags & MK_SHIFT){
		pcRuleVect->GetDXY(0, &sx, &sy);
		if(fabs(sx-dx) < fabs(sy-dy)){
			dx = sx;
		}
		else{
			dy = sy;
		}
	}
	pcRuleVect->SetDXY(1, dx, dy);
}

void CCmdRule::SetStyle(int nStyle)
{
	CRuleVect*		pcRuleVect;

	pcRuleVect = (CRuleVect*)(m_pcDataList->GetData(0));
	pcRuleVect->SetStyle(nStyle);
}

void CCmdRule::CreateBoxs(DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CRuleVect*		pcRuleVect;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(m_pcDataList == NULL){
		return;
	}
	pcRuleVect = new CRuleVect(RULEVECTID);
	m_pcDataList->SetData((CData*)pcRuleVect);
	pcRuleVect->OnFlag(ATNTION1FLAG);
	m_dSX = dx; m_dSY = dy;
	m_dEX = dx; m_dEY = dy;
	pcRuleVect->SetPointCount(4);
	SetBoxsPoint(pcRuleVect);
}

void CCmdRule::SetBoxsEndPoint(DBL dx, DBL dy)
{
	CRuleVect*		pcRuleVect;

	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	pcRuleVect = (CRuleVect*)(m_pcDataList->GetData(0));
	if(m_pcView->m_nKeyFlags & MK_SHIFT){
		if(fabs(m_dSX-dx) < fabs(m_dSY-dy)){
			dy = m_dSY - (dx - m_dSX);
		}
		else{
			dx = m_dSX - (dy - m_dSY);
		}
	}
	m_dEX = dx; m_dEY = dy;
	SetBoxsPoint(pcRuleVect);
}

void CCmdRule::SetBoxsPoint(CRuleVect* pcRuleVect)
{
	pcRuleVect->SetAtrDXY(0, 0, m_dSX, m_dSY);
	pcRuleVect->SetAtrDXY(1, 0, m_dEX, m_dSY);
	pcRuleVect->SetAtrDXY(2, 0, m_dEX, m_dEY);
	pcRuleVect->SetAtrDXY(3, 0, m_dSX, m_dEY);
}

void CCmdRule::CreateCrcl(DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CRuleVect*		pcRuleVect;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(m_pcDataList == NULL){
		return;
	}
	pcRuleVect = new CRuleVect(RULEVECTID);
	m_pcDataList->SetData((CData*)pcRuleVect);
	pcRuleVect->OnFlag(ATNTION1FLAG);
	m_dSX = dx; m_dSY = dy;
	m_dEX = dx; m_dEY = dy;
	pcRuleVect->SetPointCount(12);
	SetCrclPoint(pcRuleVect);
}

void CCmdRule::SetCrclEndPoint(DBL dx, DBL dy)
{
	CRuleVect*	pcRuleVect;

	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	pcRuleVect = (CRuleVect*)(m_pcDataList->GetData(0));
	if(m_pcView->m_nKeyFlags & MK_SHIFT){
		if(fabs(m_dSX-dx) < fabs(m_dSY-dy)){
			dy = m_dSY - (dx - m_dSX);
		}
		else{
			dx = m_dSX - (dy - m_dSY);
		}
	}
	m_dEX = dx; m_dEY = dy;
	SetCrclPoint(pcRuleVect);
}

void CCmdRule::SetCrclPoint(CRuleVect* pcRuleVect)
{
	DBL		tx, ty;
	DBL		a, b;
	DBL		ta, tb;

	a = (m_dEX - m_dSX) / 2.0;
	b = (m_dEY - m_dSY) / 2.0;
	ta = (a * 1.41421356 * 1.99) / 5.0;
	tb = (b * 1.41421356 * 1.99) / 5.0;
	tx = (m_dEX + m_dSX) / 2.0;
	ty = (m_dEY + m_dSY) / 2.0;
	pcRuleVect->SetAtrDXY(  0, BEZEATR|CURVATR, tx,    m_dSY);
	pcRuleVect->SetAtrDXY(  1, BEZEATR,         tx-ta, m_dSY);
	pcRuleVect->SetAtrDXY(  2, BEZEATR,         m_dSX, ty-tb);
	pcRuleVect->SetAtrDXY(  3, BEZEATR|CURVATR, m_dSX, ty);
	pcRuleVect->SetAtrDXY(  4, BEZEATR,         m_dSX, ty+tb);
	pcRuleVect->SetAtrDXY(  5, BEZEATR,         tx-ta, m_dEY);
	pcRuleVect->SetAtrDXY(  6, BEZEATR|CURVATR, tx,    m_dEY);
	pcRuleVect->SetAtrDXY(  7, BEZEATR,         tx+ta, m_dEY);
	pcRuleVect->SetAtrDXY(  8, BEZEATR,         m_dEX, ty+tb);
	pcRuleVect->SetAtrDXY(  9, BEZEATR|CURVATR, m_dEX, ty);
	pcRuleVect->SetAtrDXY( 10, BEZEATR,         m_dEX, ty-tb);
	pcRuleVect->SetAtrDXY( 11, BEZEATR,         tx+ta, m_dSY);
}

void CCmdRule::CreatePlgn(DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CRuleVect*		pcRuleVect;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(m_pcDataList == NULL){
		return;
	}
	pcRuleVect = new CRuleVect(RULEVECTID);
	m_pcDataList->SetData((CData*)pcRuleVect);
	pcRuleVect->OnFlag(ATNTION1FLAG);
	m_dSX = dx; m_dSY = dy;
	m_dEX = dx; m_dEY = dy;
	SetPlgnPoint(pcRuleVect);
}

void CCmdRule::SetPlgnEndPoint(DBL dx, DBL dy)
{
	CRuleVect*		pcRuleVect;

	m_dEX = dx; m_dEY = dy;
	pcRuleVect = (CRuleVect*)(m_pcDataList->GetData(0));
	SetPlgnPoint(pcRuleVect);
}

void CCmdRule::SetPlgnPoint(CRuleVect* pcRuleVect)
{
	DBL		dr;
	DBL		dsrag;
	DBL		qpai;
	int		pp;
	DBL		dx, dy;
	int		cntrkind;
	int		plgncount;

	figuenvgetcntrkind(&cntrkind);
	figuenvgetplgncount(&plgncount);
	sprintf(m_szMsgBuf, "%d %d", cntrkind, plgncount);
	HlpMsgDsp(m_szMsgBuf);

	if(cntrkind == CNTRCNTR){
		m_dCX = m_dSX;		
		m_dCY = m_dSY;		
	}
	else{
		m_dCX = (m_dSX + m_dEX) / 2.0;
		m_dCY = (m_dSY + m_dEY) / 2.0;		
	}
	dr = hypot(m_dEX - m_dCX, m_dEY - m_dCY);
	if(m_dEX == m_dCX && m_dEY == m_dCY){
		dsrag = 0;
	}
	else{
		dsrag = atan2(m_dEY-m_dCY, m_dEX - m_dCX);
	}

	qpai = (M_PI * 2.0) / (DBL)(plgncount);
	pcRuleVect->SetPointCount(plgncount);
	for(pp = 0; pp < plgncount; pp++){
		dx = cos(qpai * (DBL)pp + dsrag) * dr + m_dCX;
		dy = sin(qpai * (DBL)pp + dsrag) * dr + m_dCY;
		pcRuleVect->SetAtrDXY(pp, 0, dx, dy);
	}
}

void CCmdRule::CreateFree(DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CRuleVect*		pcRuleVect;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(m_pcDataList == NULL){
		return;
	}
	pcRuleVect = new CRuleVect(RULEVECTID);
	m_pcDataList->SetData((CData*)pcRuleVect);
	pcRuleVect->OnFlag(ATNTION1FLAG);
	pcRuleVect->SetPointCount(2);
	pcRuleVect->SetAtrDXY(0, 0, dx, dy);
	pcRuleVect->SetAtrDXY(1, 0, dx, dy);
}

void CCmdRule::SetFreePoint(int pp, DBL dx, DBL dy)
{
	CRuleVect*		pcRuleVect;

	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	pcRuleVect = (CRuleVect*)(m_pcDataList->GetData(0));
	pcRuleVect->SetPointCount(pp+2);
	/*
	if(scrncntrlkeycheck() == ON && pp >= 2){
		DBL		x0, y0;
		DBL		x1, y1;
		DBL		rag1, 
		pcRuleVect->GetDXY(pp-2, &x0, &y0);
		pcRuleVect->GetDXY(pp-1, &x1, &y1);
		len = hypot(x1-dx, y1-dy);
		if(x0 != x1 || y0 != y1){
			rag1 = atan2(y1 - y0, x1 - x0);
		}
	}
	*/
	if(m_pcView->m_nKeyFlags & MK_SHIFT){
		DBL		sx, sy;
		pcRuleVect->GetDXY(pp-1, &sx, &sy);
		if(fabs(sx-dx) < fabs(sy-dy)){
			dx = sx;
		}
		else{
			dy = sy;
		}
	}
	pcRuleVect->SetAtrDXY(pp, 0, dx, dy);
	pcRuleVect->SetAtrDXY(pp+1, 0, dx, dy);
}

void CCmdRule::SetFreeEndPoint(int pp)
{
	CRuleVect*		pcRuleVect;

	pcRuleVect = (CRuleVect*)(m_pcDataList->GetData(0));
	pcRuleVect->SetPointCount(pp);
}

