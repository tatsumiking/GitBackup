
/////////////////////////////////////////////////////////////////////////////
// CCmdSlct

#include "stdafx.h"
#include "math.h"

#include "cmd_base.h"
#include "cmd_slct.h"

#define	NOOPMODE	0
#define	SLCTMODE	100
#define	MOVEMODE	200
#define TRNSMODE	300
#define	SLCTMOVEMODE	400
#define ROUNDMODE	500

extern "C" {
	void scrnsetslctpen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	void scrndlenrltvtomm(LPDBL len);
	void scrndlenmmtorltv(LPDBL len);
	void senvgetareadotsize(int *ndot);
	void scrndxymmtorltv(LPDBL x, LPDBL y);
	int	senvsetdrawmode(int mode);
	void curveroundtrns(DBL srcx, DBL srcy, DBL x0, DBL y0, DBL rag, DBL *dstx, DBL *dsty);
}

CCmdSlct::CCmdSlct(CScrollView* pcview)
:CCmdBase(pcview)
{
	m_pcCmdSWak = NULL;
}

UINT CCmdSlct::ExecObj(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;
	DBL		minx, miny, maxx, maxy;
	int		bmode;
	
	if(event == SETEVENT2){
		event = SETEVENT;
	}
	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
	case SETEVENT2:
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
			if(CheckNearPoint(m_dSX, m_dSY, m_dEX, m_dEY) == TRUE){
			// データ指定
				bmode = senvsetdrawmode(POINTVIEW);
				pcData = m_pcDoc->SrchData(0, dx, dy);
				senvsetdrawmode(bmode);
				if(pcData != NULL){
					if(m_pcView->CheckKeyFlag(MK_SHIFT) == FALSE){
						m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
						m_pcDoc->OffFlag(ACTIVEFLAG);
					}
					m_pcDoc->GetDMiniMax(ACTIVEFLAG, &minx, &miny, &maxx, &maxy);
					if(pcData->CheckFlag(ACTIVEFLAG) == ON){
						pcData->OffFlag(ACTIVEFLAG);
					}
					else{
						pcData->OnFlag(ACTIVEFLAG);
					}
					DispSelectDataSize();
				}
			}
			else{
				// エリア指定
				if(m_pcView->CheckKeyFlag(MK_SHIFT) == FALSE){
					m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
					m_pcDoc->OffFlag(ACTIVEFLAG);
				}
				m_pcDoc->AreaInFlagOn(ACTIVEFLAG, m_dSX, m_dSY, m_dEX, m_dEY);
				DispSelectDataSize();
				ScrnAllRedraw();
			}
		}
		break;
	case MOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			m_dEX = dx; m_dEY = dy;
			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		break;
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		m_nMouseSetCount = 0;
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		m_nMouseSetCount = 0;
		break;
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		m_nMouseSetCount = 0;
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdSlct::ExecArea(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		InitClass();
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			// エリア指定矩形開始点をセット
			m_nMouseSetCount = 1;
			m_dSX = dx; m_dSY = dy;
			m_dEX = dx; m_dEY = dy;
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		else if(m_nMouseSetCount == 1){
			// エリア指定矩形終了点をセット
			m_nMouseSetCount = 0;
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
			m_dEX = dx; m_dEY = dy;
			if(m_dEX == m_dSX || m_dEY == m_dSY){
				if(((CTonView*)m_pcView)->CheckKeyFlag(MK_SHIFT) == FALSE){
					// 指定データを非指定状態に変更
					m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
					m_pcDoc->OffFlag(ACTIVEFLAG);
					DispSelectDataSize();
					ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
				}
				
				pcData = m_pcDoc->SrchData(0, dx, dy);
				if(pcData != NULL){
					pcData->OnFlag(ACTIVEFLAG);
					DispSelectDataSize();
					m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dSX, &m_dSY, &m_dEX, &m_dEY);
					ScrnAreaRedraw(m_dSX, m_dSY, m_dEX, m_dEY);
				}
			}
			else{
				if(((CTonView*)m_pcView)->CheckKeyFlag(MK_SHIFT) == FALSE){
					m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
					m_pcDoc->OffFlag(ACTIVEFLAG);
					ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
				}
				m_pcDoc->AreaInFlagOn(ACTIVEFLAG, m_dSX, m_dSY, m_dEX, m_dEY);
				DispSelectDataSize();
				m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dSX, &m_dSY, &m_dEX, &m_dEY);
				ScrnAreaRedraw(m_dSX, m_dSY, m_dEX, m_dEY);
			}
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
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdSlct::ExecAreaSlctMove(UINT event, DBL dx, DBL dy)
{
	int		ret;

	if(event == ENDEVENT){
		ret = 0;
	}
	if(event == SETEVENT2){
		event = SETEVENT;
	}
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		m_nLocalCommand = SLCTMOVEMODE;
		InitClass();
		SetSWakVectAtr();
		m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dSX, &m_dSY, &m_dEX, &m_dEY);
		SetSlctRoundAreaPoint();
		BeginDraw();
		DrawAtnRoundBoxs(m_pcDC);
		EndDraw();
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_nLocalCommand = SetLocalCommand(dx, dy);
		}
		break;
	case UPMOVEEVENT:
		SetCuserKind(dx, dy);
		break;
		//return(0);
	case DLGSETEVENT:
		ResetSWakVectAtr();
		ScrnAllRedraw();
		return(0);
	}
	switch(m_nLocalCommand){
	case SLCTMODE:
		ret = ExecAreaSlct(event, dx, dy);
		break;
	case MOVEMODE:
	case SLCTMOVEMODE:
		ret = ExecAreaMove(event, dx, dy);
		break;
	case TRNSMODE:
		ret = ExecAreaTrns(event, dx, dy);
		break;
	case ROUNDMODE:
		ret = ExecAreaRound(event, dx, dy);
		break;
	case NOOPMODE:
		ret = 0;
		if(event == ENDEVENT){
			ret = ENDEVENT;
		}
		break;
	default:
		ret = ENDEVENT;
		break;
	}
	return(ret);
}

