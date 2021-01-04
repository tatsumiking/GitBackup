

//
// Easy Scanner System
//
// object datagrup.cpp  指定単位データリンク構造格納関数
//
// Copyright (c) 1998 by THEON

#include "stdafx.h"

#include "DataGrup.h"

#include	 "../ObjLib/Baselib.h"
#include	 "../ObjLib/Senvlib.h"
#include	 "../ObjLib/Scrnlib.h"

#define	DIVCALC	1

IMPLEMENT_SERIAL(CDataGrup, CVect, 0)
CDataGrup::CDataGrup()
{
	m_no = 0;
}

CDataGrup::CDataGrup(int nDummy)
:CVect(nDummy)
{
	m_pcDataTop = NULL;
	m_no = 0;
}

CDataGrup::~CDataGrup()
{
	CData*	pcData;
	CData*	nextData;
	int		idx;

	idx = 0;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		nextData = pcData->GetNext();
		delete(pcData);
		pcData = nextData;
	}
	return;
}

int CDataGrup::CheckID()
{
	return(DATAGRUPID);
}

void CDataGrup::Serialize(CArchive& ar)
{
	int		nID;
	CData*	pcData;
	int		r, g, b;
	COLORREF	clr;

	if (ar.IsStoring())
	{
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
			m_pcDataTop = NewDataCreate(nID);
			m_pcDataTop->Serialize(ar);
		}
		else{
			m_pcDataTop = NULL;
		}
	}
	if (!(ar.IsStoring())){
		ar >> nID;
		if(nID == DIBCDATAID){
			pcData = NewDataCreate(nID);
			pcData->Serialize(ar);
		}
		else{
			m_nPointCount = nID;
			ar >> m_nFillPen;
			if(m_nFillPen != -1
			&& m_nFillPen & 0x01000000){
				clr = m_nFillPen & 0x00ffffff;
				r = GetRValue(clr); 
				g = GetGValue(clr); 
				b = GetBValue(clr);
				m_nFillPen = senvgetpenno(r, g, b);
			}
			ar >> m_nOutLinePen;
			if(m_nOutLinePen != -1
			&& m_nOutLinePen & 0x01000000){
				clr = m_nOutLinePen & 0x00ffffff;
				r = GetRValue(clr);
				g = GetGValue(clr);
				b = GetBValue(clr);
				m_nOutLinePen = senvgetpenno(r, g, b);
			}
			ar >> m_dOutLineWidth;
			CData::Serialize(ar);
		}
	}
	else{
		CVect::Serialize(ar);
	}
	//CVect::Serialize(ar);
	if (!(ar.IsStoring()))
	{
		RenewMiniMax();
	}
}

void CDataGrup::ResetVectLink()
{
	CData*	pcData;
	CData*	nextData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		nextData = pcData->GetNext();
		pcData->ResetVectLink();
		delete(pcData);
		pcData = nextData;
	}
	m_pcDataTop = NULL;
}
int CDataGrup::GetLinkCount()
{
	CData*	pcData;
	int		count;

	count = 0;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		count++;
		pcData = pcData->GetNext();
	}
	return(count);
}
//p 内部データーが空の状態であれば削除しすべてが空のときTRUEを返す
int CDataGrup::CheckNullData()
{
	CTypedPtrList<CObList,CData*>     cLnkPtrList;
	CTypedPtrList<CObList,CData*>     cDelPtrList;
	POSITION	pos;
	CData	*pcData;

	if(m_pcDataTop == NULL){
		return(TRUE);
	}
	if(m_pcDataTop != NULL){
		pcData = m_pcDataTop;
		while(1){
			if(pcData == NULL){
				break;
			}
			if(pcData->CheckNullData() == TRUE){
				cDelPtrList.AddTail(pcData);
			}
			else{
				cLnkPtrList.AddTail(pcData);
			}
			pcData = pcData->GetNext();
		}

		pos = cDelPtrList.GetHeadPosition();
		while (pos != NULL)
		{
			pcData = cDelPtrList.GetNext(pos);
			if(pcData == NULL){
				break;
			}
			delete(pcData);
		}

		m_pcDataTop = NULL;
		pos = cLnkPtrList.GetHeadPosition();
		while (pos != NULL)
		{
			pcData = cLnkPtrList.GetNext(pos);
			if(pcData == NULL){
				break;
			}
			SetData(pcData);
		}
	}

	if(m_pcDataTop == NULL){
		return(TRUE);
	}
	return(FALSE);
}

