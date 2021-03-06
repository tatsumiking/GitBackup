//
// Easy Scanner System
//
// object sptnvect.cpp  線上文字列関係関数
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"

#include "Data.h"
#include "Vect.h"
#include "MojiVect.h"
#include "StrgVect.h"
#include "SPtnVect.h"

#include	"../ObjLib/Senvlib.h"
#include	"../ObjLib/Scrnlib.h"
#include	"../ObjLib/Pdrwlib.h"
#include	"../ObjLib/Curvelib.h"

IMPLEMENT_SERIAL(CSPtnVect, CObject, 3)
CSPtnVect::CSPtnVect()
{
}

CSPtnVect::CSPtnVect(int nDummy)
	: CStrgVect(nDummy)
{
	m_nSpaceKind = OFF;
	m_nRoundKind = ON;
	m_nSetMode = CNTRCNTR;
	m_dPitch = 1.0;
	m_dFlatPar = 100.0;
	m_dThickPar = 100.0;
	m_dSlopRag = 0;
	m_nOder = ODEREQUAL;
	m_pcFitingVect = NULL;
}

CSPtnVect::~CSPtnVect()
{
	if(m_pcFitingVect != NULL){
		delete(m_pcFitingVect);
	}
}

int CSPtnVect::CheckID()
{
	return(SPTNVECTID);
}

void CSPtnVect::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nSpaceKind;
		ar << m_nRoundKind;
		ar << m_nSetMode;
		ar << m_dPitch;
		ar << m_dFlatPar;
		ar << m_dThickPar;
		ar << m_dSlopRag;
		ar << m_nOder;
	}
	else
	{
		ar >> m_nSpaceKind;
		ar >> m_nRoundKind;
		ar >> m_nSetMode;
		ar >> m_dPitch;
		ar >> m_dFlatPar;
		ar >> m_dThickPar;
		ar >> m_dSlopRag;
		ar >> m_nOder;
		m_pcFitingVect = (CVect*)NewDataCreate(BASEVECTID);
	}
	m_pcFitingVect->Serialize(ar);
	CStrgVect::Serialize(ar);
	if (!(ar.IsStoring()))
	{
		RenewMiniMax();
	}
}

//p 内部データーが空の状態であれば削除しすべてが空のときTRUEを返す
int CSPtnVect::CheckNullData()
{
	// 文字列を正規化する
	CStrgVect::CheckNullData();
	if(m_pcFitingVect == NULL){
		return(TRUE);
	}
	if(m_pcFitingVect != NULL){
		if(m_pcFitingVect->CheckNullData() == TRUE){
			delete(m_pcFitingVect);
			m_pcFitingVect = NULL;
			return(TRUE);
		}
	}
	return(FALSE);
}

void CSPtnVect::SaveStrgStoreFile(FILE * fp, int id)
{
	TCHAR	strg[RECMAX];

	if(id == CheckID()){
		memset(strg, _T(' '), RECMAX);
		GetString(strg);
		strg[RECMAX-1] = 0x0a;
		strg[RECMAX-2] = 0x0d;
		fwrite(strg, RECMAX, 1, fp);
	}
}

void CSPtnVect::LoadStrgStoreFile(FILE * fp, int id)
{
	TCHAR	strg[RECMAX];

	if(id == CheckID()){
		if(fread(strg, RECMAX, 1, fp) != 1){
			return;
		}
		if(_tcscmp(strg, m_sString) != 0){
			SetString(strg);
			StrgLineStruct();
			RenewMiniMax();
		}
	}
}

void CSPtnVect::GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	DBL			tminix, tminiy, tmaxx, tmaxy;

	(*minix) = INTMAX;
	(*miniy) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	tminix = INTMAX;
	tminiy = INTMAX;
	tmaxx = -INTMAX;
	tmaxy = -INTMAX;
	CStrgVect::GetDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
	(*minix) = min((*minix), tminix);
	(*miniy) = min((*miniy), tminiy);
	(*maxx) = max((*maxx), tmaxx);
	(*maxy) = max((*maxy), tmaxy);
	tminix = INTMAX;
	tminiy = INTMAX;
	tmaxx = -INTMAX;
	tmaxy = -INTMAX;
	m_pcFitingVect->GetDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
	(*minix) = min((*minix), tminix);
	(*miniy) = min((*miniy), tminiy);
	(*maxx) = max((*maxx), tmaxx);
	(*maxy) = max((*maxy), tmaxy);
}

void CSPtnVect::GetStringDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	DBL			tminix, tminiy, tmaxx, tmaxy;

	(*minix) = INTMAX;
	(*miniy) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	tminix = INTMAX;
	tminiy = INTMAX;
	tmaxx = -INTMAX;
	tmaxy = -INTMAX;
	CStrgVect::GetDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
	(*minix) = min((*minix), tminix);
	(*miniy) = min((*miniy), tminiy);
	(*maxx) = max((*maxx), tmaxx);
	(*maxy) = max((*maxy), tmaxy);
}

void CSPtnVect::RenewMiniMax()
{
	CStrgVect::RenewMiniMax();
	m_pcFitingVect->RenewMiniMax();
}

CData* CSPtnVect::CopyData(DBL movex, DBL movey)
{
	CSPtnVect *pcnewData;
	CVect* pcnewVect;

	pcnewData = (CSPtnVect*)NewDataCreate(SPTNVECTID);
	pcnewVect = (CVect*)m_pcFitingVect->CopyData(movex, movey);
	pcnewData->SetFitingVect(pcnewVect);

	CopyString(pcnewData, movex, movey);
	
	pcnewData->SetSpaceKind(m_nSpaceKind);
	pcnewData->SetRoundKind(m_nRoundKind);
	pcnewData->SetSetMode(m_nSetMode);
	pcnewData->SetPitch(m_dPitch);
	pcnewData->SetFlatPar(m_dFlatPar);
	pcnewData->SetThickPar(m_dThickPar);
	pcnewData->SetSlopRag(m_dSlopRag);
	pcnewData->SetOderKind(m_nOder);
	pcnewData->StrgLineStruct();
	pcnewData->RenewMiniMax();
	return((CData*)pcnewData);
}

