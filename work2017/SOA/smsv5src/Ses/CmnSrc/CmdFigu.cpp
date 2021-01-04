
/////////////////////////////////////////////////////////////////////////////
// CCmdFigu

#include "stdafx.h"
#include "CmdBase.h"
#include "CmdFigu.h"

#include "DialogSizeBoxs.h"
#include "DialogSizeCrcl.h"
#include "DialogSizeList.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Figuenv.h"

CCmdFigu::CCmdFigu(CScrollView* pcview)
	:CCmdBase(pcview)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	TCHAR	buf[RECMAX];
	FILE	*fp;

	m_dCntrX = 50; m_dCntrY = 50;
	m_dWidth = 100; m_dHight = 100;
	m_dXSize = 100; m_dYSize = 100;
	m_nXCount = 5; m_nYCount = 5;
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\defenv\\cmd_figu.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp != NULL){
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf %lf %lf %lf"), &m_dCntrX, &m_dCntrY, &m_dWidth, &m_dHight);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf %lf %d %d"), &m_dXSize, &m_dYSize, &m_nXCount, &m_nYCount);
		fclose(fp);
	}

}

CCmdFigu::~CCmdFigu()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\defenv\\cmd_figu.env"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	if(fp != NULL){
		_stprintf_s(buf, _T("%4.1lf %4.1lf %4.1lf %4.1lf\n"), m_dCntrX, m_dCntrY, m_dWidth, m_dHight);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%4.1lf %4.1lf %d %d\n"), m_dXSize, m_dYSize, m_nXCount, m_nYCount);
		baselibFputs(buf, fp);
		fclose(fp);
	}
}

