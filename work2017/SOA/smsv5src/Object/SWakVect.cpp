//
// Easy Scanner System
//
// object swakvect.cpp  枠内配置文字列関係関数
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include <mbstring.h>

#pragma optimize( "", off )

#include "Data.h"
#include "Vect.h"
#include "DataList.h"
#include "MojiVect.h"
#include "StrgVect.h"
#include "SWakVect.h"

#include	"../ObjLib/Baselib.h"
#include	"../ObjLib/Senvlib.h"
#include	"../ObjLib/Scrnlib.h"
#include	"../ObjLib/Pdrwlib.h"
#include	"../ObjLib/Curvelib.h"

IMPLEMENT_SERIAL(CSWakVect, CObject, 3)
CSWakVect::CSWakVect()
{
}

CSWakVect::CSWakVect(int nDummy)
	: CStrgVect(nDummy)
{
	int ver = senvcheckverjion();
	m_nWakuNo = 0;
	m_nSpaceKind = SPACEWAKU;
	m_dKanaPar = 100.0;
	m_dSpacePar = 0;
	m_dFlatPar = MJFREEFLATPAR;
	m_dThickPar = 0.0;
	m_dSlopRag = 0;
	m_nOder = ODEREQUAL;
	m_pcFitingVect = NULL;
	m_nYokoGumi = OFF;
	m_nFixFlag = OFF;
	m_nMirror = 0;
	m_szFildName[0] = _T('\0');
}

CSWakVect::~CSWakVect()
{
	if(m_pcFitingVect != NULL){
		delete(m_pcFitingVect);
	}
}

int CSWakVect::CheckID()
{
	return(SWAKVECTID);
}

void CSWakVect::Serialize(CArchive& ar)
{
	int		ver;
	CString	csStr;

	if (ar.IsStoring())
	{
		ar << m_nWakuNo;
		ar << m_nSpaceKind;
		ar << m_dFlatPar;
		ar << m_dSpacePar;
		ar << m_dKanaPar;
		ar << m_dSlopRag;
		ar << m_nOder;
		ar << m_nNouse;
		ar << m_nFixFlag;
		ar << m_nMirror;
		csStr = m_szFildName;
		ar << csStr;
	}else{
		ver = senvcheckverjion();
		ar >> m_nWakuNo;
		ar >> m_nSpaceKind;
		ar >> m_dFlatPar;
		ar >> m_dSpacePar;
		ar >> m_dKanaPar;
		ar >> m_dSlopRag;
		ar >> m_nOder;
		ar >> m_nNouse;
		ar >> m_nFixFlag;
		ar >> m_nMirror;
		if(ver >= VER300){
			ar >> csStr;
			_tcscpy_s(m_szFildName, csStr);
		}
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
int CSWakVect::CheckNullData()
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

void CSWakVect::SaveStrgStoreFile(FILE * fp, int id)
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

void CSWakVect::LoadStrgStoreFile(FILE * fp, int id)
{
	TCHAR	strg[RECMAX];

	if(id == CheckID()){
		if(fread(strg, RECMAX, 1, fp) != 1){
			return;
		}
		if(m_nFixFlag == OFF){
			if(_tcscmp(strg, m_sString) != 0){
				SetString(strg);
				StrgLineStruct();
				RenewMiniMax();
			}
		}
	}
}

void CSWakVect::GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	DBL		sx, sy, ex, ey;

	if(m_pcFitingVect->GetPointCount() == 2){
		GetFitingBoxs(&sx, &sy, &ex, &ey);
		if(sx < ex){
			*minix = sx;
			*maxx = ex;
		}
		else{
			*minix = ex;
			*maxx = sx;
		}
		if(sy < ey){
			*miniy = sy;
			*maxy = ey;
		}
		else{
			*miniy = ey;
			*maxy = sy;
		}
	}
	else{
		m_pcFitingVect->GetDMiniMax(minix, miniy, maxx, maxy);
	}
}
void CSWakVect::GetStringDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
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

void CSWakVect::RenewMiniMax()
{
	CStrgVect::RenewMiniMax();
	m_pcFitingVect->RenewMiniMax();
}

CData* CSWakVect::CopyData(DBL movex, DBL movey)
{
	CSWakVect *pcnewData;
	CVect* pcnewVect;

	pcnewData = (CSWakVect*)NewDataCreate(SWAKVECTID);
	pcnewVect = (CVect*)m_pcFitingVect->CopyData(movex, movey);
	pcnewData->SetFitingVect(pcnewVect);

	CopyString(pcnewData, movex, movey);

	pcnewData->SetSpaceKind(m_nSpaceKind);
	pcnewData->SetSpacePar(m_dSpacePar);
	pcnewData->SetKanaPar(m_dKanaPar);
	pcnewData->SetFlatPar(m_dFlatPar);
	pcnewData->SetThickPar(m_dThickPar);
	pcnewData->SetSlopRag(m_dSlopRag);
	pcnewData->SetOderKind(m_nOder);

	pcnewData->SetString(m_sString);
	pcnewData->RenewMiniMax();

	pcnewData->RenewMiniMax();
	pcnewData->StrgLineStruct();
	pcnewData->RenewMiniMax();
	return((CData*)pcnewData);
}

void CSWakVect::CopyString(CSWakVect * pcnewData, DBL movex, DBL movey)
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
	pcnewData->SetStrLen(d_tcslen);
	pcnewData->m_nStrgFlag = m_nStrgFlag;
	pcnewData->m_nYokoGumi = m_nYokoGumi;
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		pcnewData->SetFontSize(dwidth, dwidth);
	}
	else{
		pcnewData->SetFontSize(dhight, dhight);
	}
	pcnewData->SetFontName(m_sFontName);
	pcnewData->SetFillPen(m_nFillPen);
	pcnewData->SetOutLinePen(m_nOutLinePen);
	pcnewData->SetOutLineWidth(m_dOutLineWidth);
}

void CSWakVect::AllFree()
{
	delete(m_pcFitingVect);
	CStrgVect::AllFree();
}

void CSWakVect::NoSelectDraw(CDC *pcDC, int no)
{
	if(m_nWakuNo == no){
		scrnsetfrpen(pcDC->m_hDC);
		WakuBoxsDraw(pcDC);
		scrnpenend(pcDC->m_hDC);
	}
}

void CSWakVect::SelectDraw(CDC *pcDC, int no)
{
	if(m_nWakuNo == no){
		scrnsetatnpen(pcDC->m_hDC);
		WakuBoxsDraw(pcDC);
		scrnpenend(pcDC->m_hDC);
	}
}

void CSWakVect::OnDraw(CDC *pcDC)
{
	int		mode;

	if(senvchckstopflag() == ON){
		return;
	}
	mode = senvcheckdrawmode();
	if(mode != BLACKVIEW
	&& mode != DIRCTVIEW
	&& mode != PRINTVIEW){
		if(CheckFlag(ACTIVEFLAG) == ON){
			scrnsetwakupen(pcDC->m_hDC);
		}
		else{
			scrnsetnoslctpen(pcDC->m_hDC);
		}
		WakuBoxsDraw(pcDC);
#ifdef _DEBUG
		m_pcFitingVect->PlgnDraw(pcDC);
#endif
		scrnpenend(pcDC->m_hDC);
	}
	if(m_pcMojiTop != NULL){
		CStrgVect::OnDraw(pcDC);
	}
}

