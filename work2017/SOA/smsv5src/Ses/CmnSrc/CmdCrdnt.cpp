
/////////////////////////////////////////////////////////////////////////////
// CCmdCrdnt

#include "stdafx.h"
#include "CmdBase.h"
#include "CmdCrdnt.h"

//extern "C" {
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
//}

CCmdCrdnt::CCmdCrdnt(CScrollView* pcview)
:CCmdBase(pcview)
{
}

UINT CCmdCrdnt::ExecSortHead(UINT event, DBL dx, DBL dy)
{
	int		bmode;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		bmode = senvsetdrawmode(PRINTVIEW);
		m_pcData = SrchData(ACTIVEFLAG, dx, dy);
		senvsetdrawmode(bmode);
		if(m_pcData != NULL){
			SaveUndo();
			SortTailMain();
			//ScrnDataRedraw(m_pcData);
			ScrnAllRedraw();
			(m_pcView->GetDocument())->SetModifiedFlag();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		bmode = senvsetdrawmode(PRINTVIEW);
		SetCuserKind(dx, dy);
		senvsetdrawmode(bmode);
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

UINT CCmdCrdnt::ExecSortTail(UINT event, DBL dx, DBL dy)
{
	int		bmode;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		bmode = senvsetdrawmode(PRINTVIEW);
		m_pcData = SrchData(ACTIVEFLAG, dx, dy);
		senvsetdrawmode(bmode);
		if(m_pcData != NULL){
			SaveUndo();
			SortHeadMain();
			//ScrnDataRedraw(m_pcData);
			ScrnAllRedraw();
			(m_pcView->GetDocument())->SetModifiedFlag();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		bmode = senvsetdrawmode(PRINTVIEW);
		SetCuserKind(dx, dy);
		senvsetdrawmode(bmode);
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

UINT CCmdCrdnt::ExecSortNext(UINT event, DBL dx, DBL dy)
{
	int		bmode;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		bmode = senvsetdrawmode(PRINTVIEW);
		m_pcData = SrchData(ACTIVEFLAG, dx, dy);
		senvsetdrawmode(bmode);
		if(m_pcData != NULL){
			SaveUndo();
			SortPrevMain();
			//ScrnDataRedraw(m_pcData);
			ScrnAllRedraw();
			(m_pcView->GetDocument())->SetModifiedFlag();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		bmode = senvsetdrawmode(PRINTVIEW);
		SetCuserKind(dx, dy);
		senvsetdrawmode(bmode);
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

UINT CCmdCrdnt::ExecSortPrev(UINT event, DBL dx, DBL dy)
{
	int		bmode;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		bmode = senvsetdrawmode(PRINTVIEW);
		m_pcData = SrchData(ACTIVEFLAG, dx, dy);
		senvsetdrawmode(bmode);
		if(m_pcData != NULL){
			SaveUndo();
			SortNextMain();
			//ScrnDataRedraw(m_pcData);
			ScrnAllRedraw();
			(m_pcView->GetDocument())->SetModifiedFlag();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		bmode = senvsetdrawmode(PRINTVIEW);
		SetCuserKind(dx, dy);
		senvsetdrawmode(bmode);
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

UINT CCmdCrdnt::ExecSortNextOne(UINT event, DBL dx, DBL dy)
{
	if(event == INITEVENT){
		SaveUndo();
		SortActivePrevMain();
		ScrnAllRedraw();
		(m_pcView->GetDocument())->SetModifiedFlag();
	}
	return(ENDEVENT);
}

UINT CCmdCrdnt::ExecSortPrevOne(UINT event, DBL dx, DBL dy)
{
	if(event == INITEVENT){
		SaveUndo();
		SortActiveNextMain();
		ScrnAllRedraw();
		(m_pcView->GetDocument())->SetModifiedFlag();
	}
	return(ENDEVENT);
}

UINT CCmdCrdnt::ExecGrupSet(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CData*		pcData;
	CDataGrup*	pcDataGrup;
	POSITION	pos;
	POSITION	npos;

	if(event != INITEVENT){
		return(0);
	}

	SaveUndo();
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());

	pcDataGrup = (CDataGrup*)(pcDoc->NewData(DATAGRUPID));

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		npos = pos;
		pcData = pcObjDataList->GetNext(npos);
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			pcObjDataList->RemoveAt(pos);
			pcDataGrup->SetData(pcData);
		}
		if(npos == NULL){
			break;
		}
		pos = npos;
	}

	pcDoc->DataAddTail((CData*)pcDataGrup);
	ScrnRedraw();
	return(ENDEVENT);
}

UINT CCmdCrdnt::ExecGrupReset(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CData*		pcData;
	CData*		pcpreData;
	CDataGrup*	pcDataGrup;
	CDataFDib*	pcDataFDib;
	POSITION	pos;
	POSITION	npos;
	int	nDataID;

	if(event != INITEVENT){
		return(0);
	}

	SaveUndo();
	pcDataGrup = NULL;
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());

	m_cPtrList.RemoveAll( );

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		npos = pos;
		pcData = pcObjDataList->GetNext(npos);
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			nDataID = pcData->CheckID();
			switch(nDataID){
			case DATAGRUPID:
			case DATAFDIBID:
				pcObjDataList->RemoveAt(pos);
				m_cPtrList.AddTail(pcData);
				break;
			}
		}
		if(npos == NULL){
			break;
		}
		pos = npos;
	}

	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = pcObjDataList->GetNext(pos);
		nDataID = pcData->CheckID();
		switch(nDataID){
		case DATAGRUPID:
			pcDataGrup = (CDataGrup*)pcData;
			pcData = pcDataGrup->m_pcDataTop;
			while(pcData != NULL){
				pcDoc->DataAddTail(pcData);
				pcpreData = pcData;
				pcData = pcpreData->GetNext();
				pcpreData->SetNext(NULL);
			}
			pcDataGrup->m_pcDataTop = NULL;
			delete(pcDataGrup);
			break;
		case DATAFDIBID:
			pcDataFDib = (CDataFDib*)pcData;
			pcData = pcDataFDib->m_pcDataTop;
			while(pcData != NULL){
				pcDoc->DataAddTail(pcData);
				pcpreData = pcData;
				pcData = pcpreData->GetNext();
				pcpreData->SetNext(NULL);
			}
			pcDataFDib->m_pcDataTop = NULL;
			delete(pcDataFDib);
			break;
		}
	}
	ScrnAllRedraw();
	(m_pcView->GetDocument())->SetModifiedFlag();
	return(ENDEVENT);
}

UINT CCmdCrdnt::ExecListSet(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CData*		pcData;
	CDataList*	pcDataList;
	POSITION	pos;
	POSITION	npos;

	if(event != INITEVENT){
		return(0);
	}

	SaveUndo();
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());

	pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		npos = pos;
		pcData = pcObjDataList->GetNext(npos);
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			if(pcData->CheckID() == DATALISTID){
				pcObjDataList->RemoveAt(pos);
				ListSetLoop(pcDataList, pcData);
			}
		}
		if(npos == NULL){
			break;
		}
		pos = npos;
	}

	pcDoc->DataAddTail((CData*)pcDataList);
	ScrnRedraw();
	(m_pcView->GetDocument())->SetModifiedFlag();
	return(ENDEVENT);
}

