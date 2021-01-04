
/////////////////////////////////////////////////////////////////////////////
// CCmdKamon

#include "stdafx.h"
#include "CmdBase.h"
#include "CmdKamon.h"

#include "DialogEpsKamonLoad.h"
#include "DialogESKamonLoad.h"
#include "DialogESKamonSave.h"
#include "DialogESKamon.h"
#include "FileEps.h"

//extern "C" {
#include "../../ObjLib/Kamonlib.h"
//}

CCmdKamon::CCmdKamon(CScrollView* pcview)
:CCmdBase(pcview)
{
}

UINT CCmdKamon::ExecEPSKamonLoad(UINT event, DBL dx, DBL dy)
{
	CDialogEpsKamonLoad*	pDialogEpsKamonLoad;
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	pDialogEpsKamonLoad = new CDialogEpsKamonLoad(m_pcView);
	pDialogEpsKamonLoad->m_dKmnSize = 100.0;
	ret = pDialogEpsKamonLoad->DoModal();
	if(ret == IDOK){
		SaveUndo();
		EPSKamonLoadSetVect(pDialogEpsKamonLoad->m_szFileName);
		//ret = SETEVENT;
		ret = ENDEVENT;
	}
	else{
		ret = ENDEVENT;
	}
	delete(pDialogEpsKamonLoad);
	ScrnAllRedraw();
	return(ret);
}

UINT CCmdKamon::ExecKamonLoad(UINT event, DBL dx, DBL dy)
{
	CDialogESKamonLoad*	pDialogESKamonLoad;
	int		ret;
	DBL		size;
	int		slctno;

	if(event != INITEVENT){
		return(0);
	}
	pDialogESKamonLoad = new CDialogESKamonLoad(m_pcView);
	pDialogESKamonLoad->m_dKmnSize = 100.0;
	ret = pDialogESKamonLoad->DoModal();
	if(ret == IDOK){
		SaveUndo();
		size = pDialogESKamonLoad->m_dKmnSize;
		slctno = pDialogESKamonLoad->m_nSelect;
		ESKamonLoadSetVect(slctno, size);
		//ret = SETEVENT;
		ret = ENDEVENT;
	}
	else{
		ret = ENDEVENT;
	}
	delete(pDialogESKamonLoad);
	ScrnAllRedraw();
	return(ret);
}

UINT CCmdKamon::ExecKamonSave(UINT event, DBL dx, DBL dy)
{
	CDialogESKamonSave*	pDialogESKamonSave;
	int		ret;
	int		slctno;

	if(event != INITEVENT){
		return(0);
	}
	pDialogESKamonSave = new CDialogESKamonSave(m_pcView);
	ret = pDialogESKamonSave->DoModal();
	if(ret == IDOK){
		slctno = pDialogESKamonSave->m_nSelect;
		ESKamonSave(slctno);
	}
	delete(pDialogESKamonSave);
	ScrnAllRedraw();
	return(ENDEVENT);
}

void CCmdKamon::EPSKamonLoadSetVect(LPTSTR filename)
{
	CFileEps*	pcFileEps;
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->OffFlag(ACTIVEFLAG);
	pcFileEps = new CFileEps(m_pcView);
	pcFileEps->LoadData(filename);
	delete(pcFileEps);
}

