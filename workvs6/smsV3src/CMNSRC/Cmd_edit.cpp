
/////////////////////////////////////////////////////////////////////////////
// CCmdEdit

#include "stdafx.h"
#include "math.h"

#include "cmd_base.h"
#include "dlgtwpnt.h"
#include "dlinpair.h"
#include "cmd_edit.h"

extern "C" {
 	void scrnsetpreviewpen(HDC PaintDC, int r, int g, int b, int thick);
 	void scrnsetpointpen(HDC hDC);
 	void scrnpenend(HDC PaintDC);
	void scrngetrltvtop(LPDBL sx, LPDBL sy); 
	void scrnsetrltvtop(DBL sx, DBL sy); 
	void scrncoody(LPDBL movey);
	void scrnsetslctpen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	void scrndxymmtorltv(LPDBL x, LPDBL y);
	void scrndlenmmtorltv(LPDBL len);
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
	void poutoutareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	void poutelementinit();
	void poutbegin(int nComID);
	void poutend(int nComID);
	void poutchkmove(DBL x, DBL y);	
	void pouttypeget(int *type);
	void plotdxymmtostep(LPDBL x, LPDBL y);
	int outlibOpen();
	void outlibPutString(char *buf);
	void outlibClose();
}

CCmdEdit::CCmdEdit(CScrollView* pcview)
:CCmdBaseBmp(pcview)
{
	m_dRltvMoveX = 0;
	m_dRltvMoveY = 0;
}