CData* CDataGrup::GetData(int linkno)
{
	CData*	pcData;
	int		idx;

	idx = 0;
	pcData = m_pcDataTop;
	while(1){
		if(idx == linkno){
			return(pcData);
		}
		if(pcData == NULL){
			break;
		}
		pcData = pcData->GetNext();
		idx++;
	}
	return(NULL);
}

int CDataGrup::SetData(CData * pcSetData)
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

void CDataGrup::GetDMiniMax(DBL *minx, DBL *miny, DBL *maxx, DBL *maxy)
{
	CData*	pcData;
	DBL		tminx, tminy, tmaxx, tmaxy;

	(*minx) = INTMAX;
	(*miny) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	pcData = m_pcDataTop;
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

void CDataGrup::RenewMiniMax()
{
	CData*	pcData;

	m_dMinix = INTMAX;
	m_dMiniy = INTMAX;
	m_dMaxx = -INTMAX;
	m_dMaxy = -INTMAX;
	pcData = m_pcDataTop;
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

CData * CDataGrup::CopyData(DBL movex, DBL movey)
{
	CDataGrup*	pcnewData;
	CData*		pcData;
	CData*		pcnewSetData;

	pcnewData = (CDataGrup*)NewDataCreate(DATAGRUPID);
	pcnewData->SetFillPen(m_nFillPen);
	pcnewData->SetOutLinePen(m_nOutLinePen);
	pcnewData->SetOutLineWidth(m_dOutLineWidth);

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcnewSetData = pcData->CopyData(movex, movey);
		pcnewData->SetData(pcnewSetData);
		pcData = pcData->GetNext();
	}
	pcnewData->RenewMiniMax();
	// バックビットマップのコピーも必要
	return((CData*)pcnewData);
}

void CDataGrup::MoveTrns(DBL movex, DBL movey)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->MoveTrns(movex, movey);
		pcData = pcData->GetNext();
	}
	RenewMiniMax();
}

void CDataGrup::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SizeTrns(cntrx, cntry, timex, timey);
		pcData = pcData->GetNext();
	}
	RenewMiniMax();
}

void CDataGrup::SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SlopTrns(cntrx, cntry, ddx, ddy);
		pcData = pcData->GetNext();
	}
	RenewMiniMax();
}

void CDataGrup::MirrorTrns(DBL a, DBL b, DBL t1, DBL t2)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->MirrorTrns(a, b, t1, t2);
		pcData = pcData->GetNext();
	}
	RenewMiniMax();
}

void CDataGrup::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->RoundTrns(cntrx, cntry, tsin, tcos);
		pcData = pcData->GetNext();
	}
	RenewMiniMax();
}

void CDataGrup::StrctBoxsTrns()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->StrctBoxsTrns();
		pcData = pcData->GetNext();
	}
	RenewMiniMax();
}

void CDataGrup::MtrxTrns()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->MtrxTrns();
		pcData = pcData->GetNext();
	}
	RenewMiniMax();
}

CData* CDataGrup::SrchData(DBL xx, DBL yy, DBL *retlen)
{
	CData*	pcData;
	DBL		len;

	pcData = m_pcDataTop;
	*retlen = INTMAX;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SrchData(xx, yy, &len);
		if(*retlen > len){
			*retlen = len;
		}
		pcData = pcData->GetNext();
	}
	return(this);
}

