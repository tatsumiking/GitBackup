
/////////////////////////////////////////////////////////////////////////////
// CCmdSpcl

#include "stdafx.h"
#include "../CmnSrc/CmdBase.h"
#include "../CmnSrc/CmdSpcl.h"


#include "../CmnSrc/DialogOutLine.h"
#include "../CmnSrc/DialogShadow.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Curvelib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Pdrwlib.h"
#include "../../ObjLib/Mdfylib.h"
//}

//#define	BITMAPSIZE	512
//#define	POINTMAX	256
#define	BITMAPPAPERSIZE	4096
#define	POINTMAX	8192

CCmdSpcl::CCmdSpcl(CScrollView* pcview)
:CCmdBase(pcview)
{
}

UINT CCmdSpcl::ExecShadow(UINT event, DBL dx, DBL dy)
{
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	SaveUndo();
	ActiveObjectCopy();
	ret = DialogShadowInit();
	if(ret == TRUE){
		ShadowMain();
	}
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdSpcl::ExecOutLine(UINT event, DBL dx, DBL dy)
{
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	ret = DialogOutLineInit();
	if(ret == TRUE){
		SaveUndo();
		m_pcDoc->OffFlag(ATNTION1FLAG);
		m_pcDoc->OffFlag(ATNTION2FLAG);
		m_pcDoc->CheckOnFlag(ACTIVEFLAG, ATNTION2FLAG);
		m_pcDoc->OffFlag(ACTIVEFLAG);
		m_pcView->BeginWaitCursor(); 
		senvonconrmode();
		OutLineMain();
		senvoffconrmode();
		ScrnRedraw();
		m_pcView->RestoreWaitCursor(); 
	}
	return(ENDEVENT);
}

UINT CCmdSpcl::ExecMarge(UINT event, DBL dx, DBL dy)
{
	POSITION	pos, delpos;
	CData	*pcData;
	
	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	m_dOutLineMM = 0;
	m_nOutLineKind = 1;
	SaveUndo();
	senvsetcrtfillpenno(0);
	m_pcDoc->SetFillClrFlagOn(ACTIVEFLAG);
	m_pcDoc->OffFlag(ATNTION1FLAG);
	m_pcDoc->OffFlag(ATNTION2FLAG);
	m_pcDoc->CheckOnFlag(ACTIVEFLAG, ATNTION2FLAG);
	m_pcDoc->OffFlag(ACTIVEFLAG);
	m_pcView->BeginWaitCursor(); 
	senvonconrmode();
	OutLineMain();
	senvoffconrmode();
	pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
	while (pos != NULL){
		delpos = pos;
		pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
		if(pcData == NULL){
			break;
		}
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			m_pcDoc->m_pcObjDataList->RemoveAt(delpos);
			delete(pcData);
		}
	}
	m_pcDoc->CheckOnFlag(ATNTION1FLAG, ACTIVEFLAG);
	m_pcDoc->OffFlag(ATNTION1FLAG);
	ScrnAllRedraw();
	m_pcView->RestoreWaitCursor(); 
	//senvsetcrtfillpenno(-1);
	//m_pcDoc->SetFillClrFlagOn(ACTIVEFLAG);
	return(ENDEVENT);
}

UINT CCmdSpcl::ExecRuleOutLine(UINT event, DBL dx, DBL dy)
{
	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	SaveUndo();
	m_dOutLineMM = 0;
	m_pcDoc->OffFlag(ATNTION1FLAG);
	m_pcDoc->OffFlag(ATNTION2FLAG);
	m_pcDoc->CheckOnFlag(ACTIVEFLAG, ATNTION2FLAG);
	m_pcDoc->OffFlag(ACTIVEFLAG);
	m_pcView->BeginWaitCursor(); 
	RuleOutLineMain();
	ScrnRedraw();
	m_pcView->RestoreWaitCursor(); 
	return(ENDEVENT);
}

UINT CCmdSpcl::ExecOutLineOmit(UINT event, DBL dx, DBL dy)
{
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	ret = DialogOutLineInit();
	if(ret == TRUE){
		SaveUndo();
		CTypedPtrList<CObList,CData*> *pcObjDataList = m_pcDoc->m_pcObjDataList;
		POSITION pos = pcObjDataList->GetHeadPosition();
		while (pos != NULL)
		{
			CData* pcData = pcObjDataList->GetNext(pos);
			if(pcData->CheckFlag(ACTIVEFLAG) == ON){
				pcData->OutLineOmit(m_dOutLineMM);
			}
		}
	}
	ScrnRedraw();
	return(ENDEVENT);
}