void CSWakVect::SelectDraw(CDC *pcDC)
{
	scrnsetatnpen(pcDC->m_hDC);
	WakuBoxsDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CSWakVect::NoSelectDraw(CDC *pcDC)
{
	scrnsetslctpen(pcDC->m_hDC);
	WakuBoxsDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CSWakVect::WakuBoxsDraw(CDC *pcDC)
{
	DBL		sx, sy, ex, ey;

	if(m_pcFitingVect->GetPointCount() == 2){
		m_pcFitingVect->GetDXY(0, &sx, &sy);
		m_pcFitingVect->GetDXY(1, &ex, &ey);
		if(sx == ex || sy == ey){
			GetFitingBoxs(&sx, &sy, &ex, &ey);
			scrndxymmtorltv(&sx, &sy);
			scrndxymmtorltv(&ex, &ey);
			pdrwmove(pcDC->m_hDC, sx, sy);
			pdrwline(pcDC->m_hDC, sx, sy, sx, ey);
			pdrwline(pcDC->m_hDC, sx, ey, ex, ey);
			pdrwline(pcDC->m_hDC, ex, ey, ex, sy);
			pdrwline(pcDC->m_hDC, ex, sy, sx, sy);
			return;
		}
	}
	m_pcFitingVect->PlgnDraw(pcDC);
}

void CSWakVect::GetFitingBoxs(LPDBL sx, LPDBL sy, LPDBL ex, LPDBL ey)
{
	DBL		x1, y1, x2, y2;
	DBL		wd, hi;
	int		ftate;

	m_pcFitingVect->GetDXY(0, &x1, &y1);
	m_pcFitingVect->GetDXY(1, &x2, &y2);
	GetFontSize(&wd, &hi);
	ftate = CheckTateYoko();
	if(ftate == YOKOGAKI){
		*sx = x1;
		*sy = y1 + hi/2;
		*ex = x2;
		*ey = y2 - hi/2;
	}else if(ftate == TATEGAKI){
		*sx = x1 - wd/2;
		*sy = y1;
		*ex = x2 + wd/2;
		*ey = y2;
	}else if(ftate == TATEOKI){
		*sx = x1 - hi/2;
		*sy = y1;
		*ex = x2 + hi/2;
		*ey = y2;
	}
	else if(ftate == YOKOOKI){
		*sx = x1;
		*sy = y1 + wd/2;
		*ex = x2;
		*ey = y2 - wd/2;
	}
}

int CSWakVect::DrawNo(CDC *pcDC, int noidx)
{
	DBL		sx, sy;
	DBL		ex, ey;
	TCHAR	buf[32];
	int		ver, len;

	if(pcDC != NULL){
		if(CheckFlag(ACTIVEFLAG) == ON){
			scrnsetfrpen(pcDC->m_hDC);
		}
		else{
			scrnsetnoslctpen(pcDC->m_hDC);
		}
		ver = senvcheckverjion();
		m_nWakuNo = noidx;
		_stprintf_s(buf, 32, _T("%d"), m_nWakuNo);
		GetFitingBoxs(&sx, &sy, &ex, &ey);
		scrndxymmtorltv(&sx, &sy);
		len = _tcslen(buf);
		::TextOut(pcDC->m_hDC,(int)sx, (int)sy,  buf, len);
		scrnpenend(pcDC->m_hDC);
	}
	return(noidx);
}

void CSWakVect::OutlineDraw(CDC *pcDC)
{
	CStrgVect::OutlineDraw(pcDC);
}

void CSWakVect::AtntionDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	scrnsetslctpen(pcDC->m_hDC);
	WakuBoxsDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CSWakVect::NoactDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	scrnsetnoslctpen(pcDC->m_hDC);
	WakuBoxsDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
	CStrgVect::NoactDraw(pcDC);
}

void CSWakVect::DelDraw(CDC *pcDC)
{
	scrnsetdelpen(pcDC->m_hDC);
	WakuBoxsDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
	CStrgVect::DelDraw(pcDC);
}

void CSWakVect::MoveTrns(DBL movex, DBL movey)
{
	m_pcFitingVect->MoveTrns(movex, movey);
	CStrgVect::MoveTrns(movex, movey);
}

void CSWakVect::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	CMojiVect*	pcMojiVect;
	DBL			dW, dH;
	DBL			dM, dL;
	DBL			dSx, dSy;
	DBL			dSL;
	m_pcFitingVect->SizeTrns(cntrx, cntry, timex, timey);

	GetFontSize(&dW, &dH);
	GetStrLen(&dSL);
	GetSpaceEnv(&dM, &dL);
	if(CheckTateYoko() == TATEGAKI){
		dW = dW * timex;
		SetFontSize(dW, dW);
		dM = dM * timey;
		dL = dL * timex;
		dSL = dSL * timey;
	}
	else if(CheckTateYoko() == YOKOGAKI){
		dH = dH * timey;
		SetFontSize(dH, dH);
		dM = dM * timex;
		dL = dL * timey;
		dSL = dSL * timex;
	}
	else if(CheckTateYoko() == TATEOKI){
		dH = dH * timex;
		SetFontSize(dH, dH);
		dM = dM * timey;
		dL = dL * timex;
		dSL = dSL * timey;
	}
	else if(CheckTateYoko() == YOKOOKI){
		dW = dW * timey;
		SetFontSize(dW, dW);
		dM = dM * timex;
		dL = dL * timey;
		dSL = dSL * timex;
	}
	SetSpaceEnv(dM, dL);
	SetStrLen(dSL);

	GetStrStartXY(&dSx, &dSy);
	dSx = (dSx - cntrx) * timex + cntrx;
	dSy = (dSy - cntry) * timey + cntry;
	SetStrStartXY(dSx, dSy);
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->SizeTrns(cntrx, cntry, timex, timey);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	RenewMiniMax();

	//StringElementInit();
	//RenewMiniMax();
	//StrgLineStruct();
	//RenewMiniMax();

}

void CSWakVect::SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
{
	//m_pcFitingVect->SlopTrns(cntrx, cntry, ddx, ddy);
	CStrgVect::SlopTrns(cntrx, cntry, ddx, ddy);
}

void CSWakVect::MirrorTrns(DBL a, DBL b, DBL t1, DBL t2)
{
	m_pcFitingVect->MirrorTrns(a, b, t1, t2);
	CStrgVect::MirrorTrns(a, b, t1, t2);
}

void CSWakVect::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	m_pcFitingVect->RoundTrns(cntrx, cntry, tsin, tcos);
	CStrgVect::RoundTrns(cntrx, cntry, tsin, tcos);
}

void CSWakVect::StrctBoxsTrns()
{
	m_pcFitingVect->StrctBoxsTrns();
	CStrgVect::StrctBoxsTrns();
}

void CSWakVect::AllOnPointFlag(int bitflag)
{
	m_pcFitingVect->AllOnPointFlag(bitflag);
	CStrgVect::AllOnPointFlag(bitflag);
}

void CSWakVect::AllOffPointFlag(int bitflag)
{
	m_pcFitingVect->AllOffPointFlag(bitflag);
	CStrgVect::AllOffPointFlag(bitflag);
}

void CSWakVect::PointXLegal(DBL xx)
{
	m_pcFitingVect->PointXLegal(xx);
	CStrgVect::PointXLegal(xx);
}

void CSWakVect::PointYLegal(DBL yy)
{
	m_pcFitingVect->PointYLegal(yy);
	CStrgVect::PointYLegal(yy);
}

CData* CSWakVect::SrchNearPoint(DBL xx, DBL yy, DBL *retlen, int *retpp)
{
	CData*	ret;

	*retlen = INTMAX;
	ret = m_pcFitingVect->SrchNearPoint(xx, yy, retlen, retpp);
	return(ret);
}

CVect* CSWakVect::GetFitingVect()
{
	return(m_pcFitingVect);
}

void CSWakVect::SetFitingVect(CVect* pcVect)
{
	m_pcFitingVect = pcVect;
	pcVect->SetFillPen(-1);
	pcVect->SetOutLinePen(-1);
	pcVect->SetOutLineWidth(0.0);
}

void CSWakVect::GetFildName(LPTSTR lpFildName)
{
	int len = _tcslen(m_szFildName) + 1;
	_tcscpy_s(lpFildName, len, m_szFildName);
}

void CSWakVect::SetFildName(LPTSTR lpFildName)
{
	_tcscpy_s(m_szFildName, lpFildName);
}

void CSWakVect::GetSpaceKind(int *kind)
{
	*kind = m_nSpaceKind;
}

void CSWakVect::SetSpaceKind(int kind)
{
	m_nSpaceKind = kind;
}

void CSWakVect::GetKanaPar(LPDBL dpar)
{
	*dpar = m_dKanaPar;
}

void CSWakVect::SetKanaPar(DBL dpar)
{
	m_dKanaPar = dpar;
}

void CSWakVect::GetSpacePar(LPDBL dspacepar)
{
	*dspacepar = m_dSpacePar;
}

void CSWakVect::SetSpacePar(DBL dspacepar)
{
	m_dSpacePar = dspacepar;
}

void CSWakVect::GetFlatPar(LPDBL dpar)
{
	*dpar = m_dFlatPar;
}

void CSWakVect::SetFlatPar(DBL dpar)
{
	m_dFlatPar = dpar;
}

