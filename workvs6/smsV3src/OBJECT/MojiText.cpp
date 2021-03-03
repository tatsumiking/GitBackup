//
// Easy Scanner System
//
// object mojitext.cpp  文字列フォント構造関係関数
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include <math.h>
#include <mbstring.h>

#include "mojitext.h"

extern "C" {
	void curveroundtrns(DBL srcx, DBL srcy, DBL x0, DBL y0, DBL rag, DBL *dstx, DBL *dsty);
	int  baseiskanji(int far hc);
	int mojienvgetfonttype(LPSTR fullname);
	void mojienvgetfilename(LPSTR fontname, LPSTR filename);
	void mojienvgetsfilename(LPSTR fontname, LPSTR filename);
	void scrndxymmtorltv(LPDBL x, LPDBL y);
	void scrndlenmmtorltv(DBL *len);
	int	senvgetcrtpenno();
	void senvgetpenrgb(int pno, LPINT rval, LPINT gval, LPINT bval);
	int senvgetplgnlimit();
	void scrnsetpreviewpen(HDC PaintDC, int r, int g, int b, int thick);
	void scrnsetclrbrushandpen(HDC PaintDC, COLORREF fillclr, COLORREF lineclr, int nthick);
	int	senvcheckdrawmode();
	int senvgetclrmode();
	void scrnpenend(HDC PaintDC);
	int senvcheckverjion();
}

IMPLEMENT_SERIAL(CMojiText, CObject, 3)

CMojiText::CMojiText()
{
}

void CMojiText::Serialize(CArchive& ar)
{
	CString	csFontName;
	DBL		minx, miny, maxx, maxy;
	DBL		xtime, ytime;
	DBL		setxtime, setytime;
	int		ver;
	DBL		wd, hi;

	// バージョンを変更してm_dSizeを保存
	ver = senvcheckverjion();
	if (ar.IsStoring())
	{
		csFontName = _T(m_szFontName);
		ar << csFontName;
		ar << m_nTateYoko;
		ar << m_nCode;
		ar << m_dSpacePar;
		ar << m_dFlatPar;
		ar << m_dMoveXPar;
		ar << m_dMoveYPar;
		ar << m_nBold;
		ar << m_nSlope;
		ar << m_dTT;
		GetDMiniMax(&minx, &miny, &maxx, &maxy);
		GetTrnsXYTime(&xtime, &ytime);
		setxtime = (maxx+minx)/2 + xtime/100.0;
		setytime = (maxy+miny)/2 + ytime/100.0;
		SetTrnsXYTime(setxtime, setytime);
	}
	else
	{
		ar >> csFontName;
		ar >> m_nTateYoko;
		ar >> m_nCode;
		ar >> m_dSpacePar;
		ar >> m_dFlatPar;
		ar >> m_dMoveXPar;
		ar >> m_dMoveYPar;
		ar >> m_nBold;
		ar >> m_nSlope;
		ar >> m_dTT;
	}
	CVect::Serialize(ar);

	if (ar.IsStoring()){
		SetTrnsXYTime(xtime, ytime);
	}
	else{
		// 2002.07.23 立身 昔のデータが0（黒）であるため1（黒）に設定しなおし
		if(m_nFillPen == 0){
			m_nFillPen = 1;
		}
		if(m_nOutLinePen == 0){
			m_nOutLinePen = 1;
		}
		SetFont(csFontName.GetBuffer(RECMAX));
		GetDMiniMax(&minx, &miny, &maxx, &maxy);
		GetFontSize(&wd, &hi);
		if(0x829f <= m_nCode && m_nCode <= 0x82f1){
			wd = wd / m_dTHrgnPar * 100.0;
			hi = hi / m_dTHrgnPar * 100.0;
		}else if(0x8340 <= m_nCode && m_nCode <= 0x8396){
			wd = wd / m_dTKtknPar * 100.0;
			hi = hi / m_dTKtknPar * 100.0;
		}
		if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
			m_dSize = wd;
		}else{
			m_dSize = hi;
		}
		GetTrnsXYTime(&xtime, &ytime);
		if(xtime != 1.0 && ytime != 1.0){
			setxtime = (xtime - (maxx+minx)/2) * 100.0;
			setytime = (ytime - (maxy+miny)/2) * 100.0;
			SetTrnsXYTime(setxtime, setytime);
		}
	}
}

CMojiText::CMojiText(int nDummy)
	: CVect(nDummy)
{
	m_pcDataList = NULL;
	strcpy(m_szFontName, "ＭＳ 明朝");
	SetDXY(0, 0, 0);
	SetDXY(1, 256, 0);
	SetDXY(2, 256, 256);
	SetDXY(3, 0, 256);
	SetTrnsXYTime(1.0, 1.0);
	SetPointCount(5);
	m_nTateYoko = YOKOGAKI;
	m_nCode = 0;
	m_dSpacePar = NOSETPAR;
	m_dFlatPar = NOSETPAR;
	m_dTrnsPar = NOSETPAR;
	m_dMoveXPar = 0;
	m_dMoveYPar = 0;
	m_nBold = OFF;
	m_nSlope = OFF;
	m_dTT = 0;
	m_dHWTime = 1.0;
	m_nPop = OFF;
	m_dSEPWD = 0;
	m_dSEPHI = 0;
}

CMojiText::~CMojiText()
{
	if(m_pcDataList != NULL){
		delete(m_pcDataList);
	}
}

int CMojiText::CheckID()
{
	return(MOJITEXTID);
}

//p 内部データーが空の状態であれば削除しすべてが空のときTRUEを返す
int CMojiText::CheckNullData()
{
	if(m_pcDataList != NULL){
		if(m_pcDataList->CheckNullData() == TRUE){
			return(TRUE);
		}
	}
	return(FALSE);
}

void CMojiText::RenewMiniMax()
{
	CVect::SetPointCount(4);
	CVect::RenewMiniMax();
	CVect::SetPointCount(5);
}

void CMojiText::GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	RenewMiniMax();
	CVect::GetDMiniMax(minix, miniy, maxx, maxy);
}

void CMojiText::GetWakuDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	DBL		size;

	GetMojiWakuSize(&size);
	GetDMiniMax(minix, miniy, maxx, maxy);

	if(m_nTateYoko == TATEGAKI || m_nTateYoko == TATEOKI){
		*maxy = *miniy + size;
	}
	else{
		*maxx = *minix + size;
	}
}

void CMojiText::GetDataDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	DBL		sp, ep;
	DBL		xsize, ysize;

	(*minix) = INTMAX;
	(*miniy) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	/*
	if(m_pcDataList != NULL){
		m_pcDataList->GetDMiniMax(minix, miniy, maxx, maxy);
		return;
	}
	*/
	GetDMiniMax(minix, miniy, maxx, maxy);
	// 文字をビットマップに描画し文字位置計算が必要
	GetDataSEP(&sp, &ep);
	if(m_nTateYoko == TATEGAKI){
		ysize = *maxy - *miniy;
		(*maxy) -= sp;
		(*miniy) += (ysize - ep);
	}
	else if(m_nTateYoko == TATEOKI){
		ysize = *maxy - *miniy;
		(*maxy) -= ep;
		(*miniy) += (ysize - sp);
	}
	else{
		xsize = *maxx - *minix;
		(*minix) += sp;
		(*maxx) -= (xsize - ep);
	}
}