UINT CCmdSpcl::ExecAreaSlct(UINT event, DBL dx, DBL dy)
{
	DBL		xytime;

	switch(event){
	case INITEVENT:
		((CTonApp*)AfxGetApp())->LoadIcon(IDR_MAINFRAME); 
		InitClass();
		baseDispatch();
		InitDataAreaBoxs();
	 	DrawAreaInit(m_dSX, m_dSY, m_dEX, m_dEY);
		xytime = (m_dEY - m_dSY) / (m_dEX - m_dSX);
		MemoryInit(xytime);
		BitMapInit();
		m_pcDoc->OffFlag(ATNTION1FLAG);
		m_pcDoc->OffFlag(ATNTION2FLAG);
		m_pcDoc->CheckOnFlag(ACTIVEFLAG, ATNTION2FLAG);
		m_pcDoc->OffFlag(ACTIVEFLAG);
		scrnsetpreviewpen(m_pMemDC->m_hDC, 255, 255, 255, 1);
		((CTonView*)m_pcView)->CheckFlagOutlineDraw(ATNTION2FLAG, m_pMemDC);
		scrnpenend(m_pMemDC->m_hDC);
		DrawAreaEnd();
		((CTonApp*)AfxGetApp())->LoadStandardIcon(IDI_APPLICATION); 
		break;
	case SETEVENT:
		SaveUndo();
		DrawAreaInit(m_dSX, m_dSY, m_dEX, m_dEY);
		m_pcView->BeginWaitCursor(); 
		AreaSlctTrace(dx, dy);
		m_pcView->RestoreWaitCursor(); 
		DrawAreaEnd();
		BeginDraw();
		m_pcDataList->AtntionDraw(m_pcDC);
		EndDraw();
		break;
	case RESETEVENT:
	case ENDEVENT:
		m_nMouseSetCount = 0;
		m_pcDoc->CheckOnFlag(ATNTION1FLAG, ACTIVEFLAG);
		m_pcDoc->OffFlag(ATNTION2FLAG);
		MemoryEnd();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
	
}

UINT CCmdSpcl::ExecBezeToArc(UINT event, DBL dx, DBL dy)
{
	CVect	*pcVect;
	POSITION	pos;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	SaveUndo();
	SetDataListSelectID(BASEVECTID, ACTIVEFLAG);
	pos = m_pcPtrList->GetHeadPosition();
	while (pos != NULL)
	{
		pcVect = (CVect*)m_pcPtrList->GetNext(pos);
		BezeToArcMain(pcVect);
	}

	return(ENDEVENT);
}

// 縁取り関数
int CCmdSpcl::DialogOutLineInit()
{
	CDialogOutLine	*pcOutLine;
	int		ret;

	pcOutLine = new CDialogOutLine(m_pcView);
	pcOutLine->SetTitle(_T("オフセット複合"));
	ret = pcOutLine->DoModal();
	if(ret == IDOK){
		m_dOutLineMM = pcOutLine->m_dOutLineMM;
		m_nOutLineKind = pcOutLine->m_nOutLineKind;
		delete(pcOutLine);
		return(TRUE);
	}
	delete(pcOutLine);
	return(FALSE);
}

void CCmdSpcl::OutLineMain()
{
	DBL		dminix, dminiy, dmaxx, dmaxy;
	DBL		xytime;

 	OutLineGetDMiniMax(&dminix, &dminiy, &dmaxx, &dmaxy);
	xytime = (dmaxy - dminiy) / (dmaxx - dminix);
	MemoryInit(xytime);
 	DrawAreaInit(dminix, dminiy, dmaxx, dmaxy);

	BitMapInit();

	OutLineDataDraw();

	m_pcDoc->DataActiveCls();
	BitMapTraceMain();
	m_pcDataList = (CDataList*)m_pcDoc->NewData(DATALISTID);
	m_pcDoc->DataAddHead(m_pcDataList);
	BitMapTrace(m_lpBits, m_lpTBits, m_nXByte, m_nXSize, m_nYSize);

	OutLineDataTrns();
	DrawAreaEnd();
	MemoryEnd();
}

void CCmdSpcl::RuleOutLineMain()
{
	CTonDoc	*pcDoc;
	DBL		dminix, dminiy, dmaxx, dmaxy;
	DBL		xytime;

	// 処理後の座標を求めるため影部分を処理
	pcDoc = (CTonDoc*)m_pcView->GetDocument();

 	OutLineGetDMiniMax(&dminix, &dminiy, &dmaxx, &dmaxy);
	xytime = (dmaxy - dminiy) / (dmaxx - dminix);
	MemoryInit(xytime);
 	DrawAreaInit(dminix, dminiy, dmaxx, dmaxy);

	BitMapInit();

	OutLineDataDraw();

	pcDoc->DataActiveCls();
	BitMapTraceMain();
	m_pcDataList = (CDataList*)pcDoc->NewData(DATALISTID);
	pcDoc->DataAddHead(m_pcDataList);
	BitMapTrace(m_lpBits, m_lpTBits, m_nXByte, m_nXSize, m_nYSize);

	OutLineDataTrns();
	DrawAreaEnd();
	MemoryEnd();
}

void CCmdSpcl::OutLineDataDraw()
{
	DBL			thick;
	COLORREF	clr1;
	COLORREF	clr2;

	thick = fabs(m_dOutLineMM * 2);
	clr1 = RGB(255, 255, 255);
	if(m_dOutLineMM < 0){
		clr2 = RGB(0, 0, 0);
	}
	else{
		clr2 = RGB(255, 255, 255);
	}
	((CTonView*)m_pcView)->CheckFlagDrawSetClr(ATNTION2FLAG, m_pMemDC, clr1, clr2, thick);
}

void CCmdSpcl::OutLineDataTrns()
{
	m_pcDoc->SizeTrns(0.0, 0.0, m_dTrnsTime, m_dTrnsTime);
	m_pcDoc->MoveTrns(m_dCntrX, m_dCntrY);
	m_pcDoc->CheckOnFlag(ACTIVEFLAG, ATNTION1FLAG);
	m_pcDoc->OffFlag(ACTIVEFLAG);
	m_pcDoc->CheckOnFlag(ATNTION2FLAG, ACTIVEFLAG);
	m_pcDoc->OffFlag(ATNTION2FLAG);
	//BmpFileOut("c:\\test0003.bmp", m_lpTBits, m_nXByte, m_nXSize, m_nYSize);
}

void CCmdSpcl::OutLineGetDMiniMax(LPDBL minix, LPDBL miniy, LPDBL maxx, LPDBL maxy)
{
	DBL		dminix, dminiy, dmaxx, dmaxy;	// 元データーのサイズ

	m_pcDoc->GetDMiniMax(ATNTION2FLAG, &dminix, &dminiy, &dmaxx, &dmaxy);
	*minix = dminix - fabs(m_dOutLineMM);
	*miniy = dminiy - fabs(m_dOutLineMM);
	*maxx = dmaxx + fabs(m_dOutLineMM);
	*maxy = dmaxy + fabs(m_dOutLineMM);
}

// 影付け処理関数
void CCmdSpcl::ShadowMain()
{
	DBL		dminix, dminiy, dmaxx, dmaxy;

	// 処理後の座標を求めるため影部分を処理
	m_pcDoc->SaveArchive();
	switch(m_nKind){
	case 1:
		ShadowDataTrns1();
		break;
	case 2:
		ShadowDataTrns2();
		break;
	case 3:
		ShadowDataTrns3();
		break;
	case 4:
		ShadowDataTrns4();
		break;
	}
	MemoryInit(1.0);

	ShadowGetDMiniMax(&dminix, &dminiy, &dmaxx, &dmaxy);

	m_pcDoc->DataActiveCls();
	m_pcDoc->LoadArchive();

 	DrawAreaInit(dminix, dminiy, dmaxx, dmaxy);

	BitMapInit();
	switch(m_nKind){
	case 1:
		ShadowProg1();
		break;
	case 2:
		ShadowProg2();
		break;
	case 3:
		ShadowProg3();
		break;
	case 4:
		ShadowProg4();
		break;
	}

	m_pcDoc->DataActiveCls();
	BitMapTraceMain();
	m_pcDataList = (CDataList*)m_pcDoc->NewData(DATALISTID);
	m_pcDoc->DataAddHead(m_pcDataList);
	BitMapTrace(m_lpBits, m_lpTBits, m_nXByte, m_nXSize, m_nYSize);

	ShadowDataTrns();
	DrawAreaEnd();
	MemoryEnd();
}

int CCmdSpcl::DialogShadowInit()
{
	CDialogShadow	*pcShadow;
	int		ret;

	pcShadow = new CDialogShadow(m_pcView);
	ret = pcShadow->DoModal();
	if(ret == IDOK){
		m_nKind = pcShadow->m_nKind;
		m_dDistance = pcShadow->m_dDistance;
		m_dRoundRag = pcShadow->m_dRoundRag;
		delete(pcShadow);
		return(TRUE);
	}
	delete(pcShadow);
	return(FALSE);
}

void CCmdSpcl::ActiveObjectCopy()
{
	m_pcDoc->OffFlag(ATNTION1FLAG);
	m_pcDoc->OffFlag(ATNTION2FLAG);
	m_pcDoc->CheckOnFlag(ACTIVEFLAG, COPYFLAG);
	m_pcDoc->OffFlag(ACTIVEFLAG);

	m_pcDoc->CopyData(0, 0);

	m_pcDoc->OffFlag(ATNTION1FLAG);
	m_pcDoc->OffFlag(ATNTION2FLAG);
	m_pcDoc->CheckOnFlag(COPYFLAG, ATNTION2FLAG);
	m_pcDoc->OffFlag(COPYFLAG);
	m_pcDoc->CheckOffFlag(ACTIVEFLAG, ATNTION1FLAG);
}

void CCmdSpcl::ShadowDataTrns1()
{
	DBL		movex, movey;
	DBL		dminix, dminiy, dmaxx, dmaxy;

	m_pcDoc->GetDMiniMax(ATNTION2FLAG, &dminix, &dminiy, &dmaxx, &dmaxy);
	movex = cos(m_dRoundRag*M_1RAG) * m_dDistance;
	movey = sin(m_dRoundRag*M_1RAG) * m_dDistance;
	// データをm_pMemDcに描画処理
	m_pcDoc->MoveTrns(movex, movey);
}

void CCmdSpcl::ShadowProg1()
{
	DBL		movex, movey;

	// データをm_pMemDcに描画処理
	movex = cos(m_dRoundRag*M_1RAG) * m_dDistance;
	movey = sin(m_dRoundRag*M_1RAG) * m_dDistance;
	CTonDoc	*pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->MoveTrns(movex, movey);
	// 影の描画
	scrnsetpreviewpen(m_pMemDC->m_hDC, 255, 255, 255, 1);
	((CTonView*)m_pcView)->CheckFlagDraw(ACTIVEFLAG, m_pMemDC);
	scrnpenend(m_pMemDC->m_hDC);

	// 基図形を抜く
	//scrnsetpreviewpen(m_pMemDC->m_hDC, 255, 255, 255, 1);
	scrnsetpreviewpen(m_pMemDC->m_hDC, 0, 0, 0, 1);
	((CTonView*)m_pcView)->CheckFlagDraw(ATNTION2FLAG, m_pMemDC);
	scrnpenend(m_pMemDC->m_hDC);
}

void CCmdSpcl::ShadowDataTrns2()
{
	DBL		dtime;
	DBL		ddx, ddy;
	DBL		dminix, dminiy, dmaxx, dmaxy;

	CTonDoc	*pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->GetDMiniMax(ATNTION2FLAG, &dminix, &dminiy, &dmaxx, &dmaxy);
	dtime = m_dDistance / (dmaxy - dminiy);
	ddx = tan(m_dRoundRag*M_1RAG);
	ddy = 0;
	pcDoc->SizeTrns(dminix, dminiy, 1.0, dtime);
	pcDoc->SlopTrns(dminix, dminiy, ddx, ddy);
}

void CCmdSpcl::ShadowProg2()
{
	DBL		dtime;
	DBL		ddx, ddy;
	DBL		dminix, dminiy, dmaxx, dmaxy;

	CTonDoc	*pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->GetDMiniMax(ATNTION2FLAG, &dminix, &dminiy, &dmaxx, &dmaxy);
	dtime = m_dDistance / (dmaxy - dminiy);
	ddx = tan(m_dRoundRag*M_1RAG);
	ddy = 0;
	pcDoc->SizeTrns(dminix, dminiy, 1.0, dtime);
	pcDoc->SlopTrns(dminix, dminiy, ddx, ddy);

	// 影の描画
	scrnsetpreviewpen(m_pMemDC->m_hDC, 255, 255, 255, 1);
	((CTonView*)m_pcView)->CheckFlagDraw(ACTIVEFLAG, m_pMemDC);
	scrnpenend(m_pMemDC->m_hDC);

	// 基図形を抜く
	//scrnsetpreviewpen(m_pMemDC->m_hDC, 255, 255, 255, 1);
	scrnsetpreviewpen(m_pMemDC->m_hDC, 0, 0, 0, 1);
	((CTonView*)m_pcView)->CheckFlagDraw(ATNTION2FLAG, m_pMemDC);
	scrnpenend(m_pMemDC->m_hDC);
}

void CCmdSpcl::ShadowDataTrns3()
{
	DBL		movex, movey;

	movex = cos(m_dRoundRag*M_1RAG) * m_dDistance;
	movey = sin(m_dRoundRag*M_1RAG) * m_dDistance;
	// データをm_pMemDcに描画処理
	CTonDoc	*pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->MoveTrns(movex, movey);
}

void CCmdSpcl::ShadowProg3()
{
	DBL		movex, movey;
	DBL		baselen, steplen;
	int		nlen, i;

	CTonDoc	*pcDoc = (CTonDoc*)m_pcView->GetDocument();
	baselen = m_dDistance;
	scrndlenmmtorltv(&baselen);
	nlen = (int)baselen;
	steplen = m_dDistance / (DBL)nlen;
	for(i = 0; i <= nlen; i++){
		_stprintf_s(m_szMsgBuf, _T("描画処理 %d ／ %d"), i, nlen);
		HlpMsgDsp(m_szMsgBuf);
		//Dispatch();

		movex = cos(m_dRoundRag*M_1RAG) * steplen;
		movey = sin(m_dRoundRag*M_1RAG) * steplen;
	// データをm_pMemDcに描画処理
		pcDoc->MoveTrns(movex, movey);

		scrnsetpreviewpen(m_pMemDC->m_hDC, 255, 255, 255, 1);
		((CTonView*)m_pcView)->CheckFlagDraw(ACTIVEFLAG, m_pMemDC);
		scrnpenend(m_pMemDC->m_hDC);
	}
	// 基図形を抜く
	//scrnsetpreviewpen(m_pMemDC->m_hDC, 255, 255, 255, 1);
	scrnsetpreviewpen(m_pMemDC->m_hDC, 0, 0, 0, 1);
	((CTonView*)m_pcView)->CheckFlagDraw(ATNTION2FLAG, m_pMemDC);
	scrnpenend(m_pMemDC->m_hDC);

}

void CCmdSpcl::ShadowDataTrns4()
{
	DBL		movex, movey;
	DBL		maxsizex, maxsizey;
	DBL		dminix, dminiy, dmaxx, dmaxy;

	CTonDoc	*pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->GetDMiniMax(ATNTION2FLAG, &dminix, &dminiy, &dmaxx, &dmaxy);
	maxsizex = dmaxx - dminix;
	maxsizey = dmaxy - dminiy;
	movex = cos(m_dRoundRag*M_1RAG) * m_dDistance;
	movey = sin(m_dRoundRag*M_1RAG) * m_dDistance;
	// データをm_pMemDcに描画処理
	pcDoc->MoveTrns(movex, movey);
}

void CCmdSpcl::ShadowProg4()
{
	DBL		movex, movey;
	DBL		baselen, steplen;
	int		nlen, i;
	DBL		dminix, dminiy, dmaxx, dmaxy;
	DBL		dcntrx, dcntry;
	DBL		maxsizex, maxsizey;
	DBL		dxtime, dytime, dtime;

	CTonDoc	*pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->GetDMiniMax(ATNTION2FLAG, &dminix, &dminiy, &dmaxx, &dmaxy);
	baselen = m_dDistance;
	scrndlenmmtorltv(&baselen);
	nlen = (int)baselen;
	steplen = m_dDistance / (DBL)nlen;
	for(i = 0; i <= nlen; i++){
		_stprintf_s(m_szMsgBuf, _T("描画処理 %d ／ %d"), i, nlen);
		HlpMsgDsp(m_szMsgBuf);
		//Dispatch();

		movex = cos(m_dRoundRag*M_1RAG) * steplen;
		movey = sin(m_dRoundRag*M_1RAG) * steplen;
	// データをm_pMemDcに描画処理
		pcDoc->MoveTrns(movex, movey);
		pcDoc->GetDMiniMax(ACTIVEFLAG, &dminix, &dminiy, &dmaxx, &dmaxy);
		maxsizex = dmaxx - dminix;
		maxsizey = dmaxy - dminiy;
		dcntrx = (dmaxx + dminix) / 2;
		dcntry = (dmaxy + dminiy) / 2;
		dxtime = 1.0 - movex / maxsizex;
		dytime = 1.0 - movey / maxsizey;
		if(dxtime > dytime){
			dtime = dytime;
		}
		else{
			dtime = dxtime;
		}
		pcDoc->SizeTrns(dcntrx, dcntry, dtime, dtime);

		scrnsetpreviewpen(m_pMemDC->m_hDC, 255, 255, 255, 1);
		((CTonView*)m_pcView)->CheckFlagDraw(ACTIVEFLAG, m_pMemDC);
		scrnpenend(m_pMemDC->m_hDC);
	}
	// 基図形を抜く
	//scrnsetpreviewpen(m_pMemDC->m_hDC, 255, 255, 255, 1);
	scrnsetpreviewpen(m_pMemDC->m_hDC, 0, 0, 0, 1);
	((CTonView*)m_pcView)->CheckFlagDraw(ATNTION2FLAG, m_pMemDC);
	scrnpenend(m_pMemDC->m_hDC);
}

void CCmdSpcl::ShadowDataTrns()
{
	CTonDoc	*pcDoc;

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->SizeTrns(0.0, 0.0, m_dTrnsTime, m_dTrnsTime);
	pcDoc->MoveTrns(m_dCntrX, m_dCntrY);
	pcDoc->CheckOnFlag(ACTIVEFLAG, ATNTION1FLAG);
	pcDoc->OffFlag(ACTIVEFLAG);
	pcDoc->CheckOnFlag(ATNTION2FLAG, ACTIVEFLAG);
	pcDoc->OffFlag(ATNTION2FLAG);
	//BmpFileOut("c:\\test0003.bmp", m_lpTBits, m_nXByte, m_nXSize, m_nYSize);
}

void CCmdSpcl::ShadowGetDMiniMax(LPDBL minix, LPDBL miniy, LPDBL maxx, LPDBL maxy)
{
	DBL		dminix, dminiy, dmaxx, dmaxy;	// 元データーのサイズ
	DBL		sminix, sminiy, smaxx, smaxy;	// 影部分のサイズ

	CTonDoc	*pcDoc;

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->GetDMiniMax(ATNTION2FLAG, &dminix, &dminiy, &dmaxx, &dmaxy);
	pcDoc->GetDMiniMax(ACTIVEFLAG, &sminix, &sminiy, &smaxx, &smaxy);
	*minix = min(sminix, dminix);
	*miniy = min(sminiy, dminiy);
	*maxx = max(smaxx, dmaxx);
	*maxy = max(smaxy, dmaxy);
}

// 領域指定関数
void CCmdSpcl::AreaSlctTrace(DBL dx, DBL dy)
{
	long	lBitsSize;
	int		cnt, cnt1, cnt2, cnt3, cnt4;
	int		x, y, x1, y1, x2, y2, x3, y3, x4, y4;
	int		cus;

	m_nXByte = (((long)m_nXSize + 7L) / 8L + 3L) / 4L * 4L;
	lBitsSize = (long)m_nYSize * m_nXByte;

	m_pBitmap->GetBitmapBits(lBitsSize, m_lpBits);

	//BmpFileOut("c:\\test0001.bmp", m_lpBits, m_nXByte, m_nXSize, m_nYSize);
	//重なり部分のアウトライン抽出処理

	memcpy(m_lpTBits, m_lpBits, lBitsSize);
	//BitMapOutLineSet(m_lpBits, m_lpTBits, m_nXByte, m_nXSize, m_nYSize);
	//BmpFileOut("c:\\test0002.bmp", m_lpTBits, m_nXByte, m_nXSize, m_nYSize);

	scrndxymmtorltv(&dx, &dy);
	if(dx <= 0){	dx = 1;	}
	if(dx >= m_nXSize){	dx = m_nXSize-1;}
	if(dy <= 0){	dy = 1;	}
	if(dy >= m_nYSize){	dy = m_nYSize-1;}

	for(x1 = (int)dx, y1 = (int)dy, cnt1 = 0; x1 < m_nXSize; x1++, cnt1++){
		if(BitOnOffCheck(m_lpBits, m_nXByte, x1, y1) == 1){
			break;
		}
	}
	if(m_nXSize == x1){ cnt1 = INTMAX;}
	for(x2 = (int)dx, y2 = (int)dy, cnt2 = 0; x2 > 0; x2--, cnt2++){
		if(BitOnOffCheck(m_lpBits, m_nXByte, x2, y2) == 1){
			break;
		}
	}
	if(0 == x1){ cnt2 = INTMAX;}
	for(y3 = (int)dy, x3 = (int)dx, cnt3 = 0; y3 < m_nYSize; y3++, cnt3++){
		if(BitOnOffCheck(m_lpBits, m_nXByte, x3, y3) == 1){
			break;
		}
	}
	if(m_nYSize == y3){ cnt3 = INTMAX;}
	for(y4 = (int)dy, x4 = (int)dx, cnt4 = 0; y4 > 0; y4--, cnt4++){
		if(BitOnOffCheck(m_lpBits, m_nXByte, x4, y4) == 1){
			break;
		}
	}
	if(0 == y4){ cnt4 = INTMAX;}

	cnt = cnt1; x = x1; y = y1; cus = 0;
	if(cnt2 < cnt){ cnt = cnt2; x = x2; y = y2; cus = 4; }
	if(cnt3 < cnt){	cnt = cnt3; x = x3; y = y3; cus = 2; }
	if(cnt4 < cnt){ cnt = cnt4; x = x4; y = y4; cus = 6; }

	CTonDoc	*pcDoc = (CTonDoc*)m_pcView->GetDocument();

	m_pcDataList = (CDataList*)pcDoc->NewData(DATALISTID);
	pcDoc->DataAddHead(m_pcDataList);

	BitMapTarceSetPlgn(m_lpBits, m_lpTBits, cus, m_nXByte, x, y);

	pcDoc->SizeTrns(0.0, 0.0, m_dTrnsTime, m_dTrnsTime);
	pcDoc->MoveTrns(m_dCntrX, m_dCntrY);

	pcDoc->CheckOnFlag(ACTIVEFLAG, ATNTION1FLAG);
	pcDoc->OffFlag(ACTIVEFLAG);

}

// 共通使用関数----------------------------------------------------------

void CCmdSpcl::BitMapInit()
{
	memset(m_lpBits, 0, m_lBitsSize);
	memset(m_lpTBits, 0, m_lBitsSize);

	m_pMemDC->BitBlt( 0,0 , m_nXByte*8, m_nYSize, NULL, 0, 0, BLACKNESS);
}


void CCmdSpcl::BitMapTraceMain()
{
	long	lBitsSize;

	CTonDoc	*pcDoc = (CTonDoc*)m_pcView->GetDocument();

	m_nXByte = (((long)m_nXSize + 7L) / 8L + 3L) / 4L * 4L;
	lBitsSize = (long)m_nYSize * m_nXByte;

	m_pBitmap->GetBitmapBits(lBitsSize, m_lpBits);

	// BmpFileOut("c:\\test0001.bmp", m_lpBits, m_nXByte, m_nXSize, m_nYSize);
	//重なり部分のアウトライン抽出処理

	BitMapOutLineSet(m_lpBits, m_lpTBits, m_nXByte, m_nXSize, m_nYSize);
	//BmpFileOut("c:\\test0002.bmp", m_lpTBits, m_nXByte, m_nXSize, m_nYSize);
}

void CCmdSpcl::DrawAreaInit(DBL dminix, DBL dminiy, DBL dmaxx, DBL dmaxy)
{
	DBL		addx, addy;
	DBL		minix, miniy, maxx, maxy;
	DBL		areaxmm, areaymm;
	DBL		xtime, ytime, time;
	DBL		addrltv;

	addx = (dmaxx - dminix) / 8; 
	addy = (dmaxy - dminiy) / 8; 
	minix = dminix - addx;
	miniy = dminiy - addy;
	maxx = dmaxx + addx;
	maxy = dmaxy + addy;

	areaxmm = maxx - minix;
	areaymm = maxy - miniy;

	xtime = areaxmm / m_nXSize;
	ytime = areaymm / m_nYSize;

	if(xtime > ytime){
		time = xtime;
		addrltv = (areaymm - areaxmm) / time;
	}
	else{
		time = ytime;
		addrltv = 0;
	}
	m_nBMode = senvcheckdrawmode();
	senvsetdrawmode(PREVIEW);
	m_dBtime = scrnsetxytime(time, time);
	m_dTrnsTime = time;
	scrngetmmsize(&m_dBxmm, &m_dBymm);
	scrngetrltvtop(&m_dBsx, &m_dBsy); 
	scrngetdisprltvmove(&m_dBmvx, &m_dBmvy);
	scrnsetrltvtop(0.0, 0.0);
	scrnsetdisprltvmove(0.0, 0.0);
	scrnsetmmsize(areaxmm, areaymm);
	scrnsetrltvelement();
	scrndxymmtorltv(&minix, &miniy);
	scrndxymmtorltv(&maxx, &maxy);
	m_dCntrX = minix;
	m_dCntrY = miniy;
	m_dCntrY -= addrltv;
	scrndxyrltvtomm(&m_dCntrX, &m_dCntrY);

	scrnsetrltvtop(minix, maxy);

	scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
}

void CCmdSpcl::DrawAreaEnd()
{
	scrnsetxytime(m_dBtime, m_dBtime);
	scrnsetmmsize(m_dBxmm, m_dBymm);
	scrnsetrltvelement();
	scrnsetrltvtop(m_dBsx, m_dBsy); 
	scrnsetdisprltvmove(m_dBmvx, m_dBmvy);
	((CTonView*)m_pcView)->InitScrollElement();
	senvsetdrawmode(m_nBMode);
}

void CCmdSpcl::MemoryInit(DBL xytime)
{
	CDC		*pcDC;
	UINT	unPlanes, unBitCnt;
	DBL		sumarea;	

	m_pMemDC = NULL;
	m_pBitmap = NULL;
	unPlanes = 1;
	unBitCnt = 1;

	sumarea = BITMAPPAPERSIZE * BITMAPPAPERSIZE;
	m_nXSize = (int)(sqrt(sumarea / xytime));
	m_nYSize = (int)(m_nXSize * xytime);
	m_nXByte = ((((long)m_nXSize * (long)unBitCnt + 7) / 8L + 3L) / 4L)*4L;
	m_nXSize = m_nXByte * 8;
	m_lBitsSize = m_nXByte * m_nYSize;

	m_hBits = GlobalAlloc(GHND, m_lBitsSize);
	if(m_hBits == 0){
		return;
	}
	m_lpBits = (BYTE *) GlobalLock(m_hBits);
	if(m_lpBits == NULL){
		GlobalFree(m_hBits);
		return;
	}
	m_hTBits = GlobalAlloc(GHND, m_lBitsSize);
	if(m_hTBits == 0){
		return;
	}
	m_lpTBits = (BYTE *) GlobalLock(m_hTBits);
	if(m_lpTBits == NULL){
		GlobalFree(m_hTBits);
		return;
	}
	//メモリデバイスコンテクストの生成
	m_pBitmap = new CBitmap();
	m_pBitmap->CreateBitmap(m_nXSize, m_nYSize, unPlanes, unBitCnt, NULL);
	m_pMemDC = new CDC();

	pcDC = m_pcView->GetDC();
	m_pMemDC->CreateCompatibleDC(pcDC);
	m_pcView->ReleaseDC(pcDC);

	m_oldBitmap = m_pMemDC->SelectObject(m_pBitmap);

	m_lpCusTbl = (LPBYTE)malloc(POINTMAX);
	m_lpXYTbl1 = (XYType *) malloc(sizeof(XYType) * (POINTMAX));
	m_lpXYTbl2 = (XYType *) malloc(sizeof(XYType) * (POINTMAX));
}

void CCmdSpcl::MemoryEnd()
{
	if(m_lpXYTbl2 != NULL){
		free(m_lpXYTbl2);
	}
	if(m_lpXYTbl1 != NULL){
		free(m_lpXYTbl1);
	}
	if(m_lpCusTbl != NULL){
		free(m_lpCusTbl);
	}

	m_pMemDC->SelectObject(m_oldBitmap);
	delete(m_pBitmap);
	delete(m_pMemDC);

	if(m_lpTBits != NULL){
		GlobalUnlock(m_hTBits);
		m_lpTBits = NULL;
	}
	if(m_hTBits != 0){
		GlobalFree(m_hTBits);
		m_hTBits = 0;
	}

	if(m_lpBits != NULL){
		GlobalUnlock(m_hBits);
		m_lpBits = NULL;
	}
	if(m_hBits != 0){
		GlobalFree(m_hBits);
		m_hBits = 0;
	}
}

void CCmdSpcl::BmpFileOut(LPTSTR fname, BYTE far *m_lpBits, int xbyte, int xsize, int ysize)
{
	FILE				*fp;
	BITMAPFILEHEADER	bif;
	BITMAPINFOHEADER	bih;
	RGBQUAD				clr[2];
	int					headsize;
	BYTE				*sp;
	int					y;
	int					txbyte;

	memset((LPTSTR)(&bif), 0, sizeof(BITMAPFILEHEADER));
	memset((LPTSTR)(&bih), 0, sizeof(BITMAPINFOHEADER));
	headsize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*2;
	bif.bfType = 0x4D42;
	bif.bfSize = xbyte * ysize + headsize;
	bif.bfOffBits = headsize;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = xsize;
	bih.biHeight = ysize;
	bih.biPlanes = 1;
	bih.biBitCount = 1;
	bih.biCompression = 0;

	clr[0].rgbBlue = 0;
	clr[0].rgbGreen = 0;
	clr[0].rgbRed = 0;
	clr[0].rgbReserved = 0;
	clr[1].rgbBlue = 255;
	clr[1].rgbGreen = 255;
	clr[1].rgbRed = 255;
	clr[1].rgbReserved = 0;

	_tfopen_s(&fp, fname, _T("wb"));
	fwrite(&bif, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(clr, sizeof(RGBQUAD), 2, fp);
	txbyte = (((long)xsize + 7L) / 8L + 3L) / 4L * 4L;
	for(y = 0; y < ysize; y++){
		sp = m_lpBits + (long)y * (long)txbyte;
		fwrite(sp, 1, xbyte, fp);
	}
	fclose(fp);
}

void CCmdSpcl::BitMapOutLineSet(BYTE *lpBits, BYTE *lpTBits, int xbyte, int xsize, int ysize)
{
	int		x, y;

	// アウトラインドットを取り出す
	for(y = 0; y < ysize; y++){
		if((y % 100) == 0){
			_stprintf_s(m_szMsgBuf, _T("前処理 %d ／ %d"), y, ysize);
			HlpMsgDsp(m_szMsgBuf);
		}
		//Dispatch();
		for(x = 0; x < xsize; x++){
			if(BitOnOffCheck(lpBits, xbyte, x, y) == 1){
				if(BitOnOffCheck(lpBits, xbyte, x-1, y) == 0){
					BitOnSet(lpTBits, xbyte, x, y);
				}
				else if(BitOnOffCheck(lpBits, xbyte, x+1, y) == 0){
					BitOnSet(lpTBits, xbyte, x, y);
				}
				else if(BitOnOffCheck(lpBits, xbyte, x, y-1) == 0){
					BitOnSet(lpTBits, xbyte, x, y);
				}
				else if(BitOnOffCheck(lpBits, xbyte, x, y+1) == 0){
					BitOnSet(lpTBits, xbyte, x, y);
				}
			}
		}
	}
}

void CCmdSpcl::BitMapTrace(BYTE *lpBits, BYTE *lpTBits, int xbyte, int xsize, int ysize)
{
	int		x, y;

	// アウトライントレース処理
#ifdef _DEBUG
	BmpFileOut(_T("c:\\test0001.bmp"), lpTBits, xbyte, m_nXSize, m_nYSize);
	BmpFileOut(_T("c:\\test0002.bmp"), lpBits, xbyte, m_nXSize, m_nYSize);
#endif
	for(y = 0; y < ysize; y++){
		if((y % 100) == 0){
			_stprintf_s(m_szMsgBuf, _T("線生成処理 %d ／ %d"), y, ysize);
			HlpMsgDsp(m_szMsgBuf);
		}
		//Dispatch();
		for(x = 0; x < xsize; x++){
			if(BitOnOffCheck(lpTBits, xbyte, x, y) == 1){
				BitMapTarceSetPlgn(lpBits, lpTBits, 0, xbyte, x, y);
			}
		}
	}
}

void CCmdSpcl::BitMapTarceSetPlgn(BYTE *lpBits, BYTE *lpTBits, int scus, int xbyte, int x, int y)
{
	int		rsx, rsy;
	int		tx, ty;
	int		tsx, tsy;
	int		cus, bcus;
	long	idx;
	FILE	*fp;
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	TCHAR	buf[RECMAX];


	SetPlgnInit();
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\pointdel.lst"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	rsx = x; rsy = y;
	tsx = x; tsy = y;
	tx = x; ty = y;
	cus = FirstCusGet(lpBits, scus, xbyte, x, y);
	_stprintf_s(buf, _T("%d %d\n"), x, y);
	baselibFputs(buf, fp);
	idx = 0;
	while(1){
		bcus = cus;
		NextCusGet(lpTBits, xbyte, &tx, &ty, &cus);
		_stprintf_s(buf, _T("%d %d\n"), tx, ty);
		baselibFputs(buf, fp);
		if(cus == -1){
			break;
		}
		m_lpCusTbl[idx] = cus;
		idx++;
		if(POINTMAX/4 <= idx){
			SetXYTblSetPlgn(lpTBits, xbyte, idx, tsx, tsy);
			idx = 0;
			tsx = tx;
			tsy = ty;
			//BmpFileOut("c:\\test0001.bmp", lpTBits, xbyte, m_nXSize, m_nYSize);
		}
		if(tx == rsx && ty == rsy){
			if(idx != 0){
				SetXYTblSetPlgn(lpTBits, xbyte, idx, tsx, tsy);
			}
			break;
		}
	}
	fclose(fp);
	_tfopen_s(&fp, fname, _T("r"));
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		_stscanf_s(buf, _T("%d %d"), &tx, &ty);
		BitOffSet(lpTBits, xbyte, tx, ty);
	}
	fclose(fp);
	
	SetPlgnEnd();
}

void CCmdSpcl::SetXYTblSetPlgn(BYTE *lpTBits, int xbyte, int idx, int tsx, int tsy)
{
	int		tx, ty;
	int		bx, by;
	int		point;
	int		cus;
	int		addx, addy;
	int		i;

	tx = tsx;
	ty = tsy;
	bx = tsx;
	by = tsy;
	point = 0;
	PlgnPointSet(m_lpXYTbl1, point, tx, ty);
	point++;
	for(i = 0; i < idx; i++){
		cus = m_lpCusTbl[i];
		CusAddGet(cus, &addx, &addy);
		tx += addx;
		ty += addy;
		if(bx != tx && by != ty){
			PlgnPointSet(m_lpXYTbl1, point, tx, ty);
			point++;
			bx = tx;
			by = ty;
		}
	}	
	PlgnPointSet(m_lpXYTbl1, point, tx, ty);
	point++;
	SetDataListPolygon(m_lpXYTbl1, m_lpXYTbl2, point);
}

void CCmdSpcl::NextCusGet(BYTE *lpTBits, int xbyte, int *retx, int *rety, int *retcus)
{
	int		cus;
	int		x, y;
	int		addx, addy;
	int		i;

	cus = CusLegal(*retcus -3);
	x = *retx;
	y = *rety;
	*retcus = -1;
	for(i = 0; i < 8; i++){
		CusAddGet(cus, &addx, &addy);
		if(BitOnOffCheck(lpTBits, xbyte, x+addx, y+addy) == 1){
			*retx = x+addx;
			*rety = y+addy;
			*retcus = cus;
			break;
		}
		cus = CusLegal(cus+1);
	}
}

int CCmdSpcl::FirstCusGet(BYTE *lpBits, int scus, int xbyte, int x, int y)
{
	int	cus;
	int	addx, addy;

	for(cus = scus+1; cus != scus; cus++){
		if(cus == 8){
			cus = 0;
		}
		CusAddGet(cus, &addx, &addy);
		if(BitOnOffCheck(lpBits, xbyte, x+addx, y+addy) == 0){
			break;
		}
	}
	return(cus);
}

void CCmdSpcl::CusAddGet(int cus, int *addx, int *addy)
{
	switch(cus){
	case 7:
	case 0:
	case 1:
		*addx = -1;
		break;
	case 2:
	case 6:
		*addx = 0;
		break;
	case 3:
	case 4:
	case 5:
		*addx = 1;
		break;
	}
	switch(cus){
	case 1:
	case 2:
	case 3:
		*addy = 1;
		break;
	case 0:
	case 4:
		*addy = 0;
		break;
	case 5:
	case 6:
	case 7:
		*addy = -1;
		break;
	}
}

int CCmdSpcl::CusLegal(int cus)
{
	if(cus < 0){
		cus += 8;
	}
	if(7 < cus){
		cus -= 8;
	}
	return(cus);
}

int CCmdSpcl::BitOnOffCheck(BYTE *lpBits, int xbyte, int x, int y)
{
	int		nbyte;
	int		mask;
	BYTE	*sp;

	if(x < 0){
		return(0);
	}
	if(m_nXSize <= x){
		return(0);
	}
	if(y < 0){
		return(0);
	}
	if(m_nYSize <= y){
		return(0);
	}
	nbyte = x / 8;
	mask = 0x0080 >> (x % 8);
	sp = lpBits + (long)y * (long)xbyte + (long)nbyte;
	if(*sp & mask){
		return(1);
	}
	else{
		return(0);
	}
}

void CCmdSpcl::BitOnSet(BYTE *lpBits, int xbyte, int x, int y)
{
	int		nbyte;
	int		mask;
	BYTE	*sp;

	if(x < 0){
		return;
	}
	if(m_nXSize <= x){
		return;
	}
	if(y < 0){
		return;
	}
	if(m_nYSize <= y){
		return;
	}
	nbyte = x / 8;
	mask = 0x0080 >> (x % 8);
	sp = lpBits + (long)y * (long)xbyte + (long)nbyte;
	(*sp) |= mask;
}

void CCmdSpcl::BitOffSet(BYTE *lpBits, int xbyte, int x, int y)
{
	int		nbyte;
	int		mask;
	BYTE	*sp;

	if(x < 0){
		return;
	}
	if(m_nXSize <= x){
		return;
	}
	if(y < 0){
		return;
	}
	if(m_nYSize <= y){
		return;
	}
	nbyte = x / 8;
	mask = 0x0080 >> (x % 8);
	sp = lpBits + (long)y * (long)xbyte + (long)nbyte;
	(*sp) &= ~mask;
}

void CCmdSpcl::PlgnPointSet(XYType *lpXYTbl, int points, int tx, int ty)
{
	lpXYTbl[points].x = tx;
	lpXYTbl[points].y = ty;
}

void CCmdSpcl::SetDataListPolygon(XYType *lpXYTbl1, XYType *lpXYTbl2, int points)
{
	int		cnt1, cnt2;
	int		tp, pp;
	DBL		dx, dy;

	cnt1 = points;
	m_dFureLimit = 1;
	mdfyinit(m_dFureLimit);
	cnt2 = mdfyfirst(lpXYTbl1, cnt1, lpXYTbl2);
	if(cnt2 == 0){
		return;
	}
	cnt1 = mdfysecond(lpXYTbl2, cnt2, lpXYTbl1);
	if(cnt1 == 0){
		return;
	}
	for(tp = 0, pp = m_nPcnt; pp < m_nPcnt + cnt1; tp++, pp++){
		dx = lpXYTbl1[tp].x;
		dy = lpXYTbl1[tp].y;
		dy = m_nYSize - dy;
		m_pcVect->SetAtrDXY(pp, 0, dx, dy);
	}
	m_nPcnt = m_nPcnt + cnt1;
}

void CCmdSpcl::SetPlgnInit()
{
	m_nPcnt = 0;
	m_pcVect = new CVect(BASEVECTID);
	//m_pcVect->OffFlag(REJIONFLAG);
}

void CCmdSpcl::SetPlgnEnd()
{
	m_pcVect->SetPointCount(m_nPcnt);
	PlgnLineToArcRejionSet(m_pcVect);
	m_nPcnt = m_pcVect->GetPointCount();
	if(2 < m_nPcnt){
		m_pcVect->RenewMiniMax();
		m_pcVect->OnFlag(ATNTION1FLAG);
		m_pcDataList->SetData((CData*)m_pcVect);
	}
	else{
		delete(m_pcVect);
		m_pcVect = NULL;
	}
}

void CCmdSpcl::PlgnLineToArcRejionSet(CVect *pcVect)
{
	int		pcnt;
	int		pp1, pp3;
	int		atr;
	DBL		x1, y1, x2, y2;
	CVect	*pcTmpVect;
	int		dp;

	dp = 0;
	pcnt = pcVect->GetPointCount();
	if(pcnt < 5){
		return;
	}
	pcTmpVect = new CVect(BASEVECTID);
	for(pp1 = 0; pp1 < pcnt-1;){
		if(PlgnCurveChk(pcVect, pp1, &x2, &y2, &pp3) == ON){
			pcVect->GetDXY(pp1, &x1, &y1);
			pcTmpVect->SetAtrDXY(dp, CURVATR, x1, y1);
			dp++;
			pcTmpVect->SetAtrDXY(dp, CONTATR|CURVATR, x2, y2);
			dp++;
			pp1 = pp3;
		}
		else{
			pcVect->GetDXY(pp1, &x1, &y1);
			pcTmpVect->SetAtrDXY(dp, 0, x1, y1);
			dp++;
			pp1++;
		}
	}
	pcnt = dp;
	for(pp1 = 0; pp1 < pcnt; pp1++){
		pcTmpVect->GetAtrDXY(pp1, &atr, &x1, &y1);
		pcVect->SetAtrDXY(pp1, atr, x1, y1);
	}
	pcVect->SetPointCount(pcnt);
	delete(pcTmpVect);
}

int CCmdSpcl::PlgnCurveChk(CVect *pcVect, int pp1, DBL *retx, DBL *rety, int *retpp)
{
	int		pcnt;
	int		pp2, pp3, tpp;
	DBL		x1, y1, x2, y2, x3, y3;
	DBL		x0, y0, r;
	DBL		rag01, rag03;
	DBL		subrag;
	DBL		tx, ty, thx, thy;
	DBL		limit;
	DBL		len1, len2;
	int		ret;

	limit = m_dFureLimit*2.0;
	pcnt = pcVect->GetPointCount();
	if(pp1 == pcnt - 1
	|| pp1 == pcnt - 2){
		return(OFF);
	}

	for(pp3 = pp1 + 3; pp3 < pcnt; pp3++){
		pp2 = (pp1+pp3)/2;
		pcVect->GetDXY(pp1, &x1, &y1);
		pcVect->GetDXY(pp2, &x2, &y2);
		pcVect->GetDXY(pp3, &x3, &y3);
		if(curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0) == FAIL){
			return(OFF);
		}
		if(curve3pragrget(x1,y1,x2,y2,x3,y3,x0,y0,&rag01,&rag03,&r) == FAIL){
			return(OFF);
		}
		subrag = rag03 - rag01;
		if(M_PI_4 < fabs(subrag)){
			break;
		}

		ret = 0;
		pcVect->GetDXY(pp1, &x1, &y1);
		for(tpp = pp1+1; tpp < pp3; tpp++){
			pcVect->GetDXY(tpp, &tx, &ty);
			if(PlgnCrclDastance(tx, ty, x0, y0, r, limit) == FAIL){
				ret = FAIL;
				break;
			}
			thx = (x1 + tx) / 2;
			thy = (y1 + ty) / 2;
			if(PlgnCrclDastance(thx, thy, x0, y0, r, limit*2) == FAIL){
				ret = FAIL;
				break;
			}
			x1 = tx;
			y1 = ty;
		}
		thx = (x1 + x3) / 2;
		thy = (y1 + y3) / 2;
		if(PlgnCrclDastance(thx, thy, x0, y0, r, limit*2) == FAIL){
			ret = FAIL;
		}
		pcVect->GetDXY(pp3, &x3, &y3);

		if(ret == FAIL){
			break;
		}
	}
	if(pp3 >= pcnt){
		return(OFF);
	}
	pp3 = pp3-1;
	if(pp3 < pp1+3){
		return(OFF);
	}
	pp2 = (pp1+pp3)/2;
	pcVect->GetDXY(pp1, &x1, &y1);
	pcVect->GetDXY(pp2, &x2, &y2);
	pcVect->GetDXY(pp3, &x3, &y3);
	len1 = hypot(x1-x2, y1-y2);
	len2 = hypot(x2-x3, y2-y3);
	if(len1 < limit*2 && len2 < limit*2){
		return(OFF);
	}
	*retx = x2;
	*rety = y2;
	*retpp = pp3;
	return(ON);
	/*
	if(curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0) == FAIL){
		return(OFF);
	}
	if(curve3pragrget(x1,y1,x2,y2,x3,y3,x0,y0,&rag01,&rag03,&r) == FAIL){
		return(OFF);
	}
	subrag = rag03-rag01;

	rag02 = subrag/2.0 + rag01;
	*retx = cos(rag02)*r+x0;
	*rety = sin(rag02)*r+y0;
	*retpp = pp3;
	return(ON);
	*/
}