void CSWakVect::GetThickPar(LPDBL dpar)
{
	*dpar = m_dThickPar;
}

void CSWakVect::SetThickPar(DBL dpar)
{
	m_dThickPar = dpar;
}

void CSWakVect::GetSlopRag(LPDBL drag)
{
	*drag = m_dSlopRag;
}

void CSWakVect::SetSlopRag(DBL drag)
{
	m_dSlopRag = drag;
}

void CSWakVect::GetOderKind(int *kind)
{
	*kind = m_nOder;
}

void CSWakVect::SetOderKind(int kind)
{
	m_nOder = kind;
}

void CSWakVect::SetString(TCHAR *pstr)
{
	int		flag;
	DBL		wd, hi;
	int		len, idx;
	int		code;
	int		codetbl[STRGMAX];
	CMojiVect*	pcMojiVect;

	if(m_sString[0] != _T('\0')){
		AllMojiFree();
		m_sString[0] = _T('\0');
	}
	flag = CheckTateYoko();
	len = _tcslen(pstr);
	if(len <= 0){
		return;
	}
	GetFontSize(&wd, &hi);
	if(flag == TATEGAKI || flag == YOKOOKI){
		SetFontSize(wd, wd);
	}
	else{
		SetFontSize(hi, hi);
	}
	if(STRGMAX < len){
		// 2001.04.27　立身　修正
		pstr[STRGMAX-1] = _T('\0');
		pstr[STRGMAX-2] = _T('\0');
	}
	_tcscpy_s(m_sString, pstr);
	len = basesetcodetbl(pstr, codetbl);
	codetbl[len] = 0;

	idx = 0;
	while(1){
		if(idx >= len){
			break;
		}
		code = codetbl[idx];
		if(code == 0){
			break;
		}
		
		if(code < 0x100){
#ifdef UNICODE
			code = baseCodeHanToZen(codetbl[idx]);
#else
			code = _mbbtombc(codetbl[idx]);
#endif
		}

		pcMojiVect = (CMojiVect*)NewDataCreate(MOJIVECTID);
		pcMojiVect->BaseFontLink(m_pcFSF1Font);
		pcMojiVect->WNTTFontLink(m_pcWNTTFont);
		pcMojiVect->ESF0FontLink(m_pcESF0Font);
		pcMojiVect->ESF1FontLink(m_pcESF1Font);
		pcMojiVect->FBU0FontLink(m_pcFBU0Font);
		pcMojiVect->ARCFontLink(m_pcARCFont);
		pcMojiVect->ARKFontLink(m_pcARKFont);
		pcMojiVect->SCRFontLink(m_pcSCRFont);

		pcMojiVect->SetTateYoko(flag);
		pcMojiVect->m_nBaseCode = codetbl[idx];
		pcMojiVect->m_nCode = code;
		m_nFontType = FontSelect(m_sFontName, flag);
		pcMojiVect->FontLoad(m_nFontType);

		SetMojiLink(pcMojiVect);

		idx++;
	}

	StringElementInit();
}

