
/////////////////////////////////////////////////////////////////////////////
// CCmdMsur

#include "stdafx.h"
#include "CmdBase.h"
#include "CmdMsur.h"

#include "DialogMsurAtrSet.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
//}

CCmdMsur::CCmdMsur(CScrollView* pcview)
:CCmdBase(pcview)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	TCHAR	buf[RECMAX];
	FILE	*fp;

	m_nFlotCount = 1;
	_tcscpy_s(m_szUnit, _T("mm"));
	m_dFontSize = 5.0;
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\defenv\\cmd_msur.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp != NULL){
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d"), &m_nFlotCount);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), &m_szUnit, QRECMAX);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf"), &m_dFontSize);
		fclose(fp);
	}
	if(m_dFontSize <= 0){
		m_dFontSize = 5.0;
	}
}

CCmdMsur::~CCmdMsur()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\defenv\\cmd_msur.env"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	if(fp != NULL){
		_stprintf_s(buf, _T("%d\n"), m_nFlotCount);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%s\n"), m_szUnit);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%lf\n"), m_dFontSize);
		baselibFputs(buf, fp);
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
	TCHAR	szMsurStrg[RECMAX];
	int		ret;

	m_pcMsurVect->GetMsurStrg(szMsurStrg);
	m_pcMsurVect->GetFontSize(&m_dFontSize);
	pcDialogMsurAtrSet = new CDialogMsurAtrSet(m_pcView);
	pcDialogMsurAtrSet->m_dFontSize = m_dFontSize;
	pcDialogMsurAtrSet->m_csMsurStrg = szMsurStrg;
	ret = pcDialogMsurAtrSet->DoModal();
	if(ret == IDOK){
		m_dFontSize = pcDialogMsurAtrSet->m_dFontSize;
		_tcscpy_s(szMsurStrg, (pcDialogMsurAtrSet->m_csMsurStrg).GetBuffer(RECMAX));
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
	TCHAR	formatstr[RECMAX];
	TCHAR	strg[RECMAX];

	m_pcMsurVect->GetDXY(0, &sx, &sy);
	m_pcMsurVect->GetDXY(1, &ex, &ey);
	len = fabs(ex - sx);
	_stprintf_s(formatstr, _T("%c2.%dlf%s"), PAT, m_nFlotCount, m_szUnit);	
	_stprintf_s(strg, formatstr, len);
	m_pcMsurVect->SetMsurStrg(strg);
}

void CCmdMsur::MsurYAtrSet()
{
	DBL		sx, sy, ex, ey;
	DBL		len;
	TCHAR	formatstr[RECMAX];
	TCHAR	strg[RECMAX];

	m_pcMsurVect->GetDXY(0, &sx, &sy);
	m_pcMsurVect->GetDXY(1, &ex, &ey);
	len = fabs(ey - sy);
	_stprintf_s(formatstr, _T("%c2.%dlf%s"), PAT, m_nFlotCount, m_szUnit);	
	_stprintf_s(strg, formatstr, len);
	m_pcMsurVect->SetMsurStrg(strg);
}

