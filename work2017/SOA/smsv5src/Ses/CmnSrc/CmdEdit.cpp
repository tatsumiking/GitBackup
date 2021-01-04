
/////////////////////////////////////////////////////////////////////////////
// CCmdEdit

#include "stdafx.h"
#include "CmdBase.h"
#include "CmdEdit.h"
#include "DialogInputNum.h"

//extern "C" {
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Pout.h"
#include "../../ObjLib/Plot.h"
#include "../../ObjLib/Outlib.h"
//}

CCmdEdit::CCmdEdit(CScrollView* pcview)
:CCmdBase(pcview)
{
}

UINT CCmdEdit::ExecDelete()
{
	CTonDoc	*pcDoc;

	SaveUndo();
	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
	pcDoc->DataActiveCls();
	ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
	return(ENDEVENT);
}

UINT CCmdEdit::ExecMove(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		InitDataAreaBoxs();
		ScrnAllRedraw();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			CntrKindInit(dx, dy);
			ScrnAllRedraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			CntrKindMove(dx, dy);
			SaveUndo();
			MoveTrnsMain();
			ScrnAllRedraw();
			InitDataAreaBoxs();
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			CntrKindMove(dx, dy);
			DrawAreaBoxs(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:	// —vC³
		CDC	*pcDC;
		int	mode;

		pcDC = ((CTonView*)m_pcView)->m_pcDC;
		if(pcDC != NULL){
			mode = pcDC->GetROP2();
			pcDC->SetROP2(R2_XORPEN);
			DrawAreaBoxs(pcDC);
			pcDC->SetROP2(mode);
		}
		break;
	case RESETEVENT:
	case ENDEVENT:
		ScrnAllRedraw();
		m_nMouseSetCount = 0;
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdEdit::ExecCopy(UINT event, DBL dx, DBL dy)
{
	CTonDoc *pcDoc;

	switch(event){
	case INITEVENT:
		InitDataAreaBoxs();
		ScrnAllRedraw();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			SaveUndo();
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			pcDoc->OffFlag(COPYFLAG);
			pcDoc->CheckOnFlag(ACTIVEFLAG, COPYFLAG);
			pcDoc->OffFlag(ACTIVEFLAG);
			pcDoc->CopyData(0, 0);
			pcDoc->OffFlag(COPYFLAG);
			InitDataAreaBoxs();
			CntrKindInit(dx, dy);
			ScrnAllRedraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			CntrKindMove(dx, dy);
			MoveTrnsMain();
			ScrnAllRedraw();
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
		if(m_nMouseSetCount == 1){
			CntrKindMove(dx, dy);
			ScrnAllRedraw();
		}
		break;
	case REDRAWEVENT:	// —vC³
		CDC	*pcDC;
		int	mode;

		pcDC = ((CTonView*)m_pcView)->m_pcDC;
		if(pcDC != NULL){
			mode = pcDC->GetROP2();
			pcDC->SetROP2(R2_XORPEN);
			DrawAreaBoxs(pcDC);
			pcDC->SetROP2(mode);
		}
		break;
	case RESETEVENT:
	case ENDEVENT:
		ScrnAllRedraw();
		pcDoc = (CTonDoc*)(m_pcView->GetDocument());
		pcDoc->OffFlag(COPYFLAG);
		m_nMouseSetCount = 0;
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdEdit::ExecPMove(UINT event, DBL dx, DBL dy)
{
	DBL		movex, movey;

	switch(event){
	case INITEVENT:
		m_nFirstFlag = ON;
		PltMoveOutInit();
		InitDataAreaBoxs();
		BeginDraw();
		DrawAreaBoxs(m_pcDC);
		EndDraw();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			m_dCX = dx; m_dCY = dy;
			m_dBX = dx; m_dBY = dy;
			DrawAreaBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			EndDraw();
			m_dCX = dx;
			m_dCY = dy;
			movex = m_dCX - m_dBX;
			movey = m_dCY - m_dBY;
			m_dSX = m_dBSX + movex;
			m_dSY = m_dBSY + movey;
			m_dEX = m_dBEX + movex;
			m_dEY = m_dBEY + movey;
			SaveUndo();
			MoveTrnsMain();
			InitDataAreaBoxs();
			ScrnAllRedraw();
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			m_dCX = dx;
			m_dCY = dy;
			movex = m_dCX - m_dBX;
			movey = m_dCY - m_dBY;
			m_dSX = m_dBSX + movex;
			m_dSY = m_dBSY + movey;
			m_dEX = m_dBEX + movex;
			m_dEY = m_dBEY + movey;
			DrawAreaBoxs(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:	// —vC³
		CDC	*pcDC;
		int	mode;

		pcDC = ((CTonView*)m_pcView)->m_pcDC;
		if(pcDC != NULL){
			mode = pcDC->GetROP2();
			pcDC->SetROP2(R2_XORPEN);
			DrawAreaBoxs(pcDC);
			pcDC->SetROP2(mode);
		}
		break;
	case RESETEVENT:
		if(m_nFirstFlag == ON){
			if(AfxMessageBox(_T("ÌßÛ¯À°‚ÍÚ‘±‚³‚ê‚Ä‚¢‚Ü‚·‚©"), MB_OKCANCEL, -1) != IDOK){
				return(0);
			}
			m_nFirstFlag = OFF;
		}
		PlotMoveChk(dx, dy);
		break;
	case ENDEVENT:
		BeginDraw();
		DrawAreaBoxs(m_pcDC);
		EndDraw();
		PltMoveOutEnd();
		m_nMouseSetCount = 0;
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdEdit::ExecMoveRltv(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CDialogInputNum	*pDialogInputNum;
	DBL		movex, movey;

	if(INITEVENT != event){
		return(0);
	}
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("‰¡•ûŒüˆÚ“®—ÊŽw’è"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("ˆÚ“®—Ê"));
	pDialogInputNum->m_dInputNum = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		movex = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("c•ûŒüˆÚ“®—ÊŽw’è"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("ˆÚ“®—Ê"));
	pDialogInputNum->m_dInputNum = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		movey = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	SaveUndo();
	ScrnGetCoodMoveXY(&movey, &movey);
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
	pcDoc->MoveTrns(movex, movey);
	ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
	ScrnRedraw();
	return(ENDEVENT);
}

UINT CCmdEdit::ExecMoveCood(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CDialogInputNum	*pDialogInputNum;
	DBL		minix, miniy, maxx, maxy;
	DBL		cx, cy;
	DBL		coodx, coody;
	DBL		movex, movey;

	if(INITEVENT != event){
		return(0);
	}
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	GetDataCenter(minix, miniy, maxx, maxy, &cx, &cy);
	coodx = cx; coody = cy;
	ScrnCnvMMToCood(&coodx, &coody);

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("‰¡•ûŒüÀ•WˆÊ’uŽw’è"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("‰¡À•W"));
	pDialogInputNum->m_dInputNum = FLOOR100(coodx);
	if(pDialogInputNum->DoModal() == IDOK){
		coodx = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("c•ûŒüÀ•WˆÊ’uŽw’è"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("cÀ•W"));
	pDialogInputNum->m_dInputNum = FLOOR100(coody);
	if(pDialogInputNum->DoModal() == IDOK){
		coody = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	SaveUndo();
	ScrnCnvCoodToMM(&coodx, &coody);
	movex = coodx - cx;
	movey = coody - cy;
	pcDoc->MoveTrns(movex, movey);
	ScrnAreaRedraw(minix, miniy, maxx, maxy);
	ScrnRedraw();
	return(ENDEVENT);
}

UINT CCmdEdit::ExecMoveCntr(UINT event, DBL dx, DBL dy)
{
	CTonDoc *pcDoc;
	CData	*pcData;

	switch(event){
	case INITEVENT:
		pcDoc = (CTonDoc*)(m_pcView->GetDocument());
		pcDoc->OffFlag(ATNTION1FLAG);
		InitDataAreaBoxs();
		BeginDraw();
		DrawAreaBoxs(m_pcDC);
		EndDraw();
		break;
	case SETEVENT:
		pcDoc = (CTonDoc*)(m_pcView->GetDocument());
		pcData = pcDoc->SrchData(0, dx, dy);
		if(pcData != NULL){
			if(pcData->CheckFlag(ATNTION1FLAG) == ON){
				pcData->OffFlag(ATNTION1FLAG);
			}
			else{
				pcData->OnFlag(ATNTION1FLAG);
			}
			BeginDraw();
			pcData->AtntionDraw(m_pcDC);
			EndDraw();
		}
		break;
	case MOVEEVENT:
		break;
	case RESETEVENT:
		BeginDraw();
		DrawAreaBoxs(m_pcDC);
		EndDraw();
		DBL		minix, miniy, maxx, maxy;
		DBL		datx, daty, wakx, waky;
		DBL		movex, movey;

		pcDoc = (CTonDoc*)(m_pcView->GetDocument());
		pcDoc->GetDMiniMax(ATNTION1FLAG, &minix, &miniy, &maxx, &maxy);
		if(minix > maxx){
			return(ENDEVENT);
		}
		datx = (m_dSX + m_dEX) / 2;
		daty = (m_dSY + m_dEY) / 2;
		wakx = (minix + maxx) / 2;
		waky = (miniy + maxy) / 2;
		movex = wakx - datx;
		movey = waky - daty;
		SaveUndo();
		pcDoc->MoveTrns(movex, movey);

		ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
		ScrnRedraw();

		pcDoc = (CTonDoc*)(m_pcView->GetDocument());
		pcDoc->OffFlag(ATNTION1FLAG);
		InitDataAreaBoxs();
		BeginDraw();
		DrawAreaBoxs(m_pcDC);
		EndDraw();

		//return(REDRAWEVENT);
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdEdit::ExecCopyRltv(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CDialogInputNum	*pDialogInputNum;
	DBL		movex, movey;

	if(INITEVENT != event){
		return(0);
	}
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("‰¡•ûŒü•¡ŽÊˆÊ’uŽw’è"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("‰¡•¡ŽÊˆÊ’u"));
	pDialogInputNum->m_dInputNum = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		movex = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("c•ûŒü•¡ŽÊˆÊ’uŽw’è"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("c•¡ŽÊˆÊ’u"));
	pDialogInputNum->m_dInputNum = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		movey = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	SaveUndo();
	ScrnGetCoodMoveXY(&movey, &movey);
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->CheckOnFlag(ACTIVEFLAG, COPYFLAG);
	pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
	pcDoc->OffFlag(ACTIVEFLAG);
	pcDoc->CopyData(movex, movey);
	pcDoc->OffFlag(COPYFLAG);
	ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
	ScrnRedraw();
	return(ENDEVENT);
}

UINT CCmdEdit::ExecCopyCood(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CDialogInputNum	*pDialogInputNum;
	DBL		minix, miniy, maxx, maxy;
	DBL		cx, cy;
	DBL		coodx, coody;
	DBL		copyx, copyy;

	if(INITEVENT != event){
		return(0);
	}
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	GetDataCenter(minix, miniy, maxx, maxy, &cx, &cy);
	coodx = cx; coody = cy;
	ScrnCnvMMToCood(&coodx, &coody);
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("‰¡•ûŒüÀ•WˆÊ’uŽw’è"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("‰¡À•W"));
	pDialogInputNum->m_dInputNum = FLOOR100(coodx);
	if(pDialogInputNum->DoModal() == IDOK){
		coodx = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("c•ûŒüÀ•WˆÊ’uŽw’è"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("cÀ•W"));
	pDialogInputNum->m_dInputNum = FLOOR100(coody);
	if(pDialogInputNum->DoModal() == IDOK){
		coody = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}
	SaveUndo();
	ScrnCnvCoodToMM(&coodx, &coody);
	copyx = coodx - cx;
	copyy = coody - cy;
	pcDoc->CheckOnFlag(ACTIVEFLAG, COPYFLAG);
	pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
	pcDoc->OffFlag(ACTIVEFLAG);
	pcDoc->CopyData(copyx, copyy);
	pcDoc->OffFlag(COPYFLAG);
	ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
	ScrnRedraw();
	return(ENDEVENT);
}


UINT CCmdEdit::ExecCopyPich(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CDialogInputNum	*pDialogInputNum;
	DBL		minix, miniy, maxx, maxy;
	DBL		xsize, ysize;
	DBL		xcount, ycount;
	DBL		spacex, spacey;
	DBL		movex, movey;
	DBL		x, y;


	if(INITEVENT != event){
		return(0);
	}
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("‰¡•ûŒü•¡ŽÊŒÂ”Žw’è"));
	pDialogInputNum->SetUnitText(_T("ŒÂ"));
	pDialogInputNum->SetTitleText(_T("‰¡ŒÂ”"));
	pDialogInputNum->m_dInputNum = 1;
	if(pDialogInputNum->DoModal() == IDOK){
		xcount = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("‰¡•ûŒü•¡ŽÊŠÔŠuŽw’è"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("‰¡ŠÔŠu"));
	pDialogInputNum->m_dInputNum = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		spacex = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("c•ûŒü•¡ŽÊŒÂ”Žw’è"));
	pDialogInputNum->SetUnitText(_T("ŒÂ"));
	pDialogInputNum->SetTitleText(_T("cŒÂ”"));
	pDialogInputNum->m_dInputNum = 1;
	if(pDialogInputNum->DoModal() == IDOK){
		ycount = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("c•ûŒü•¡ŽÊŠÔŠuŽw’è"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("cŠÔŠu"));
	pDialogInputNum->m_dInputNum = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		spacey = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}

	SaveUndo();
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());

	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	scrngetmmsize(&xsize, &ysize);
	movex = spacex - minix;
	movey = ysize - spacey - maxy;
	pcDoc->MoveTrns(movex, movey);

	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	xsize = maxx - minix;
	ysize = maxy - miniy;
	pcDoc->CheckOnFlag(ACTIVEFLAG, COPYFLAG);
	for(y = 0; y < ycount; y++){
		for(x = 0; x < xcount; x++){
			if(x == 0 && y == 0){
		
			}
			else{
				movex = (xsize + spacex) * x; 
				movey = - (ysize + spacey) * y; 
				pcDoc->CopyData(movex, movey);
			}
		}
	}
	pcDoc->OffFlag(COPYFLAG);
	ScrnRedraw();
	return(ENDEVENT);
}

UINT CCmdEdit::ExecCopyPichNoMove(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CDialogInputNum	*pDialogInputNum;
	DBL		minix, miniy, maxx, maxy;
	DBL		xsize, ysize;
	DBL		xcount, ycount;
	DBL		spacex, spacey;
	DBL		movex, movey;
	DBL		x, y;
	DBL		singex, singey;

	if(INITEVENT != event){
		return(0);
	}
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("‰¡•ûŒü•¡ŽÊŒÂ”Žw’è"));
	pDialogInputNum->SetUnitText(_T("ŒÂ"));
	pDialogInputNum->SetTitleText(_T("‰¡ŒÂ”"));
	pDialogInputNum->m_dInputNum = 1;
	if(pDialogInputNum->DoModal() == IDOK){
		xcount = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("‰¡•ûŒü•¡ŽÊŠÔŠuŽw’è"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("‰¡ŠÔŠu"));
	pDialogInputNum->m_dInputNum = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		spacex = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("c•ûŒü•¡ŽÊŒÂ”Žw’è"));
	pDialogInputNum->SetUnitText(_T("ŒÂ"));
	pDialogInputNum->SetTitleText(_T("cŒÂ”"));
	pDialogInputNum->m_dInputNum = 1;
	if(pDialogInputNum->DoModal() == IDOK){
		ycount = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("c•ûŒü•¡ŽÊŠÔŠuŽw’è"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("cŠÔŠu"));
	pDialogInputNum->m_dInputNum = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		spacey = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}

	SaveUndo();
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());

	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	scrngetmmsize(&xsize, &ysize);
	//movex = spacex - minix;
	//movey = ysize - spacey - maxy;
	//pcDoc->MoveTrns(movex, movey);

	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	xsize = maxx - minix;
	ysize = maxy - miniy;
	singex = (xcount>0)?1:-1;
	singey = (ycount>0)?1:-1;
	xcount = fabs(xcount);
	ycount = fabs(ycount);
	pcDoc->CheckOnFlag(ACTIVEFLAG, COPYFLAG);
	for(y = 0; y < ycount; y++){
		for(x = 0; x < xcount; x++){
			if(x == 0 && y == 0){
		
			}
			else{
				movex = (xsize + spacex) * x * singex;
				movey = - (ysize + spacey) * y * singey; 
				pcDoc->CopyData(movex, movey);
			}
		}
	}
	pcDoc->OffFlag(COPYFLAG);
	ScrnRedraw();
	return(ENDEVENT);
}

UINT CCmdEdit::ExecCopyRPich(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CDialogInputNum	*pDialogInputNum;
	DBL		minix, miniy, maxx, maxy;
	DBL		xsize, ysize;
	DBL		xcount, ycount;
	DBL		spacex, spacey;
	DBL		movex, movey;
	DBL		x, y;


	if(INITEVENT != event){
		return(0);
	}
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("‰¡•ûŒü•¡ŽÊŒÂ”Žw’è"));
	pDialogInputNum->SetUnitText(_T("ŒÂ"));
	pDialogInputNum->SetTitleText(_T("‰¡ŒÂ”"));
	pDialogInputNum->m_dInputNum = 1;
	if(pDialogInputNum->DoModal() == IDOK){
		xcount = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("‰¡•ûŒü•¡ŽÊŠÔŠuŽw’è"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("‰¡ŠÔŠu"));
	pDialogInputNum->m_dInputNum = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		spacex = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("c•ûŒü•¡ŽÊŒÂ”Žw’è"));
	pDialogInputNum->SetUnitText(_T("ŒÂ"));
	pDialogInputNum->SetTitleText(_T("cŒÂ”"));
	pDialogInputNum->m_dInputNum = 1;
	if(pDialogInputNum->DoModal() == IDOK){
		ycount = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("c•ûŒü•¡ŽÊŠÔŠuŽw’è"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("cŠÔŠu"));
	pDialogInputNum->m_dInputNum = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		spacey = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(ENDEVENT);
	}

	SaveUndo();
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());

	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	scrngetmmsize(&xsize, &ysize);
	movex = xsize - spacex - maxx;
	movey = ysize - spacey - maxy;
	pcDoc->MoveTrns(movex, movey);

	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	xsize = maxx - minix;
	ysize = maxy - miniy;
	pcDoc->CheckOnFlag(ACTIVEFLAG, COPYFLAG);
	for(x = 0; x < xcount; x++){
		for(y = 0; y < ycount; y++){
			if(x == 0 && y == 0){
		
			}
			else{
				movex = - (xsize + spacex) * x; 
				movey = - (ysize + spacey) * y; 
				pcDoc->CopyData(movex, movey);
			}
		}
	}
	pcDoc->OffFlag(COPYFLAG);
	ScrnRedraw();
	return(ENDEVENT);
}

void CCmdEdit::MoveTrnsMain()
{
	CTonDoc*	pcDoc;
	DBL		movex, movey;

	movex = m_dSX - m_dBSX;
	movey = m_dSY - m_dBSY;
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->MoveTrns(movex, movey);
}

int CCmdEdit::PltMoveOutInit()
{
	DBL		xmm, ymm;
	DBL		areasx, areasy, areaex, areaey;
	int		ret;

	m_nComID = 1;
	scrngetmmsize(&xmm, &ymm);
	areasx = 0;
	areasy = 0;
	areaex = xmm;
	areaey = ymm;
	poutoutareaset(areasx, areasy, areaex, areaey);
	ret = outlibOpen();
	if(ret == FAIL){
		m_nComID = -1;
		AfxMessageBox(_T("o—Íæ‚ª‰Šú‰»‚Å‚«‚Ü‚¹‚ñ"), MB_OK, 0);
		return(FAIL);
	}
	poutelementinit();
	poutbegin(m_nComID);
	return(0);
}

void CCmdEdit::PlotMoveChk(DBL dx, DBL dy)
{
	if(m_nComID != -1){
		plotdxymmtostep(&dx, &dy);
		poutchkmove(dx, dy);
	}
}

void CCmdEdit::PltMoveOutEnd()
{
	if(m_nComID != -1){
		poutend(m_nComID);
		outlibClose();
	}
}

void CCmdEdit::BitmapCreate()
{
	DrawAreaInit(m_dSX, m_dSY, m_dEX, m_dEY);
	MemoryInit();
	m_pMemDC->BitBlt(0, 0, m_nXSize, m_nYSize, NULL, 0, 0, BLACKNESS);
	scrnsetpreviewpen(m_pMemDC->m_hDC, 255, 255, 255, 1);
	((CTonView*)m_pcView)->ActiveObjectOnDraw(m_pMemDC);
	scrnpenend(m_pMemDC->m_hDC);
	DrawAreaEnd();
}

void CCmdEdit::BitmapDelete()
{
	MemoryEnd();
}

void CCmdEdit::DrawAreaInit(DBL dminx, DBL dminy, DBL dmaxx, DBL dmaxy)
{
	DBL		minx, miny, maxx, maxy;

	scrngetrltvtop(&m_dBsx, &m_dBsy); 
	minx = dminx; miny = dminy;
	maxx = dmaxx; maxy = dmaxy;
	scrnsetrltvtop(0, 0);
	scrndxymmtorltv(&minx, &miny);
	scrndxymmtorltv(&maxx, &maxy);
	m_nXSize = (int)(maxx - minx);
	m_nYSize = (int)(miny - maxy);
	scrnsetrltvtop(minx, maxy);
	//scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
}

void CCmdEdit::DrawAreaEnd()
{
	scrnsetrltvtop(m_dBsx, m_dBsy); 
}

void CCmdEdit::MemoryInit()
{
	CDC		*pcDC;
	int		ret;

	m_pMemDC = NULL;
	m_pBitmap = NULL;

	pcDC = m_pcView->GetDC();
	//ƒƒ‚ƒŠƒfƒoƒCƒXƒRƒ“ƒeƒNƒXƒg‚Ì¶¬
	m_pBitmap = new CBitmap();
	ret = m_pBitmap->CreateCompatibleBitmap(pcDC, m_nXSize, m_nYSize);
	m_pMemDC = new CDC();

	m_pMemDC->CreateCompatibleDC(pcDC);
	m_pcView->ReleaseDC(pcDC);
	m_oldBitmap = m_pMemDC->SelectObject(m_pBitmap);
}

void CCmdEdit::MemoryEnd()
{
	m_pMemDC->SelectObject(m_oldBitmap);
	delete(m_pBitmap);
	delete(m_pMemDC);
}

void CCmdEdit::DrawImage(CDC *pcDC)
{
	DBL		sx, sy, ex, ey;

	sx = m_dSX; sy = m_dSY;
	ex = m_dEX; ey = m_dEY;

	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);

	pcDC->BitBlt((int)sx, (int)ey, m_nXSize, m_nYSize, m_pMemDC, 0, 0, SRCINVERT);
}

DBL CCmdEdit::CntrKindInit(DBL dx, DBL dy)
{
	m_dBX = dx;
	m_dBY = dy;
	m_nCntrKind = CNTRCNTR;
	m_dCX = dx;
	m_dCY = dy;
	return(0.0);
}

void CCmdEdit::CntrKindMove(DBL dx, DBL dy)
{
	DBL		sx, sy;
	DBL		ex, ey;
	DBL		movex, movey;
	DBL		cx, cy;

	GetDataCenter(m_dBSX, m_dBSY, m_dBEX, m_dBEY, &cx, &cy);
	movex = dx - cx;
	movey = dy - cy;
	sx = m_dBSX;
	ex = m_dBEX;
	sy = m_dBSY;
	ey = m_dBEY;
	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	if(pcSysEnv->ScrnCheckShiftKey() == ON){
		if(fabs(movex) > fabs(movey)){
			sx += movex;
			ex += movex;
		}
		else{
			sy += movey;
			ey += movey;
		}
	}
	else{
		sx += movex;
		sy += movey;
		ex += movex;
		ey += movey;
	}
	m_dSX = sx;
	m_dSY = sy;
	m_dEX = ex;
	m_dEY = ey;

	m_dCX = dx;
	m_dCY = dy;
}