UINT CCmdSlct::ExecPart(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
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

UINT CCmdSlct::ExecAll()
{
	CTonDoc	*pcDoc;

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->OnFlag(ACTIVEFLAG);
	DispSelectDataSize();
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdSlct::ExecChng()
{
	CTonDoc	*pcDoc;

	pcDoc = (CTonDoc*)m_pcView->GetDocument();

	pcDoc->OffFlag(ATNTION1FLAG);
	pcDoc->CheckOnFlag(ACTIVEFLAG, ATNTION1FLAG);
	pcDoc->OnFlag(ACTIVEFLAG);
	pcDoc->CheckOffFlag(ATNTION1FLAG, ACTIVEFLAG);
	pcDoc->OffFlag(ATNTION1FLAG);
	DispSelectDataSize();
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdSlct::ExecLock()
{
	CTonDoc	*pcDoc;
	CData* pData;
	POSITION pos;

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pos = pcDoc->m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = pcDoc->m_pcObjDataList->GetNext(pos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			pData->OffFlag(ACTIVEFLAG);
			pData->OnFlag(VSUBFLAG);
		}
	}
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdSlct::ExecUnlock()
{
	CTonDoc	*pcDoc;
	CData* pData;
	POSITION pos;

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pos = pcDoc->m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = pcDoc->m_pcObjDataList->GetNext(pos);
		if(pData->CheckFlag(VSUBFLAG) == ON){
			pData->OffFlag(VSUBFLAG);
		}
	}
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdSlct::ExecAreaSlct(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		InitClass();
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			// エリア指定矩形開始点をセット
			BeginDraw();
			DrawAtnRoundBoxs(m_pcDC);
			EndDraw();
			m_pcView->StartTimer();
			m_nMouseSetCount = 1;
			m_dSX = dx; m_dSY = dy;
			m_dEX = dx; m_dEY = dy;
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		else if(m_nMouseSetCount == 1){
			// エリア指定矩形終了点をセット
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
			m_dEX = dx; m_dEY = dy;
			if(m_pcView->EndTimer() <= 1){	//ドラッグしなかった時
				if(((CTonView*)m_pcView)->CheckKeyFlag(MK_SHIFT) == FALSE){
					m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
					m_pcDoc->OffFlag(ACTIVEFLAG);
					ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY); 
				}
				pcData = m_pcDoc->SrchData(0, dx, dy);
				if(pcData != NULL){
					pcData->OnFlag(ACTIVEFLAG);
				}
				SetSWakVectAtr();
				m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dSX, &m_dSY, &m_dEX, &m_dEY);
				SetSlctRoundAreaPoint();
				DispSelectDataSize();
				ScrnAreaRedraw(m_dSX, m_dSY, m_dEX, m_dEY);
				ScrnAllRedraw();
			}
			else{
				if(m_dSX > m_dEX){
					dx = m_dSX;
					m_dSX = m_dEX;
					m_dEX = dx;
				}
				if(m_dSY < m_dEY){
					dy = m_dSY;
					m_dSY = m_dEY;
					m_dEY = dy;
				}
				if(((CTonView*)m_pcView)->CheckKeyFlag(MK_SHIFT) == FALSE){
					m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
					m_pcDoc->OffFlag(ACTIVEFLAG);
					ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY); 
				}
				m_pcDoc->AreaInFlagOnOff(ACTIVEFLAG, m_dSX, m_dSY, m_dEX, m_dEY);
				m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dSX, &m_dSY, &m_dEX, &m_dEY);
				SetSlctRoundAreaPoint();
				SetSWakVectAtr();
				DispSelectDataSize();
				ScrnAreaRedraw(m_dSX, m_dSY, m_dEX, m_dEY);
				ScrnAllRedraw();
			}
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			m_dEX = dx; m_dEY = dy;
			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		break;
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		break;
	case REDRAWEVENT:
		BeginDraw();
		DrawAtnRoundBoxs(m_pcDC);
		EndDraw();
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		break;
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			DrawAtnRoundBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdSlct::ExecAreaMove(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;
	DBL		movex, movey;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		InitDataAreaBoxs();
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			BeginDraw();
			DrawAtnRoundBoxs(m_pcDC);
			EndDraw();
			m_pcView->StartTimer();
			m_pcView->m_nCursorKind = CUSKIND_SIZEALL;
			::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
			InitDataAreaBoxs();
			m_dCX = dx; m_dCY = dy;
			m_dMBX = dx; m_dMBY = dy;
			m_nMouseSetCount = 1;
			BeginDraw();
			DrawAtnRoundBoxs(m_pcDC);
			EndDraw();
		}
		else{
			BeginDraw();
			DrawAtnRoundBoxs(m_pcDC);
			EndDraw();
			if(m_pcView->EndTimer() <= 1){	//ドラッグしなかった時
				// トルグで指定非指定を処理
				if(((CTonView*)m_pcView)->CheckKeyFlag(MK_SHIFT) == TRUE){
					m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
					m_pcDoc->OffFlag(ACTIVEFLAG);
					ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
				}
				pcData = m_pcDoc->SrchData(0, dx, dy);
				if(pcData != NULL){
					pcData->OnFlag(ACTIVEFLAG);
				}
				SetSWakVectAtr();
				m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dSX, &m_dSY, &m_dEX, &m_dEY);
				SetSlctRoundAreaPoint();
				ScrnAreaRedraw(m_dSX, m_dSY, m_dEX, m_dEY);
				ScrnAllRedraw();
				m_nMouseSetCount = 0;
				return(0);
			}
			movex = dx - m_dMBX;
			movey = dy - m_dMBY;
			SaveUndo();
			m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
			if(((CTonView*)m_pcView)->CheckKeyFlag(MK_SHIFT) == TRUE){
				if(fabs(movex) > fabs(movey)){
					movey = 0;
				}
				else{
					movex = 0;
				}
			}
			m_pcDoc->MoveTrns(movex, movey);
			SetSWakLocate();
			m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dSX, &m_dSY, &m_dEX, &m_dEY);
			SetSlctRoundAreaPoint();
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnAreaRedraw(m_dSX, m_dSY, m_dEX, m_dEY);
			m_dBSX = m_dSX; m_dBSY = m_dSY;
			m_dBEX = m_dEX; m_dBEY = m_dEY;
			ScrnAllRedraw();
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
		if(m_nMouseSetCount == 1){
			if(((CTonView*)m_pcView)->CheckKeyFlag(MK_SHIFT) == TRUE){
				movex = dx - m_dMBX;
				movey = dy - m_dMBY;
				if(fabs(movex) > fabs(movey)){
					dy = m_dMBY;
				}
				else{
					dx = m_dMBX;
				}
			}
			movex = dx - m_dCX;
			movey = dy - m_dCY;
			if(movex == 0 && movey == 0){
				return(0);
			}
			BeginDraw();
			DrawAtnRoundBoxs(m_pcDC);
			m_dEX += movex; m_dSX += movex;
			m_dEY += movey; m_dSY += movey;
			m_dCX = dx; m_dCY = dy;
			SetSlctRoundAreaPoint();

			DrawAtnRoundBoxs(m_pcDC);
			EndDraw();
		}
		break;
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnRoundBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		break;
	case REDRAWEVENT:
		BeginDraw();
		DrawAtnRoundBoxs(m_pcDC);
		EndDraw();
		break;
	case RESETEVENT:
		break;
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnRoundBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdSlct::ExecAreaTrns(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			BeginDraw();
			DrawAtnRoundBoxs(m_pcDC);
			EndDraw();
			InitDataAreaBoxs();
			m_dBSX = m_dSX; m_dBSY = m_dSY;
			m_dBEX = m_dEX; m_dBEY = m_dEY;
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
			CntrKindMoveNew(dx, dy);
			SaveUndo();
			MainSizeTrns();
			SetSWakLocate();
			m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dSX, &m_dSY, &m_dEX, &m_dEY);
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnAreaRedraw(m_dSX, m_dSY, m_dEX, m_dEY);
			m_dBSX = m_dSX; m_dBSY = m_dSY;
			m_dBEX = m_dEX; m_dBEY = m_dEY;
			m_nMouseSetCount = 0;
			ScrnAllRedraw();
		}
		break;
	case MOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			CntrKindMoveNew(dx, dy);
			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		break;
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		m_nMouseSetCount = 0;
		break;
	case REDRAWEVENT:
		BeginDraw();
		DrawAtnBoxs(m_pcDC);
		EndDraw();
		break;
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			DrawAtnRoundBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdSlct::ExecAreaRound(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			BeginDraw();
			DrawAtnRoundBoxs(m_pcDC);
			EndDraw();
			SetSlctRoundAreaPoint();
			m_dBSX = m_dSX; m_dBSY = m_dSY;
			m_dBEX = m_dEX; m_dBEY = m_dEY;
			BeginDraw();
			DrawAtnRoundBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 1;
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnRoundBoxs(m_pcDC);
			EndDraw();
			RoundSlctArea(dx, dy);
			SaveUndo();
			MainRoundTrns();
			SetSWakLocate();
			ScrnAllRedraw();
			m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dSX, &m_dSY, &m_dEX, &m_dEY);
			SetSlctRoundAreaPoint();
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnRoundBoxs(m_pcDC);
			RoundSlctArea(dx, dy);
			DrawAtnRoundBoxs(m_pcDC);
			EndDraw();
		}
		break;
	case UPMOVEEVENT:
		m_nMouseSetCount = 0;
		break;
	case REDRAWEVENT:
		BeginDraw();
		DrawAtnRoundBoxs(m_pcDC);
		EndDraw();
		break;
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			DrawAtnRoundBoxs(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

void CCmdSlct::MainSizeTrns()
{
	CTonDoc*	pcDoc;
	DBL		movex, movey;
	DBL		cx, cy;
	DBL		timex, timey;

	movex = m_dSX - m_dBSX;
	movey = m_dSY - m_dBSY;
	cx = m_dSX;
	cy = m_dSY;
	timex = (m_dEX - m_dSX) / (m_dBEX - m_dBSX);
	timey = (m_dEY - m_dSY) / (m_dBEY - m_dBSY);
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->MoveTrns(movex, movey);
	pcDoc->SizeTrns(cx, cy, timex, timey);
}

void CCmdSlct::DispSelectDataSize()
{
	CTonDoc *pcDoc;
	DBL		minx, miny, maxx, maxy;
	DBL		sizex, sizey;
	char	msgbuf[HRECMAX];

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDoc->GetDMiniMax(ACTIVEFLAG, &minx, &miny, &maxx, &maxy);
	sizex = maxx - minx;
	sizey = maxy - miny;
	if(sizex < 0){
		sizex = 0;
	}
	if(sizex > 1000000){
		sizex = 1000000;
	}
	if(sizey < 0){
		sizey = 0;
	}
	if(sizey > 1000000){
		sizey = 1000000;
	}
	sprintf(msgbuf, "ｻｲｽﾞ%4.1lfmm %4.1lfmm", sizex, sizey);
	HlpMsgDsp(3, msgbuf);
}

int CCmdSlct::SetLocalCommand(DBL dx, DBL dy)
{
	DBL		dlen;
	int		ndot;

	senvgetareadotsize(&ndot);
	dlen = CntrKindInitNew(dx, dy);
	scrndlenmmtorltv(&dlen);
	if(dlen < (ndot+4)){
		return(TRNSMODE);
	}
	if(m_dSX < dx && dx < m_dEX
	&& m_dSY < dy && dy < m_dEY){
		return(MOVEMODE);
	}
	dlen = hypot(m_dREX - dx, m_dREY - dy);
	scrndlenmmtorltv(&dlen);
	if(dlen < 16){
		return(ROUNDMODE);
	}
	return(SLCTMODE);
}

void CCmdSlct::SetCuserKind(DBL dx, DBL dy)
{
	DBL		dlen;
	int		ndot;

	CTonDoc *pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	if(m_pcWnd->m_nOptChangeCurser == 0){
		CData *pcData = pcDoc->SrchData(0, dx, dy);
		if(pcData != NULL && pcData->CheckFlag(ACTIVEFLAG) == OFF){
			m_pcView->m_nCursorKind = CUSKIND_ARROWPLUS;
			::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
			return;
		}
	}
	senvgetareadotsize(&ndot);
	dlen = CntrKindInitNew(dx, dy);
	scrndlenmmtorltv(&dlen);
	if(dlen < (ndot+4)){
		switch(m_nCntrKind){
		case LEFTUP:
		case RIGHTDOWN:
			m_pcView->m_nCursorKind = CUSKIND_SIZENESW;
			::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
			break;
		case LEFTDOWN:
		case RIGHTUP:
			m_pcView->m_nCursorKind = CUSKIND_SIZENWSE;
			::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
			break;
		case LEFTCNTR:
		case RIGHTCNTR:
			m_pcView->m_nCursorKind = CUSKIND_SIZEWE;
			::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
			break;
		case CNTRUP:
		case CNTRDOWN:
			m_pcView->m_nCursorKind = CUSKIND_SIZENS;
			::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
			break;
		}
	}
	else if(m_dSX < dx && dx < m_dEX && m_dSY < dy && dy < m_dEY){
		m_pcView->m_nCursorKind = CUSKIND_SIZEALL;
		::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
	}
	else{
		m_pcView->m_nCursorKind = CUSKIND_ARROW;
		::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
	}
}

int CCmdSlct::ActiveOnOffObject(DBL dx, DBL dy)
{
	CTonDoc *pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	CData *pcData = pcDoc->SrchData(0, dx, dy);
	if(pcData != NULL){
	// 2002.01.15 指定データがシフトで解除されない
	//&& pcData->CheckFlag(ACTIVEFLAG) != ON){
		if(m_pcView->CheckKeyFlag(MK_SHIFT) == FALSE){
			pcDoc->OffFlag(ACTIVEFLAG);
		}
		pcData = pcDoc->SrchData(0, dx, dy);
		if(pcData != NULL){
			if(pcData->CheckFlag(ACTIVEFLAG) == ON){
				pcData->OffFlag(ACTIVEFLAG);
			}
			else{
				pcData->OnFlag(ACTIVEFLAG);
			}
		}
		pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dSX, &m_dSY, &m_dEX, &m_dEY);
		DispSelectDataSize();
		return(TRUE);
	}
	return(FALSE);
}