void CSWakVect::StringElementInit()
{
	int		pnofill, pnooutline;
	int		flag;
	DBL		doutline;
	DBL		sx, sy;
	DBL		tsx, tsy;
	DBL		wd, hi;
	DBL		kpar, fpar;
	DBL		kaddx, kaddy;
	DBL		kwd, khi;
	DBL		dsingew, dsingeh;
	DBL		time;
	CMojiVect*	pcMojiVect;
	UINT	code;

	flag = CheckTateYoko();
	pnofill = GetFillPen();
	pnooutline = GetOutLinePen();
	doutline = GetOutLineWidth();
	m_dLinelen = GetVectLength();
	GetStrStartXY(&sx, &sy);
	GetFontSize(&wd, &hi);
	dsingew = (0 < wd)?1:-1;
	dsingeh = (0 < hi)?1:-1;
	wd = fabs(wd);
	hi = fabs(hi);
	GetKanaPar(&kpar);
	GetFlatPar(&fpar);
	if(fpar != MJFREEFLATPAR){
		if(flag == TATEGAKI || flag == YOKOOKI){
			hi = wd * fpar / 100.0;
		}else{
			wd = hi * fpar / 100.0;
		}
	}

	kwd = wd * kpar / 100.0;
	khi = hi * kpar / 100.0;
	kaddx = (wd - kwd) / 2;
	kaddy = (hi - khi) / 2;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		// 1/3 1/4 スペース処理
		if(pcMojiVect->CheckSpecialSpace(&time) == TRUE){
			pcMojiVect->SetStartXY(sx, sy);
			if(flag == TATEGAKI || flag == YOKOOKI){
				pcMojiVect->SetFontSize(wd, hi*time);
				sy -= hi*time;
			}
			else{
				pcMojiVect->SetFontSize(wd*time, hi);
				sy += wd*time;
			}
		}else if(pcMojiVect->IsZenKaku() == TRUE){
			code = pcMojiVect->m_nBaseCode;
			pcMojiVect->SetTateYoko(flag);
			if(flag == TATEGAKI || flag == YOKOOKI){
				tsx = sx - wd / 2.0;
				tsy = sy - hi;
			}
			else{
				tsx = sx;
				tsy = sy - hi / 2.0;
			}
			if(0x829f <= code && code <= 0x82f1){
				pcMojiVect->SetStartXY(tsx+kaddx, tsy-kaddy);
				pcMojiVect->SetFontSize(kwd, khi);
			}
			else if(0x8340 <= code && code <= 0x8396){
				pcMojiVect->SetStartXY(tsx+kaddx, tsy-kaddy);
				pcMojiVect->SetFontSize(kwd, khi);
			}
			else{
				pcMojiVect->SetStartXY(tsx, tsy);
				pcMojiVect->SetFontSize(wd, hi);
			}

			if(flag == TATEGAKI || flag == YOKOOKI){
				sy -= hi;
			}
			else{
				sx += wd;
			}
		}else{
			pcMojiVect->SetStartXY(sx, sy);
			if(flag == TATEGAKI || flag == YOKOOKI){
				pcMojiVect->SetFontSize(wd, hi/2);
				sy -= hi/2;
			}
			else{
				pcMojiVect->SetFontSize(wd/2, hi);
				sy += wd/2;
			}
		}
		pcMojiVect->SetOutLinePen(pnooutline);
		pcMojiVect->SetFillPen(pnofill);
		pcMojiVect->SetOutLineWidth(doutline);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CSWakVect::StrgFiting()
{
	TCHAR	szStr[RECMAX];

	_tcscpy_s(szStr, m_sString);
	SetString(szStr);
	StrgLineStruct();
}

void CSWakVect::StrgSlopTrns()
{
	DBL		sx, sy;
	DBL		ddx, ddy;

	if(m_dSlopRag != 0){
		GetStrStartXY(&sx, &sy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
			ddx = 0;
			ddy = tan(m_dSlopRag);
			SlopTrns(sx, sy, ddx, ddy);
		}
		else{
			ddx = tan(m_dSlopRag);
			ddy = 0;
			SlopTrns(sx, sy, ddx, ddy);
		}
	}
}

void CSWakVect::StrgLineStruct()
{
	DBL	dwidth, dhight;
	//DBL	dthick;

	if(m_sString[0] == _T('\0')){
		return;
	}
	GetFontSize(&dwidth, &dhight);
	//dthick = dhight * (m_dThickPar) / 1000;
	//SetOutLineWidth(dthick);
	switch(m_nOder){
	case ODERLEFT:
		switch(m_nSpaceKind){
		case SPACEWAKU:
			SetLenStrgLineStructLeftWaku();
			break;
		case SPACEDATA:
			SetLenStrgLineStructLeftData();
			break;
		}
		break;
	case ODERCNTR:
		switch(m_nSpaceKind){
		case SPACEWAKU:
			SetLenStrgLineStructCntrWaku();
			break;
		case SPACEDATA:
			SetLenStrgLineStructCntrData();
			break;
		}
		break;
	case ODERRIGHT:
		switch(m_nSpaceKind){
		case SPACEWAKU:
			SetLenStrgLineStructRightWaku();
			break;
		case SPACEDATA:
			SetLenStrgLineStructRightData();
			break;
		}
		break;
	case ODEREQUAL:
	default:
		m_nOder = ODEREQUAL;
		switch(m_nSpaceKind){
		case SPACEWAKU:
			SetLenStrgLineStructEqualWaku();
			break;
		case SPACEDATA:
			SetLenStrgLineStructEqualData();
			break;
		}
		break;
	}
	StrgLineStructTrns();

	StrgSlopTrns();
}

void CSWakVect::SetLenStrgLineStructLeftData()
{
	int		flag;
	DBL		size, space;
	DBL		sumlen, sumspace;
	int			nYokoMojiIn;
	CMojiVect*	pcMojiVect;
	DBL		setlen;
	DBL		sp, ep;
	DBL		wd, hi;

	flag = CheckTateYoko();
	sumlen = GetDataSumLength();
	sumspace = GetSumSpace();
	if(m_dCharCount > 0){
		GetFontSize(&wd, &hi);
		if(flag == TATEGAKI || flag == TATEOKI){
			sumlen = hi * m_dCharCount;
		}else{
			sumlen = wd * m_dCharCount;
		}
	}
	sumlen = sumlen + sumspace;
	m_dLinelen = GetVectLength();
	//if(m_dLinelen < sumlen || m_dFlatPar == MJFREEFLATPAR){
	if(m_dLinelen < sumlen){
		SetLenStrgLineStructEqualData();
		return;
	}
	setlen = 0;
	nYokoMojiIn = OFF;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		size = GetBody(pcMojiVect);
		space = GetSpace(pcMojiVect);
		if(pcMojiVect->IsNumCode() == TRUE && m_nYokoGumi == ON){
			if(nYokoMojiIn == OFF){
				nYokoMojiIn = ON;
				setlen = setlen + size + space;
			}
			pcMojiVect->SetLenFromStrgTop(setlen - size/2 - space);
		}
		else{
			nYokoMojiIn = OFF;
			pcMojiVect->GetDataSEP(&sp, &ep);
			setlen = setlen + size/2 - sp;
			pcMojiVect->SetLenFromStrgTop(setlen);
			setlen = setlen - size/2 + ep + space;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CSWakVect::SetLenStrgLineStructLeftWaku()
{
	int		flag;
	DBL		top, last;
	DBL		size, space;
	DBL		sumlen, sumspace;
	int			nYokoMojiIn;
	CMojiVect*	pcMojiVect;
	DBL		setlen;
	DBL		wd, hi;

	flag = CheckTateYoko();
	GetTopLastSpace(&top, &last);
	sumspace = GetSumSpace();
	sumlen = GetWakuSumLength();
	if(m_dCharCount > 0){
		GetFontSize(&wd, &hi);
		if(flag == TATEGAKI || flag == TATEOKI){
			sumlen = hi * m_dCharCount;
		}else{
			sumlen = wd * m_dCharCount;
		}
	}
	sumlen = sumlen + sumspace - top - last;
	m_dLinelen = GetVectLength();
	//if(m_dLinelen < sumlen || m_dFlatPar == MJFREEFLATPAR){
	if(m_dLinelen < sumlen){
		SetLenStrgLineStructEqualWaku();
		return;
	}

	setlen = -top;
	nYokoMojiIn = OFF;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		size = GetBody(pcMojiVect);
		if(pcMojiVect->IsNumCode() == TRUE && m_nYokoGumi == ON){
			if(nYokoMojiIn == OFF){
				nYokoMojiIn = ON;
				setlen = setlen + size + space;
			}
			pcMojiVect->SetLenFromStrgTop(setlen - size / 2 - space);
		}
		else{
			nYokoMojiIn = OFF;
			setlen += size/2;
			pcMojiVect->SetLenFromStrgTop(setlen);
			space = GetSpace(pcMojiVect);
			setlen = setlen + size / 2 + space;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CSWakVect::SetLenStrgLineStructCntrData()
{
	DBL		size, space;
	DBL		sumlen, sumspace;
	int			nYokoMojiIn;
	CMojiVect*	pcMojiVect;
	DBL		setlen;
	DBL		sp, ep;

	sumlen = GetDataSumLength();
	sumspace = GetSumSpace();
	sumlen = sumlen + sumspace;
	m_dLinelen = GetVectLength();
	//if(m_dLinelen < sumlen || m_dFlatPar == MJFREEFLATPAR){
	if(m_dLinelen < sumlen){
		SetLenStrgLineStructEqualData();
		return;
	}

	setlen = (m_dLinelen - sumlen) / 2;
	nYokoMojiIn = OFF;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		size = GetBody(pcMojiVect);
		space = GetSpace(pcMojiVect);
		if(pcMojiVect->IsNumCode() == TRUE && m_nYokoGumi == ON){
			if(nYokoMojiIn == OFF){
				nYokoMojiIn = ON;
				setlen = setlen + size + space;
			}
			pcMojiVect->SetLenFromStrgTop(setlen - size / 2 - space);
		}
		else{
			nYokoMojiIn = OFF;
			pcMojiVect->GetDataSEP(&sp, &ep);
			setlen = setlen + size/2 - sp;
			pcMojiVect->SetLenFromStrgTop(setlen);
			setlen = setlen - size/2 + ep + space;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CSWakVect::SetLenStrgLineStructCntrWaku()
{
	DBL		top, last;
	DBL		size, space;
	DBL		sumlen, sumspace;
	int			nYokoMojiIn;
	CMojiVect*	pcMojiVect;
	DBL		setlen;

	GetTopLastSpace(&top, &last);
	sumspace = GetSumSpace();
	sumlen = GetWakuSumLength();
	sumlen = sumlen + sumspace - top - last;
	m_dLinelen = GetVectLength();
	//if(m_dLinelen < sumlen || m_dFlatPar == MJFREEFLATPAR){
	if(m_dLinelen < sumlen){
		SetLenStrgLineStructEqualWaku();
		return;
	}

	setlen = (m_dLinelen - sumlen) / 2 - top;
	nYokoMojiIn = OFF;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		size = GetBody(pcMojiVect);
		if(pcMojiVect->IsNumCode() == TRUE && m_nYokoGumi == ON){
			if(nYokoMojiIn == OFF){
				nYokoMojiIn = ON;
				setlen = setlen + size + space;
			}
			pcMojiVect->SetLenFromStrgTop(setlen - size / 2 - space);
		}
		else{
			nYokoMojiIn = OFF;
			setlen += size/2;
			pcMojiVect->SetLenFromStrgTop(setlen);
			space = GetSpace(pcMojiVect);
			setlen = setlen + size / 2 + space;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CSWakVect::SetLenStrgLineStructRightData()
{
	DBL		size, space;
	DBL		sumlen, sumspace;
	int			nYokoMojiIn;
	CMojiVect*	pcMojiVect;
	DBL		setlen;
	DBL		sp, ep;

	sumlen = GetDataSumLength();
	sumspace = GetSumSpace();
	sumlen = sumlen + sumspace;
	m_dLinelen = GetVectLength();
	//if(m_dLinelen < sumlen || m_dFlatPar == MJFREEFLATPAR){
	if(m_dLinelen < sumlen){
		SetLenStrgLineStructEqualData();
		return;
	}

	setlen = m_dLinelen - sumlen;
	nYokoMojiIn = OFF;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		size = GetBody(pcMojiVect);
		space = GetSpace(pcMojiVect);
		if(pcMojiVect->IsNumCode() == TRUE && m_nYokoGumi == ON){
			if(nYokoMojiIn == OFF){
				nYokoMojiIn = ON;
				setlen = setlen + size + space;
			}
			pcMojiVect->SetLenFromStrgTop(setlen - size / 2 - space);
		}
		else{
			nYokoMojiIn = OFF;
			pcMojiVect->GetDataSEP(&sp, &ep);
			setlen = setlen + size/2 - sp;
			pcMojiVect->SetLenFromStrgTop(setlen);
			setlen = setlen - size/2 + ep + space;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CSWakVect::SetLenStrgLineStructRightWaku()
{
	DBL		top, last;
	DBL		size, space;
	DBL		sumlen, sumspace;
	int			nYokoMojiIn;
	CMojiVect*	pcMojiVect;
	DBL		setlen;

	GetTopLastSpace(&top, &last);
	sumspace = GetSumSpace();
	sumlen = GetWakuSumLength();
	sumlen = sumlen + sumspace - top - last;
	m_dLinelen = GetVectLength();
	//if(m_dLinelen < sumlen || m_dFlatPar == MJFREEFLATPAR){
	if(m_dLinelen < sumlen){
		SetLenStrgLineStructEqualWaku();
		return;
	}

	setlen = m_dLinelen - sumlen - top;
	nYokoMojiIn = OFF;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		size = GetBody(pcMojiVect);
		if(pcMojiVect->IsNumCode() == TRUE && m_nYokoGumi == ON){
			if(nYokoMojiIn == OFF){
				nYokoMojiIn = ON;
				setlen = setlen + size + space;
			}
			pcMojiVect->SetLenFromStrgTop(setlen - size / 2 - space);
		}
		else{
			nYokoMojiIn = OFF;
			setlen += size/2;
			pcMojiVect->SetLenFromStrgTop(setlen);
			space = GetSpace(pcMojiVect);
			setlen = setlen + size / 2 + space;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CSWakVect::SetLenStrgLineStructEqualData()
{
	int		flag;
	int		nYokoMojiIn;
	CMojiVect*	pcMojiVect;
	DBL		size, space;
	DBL		mojicnt;
	DBL		sumlen, sumspace;
	DBL		sp, ep;
	DBL		datatime, spacetime, addspace;
	DBL		setlen;
	DBL		wd, hi;

	flag = CheckTateYoko();
	sumlen = GetDataSumLength();
	if(sumlen == 0) return;
	sumspace = GetSumSpace();
	if(m_dCharCount <= 0){
		mojicnt = GetMojiCount();
	}else{
		mojicnt = m_dCharCount;
		GetFontSize(&wd, &hi);
		if(flag == TATEGAKI || flag == YOKOOKI){
			sumlen = hi * mojicnt;
		}else{
			sumlen = wd * mojicnt;
		}
	}
	m_dLinelen = GetVectLength();
	if(m_dLinelen < (sumlen + sumspace)){
		datatime = (m_dLinelen - sumspace) / sumlen;
		spacetime = 1.0;
		addspace = 0.0;
	}
	else{
		if(mojicnt == 1){
			datatime = 1.0;
			spacetime = 1.0;
			addspace = 0.0;
		}
		else{
			datatime = 1.0;
			spacetime = 1.0;
			addspace = (m_dLinelen - sumspace - sumlen) / (mojicnt-1);
		}
	}

	if(mojicnt == 1){
		setlen = m_dLinelen / 2;
	}
	else{
		setlen = 0;
	}
	nYokoMojiIn = OFF;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->GetFontSize(&wd, &hi);
		if(flag == TATEGAKI || flag == YOKOOKI){
			hi = hi * datatime;
			size = hi;
		}
		else{
			wd = wd * datatime;
			size = wd;
		}
		pcMojiVect->SetFontSize(wd, hi);
		space = GetSpace(pcMojiVect);
		space = space * spacetime + addspace;
		if(pcMojiVect->IsNumCode() == TRUE && m_nYokoGumi == ON){
			if(flag == TATEGAKI || flag == YOKOOKI){
				size = wd * datatime;
			}
			else{
				size = hi * datatime;
			}
			if(nYokoMojiIn == OFF){
				nYokoMojiIn = ON;
				setlen = setlen + size + space;
			}
			pcMojiVect->SetLenFromStrgTop(setlen - size / 2 - space);
		}
		else{
			nYokoMojiIn = OFF;
			pcMojiVect->GetDataSEP(&sp, &ep);
			if(mojicnt != 1){
				setlen = setlen + size/2 - sp;
			}
			pcMojiVect->SetLenFromStrgTop(setlen);
			setlen = setlen - size/2 + ep + space;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CSWakVect::SetLenStrgLineStructEqualWaku()
{
	int		flag;
	int		nYokoMojiIn;
	CMojiVect*	pcMojiVect;
	DBL		top, last;
	DBL		size, space;
	DBL		mojicnt;
	DBL		sumlen, sumspace;
	DBL		datatime, spacetime, addspace;
	DBL		setlen;
	DBL		wd, hi;

	flag = CheckTateYoko();
	GetTopLastSpace(&top, &last);
	sumlen = GetWakuSumLength();
	if(sumlen == 0) return;
	sumspace = GetSumSpace();
	if(m_dCharCount <= 0){
		mojicnt = GetMojiCount();
	}else{
		mojicnt = m_dCharCount;
		GetFontSize(&wd, &hi);
		if(flag == TATEGAKI || flag == YOKOOKI){
			sumlen = hi * mojicnt;
		}else{
			sumlen = wd * mojicnt;
		}
	}
	sumlen -= (top + last);
	m_dLinelen = GetVectLength();

	if(m_dLinelen < (sumlen+sumspace)){
		datatime = (m_dLinelen - sumspace) / sumlen;
		spacetime = 1.0;
		addspace = 0.0;
	}
	else{
		if(mojicnt == 1){
			datatime = 1.0;
			spacetime = 1.0;
			addspace = 0.0;
		}
		else{
			datatime = 1.0;
			spacetime = 1.0;
			addspace = (m_dLinelen - sumspace - sumlen) / (mojicnt-1);
		}
	}

	if(mojicnt == 1){
		setlen = m_dLinelen / 2;
	}
	else{
		setlen = - top*datatime;
	}
	nYokoMojiIn = OFF;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->GetFontSize(&wd, &hi);
		if(flag == TATEGAKI || flag == YOKOOKI){
			hi = hi * datatime;
			size = hi;
		}
		else{
			wd = wd * datatime;
			size = wd;
		}
		pcMojiVect->SetFontSize(wd, hi);
		pcMojiVect->RenewMiniMax();
		space = GetSpace(pcMojiVect);
		space = space * spacetime + addspace;
		if(pcMojiVect->IsNumCode() == TRUE && m_nYokoGumi == ON){
			if(flag == TATEGAKI || flag == YOKOOKI){
				size = wd * datatime;
			}
			else{
				size = hi * datatime;
			}
			if(nYokoMojiIn == OFF){
				nYokoMojiIn = ON;
				setlen = setlen + size + space;
			}
			pcMojiVect->SetLenFromStrgTop(setlen - size / 2 - space);
		}
		else{
			nYokoMojiIn = OFF;
			if(mojicnt != 1){
				setlen += size/2;
			}
			pcMojiVect->SetLenFromStrgTop(setlen);
			setlen = setlen + size/2 + space;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CSWakVect::GetTopLastSpace(LPDBL top, LPDBL last)
{
	int		flag;
	DBL		swd, shi;
	CMojiVect*	pcMojiVect;
	CMojiVect*	pcTMojiVect;
	DBL		mwd, mhi;
	DBL		wminx, wminy, wmaxx, wmaxy;
	DBL		dminx, dminy, dmaxx, dmaxy;

	flag = CheckTateYoko();
	GetFontSize(&swd, &shi);
	pcMojiVect = m_pcMojiTop;
	if(pcMojiVect == NULL){
		*top = 0;
		*last = 0;
	}
	pcMojiVect->GetFontSize(&mwd, &mhi);
	pcMojiVect->GetWakuDMiniMax(&wminx, &wminy, &wmaxx, &wmaxy);
	pcMojiVect->GetDataDMiniMax(&dminx, &dminy, &dmaxx, &dmaxy);
	if(flag == TATEGAKI || flag == YOKOOKI){
		//*top = wmaxy - dmaxy + (shi - mhi) / 2;
		*top = wmaxy - dmaxy;
	}
	else{
		//*top = dminx - wminx + (swd - mwd) / 2;
		*top = dminx - wminx;
	}
	pcTMojiVect = pcMojiVect;
	while(1){
		if(pcTMojiVect == NULL){
			break;
		}
		pcMojiVect = pcTMojiVect;
		pcTMojiVect = pcMojiVect->GetNextMoji();
	}
	pcMojiVect->GetFontSize(&mwd, &mhi);
	pcMojiVect->GetWakuDMiniMax(&wminx, &wminy, &wmaxx, &wmaxy);
	pcMojiVect->GetDataDMiniMax(&dminx, &dminy, &dmaxx, &dmaxy);
	if(flag == TATEGAKI || flag == YOKOOKI){
		//*last = dminy - wminy + (shi - mhi) / 2;
		*last = dminy - wminy;
	}
	else{
		//*last = wmaxx - dmaxx + (swd - mwd) / 2;
		*last = wmaxx - dmaxx;
	}
	*top = 0;
	*last = 0;
}

void CSWakVect::StrgLineStructTrns()
{
	int		flag;
	CMojiVect*	pcMojiVect;
	CMojiVect*	pcMojiVectTable[RECMAX];
	int		nYokoMojiIdx;
	DBL		setlen;
	DBL		mminix, mminiy, mmaxx, mmaxy;
	DBL		cx, cy;
	DBL		movex, movey;
	DBL		tsin, tcos;
	DBL		rag, dx, dy;
	DBL		dw, dh;

	GetFontSize(&dw, &dh);
	nYokoMojiIdx = 0;
	pcMojiVect = m_pcMojiTop;
	flag = CheckTateYoko();
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(pcMojiVect->IsNumCode() == TRUE && m_nYokoGumi == ON){
			pcMojiVectTable[nYokoMojiIdx] = pcMojiVect;
			nYokoMojiIdx++;
		}
		else{
			if(nYokoMojiIdx != 0){
				if(m_nSpaceKind == SPACEWAKU){
					YokoGumiStructTrnsData(pcMojiVectTable, nYokoMojiIdx);
				}
				else{
					YokoGumiStructTrnsData(pcMojiVectTable, nYokoMojiIdx);
				}
				nYokoMojiIdx = 0;
			}
			setlen = pcMojiVect->GetLenFromStrgTop();
			rag = GetVectPointRagAndCood(&setlen, &dx, &dy);
			if(flag == TATEGAKI || flag == YOKOOKI){
				rag += M_PI_2;
			}
			pcMojiVect->GetWakuDMiniMax(&mminix, &mminiy, &mmaxx, &mmaxy);
			cx = (mminix + mmaxx) / 2;
			cy = (mminiy + mmaxy) / 2;
			movex = dx - cx;
			movey = dy - cy;
			tsin = sin(rag);
			tcos = cos(rag);
			pcMojiVect->MoveTrns(movex, movey);
			pcMojiVect->RoundTrns(dx, dy, tsin, tcos);
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	if(nYokoMojiIdx != 0){
		if(m_nSpaceKind == SPACEWAKU){
			YokoGumiStructTrnsData(pcMojiVectTable, nYokoMojiIdx);
		}
		else{
			YokoGumiStructTrnsData(pcMojiVectTable, nYokoMojiIdx);
		}
	}
}

DBL CSWakVect::GetVectLength()
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

DBL CSWakVect::GetRejionLength()
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

DBL CSWakVect::GetNorejionLength()
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

DBL CSWakVect::GetBezeLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
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

DBL CSWakVect::GetArcLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

DBL CSWakVect::GetFreeCurveLength(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

DBL CSWakVect::GetLineLength(DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		len;

	len = hypot(x1-x2, y1-y2);
	return(len);
}

// 点までの長さを求める
DBL CSWakVect::GetVectPointLength(DBL xx, DBL yy)
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

DBL CSWakVect::GetRejionPointLength(int *flag, DBL xx, DBL yy)
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

DBL CSWakVect::GetNorejionPointLength(int *flag, DBL xx, DBL yy)
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

DBL CSWakVect::GetBezePointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
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

DBL CSWakVect::GetArcPointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

DBL CSWakVect::GetFreeCurvePointLength(int *flag, DBL xx, DBL yy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

DBL CSWakVect::GetLinePointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2)
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
DBL CSWakVect::GetVectPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy)
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

DBL CSWakVect::GetRejionPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy)
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

DBL CSWakVect::GetNorejionPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy)
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

DBL CSWakVect::GetBezePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
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

DBL CSWakVect::GetArcPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

DBL CSWakVect::GetFreeCurvePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

DBL CSWakVect::GetLinePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2)
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

void CSWakVect::DeleteData(CData *delData)
{
	CVect	*pcVect;

	if(m_pcFitingVect == delData){
		pcVect = (CVect*)(m_pcFitingVect->CopyData(0, 0));
		m_pcFitingVect = pcVect;
		return;
	}
	CStrgVect::DeleteData(delData);
}

CData* CSWakVect::SrchData(DBL dx, DBL dy, DBL *retlen)
{
	return(SrchDataBoxs(dx, dy, retlen));
}

CData* CSWakVect::SrchDataBoxs(DBL dx, DBL dy, DBL *retlen)
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

DBL CSWakVect::GetPropStrglen()
{
	DBL		wd, hi;
	DBL		sumlen;
	DBL		strsize, mjsize, time, ttime;
	int		posttbl[FDIBCSIZE];
	int		npretbl[FDIBCSIZE], nposttbl[FDIBCSIZE];
	int		i;
	CMojiVect	*pcMojiVect;
	DBL		mini;

	GetFontSize(&wd, &hi);
	sumlen = 0;
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		strsize = wd;
	}
	else{
		strsize = hi;
	}
	time = strsize / (DBL)FDIBCSIZE;
	pcMojiVect = m_pcMojiTop;
	for(i = 0; i < FDIBCSIZE; i++){
		posttbl[i] = 0;
	}
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		mjsize = GetBody(pcMojiVect);
		pcMojiVect->GetPrePostTable(mjsize, mjsize, npretbl, nposttbl);
		ttime = time * mjsize / strsize;
		mini = GetMiniSpace(npretbl, posttbl) * ttime;
		sumlen += (mjsize - mini);
		pcMojiVect = pcMojiVect->GetNextMoji();
		for(i = 0; i < FDIBCSIZE; i++){
			posttbl[i] = nposttbl[i];
		}
	}
	for(i = 0; i < FDIBCSIZE; i++){
		npretbl[i] = 0;
	}
	ttime = time * mjsize / strsize;
	mini = GetMiniSpace(npretbl, posttbl) * ttime;
	sumlen -= mini;
	return(sumlen);
}

DBL CSWakVect::GetMiniSpace(int pretbl[], int posttbl[])
{
	int		i;
	DBL		mini, tmini;
	DBL		postmini, premini;

	mini = FDIBCSIZE;
	for(i = 0; i < FDIBCSIZE; i++){
		if(pretbl[i] != FDIBCSIZE && posttbl[i] != FDIBCSIZE){
			tmini = pretbl[i] + posttbl[i];
			if(mini > tmini){
				mini = tmini;
			}
		}
	}
	if(mini == FDIBCSIZE){
		premini = FDIBCSIZE;
		postmini = FDIBCSIZE;
		for(i = 0; i < FDIBCSIZE; i++){
			if(pretbl[i] != FDIBCSIZE ){
				if(premini > pretbl[i]){
					premini = pretbl[i];
				}
			}
			if(posttbl[i] != FDIBCSIZE ){
				if(postmini > posttbl[i]){
					postmini = posttbl[i];
				}
			}
		}
		if(premini == FDIBCSIZE){
			premini = 0;
		}
		if(postmini == FDIBCSIZE){
			postmini = 0;
		}
		mini = premini + postmini;
	}
	return(mini);
}

DBL CSWakVect::GetSumSpace()
{
	CMojiVect*	pcMojiVect;
	int			nYokoMojiIn;
	DBL		sumlen;
	
	sumlen = 0;
	nYokoMojiIn = OFF;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(pcMojiVect->IsNumCode() == TRUE && m_nYokoGumi == ON){
			if(nYokoMojiIn == OFF){
				nYokoMojiIn = ON;
				sumlen += GetSpace(pcMojiVect);
			}
		}
		else{
			nYokoMojiIn = OFF;
			sumlen += GetSpace(pcMojiVect);
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	return(sumlen);
}

DBL CSWakVect::GetBody()
{
	DBL		wd, hi;

	GetFontSize(&wd, &hi);
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		return(wd);
	}
	else{
		return(hi);
	}
}

DBL CSWakVect::GetSpace(CMojiVect *pcMojiVect)
{
	DBL		wd, hi;
	DBL		spacemm;

	if(m_dSpacePar == MJFREESPACEPAR){
		return(0.0);
	}
	GetFontSize(&wd, &hi);
	if(CheckTateYoko() == TATEGAKI){
		spacemm = (hi * m_dSpacePar) / 100.0;
	}
	else{
		spacemm = (wd * m_dSpacePar) / 100.0;
	}
	pcMojiVect = pcMojiVect->GetNextMoji();
	if(pcMojiVect == NULL){
		return(0);
	}
	return(spacemm);

}

DBL CSWakVect::GetBody(CMojiVect *pcMojiVect)
{
	DBL		wd, hi;

	pcMojiVect->GetFontSize(&wd, &hi);
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		return(hi);
	}
	else{
		return(wd);
	}
}

DBL CSWakVect::GetDataSumLength()
{
	int		flag;
	DBL		xsumlen, ysumlen;

	flag = CheckTateYoko();
	GetDataSumLen(&xsumlen, &ysumlen);
	if(flag == TATEGAKI || flag == YOKOOKI){
		return(ysumlen);
	}
	else{
		return(xsumlen);
	}
}

DBL CSWakVect::GetWakuSumLength()
{
	int		flag;
	DBL		xsumlen, ysumlen;

	flag = CheckTateYoko();
	GetWakuSumLen(&xsumlen, &ysumlen);
	if(flag == TATEGAKI || flag == YOKOOKI){
		return(ysumlen);
	}
	else{
		return(xsumlen);
	}
}

int CSWakVect::GetMojiCount()
{
	CMojiVect*	pcMojiVect;
	int		nYokoMojiIn;
	int		count;

	count = 0;
	nYokoMojiIn = OFF;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(pcMojiVect->IsNumCode() == TRUE && m_nYokoGumi == ON){
			if(nYokoMojiIn == OFF){
				nYokoMojiIn = ON;
				count++;
			}
		}
		else{
			nYokoMojiIn = OFF;
			count++;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	return(count);
}

void CSWakVect::WakuSave(FILE * fp)
{
	TCHAR	buf[RECMAX4];
	DBL		mmxsize, mmysize;
	TCHAR	tatestr[NRECMAX];
	DBL		x1, y1, x2, y2;
	DBL		dsx, dsy;
	DBL		dwd, dhi;
	int		fillpen, linepen;
	int		r, g, b;
	long	fillclr, lineclr;

	scrngetmmsize(&mmxsize, &mmysize);
	fillpen = GetFillPen();
	linepen = GetOutLinePen();
	if(fillpen == -1){
		fillclr = -1;
	}
	else{
		if(fillpen == 0){
			r = 0; g = 0; b = 0;
		}
		else{
			senvgetpenrgb(fillpen, &r, &g, &b);
		}
		fillclr = RGB(r, g, b);
	}

	if(linepen == -1){
		lineclr = -1;
	}
	else{
		if(fillpen == 0){
			r = 0; g = 0; b = 0;
		}
		else{
			senvgetpenrgb(linepen, &r, &g, &b);
		}
		lineclr = RGB(r, g, b);
	}

	m_pcFitingVect->GetDXY(0, &x1, &y1);
	m_pcFitingVect->GetDXY(1, &x2, &y2);
	y1 = mmysize - y1;
	y2 = mmysize - y2;
	GetFontSize(&dwd, &dhi);
	if(CheckTateYoko() == YOKOGAKI){
		_tcscpy_s(tatestr, _T("横"));
		dsx = x1;
		dsy = y1 - dhi / 2;
		dwd = x2 - x1;
	}
	else{
		_tcscpy_s(tatestr, _T("縦"));
		dsx = x1 - dwd / 2;
		dsy = y1;
		dhi = y2 - y1;
	}
	_stprintf_s(buf, RECMAX, _T("\"SWAKVECT\",\"%s\",\"%s\", %4.1lf, %4.1lf, %4.1lf, %4.1lf, \"%s\", %ld, %ld, %d, %d, %lf, %lf, %lf, %lf, %lf, %d, %d, %d, %1.1lf\n"),
		m_sString, m_sFontName, dsx, dsy, dwd, dhi, tatestr, fillclr, lineclr,
		m_nOder, m_nSpaceKind, m_dFlatPar, m_dSpacePar, m_dKanaPar, 
		m_dThickPar, m_dSlopRag, m_nYokoGumi, m_nFixFlag, m_nMirror,
		m_dCharCount);

	baselibFputs(buf, fp);
}

void CSWakVect::WakuLoad(FILE *fp, LPTSTR buf)
{
	TCHAR	*sp;
	int		len;
	TCHAR	numstr[HRECMAX];
	DBL		mmxsize, mmysize;
	TCHAR	szMatching[RECMAX];
	TCHAR	szInStr[RECMAX];
	TCHAR	szFontName[HRECMAX];
	TCHAR	tatestr[NRECMAX];
	DBL		dsx, dsy;
	DBL		dwd, dhi;
	long	fillclr, lineclr;
	DBL		dTCHARCount;
	
	int		nInt;
	int		r, g, b;
	int		fillpenno, linepenno;
	DBL		x1, y1, x2, y2;

	scrngetmmsize(&mmxsize, &mmysize);
	sp = buf;
	sp = basecsvstrdivcpy(sp, szMatching);
	len = _tcslen(_T("SWAKVECT"));
	if(_tcsnccmp(szMatching, _T("SWAKVECT"), len) != 0){
		WakuCnvLoad(fp, buf);
		return;
	}
	sp = basecsvstrdivcpy(sp, szInStr);
	sp = basecsvstrdivcpy(sp, szFontName);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &dsx);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &dsy);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &dwd);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &dhi);
	sp = basecsvstrdivcpy(sp, tatestr);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%ld"), &fillclr);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%ld"), &lineclr);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%d"), &m_nOder);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%d"), &m_nSpaceKind);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &m_dFlatPar);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &m_dSpacePar);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &m_dKanaPar);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &m_dThickPar);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &m_dSlopRag);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%d"), &nInt);
	m_nYokoGumi = (BYTE)nInt;
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%d"), &m_nFixFlag);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%d"), &m_nMirror);
	//　2008.01.15 追加
	dTCHARCount = 0;
	if(sp[0] != _T('\n') && sp[1] != _T('\n')){
		sp = basecsvstrdivcpy(sp, numstr);
		if(isdigit((int)numstr[0])){
			_stscanf_s(numstr, _T("%lf"), &dTCHARCount);
		}
	}

	if(fillclr == -1){
		fillpenno = -1;
	}
	else{
		r = GetRValue(fillclr);
		g = GetGValue(fillclr);
		b = GetBValue(fillclr);
		fillpenno = senvgetpenno(r, g, b);
	}

	if(lineclr == -1){
		linepenno = -1;
	}
	else{
		r = GetRValue(lineclr);
		g = GetGValue(lineclr);
		b = GetBValue(lineclr);
		lineclr = senvgetpenno(r, g, b);
	}

	senvsetcrtfillpenno(fillpenno);
	senvsetcrtlinepenno(linepenno);
	SetFillPen(fillpenno);
	if(m_pcFitingVect == NULL){
		m_pcFitingVect = new CVect(BASEVECTID);
	}
	len = _tcslen(_T("横"));
	if(_tcsnccmp(tatestr, _T("横"), len) == 0){
		SetTateYoko(YOKOGAKI);
		x1 = dsx;
		y1 = dsy + dhi / 2;
		x2 = dsx + dwd;
		y2 = y1;
		SetFontSize(dhi, dhi);
	}
	else{
		SetTateYoko(TATEGAKI);
		x1 = dsx + dwd / 2;
		y1 = dsy;
		x2 = x1;
		y2 = dsy + dhi;
		SetFontSize(dwd, dwd);
	}
	y1 = mmysize - y1;
	y2 = mmysize - y2;
	m_pcFitingVect->SetAtrDXY(0, 0, x1, y1);
	m_pcFitingVect->SetAtrDXY(1, 0, x2, y2);
	m_pcFitingVect->SetPointCount(2);
	m_pcFitingVect->OffFlag(REJIONFLAG);
	SetStrStartXY(x1, y1);
	SetStrLen(0.0);
	SetFontName(szFontName);
	SetMaxCharCount(dTCHARCount);
	SetString(szInStr);
	RenewMiniMax();
	if(szInStr[0] != _T('\0')){
		StrgLineStruct();
		RenewMiniMax();
	}
}

