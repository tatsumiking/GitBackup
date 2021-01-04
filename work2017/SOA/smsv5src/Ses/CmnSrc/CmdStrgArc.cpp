
/////////////////////////////////////////////////////////////////////////////
// CCmdStrgArc

#include "stdafx.h"
#include "CmdBase.h"
#include "CmdSpclStrg.h"
#include "CmdStrgArc.h"

#define	FREEARCLIMIT	10.0

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Curvelib.h"
//}

CCmdStrgArc::CCmdStrgArc(CScrollView* pcview)
:CCmdSpclStrg(pcview)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	envfname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	m_dR = 100;
	m_dSRag = 180;
	m_dERag = 0;
	m_bNoround = FALSE;
	m_bEqual = FALSE;
	m_nSetMode = LEFTDOWN;
	senvgetexepath(exepath);
	_stprintf_s(envfname, _T("%s\\defenv\\cmnastrg.env"), exepath);
	_tfopen_s(&fp, envfname, _T("r"));
	if(fp == NULL){
		return;
	}
	baselibCrDelFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%lf"), &m_dR);
	baselibCrDelFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%lf %lf"), &m_dSRag, &m_dERag);
	baselibCrDelFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%d %d %d"), &m_bNoround, &m_bEqual, &m_nSetMode);
	fclose(fp);
}

CCmdStrgArc::~CCmdStrgArc()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	envfname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	senvgetexepath(exepath);
	_stprintf_s(envfname, _T("%s\\defenv\\cmnastrg.env"), exepath);
	_tfopen_s(&fp, envfname, _T("w"));
	if(fp == NULL){
		return;
	}
	_stprintf_s(buf, _T("%4.1lf\n"), m_dR);
	baselibFputs(buf, fp);
	_stprintf_s(buf, _T("%4.1lf %4.1lf\n"), m_dSRag, m_dERag);
	baselibFputs(buf, fp);
	_stprintf_s(buf, _T("%d %d %d\n"), m_bNoround, m_bEqual, m_nSetMode);
	baselibFputs(buf, fp);
	fclose(fp);
}

