
//
// Easy Scanner System
//
// object strgtext.cpp  文字列フォント構造関係関数
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include <math.h>

#include <mbstring.h>
#include "datagrup.h"
#include "strgtext.h"

extern "C" {
	int	senvchckstopflag();
	int basesetcodetbl(LPSTR strg, UINT code[]);
	void senvgetpenrgb(int pno, LPINT rval, LPINT gval, LPINT bval);
	void scrnsetdelpen(HDC PaintDC);
	void scrnsetslctpen(HDC PaintDC);
	void scrnsetnoslctpen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	HBRUSH senvgetmainbrush();
	void scrndxymmtorltv(DBL *x, DBL *y);
	void scrndlenrltvtomm(DBL *dlen);
	int	senvcheckdrawmode();
}

IMPLEMENT_SERIAL(CStrgText, CObject, 3)
CStrgText::CStrgText()
{
}

// そのうちにm_pcMojiTopをm_pcDataTopに
// SetMojiLinkをSetDataにする

CStrgText::CStrgText(int nDummy)
	: CVect(nDummy)
{
	strcpy(m_szFontName, "ＭＳ 明朝");
	SetTateYoko(YOKOGAKI);
	m_dStrlen = 0.0;
	m_dSX = 0.0;
	m_dSY = 0.0;
	m_dSize = 256;
	m_dLineSpace = 0;
	m_dSpacePar = 0.0;
	m_dFlatPar = 100.0;
	m_nBold = OFF;
	m_nSlope = OFF;
	m_cMojiTextList.RemoveAll( );
}

CStrgText::~CStrgText()
{
	AllFree();
}

int CStrgText::CheckID()
{
	return(STRGTEXTID);
}

void CStrgText::Serialize(CArchive& ar)
{
	POSITION	pos;
	int			nID;
	CMojiText	*pcMoji;

	if (ar.IsStoring())
	{
		ar << CString(m_szFontName);
		ar << m_nStrgFlag;
		ar << m_dStrlen;
		ar << m_dLineSpace;
		ar << m_dSX;
		ar << m_dSY;
		ar << m_dSize;
		ar << m_dSpacePar;
		ar << m_dFlatPar;
		ar << m_nBold;
		ar << m_nSlope;
		pos = m_cMojiTextList.GetHeadPosition();
		while(pos){
			pcMoji = m_cMojiTextList.GetNext(pos);
			nID = pcMoji->CheckID();
			ar << nID;
			pcMoji->Serialize(ar);
		}
		nID = NOOPDATAID;
		ar << nID;
	}
	else
	{
		CString csFontName;
		ar >> csFontName;
		lstrcpy(m_szFontName, csFontName);
		ar >> m_nStrgFlag;
		ar >> m_dStrlen;
		ar >> m_dLineSpace;
		ar >> m_dSX;
		ar >> m_dSY;
		ar >> m_dSize;
		ar >> m_dSpacePar;
		ar >> m_dFlatPar;
		ar >> m_nBold;
		ar >> m_nSlope;
		while(1){
			ar >> nID;
			if(nID == NOOPDATAID){
				break;
			}
			pcMoji = (CMojiText*)NewDataCreate(nID);
			pcMoji->m_dTKtknPar = m_dTKtknPar; 
			pcMoji->m_dTHrgnPar = m_dTHrgnPar; 
			pcMoji->SetBaseFontSize(m_dSize);
			pcMoji->Serialize(ar);
			m_cMojiTextList.AddTail(pcMoji);
		}
	}
	CVect::Serialize(ar);

	if (!(ar.IsStoring()))
	{
		// 2002.07.23 立身 昔のデータが0（黒）であるため1（黒）に設定しなおし
		if(m_nFillPen == 0){
			m_nFillPen = 1;
		}
		if(m_nOutLinePen == 0){
			m_nOutLinePen = 1;
		}
		RenewMiniMax();
	}
}

