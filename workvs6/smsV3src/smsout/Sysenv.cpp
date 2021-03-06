// sysenv.cpp : アプリケーション用環境定義
//

#include "stdafx.h"

#include "sysenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
	void scrndxymmtorltv(LPDBL x, LPDBL y);
	void scrnsetslctpen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
}

CSysEnv::CSysEnv()
{
	int		i;
	DBL		dx, dy;

	m_dCntrX = 0;
	m_dCntrY = 0;
	m_dSubX = 0;
	m_dSubY = 0;
	m_dSubRag = 0;
	m_nSubFlag = OFF;
	m_dGrid = 0;
	m_nGridFlag = OFF;
	m_nCntrlKey = OFF;
	m_nShiftKey = OFF;
	for(i = 0; i < CHKPOINTMAX; i++){
		dx = 0;
		dy = 0;
		ScrnSetCheckPoint(i, dx, dy);
	}

	m_dWidth = NOSETMM;
	m_dHight = NOSETMM;
	m_dSX = NOSETMM;
	m_dSY = NOSETMM;
	m_szStrg[0] = '\0';
	m_szFont[0] = '\0';
	m_szFildName[0] = '\0';
	m_nTateYoko = 0;
	m_dFlatPar = MJFREEFLATPAR;
	m_dSpaceMM = 0;
	m_dKanaPar = 100;
	m_nOderKind = ODEREQUAL;
	m_nSpaceKind = SPACEWAKU;
	m_dCharCount = 0;
	m_nYokoGumi = OFF;
	m_nFixFlag = OFF;
	m_nMirror = NOMIRROR;
	m_nShadowKind = 0;
	m_clrShadow = RGB(64,64,64);
	m_dShadowDistance = 5.0;
	m_dShadowRoundRag = -45;
}

void CSysEnv::ScrnCnvCoodToMM(LPDBL mmx, LPDBL mmy)
{
	DBL		mmxsize, mmysize;

	scrngetmmsize(&mmxsize, &mmysize);
	*mmx = (*mmx) + m_dCntrX;
	*mmy = (*mmy) + m_dCntrY;
}

void CSysEnv::ScrnCnvMMToCood(LPDBL coodx, LPDBL coody)
{
	DBL		mmxsize, mmysize;

	scrngetmmsize(&mmxsize, &mmysize);
	*coodx = (*coodx) - m_dCntrX;
	*coody = (*coody) - m_dCntrY;
}

void CSysEnv::ScrnGetCoodMoveXY(LPDBL movex, LPDBL movey)
{
	//*movey = -(*movey);
}

void CSysEnv::ScrnGetCntrXY(LPDBL cntrx, LPDBL cntry)
{
	*cntrx = m_dCntrX;
	*cntry = m_dCntrY;
}

void CSysEnv::ScrnSetCntrXY(DBL cntrx, DBL cntry)
{
	m_dCntrX = cntrx;
	m_dCntrY = cntry;
}

int CSysEnv::ScrnGetSubXY(LPDBL subx, LPDBL suby)
{
	*subx = m_dSubX;
	*suby = m_dSubY;
	return(m_nSubFlag);
}

void CSysEnv::ScrnSetSubXY(DBL subx, DBL suby)
{
	m_dSubX = subx;
	m_dSubY = suby;
	m_nSubFlag = ON;
}

void CSysEnv::ScrnGetSubRag(LPDBL subrag)
{
	*subrag = m_dSubRag;
}

void CSysEnv::ScrnSetSubRag(DBL subrag)
{
	m_dSubRag = subrag;
	m_nSubFlag = ON;
}

int	CSysEnv::ScrnOffSubFlag()
{
	int		flag;

	flag = m_nSubFlag;
	m_nSubFlag = OFF;
	return(flag);
}

void CSysEnv::ScrnGetGrid(LPDBL grid)
{
	*grid = m_dGrid;
}

void CSysEnv::ScrnSetGrid(DBL grid)
{
	m_dGrid = grid;
	m_nGridFlag = ON;
}

void CSysEnv::ScrnAdjustPoint(LPDBL retdx, LPDBL retdy)
{
	DBL		dx, dy;
	int		pp;
	CData	*pcData;

	dx = *retdx;
	dy = *retdy;
	CTonApp *pcApp= (CTonApp*)AfxGetApp();
	CMainFrame *pcWnd = (CMainFrame*)pcApp->GetMainWnd();
	CTonView *pcView = pcWnd->m_pcCrtView; 
	CTonDoc *pcDoc = pcView->GetDocument();
	pcData = pcDoc->SrchDataPoint(dx, dy, &pp);
	if(pcData != NULL){
		((CVect*)pcData)->GetDXY(pp, &dx, &dy);
		*retdx = dx;
		*retdy = dy;
	}
}

void CSysEnv::ScrnAdjustGrid(LPDBL retdx, LPDBL retdy)
{
	DBL		dx, dy;
	long	cntx, cnty;

	dx = (*retdx);
	dy = (*retdy);
	dx = dx - m_dCntrX;
	dy = dy - m_dCntrY;
	cntx = (int)(dx / m_dGrid + 0.5);
	cnty = (int)(dy / m_dGrid + 0.5);
	dx = (DBL)cntx * m_dGrid + m_dCntrX;
	dy = (DBL)cnty * m_dGrid + m_dCntrY;
	(*retdx) = dx;
	(*retdy) = dy;
}

int CSysEnv::ScrnCheckCntrlKey()
{
	return(m_nCntrlKey);
}

void CSysEnv::ScrnSetCntrlKey(int flag)
{
	m_nCntrlKey = flag;
}

int CSysEnv::ScrnCheckShiftKey()
{
	return(m_nShiftKey);
}

void CSysEnv::ScrnSetShiftKey(int flag)
{
	m_nShiftKey = flag;
}

int CSysEnv::ScrnCheckGrid()
{
	return(m_nGridFlag);
}

void CSysEnv::ScrnOnOffGrid(int flag)
{
	m_nGridFlag = flag;
}

void CSysEnv::ScrnAllDrawCheckPoint(CDC *pcDC)
{
	int		i;

	pcDC->SetROP2(R2_XORPEN);
	scrnsetslctpen(pcDC->m_hDC);
	for(i = 0; i < CHKPOINTMAX; i++){
		ScrnDrawCheckPoint(pcDC, i);
	}
	scrnpenend(pcDC->m_hDC);
	pcDC->SetROP2(R2_COPYPEN);
}

void CSysEnv::ScrnDrawCheckPoint(CDC *pcDC, int idx)
{
	DBL		dx, dy;
	int		x, y;

	dx = m_dChkX[idx];
	dy = m_dChkY[idx];
	scrndxymmtorltv(&dx, &dy);
	x = (int)dx; y = (int)dy-8;
	pcDC->MoveTo(x, y);
	x = (int)dx; y = (int)dy+8;
	pcDC->LineTo(x, y);
	x = (int)dx-8; y = (int)dy;
	pcDC->MoveTo(x, y);
	x = (int)dx+8; y = (int)dy;
	pcDC->LineTo(x, y);
}

void CSysEnv::ScrnSetCheckPoint(int idx, DBL dx, DBL dy)
{
	m_dChkX[idx] = dx;
	m_dChkY[idx] = dy;
}

void CSysEnv::ScrnGetCheckPoint(int idx, LPDBL dx, LPDBL dy)
{
	*dx = m_dChkX[idx];
	*dy = m_dChkY[idx];
}

int CSysEnv::ScrnIncCheckPoint(int idx)
{
	idx++;
	if(CHKPOINTMAX <= idx){
		idx = 0;
	}
	return(idx);
}

