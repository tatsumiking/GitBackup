
//
// Easy Scanner System
//
// object wakuvect.cpp  ògì‡ï∂éöóÒä÷åWä÷êî
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"

#include "Data.h"
#include "Vect.h"
#include "DataList.h"
#include "MojiVect.h"
#include "StrgVect.h"
#include "WakuVect.h"

#include	"../ObjLib/Baselib.h"
#include	"../ObjLib/Senvlib.h"
#include	"../ObjLib/Scrnlib.h"
#include	"../ObjLib/Wakuenv.h"

IMPLEMENT_SERIAL(CWakuVect, CObject, 3)
CWakuVect::CWakuVect()
{
}

void CWakuVect::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << CString(m_szMatching);
		ar << m_dTrns;
		ar << m_nFixFlag;
		// ar << 
	}
	else
	{
		CString	csMatching;
		ar >> csMatching;
		_tcscpy_s(m_szMatching, csMatching);
		ar >> m_dTrns;
		ar >> m_nFixFlag;
		// ar >>
	}
	CStrgVect::Serialize(ar);
	if (!(ar.IsStoring()))
	{
		RenewMiniMax();
	}
}

CWakuVect::CWakuVect(int nDummy)
	: CStrgVect(nDummy)
{
	m_nWakuNo = 0;	
	m_szMatching[0] = _T('\0');
	//m_dTrns = 0.5;
	m_dTrns = 0.1;
	m_nFixFlag = 0;
}

CWakuVect::~CWakuVect()
{
}

int CWakuVect::CheckID()
{
	return(WAKUVECTID);
}

//p ì‡ïîÉfÅ[É^Å[Ç™ãÛÇÃèÛë‘Ç≈Ç†ÇÍÇŒçÌèúÇµÇ∑Ç◊ÇƒÇ™ãÛÇÃÇ∆Ç´TRUEÇï‘Ç∑
int CWakuVect::CheckNullData()
{
	// ï∂éöóÒÇê≥ãKâªÇ∑ÇÈ
	CStrgVect::CheckNullData();
	return(FALSE);
}

void CWakuVect::AtntionDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	scrnsetslctpen(pcDC->m_hDC);
	StockClrDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CWakuVect::NoactDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	scrnsetnoslctpen(pcDC->m_hDC);
	StockClrDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CWakuVect::DelDraw(CDC *pcDC)
{
	scrnsetdelpen(pcDC->m_hDC);
	StockClrDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CWakuVect::OnDraw(CDC *pcDC)
{
	int		mode;
	int		nlimit;

	nlimit = senvgetplgnlimit();
	if(senvchckstopflag() == ON){
		return;
	}
	mode = senvcheckdrawmode(); 
	if(mode != PRINTVIEW
	&& mode != DIRCTVIEW
	&& mode != METAFVIEW){
		//penno = GetFillPen();
		//senvgetpenrgb(penno, &rval, &gval, &bval);
		//scrnsetpreviewpen(pcDC->m_hDC, rval, gval, bval, nlimit);
		scrnsetfrpen(pcDC->m_hDC);
		WakuBoxsDraw(pcDC);
		scrnpenend(pcDC->m_hDC);
	}
	if(m_pcMojiTop != NULL){
		CStrgVect::OnDraw(pcDC);
	}
}

void CWakuVect::StockClrDraw(CDC *pcDC)
{
	int		mode;

	if(senvchckstopflag() == ON){
		return;
	}
	mode = senvcheckdrawmode(); 
	if(mode != PRINTVIEW
	&& mode != DIRCTVIEW
	&& mode != METAFVIEW){
		WakuBoxsDraw(pcDC);
	}
	if(m_pcMojiTop != NULL){
		//CStrgVect::OnDraw(pcDC);
		CStrgVect::StockClrDraw(pcDC);
	}
}

void CWakuVect::OutlineDraw(CDC *pcDC)
{
	if(m_pcMojiTop != NULL){
		CStrgVect::OutlineDraw(pcDC);
	}
}

void CWakuVect::WakuBoxsDraw(CDC *pcDC)
{
	DBL		sx, sy, ex, ey;
	DBL		dw, dh, dm, dl;
	DBL		dsl;

	GetStrStartXY(&sx, &sy);
	GetFontSize(&dw, &dh);
	GetSpaceEnv(&dm, &dl);
	GetStrLen(&dsl);
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		ex = sx + dw;
		ey = sy - dsl;
		BoxsDraw(pcDC, sx, sy, ex, ey);
	}
	else{
		ey = sy - dh;
		ex = sx + dsl;
		BoxsDraw(pcDC, sx, sy, ex, ey);
	}
}