void CSWakVect::WakuCnvLoad(FILE *fp, LPTSTR buf)
{
	TCHAR	*sp;
	int		len;
	TCHAR	szMatching[RECMAX];
	TCHAR	szInStr[RECMAX];
	TCHAR	szFontName[HRECMAX];
	TCHAR	numstr[HRECMAX];
	DBL		dsx, dsy;
	DBL		dwd, dhi;
	long	fillclr, lineclr;
	TCHAR	tatestr[NRECMAX];
	DBL		TCHARcount;
	int		fillpenno, linepenno;
	DBL		linethick;
	DBL		dspace, dspacepar;
	DBL		mmxsize, mmysize;
	DBL		x1, y1, x2, y2;
	int		r, g, b;
	int		flag;
	int		nInt;

	scrngetmmsize(&mmxsize, &mmysize);
	sp = buf;
	sp = basecsvstrdivcpy(sp, szMatching);
	sp = basecsvstrdivcpy(sp, szInStr);
	sp = basecsvstrdivcpy(sp, szFontName);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &dsx);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &dsy);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &dwd);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &dhi);
	sp = basecsvstrdivcpy(sp, tatestr);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%ld"), &fillclr);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%ld"), &lineclr);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &TCHARcount);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &linethick);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%x"), &flag);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &m_dTrns);
	// 040415 追加
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &dspace);
	// 040415 追加
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%d"), &nInt);
	m_nYokoGumi = (BYTE)nInt;

	if(fillclr == -1){
		fillpenno = -1;
	}
	else{
		r = GetRValue(fillclr);
		g = GetGValue(fillclr);
		b = GetBValue(fillclr);
		fillpenno = senvgetpenno(r, g, b);
	}

	if(lineclr == -1){
		linepenno = -1;
	}
	else{
		r = GetRValue(lineclr);
		g = GetGValue(lineclr);
		b = GetBValue(lineclr);
		linepenno = senvgetpenno(r, g, b);
	}

	senvsetcrtfillpenno(fillpenno);
	senvsetcrtlinepenno(linepenno);
	SetFillPen(fillpenno);
	if(m_pcFitingVect == NULL){
		m_pcFitingVect = new CVect(BASEVECTID);
	}

	dsy = mmysize - dsy;
	len = _tcslen(_T("横"));
	if (_tcsnccmp(tatestr, _T("横"), len) == 0) {
		SetTateYoko(YOKOGAKI);
		x1 = dsx;
		y1 = dsy - dhi / 2;
		x2 = dsx + dwd;
		y2 = y1;
		SetFontSize(dhi, dhi);
		dspacepar = dspace / dhi * 100.0;
	}
	else{
		SetTateYoko(TATEGAKI);
		x1 = dsx + dwd / 2;
		y1 = dsy;
		x2 = x1;
		y2 = dsy - dhi;
		SetFontSize(dwd, dwd);
		dspacepar = dspace / dwd * 100.0;
	}

	m_nFixFlag = flag & 0x000f;
	if(flag & 0x0010){
		m_nSpaceKind = SPACEDATA;
	}
	else{
		m_nSpaceKind = SPACEWAKU;
	}
	m_pcFitingVect->SetAtrDXY(0, 0, x1, y1);
	m_pcFitingVect->SetAtrDXY(1, 0, x2, y2);
	m_pcFitingVect->SetPointCount(2);
	m_pcFitingVect->OffFlag(REJIONFLAG);
	SetStrStartXY(x1, y1);
	SetStrLen(0.0);
	SetFontName(szFontName);
	if(TCHARcount <= 0){
		TCHARcount = 0;
		SetOderKind(ODEREQUAL);
	}else{
		SetOderKind(ODERLEFT);
	}
	SetMaxCharCount(TCHARcount);
	SetString(szInStr);
	RenewMiniMax();
	if(szInStr[0] != _T('\0')){
		StrgLineStruct();
		RenewMiniMax();
	}
}