CData* CMojiText::SrchData(DBL x, DBL y, DBL *retlen)
{
	DBL		sx, sy, ex, ey;
	CData	*retData;

	*retlen = INTMAX;
	if(m_pcDataList != NULL){
		retData = m_pcDataList->SrchData(x, y, retlen);
		return(this);
	}
	GetDataDMiniMax(&sx, &sy, &ex, &ey);
	*retlen = GetPreLengthBoxs(x, y, sx, sy, ex, ey);
	return(this);
}

CData* CMojiText::SrchDataBoxs(DBL x, DBL y, DBL *retlen)
{
	DBL		sx, sy, ex, ey;
	CData	*retData;

	*retlen = INTMAX;
	if(m_pcDataList != NULL){
		// 文字を構成するベクトルを返す
		retData = m_pcDataList->SrchDataBoxs(x, y, retlen);
		return(this);
	}
	GetDataDMiniMax(&sx, &sy, &ex, &ey);
	*retlen = GetPreLengthBoxs(x, y, sx, sy, ex, ey);
	return(this);
}

CData *CMojiText::SrchNearOnLinePoint(DBL xx, DBL yy, DBL *retlen, DBL *retx, DBL *rety, int *retpp)
{
	CData	*retData;

	retData = NULL;
	*retlen = INTMAX;
	*retx = 0;
	*rety = 0;
	*retpp = -1;
	if(m_pcDataList != NULL){
		retData = m_pcDataList->SrchNearOnLinePoint(xx, yy, retlen, retx, rety, retpp);
	}
	return(retData);
}

CData* CMojiText::SrchNearPoint(DBL xx, DBL yy, DBL * retlen, int * retpp)
{
	CData	*retData;

	*retlen = INTMAX;
	*retpp = 0;
	retData = NULL;
	if(m_pcDataList != NULL){
		retData = m_pcDataList->SrchNearPoint(xx, yy, retlen, retpp);
	}
	return(retData);
}

void CMojiText::DeleteData(CData * delData)
{
	if(m_pcDataList != NULL){
		m_pcDataList->DeleteData(delData);
	}
}

CData * CMojiText::CopyData(DBL movex, DBL movey)
{
	CMojiText	*pcnewData;
	int		pcnt, pp, atr;
	DBL		dx, dy;

	pcnewData = (CMojiText*)NewDataCreate(MOJITEXTID);
	pcnt = GetPointCount();
	pcnewData->SetPointCount(pcnt);
	for(pp = 0; pp < 4; pp++){
		GetAtrDXY(pp, &atr, &dx, &dy);
		pcnewData->SetAtrDXY(pp, atr, dx+movex, dy+movey);
	}
	// １文字変形用パラメーター
	GetAtrDXY(pp, &atr, &dx, &dy);
	pcnewData->SetAtrDXY(pp, atr, dx, dy);

	pcnewData->SetBaseFontSize(m_dSize);
	pcnewData->SetTT(m_dTT);
	pcnewData->SetSpacePar(m_dSpacePar);
	pcnewData->SetFlatPar(m_dFlatPar);
	pcnewData->SetMoveXPar(m_dMoveXPar);
	pcnewData->SetMoveYPar(m_dMoveYPar);
	pcnewData->SetFillPen(m_nFillPen);
	pcnewData->SetBold(m_nBold);
	pcnewData->SetSlope(m_nSlope);
	pcnewData->SetFont(m_szFontName);
	pcnewData->SetTateGaki(m_nTateYoko);
	pcnewData->SetCode(m_nCode);
	return((CData*)pcnewData);
}

void CMojiText::MoveTrns(DBL movex, DBL movey)
{
	CVect::SetPointCount(4);
	CVect::MoveTrns(movex, movey);
	CVect::SetPointCount(5);
	if(m_pcDataList != NULL){
		m_pcDataList->MoveTrns(movex, movey);
	}
}

void CMojiText::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == TATEOKI){
		m_dSize *= timex;
	}
	else{
		m_dSize *= timey;
	}
	CVect::SetPointCount(4);
	CVect::SizeTrns(cntrx, cntry, timex, timey);
	CVect::SetPointCount(5);
	if(m_pcDataList != NULL){
		m_pcDataList->SizeTrns(cntrx, cntry, timex, timey);
	}
}

void CMojiText::SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
{
	if(ddx != 0 && ddy != 0){
		m_nSlope = TRUE;
	}
}

void CMojiText::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	CVect::SetPointCount(4);
	CVect::RoundTrns(cntrx, cntry, tsin, tcos);
	CVect::SetPointCount(5);
	if(m_pcDataList != NULL){
		m_pcDataList->RoundTrns(cntrx, cntry, tsin, tcos);
	}
}

void CMojiText::GetDrawElement(DBL *retsx, DBL *retsy, DBL *retwd, DBL *rethi, DBL *retrag)
{
	DBL		dX[4], dY[4];
	DBL		sx, sy;
	DBL		sizex, sizey, size;
	DBL		hi, wd;
	DBL		xtime, ytime;
	DBL		rag;

	GetFontBoxs(dX, dY);
	scrndxymmtorltv(&(dX[0]), &(dY[0]));
	scrndxymmtorltv(&(dX[1]), &(dY[1]));
	scrndxymmtorltv(&(dX[2]), &(dY[2]));
	scrndxymmtorltv(&(dX[3]), &(dY[3]));
	wd = hypot(dX[1]-dX[0], dY[1]-dY[0]);
	hi = hypot(dX[3]-dX[0], dY[3]-dY[0]);
	sizex = wd;
	sizey = hi;
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		size = wd;
	}
	else{
		size = hi;
	}
	GetTrnsXYTime(&xtime, &ytime);
	wd = wd * xtime;
	hi = hi * ytime;
	if(m_dTrnsPar != NOSETPAR){
		wd *= m_dTrnsPar / 100.0;
		hi *= m_dTrnsPar / 100.0;
	}
	rag = atan2(dY[1]-dY[0], dX[1]-dX[0]);
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == TATEOKI){
		sx = dX[2];
		sy = dY[2];
	}
	else{
		sx = dX[0];
		sy = dY[0];
	}
	if(m_nTateYoko == TATEGAKI){
		sx = sx - (sizex - wd) / 2 + size * m_dMoveXPar / 100.0;
		sy = sy + (sizey - hi) / 2 + size * m_dMoveYPar / 100.0;
	}
	else{
		sx = sx + (sizex - wd) / 2 + size * m_dMoveXPar / 100.0;
		sy = sy - (sizey - hi) / 2 + size * m_dMoveYPar / 100.0;
	}
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == TATEOKI){
			rag += M_PI_2;
	}
	*retsx = sx;
	*retsy = sy;
	*retwd = wd;
	*rethi = hi;
	*retrag = rag;
}