void CWakuVect::WakuSave(FILE * fp)
{
	TCHAR	buf[RECMAX4];
	TCHAR	instrg[RECMAX];
	TCHAR	tatestr[10];
	TCHAR	sFName[RECMAX];
	DBL		mmxsize, mmysize;
	DBL		dsx, dsy;
	DBL		dw, dh;
	DBL		dsl;
	DBL		wd, hi;
	DBL		dms, dls;
	int		fillpen, linepen;
	int		rval, gval, bval;
	long	fillclr, lineclr;
	DBL		dcnt;
	DBL		thick;
	int		flag;
	int		yokomoji;

	scrngetmmsize(&mmxsize, &mmysize);
	GetFontName(sFName);
	GetStrStartXY(&dsx, &dsy); 
	GetFontSize(&dw, &dh);
	GetStrLen(&dsl);
	GetSpaceEnv(&dms, &dls);
	yokomoji = CheckYokoMojiFlag();
	fillpen = GetFillPen();
	linepen = GetOutLinePen();
	if(fillpen == -1){
		fillclr = -1;
	}
	else{
		if(fillpen == 0){
			rval = 0; gval = 0; bval = 0;
		}
		else{
			senvgetpenrgb(fillpen, &rval, &gval, &bval);
		}
		fillclr = RGB(rval, gval, bval);
	}
	if(linepen == -1){
		lineclr = -1;
	}
	else{
		if(linepen == 0){
			rval = 0; gval = 0; bval = 0;
		}
		else{
			senvgetpenrgb(linepen, &rval, &gval, &bval);
		}
		lineclr = RGB(rval, gval, bval);
	}
	dcnt = GetMaxCharCount();
	thick = GetOutLineWidth();
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		_tcscpy_s(tatestr, _T("èc"));
		wd = dw;
		hi = dsl;
		dsy = mmysize - dsy;
	}
	else{
		_tcscpy_s(tatestr, _T("â°"));
		wd = dsl;
		hi = dh;
		dsy = mmysize - dsy;
	}
	GetString(instrg);
	if(CheckSpaceKind() == ON){
		flag = 0x0010 | m_nFixFlag;
	}
	else{
		flag = m_nFixFlag;
	}
	_stprintf_s(buf, RECMAX4, _T("\"%s\",\"%s\",\"%s\", %4.1lf, %4.1lf, %4.1lf, %4.1lf, \"%s\", %ld, %ld, %2.1lf, %2.1lf, %x, %2.1lf, %2.1lf, %d\n"),
		m_szMatching, instrg, sFName, dsx, dsy, wd, hi, tatestr, fillclr, lineclr, dcnt, thick, flag, m_dTrns, dms, yokomoji);
	baselibFputs(buf, fp);
}

