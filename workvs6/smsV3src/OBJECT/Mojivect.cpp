
//
// Easy Scanner System
//
// object mojivect.cpp  文字列フォント構造関係関数
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include <math.h>
#include <mbstring.h>

#pragma optimize( "", off )

#include "mojivect.h"

extern "C" {
	void Dispatch();
	int basesjtoj(int far sjis);
	int basejtosj(int far jis);
	void basesjtokuten(int far sjis, int far *ku, int far *ten);
	int basekutentojis(int kuten);
	void  pdrwmove(HDC PaintDC, DBL x1, DBL y1);
	void  pdrwline(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2);
	void strctboxsbasepointset(DBL x[], DBL y[]);
	void strctboxstrnspointset(DBL x[], DBL y[]);
	void plotdxymmtostep(LPDBL x, LPDBL y);
	int	  poutpaintmodechk();
	int  poutareachk(DBL sx, DBL sy, DBL ex, DBL ey);
	void poutpenchng(int nComID, int nPenno);
	void  poutblockend(int nComID);
	void  poutblockstart(int nComID);
	int	senvchckstopflag();
	int senvchckflag(int flag);
	int	senvcheckmojiwaku();
	void senvgetpenrgb(int pno, LPINT rval, LPINT gval, LPINT bval);
	int	senvcheckdrawmode();
	int senvgetclrmode();
	int senvchknoselectstrgvect();
	int	scrncheckswflag(int bit);
	void scrnsetclrbrushandpen(HDC PaintDC, COLORREF fillclr, COLORREF lineclr, int nthick);
	int senvgetplgnlimit();
	int senvgetcrtfillpenno();
	int senvgetcrtlinepenno();
	DBL senvgetcrtlinethick();
	void senvsetcrtfillpenno(int penno);
	void senvsetcrtlinepenno(int penno);
	void senvsetcrtlinethick(DBL thick);
	void scrnsetpreviewpen(HDC PaintDC, int r, int g, int b, int thick);
	void scrnsetfigupen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	void scrndxymmtorltv(LPDBL x, LPDBL y);
	void scrndlenmmtorltv(LPDBL thick);
}

IMPLEMENT_SERIAL(CMojiVect, CObject, 3)

CMojiVect::CMojiVect()
{
}

CRect* CMojiVect::GetBoundingRect()
{
	DBL		minx, miny, maxx, maxy;
	int		sx, sy, ex, ey;

	GetWakuDMiniMax(&minx, &miny, &maxx, &maxy);
	scrndxymmtorltv(&minx, &miny);
	scrndxymmtorltv(&maxx, &maxy);
	sx = (int)minx;
	sy = (int)miny;
	ex = (int)maxx;
	ey = (int)maxy;
	m_rectMoji.SetRect(sx, sy, ex, ey);
	return(&m_rectMoji);
}

CMojiVect::CMojiVect(int nDummy)
	: CVect(nDummy)
{
	m_pcDataList = NULL;
	m_pcMojiNext = NULL;
	m_fAtrFlag = 0;
	m_hFno = 0;
	//m_rectMoji;
	m_nZenKaku = 0;
	m_nCode = 0;
	m_nBaseCode = 0;
	SetFontSize((DBL)FSF1MESH, (DBL)FSF1MESH);
	SetStartXY((DBL)0, (DBL)0);
	//SetPointCount(4);
	SetPointCount(6);
	SetMoveXY(0.0, 0.0);
	SetTimeXY(1.0, 1.0);
	// 0 sx, sy    1 ex, sy    2 ex, ey    3 sx, ey
	// 6 movex, movey    6 timex, timey
}

CMojiVect::~CMojiVect()
{
	m_pcFSF1Font->Free(m_hFno);
	if(m_pcDataList != NULL){
		delete(m_pcDataList);
	}
}

int CMojiVect::CheckID()
{
	return(MOJIVECTID);
}

void CMojiVect::Serialize(CArchive& ar)
{
	BYTE	*spFBuf;
	int		dlen;
	int		idx;
	int		nID;

	if (ar.IsStoring())
	{
		ar << m_fAtrFlag;
		ar << m_rectMoji;
		ar << m_nZenKaku;
		ar << m_nCode;
		ar << m_nBaseCode;
		m_pcFSF1Font->DataLoad(m_hFno);
		dlen = m_pcFSF1Font->m_nDataLen;
		spFBuf = m_pcFSF1Font->m_bFBuf;
		ar << dlen;
		for(idx = 0; idx < dlen; idx++){
			ar << spFBuf[idx];
		}
		if(m_pcDataList == NULL){
			nID = NOOPDATAID;
			ar << nID;
		}
		else{
			nID = m_pcDataList->CheckID();
			ar << nID;
			m_pcDataList->Serialize(ar);
		}
		// ar << 
	}
	else
	{
		ar >> m_fAtrFlag;
		ar >> m_rectMoji;
		ar >> m_nZenKaku;
		ar >> m_nCode;
		ar >> m_nBaseCode;
		m_hFno = m_pcFSF1Font->Create();
		ar >> dlen;
		m_pcFSF1Font->m_nDataLen = dlen;
		spFBuf = m_pcFSF1Font->m_bFBuf;
		for(idx = 0; idx < dlen; idx++){
			ar >> spFBuf[idx];
		}
		m_pcFSF1Font->DataSave(m_hFno);
		ar >> nID;
		if(nID != NOOPDATAID){
			m_pcDataList = (CDataList *)NewDataCreate(nID);
			m_pcDataList->Serialize(ar);
		}
		// ar >>
	}
	CVect::Serialize(ar);
	if (!(ar.IsStoring()))
	{
		RenewMiniMax();
	}
}

//p 内部データーが空の状態であれば削除しすべてが空のときTRUEを返す
int CMojiVect::CheckNullData()
{
	if(m_hFno == 0){
		if(m_pcDataList != NULL){
			delete(m_pcDataList);
		}
		return(TRUE);
	}
	if(m_pcDataList != NULL){
		if(m_pcDataList->CheckNullData() == TRUE){
			delete(m_pcDataList);
			m_pcDataList = NULL;
			return(TRUE);
		}
	}
	return(FALSE);
}

void CMojiVect::GetFontBoxs(DBL x[], DBL y[])
{
	GetDXY(0, &(x[0]), &(y[0]));
	GetDXY(1, &(x[1]), &(y[1]));
	GetDXY(2, &(x[2]), &(y[2]));
	GetDXY(3, &(x[3]), &(y[3]));
}

void CMojiVect::SetFontSize(DBL dFWsize, DBL dFHsize)
{
	DBL		sx, sy;
	DBL		ex, ey;

	GetDXY(0, &sx, &sy);
	ex = sx + dFWsize;
	ey = sy + dFHsize;
	SetDXY(1, ex, sy);
	SetDXY(2, ex, ey);
	SetDXY(3, sx, ey);
}

void CMojiVect::GetFontSize(DBL *dFWsize, DBL *dFHsize)
{
	DBL		sx, sy;
	DBL		ex, ey;

	GetDXY(0, &sx, &sy);
	GetDXY(1, &ex, &ey);
	*dFWsize = hypot(ex-sx, ey-sy);
	if(ex < sx){
		(*dFWsize) *= -1;
	}
	GetDXY(3, &ex, &ey);
	*dFHsize = hypot(ex-sx, ey-sy); 
	if(ey < sy){
		(*dFHsize) *= -1;
	}
}

