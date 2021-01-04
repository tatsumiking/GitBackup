//
// Easy Scanner System Ver2.0
//
// object clipdata.cpp  構造
//
//  Copyright (c) 2000 by THEON

#include "StdAfx.h"

#include "ClipData.h"

#include	 "../ObjLib/Pdrwlib.h"

IMPLEMENT_SERIAL(CClipData, CVect, 0)
CClipData::CClipData()
{
}

//p オブジェクトの生成
CClipData::CClipData(int nDummy)
:CVect(nDummy)
{
	m_pcClipTop = NULL;
	m_pcDataTop = NULL;
}

//p オブジェクトの消滅
CClipData::~CClipData()
{
	CData*	pcData;
	CData*	nextData;

	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		nextData = pcData->GetNext();
		delete(pcData);
		pcData = nextData;
	}

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		nextData = pcData->GetNext();
		delete(pcData);
		pcData = nextData;
	}
}

int CClipData::CheckID()
{
	return(CLIPDATAID);
}

void CClipData::Serialize(CArchive& ar)
{
	int		nID;

	if (ar.IsStoring())
	{
		if(m_pcClipTop != NULL){
			nID = m_pcClipTop->CheckID();
			ar << nID;
			m_pcClipTop->Serialize(ar);
		}
		else{
			nID = NOOPDATAID;
			ar << nID;
		}

		if(m_pcDataTop != NULL){
			nID = m_pcDataTop->CheckID();
			ar << nID;
			m_pcDataTop->Serialize(ar);
		}
		else{
			nID = NOOPDATAID;
			ar << nID;
		}
	}
	else
	{
		ar >> nID;
		if(nID != NOOPDATAID){
			m_pcClipTop = NewDataCreate(nID);
			m_pcClipTop->Serialize(ar);
		}
		else{
			m_pcClipTop = NULL;
		}

		ar >> nID;
		if(nID != NOOPDATAID){
			m_pcDataTop = NewDataCreate(nID);
			m_pcDataTop->Serialize(ar);
		}
		else{
			m_pcDataTop = NULL;
		}
	}
	CVect::Serialize(ar);
	if (!(ar.IsStoring()))
	{
		RenewMiniMax();
	}
}

void CClipData::InitElement()
{
}

void CClipData::GetDMiniMax(DBL *minx, DBL *miny, DBL *maxx, DBL *maxy)
{
	CData*	pcData;
	DBL		tminx, tminy, tmaxx, tmaxy;

	(*minx) = INTMAX;
	(*miny) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->GetDMiniMax(&tminx, &tminy, &tmaxx, &tmaxy);
 		(*minx) = min((*minx), tminx);
		(*miny) = min((*miny), tminy);
		(*maxx) = max((*maxx), tmaxx);
		(*maxy) = max((*maxy), tmaxy);
		pcData = pcData->GetNext();
	}
}

void CClipData::RenewMiniMax()
{
	CData*	pcData;

	m_dMinix = INTMAX;
	m_dMiniy = INTMAX;
	m_dMaxx = -INTMAX;
	m_dMaxy = -INTMAX;
	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		// 助長な処理がある変更関係のRenewは見直す必要あり
		pcData->RenewMiniMax();
		m_dMinix = min(pcData->m_dMinix, m_dMinix);
		m_dMiniy = min(pcData->m_dMiniy, m_dMiniy);
		m_dMaxx = max(pcData->m_dMaxx, m_dMaxx);
		m_dMaxy = max(pcData->m_dMaxy, m_dMaxy);
		pcData = pcData->GetNext();
	}
}

void CClipData::MoveTrns(DBL movex, DBL movey)
{
	CData*	pcData;

	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->MoveTrns(movex, movey);
		pcData = pcData->GetNext();
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->MoveTrns(movex, movey);
		pcData = pcData->GetNext();
	}
}

void CClipData::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	CData*	pcData;

	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SizeTrns(cntrx, cntry, timex, timey);
		pcData = pcData->GetNext();
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SizeTrns(cntrx, cntry, timex, timey);
		pcData = pcData->GetNext();
	}
}

void CClipData::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	CData*	pcData;

	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->RoundTrns(cntrx, cntry, tsin, tcos);
		pcData = pcData->GetNext();
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->RoundTrns(cntrx, cntry, tsin, tcos);
		pcData = pcData->GetNext();
	}
}

CData *CClipData::SrchData(DBL xx, DBL yy, DBL *retlen)
{
	CData	*pcData;
	DBL			tlen;

	*retlen = INTMAX;
	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SrchData(xx, yy, &tlen);
		if(tlen < (*retlen)){
			*retlen = tlen;
		}
		pcData = pcData->GetNext();
	}
	return(this);
}