CData* CDataGrup::SrchIdsData(int *nIDary, int nIDcnt, DBL xx, DBL yy, DBL *retlen)
{
	CData*	pcData;
	CData*	pctData;
	CData*	pcretData;
	DBL		len;
	int		i, nID;

	pcretData = NULL;
	*retlen = INTMAX;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		nID = pcData->CheckID();
		for(i = 0; i < nIDcnt; i++){
			if(nIDary[i] == nID){
				pctData = pcData->SrchData(xx, yy, &len);
				break;
			}
		}
		if(nIDcnt == i){
			pctData = pcData->SrchIdsData(nIDary, nIDcnt, xx, yy, &len);
		}
		if(*retlen > len){
			*retlen = len;
			pcretData = pctData;
		}
		pcData = pcData->GetNext();
	}
	return(pcretData);
}

CData* CDataGrup::SrchDataBoxs(DBL xx, DBL yy, DBL *retlen)
{
	CData*	pcData;
	DBL		len;

	pcData = m_pcDataTop;
	*retlen = INTMAX;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SrchDataBoxs(xx, yy, &len);
		if(*retlen > len){
			*retlen = len;
		}
		pcData = pcData->GetNext();
	}
	return(this);
}

CData* CDataGrup::SrchIdsDataBoxs(int *nIDary, int nIDcnt, DBL xx, DBL yy, DBL *retlen)
{
	CData*	pcData;
	CData*	pctData;
	CData*	pcretData;
	DBL		len;
	int		i, nID;

	pcretData = NULL;
	*retlen = INTMAX;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		nID = pcData->CheckID();
		for(i = 0; i < nIDcnt; i++){
			if(nIDary[i] == nID){
				pctData = pcData->SrchDataBoxs(xx, yy, &len);
				break;
			}
		}
		if(nIDcnt == i){
			pctData = pcData->SrchIdsData(nIDary, nIDcnt, xx, yy, &len);
		}
		if(*retlen > len){
			*retlen = len;
			pcretData = pctData;
		}
		pcData = pcData->GetNext();
	}
	return(pcretData);
}

CData* CDataGrup::SrchMoji(DBL xx, DBL yy, DBL *retlen, int *retlinkno)
{
	CData*	pcData;
	CData*	ptData;
	CData*	pretData;
	DBL		len;
	int		linkno;

	pretData = NULL;
	*retlen = INTMAX;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		ptData = pcData->SrchMoji(xx, yy, &len, &linkno);
		if(*retlen > len){
			*retlen = len;
			*retlinkno = linkno;
			pretData = ptData;
		}
		pcData = pcData->GetNext();
	}
	return(pretData);
}

void CDataGrup::FlagCheckMojiChange(int flag)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->FlagCheckMojiChange(flag);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::PlotOut(int nComID)
{
	CData*	pcData;

	baseDispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->PlotOut(nComID);
		pcData = pcData->GetNext();
		baseDispatch();
		if(senvchckflag(STOPFLAG) == ON){
			return;
		}
	}
}

void CDataGrup::CheckFlagDraw(int flag, CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->CheckFlagDraw(flag, pcDC);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->CheckFlagDrawSetClr(flag, pcDC, clr1, clr2, thick);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::CheckFlagOutlineDraw(int flag, CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->CheckFlagOutlineDraw(flag, pcDC);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::SelectDraw(CDC *pcDC, int no)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SelectDraw(pcDC, no);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::NoSelectDraw(CDC *pcDC, int no)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->NoSelectDraw(pcDC, no);
		pcData = pcData->GetNext();
	}
}

