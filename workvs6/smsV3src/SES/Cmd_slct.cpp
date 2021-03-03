
/////////////////////////////////////////////////////////////////////////////
// CCmdSlct

#include "stdafx.h"
#include "math.h"

#include "..\\cmnsrc\\cmd_base.h"
#include "..\\ses\\cmd_slct.h"

#define	NOOPMODE	0
#define	SLCTMODE	100
#define	MOVEMODE	200
#define TRNSMODE	300
#define	SLCTMOVEMODE	400

extern "C" {
	void scrndlenrltvtomm(LPDBL len);
	void scrndlenmmtorltv(LPDBL len);
	void senvgetareadotsize(int *ndot);

}

CCmdSlct::CCmdSlct(CScrollView* pcview)
:CCmdBase(pcview)
{
}

UINT CCmdSlct::ExecObj(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;
	DBL		minx, miny, maxx, maxy;
	
	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_nMouseSetCount = 1;
			m_dSX = dx; m_dSY = dy;
			m_dEX = dx; m_dEY = dy;
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
			if(m_pcView->CheckKeyFlag(MK_SHIFT) == FALSE){
				m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
				m_pcDoc->OffFlag(ACTIVEFLAG);
				ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			}
		}
		else if(m_nMouseSetCount == 1){
			m_nMouseSetCount = 0;
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
			m_dEX = dx; m_dEY = dy;
			if(m_dEX == m_dSX || m_dEY == m_dSY){
				pcData = m_pcDoc->SrchData(0, dx, dy);
				if(pcData != NULL){
					if(m_pcView->CheckKeyFlag(MK_SHIFT) == FALSE){
						m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
						m_pcDoc->OffFlag(ACTIVEFLAG);
						ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
					}
					m_pcDoc->GetDMiniMax(ACTIVEFLAG, &minx, &miny, &maxx, &maxy);
					if(pcData->CheckFlag(ACTIVEFLAG) == ON){
						pcData->OffFlag(ACTIVEFLAG);
					}
					else{
						pcData->OnFlag(ACTIVEFLAG);
					}
					DispSelectDataSize();
					ScrnAreaRedraw(minx, miny, maxx, maxy);
					ScrnRedraw();
					//return(REDRAWEVENT);
				}
			}
			else{
				if(m_pcView->CheckKeyFlag(MK_SHIFT) == FALSE){
					m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
					m_pcDoc->OffFlag(ACTIVEFLAG);
					ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
				}
				m_pcDoc->AreaInFlagOn(ACTIVEFLAG, m_dSX, m_dSY, m_dEX, m_dEY);
				DispSelectDataSize();
				ScrnRedraw();
				//return(REDRAWEVENT);
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
					ScrnRedraw();

					//return(REDRAWEVENT);
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
				ScrnRedraw();
				//return(REDRAWEVENT);
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
			if(m_dEX == m_dSX || m_dEY == m_dSY){
				if(((CTonView*)m_pcView)->CheckKeyFlag(MK_SHIFT) == FALSE){
					// 指定データを非指定状態に変更
					m_pcDoc->OffFlag(ACTIVEFLAG);
				}
				pcData = m_pcDoc->SrchData(0, dx, dy);
				if(pcData != NULL){
					pcData->OnFlag(ACTIVEFLAG);
				}
				m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dSX, &m_dSY, &m_dEX, &m_dEY);
				DispSelectDataSize();
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
					m_pcDoc->OffFlag(ACTIVEFLAG);
				}
				m_pcDoc->AreaInFlagOnOff(ACTIVEFLAG, m_dSX, m_dSY, m_dEX, m_dEY);
				m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dSX, &m_dSY, &m_dEX, &m_dEY);
				DispSelectDataSize();
				ScrnAllRedraw();
			}
			m_nMouseSetCount = 0;
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

// 選択移動はこの処理が行う
UINT CCmdSlct::ExecAreaSlctMove(UINT event, DBL dx, DBL dy)
{
	int		ret;

	if(event == ENDEVENT){
		ret = 0;
	}
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		m_nLocalCommand = SLCTMODE;
		InitClass();
		m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dSX, &m_dSY, &m_dEX, &m_dEY);
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_nLocalCommand = SetLocalCommand(dx, dy);
			if(TRNSMODE != m_nLocalCommand
			&& SLCTMOVEMODE != m_nLocalCommand){
				m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
				/*
				if(ActiveOnOffObject(dx, dy) == TRUE){
					m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dSX, &m_dSY, &m_dEX, &m_dEY);
					ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
					ScrnAreaRedraw(m_dSX, m_dSY, m_dEX, m_dEY);
					m_nLocalCommand = SLCTMOVEMODE;
				}
				*/
			}
		}
		break;
	case UPMOVEEVENT:
		m_nMouseSetCount = 0;
		SetCuserKind(dx, dy);
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
			m_pcView->StartTimer();
			m_pcView->m_nCursorKind = CUSKIND_SIZEALL;
			::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
			InitDataAreaBoxs();
			m_dCX = dx; m_dCY = dy;
			m_dMBX = dx; m_dMBY = dy;
			m_nMouseSetCount = 1;
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		else{
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
			if(m_pcView->EndTimer() <= 1){	//ドラッグしなかった時
				// トルグで指定非指定を処理
				m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
				if(((CTonView*)m_pcView)->CheckKeyFlag(MK_SHIFT) == TRUE){
					pcData = m_pcDoc->SrchData(0, dx, dy);
					if(pcData != NULL){
						if(pcData->CheckFlag(ACTIVEFLAG) == ON){
							pcData->OffFlag(ACTIVEFLAG);
						}
						else{
							pcData->OnFlag(ACTIVEFLAG);
						}
					}
					m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dSX, &m_dSY, &m_dEX, &m_dEY);
					ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
					ScrnAreaRedraw(m_dSX, m_dSY, m_dEX, m_dEY);
				}
				m_nMouseSetCount = 0;
				return(0);
			}
			movex = dx - m_dMBX;
			movey = dy - m_dMBY;
			SaveUndo();
			if(((CTonView*)m_pcView)->CheckKeyFlag(MK_SHIFT) == TRUE){
				if(fabs(movex) > fabs(movey)){
					movey = 0;
				}
				else{
					movex = 0;
				}
			}
			m_pcDoc->MoveTrns(movex, movey);
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
			DrawAtnBoxs(m_pcDC);
			m_dEX += movex; m_dSX += movex;
			m_dEY += movey; m_dSY += movey;
			m_dCX = dx; m_dCY = dy;

			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		break;
	case RESETEVENT:
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

UINT CCmdSlct::ExecAreaTrns(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
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
			ScrnAllRedraw();
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			CntrKindMoveNew(dx, dy);
			DrawAtnBoxs(m_pcDC);
			EndDraw();
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

UINT CCmdSlct::ExecAreaRound(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
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
			ScrnAllRedraw();
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			CntrKindMoveNew(dx, dy);
			DrawAtnBoxs(m_pcDC);
			EndDraw();
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