//p 内部データーが空の状態であれば削除しすべてが空のときTRUEを返す
int CStrgText::CheckNullData()
{
	POSITION	pos, tpos;
	CMojiText*	pcMoji;

	if(m_cMojiTextList.IsEmpty()){
		return(TRUE);
	}
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		tpos = pos;
		pcMoji = m_cMojiTextList.GetNext(pos);
		if(pcMoji->CheckNullData() == TRUE){
			m_cMojiTextList.RemoveAt(tpos);
			delete(pcMoji);
		}
	}
	if(m_cMojiTextList.IsEmpty()){
		return(TRUE);
	}
	return(FALSE);
}

void CStrgText::SetTateYoko(int flag)
{
	CMojiText	*pcMoji;

	m_nStrgFlag &= 0xfff0;
	m_nStrgFlag |= flag;
	POSITION pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->SetTateGaki(flag);
	}
}

int CStrgText::CheckTateYoko()
{
	return(m_nStrgFlag & 0x000f);
}

void CStrgText::SetFontName(LPSTR sFname)
{
	strcpy(m_szFontName, sFname);
}

void CStrgText::GetFontName(LPSTR sFname)
{
	strcpy(sFname, m_szFontName);
}

void CStrgText::SetSpaceEnv(DBL dMSpace, DBL dLSpace)
{
	SetDXY(1, dMSpace, dLSpace);
}

void CStrgText::GetSpaceEnv(DBL *dMSpace, DBL *dLSpace)
{
	GetDXY(1, dMSpace, dLSpace);
}

void CStrgText::SetStrStartXY(DBL dSx, DBL dSy)
{
	SetDXY(2, dSx, dSy);
}

void CStrgText::GetStrStartXY(DBL *dSx, DBL *dSy)
{
	GetDXY(2, dSx, dSy);
}

void CStrgText::SetStrLen(DBL strlen)
{
	m_dStrlen = strlen;
}

void CStrgText::GetStrLen(LPDBL strlen)
{
	*strlen = m_dStrlen;
}

void CStrgText::SetLineSpace(DBL space)
{
	m_dLineSpace = space;
}

void CStrgText::GetLineSpace(LPDBL space)
{
	*space = m_dLineSpace;
}

void CStrgText::SetStartXY(DBL sx, DBL sy)
{
	m_dSX = sx;
	m_dSY = sy;
}

void CStrgText::GetStartXY(LPDBL sx, LPDBL sy)
{
	*sx = m_dSX;
	*sy = m_dSY;
}

void CStrgText::SetSize(DBL size)
{
	m_dSize = size;
}

void CStrgText::GetSize(LPDBL size)
{
	*size = m_dSize;
}

void CStrgText::SetSpacePar(DBL par)
{
	m_dSpacePar = par;
}

void CStrgText::GetSpacePar(LPDBL par)
{
	*par = m_dSpacePar;
}

void CStrgText::SetFlatPar(DBL par)
{
	m_dFlatPar = par;
}

void CStrgText::GetFlatPar(LPDBL par)
{
	*par = m_dFlatPar;
}

void CStrgText::GetBold(LPINT flag)
{
	*flag = m_nBold;
}

void CStrgText::SetBold(int flag)
{
	m_nBold = flag;
}

void CStrgText::GetSlope(LPINT flag)
{
	*flag = m_nSlope;
}

void CStrgText::SetSlope(int flag)
{
	m_nSlope = flag;
}

void CStrgText::AllFree()
{
	POSITION	pos;
	CMojiText*	pcMoji;

	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		delete(pcMoji);
	}
	m_cMojiTextList.RemoveAll( );
}

CData *CStrgText::GetIdxMoji(int nidx)
{
	POSITION	pos;
	int			idx;
	CMojiText*	pcMoji;

	idx = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		if(idx == nidx){
			return((CData*)pcMoji);
		}
		idx++;
	}
	return(NULL);
}