BOOL CCmdSlct::CheckNearPoint(DBL sx, DBL sy, DBL ex, DBL ey)
{
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	if((sx - AREADOT) < ex &&  ex < (sx + AREADOT)
	&& (sy - AREADOT) < ey &&  ey < (sy + AREADOT)){
		return(TRUE);
	}
	return(FALSE);
}

void CCmdSlct::SetSWakVectAtr()
{
	CData	*pcData;

	if(m_pcCmdSWak == NULL){
		return;
	}
	m_pcCmdSWak->GetItemElement();
	pcData = m_pcDoc->CheckSelectOne(SWAKVECTID);
	if(pcData == NULL){
		m_pcCmdSWak->SetDataListSelectID(SWAKVECTID, ACTIVEFLAG);
		m_pcCmdSWak->SetAnyWakuAtr();
		SetToolBarElement();
	}
	else{
		m_pcCmdSWak->m_pcSWakVect = (CSWakVect *)pcData;
		m_pcCmdSWak->GetDataElement();
		m_pcCmdSWak->SetItemElement();
	}
}

void CCmdSlct::ResetSWakVectAtr()
{
	CData	*pcData;

	if(m_pcCmdSWak == NULL){
		return;
	}
	m_pcCmdSWak->InitClass();
	pcData = m_pcDoc->CheckSelectOne(SWAKVECTID);
	if(pcData == NULL){
		GetToolBarElement();
		m_pcCmdSWak->SetDataListSelectID(SWAKVECTID, ACTIVEFLAG);
		m_pcCmdSWak->ResetAnyWakuAtr();
	}
	else{
		m_pcCmdSWak->m_pcSWakVect = (CSWakVect *)pcData;
		m_pcCmdSWak->GetItemElement();
		m_pcCmdSWak->SetDataElement();
	}
}