void CMojiVect::SetStartXY(DBL dSx, DBL dSy)
{
	DBL		sx, sy;
	DBL		ex, ey;
	DBL		xlen, ylen;

	sx = dSx;
	sy = dSy;
	GetFontSize(&xlen, &ylen);
	ex = sx + xlen;
	ey = sy + ylen;
	SetDXY(0, sx, sy);
	SetDXY(1, ex, sy);
	SetDXY(2, ex, ey);
	SetDXY(3, sx, ey);
}

void CMojiVect::GetStartXY(DBL *dSx, DBL *dSy)
{
	GetDXY(0, dSx, dSy);
}

int CMojiVect::CheckOneMojiTrns()
{
	DBL		mvx, mvy;
	DBL		tmx, tmy;

	GetMoveXY(&mvx, &mvy);
	GetTimeXY(&tmx, &tmy);
	if(mvx == 0.0 && mvy == 0.0
	&& tmx == 1.0 && tmy == 1.0){
		return(OFF);
	}
	return(ON);
}

void CMojiVect::SetMoveXY(DBL dmovex, DBL dmovey)
{
	SetDXY(4, dmovex, dmovey);
}

void CMojiVect::GetMoveXY(LPDBL dmovex, LPDBL dmovey)
{
	GetDXY(4, dmovex, dmovey);
}

void CMojiVect::SetTimeXY(DBL dtimex, DBL dtimey)
{
	SetDXY(5, dtimex, dtimey);
}

void CMojiVect::GetTimeXY(LPDBL dtimex, LPDBL dtimey)
{
	GetDXY(5, dtimex, dtimey);
}

void CMojiVect::SetNextMoji(CMojiVect *pMojiVect)
{
	m_pcMojiNext = pMojiVect;
}

CMojiVect* CMojiVect::GetNextMoji()
{
	return(m_pcMojiNext);
}

void CMojiVect::FontLoad(int nFontType, UINT sjis, UINT code)
{
	int		flag;

	if(code > 0x100){
		m_nZenKaku = ON;
		m_nCode = sjis;
		m_nBaseCode = code;
	}
	else{
		m_nZenKaku = OFF;
		m_nCode = sjis;
		m_nBaseCode = code;
	}
	m_hFno = m_pcFSF1Font->Create();
	flag = CheckTateYoko();
	DrawModeSet(OFF);
	switch(nFontType){
	case ES0FONT:
		m_pcESF0Font->SF1TrnsDataLoad(sjis);
		break;
	case ES1FONT:
		m_pcESF1Font->SF1TrnsDataLoad(sjis);
		break;
	case TT1FONT:
	case TT2FONT:
	case TT3FONT:
		if(m_pcESF1Font->SF1TrnsGaijiDataLoad(sjis) == FAIL){
			DrawModeSet(ON);
			if(m_nZenKaku == ON){
				m_pcWNTTFont->SF1TrnsDataLoad(sjis);
			}else{
				m_pcWNTTFont->SF1TrnsDataLoad(code);
			}
		}
		if(flag == TATEGAKI || flag == YOKOOKI){
			if(m_nBaseCode == 0x817c
			|| m_nBaseCode == 0x8181){
				m_pcFSF1Font->TrnsHanKaku();
			}
		}
		break;
	case ARCFONT:
	case MAXFONT:
		ChangeIBMCode(&sjis);
		if(m_nZenKaku == OFF && flag == TATEGAKI){
			m_pcESF1Font->SetTateYoko(YOKOGAKI);
			m_pcARCFont->SetTateYoko(YOKOGAKI);
		}else if(m_nZenKaku == OFF && flag == YOKOOKI){
			m_pcESF1Font->SetTateYoko(TATEOKI);
			m_pcARCFont->SetTateYoko(TATEOKI);
		}
		if(m_pcESF1Font->SF1TrnsGaijiDataLoad(sjis) == FAIL){
			m_pcARCFont->SF1TrnsDataLoad(sjis);
		}
		if(m_nZenKaku == OFF 
		&& (flag == TATEGAKI || flag == YOKOOKI)){
			m_pcFSF1Font->TrnsHanKaku();
		}
		if(flag == TATEGAKI || flag == YOKOOKI){
			if(m_nBaseCode == 0x817c
			|| m_nBaseCode == 0x8181){
				m_pcFSF1Font->TrnsHanKaku();
			}
		}
		break;
	case ARKFONT:
		if(m_nZenKaku == OFF && flag == TATEGAKI){
			m_pcESF1Font->SetTateYoko(YOKOGAKI);
			m_pcARKFont->SetTateYoko(YOKOGAKI);
		}else if(m_nZenKaku == OFF && flag == YOKOOKI){
			m_pcESF1Font->SetTateYoko(TATEOKI);
			m_pcARKFont->SetTateYoko(TATEOKI);
		}
		if(m_pcESF1Font->SF1TrnsGaijiDataLoad(sjis) == FAIL){
			m_pcARKFont->SF1TrnsDataLoad(sjis);
		}
		if(m_nZenKaku == OFF 
		&& (flag == TATEGAKI || flag == YOKOOKI)){
			m_pcFSF1Font->TrnsHanKaku();
		}
		if(flag == TATEGAKI || flag == YOKOOKI){
			if(m_nBaseCode == 0x817c
			|| m_nBaseCode == 0x8181){
				m_pcFSF1Font->TrnsHanKaku();
			}
		}
		break;
	case SCRFONT:
		m_pcSCRFont->SF1TrnsDataLoad(sjis);
		break;
	}
	m_pcFSF1Font->PointMiniMaxGet();
	m_pcFSF1Font->DataSave(m_hFno);
}

void CMojiVect::BoldTrns(DBL par)
{
	m_pcFSF1Font->BoldTrns(m_hFno, par);
}