void CSPtnVect::CopyString(CSPtnVect * pcnewData, DBL movex, DBL movey)
{
	DBL		dstartx, dstarty;
	DBL		dwidth, dhight;
	DBL		d_tcslen;
	DBL		dmojispace, dstrgspace;

	pcnewData->BaseFontLink(m_pcFSF1Font);
	pcnewData->WNTTFontLink(m_pcWNTTFont);
	pcnewData->ESF0FontLink(m_pcESF0Font);
	pcnewData->ESF1FontLink(m_pcESF1Font);
	pcnewData->FBU0FontLink(m_pcFBU0Font);
	pcnewData->ARCFontLink(m_pcARCFont);
	pcnewData->ARKFontLink(m_pcARKFont);
	pcnewData->SCRFontLink(m_pcSCRFont);

	GetSpaceEnv(&dmojispace, &dstrgspace);
	GetStrStartXY(&dstartx, &dstarty);
	GetFontSize(&dwidth, &dhight);
	GetStrLen(&d_tcslen);

	pcnewData->SetSpaceEnv(dmojispace, dstrgspace);
	pcnewData->SetStrStartXY(dstartx+movex, dstarty+movey);
	pcnewData->SetFontSize(dwidth, dhight);
	pcnewData->SetStrLen(d_tcslen);
	pcnewData->SetTateYoko(CheckTateYoko());
	pcnewData->SetFontName(m_sFontName);
	pcnewData->SetString(m_sString);
	pcnewData->SetFillPen(m_nFillPen);
	pcnewData->SetOutLinePen(m_nOutLinePen);
	pcnewData->SetOutLineWidth(m_dOutLineWidth);
	pcnewData->RenewMiniMax();
}

void CSPtnVect::AllFree()
{
	delete(m_pcFitingVect);
	CStrgVect::AllFree();
}

void CSPtnVect::OnDraw(CDC *pcDC)
{
	int		mode;

	if(senvchckstopflag() == ON){
		return;
	}
	mode = senvcheckdrawmode(); 
	if(mode != PREVIEW
	&& mode != BLACKVIEW
	&& mode != PRINTVIEW
	&& mode != DIRCTVIEW
	&& mode != METAFVIEW){
		m_pcFitingVect->PlgnDraw(pcDC);
	}
	else if(CheckFlag(UNDERLINEFLAG) == ON){
		m_pcFitingVect->PlgnDraw(pcDC);
	}
	if(m_pcMojiTop != NULL){
		CStrgVect::OnDraw(pcDC);
	}
}

void CSPtnVect::OutlineDraw(CDC *pcDC)
{
	CStrgVect::OutlineDraw(pcDC);
}

void CSPtnVect::AtntionDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	m_pcFitingVect->AtntionDraw(pcDC);
	//StrgVect::AtntionDraw(pcDC);
}

void CSPtnVect::NoactDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	m_pcFitingVect->NoactDraw(pcDC);
	CStrgVect::NoactDraw(pcDC);
}

void CSPtnVect::DelDraw(CDC *pcDC)
{
	m_pcFitingVect->DelDraw(pcDC);
	CStrgVect::DelDraw(pcDC);
}

void CSPtnVect::MoveTrns(DBL movex, DBL movey)
{
	m_pcFitingVect->MoveTrns(movex, movey);
	CStrgVect::MoveTrns(movex, movey);
}

void CSPtnVect::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	m_pcFitingVect->SizeTrns(cntrx, cntry, timex, timey);
	CStrgVect::SizeTrns(cntrx, cntry, timex, timey);
}

void CSPtnVect::SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
{
	//m_pcFitingVect->SlopTrns(cntrx, cntry, ddx, ddy);
	CStrgVect::SlopTrns(cntrx, cntry, ddx, ddy);
}

void CSPtnVect::MirrorTrns(DBL a, DBL b, DBL t1, DBL t2)
{
	m_pcFitingVect->MirrorTrns(a, b, t1, t2);
	CStrgVect::MirrorTrns(a, b, t1, t2);
}

void CSPtnVect::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	m_pcFitingVect->RoundTrns(cntrx, cntry, tsin, tcos);
	CStrgVect::RoundTrns(cntrx, cntry, tsin, tcos);
}

void CSPtnVect::StrctBoxsTrns()
{
	m_pcFitingVect->StrctBoxsTrns();
	CStrgVect::StrctBoxsTrns();
}

void CSPtnVect::AllOnPointFlag(int bitflag)
{
	m_pcFitingVect->AllOnPointFlag(bitflag);
	CStrgVect::AllOnPointFlag(bitflag);
}

void CSPtnVect::AllOffPointFlag(int bitflag)
{
	m_pcFitingVect->AllOffPointFlag(bitflag);
	CStrgVect::AllOffPointFlag(bitflag);
}

void CSPtnVect::PointXLegal(DBL xx)
{
	m_pcFitingVect->PointXLegal(xx);
	CStrgVect::PointXLegal(xx);
}

void CSPtnVect::PointYLegal(DBL yy)
{
	m_pcFitingVect->PointYLegal(yy);
	CStrgVect::PointYLegal(yy);
}

CData* CSPtnVect::SrchNearPoint(DBL xx, DBL yy, DBL *retlen, int *retpp)
{
	CData*	ret;

	*retlen = INTMAX;
	ret = m_pcFitingVect->SrchNearPoint(xx, yy, retlen, retpp);
	return(ret);
}

CVect* CSPtnVect::GetFitingVect()
{
	return(m_pcFitingVect);
}

void CSPtnVect::SetFitingVect(CVect* pcVect)
{
	m_pcFitingVect = pcVect;
	pcVect->SetFillPen(-1);
	pcVect->SetOutLinePen(-1);
	pcVect->SetOutLineWidth(0.0);
}

void CSPtnVect::GetSpaceKind(int *flag)
{
	*flag = m_nSpaceKind;
}

void CSPtnVect::SetSpaceKind(int flag)
{
	m_nSpaceKind = flag;
}

void CSPtnVect::GetRoundKind(int *flag)
{
	*flag = m_nRoundKind;
}

void CSPtnVect::SetRoundKind(int flag)
{
	m_nRoundKind = flag;
}

void CSPtnVect::GetSetMode(int *mode)
{
	*mode = m_nSetMode;
}

void CSPtnVect::SetSetMode(int mode)
{
	m_nSetMode = mode;
}

void CSPtnVect::GetPitch(LPDBL dpitch)
{
	*dpitch = m_dPitch;
}

void CSPtnVect::SetPitch(DBL dpitch)
{
	m_dPitch = dpitch;
}

void CSPtnVect::GetFlatPar(LPDBL dflatpar)
{
	*dflatpar = m_dFlatPar;
}