void CMojiText::FillClrDraw(CDC *pcDC)
{
	DBL		sx, sy;
	DBL		hi, wd;
	DBL		rag;

	if(m_pcDataList != NULL){
		m_pcDataList->StockClrDraw(pcDC);
		return;
	}

	GetDrawElement(&sx, &sy, &wd, &hi, &rag);

	switch(m_nFontType){
	case TT1FONT:
	case TT2FONT:
	case TT3FONT:
	case TT4FONT:
		if(DrawOutESF1Font(pcDC, sx, sy, wd, hi, rag) == FALSE){
			DrawOutTrueTypeFont(pcDC, sx, sy, wd, hi, rag);
		}
		break;
	case ES0FONT:
		DrawOutESF0Font(pcDC, sx, sy, wd, hi, rag);
		break;
	case ES1FONT:
		DrawOutESF1Font(pcDC, sx, sy, wd, hi, rag);
		break;
	case ARCFONT:
	case MAXFONT:
		if(DrawOutESF1Font(pcDC, sx, sy, wd, hi, rag) == FALSE){
			DrawOutARCFont(pcDC, sx, sy, wd, hi, rag);
		}
		break;
	}
}

void CMojiText::CheckFlagDraw(int flag, CDC *pcDC)
{
	FillClrDraw(pcDC);
}

void CMojiText::CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick)
{
	int		nthick;

	nthick = (int)thick;
	scrnsetclrbrushandpen(pcDC->m_hDC, clr1, clr1, nthick);
	FillClrDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CMojiText::DrawMojiWaku(CDC *pcDC)
{
	DBL		x0, y0;
	DBL		x1, y1;
	DBL		x2, y2;
	DBL		x3, y3;

	GetDXY(0, &x0, &y0);
	GetDXY(1, &x1, &y1);
	GetDXY(2, &x2, &y2);
	GetDXY(3, &x3, &y3);
	scrndxymmtorltv(&x0, &y0);
	scrndxymmtorltv(&x1, &y1);
	scrndxymmtorltv(&x2, &y2);
	scrndxymmtorltv(&x3, &y3);
	pcDC->MoveTo((int)x0, (int)y0);
	pcDC->LineTo((int)x1, (int)y1);
	pcDC->LineTo((int)x2, (int)y2);
	pcDC->LineTo((int)x3, (int)y3);
	pcDC->LineTo((int)x0, (int)y0);
}

void CMojiText::OnDraw(CDC *pcDC)
{
	int		mode, crtno;
	int		penno;
	int		rval, gval, bval;
	DBL		size, thick;
	int		nthick;
	int		nlimit;

	if(m_pcDataList != NULL){
		m_pcDataList->OnDraw(pcDC);
		return;
	}
	nlimit = senvgetplgnlimit();
	mode = senvcheckdrawmode();
	crtno = senvgetcrtpenno();
	penno = GetFillPen();
	if(mode == PRINTVIEW && crtno != -1){
		penno = GetFillPen();
		if(penno != crtno){
			return;
		}
	}
	if(penno == -1){
		return;
	}
	if(m_nPop == ON){
		SetFSF1Data();
		GetFontBody(&size);
		thick = size * 0.1;
		scrndlenmmtorltv(&thick);
		nthick = (int)thick;
		if(nthick < nlimit){
			nthick = nlimit;
		}
		scrnsetpreviewpen(pcDC->m_hDC, 255, 255, 255, nthick);
		m_pcFSF1Font->PlgnDraw(pcDC);
		scrnpenend(pcDC->m_hDC);

		if(penno == 0){
			rval = 0; gval = 0; bval = 0;
		}
		else{
			senvgetpenrgb(penno, &rval, &gval, &bval);
		}
		if(senvgetclrmode() == BLACK){
			rval = 0; gval = 0; bval = 0;
		}
		scrnsetpreviewpen(pcDC->m_hDC, rval, gval, bval, nlimit);
		m_pcFSF1Font->FillDraw(pcDC);
		scrnpenend(pcDC->m_hDC);
		return;
	}
	if(penno == 0){
		rval = 0; gval = 0; bval = 0;
	}
	else{
		senvgetpenrgb(penno, &rval, &gval, &bval);
	}
	if(senvgetclrmode() == BLACK){
		rval = 0; gval = 0; bval = 0;
	}
	scrnsetpreviewpen(pcDC->m_hDC, rval, gval, bval, nlimit);
	FillClrDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CMojiText::OutlineDraw(CDC *pcDC)
{
}

void CMojiText::SetFSF1Data()
{
	DBL		sx, sy;
	DBL		wd, hi;
	DBL		rag;

	switch(m_nFontType){
	case TT1FONT:
	case TT2FONT:
	case TT3FONT:
	case TT4FONT:
		if(DataChangeESF1Font() == FALSE){
			DataChangeTrueTypeFont();
		}
		break;
	case ES0FONT:
		DataChangeESF0Font();
		break;
	case ES1FONT:
		DataChangeESF1Font();
		break;
	case ARCFONT:
	case MAXFONT:
		if(DataChangeESF1Font() == FALSE){
			DataChangeARCFont();
		}
		break;
	}
	GetDrawElement(&sx, &sy, &wd, &hi, &rag);
	if(m_nTateYoko == TATEGAKI){
		if(IsZenkaku()){
			rag -= M_PI_2;
		}
		sx = sx-wd;
	}
	else if(m_nTateYoko == YOKOOKI){
		if(IsZenkaku()){
			rag -= M_PI_2;
		}
		sy = sy-hi;
	}
	else{
		sy = sy-hi;
	}
	m_pcFSF1Font->SetTateYoko(m_nTateYoko);
	m_pcFSF1Font->SetBold(m_nBold);
	m_pcFSF1Font->SetSlope(m_nSlope);
	// 文字の左上が文字の始点
	m_pcFSF1Font->TrnsElementInit(sx, sy, wd, hi, rag);
}

void CMojiText::DataChange()
{
	int		flag;
	DBL		dX[4], dY[4];
	DBL		dW, dH;
	DBL		dSx, dSy;
	DBL		xtime, ytime;
	DBL		sizex, sizey, size;
	DBL		cx, cy;
	DBL		rag;
	DBL		tsin, tcos;
	int		pcntmax;
	int		spp, epp, pcnt;
	BYTE	*bufsp;
	CVect	*pcVect;
	int		penno;
	DBL		thick;

	if(m_pcDataList != NULL){
		return;
	}
	flag = CheckTateYoko();
	switch(m_nFontType){
	case TT1FONT:
	case TT2FONT:
	case TT3FONT:
	case TT4FONT:
		if(DataChangeESF1Font() == FALSE){
			DataChangeTrueTypeFont();
		}
		break;
	case ES0FONT:
		DataChangeESF0Font();
		break;
	case ES1FONT:
		DataChangeESF1Font();
		break;
	case ARCFONT:
	case MAXFONT:
		if(DataChangeESF1Font() == FALSE){
			DataChangeARCFont();
		}
		break;
	}

	m_pcFSF1Font->PointMiniMaxGet();

	m_pcDataList = (CDataList*)NewDataCreate(DATALISTID);
	if(m_pcDataList == NULL){
		return;
	}
	GetFontSize(&dW, &dH);
	sizex = dW;
	sizey = dH;
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		size = dW;
	}
	else{
		size = dH;
	}
	GetTrnsXYTime(&xtime, &ytime);
	dW = dW * xtime;
	dH = dH * ytime;
	if(m_dTrnsPar != NOSETPAR){
		dW *= m_dTrnsPar / 100.0;
		dH *= m_dTrnsPar / 100.0;
	}
	if(m_dFlatPar != NOSETPAR){
		if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
			if(IsZenkaku() && m_nCode == 0x817c){
				dW *= m_dFlatPar / 100.0;
			}
			else{
				dH *= m_dFlatPar / 100.0;
			}
		}
		else{
			dW *= m_dFlatPar / 100.0;
		}
	}
	GetFontBoxs(dX, dY);
	if(m_nTateYoko == TATEGAKI){
		rag = atan2(dY[1]-dY[0], dX[1]-dX[0]);
		// 2001.08.10 文字解除されると逆になる現象の修正
		//dSx = dX[2] + (sizex - dW) / 2 + sizex * m_dMoveXPar / 100.0;
		//dSy = dY[2] + (sizey - dH) / 2 - sizey * m_dMoveYPar / 100.0;
		dSx = dX[0] + (sizex - dW) / 2 + sizex * m_dMoveXPar / 100.0;
		dSy = dY[0] + (sizey - dH) / 2 - sizey * m_dMoveYPar / 100.0;
	}
	else{
		rag = atan2(dY[1]-dY[0], dX[1]-dX[0]);
		dSx = dX[0] + (sizex - dW) / 2 + sizex * m_dMoveXPar / 100.0;
		dSy = dY[0] + (sizey - dH) / 2 - sizey * m_dMoveYPar / 100.0;
	}
	m_pcFSF1Font->SetTateYoko(flag);
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

	cx = dX[0];
	cy = dY[0];
	tsin = sin(rag);
	tcos = cos(rag);
	m_pcDataList->RoundTrns(cx, cy, tsin, tcos);
}