void CStrgText::CheckFlagDraw(int flag, CDC *pcDC)
{
	POSITION	pos;
	CMojiText*	pcMoji;

	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->CheckFlagDraw(flag, pcDC);
	}
}

void CStrgText::CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick)
{
	POSITION	pos;
	CMojiText*	pcMoji;

	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->CheckFlagDrawSetClr(flag, pcDC, clr1, clr2, thick);
	}
}

void CStrgText::AtntionDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	scrnsetslctpen(pcDC->m_hDC);
	StockClrDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CStrgText::AtntionMojiDraw(CDC *pcDC, int nidx)
{
	CMojiText*	pcMojiText;

	scrnsetslctpen(pcDC->m_hDC);
	pcMojiText = (CMojiText*)GetIdxMoji(nidx);
	if(pcMojiText != NULL){
		pcMojiText->FillClrDraw(pcDC);
	}
	scrnpenend(pcDC->m_hDC);
}

void CStrgText::NoactDraw(CDC *pcDC)
{
	CMojiText	*pcMoji;

	if(senvchckstopflag() == ON){
		return;
	}
	scrnsetnoslctpen(pcDC->m_hDC);
	POSITION pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		if(senvchckstopflag() == ON){
			return;
		}
		pcMoji->FillClrDraw(pcDC);
	}
	scrnpenend(pcDC->m_hDC);
}

void CStrgText::DelDraw(CDC *pcDC)
{
	scrnsetdelpen(pcDC->m_hDC);
	StockClrDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CStrgText::OnDraw(CDC *pcDC)
{
	POSITION	pos;
	CMojiText*	pcMoji;

	if(senvchckstopflag() == ON){
		return;
	}
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		if(senvchckstopflag() == ON){
			return;
		}
		pcMoji->OnDraw(pcDC);
	}
}

void CStrgText::StockClrDraw(CDC *pcDC)
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		sx, sy, ex, ey;
	DBL		wd, hi;

	if(senvgetmainbrush() != NULL){
		pos = m_cMojiTextList.GetHeadPosition();
		while(pos != NULL){
			if(senvchckstopflag() == ON){
				return;
			}
			pcMoji = m_cMojiTextList.GetNext(pos);
			pcMoji->FillClrDraw(pcDC);
		}
	}
	GetStartXY(&sx, &sy);
	GetFontSize(&wd, &hi);
	ex = sx + wd;
	ey = sy + hi;
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	BoxsDraw(pcDC, (int)sx-2, (int)sy-2, (int)sx+2, (int)sy+2);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)sx, (int)ey);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)sy);
}

void CStrgText::OutlineDraw(CDC *pcDC)
{
	POSITION	pos;
	CMojiText*	pcMoji;

	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->FillClrDraw(pcDC);
	}
}

void CStrgText::SetString(char *pstr)
{
	int		len, idx;
	UINT	codetbl[RECMAX];
	CMojiText*	pcMoji;

	CStrgText::AllFree();
	len = strlen(pstr);
	if(len <= 0){
		return;
	}
	if(RECMAX <= len){
		pstr[RECMAX-1] = '\0';
	}
	len = basesetcodetbl(pstr, codetbl);
	idx = 0;
	for(idx = 0; idx < len; idx++){
		pcMoji = (CMojiText*)NewDataCreate(MOJITEXTID);
		pcMoji->SetFont(m_szFontName);
		pcMoji->SetTateGaki(CheckTateYoko());
		pcMoji->SetCode(codetbl[idx]);
		pcMoji->SetFillPen(m_nFillPen);
		pcMoji->SetSpacePar(m_dSpacePar);
		pcMoji->SetFlatPar(m_dFlatPar);
		pcMoji->SetBold(m_nBold);
		pcMoji->SetSlope(m_nSlope);
		m_cMojiTextList.AddTail(pcMoji);
	}
	StringElementInit();
}