void CSPtnVect::SetFlatPar(DBL dflatpar)
{
	m_dFlatPar = dflatpar;
}

void CSPtnVect::GetThickPar(LPDBL dthickpar)
{
	*dthickpar = m_dThickPar;
}

void CSPtnVect::SetThickPar(DBL dthickpar)
{
	m_dThickPar = dthickpar;
}

void CSPtnVect::GetSlopRag(LPDBL dsloprag)
{
	*dsloprag = m_dSlopRag;
}

void CSPtnVect::SetSlopRag(DBL dsloprag)
{
	m_dSlopRag = dsloprag;
}

void CSPtnVect::GetOderKind(int *noder)
{
	*noder = m_nOder;
}

void CSPtnVect::SetOderKind(int noder)
{
	m_nOder = noder;
}

void CSPtnVect::StrgFiting()
{
	TCHAR	szStr[RECMAX];

	_tcscpy_s(szStr, m_sString);
	SetString(szStr);
	StrgLineStruct();
}

void CSPtnVect::StrgLineStruct()
{
	DBL	dwidth, dhight;
	DBL	dthick;

	GetFontSize(&dwidth, &dhight);
	dthick = dhight * (m_dThickPar-100) / 1000;
	SetOutLineWidth(dthick);
	switch(m_nOder){
	case ODERLEFT:
		if(m_nSpaceKind == ON){
			SetLenStrgLineStructLeftData();
		}
		else{
			SetLenStrgLineStructLeftWaku();
		}
		break;
	case ODERCNTR:
		if(m_nSpaceKind == ON){
			SetLenStrgLineStructCntrData();
		}
		else{
			SetLenStrgLineStructCntrWaku();
		}
		break;
	case ODERRIGHT:
		if(m_nSpaceKind == ON){
			SetLenStrgLineStructRightData();
		}
		else{
			SetLenStrgLineStructRightWaku();
		}
		break;
	case ODEREQUAL:
	default:
		m_nOder = ODEREQUAL;
		if(m_nSpaceKind == ON){
			SetLenStrgLineStructSpace();
		}
		else{
			SetLenStrgLineStructEqual();
		}
		break;
	}
	StrgLineStructTrns();
}

void CSPtnVect::SetLenStrgLineStructLeftData()
{
	DBL		xsumlen, ysumlen;
	DBL		dwidth, dhight;
	DBL		setlen, setspace;
	DBL		tsetlen;
	DBL		space;
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;

	m_dLinelen = GetVectLength();
	GetDataSumLen(&xsumlen, &ysumlen);
	GetFontSize(&dwidth, &dhight);
	m_dHalffontsize = 0;
	m_dStartlen = 0;
	m_dTrnstime = 1;
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		setspace = dhight * (m_dPitch - 1.0);
	}
	else{
		setspace = dwidth * (m_dPitch - 1.0);
	}
	setlen = 0;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			tsetlen = setlen + (maxy - miniy) / 2;
		}
		else{
			tsetlen = setlen + (maxx - minix) / 2;
		}
		pcMojiVect->SetLenFromStrgTop(tsetlen);
		if(pcMojiVect->m_nZenKaku == ON){
			space = setspace;
		}
		else{
			space = setspace/2;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			setlen = setlen + (maxy - miniy) + space;
		}
		else{
			setlen = setlen + (maxx - minix) + space;
		}
	}
}

void CSPtnVect::SetLenStrgLineStructLeftWaku()
{
	DBL		xsumlen, ysumlen;
	DBL		dwidth, dhight;
	DBL		setlen, setspace;
	DBL		tsetlen;
	DBL		space;
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;

	m_dLinelen = GetVectLength();
	GetWakuSumLen(&xsumlen, &ysumlen);
	GetFontSize(&dwidth, &dhight);
	m_dHalffontsize = 0;
	m_dStartlen = 0;
	m_dTrnstime = 1;
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		setspace = dhight * (m_dPitch - 1.0);
	}
	else{
		setspace = dwidth * (m_dPitch - 1.0);
	}
	setlen = 0;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			tsetlen = setlen + (maxy - miniy) / 2;
		}
		else{
			tsetlen = setlen + (maxx - minix) / 2;
		}
		pcMojiVect->SetLenFromStrgTop(tsetlen);
		if(pcMojiVect->m_nZenKaku == ON){
			space = setspace;
		}
		else{
			space = setspace/2;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			setlen = setlen + (maxy - miniy) + space;
		}
		else{
			setlen = setlen + (maxx - minix) + space;
		}
	}
}

void CSPtnVect::SetLenStrgLineStructCntrData()
{
	DBL		xsumlen, ysumlen;
	DBL		dwidth, dhight;
	DBL		setlen, setspace;
	DBL		tsetlen;
	DBL		sumspace, space;
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;
	int		zen, han;

	m_dLinelen = GetVectLength();
	GetDataSumLen(&xsumlen, &ysumlen);
	GetFontSize(&dwidth, &dhight);
	m_dHalffontsize = 0;
	m_dStartlen = 0;
	m_dTrnstime = 1;
	GetSpaceMojiCount(&zen, &han);	
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		setspace = dhight * (m_dPitch - 1.0);
		sumspace = zen * setspace + han * setspace / 2;
		m_dStartlen = (m_dLinelen - (ysumlen + sumspace)) / 2;
	}
	else{
		setspace = dwidth * (m_dPitch - 1.0);
		sumspace = zen * setspace + han * setspace / 2;
		m_dStartlen = (m_dLinelen - (xsumlen + sumspace)) / 2;
	}
	setlen = 0;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			tsetlen = setlen + (maxy - miniy) / 2;
		}
		else{
			tsetlen = setlen + (maxx - minix) / 2;
		}
		pcMojiVect->SetLenFromStrgTop(tsetlen);
		if(pcMojiVect->m_nZenKaku == ON){
			space = setspace;
		}
		else{
			space = setspace/2;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			setlen = setlen + (maxy - miniy) + space;
		}
		else{
			setlen = setlen + (maxx - minix) + space;
		}
	}
}