int CDataGrup::DrawNo(CDC *pcDC, int noidx)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		noidx = pcData->DrawNo(pcDC, noidx);
		pcData = pcData->GetNext();
	}
	return(noidx);
}
void CDataGrup::DrawSekijiNo(CDC *pcDC)
{
	DBL		sx, sy;
	DBL		ex, ey;
	TCHAR	buf[16];
	
	if(m_no == 0){
		return;
	}
	_stprintf_s(buf, 16, _T("%03d"), m_no);
	GetDMiniMax(&sx, &sy, &ex, &ey);
	scrndxymmtorltv(&sx, &ey);
	::TextOut(pcDC->m_hDC,(int)sx, (int)ey-12,  buf, _tcslen(buf));
}
void CDataGrup::AtntionDraw(CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(senvchckstopflag() == ON){
			break;
		}
		pcData->AtntionDraw(pcDC);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::NoactDraw(CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(senvchckstopflag() == ON){
			break;
		}
		pcData->NoactDraw(pcDC);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::DelDraw(CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->DelDraw(pcDC);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::StockClrDraw(CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(senvchckstopflag() == ON){
			break;
		}
		pcData->StockClrDraw(pcDC);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::OnDraw(CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(senvchckstopflag() == ON){
			break;
		}
		pcData->OnDraw(pcDC);
		pcData = pcData->GetNext();
	}
}
void CDataGrup::GdiplusFillDraw(CGdiPlusEx *pcGpe)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while (1) {
		if (pcData == NULL) {
			break;
		}
		if (senvchckstopflag() == ON) {
			break;
		}
		pcData->GdiplusFillDraw(pcGpe);
		pcData = pcData->GetNext();
	}
}
void CDataGrup::OutlineDraw(CDC *pcDC)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->OutlineDraw(pcDC);
		pcData = pcData->GetNext();
	}
}

CRect* CDataGrup::GetBoundingRect()
{
	DBL	dminx, dminy, dmaxx, dmaxy;
	int	nminx, nminy, nmaxx, nmaxy;

	GetDMiniMax(&dminx, &dminy, &dmaxx, &dmaxy);
	scrndxymmtorltv(&dminx, &dminy);
	scrndxymmtorltv(&dmaxx, &dmaxy);
	nminx = (int)dminx;
	nminy = (int)dminy;
	nmaxx = (int)dmaxx;
	nmaxy = (int)dmaxy;
	m_rectData.SetRect(nminx, nminy, nmaxx, nmaxy);
	return(&m_rectData);
}

void CDataGrup::InAreaDataAtntionOn(DBL sx, DBL sy, DBL ex, DBL ey)
{
	CData*	pcData;

	if(CheckFlag(ACTIVEFLAG) == OFF){
		return;
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->InAreaDataAtntionOn(sx, sy, ex, ey);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::OffFlag(int bitflag)
{
	CData*	pcData;

	CVect::OffFlag(bitflag);
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->OffFlag(bitflag);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::OnFlag(int bitflag)
{
	CData*	pcData;

	CVect::OnFlag(bitflag);
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->OnFlag(bitflag);
		pcData = pcData->GetNext();
	}
}

int CDataGrup::CheckFlag(int bitflag)
{
	return(CVect::CheckFlag(bitflag));
}

void CDataGrup::ChngFlag(int bitflag)
{
	CData*	pcData;

	CVect::ChngFlag(bitflag);
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->OnFlag(bitflag);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::SetAtnVectFSF1fBuf(CFSF1Font * pcFSF1Font)
{
	CData*	pcData;

	if(CheckFlag(ACTIVEFLAG) == OFF){
		return;
	}
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SetAtnVectFSF1fBuf(pcFSF1Font);
		pcData = pcData->GetNext();
	}
}

CData *CDataGrup::SrchNearOnLinePoint(DBL xx, DBL yy, DBL * retlen, DBL * retx, DBL * rety, int * retpp)
{
	CData*	pcData;
	CData*  retData;
	CData*  tpcData;
	DBL		len;
	DBL		x, y;
	int		pp;

	retData = NULL;
	*retlen = INTMAX;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		tpcData = pcData->SrchNearOnLinePoint(xx, yy, &len, &x, &y, &pp);
		if(*retlen > len){
			*retlen = len;
			*retx = x;
			*rety = y;
			*retpp = pp;
			retData = tpcData;
		}
		pcData = pcData->GetNext();
	}
	return(retData);
}

CData* CDataGrup::SrchNearPoint(DBL xx, DBL yy, DBL * retlen, int * retpp)
{
	CData*	pcData;
	CData*  retData;
	CData*  tpcData;
	DBL		len;
	int		pp;

	retData = NULL;
	*retlen = INTMAX;
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		tpcData = pcData->SrchNearPoint(xx, yy, &len, &pp);
		if(*retlen > len){
			*retlen = len;
			*retpp = pp;
			retData = tpcData;
		}
		pcData = pcData->GetNext();
	}
	return(retData);
}