void CStrgText::GetString(char *pstr)
{
	POSITION	pos;
	CMojiText*	pcMoji;
	UINT	code;
	LPSTR	sp;

	sp = pstr;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetCode(&code);
		if(code < 0x0100){
			*sp++ = code;
		}
		else{
			*sp++ = (code >> 8) & 0x00ff;
			*sp++ = (code) & 0x00ff;
		}
	}
	*sp = '\0';
}

int CStrgText::GetBeforeMojiIdx(int mojiidx)
{
	return(mojiidx-1);
}

int CStrgText::GetAfterMojiIdx(int mojiidx)
{
	POSITION	pos;
	CMojiText*	pcMoji;
	int		idx, tidx;

	idx = mojiidx+1;
	tidx = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		if(tidx == idx){
			return(idx);
		}
		tidx++;
	}
	idx = -1;
	return(idx);
}

void CStrgText::MoveAfterMoji(int mojiidx, DBL movex, DBL movey)
{
	POSITION	pos;
	CMojiText*	pcMoji;
	int		idx;

	idx = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		if(mojiidx < idx){
			pcMoji->MoveTrns(movex, movey);
		}
		idx++;
	}
}

void CStrgText::GetMojiWakuMiniMax(int idx, DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	CMojiText*	pcMoji;

	pcMoji = (CMojiText*)GetIdxMoji(idx);
	if(pcMoji != NULL){
		pcMoji->GetWakuDMiniMax(minix, miniy, maxx, maxy);
	}
}

void CStrgText::GetMojiMiniMax(int idx, DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	CMojiText*	pcMoji;

	pcMoji = (CMojiText*)GetIdxMoji(idx);
	if(pcMoji != NULL){
		pcMoji->GetDataDMiniMax(minix, miniy, maxx, maxy);
	}
}

void CStrgText::GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	GetWakuDMiniMax(minix, miniy, maxx, maxy);
}

void CStrgText::GetWakuDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		wd, hi;
	DBL		sx, sy, ex, ey;
	DBL		tminix, tminiy, tmaxx, tmaxy;

	(*minix) = INTMAX;
	(*miniy) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	if(m_cMojiTextList.IsEmpty()){
		GetStartXY(&sx, &sy);
		GetFontSize(&wd, &hi);
		ex = sx + wd;
		ey = sy + hi;
		(*minix) = min((*minix), sx);
		(*miniy) = min((*miniy), sy);
		(*maxx) = max((*maxx), sx);
		(*maxy) = max((*maxy), sy);
		return;
	}

	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetWakuDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
		(*minix) = min((*minix), tminix);
		(*miniy) = min((*miniy), tminiy);
		(*maxx) = max((*maxx), tmaxx);
		(*maxy) = max((*maxy), tmaxy);
	}
}

void CStrgText::GetDataDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		wd, hi;
	DBL		sx, sy, ex, ey;
	DBL		tminix, tminiy, tmaxx, tmaxy;

	(*minix) = INTMAX;
	(*miniy) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	if(m_cMojiTextList.IsEmpty()){
		GetStartXY(&sx, &sy);
		GetFontSize(&wd, &hi);
		ex = sx + wd;
		ey = sy + hi;
		(*minix) = min((*minix), sx);
		(*miniy) = min((*miniy), sy);
		(*maxx) = max((*maxx), sx);
		(*maxy) = max((*maxy), sy);
		return;
	}

	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetDataDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
		(*minix) = min((*minix), tminix);
		(*miniy) = min((*miniy), tminiy);
		(*maxx) = max((*maxx), tmaxx);
		(*maxy) = max((*maxy), tmaxy);
	}
}

DBL CStrgText::GetDataSumLength()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		sp, ep;
	DBL		sumlen;

	sumlen = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetDataSEP(&sp, &ep);
		sumlen += (ep-sp);	
	}
	return(sumlen);
}

DBL CStrgText::GetWakuSumLength()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		sumlen, size;

	sumlen = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetFontWakuWidth(&size);
		sumlen += size;
	}
	return(sumlen);
}