void CSPtnVect::SetLenStrgLineStructCntrWaku()
{
	DBL		xsumlen, ysumlen;
	DBL		dwidth, dhight;
	DBL		setlen, setspace;
	DBL		tsetlen;
	DBL		sumspace, space;
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;
	int		zen, han;

	m_dLinelen = GetVectLength();
	GetWakuSumLen(&xsumlen, &ysumlen);
	GetFontSize(&dwidth, &dhight);
	m_dHalffontsize = 0;
	m_dStartlen = 0;
	m_dTrnstime = 1;
	GetSpaceMojiCount(&zen, &han);	
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		setspace = dhight * (m_dPitch - 1.0);
		sumspace = zen * setspace + han * setspace / 2;
		m_dStartlen = (m_dLinelen - (ysumlen + sumspace)) / 2;
	}
	else{
		setspace = dwidth * (m_dPitch - 1.0);
		sumspace = zen * setspace + han * setspace / 2;
		m_dStartlen = (m_dLinelen - (xsumlen + sumspace)) / 2;
	}
	setlen = 0;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			tsetlen = setlen + (maxy - miniy) / 2;
		}
		else{
			tsetlen = setlen + (maxx - minix) / 2;
		}
		pcMojiVect->SetLenFromStrgTop(tsetlen);
		if(pcMojiVect->m_nZenKaku == ON){
			space = setspace;
		}
		else{
			space = setspace/2;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			setlen = setlen + (maxy - miniy) + space;
		}
		else{
			setlen = setlen + (maxx - minix) + space;
		}
	}
}

void CSPtnVect::SetLenStrgLineStructRightData()
{
	DBL		xsumlen, ysumlen;
	DBL		dwidth, dhight;
	DBL		setlen, setspace;
	DBL		tsetlen;
	DBL		sumspace, space;
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;
	int		zen, han;

	m_dLinelen = GetVectLength();
	GetDataSumLen(&xsumlen, &ysumlen);
	GetFontSize(&dwidth, &dhight);
	m_dHalffontsize = 0;
	m_dStartlen = 0;
	m_dTrnstime = 1;
	GetSpaceMojiCount(&zen, &han);	
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		setspace = dhight * (m_dPitch - 1.0);
		sumspace = zen * setspace + han * setspace / 2;
		m_dStartlen = m_dLinelen - (ysumlen + sumspace);
	}
	else{
		setspace = dwidth * (m_dPitch - 1.0);
		sumspace = zen * setspace + han * setspace / 2;
		m_dStartlen = m_dLinelen - (xsumlen + sumspace);
	}
	setlen = 0;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			tsetlen = setlen + (maxy - miniy) / 2;
		}
		else{
			tsetlen = setlen + (maxx - minix) / 2;
		}
		pcMojiVect->SetLenFromStrgTop(setlen);
		if(pcMojiVect->m_nZenKaku == ON){
			space = setspace;
		}
		else{
			space = setspace/2;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			setlen = setlen + (maxy - miniy) + space;
		}
		else{
			setlen = setlen + (maxx - minix) + space;
		}
	}
}

void CSPtnVect::SetLenStrgLineStructRightWaku()
{
	DBL		xsumlen, ysumlen;
	DBL		dwidth, dhight;
	DBL		setlen, setspace;
	DBL		tsetlen;
	DBL		sumspace, space;
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;
	int		zen, han;

	m_dLinelen = GetVectLength();
	GetWakuSumLen(&xsumlen, &ysumlen);
	GetFontSize(&dwidth, &dhight);
	m_dHalffontsize = 0;
	m_dStartlen = 0;
	m_dTrnstime = 1;
	GetSpaceMojiCount(&zen, &han);	
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		setspace = dhight * (m_dPitch - 1.0);
		sumspace = zen * setspace + han * setspace / 2;
		m_dStartlen = m_dLinelen - (ysumlen + sumspace);
	}
	else{
		setspace = dwidth * (m_dPitch - 1.0);
		sumspace = zen * setspace + han * setspace / 2;
		m_dStartlen = m_dLinelen - (xsumlen + sumspace);
	}
	setlen = 0;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			tsetlen = setlen + (maxy - miniy) / 2;
		}
		else{
			tsetlen = setlen + (maxx - minix) / 2;
		}
		pcMojiVect->SetLenFromStrgTop(tsetlen);
		if(pcMojiVect->m_nZenKaku == ON){
			space = setspace;
		}
		else{
			space = setspace/2;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			setlen = setlen + (maxy - miniy) + space;
		}
		else{
			setlen = setlen + (maxx - minix) + space;
		}
	}
}

void CSPtnVect::SetLenStrgLineStructSpace()
{
	CMojiVect*	pcMojiVect;
	DBL		tmplinelen;
	DBL		dmojispace, dstrgspace;
	DBL		dwidth, dhight;
	DBL		sminix, sminiy, smaxx, smaxy;
	DBL		mminix, mminiy, mmaxx, mmaxy;
	DBL		setlen;

	GetWakuDMiniMax(&sminix, &sminiy, &smaxx, &smaxy);
	GetSpaceEnv(&dmojispace, &dstrgspace);
	GetFontSize(&dwidth, &dhight);
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		m_dStrglen = smaxy - sminiy;
		m_dHalffontsize = dhight / 2;
	}	
	else{
		m_dStrglen = smaxx - sminix;
		m_dHalffontsize = dwidth / 2;
	}

	m_dLinelen = GetVectLength();
	if(m_pcFitingVect->CheckFlag(REJIONFLAG) == ON
	&& m_pcFitingVect->GetPointCount() != 2){
		m_dStrglen = m_dStrglen+dmojispace;
		m_dStartlen = 0;
		m_dTrnstime = m_dLinelen / m_dStrglen;
	}
	else{
		//文字の中心までの長さをひく
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			m_dStrglen = m_dStrglen - dhight;
			tmplinelen = m_dLinelen - dhight;
		}
		else{
			m_dStrglen = m_dStrglen - dwidth;
			tmplinelen = m_dLinelen - dwidth;
		}
		m_dStartlen = 0;
		m_dTrnstime = tmplinelen / m_dStrglen;
	}

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->GetWakuDMiniMax(&mminix, &mminiy, &mmaxx, &mmaxy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			setlen = (mmaxy + mminiy) / 2 - (sminiy + m_dHalffontsize);
		}
		else{
			setlen = (mmaxx + mminix) / 2 - (sminix + m_dHalffontsize);
		}
		pcMojiVect->SetLenFromStrgTop(setlen);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CSPtnVect::SetLenStrgLineStructEqual()
{
	CMojiVect*	pcMojiVect;
	DBL	mminix, mminiy, mmaxx, mmaxy;
	DBL	dwd, dhi;
	DBL	setlen;
	DBL	tmplinelen;

	m_dStrglen = 0;
	pcMojiVect = m_pcMojiTop;
	if(pcMojiVect == NULL){
		return;
	}
	pcMojiVect->GetWakuDMiniMax(&mminix, &mminiy, &mmaxx, &mmaxy);
	m_dHalffontsize = (mmaxx - mminix)/2;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->GetFontSize(&dwd, &dhi);
		setlen = m_dStrglen;
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			m_dStrglen += dhi;
		}
		else{
			m_dStrglen += dwd;
		}
		pcMojiVect->SetLenFromStrgTop(setlen);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}

	m_dLinelen = GetVectLength();
	if(m_pcFitingVect->CheckFlag(REJIONFLAG) == ON
	&& m_pcFitingVect->GetPointCount() != 2){
		m_dStartlen = 0;
		m_dTrnstime = m_dLinelen / m_dStrglen;
	}
	else{
		//文字の中心までの長さをひく
		pcMojiVect = m_pcMojiTop;
		pcMojiVect->GetFontSize(&dwd, &dhi);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			m_dStrglen = m_dStrglen - dhi;
			tmplinelen = m_dLinelen - dhi;
		}
		else{
			m_dStrglen = m_dStrglen - dwd;
			tmplinelen = m_dLinelen - dwd;
		}
		m_dStartlen = 0;
		m_dTrnstime = tmplinelen / m_dStrglen;
	}
}