UINT CCmdFigu::ExecPoint(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case SETEVENT:
		SaveUndo();
		CreatePoint(dx, dy);
		if(m_pcDataList == NULL){
			break;
		}
		AddLinkDoc(m_pcDataList);
		ActiveDataAllOff();
		ScrnDataRedraw(m_pcDataList);
		break;
	case RESETEVENT:
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdFigu::ExecLine(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			SaveUndo();
			CreateLine(dx, dy);
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
			SetLineEndPoint(dx, dy);
			m_pcDataList->RenewMiniMax();
			DrawModeSET();
			m_pcDataList->OnDraw(m_pcDC);
			EndDraw();
			AddLinkDoc(m_pcDataList);
			ActiveDataAllOff();
			ScrnDataRedraw(m_pcDataList);
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			SetLineEndPoint(dx, dy);
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

UINT CCmdFigu::ExecBoxs(UINT event, DBL dx, DBL dy)
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
			EndDraw();
			AddLinkDoc(m_pcDataList);
			ActiveDataAllOff();
			ScrnDataRedraw(m_pcDataList);
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
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

UINT CCmdFigu::ExecCrcl(UINT event, DBL dx, DBL dy)
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
			EndDraw();
			AddLinkDoc(m_pcDataList);
			ActiveDataAllOff();
			ScrnDataRedraw(m_pcDataList);
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
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

UINT CCmdFigu::ExecPlgn(UINT event, DBL dx, DBL dy)
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
			EndDraw();
			AddLinkDoc(m_pcDataList);
			ActiveDataAllOff();
			ScrnDataRedraw(m_pcDataList);
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
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

UINT CCmdFigu::ExecFree(UINT event, DBL dx, DBL dy)
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
			EndDraw();
			AddLinkDoc(m_pcDataList);
			ActiveDataAllOff();
			ScrnDataRedraw(m_pcDataList);
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

UINT CCmdFigu::ExecSizeBoxs(UINT event, DBL dx, DBL dy)
{
	CDialogSizeBoxs *pDialogSizeBoxs;
	DBL		coodxx, coodyy; 

	if(event != INITEVENT){
		return(0);
	}
	coodxx = m_dCntrX;
	coodyy = m_dCntrY;
	ScrnCnvMMToCood(&coodxx, &coodyy);
	pDialogSizeBoxs = new CDialogSizeBoxs(m_pcView);
	pDialogSizeBoxs->m_dCntrX = coodxx;
	pDialogSizeBoxs->m_dCntrY = coodyy;
	pDialogSizeBoxs->m_dWidth = m_dWidth;
	pDialogSizeBoxs->m_dHight = m_dHight;
	if(pDialogSizeBoxs->DoModal() == IDOK){
		SaveUndo();
		coodxx = pDialogSizeBoxs->m_dCntrX;
		coodyy = pDialogSizeBoxs->m_dCntrY;
		m_dWidth = pDialogSizeBoxs->m_dWidth;
		m_dHight = pDialogSizeBoxs->m_dHight;
		ScrnCnvCoodToMM(&coodxx, &coodyy);
		m_dCntrX = coodxx;
		m_dCntrY = coodyy;
		dx = m_dCntrX - m_dWidth / 2;
		dy = m_dCntrY - m_dHight / 2;
		CreateBoxs(dx, dy);	
		dx = m_dCntrX + m_dWidth / 2;
		dy = m_dCntrY + m_dHight / 2;
		SetBoxsEndPoint(dx, dy);	
		m_pcDataList->RenewMiniMax();
		ScrnRedraw();
		ActiveDataAllOff();
		AddLinkDoc(m_pcDataList);
	}
	delete(pDialogSizeBoxs);
	return(ENDEVENT);
}

UINT CCmdFigu::ExecSizeCrcl(UINT event, DBL dx, DBL dy)
{
	CDialogSizeCrcl *pDialogSizeCrcl;
	DBL		coodxx, coodyy;

	if(event != INITEVENT){
		return(0);
	}
	pDialogSizeCrcl = new CDialogSizeCrcl(m_pcView);
	coodxx = m_dCntrX;
	coodyy = m_dCntrY;
	ScrnCnvMMToCood(&coodxx, &coodyy);
	pDialogSizeCrcl->m_dCntrX = coodxx;
	pDialogSizeCrcl->m_dCntrY = coodyy;
	pDialogSizeCrcl->m_dWidth = m_dWidth;
	pDialogSizeCrcl->m_dHight = m_dHight;
	if(pDialogSizeCrcl->DoModal() == IDOK){
		SaveUndo();
		coodxx = pDialogSizeCrcl->m_dCntrX;
		coodyy = pDialogSizeCrcl->m_dCntrY;
		m_dWidth = pDialogSizeCrcl->m_dWidth;
		m_dHight = pDialogSizeCrcl->m_dHight;
		ScrnCnvCoodToMM(&coodxx, &coodyy);
		m_dCntrX = coodxx;
		m_dCntrY = coodyy;
		dx = m_dCntrX - m_dWidth / 2;
		dy = m_dCntrY - m_dHight / 2;
		CreateCrcl(dx, dy);	
		dx = m_dCntrX + m_dWidth / 2;
		dy = m_dCntrY + m_dHight / 2;
		SetCrclEndPoint(dx, dy);	
		m_pcDataList->RenewMiniMax();
		ScrnRedraw();
		ActiveDataAllOff();
		AddLinkDoc(m_pcDataList);
	}
	delete(pDialogSizeCrcl);
	return(ENDEVENT);
}

UINT CCmdFigu::ExecSizeList(UINT event, DBL dx, DBL dy)
{
	CDialogSizeList *pDialogSizeList;

	if(event != INITEVENT){
		return(0);
	}
	pDialogSizeList = new CDialogSizeList(m_pcView);
	pDialogSizeList->m_dXSize = m_dXSize;
	pDialogSizeList->m_dYSize = m_dYSize;
	pDialogSizeList->m_nXCount = m_nXCount;
	pDialogSizeList->m_nYCount = m_nYCount;
	if(pDialogSizeList->DoModal() == IDOK){
		SaveUndo();
		m_dXSize = pDialogSizeList->m_dXSize;
		m_dYSize = pDialogSizeList->m_dYSize;
		m_nXCount = pDialogSizeList->m_nXCount;
		m_nYCount = pDialogSizeList->m_nYCount;
		CreateList(m_dXSize, m_dYSize, m_nXCount, m_nYCount);
		m_pcDataList->RenewMiniMax();
		ScrnRedraw();
		ActiveDataAllOff();
		AddLinkDoc(m_pcDataList);
	}
	delete(pDialogSizeList);
	return(ENDEVENT);
}

void CCmdFigu::CreatePoint(DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CVect*		pcVect;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(m_pcDataList == NULL){
		return;
	}
	pcVect = new CVect(BASEVECTID);
	m_pcDataList->SetData((CData*)pcVect);
	pcVect->OnFlag(ATNTION1FLAG);
	pcVect->OnFlag(REJIONFLAG);
	pcVect->SetPointCount(1);
	pcVect->SetAtrDXY(0, 0, dx, dy);
	pcVect->RenewMiniMax();
}

void CCmdFigu::CreateLine(DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CVect*		pcVect;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(m_pcDataList == NULL){
		return;
	}
	pcVect = new CVect(BASEVECTID);
	m_pcDataList->SetData((CData*)pcVect);
	pcVect->OnFlag(ATNTION1FLAG);
	pcVect->OffFlag(REJIONFLAG);
	pcVect->SetPointCount(2);
	pcVect->SetAtrDXY(0, 0, dx, dy);
	pcVect->SetAtrDXY(1, 0, dx, dy);
}

void CCmdFigu::SetLineEndPoint(DBL dx, DBL dy)
{
	CTonApp*	theApp;
	CSysEnv*	pcSysEnv;
	CVect*		pcVect;

	pcVect = (CVect*)(m_pcDataList->GetData(0));
	theApp = (CTonApp*)AfxGetApp();
	pcSysEnv = theApp->m_pcSysEnv;
	if(pcSysEnv->ScrnCheckShiftKey() == ON){
		DBL		sx, sy;
		pcVect->GetDXY(0, &sx, &sy);
		if(fabs(sx-dx) < fabs(sy-dy)){
			dx = sx;
		}
		else{
			dy = sy;
		}
	}
	pcVect->SetDXY(1, dx, dy);
}

void CCmdFigu::CreateBoxs(DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CVect*		pcVect;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(m_pcDataList == NULL){
		return;
	}
	pcVect = new CVect(BASEVECTID);
	m_pcDataList->SetData((CData*)pcVect);
	pcVect->OnFlag(ATNTION1FLAG);
	m_dSX = dx; m_dSY = dy;
	m_dEX = dx; m_dEY = dy;
	pcVect->SetPointCount(4);
	SetBoxsPoint(pcVect);
}

void CCmdFigu::SetBoxsEndPoint(DBL dx, DBL dy)
{
	CVect*		pcVect;

	pcVect = (CVect*)(m_pcDataList->GetData(0));
	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	if(pcSysEnv->ScrnCheckShiftKey() == ON){
		if(fabs(m_dSX-dx) < fabs(m_dSY-dy)){
			dy = m_dSY - (dx - m_dSX);
		}
		else{
			dx = m_dSX - (dy - m_dSY);
		}
	}
	m_dEX = dx; m_dEY = dy;
	SetBoxsPoint(pcVect);
}

void CCmdFigu::SetBoxsPoint(CVect* pcVect)
{
	pcVect->SetAtrDXY(0, 0, m_dSX, m_dSY);
	pcVect->SetAtrDXY(1, 0, m_dEX, m_dSY);
	pcVect->SetAtrDXY(2, 0, m_dEX, m_dEY);
	pcVect->SetAtrDXY(3, 0, m_dSX, m_dEY);
}

void CCmdFigu::CreateCrcl(DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CVect*		pcVect;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(m_pcDataList == NULL){
		return;
	}
	pcVect = new CVect(BASEVECTID);
	m_pcDataList->SetData((CData*)pcVect);
	pcVect->OnFlag(ATNTION1FLAG);
	m_dSX = dx; m_dSY = dy;
	m_dEX = dx; m_dEY = dy;
	pcVect->SetPointCount(12);
	SetCrclPoint(pcVect);
}

void CCmdFigu::SetCrclEndPoint(DBL dx, DBL dy)
{
	CVect*	pcVect;

	pcVect = (CVect*)(m_pcDataList->GetData(0));
	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	if(pcSysEnv->ScrnCheckShiftKey() == ON){
		if(fabs(m_dSX-dx) < fabs(m_dSY-dy)){
			dy = m_dSY - (dx - m_dSX);
		}
		else{
			dx = m_dSX - (dy - m_dSY);
		}
	}
	m_dEX = dx; m_dEY = dy;
	SetCrclPoint(pcVect);
}

void CCmdFigu::SetCrclPoint(CVect* pcVect)
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
	pcVect->SetAtrDXY(  0, BEZEATR|CURVATR, tx,    m_dSY);
	pcVect->SetAtrDXY(  1, BEZEATR,         tx+ta, m_dSY);
	pcVect->SetAtrDXY(  2, BEZEATR,         m_dEX, ty-tb);
	pcVect->SetAtrDXY(  3, BEZEATR|CURVATR, m_dEX, ty);
	pcVect->SetAtrDXY(  4, BEZEATR,         m_dEX, ty+tb);
	pcVect->SetAtrDXY(  5, BEZEATR,         tx+ta, m_dEY);
	pcVect->SetAtrDXY(  6, BEZEATR|CURVATR, tx,    m_dEY);
	pcVect->SetAtrDXY(  7, BEZEATR,         tx-ta, m_dEY);
	pcVect->SetAtrDXY(  8, BEZEATR,         m_dSX, ty+tb);
	pcVect->SetAtrDXY(  9, BEZEATR|CURVATR, m_dSX, ty);
	pcVect->SetAtrDXY( 10, BEZEATR,         m_dSX, ty-tb);
	pcVect->SetAtrDXY( 11, BEZEATR,         tx-ta, m_dSY);
}

void CCmdFigu::CreatePlgn(DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CVect*		pcVect;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(m_pcDataList == NULL){
		return;
	}
	pcVect = new CVect(BASEVECTID);
	m_pcDataList->SetData((CData*)pcVect);
	pcVect->OnFlag(ATNTION1FLAG);
	m_dSX = dx; m_dSY = dy;
	m_dEX = dx; m_dEY = dy;
	SetPlgnPoint(pcVect);
}

void CCmdFigu::SetPlgnEndPoint(DBL dx, DBL dy)
{
	CVect*		pcVect;

	pcVect = (CVect*)(m_pcDataList->GetData(0));
	m_dEX = dx; m_dEY = dy;
	SetPlgnPoint(pcVect);
}

void CCmdFigu::SetPlgnPoint(CVect* pcVect)
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
	_stprintf_s(m_szMsgBuf, _T("%d %d"), cntrkind, plgncount);
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

	qpai = -(M_PI * 2.0) / (DBL)(plgncount);
	pcVect->SetPointCount(plgncount);
	for(pp = 0; pp < plgncount; pp++){
		dx = cos(qpai * (DBL)pp + dsrag) * dr + m_dCX;
		dy = sin(qpai * (DBL)pp + dsrag) * dr + m_dCY;
		pcVect->SetAtrDXY(pp, 0, dx, dy);
	}
}