void CMojiText::SetVect(LPBYTE fbuf, int pcnt, CVect *pcVect)
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

void CMojiText::SetPosition(DBL sx, DBL sy, DBL size, DBL par, DBL rag)
{
	DBL		wd, hi;
	DBL		ex, ey;
	DBL		movex, movey;
	DBL		cx, cy;
	DBL 	tsin, tcos;

	ResetFontSize(size, par, &wd, &hi);
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		size = wd;
	}
	else{
		size = hi;
	}
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		ex = sx + hi;
		ey = sy + wd;
	}
	else{
		ex = sx + wd;
		ey = sy + hi;
	}
	if(m_dMoveXPar != NOSETPAR){
		movex = size * m_dMoveXPar / 100.0;
	}
	else{
		movex = 0;
	}
	if(m_dMoveXPar != NOSETPAR){
		movey = size * m_dMoveYPar / 100.0;
	}
	else{
		movey = 0;
	}

	SetDXY(0, sx, sy);
	SetDXY(1, ex, sy);
	SetDXY(2, ex, ey);
	SetDXY(3, sx, ey);

	CVect::SetPointCount(4);
	CVect::MoveTrns(movex, movey);
	cx = sx;
	cy = sy;
	tsin = sin(rag);
	tcos = cos(rag);
	CVect::RoundTrns(cx, cy, tsin, tcos);
	CVect::SetPointCount(5);

	if(m_pcDataList != NULL){
		delete(m_pcDataList);
		m_pcDataList = NULL;
	}
}

void CMojiText::GetFontBoxs(DBL x[], DBL y[])
{
	GetDXY(0, &(x[0]), &(y[0]));
	GetDXY(1, &(x[1]), &(y[1]));
	GetDXY(2, &(x[2]), &(y[2]));
	GetDXY(3, &(x[3]), &(y[3]));
}

void CMojiText::SetFontSize(DBL dFWsize, DBL dFHsize)
{
	DBL		x0, y0;
	DBL		x1, y1;
	DBL		x2, y2;
	DBL		x3, y3;
	DBL		lenx, leny;
	DBL		timex, timey;

	GetDXY(0, &x0, &y0);
	GetDXY(1, &x1, &y1);
	GetDXY(2, &x2, &y2);
	GetDXY(3, &x3, &y3);
	SetDXY(0, x0, y0);
	SetDXY(1, x0+dFWsize, y0);
	SetDXY(2, x0+dFWsize, y0+dFHsize);
	SetDXY(3, x0, y0+dFHsize);
	lenx = hypot(x0-x1, y0-y1);
	leny = hypot(x0-x3, y0-y3);
	if(m_pcDataList != NULL){
		timex = fabs(dFWsize) / lenx;
		timey = fabs(dFHsize) / leny;
		m_pcDataList->SizeTrns(x0, y0, timex, timey);
	}
}

void CMojiText::SetBaseFontSize(DBL dSize)
{
	m_dSize = dSize;
}

void CMojiText::GetBaseFontSize(DBL *dSize)
{
	*dSize = m_dSize;
}

void CMojiText::GetFontSize(DBL *dFWsize, DBL *dFHsize)
{
	DBL		sx, sy;
	DBL		ex, ey;

	GetDXY(0, &sx, &sy);
	GetDXY(1, &ex, &ey);
	*dFWsize = hypot(ex-sx, ey-sy);
	GetDXY(3, &ex, &ey);
	*dFHsize = hypot(ex-sx, ey-sy); 
}

void CMojiText::SetTT(DBL tt)
{
	m_dTT = tt;
}

void CMojiText::GetTT(LPDBL tt)
{
	*tt = m_dTT;
}

void CMojiText::SetFont(LPSTR lpFontName)
{
	strcpy(m_szFontName, lpFontName);
	m_nFontType = mojienvgetfonttype(lpFontName);
	switch(m_nFontType){
	case TT2FONT:
	case TT3FONT:
	case TT4FONT:
		SetHWTimeTureTypeFont();
		break;
	default:
		m_dHWTime = 1.0;
		break;
	}
}

void CMojiText::GetFont(LPSTR lpFontName)
{
	strcpy(lpFontName, m_szFontName);
}

void CMojiText::SetTateGaki(int tategaki)
{
	m_nTateYoko = tategaki;
}

void CMojiText::GetTateGaki(LPINT tategaki)
{
	*tategaki = m_nTateYoko;
}

void CMojiText::SetCode(UINT code)
{
	m_nCode = code;
}

void CMojiText::GetCode(LPUINT code)
{
	*code = m_nCode;
}

void CMojiText::SetStartXY(DBL dSx, DBL dSy)
{
	DBL		sx, sy;
	DBL		movex, movey;

	GetDXY(0, &sx, &sy);
	movex = dSx - sx;
	movey = dSy - sy;
	CVect::SetPointCount(4);
	CVect::MoveTrns(movex, movey);
	CVect::SetPointCount(5);
	if(m_pcDataList != NULL){
		m_pcDataList->MoveTrns(movex, movey);
	}
}

void CMojiText::GetStartXY(DBL *dSx, DBL *dSy)
{
	GetDXY(0, dSx, dSy);
}

void CMojiText::GetSpacePar(LPDBL spacepar)
{
	*spacepar = m_dSpacePar;
}

void CMojiText::SetSpacePar(DBL spacepar)
{
	m_dSpacePar = spacepar;
}

void CMojiText::GetFlatPar(LPDBL flatpar)
{
	*flatpar = m_dFlatPar;
}

void CMojiText::SetFlatPar(DBL flatpar)
{
	m_dFlatPar = flatpar;
}

void CMojiText::GetMoveXPar(LPDBL movexpar)
{
	*movexpar = m_dMoveXPar;
}

void CMojiText::SetMoveXPar(DBL movexpar)
{
	m_dMoveXPar = movexpar;
}

void CMojiText::GetMoveYPar(LPDBL moveypar)
{
	*moveypar = m_dMoveYPar;
}

void CMojiText::SetMoveYPar(DBL moveypar)
{
	m_dMoveYPar = moveypar;
}