void CSWakVect::YokoGumiStructTrnsWaku(CMojiVect *pcMojiTbl[], int nCntMax)
{
	CMojiVect	*pcMojiVect;
	DBL		setlen;
	DBL		rag, dx, dy;
	DBL		minix, miniy, maxx, maxy;
	DBL		dW, dH;
	DBL		sx, sy, ex, ey;
	int		idx;
	DBL		step, tt;
	DBL		cx, cy;
	DBL		movex, movey;
	DBL		tsin, tcos;

	pcMojiVect = pcMojiTbl[0];
	setlen = pcMojiVect->GetLenFromStrgTop();
	rag = GetVectPointRagAndCood(&setlen, &dx, &dy);
	if(CheckTateYoko() == TATEGAKI){
		rag += M_PI_2;
	}
	pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
	dW = (maxx - minix) / nCntMax;
	dH = maxy - miniy;
	sx = dx - (maxx - minix) / 2;
	ex = dx + (maxx - minix) / 2;
	sy = dy;
	ey = dy;
	curveroundtrns(sx, sy, dx, dy, rag, &sx, &sy);
	curveroundtrns(ex, ey, dx, dy, rag, &ex, &ey);
	step = 1.0 / nCntMax;
	tt = step / 2.0;
	for(idx = 0; idx < nCntMax; idx++){
		dx = (ex - sx) * tt + sx;
		dy = (ey - sy) * tt + sy;
		pcMojiVect = pcMojiTbl[idx];
		pcMojiVect->SetFontSize(dW, dH);
		pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
		cx = (minix + maxx) / 2;
		cy = (miniy + maxy) / 2;
		movex = dx - cx;
		movey = dy - cy;
		tsin = sin(rag);
		tcos = cos(rag);
		pcMojiVect->MoveTrns(movex, movey);
		pcMojiVect->RoundTrns(dx, dy, tsin, tcos);
		tt += step;
	}
}