void CDataGrup::DeleteData(CData * delData)
{
	CData*	crtData;
	CData*  preData;

	crtData = m_pcDataTop;
	if(crtData == NULL){
		return;
	}
	if(crtData == delData){
		crtData = crtData->GetNext();
		m_pcDataTop = crtData;
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

void CDataGrup::FlagOnDelData(int flag)
{
	CData*	crtData;
	CData*  preData;
	CData*  nextData;

	crtData = m_pcDataTop;
	if(crtData == NULL){
		return;
	}
	if(crtData->CheckFlag(flag) == ON){
		nextData = crtData->GetNext();
		m_pcDataTop = nextData;
		delete(crtData);
		return;
	}
	else{
		crtData->FlagOnDelData(flag);
	}
	while(1){
		preData = crtData;
		crtData = crtData->GetNext();
		if(crtData == NULL){
			break;
		}
		if(crtData->CheckFlag(flag) == ON){
			nextData = crtData->GetNext();
			preData->SetNext(nextData);
			delete(crtData);
			crtData = preData;
		}
		else{
			crtData->FlagOnDelData(flag);
		}
	}
}

void CDataGrup::SaveStrgStoreFile(FILE * fp, int id)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SaveStrgStoreFile(fp, id);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::LoadStrgStoreFile(FILE * fp, int id)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->LoadStrgStoreFile(fp, id);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::PointYLegal(DBL yy)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->PointYLegal(yy);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::PointXLegal(DBL xx)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->PointXLegal(xx);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::AllOnPointFlag(int bitflag)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->AllOnPointFlag(bitflag);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::AllOffPointFlag(int bitflag)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->AllOffPointFlag(bitflag);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::PointPicup()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->PointPicup();
		pcData = pcData->GetNext();
	}
}

void CDataGrup::SetDefOutLineClr()
{
	CData*	pcData;

	CVect::SetDefOutLineClr();
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SetDefOutLineClr();
		pcData = pcData->GetNext();
	}
}

void CDataGrup::SetDefFillClr()
{
	CData*	pcData;

	CVect::SetDefFillClr();
	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SetDefFillClr();
		pcData = pcData->GetNext();
	}
}

void CDataGrup::SaveDxfFile()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SaveDxfFile();
		pcData = pcData->GetNext();
	}
}

void CDataGrup::SaveEpsFile()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SaveEpsFile();
		pcData = pcData->GetNext();
	}
}

void CDataGrup::OutLineOmit(DBL mm)
{
	CData*	pcData;
	CData*	pcnewData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(pcData->CheckID() == BASEVECTID){
			pcData->OnFlag(COPYFLAG);	
		}
		else{
			pcData->OutLineOmit(mm);
		}
		pcData = pcData->GetNext();
	}

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		if(pcData->CheckFlag(COPYFLAG) == ON){
			pcData->OffFlag(COPYFLAG);
			pcnewData = pcData->CopyData(0.0, 0.0);
			pcnewData->OutLineOmit(mm);
			SetData(pcnewData);
		}
		pcData = pcData->GetNext();
	}

	RenewMiniMax();
}

void CDataGrup::ReverceRejion()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->ReverceRejion();
		pcData = pcData->GetNext();
	}
}

void CDataGrup::SetUseClrPenno(int notbl[], LPINT cnt)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->SetUseClrPenno(notbl, cnt);
		pcData = pcData->GetNext();
	}
}

void CDataGrup::StrgFiting()
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->StrgFiting();
		pcData = pcData->GetNext();
	}
}

void CDataGrup::CnvBezeToLine(int divcnt)
{
	CData*	pcData;

	pcData = m_pcDataTop;
	while(1){
		if(pcData == NULL){
			break;
		}
		pcData->CnvBezeToLine(divcnt);
		pcData = pcData->GetNext();
	}
}
