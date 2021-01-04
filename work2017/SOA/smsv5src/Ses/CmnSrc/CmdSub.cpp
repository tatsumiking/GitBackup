
/////////////////////////////////////////////////////////////////////////////
// CCmdSub

#include "stdafx.h"
#include "CmdBase.h"
#include "CmdSub.h"

#include "DialogInputNum.h"
#include "DialogScrnClr.h"
#include "DialogInputNumPair.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
//}

CCmdSub::CCmdSub(CScrollView* pcview)
:CCmdBase(pcview)
{
	m_pcDataList = NULL;
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	TCHAR	buf[RECMAX];
	FILE	*fp;

	m_dWakuSpace = 2.0;
	m_dCntrMM = 5.0;
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\defenv\\cmd_sub.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp != NULL){
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf"), &m_dWakuSpace);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf"), &m_dCntrMM);
		fclose(fp);
	}
	m_pcWakuDataList = NULL;
}

CCmdSub::~CCmdSub()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\defenv\\cmd_sub.env"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	if(fp != NULL){
		_stprintf_s(buf, _T("%4.1lf\n"), m_dWakuSpace);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%4.1lf\n"), m_dCntrMM);
		baselibFputs(buf, fp);
		fclose(fp);
	}

}

UINT CCmdSub::ExecSubGrid(UINT event, DBL dx, DBL dy)
{
	CDialogInputNum	*pcDialogInputNum;
	DBL			dGrid;

	if(event != INITEVENT){
		return(ENDEVENT);
	}
	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	pcSysEnv->ScrnGetGrid(&dGrid);
	pcDialogInputNum = new CDialogInputNum(m_pcView);
	pcDialogInputNum->SetDialogTitle(_T("グリッドサイズ設定"));
	pcDialogInputNum->SetUnitText(_T("mm"));
	pcDialogInputNum->SetTitleText(_T("グリッド"));
	pcDialogInputNum->m_dInputNum = FLOOR100(dGrid);
	if(pcDialogInputNum->DoModal() == IDOK){
		dGrid = pcDialogInputNum->m_dInputNum;
		delete(pcDialogInputNum);
	}
	else{
		dGrid = 0;
		delete(pcDialogInputNum);
	}
	pcSysEnv->ScrnSetGrid(dGrid);
	ScrnAllRedraw();	
	return(ENDEVENT);
}

void CCmdSub::ResetWakuData()
{
	m_pcWakuDataList = NULL;
}