void CSPtnVect::StrgLineStructTrns()
{
	CMojiVect*	pcMojiVect;
	DBL		setlen;
	DBL		mminix, mminiy, mmaxx, mmaxy;
	DBL		cx, cy;
	DBL		movex, movey;
	DBL		tsin, tcos;
	DBL		rag, dx, dy;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		setlen = pcMojiVect->GetLenFromStrgTop();
		setlen = setlen*m_dTrnstime + m_dHalffontsize + m_dStartlen;
		if(m_pcFitingVect->CheckFlag(REJIONFLAG) == ON){
			if(m_dLinelen < setlen){
				setlen = setlen - m_dLinelen;
			}
		}
		pcMojiVect->GetWakuDMiniMax(&mminix, &mminiy, &mmaxx, &mmaxy);
		if(CheckTateYoko() == YOKOGAKI || CheckTateYoko() == TATEOKI){
			switch(m_nSetMode){
			case LEFTDOWN:
				cx = (mminix + mmaxx) / 2;
				cy = mminiy;
				break;
			case CNTRCNTR:
				cx = (mminix + mmaxx) / 2;
				cy = (mminiy + mmaxy) / 2;
				break;
			case RIGHTUP:
				cx = (mminix + mmaxx) / 2;
				cy = mmaxy;
				break;
			}
		}
		else{
			switch(m_nSetMode){
			case LEFTDOWN:
				cx = mminix;
				cy = (mminiy + mmaxy) / 2;
				break;
			case CNTRCNTR:
				cx = (mminix + mmaxx) / 2;
				cy = (mminiy + mmaxy) / 2;
				break;
			case RIGHTUP:
				cx = mmaxx;
				cy = (mminiy + mmaxy) / 2;
				break;
			}
		}
		rag = GetVectPointRagAndCood(&setlen, &dx, &dy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			rag += M_PI_2;
		}
		movex = dx - cx;
		movey = dy - cy;
		tsin = sin(rag);
		tcos = cos(rag);
		pcMojiVect->MoveTrns(movex, movey);
		if(m_nRoundKind == ON){
			pcMojiVect->RoundTrns(dx, dy, tsin, tcos);
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

DBL CSPtnVect::GetVectLength()
{
	int		pcnt;
	DBL		x1, y1, x2, y2;
	DBL		sumlen;

	sumlen = 0;
	pcnt = m_pcFitingVect->GetPointCount();
	if(pcnt <= 1){
		return(sumlen);
	}
	if(pcnt == 2){
		m_pcFitingVect->GetDXY(0, &x1, &y1);
		m_pcFitingVect->GetDXY(1, &x2, &y2);
		sumlen = GetLineLength(x1, y1, x2, y2);
		return(sumlen);
	}
	if(m_pcFitingVect->CheckFlag(REJIONFLAG) == ON){
		sumlen = GetRejionLength();
	}
	else{
		sumlen = GetNorejionLength();
	}
	return(sumlen);
}

DBL CSPtnVect::GetRejionLength()
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	pcnt = m_pcFitingVect->GetPointCount();
	for(pp = 0; pp < pcnt;){
		p0 = m_pcFitingVect->PointLegal(pcnt, pp-1);
		p1 = m_pcFitingVect->PointLegal(pcnt, pp+0);
		p2 = m_pcFitingVect->PointLegal(pcnt, pp+1);
		p3 = m_pcFitingVect->PointLegal(pcnt, pp+2);
		p4 = m_pcFitingVect->PointLegal(pcnt, pp+3);
		m_pcFitingVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		m_pcFitingVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		m_pcFitingVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		m_pcFitingVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		m_pcFitingVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				len = GetLineLength(x1, y1, x2, y2);
				sumlen += len;
				len = GetLineLength(x2, y2, x3, y3);
				sumlen += len;
			}
			else if(p2 == 0){
				len = GetLineLength(x1, y1, x2, y2);
				sumlen += len;
			}
			else{
				len = GetBezeLength(x1, y1, x2, y2, x3, y3, x4, y4);
				sumlen += len;
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				len = GetArcLength(x1, y1, x2, y2, x3, y3);
				sumlen += len;
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					len = GetArcLength(x1, y1, x2, y2, x3, y3);
					sumlen += len;
				}
				else{
					len = GetFreeCurveLength(atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
					sumlen += len;
				}
			}
			else{
				len = GetLineLength(x1, y1, x2, y2);
				sumlen += len;
			}
			pp += 1;
		}
	}
	return(sumlen);
}