void CWakuVect::WakuLoad(FILE *fp, LPTSTR buf)
{
	TCHAR	*sp;
	TCHAR	instr[RECMAX];
	TCHAR	sFName[HRECMAX];
	TCHAR	numstr[HRECMAX];
	int		len;
	DBL		dsx, dsy;
	DBL		dwd, dhi;
	int		bfillpenno, blinepenno;
	DBL		blinethick;
	long	fillclr, lineclr;
	TCHAR	tatestr[10];
	DBL		TCHARcount;
	int		fillpenno, linepenno;
	DBL		linethick;
	DBL		mmxsize, mmysize;
	DBL		dw, dh;
	DBL		dsl;
	DBL		dms, dls;
	int		r, g, b;
	int		flag;
	int		yokomoji;

	dms = 0;
	dls = 0;

	bfillpenno = senvgetcrtfillpenno();
	blinepenno = senvgetcrtlinepenno();
	blinethick = senvgetcrtlinethick();

	sp = buf;
	sp = basecsvstrdivcpy(sp, m_szMatching);
	sp = basecsvstrdivcpy(sp, instr);
	sp = basecsvstrdivcpy(sp, sFName);
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
	// 040415 í«â¡
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &dms);
	// 040415 í«â¡
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%d"), &yokomoji);

	m_nFixFlag = flag & 0x000f;
	if(flag & 0x0010){
		SetSpaceKind(ON);
	}
	else{
		SetSpaceKind(OFF);
	}
	scrngetmmsize(&mmxsize, &mmysize);
	len = _tcslen(_T("èc"));
	if(_tcsnccmp(tatestr, _T("èc"), len) == 0){
		dw = dwd;
		dh = dwd;
		dsl = dhi;
		SetTateYoko(TATEGAKI);
	}
	else{
		dw = dhi;
		dh = dhi;
		dsl = dwd;
		SetTateYoko(YOKOGAKI);
	}
	dsy = mmysize - dsy;
	SetFontName(sFName);
	SetStrStartXY(dsx, dsy); 
	SetFontSize(dw, dh);
	SetStrLen(dsl);
	SetSpaceEnv(dms, dls);
	SetYokoMojiFlag(yokomoji);
	if(fillclr == -1){
		fillpenno = -1;
	}
	else{
		r = GetRValue(fillclr);
		g = GetGValue(fillclr);
		b = GetBValue(fillclr);
		fillpenno = senvgetpenno(r, g, b);
	}

	SetFillPen(fillpenno);
	if(lineclr == -1){
		linepenno = -1;
	}
	else{
		r = GetRValue(lineclr);
		g = GetGValue(lineclr);
		b = GetBValue(lineclr);
		linepenno = senvgetpenno(r, g, b);
	}
	SetOutLinePen(linepenno);
	SetMaxCharCount(TCHARcount);
	SetOutLineWidth(linethick);
	senvsetcrtfillpenno(fillpenno);
	senvsetcrtlinepenno(linepenno);
	senvsetcrtlinethick(linethick);
	SetString(instr);

	senvsetcrtfillpenno(bfillpenno);
	senvsetcrtlinepenno(blinepenno);
	senvsetcrtlinethick(blinethick);
}