UINT CCmdCrdnt::ExecListReset(UINT event, DBL dx, DBL dy)
{
	CTonDoc*	pcDoc;
	CData*		pcData;
	CData*		pcTmpData;
	CDataList*	pcDataList;
	POSITION	pos;
	POSITION	npos;

	if(event != INITEVENT){
		return(0);
	}

	SaveUndo();


	pcDataList = NULL;
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		npos = pos;
		pcData = pcObjDataList->GetNext(npos);
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			if(pcData->CheckID() == DATALISTID){
				pcData->OnFlag(ATNTION1FLAG);
			}
			else{
				if(pcData->CheckID() == DATALOGOID){
					pcTmpData = (CDataList*)(((CDataLogo*)pcData)->m_pcDataTop);
					pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
					pcDataList->SetData(pcTmpData);
					pcDoc->DataAddTail(pcDataList);
					((CDataLogo*)pcData)->m_pcDataTop = NULL;
					pcDataList->OnFlag(ATNTION1FLAG);
					pcData->OffFlag(ATNTION1FLAG);
				}
				else{
					pcData->OffFlag(ATNTION1FLAG);
				}
			}
		}
		if(npos == NULL){
			break;
		}
		pos = npos;
	}

	pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		npos = pos;
		pcData = pcObjDataList->GetNext(npos);
		if(pcData->CheckFlag(ATNTION1FLAG) == ON){
			pcObjDataList->RemoveAt(pos);
			pcDataList = (CDataList*)pcData;
			ListResetLoop(pcDataList);
			pcDataList->m_pcDataTop = NULL;
			delete(pcDataList);
		}
		if(npos == NULL){
			break;
		}
		pos = npos;
	}
	ScrnRedraw();
	(m_pcView->GetDocument())->SetModifiedFlag();
	return(ENDEVENT);
}