DBL CSPtnVect::GetNorejionLength()
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		len;
	DBL		sumlen;

	pcnt = m_pcFitingVect->GetPointCount();

	sumlen = 0;
	for(pp = 0; pp < pcnt-1;){
		p0 = m_pcFitingVect->PointLegal(pcnt, pp-1);
		p1 = m_pcFitingVect->PointLegal(pcnt, pp+0);
		p2 = m_pcFitingVect->PointLegal(pcnt, pp+1);
		p3 = m_pcFitingVect->PointLegal(pcnt, pp+2);
		p4 = m_pcFitingVect->PointLegal(pcnt, pp+3);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
		m_pcFitingVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		m_pcFitingVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		m_pcFitingVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		m_pcFitingVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		m_pcFitingVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				len = GetLineLength(x1, y1, x2, y2);
				sumlen += len;
				len = GetLineLength(x2, y2, x3, y3);
				sumlen += len;
			}
			else if(p3 == 0){
				len = GetLineLength(x1, y1, x2, y2);
				sumlen += len;
			}
			else{
				len = GetBezeLength(x1, y1, x2, y2, x3, y3, x4, y4);
				sumlen += len;
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				len = GetLineLength(x1, y1, x2, y2);
				sumlen += len;
			}
			else{
				len = GetArcLength(x1, y1, x2, y2, x3, y3);
				sumlen += len;
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				len = GetFreeCurveLength(atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				sumlen += len;
			}
			else{
				len = GetLineLength(x1, y1, x2, y2);
				sumlen += len;
			}
			pp += 1;
		}
	}
	return(sumlen);
}

DBL CSPtnVect::GetBezeLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL		t, t1, b1, b2, b3, b4;
	DBL		sx, sy, ex, ey;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	sx = x1; sy = y1;
	for(t = 0.1; t < 1.0; t += 0.1){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		ex = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ey = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		len = GetLineLength(sx, sy, ex, ey);
		sumlen += len;
		sx = ex; sy = ey;
	}
	ex = x4; ey = y4;
	len = GetLineLength(sx, sy, ex, ey);
	len = hypot(sx-ex, sy-ey);
	sumlen += len;
	return(sumlen);
}

DBL CSPtnVect::GetArcLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL		x0, y0, rag01, rag03, dr;
	int		stat;
	DBL		rag, steprag;
	DBL		sx, sy, ex, ey;
	int		count, idx;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat == FAIL){
		len = GetLineLength(x1, y1, x2, y2);
		sumlen += len;
		len = GetLineLength(x1, y1, x2, y2);
		sumlen += len;
		return(sumlen);
	}
	stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &rag01, &rag03, &dr);
	if(stat == FAIL){
		len = GetLineLength(x1, y1, x2, y2);
		sumlen += len;
		len = GetLineLength(x1, y1, x2, y2);
		sumlen += len;
		return(sumlen);
	}

	count = (int)(fabs(rag03 - rag01) / M_5RAG);
	if(count < 2){
		count = 2;
	}
	steprag = (rag03 - rag01) / (DBL)count;

	sx = x1; sy = y1;
	for(idx = 1, rag = rag01+steprag; idx < count; idx++, rag += steprag){
		ex = cos(rag)*dr+x0;
		ey = sin(rag)*dr+y0;
		len = GetLineLength(sx, sy, ex, ey);
		sumlen += len;
		sx = ex; sy = ey;
	}
	ex = x3; ey = y3;
	len = GetLineLength(sx, sy, ex, ey);
	sumlen += len;
	return(sumlen);
}

DBL CSPtnVect::GetFreeCurveLength(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	rag1, rag2;
	DBL	xp1, yp1, xp2, yp2;
	DBL	len;

	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);

	if(rag1 == NORAG && rag2 == NORAG){
		len = GetLineLength(x1, y1, x2, y2);
	}
	else{
		curvenoragtocurverag(&rag1, &rag2);
		curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
		curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
		len = GetBezeLength(x1, y1, xp1, yp1, xp2, yp2, x2, y2);
	}
	return(len);
}

DBL CSPtnVect::GetLineLength(DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		len;

	len = hypot(x1-x2, y1-y2);
	return(len);
}

// 点までの長さを求める
DBL CSPtnVect::GetVectPointLength(DBL xx, DBL yy)
{
	int		pcnt;
	int		flag;
	DBL		x1, y1, x2, y2;
	DBL		sumlen;

	flag = OFF;
	sumlen = 0;
	pcnt = m_pcFitingVect->GetPointCount();
	if(pcnt <= 1){
		return(sumlen);
	}
	if(pcnt == 2){
		m_pcFitingVect->GetDXY(0, &x1, &y1);
		m_pcFitingVect->GetDXY(1, &x2, &y2);
		sumlen = GetLinePointLength(&flag, xx, yy, x1, y1, x2, y2);
		return(sumlen);
	}
	if(m_pcFitingVect->CheckFlag(REJIONFLAG) == ON){
		sumlen = GetRejionPointLength(&flag, xx, yy);
	}
	else{
		sumlen = GetNorejionPointLength(&flag, xx, yy);
	}
	return(sumlen);
}

DBL CSPtnVect::GetRejionPointLength(int *flag, DBL xx, DBL yy)
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	pcnt = m_pcFitingVect->GetPointCount();
	for(pp = 0; pp < pcnt;){
		p0 = m_pcFitingVect->PointLegal(pcnt, pp-1);
		p1 = m_pcFitingVect->PointLegal(pcnt, pp+0);
		p2 = m_pcFitingVect->PointLegal(pcnt, pp+1);
		p3 = m_pcFitingVect->PointLegal(pcnt, pp+2);
		p4 = m_pcFitingVect->PointLegal(pcnt, pp+3);
		m_pcFitingVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		m_pcFitingVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		m_pcFitingVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		m_pcFitingVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		m_pcFitingVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
				len = GetLinePointLength(flag, xx, yy, x2, y2, x3, y3);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else if(p2 == 0){
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else{
				len = GetBezePointLength(flag, xx, yy, x1, y1, x2, y2, x3, y3, x4, y4);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				len = GetArcPointLength(flag, xx, yy, x1, y1, x2, y2, x3, y3);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					len = GetArcPointLength(flag, xx, yy, x1, y1, x2, y2, x3, y3);
					sumlen += len;
					if(*flag == ON){
						return(sumlen);
					}
				}
				else{
					len = GetFreeCurvePointLength(flag, xx, yy, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
					sumlen += len;
					if(*flag == ON){
						return(sumlen);
					}
				}
			}
			else{
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 1;
		}
	}
	return(sumlen);
}