void CCmdSlct::SetSWakLocate()
{
	CData	*pcData;
	DBL		sx, sy, ex, ey;

	if(m_pcCmdSWak == NULL){
		return;
	}
	pcData = m_pcDoc->CheckSelectOne(SWAKVECTID);
	if(pcData != NULL){
		m_pcCmdSWak->m_pcSWakVect = (CSWakVect *)pcData;
		m_pcCmdSWak->m_pcSWakVect->GetDMiniMax(&sx, &sy, &ex, &ey);
		m_pcCmdSWak->m_dSX = sx;
		m_pcCmdSWak->m_dSY = sy;
		m_pcCmdSWak->m_dEX = ex;
		m_pcCmdSWak->m_dEY = ey;
		m_pcCmdSWak->SetSWakLocate();

	}
}

void CCmdSlct::SetSlctRoundAreaPoint()
{
	DBL		len;

	m_dX1 = m_dSX;
	m_dY1 = m_dSY;
	m_dX2 = m_dSX;
	m_dY2 = m_dEY;
	m_dX3 = m_dEX;
	m_dY3 = m_dEY;
	m_dX4 = m_dEX;
	m_dY4 = m_dSY;

	len = 32;
	scrndlenrltvtomm(&len);
	m_dX0 = (m_dSX + m_dEX) / 2.0;
	m_dY0 = (m_dSY + m_dEY) / 2.0;
	m_dRSX = m_dX0;
	m_dRSY = m_dSY;
	m_dREX = m_dX0;
	m_dREY = m_dSY-len;

	m_dBaseRag = atan2(m_dY0 - m_dREY, m_dX0 - m_dREX);
}