CData *CClipData::SrchNearPoint(DBL xx, DBL yy, DBL * retlen, int * retpp)
{
	CData	*pcData;
	CData	*pcRetData;
	CData	*pcTData;
	DBL			tlen;
	int			tpp;

	*retlen = INTMAX;
	pcRetData = NULL;
	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcTData = pcData->SrchNearPoint(xx, yy, &tlen, &tpp);
		if(tlen < (*retlen)){
			pcRetData = pcTData;
			*retlen = tlen;
			*retpp = tpp;
		}
		pcData = pcData->GetNext();
	}
	return(pcRetData);
}

CData *CClipData::SrchNearOnLinePoint(DBL xx, DBL yy, DBL * retlen, DBL * retx, DBL * rety, int * retpp)
{
	CData	*pcData;
	CData	*pcRetData;
	CData	*pcTData;
	DBL			tlen;
	int			tpp;
	DBL			tx, ty;

	*retlen = INTMAX;
	pcRetData = NULL;
	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcTData = pcData->SrchNearOnLinePoint(xx, yy, &tlen, &tx, &ty, &tpp);
		if(tlen < (*retlen)){
			pcRetData = pcTData;
			*retlen = tlen;
			*retpp = tpp;
			*retx = tx;
			*rety = ty;
		}
		pcData = pcData->GetNext();
	}
	return(pcRetData);
}

void CClipData::AtntionDraw(CDC *pcDC)
{
	CData	*pcData;

	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->AtntionDraw(pcDC);
		pcData = pcData->GetNext();
	}
}

void CClipData::NoactDraw(CDC *pcDC)
{
	CData	*pcData;

	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->NoactDraw(pcDC);
		pcData = pcData->GetNext();
	}
}

void CClipData::DelDraw(CDC *pcDC)
{
	CData	*pcData;

	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->DelDraw(pcDC);
		pcData = pcData->GetNext();
	}
}

void CClipData::StockClrDraw(CDC *pcDC)
{
	CData	*pcData;

	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->StockClrDraw(pcDC);
		pcData = pcData->GetNext();
	}
}

void CClipData::OnDraw(CDC *pcDC)
{
	CData	*pcData;
	CRect		crect;
	int		nSavedDC;

	nSavedDC = pcDC->SaveDC();
	pdrawclipmode(ON);
	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcDC->BeginPath();
		pcData->OutlineDraw(pcDC);
		pcDC->EndPath();
		pcDC->SelectClipPath(RGN_AND);
		pcData = pcData->GetNext();
	}
	pdrawclipmode(OFF);

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->OnDraw(pcDC);
		pcData = pcData->GetNext();
	}
	pcDC->RestoreDC(nSavedDC);

	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->OnDraw(pcDC);
		pcData = pcData->GetNext();
	}

}

CData * CClipData::CopyData(DBL movex, DBL movey)
{
	CClipData	*pcnewClipData;
	CData	*pcData;
	CData	*pcnewData;

	pcnewClipData = new CClipData(CLIPDATAID);
	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcnewData = (CData*)(pcData->CopyData(movex, movey));
		pcnewClipData->AddClip(pcnewData);
		pcData = pcData->GetNext();
	}

	pcData = m_pcClipTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcnewData = (CData*)(pcData->CopyData(movex, movey));
		pcnewClipData->AddData(pcnewData);
		pcData = pcData->GetNext();
	}
	return(pcnewClipData);
}

int CClipData::AddClip(CData *pcSetData)
{
	CData*	pcData;
	CData*	pcPreData;
	int		idx;

	idx = 0;
	pcData = m_pcClipTop;
	if(pcData == NULL){
		m_pcClipTop = pcSetData;
		pcSetData->SetNext(NULL);
		return(idx);
	}
	while(1){
		idx++;
		pcPreData = pcData;
		pcData = pcPreData->GetNext();
		if(pcData == NULL){
			pcPreData->SetNext(pcSetData);
			pcSetData->SetNext(NULL);
			return(idx);
			break;
		}
	}
	return(0);
}

int CClipData::AddData(CData *pcSetData)
{
	CData*	pcData;
	CData*	pcPreData;
	int		idx;

	idx = 0;
	pcData = m_pcDataTop;
	if(pcData == NULL){
		m_pcDataTop = pcSetData;
		pcSetData->SetNext(NULL);
		return(idx);
	}
	while(1){
		idx++;
		pcPreData = pcData;
		pcData = pcPreData->GetNext();
		if(pcData == NULL){
			pcPreData->SetNext(pcSetData);
			pcSetData->SetNext(NULL);
			return(idx);
			break;
		}
	}
	return(0);
}

void CClipData::DeleteData(CData *delData)
{
	CData*	crtData;
	CData*  preData;

	crtData = m_pcClipTop;
	if(crtData == NULL){
		return;
	}
	if(crtData == delData){
		crtData = crtData->GetNext();
		m_pcClipTop = crtData;
		return;
	}
	else{
		crtData->DeleteData(delData);
	}
	while(1){
		preData = crtData;
		crtData = crtData->GetNext();
		if(crtData == NULL){
			break;
		}
		if(crtData == delData){
			crtData = crtData->GetNext();
			preData->SetNext(crtData);
			break;
		}
		else{
			crtData->DeleteData(delData);
		}
	}
}