UINT CCmdStrgArc::ExecStrgSetArc(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;

	switch(event){
	case INITEVENT:
		m_pcDialogStrgArc = new CDialogStrgArc(m_pcView);
		m_pcDialogStrgArc->m_dR = m_dR;
		m_pcDialogStrgArc->m_dSRag = m_dSRag;
		m_pcDialogStrgArc->m_dERag = m_dERag;
		m_pcDialogStrgArc->m_chkNoround = m_bNoround;
		m_pcDialogStrgArc->m_chkStrgEqual = m_bEqual;
		m_pcDialogStrgArc->m_nSetMode = m_nSetMode;
		if(m_pcDialogStrgArc->Create(IDD_STRG_ARC) == IDOK){
			LocateStrgArcDlg();
		}
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		m_nIDary[0] = STRGVECTID;
		m_nIDcnt = 1;
		pcData = SrchIdsData(m_nIDary, m_nIDcnt, dx, dy);
		if(pcData != NULL){ 
			SaveUndo();
			m_pcStrgVect = (CStrgVect*)pcData;
			m_pcStrgVect->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
			TrnsStrgSetArc();
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnDataRedraw(m_pcStrgVect);
			m_nMouseSetCount = 0;
		}
		break;
	case ENDEVENT:
		m_pcDialogStrgArc->DestroyWindow();
		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdStrgArc::ExecStrgFreeArc(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;

	switch(event){
	case INITEVENT:
		m_pcDialogStrgArc = new CDialogStrgArc(m_pcView);
		m_pcDialogStrgArc->m_dR = m_dR;
		m_pcDialogStrgArc->m_dSRag = m_dSRag;
		m_pcDialogStrgArc->m_dERag = m_dERag;
		m_pcDialogStrgArc->m_chkNoround = m_bNoround;
		m_pcDialogStrgArc->m_chkStrgEqual = m_bEqual;
		m_pcDialogStrgArc->m_nSetMode = m_nSetMode;
		if(m_pcDialogStrgArc->Create(IDD_STRG_ARC) == IDOK){
			LocateStrgArcDlg();
		}
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_nIDary[0] = STRGVECTID;
			m_nIDcnt = 1;
			pcData = SrchIdsData(m_nIDary, m_nIDcnt, dx, dy);
			if(pcData != NULL){ 
				SaveUndo();
				m_pcStrgVect = (CStrgVect*)pcData;
				m_pcStrgVect->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
				dx = (m_dBSX+m_dBEX) / 2;
				dy = (m_dBSY+m_dBEY) / 2;
				m_dX1 = dx; m_dY1 = dy;
				m_dX2 = dx; m_dY2 = dy;
				m_dX3 = dx; m_dY3 = dy;
				ArcCntrRagSet();
				CreateStrgArcSetVect();
				BeginDraw();
				m_pcDataList->AtntionDraw(m_pcDC);
				EndDraw();
				m_nMouseSetCount = 1;
			}
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			EndDraw();
			ArcCntrRagSet();
			SetArcPoint();
			TrnsStrgFreeArc();
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnDataRedraw(m_pcStrgVect);
			if(m_pcDataList != NULL){
				delete(m_pcDataList);
			}
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			EndDraw();
			m_dX1 = dx; m_dY1 = dy;
			m_dX3 = m_dX2 - (dx-m_dX2); m_dY3 = dy;
			ArcCntrRagSet();
			SetArcPoint();
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		break;
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcDataList->AtntionDraw(m_pcDC);
			EndDraw();
			if(m_pcDataList != NULL){
				delete(m_pcDataList);
			}
			m_nMouseSetCount = 0;
		}
		m_pcDialogStrgArc->DestroyWindow();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

void CCmdStrgArc::LocateStrgArcDlg()
{
	CTonApp*	theApp;
	CMainFrame*	theWnd;
	CRect	prect;
	CRect	crect;
	int		x, y, cx, cy;

	m_pcView->GetWindowRect(&prect);
	m_pcDialogStrgArc->GetWindowRect(&crect);
	x = prect.right - crect.Width();
	y = prect.top;
	cx = crect.Width();
	cy = crect.Height();

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	m_pcDialogStrgArc->ShowWindow(SW_SHOW);
	::SetWindowPos(m_pcDialogStrgArc->m_hWnd, HWND_TOP, x, y, cx, cy, 0);
	Dispatch();	
}

void CCmdStrgArc::TrnsStrgSetArc()
{
	DBL		minx, miny, maxx, maxy;

	m_pcDialogStrgArc->GetItemElement();
	m_dR = m_pcDialogStrgArc->m_dR;
	m_dSRag = m_pcDialogStrgArc->m_dSRag;
	m_dERag = m_pcDialogStrgArc->m_dERag;
	m_bEqual = m_pcDialogStrgArc->m_chkStrgEqual;
	m_bNoround = m_pcDialogStrgArc->m_chkNoround;
	m_nSetMode = m_pcDialogStrgArc->m_nSetMode;
	m_pcStrgVect->GetDMiniMax(&minx, &miny, &maxx, &maxy);
	m_dCX = (minx + maxx) / 2;
	m_dCY = (miny + maxy) / 2 - m_dR;

	CreateStrgArcSetVect();
	
	StrgLineStructMain();
	if(m_pcDataList != NULL){
		delete(m_pcDataList);
	}
}

void CCmdStrgArc::TrnsStrgFreeArc()
{
	m_pcDialogStrgArc->GetItemElement();
	m_bEqual = m_pcDialogStrgArc->m_chkStrgEqual;
	m_bNoround = m_pcDialogStrgArc->m_chkNoround;
	m_nSetMode = m_pcDialogStrgArc->m_nSetMode;

	StrgLineStructMain();
}

void CCmdStrgArc::CreateStrgArcSetVect()
{
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(m_pcDataList == NULL){
		return;
	}
	m_pcVect = new CVect(BASEVECTID);
	if(m_pcVect == NULL){
		return;
	}
	m_pcDataList->SetData((CData*)m_pcVect);
	m_pcVect->OffFlag(REJIONFLAG);
	SetArcPoint();
}

void CCmdStrgArc::SetArcPoint()
{
	int			i, cnt;
	int			dp;
	DBL			dx, dy;
	DBL			subrag, divrag;
	DBL			rag;

	subrag = m_dERag - m_dSRag;
	cnt = (int)fabs((subrag / FREEARCLIMIT)) + 1;
	divrag = subrag / (DBL)cnt;
	dp = 0;
	for(i = 0; i < cnt; i++){
		rag = (m_dSRag + divrag * (DBL)i) * curvedegree1get();
		dx = cos(rag)*m_dR + m_dCX;
		dy = sin(rag)*m_dR + m_dCY;
		m_pcVect->SetAtrDXY(dp, CURVATR|CONTATR, dx, dy);
		dp++;
	}
	rag = m_dERag * curvedegree1get();
	dx = cos(rag)*m_dR + m_dCX;
	dy = sin(rag)*m_dR + m_dCY;
	m_pcVect->SetAtrDXY(dp, 0, dx, dy);
	dp++;
	m_pcVect->SetAtr(0, CURVATR);
	m_pcVect->SetPointCount(dp);
	m_pcDataList->RenewMiniMax();
}

BOOL CCmdStrgArc::ArcCntrRagSet()
{
	int		stat;
	DBL		x1, y1, x2, y2, x3, y3;
	DBL		x0, y0, dr, rag01, rag03;

	x1 = m_dX1; y1 = m_dY1;
	x2 = m_dX2; y2 = m_dY2;
	x3 = m_dX3; y3 = m_dY3;
	m_dCX = m_dX2;
	m_dCY = m_dY2;
	m_dR = 0;
	m_dSRag = 0;
	m_dERag = 0;

	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat == FAIL){
		return(FALSE);
	}
	stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &rag01, &rag03, &dr);
	if(stat == FAIL){
		return(FALSE);
	}

	m_dCX = x0;
	m_dCY = y0;
	m_dR = dr;
	m_dSRag = rag01 / curvedegree1get();
	m_dERag = rag03 / curvedegree1get();
	m_pcDialogStrgArc->GetItemElement();
	m_pcDialogStrgArc->m_dR = FLOOR100(m_dR);
	m_pcDialogStrgArc->m_dSRag = FLOOR100(m_dSRag);
	m_pcDialogStrgArc->m_dERag = FLOOR100(m_dERag);
	m_pcDialogStrgArc->SetItemElement();
	return(TRUE);
}