void CCmdSlct::DrawAtnRoundBoxs(CDC *pcDC)
{
	DBL		x1, y1;
	DBL		x2, y2;
	DBL		x3, y3;
	DBL		x4, y4;
	DBL		sx, sy;
	DBL		ex, ey;
	int		rval, gval, bval;

	x1 = m_dX1; y1 = m_dY1;
	x2 = m_dX2; y2 = m_dY2;
	x3 = m_dX3; y3 = m_dY3;
	x4 = m_dX4; y4 = m_dY4;
	sx = m_dRSX; sy = m_dRSY;
	ex = m_dREX; ey = m_dREY;

	scrndxymmtorltv(&x1,&y1);
	scrndxymmtorltv(&x2,&y2);
	scrndxymmtorltv(&x3,&y3);
	scrndxymmtorltv(&x4,&y4);
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	scrnsetslctpen(pcDC->m_hDC);
	pcDC->MoveTo((int)x1, (int)y1);
	pcDC->LineTo((int)x2, (int)y2);
	pcDC->LineTo((int)x3, (int)y3);
	pcDC->LineTo((int)x4, (int)y4);
	pcDC->LineTo((int)x1, (int)y1);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)ey);
	DrawBoxs(m_pcDC, ex-3, ey-3, ex+3, ey+3);
	scrnpenend(pcDC->m_hDC);
}