DBL CSPtnVect::GetNorejionPointLength(int *flag, DBL xx, DBL yy)
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	pcnt = m_pcFitingVect->GetPointCount();
	for(pp = 0; pp < pcnt-1;){
		p0 = m_pcFitingVect->PointLegal(pcnt, pp-1);
		p1 = m_pcFitingVect->PointLegal(pcnt, pp+0);
		p2 = m_pcFitingVect->PointLegal(pcnt, pp+1);
		p3 = m_pcFitingVect->PointLegal(pcnt, pp+2);
		p4 = m_pcFitingVect->PointLegal(pcnt, pp+3);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
		m_pcFitingVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		m_pcFitingVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		m_pcFitingVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		m_pcFitingVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		m_pcFitingVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
				len = GetLinePointLength(flag, xx, yy, x2, y2, x3, y3);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else if(p3 == 0){
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else{
				len = GetBezePointLength(flag, xx, yy, x1, y1, x2, y2, x3, y3, x4, y4);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else{
				len = GetArcPointLength(flag, xx, yy, x1, y1, x2, y2, x3, y3);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				len = GetFreeCurvePointLength(flag, xx, yy, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			else{
				len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
				sumlen += len;
				if(*flag == ON){
					return(sumlen);
				}
			}
			pp += 1;
		}
	}
	return(sumlen);
}

DBL CSPtnVect::GetBezePointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL		t, t1, b1, b2, b3, b4;
	DBL		sx, sy, ex, ey;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	sx = x1; sy = y1;
	for(t = 0.1; t < 1.0; t += 0.1){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		ex = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ey = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		len = GetLinePointLength(flag, xx, yy, sx, sy, ex, ey);
		sumlen += len;
		if(*flag == ON){
			return(sumlen);
		}
		sx = ex; sy = ey;
	}
	ex = x4; ey = y4;
	len = GetLinePointLength(flag, xx, yy, sx, sy, ex, ey);
	sumlen += len;
	return(sumlen);
}

DBL CSPtnVect::GetArcPointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL		x0, y0, rag01, rag03, dr;
	int		stat;
	DBL		rag, steprag;
	DBL		sx, sy, ex, ey;
	int		count, idx;
	DBL		len;
	DBL		sumlen;

	sumlen = 0;
	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat == FAIL){
		len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
		sumlen += len;
		if(*flag == ON){
			return(sumlen);
		}
		len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
		sumlen += len;
		return(sumlen);
	}
	stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &rag01, &rag03, &dr);
	if(stat == FAIL){
		len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
		sumlen += len;
		if(*flag == ON){
			return(sumlen);
		}
		len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
		sumlen += len;
		return(sumlen);
	}

	count = (int)(fabs(rag03 - rag01) / M_5RAG);
	if(count < 2){
		count = 2;
	}
	steprag = (rag03 - rag01) / (DBL)count;

	sx = x1; sy = y1;
	for(idx = 1, rag = rag01+steprag; idx < count; idx++, rag += steprag){
		ex = cos(rag)*dr+x0;
		ey = sin(rag)*dr+y0;
		len = GetLinePointLength(flag, xx, yy, sx, sy, ex, ey);
		sumlen += len;
		if(*flag == ON){
			return(sumlen);
		}
		sx = ex; sy = ey;
	}
	ex = x3; ey = y3;
	len = GetLinePointLength(flag, xx, yy, sx, sy, ex, ey);
	sumlen += len;
	return(sumlen);
}

DBL CSPtnVect::GetFreeCurvePointLength(int *flag, DBL xx, DBL yy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	rag1, rag2;
	DBL	xp1, yp1, xp2, yp2;
	DBL	len;

	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);

	if(rag1 == NORAG && rag2 == NORAG){
		len = GetLinePointLength(flag, xx, yy, x1, y1, x2, y2);
	}
	else{
		curvenoragtocurverag(&rag1, &rag2);
		curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
		curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
		len = GetBezePointLength(flag, xx, yy, x1, y1, xp1, yp1, xp2, yp2, x2, y2);
	}
	return(len);
}

DBL CSPtnVect::GetLinePointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		len;
	DBL		cx, cy;
	int		ret;
	DBL		tlen;

	*flag = OFF;
	len = hypot(x1-x2, y1-y2);
	ret = curveperpendicular(x1, y1, x2, y2, xx, yy, &cx, &cy);
	if(ret != FAIL){
		ret = curveonlinechk(x1, y1, x2, y2, cx, cy);
		if(ret == ON){
			tlen = hypot(xx - cx, yy - cy);
			if(tlen < 1.0){
				*flag = ON;
				len = hypot(x1-cx, y1-cy);
			}
		}
	}
	return(len);
}

// 開始点からの距離が０になった角度と位置を求める
DBL CSPtnVect::GetVectPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy)
{
	int		pcnt;
	DBL		rag;

	rag = 0;
	pcnt = m_pcFitingVect->GetPointCount();
	if(pcnt <= 1){
		return(rag);
	}
	if(pcnt == 2){
		rag = GetNorejionPointRagAndCood(setlen, dx, dy);
		return(rag);
	}
	if(m_pcFitingVect->CheckFlag(REJIONFLAG) == ON){
		rag = GetRejionPointRagAndCood(setlen, dx, dy);
	}
	else{
		rag = GetNorejionPointRagAndCood(setlen, dx, dy);
	}
	return(rag);
}