void CMojiText::SetTrnsXYTime(DBL xtime, DBL ytime)
{
	SetDXY(4, xtime, ytime);
}

void CMojiText::GetTrnsXYTime(LPDBL xtime, LPDBL ytime)
{
	GetDXY(4, xtime, ytime);
}

void CMojiText::AddMoveXY(DBL xmove, DBL ymove)
{
	DBL		dX[4], dY[4];
	DBL		xpar, ypar;
	DBL		size;
	DBL		wd, hi;

	GetFontBoxs(dX, dY);
	wd = hypot(dX[1]-dX[0], dY[1]-dY[0]);
	hi = hypot(dX[3]-dX[0], dY[3]-dY[0]);
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == TATEOKI){
		size = wd;
	}
	else{
		size = hi;
	}
	xpar = xmove / size * 100.0; 
	ypar = -ymove / size * 100.0; 
	m_dMoveXPar += xpar;
	m_dMoveYPar += ypar;
}

void CMojiText::AddTrnsXYTime(DBL xtime, DBL ytime)
{
	DBL		srcxtime, srcytime;

	GetTrnsXYTime(&srcxtime, &srcytime);
	srcxtime *= xtime;
	srcytime *= ytime;
	SetTrnsXYTime(srcxtime, srcytime);
}

void CMojiText::GetTrnsPar(LPDBL trnspar)
{
	*trnspar = m_dTrnsPar;
}

void CMojiText::SetTrnsPar(DBL trnspar)
{
	m_dTrnsPar = trnspar;
}

void CMojiText::GetBold(LPINT flag)
{
	*flag = m_nBold;
}

void CMojiText::SetBold(int flag)
{
	m_nBold = flag;
}

void CMojiText::GetSlope(LPINT flag)
{
	*flag = m_nSlope;
}

void CMojiText::SetSlope(int flag)
{
	m_nSlope = flag;
}

void CMojiText::ResetFontSize(DBL size, DBL par, DBL *width, DBL *hight)
{
	DBL		tpar;

	// 2001.06.29 差込時に文字が大きくなるバグ対処
	m_dSize = size;
	if(m_dFlatPar == MJFREEFLATPAR){
		*width = size;
		*hight = size;
	}
	else{
		if(m_dFlatPar == NOSETPAR){
			tpar = 100;
		}
		else{
			tpar = m_dFlatPar;
		}
		par = par * tpar / 100.0;
		if(m_nTateYoko){
			*width = size;
			*hight = size * par / 100.0;
		}
		else{
			*width = size * par / 100.0;
			*hight = size;
		}
	}
}

int CMojiText::IsZenkaku()
{
	if(0x0100 < m_nCode){
		return(TRUE);
	}
	else{
		return(FALSE);
	}
}

void CMojiText::GetFontBody(DBL *size)
{
	DBL		wd, hi;

	GetFontSize(&wd, &hi);
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == TATEOKI){
		*size = wd;
	}
	else{
		*size = hi;
	}
}

void CMojiText::GetFontWakuWidth(DBL *size)
{
	DBL		minx, miny, maxx, maxy;
	DBL		wd, hi;

	GetWakuDMiniMax(&minx, &miny, &maxx, &maxy);
	wd = maxx - minx;
	hi = maxy - miny;
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == TATEOKI){
		*size = hi;
	}
	else{
		*size = wd;
	}
}

void CMojiText::GetFontWidth(DBL *size)
{
	DBL		wd, hi;

	GetFontSize(&wd, &hi);
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == TATEOKI){
		*size = hi;
	}
	else{
		*size = wd;
	}
}

void CMojiText::GetMojiWakuSize(LPDBL mojisize)
{
	DBL		wd, hi;
	DBL		sp, ep;

	GetFontSize(&wd, &hi);
	switch(m_nFontType){
	case TT1FONT:
	case TT2FONT:
	case TT3FONT:
	case TT4FONT:
		GetMojiWakuSizeTrueTypeFont(mojisize);
		break;
	default:
		if(m_nTateYoko == TATEGAKI || m_nTateYoko == TATEOKI){
			*mojisize = hi;
		}
		else{
			*mojisize = wd;
		}
		if(IsZenkaku() == FALSE){
			GetDataSEP(&sp, &ep);
			(*mojisize) = (ep+sp);
		}
		break;
	}
}

//P 文字枠の先頭からのデータ開始位置、データ終了位置
void CMojiText::GetDataSEP(LPDBL sp, LPDBL ep)
{
	DBL		wd, hi;
	DBL		time;
	int		y;
	DBL		sx, ex;
	DBL		tsx, tex;

	GetFontSize(&wd, &hi);
	if(m_dSEPWD == wd && m_dSEPHI == hi){
		*sp = m_dSEPSP;
		*ep = m_dSEPEP;
		return;
	}
	m_dSEPWD = wd;
	m_dSEPHI = hi;
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		time = hi / FPROPSIZE;
	}
	else{
		time = wd / FPROPSIZE;
	}

	if(m_nCode == 0x0020){
		*sp = 0;
		*ep = FPROPSIZE * time / 2.0;
		m_dSEPSP = *sp;
		m_dSEPEP = *ep;
		return;
	}

	m_pcFSF1Font->m_pcMemDC->BitBlt(0, 0, FDIBCSIZE, FDIBCSIZE, NULL, 0, 0, WHITENESS);

	switch(m_nFontType){
	case TT1FONT:
	case TT2FONT:
	case TT3FONT:
	case TT4FONT:
		if(DrawOutMemDCESF1Font() == FALSE){
			DrawOutMemDCTrueTypeFont();
		}
		break;
	case ES0FONT:
		DrawOutMemDCESF0Font();
		break;
	case ES1FONT:
		DrawOutMemDCESF1Font();
		break;
	case ARCFONT:
	case MAXFONT:
		if(DrawOutMemDCESF1Font() == FALSE){
			DrawOutMemDCARCFont();
		}
		break;
	}
	m_pcFSF1Font->m_lpBits = (LPBYTE)GlobalLock(m_pcFSF1Font->m_hDIB);
	if(m_pcFSF1Font->m_lpBits == NULL){
		return;
	}
	m_pcFSF1Font->m_pcBitmap->GetBitmapBits(m_pcFSF1Font->m_dwSize, m_pcFSF1Font->m_lpBits);

	//m_pcFSF1Font->BmpFileOut("c:\\test001.bmp", FDIBCSIZE, FDIBCSIZE);

	sx = FDIBCSIZE;
	ex = FDIBCSIZE;
	for(y = 0; y < FPROPSIZE; y++){
		tsx = m_pcFSF1Font->PreFirstXOnBit(y);	
		tex = m_pcFSF1Font->PostFirstXOnBit(y);
		if(tsx != FDIBCSIZE){
			if(sx > tsx){
				sx = tsx;
			}
		}
		if(tex != FDIBCSIZE){
			if(ex > tex){
				ex = tex;
			}
		}
	}
	GlobalUnlock(m_pcFSF1Font->m_hDIB);
	if(sx == FDIBCSIZE && ex == FDIBCSIZE){
		*sp = 0;
		*ep = FPROPSIZE * time;
	}
	else{
		// 文字枠の先頭からの距離
		*sp = (sx - FPRSPSIZE) * time;
		*ep = ((FDIBCSIZE - ex) - FPRSPSIZE) * time;
	}
	m_dSEPSP = *sp;
	m_dSEPEP = *ep;
}

