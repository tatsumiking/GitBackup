
/////////////////////////////////////////////////////////////////////////////
// CCmdLine

#include "stdafx.h"
#include "math.h"

#include "CmdBase.h"
#include "CmdLine.h"
#include "CrossLib.h"

#define	SLCTCMND	1
#define MAINCMND	2

#define	POINT1		1
#define	POINT2		2

#define	CRCLDIVCNT	16

//extern "C" {
	#include "../../ObjLib/Scrnlib.h"
	#include "../../ObjLib/Senvlib.h"
	#include "../../ObjLib/Curvelib.h"
	#include "../../ObjLib/Crosslib.h"
	#include "../../ObjLib/Pdrwlib.h"
	#include "../../ObjLib/Strcttrn.h"
//}

CCmdLine::CCmdLine(CScrollView* pcview)
:CCmdBase(pcview)
{
}

UINT CCmdLine::ExecLineMove(UINT event, DBL dx, DBL dy)
{
	int		retpp;
	int		atr;
	DBL		movex, movey;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
			if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
				SaveUndo();
				m_pcVect->OnFlag(ATNTION1FLAG);
				m_pcVect->GetAtrDXY(retpp, &atr, &dx, &dy);
				m_nPP = retpp;
				m_dSX = dx;
				m_dSY = dy;
				m_dBX = dx;
				m_dBY = dy;
				BeginDraw();
				m_pcVect->PLDraw(m_pcDC);
				m_pcVect->AtntionDraw(m_pcDC);
				EndDraw();
				m_nMouseSetCount = 1;
			}
		}
		else{
			m_pcVect->RenewMiniMax();
			m_pcVect->OffFlag(ATNTION1FLAG);
			m_nMouseSetCount = 0;
			ScrnAllRedraw();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			movex = dx - m_dSX;
			movey = dy - m_dSY;
			BeginDraw();
			m_pcVect->AtntionDraw(m_pcDC);
			m_pcVect->MoveTrns(movex, movey);
			m_pcVect->AtntionDraw(m_pcDC);
			EndDraw();
			m_dSX = dx;
			m_dSY = dy;
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 1){
			movex = m_dBX - m_dSX;
			movey = m_dBY - m_dSY;
			m_pcVect->MoveTrns(movex, movey);
			m_pcVect->RenewMiniMax();	
			m_pcVect->OffFlag(ATNTION1FLAG);
			m_nMouseSetCount = 0;
			ScrnAllRedraw();
		}
		break;
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			movex = m_dBX - m_dSX;
			movey = m_dBY - m_dSY;
			m_pcVect->MoveTrns(movex, movey);
			m_pcVect->RenewMiniMax();	
			EndDraw();
			m_pcVect->OffFlag(ATNTION1FLAG);
			m_nMouseSetCount = 0;
			ScrnAllRedraw();
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdLine::ExecLineDel(UINT event, DBL dx, DBL dy)
{
	int			retpp;
	CTonDoc*	pcDoc;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		break;
	case SETEVENT:
		m_pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
		if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
			SaveUndo();
			m_pcDoc->DeleteData(m_pcVect);
			delete(m_pcVect);
			m_pcVect = NULL;
			ScrnAllRedraw();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdLine::ExecLineLineing(UINT event, DBL dx, DBL dy)
{
	int		tpp;
	DBL		len1, len2;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_pcVect = (CVect*)SrchDataPoint(dx, dy, &tpp);
			if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
				m_nPP = tpp;
				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, -1);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 1;
			}
		}
		else if(m_nMouseSetCount == 1){
			m_pcVect2 = (CVect*)SrchDataPoint(dx, dy, &tpp);
			if(m_pcVect2 != NULL 
			&& m_pcVect2->CheckID() == BASEVECTID
			&& m_pcVect2 == m_pcVect){
				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, -1);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nPP2 = tpp;
				len1 = m_pcVect->GetAreaLength(m_nPP, m_nPP2);
				len2 = m_pcVect->GetAreaLength(m_nPP2, m_nPP);
				if(len1 > len2){
					tpp = m_nPP;
					m_nPP = m_nPP2;
					m_nPP2 = tpp;
				}
				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 2;
			}
		}
		else if(m_nMouseSetCount == 2){
			SaveUndo();
			LineLineingMain();
			m_pcVect->RenewMiniMax();
			m_nMouseSetCount = 0;
			ScrnAllRedraw();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 0){
			// undo
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, -1);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
			m_nMouseSetCount = 0;		
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
			tpp = m_nPP2;
			m_nPP2 = m_nPP;
			m_nPP = tpp;
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
		}
		break;
	case ENDEVENT:
		ScrnAllRedraw();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdLine::ExecLineCurve(UINT event, DBL dx, DBL dy)
{
	DBL		len1, len2;
	int		tpp;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_pcVect = (CVect*)SrchDataPoint(dx, dy, &tpp);
			if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
				m_nPP = tpp;
				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, -1);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 1;
			}
		}
		else if(m_nMouseSetCount == 1){
			m_pcVect2 = (CVect*)SrchDataPoint(dx, dy, &tpp);
			if(m_pcVect2 != NULL 
			&& m_pcVect2->CheckID() == BASEVECTID
			&& m_pcVect == m_pcVect2){
				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, -1);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();

				m_nPP2 = tpp;
				len1 = m_pcVect->GetAreaLength(m_nPP, m_nPP2);
				len2 = m_pcVect->GetAreaLength(m_nPP2, m_nPP);
				if(len1 > len2){
					tpp = m_nPP;
					m_nPP = m_nPP2;
					m_nPP2 = tpp;
				}

				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 2;
			}
		}
		else if(m_nMouseSetCount == 2){
			SaveUndo();
			LineCurveMain();
			m_pcVect->RenewMiniMax();
			m_nMouseSetCount = 0;
			ScrnAllRedraw();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 0){
			// undo
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, -1);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
			m_nMouseSetCount = 0;		
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
			tpp = m_nPP2;
			m_nPP2 = m_nPP;
			m_nPP = tpp;
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
		}
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdLine::ExecLineCut(UINT event, DBL dx, DBL dy)
{
	int		tpp;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_pcVect = (CVect*)SrchDataPoint(dx, dy, &tpp);
			if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
				m_nPP = tpp;
				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, -1);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 1;
			}
		}
		else if(m_nMouseSetCount == 1){
			m_pcVect2 = (CVect*)SrchDataPoint(dx, dy, &tpp);
			if(m_pcVect2 != NULL 
			&& m_pcVect2->CheckID() == BASEVECTID
			&& m_pcVect == m_pcVect2){
				m_nPP2 = tpp;
				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, -1);
				if(m_pcVect->CheckFlag(REJIONFLAG) == OFF){
					if(m_nPP > m_nPP2){
						tpp = m_nPP2;
						m_nPP2 = m_nPP;
						m_nPP = tpp;
					}
				}
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 2;
			}
		}
		else if(m_nMouseSetCount == 2){
			SaveUndo();
			LineCutMain();
			m_pcVect->RenewMiniMax();	
			if(m_pcVect2 != NULL){
				m_pcVect2->PLDraw(m_pcDC);
				m_pcVect2->RenewMiniMax();	
			}
			m_nMouseSetCount = 0;
			ScrnAllRedraw();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 0){
			// undo
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, -1);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
			m_nMouseSetCount = 0;		
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
			tpp = m_nPP2;
			m_nPP2 = m_nPP;
			m_nPP = tpp;
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
		}
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdLine::ExecLineJoin(UINT event, DBL dx, DBL dy)
{
	int		pcnt, tpp;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_pcVect = (CVect*)SrchDataPoint(dx, dy, &tpp);
			if(m_pcVect != NULL 
			&& m_pcVect->CheckID() == BASEVECTID
			&& m_pcVect->CheckFlag(REJIONFLAG) == OFF){
				pcnt = m_pcVect->GetPointCount();
				if(tpp == 0 || tpp == pcnt-1){				
					m_nPP = tpp;
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
			m_pcVect2 = (CVect*)SrchDataPoint(dx, dy, &tpp);
			if(m_pcVect2 != NULL 
			&& m_pcVect2->CheckID() == BASEVECTID
			&& m_pcVect2->CheckFlag(REJIONFLAG) == OFF){
				pcnt = m_pcVect2->GetPointCount();
				if(tpp == 0 || tpp == pcnt-1){				
					SaveUndo();
					m_nPP2 = tpp;
					LineJoinMain();
					m_pcVect->RenewMiniMax();	
					ScrnAllRedraw();
					m_nMouseSetCount = 0;		
				}
			}
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}


UINT CCmdLine::ExecLineCutJoin(UINT event, DBL dx, DBL dy)
{
	DBL		mm;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_nMouseSetCount = 0;
		m_pcDialogInputNum = new CDialogInputNum(m_pcView);
		m_pcDialogInputNum->SetModaless();
		m_pcDialogInputNum->SetDialogTitle(_T("切断接続距離設定"));
		m_pcDialogInputNum->SetUnitText(_T("mm"));
		m_pcDialogInputNum->SetTitleText(_T("間隔"));
		m_pcDialogInputNum->m_dInputNum = 0.0;
		if(m_pcDialogInputNum->Create(IDD_INNUM, m_pcView)){
			m_pcDialogInputNum->SetItemElement();
			m_pcDialogInputNum->ShowWindow(SW_SHOW);
		}
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
			m_dEX = dx;
			m_dEY = dy;
			m_pcDialogInputNum->GetItemElement();
			mm = m_pcDialogInputNum->m_dInputNum;
			LineCutJointMain(mm);
			m_nMouseSetCount = 0;
			ScrnAllRedraw();
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
	case DLGRESETEVENT:
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnLine(m_pcDC);
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

UINT CCmdLine::ExecLineBeze(UINT event, DBL dx, DBL dy)
{
	int		ret;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		SaveUndo();
		m_nMouseSetCount = 0;
		m_nLocalCommand = SLCTCMND;
		SelectCommand(INITEVENT, dx, dy);
		break;
	case SETEVENT:
		switch(m_nLocalCommand){
		case SLCTCMND:
			SaveUndo();
			ret = SelectCommand(event, dx, dy);
			if(ret == SETEVENT){
				m_nLocalCommand = MAINCMND;
				BezeCommand(INITEVENT, dx, dy);
			}
			if(ret == ENDEVENT){
				return(ENDEVENT);
			}
			break;
		case MAINCMND:
			ret = BezeCommand(event, dx, dy);
			if(ret == SETEVENT){
				m_nLocalCommand = SLCTCMND;
				SelectCommand(INITEVENT, dx, dy);
				//return(REDRAWEVENT);
			}
			break;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		switch(m_nLocalCommand){
		case SLCTCMND:
			SelectCommand(event, dx, dy);
			break;
		case MAINCMND:
			BezeCommand(event, dx, dy);
			break;
		}
		break;
	case REDRAWEVENT:
		switch(m_nLocalCommand){
		case SLCTCMND:
			SelectCommand(event, dx, dy);
			break;
		case MAINCMND:
			BezeCommand(event, dx, dy);
			break;
		}
		break;
	case RESETEVENT:
		switch(m_nLocalCommand){
		case SLCTCMND:
			ret = SelectCommand(event, dx, dy);
			if(ret == ENDEVENT){
				return(ENDEVENT);
			}
			break;
		case MAINCMND:
			ret = BezeCommand(event, dx, dy);
			if(ret == SETEVENT){
				m_nLocalCommand = SLCTCMND;
				SelectCommand(INITEVENT, dx, dy);
				//return(REDRAWEVENT);
			}
			break;
		}
		break;
	case ENDEVENT:
		switch(m_nLocalCommand){
		case SLCTCMND:
			SelectCommand(event, dx, dy);
			break;
		case MAINCMND:
			BezeCommand(event, dx, dy);
			break;
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdLine::ExecLineArc(UINT event, DBL dx, DBL dy)
{
	int		ret;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		SaveUndo();
		m_nMouseSetCount = 0;
		m_nLocalCommand = SLCTCMND;
		SelectCommand(INITEVENT, dx, dy);
		break;
	case SETEVENT:
		switch(m_nLocalCommand){
		case SLCTCMND:
			ret = SelectCommand(event, dx, dy);
			if(ret == SETEVENT){
				SaveUndo();
				m_nLocalCommand = MAINCMND;
				ArcCommand(INITEVENT, dx, dy);
			}
			if(ret == ENDEVENT){
				return(ENDEVENT);
			}
			break;
		case MAINCMND:
			ret = ArcCommand(event, dx, dy);
			if(ret == SETEVENT){
				m_nLocalCommand = SLCTCMND;
				SelectCommand(INITEVENT, dx, dy);
				//return(REDRAWEVENT);
			}
			break;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		switch(m_nLocalCommand){
		case SLCTCMND:
			SelectCommand(event, dx, dy);
			break;
		case MAINCMND:
			ArcCommand(event, dx, dy);
			break;
		}
		break;
	case REDRAWEVENT:
		switch(m_nLocalCommand){
		case SLCTCMND:
			SelectCommand(event, dx, dy);
			break;
		case MAINCMND:
			ArcCommand(event, dx, dy);
			break;
		}
		break;
	case RESETEVENT:
		switch(m_nLocalCommand){
		case SLCTCMND:
			ret = SelectCommand(event, dx, dy);
			if(ret == ENDEVENT){
				return(ENDEVENT);
			}
			break;
		case MAINCMND:
			ret = ArcCommand(event, dx, dy);
			if(ret == SETEVENT){
				m_nLocalCommand = SLCTCMND;
				SelectCommand(INITEVENT, dx, dy);
				//return(REDRAWEVENT);
			}
			break;
		}
		break;
	case ENDEVENT:
		switch(m_nLocalCommand){
		case SLCTCMND:
			SelectCommand(event, dx, dy);
			break;
		case MAINCMND:
			ArcCommand(event, dx, dy);
			break;
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdLine::ExecLineHalfArc(UINT event, DBL dx, DBL dy)
{
	int		ret;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_nMouseSetCount = 0;
		m_nLocalCommand = SLCTCMND;
		SelectCommand(INITEVENT, dx, dy);
		break;
	case SETEVENT:
		switch(m_nLocalCommand){
		case SLCTCMND:
			ret = SelectCommand(event, dx, dy);
			if(ret == SETEVENT){
				SaveUndo();
				m_nLocalCommand = MAINCMND;
				HalfArcCommand(INITEVENT, dx, dy);
			}
			if(ret == ENDEVENT){
				return(ENDEVENT);
			}
			break;
		case MAINCMND:
			ret = HalfArcCommand(event, dx, dy);
			if(ret == SETEVENT){
				m_nLocalCommand = SLCTCMND;
				SelectCommand(INITEVENT, dx, dy);
				//return(REDRAWEVENT);
			}
			break;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		switch(m_nLocalCommand){
		case SLCTCMND:
			SelectCommand(event, dx, dy);
			break;
		case MAINCMND:
			HalfArcCommand(event, dx, dy);
			break;
		}
		break;
	case REDRAWEVENT:
		switch(m_nLocalCommand){
		case SLCTCMND:
			SelectCommand(event, dx, dy);
			break;
		case MAINCMND:
			HalfArcCommand(event, dx, dy);
			break;
		}
		break;
	case RESETEVENT:
		switch(m_nLocalCommand){
		case SLCTCMND:
			SelectCommand(event, dx, dy);
			break;
		case MAINCMND:
			HalfArcCommand(event, dx, dy);
			break;
		}
		break;
	case ENDEVENT:
		switch(m_nLocalCommand){
		case SLCTCMND:
			SelectCommand(event, dx, dy);
			break;
		case MAINCMND:
			HalfArcCommand(event, dx, dy);
			break;
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdLine::ExecLineCrcl(UINT event, DBL dx, DBL dy)
{
	int		retpp;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		m_pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
		if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
			SaveUndo();
			BeginDraw();
			m_pcVect->PLDraw(m_pcDC);
			SelectVectCrcling();
			scrnsetfigupen(m_pcDC->m_hDC);
			m_pcVect->PLDraw(m_pcDC);
			m_pcVect->RenewMiniMax();
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
			//return(REDRAWEVENT);
		}
		break;
	case RESETEVENT:
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdLine::ExecLineConerset(UINT event, DBL dx, DBL dy)
{
	int		retpp;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_nMouseSetCount = 0;
		m_pcDialogInputNum = new CDialogInputNum(m_pcView);
		m_pcDialogInputNum->SetModaless();
		m_pcDialogInputNum->SetDialogTitle(_T("角付け範囲半径設定"));
		m_pcDialogInputNum->SetUnitText(_T("mm"));
		m_pcDialogInputNum->SetTitleText(_T("範囲半径"));
		m_pcDialogInputNum->m_dInputNum = 10.0;
		if(m_pcDialogInputNum->Create(IDD_INNUM, m_pcView)){
			m_pcDialogInputNum->SetItemElement();
			m_pcDialogInputNum->ShowWindow(SW_SHOW);
		}
		break;
	case SETEVENT:
		m_pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
		if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
			SaveUndo();
			m_nPP = retpp;
			m_pcDialogInputNum->GetItemElement();
			m_dConerrmm = m_pcDialogInputNum->m_dInputNum;
			ConersetMain();
			m_pcVect->RenewMiniMax();
			ScrnAllRedraw();
		}
		break;
	case DLGRESETEVENT:
	case ENDEVENT:
		m_pcDialogInputNum->DestroyWindow();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdLine::ExecLineConerarc(UINT event, DBL dx, DBL dy)
{
	int		retpp;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_nMouseSetCount = 0;
		m_pcDialogInputNum = new CDialogInputNum(m_pcView);
		m_pcDialogInputNum->SetModaless();
		m_pcDialogInputNum->SetDialogTitle(_T("角Ｒ付けＲ半径設定"));
		m_pcDialogInputNum->SetUnitText(_T("mm"));
		m_pcDialogInputNum->SetTitleText(_T("Ｒ半径"));
		m_pcDialogInputNum->m_dInputNum = 10.0;
		if(m_pcDialogInputNum->Create(IDD_INNUM, m_pcView)){
			m_pcDialogInputNum->SetItemElement();
			m_pcDialogInputNum->ShowWindow(SW_SHOW);
		}
		break;
	case SETEVENT:
		m_pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
		if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
			SaveUndo();
			m_nPP = retpp;
			m_pcDialogInputNum->GetItemElement();
			m_dConerrmm = m_pcDialogInputNum->m_dInputNum;
			ConerarcMain();
			m_pcVect->RenewMiniMax();
			ScrnAllRedraw();
		}
		break;
	case DLGRESETEVENT:
	case ENDEVENT:
		m_pcDialogInputNum->DestroyWindow();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdLine::ExecLineDiv(UINT event, DBL dx, DBL dy)
{
	DBL		retx, rety;
	int		retpp;
	int		cnt;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_nMouseSetCount = 0;
		HlpMsgDsp(_T("分割線を指定してください"));
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_pcVect = (CVect*)SrchDataOnLinePoint(dx, dy, &retx, &rety, &retpp);
			if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
				m_nPP = retpp;
				cnt = ReadLineDivCount();
				if(1 < cnt){
					SaveUndo();
					LineDivMain(cnt);
					m_pcVect->RenewMiniMax();
				}
				ScrnAllRedraw();
			}
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

void CCmdLine::LineLineingMain()
{
	int		pcnt, pp, atr;

	pcnt = m_pcVect->GetPointCount();
	if(m_nPP == m_nPP2){
		for(pp = 0; pp < pcnt; pp++){
			m_pcVect->GetAtr(pp, &atr);
			atr = 0;
			m_pcVect->SetAtr(pp, atr);
		}
	}
	else if(m_nPP < m_nPP2){
		for(pp = m_nPP; pp < m_nPP2; pp++){
			m_pcVect->GetAtr(pp, &atr);
			atr = 0;
			m_pcVect->SetAtr(pp, atr);
		}
	}
	else{ //if(m_nPP > m_nPP2){
		for(pp = m_nPP; pp < pcnt; pp++){
			m_pcVect->GetAtr(pp, &atr);
			atr = 0;
			m_pcVect->SetAtr(pp, atr);
		}
		for(pp = 0; pp < m_nPP2; pp++){
			m_pcVect->GetAtr(pp, &atr);
			atr = 0;
			m_pcVect->SetAtr(pp, atr);
		}
	}
}

void CCmdLine::LineCurveMain()
{
	int		pcnt, pp, atr;

	pcnt = m_pcVect->GetPointCount();
	if(m_nPP == m_nPP2){
		for(pp = 0; pp < pcnt; pp++){
			m_pcVect->GetAtr(pp, &atr);
			atr = CURVATR | CONTATR;
			m_pcVect->SetAtr(pp, atr);
		}
	}
	else if(m_nPP < m_nPP2){
		for(pp = m_nPP; pp < m_nPP2; pp++){
			m_pcVect->GetAtr(pp, &atr);
			if(pp == m_nPP){
				atr |= CURVATR;
			}
			else{
				atr = CURVATR | CONTATR;
			}
			m_pcVect->SetAtr(pp, atr);
		}
	}
	else{ //if(m_nPP > m_nPP2){
		for(pp = m_nPP; pp < pcnt; pp++){
			m_pcVect->GetAtr(pp, &atr);
			if(pp == m_nPP){
				atr |= CURVATR;
			}
			else{
				atr = CURVATR | CONTATR;
			}
			m_pcVect->SetAtr(pp, atr);
		}
		for(pp = 0; pp < m_nPP2; pp++){
			m_pcVect->GetAtr(pp, &atr);
			atr = CURVATR | CONTATR;
			m_pcVect->SetAtr(pp, atr);
		}
	}
}

void CCmdLine::LineCutMain()
{
	CTonDoc*	pcDoc;
	CDataList*	pcDataList;
	int			pcnt, pp, dp;
	int			atr;
	DBL			dx, dy;
	CVect		*tmpVect;

	pcnt = m_pcVect->GetPointCount();
	if(m_pcVect->CheckFlag(REJIONFLAG) == ON){
		tmpVect = new CVect(BASEVECTID);
		if(m_nPP <= m_nPP2){
			dp = 0;
			for(pp = m_nPP2; pp < pcnt; pp++, dp++){
				m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
				tmpVect->SetAtrDXY(dp, atr, dx, dy);
			}
			for(pp = 0; pp <= m_nPP; pp++, dp++){
				m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
				tmpVect->SetAtrDXY(dp, atr, dx, dy);
			}
			pcnt = dp;
			for(pp = 0; pp < pcnt; pp++){
				tmpVect->GetAtrDXY(pp, &atr, &dx, &dy);
				m_pcVect->SetAtrDXY(pp, atr, dx, dy);
			}
			m_pcVect->GetAtr(0, &atr);
			atr &= ~CONTATR;
			m_pcVect->SetAtr(0, atr);
			m_pcVect->SetAtr(pcnt-1, 0);
			m_pcVect->SetPointCount(pcnt);
			m_pcVect->OffFlag(REJIONFLAG);
			m_pcVect2 = NULL;
			return;
		}
		else{	// if(m_nPP > m_nPP2;
			dp = 0;
			for(pp = m_nPP2; pp <= m_nPP; pp++, dp++){
				m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
				tmpVect->SetAtrDXY(dp, atr, dx, dy);
			}
			pcnt = dp;
			for(pp = 0; pp < pcnt; pp++){
				tmpVect->GetAtrDXY(pp, &atr, &dx, &dy);
				m_pcVect->SetAtrDXY(pp, atr, dx, dy);
			}
			m_pcVect->GetAtr(0, &atr);
			atr &= ~CONTATR;
			m_pcVect->SetAtr(0, atr);
			m_pcVect->SetAtr(pcnt-1, 0);
			m_pcVect->SetPointCount(pcnt);
			m_pcVect->OffFlag(REJIONFLAG);
			m_pcVect2 = NULL;
		}
		delete(tmpVect);
	}
	else{	// RejionOff
		pcDoc = (CTonDoc*)(m_pcView->GetDocument());
		pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
		if(pcDataList == NULL){
			return;
		}
		m_pcVect2 = new CVect(BASEVECTID);
		pcDataList->SetData((CData*)m_pcVect2);
		m_pcVect2->OnFlag(ATNTION1FLAG);

		dp = 0;
		for(pp = m_nPP2; pp < pcnt; pp++, dp++){
			m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
			m_pcVect2->SetAtrDXY(dp, atr, dx, dy);
		}
		m_pcVect->GetAtr(0, &atr);
		atr &= ~CONTATR;
		m_pcVect->SetAtr(0, atr);
		m_pcVect->SetAtr(m_nPP, 0);
		m_pcVect->SetPointCount(m_nPP+1);
		m_pcVect->OffFlag(REJIONFLAG);
		m_pcVect2->GetAtr(0, &atr);
		atr &= ~CONTATR;
		m_pcVect2->SetAtr(0, atr);
		m_pcVect2->SetAtr(dp-1, 0);
		m_pcVect2->SetPointCount(dp);
		m_pcVect2->OffFlag(REJIONFLAG);

		pcDoc->DataAddTail((CData*)pcDataList);
	}
}

void CCmdLine::LineJoinMain()
{
	CTonDoc*	pcDoc;
	int			pcnt1, pcnt2;
	int			sp, dp;
	int			atr;
	DBL			dx, dy;

	if(m_pcVect == m_pcVect2){
		m_pcVect->OnFlag(REJIONFLAG);
		return;
	}

	if(m_nPP == 0 && m_nPP2 == 0){
		m_pcVect->ReverceRejion();
	}
	else if(m_nPP != 0 && m_nPP2 != 0){
		m_pcVect2->ReverceRejion();
	}
	else if(m_nPP == 0 && m_nPP2 != 0){
		m_pcVect->ReverceRejion();
		m_pcVect2->ReverceRejion();
	}
	pcnt1 = m_pcVect->GetPointCount();
	pcnt2 = m_pcVect2->GetPointCount();
	for(sp = 0, dp = pcnt1; sp < pcnt2; sp++, dp++){
		m_pcVect2->GetAtrDXY(sp, &atr, &dx, &dy);
		m_pcVect->SetAtrDXY(dp, atr, dx, dy);
	}
	m_pcVect->SetPointCount(dp);

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->DeleteData(m_pcVect2);
	delete(m_pcVect2);
	m_pcVect2 = NULL;
}

void CCmdLine::LineCutJointMain(DBL mm)
{
	CCrossLib	*pcCrossLib;
	dlinetype	ln;
	dcrostype	crs[RECMAX];
	int			crsmax;
	CVect		*pcVect11;
	CVect		*pcVect12;
	CVect		*pcVect21;
	CVect		*pcVect22;
	int			i;

	ln.x1 = m_dSX; ln.y1 = m_dSY;
	ln.x2 = m_dEX; ln.y2 = m_dEY;
	if(mm <= 0.1){
		mm = 0.1;
	}
	SetDataListSelectID(BASEVECTID, ACTIVEFLAG);
	crsmax = 0;
	pcCrossLib = new CCrossLib(m_pcView);
	pcCrossLib->SetDataList(&m_cPtrList);
	pcCrossLib->GetCrossPoint(ln, crs, &crsmax, RECMAX);
	delete(pcCrossLib);
	if(crsmax == 0){
		return;
	}

	for(i = 0; i < crsmax; i++){
		crs[i].len = hypot(m_dSX - crs[i].xx, m_dSY - crs[i].yy);
	}
	SortCrossPoint(crs, crsmax);

	if(crsmax == 1){
		// １つの交点
		m_pcVect2 = NULL;
		CutVect(mm, crs[0]);
		m_pcVect->RenewMiniMax();
		if(m_pcVect2 != NULL){
			CreateNewVect(m_pcVect2);
			m_pcVect2->RenewMiniMax();
		}
	}
	else if(crs[0].pcData == crs[1].pcData){
		// ２つの交点、１つのベクトル
		OneVectCutJoin(mm, crs[0], crs[1]);
		m_pcVect->RenewMiniMax();
		if(m_pcVect2 != NULL){
			CreateNewVect(m_pcVect2);
			m_pcVect2->RenewMiniMax();
		}
	}
	else{
		// ２つの交点、２つのベクトル
		// カット処理
		m_pcVect2 = NULL;
		CutVect(mm, crs[0]);
		pcVect11 = m_pcVect;
		pcVect12 = m_pcVect2;
		m_pcVect->RenewMiniMax();
		if(m_pcVect2 != NULL){
			m_pcVect2->RenewMiniMax();
		}
		m_pcVect2 = NULL;
		CutVect(mm, crs[1]);
		pcVect21 = m_pcVect;
		pcVect22 = m_pcVect2;
		m_pcVect->RenewMiniMax();
		if(m_pcVect2 != NULL){
			m_pcVect2->RenewMiniMax();
		}

		JoinVect(pcVect11, pcVect12, pcVect21, pcVect22);
		m_pcVect->RenewMiniMax();
		if(m_pcVect2 != NULL){
			CreateNewVect(m_pcVect2);
			m_pcVect2->RenewMiniMax();
		}
	}
}

void CCmdLine::SortCrossPoint(dcrostype * crs, int crsmax)
{
	int		i, j;
	dcrostype tcrs;

	for(i = 0; i < crsmax; i++){
		for(j = i; j < crsmax; j++){
			if(crs[i].len > crs[j].len){
				tcrs = crs[i];
				crs[i] = crs[j];
				crs[j] = tcrs;
			}
		}
	}
}

void CCmdLine::CutVect(DBL mm, dcrostype crs)
{
	DBL		xx, yy;
	int		pcnt, pp, p1, p2, dp, np;
	CVect	*tmpVect;
	DBL		len;

	len = mm / 2.0;
	m_pcVect = (CVect*)crs.pcData;
	pp = crs.pp;
	xx = crs.xx;
	yy = crs.yy;

	pcnt = m_pcVect->GetPointCount();
	if(m_pcVect->CheckFlag(REJIONFLAG) == ON){
		tmpVect = new CVect(BASEVECTID);
		dp = 0;
		p1 = CrossPointAfter(len, xx, yy, m_pcVect, pp);
		p2 = CrossPointBefore(len, xx, yy, m_pcVect, pp);
		dp = SetDivFiguAfter(len, xx, yy, m_pcVect, p1, tmpVect, dp);
		np = m_pcVect->NextPointGet(p1);
		dp = AreaCopy(m_pcVect, np, pcnt-1, tmpVect, dp);
		dp = AreaCopy(m_pcVect, 0, p2-1, tmpVect, dp);
		dp = SetDivFiguBefore(len, xx, yy, m_pcVect, p2, tmpVect, dp);

		pcnt = dp;
		dp = 0;
		dp = AreaCopy(tmpVect, 0, pcnt-1, m_pcVect, dp);
		m_pcVect->SetPointCount(pcnt);
		m_pcVect->OffFlag(REJIONFLAG);
		delete(tmpVect);
	}
	else{
		m_pcVect2 = new CVect(BASEVECTID);
		dp = 0;
		p1 = CrossPointAfter(len, xx, yy, m_pcVect, pp);
		p2 = CrossPointBefore(len, xx, yy, m_pcVect, pp);
		dp = SetDivFiguAfter(len, xx, yy, m_pcVect, p1, m_pcVect2, dp);
		np = m_pcVect->NextPointGet(p1);
		dp = AreaCopy(m_pcVect, np, pcnt-1, m_pcVect2, dp);
		m_pcVect2->SetPointCount(dp);
		m_pcVect2->OffFlag(REJIONFLAG);

		tmpVect = new CVect(BASEVECTID);
		dp = 0;
		dp = AreaCopy(m_pcVect, 0, p2-1, tmpVect, dp);
		dp = SetDivFiguBefore(len, xx, yy, m_pcVect, p2, tmpVect, dp);

		pcnt = dp;
		dp = 0;
		dp = AreaCopy(tmpVect, 0, pcnt-1, m_pcVect, dp);
		m_pcVect->SetPointCount(pcnt);
		m_pcVect->OffFlag(REJIONFLAG);
		delete(tmpVect);
	}
}

void CCmdLine::CreateNewVect(CVect * pcVect)
{
	CTonDoc		*pcDoc;
	CDataList	*pcDataList;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	if(pcDataList == NULL){
		return;
	}
	pcDataList->SetData((CData*)pcVect);
	pcDoc->DataAddTail((CData*)pcDataList);
}

void CCmdLine::JoinVect(CVect * pcVect11, CVect * pcVect12, CVect * pcVect21, CVect * pcVect22)
{
	if(pcVect12 == NULL && pcVect22 == NULL){
	// 両方とも閉図形のとき
		JoinCloseClose(pcVect11, pcVect21);
	}
	else if(pcVect12 != NULL && pcVect22 == NULL){
	// 閉図形と開放図形のとき
		JoinOpenClose(pcVect11, pcVect12, pcVect21);
	}
	else if(pcVect12 == NULL && pcVect22 != NULL){
		JoinOpenClose(pcVect21, pcVect22, pcVect11);
	}
	else if(pcVect12 != NULL && pcVect22 != NULL){
		JoinOpenOpen(pcVect11, pcVect12, pcVect21, pcVect22);
	}
}

void CCmdLine::JoinCloseClose(CVect *pcVect11, CVect *pcVect21)
{
	CTonDoc*	pcDoc;
	int		pcnt1, pcnt2;
	int		dp;
	DBL		sx, sy, ex, ey, xx, yy;
	int		k1s, k2s;

	sx = m_dSX; sy = m_dSY;
	ex = m_dEX; ey = m_dEY;
	pcnt1 = pcVect11->GetPointCount();
	pcnt2 = pcVect21->GetPointCount();
	pcVect11->GetDXY(0, &xx, &yy);
	k1s = curvenkindget(sx, sy, ex, ey, xx, yy);
	pcVect21->GetDXY(0, &xx, &yy);
	k2s = curvenkindget(sx, sy, ex, ey, xx, yy);
	if(k1s == k2s){
		pcVect21->ReverceRejion();
	}
	dp = pcnt1;
	dp = AreaCopy(pcVect21, 0, pcnt2-1, pcVect11, dp);
	pcnt1 = dp;
	pcVect11->SetPointCount(pcnt1);
	pcVect11->OnFlag(REJIONFLAG);
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->DeleteData(pcVect21);
	delete(pcVect21);

	m_pcVect = pcVect11;
	m_pcVect2 = NULL;
	return;
}

void CCmdLine::JoinOpenClose(CVect * pcVect11, CVect * pcVect12, CVect * pcVect21)
{
	CTonDoc*	pcDoc;
	int		pcnt1, pcnt2, pcnt3;
	int		dp;
	DBL		sx, sy, ex, ey, xx, yy;
	int		k1e, k2s;

	sx = m_dSX; sy = m_dSY;
	ex = m_dEX; ey = m_dEY;
	pcnt1 = pcVect11->GetPointCount();
	pcnt2 = pcVect21->GetPointCount();
	pcnt3 = pcVect12->GetPointCount();
	pcVect11->GetDXY(pcnt1-1, &xx, &yy);
	k1e = curvenkindget(sx, sy, ex, ey, xx, yy);
	pcVect21->GetDXY(0, &xx, &yy);
	k2s = curvenkindget(sx, sy, ex, ey, xx, yy);
	if(k1e != k2s){
		pcVect21->ReverceRejion();
	}
	dp = pcnt1;
	dp = AreaCopy(pcVect21, 0, pcnt2-1, pcVect11, dp);
	dp = AreaCopy(pcVect12, 0, pcnt3-1, pcVect11, dp);
	pcnt1 = dp;
	pcVect11->SetPointCount(pcnt1);
	pcVect11->OffFlag(REJIONFLAG);

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->DeleteData(pcVect12);
	pcDoc->DeleteData(pcVect21);

	delete(pcVect12);
	delete(pcVect21);

	m_pcVect = pcVect11;
	m_pcVect2 = NULL;
}
void CCmdLine::JoinOpenOpen(CVect *pcVect11, CVect *pcVect12, CVect *pcVect21, CVect *pcVect22)
{
	CTonDoc*	pcDoc;
	int		pcnt11, pcnt22, pcnt21, pcnt12;
	int		dp;
	DBL		sx, sy, ex, ey, xx, yy;
	int		k1e, k2s;

	sx = m_dSX; sy = m_dSY;
	ex = m_dEX; ey = m_dEY;

	pcnt11 = pcVect11->GetPointCount();
	pcnt22 = pcVect22->GetPointCount();
	pcnt21 = pcVect21->GetPointCount();
	pcnt12 = pcVect12->GetPointCount();
	pcVect11->GetDXY(pcnt11-1, &xx, &yy);
	k1e = curvenkindget(sx, sy, ex, ey, xx, yy);
	pcVect22->GetDXY(0, &xx, &yy);
	k2s = curvenkindget(sx, sy, ex, ey, xx, yy);
	if(k1e == k2s){
		dp = pcnt11;
		dp = AreaCopy(pcVect22, 0, pcnt22-1, pcVect11, dp);
		pcnt11 = dp;
		pcVect11->SetPointCount(pcnt11);
		pcVect11->OffFlag(REJIONFLAG);

		dp = pcnt21;
		dp = AreaCopy(pcVect12, 0, pcnt12-1, pcVect21, dp);
		pcnt21 = dp;
		pcVect21->SetPointCount(pcnt21);
		pcVect21->OffFlag(REJIONFLAG);
		pcDoc = (CTonDoc*)(m_pcView->GetDocument());
		pcDoc->DeleteData(pcVect22);
		pcDoc->DeleteData(pcVect12);
		delete(pcVect12);
		delete(pcVect22);
		m_pcVect = pcVect11;
		m_pcVect2 = pcVect21;
	}	
	else {
		dp = pcnt11;
		dp = AreaCopy(pcVect21, 0, pcnt21-1, pcVect11, dp);
		pcnt11 = dp;
		pcVect11->SetPointCount(pcnt11);
		pcVect11->OffFlag(REJIONFLAG);

		dp = pcnt22;
		dp = AreaCopy(pcVect12, 0, pcnt12-1, pcVect22, dp);
		pcnt22 = dp;
		pcVect22->SetPointCount(pcnt22);
		pcVect22->OffFlag(REJIONFLAG);
		pcDoc = (CTonDoc*)(m_pcView->GetDocument());
		pcDoc->DeleteData(pcVect12);
		pcDoc->DeleteData(pcVect21);
		delete(pcVect12);
		delete(pcVect21);
		m_pcVect = pcVect11;
		m_pcVect2 = pcVect22;
	}	

}

void CCmdLine::OneVectCutJoin(DBL mm, dcrostype crs1, dcrostype crs2)
{
	int		pcnt;
	int		dp;
	int		pp1, pp2, tpp;
	DBL		xx1, xx2, txx;
	DBL		yy1, yy2, tyy;
	CVect	*tmpVect;
	int		p11, p12, p21, p22;
	int		np;
	DBL		len;

	len = mm / 2.0;

	xx1 = crs1.xx; yy1 = crs1.yy; pp1 = crs1.pp;
	xx2 = crs2.xx; yy2 = crs2.yy; pp2 = crs2.pp;
	m_pcVect = ((CVect*)crs1.pcData);
	pcnt = m_pcVect->GetPointCount();

	if(pp1 > pp2){
		tpp = pp1; txx = xx1; tyy = yy1;
		pp1 = pp2; xx1 = xx2; yy1 = yy2;
		pp2 = tpp; xx2 = txx; yy2 = tyy;
	}
	p11 = CrossPointBefore(len, xx1, yy1, m_pcVect, pp1);
	p12 = CrossPointAfter(len, xx1, yy1, m_pcVect, pp1);
	p21 = CrossPointBefore(len, xx2, yy2, m_pcVect, pp2);
	p22 = CrossPointAfter(len, xx2, yy2, m_pcVect, pp2);

	m_pcVect2 = new CVect(BASEVECTID);
	dp = 0;
	dp = SetDivFiguAfter(len, xx1, yy1, m_pcVect, p12, m_pcVect2, dp);
	np = m_pcVect->NextPointGet(p12);
	dp = AreaCopy(m_pcVect, np, p21-1, m_pcVect2, dp);
	dp = SetDivFiguBefore(len, xx2, yy2, m_pcVect, p21, m_pcVect2, dp);
	m_pcVect2->SetPointCount(dp);
	m_pcVect2->OnFlag(REJIONFLAG);

	tmpVect = new CVect(BASEVECTID);
	dp = 0;
	dp = SetDivFiguAfter(len, xx2, yy2, m_pcVect, p22, tmpVect, dp);
	np = m_pcVect->NextPointGet(p22);
	dp = AreaCopy(m_pcVect, np, pcnt-1, tmpVect, dp);
	dp = AreaCopy(m_pcVect, 0, p11-1, tmpVect, dp);
	dp = SetDivFiguBefore(len, xx1, yy1, m_pcVect, p11, tmpVect, dp);
	tmpVect->SetPointCount(dp);
	tmpVect->OnFlag(REJIONFLAG);

	pcnt = dp;
	dp = 0;
	dp = AreaCopy(tmpVect, 0, pcnt-1, m_pcVect, dp);
	m_pcVect->SetPointCount(dp);
	m_pcVect->OnFlag(REJIONFLAG);
	delete(tmpVect);
}

UINT CCmdLine::SelectCommand(UINT event, DBL dx, DBL dy)
{
	int		tpp;
	DBL		len1, len2;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		HlpMsgDsp(_T("始点を指定してください"));
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_pcVect = (CVect*)SrchDataPoint(dx, dy, &tpp);
			if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
				m_nPP = tpp;
				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, -1);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 1;
				HlpMsgDsp(_T("終点を指定してください"));
			}
		}
		else if(m_nMouseSetCount == 1){
			m_pcVect2 = (CVect*)SrchDataPoint(dx, dy, &tpp);
			if(m_pcVect2 != NULL 
			&& m_pcVect2->CheckID() == BASEVECTID
			&& m_pcVect == m_pcVect2
			&& m_nPP != tpp){
				m_nPP2 = tpp;
				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, -1);
				if(m_pcVect2->CheckFlag(REJIONFLAG) == ON){
					len1 = m_pcVect->GetAreaLength(m_nPP, m_nPP2);
					len2 = m_pcVect->GetAreaLength(m_nPP2, m_nPP);
					if(len1 > len2){
						tpp = m_nPP;
						m_nPP = m_nPP2;
						m_nPP2 = tpp;
					}
				}
				else{
					if(m_nPP2 < m_nPP){
						tpp = m_nPP;
						m_nPP = m_nPP2;
						m_nPP2 = tpp;
					}
				}
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 2;
				HlpMsgDsp(_T("始点終点を確定してください"));
			}
		}
		else if(m_nMouseSetCount == 2){
			m_nMouseSetCount = 0;
			return(SETEVENT);
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 0){
			return(ENDEVENT);
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, -1);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
			m_nMouseSetCount = 0;		
			HlpMsgDsp(_T("始点を指定してください"));
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
			tpp = m_nPP2;
			m_nPP2 = m_nPP;
			m_nPP = tpp;
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
		}
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdLine::HalfArcCommand(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		HlpMsgDsp(_T("右ボタンで反転、左ボタンで確定"));
		m_nMouseSetCount = 0;
		BeginDraw();
		m_pcVect->PLDraw(m_pcDC);
		HalfArcSelectVect();
		m_pcVect->RenewMiniMax();
		m_pcVect->PLDraw(m_pcDC);
		EndDraw();
		break;
	case SETEVENT:
		return(SETEVENT);
	case RESETEVENT:
		m_nMouseSetCount = 0;
		BeginDraw();
		m_pcVect->PLDraw(m_pcDC);
		HalfArcRevace();
		m_pcVect->RenewMiniMax();
		m_pcVect->PLDraw(m_pcDC);
		EndDraw();
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

void CCmdLine::HalfArcSelectVect()
{
	int			pcnt, pp, dp;
	int			atr;
	DBL			dx, dy;
	CVect		*tmpVect;
	DBL			dr, rag;

	pcnt = m_pcVect->GetPointCount();
	tmpVect = new CVect(BASEVECTID);
	if(m_pcVect->CheckFlag(REJIONFLAG) == ON){
		if(m_nPP <= m_nPP2){
			dp = 0;
			for(pp = m_nPP2; pp < pcnt; pp++, dp++){
				m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
				tmpVect->SetAtrDXY(dp, atr, dx, dy);
			}
			for(pp = 0; pp <= m_nPP; pp++, dp++){
				m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
				tmpVect->SetAtrDXY(dp, atr, dx, dy);
			}
		}
		else{	// if(m_nPP > m_nPP2;
			dp = 0;
			for(pp = m_nPP2; pp <= m_nPP; pp++, dp++){
				m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
				tmpVect->SetAtrDXY(dp, atr, dx, dy);
			}
		}
		m_pcVect->GetDXY(m_nPP, &m_dSX, &m_dSY);
		m_pcVect->GetDXY(m_nPP2, &m_dEX, &m_dEY);

		m_dCX = (m_dSX + m_dEX) / 2.0;
		m_dCY = (m_dSY + m_dEY) / 2.0;
		dr = hypot(m_dCY - m_dSY, m_dCX - m_dSX);
		rag = atan2(m_dCY - m_dSY, m_dCX - m_dSX);
		m_dCX = cos(rag+M_PI_2)*dr + m_dCX;
		m_dCY = sin(rag+M_PI_2)*dr + m_dCY;
		tmpVect->SetAtrDXY(dp-1, CURVATR, m_dSX, m_dSY);
		tmpVect->SetAtrDXY(dp, CURVATR|CONTATR, m_dCX, m_dCY);
		m_nPP = dp;
		dp++;			
		pcnt = dp;
		for(pp = 0; pp < pcnt; pp++){
			tmpVect->GetAtrDXY(pp, &atr, &dx, &dy);
			m_pcVect->SetAtrDXY(pp, atr, dx, dy);
		}
		m_pcVect->SetPointCount(pcnt);
	}
	else{	// RejionOff
		dp = 0;
		for(pp = 0; pp <= m_nPP; pp++, dp++){
			m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
			tmpVect->SetAtrDXY(dp, atr, dx, dy);
		}
		m_pcVect->GetDXY(m_nPP, &m_dSX, &m_dSY);
		m_pcVect->GetDXY(m_nPP2, &m_dEX, &m_dEY);

		m_dCX = (m_dSX + m_dEX) / 2.0;
		m_dCY = (m_dSY + m_dEY) / 2.0;
		dr = hypot(m_dCY - m_dSY, m_dCX - m_dSX);
		rag = atan2(m_dCY - m_dSY, m_dCX - m_dSX);
		m_dCX = cos(rag+M_PI_2)*dr + m_dCX;
		m_dCY = sin(rag+M_PI_2)*dr + m_dCY;
		tmpVect->SetAtrDXY(dp-1, CURVATR, m_dSX, m_dSY);
		tmpVect->SetAtrDXY(dp, CURVATR|CONTATR, m_dCX, m_dCY);
		m_nPP = dp;
		dp++;
		for(pp = m_nPP2; pp < pcnt; pp++, dp++){
			m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
			tmpVect->SetAtrDXY(dp, atr, dx, dy);
		}
	
		pcnt = dp;
		for(pp = 0; pp < pcnt; pp++){
			tmpVect->GetAtrDXY(pp, &atr, &dx, &dy);
			m_pcVect->SetAtrDXY(pp, atr, dx, dy);
		}
		m_pcVect->SetPointCount(pcnt);
	}
	delete(tmpVect);
}

void CCmdLine::HalfArcRevace()
{
	DBL	sx, sy, ex, ey;
	DBL	a, b, t1, t2;

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
	curvesymxytrns(a, b, t1, t2, &m_dCX, &m_dCY);
	m_pcVect->SetDXY(m_nPP, m_dCX, m_dCY);
}

UINT CCmdLine::ArcCommand(UINT event, DBL dx, DBL dy)
{
	int		pcnt;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		HlpMsgDsp(_T("中点を指定してください、右ボタンで確定です"));
		m_pcCopyVect = (CVect*)m_pcVect->CopyData(0.0, 0.0);
		ArcSelectVect();
		BeginDraw();
		DrawSelectArc(m_pcDC);
		EndDraw();
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			BeginDraw();
			DrawSelectArc(m_pcDC);
			ArcSelectPoint(dx, dy);
			DrawSelectArc(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
			HlpMsgDsp(_T("中点を確定してください、右ボタンで確定です"));
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawSelectArc(m_pcDC);
			ArcMovePoint(dx, dy);
			DrawSelectArc(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
			HlpMsgDsp(_T("中点を指定してください、右ボタンで確定です"));
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawSelectArc(m_pcDC);
			ArcMovePoint(dx, dy);
			DrawSelectArc(m_pcDC);
			EndDraw();
		}
		break;
	case RESETEVENT:
		BeginDraw();
		DrawSelectArc(m_pcDC);
		m_pcData = m_pcDoc->UseFitingVect(m_pcVect);
		if(m_pcData != NULL){
			DrawModeSET();
			m_pcData->DelDraw(m_pcDC);
			DrawModeXOR();
			m_pcData->StrgFiting();
			m_pcData->RenewMiniMax();
			ScrnDataRedraw(m_pcData);
		}
		m_pcCopyVect->PLDraw(m_pcDC);
		m_pcVect->PLDraw(m_pcDC);
		m_pcVect->RenewMiniMax();
		EndDraw();
		delete(m_pcCopyVect);
		m_pcCopyVect = NULL;
		return(SETEVENT);
	case ENDEVENT:
		if(m_pcCopyVect != NULL){
			pcnt = m_pcCopyVect->GetPointCount();
			AreaCopy(m_pcCopyVect, 0, pcnt-1, m_pcVect, 0);	
			m_pcVect->RenewMiniMax();
			delete(m_pcCopyVect);
			m_pcCopyVect = NULL;
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdLine::BezeCommand(UINT event, DBL dx, DBL dy)
{
	int		pcnt;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		m_pcCopyVect = (CVect*)m_pcVect->CopyData(0.0, 0.0);
		BezeSelectVect();
		BeginDraw();
		DrawSelectBeze(m_pcDC);
		EndDraw();
		HlpMsgDsp(_T("制御点を指定してください、右ボタンで確定です"));
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			BeginDraw();
			DrawSelectBeze(m_pcDC);
			BezeSelectPoint(dx, dy);
			DrawSelectBeze(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
			HlpMsgDsp(_T("制御点を確定してください、右ボタンで確定です"));
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawSelectBeze(m_pcDC);
			BezeMovePoint(dx, dy);
			DrawSelectBeze(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawSelectBeze(m_pcDC);
			BezeMovePoint(dx, dy);
			DrawSelectBeze(m_pcDC);
			EndDraw();
		}
		break;
	case RESETEVENT:
		BeginDraw();
		DrawSelectBeze(m_pcDC);
		m_pcVect->RenewMiniMax();
		m_pcData = m_pcDoc->UseFitingVect(m_pcVect);
		if(m_pcData != NULL){
			DrawModeSET();
			m_pcData->DelDraw(m_pcDC);
			DrawModeXOR();
			m_pcData->StrgFiting();
			m_pcData->RenewMiniMax();
			ScrnDataRedraw(m_pcData);
		}
		m_pcCopyVect->PLDraw(m_pcDC);
		m_pcVect->PLDraw(m_pcDC);
		EndDraw();
		delete(m_pcCopyVect);
		m_pcCopyVect = NULL;
		return(SETEVENT);
	case ENDEVENT:
		if(m_pcCopyVect != NULL){
			pcnt = m_pcCopyVect->GetPointCount();
			AreaCopy(m_pcCopyVect, 0, pcnt-1, m_pcVect, 0);	
			m_pcVect->RenewMiniMax();
			delete(m_pcCopyVect);
			m_pcCopyVect = NULL;
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);

}

void CCmdLine::ArcSelectVect()
{
	int			pcnt, pp, dp;
	int			atr;
	DBL			dx, dy;
	CVect		*tmpVect;
	int		p0, p1, p2, p3, p4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;

	pcnt = m_pcVect->GetPointCount();
	if(m_pcVect->CheckFlag(REJIONFLAG) == ON){
		tmpVect = new CVect(BASEVECTID);
		if(m_nPP <= m_nPP2){
			dp = 0;
			for(pp = m_nPP2; pp < pcnt; pp++, dp++){
				m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
				tmpVect->SetAtrDXY(dp, atr, dx, dy);
			}
			for(pp = 0; pp <= m_nPP; pp++, dp++){
				m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
				tmpVect->SetAtrDXY(dp, atr, dx, dy);
			}
		}
		else{	// if(m_nPP > m_nPP2;
			dp = 0;
			for(pp = m_nPP2; pp <= m_nPP; pp++, dp++){
				m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
				tmpVect->SetAtrDXY(dp, atr, dx, dy);
			}
		}
		m_pcVect->GetDXY(m_nPP, &m_dSX, &m_dSY);
		m_pcVect->GetDXY(m_nPP2, &m_dEX, &m_dEY);

		m_dCX = (m_dSX + m_dEX) / 2.0;
		m_dCY = (m_dSY + m_dEY) / 2.0;
		tmpVect->SetAtrDXY(dp-1, CURVATR, m_dSX, m_dSY);
		tmpVect->SetAtrDXY(dp, CURVATR|CONTATR, m_dCX, m_dCY);
		m_nPP = dp;
		dp++;			
		pcnt = dp;
		for(pp = 0; pp < pcnt; pp++){
			tmpVect->GetAtrDXY(pp, &atr, &dx, &dy);
			m_pcVect->SetAtrDXY(pp, atr, dx, dy);
		}
		m_pcVect->SetPointCount(pcnt);
		delete(tmpVect);
	}
	else{	// RejionOff
		tmpVect = new CVect(BASEVECTID);
		dp = 0;
		for(pp = 0; pp <= m_nPP; pp++, dp++){
			m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
			tmpVect->SetAtrDXY(dp, atr, dx, dy);
		}
		m_pcVect->GetDXY(m_nPP, &m_dSX, &m_dSY);
		m_pcVect->GetDXY(m_nPP2, &m_dEX, &m_dEY);

		m_dCX = (m_dSX + m_dEX) / 2.0;
		m_dCY = (m_dSY + m_dEY) / 2.0;
		tmpVect->SetAtrDXY(dp-1, CURVATR, m_dSX, m_dSY);
		tmpVect->SetAtrDXY(dp, CURVATR|CONTATR, m_dCX, m_dCY);
		m_nPP = dp;
		dp++;
		for(pp = m_nPP2; pp < pcnt; pp++, dp++){
			m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
			tmpVect->SetAtrDXY(dp, atr, dx, dy);
		}
	
		pcnt = dp;
		for(pp = 0; pp < pcnt; pp++){
			tmpVect->GetAtrDXY(pp, &atr, &dx, &dy);
			m_pcVect->SetAtrDXY(pp, atr, dx, dy);
		}
		m_pcVect->SetPointCount(pcnt);
		delete(tmpVect);
	}
	p2 = m_nPP;
	p0 = m_pcVect->PointLegal(pcnt, p2-2);
	p1 = m_pcVect->PointLegal(pcnt, p2-1);
	p3 = m_pcVect->PointLegal(pcnt, p2+1);
	p4 = m_pcVect->PointLegal(pcnt, p2+2);
	m_pcVect->GetDXY(p0, &x0, &y0);
	m_pcVect->GetDXY(p1, &x1, &y1);
	m_pcVect->GetDXY(p2, &x2, &y2);
	m_pcVect->GetDXY(p3, &x3, &y3);
	m_pcVect->GetDXY(p4, &x4, &y4);

	m_dX1 = x1;
	m_dY1 = y1;
	m_dX2 = x2;
	m_dY2 = y2;
	m_dX3 = x3;
	m_dY3 = y3;
}

void CCmdLine::BezeSelectVect()
{
	int			pcnt, pp, dp;
	int			atr;
	DBL			dx, dy;
	CVect		*tmpVect;
	int		p0, p1, p2, p3, p4, p5;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;
	DBL		len, rag01, rag54;

	pcnt = m_pcVect->GetPointCount();
	if(m_pcVect->CheckFlag(REJIONFLAG) == ON){
		tmpVect = new CVect(BASEVECTID);
		if(m_nPP <= m_nPP2){
			dp = 0;
			for(pp = m_nPP2; pp < pcnt; pp++, dp++){
				m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
				tmpVect->SetAtrDXY(dp, atr, dx, dy);
			}
			for(pp = 0; pp <= m_nPP; pp++, dp++){
				m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
				tmpVect->SetAtrDXY(dp, atr, dx, dy);
			}
		}
		else{	// if(m_nPP > m_nPP2;
			dp = 0;
			for(pp = m_nPP2; pp <= m_nPP; pp++, dp++){
				m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
				tmpVect->SetAtrDXY(dp, atr, dx, dy);
			}
		}
		m_pcVect->GetDXY(m_nPP, &m_dSX, &m_dSY);
		m_pcVect->GetDXY(m_nPP2, &m_dEX, &m_dEY);

		x1 = (m_dEX - m_dSX) / 3.0 + m_dSX;
		y1 = (m_dEY - m_dSY) / 3.0 + m_dSY;
		x2 = (m_dSX - m_dEX) / 3.0 + m_dEX;
		y2 = (m_dSY - m_dEY) / 3.0 + m_dEY;
		tmpVect->SetAtrDXY(dp-1, CURVATR|BEZEATR, m_dSX, m_dSY);
		tmpVect->SetAtrDXY(dp, BEZEATR, x1, y1);
		p1 = dp;
		dp++;
		tmpVect->SetAtrDXY(dp, BEZEATR, x2, y2);
		p2 = dp;
		dp++;

		m_nPP = p1;
		m_nPP2 = p2;

		pcnt = dp;
		for(pp = 0; pp < pcnt; pp++){
			tmpVect->GetAtrDXY(pp, &atr, &dx, &dy);
			m_pcVect->SetAtrDXY(pp, atr, dx, dy);
		}
		m_pcVect->SetPointCount(pcnt);
		delete(tmpVect);
	}
	else{	// RejionOff
		tmpVect = new CVect(BASEVECTID);
		dp = 0;
		for(pp = 0; pp <= m_nPP; pp++, dp++){
			m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
			tmpVect->SetAtrDXY(dp, atr, dx, dy);
		}
		m_pcVect->GetDXY(m_nPP, &m_dSX, &m_dSY);
		m_pcVect->GetDXY(m_nPP2, &m_dEX, &m_dEY);

		x1 = (m_dEX - m_dSX) / 3.0 + m_dSX;
		y1 = (m_dEY - m_dSY) / 3.0 + m_dSY;
		x2 = (m_dSX - m_dEX) / 3.0 + m_dEX;
		y2 = (m_dSY - m_dEY) / 3.0 + m_dEY;
		tmpVect->SetAtrDXY(dp-1, CURVATR|BEZEATR, m_dSX, m_dSY);
		tmpVect->SetAtrDXY(dp, BEZEATR, x1, y1);
		p1 = dp;
		dp++;
		tmpVect->SetAtrDXY(dp, BEZEATR, x2, y2);
		p2 = dp;
		dp++;

		for(pp = m_nPP2; pp < pcnt; pp++, dp++){
			m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
			tmpVect->SetAtrDXY(dp, atr, dx, dy);
		}
	
		m_nPP = p1;
		m_nPP2 = p2;

		pcnt = dp;
		for(pp = 0; pp < pcnt; pp++){
			tmpVect->GetAtrDXY(pp, &atr, &dx, &dy);
			m_pcVect->SetAtrDXY(pp, atr, dx, dy);
		}
		m_pcVect->SetPointCount(pcnt);
		delete(tmpVect);
	}

	p2 = m_nPP;
	p3 = m_nPP2;
	p0 = m_pcVect->PointLegal(pcnt, p2-2);
	p1 = m_pcVect->PointLegal(pcnt, p2-1);
	p4 = m_pcVect->PointLegal(pcnt, p3+1);
	p5 = m_pcVect->PointLegal(pcnt, p3+2);
	m_pcVect->GetDXY(p0, &x0, &y0);
	m_pcVect->GetDXY(p1, &x1, &y1);
	m_pcVect->GetDXY(p2, &x2, &y2);
	m_pcVect->GetDXY(p3, &x3, &y3);
	m_pcVect->GetDXY(p4, &x4, &y4);
	m_pcVect->GetDXY(p5, &x5, &y5);

	len = hypot(y4-y1, x4-x1)/3;

	if(x0 == x1 && y0 == y1){
		rag01 = 0;
	}
	else{
		rag01 = atan2(y1 - y0, x1 - x0);
	}
	if(x5 == x4 && y5 == y4){
		rag54 = 0;
	}
	else{
		rag54 = atan2(y4 - y5, x4 - x5);
	}
	x2 = cos(rag01) * len + x1;
	y2 = sin(rag01) * len + y1;
	m_pcVect->SetDXY(p2, x2, y2);
	x3 = cos(rag54) * len + x4;
	y3 = sin(rag54) * len + y4;
	m_pcVect->SetDXY(p3, x3, y3);
	m_dX1 = x1;
	m_dY1 = y1;
	m_dX2 = x2;
	m_dY2 = y2;
	m_dX3 = x3;
	m_dY3 = y3;
	m_dX4 = x4;
	m_dY4 = y4;
}

void CCmdLine::DrawSelectArc(CDC *pcDC)
{
	DBL	x1, y1, x2, y2, x3, y3;

	x1 = m_dX1;
	y1 = m_dY1;
	x2 = m_dX2;
	y2 = m_dY2;
	x3 = m_dX3;
	y3 = m_dY3;
	scrndxymmtorltv(&x1, &y1);
	scrndxymmtorltv(&x2, &y2);
	scrndxymmtorltv(&x3, &y3);
	scrnsetpointpen(pcDC->m_hDC);
	pdrwmove(pcDC->m_hDC, x1, y1);
	pdrwarc(pcDC->m_hDC, x1, y1, x2, y2, x3, y3);
	pdrwpoint(pcDC->m_hDC, x1, y1);
	pdrwpoint(pcDC->m_hDC, x2, y2);
	pdrwpoint(pcDC->m_hDC, x3, y3);
	scrnpenend(pcDC->m_hDC);
}

void CCmdLine::DrawSelectBeze(CDC *pcDC)
{
	DBL	x1, y1, x2, y2, x3, y3, x4, y4;

	x1 = m_dX1;
	y1 = m_dY1;
	x2 = m_dX2;
	y2 = m_dY2;
	x3 = m_dX3;
	y3 = m_dY3;
	x4 = m_dX4;
	y4 = m_dY4;
	scrndxymmtorltv(&x1, &y1);
	scrndxymmtorltv(&x2, &y2);
	scrndxymmtorltv(&x3, &y3);
	scrndxymmtorltv(&x4, &y4);
	scrnsetpointpen(pcDC->m_hDC);
	pdrwmove(pcDC->m_hDC, x1, y1);
	pdrwbezecntrl(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	pdrwbeze(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	scrnpenend(pcDC->m_hDC);
}

void CCmdLine::ArcMovePoint(DBL dx, DBL dy)
{
	m_dX2 = dx;
	m_dY2 = dy;
	m_pcVect->SetDXY(m_nPP, dx, dy);
}

void CCmdLine::BezeMovePoint(DBL dx, DBL dy)
{
	switch(m_nSelectPoint){
	case POINT1:
		m_dX2 = dx;
		m_dY2 = dy;
		m_pcVect->SetDXY(m_nPP, dx, dy);
		break;
	case POINT2:
		m_dX3 = dx;
		m_dY3 = dy;
		m_pcVect->SetDXY(m_nPP2, dx, dy);
		break;
	}
}

void CCmdLine::BezeSelectPoint(DBL dx, DBL dy)
{
	DBL		len1, len2;

	len1 = hypot(dx - m_dX2, dy - m_dY2);
	len2 = hypot(dx - m_dX3, dy - m_dY3);
	if(len1 < len2){
		m_nSelectPoint = POINT1;
	}
	else{
		m_nSelectPoint = POINT2;
	}
}

void CCmdLine::ArcSelectPoint(DBL dx, DBL dy)
{

}

void CCmdLine::SelectVectCrcling()
{
	int	pcnt, pp;
	DBL	minix, miniy, maxx, maxy;
	DBL	x0, y0;
	DBL	sumlen, dp;
	int	atr;
	DBL	dx, dy;
	DBL	r, divrag, rag;

	m_pcVect->GetDMiniMax(&minix, &miniy, &maxx, &maxy);
	x0 = (maxx + minix) / 2.0;
	y0 = (maxy + miniy) / 2.0;
	sumlen = 0;
	dp = 0;
	pcnt = m_pcVect->GetPointCount();
	for(pp = 0; pp < pcnt; pp++){
		m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
		if(atr != BEZEATR){
			dp += 1;
			sumlen += hypot(dx-x0, dy-y0);
		}
	}
	r = sumlen / dp;
	divrag = M_PI*2 / CRCLDIVCNT;
	rag = 0;
	for(pp = 0; pp < CRCLDIVCNT; pp++){
		rag = divrag * (DBL)pp;
		dx = cos(rag) * r + x0;
		dy = sin(rag) * r + y0;
		m_pcVect->SetAtrDXY(pp, CURVATR | CONTATR, dx, dy);
	}
	m_pcVect->SetPointCount(CRCLDIVCNT);
}

void CCmdLine::ConersetMain()
{
	int		pcnt;
	int		p1, p2, p3, p4;
	DBL		x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		x0, y0, dx, dy;
	int		dp, sp, atr;
	CVect*	tmpVect;

	if(GetConerAreaOutPoint(m_dConerrmm, &p1, &p4) == FAIL){
		return;
	}
	pcnt = m_pcVect->GetPointCount();
	p2 = m_pcVect->PointLegal(pcnt, p1+1);
	p3 = m_pcVect->PointLegal(pcnt, p4-1);
	m_pcVect->GetDXY(p1, &x1, &y1);
	m_pcVect->GetDXY(p2, &x2, &y2);
	m_pcVect->GetDXY(p3, &x3, &y3);
	m_pcVect->GetDXY(p4, &x4, &y4);
	if(strclinecrossget(x1, y1, x2, y2, x3, y3, x4, y4, &x0, &y0) == FAIL){
		return;
	}

	tmpVect = new CVect(BASEVECTID);
	if(p1 < p4){
		dp = 0;
		for(sp = 0; sp <= p1; sp++){
			m_pcVect->GetAtrDXY(sp, &atr, &dx, &dy);
			tmpVect->SetAtrDXY(dp, atr, dx, dy);
			dp++;
		}
		atr = 0;
		tmpVect->SetAtrDXY(dp, atr, x0, y0);
		dp++;
		for(sp = p4; sp < pcnt; sp++){
			m_pcVect->GetAtrDXY(sp, &atr, &dx, &dy);
			tmpVect->SetAtrDXY(dp, atr, dx, dy);
			dp++;
		}
	}
	else{
		dp = 0;
		atr = 0;
		tmpVect->SetAtrDXY(dp, atr, x0, y0);
		dp++;
		for(sp = p4; sp <= p1; sp++){
			m_pcVect->GetAtrDXY(sp, &atr, &dx, &dy);
			tmpVect->SetAtrDXY(dp, atr, dx, dy);
			dp++;
		}
	}
	for(sp = 0; sp < dp; sp++){
		tmpVect->GetAtrDXY(sp, &atr, &dx, &dy);
		m_pcVect->SetAtrDXY(sp, atr, dx, dy);
	}
	m_pcVect->SetPointCount(dp);
	delete(tmpVect);
}

void CCmdLine::ConerarcMain()
{
	int		pcnt;
	int		p1, p2, p3, p4;
	DBL		x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		dx, dy;
	int		dp, sp, atr;
	CVect*	tmpVect;

	if(GetConerAreaOutPoint(m_dConerrmm, &p1, &p4) == FAIL){
		return;
	}
	pcnt = m_pcVect->GetPointCount();
	p2 = m_pcVect->PointLegal(pcnt, p1+1);
	p3 = m_pcVect->PointLegal(pcnt, p4-1);
	m_pcVect->GetDXY(p1, &x1, &y1);
	m_pcVect->GetDXY(p2, &x2, &y2);
	m_pcVect->GetDXY(p3, &x3, &y3);
	m_pcVect->GetDXY(p4, &x4, &y4);
	if(strcconrarc(&x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4, m_dConerrmm) == FAIL){
		return;
	}

	tmpVect = new CVect(BASEVECTID);
	if(p1 < p4){
		dp = 0;
		for(sp = 0; sp <= p1; sp++){
			m_pcVect->GetAtrDXY(sp, &atr, &dx, &dy);
			tmpVect->SetAtrDXY(dp, atr, dx, dy);
			dp++;
		}
		atr = CURVATR;
		tmpVect->SetAtrDXY(dp, atr, x1, y1);
		dp++;
		atr = CURVATR|CONTATR;
		tmpVect->SetAtrDXY(dp, atr, x3, y3);
		dp++;
		sp = m_pcVect->PointLegal(pcnt, p4-1);
		m_pcVect->GetAtrDXY(sp, &atr, &dx, &dy);
		atr &= ~CONTATR;
		tmpVect->SetAtrDXY(dp, atr, x2, y2);
		dp++;
		for(sp = p4; sp < pcnt; sp++){
			m_pcVect->GetAtrDXY(sp, &atr, &dx, &dy);
			tmpVect->SetAtrDXY(dp, atr, dx, dy);
			dp++;
		}
	}
	else{
		dp = 0;
		atr = CURVATR;
		tmpVect->SetAtrDXY(dp, atr, x1, y1);
		dp++;
		atr = CURVATR|CONTATR;
		tmpVect->SetAtrDXY(dp, atr, x3, y3);
		dp++;
		sp = m_pcVect->PointLegal(pcnt, p4-1);
		m_pcVect->GetAtrDXY(sp, &atr, &dx, &dy);
		atr &= ~CONTATR;
		tmpVect->SetAtrDXY(dp, atr, x2, y2);
		dp++;
		for(sp = p4; sp <= p1; sp++){
			m_pcVect->GetAtrDXY(sp, &atr, &dx, &dy);
			tmpVect->SetAtrDXY(dp, atr, dx, dy);
			dp++;
		}
	}
	for(sp = 0; sp < dp; sp++){
		tmpVect->GetAtrDXY(sp, &atr, &dx, &dy);
		m_pcVect->SetAtrDXY(sp, atr, dx, dy);
	}
	m_pcVect->SetPointCount(dp);
	delete(tmpVect);
}

int CCmdLine::GetConerAreaOutPoint(DBL dotr, int *retpp1, int *retpp2)
{
	int		pcnt, pp1, pp2;
	DBL		len;
	DBL		x1, y1, x2, y2;

	if(dotr < 0){
		dotr *= -1;
	}

	m_pcVect->GetDXY(m_nPP, &x1, &y1);
	pcnt = m_pcVect->GetPointCount();
	pp1 = m_nPP;
	while(1){
		pp1 = m_pcVect->PointLegal(pcnt, pp1-1);
		if(pp1 == m_nPP){
			return(FAIL);
		}
		m_pcVect->GetDXY(pp1, &x2, &y2);
		len = hypot(x1 - x2, y1 - y2);
		if(dotr < len){
			break;
		}
	}
	pp2 = m_nPP;
	while(1){
		pp2 = m_pcVect->PointLegal(pcnt, pp2+1);
		if(pp2 == m_nPP){
			return(FAIL);
		}
		m_pcVect->GetDXY(pp2, &x2, &y2);
		len = hypot(x1 - x2, y1 - y2);
		if(dotr < len){
			break;
		}
	}
	*retpp1 = pp1;
	*retpp2 = pp2;
	return(TRUE);
}

int CCmdLine::ReadLineDivCount()
{
	CDialogInputNum	*pDialogInputNum;
	int			ndivcnt;

	ndivcnt = 3;
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("線分割個数設定"));
	pDialogInputNum->SetUnitText(_T("個"));
	pDialogInputNum->SetTitleText(_T("分割数"));
	pDialogInputNum->m_dInputNum = ndivcnt;
	if(pDialogInputNum->DoModal() == IDOK){
		ndivcnt = (int)(pDialogInputNum->m_dInputNum);
	}
	else{
		ndivcnt = 0;
	}
	delete(pDialogInputNum);
	return(ndivcnt);
}

void CCmdLine::LineDivMain(int ndivcnt)
{
	CVect	*pctmpVect;
	int		pcnt, sp, dp;
	int		atr;
	DBL		dx, dy;
	int		p1, p2;
	DBL		x1, y1, x2, y2;
	int		cnt;
	DBL		tt, divtt;

	pctmpVect = new CVect(BASEVECTID);

	dp = 0;
	pcnt = m_pcVect->GetPointCount();
	for(sp = 0; sp < m_nPP; sp++){
		m_pcVect->GetAtrDXY(sp, &atr, &dx, &dy);
		pctmpVect->SetAtrDXY(dp, atr, dx, dy);
		dp++;
	}
	sp = m_pcVect->NextPointGet(m_nPP);
	p1 = m_pcVect->PointLegal(pcnt, m_nPP);
	p2 = m_pcVect->PointLegal(pcnt, sp);
	m_pcVect->GetAtrDXY(p1, &atr, &x1, &y1);
	m_pcVect->GetAtrDXY(p2, &atr, &x2, &y2);
	divtt = 1.0 / (DBL)ndivcnt;
	for(cnt = 0; cnt < ndivcnt; cnt++){
		tt = divtt * (DBL)cnt;
		dx = (x2 - x1) * tt + x1;
		dy = (y2 - y1) * tt + y1;
		// 曲線を分割するときはこの部分を修正
		pctmpVect->SetAtrDXY(dp, 0, dx, dy);
		dp++;
	}
	for(; sp < pcnt; sp++){
		m_pcVect->GetAtrDXY(sp, &atr, &dx, &dy);
		pctmpVect->SetAtrDXY(dp, atr, dx, dy);
		dp++;
	}

	pcnt = dp;
	for(sp = 0; sp < pcnt; sp++){
		pctmpVect->GetAtrDXY(sp, &atr, &dx, &dy);
		m_pcVect->SetAtrDXY(sp, atr, dx, dy);
	}
	m_pcVect->SetPointCount(pcnt);

	delete(pctmpVect);
}

int CCmdLine::SetDivFiguAfter(DBL len, DBL xx, DBL yy, CVect *srcVect, int pp, CVect *dstVect, int dp)
{
	int		kind;
	int		pcnt, p1, p2, p3, p4;
	DBL		len1, len2;
	dbezetype	bz, bz1, bz2;
	darctype	arc;
	DBL		x1, y1, x2, y2;
	DBL		xt, yt;
	DBL		t, t1, b1, b2, b3, b4;

	pcnt = srcVect->GetPointCount();
	kind = srcVect->GetFiguKind(pp);
	p1 = srcVect->PointLegal(pcnt, pp+0);
	p2 = srcVect->PointLegal(pcnt, pp+1);
	p3 = srcVect->PointLegal(pcnt, pp+2);
	p4 = srcVect->PointLegal(pcnt, pp+3);

	if(kind == LINEKIND){
		srcVect->GetDXY(p1, &x1, &y1);
		srcVect->GetDXY(p2, &x2, &y2);
		len1 = hypot(xx-x2, yy-y2);
		xt = (x2 - xx) * len / len1 + xx;
		yt = (y2 - yy) * len / len1 + yy;
		dstVect->SetAtrDXY(dp++, 0, xt, yt);
	}
	else{
		switch(kind){
		case BEZEKIND:
			srcVect->GetDXY(p1, &(bz.x1), &(bz.y1));
			srcVect->GetDXY(p2, &(bz.x2), &(bz.y2));
			srcVect->GetDXY(p3, &(bz.x3), &(bz.y3));
			srcVect->GetDXY(p4, &(bz.x4), &(bz.y4));
			break;
		case FCBEZEKIND:
			srcVect->GetFreeCurveBeze(p1, &bz);
			break;
		case ARCKIND:
			srcVect->GetDXY(p1, &(arc.x1), &(arc.y1));
			srcVect->GetDXY(p2, &(arc.x2), &(arc.y2));
			srcVect->GetDXY(p3, &(arc.x3), &(arc.y3));
			cruvearctobeze(arc, &bz);
			break;
		}
		len2 = INTMAX;
		for(t = 1.0; t >= 0; t -= 0.002){
			t1 = 1 - t;
			b1 = t1 * t1 * t1;
			b2 = 3 * t * t1 * t1;
			b3 = 3 * t * t * t1;
			b4 = t * t * t;
			xt = (b1 * bz.x1 + b2 * bz.x2 + b3 * bz.x3 + b4 * bz.x4);
			yt = (b1 * bz.y1 + b2 * bz.y2 + b3 * bz.y3 + b4 * bz.y4);
			len1 = hypot(xx-xt, yy-yt);
			if(len1 <= len){
				break;
			}
			if(len2 < len1){
				break;
			}
			len2 = len1;
		}
		if(t > 0.0){
			crossbezettdiv(&bz, t, &bz1, &bz2);
		}
		else{
			crossbezediv(&bz, xx, yy, &bz1, &bz2);
		}
		dstVect->SetAtrDXY(dp++, CURVATR|BEZEATR, bz2.x1, bz2.y1);
		dstVect->SetAtrDXY(dp++, BEZEATR, bz2.x2, bz2.y2);
		dstVect->SetAtrDXY(dp++, BEZEATR, bz2.x3, bz2.y3);
	}
	return(dp);
}

int CCmdLine::SetDivFiguBefore(DBL len, DBL xx, DBL yy, CVect *srcVect, int pp, CVect *dstVect, int dp)
{
	int		kind;
	int		pcnt, p1, p2, p3, p4;
	DBL		len1, len2;
	DBL		x1, y1, x2, y2;
	DBL		xt, yt;
	DBL		t, t1, b1, b2, b3, b4;
	dbezetype	bz, bz1, bz2;
	darctype	arc;

	pcnt = srcVect->GetPointCount();
	kind = srcVect->GetFiguKind(pp);
	p1 = srcVect->PointLegal(pcnt, pp+0);
	p2 = srcVect->PointLegal(pcnt, pp+1);
	p3 = srcVect->PointLegal(pcnt, pp+2);
	p4 = srcVect->PointLegal(pcnt, pp+3);

	if(kind == LINEKIND){
		srcVect->GetDXY(p1, &x1, &y1);
		srcVect->GetDXY(p2, &x2, &y2);
		len1 = hypot(xx-x1, yy-y1);
		xt = (x1 - xx) * len / len1 + xx;	 
		yt = (y1 - yy) * len / len1 + yy;
		dstVect->SetAtrDXY(dp++, 0, x1, y1);
		dstVect->SetAtrDXY(dp++, 0, xt, yt);
	}
	else{
		switch(kind){
		case BEZEKIND:
			srcVect->GetDXY(p1, &(bz.x1), &(bz.y1));
			srcVect->GetDXY(p2, &(bz.x2), &(bz.y2));
			srcVect->GetDXY(p3, &(bz.x3), &(bz.y3));
			srcVect->GetDXY(p4, &(bz.x4), &(bz.y4));
			break;
		case FCBEZEKIND:
			srcVect->GetFreeCurveBeze(p1, &bz);
			break;
		case ARCKIND:
			srcVect->GetDXY(p1, &(arc.x1), &(arc.y1));
			srcVect->GetDXY(p2, &(arc.x2), &(arc.y2));
			srcVect->GetDXY(p3, &(arc.x3), &(arc.y3));
			cruvearctobeze(arc, &bz);
			break;
		}
		len2 = INTMAX;
		for(t = 0.001; t < 1.0; t += 0.002){
			t1 = 1 - t;
			b1 = t1 * t1 * t1;
			b2 = 3 * t * t1 * t1;
			b3 = 3 * t * t * t1;
			b4 = t * t * t;
			xt = (b1 * bz.x1 + b2 * bz.x2 + b3 * bz.x3 + b4 * bz.x4);
			yt = (b1 * bz.y1 + b2 * bz.y2 + b3 * bz.y3 + b4 * bz.y4);
			len1 = hypot(xx-xt, yy-yt);
			if(len1 <= len){
				break;
			}
			if(len2 < len1){
				break;
			}
			len2 = len1;
		}
		if(t < 1.0){
			crossbezettdiv(&bz, t, &bz1, &bz2);
		}
		else{
			crossbezediv(&bz, xx, yy, &bz1, &bz2);
		}
		dstVect->SetAtrDXY(dp++, CURVATR|BEZEATR, bz1.x1, bz1.y1);
		dstVect->SetAtrDXY(dp++, BEZEATR, bz1.x2, bz1.y2);
		dstVect->SetAtrDXY(dp++, BEZEATR, bz1.x3, bz1.y3);
		dstVect->SetAtrDXY(dp++, 0, bz1.x4, bz1.y4);
	}
	return(dp);
}

int CCmdLine::CrossPointAfter(DBL len, DBL xx, DBL yy, CVect *srcVect, int pp)
{
	int		pcnt, tp, p1;
	DBL		x1, y1;
	DBL		len1;

	pcnt = m_pcVect->GetPointCount();
	tp = pp;
	p1 = m_pcVect->NextPointGet(tp);
	while(1){
		srcVect->GetDXY(p1, &x1, &y1);
		len1 = hypot(xx - x1, yy - y1);
		if(len1 > len){
			break;
		}
		tp = p1;
		p1 = m_pcVect->NextPointGet(tp);
		if(tp == p1){
			break;
		}
		if(p1 == pp){
			break;
		}
	}
	return(tp);
}

int CCmdLine::CrossPointBefore(DBL len, DBL xx, DBL yy, CVect *srcVect, int pp)
{
	int		pcnt, tp, p1;
	DBL		x1, y1;
	DBL		len1;

	pcnt = m_pcVect->GetPointCount();
	p1 = pp;
	while(1){
		srcVect->GetDXY(p1, &x1, &y1);
		len1 = hypot(xx - x1, yy - y1);
		if(len1 > len){
			break;
		}
		tp = p1;
		p1 = m_pcVect->BeforePointGet(p1);
		if(tp == p1){
			break;
		}
		if(p1 == pp){
			break;
		}
	}
	return(p1);
}

void CCmdLine::ToBeze()
{
	POSITION	pos;
	CData		*pcData;
	CVect		*pcVect;

	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_cPtrList.GetNext(pos);
		if(pcData == NULL){
			break;
		}
		pcVect = (CVect*)pcData;
		ToBezeVect(pcVect);
	}
}

void CCmdLine::ToBezeVect(CVect *pcVect)
{
	int		pcnt, pp, dp;
	int		p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		rag1, rag2;
	int		rag1no, rag2no;
	DBL		xp1, yp1, xp2, yp2;
	dbezetype	bz;
	darctype	ac;
	CVect		*tmpVect;

	pcnt = pcVect->GetPointCount();
	if(pcnt <= 1){
		return;
	}
	dp = 0;
	tmpVect = new CVect(BASEVECTID);
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
				tmpVect->SetAtrDXY(dp++, 0, x1, y1);
				tmpVect->SetAtrDXY(dp++, 0, x2, y2);
			}
			else if(p2 == 0){
				tmpVect->SetAtrDXY(dp++, 0, x1, y1);
			}
			else{
				tmpVect->SetAtrDXY(dp++, atr1, x1, y1);
				tmpVect->SetAtrDXY(dp++, atr2, x2, y2);
				tmpVect->SetAtrDXY(dp++, atr3, x3, y3);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				ac.x1 = x1; ac.y1 = y1;
				ac.x2 = x2; ac.y2 = y2;
				ac.x3 = x3; ac.y3 = y3;
				cruvearctobeze(ac, &bz);
				tmpVect->SetAtrDXY(dp++, CURVATR|BEZEATR, bz.x1, bz.y1);
				tmpVect->SetAtrDXY(dp++, BEZEATR, bz.x2, bz.y2);
				tmpVect->SetAtrDXY(dp++, BEZEATR, bz.x3, bz.y3);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					ac.x1 = x0; ac.y1 = y0;
					ac.x2 = x1; ac.y2 = y1;
					ac.x3 = x2; ac.y3 = y2;
					cruvearctobeze(ac, &bz);
					tmpVect->SetAtrDXY(dp++, CURVATR|BEZEATR, bz.x1, bz.y1);
					tmpVect->SetAtrDXY(dp++, BEZEATR, bz.x2, bz.y2);
					tmpVect->SetAtrDXY(dp++, BEZEATR, bz.x3, bz.y3);
				}
				else{
					rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
					rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);
					if(rag1 == NORAG && rag2 == NORAG){
						tmpVect->SetAtrDXY(dp++, 0, x1, y1);
					}
					else{
						curvenoragtocurverag(&rag1, &rag2);
						rag1no = curveragnoget(rag1);
						rag2no = curveragnoget(rag2);
						curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
						curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
						bz.x1 = x1; bz.y1 = y1;
						bz.x2 = xp1; bz.y2 = yp1;
						bz.x3 = xp2; bz.y3 = yp2;
						bz.x4 = x2; bz.y4 = y2;
						tmpVect->SetAtrDXY(dp++, CURVATR|BEZEATR, bz.x1, bz.y1);
						tmpVect->SetAtrDXY(dp++, BEZEATR, bz.x2, bz.y2);
						tmpVect->SetAtrDXY(dp++, BEZEATR, bz.x3, bz.y3);
					}
				}
			}
			else{
				tmpVect->SetAtrDXY(dp++, 0, x1, y1);
			}
			pp += 1;
		}
	}

	pcnt = dp;
	for(dp = 0; dp < pcnt; dp++){
		tmpVect->GetAtrDXY(dp, &atr1, &x1, &y1);
		pcVect->SetAtrDXY(dp, atr1, x1, y1);
	}
	pcVect->SetPointCount(pcnt);
	pcVect->RenewMiniMax();
	delete(tmpVect);
}