void CMojiVect::FontReload(int nFontType, UINT sjis, UINT code)
{
	int		flag;
	DBL		dW, dH;
	DBL		sx, sy, ex, ey;
	DBL		trnsx[8], trnsy[8];
	DBL		basex[8], basey[8];

	flag = CheckTateYoko();
	if(code > 0x100){
		m_nZenKaku = ON;
		m_nCode = sjis;
		m_nBaseCode = code;
	}
	else{
		m_nZenKaku = OFF;
		m_nCode = sjis;
		m_nBaseCode = code;
	}
	DrawModeSet(OFF);
	switch(nFontType){
	case ES0FONT:
		m_pcESF0Font->SF1TrnsDataLoad(sjis);
		break;
	case ES1FONT:
		m_pcESF1Font->SF1TrnsDataLoad(sjis);
		break;
	case TT1FONT:
	case TT2FONT:
	case TT3FONT:
		if(m_pcESF1Font->SF1TrnsGaijiDataLoad(sjis) == FAIL){
			DrawModeSet(ON);
			if(m_nZenKaku == ON){
				m_pcWNTTFont->SF1TrnsDataLoad(sjis);
			}
			else{
				m_pcWNTTFont->SF1TrnsDataLoad(code);
			}
		}
		if(flag == TATEGAKI || flag == YOKOOKI){
			if(m_nBaseCode == 0x817c
			|| m_nBaseCode == 0x8181){
				m_pcFSF1Font->TrnsHanKaku();
			}
		}
		break;
	case ARCFONT:
	case MAXFONT:
		if(m_nZenKaku == OFF && flag == TATEGAKI){
			m_pcESF1Font->SetTateYoko(YOKOGAKI);
			m_pcARCFont->SetTateYoko(YOKOGAKI);
		}else if(m_nZenKaku == OFF && flag == YOKOOKI){
			m_pcESF1Font->SetTateYoko(TATEOKI);
			m_pcARCFont->SetTateYoko(TATEOKI);
		}
		if(m_pcESF1Font->SF1TrnsGaijiDataLoad(sjis) == FAIL){
			m_pcARCFont->SF1TrnsDataLoad(sjis);
		}
		if(m_nZenKaku == OFF 
		&& (flag == TATEGAKI || flag == YOKOOKI)){
			m_pcFSF1Font->TrnsHanKaku();
		}
		if(flag == TATEGAKI || flag == YOKOOKI){
			if(m_nBaseCode == 0x817c
			|| m_nBaseCode == 0x8181){
				m_pcFSF1Font->TrnsHanKaku();
			}
		}
		break;
	case ARKFONT:
		if(m_nZenKaku == OFF && flag == TATEGAKI){
			m_pcESF1Font->SetTateYoko(YOKOGAKI);
			m_pcARKFont->SetTateYoko(YOKOGAKI);
		}else if(m_nZenKaku == OFF && flag == YOKOOKI){
			m_pcESF1Font->SetTateYoko(TATEOKI);
			m_pcARKFont->SetTateYoko(TATEOKI);
		}
		if(m_pcESF1Font->SF1TrnsGaijiDataLoad(sjis) == FAIL){
			m_pcARKFont->SF1TrnsDataLoad(sjis);
		}
		if(m_nZenKaku == OFF 
		&& (flag == TATEGAKI || flag == YOKOOKI)){
			m_pcFSF1Font->TrnsHanKaku();
		}
		if(flag == TATEGAKI || flag == YOKOOKI){
			if(m_nBaseCode == 0x817c
			|| m_nBaseCode == 0x8181){
				m_pcFSF1Font->TrnsHanKaku();
			}
		}
		break;
	case SCRFONT:
		m_pcSCRFont->SF1TrnsDataLoad(sjis);
		break;
	}
	m_pcFSF1Font->PointMiniMaxGet();
	m_pcFSF1Font->DataSave(m_hFno);
	if(m_pcDataList != NULL){
		delete(m_pcDataList);
		m_pcDataList = NULL;
		GetFontBoxs(trnsx, trnsy);
		DataChange();
		GetFontSize(&dW, &dH);
		GetStartXY(&sx, &sy);
		ex = sx + dW;
		ey = sy + dH;
		basex[0] = sx; basey[0] = sy;
		basex[1] = ex; basey[1] = sy;
		basex[2] = ex; basey[2] = ey;
		basex[3] = sx; basey[3] = ey;
		strctboxsbasepointset(basex, basey);
		strctboxstrnspointset(trnsx, trnsy);
		m_pcDataList->StrctBoxsTrns();
	}
}

void CMojiVect::PlotOut(int nComID)
{
	int		flag;
	DBL		dW, dH;
	DBL		dSx, dSy;
	DBL		sx, sy, ex, ey;
	int		stat;

	Dispatch();
	flag = CheckTateYoko();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}
	if(m_pcDataList != NULL){
		m_pcDataList->PlotOut(nComID);
		if(senvcheckmojiwaku() == ON){
			if(poutpaintmodechk() == ON){
				poutblockstart(nComID);
				SetPointCount(4);
				CVect::PlotOut(nComID);
				SetPointCount(6);
				poutblockend(nComID);
			}
		}
		return;
	}

	GetFontSize(&dW, &dH);
	GetStartXY(&dSx, &dSy);
	sx = dSx; sy = dSy;
	ex = sx+dW; ey = sy+dH;
	plotdxymmtostep(&sx, &sy);
	plotdxymmtostep(&ex, &ey);
	stat = poutareachk(sx, sy, ex, ey);
	if(stat != OFF){
		m_pcFSF1Font->SetTateYoko(flag);
		m_pcFSF1Font->TrnsPlotOut(nComID, dSx, dSy, dW, dH, m_hFno);
		if(senvcheckmojiwaku() == ON){
			if(poutpaintmodechk() == ON){
				poutblockstart(nComID);
				SetPointCount(4);
				CVect::PlotOut(nComID);
				SetPointCount(6);
				poutblockend(nComID);
			}
		}
	}
}

void CMojiVect::GetPrePostTable(DBL wd, DBL hi, int pre[], int post[])
{
	int		flag;
	DBL		dW, dH;
	DBL		dSx, dSy;
	DBL		sx, sy;
	DBL		timex, timey;
	int		x;

	flag = CheckTateYoko();
	GetFontSize(&dW, &dH);
	GetStartXY(&dSx, &dSy);
	timex = dW / wd;
	timey = dH / hi;
	sx = ((wd - dW) / 2.0) * FDIBCSIZE / wd;
	sy = ((hi - dH) / 2.0) * FDIBCSIZE / hi;
	m_pcFSF1Font->PlgnDrawDIBC(flag, m_hFno, sx, sy, timex, timey);
	m_pcFSF1Font->GetPrePostTable(pre, post);
	if(m_nCode == 0x88ea 
	&& (flag == TATEGAKI || flag == YOKOOKI)){
		for(x = 0; x < FDIBCSIZE; x++){
			pre[x] = pre[x] / 2;
			post[x] = post[x] / 2;
		}
	}

}

void CMojiVect::GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	GetWakuDMiniMax(minix, miniy, maxx, maxy);
}

void CMojiVect::GetWakuDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	SetPointCount(4);
	CVect::RenewMiniMax();
	CVect::GetDMiniMax(minix, miniy, maxx, maxy);
	SetPointCount(6);
}

void CMojiVect::GetDataDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	int		flag;
	DBL		dW, dH;
	DBL		dSx, dSy;
	DBL		time;

	flag = CheckTateYoko();
	(*minix) = INTMAX;
	(*miniy) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	GetFontSize(&dW, &dH);
	GetStartXY(&dSx, &dSy);
	if(CheckSpecialSpace(&time) == TRUE
	|| m_nBaseCode == 0x0020){
		*minix = dSx;
		*miniy = dSy;
		*maxx = dSx + dW;
		*maxy = dSy + dH;
		return;
	}
	if(m_pcDataList != NULL){
		m_pcDataList->GetDMiniMax(minix, miniy, maxx, maxy);
		return;
	}
	m_pcFSF1Font->SetTateYoko(flag);
	m_pcFSF1Font->DataLoad(m_hFno);
	m_pcFSF1Font->TrnsElementSet(dSx, dSy, dW, dH);
	m_pcFSF1Font->GetDataDMiniMax(minix, miniy, maxx, maxy);
}