void CMojiText::GetPrePostTable(DBL pre[], DBL post[])
{
	DBL		wd, hi;
	DBL		time;
	int		y;
	DBL		sx, ex;
	DBL		sub;

	GetFontSize(&wd, &hi);
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == TATEOKI){
		time = hi / FPROPSIZE;
	}
	else{
		time = wd / FPROPSIZE;
	}
	if(m_nCode == 0x0020){
		for(y = 0; y < FPROPSIZE; y++){
			pre[y] = 0;
			post[y] = FPRSPSIZE  * time  / 2.0;
		}
		return;
	}
	m_pcFSF1Font->m_pcMemDC->BitBlt(0, 0, FDIBCSIZE, FDIBCSIZE, NULL, 0, 0, WHITENESS);
	
	switch(m_nFontType){
	case TT1FONT:
	case TT2FONT:
	case TT3FONT:
	case TT4FONT:
		if(DrawOutMemDCESF1Font() == FALSE){
			DrawOutMemDCTrueTypeFont();
		}
		break;
	case ES0FONT:
		DrawOutMemDCESF0Font();
		break;
	case ES1FONT:
		DrawOutMemDCESF1Font();
		break;
	case ARCFONT:
	case MAXFONT:
		if(DrawOutMemDCESF1Font() == FALSE){
			DrawOutMemDCARCFont();
		}
		break;
	}

	m_pcFSF1Font->m_lpBits = (LPBYTE)GlobalLock(m_pcFSF1Font->m_hDIB);
	if(m_pcFSF1Font->m_lpBits == NULL){
		return;
	}
	m_pcFSF1Font->m_pcBitmap->GetBitmapBits(m_pcFSF1Font->m_dwSize, m_pcFSF1Font->m_lpBits);
	//m_pcFSF1Font->BmpFileOut("c:\\test.bmp", FDIBCSIZE, FDIBCSIZE);
	sub = FDIBCSIZE - (FPROPSIZE+FPRSPSIZE*2);
	for(y = 0; y < FPROPSIZE; y++){
		sx = m_pcFSF1Font->PreFirstXOnBit(y);
		ex = m_pcFSF1Font->PostFirstXOnBit(y);
		if(sx == FDIBCSIZE){
			pre[y] = INTMAX;
		}
		else{
			pre[y] = (sx-FPRSPSIZE) * time;
		}

		if(ex == FDIBCSIZE){
			post[y] = INTMAX;
		}
		else{
			post[y] = (ex-sub-FPRSPSIZE)  * time;
		}
	}
	GlobalUnlock(m_pcFSF1Font->m_hDIB);
}

void CMojiText::SetUseClrPenno(int notbl[], LPINT cnt)
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

// TrueTypeフォント関係関数

void CMojiText::GetMojiWakuSizeTrueTypeFont(LPDBL mojisize)
{
	DBL		wd, hi;
	DBL		time;
	char	fullname[RECMAX];
	CFont	cFont;
	CFont	*pcOldFont;
	char	szStr[8];
	int		len;
	CSize	cSize;

	GetFontSize(&wd, &hi);
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == TATEOKI){
		time = hi / FPROPSIZE;
		sprintf(fullname, "@%s", m_szFontName);
	}
	else{
		time = wd / FPROPSIZE;
		sprintf(fullname, "%s", m_szFontName);
	}

	if(m_nTateYoko == TATEGAKI || m_nTateYoko == TATEOKI){
		if(m_nCode == 0x817c){
			// 縦書きのマイナスを処理
			FontCreate(&cFont, FPROPSIZE, FPROPSIZE, -900, 0, 0, fullname, m_nFontType);
		}
		else{
			FontCreate(&cFont, FPROPSIZE, FPROPSIZE, 0, 0, 0, fullname, m_nFontType);
		}
	}
	else{
		FontCreate(&cFont, FPROPSIZE, FPROPSIZE, 0, 0, 0, fullname, m_nFontType);
	}
	CDC *pcDC = m_pcWnd->GetDC();
	pcOldFont = pcDC->SelectObject(&cFont);

	if(IsZenkaku()){
		szStr[0] = (m_nCode >> 8) & 0x00ff;
		szStr[1] = (m_nCode) & 0x00ff;
		szStr[2] = '\0';
		len = 2;
	}
	else{
		szStr[0] = (m_nCode) & 0x00ff;
		szStr[1] = '\0';
		len = 1;
	}
	cSize = pcDC->GetTextExtent(szStr, len);
	pcDC->SelectObject(pcOldFont);
	cFont.DeleteObject();
	m_pcWnd->ReleaseDC(pcDC);
	*mojisize = (DBL)(cSize.cx)*time;
}

void CMojiText::DrawOutMemDCTrueTypeFont()
{
	DBL		wd, hi;
	DBL		sx, sy;
	DBL		fontrag;

	wd = FPROPSIZE; hi = FPROPSIZE;
	if(m_nTateYoko == TATEGAKI){
		sx = FPROPSIZE/2; sy = 0;
		fontrag = 0;
	}
	else if(m_nTateYoko == YOKOGAKI){
		sx = FPROPSIZE/2; sy = FPROPSIZE;
		fontrag = 0;
	}
	else if(m_nTateYoko == TATEOKI){
		sx = FPROPSIZE/2; sy = 0;
		fontrag = 0;
	}
	else if(m_nTateYoko == YOKOOKI){
		sx = FPROPSIZE/2; sy = FPROPSIZE;
		fontrag = 0;
	}

	DrawOutTrueTypeFont(m_pcFSF1Font->m_pcMemDC, sx, sy, wd, hi, fontrag);
}

