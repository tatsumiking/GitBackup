
/////////////////////////////////////////////////////////////////////////////
// CCmdMsur

#include "stdafx.h"
#include "math.h"

#include "cmd_base.h"
#include "cmd_msur.h"
#include "dlgmsatr.h"

extern "C" {
	void senvgetexepath(LPSTR path);
}

CCmdMsur::CCmdMsur(CScrollView* pcview)
:CCmdBase(pcview)
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	char	buf[HRECMAX];
	FILE	*fp;

	m_nFlotCount = 1;
	strcpy(m_szUnit, "mm");
	m_dFontSize = 5.0;
	senvgetexepath(exepath);
	sprintf(fname, "%s\\defenv\\cmd_msur.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%d", &m_nFlotCount);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%s", &m_szUnit);
		CrDelFgets(buf, HRECMAX, fp);
		sscanf(buf, "%lf", &m_dFontSize);
		fclose(fp);
	}
	if(m_dFontSize <= 0){
		m_dFontSize = 5.0;
	}
}

CCmdMsur::~CCmdMsur()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\defenv\\cmd_msur.env", exepath);
	if((fp = fopen(fname, "w")) != NULL){
		fprintf(fp, "%d\n", m_nFlotCount);
		fprintf(fp, "%s\n", m_szUnit);
		fprintf(fp, "%lf\n", m_dFontSize);
		fclose(fp);
	}
}