void CMojiVect::GetDataSEP(LPDBL sp, LPDBL ep)
{
	int		flag;
	DBL		wminix, wminiy, wmaxx, wmaxy;
	DBL		dminix, dminiy, dmaxx, dmaxy;

	flag = CheckTateYoko();
	GetWakuDMiniMax(&wminix, &wminiy, &wmaxx, &wmaxy);
	GetDataDMiniMax(&dminix, &dminiy, &dmaxx, &dmaxy);
	if(flag == TATEGAKI || flag == YOKOOKI){
		*sp = wmaxy - dmaxy;
		*ep = wmaxy - dminiy;
	}
	else{
		*sp = dminix - wminix;
		*ep = dmaxx - wminix;
	}
}

CData* CMojiVect::SrchData(DBL x, DBL y, DBL *retlen)
{
	int		flag;
	DBL		dW, dH;
	DBL		dSx, dSy;
	DBL		sx, sy, ex, ey;
	CData	*retData;

	flag = CheckTateYoko();
	*retlen = INTMAX;
	if(m_pcDataList != NULL){
		// 文字を構成するベクトルを返す
		retData = m_pcDataList->SrchData(x, y, retlen);
		return(retData);
	}
	GetFontSize(&dW, &dH);
	GetStartXY(&dSx, &dSy);
	m_pcFSF1Font->SetTateYoko(flag);
	m_pcFSF1Font->DataLoad(m_hFno);
	m_pcFSF1Font->TrnsElementSet(dSx, dSy, dW, dH);
	m_pcFSF1Font->GetDataDMiniMax(&sx, &sy, &ex, &ey);
	*retlen = GetPreLengthBoxs(x, y, sx, sy, ex, ey);
	return(this);
}

CData* CMojiVect::SrchDataBoxs(DBL x, DBL y, DBL *retlen)
{
	int		flag;
	DBL		dW, dH;
	DBL		dSx, dSy;
	DBL		sx, sy, ex, ey;
	CData	*retData;

	flag = CheckTateYoko();
	*retlen = INTMAX;
	if(m_pcDataList != NULL){
		// 文字を構成するベクトルを返す
		retData = m_pcDataList->SrchDataBoxs(x, y, retlen);
		return(retData);
	}

	GetFontSize(&dW, &dH);
	GetStartXY(&dSx, &dSy);
	m_pcFSF1Font->SetTateYoko(flag);
	m_pcFSF1Font->DataLoad(m_hFno);
	m_pcFSF1Font->TrnsElementSet(dSx, dSy, dW, dH);
	m_pcFSF1Font->GetDataDMiniMax(&sx, &sy, &ex, &ey);
	*retlen = GetPreLengthBoxs(x, y, sx, sy, ex, ey);
	return(this);
}

CData* CMojiVect::SrchNearOnLinePoint(DBL xx, DBL yy, DBL *retlen, DBL *retx, DBL *rety, int *retpp)
{
	DBL		dSx, dSy;
	DBL		dW, dH;
	DBL		sx, sy, ex, ey;
	DBL		minx, miny, maxx, maxy;
	DBL		len;
	DBL		x, y;
	int		pp;
	CData	*pcData;
	CData	*retData;
	
	*retlen = INTMAX;
	*retpp = 0;
	retData = NULL;
	if(senvchknoselectstrgvect() == ON){
		return(retData);
	}
	if(m_pcDataList == NULL){
		GetFontSize(&dW, &dH);
		GetStartXY(&dSx, &dSy);
		sx = dSx; sy = dSy;
		ex = sx + dW; ey = sy + dH;
		minx = min(sx, ex);
		miny = min(sy, ey);
		maxx = max(sx, ex);
		maxy = max(sy, ey);
		if(xx < minx || yy < miny || xx > maxx || yy > maxy){
			return(NULL);
		}
		GetDataDMiniMax(&minx, &miny, &maxx, &maxy);
		if(xx < minx || yy < miny || xx > maxx || yy > maxy){
			return(NULL);
		}
		DataChange();
	}

	if(m_pcDataList != NULL){
		// 文字を構成するベクトルを返す
		len = INTMAX;
		pcData = m_pcDataList->SrchNearOnLinePoint(xx, yy, &len, &x, &y, &pp);
		if(*retlen > len){
			*retlen = len;
			*retx = x;
			*rety = y;
			*retpp = pp;
			retData = pcData;
		}
	}
	return(retData);
}

CData* CMojiVect::SrchNearPoint(DBL xx, DBL yy, DBL *retlen, int *retpp)
{
	DBL		dSx, dSy;
	DBL		dW, dH;
	DBL		sx, sy, ex, ey;
	DBL		minx, miny, maxx, maxy;
	DBL		len;
	int		pp;
	CData	*pcData;
	CData	*retpcData;
	
	*retlen = INTMAX;
	*retpp = 0;
	retpcData = NULL;
	if(m_pcDataList == NULL){
		GetFontSize(&dW, &dH);
		GetStartXY(&dSx, &dSy);
		sx = dSx; sy = dSy;
		ex = sx + dW; ey = sy + dH;
		minx = min(sx, ex);
		miny = min(sy, ey);
		maxx = max(sx, ex);
		maxy = max(sy, ey);
		if(xx < minx || yy < miny || xx > maxx || yy > maxy){
			return(NULL);
		}
		GetDataDMiniMax(&minx, &miny, &maxx, &maxy);
		if(xx < minx || yy < miny || xx > maxx || yy > maxy){
			return(NULL);
		}
		DataChange();
	}

	if(m_pcDataList != NULL){
		// 文字を構成するベクトルを返す
		len = INTMAX;
		pcData = m_pcDataList->SrchNearPoint(xx, yy, &len, &pp);
		if(*retlen > len){
			*retlen = len;
			*retpp = pp;
			retpcData = pcData;
		}
	}
	return(retpcData);
}

void CMojiVect::RenewMiniMax()
{
	if(m_pcDataList != NULL){
		m_pcDataList->RenewMiniMax();
	}
}

CData * CMojiVect::CopyData(DBL movex, DBL movey)
{
	CMojiVect	*pcnewData;
	int		pp;
	DBL		x, y;

	pcnewData = (CMojiVect*)NewDataCreate(MOJIVECTID);

	//GetFontSize(&dFWsize, &dFHsize);
	//GetStartXY(&dSx, &dSy);
	//pcnewData->SetFontSize(dFWsize, dFHsize);
	//pcnewData->SetStartXY(dSx+movex, dSy+movey);
	for(pp = 0; pp < 4; pp++){
		GetDXY(pp, &x, &y);
		pcnewData->SetDXY(pp, x+movex, y+movey);
	}
	for(;pp < 6; pp++){
		GetDXY(pp, &x, &y);
		pcnewData->SetDXY(pp, x, y);
	}
	pcnewData->m_hFno = m_pcFSF1Font->Create();
	m_pcFSF1Font->DataLoad(m_hFno);
	m_pcFSF1Font->DataSave(pcnewData->m_hFno);
	pcnewData->SetLenFromStrgTop(m_dLenFromStrgTop);
	pcnewData->SetNextMoji(NULL);
	pcnewData->SetFillPen(m_nFillPen);
	pcnewData->SetOutLinePen(m_nOutLinePen);
	pcnewData->SetOutLineWidth(m_dOutLineWidth);
	//2001.04.10 立身　コピーで小さくなるバグ対応のため追加
	pcnewData->m_fAtrFlag = m_fAtrFlag;
	pcnewData->m_nZenKaku = m_nZenKaku;
	pcnewData->m_nCode = m_nCode; 
	pcnewData->m_nBaseCode = m_nBaseCode;
	if(m_pcDataList != NULL){
		pcnewData->m_pcDataList = (CDataList *)(m_pcDataList->CopyData(movex, movey));
	}
	return((CData*)pcnewData);
}