void CStrgText::RenewMiniMax()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		minix, miniy, maxx, maxy;

	m_dMinix = INTMAX;
	m_dMiniy = INTMAX;
	m_dMaxx = -INTMAX;
	m_dMaxy = -INTMAX;
	if(m_cMojiTextList.IsEmpty()){
		GetDMiniMax(&m_dMinix, &m_dMiniy, &m_dMaxx, &m_dMaxy);
	}
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->RenewMiniMax();
		pcMoji->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
		m_dMinix = min(minix, m_dMinix);
		m_dMiniy = min(miniy, m_dMiniy);
		m_dMaxx = max(maxx, m_dMaxx);
		m_dMaxy = max(maxy, m_dMaxy);
	}
}

// ここまで
CData * CStrgText::CopyData(DBL movex, DBL movey)
{
	CStrgText *pcnewData;

	pcnewData = (CStrgText*)NewDataCreate(STRGTEXTID);
	CopyDataElement(pcnewData, movex, movey);
	pcnewData->RenewMiniMax();
	return((CData*)pcnewData);
}

void CStrgText::CopyDataElement(CStrgText *pcnewData, DBL movex, DBL movey)
{
	POSITION	pos;
	CMojiText	*pcMoji;
	CMojiText	*pcnewMoji;

	pcnewData->SetFontName(m_szFontName);
	pcnewData->SetStrLen(m_dStrlen);
	pcnewData->SetLineSpace(m_dLineSpace);
	pcnewData->SetStartXY(m_dSX+movex, m_dSY+movey);
	pcnewData->SetSize(m_dSize);
	pcnewData->SetSpacePar(m_dSpacePar);
	pcnewData->SetFlatPar(m_dFlatPar);
	pcnewData->SetBold(m_nBold);
	pcnewData->SetSlope(m_nSlope);
	pcnewData->m_nStrgFlag = m_nStrgFlag;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcnewMoji = (CMojiText*)pcMoji->CopyData(movex, movey);
		pcnewData->m_cMojiTextList.AddTail(pcnewMoji);
	}
	pcnewData->SetFillPen(m_nFillPen);
}

void CStrgText::MoveTrns(DBL movex, DBL movey)
{
	POSITION	pos;
	CMojiText*	pcMoji;

	m_dSX = m_dSX + movex;
	m_dSY = m_dSY + movey;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->MoveTrns(movex, movey);
	}
	RenewMiniMax();
}

void CStrgText::IdxMoveTrns(int idx, DBL movex, DBL movey)
{
	CMojiText*	pcMoji;

	pcMoji = (CMojiText*)GetIdxMoji(idx);
	if(pcMoji != NULL){
		pcMoji->MoveTrns(movex, movey);
	}
}

void CStrgText::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	POSITION	pos;
	CMojiText*	pcMoji;

	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
		m_dSize *= timex;
	}
	else{
		m_dSize *= timey;
	}
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->SizeTrns(cntrx, cntry, timex, timey);
	}
	RenewMiniMax();
}

void CStrgText::IdxSizeTrns(int idx, DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	CMojiText*	pcMoji;

	pcMoji = (CMojiText*)GetIdxMoji(idx);
	if(pcMoji != NULL){
		pcMoji->SizeTrns(cntrx, cntry, timex, timey);
	}
}

void CStrgText::SlopeTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
{
	POSITION	pos;
	CMojiText*	pcMoji;

	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->SlopTrns(cntrx, cntry, ddx, ddy);
	}
}

void CStrgText::MirrorTrns(DBL a, DBL b, DBL t1, DBL t2)
{
	POSITION	pos;
	CMojiText*	pcMoji;

	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->MirrorTrns(a, b, t1, t2);
	}
	RenewMiniMax();
}

void CStrgText::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	POSITION	pos;
	CMojiText*	pcMoji;

	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->RoundTrns(cntrx, cntry, tsin, tcos);
	}
	RenewMiniMax();
}