UINT CCmdEdit::ExecDelete()
{
	CTonDoc	*pcDoc;

	SaveUndo();
	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->DataActiveCls();
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdEdit::ExecMove(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		InitDataAreaBoxs();
		BitmapCreate();
		m_pcView->RedrawFlagOn();
		ScrnRedraw();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			CntrKindInit(dx, dy);
			DrawAreaBoxs(m_pcDC);
			DrawImage(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			DrawImage(m_pcDC);
			CntrKindMove(dx, dy);
			EndDraw();
			SaveUndo();
			MoveTrnsMain();

			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			m_pcView->RedrawFlagOn();
			ScrnRedraw();
			m_pcView->UpdateWindow();

			InitDataAreaBoxs();
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			DrawImage(m_pcDC);
			CntrKindMove(dx, dy);
			DrawAreaBoxs(m_pcDC);
			DrawImage(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:	// —vC³
		CDC	*pcDC;
		int	mode;

		pcDC = m_pcView->m_pcDC;
		if(pcDC != NULL){
			mode = pcDC->GetROP2();
			pcDC->SetROP2(R2_XORPEN);
			DrawAreaBoxs(pcDC);
			pcDC->SetROP2(mode);
		}
		break;
	case RESETEVENT:
	case ENDEVENT:
		BeginDraw();
		DrawAreaBoxs(m_pcDC);
		if(m_nMouseSetCount == 1){
			DrawImage(m_pcDC);
		}
		EndDraw();
		m_nMouseSetCount = 0;
		BitmapDelete();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdEdit::ExecMove2PSlct(UINT event, DBL dx, DBL dy)
{
	CTonDoc *pcDoc;
	CData*	pcData;
	DBL		tlen;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			pcData = pcDoc->SrchData(ACTIVEFLAG, dx, dy);
			if(pcData != NULL){
				tlen = INTMAX;
				m_pcVect = (CVect*)(pcData->SrchNearPoint(dx, dy, &tlen, &m_nPP));
				if(m_pcVect != NULL){
					BeginDraw();
					scrnsetpointpen(m_pcDC->m_hDC);
					m_pcVect->PointAreaDraw(m_pcDC, m_nPP, -1);
					scrnpenend(m_pcDC->m_hDC);
					EndDraw();
					m_nMouseSetCount = 1;
				}
			}
		}
		else if(m_nMouseSetCount == 1){
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			m_pcData = pcDoc->SrchData(ACTIVEFLAG, dx, dy);
			if(m_pcData != NULL){
				tlen = INTMAX;
				m_pcVect2 = (CVect*)(m_pcData->SrchNearPoint(dx, dy, &tlen, &m_nPP2));
				if(m_pcVect != NULL){
					BeginDraw();
					scrnsetpointpen(m_pcDC->m_hDC);
					m_pcVect2->PointAreaDraw(m_pcDC, m_nPP2, -1);
					scrnpenend(m_pcDC->m_hDC);
					EndDraw();
					SaveUndo();
					Move2PointMain();
					ScrnAllRedraw();
					m_nMouseSetCount = 0;
				}
			}
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case RESETEVENT:
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, -1);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
		}
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
		pcDoc = (CTonDoc*)(m_pcView->GetDocument());
		pcDoc->OffFlag(COPYFLAG);
		pcDoc->CheckOnFlag(ACTIVEFLAG, COPYFLAG);
		pcDoc->OffFlag(ACTIVEFLAG);
		pcDoc->CopyData(0, 0);
		//pcDoc->OffFlag(COPYFLAG);
		InitDataAreaBoxs();
		BitmapCreate();
		m_pcView->RedrawFlagOn();
		ScrnRedraw();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			CntrKindInit(dx, dy);
			DrawAreaBoxs(m_pcDC);
			DrawImage(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			DrawImage(m_pcDC);
			EndDraw();
			CntrKindMove(dx, dy);
			SaveUndo();
			MoveTrnsMain();
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			m_pcView->RedrawFlagOn();
			ScrnRedraw();

			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			pcDoc->OffFlag(COPYFLAG);
			pcDoc->CheckOnFlag(ACTIVEFLAG, COPYFLAG);
			pcDoc->OffFlag(ACTIVEFLAG);
			pcDoc->CopyData(0, 0);
			//pcDoc->OffFlag(COPYFLAG);
			InitDataAreaBoxs();
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			DrawImage(m_pcDC);
			CntrKindMove(dx, dy);
			DrawAreaBoxs(m_pcDC);
			DrawImage(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:	// —vC³
		CDC	*pcDC;
		int	mode;

		pcDC = m_pcView->m_pcDC;
		if(pcDC != NULL){
			mode = pcDC->GetROP2();
			pcDC->SetROP2(R2_XORPEN);
			DrawAreaBoxs(pcDC);
			pcDC->SetROP2(mode);
		}
		break;
	case RESETEVENT:
	case ENDEVENT:
		BeginDraw();
		DrawAreaBoxs(m_pcDC);
		if(m_nMouseSetCount == 1){
			DrawImage(m_pcDC);
		}
		EndDraw();
		pcDoc = (CTonDoc*)(m_pcView->GetDocument());
		pcDoc->DataActiveCls();
		pcDoc->CheckOnFlag(COPYFLAG, ACTIVEFLAG);
		pcDoc->OffFlag(COPYFLAG);
		m_nMouseSetCount = 0;
		BitmapDelete();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdEdit::ExecPMove(UINT event, DBL dx, DBL dy)
{
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
			// 2001.02.01 —§g
			if(m_pcView->m_nKeyFlags & MK_SHIFT){
				CntrKindInit(dx, dy);
			}
			else{
				m_dCX = dx; m_dCY = dy;
				m_dBX = dx; m_dBY = dy;
			}
			DrawAreaBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAreaBoxs(m_pcDC);
			EndDraw();
			CntrKindMove(dx, dy);
			SaveUndo();
			MoveTrnsMain();
			InitDataAreaBoxs();
			ScrnAllRedraw();
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
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

		pcDC = m_pcView->m_pcDC;
		if(pcDC != NULL){
			mode = pcDC->GetROP2();
			pcDC->SetROP2(R2_XORPEN);
			DrawAreaBoxs(pcDC);
			pcDC->SetROP2(mode);
		}
		break;
	case RESETEVENT:
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
	CDialogInputNumPair	*pDialogInputNumPair;
	DBL		movex, movey;

	if(INITEVENT != event){
		return(0);
	}
TOPLabel:
	pDialogInputNumPair = new CDialogInputNumPair(m_pcView);
	pDialogInputNumPair->SetDialogTitle("ˆÚ“®—ÊŽw’è");
	pDialogInputNumPair->SetUnitText("mm", "mm");
	pDialogInputNumPair->SetTitleText("‰¡ˆÚ“®—Ê", "cˆÚ“®—Ê");
	pDialogInputNumPair->m_dInputNumX = m_dRltvMoveX;
	pDialogInputNumPair->m_dInputNumY = m_dRltvMoveY;
	if(pDialogInputNumPair->DoModal() == IDOK){
		m_dRltvMoveX = pDialogInputNumPair->m_dInputNumX;
		m_dRltvMoveY = pDialogInputNumPair->m_dInputNumY;
		delete(pDialogInputNumPair);
	}
	else{
		delete(pDialogInputNumPair);
		return(ENDEVENT);
	}
	movex = m_dRltvMoveX;
	movey = m_dRltvMoveY;
	SaveUndo();
	ScrnGetCoodMoveXY(&movey, &movey);
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
	pcDoc->MoveTrns(movex, movey);
	ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
	ScrnRedraw();
	goto TOPLabel;
}

UINT CCmdEdit::ExecMoveCood(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CDialogInputNumPair	*pDialogInputNumPair;
	DBL		minix, miniy, maxx, maxy;
	DBL		cx, cy;
	DBL		coodx, coody;
	DBL		movex, movey;

	if(INITEVENT != event){
		return(0);
	}
TOPLabel:
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	GetDataCenter(minix, miniy, maxx, maxy, &cx, &cy);
	coodx = cx;
	coody = cy;
	ScrnCnvMMToCood(&coodx, &coody);

	pDialogInputNumPair = new CDialogInputNumPair(m_pcView);
	pDialogInputNumPair->SetDialogTitle("À•WˆÊ’uŽw’è");
	pDialogInputNumPair->SetUnitText("mm", "mm");
	pDialogInputNumPair->SetTitleText("‰¡À•W", "cÀ•W");
	pDialogInputNumPair->m_dInputNumX = FLOOR100(coodx);
	pDialogInputNumPair->m_dInputNumY = FLOOR100(coody);
	if(pDialogInputNumPair->DoModal() == IDOK){
		coodx = pDialogInputNumPair->m_dInputNumX;
		coody = pDialogInputNumPair->m_dInputNumY;
		delete(pDialogInputNumPair);
	}
	else{
		delete(pDialogInputNumPair);
		return(ENDEVENT);
	}

	SaveUndo();
	ScrnCnvCoodToMM(&coodx, &coody);
	movex = coodx - cx;
	movey = coody - cy;
	pcDoc->MoveTrns(movex, movey);
	ScrnAreaRedraw(minix, miniy, maxx, maxy);
	ScrnRedraw();
	goto TOPLabel;
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
	case UPMOVEEVENT:
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

		//ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
		//ScrnRedraw();
		// 2001.10.01 —§g üó‘Ô‚Ì‚Æ‚«Žw’èƒf[ƒ^‚ªÁ‚¦‚éƒoƒOC³
		ScrnAllRedraw();

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
	CDialogInputNumPair	*pDialogInputNumPair;
	DBL		movex, movey;

	if(INITEVENT != event){
		return(0);
	}

TOPLabel:
	pDialogInputNumPair = new CDialogInputNumPair(m_pcView);
	pDialogInputNumPair->SetDialogTitle("ˆÚ“®—ÊŽw’è");
	pDialogInputNumPair->SetUnitText("mm", "mm");
	pDialogInputNumPair->SetTitleText("‰¡ˆÚ“®—Ê", "cˆÚ“®—Ê");
	pDialogInputNumPair->m_dInputNumX = 0;
	pDialogInputNumPair->m_dInputNumY = 0;
	if(pDialogInputNumPair->DoModal() == IDOK){
		movex = pDialogInputNumPair->m_dInputNumX;
		movey = pDialogInputNumPair->m_dInputNumY;
		delete(pDialogInputNumPair);
	}
	else{
		delete(pDialogInputNumPair);
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
	goto TOPLabel;

}

UINT CCmdEdit::ExecCopyCood(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CDialogInputNumPair	*pDialogInputNumPair;
	DBL		minix, miniy, maxx, maxy;
	DBL		cx, cy;
	DBL		coodx, coody;
	DBL		copyx, copyy;

	if(INITEVENT != event){
		return(0);
	}
TOPLabel:
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minix, &miniy, &maxx, &maxy);
	GetDataCenter(minix, miniy, maxx, maxy, &cx, &cy);
	coodx = cx; coody = cy;
	ScrnCnvMMToCood(&coodx, &coody);

	pDialogInputNumPair = new CDialogInputNumPair(m_pcView);
	pDialogInputNumPair->SetDialogTitle("À•WˆÊ’uŽw’è");
	pDialogInputNumPair->SetUnitText("mm", "mm");
	pDialogInputNumPair->SetTitleText("‰¡À•W", "cÀ•W");
	pDialogInputNumPair->m_dInputNumX = FLOOR100(coodx);
	pDialogInputNumPair->m_dInputNumY = FLOOR100(coody);
	if(pDialogInputNumPair->DoModal() == IDOK){
		coodx = pDialogInputNumPair->m_dInputNumX;
		coody = pDialogInputNumPair->m_dInputNumY;
		delete(pDialogInputNumPair);
	}
	else{
		delete(pDialogInputNumPair);
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
	goto TOPLabel;
}

UINT CCmdEdit::ExecCopyMirror(UINT event, DBL dx, DBL dy)
{
	CTonDoc *pcDoc;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_dSX = dx;
			m_dSY = dy;
			m_dEX = dx;
			m_dEY = dy;
			BeginDraw();
			DrawAtnLine(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			SaveUndo();
			BeginDraw();
			DrawAtnLine(m_pcDC);
			m_dEX = dx;
			m_dEY = dy;
			EndDraw();
			pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
			MainMirrCopy();
			m_nMouseSetCount = 0;
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnRedraw();
			
			//return(REDRAWEVENT);
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnLine(m_pcDC);
			m_dEX = dx;
			m_dEY = dy;
			DrawAtnLine(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnLine(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		break;
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnLine(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
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
	pDialogInputNum->SetDialogTitle("‰¡•ûŒü•¡ŽÊŒÂ”Žw’è");
	pDialogInputNum->SetUnitText("ŒÂ");
	pDialogInputNum->SetTitleText("‰¡ŒÂ”");
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
	pDialogInputNum->SetDialogTitle("‰¡•ûŒü•¡ŽÊŠÔŠuŽw’è");
	pDialogInputNum->SetUnitText("mm");
	pDialogInputNum->SetTitleText("‰¡ŠÔŠu");
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
	pDialogInputNum->SetDialogTitle("c•ûŒü•¡ŽÊŒÂ”Žw’è");
	pDialogInputNum->SetUnitText("ŒÂ");
	pDialogInputNum->SetTitleText("cŒÂ”");
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
	pDialogInputNum->SetDialogTitle("c•ûŒü•¡ŽÊŠÔŠuŽw’è");
	pDialogInputNum->SetUnitText("mm");
	pDialogInputNum->SetTitleText("cŠÔŠu");
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
	pDialogInputNum->SetDialogTitle("‰¡•ûŒü•¡ŽÊŒÂ”Žw’è");
	pDialogInputNum->SetUnitText("ŒÂ");
	pDialogInputNum->SetTitleText("‰¡ŒÂ”");
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
	pDialogInputNum->SetDialogTitle("‰¡•ûŒü•¡ŽÊŠÔŠuŽw’è");
	pDialogInputNum->SetUnitText("mm");
	pDialogInputNum->SetTitleText("‰¡ŠÔŠu");
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
	pDialogInputNum->SetDialogTitle("c•ûŒü•¡ŽÊŒÂ”Žw’è");
	pDialogInputNum->SetUnitText("ŒÂ");
	pDialogInputNum->SetTitleText("cŒÂ”");
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
	pDialogInputNum->SetDialogTitle("c•ûŒü•¡ŽÊŠÔŠuŽw’è");
	pDialogInputNum->SetUnitText("mm");
	pDialogInputNum->SetTitleText("cŠÔŠu");
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

DBL CCmdEdit::CntrKindInit(DBL dx, DBL dy)
{
	DBL		cx, cy;

	GetDataCenter(m_dBSX, m_dBSY, m_dBEX, m_dBEY, &cx, &cy);
	m_dBX = cx;
	m_dBY = cy;
	m_nCntrKind = CNTRCNTR;
	m_dCX = cx;
	m_dCY = cy;

	MoveMousePoint(dx, dy, cx, cy);
	return(0.0);
}

void CCmdEdit::CntrKindMove(DBL dx, DBL dy)
{
	DBL		sx, sy;
	DBL		ex, ey;
	DBL		movex, movey;
	DBL		cx, cy;

	cx = m_dBX;
	cy = m_dBY;
	movex = dx - cx;
	movey = dy - cy;
	sx = m_dBSX;
	ex = m_dBEX;
	sy = m_dBSY;
	ey = m_dBEY;
	if(m_pcView->m_nKeyFlags & MK_SHIFT){
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
	int		type;

	m_nComID = 1;
	scrngetmmsize(&xmm, &ymm);
	areasx = 0;
	areasy = 0;
	areaex = xmm;
	areaey = ymm;
	poutoutareaset(areasx, areasy, areaex, areaey);
	pouttypeget(&type);
	ret = outlibOpen();
	if(ret == FAIL){
		m_nComID = -1;
		AfxMessageBox("o—Íæ‚ª‰Šú‰»‚Å‚«‚Ü‚¹‚ñ", MB_OK, 0);
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
	int		type;

	if(m_nComID != -1){
		pouttypeget(&type);
		if(type == CJ2HP || type == CJHP){
			poutchkmove(0.0, 0.0);
		}
		poutend(m_nComID);
		outlibClose();
	}
}

void CCmdEdit::BitmapCreate()
{
	InitDrawArea(m_dSX, m_dSY, m_dEX, m_dEY);
	InitImage();
	m_pcMemDC->BitBlt(0, 0, m_nXSize, m_nYSize, NULL, 0, 0, BLACKNESS);
	scrnsetpreviewpen(m_pcMemDC->m_hDC, 255, 255, 255, 1);
	((CTonView*)m_pcView)->ActiveObjectOnDraw(m_pcMemDC);
	scrnpenend(m_pcMemDC->m_hDC);
	EndDrawArea();
}

void CCmdEdit::BitmapDelete()
{
	EndImage();
}

void CCmdEdit::DrawImage(CDC *pcDC)
{
	DBL		sx, sy, ex, ey;

	sx = m_dSX; sy = m_dSY;
	ex = m_dEX; ey = m_dEY;

	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);

	pcDC->BitBlt((int)sx, (int)ey, m_nXSize, m_nYSize, m_pcMemDC, 0, 0, SRCINVERT);
}

void CCmdEdit::Move2PointMain()
{
	DBL		xlen, ylen, len;
	CDialog2PointSize*	pDialog2PointSize;
	DBL		movex, movey;

	m_pcVect->GetDXY(m_nPP, &m_dSX, &m_dSY);
	m_pcVect2->GetDXY(m_nPP2, &m_dEX, &m_dEY);
	len = hypot(m_dEX - m_dSX, m_dEY - m_dSY);
	xlen = fabs(m_dEX - m_dSX);
	ylen = fabs(m_dEY - m_dSY);
	len = FLOOR100(len);
	xlen = FLOOR100(xlen);
	ylen = FLOOR100(ylen);
	if(len == 0){
		return;
	}
	pDialog2PointSize = new CDialog2PointSize(m_pcView);
	pDialog2PointSize->m_dLength = len;
	pDialog2PointSize->m_dXSize = xlen;
	pDialog2PointSize->m_dYSize = ylen;
	if(pDialog2PointSize->DoModal() == IDOK){
		if(pDialog2PointSize->m_dLength != len){
			movex = (m_dEX - m_dSX) * (pDialog2PointSize->m_dLength - len) / len;
			movey = (m_dEY - m_dSY) * (pDialog2PointSize->m_dLength - len) / len;
		}
		else{
			movex = 0;
			movey = 0;
			if(pDialog2PointSize->m_dXSize != xlen){
				movex = (m_dEX - m_dSX) * (pDialog2PointSize->m_dXSize - xlen) / xlen;
			}
			if(pDialog2PointSize->m_dYSize != ylen){
				movey = (m_dEY - m_dSY) * (pDialog2PointSize->m_dYSize - ylen) / ylen;
			}
		}
		m_pcData->MoveTrns(movex, movey);
		m_pcData->RenewMiniMax();
	}
	delete(pDialog2PointSize);
}

void CCmdEdit::MainMirrCopy()
{
	CTonDoc *pcDoc;
	DBL	sx, sy, ex, ey;
	DBL	a, b, t1, t2;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->OffFlag(COPYFLAG);
	pcDoc->CheckOnFlag(ACTIVEFLAG, COPYFLAG);
	pcDoc->OffFlag(ACTIVEFLAG);
	pcDoc->CopyData(0, 0);
	sx = m_dSX;
	sy = m_dSY;
	ex = m_dEX;
	ey = m_dEY;
	if(sx == ex){
		a = 100000.0;
		b = sx;
	}
	else{
		a = (ey - sy) / (ex - sx);
		b = ey - a*ex;
	}
	t1 = a * a;
	t2 = 2.0 * a;
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->MirrTrns(a, b, t1, t2);
	pcDoc->OffFlag(COPYFLAG);
}