int CCmdSpcl::PlgnCrclDastance(DBL tx, DBL ty, DBL x0, DBL y0, DBL r, DBL flimit)
{
	DBL		frx, fry, r2, deltr;

	frx = tx - x0;
	fry = ty - y0;
	r2 = sqrt(frx*frx + fry*fry);
	deltr = fabs(r2 - r);
	if(deltr > flimit){
		return(FAIL);
	}
	return(0);
}

void CCmdSpcl::BitmapFileOut()
{
	long	lBitsSize;

	CTonDoc	*pcDoc = (CTonDoc*)m_pcView->GetDocument();

	m_nXByte = (((long)m_nXSize + 7L) / 8L + 3L) / 4L * 4L;
	lBitsSize = (long)m_nYSize * m_nXByte;

	m_pBitmap->GetBitmapBits(lBitsSize, m_lpBits);

	BmpFileOut(_T("c:\\test0001.bmp"), m_lpBits, m_nXByte, m_nXSize, m_nYSize);
}

//　図形の円弧に近似できるベーゼ曲線を円弧に変換する
void CCmdSpcl::BezeToArcMain(CVect *pcVect)
{
	CVect	*pcTVect;
	int pcnt;
	int dp, pp;
	int p0, p1, p2, p3, p4;
	int atr, atr0, atr1, atr2, atr3, atr4;
	double x0, x1, x2, x3, x4;
	double y0, y1, y2, y3, y4;

	pcTVect = new CVect(BASEVECTID);
	pcnt = pcVect->GetPointCount();
	pcVect->GetAtrDXY(0, &atr1, &x1, &y1);
	pcVect->GetAtrDXY(pcnt-1, &atr2, &x2, &y2);
	if(EQUAL(x1, x2) && EQUAL(y1, y2)){
		pcVect->OnFlag(REJIONFLAG);
		pcnt--;
	}
	dp = 0;
	for(pp = 0; pp < pcnt;){
		p0 = pcVect->PointLegal(pcnt, pp-1);
		p1 = pcVect->PointLegal(pcnt, pp+0);
		p2 = pcVect->PointLegal(pcnt, pp+1);
		p3 = pcVect->PointLegal(pcnt, pp+2);
		p4 = pcVect->PointLegal(pcnt, pp+3);
		pcVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		pcVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		pcVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		pcVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		pcVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				pcTVect->SetAtrDXY(dp++, 0, x1, y1);
				pcTVect->SetAtrDXY(dp++, 0, x2, y2);
			}
			else if(p2 == 0){
				pcTVect->SetAtrDXY(dp++, 0, x1, y1);
			}
			else{
				dp = BezeToArcFigu(x1, y1, x2, y2, x3, y3, x4, y4, pcTVect, dp);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 == 0){
				pcTVect->SetAtrDXY(dp++, 0, x1, y1);
			}
			else{
				pcTVect->SetAtrDXY(dp++, atr1, x1, y1);
				pcTVect->SetAtrDXY(dp++, atr2, x2, y2);
			}
			pp += 2;
		}
		else{
			pcTVect->SetAtrDXY(dp++, atr1, x1, y1);
			pp += 1;
		}
	}

	for(pp = 0; pp < dp; pp++){
		pcTVect->GetAtrDXY(pp, &atr, &x1, &y1);
		pcVect->SetAtrDXY(pp, atr, x1, y1);
	}
	pcVect->SetPointCount(dp);
	delete(pcTVect);
}