void CStrgText::IdxRoundTrns(int idx, DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	CMojiText*	pcMoji;

	pcMoji = (CMojiText*)GetIdxMoji(idx);
	if(pcMoji != NULL){
		pcMoji->RoundTrns(cntrx, cntry, tsin, tcos);
	}
}

void CStrgText::OnFlag(int bitflag)
{
	POSITION	pos;
	CMojiText*	pcMoji;

	CVect::OnFlag(bitflag);
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->OnFlag(bitflag);
	}
}

void CStrgText::OffFlag(int bitflag)
{
	POSITION	pos;
	CMojiText*	pcMoji;

	CVect::OffFlag(bitflag);
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->OffFlag(bitflag);
	}
}

void CStrgText::DataChange()
{
	POSITION	pos;
	CMojiText*	pcMoji;

	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->DataChange();
	}
}

void CStrgText::DataChangeSetDataGrup(CDataGrup *pcDataGrup)
{
	POSITION	pos;
	CMojiText*	pcMoji;
	CDataList*	pcDataList;

	DataChange();
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		if((pcMoji->m_pcDataList) != NULL){
			pcDataList = (CDataList*)(pcMoji->m_pcDataList)->CopyData(0.0, 0.0);
			// 2002.01.15 文字解除すると縁取られる修正
			pcDataList->SetOutLinePen(-1);
			pcDataList->RenewMiniMax();
			pcDataGrup->SetData(pcDataList);
		}
	}
}

BOOL CStrgText::CheckDataChange()
{
	return(TRUE);
}

void CStrgText::FreeIdxObject(int idx)
{
	POSITION	pos, tpos;
	CMojiText*	pcMoji;
	int		tidx;

	tidx = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		tpos = pos;
		pcMoji = m_cMojiTextList.GetNext(pos);
		if(tidx == idx){
			m_cMojiTextList.RemoveAt(tpos);
			delete(pcMoji);
			break;
		}
		tidx++;
	}
}

int CStrgText::CheckIdxFlag(int idx, int flag)
{
	CMojiText*	pcMoji;
	int			stat;

	stat = OFF;
	pcMoji = (CMojiText*)GetIdxMoji(idx);
	if(pcMoji != NULL){
		stat = pcMoji->CheckFlag(flag);
	}
	return(stat);
}

void CStrgText::OnIdxFlag(int idx, int flag)
{
	CMojiText*	pcMoji;

	pcMoji = (CMojiText*)GetIdxMoji(idx);
	if(pcMoji != NULL){
		pcMoji->OnFlag(flag);
	}
}

void CStrgText::OffIdxFlag(int idx, int flag)
{
	CMojiText*	pcMoji;

	pcMoji = (CMojiText*)GetIdxMoji(idx);
	if(pcMoji != NULL){
		pcMoji->OffFlag(flag);
	}
}

void CStrgText::SetFontSize(DBL dFWsize, DBL dFHsize)
{
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
		m_dSize = dFWsize;
		m_dFlatPar = dFHsize / m_dSize * 100.0;
	}
	else{
		m_dSize = dFHsize;
		m_dFlatPar = dFWsize / m_dSize * 100.0;
	}
}

void CStrgText::GetFontSize(DBL *width, DBL *hight)
{
	if(m_dFlatPar == MJFREEFLATPAR){
		*width = m_dSize;
		*hight = m_dSize;
	}
	else{
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			*width = m_dSize;
			*hight = m_dSize * m_dFlatPar / 100.0;
		}
		else{
			*width = m_dSize * m_dFlatPar / 100.0;
			*hight = m_dSize;
		}
	}
}

void CStrgText::BoxsDraw(CDC *pcDC, DBL sx, DBL sy, DBL ex, DBL ey)
{
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)sx, (int)ey);
	pcDC->LineTo((int)ex, (int)ey);
	pcDC->LineTo((int)ex, (int)sy);
	pcDC->LineTo((int)sx, (int)sy);
}