void CMojiText::DrawOutTrueTypeFont(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL fontrag)
{
	char	fullname[RECMAX];
	int		nx, ny;
	CFont	cFont;
	CFont	*pcOldFont;
	char	szStr[8];
	CString	cStr;
	int		nhi, nwd;
	int		round, bold, slope;
	DBL		rag, xsize, ysize;
	DBL		x1, y1, x2, y2, x3, y3, x4, y4;

	if(m_nTateYoko == TATEGAKI && IsZenkaku() && m_nCode == 0x817c){
		sx -= wd;
	}
	if(m_nTateYoko == YOKOOKI && IsZenkaku() && m_nCode == 0x817c){
		sy += wd;
	}

	nwd = (int)wd;
	nhi = (int)hi;
	if(nwd < 1){
		nwd = 1;
	}
	if(nhi < 1){
		nhi = 1;
	}

	round = (int)(fontrag / M_1RAG * 10.0 * -1.0);
	bold = 0;
	if(m_nBold == ON){
		bold = FW_BOLD;
	}
	if(m_nSlope == ON){
		slope = TRUE;
	}
	else{
		slope = FALSE;
	}

	if(m_nTateYoko == TATEGAKI){
		sprintf(fullname, "@%s", m_szFontName);
		if(IsZenkaku() && m_nCode == 0x817c){
			// 縦書きのマイナスを処理
			FontCreate(&cFont, nhi, nwd, round+900, bold, slope, fullname, m_nFontType);
		}
		else{
			FontCreate(&cFont, nwd, nhi, round, bold, slope, fullname, m_nFontType);
		}
	}
	else if(m_nTateYoko == YOKOGAKI){
		strcpy(fullname, m_szFontName);
		FontCreate(&cFont, nhi, nwd, round, bold, slope, fullname, m_nFontType);
	}
	else if(m_nTateYoko == YOKOOKI){
		sprintf(fullname, "@%s", m_szFontName);
		if(IsZenkaku() && m_nCode == 0x817c){
			// 縦書きのマイナスを処理
			FontCreate(&cFont, nwd, nhi, round+900, bold, slope, fullname, m_nFontType);
		}
		else{
			FontCreate(&cFont, nhi, nwd, round, bold, slope, fullname, m_nFontType);
		}
	}
	else if(m_nTateYoko == TATEOKI){
		strcpy(fullname, m_szFontName);
		FontCreate(&cFont, nwd, nhi, round, bold, slope, fullname, m_nFontType);
	}
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == TATEOKI){
		rag = fontrag+M_PI_2;
	}
	else{
		rag = fontrag;
	}
	pcOldFont = pcDC->SelectObject(&cFont);

	if(IsZenkaku()){
		szStr[0] = (m_nCode >> 8) & 0x00ff;
		szStr[1] = (m_nCode) & 0x00ff;
		szStr[2] = '\0';
	}
	else{
		szStr[0] = (m_nCode) & 0x00ff;
		szStr[1] = '\0';
	}
	cStr = _T(szStr);
	if(m_nTateYoko == YOKOGAKI || m_nTateYoko == YOKOOKI){
		xsize = wd;
		ysize = hi;
		sx = sx;
		sy = sy;
	}
	else if(m_nTateYoko == TATEGAKI){
		xsize = wd;
		ysize = hi;
		//sx = sx-xsize;
		//sy = sy+ysize;
	}
	else if(m_nTateYoko == TATEOKI){
		xsize = wd;
		ysize = hi;
		sx = sx;
		sy = sy;
	}
	x1 = sx;
	y1 = sy;
	x2 = x1;
	y2 = y1-ysize;
	x3 = x1+xsize;
	y3 = y1-ysize;
	x4 = x1+xsize;
	y4 = y1;
	curveroundtrns(x1, y1, x1, y1, rag, &x1, &y1);
	curveroundtrns(x2, y2, x1, y1, rag, &x2, &y2);
	curveroundtrns(x3, y3, x1, y1, rag, &x3, &y3);
	curveroundtrns(x4, y4, x1, y1, rag, &x4, &y4);
#ifdef _NODEBUG
	pcDC->MoveTo((int)x1, (int)y1);
	pcDC->LineTo((int)x2, (int)y2);
	pcDC->LineTo((int)x3, (int)y3);
	pcDC->LineTo((int)x4, (int)y4);
	pcDC->LineTo((int)x1, (int)y1);
#endif
	if(m_nTateYoko == TATEGAKI){
		nx = (int)x1;
		ny = (int)y1;
	}
	else if(m_nTateYoko == TATEOKI){
		nx = (int)x1;
		ny = (int)y1;
	}
	else{
		nx = (int)x2;
		ny = (int)y2;
	}
	pcDC->TextOut(nx, ny, cStr);
	pcDC->SelectObject(pcOldFont);
	cFont.DeleteObject();
}

void CMojiText::SetHWTimeTureTypeFont()
{
	CFont	cFont;
	CFont	*pcOldFont;
	TEXTMETRIC sMetrics;

	FontCreate(&cFont, 256, 0, 0, 0, 0, m_szFontName, m_nFontType);
	pcOldFont = m_pcFSF1Font->m_pcMemDC->SelectObject(&cFont);
	m_pcFSF1Font->m_pcMemDC->GetTextMetrics(&sMetrics);
	m_pcFSF1Font->m_pcMemDC->SelectObject(pcOldFont);
	cFont.DeleteObject();
	m_dHWTime = (DBL)(sMetrics.tmAveCharWidth) / (DBL)(sMetrics.tmHeight);
}

void CMojiText::FontCreate(CFont *pcFont, int wd, int hi, int round, int bold, int slope, LPSTR name, int type)
{
	hi = (int)(hi * m_dHWTime);
	if(type == TT2FONT){
		pcFont->CreateFont(wd,hi,round,0,bold,slope,0,0,SHIFTJIS_CHARSET,0,0,0,0,name);
	}
	else if(type == TT3FONT){
		pcFont->CreateFont(wd,hi,round,0,bold,slope,0,0,ANSI_CHARSET,0,0,0,0,name);
	}
	else if(type == TT4FONT){
		pcFont->CreateFont(wd,hi,round,0,bold,slope,0,0,SYMBOL_CHARSET,0,0,0,0,name);
	}
	else{
		pcFont->CreateFont(wd,hi,round,0,bold,slope,0,0,DEFAULT_CHARSET,0,0,0,0,name);
	}
}

void CMojiText::DataChangeTrueTypeFont()
{
	char	fullname[RECMAX];

	m_pcWNTTFont->SetTateYoko(m_nTateYoko);
	if(m_nTateYoko == YOKOGAKI){
		sprintf(fullname, "%s", m_szFontName);
		m_pcWNTTFont->FontNameSet(fullname);
	}
	else if(m_nTateYoko == TATEGAKI){
		sprintf(fullname, "@%s", m_szFontName);
		m_pcWNTTFont->FontNameSet(fullname);
	}
	else if(m_nTateYoko == YOKOOKI){
		sprintf(fullname, "@%s", m_szFontName);
		m_pcWNTTFont->FontNameSet(fullname);
	}
	else if(m_nTateYoko == TATEOKI){
		sprintf(fullname, "%s", m_szFontName);
		m_pcWNTTFont->FontNameSet(fullname);
	}
	m_pcWNTTFont->SF1TrnsDataLoad(m_nCode);
}

// FT2書体関係関数

void CMojiText::DrawOutMemDCARCFont()
{
	DBL		wd, hi;
	DBL		sx, sy;
	DBL		fontrag;

	wd = FPROPSIZE; hi = FPROPSIZE;
	sx = FPROPSIZE/2; sy = FPROPSIZE;
	if(m_nTateYoko == TATEGAKI){
		wd = FPROPSIZE; hi = FPROPSIZE;
		sx = FPROPSIZE+FPROPSIZE/2; sy = 0;
		fontrag = 0;
	}
	else if(m_nTateYoko == YOKOGAKI){
		fontrag = 0;
	}
	else if(m_nTateYoko == YOKOOKI){
		fontrag = 0;
	}
	else if(m_nTateYoko == TATEOKI){
		fontrag = 0;
	}
	DrawOutARCFont(m_pcFSF1Font->m_pcMemDC, sx, sy, wd, hi, fontrag);
}

void CMojiText::DrawOutARCFont(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL fontrag)
{
	char	filename[RECMAX];

	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		if(IsZenkaku() && m_nCode == 0x817c){
			fontrag += M_PI_2;
		}
	}

	mojienvgetfilename(m_szFontName, filename);
	m_pcARCFont->FontNameSet(filename, m_nFontType);
	m_pcARCFont->SetTateYoko(m_nTateYoko);
	if(m_nTateYoko == TATEGAKI){
		if(IsZenkaku()){
			fontrag -= M_PI_2;
		}
		sx = sx-wd;
	}
	else if(m_nTateYoko == YOKOOKI){
		if(IsZenkaku()){
			fontrag -= M_PI_2;
		}
		sy = sy-hi;
	}
	else{
		sy = sy-hi;
	}
	m_pcARCFont->SetBold(m_nBold);
	m_pcARCFont->SetSlope(m_nSlope);
	m_pcARCFont->DataLoad(m_nCode);
	// 文字の左上が文字の始点
	m_pcARCFont->DrawOut(pcDC, sx, sy, wd, hi, fontrag);
}