void CCmdKamon::ESKamonLoadSetVect(int slctno, DBL size)
{
	CTonDoc*	pcDoc;
	CDataList	*pcDataList;
	CVect		*pcVect;
	kamonheadtype head;
	DBL		dTrnsTime;
	int		pcnt;
	int		flag;
	int		atr;
	DBL		dx, dy;
	int		dp;

	pcVect == NULL;
	dTrnsTime = size / 256.0;
	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->OffFlag(ACTIVEFLAG);
	pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(pcDataList == NULL){
		return;
	}
	kamonlibopen(_T("r+b"));
	pcnt = kamonlibreadhead(slctno, &head);
	flag = ON;
	for(;pcnt > 0; pcnt--){
		kamonlibreadatrxy(&atr, &dx, &dy);
		dx = dx * dTrnsTime;
		dy = (256-dy) * dTrnsTime;
		if(flag == ON){
			pcVect = new CVect(BASEVECTID);
			pcVect->OnFlag(ATNTION1FLAG);
			dp = 0;
			flag = OFF;
		}
		if (pcVect == NULL) {
			return;
		}
		pcVect->SetAtrDXY(dp, atr, dx, dy);
		dp++;
		if(atr & REJIONMASK){
			if(atr & KAMONREJION){
				pcVect->OnFlag(REJIONFLAG);
			}
			else{
				pcVect->OffFlag(REJIONFLAG);
			}
			pcVect->SetPointCount(dp);
			pcVect->RenewMiniMax();
			pcDataList->SetData(pcVect);
			flag = ON;
		}
	}	
	pcDoc->DataAddTail((CData*)pcDataList);
	kamonlibclose();
}

void CCmdKamon::ESKamonSave(int slctno)
{
	CDialogESKamon	*pcDialogESKamon;
	CTonDoc*	pcDoc;
	POSITION	pos;
	CData		*pcData;
	CVect		*pcVect;
	kamonheadtype head;
	int		ret;
	DBL		sx, sy, ex, ey;
	DBL		sizex, sizey;
	DBL		dTrnsTime;
	int		cnt;
	int		sumpcnt, pcnt;
	int		pp;
	int		atr;
	DBL		dx, dy;


	kamonlibopen(_T("r+b"));
	ret = kamonlibreadhead(slctno, &head);
	if(ret == FAIL){
		head.name[0] = _T('\0');
		head.fullname[0] = _T('\0');
	}
	kamonlibclose();
	pcDialogESKamon = new CDialogESKamon(m_pcView);
	pcDialogESKamon->m_csName = head.name;
	pcDialogESKamon->m_csFullName = head.fullname;
	if(pcDialogESKamon->DoModal() == IDOK){
		// C³•K—v
		//_tcscpy_s(head.name, pcDialogESKamon->m_csName.GetBuffer(RECMAX));
		//_tcscpy_s(head.fullname, pcDialogESKamon->m_csFullName.GetBuffer(RECMAX));
	}

	pcDoc = (CTonDoc*)m_pcView->GetDocument();

	cnt = SetDataListSelectID(STRGVECTID, ACTIVEFLAG);
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_cPtrList.GetNext(pos);
		if(pcData == NULL){
			break;
		}
		((CStrgVect*)pcData)->DataChange();
	}

	cnt = SetDataListSelectID(BASEVECTID, ACTIVEFLAG);
	sumpcnt = 0;
	DataListGetDMiniMax(&sx, &sy, &ex, &ey);
	sizex = ex-sx;
	sizey = ey-sy;
	if(sizex > sizey){
		dTrnsTime = 254.0 / sizex;
	}
	else{
		dTrnsTime = 254.0 / sizey;
	}

	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_cPtrList.GetNext(pos);
		if(pcData == NULL){
			break;
		}
		pcnt = ((CVect*)pcData)->GetPointCount();
		sumpcnt += pcnt;
	}

	kamonlibopen(_T("r+b"));
	kamonlibwritehead(slctno, &head, sumpcnt);

	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_cPtrList.GetNext(pos);
		if(pcData == NULL){
			break;
		}
		pcVect = (CVect*)pcData;
		pcnt = pcVect->GetPointCount();
		for(pp = 0; pp < pcnt; pp++){
			pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
			dx = (dx - sx) * dTrnsTime+1;
			dy = 254-(dy - sy) * dTrnsTime+1;
			if(pp == pcnt-1){
				atr |= REJIONMASK;
				if(pcVect->CheckFlag(REJIONFLAG) == ON){
					atr |= KAMONREJION;
				}
			}
			kamonlibwriteatrxy(atr, dx, dy);
		}
	}
	kamonlibclose();
}