// ベーゼ曲線を円弧に近似する
int CCmdSpcl::BezeToArcFigu(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4, CVect *pcVect, int dp)
{
	DBL		t, t1;
	int		i, cnt, stat;
	DBL		b1, b2, b3, b4;
	DBL		x0, y0;
	DBL		rag, rag01, rag03, steprag;
	DBL		dr;
	DBL		tx1, ty1, tx2, ty2;
	DBL		step, len;

	t = 0.5;
	t1 = 1 - t;
	b1 = t1 * t1 * t1;
	b2 = 3 * t * t1 * t1;
	b3 = 3 * t * t * t1;
	b4 = t * t * t;
	tx2 = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
	ty2 = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
	stat = curve3pcenterget(x1, y1, tx2, ty2, x4, y4, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(x1, y1, tx2, ty2, x4, y4, x0, y0, &rag01, &rag03, &dr);
	}
	if(stat == FAIL){
		pcVect->SetAtrDXY(dp++, 0, x1, y1);
		return(dp);
	}
	cnt = 4;
	steprag = (rag03 - rag01) / (DBL)cnt;
	step = 1.0 / (DBL)cnt;
	for(i = 1; i < cnt; i++){
		rag = steprag * i + rag01;
		tx1 = cos(rag)*dr+x0;
		ty1 = sin(rag)*dr+y0;
		t = step * i;
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		tx2 = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ty2 = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		len = hypot(tx1-tx2, ty1-ty2);
		if(dr * 0.02 < len){
			pcVect->SetAtrDXY(dp++, BEZEATR|CURVATR, x1, y1);
			pcVect->SetAtrDXY(dp++, BEZEATR, x2, y2);
			pcVect->SetAtrDXY(dp++, BEZEATR, x3, y3);
			return(dp);
		}
	}
	pcVect->SetAtrDXY(dp++, CURVATR, x1, y1);
	rag = (rag03 - rag01) / 2 + rag01;
	x2 = cos(rag)*dr+x0;
	y2 = sin(rag)*dr+y0;
	pcVect->SetAtrDXY(dp++, CURVATR|CONTATR, x2, y2);
	return(dp);
}