void CCmdFigu::CreateFree(DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CVect*		pcVect;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(m_pcDataList == NULL){
		return;
	}
	pcVect = new CVect(BASEVECTID);
	m_pcDataList->SetData((CData*)pcVect);
	pcVect->OffFlag(REJIONFLAG);
	pcVect->OnFlag(ATNTION1FLAG);
	pcVect->SetPointCount(2);
	pcVect->SetAtrDXY(0, 0, dx, dy);
	pcVect->SetAtrDXY(1, 0, dx, dy);
}

void CCmdFigu::SetFreePoint(int pp, DBL dx, DBL dy)
{
	CVect*		pcVect;

	pcVect = (CVect*)(m_pcDataList->GetData(0));
	pcVect->SetPointCount(pp+2);
	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	if(pcSysEnv->ScrnCheckShiftKey() == ON){
		DBL		sx, sy;
		pcVect->GetDXY(pp-1, &sx, &sy);
		if(fabs(sx-dx) < fabs(sy-dy)){
			dx = sx;
		}
		else{
			dy = sy;
		}
	}
	pcVect->SetAtrDXY(pp, 0, dx, dy);
	pcVect->SetAtrDXY(pp+1, 0, dx, dy);
}

void CCmdFigu::SetFreeEndPoint(int pp)
{
	CVect*	pcVect;
	DBL		x0, y0;
	DBL		x1, y1;
	DBL		len;

	pcVect = (CVect*)(m_pcDataList->GetData(0));
	pcVect->GetDXY(0, &x0, &y0);
	pcVect->GetDXY(pp-1, &x1, &y1);
	scrndxymmtorltv(&x0, &y0);
	scrndxymmtorltv(&x1, &y1);
	len = hypot(x1-x0, y1-y0);
	if(len < SNEARLIMIT){
		pcVect->OnFlag(REJIONFLAG);
		pcVect->SetPointCount(pp-1);
	}
	else{
		pcVect->OffFlag(REJIONFLAG);
		pcVect->SetPointCount(pp);
	}
}