CData* CStrgText::SrchData(DBL dx, DBL dy, DBL *retlen)
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		sx, sy, ex, ey;
	DBL		tlen;

	if(m_cMojiTextList.IsEmpty()){
		GetDMiniMax(&sx, &sy, &ex, &ey);
		*retlen = GetPreLengthBoxs(dx, dy, sx, sy, ex, ey);
		return(this);
	}
	*retlen = INTMAX;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->SrchData(dx, dy, &tlen);
		if((*retlen) > tlen){
			(*retlen) = tlen;
		}
	}
	return(this);	
}

CData* CStrgText::SrchDataBoxs(DBL dx, DBL dy, DBL *retlen)
{
	SrchData(dx, dy, retlen);
	return(this);
}

CData *CStrgText::SrchNearOnLinePoint(DBL dx, DBL dy, DBL *retlen, DBL *retx, DBL *rety, int *retpp)
{
	POSITION	pos;
	CMojiText*	pcMoji;
	CData*	pcData;
	CData*	retData;
	DBL		len;
	DBL		x, y;
	int		pp;

	*retlen = INTMAX;
	if(m_cMojiTextList.IsEmpty()){
		return(NULL);
	}
	retData = NULL;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcData = pcMoji->SrchNearOnLinePoint(dx, dy, &len, &x, &y, &pp);
		if((*retlen) > len){
			*retlen = len;
			*retx = x;
			*rety = y;
			*retpp = pp;
			retData = pcData;
		}
	}
	return(retData);
}

CData* CStrgText::SrchNearPoint(DBL dx, DBL dy, DBL *retlen, int *retpp)
{
	POSITION	pos;
	CMojiText*	pcMoji;
	CData*	pcData;
	CData*	retData;
	DBL		len;
	int		pp;

	*retlen = INTMAX;
	*retpp = -1;
	if(m_cMojiTextList.IsEmpty()){
		return(NULL);
	}
	retData = NULL;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcData = pcMoji->SrchNearPoint(dx, dy, &len, &pp);
		if((*retlen) > len){
			*retlen = len;
			*retpp = pp;
			retData = pcData;
		}
	}
	return(retData);
}

CData* CStrgText::SrchMoji(DBL dx, DBL dy, DBL *retlen, int *retmidx)
{
	POSITION	pos;
	CMojiText*	pcMoji;
	int		idx;
	DBL		tlen;

	*retlen = INTMAX;
	*retmidx = -1;
	if(m_cMojiTextList.IsEmpty()){
		return(NULL);
	}
	idx = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		// 2002.08.20 立身 DFG中太楷書体で氏名枠のスペースの次の文字が選択できない
		if(pcMoji->m_nCode != 0x8140){
			pcMoji->SrchData(dx, dy, &tlen);
			if((*retlen) > tlen){
				(*retlen) = tlen;
				*retmidx = idx;
			}
		}
		idx++;
	}
	return(this);
}

void CStrgText::SetDefFillClr()
{
	POSITION	pos;
	CMojiText*	pcMoji;

	CVect::SetDefFillClr();
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->SetDefFillClr();
	}
}

void CStrgText::DeleteData(CData *delData)
{
	POSITION	pos, delpos;
	CMojiText*	pcMoji;

	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		delpos = pos;
		pcMoji = m_cMojiTextList.GetNext(pos);
		if(delData == pcMoji){
			m_cMojiTextList.RemoveAt(delpos);
		}
		else{
			pcMoji->DeleteData(delData);
		}
	}
}

void CStrgText::StrgFiting()
{
	StringElementInit();
}

void CStrgText::StringElementInit()
{
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		TategakiElementInit();
	}
	else{
		YokogakiElementInit();
	}
}