void CMojiText::DataChangeARCFont()
{
	char	filename[RECMAX];

	mojienvgetfilename(m_szFontName, filename);
	m_pcARCFont->FontNameSet(filename, m_nFontType);
	m_pcARCFont->SetTateYoko(m_nTateYoko);
	m_pcARCFont->SF1TrnsDataLoad(m_nCode);
}

// ESF0書体関係関数

int CMojiText::DrawOutMemDCESF0Font()
{
	DBL		wd, hi;
	DBL		sx, sy;
	DBL		fontrag;
	int		ret;

	wd = FPROPSIZE; hi = FPROPSIZE;
	sx = FPROPSIZE/2; sy = FPROPSIZE;
	if(m_nTateYoko == TATEGAKI){
		wd = FPROPSIZE; hi = FPROPSIZE;
		sx = FPROPSIZE+FPROPSIZE/2; sy = 0;
		fontrag = 0;
	}
	else if(m_nTateYoko == YOKOGAKI){
		fontrag = 0;
	}
	else if(m_nTateYoko == YOKOOKI){
		fontrag = 0;
	}
	else if(m_nTateYoko == TATEOKI){
		fontrag = 0;
	}
	ret = DrawOutESF0Font(m_pcFSF1Font->m_pcMemDC, sx, sy, wd, hi, fontrag);
	return(ret);
}

int CMojiText::DrawOutESF0Font(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL fontrag)
{
	char	filename[RECMAX];
	int		ret;

	mojienvgetfilename(m_szFontName, filename);
	if(filename[0] == '\0'){
		return(FALSE);
	}
	m_pcESF0Font->FontNameSet(filename);
	m_pcESF0Font->SetTateYoko(m_nTateYoko);
	if(m_nTateYoko == TATEGAKI){
		if(IsZenkaku()){
			fontrag -= M_PI_2;
		}
		sx = sx-wd;
	}
	else if(m_nTateYoko == YOKOOKI){
		if(IsZenkaku()){
			fontrag -= M_PI_2;
		}
		sy = sy-hi;
	}
	else{
		sy = sy-hi;
	}
	m_pcESF0Font->SetBold(m_nBold);
	m_pcESF0Font->SetSlope(m_nSlope);
	ret = m_pcESF0Font->DataLoad(m_nCode);
	if(ret == FAIL){
		return(FALSE);
	}
	// 文字の左上が文字の始点
	m_pcESF0Font->DrawOut(pcDC, sx, sy, wd, hi, fontrag);
	return(TRUE);
}

int CMojiText::DataChangeESF0Font()
{
	char	filename[RECMAX];
	int		ret;

	mojienvgetfilename(m_szFontName, filename);
	if(filename[0] == '\0'){
		return(FALSE);
	}
	m_pcESF0Font->FontNameSet(filename);
	m_pcESF0Font->SetTateYoko(m_nTateYoko);
	ret = m_pcESF0Font->SF1TrnsDataLoad(m_nCode);
	if(ret == FAIL){
		return(FALSE);
	}
	return(TRUE);
}

// ESF1書体関係関数

int CMojiText::DrawOutMemDCESF1Font()
{
	DBL		wd, hi;
	DBL		sx, sy;
	DBL		fontrag;
	int		ret;

	wd = FPROPSIZE; hi = FPROPSIZE;
	sx = FPROPSIZE/2; sy = FPROPSIZE;
	if(m_nTateYoko == TATEGAKI){
		wd = FPROPSIZE; hi = FPROPSIZE;
		sx = FPROPSIZE+FPROPSIZE/2; sy = 0;
		fontrag = 0;
	}
	else if(m_nTateYoko == YOKOGAKI){
		fontrag = 0;
	}
	else if(m_nTateYoko == YOKOOKI){
		fontrag = 0;
	}
	else if(m_nTateYoko == TATEOKI){
		fontrag = 0;
	}
	ret = DrawOutESF1Font(m_pcFSF1Font->m_pcMemDC, sx, sy, wd, hi, fontrag);
	return(ret);
}

int CMojiText::DrawOutESF1Font(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL fontrag)
{
	char	filename[RECMAX];
	int		ret;

	switch(m_nFontType){
	case TT1FONT:
	case TT2FONT:
	case TT3FONT:
	case TT4FONT:
		mojienvgetsfilename(m_szFontName, filename);
		if(filename[0] == '\0'){
			return(FALSE);
		}
		GetGaijiFileName(filename);
		m_pcESF1Font->FontNameSet(filename);
		break;
	default:
		mojienvgetfilename(m_szFontName, filename);
		if(filename[0] == '\0'){
			return(FALSE);
		}
		m_pcESF1Font->FontNameSet(filename);
		break;
	}

	m_pcESF1Font->SetTateYoko(m_nTateYoko);
	if(m_nTateYoko == TATEGAKI){
		if(IsZenkaku()){
			fontrag -= M_PI_2;
		}
		sx = sx-wd;
	}
	else if(m_nTateYoko == YOKOOKI){
		if(IsZenkaku()){
			fontrag -= M_PI_2;
		}
		sy = sy-hi;
	}
	else{
		sy = sy-hi;
	}
	m_pcESF1Font->SetBold(m_nBold);
	m_pcESF1Font->SetSlope(m_nSlope);
	ret = m_pcESF1Font->DataLoad(m_nCode);
	if(ret == FAIL){
		return(FALSE);
	}
	// 文字の左上が文字の始点
	m_pcESF1Font->DrawOut(pcDC, sx, sy, wd, hi, fontrag);
	return(TRUE);
}

int CMojiText::DataChangeESF1Font()
{
	char	filename[RECMAX];
	int		ret;

	switch(m_nFontType){
	case TT1FONT:
	case TT2FONT:
	case TT3FONT:
	case TT4FONT:
		mojienvgetsfilename(m_szFontName, filename);
		if(filename[0] == '\0'){
			return(FALSE);
		}
		GetGaijiFileName(filename);
		m_pcESF1Font->FontNameSet(filename);
		break;
	default:
		mojienvgetfilename(m_szFontName, filename);
		if(filename[0] == '\0'){
			return(FALSE);
		}
		m_pcESF1Font->FontNameSet(filename);
		break;
	}
	m_pcESF1Font->SetTateYoko(m_nTateYoko);
	ret = m_pcESF1Font->SF1TrnsDataLoad(m_nCode);
	if(ret == FAIL){
		return(FALSE);
	}
	return(TRUE);
}

void CMojiText::GetGaijiFileName(LPSTR sGJFaceName)
{
	int		idx;
	int		hc;

	idx = 0;
	while(1){
		hc = (sGJFaceName[idx]) & 0x00ff;
		if(baseiskanji(hc) == ON){
			if(8 < (idx + 2)){
				break;
			}
			idx += 2;
		}
		else{
			if(8 < (idx+1)){
				break;
			}
			idx++;
		}
	}
	sGJFaceName[idx] = '\0';
}