UINT CCmdMsur::ExecXCreate(UINT event, DBL dx, DBL dy)
{
	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			/*
			if(pcSysEnv->ScrnCheckShiftKey() == ON){
				pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
				if(pcVect != NULL){
					pcVect->GetDXY(retpp, &dx, &dy);
				}
			}
			*/
			SaveUndo();
			m_pcMsurVect = new CMsurVect(MSURVECTID);
			m_pcMsurVect->SetFontSize(m_dFontSize);
			m_pcMsurVect->SetType(XMSURTYPE);
			m_pcMsurVect->SetDXY(0, dx, dy);
			m_pcMsurVect->SetDXY(1, dx, dy);
			m_pcMsurVect->SetDXY(2, dx, dy);
			BeginDraw();
			m_pcMsurVect->AtntionDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcMsurVect->AtntionDraw(m_pcDC);
			m_pcMsurVect->SetDXY(1, dx, dy);
			m_pcMsurVect->SetDXY(2, dx, dy);
			m_pcMsurVect->AtntionDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 2;
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			m_pcMsurVect->AtntionDraw(m_pcDC);
			MsurXAtrSet();			
			m_pcMsurVect->SetDXY(2, dx, dy);
			DrawModeSET();
			m_pcMsurVect->OnDraw(m_pcDC);
			EndDraw();
			m_pcMsurVect->RenewMiniMax();
			AddLinkDoc((CData*)m_pcMsurVect);
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcMsurVect->AtntionDraw(m_pcDC);
			m_pcMsurVect->SetDXY(1, dx, dy);
			m_pcMsurVect->SetDXY(2, dx, dy);
			m_pcMsurVect->AtntionDraw(m_pcDC);
			EndDraw();
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			m_pcMsurVect->AtntionDraw(m_pcDC);
			m_pcMsurVect->SetDXY(2, dx, dy);
			m_pcMsurVect->AtntionDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcMsurVect->AtntionDraw(m_pcDC);
			EndDraw();
			delete(m_pcMsurVect);
			m_nMouseSetCount = 0;
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			m_pcMsurVect->AtntionDraw(m_pcDC);
			m_pcMsurVect->SetDXY(1, dx, dy);
			m_pcMsurVect->SetDXY(2, dx, dy);
			m_pcMsurVect->AtntionDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		break;
	case ENDEVENT:
		if(m_nMouseSetCount != 0){
			BeginDraw();
			m_pcMsurVect->AtntionDraw(m_pcDC);
			EndDraw();
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdMsur::ExecYCreate(UINT event, DBL dx, DBL dy)
{
	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			SaveUndo();
			m_pcMsurVect = new CMsurVect(MSURVECTID);
			m_pcMsurVect->SetFontSize(m_dFontSize);
			m_pcMsurVect->SetType(YMSURTYPE);
			m_pcMsurVect->SetDXY(0, dx, dy);
			m_pcMsurVect->SetDXY(1, dx, dy);
			m_pcMsurVect->SetDXY(2, dx, dy);
			BeginDraw();
			m_pcMsurVect->AtntionDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcMsurVect->AtntionDraw(m_pcDC);
			m_pcMsurVect->SetDXY(1, dx, dy);
			m_pcMsurVect->SetDXY(2, dx, dy);
			m_pcMsurVect->AtntionDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 2;
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			m_pcMsurVect->AtntionDraw(m_pcDC);
			MsurYAtrSet();			
			m_pcMsurVect->SetDXY(2, dx, dy);
			DrawModeSET();
			m_pcMsurVect->OnDraw(m_pcDC);
			EndDraw();
			m_pcMsurVect->RenewMiniMax();
			AddLinkDoc((CData*)m_pcMsurVect);
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcMsurVect->AtntionDraw(m_pcDC);
			m_pcMsurVect->SetDXY(1, dx, dy);
			m_pcMsurVect->SetDXY(2, dx, dy);
			m_pcMsurVect->AtntionDraw(m_pcDC);
			EndDraw();
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			m_pcMsurVect->AtntionDraw(m_pcDC);
			m_pcMsurVect->SetDXY(2, dx, dy);
			m_pcMsurVect->AtntionDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcMsurVect->AtntionDraw(m_pcDC);
			EndDraw();
			delete(m_pcMsurVect);
			m_nMouseSetCount = 0;
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			m_pcMsurVect->AtntionDraw(m_pcDC);
			m_pcMsurVect->SetDXY(1, dx, dy);
			m_pcMsurVect->SetDXY(2, dx, dy);
			m_pcMsurVect->AtntionDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		break;
	case ENDEVENT:
		if(m_nMouseSetCount != 0){
			BeginDraw();
			m_pcMsurVect->AtntionDraw(m_pcDC);
			EndDraw();
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdMsur::ExecAtrSet(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;
	int		ret;

	switch(event){
	case INITEVENT:
		break;
	case SETEVENT:
		m_nIDary[0] = MSURVECTID;
		m_nIDcnt = 1;
		pcData = SrchIdsData(m_nIDary, m_nIDcnt, dx, dy);
		if(pcData != NULL){ 
			SaveUndo();
			m_pcMsurVect = (CMsurVect*)pcData;
			ret = AtrSetMain();
			if(ret == TRUE){
				ScrnDataRedraw(m_pcMsurVect);
			}
		}
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

BOOL CCmdMsur::AtrSetMain()
{
	CDialogMsurAtrSet *pcDialogMsurAtrSet;
	char	szMsurStrg[RECMAX];
	int		ret;

	m_pcMsurVect->GetMsurStrg(szMsurStrg);
	m_pcMsurVect->GetFontSize(&m_dFontSize);
	pcDialogMsurAtrSet = new CDialogMsurAtrSet(m_pcView);
	pcDialogMsurAtrSet->m_dFontSize = m_dFontSize;
	pcDialogMsurAtrSet->m_csMsurStrg = _T(szMsurStrg);
	ret = pcDialogMsurAtrSet->DoModal();
	if(ret == IDOK){
		m_dFontSize = pcDialogMsurAtrSet->m_dFontSize;
		strcpy(szMsurStrg, (pcDialogMsurAtrSet->m_csMsurStrg).GetBuffer(RECMAX));
		m_pcMsurVect->SetMsurStrg(szMsurStrg);
		m_pcMsurVect->SetFontSize(m_dFontSize);
		return(TRUE);
	}
	return(FALSE);
}

void CCmdMsur::MsurXAtrSet()
{
	DBL		sx, sy, ex, ey;
	DBL		len;
	char	formatstr[RECMAX];
	char	strg[RECMAX];

	m_pcMsurVect->GetDXY(0, &sx, &sy);
	m_pcMsurVect->GetDXY(1, &ex, &ey);
	len = fabs(ex - sx);
	sprintf(formatstr, "%c2.%dlf%s", PAT, m_nFlotCount, m_szUnit);	
	sprintf(strg, formatstr, len);
	m_pcMsurVect->SetMsurStrg(strg);
}

void CCmdMsur::MsurYAtrSet()
{
	DBL		sx, sy, ex, ey;
	DBL		len;
	char	formatstr[RECMAX];
	char	strg[RECMAX];

	m_pcMsurVect->GetDXY(0, &sx, &sy);
	m_pcMsurVect->GetDXY(1, &ex, &ey);
	len = fabs(ey - sy);
	sprintf(formatstr, "%c2.%dlf%s", PAT, m_nFlotCount, m_szUnit);	
	sprintf(strg, formatstr, len);
	m_pcMsurVect->SetMsurStrg(strg);
}

