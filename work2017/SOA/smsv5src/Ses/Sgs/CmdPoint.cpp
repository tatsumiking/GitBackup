
/////////////////////////////////////////////////////////////////////////////
// CCmdXXXX

#include "stdafx.h"
#include "math.h"

#include "CmdBase.h"
#include "CmdPoint.h"
#include "Dialog2PointSize.h"

//extern "C" {
	#include "../../ObjLib/Crosslib.h"
	#include "../../ObjLib/Curvelib.h"
	#include "../../ObjLib/Pdrwlib.h"
	#include "../../ObjLib/Scrnlib.h"
	#include "../../ObjLib/Senvlib.h"
//}

CCmdPoint::CCmdPoint(CScrollView* pcview)
:CCmdBase(pcview)
{
}

UINT CCmdPoint::ExecPointMove(UINT event, DBL dx, DBL dy)
{
	int		retpp;

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
				m_nPP = retpp;
				m_pcVect->OnFlag(ATNTION1FLAG);
				m_pcVect->GetDXY(m_nPP, &m_dBX, &m_dBY);
				MoveMousePoint(dx, dy, m_dBX, m_dBY);
				BeginDraw();
				DrawModeXOR();
				m_pcVect->PLDraw(m_pcDC);

				m_pcVect->AtntionDraw(m_pcDC);
				EndDraw();
				m_nMouseSetCount = 1;
			}
		}
		else{
			BeginDraw();
			m_pcVect->AtntionDraw(m_pcDC);
			m_pcVect->SetDXY(m_nPP, dx, dy);
			m_pcVect->RenewMiniMax();
			m_pcData = m_pcDoc->UseFitingVect(m_pcVect);
			m_pcVect->PLDraw(m_pcDC);
			EndDraw();
			m_pcVect->OffFlag(ATNTION1FLAG);
			if(m_pcData != NULL){
				DrawModeSET();
				m_pcData->DelDraw(m_pcDC);
				DrawModeXOR();
				m_pcData->StrgFiting();
				m_pcData->RenewMiniMax();
				ScrnDataRedraw(m_pcData);
			}
			else{
				ScrnAllRedraw();
			}
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcVect->AtntionDraw(m_pcDC);
			m_pcVect->SetDXY(m_nPP, dx, dy);
			m_pcVect->AtntionDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcVect->AtntionDraw(m_pcDC);
			m_pcVect->SetDXY(m_nPP, m_dBX, m_dBY);
			m_pcVect->RenewMiniMax();
			m_pcVect->AtntionDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
			m_pcVect->OffFlag(ATNTION1FLAG);
		}
		break;
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcVect->AtntionDraw(m_pcDC);
			m_pcVect->SetDXY(m_nPP, m_dBX, m_dBY);
			m_pcVect->RenewMiniMax();
			m_pcData = m_pcDoc->UseFitingVect(m_pcVect2);
			if(m_pcData != NULL){
				DrawModeSET();
				m_pcData->DelDraw(m_pcDC);
				DrawModeXOR();
				m_pcData->StrgFiting();
				m_pcData->RenewMiniMax();
				ScrnDataRedraw(m_pcData);
			}
			m_pcVect->AtntionDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
			m_pcVect->OffFlag(ATNTION1FLAG);
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdPoint::ExecPointDel(UINT event, DBL dx, DBL dy)
{
	int		retpp;
	DBL		xx, yy;
	DBL		len, mm;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_pcBackVect = new CVect(BASEVECTID);
		m_nPP = -1;
		break;
	case SETEVENT:
		m_pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
		if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
			m_pcVect->GetDXY(retpp, &xx, &yy);
			len = hypot(xx - dx, yy - dy);
			mm = 32;
			scrndlenrltvtomm(&mm);
			if(mm < len){
				break;
			}

			SaveUndo();
			m_nPP = retpp;
			BeginDraw();
			m_pcVect->PLDraw(m_pcDC);
			m_pcData = m_pcDoc->UseFitingVect(m_pcVect);
			if(m_pcData != NULL){
				if(3 <= m_pcVect->GetPointCount()){
					PointDelMain();
				}
				DrawModeSET();
				m_pcData->DelDraw(m_pcDC);
				DrawModeXOR();
				m_pcData->StrgFiting();
				m_pcData->RenewMiniMax();
				ScrnDataRedraw(m_pcData);
			}
			else{
				PointDelMain();
			}
			if(m_pcVect != NULL){
				m_pcVect->PLDraw(m_pcDC);
				m_pcVect->RenewMiniMax();
			}
			ScrnAllRedraw();
			EndDraw();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nPP != -1){
			RestoreBackVect();
			m_nPP = -1;
			ScrnAllRedraw();
		}
		break;
	case ENDEVENT:
		ScrnAllRedraw();
		delete(m_pcBackVect);
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdPoint::ExecPointAdd(UINT event, DBL dx, DBL dy)
{
	DBL		retx, rety;
	int		retpp;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_pcBackVect = new CVect(BASEVECTID);
		m_nMouseSetCount = 0;
		m_nPP = -1;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_pcVect = (CVect*)SrchDataOnLinePoint(dx, dy, &retx, &rety, &retpp);
			if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
				SaveUndo();
				m_pcVect->OnFlag(ATNTION1FLAG);
				m_nPP = retpp;
				m_pcVect->GetDXY(m_nPP, &m_dBX, &m_dBY);
				BeginDraw();
				m_pcVect->PLDraw(m_pcDC);
				PointAddMain(retx, rety);
				m_pcVect->RenewMiniMax();
				m_pcVect->AtntionDraw(m_pcDC);
				EndDraw();
				m_nMouseSetCount = 1;
			}
		}
		else{
			m_pcVect->SetDXY(m_nPP, dx, dy);
			m_pcVect->OffFlag(ATNTION1FLAG);
			m_pcVect->RenewMiniMax();
			m_pcData = m_pcDoc->UseFitingVect(m_pcVect);
			if(m_pcData != NULL){
				m_pcData->StrgFiting();
				m_pcData->RenewMiniMax();
			}
			ScrnAllRedraw();
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcVect->AtntionDraw(m_pcDC);
			m_pcVect->SetDXY(m_nPP, dx, dy);
			m_pcVect->AtntionDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 0){
			RestoreBackVect();
			ScrnAllRedraw();
		}
		else if(m_nMouseSetCount == 1){
			RestoreBackVect();
			m_pcVect->RenewMiniMax();
			m_nMouseSetCount = 0;
			ScrnAllRedraw();
		}
		break;
	case ENDEVENT:
		delete(m_pcBackVect);
		ScrnAllRedraw();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdPoint::ExecPointChngAtr(UINT event, DBL dx, DBL dy)
{
	int		retpp;
	int		atr;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		m_pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
		if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
			m_pcVect->GetAtr(retpp, &atr);
			if(atr & CONTATR){
				atr &= ~CONTATR;
			}
			else{
				atr |= CONTATR;
			}
			m_pcVect->SetAtr(retpp, atr);
			m_pcVect->RenewMiniMax();
			m_pcData = m_pcDoc->UseFitingVect(m_pcVect);
			if(m_pcData != NULL){
				m_pcData->StrgFiting();
				m_pcData->RenewMiniMax();
			}
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

UINT CCmdPoint::ExecAnyDel(UINT event, DBL dx, DBL dy)
{
	DBL		retlen;
	int		retpp;
	DBL		len1, len2;
	int		tpp;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_pcBackVect = new CVect(BASEVECTID);
		m_nMouseSetCount = 0;
		m_nPP = -1;
		m_nPP2 = -1;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
			if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
				m_nPP = retpp;
				m_nPP2 = -1;
				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 1;
			}
		}
		else if(m_nMouseSetCount == 1){
			m_pcVect->SrchNearPoint(dx, dy, &retlen, &retpp);
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
			m_nPP2 = retpp;
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
		else{
			SaveUndo();
			MainAnyPointDelete();
			m_pcData = m_pcDoc->UseFitingVect(m_pcVect);
			if(m_pcData != NULL){
				m_pcData->StrgFiting();
				m_pcData->RenewMiniMax();
			}

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
			if(m_nPP != -1 && m_nPP2 != -1){
				RestoreBackVect();
				ScrnAllRedraw();
			}
		}
		else if(m_nMouseSetCount == 1){
			m_nMouseSetCount = 0;
			ScrnAllRedraw();
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
			scrnpenend(m_pcDC->m_hDC);
			retpp = m_nPP2;
			m_nPP2 = m_nPP;
			m_nPP = retpp;
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
			scrnpenend(m_pcDC->m_hDC);
			ScrnAllRedraw();
			EndDraw();
		}
		break;
	case ENDEVENT:
		m_nPP = -1;
		m_nPP2 = -1;
		delete(m_pcBackVect);
		ScrnAllRedraw();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);

}

