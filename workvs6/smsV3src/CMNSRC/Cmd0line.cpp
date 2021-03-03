
/////////////////////////////////////////////////////////////////////////////
// CCmdLine

#include "stdafx.h"
#include "math.h"

#include "cmd_base.h"
#include "cmd_line.h"
#include "crosvect.h"

#define	SLCTCMND	1
#define MAINCMND	2

#define	POINT1		1
#define	POINT2		2

#define	CRCLDIVCNT	16

extern "C" {
void curvesymxytrns(DBL a, DBL b, DBL t1, DBL t2, DBL *x, DBL *y);
int crossbezediv(dbezetype *sbz, DBL xx, DBL yy, dbezetype *dbz1, dbezetype *dbz2);
int crossarcdiv(darctype *sac, DBL xx, DBL yy, darctype *dac1, darctype *dac2);
int curvenkindget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
void scrnsetslctpen(HDC hDC);	
void scrnsetpointpen(HDC hDC);
void scrnsetfigupen(HDC hDC);
void scrnpenend(HDC hDC);
void scrndxymmtorltv(LPDBL x, LPDBL y);
void pdrwmove(HDC PaintDC, DBL x1, DBL y1);
void pdrwarc(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
void pdrwbezecntrl(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
void pdrwbeze(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
int pdrwreverseatrget(int atr1, int atr2);
int strcconrarc(DBL *xa, DBL *ya, DBL *xb, DBL *yb, DBL *xc, DBL *yc, DBL *xd, DBL *yd, DBL cnrr);
int strclinecrossget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4, DBL *x, DBL *y);
}

CCmdLine::CCmdLine(CScrollView* pcview)
:CCmdBase(pcview)
{
}

UINT CCmdLine::ExecLineMove(UINT event, DBL dx, DBL dy)
{
	int		retpp;
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
				m_nPP = retpp;
				m_dSX = dx;
				m_dSY = dy;
				m_dBX = dx;
				m_dBY = dy;
				BeginDraw();
				DrawModeSET();
				m_pcVect->DelDraw(m_pcDC);
				DrawModeXOR();
				m_pcVect->AtntionDraw(m_pcDC);
				EndDraw();
				m_nMouseSetCount = 1;
			}
		}
		else{
			BeginDraw();
			m_pcVect->AtntionDraw(m_pcDC);
			DrawModeSET();
			m_pcVect->PLDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
			m_pcVect->RenewMiniMax();	
			m_pcVect->OffFlag(ATNTION1FLAG);
			//return(REDRAWEVENT);
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
			BeginDraw();
			m_pcVect->AtntionDraw(m_pcDC);
			m_pcVect->MoveTrns(movex, movey);
			m_pcVect->RenewMiniMax();	
			DrawModeSET();
			m_pcVect->PLDraw(m_pcDC);
			EndDraw();
			m_pcVect->OffFlag(ATNTION1FLAG);
			m_nMouseSetCount = 0;
		}
		break;
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			movex = m_dBX - m_dSX;
			movey = m_dBY - m_dSY;
			BeginDraw();
			m_pcVect->AtntionDraw(m_pcDC);
			m_pcVect->MoveTrns(movex, movey);
			m_pcVect->RenewMiniMax();	
			DrawModeSET();
			m_pcVect->PLDraw(m_pcDC);
			EndDraw();
			m_pcVect->OffFlag(ATNTION1FLAG);
			m_nMouseSetCount = 0;
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
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		m_pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
		if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
			SaveUndo();
			BeginDraw();
			DrawModeSET();
			m_pcVect->DelDraw(m_pcDC);
			EndDraw();
			pcDoc = (CTonDoc*)m_pcView->GetDocument();
			pcDoc->DeleteData(m_pcVect);
			delete(m_pcVect);
			m_pcVect = NULL;
			m_nMouseSetCount = 1;
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
				m_nPP2 = tpp;
				BeginDraw();
				scrnsetpointpen(m_pcDC->m_hDC);
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, -1);
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 2;
			}
		}
		else if(m_nMouseSetCount == 2){
			SaveUndo();
			BeginDraw();
			DrawModeSET();
			m_pcVect->DelDraw(m_pcDC);
			LineLineingMain();
			scrnsetfigupen(m_pcDC->m_hDC);
			m_pcVect->PLDraw(m_pcDC);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
			m_pcVect->RenewMiniMax();
			m_nMouseSetCount = 0;
			//return(REDRAWEVENT);
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

UINT CCmdLine::ExecLineCurve(UINT event, DBL dx, DBL dy)
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
				m_pcVect->PointAreaDraw(m_pcDC, m_nPP, m_nPP2);
				scrnpenend(m_pcDC->m_hDC);
				EndDraw();
				m_nMouseSetCount = 2;
			}
		}
		else if(m_nMouseSetCount == 2){
			SaveUndo();
			BeginDraw();
			DrawModeSET();
			m_pcVect->DelDraw(m_pcDC);
			LineCurveMain();
			scrnsetfigupen(m_pcDC->m_hDC);
			m_pcVect->PLDraw(m_pcDC);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
			m_pcVect->RenewMiniMax();
			m_nMouseSetCount = 0;
			//return(REDRAWEVENT);
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
			BeginDraw();
			DrawModeSET();
			m_pcVect->DelDraw(m_pcDC);
			LineCutMain();
			scrnsetfigupen(m_pcDC->m_hDC);
			m_pcVect->PLDraw(m_pcDC);
			m_pcVect->RenewMiniMax();	
			if(m_pcVect2 != NULL){
				m_pcVect2->PLDraw(m_pcDC);
				m_pcVect2->RenewMiniMax();	
			}
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
			m_nMouseSetCount = 0;
			//return(REDRAWEVENT);
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
					BeginDraw();
					scrnsetpointpen(m_pcDC->m_hDC);
					m_pcVect2->PointAreaDraw(m_pcDC, m_nPP2, -1);
					scrnpenend(m_pcDC->m_hDC);
					DrawModeSET();
					m_pcVect->DelDraw(m_pcDC);
					m_pcVect2->DelDraw(m_pcDC);
					LineJoinMain();
					scrnsetfigupen(m_pcDC->m_hDC);
					m_pcVect->PLDraw(m_pcDC);
					m_pcVect->RenewMiniMax();	
					scrnsetfigupen(m_pcDC->m_hDC);
					EndDraw();
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
		m_pcDialogInputNum->SetDialogTitle("切断接続距離設定");
		m_pcDialogInputNum->SetUnitText("mm");
		m_pcDialogInputNum->SetTitleText("間隔");
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
			BeginDraw();
			DrawAtnLine(m_pcDC);
			EndDraw();
			m_dEX = dx;
			m_dEY = dy;
			m_pcDialogInputNum->GetItemElement();
			mm = m_pcDialogInputNum->m_dInputNum;
			LineCutJointMain(mm);
			m_nMouseSetCount = 0;
			return(REDRAWEVENT);
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
			DrawModeSET();
			m_pcVect->DelDraw(m_pcDC);
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
		m_pcDialogInputNum->SetDialogTitle("角付け範囲半径設定");
		m_pcDialogInputNum->SetUnitText("mm");
		m_pcDialogInputNum->SetTitleText("範囲半径");
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
			BeginDraw();
			DrawModeSET();
			m_pcVect->DelDraw(m_pcDC);
			ConersetMain();
			m_pcVect->RenewMiniMax();
			scrnsetfigupen(m_pcDC->m_hDC);
			m_pcVect->PLDraw(m_pcDC);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
			//return(REDRAWEVENT);
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
		m_pcDialogInputNum->SetDialogTitle("角Ｒ付けＲ半径設定");
		m_pcDialogInputNum->SetUnitText("mm");
		m_pcDialogInputNum->SetTitleText("Ｒ半径");
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
			BeginDraw();
			DrawModeSET();
			m_pcVect->DelDraw(m_pcDC);
			ConerarcMain();
			m_pcVect->RenewMiniMax();
			scrnsetfigupen(m_pcDC->m_hDC);
			m_pcVect->PLDraw(m_pcDC);
			scrnpenend(m_pcDC->m_hDC);
			EndDraw();
			//return(REDRAWEVENT);
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
		HlpMsgDsp("分割線を指定してください");
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_pcVect = (CVect*)SrchDataOnLinePoint(dx, dy, &retx, &rety, &retpp);
			if(m_pcVect != NULL && m_pcVect->CheckID() == BASEVECTID){
				m_nPP = retpp;
				BeginDraw();
				scrnsetslctpen(m_pcDC->m_hDC);
				m_pcVect->OneFiguDraw(m_pcDC, m_nPP);
				scrnpenend(m_pcDC->m_hDC);

				cnt = ReadLineDivCount();

				if(1 < cnt){
					SaveUndo();
					DrawModeSET();
					m_pcVect->DelDraw(m_pcDC);
					EndDraw();

					LineDivMain(cnt);
					m_pcVect->RenewMiniMax();
				}
				BeginDraw();
				DrawModeSET();
				m_pcVect->PLDraw(m_pcDC);
				EndDraw();

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
			m_pcVect->SetPointCount(pcnt);
			m_pcVect->OffFlag(REJIONFLAG);
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
			m_pcVect->SetPointCount(pcnt);
			m_pcVect->OffFlag(REJIONFLAG);
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
		m_pcVect->SetPointCount(m_nPP+1);
		m_pcVect->OffFlag(REJIONFLAG);
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
	CCrossVect	*pcCrossVect;
	dlinetype	ln;
	dcrostype	crs[64];
	dcrostype	crstmp;
	int			crsmax;
	CTonDoc		*pcDoc;
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
	crstmp.xx = 0;
	crstmp.yy = 0;
	while(1){
		SetDataListSelectID(BASEVECTID, ACTIVEFLAG);
		crsmax = 0;
		pcCrossVect = new CCrossVect(m_pcView);
		pcCrossVect->SetSpaceMM(mm);
		pcCrossVect->SetDataList(&m_cPtrList);
		pcCrossVect->GetCrossPoint(ln, crs, &crsmax);
		delete(pcCrossVect);

		if(crsmax == 0){
			break;
		}
		if(crstmp.xx == crs[0].xx
		&& crstmp.yy == crs[0].yy){
			break;
		}

		crstmp.xx = crs[0].xx;
		crstmp.yy = crs[0].yy;
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
			break;
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
				CreateNewVect(m_pcVect2);
				m_pcVect2->RenewMiniMax();
			}
			m_pcVect2 = NULL;
			CutVect(mm, crs[1]);
			pcVect21 = m_pcVect;
			pcVect22 = m_pcVect2;
			m_pcVect->RenewMiniMax();
			if(m_pcVect2 != NULL){
				CreateNewVect(m_pcVect2);
				m_pcVect2->RenewMiniMax();
			}

			JoinVect(pcVect11, pcVect12, pcVect21, pcVect22);
			m_pcVect->RenewMiniMax();
			if(m_pcVect2 != NULL){
				m_pcVect2->RenewMiniMax();
			}
		}
		break;
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
	int		pcnt, pp, dp;
	CVect	*tmpVect;
	int		kind;
	dbezetype	bz, bz1, bz2;
	darctype	ac, ac1, ac2;
	int		p0, p1, p2, p3, p4;
	int		np;
	DBL		x1, y1;
	DBL		x2, y2;
	DBL		xt, yt;
	DBL		len1, len2;

	len2 = mm / 2.0;
	m_pcVect = (CVect*)crs.pcData;
	pp = crs.pp;
	xx = crs.xx;
	yy = crs.yy;
	pcnt = m_pcVect->GetPointCount();
	p1 = m_pcVect->PointLegal(pcnt, pp+0);
	p2 = m_pcVect->PointLegal(pcnt, pp+1);
	p3 = m_pcVect->PointLegal(pcnt, pp+2);
	p4 = m_pcVect->PointLegal(pcnt, pp+3);

	if(m_pcVect->CheckFlag(REJIONFLAG) == ON){
		tmpVect = new CVect(BASEVECTID);
		dp = 0;
		kind = m_pcVect->GetFiguKind(pp);
		switch(kind){
		case LINEKIND:
			m_pcVect->GetDXY(p1, &x1, &y1);
			m_pcVect->GetDXY(p2, &x2, &y2);
			len1 = hypot(xx-x2, yy-y2);
			xt = (x2 - xx) * len2 / len1 + xx;
			yt = (y2 - yy) * len2 / len1 + yy;

			tmpVect->SetAtrDXY(dp++, 0, xt, yt);
			break;
		case BEZEKIND:
			m_pcVect->GetDXY(p1, &(bz.x1), &(bz.y1));
			m_pcVect->GetDXY(p2, &(bz.x2), &(bz.y2));
			m_pcVect->GetDXY(p3, &(bz.x3), &(bz.y3));
			m_pcVect->GetDXY(p4, &(bz.x4), &(bz.y4));

			m_pcVect->GetDXY(p1, &x1, &y1);
			m_pcVect->GetDXY(p4, &x2, &y2);
			len1 = hypot(xx-x2, yy-y2);
			xt = (x2 - xx) * len2 / len1 + xx;
			yt = (y2 - yy) * len2 / len1 + yy;
			m_pcVect->SetAtrDXY(dp, 0, xt, yt);

			crossbezediv(&bz, xt, yt, &bz1, &bz2);
			tmpVect->SetAtrDXY(dp++, CURVATR|BEZEATR, bz2.x1, bz2.y1);
			tmpVect->SetAtrDXY(dp++, BEZEATR, bz2.x2, bz2.y2);
			tmpVect->SetAtrDXY(dp++, BEZEATR, bz2.x3, bz2.y3);
			break;
		case FCBEZEKIND:
			m_pcVect->GetFreeCurveBeze(p1, &bz);

			m_pcVect->GetDXY(p1, &x1, &y1);
			m_pcVect->GetDXY(p2, &x2, &y2);
			len1 = hypot(xx-x2, yy-y2);
			xt = (x2 - xx) * len2 / len1 + xx;
			yt = (y2 - yy) * len2 / len1 + yy;

			crossbezediv(&bz, xt, yt, &bz1, &bz2);
			tmpVect->SetAtrDXY(dp++, CURVATR|BEZEATR, bz2.x1, bz2.y1);
			tmpVect->SetAtrDXY(dp++, BEZEATR, bz2.x2, bz2.y2);
			tmpVect->SetAtrDXY(dp++, BEZEATR, bz2.x3, bz2.y3);
			break;
		case ARCKIND:
			m_pcVect->GetDXY(p1, &(ac.x1), &(ac.y1));
			m_pcVect->GetDXY(p2, &(ac.x2), &(ac.y2));
			m_pcVect->GetDXY(p3, &(ac.x3), &(ac.y3));

			m_pcVect->GetDXY(p1, &x1, &y1);
			m_pcVect->GetDXY(p3, &x2, &y2);
			len1 = hypot(xx-x2, yy-y2);
			xt = (x2 - xx) * len2 / len1 + xx;
			yt = (y2 - yy) * len2 / len1 + yy;

			crossarcdiv(&ac, xt, yt, &ac1, &ac2);
			tmpVect->SetAtrDXY(dp++, CURVATR, ac2.x1, ac2.y1);
			tmpVect->SetAtrDXY(dp++, CURVATR|CONTATR, ac2.x2, ac2.y2);
			break;
		}
		np = m_pcVect->NextPointGet(pp);
		dp = AreaCopy(m_pcVect, np, pcnt-1, tmpVect, dp);

		dp = AreaCopy(m_pcVect, 0, pp, tmpVect, dp);

		switch(kind){
		case LINEKIND:
			m_pcVect->GetDXY(p1, &x1, &y1);
			m_pcVect->GetDXY(p2, &x2, &y2);
			len1 = hypot(xx-x1, yy-y1);
			xt = (x1 - xx) * len2 / len1 + xx;	 
			yt = (y1 - yy) * len2 / len1 + yy;

			tmpVect->SetAtrDXY(dp++, 0, xt, yt);
			break;
		case BEZEKIND:
			m_pcVect->GetDXY(p1, &x1, &y1);
			m_pcVect->GetDXY(p4, &x2, &y2);
			len1 = hypot(xx-x1, yy-y1);
			xt = (x1 - xx) * len2 / len1 + xx;
			yt = (y1 - yy) * len2 / len1 + yy;
			m_pcVect->SetAtrDXY(dp, 0, xt, yt);

			crossbezediv(&bz, xt, yt, &bz1, &bz2);
			tmpVect->SetAtrDXY(dp++, CURVATR|BEZEATR, bz1.x1, bz1.y1);
			tmpVect->SetAtrDXY(dp++, BEZEATR, bz1.x2, bz1.y2);
			tmpVect->SetAtrDXY(dp++, BEZEATR, bz1.x3, bz1.y3);
			tmpVect->SetAtrDXY(dp++, 0, bz1.x4, bz1.y4);
			break;
		case FCBEZEKIND:
			m_pcVect->GetDXY(p1, &x1, &y1);
			m_pcVect->GetDXY(p2, &x2, &y2);
			len1 = hypot(xx-x1, yy-y1);
			xt = (x1 - xx) * len2 / len1 + xx;
			yt = (y1 - yy) * len2 / len1 + yy;

			crossbezediv(&bz, xt, yt, &bz1, &bz2);
			tmpVect->SetAtrDXY(dp++, CURVATR|BEZEATR, bz1.x1, bz1.y1);
			tmpVect->SetAtrDXY(dp++, BEZEATR, bz1.x2, bz1.y2);
			tmpVect->SetAtrDXY(dp++, BEZEATR, bz1.x3, bz1.y3);
			tmpVect->SetAtrDXY(dp++, 0, bz1.x4, bz1.y4);
			break;
		case ARCKIND:
			m_pcVect->GetDXY(p1, &x1, &y1);
			m_pcVect->GetDXY(p3, &x2, &y2);
			len1 = hypot(xx-x1, yy-y1);
			xt = (x1 - xx) * len2 / len1 + xx;
			yt = (y1 - yy) * len2 / len1 + yy;

			crossarcdiv(&ac, xt, yt, &ac1, &ac2);
			tmpVect->SetAtrDXY(dp++, CURVATR, ac1.x1, ac1.y1);
			tmpVect->SetAtrDXY(dp++, CURVATR|CONTATR, ac1.x2, ac1.y2);
			tmpVect->SetAtrDXY(dp++, 0, ac1.x3, ac1.y3);
			break;
		}
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
		kind = m_pcVect->GetFiguKind(pp);
		switch(kind){
		case LINEKIND:
			m_pcVect->GetDXY(p1, &x1, &y1);
			m_pcVect->GetDXY(p2, &x2, &y2);
			len1 = hypot(xx-x2, yy-y2);
			xt = (x2 - xx) * len2 / len1 + xx;
			yt = (y2 - yy) * len2 / len1 + yy;

			m_pcVect2->SetAtrDXY(dp++, 0, xt, yt);
			break;
		case BEZEKIND:
			m_pcVect->GetDXY(p1, &(bz.x1), &(bz.y1));
			m_pcVect->GetDXY(p2, &(bz.x2), &(bz.y2));
			m_pcVect->GetDXY(p3, &(bz.x3), &(bz.y3));
			m_pcVect->GetDXY(p4, &(bz.x4), &(bz.y4));

			m_pcVect->GetDXY(p1, &x1, &y1);
			m_pcVect->GetDXY(p4, &x2, &y2);
			len1 = hypot(xx-x2, yy-y2);
			xt = (x2 - xx) * len2 / len1 + xx;
			yt = (y2 - yy) * len2 / len1 + yy;
			m_pcVect->SetAtrDXY(dp, 0, xt, yt);

			crossbezediv(&bz, xt, yt, &bz1, &bz2);
			m_pcVect2->SetAtrDXY(dp++, CURVATR|BEZEATR, bz2.x1, bz2.y1);
			m_pcVect2->SetAtrDXY(dp++, BEZEATR, bz2.x2, bz2.y2);
			m_pcVect2->SetAtrDXY(dp++, BEZEATR, bz2.x3, bz2.y3);
			break;
		case FCBEZEKIND:
			m_pcVect->GetFreeCurveBeze(p1, &bz);

			m_pcVect->GetDXY(p1, &x1, &y1);
			m_pcVect->GetDXY(p2, &x2, &y2);
			len1 = hypot(xx-x2, yy-y2);
			xt = (x2 - xx) * len2 / len1 + xx;
			yt = (y2 - yy) * len2 / len1 + yy;

			crossbezediv(&bz, xt, yt, &bz1, &bz2);
			m_pcVect2->SetAtrDXY(dp++, CURVATR|BEZEATR, bz2.x1, bz2.y1);
			m_pcVect2->SetAtrDXY(dp++, BEZEATR, bz2.x2, bz2.y2);
			m_pcVect2->SetAtrDXY(dp++, BEZEATR, bz2.x3, bz2.y3);
			break;
		case ARCKIND:
			m_pcVect->GetDXY(p1, &(ac.x1), &(ac.y1));
			m_pcVect->GetDXY(p2, &(ac.x2), &(ac.y2));
			m_pcVect->GetDXY(p3, &(ac.x3), &(ac.y3));

			m_pcVect->GetDXY(p1, &x1, &y1);
			m_pcVect->GetDXY(p3, &x2, &y2);
			len1 = hypot(xx-x2, yy-y2);
			xt = (x2 - xx) * len2 / len1 + xx;
			yt = (y2 - yy) * len2 / len1 + yy;
			m_pcVect->SetAtrDXY(dp, 0, xt, yt);

			crossarcdiv(&ac, xt, yt, &ac1, &ac2);
			m_pcVect2->SetAtrDXY(dp++, CURVATR, ac2.x1, ac2.y1);
			m_pcVect2->SetAtrDXY(dp++, CURVATR|CONTATR, ac2.x2, ac2.y2);
			break;
		}
		np = m_pcVect->NextPointGet(pp);
		dp = AreaCopy(m_pcVect, np, pcnt-1, m_pcVect2, dp);

		m_pcVect2->SetPointCount(dp);
		m_pcVect2->OffFlag(REJIONFLAG);

		dp = np;
		switch(kind){
		case LINEKIND:
			len1 = hypot(xx-x1, yy-y1);
			xt = (x1 - xx) * len2 / len1 + xx;	 
			yt = (y1 - yy) * len2 / len1 + yy;

			m_pcVect->SetAtrDXY(dp++, 0, xt, yt);
			break;
		case BEZEKIND:
			len1 = hypot(xx-x1, yy-y1);
			xt = (x1 - xx) * len2 / len1 + xx;
			yt = (y1 - yy) * len2 / len1 + yy;
			m_pcVect->SetAtrDXY(dp, 0, xt, yt);

			crossbezediv(&bz, xt, yt, &bz1, &bz2);
			m_pcVect->SetAtrDXY(dp++, CURVATR|BEZEATR, bz1.x1, bz1.y1);
			m_pcVect->SetAtrDXY(dp++, BEZEATR, bz1.x2, bz1.y2);
			m_pcVect->SetAtrDXY(dp++, BEZEATR, bz1.x3, bz1.y3);
			m_pcVect->SetAtrDXY(dp++, 0, bz1.x4, bz1.y4);
			break;
		case FCBEZEKIND:
			len1 = hypot(xx-x1, yy-y1);
			xt = (x1 - xx) * len2 / len1 + xx;
			yt = (y1 - yy) * len2 / len1 + yy;

			crossbezediv(&bz, xt, yt, &bz1, &bz2);
			m_pcVect->SetAtrDXY(dp++, CURVATR|BEZEATR, bz1.x1, bz1.y1);
			m_pcVect->SetAtrDXY(dp++, BEZEATR, bz1.x2, bz1.y2);
			m_pcVect->SetAtrDXY(dp++, BEZEATR, bz1.x3, bz1.y3);
			m_pcVect->SetAtrDXY(dp++, 0, bz1.x4, bz1.y4);
			break;
		case ARCKIND:
			len1 = hypot(xx-x1, yy-y1);
			xt = (x1 - xx) * len2 / len1 + xx;
			yt = (y1 - yy) * len2 / len1 + yy;
			m_pcVect->SetAtrDXY(dp, 0, xt, yt);

			crossarcdiv(&ac, xt, yt, &ac1, &ac2);
			m_pcVect->SetAtrDXY(dp++, CURVATR, ac1.x1, ac1.y1);
			m_pcVect->SetAtrDXY(dp++, CURVATR|CONTATR, ac1.x2, ac1.y2);
			m_pcVect->SetAtrDXY(dp++, 0, ac1.x3, ac1.y3);
			break;
		}
		m_pcVect->SetPointCount(dp);
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
	int		k1s, k1e, k2s, k2e;

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
	int		k1s, k1e, k2s, k2e;

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
	int		k1s, k1e, k2s, k2e;

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
	int		pp1, pp2, tpp;
	int		dp;
	DBL		xx1, xx2, txx;
	DBL		yy1, yy2, tyy;
	CVect	*tmpVect;
	dbezetype	bz1, bz11, bz12;
	darctype	ac1, ac11, ac12;
	int		p11, p12, p13, p14;
	int		kind1, np1;
	dbezetype	bz2, bz21, bz22;
	darctype	ac2, ac21, ac22;
	int		p21, p22, p23, p24;
	int		kind2, np2;
	DBL		x1, y1;
	DBL		x2, y2;
	DBL		xt, yt;
	DBL		len1, len2;

	len2 = mm / 2.0;

	xx1 = crs1.xx; yy1 = crs1.yy; pp1 = crs1.pp;
	xx2 = crs2.xx; yy2 = crs2.yy; pp2 = crs2.pp;
	m_pcVect = ((CVect*)crs1.pcData);
	pcnt = m_pcVect->GetPointCount();

	m_pcVect2 = new CVect(BASEVECTID);
	if(pp1 > pp2){
		tpp = pp1; txx = xx1; tyy = yy1;
		pp1 = pp2; xx1 = xx2; yy1 = yy2;
		pp2 = tpp; xx2 = txx; yy2 = tyy;
	}
	dp = 0;

	//dp = SetStartDivFigu(m_pcVect, pp1, crs1, m_pcVect2, dp, mm);

	p11 = m_pcVect->PointLegal(pcnt, pp1+0);
	p12 = m_pcVect->PointLegal(pcnt, pp1+1);
	p13 = m_pcVect->PointLegal(pcnt, pp1+2);
	p14 = m_pcVect->PointLegal(pcnt, pp1+3);
	kind1 = m_pcVect->GetFiguKind(pp1);
	switch(kind1){
	case LINEKIND:
		m_pcVect->GetDXY(p11, &x1, &y1);
		m_pcVect->GetDXY(p12, &x2, &y2);
		len1 = hypot(xx1-x2, yy1-y2);
		xt = (x2 - xx1) * len2 / len1 + xx1;
		yt = (y2 - yy1) * len2 / len1 + yy1;

		m_pcVect2->SetAtrDXY(dp++, 0, xt, yt);
		break;
	case BEZEKIND:
		m_pcVect->GetDXY(p11, &(bz1.x1), &(bz1.y1));
		m_pcVect->GetDXY(p12, &(bz1.x2), &(bz1.y2));
		m_pcVect->GetDXY(p13, &(bz1.x3), &(bz1.y3));
		m_pcVect->GetDXY(p14, &(bz1.x4), &(bz1.y4));

		m_pcVect->GetDXY(p11, &x1, &y1);
		m_pcVect->GetDXY(p14, &x2, &y2);
		len1 = hypot(xx1-x2, yy1-y2);
		xt = (x2 - xx1) * len2 / len1 + xx1;
		yt = (y2 - yy1) * len2 / len1 + yy1;

		crossbezediv(&bz1, xt, yt, &bz11, &bz12);
		m_pcVect2->SetAtrDXY(dp++, CURVATR|BEZEATR, bz12.x1, bz12.y1);
		m_pcVect2->SetAtrDXY(dp++, BEZEATR, bz12.x2, bz12.y2);
		m_pcVect2->SetAtrDXY(dp++, BEZEATR, bz12.x3, bz12.y3);
		break;
	case FCBEZEKIND:
		m_pcVect->GetFreeCurveBeze(pp1, &bz1);

		m_pcVect->GetDXY(p11, &x1, &y1);
		m_pcVect->GetDXY(p12, &x2, &y2);
		len1 = hypot(xx1-x2, yy1-y2);
		xt = (x2 - xx1) * len2 / len1 + xx1;
		yt = (y2 - yy1) * len2 / len1 + yy1;

		crossbezediv(&bz1, xt, yt, &bz11, &bz12);
		m_pcVect2->SetAtrDXY(dp++, CURVATR|BEZEATR, bz12.x1, bz12.y1);
		m_pcVect2->SetAtrDXY(dp++, BEZEATR, bz12.x2, bz12.y2);
		m_pcVect2->SetAtrDXY(dp++, BEZEATR, bz12.x3, bz12.y3);
		break;
	case ARCKIND:
		m_pcVect->GetDXY(p11, &(ac1.x1), &(ac1.y1));
		m_pcVect->GetDXY(p12, &(ac1.x2), &(ac1.y2));
		m_pcVect->GetDXY(p13, &(ac1.x3), &(ac1.y3));

		m_pcVect->GetDXY(p11, &x1, &y1);
		m_pcVect->GetDXY(p13, &x2, &y2);
		len1 = hypot(xx1-x2, yy1-y2);
		xt = (x2 - xx1) * len2 / len1 + xx1;
		yt = (y2 - yy1) * len2 / len1 + yy1;

		crossarcdiv(&ac1, xt, yt, &ac11, &ac12);
		m_pcVect2->SetAtrDXY(dp++, CURVATR, ac12.x1, ac12.y1);
		m_pcVect2->SetAtrDXY(dp++, CURVATR|CONTATR, ac12.x2, ac12.y2);
		break;
	}

	np1 = m_pcVect->NextPointGet(pp1);

	dp = AreaCopy(m_pcVect, np1, pp2, m_pcVect2, dp);

	p21 = m_pcVect->PointLegal(pcnt, pp2+0);
	p22 = m_pcVect->PointLegal(pcnt, pp2+1);
	p23 = m_pcVect->PointLegal(pcnt, pp2+2);
	p24 = m_pcVect->PointLegal(pcnt, pp2+3);
	kind2 = m_pcVect->GetFiguKind(pp2);
	switch(kind2){
	case LINEKIND:
		m_pcVect->GetDXY(p21, &x1, &y1);
		m_pcVect->GetDXY(p22, &x2, &y2);
		len1 = hypot(xx2-x1, yy2-y1);
		xt = (x1 - xx2) * len2 / len1 + xx2;
		yt = (y1 - yy2) * len2 / len1 + yy2;

		m_pcVect2->SetAtrDXY(dp, 0, xt, yt);
		dp++;
		break;
	case BEZEKIND:
		m_pcVect->GetDXY(p21, &(bz2.x1), &(bz2.y1));
		m_pcVect->GetDXY(p22, &(bz2.x2), &(bz2.y2));
		m_pcVect->GetDXY(p23, &(bz2.x3), &(bz2.y3));
		m_pcVect->GetDXY(p24, &(bz2.x4), &(bz2.y4));

		m_pcVect->GetDXY(p21, &x1, &y1);
		m_pcVect->GetDXY(p24, &x2, &y2);
		len1 = hypot(xx2-x1, yy2-y1);
		xt = (x1 - xx2) * len2 / len1 + xx2;
		yt = (y1 - yy2) * len2 / len1 + yy2;

		crossbezediv(&bz2, xt, yt, &bz21, &bz22);
		m_pcVect2->SetAtrDXY(dp++, CURVATR|BEZEATR, bz21.x1, bz21.y1);
		m_pcVect2->SetAtrDXY(dp++, BEZEATR, bz21.x2, bz21.y2);
		m_pcVect2->SetAtrDXY(dp++, BEZEATR, bz21.x3, bz21.y3);
		m_pcVect2->SetAtrDXY(dp++, 0, bz21.x4, bz21.y4);
		break;
	case FCBEZEKIND:
		m_pcVect->GetFreeCurveBeze(pp2, &bz2);

		m_pcVect->GetDXY(p21, &x1, &y1);
		m_pcVect->GetDXY(p22, &x2, &y2);
		len1 = hypot(xx2-x1, yy2-y1);
		xt = (x1 - xx2) * len2 / len1 + xx2;
		yt = (y1 - yy2) * len2 / len1 + yy2;

		crossbezediv(&bz2, xt, yt, &bz21, &bz22);
		m_pcVect2->SetAtrDXY(dp++, CURVATR|BEZEATR, bz21.x1, bz21.y1);
		m_pcVect2->SetAtrDXY(dp++, BEZEATR, bz21.x2, bz21.y2);
		m_pcVect2->SetAtrDXY(dp++, BEZEATR, bz21.x3, bz21.y3);
		m_pcVect2->SetAtrDXY(dp++, 0, bz21.x4, bz21.y4);
		break;
	case ARCKIND:
		m_pcVect->GetDXY(p21, &(ac2.x1), &(ac2.y1));
		m_pcVect->GetDXY(p22, &(ac2.x2), &(ac2.y2));
		m_pcVect->GetDXY(p23, &(ac2.x3), &(ac2.y3));

		m_pcVect->GetDXY(p21, &x1, &y1);
		m_pcVect->GetDXY(p23, &x2, &y2);
		len1 = hypot(xx2-x1, yy2-y1);
		xt = (x1 - xx2) * len2 / len1 + xx2;
		yt = (y1 - yy2) * len2 / len1 + yy2;

		crossarcdiv(&ac2, xt, yt, &ac21, &ac22);
		m_pcVect2->SetAtrDXY(dp++, CURVATR, ac21.x1, ac21.y1);
		m_pcVect2->SetAtrDXY(dp++, CURVATR|CONTATR, ac21.x2, ac21.y2);
		m_pcVect2->SetAtrDXY(dp++, 0, ac21.x3, ac21.y3);
		break;
	}
	np2 = m_pcVect->NextPointGet(pp2);
	m_pcVect2->SetPointCount(dp);
	m_pcVect2->OnFlag(REJIONFLAG);

	tmpVect = new CVect(BASEVECTID);

	dp = 0;
	switch(kind2){
	case LINEKIND:
		m_pcVect->GetDXY(p21, &x1, &y1);
		m_pcVect->GetDXY(p22, &x2, &y2);
		len1 = hypot(xx2-x2, yy2-y2);
		xt = (x2 - xx2) * len2 / len1 + xx2;
		yt = (y2 - yy2) * len2 / len1 + yy2;

		tmpVect->SetAtrDXY(dp++, 0, xt, yt);
		break;
	case BEZEKIND:
		m_pcVect->GetDXY(p21, &x1, &y1);
		m_pcVect->GetDXY(p24, &x2, &y2);
		len1 = hypot(xx2-x2, yy2-y2);
		xt = (x2 - xx2) * len2 / len1 + xx2;
		yt = (y2 - yy2) * len2 / len1 + yy2;

		crossbezediv(&bz2, xt, yt, &bz21, &bz22);
		tmpVect->SetAtrDXY(dp++, CURVATR|BEZEATR, bz22.x1, bz22.y1);
		tmpVect->SetAtrDXY(dp++, BEZEATR, bz22.x2, bz22.y2);
		tmpVect->SetAtrDXY(dp++, BEZEATR, bz22.x3, bz22.y3);
		break;
	case FCBEZEKIND:

		m_pcVect->GetDXY(p21, &x1, &y1);
		m_pcVect->GetDXY(p22, &x2, &y2);
		len1 = hypot(xx2-x2, yy2-y2);
		xt = (x2 - xx2) * len2 / len1 + xx2;
		yt = (y2 - yy2) * len2 / len1 + yy2;

		crossbezediv(&bz2, xt, yt, &bz21, &bz22);
		tmpVect->SetAtrDXY(dp++, CURVATR|BEZEATR, bz22.x1, bz22.y1);
		tmpVect->SetAtrDXY(dp++, BEZEATR, bz22.x2, bz22.y2);
		tmpVect->SetAtrDXY(dp++, BEZEATR, bz22.x3, bz22.y3);
		break;
	case ARCKIND:
		m_pcVect->GetDXY(p21, &x1, &y1);
		m_pcVect->GetDXY(p23, &x2, &y2);
		len1 = hypot(xx2-x2, yy2-y2);
		xt = (x2 - xx2) * len2 / len1 + xx2;
		yt = (y2 - yy2) * len2 / len1 + yy2;

		crossarcdiv(&ac2, xt, yt, &ac21, &ac22);
		tmpVect->SetAtrDXY(dp++, CURVATR, ac22.x1, ac22.y1);
		tmpVect->SetAtrDXY(dp++, CURVATR|CONTATR, ac22.x2, ac22.y2);
		break;
	}

	dp = AreaCopy(m_pcVect, np2, pcnt-1, tmpVect, dp);
	dp = AreaCopy(m_pcVect, 0, pp1, tmpVect, dp);

	switch(kind1){
	case LINEKIND:
		m_pcVect->GetDXY(p11, &x1, &y1);
		m_pcVect->GetDXY(p12, &x2, &y2);
		len1 = hypot(xx1-x1, yy1-y1);
		xt = (x1 - xx1) * len2 / len1 + xx1;
		yt = (y1 - yy1) * len2 / len1 + yy1;

		tmpVect->SetAtrDXY(dp++, 0, xt, yt);
		break;
	case BEZEKIND:
		m_pcVect->GetDXY(p11, &x1, &y1);
		m_pcVect->GetDXY(p14, &x2, &y2);
		len1 = hypot(xx1-x1, yy1-y1);
		xt = (x1 - xx1) * len2 / len1 + xx1;
		yt = (y1 - yy1) * len2 / len1 + yy1;

		crossbezediv(&bz1, xt, yt, &bz11, &bz12);
		tmpVect->SetAtrDXY(dp++, CURVATR|BEZEATR, bz11.x1, bz11.y1);
		tmpVect->SetAtrDXY(dp++, BEZEATR, bz11.x2, bz11.y2);
		tmpVect->SetAtrDXY(dp++, BEZEATR, bz11.x3, bz11.y3);
		tmpVect->SetAtrDXY(dp++, 0, bz11.x4, bz11.y4);
		break;
	case FCBEZEKIND:
		m_pcVect->GetDXY(p11, &x1, &y1);
		m_pcVect->GetDXY(p12, &x2, &y2);
		len1 = hypot(xx1-x1, yy1-y1);
		xt = (x1 - xx1) * len2 / len1 + xx1;
		yt = (y1 - yy1) * len2 / len1 + yy1;

		crossbezediv(&bz1, xt, yt, &bz11, &bz12);
		tmpVect->SetAtrDXY(dp++, CURVATR|BEZEATR, bz11.x1, bz11.y1);
		tmpVect->SetAtrDXY(dp++, BEZEATR, bz11.x2, bz11.y2);
		tmpVect->SetAtrDXY(dp++, BEZEATR, bz11.x3, bz11.y3);
		tmpVect->SetAtrDXY(dp++, 0, bz11.x4, bz11.y4);
		break;
	case ARCKIND:
		m_pcVect->GetDXY(p11, &x1, &y1);
		m_pcVect->GetDXY(p13, &x2, &y2);
		len1 = hypot(xx1-x1, yy1-y1);
		xt = (x1 - xx1) * len2 / len1 + xx1;
		yt = (y1 - yy1) * len2 / len1 + yy1;

		crossarcdiv(&ac1, xt, yt, &ac11, &ac12);
		tmpVect->SetAtrDXY(dp++, CURVATR, ac11.x1, ac11.y1);
		tmpVect->SetAtrDXY(dp++, CURVATR|CONTATR, ac11.x2, ac11.y2);
		tmpVect->SetAtrDXY(dp++, 0, ac11.x3, ac11.y3);
		break;
	}

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
		HlpMsgDsp("始点を指定してください");
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
				HlpMsgDsp("終点を指定してください");
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
				HlpMsgDsp("始点終点を確定してください");
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
			HlpMsgDsp("始点を指定してください");
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
		HlpMsgDsp("右ボタンで反転、左ボタンで確定");
		m_nMouseSetCount = 0;
		BeginDraw();
		DrawModeSET();
		m_pcVect->DelDraw(m_pcDC);
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
		DrawModeSET();
		m_pcVect->DelDraw(m_pcDC);
		HalfArcRevace();
		scrnsetfigupen(m_pcDC->m_hDC);
		m_pcVect->RenewMiniMax();
		m_pcVect->PLDraw(m_pcDC);
		scrnpenend(m_pcDC->m_hDC);
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
		HlpMsgDsp("中点を指定してください、右ボタンで確定です");
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
			HlpMsgDsp("中点を確定してください、右ボタンで確定です");
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawSelectArc(m_pcDC);
			ArcMovePoint(dx, dy);
			DrawSelectArc(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
			HlpMsgDsp("中点を指定してください、右ボタンで確定です");
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
		DrawModeSET();
		m_pcData = m_pcDoc->UseFitingVect(m_pcVect);
		if(m_pcData != NULL){
			m_pcData->DelDraw(m_pcDC);
			m_pcData->StrgFiting();
			m_pcData->RenewMiniMax();
			ScrnDataRedraw(m_pcData);
		}
		m_pcCopyVect->DelDraw(m_pcDC);
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
		HlpMsgDsp("制御点を指定してください、右ボタンで確定です");
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			BeginDraw();
			DrawSelectBeze(m_pcDC);
			BezeSelectPoint(dx, dy);
			DrawSelectBeze(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
			HlpMsgDsp("制御点を確定してください、右ボタンで確定です");
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
		DrawModeSET();
		m_pcVect->RenewMiniMax();
		m_pcData = m_pcDoc->UseFitingVect(m_pcVect);
		if(m_pcData != NULL){
			m_pcData->DelDraw(m_pcDC);
			m_pcData->StrgFiting();
			m_pcData->RenewMiniMax();
			ScrnDataRedraw(m_pcData);
		}
		m_pcCopyVect->DelDraw(m_pcDC);
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
		atr = 0;
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
		atr = 0;
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
}

int CCmdLine::ReadLineDivCount()
{
	CDialogInputNum	*pDialogInputNum;
	int			ndivcnt;

	ndivcnt = 3;
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle("線分割個数設定");
	pDialogInputNum->SetUnitText("個");
	pDialogInputNum->SetTitleText("分割数");
	pDialogInputNum->m_dInputNum = ndivcnt;
	if(pDialogInputNum->DoModal() == IDOK){
		ndivcnt = pDialogInputNum->m_dInputNum;
	}
	else{
		ndivcnt = 0;
	}
	delete(pDialogInputNum);
	return(ndivcnt);
}

void CCmdLine::LineDivMain(int ndivcnt)
{
	CTonDoc*	pcDoc;
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