void CCmdSlct::RoundSlctArea(DBL dx, DBL dy)
{
	DBL rag1, rag2, rag;

	rag1 = atan2(m_dY0 - m_dREY, m_dX0 - m_dREX);
	rag2 = atan2(m_dY0 - dy, m_dX0 - dx);
	rag = rag2 - rag1;
	curveroundtrns(m_dX1, m_dY1, m_dX0, m_dY0, rag, &m_dX1, &m_dY1);
	curveroundtrns(m_dX2, m_dY2, m_dX0, m_dY0, rag, &m_dX2, &m_dY2);
	curveroundtrns(m_dX3, m_dY3, m_dX0, m_dY0, rag, &m_dX3, &m_dY3);
	curveroundtrns(m_dX4, m_dY4, m_dX0, m_dY0, rag, &m_dX4, &m_dY4);
	curveroundtrns(m_dSX, m_dSY, m_dX0, m_dY0, rag, &m_dSX, &m_dSY);
	curveroundtrns(m_dEX, m_dEY, m_dX0, m_dY0, rag, &m_dEX, &m_dEY);
}

void CCmdSlct::MainRoundTrns()
{
	DBL		rag1, rag2, rag;
	DBL		tsin, tcos;
	DBL		cx, cy;

	if(m_dREX == m_dX0 && m_dREY == m_dY0){
		return;
	}
	rag1 = m_dBaseRag;
	rag2 = atan2(m_dY0 - m_dREY, m_dX0 - m_dREX);
	rag = rag2 - rag1;
	tsin = sin(rag);
	tcos = cos(rag);
	cx = m_dX0;
	cy = m_dY0;
	m_pcDoc->RoundTrns(cx, cy, tsin, tcos);
}