void CMojiVect::MoveTrns(DBL movex, DBL movey)
{
	DBL		bmvx, bmvy;

	SetPointCount(4);
	CVect::MoveTrns(movex, movey);
	SetPointCount(6);
	if(m_pcDataList != NULL){
		m_pcDataList->MoveTrns(movex, movey);
	}
	if(scrncheckswflag(MJONEBIT) == ON){
		GetMoveXY(&bmvx, &bmvy);
		bmvx += movex;
		bmvy += movey;
		SetMoveXY(movex, movey);
	}
}

void CMojiVect::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	DBL		mvx, mvy;
	DBL		btmx, btmy;

	SetPointCount(4);
	CVect::SizeTrns(cntrx, cntry, timex, timey);
	SetPointCount(6);
	if(m_pcDataList != NULL){
		m_pcDataList->SizeTrns(cntrx, cntry, timex, timey);
	}
	if(scrncheckswflag(MJONEBIT) == ON){
		GetMoveXY(&mvx, &mvy);
		mvx *= timex;
		mvy *= timey;
		SetMoveXY(mvx, mvy);
		GetTimeXY(&btmx, &btmy);
		btmx *= timex;
		btmy *= timey;
		SetTimeXY(btmx, btmy);
	}
}

void CMojiVect::SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
{
	if(m_pcDataList == NULL){
		DataChange();
	}
	SetPointCount(4);
	CVect::SlopTrns(cntrx, cntry, ddx, ddy);
	SetPointCount(6);
	m_pcDataList->SlopTrns(cntrx, cntry, ddx, ddy); 
}

void CMojiVect::MirrorTrns(DBL a, DBL b, DBL t1, DBL t2)
{
	if(m_pcDataList == NULL){
		DataChange();
	}
	SetPointCount(4);
	CVect::MirrorTrns(a, b, t1, t2);
	SetPointCount(6);
	m_pcDataList->MirrorTrns(a, b, t1, t2);
}

void CMojiVect::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	if(m_pcDataList == NULL){
		DataChange();
	}
	SetPointCount(4);
	CVect::RoundTrns(cntrx, cntry, tsin, tcos);
	SetPointCount(6);
	m_pcDataList->RoundTrns(cntrx, cntry, tsin, tcos);
}

void CMojiVect::StrctBoxsTrns()
{
	if(m_pcDataList == NULL){
		DataChange();
	}
	SetPointCount(4);
	CVect::StrctBoxsTrns();
	SetPointCount(6);
	m_pcDataList->StrctBoxsTrns();
}

void CMojiVect::DataChange()
{
	int		flag;
	DBL		dW, dH;
	DBL		dSx, dSy;
	int		pcntmax;
	int		spp, epp, pcnt;
	BYTE	*bufsp;
	CVect	*pcVect;
	int		penno;
	DBL		thick;

	flag = CheckTateYoko();
	if(m_pcDataList != NULL){
		return;
	}

	m_pcDataList = (CDataList*)NewDataCreate(DATALISTID);
	if(m_pcDataList == NULL){
		return;
	}
	GetFontSize(&dW, &dH);
	GetStartXY(&dSx, &dSy);
	m_pcFSF1Font->SetTateYoko(flag);
	m_pcFSF1Font->DataLoad(m_hFno);
	m_pcFSF1Font->TrnsElementSet(dSx, dSy, dW, dH);
	pcntmax = m_pcFSF1Font->PointCountGet();
	spp = 0;
	while(1){
		if(pcntmax <= spp){
			break;
		}
		epp = m_pcFSF1Font->RejionEndGet(pcntmax, spp);
		pcnt = epp - spp;
		if(pcnt <= 0){
			break;
		}
		pcVect = (CVect*)NewDataCreate(BASEVECTID);
		if(pcVect == NULL){
			break;
		}
		if(m_pcFSF1Font->RejionChk(epp - 1)){
			bufsp = &(m_pcFSF1Font->m_bFBuf[spp*FSF1SIZADD]);		
			SetVect(bufsp, pcnt, pcVect);
		}
		else{
			bufsp = &(m_pcFSF1Font->m_bFBuf[spp*FSF1SIZADD]);
			SetVect(bufsp, pcnt, pcVect);
			pcVect->OffFlag(REJIONFLAG);
		}
		pcVect->SetPointCount(pcnt);
		pcVect->RenewMiniMax();
		m_pcDataList->SetData((CData*)pcVect);
		spp = epp;
	}
	penno = GetFillPen();
	m_pcDataList->SetFillPen(penno);
	penno = GetOutLinePen();
	m_pcDataList->SetOutLinePen(penno);
	thick = GetOutLineWidth();
	m_pcDataList->SetOutLineWidth(thick);
}

void CMojiVect::SetVect(LPBYTE fbuf, int pcnt, CVect *pcVect)
{
	int		pp;
	int		atr;
	DBL		dx, dy;

	for(pp = 0; pp < pcnt; pp++){
		m_pcFSF1Font->AtrDXYGet(fbuf, pp, &atr, &dx, &dy);
		m_pcFSF1Font->FontScaleTrns(dx, dy, &dx, &dy);
		pcVect->SetAtrDXY(pp, atr, dx, dy);
	}
}

BOOL CMojiVect::IsZenKaku()
{
	if(m_nZenKaku == ON){
		return(TRUE);
	}
	else{
		return(FALSE);
	}
}

void CMojiVect::StockClrDraw(CDC *pcDC)
{
	int		flag;
	DBL		dW, dH;
	DBL		dSx, dSy;

	flag = CheckTateYoko();
	if(m_pcDataList != NULL){
		m_pcDataList->StockClrDraw(pcDC);
		DrawMojiWaku(pcDC);
		return;
	}

	GetFontSize(&dW, &dH);
	GetStartXY(&dSx, &dSy);
	m_pcFSF1Font->SetTateYoko(flag);
	m_pcFSF1Font->TrnsPlgnDraw(pcDC, dSx, dSy, dW, dH, m_hFno);
	DrawMojiWaku(pcDC);
}

void CMojiVect::CheckFlagDraw(int flag, CDC *pcDC)
{
	int		tateyoko;
	DBL		dW, dH;
	DBL		dSx, dSy;

	tateyoko = CheckTateYoko();
	if(m_pcDataList != NULL){
		m_pcDataList->CheckFlagDraw(flag, pcDC);
		DrawMojiWaku(pcDC);
		return;
	}
	GetFontSize(&dW, &dH);
	GetStartXY(&dSx, &dSy);
	m_pcFSF1Font->SetTateYoko(tateyoko);
	if(CheckDrawMode() == ON){
		SetPolyFillMode(pcDC->m_hDC, WINDING);
	}
	m_pcFSF1Font->TrnsFillDraw(pcDC, dSx, dSy, dW, dH, m_hFno);
	if(CheckDrawMode() == ON){
		SetPolyFillMode(pcDC->m_hDC, ALTERNATE);
	}
	m_pcFSF1Font->TrnsPlgnDraw(pcDC, dSx, dSy, dW, dH, m_hFno);
	DrawMojiWaku(pcDC);
}