void CWakuVect::OldWakuLoad(FILE *fp, LPTSTR buf)
{
	DBL		xsize, ysize;
	int		bfillpenno, blinepenno;
	DBL		blinethick;
	TCHAR	*sp;
	int		flag;
	TCHAR	szDmy[RECMAX];
	int		nDmy;
	DBL		dDmy;
	int		atr;
	DBL		sx, sy, ex, ey;
	int		fillpenno, linepenno;
	DBL		linethick;
	TCHAR	szFontName[RECMAX];
	DBL		dsx, dsy;
	DBL		dw, dh, dsl;
	DBL		dms, dls;
	UINT	code, hc, lc;
	int		idx;
	TCHAR	instr[RECMAX];
	int		len1, len2;

	wakuenvgetpapersize(&xsize, &ysize);
	bfillpenno = senvgetcrtfillpenno();
	blinepenno = senvgetcrtlinepenno();
	blinethick = senvgetcrtlinethick();
	sp = buf;
	if(basestrgcntget(buf) < 12){
		//_stscanf_s(buffer, "%s %x %d", temp, &flag, &space);
		sp = basesptostrg(sp, szDmy);
		sp = basesptoinum(sp, &flag);
		sp = basesptodnum(sp, &dms);
		_tcscpy_s(szFontName, _T("ÇdÇrû≤èëÇe"));
		dls = 0;
		fillpenno = 0; linepenno = 0; linethick = 0;
		baselibCrDelFgets(buf, RECMAX, fp);		// mmvct_start 39
		baselibCrDelFgets(buf, RECMAX, fp);		// atr x1, y1
		_stscanf_s(buf, _T("%x %lf %lf\n"), &atr, &sx, &sy);
		baselibCrDelFgets(buf, RECMAX, fp);		// atr x2, y2
		baselibCrDelFgets(buf, RECMAX, fp);		// atr x3, y3
		_stscanf_s(buf, _T("%x %lf %lf\n"), &atr, &ex, &ey);
		baselibCrDelFgets(buf, RECMAX, fp);		// atr x4, y4
		baselibCrDelFgets(buf, RECMAX, fp);		// mmvct_end
		sy = ysize - sy;
		ey = ysize - ey;
		dsx = sx;
		dsy = sy;
		if(flag & 0x02){
			dw = ex - sx;
			dh = dw;
			dsl = ey - sy;
		}
		else{
			dw = ey - sy;
			dh = dw;
			dsl = ex - sx;
		}
	}
	else{
		//_stscanf_s(buf, "%s %x %d %d %lf %s %lf %lf %lf %lf %lf %lf %lf %d %d %lf %lf %d %d",
		//				temp, &flag, &nfillpenno, &nlinepenno, &dlinethick, szFontName,
		//				&dFHight, &dFWidth, &dMojiSpace, &dLineSpace, &dStartX, &dStartY, &dStrgLen,
		//				&nukiflag, &kageflag, &dxadd, &dyadd, &hogosw, &vflag);
		sp = basesptostrg(sp, szDmy);
		sp = basesptoinum(sp, &flag);
		sp = basesptoinum(sp, &fillpenno);
		sp = basesptoinum(sp, &linepenno);
		sp = basesptodnum(sp, &linethick);
		sp = basesptostrg(sp, szFontName);
		sp = basesptodnum(sp, &dh);
		sp = basesptodnum(sp, &dw);
		sp = basesptodnum(sp, &dms);
		sp = basesptodnum(sp, &dls);
		sp = basesptodnum(sp, &dsx);
		sp = basesptodnum(sp, &dsy);
		sp = basesptodnum(sp, &dsl);
		sp = basesptoinum(sp, &nDmy);	// nukiflag
		sp = basesptoinum(sp, &nDmy);	// kageflag
		sp = basesptodnum(sp, &dDmy);	// dxadd
		sp = basesptodnum(sp, &dDmy);	// dyadd
		sp = basesptoinum(sp, &nDmy);	// hogosw
		sp = basesptoinum(sp, &nDmy);	// vflag
		//dsy = ysize - dsy;
	}
	idx = 0;
	len1 = _tcslen(_T("mmwk_end"));
	len2 = _tcslen(_T("moji2start"));
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(_tcsnccmp(buf, _T("mmwk_end"), len1) == 0){
			break;
		}
		else if(_tcsnccmp(buf, _T("moji2start"), len2) == 0){
			_stscanf_s(buf, _T("%s %d %x"), szDmy, RECMAX, &nDmy, &code);
			hc = (code & 0x00ff00) >> 8;
			lc = (code & 0x00ff);
			if(hc != 0){
				instr[idx++] = hc;
				instr[idx++] = lc;
			}
			else{
				instr[idx++] = lc;
			}
		}
	}
	instr[idx] = _T('\0');

	if(flag & 0x01){
		SetTateYoko(TATEGAKI);
	}
	else{
		SetTateYoko(YOKOGAKI);
	}
	len1 = _tcslen(_T("ÇsÇsÇlÇr∫ﬁºØ∏"));
	len2 = _tcslen(_T("ÇsÇsÇlÇrñæí©"));

	if(_tcsnccmp(szFontName, _T("ÇsÇsÇlÇr∫ﬁºØ∏"), len1) == 0){
		_tcscpy_s(szFontName, _T("ÇlÇrÉSÉVÉbÉN"));
	}
	if(_tcsnccmp(szFontName, _T("ÇsÇsÇlÇrñæí©"), len2) == 0){
		_tcscpy_s(szFontName, _T("ÇlÇrñæí©"));
	}
	SetFontName(szFontName);
	SetStrStartXY(dsx, dsy); 
	SetFontSize(dw, dh);
	SetStrLen(dsl);
	SetSpaceEnv(dms, dls);
	SetFillPen(fillpenno);
	SetOutLinePen(linepenno);
	SetOutLineWidth(linethick);
	senvsetcrtfillpenno(fillpenno);
	senvsetcrtlinepenno(linepenno);
	senvsetcrtlinethick(linethick);

	SetString(instr);

	senvsetcrtfillpenno(bfillpenno);
	senvsetcrtlinepenno(blinepenno);
	senvsetcrtlinethick(blinethick);
}