DBL CSPtnVect::GetRejionPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy)
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		rag;

	rag = 0;
	pcnt = m_pcFitingVect->GetPointCount();
	for(pp = 0; pp < pcnt;){
		p0 = m_pcFitingVect->PointLegal(pcnt, pp-1);
		p1 = m_pcFitingVect->PointLegal(pcnt, pp+0);
		p2 = m_pcFitingVect->PointLegal(pcnt, pp+1);
		p3 = m_pcFitingVect->PointLegal(pcnt, pp+2);
		p4 = m_pcFitingVect->PointLegal(pcnt, pp+3);
		m_pcFitingVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		m_pcFitingVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		m_pcFitingVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		m_pcFitingVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		m_pcFitingVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
				rag = GetLinePointRagAndCood(setlen, dx, dy, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else if(p2 == 0){
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else{
				rag = GetBezePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2, x3, y3, x4, y4);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				rag = GetArcPointRagAndCood(setlen, dx, dy, x1, y1, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					rag = GetArcPointRagAndCood(setlen, dx, dy, x1, y1, x2, y2, x3, y3);
					if((*setlen) <= 0){
						return(rag);
					}
				}
				else{
					rag = GetFreeCurvePointRagAndCood(setlen, dx, dy, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
					if((*setlen) <= 0){
						return(rag);
					}
				}
			}
			else{
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 1;
		}
	}
	return(rag);
}

DBL CSPtnVect::GetNorejionPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy)
{
	int		pcnt;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		rag, len;

	rag = 0;
	pcnt = m_pcFitingVect->GetPointCount();
	for(pp = 0; pp < pcnt-1;){
		p0 = m_pcFitingVect->PointLegal(pcnt, pp-1);
		p1 = m_pcFitingVect->PointLegal(pcnt, pp+0);
		p2 = m_pcFitingVect->PointLegal(pcnt, pp+1);
		p3 = m_pcFitingVect->PointLegal(pcnt, pp+2);
		p4 = m_pcFitingVect->PointLegal(pcnt, pp+3);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
		m_pcFitingVect->GetAtrDXY(p0, &atr0, &x0, &y0);
		m_pcFitingVect->GetAtrDXY(p1, &atr1, &x1, &y1);
		m_pcFitingVect->GetAtrDXY(p2, &atr2, &x2, &y2);
		m_pcFitingVect->GetAtrDXY(p3, &atr3, &x3, &y3);
		m_pcFitingVect->GetAtrDXY(p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
				rag = GetLinePointRagAndCood(setlen, dx, dy, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else if(p3 == 0){
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else{
				rag = GetBezePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2, x3, y3, x4, y4);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else{
				rag = GetArcPointRagAndCood(setlen, dx, dy, x1, y1, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				rag = GetFreeCurvePointRagAndCood(setlen, dx, dy, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			else{
				rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
				if((*setlen) <= 0){
					return(rag);
				}
			}
			pp += 1;
		}
	}
	// 最後の線分を延長したところの座標を求める
	if(0 < *setlen){
		x1 = m_dLastX1; y1 = m_dLastY1;
		x2 = m_dLastX2; y2 = m_dLastY2;
		len = hypot(x1-x2, y1-y2);
		(*dx) = (x2 - x1) * ((*setlen)+len) / len + x1;
		(*dy) = (y2 - y1) * ((*setlen)+len) / len + y1;
	}
	return(rag);
}

DBL CSPtnVect::GetBezePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL		t, t1, b1, b2, b3, b4;
	DBL		sx, sy, ex, ey;
	DBL		len;
	DBL		rag;

	rag = 0;
	sx = x1; sy = y1;
	for(t = 0.1; t < 1.0; t += 0.1){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		ex = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ey = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		rag = GetLinePointRagAndCood(setlen, dx, dy, sx, sy, ex, ey);
		if((*setlen) <= 0){
			return(rag);
		}
		sx = ex; sy = ey;
	}
	ex = x4; ey = y4;
	rag = GetLinePointRagAndCood(setlen, dx, dy, sx, sy, ex, ey);
	if((*setlen) <= 0){
		return(rag);
	}
	len = hypot(sx-ex, sy-ey);
	return(rag);
}

DBL CSPtnVect::GetArcPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL		x0, y0, rag01, rag03, dr;
	int		stat;
	DBL		rag, steprag;
	DBL		sx, sy, ex, ey;
	int		count, idx;
	DBL		retrag;

	rag = 0;
	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat == FAIL){
		rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
		rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
		return(rag);
	}
	stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &rag01, &rag03, &dr);
	if(stat == FAIL){
		rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
		rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
		return(rag);
	}

	count = (int)(fabs(rag03 - rag01) / M_5RAG);
	if(count < 2){
		count = 2;
	}
	steprag = (rag03 - rag01) / (DBL)count;

	sx = x1; sy = y1;
	for(idx = 1, rag = rag01+steprag; idx < count; idx++, rag += steprag){
		ex = cos(rag)*dr+x0;
		ey = sin(rag)*dr+y0;
		retrag = GetLinePointRagAndCood(setlen, dx, dy, sx, sy, ex, ey);
		if((*setlen) <= 0){
			return(retrag);
		}
		sx = ex; sy = ey;
	}
	ex = x3; ey = y3;
	retrag = GetLinePointRagAndCood(setlen, dx, dy, sx, sy, ex, ey);
	if((*setlen) <= 0){
		return(retrag);
	}
	return(retrag);
}

DBL CSPtnVect::GetFreeCurvePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	rag;
	DBL	rag1, rag2;
	DBL	xp1, yp1, xp2, yp2;

	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);

	if(rag1 == NORAG && rag2 == NORAG){
		rag = GetLinePointRagAndCood(setlen, dx, dy, x1, y1, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
	}
	else{
		curvenoragtocurverag(&rag1, &rag2);
		curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
		curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
		rag = GetBezePointRagAndCood(setlen, dx, dy, x1, y1, xp1, yp1, xp2, yp2, x2, y2);
		if((*setlen) <= 0){
			return(rag);
		}
	}
	return(rag);
}

DBL CSPtnVect::GetLinePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		len;
	DBL		rag;

	len = hypot(x1-x2, y1-y2);
	if(y2 == y1 && x2 == x1){
		rag = 0;
	}
	else{
		rag = atan2(y2-y1, x2-x1);
	}
	if((*setlen) <= len){
		(*dx) = (x2 - x1) * (*setlen) / len + x1;
		(*dy) = (y2 - y1) * (*setlen) / len + y1;
		(*setlen) = 0;
	}
	else{
		(*setlen) = (*setlen) - len;
	}
	m_dLastX1 = x1; m_dLastY1 = y1;
	m_dLastX2 = x2; m_dLastY2 = y2;
	return(rag);
}

void CSPtnVect::DeleteData(CData *delData)
{
	CVect	*pcVect;

	if(m_pcFitingVect == delData){
		pcVect = (CVect*)(m_pcFitingVect->CopyData(0, 0));
		m_pcFitingVect = pcVect;
		return;
	}
	CStrgVect::DeleteData(delData);
}

CData* CSPtnVect::SrchDataBoxs(DBL dx, DBL dy, DBL *retlen)
{
	DBL	minx, miny, maxx, maxy;
	DBL	xx, yy, len;
	int	pcnt, pp;
	DBL	dW, dH;	

	pcnt = m_pcFitingVect->GetPointCount();
	if(pcnt == 2){
		GetDMiniMax(&minx, &miny, &maxx, &maxy);
		*retlen = GetPreLengthBoxs(dx, dy, minx, miny, maxx, maxy);
	}
	else{
		m_pcFitingVect->SrchNearOnLinePoint(dx, dy, &len, &xx, &yy, &pp);
		GetFontSize(&dW, &dH);
		if(len < dH){
			*retlen = 0;
		}
		else{
			*retlen = len;
		}
	}
	return(this);
}