UINT CCmdPoint::ExecAnyMove(UINT event, DBL dx, DBL dy)
{
	DBL		retlen;
	int		retpp;
	DBL		len1, len2;
	int		tpp;
	DBL		movex, movey;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_pcBackVect = new CVect(BASEVECTID);
		m_nMouseSetCount = 0;
		m_nPP = -1;
		m_nPP2 = -1;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
			if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
				m_nPP = retpp;
				m_nPP2 = -1;

				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();

				m_nMouseSetCount = 1;
			}
		}
		else if(m_nMouseSetCount == 1){
			m_pcVect->SrchNearPoint(dx, dy, &retlen, &retpp);
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
			m_nPP2 = retpp;
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
			StoreBackVect();
		}
		else if(m_nMouseSetCount == 2){
			SaveUndo();
			BeginDraw();
			m_pcVect->PLDraw(m_pcDC);
			m_pcVect->AtntionDraw(m_pcDC);
			EndDraw();
			m_dBX = dx;
			m_dBY = dy;
			m_nMouseSetCount = 3;
		}
		else if(m_nMouseSetCount == 3){
			movex = dx - m_dBX; 
			movey = dy - m_dBY; 
			MainAnyPointMove(movex, movey);
			m_dBX = dx;
			m_dBY = dy;

			m_pcData = m_pcDoc->UseFitingVect(m_pcVect);
			if(m_pcData != NULL){
				m_pcData->StrgFiting();
				m_pcData->RenewMiniMax();
			}
			m_pcVect->RenewMiniMax();
			m_nMouseSetCount = 0;
			ScrnAllRedraw();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 3){
			BeginDraw();
			m_pcVect->AtntionDraw(m_pcDC);
			movex = dx - m_dBX; 
			movey = dy - m_dBY; 
			MainAnyPointMove(movex, movey);
			m_dBX = dx;
			m_dBY = dy;
			m_pcVect->AtntionDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 0){
			if(m_nPP != -1 && m_nPP2 != -1){
				BeginDraw();
				m_pcVect->PLDraw(m_pcDC);
				RestoreBackVect();
				m_nPP = -1;
				m_nPP2 = -1;

				scrnsetfigupen(m_pcDC->m_hDC);
				m_pcVect->PLDraw(m_pcDC);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				//return(REDRAWEVENT);
			}
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
			scrnpenend(m_pcDC->m_hDC);
			retpp = m_nPP2;
			m_nPP2 = m_nPP;
			m_nPP = retpp;
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
		}
		break;
	case ENDEVENT:
		m_nPP = -1;
		m_nPP2 = -1;
		delete(m_pcBackVect);
		ScrnAllRedraw();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdPoint::Exec2Point(UINT event, DBL dx, DBL dy)
{
	int		retpp;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_nMouseSetCount = 0;
		m_nPP = -1;
		m_nPP2 = -1;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
			if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
				m_nPP = retpp;
				m_nPP2 = -1;
				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, -1);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 1;
			}
		}
		else if(m_nMouseSetCount == 1){
			m_pcVect2 = (CVect*)SrchDataPoint(dx, dy, &retpp);
			if(m_pcVect2 != NULL && m_pcVect2->CheckID() == BASEVECTID){
				m_nPP2 = retpp;
				m_pcVect2->GetDXY(m_nPP2, &m_dBX, &m_dBY);
				SaveUndo();
				Move2PointMain();
				m_pcData = m_pcDoc->UseFitingVect(m_pcVect2);
				if(m_pcData != NULL){
					m_pcData->StrgFiting();
					m_pcData->RenewMiniMax();
				}
				m_pcVect2->RenewMiniMax();
				m_nMouseSetCount = 0;
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
		if(m_nMouseSetCount == 0){
			if(m_nPP != -1 && m_nPP2 != -1){
				m_pcVect2->SetDXY(m_nPP2, m_dBX, m_dBY);
				ScrnAllRedraw();
			}
		}
		else if(m_nMouseSetCount == 1){
			m_nMouseSetCount = 0;
			ScrnAllRedraw();
		}
		break;
	case ENDEVENT:
		m_nPP = -1;
		m_nPP2 = -1;
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdPoint::ExecPicup(UINT event, DBL dx, DBL dy)
{
	int		retpp;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_pcDoc->AllOffPointFlag(PATN0ATR);	
		break;
	case SETEVENT:
		m_pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
		if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
			if(m_pcVect->CheckPointFlag(retpp, PATN0ATR) == ON){
				m_pcVect->OffPointFlag(retpp, PATN0ATR);
			}
			else{
				m_pcVect->OnPointFlag(retpp, PATN0ATR);
			}
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, retpp, -1);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
		}
		break;
	case RESETEVENT:
		SaveUndo();
		m_pcDoc->PointPicup();
		m_pcDoc->AllOffPointFlag(PATN0ATR);	
		ScrnAllRedraw();
		return(REDRAWEVENT);
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdPoint::ExecSlopLegal(UINT event, DBL dx, DBL dy)
{
	int		retpp;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		SaveUndo();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		m_pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
		if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
			m_nPP = retpp;
			m_pcVect->OnFlag(ATNTION1FLAG);
			dx = m_dBX; dy = m_dBY;
			SlopLegalMain(m_pcVect, m_nPP);
			m_pcData = m_pcDoc->UseFitingVect(m_pcVect);
			if(m_pcData != NULL){
				m_pcData->StrgFiting();
				m_pcData->RenewMiniMax();
			}
			m_pcVect->OffFlag(ATNTION1FLAG);
			m_pcVect->RenewMiniMax();
			m_nMouseSetCount = 1;
			ScrnAllRedraw();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 1){
			m_pcVect->SetDXY(m_nPP, m_dBX, m_dBY);
			m_pcVect->RenewMiniMax();
			m_nMouseSetCount = 0;
			ScrnAllRedraw();
		}
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdPoint::ExecXLegal(UINT event, DBL dx, DBL dy)
{
	int		retpp;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_pcDoc->AllOffPointFlag(PATN0ATR);
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		m_pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
		if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
			if(m_nMouseSetCount == 0){
				m_pcVect->GetDXY(retpp, &m_dSX, &m_dSY);
				m_nMouseSetCount = 1;
			}
			if(m_pcVect->CheckPointFlag(retpp, PATN0ATR) == ON){
				m_pcVect->OffPointFlag(retpp, PATN0ATR);
			}
			else{
				m_pcVect->OnPointFlag(retpp, PATN0ATR);
			}
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, retpp, -1);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
		}
		break;
	case RESETEVENT:
		SaveUndo();
		m_pcDoc->PointXLegal(m_dSY);
		m_pcDoc->FitingData();
		m_pcDoc->AllOffPointFlag(PATN0ATR);	
		m_nMouseSetCount = 0;
		ScrnAllRedraw();
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdPoint::ExecYLegal(UINT event, DBL dx, DBL dy)
{
	int		retpp;

	switch(event){
	case INITEVENT:
		m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
		m_pcDoc->AllOffPointFlag(PATN0ATR);	
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		m_pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
		if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
			if(m_nMouseSetCount == 0){
				m_pcVect->GetDXY(retpp, &m_dSX, &m_dSY);
				m_nMouseSetCount = 1;
			}
			if(m_pcVect->CheckPointFlag(retpp, PATN0ATR) == ON){
				m_pcVect->OffPointFlag(retpp, PATN0ATR);
			}
			else{
				m_pcVect->OnPointFlag(retpp, PATN0ATR);
			}
			BeginDraw();
			scrnsetpointpen(m_pcDC->m_hDC);
			m_pcVect->PointAreaDraw(m_pcDC, retpp, -1);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
		}
		break;
	case RESETEVENT:
		SaveUndo();
		m_pcDoc->PointYLegal(m_dSX);
		m_pcDoc->FitingData();
		m_pcDoc->AllOffPointFlag(PATN0ATR);	
		m_nMouseSetCount = 0;
		ScrnAllRedraw();
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

// “à•”ŠÖ”
void CCmdPoint::PointDelMain()
{
	int		pcnt;
	int		pp;
	int		atr;
	DBL		dx, dy;

	pcnt = m_pcVect->GetPointCount();
	if(pcnt == 1){
		//pcDoc = (CTonDoc*)m_pcView->GetDocument();
		//pcDoc->DeleteData(m_pcVect);
		//delete(m_pcVect);
		//m_pcVect = NULL;
		return;
	}

	for(pp = 0; pp < pcnt; pp++){
		m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
		m_pcBackVect->SetAtrDXY(pp, atr, dx, dy);
	}
	m_pcBackVect->SetPointCount(pcnt);

	for(pp = m_nPP+1; pp < pcnt; pp++){
		m_pcBackVect->GetAtrDXY(pp, &atr, &dx, &dy);
		m_pcVect->SetAtrDXY(pp-1, atr, dx, dy);
	}
	m_pcVect->SetPointCount(pcnt-1);
}

void CCmdPoint::PointAddMain(DBL setx, DBL sety)
{
	CVect	*pcTVect;
	int		pcnt;
	int		pp, dp;
	int		atr;
	DBL		dx, dy;
	int		kind;
	int		np;
	dfigutype	fg;
	dbezetype	bz, bz1, bz2;
	darctype	ac, ac1, ac2;

	pcTVect = new CVect(BASEVECTID);
	pcnt = m_pcVect->GetPointCount();
	for (pp = 0; pp <= m_nPP; pp++) {
		m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
		pcTVect->SetAtrDXY(pp, atr, dx, dy);
	}
	dp = pp;
	kind = m_pcVect->GetFigu(m_nPP, &np, &fg);
	switch (kind) {
	case BEZEKIND:
		bz.x1 = fg.x1; bz.y1 = fg.y1;
		bz.x2 = fg.x2; bz.y2 = fg.y2;
		bz.x3 = fg.x3; bz.y3 = fg.y3;
		bz.x4 = fg.x4; bz.y4 = fg.y4;
		crossbezediv(&bz, setx, sety, &bz1, &bz2);
		pcTVect->SetAtrDXY(dp++, BEZEATR, bz1.x2, bz1.y2);
		pcTVect->SetAtrDXY(dp++, BEZEATR, bz1.x3, bz1.y3);
		m_nPP = dp;
		pcTVect->SetAtrDXY(dp++, BEZEATR | CURVATR, setx, sety);
		pcTVect->SetAtrDXY(dp++, BEZEATR, bz2.x2, bz2.y2);
		pcTVect->SetAtrDXY(dp++, BEZEATR, bz2.x3, bz2.y3);
		break;
	case ARCKIND:
		ac.x1 = fg.x1; ac.y1 = fg.y1;
		ac.x2 = fg.x2; ac.y2 = fg.y2;
		ac.x3 = fg.x3; ac.y3 = fg.y3;
		crossarcdiv(&ac, setx, sety, &ac1, &ac2);
		pcTVect->SetAtrDXY(dp++, CURVATR | CONTATR, ac1.x2, ac1.y2);
		m_nPP = dp;
		pcTVect->SetAtrDXY(dp++, CURVATR, setx, sety);
		pcTVect->SetAtrDXY(dp++, CURVATR | CONTATR, ac2.x2, ac2.y2);
		break;
	case FREEKIND:
		m_nPP = dp;
		pcTVect->SetAtrDXY(dp++, CURVATR | CONTATR, setx, sety);
		break;
	case LINEKIND:
		m_nPP = dp;
		pcTVect->SetAtrDXY(dp++, 0, setx, sety);
		break;
	}
	for (pp = np; pp < pcnt; pp++, dp++) {
		m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
		pcTVect->SetAtrDXY(dp, atr, dx, dy);
	}
	pcnt = dp;
	for (pp = 0; pp < pcnt; pp++) {
		pcTVect->GetAtrDXY(pp, &atr, &dx, &dy);
		m_pcVect->SetAtrDXY(pp, atr, dx, dy);
	}
	m_pcVect->SetPointCount(pcnt);
	delete(pcTVect);

	pcnt = m_pcVect->GetPointCount();
	for (pp = 0; pp < pcnt; pp++) {
		m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
		m_pcBackVect->SetAtrDXY(pp, atr, dx, dy);
	}
	m_pcBackVect->SetPointCount(pcnt);
}

void CCmdPoint::RestoreBackVect()
{
	int		pcnt, pp;
	int		atr;
	DBL		dx, dy;

	pcnt = m_pcBackVect->GetPointCount();
	for(pp = 0; pp < pcnt; pp++){
		m_pcBackVect->GetAtrDXY(pp, &atr, &dx, &dy);
		m_pcVect->SetAtrDXY(pp, atr, dx, dy);
	}
	m_pcVect->SetPointCount(pcnt);
	m_pcVect->RenewMiniMax();
}

void CCmdPoint::MainAnyPointDelete()
{
	int		pcnt, pp, dp;
	int		atr;
	DBL		dx, dy;

	pcnt = m_pcVect->GetPointCount();
	for(pp = 0; pp < pcnt; pp++){
		m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
		m_pcBackVect->SetAtrDXY(pp, atr, dx, dy);
	}
	m_pcBackVect->SetPointCount(pcnt);

	if(m_nPP < m_nPP2){
		dp = m_nPP+1;
		for(pp = m_nPP2; pp < pcnt; pp++, dp++){
			m_pcBackVect->GetAtrDXY(pp, &atr, &dx, &dy);
			m_pcVect->SetAtrDXY(dp, atr, dx, dy);
		}
		if(1 < dp){
			m_pcVect->GetAtr(0, &atr);
			atr &= ~CONTATR;	
			m_pcVect->SetAtr(0, atr);
			m_pcVect->SetAtr(dp-1, 0);
		}
		pcnt = dp;
	}
	else{
		dp = 0;
		for(pp = m_nPP2; pp <= m_nPP; pp++, dp++){
			m_pcBackVect->GetAtrDXY(pp, &atr, &dx, &dy);
			m_pcVect->SetAtrDXY(dp, atr, dx, dy);
		}
		if(1 < dp){
			m_pcVect->GetAtr(0, &atr);
			atr &= ~CONTATR;	
			m_pcVect->SetAtr(0, atr);
			m_pcVect->SetAtr(dp-1, 0);
		}
		pcnt = dp;

	}

	if(pcnt == 0){
		atr = 0;
		m_pcVect->SetAtrDXY(0, atr, dx, dy);
		pcnt = 1;
	}

	m_pcVect->SetPointCount(pcnt);
	m_pcVect->RenewMiniMax();
}

void CCmdPoint::MainAnyPointMove(DBL movex, DBL movey)
{
	int		pcnt, pp;
	int		atr;
	DBL		dx, dy;

	if(m_nPP <= m_nPP2){
		for(pp = m_nPP; pp <= m_nPP2; pp++){
			m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
			dx += movex;
			dy += movey;
			m_pcVect->SetAtrDXY(pp, atr, dx, dy);
		}
	}
	else{
		pcnt = m_pcVect->GetPointCount();
		for(pp = m_nPP; pp <= pcnt; pp++){
			m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
			dx += movex;
			dy += movey;
			m_pcVect->SetAtrDXY(pp, atr, dx, dy);
		}
		for(pp = 0; pp <= m_nPP2; pp++){
			m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
			dx += movex;
			dy += movey;
			m_pcVect->SetAtrDXY(pp, atr, dx, dy);
		}

	}
}

void CCmdPoint::StoreBackVect()
{
	int		pcnt, pp;
	int		atr;
	DBL		dx, dy;

	pcnt = m_pcVect->GetPointCount();
	for(pp = 0; pp < pcnt; pp++){
		m_pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
		m_pcBackVect->SetAtrDXY(pp, atr, dx, dy);
	}
	m_pcBackVect->SetPointCount(pcnt);
}

void CCmdPoint::Move2PointMain()
{
	DBL		xlen, ylen, len;
	CDialog2PointSize*	pDialog2PointSize;

	m_pcVect->GetDXY(m_nPP, &m_dSX, &m_dSY);
	m_pcVect2->GetDXY(m_nPP2, &m_dEX, &m_dEY);
	len = hypot(m_dEX - m_dSX, m_dEY - m_dSY);
	xlen = fabs(m_dEX - m_dSX);
	ylen = fabs(m_dEY - m_dSY);
	len = FLOOR100(len);
	xlen = FLOOR100(xlen);
	ylen = FLOOR100(ylen);
	pDialog2PointSize = new CDialog2PointSize(m_pcView);
	pDialog2PointSize->m_dLength = len;
	pDialog2PointSize->m_dXSize = xlen;
	pDialog2PointSize->m_dYSize = ylen;
	if(pDialog2PointSize->DoModal() == IDOK){
		if(pDialog2PointSize->m_dLength != len){
			m_dEX = (m_dEX - m_dSX) * pDialog2PointSize->m_dLength / len + m_dSX;
			m_dEY = (m_dEY - m_dSY) * pDialog2PointSize->m_dLength / len + m_dSY;
		}
		else{
			if(pDialog2PointSize->m_dXSize != xlen){
				m_dEX = (m_dEX - m_dSX) * pDialog2PointSize->m_dXSize / xlen + m_dSX;
			}
			if(pDialog2PointSize->m_dYSize != ylen){
				m_dEY = (m_dEY - m_dSY) * pDialog2PointSize->m_dYSize / ylen + m_dSY;
			}
		}
		BeginDraw();
		DrawModeSET();
		m_pcVect2->DelDraw(m_pcDC);
		m_pcVect2->SetDXY(m_nPP2, m_dEX, m_dEY);
		m_pcVect2->PLDraw(m_pcDC);
		EndDraw();
	}
	delete(pDialog2PointSize);
}

void CCmdPoint::SlopLegalMain(CVect *pcVect, int pp)
{
	DBL		dx, dy;
	DBL		len;
	DBL		cx, cy, rag;
	DBL		sx, sy, ex, ey;
	DBL		len1, len2, len3, len4;
	DBL		xx1, yy1, xx2, yy2, xx3, yy3, xx4, yy4;
	int		pcnt, p1, p2;
	dlinetype	lnx, lny, ln2;

	CTonApp *theApp = (CTonApp*)AfxGetApp();
	CSysEnv *pcSysEnv = theApp->m_pcSysEnv;
	pcSysEnv->ScrnGetSubXY(&cx, &cy);
	pcSysEnv->ScrnGetSubRag(&rag);
	sx = -100; ex = 100;
	sy = cy; ey = cy;
	curveroundtrns(sx, sy, cx, cy, -rag, &(lnx.x1), &(lnx.y1));
	curveroundtrns(ex, ey, cx, cy, -rag, &(lnx.x2), &(lnx.y2));
	sx = cx; ex = cx;
	sy = -100; ey = 100;
	curveroundtrns(sx, sy, cx, cy, -rag, &(lny.x1), &(lny.y1));
	curveroundtrns(ex, ey, cx, cy, -rag, &(lny.x2), &(lny.y2));

	pcnt = 	pcVect->GetPointCount();
	pcVect->GetDXY(pp, &dx, &dy);
	p1 = pcVect->PointLegal(pcnt, pp-1);
	p2 = pcVect->PointLegal(pcnt, pp);
	pcVect->GetDXY(p1, &(ln2.x1), &(ln2.y1));
	pcVect->GetDXY(p2, &(ln2.x2), &(ln2.y2));
	curvecrosslnlnxyget(lnx, ln2, &xx1, &yy1);
	curvecrosslnlnxyget(lny, ln2, &xx2, &yy2);
	p1 = pcVect->PointLegal(pcnt, pp);
	p2 = pcVect->PointLegal(pcnt, pp+1);
	pcVect->GetDXY(p1, &(ln2.x1), &(ln2.y1));
	pcVect->GetDXY(p2, &(ln2.x2), &(ln2.y2));
	curvecrosslnlnxyget(lnx, ln2, &xx3, &yy3);
	curvecrosslnlnxyget(lny, ln2, &xx4, &yy4);

	len1 = hypot(dx - xx1, dy - yy1);
	len2 = hypot(dx - xx2, dy - yy2);	
	len3 = hypot(dx - xx3, dy - yy3);
	len4 = hypot(dx - xx4, dy - yy4);	
	if(len1 < len2){
		len = len1;
		dx = xx1; dy = yy1;
	}
	else{
		len = len2;
		dx = xx2; dy = yy2;
	}
	if(len3 < len){
		len = len3;
		dx = xx3; dy = yy3;
	}
	if(len4 < len){
		len = len4;
		dx = xx4; dy = yy4;
	}
	pcVect->SetDXY(pp, dx, dy);
}