UINT CCmdCrdnt::ExecCrdntTop(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;
	DBL		sx, sy, ex, ey, cy;
	POSITION	pos;
	DBL		movex, movey;

	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT2:
		pcData = SrchData(ACTIVEFLAG, dx, dy);
		if(pcData != NULL){
			SaveUndo();
			pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
			cy = ey;
			pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
			while (pos != NULL){
				pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
				if(pcData != NULL){
					if(pcData->CheckFlag(ACTIVEFLAG) == ON){
						pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
						movex = 0.0;
						movey = cy - ey;
						pcData->MoveTrns(movex, movey);
					}
				}
			}
			ScrnAllRedraw();
			return(ENDEVENT);
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

UINT CCmdCrdnt::ExecCrdntVCenter(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;
	DBL		sx, sy, ex, ey, cy;
	POSITION	pos;
	DBL		movex, movey;

	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT2:
		pcData = SrchData(ACTIVEFLAG, dx, dy);
		if(pcData != NULL){
			SaveUndo();
			pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
			cy = (ey + sy) / 2.0;
			pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
			while (pos != NULL){
				pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
				if(pcData != NULL){
					if(pcData->CheckFlag(ACTIVEFLAG) == ON){
						pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
						movex = 0.0;
						movey = cy - (ey + sy) / 2.0;
						pcData->MoveTrns(movex, movey);
					}
				}
			}
			ScrnAllRedraw();
			return(ENDEVENT);
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

UINT CCmdCrdnt::ExecCrdntBottom(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;
	DBL		sx, sy, ex, ey, cy;
	POSITION	pos;
	DBL		movex, movey;

	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT2:
		pcData = SrchData(ACTIVEFLAG, dx, dy);
		if(pcData != NULL){
			SaveUndo();
			pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
			cy = sy;
			pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
			while (pos != NULL){
				pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
				if(pcData != NULL){
					if(pcData->CheckFlag(ACTIVEFLAG) == ON){
						pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
						movex = 0.0;
						movey = cy - sy;
						pcData->MoveTrns(movex, movey);
					}
				}
			}
			ScrnAllRedraw();
			return(ENDEVENT);
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

UINT CCmdCrdnt::ExecCrdntLeft(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;
	DBL		sx, sy, ex, ey, cx;
	POSITION	pos;
	DBL		movex, movey;

	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT2:
		pcData = SrchData(ACTIVEFLAG, dx, dy);
		if(pcData != NULL){
			SaveUndo();
			pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
			cx = sx;
			pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
			while (pos != NULL){
				pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
				if(pcData != NULL){
					if(pcData->CheckFlag(ACTIVEFLAG) == ON){
						pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
						movex = cx - sx;
						movey = 0.0;
						pcData->MoveTrns(movex, movey);
					}
				}
			}
			ScrnAllRedraw();
			return(ENDEVENT);
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

UINT CCmdCrdnt::ExecCrdntHCenter(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;
	DBL		sx, sy, ex, ey, cx;
	POSITION	pos;
	DBL		movex, movey;

	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT2:
		pcData = SrchData(ACTIVEFLAG, dx, dy);
		if(pcData != NULL){
			SaveUndo();
			pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
			cx = (ex + sx) / 2.0;
			pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
			while (pos != NULL){
				pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
				if(pcData != NULL){
					if(pcData->CheckFlag(ACTIVEFLAG) == ON){
						pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
						movex = cx - (ex + sx) / 2.0;
						movey = 0.0;
						pcData->MoveTrns(movex, movey);
					}
				}
			}
			ScrnAllRedraw();
			return(ENDEVENT);
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

UINT CCmdCrdnt::ExecCrdntRight(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;
	DBL		sx, sy, ex, ey, cx;
	POSITION	pos;
	DBL		movex, movey;

	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT2:
		pcData = SrchData(ACTIVEFLAG, dx, dy);
		if(pcData != NULL){
			SaveUndo();
			pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
			cx = ex;
			pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
			while (pos != NULL){
				pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
				if(pcData != NULL){
					if(pcData->CheckFlag(ACTIVEFLAG) == ON){
						pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
						movex = cx - ex;
						movey = 0.0;
						pcData->MoveTrns(movex, movey);
					}
				}
			}
			ScrnAllRedraw();
			return(ENDEVENT);
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

UINT CCmdCrdnt::ExecMoveHCenter(UINT event, DBL dx, DBL dy)
{
	DBL		xsize, ysize;
	DBL		sx, sy, ex, ey, cx, cy;
	DBL		movex, movey;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	scrngetmmsize(&xsize, &ysize);
	cx = (m_pcDoc->m_dLeftSpace + xsize - m_pcDoc->m_dRightSpace) / 2.0;
	cy = (m_pcDoc->m_dTopSpace + ysize - m_pcDoc->m_dBottomSpace) / 2.0;
	SaveUndo();
	m_pcDoc->GetDMiniMax(ACTIVEFLAG, &sx, &sy, &ex, &ey);
	movex = cx - (ex + sx) / 2.0;
	movey = 0.0;
	m_pcDoc->MoveTrns(movex, movey);
	/*
	POSITION	pos;
	CData		*pcData;
	pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
	while (pos != NULL){
		pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
		if(pcData != NULL){
			if(pcData->CheckFlag(ACTIVEFLAG) == ON){
				pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
				movex = cx - (ex + sx) / 2.0;
				movey = 0.0;
				pcData->MoveTrns(movex, movey);
			}
		}
	}
	*/
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdCrdnt::ExecMoveVCenter(UINT event, DBL dx, DBL dy)
{
	DBL		xsize, ysize;
	DBL		sx, sy, ex, ey, cx, cy;
	DBL		movex, movey;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	scrngetmmsize(&xsize, &ysize);
	cx = (m_pcDoc->m_dLeftSpace + xsize - m_pcDoc->m_dRightSpace) / 2.0;
	cy = (m_pcDoc->m_dBottomSpace + ysize - m_pcDoc->m_dTopSpace) / 2.0;
	SaveUndo();
	m_pcDoc->GetDMiniMax(ACTIVEFLAG, &sx, &sy, &ex, &ey);
	movex = 0;
	movey = cy - (ey + sy) / 2.0;
	m_pcDoc->MoveTrns(movex, movey);
	/*
	POSITION	pos;
	CData	*pcData;
	pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
	while (pos != NULL){
		pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
		if(pcData != NULL){
			if(pcData->CheckFlag(ACTIVEFLAG) == ON){
				pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
				movex = 0;
				movey = cy - (ey + sy) / 2.0;
				pcData->MoveTrns(movex, movey);
			}
		}
	}
	*/
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdCrdnt::ExecMoveHVCenter(UINT event, DBL dx, DBL dy)
{
	DBL		xsize, ysize;
	DBL		sx, sy, ex, ey, cx, cy;
	DBL		movex, movey;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	scrngetmmsize(&xsize, &ysize);
	cx = (m_pcDoc->m_dLeftSpace + xsize - m_pcDoc->m_dRightSpace) / 2.0;
	cy = (m_pcDoc->m_dBottomSpace + ysize - m_pcDoc->m_dTopSpace) / 2.0;
	SaveUndo();
	m_pcDoc->GetDMiniMax(ACTIVEFLAG, &sx, &sy, &ex, &ey);
	movex = cx - (ex + sx) / 2.0;
	movey = cy - (ey + sy) / 2.0;
	m_pcDoc->MoveTrns(movex, movey);
	/*
	POSITION	pos;
	CData	*pcData;
	pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
	while (pos != NULL){
		pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
		if(pcData != NULL){
			if(pcData->CheckFlag(ACTIVEFLAG) == ON){
				pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
				movex = cx - (ex + sx) / 2.0;
				movey = cy - (ey + sy) / 2.0;
				pcData->MoveTrns(movex, movey);
			}
		}
	}
	*/
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdCrdnt::ExecSizeHSame(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;
	DBL		sx, sy, ex, ey;
	DBL		sizex, sizey;
	POSITION	pos;
	DBL		timex, timey;

	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT2:
		pcData = SrchData(ACTIVEFLAG, dx, dy);
		if(pcData != NULL){
			SaveUndo();
			pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
			sizex = ex - sx;
			sizey = ey - sy;
			pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
			while (pos != NULL){
				pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
				if(pcData != NULL){
					if(pcData->CheckFlag(ACTIVEFLAG) == ON){
						pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
						timex = sizex / (ex - sx);
						timey = 1.0;
						pcData->SizeTrns(sx, sy, timex, timey);
					}
				}
			}
			ScrnAllRedraw();
			return(ENDEVENT);
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

UINT CCmdCrdnt::ExecSizeVSame(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;
	DBL		sx, sy, ex, ey;
	DBL		sizex, sizey;
	POSITION	pos;
	DBL		timex, timey;

	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT2:
		pcData = SrchData(ACTIVEFLAG, dx, dy);
		if(pcData != NULL){
			SaveUndo();
			pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
			sizex = ex - sx;
			sizey = ey - sy;
			pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
			while (pos != NULL){
				pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
				if(pcData != NULL){
					if(pcData->CheckFlag(ACTIVEFLAG) == ON){
						pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
						timex = 1.0;
						timey = sizey / (ey - sy);
						pcData->SizeTrns(sx, sy, timex, timey);
					}
				}
			}
			ScrnAllRedraw();
			return(ENDEVENT);
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

UINT CCmdCrdnt::ExecSizeHVSame(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;
	DBL		sx, sy, ex, ey;
	DBL		sizex, sizey;
	POSITION	pos;
	DBL		timex, timey;

	switch(event){
	case INITEVENT:
		InitClass();
		m_nMouseSetCount = 0;
		break;
	case SETEVENT2:
		pcData = SrchData(ACTIVEFLAG, dx, dy);
		if(pcData != NULL){
			SaveUndo();
			pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
			sizex = ex - sx;
			sizey = ey - sy;
			pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
			while (pos != NULL){
				pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
				if(pcData != NULL){
					if(pcData->CheckFlag(ACTIVEFLAG) == ON){
						pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
						timex = sizex / (ex - sx);
						timey = sizey / (ey - sy);
						pcData->SizeTrns(sx, sy, timex, timey);
					}
				}
			}
			ScrnAllRedraw();
			return(ENDEVENT);
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

UINT CCmdCrdnt::ExecSpaseHEqual(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;
	DBL		xsize, ysize;
	DBL		sx, sy, ex, ey;
	POSITION	pos;
	DBL		sumlen, sumspace, space;
	int		count;
	CMinMaxStruct	*pcMinMax;
	DBL		movex, movey;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	SaveUndo();
	m_pcDoc->GetDMiniMax(ACTIVEFLAG, &sx, &sy, &ex, &ey);
	xsize = ex - sx;
	ysize = ey - sy;

	m_pcPtrList = new CMinMaxStructList;

	sumlen = 0;
	count = 0;
	pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
			pcMinMax = new CMinMaxStruct;
			pcMinMax->m_dMax = ex;
			pcMinMax->m_dMin = sx;
			pcMinMax->m_pcData = pcData;
			SortAddList(pcMinMax);
			sumlen += (ex - sx);
			count++;
		}
	}

	space = 0;
	sumspace = xsize - sumlen;
	if(count < 3){
		DeleteAllList();
		return(ENDEVENT);
	}

	space = sumspace / (count-1);
	pos = m_pcPtrList->GetHeadPosition();
	pcMinMax = m_pcPtrList->GetNext(pos);
	ex = pcMinMax->m_dMax;
	while (pos != NULL)
	{
		pcMinMax = m_pcPtrList->GetNext(pos);
		sx = pcMinMax->m_dMin;
		movey = 0;
		movex = (ex + space) - sx;
		(pcMinMax->m_pcData)->MoveTrns(movex, movey);
		(pcMinMax->m_pcData)->RenewMiniMax();
		ex = (ex + space) + (pcMinMax->m_dMax - pcMinMax->m_dMin);
	}	

	DeleteAllList();
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdCrdnt::ExecSpaseVEqual(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;
	DBL		xsize, ysize;
	DBL		sx, sy, ex, ey;
	POSITION	pos;
	DBL		sumlen, sumspace, space;
	int		count;
	CMinMaxStruct	*pcMinMax;
	DBL		movex, movey;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	SaveUndo();
	m_pcDoc->GetDMiniMax(ACTIVEFLAG, &sx, &sy, &ex, &ey);
	xsize = ex - sx;
	ysize = ey - sy;

	m_pcPtrList = new CMinMaxStructList;

	sumlen = 0;
	count = 0;
	pos = m_pcDoc->m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_pcDoc->m_pcObjDataList->GetNext(pos);
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			pcData->GetDMiniMax(&sx, &sy, &ex, &ey);
			pcMinMax = new CMinMaxStruct;
			pcMinMax->m_dMax = ey;
			pcMinMax->m_dMin = sy;
			pcMinMax->m_pcData = pcData;
			SortAddList(pcMinMax);
			sumlen += (ey - sy);
			count++;
		}
	}

	space = 0;
	sumspace = ysize - sumlen;
	if(count < 3){
		DeleteAllList();
		return(ENDEVENT);
	}

	space = sumspace / (count-1);
	pos = m_pcPtrList->GetHeadPosition();
	pcMinMax = m_pcPtrList->GetNext(pos);
	ey = pcMinMax->m_dMax;
	while (pos != NULL)
	{
		pcMinMax = m_pcPtrList->GetNext(pos);
		sy = pcMinMax->m_dMin;
		movex = 0;
		movey = (ey + space) - sy;
		(pcMinMax->m_pcData)->MoveTrns(movex, movey);
		(pcMinMax->m_pcData)->RenewMiniMax();
		ey = (ey + space) + (pcMinMax->m_dMax - pcMinMax->m_dMin);
	}	

	DeleteAllList();
	ScrnAllRedraw();
	return(ENDEVENT);
}

void CCmdCrdnt::ListResetLoop(CDataList *pcDataList)
{
	CTonDoc*	pcDoc;
	CData*		pcData;
	CData*		pcpreData;
	CDataList*	pcTmpDataList;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcData = pcDataList->m_pcDataTop;
	while(pcData != NULL){
		if(pcData->CheckID() == BASEVECTID){
			pcTmpDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
			pcTmpDataList->SetData(pcData);
			pcDoc->DataAddTail(pcTmpDataList);
		}
		else if(pcData->CheckID() == DATALISTID){
			pcTmpDataList = (CDataList*)pcData;
			ListResetLoop(pcTmpDataList);
		}
		else{
			pcDoc->DataAddTail(pcData);
		}
		pcpreData = pcData;
		pcData = pcpreData->GetNext();
		pcpreData->SetNext(NULL);
	}
}

void CCmdCrdnt::SortHeadMain()
{
	CTonDoc*	pcDoc;
	CData		*pcData;
	POSITION	pos;
	POSITION	npos;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		npos = pos;
		pcData = pcObjDataList->GetNext(npos);
		if(m_pcData == pcData){
			pcObjDataList->RemoveAt(pos);
			pcObjDataList->AddHead(pcData);
			break;
		}
		if(npos == NULL){
			break;
		}
		pos = npos;
	}
}

void CCmdCrdnt::SortTailMain()
{
	CTonDoc*	pcDoc;
	CData		*pcData;
	POSITION	pos;
	POSITION	npos;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		npos = pos;
		pcData = pcObjDataList->GetNext(npos);
		if(m_pcData == pcData){
			pcObjDataList->RemoveAt(pos);
			pcObjDataList->AddTail(pcData);
			break;
		}
		if(npos == NULL){
			break;
		}
		pos = npos;
	}
}

void CCmdCrdnt::SortNextMain()
{
	CTonDoc*	pcDoc;
	CData		*pcData;
	CData		*pnData;
	POSITION	pos;
	POSITION	npos;
	POSITION	tpos;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		npos = pos;
		pcData = pcObjDataList->GetNext(npos);
		if(m_pcData == pcData){
			if(npos != NULL){
				tpos = npos;
				pnData = pcObjDataList->GetNext(tpos);
				pcObjDataList->SetAt(pos, pnData);
				pcObjDataList->SetAt(npos, pcData);
				break;
			}
		}
		if(npos == NULL){
			break;
		}
		pos = npos;
	}
}

void CCmdCrdnt::SortPrevMain()
{
	CTonDoc*	pcDoc;
	CData		*pcData;
	CData		*ppcData;
	POSITION	pos;
	POSITION	ppos;
	POSITION	tpos;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	pos = pcObjDataList->GetTailPosition();
	while (pos != NULL)
	{
		ppos = pos;
		pcData = pcObjDataList->GetPrev(ppos);
		if(m_pcData == pcData){
			if(ppos != NULL){
				tpos = ppos;
				ppcData = pcObjDataList->GetPrev(tpos);
				pcObjDataList->SetAt(pos, ppcData);
				pcObjDataList->SetAt(ppos, pcData);
				break;
			}
		}
		if(ppos == NULL){
			break;
		}
		pos = ppos;
	}
}

void CCmdCrdnt::SortActiveNextMain()
{
	CTonDoc*	pcDoc;
	CData		*pcData;
	CData		*pnData;
	POSITION	pos;
	POSITION	npos;
	POSITION	tpos;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		npos = pos;
		pcData = pcObjDataList->GetNext(npos);
		if(pcData->CheckFlag(ACTIVEFLAG)){
			if(npos != NULL){
				tpos = npos;
				pnData = pcObjDataList->GetNext(tpos);
				pcObjDataList->SetAt(pos, pnData);
				pcObjDataList->SetAt(npos, pcData);
				break;
			}
		}
		if(npos == NULL){
			break;
		}
		pos = npos;
	}
}

void CCmdCrdnt::SortActivePrevMain()
{
	CTonDoc*	pcDoc;
	CData		*pcData;
	CData		*ppcData;
	POSITION	pos;
	POSITION	ppos;
	POSITION	tpos;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	pos = pcObjDataList->GetTailPosition();
	while (pos != NULL)
	{
		ppos = pos;
		pcData = pcObjDataList->GetPrev(ppos);
		if(pcData->CheckFlag(ACTIVEFLAG)){
			if(ppos != NULL){
				tpos = ppos;
				ppcData = pcObjDataList->GetPrev(tpos);
				pcObjDataList->SetAt(pos, ppcData);
				pcObjDataList->SetAt(ppos, pcData);
				break;
			}
		}
		if(ppos == NULL){
			break;
		}
		pos = ppos;
	}
}

int CCmdCrdnt::ListSetLoop(CDataList * pcSetDataList, CData* pcSetData)
{
	CData*	pcData;
	CDataList*	pcDataList;
	int		ret;
	int		restflag;

	if(pcSetData->CheckID() == DATALISTID){
		pcDataList = (CDataList*)pcSetData;
		pcData = pcDataList->m_pcDataTop;
		restflag = ON;
		while(pcData != NULL){
			ret = ListSetLoop(pcSetDataList, pcData);
			if(ret == ON){
				break;
			}
			pcData = pcData->GetNext();
		}
		pcDataList->m_pcDataTop = NULL;
		delete(pcDataList);
		return(OFF);
	}
	else if(pcSetData->CheckID() == BASEVECTID){
		pcSetDataList->SetData(pcSetData);
		return(ON);
	}
	return(OFF);
}

void CCmdCrdnt::SetCuserKind(DBL dx, DBL dy)
{
	CTonDoc *pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	CData *pcData = pcDoc->SrchData(ACTIVEFLAG, dx, dy);
	if(pcData != NULL){
		m_pcView->m_nCursorKind = CUSKIND_ARROWPLUS;
		::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
		return;
	}
	m_pcView->m_nCursorKind = CUSKIND_ARROW;
	::SendMessage(m_pcView->m_hWnd, WM_SETCURSOR, 0, 0);
}

void CCmdCrdnt::DeleteAllList()
{
	POSITION	pos, tpos;
	CMinMaxStruct	*pcMinMax;

	pos = m_pcPtrList->GetHeadPosition();
	while (pos != NULL)
	{
		tpos = pos;
		pcMinMax = m_pcPtrList->GetNext(pos);
		m_pcPtrList->RemoveAt(tpos);
		delete(pcMinMax);
	}
	delete(m_pcPtrList);
}

void CCmdCrdnt::SortAddList(CMinMaxStruct *pcSetMinMax)
{
	POSITION	pos, setpos;
	CMinMaxStruct	*pcMinMax;

	pos = m_pcPtrList->GetHeadPosition();
	if(pos == NULL){
		m_pcPtrList->AddTail(pcSetMinMax);
		return;
	}
	while (pos != NULL)
	{
		setpos = pos;
		pcMinMax = m_pcPtrList->GetNext(pos);
		if(pcMinMax->m_dMin > pcSetMinMax->m_dMin){
			m_pcPtrList->InsertBefore(setpos, pcSetMinMax);
			return;
		}
	}
	m_pcPtrList->AddTail(pcSetMinMax);
}