// 内部関数
void CStrgText::TategakiElementInit()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	int		hcnt, zcnt;
	DBL		charcnt;
	DBL		dW, dH;
	DBL		space;
	DBL		sx, sy;
	UINT	code;

	GetCharCount(&hcnt, &zcnt);
	charcnt = hcnt/2 + zcnt;
	GetFontSize(&dW, &dH);
	if(m_dStrlen != 0){
		if((hcnt == 1 && zcnt == 0)
		|| (hcnt == 0 && zcnt == 1)){
			space = (m_dStrlen - (hcnt*dH/2 + zcnt*dH)) / 2;
			sx = m_dSX; sy = m_dSY - space - dH;
		}
		else{
			space = (m_dStrlen - (hcnt*dH/2 + zcnt*dH)) / (charcnt - 1);
			sx = m_dSX; sy = m_dSY - dH;
		}
	}
	else{
		sx = m_dSX; sy = m_dSY - dH;
		space = dH * m_dSpacePar / 100.0;
	}
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->SetFontSize(dW, dH);
		pcMoji->SetBaseFontSize(dW);
		pcMoji->SetStartXY(sx, sy);
		pcMoji->GetCode(&code);
		if(code < 0x0100){
			sy -= (dH/2 + space/2);
		}
		else{
			sy -= (dH + space);
		}
	}
}

void CStrgText::YokogakiElementInit()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	int		hcnt, zcnt;
	DBL		charcnt;
	DBL		dW, dH;
	DBL		space;
	DBL		sx, sy;
	UINT	code;

	GetCharCount(&hcnt, &zcnt);
	charcnt = hcnt/2 + zcnt;
	GetFontSize(&dW, &dH);
	if(m_dStrlen != 0){
		if((hcnt == 1 && zcnt == 0)
		|| (hcnt == 0 && zcnt == 1)){
			space = (m_dStrlen - (hcnt*dW/2 + zcnt*dW)) / 2;
			sx = m_dSX; sy = m_dSY + space;
		}
		else{
			space = (m_dStrlen - (hcnt*dW/2 + zcnt*dW)) / (charcnt - 1);
			sx = m_dSX; sy = m_dSY;
		}
	}
	else{
		sx = m_dSX; sy = m_dSY;
		space = dW * m_dSpacePar / 100.0;
	}
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->SetFontSize(dW, dH);
		pcMoji->SetBaseFontSize(dH);
		pcMoji->SetStartXY(sx, sy);
		pcMoji->GetCode(&code);
		if(code < 0x0100){
			sx += (dW/2 + space/2);
		}
		else{
			sx += (dW + space);
		}
	}
}

void CStrgText::GetSpaceMojiCount(LPINT retzen, LPINT rethan)
{
	POSITION	pos;
	CMojiText*	pcMoji;
	int			zen, han;
	UINT		code;

	zen = 0;
	han = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		if(pos == NULL){
			// 最後の文字のスペースはカウントしない
			break;
		}
		pcMoji->GetCode(&code);
		if(code < 0x0100){
			han++;
		}
		else{
			zen++;
		}
	}
	*retzen = zen;
	*rethan = han;
}

void CStrgText::GetCharCount(int* rethan, int* retzen)
{
	POSITION	pos;
	CMojiText*	pcMoji;
	int			zen, han;
	UINT		code;

	zen = 0;
	han = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetCode(&code);
		if(code < 0x0100){
			han++;
		}
		else{
			zen++;
		}
	}
	*retzen = zen;
	*rethan = han;
}

void CStrgText::SetUseClrPenno(int notbl[], LPINT cnt)
{
	CMojiText*	pcMoji;

	POSITION pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->SetUseClrPenno(notbl, cnt);
	}
}

void CStrgText::ChangeFont(LPSTR sFname)
{
	CMojiText *pcMoji;

	SetFontName(sFname);
	POSITION pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->SetFont(sFname);
	}
}

void CStrgText::ChangeFontIdx(LPSTR sFname, int setidx)
{
	CMojiText *pcMoji;
	int		idx;

	SetFontName(sFname);
	idx = 0;
	POSITION pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		if(idx == setidx){
			pcMoji->SetFont(sFname);
			break;
		}
		idx++;
	}
}