void CWakuVect::SetMatchStrg(LPTSTR mstrg)
{
	_tcscpy_s(m_szMatching, mstrg);
}

void CWakuVect::GetMatchStrg(LPTSTR mstrg)
{
	int len = _tcslen(m_szMatching) + 1;
	_tcscpy_s(mstrg, len, m_szMatching);
}

int CWakuVect::CheckFixFlag()
{
	return(m_nFixFlag);
}

void CWakuVect::SetFixFlag(int flag)
{
	m_nFixFlag = flag;
}

void CWakuVect::SetTrnsPar(DBL par)
{
	m_dTrns = par;
}

void CWakuVect::GetTrnsPar(LPDBL par)
{
	*par = m_dTrns;
}

void CWakuVect::SetString(TCHAR *pstr)
{
	CStrgVect::SetString(pstr);
}

void CWakuVect::SaveStrgStoreFile(FILE * fp, int id)
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

void CWakuVect::LoadStrgStoreFile(FILE * fp, int id)
{
	TCHAR	strg[RECMAX];

	if(id == CheckID()){
		if(fread(strg, RECMAX, 1, fp) != 1){
			return;
		}
		if(_tcscmp(strg, m_sString) != 0){
			SetString(strg);
		}
	}
}

CData* CWakuVect::CopyData(DBL movex, DBL movey)
{
	CWakuVect *pcnewData;
	TCHAR	fontname[FNAMEMAX];
	DBL		dstartx, dstarty;
	DBL		dwidth, dhight;
	DBL		d_tcslen;
	DBL		dmojispace, dstrgspace;

	pcnewData = (CWakuVect*)NewDataCreate(WAKUVECTID);			

	GetSpaceEnv(&dmojispace, &dstrgspace);
	GetStrStartXY(&dstartx, &dstarty);
	GetFontSize(&dwidth, &dhight);
	GetStrLen(&d_tcslen);
	GetFontName(fontname);

	pcnewData->SetSpaceEnv(dmojispace, dstrgspace);
	pcnewData->SetStrStartXY(dstartx+movex, dstarty+movey);
	pcnewData->SetFontSize(dwidth, dhight);
	pcnewData->SetStrLen(d_tcslen);
	pcnewData->m_nStrgFlag = m_nStrgFlag;
	pcnewData->m_nYokoGumi = m_nYokoGumi;
	pcnewData->m_nStrgFlag = m_nStrgFlag;
	pcnewData->SetFontName(fontname);
	pcnewData->SetString(m_sString);
	pcnewData->SetFillPen(m_nFillPen);
	pcnewData->SetOutLinePen(m_nOutLinePen);
	pcnewData->SetOutLineWidth(m_dOutLineWidth);
	pcnewData->RenewMiniMax();
	return((CData*)pcnewData);
}

int CWakuVect::DrawNo(CDC *pcDC, int noidx)
{
	DBL		sx, sy;
	TCHAR	buf[16];

	m_nWakuNo = noidx;
	_stprintf_s(buf, _T("%d"), m_nWakuNo);
	GetStrStartXY(&sx, &sy);
	scrndxymmtorltv(&sx, &sy);
	if(pcDC != NULL){
		::TextOut(pcDC->m_hDC,(int)sx, (int)sy,  buf, _tcslen(buf));
	}
	noidx++;
	return(noidx);
}