UINT CCmdSub::ExecSubWaku(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CData*		pcData;

	InitClass();
	if(m_pcDoc->DataActiveCheck() == FALSE){
		return(ENDEVENT);
	}
	SaveUndo();
	if(m_pcWakuDataList == NULL){
		m_dWakuSpace = 2.0;
		InitDataAreaBoxs();
		m_dSX -= m_dWakuSpace;
		m_dSY -= m_dWakuSpace;
		m_dEX += m_dWakuSpace;
		m_dEY += m_dWakuSpace;
		CreateWakuBoxs();
		m_pcWakuDataList = m_pcDataList;
		m_pcDataList->OnFlag(VWAKUFLAG);
		//BeginDraw();
		//m_pcDataList->OnDraw(m_pcDC);
		//EndDraw();
	}
	else{
		SetDataListSelectID(DATALISTID, VWAKUFLAG);
		POSITION pos = m_cPtrList.GetHeadPosition();
		while (pos != NULL)
		{
			pcData = m_cPtrList.GetNext(pos);
			if(pcData == NULL){
				break;
			}
			if(pcData == m_pcWakuDataList){
				BeginDraw();
				m_pcDataList->DelDraw(m_pcDC);
				EndDraw();
				pcDoc = (CTonDoc*)(m_pcView->GetDocument());
				pcDoc->DeleteData(m_pcWakuDataList);
				delete(m_pcWakuDataList);
				break;
			}
		}
		m_pcWakuDataList = NULL;
	}
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdSub::ExecSubAnyWaku(UINT event, DBL dx, DBL dy)
{
	CTonDoc *pcDoc;

	switch(event){
	case INITEVENT:
		InitClass();
		if(m_pcDoc->DataActiveCheck() == FALSE){
			return(ENDEVENT);
		}
		m_nMouseSetCount = 0;
		m_pcDialogInputNum = new CDialogInputNum(m_pcView);
		m_pcDialogInputNum->SetModaless();
		m_pcDialogInputNum->SetDialogTitle(_T("連続枠付け設定"));
		m_pcDialogInputNum->SetUnitText(_T("mm"));
		m_pcDialogInputNum->SetTitleText(_T("枠付間隔"));
		m_pcDialogInputNum->m_dInputNum = FLOOR100(m_dWakuSpace);
		if(m_pcDialogInputNum->Create(IDD_INNUM, m_pcView)){
			m_pcDialogInputNum->SetItemElement();
			m_pcDialogInputNum->ShowWindow(SW_SHOW);
		}
		pcDoc = (CTonDoc*)(m_pcView->GetDocument());
		pcDoc->OffFlag(ATNTION1FLAG);
		//return(REDRAWEVENT);
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_nMouseSetCount = 1;
			m_dSX = dx; m_dSY = dy;
			m_dEX = dx; m_dEY = dy;
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		else if(m_nMouseSetCount == 1){
			m_nMouseSetCount = 0;
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
			m_dEX = dx; m_dEY = dy;
			m_pcDialogInputNum->GetItemElement();
			m_dWakuSpace = m_pcDialogInputNum->m_dInputNum;
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			pcDoc->AreaInFlagOn(ATNTION1FLAG, m_dSX, m_dSY, m_dEX, m_dEY);
			GetFlagOnDMiniMax(ATNTION1FLAG, &m_dSX, &m_dSY, &m_dEX, &m_dEY);
			if(m_dSX > m_dEX){
				return(0);
			}
			SaveUndo();
			m_dSX -= m_dWakuSpace;
			m_dSY -= m_dWakuSpace;
			m_dEX += m_dWakuSpace;
			m_dEY += m_dWakuSpace;
			CreateWakuBoxs();
			pcDoc->OffFlag(ATNTION1FLAG);
			BeginDraw();
			m_pcDataList->OnDraw(m_pcDC);
			EndDraw();
			ScrnAllRedraw();
			//return(REDRAWEVENT);
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			m_dEX = dx; m_dEY = dy;
			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		break;
	case DLGRESETEVENT:
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		m_pcDialogInputNum->DestroyWindow();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdSub::ExecSubWakuDel(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CData* pcData;

	if(event != INITEVENT){
		return(0);
	}

	SaveUndo();
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	SetDataListSelectID(DATALISTID, VWAKUFLAG);
	POSITION pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_cPtrList.GetNext(pos);
		if(pcData == NULL){
			break;
		}
		pcDoc->DeleteData(pcData);
		delete(pcData);
	}

	m_pcWakuDataList = NULL;
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdSub::ExecSubCntrline(UINT event, DBL dx, DBL dy)
{
	CDialogInputNum	*pcDialogInputNum;
	CTonDoc*	pcDoc;
	DBL			sx, sy, ex, ey;
	CVect*		pcVect;
	int			backfillpenno;
	int			backlinepenno;
	DBL			backlinethick;

	if(event != INITEVENT){
		return(0);
	}
	//m_dCntrMM = 5;
	pcDialogInputNum = new CDialogInputNum(m_pcView);
	pcDialogInputNum->SetDialogTitle(_T("中心線の長さ設定"));
	pcDialogInputNum->SetUnitText(_T("mm"));
	pcDialogInputNum->SetTitleText(_T("長さ"));
	pcDialogInputNum->m_dInputNum = FLOOR100(m_dCntrMM);
	if(pcDialogInputNum->DoModal() == IDOK){
		m_dCntrMM = pcDialogInputNum->m_dInputNum;
		delete(pcDialogInputNum);
	}
	else{
		delete(pcDialogInputNum);
		return(ENDEVENT);
	}

	SaveUndo();
	InitDataAreaBoxs();
	m_dCX = (m_dSX + m_dEX) / 2;
	m_dCY = (m_dSY + m_dEY) / 2;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(m_pcDataList == NULL){
		return(ENDEVENT);
	}

	sx = m_dCX; ex = m_dCX;
	sy = m_dSY-m_dCntrMM; ey = m_dSY-m_dCntrMM*2;
	pcVect = new CVect(BASEVECTID);
	m_pcDataList->SetData((CData*)pcVect);
	pcVect->SetAtrDXY(0, 0, sx, sy);
	pcVect->SetAtrDXY(1, 0, ex, ey);
	pcVect->OffFlag(REJIONFLAG);
	pcVect->SetPointCount(2);

	sx = m_dCX; ex = m_dCX;
	sy = m_dEY+m_dCntrMM; ey = m_dEY+m_dCntrMM*2;
	pcVect = new CVect(BASEVECTID);
	m_pcDataList->SetData((CData*)pcVect);
	pcVect->SetAtrDXY(0, 0, sx, sy);
	pcVect->SetAtrDXY(1, 0, ex, ey);
	pcVect->OffFlag(REJIONFLAG);
	pcVect->SetPointCount(2);

	sx = m_dSX-m_dCntrMM; ex = m_dSX-m_dCntrMM*2;
	sy = m_dCY; ey = m_dCY;
	pcVect = new CVect(BASEVECTID);
	m_pcDataList->SetData((CData*)pcVect);
	pcVect->SetAtrDXY(0, 0, sx, sy);
	pcVect->SetAtrDXY(1, 0, ex, ey);
	pcVect->OffFlag(REJIONFLAG);
	pcVect->SetPointCount(2);

	sx = m_dEX+m_dCntrMM; ex = m_dEX+m_dCntrMM*2;
	sy = m_dCY; ey = m_dCY;
	pcVect = new CVect(BASEVECTID);
	m_pcDataList->SetData((CData*)pcVect);
	pcVect->SetAtrDXY(0, 0, sx, sy);
	pcVect->SetAtrDXY(1, 0, ex, ey);
	pcVect->OffFlag(REJIONFLAG);
	pcVect->SetPointCount(2);

	backfillpenno = senvgetcrtfillpenno();
	backlinepenno = senvgetcrtlinepenno();
	backlinethick = senvgetcrtlinethick();
	senvsetcrtfillpenno(-1);
	m_pcDataList->SetDefFillClr();
	senvsetcrtfillpenno(backfillpenno);
	senvsetcrtlinepenno(backlinepenno);
	senvsetcrtlinethick(backlinethick);
	m_pcDataList->RenewMiniMax();
	AddLinkDoc(m_pcDataList);

	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdSub::ExecScrnClrSet(UINT event, DBL dx, DBL dy)
{
	CDialogScrnClr *pcScrnClrSet;

	if(event != INITEVENT){
		return(0);
	}

	pcScrnClrSet = new CDialogScrnClr(m_pcView);
	if(pcScrnClrSet->DoModal() == IDOK){
		ScrnAllRedraw();
	}
	delete(pcScrnClrSet);
	return(ENDEVENT);
}

UINT CCmdSub::ExecCntrLineFree(UINT event, DBL dx, DBL dy)
{
	CTonApp *theApp;
	CSysEnv *pcSysEnv;

	theApp = (CTonApp*)AfxGetApp();
	pcSysEnv = theApp->m_pcSysEnv;
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		BeginDraw();
		((CTonView*)m_pcView)->WorkAreaCntrLineDraw(m_pcDC);
		EndDraw();
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_nMouseSetCount = 1;
			BeginDraw();
			((CTonView*)m_pcView)->WorkAreaCntrLineDraw(m_pcDC);
			m_dEX = dx; m_dEY = dy;
			pcSysEnv->ScrnSetCntrXY(dx, dy);
			((CTonView*)m_pcView)->WorkAreaCntrLineDraw(m_pcDC);
			EndDraw();
		}
		else if(m_nMouseSetCount == 1){
			m_nMouseSetCount = 0;
			BeginDraw();
			((CTonView*)m_pcView)->WorkAreaCntrLineDraw(m_pcDC);
			EndDraw();
			pcSysEnv->ScrnSetCntrXY(dx, dy);
			ScrnAllRedraw();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			((CTonView*)m_pcView)->WorkAreaCntrLineDraw(m_pcDC);
			pcSysEnv->ScrnSetCntrXY(dx, dy);
			((CTonView*)m_pcView)->WorkAreaCntrLineDraw(m_pcDC);
			EndDraw();
		}
		break;
	case ENDEVENT:
		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdSub::ExecCntrLineCood(UINT event, DBL dx, DBL dy)
{
	DBL		cx, cy;
	CDialogInputNumPair	*pcDialogInputNumPair;
	CTonApp *theApp;
	CSysEnv *pcSysEnv;

	if(INITEVENT != event){
		return(0);
	}
	theApp = (CTonApp*)AfxGetApp();
	pcSysEnv = theApp->m_pcSysEnv;
	pcSysEnv->ScrnGetCntrXY(&cx, &cy);
	pcDialogInputNumPair = new CDialogInputNumPair(m_pcView);
	pcDialogInputNumPair->SetDialogTitle(_T("原点座標指定"));
	pcDialogInputNumPair->SetUnitText(_T("mm"), _T("mm"));
	pcDialogInputNumPair->SetTitleText(_T("Ｘ座標"), _T("Ｙ座標"));
	pcDialogInputNumPair->m_dInputNumX = FLOOR100(cx);
	pcDialogInputNumPair->m_dInputNumY = FLOOR100(cy);
	if(pcDialogInputNumPair->DoModal() == IDOK){
		cx = pcDialogInputNumPair->m_dInputNumX;
		cy = pcDialogInputNumPair->m_dInputNumY;
	}
	delete(pcDialogInputNumPair);
	//ScrnCnvCoodToMM(&cx, &cy);

	pcSysEnv->ScrnSetCntrXY(cx, cy);
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdSub::ExecCntrLineRltv(UINT event, DBL dx, DBL dy)
{
	DBL		cx, cy;
	CDialogInputNumPair	*pcDialogInputNumPair;
	CTonApp *theApp;
	CSysEnv *pcSysEnv;

	if(INITEVENT != event){
		return(0);
	}
	theApp = (CTonApp*)AfxGetApp();
	pcSysEnv = theApp->m_pcSysEnv;
	pcSysEnv->ScrnGetCntrXY(&cx, &cy);
	pcDialogInputNumPair = new CDialogInputNumPair(m_pcView);
	pcDialogInputNumPair->SetDialogTitle(_T("原点座標相対移動"));
	pcDialogInputNumPair->SetUnitText(_T("mm"), _T("mm"));
	pcDialogInputNumPair->SetTitleText(_T("Ｘ座標"), _T("Ｙ座標"));
	pcDialogInputNumPair->m_dInputNumX = 0;
	pcDialogInputNumPair->m_dInputNumY = 0;
	if(pcDialogInputNumPair->DoModal() == IDOK){
		cx += pcDialogInputNumPair->m_dInputNumX;
		cy += pcDialogInputNumPair->m_dInputNumY;
	}
	delete(pcDialogInputNumPair);
	pcSysEnv->ScrnSetCntrXY(cx, cy);
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdSub::ExecSubLineFree(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		m_pcView->m_nSublineDraw = OFF;
		ScrnAllRedraw();
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_nMouseSetCount = 1;
			BeginDraw();
			m_pcView->WorkAreaSubLineDraw(m_pcDC);
			m_dEX = dx; m_dEY = dy;
			m_pcSysEnv->ScrnSetSubXY(dx, dy);
			m_pcView->WorkAreaSubLineDraw(m_pcDC);
			EndDraw();
		}
		else if(m_nMouseSetCount == 1){
			m_nMouseSetCount = 0;
			BeginDraw();
			m_pcView->WorkAreaSubLineDraw(m_pcDC);
			m_pcSysEnv->ScrnSetSubXY(dx, dy);
			m_pcView->WorkAreaSubLineDraw(m_pcDC);
			EndDraw();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcView->WorkAreaSubLineDraw(m_pcDC);
			m_pcSysEnv->ScrnSetSubXY(dx, dy);
			m_pcView->WorkAreaSubLineDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		BeginDraw();
		m_pcView->WorkAreaSubLineDraw(m_pcDC);
		EndDraw();
		break;
	case ENDEVENT:
		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdSub::ExecSubLineFreerag(UINT event, DBL dx, DBL dy)
{
	DBL		cx, cy;
	DBL		rag;

	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		m_pcView->m_nSublineDraw = OFF;
		ScrnAllRedraw();
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_nMouseSetCount = 1;
			BeginDraw();
			m_pcView->WorkAreaSubLineDraw(m_pcDC);
			m_pcSysEnv->ScrnGetSubXY(&cx, &cy);
			if(cx != dx || cy != dy){
				rag = atan2(dy - cy, dx - cx);
				m_pcSysEnv->ScrnSetSubRag(-rag);
			}
			m_pcView->WorkAreaSubLineDraw(m_pcDC);
			EndDraw();
		}
		else if(m_nMouseSetCount == 1){
			m_nMouseSetCount = 0;
			BeginDraw();
			m_pcView->WorkAreaSubLineDraw(m_pcDC);
			m_pcSysEnv->ScrnGetSubXY(&cx, &cy);
			if(cx != dx || cy != dy){
				rag = atan2(dy - cy, dx - cx);
				m_pcSysEnv->ScrnSetSubRag(-rag);
			}
			m_pcView->WorkAreaSubLineDraw(m_pcDC);
			EndDraw();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcView->WorkAreaSubLineDraw(m_pcDC);
			m_pcSysEnv->ScrnGetSubXY(&cx, &cy);
			if(cx != dx || cy != dy){
				rag = atan2(dy - cy, dx - cx);
				m_pcSysEnv->ScrnSetSubRag(-rag);
			}
			m_pcView->WorkAreaSubLineDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		BeginDraw();
		m_pcView->WorkAreaSubLineDraw(m_pcDC);
		EndDraw();
		break;
	case ENDEVENT:
		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdSub::ExecSubLineCood(UINT event, DBL dx, DBL dy)
{
	DBL		cx, cy;
	CDialogInputNumPair	*pcDialogInputNumPair;
	CTonApp *theApp;
	CSysEnv *pcSysEnv;

	if(INITEVENT != event){
		return(0);
	}
	theApp = (CTonApp*)AfxGetApp();
	pcSysEnv = theApp->m_pcSysEnv;
	pcSysEnv->ScrnGetSubXY(&cx, &cy);
	ScrnCnvMMToCood(&cx, &cy);
	pcDialogInputNumPair = new CDialogInputNumPair(m_pcView);
	pcDialogInputNumPair->SetDialogTitle(_T("補助線座標指定"));
	pcDialogInputNumPair->SetUnitText(_T("mm"), _T("mm"));
	pcDialogInputNumPair->SetTitleText(_T("Ｘ座標"), _T("Ｙ座標"));
	pcDialogInputNumPair->m_dInputNumX = FLOOR100(cx);
	pcDialogInputNumPair->m_dInputNumY = FLOOR100(cy);
	if(pcDialogInputNumPair->DoModal() == IDOK){
		cx = pcDialogInputNumPair->m_dInputNumX;
		cy = pcDialogInputNumPair->m_dInputNumY;
	}
	delete(pcDialogInputNumPair);
	ScrnCnvCoodToMM(&cx, &cy);

	BeginDraw();
	((CTonView*)m_pcView)->WorkAreaSubLineDraw(m_pcDC);
	pcSysEnv->ScrnSetSubXY(cx, cy);
	((CTonView*)m_pcView)->WorkAreaSubLineDraw(m_pcDC);
	EndDraw();
	return(ENDEVENT);
}

UINT CCmdSub::ExecSubLineRltv(UINT event, DBL dx, DBL dy)
{
	DBL		cx, cy;
	CDialogInputNumPair	*pcDialogInputNumPair;
	CTonApp *theApp;
	CSysEnv *pcSysEnv;

	if(INITEVENT != event){
		return(0);
	}
	theApp = (CTonApp*)AfxGetApp();
	pcSysEnv = theApp->m_pcSysEnv;
	pcSysEnv->ScrnGetSubXY(&cx, &cy);
	pcDialogInputNumPair = new CDialogInputNumPair(m_pcView);
	pcDialogInputNumPair->SetDialogTitle(_T("補助線相対移動"));
	pcDialogInputNumPair->SetUnitText(_T("mm"), _T("mm"));
	pcDialogInputNumPair->SetTitleText(_T("Ｘ座標"), _T("Ｙ座標"));
	pcDialogInputNumPair->m_dInputNumX = 0;
	pcDialogInputNumPair->m_dInputNumY = 0;
	if(pcDialogInputNumPair->DoModal() == IDOK){
		cx += pcDialogInputNumPair->m_dInputNumX;
		cy += pcDialogInputNumPair->m_dInputNumY;
	}
	delete(pcDialogInputNumPair);
	BeginDraw();
	((CTonView*)m_pcView)->WorkAreaSubLineDraw(m_pcDC);
	pcSysEnv->ScrnSetSubXY(cx, cy);
	((CTonView*)m_pcView)->WorkAreaSubLineDraw(m_pcDC);
	EndDraw();
	return(ENDEVENT);
}

UINT CCmdSub::ExecSubLineRag(UINT event, DBL dx, DBL dy)
{
	DBL		rag;
	CDialogInputNum	*pcDialogInputNum;
	CTonApp *theApp;
	CSysEnv *pcSysEnv;

	if(INITEVENT != event){
		return(0);
	}
	pcDialogInputNum = new CDialogInputNum(m_pcView);
	pcDialogInputNum->SetDialogTitle(_T("補助線角度指定"));
	pcDialogInputNum->SetUnitText(_T("度"));
	pcDialogInputNum->SetTitleText(_T("角度"));
	pcDialogInputNum->m_dInputNum = 0;
	if(pcDialogInputNum->DoModal() == IDOK){
		rag = pcDialogInputNum->m_dInputNum * M_1RAG;
	}
	delete(pcDialogInputNum);
	BeginDraw();
	((CTonView*)m_pcView)->WorkAreaSubLineDraw(m_pcDC);
	theApp = (CTonApp*)AfxGetApp();
	pcSysEnv = theApp->m_pcSysEnv;
	pcSysEnv->ScrnSetSubRag(rag);
	((CTonView*)m_pcView)->WorkAreaSubLineDraw(m_pcDC);
	EndDraw();
	return(ENDEVENT);
}

UINT CCmdSub::ExecSubLineDel(UINT event, DBL dx, DBL dy)
{
	CTonApp *theApp;
	CSysEnv *pcSysEnv;

	if(INITEVENT != event){
		return(0);
	}
	BeginDraw();
	((CTonView*)m_pcView)->WorkAreaSubLineDraw(m_pcDC);
	theApp = (CTonApp*)AfxGetApp();
	pcSysEnv = theApp->m_pcSysEnv;
	pcSysEnv->ScrnOffSubFlag();
	EndDraw();
	return(ENDEVENT);
}

void CCmdSub::CreateWakuBoxs()
{
	CTonDoc*	pcDoc;
	CVect*		pcVect;
	int		backfillpenno;
	int		backlinepenno;
	DBL		backlinethick;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(m_pcDataList == NULL){
		return;
	}
	pcVect = new CVect(BASEVECTID);
	m_pcDataList->SetData((CData*)pcVect);
	pcVect->SetPointCount(4);
	SetBoxsPoint(pcVect);

	backfillpenno = senvgetcrtfillpenno();
	backlinepenno = senvgetcrtlinepenno();
	backlinethick = senvgetcrtlinethick();
	senvsetcrtfillpenno(-1);
	m_pcDataList->SetDefFillClr();
	senvsetcrtfillpenno(backfillpenno);
	senvsetcrtlinepenno(backlinepenno);
	senvsetcrtlinethick(backlinethick);
	m_pcDataList->OnFlag(VWAKUFLAG);
	m_pcDataList->RenewMiniMax();
	AddLinkDoc(m_pcDataList);
}

void CCmdSub::SetBoxsPoint(CVect* pcVect)
{
	pcVect->SetAtrDXY(0, 0, m_dSX, m_dSY);
	pcVect->SetAtrDXY(1, 0, m_dEX, m_dSY);
	pcVect->SetAtrDXY(2, 0, m_dEX, m_dEY);
	pcVect->SetAtrDXY(3, 0, m_dSX, m_dEY);
}

void CCmdSub::GetFlagOnDMiniMax(int flag, LPDBL minix, LPDBL miniy, LPDBL maxx, LPDBL maxy)
{
	CTonDoc*	pcDoc;
	CData*		pcData;
	POSITION	nextpos;
	DBL			tminix, tminiy, tmaxx, tmaxy;

	(*minix) = INTMAX;
	(*miniy) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	POSITION pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pcData = pcObjDataList->GetNext(nextpos);
		if(pcData->CheckFlag(flag) == ON){
			tminix = INTMAX; tminiy = INTMAX;
			tmaxx = -INTMAX; tmaxy = -INTMAX;
			pcData->GetDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
			(*minix) = min((*minix), tminix);
			(*miniy) = min((*miniy), tminiy);
			(*maxx) = max((*maxx), tmaxx);
			(*maxy) = max((*maxy), tmaxy);
		}
		pos = nextpos;
	}
}