void CMojiVect::CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick)
{
	int		tateyoko;
	DBL		dW, dH;
	DBL		dSx, dSy;
	DBL		dthick;
	int		penno;
	int		nthick;

	tateyoko = CheckTateYoko();
	penno = GetFillPen();
	if(m_pcDataList != NULL){
		m_pcDataList->CheckFlagDrawSetClr(flag, pcDC, clr1, clr2, thick);
		scrnsetclrbrushandpen(pcDC->m_hDC, clr1, clr1, 1);
		DrawMojiWaku(pcDC);
		scrnpenend(pcDC->m_hDC);
		return;
	}
	dthick = GetOutLineWidth();
	if(clr1 != clr2){
		dthick = dthick - thick;
	}
	else{
		dthick = dthick + thick;
	}
	if(dthick > 0){
		scrndlenmmtorltv(&dthick);
		nthick = (int)dthick;
		scrnsetclrbrushandpen(pcDC->m_hDC, clr1, clr1, nthick);
	}
	else{
		dthick = fabs(dthick);
		scrndlenmmtorltv(&dthick);
		nthick = (int)dthick;
		scrnsetclrbrushandpen(pcDC->m_hDC, clr1, clr2, nthick);
	}

	GetFontSize(&dW, &dH);
	GetStartXY(&dSx, &dSy);
	tateyoko = CheckTateYoko();
	if(flag == TATEGAKI || flag == YOKOOKI){
		dSx = dSx - dW / 2;
		dSy = dSy - dH;
	}
	else{
		dSy = dSy - dH / 2;
	}
	m_pcFSF1Font->SetTateYoko(tateyoko);
	if(penno != -1){
		if(CheckDrawMode() == ON){
			SetPolyFillMode(pcDC->m_hDC, WINDING);
		}
		m_pcFSF1Font->TrnsFillDraw(pcDC, dSx, dSy, dW, dH, m_hFno);
		if(CheckDrawMode() == ON){
			SetPolyFillMode(pcDC->m_hDC, ALTERNATE);
		}
	}
	m_pcFSF1Font->TrnsPlgnDraw(pcDC, dSx, dSy, dW, dH, m_hFno);
	DrawMojiWaku(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CMojiVect::DrawMojiWaku(CDC *pcDC)
{
	int		mode;

	if(senvcheckmojiwaku() == OFF){
		return;
	}
	mode = senvcheckdrawmode(); 
	if(mode == BLACKVIEW
	|| mode == PRINTVIEW
	|| mode == DIRCTVIEW
	|| mode == METAFVIEW){
		return;
	}
	SetPointCount(4);
	CVect::PlgnDraw(pcDC);
	SetPointCount(6);
}

void CMojiVect::OnDraw(CDC *pcDC)
{
	int		flag;
	int		penno;
	int		rval, gval, bval;
	DBL		thick;
	int		nthick;
	int		nlimit;
	DBL		dW, dH;
	DBL		dSx, dSy;
	int		mode;

	flag = CheckTateYoko();
	nlimit = senvgetplgnlimit();
	if(senvchckstopflag() == ON){
		return;
	}
	if(m_pcDataList != NULL){
		if(CheckDrawMode() == ON){
			//SetPolyFillMode(pcDC->m_hDC, WINDING);
		}
		m_pcDataList->OnDraw(pcDC);
		if(CheckDrawMode() == ON){
			//SetPolyFillMode(pcDC->m_hDC, ALTERNATE);
		}
		scrnsetfigupen(pcDC->m_hDC);
		DrawMojiWaku(pcDC);
		scrnpenend(pcDC->m_hDC);
		return;
	}

	mode = senvcheckdrawmode(); 
	if(mode == PRINTVIEW
	|| mode == PREVIEW){
		OnDrawOutPrint(pcDC);
		scrnsetfigupen(pcDC->m_hDC);
		DrawMojiWaku(pcDC);
		scrnpenend(pcDC->m_hDC);
		return;
	}

	penno = GetFillPen();
	if(penno != -1){
		if(mode == BLACKVIEW
		|| mode == DIRCTVIEW){
			if(penno == 0){
				rval = 0; gval = 0; bval = 0;
			}
			else{
				senvgetpenrgb(penno, &rval, &gval, &bval);
			}
			scrnsetpreviewpen(pcDC->m_hDC, rval, gval, bval, nlimit);

			GetFontSize(&dW, &dH);
			GetStartXY(&dSx, &dSy);
			m_pcFSF1Font->SetTateYoko(flag);
			if(CheckDrawMode() == ON){
				SetPolyFillMode(pcDC->m_hDC, WINDING);
			}
			m_pcFSF1Font->TrnsFillDraw(pcDC, dSx, dSy, dW, dH, m_hFno);
			if(CheckDrawMode() == ON){
				SetPolyFillMode(pcDC->m_hDC, ALTERNATE);
			}
			scrnpenend(pcDC->m_hDC);
		}
	}

	penno = GetOutLinePen();
	if(penno != -1){
		mode = senvcheckdrawmode();
		if(mode == PRINTVIEW
		|| mode == PREVIEW
		|| mode == DIRCTVIEW
		|| mode == BLACKVIEW){
			thick = GetOutLineWidth();
			scrndlenmmtorltv(&thick);
			nthick = (int)thick;
			if(nthick < nlimit){
				nthick = nlimit;
			}
			if(penno == 0){
				rval = 0; gval = 0; bval = 0;
			}
			else{
				senvgetpenrgb(penno, &rval, &gval, &bval);
			}
			scrnsetpreviewpen(pcDC->m_hDC, rval, gval, bval, nthick);
		}
		else{ 
			scrnsetfigupen(pcDC->m_hDC);
		}
		GetFontSize(&dW, &dH);
		GetStartXY(&dSx, &dSy);
		m_pcFSF1Font->SetTateYoko(flag);
		m_pcFSF1Font->TrnsPlgnDraw(pcDC, dSx, dSy, dW, dH, m_hFno);
		DrawMojiWaku(pcDC);
		scrnpenend(pcDC->m_hDC);
	}
}

void CMojiVect::OnDrawOutPrint(CDC *pcDC)
{
	int		flag;
	int		penno;
	int		brval, bgval, bbval;
	int		prval, pgval, pbval;
	COLORREF	fillclr, lineclr;
	DBL		thick;
	int		nthick;
	int		nlimit;
	DBL		dW, dH;
	DBL		dSx, dSy;

	flag = CheckTateYoko();
	nlimit = senvgetplgnlimit();
	penno = GetFillPen();
	if(penno != -1){
		if(penno == 0){
			brval = 0; bgval = 0; bbval = 0;
		}
		else{
			senvgetpenrgb(penno, &brval, &bgval, &bbval);
		}
		if(senvgetclrmode() == BLACK){
			brval = 0;
			bgval = 0;
			bbval = 0;
		}
		penno = GetOutLinePen();
		if(penno == -1){
			prval = brval; pgval = bgval; pbval = bbval;
			nthick = 1;
		}
		else{
			if(penno == 0){
				prval = 0; pgval = 0; pbval = 0;
			}
			else{
				senvgetpenrgb(penno, &prval, &pgval, &pbval);
			}
			if(senvgetclrmode() == BLACK){
				prval = 0;
				pgval = 0;
				pbval = 0;
			}
			thick = GetOutLineWidth();
			scrndlenmmtorltv(&thick);
			nthick = (int)thick;
		}
		if(nthick < nlimit){
			nthick = nlimit;
		}
		fillclr = RGB(brval, bgval, bbval);
		lineclr = RGB(prval, pgval, pbval);
		//scrnsetclrbrushandpen(pcDC->m_hDC, fillclr, lineclr, nthick);
		scrnsetclrbrushandpen(pcDC->m_hDC, fillclr, -1, 0);
		GetFontSize(&dW, &dH);
		GetStartXY(&dSx, &dSy);
		m_pcFSF1Font->SetTateYoko(flag);
		if(CheckDrawMode() == ON){
			SetPolyFillMode(pcDC->m_hDC, WINDING);
		}
		m_pcFSF1Font->TrnsFillDraw(pcDC, dSx, dSy, dW, dH, m_hFno);
		if(CheckDrawMode() == ON){
			SetPolyFillMode(pcDC->m_hDC, ALTERNATE);
		}
		scrnpenend(pcDC->m_hDC);
		//return;
	}

		penno = GetOutLinePen();
		if(penno == -1){
			return;
		}

		thick = GetOutLineWidth();
		scrndlenmmtorltv(&thick);
		nthick = (int)thick;
		if(nthick < nlimit){
			nthick = nlimit;
		}
		if(penno == 0){
			prval = 0; pgval = 0; pbval = 0;
		}
		else{
			senvgetpenrgb(penno, &prval, &pgval, &pbval);
		}
		if(senvgetclrmode() == BLACK){
			prval = 0;
			pgval = 0;
			pbval = 0;
		}
		scrnsetpreviewpen(pcDC->m_hDC, prval, pgval, pbval, nthick);
		GetFontSize(&dW, &dH);
		GetStartXY(&dSx, &dSy);
		m_pcFSF1Font->SetTateYoko(flag);
		m_pcFSF1Font->TrnsPlgnDraw(pcDC, dSx, dSy, dW, dH, m_hFno);
		scrnpenend(pcDC->m_hDC);
}

void CMojiVect::OutlineDraw(CDC *pcDC)
{
	int		flag;
	int		penno;
	int		rval, gval, bval;
	DBL		thick;
	int		nthick;
	int		nlimit;
	DBL		dW, dH;
	DBL		dSx, dSy;

	flag = CheckTateYoko();
	nlimit = senvgetplgnlimit();
	if(m_pcDataList != NULL){
		m_pcDataList->OutlineDraw(pcDC);
		DrawMojiWaku(pcDC);
		return;
	}

	penno = GetOutLinePen();
	if(penno == -1){
		return;
	}
	thick = GetOutLineWidth();
	scrndlenmmtorltv(&thick);
	nthick = (int)thick;
	if(nthick < nlimit){
		nthick = nlimit;
	}
	if(penno == 0){
		rval = 0; gval = 0; bval = 0;
	}
	else{
		senvgetpenrgb(penno, &rval, &gval, &bval);
	}
	scrnsetpreviewpen(pcDC->m_hDC, rval, gval, bval, nthick);

	GetFontSize(&dW, &dH);
	GetStartXY(&dSx, &dSy);
	m_pcFSF1Font->SetTateYoko(flag);
	m_pcFSF1Font->TrnsPlgnDraw(pcDC, dSx, dSy, dW, dH, m_hFno);
	DrawMojiWaku(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CMojiVect::SetOutLineWidth(DBL dThick)
{
	CVect::SetOutLineWidth(dThick);
	if(m_pcDataList != NULL){
		m_pcDataList->SetOutLineWidth(dThick);
	}
}

void CMojiVect::SetOutLinePen(int nPen)
{
	CVect::SetOutLinePen(nPen);
	if(m_pcDataList != NULL){
		m_pcDataList->SetOutLinePen(nPen);
	}
}

void CMojiVect::SetFillPen(int nPen)
{
	CVect::SetFillPen(nPen);
	if(m_pcDataList != NULL){
		m_pcDataList->SetFillPen(nPen);
	}
}

void CMojiVect::SetDefOutLineClr()
{
	CVect::SetDefOutLineClr();
	if(m_pcDataList != NULL){
		m_pcDataList->SetDefOutLineClr();
	}
}

void CMojiVect::SetDefFillClr()
{
	CVect::SetDefFillClr();
	if(m_pcDataList != NULL){
		m_pcDataList->SetDefFillClr();
	}
}

void CMojiVect::SetLenFromStrgTop(DBL setlen)
{
	m_dLenFromStrgTop = setlen;
}

DBL CMojiVect::GetLenFromStrgTop()
{
	return(m_dLenFromStrgTop);
}

void CMojiVect::SetTateYoko(int flag)
{
	m_fAtrFlag = (m_fAtrFlag & 0x0fffc) | flag;
}

int CMojiVect::CheckTateYoko()
{
	int		flag;

	flag = m_fAtrFlag & 0x0003;
	return(flag);
}

void CMojiVect::DrawModeSet(int flag)
{
	if(flag == ON){
		m_fAtrFlag |= MOJIDRAWWINDING;
	}else{
		m_fAtrFlag &= ~MOJIDRAWWINDING;
	}
}

int CMojiVect::CheckDrawMode()
{
	if(m_fAtrFlag & MOJIDRAWWINDING){
		return(ON);
	}else{
		return(ON);
	}
}

void CMojiVect::DeleteData(CData *delData)
{
	if(m_pcDataList != NULL){
		m_pcDataList->DeleteData(delData);
	}
}

void CMojiVect::ReverceRejion()
{
	if(m_pcDataList != NULL){
		m_pcDataList->ReverceRejion();
	}
}

void CMojiVect::OffFlag(int bitflag)
{
	CVect::OffFlag(bitflag);
	if(m_pcDataList != NULL){
		m_pcDataList->OffFlag(bitflag);
	}
}

void CMojiVect::OnFlag(int bitflag)
{
	CVect::OnFlag(bitflag);
	if(m_pcDataList != NULL){
		m_pcDataList->OnFlag(bitflag);
	}
}

void CMojiVect::SaveEpsFile()
{
	if(m_pcDataList != NULL){
		m_pcDataList->SaveEpsFile();	
	}
}

void CMojiVect::SetUseClrPenno(int notbl[], LPINT cnt)
{
	int		penno, i;

	penno = GetFillPen();
	if(penno == 0){
		SetOutLinePen(1);
		penno = 1;
	}
	for(i = 0; i < *cnt; i++){
		if(notbl[i] == penno){
			return;
		}
	}
	notbl[i] = penno;
	i++;
	*cnt = i;
}

void CMojiVect::PointPicup()
{
	if(m_pcDataList != NULL){
		m_pcDataList->PointPicup();
	}
}

void CMojiVect::AllOffPointFlag(int bitflag)
{
	if(m_pcDataList != NULL){
		m_pcDataList->AllOffPointFlag(bitflag);
	}
}

void CMojiVect::AllOnPointFlag(int bitflag)
{
	if(m_pcDataList != NULL){
		m_pcDataList->AllOnPointFlag(bitflag);
	}
}

void CMojiVect::PointXLegal(DBL xx)
{
	if(m_pcDataList != NULL){
		m_pcDataList->PointXLegal(xx);
	}
}

void CMojiVect::PointYLegal(DBL yy)
{
	if(m_pcDataList != NULL){
		m_pcDataList->PointYLegal(yy);
	}
}

void CMojiVect::ChangeIBMCode(UINT *sjis)
{
	int		ku, ten, sumten;
	int		jis;

	basesjtokuten(*sjis, &ku, &ten); 
	if(0xfa5c <= (*sjis) && (*sjis) <= 0xfc4b){
		sumten = ku * 94 + ten - 1;
		sumten -= (26*94+28);
		ku = sumten / 94;
		ten = sumten % 94 + 1;
		jis = basekutentojis(ku*100+ten);
		*sjis = basejtosj(jis);
	}
	else if(0xfa40 <= (*sjis) && (*sjis) <= 0xfa49){
		(*sjis) = (*sjis) - 0x0b51;
	}
	else if(0xfa4a <= (*sjis) && (*sjis) <= 0xfa53){
		(*sjis) = (*sjis) - 0x72f6;
	}
	else if(0xfa54 <= (*sjis) && (*sjis) <= 0xfa57){
		(*sjis) = (*sjis) - 0x0b5B;
	}
	else if(0xfa58 == (*sjis)){
		(*sjis) = 0x878a;
	}
	else if(0xfa59 == (*sjis)){
		(*sjis) = 0x8782;
	}
	else if(0xfa5a == (*sjis)){
		(*sjis) = 0x8784;
	}
	else if(0xfa5b == (*sjis)){
		(*sjis) = 0x879a;
	}
}

int CMojiVect::IsNumCode()
{
	if(0x30 <= m_nBaseCode 
	&& m_nBaseCode <= 0x39){
		return(TRUE);
	}
	if(0x00824f <= m_nBaseCode
	&& m_nBaseCode <= 0x008258){
		return(TRUE);
	}
	if(0x008143 == m_nBaseCode){
		return(TRUE);
	}
	if(0x008146 == m_nBaseCode){
		return(TRUE);
	}
	if(0x00815e == m_nBaseCode){
		return(TRUE);
	}
	if(0x008166 == m_nBaseCode){
		return(TRUE);
	}
	if(0x00818f == m_nBaseCode){
		return(TRUE);
	}
	if(0x008190 == m_nBaseCode){
		return(TRUE);
	}
	if(0x008193 == m_nBaseCode){
		return(TRUE);
	}
	return(FALSE);
}

int CMojiVect::CheckSpecialSpace(DBL *time)
{
	if(m_nBaseCode == 0x0f9fc){
		*time = 0.25;
		return(TRUE);
	}else if(m_nBaseCode == 0x0f9fb){
		*time = 0.333333;
		return(TRUE);
	}
	return(FALSE);
}

void CMojiVect::Shadow1Draw(CDC *pcDC, COLORREF clr, int nCnt, DBL dAddX, DBL dAddY)
{
	int		flag;
	DBL		dW, dH;
	DBL		dSX, dSY;
	DBL		dStepX, dStepY;
	int		rval, gval, bval;
	CDataList *pcDataList;
	int		i;

	flag = CheckTateYoko();
	rval = GetRValue(clr);
	gval = GetGValue(clr);
	bval = GetBValue(clr);
	dStepX = -dAddX / nCnt;
	dStepY = -dAddY / nCnt;
	if(m_pcDataList != NULL){
		scrnsetpreviewpen(pcDC->m_hDC, rval, gval, bval, 3);
		pcDataList = (CDataList*)m_pcDataList->CopyData(dAddX, dAddY);
		for(i = 0; i < nCnt-2; i++){
			pcDataList->MoveTrns(dStepX, dStepY);
			pcDataList->StockClrDraw(pcDC);
		}
		delete(pcDataList);
		scrnpenend(pcDC->m_hDC);
		return;
	}

	GetFontSize(&dW, &dH);
	GetStartXY(&dSX, &dSY);
	m_pcFSF1Font->SetTateYoko(flag);
	scrnsetpreviewpen(pcDC->m_hDC, rval, gval, bval, 3);
	for(i = 0; i < nCnt-2; i++){
		m_pcFSF1Font->TrnsPlgnDraw(pcDC, dSX+dAddX-dStepX*i, dSY+dAddY-dStepY*i, dW, dH, m_hFno);
	}
	scrnpenend(pcDC->m_hDC);
}

void CMojiVect::Shadow2Draw(CDC *pcDC, COLORREF clr, int nCnt, DBL dAddX, DBL dAddY)
{
	int		flag;
	DBL		dW, dH;
	DBL		dSX, dSY;
	DBL		dStepX, dStepY;
	int		rval, gval, bval;
	DBL		drval, dgval, dbval;
	CDataList *pcDataList;
	int		i;

	flag = CheckTateYoko();
	rval = GetRValue(clr);
	gval = GetGValue(clr);
	bval = GetBValue(clr);
	drval = (DBL)(255-rval) / (DBL)nCnt;
	dgval = (DBL)(255-gval) / (DBL)nCnt;
	dbval = (DBL)(255-bval) / (DBL)nCnt;
	dStepX = -dAddX / nCnt;
	dStepY = -dAddY / nCnt;
	if(m_pcDataList != NULL){
		pcDataList = (CDataList*)m_pcDataList->CopyData(dAddX, dAddY);
		for(i = 0; i < nCnt-1; i++){
			rval = 255 - (int)(drval * i);
			gval = 255 - (int)(dgval * i);
			bval = 255 - (int)(dbval * i);
			scrnsetpreviewpen(pcDC->m_hDC, rval, gval, bval, 3);
			pcDataList->MoveTrns(dStepX, dStepY);
			pcDataList->StockClrDraw(pcDC);
			scrnpenend(pcDC->m_hDC);
		}
		delete(pcDataList);
		return;
	}

	GetFontSize(&dW, &dH);
	GetStartXY(&dSX, &dSY);
	m_pcFSF1Font->SetTateYoko(flag);
	for(i = 0; i < nCnt-2; i++){
		rval = 255 - (int)(drval * i);
		gval = 255 - (int)(dgval * i);
		bval = 255 - (int)(dbval * i);
		scrnsetpreviewpen(pcDC->m_hDC, rval, gval, bval, 3);
		m_pcFSF1Font->TrnsPlgnDraw(pcDC, dSX+dAddX-dStepX*i, dSY+dAddY-dStepY*i, dW, dH, m_hFno);
		scrnpenend(pcDC->m_hDC);
	}
	scrnpenend(pcDC->m_hDC);
}