void CSWakVect::YokoGumiStructTrnsData(CMojiVect *pcMojiTbl[], int nCntMax)
{
	CMojiVect	*pcMojiVect;
	DBL		setlen;
	DBL		rag, dx, dy;
	DBL		minix, miniy, maxx, maxy;
	DBL		miniwx, miniwy, maxwx, maxwy;
	DBL		minidx, minidy, maxdx, maxdy;
	DBL		dW, dH;
	DBL		dSumLen, dCrtPos;
	DBL		sx, sy, ex, ey;
	int		idx;
	DBL		cx, cy;
	DBL		movex, movey;
	DBL		tsin, tcos;

	pcMojiVect = pcMojiTbl[0];
	setlen = pcMojiVect->GetLenFromStrgTop();
	rag = GetVectPointRagAndCood(&setlen, &dx, &dy);
	if(CheckTateYoko() == TATEGAKI){
		rag += M_PI_2;
	}
	pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
	dSumLen = GetTableDataSize(pcMojiTbl, nCntMax) * 1.2;
	dW = (maxx - minix) * (maxx - minix) / dSumLen;
	dH = maxy - miniy;
	dSumLen = (maxx - minix);
	sx = dx - (maxx - minix) / 2;
	ex = dx + (maxx - minix) / 2;
	sy = dy;
	ey = dy;
	curveroundtrns(sx, sy, dx, dy, rag, &sx, &sy);
	curveroundtrns(ex, ey, dx, dy, rag, &ex, &ey);
	dCrtPos = 0;
	for(idx = 0; idx < nCntMax; idx++){
		pcMojiVect = pcMojiTbl[idx];
		pcMojiVect->SetFontSize(dW, dH);
		pcMojiVect->GetWakuDMiniMax(&miniwx, &miniwy, &maxwx, &maxwy);
		pcMojiVect->GetDataDMiniMax(&minidx, &minidy, &maxdx, &maxdy);
		dCrtPos += (maxdx - minidx) / 2.0 * 1.2;
		dx = (ex - sx) * dCrtPos / dSumLen + sx;
		dy = (ey - sy) * dCrtPos / dSumLen + sy;
		cx = (maxdx + minidx) / 2.0;
		cy = (maxwy + miniwy) / 2.0;
		movex = dx - cx;
		movey = dy - cy;
		tsin = sin(rag);
		tcos = cos(rag);
		pcMojiVect->MoveTrns(movex, movey);
		pcMojiVect->RoundTrns(dx, dy, tsin, tcos);
		dCrtPos += (maxdx - minidx) / 2.0 * 1.2;
	}
}

DBL CSWakVect::GetTableDataSize(CMojiVect *pcMojiTbl[], int nCntMax)
{
	CMojiVect	*pcMojiVect;
	int		idx;
	DBL		sum;
	DBL		minix, miniy, maxx, maxy;

	sum = 0;
	for(idx = 0; idx < nCntMax; idx++){
		pcMojiVect = pcMojiTbl[idx];
		pcMojiVect->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
		sum += (maxx - minix);
	}
	return(sum);
}