void CCmdFigu::CreateList(DBL xsize, DBL ysize, int xcount, int ycount)
{
	CTonDoc*	pcDoc;
	CVect*		pcVect;
	int			x, y;
	DBL			sx, sy, ex, ey;
	DBL			tx, ty;
	DBL			xdiv, ydiv;

	sx = 0; sy = 0;
	ex = xsize; ey = ysize;
	ScrnCnvCoodToMM(&sx, &sy);
	ScrnCnvCoodToMM(&ex, &ey);
	xdiv = (ex - sx) / xcount;
	ydiv = (ey - sy) / ycount;
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(m_pcDataList == NULL){
		return;
	}
	pcVect = new CVect(BASEVECTID);
	m_pcDataList->SetData((CData*)pcVect);
	pcVect->OnFlag(ATNTION1FLAG);
	pcVect->OnFlag(REJIONFLAG);
	pcVect->SetPointCount(4);
	pcVect->SetAtrDXY(0, 0, sx, sy);
	pcVect->SetAtrDXY(1, 0, ex, sy);
	pcVect->SetAtrDXY(2, 0, ex, ey);
	pcVect->SetAtrDXY(3, 0, sx, ey);
	tx = sx;
	for(x = 1; x < xcount; x++){
		tx += xdiv;
		pcVect = new CVect(BASEVECTID);
		m_pcDataList->SetData((CData*)pcVect);
		pcVect->OnFlag(ATNTION1FLAG);
		pcVect->OffFlag(REJIONFLAG);
		pcVect->SetPointCount(2);
		pcVect->SetAtrDXY(0, 0, tx, sy);
		pcVect->SetAtrDXY(1, 0, tx, ey);
	}
	ty = sy;
	for(y = 1; y < ycount; y++){
		ty += ydiv;
		pcVect = new CVect(BASEVECTID);
		m_pcDataList->SetData((CData*)pcVect);
		pcVect->OnFlag(ATNTION1FLAG);
		pcVect->OffFlag(REJIONFLAG);
		pcVect->SetPointCount(2);
		pcVect->SetAtrDXY(0, 0, sx, ty);
		pcVect->SetAtrDXY(1, 0, ex, ty);
	}
}