void CWakuVect::GetDMiniMax(DBL * minx, DBL * miny, DBL * maxx, DBL * maxy)
{
	DBL		dstartx, dstarty;
	DBL		dwidth, dhight;
	DBL		d_tcslen;
	DBL		ex, ey;
	
	GetStrStartXY(&dstartx, &dstarty);
	(*minx) = dstartx;
	(*miny) = dstarty;
	(*maxx) = dstartx;
	(*maxy) = dstarty;
	/*
	CStrgVect::GetDMiniMax(&tminx, &tminy, &tmaxx, &tmaxy);
	(*minx) = min((*minx), tminx);
	(*maxx) = max((*maxx), tmaxx);
	(*miny) = min((*miny), tminy);
	(*maxy) = max((*maxy), tmaxy);
	*/
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		GetFontSize(&dwidth, &dhight);
		GetStrLen(&d_tcslen);
		ex = dstartx + dwidth;
		ey = dstarty - d_tcslen;
	}
	else{
		GetFontSize(&dwidth, &dhight);
		GetStrLen(&d_tcslen);
		ex = dstartx + d_tcslen;
		ey = dstarty - dhight;
	}
	(*minx) = min((*minx), ex);
	(*maxx) = max((*maxx), ex);
	(*miny) = min((*miny), ey);
	(*maxy) = max((*maxy), ey);
}

void CWakuVect::SelectDraw(CDC *pcDC)
{
	DBL		dwidth, dhight;
	DBL		d_tcslen;
	DBL		sx, sy, ex, ey;

	GetStrStartXY(&sx, &sy);
	GetFontSize(&dwidth, &dhight);
	GetStrLen(&d_tcslen);
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		GetFontSize(&dwidth, &dhight);
		GetStrLen(&d_tcslen);
		ex = sx + dwidth;
		ey = sy - d_tcslen;
	}
	else{
		GetFontSize(&dwidth, &dhight);
		GetStrLen(&d_tcslen);
		ex = sx + d_tcslen;
		ey = sy - dhight;
	}
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);

	scrnsetatnpen(pcDC->m_hDC);
	pcDC->MoveTo((long)sx, (long)sy); 
	pcDC->LineTo((long)sx, (long)ey); 
	pcDC->LineTo((long)ex, (long)ey); 
	pcDC->LineTo((long)ex, (long)sy); 
	pcDC->LineTo((long)sx, (long)sy); 
	scrnpenend(pcDC->m_hDC);
}

void CWakuVect::NoSelectDraw(CDC *pcDC)
{
	DBL		dwidth, dhight;
	DBL		d_tcslen;
	DBL		sx, sy, ex, ey;

	GetStrStartXY(&sx, &sy);
	GetFontSize(&dwidth, &dhight);
	GetStrLen(&d_tcslen);
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		GetFontSize(&dwidth, &dhight);
		GetStrLen(&d_tcslen);
		ex = sx + dwidth;
		ey = sy - d_tcslen;
	}
	else{
		GetFontSize(&dwidth, &dhight);
		GetStrLen(&d_tcslen);
		ex = sx + d_tcslen;
		ey = sy - dhight;
	}
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);

	scrnsetslctpen(pcDC->m_hDC);
	pcDC->MoveTo((long)sx, (long)sy); 
	pcDC->LineTo((long)sx, (long)ey); 
	pcDC->LineTo((long)ex, (long)ey); 
	pcDC->LineTo((long)ex, (long)sy); 
	pcDC->LineTo((long)sx, (long)sy); 
	scrnpenend(pcDC->m_hDC);
}

void CWakuVect::DelBoxs(CDC *pcDC)
{
	CRect	rect;
	DBL		sx, sy, ex, ey;
	DBL		dw, dh, dm, dl;
	DBL		dsl;

	GetStrStartXY(&sx, &sy);
	GetFontSize(&dw, &dh);
	GetSpaceEnv(&dm, &dl);
	GetStrLen(&dsl);
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == YOKOOKI){
		ex = sx + dw;
		ey = sy - dsl;
	}
	else{
		ey = sy - dh;
		ex = sx + dsl;
	}
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	rect.left = (long)sx;
	rect.right = (long)ex;
	rect.top = (long)sy;
	rect.bottom = (long)ey;
	pcDC->FillSolidRect(&rect, RGB(255, 255, 255));

}

