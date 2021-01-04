//
// Easy Scanner System
//
// object strgvect.cpp  文字列フォント構造関係関数
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"

#include "Data.h"
#include "Vect.h"
#include "DataList.h"
#include "DataGrup.h"
#include "MojiVect.h"
#include "StrgVect.h"


#include "ESF0Font.h"
#include "ESF1Font.h"
#include "WinTTFont.h"
#include "ARCFont.h"
#include "ARKFont.h"
#include "SCRFont.h"

#include	"../ObjLib/Baselib.h"
#include	"../ObjLib/Senvlib.h"
#include	"../ObjLib/Scrnlib.h"
#include	"../ObjLib/Mojienv.h"
#include	"../ObjLib/Pout.h"

#define		SPACEKINDFLAG	0x0010

IMPLEMENT_SERIAL(CStrgVect, CObject, 3)
CStrgVect::CStrgVect()
{
}

// そのうちにm_pcMojiTopをm_pcDataTopに
// SetMojiLinkをSetDataにする
CRect* CStrgVect::GetBoundingRect()
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
	m_rectData.SetRect(sx, sy, ex, ey);
	return(&m_rectData);
}

CStrgVect::CStrgVect(int nDummy)
	: CVect(nDummy)
{
	m_sFontName[0] = _T('\0');
	m_sString[0] = _T('\0');
	SetTateYoko(YOKOGAKI);
	SetSpaceKind(0);
	m_nStrgFlag = 0;
	m_nYokoGumi = 0;
	m_nFlag3 = 0;
	m_nFlag4 = 0;
	m_pcMojiTop = NULL;
	m_dCharCount = 0.0;
	mojienvgettrnslimit(&m_dTrns);
	SetPointCount(4);
	SetFontSize(102.4, 102.4);
	SetSpaceEnv(0.0, 0.0);
	SetStrStartXY(0.0, 0.0);
	SetStrLen(0.0);
	m_nShadowKind = 0;
	m_clrShadow = RGB(0, 0, 0);
	m_dShadowDistance = 0;
	m_dShadowRoundRag = 0;
}

CStrgVect::~CStrgVect()
{
	AllFree();
}

int CStrgVect::CheckID()
{
	return(STRGVECTID);
}

void CStrgVect::Serialize(CArchive& ar)
{
	CMojiVect*	pcMoji;
	int			nID;

	if (ar.IsStoring())
	{
		ar << CString(m_sFontName);
		ar << m_nStrgFlag;
		ar << m_nYokoGumi;
		ar << m_nFlag3;
		ar << m_nFlag4;
		ar << CString(m_sString);
		ar << m_dCharCount;
		ar << m_rectStrg;
		pcMoji = m_pcMojiTop;
		while(1){
			if(pcMoji == NULL){
				nID = NOOPDATAID;
				ar << nID;
				break;
			}
			else{
				nID = pcMoji->CheckID();
				ar << nID;
			}
			pcMoji->Serialize(ar);
			pcMoji = pcMoji->GetNextMoji();
		}
		// ar << 
	}
	else
	{
		CString csFontName;
		CString csString;
		ar >> csFontName;
		_tcscpy_s(m_sFontName, csFontName);
		ar >> m_nStrgFlag;
		ar >> m_nYokoGumi;
		ar >> m_nFlag3;
		ar >> m_nFlag4;
		ar >> csString;
		_tcscpy_s(m_sString, csString);
		ar >> m_dCharCount;
		if(m_dCharCount < 0 || 100 < m_dCharCount){
			m_dCharCount = 0;
		}
		ar >> m_rectStrg;
		while(1){
			ar >> nID;
			if(nID == NOOPDATAID){
				pcMoji = NULL;
			}
			else{
				pcMoji = (CMojiVect *)NewDataCreate(nID);
			}
			SetMojiLink(pcMoji);
			if(pcMoji == NULL){
				break;
			}
			pcMoji->Serialize(ar);
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
int CStrgVect::CheckNullData()
{
	CTypedPtrList<CObList,CMojiVect*>     cLnkPtrList;
	CTypedPtrList<CObList,CMojiVect*>     cDelPtrList;
	POSITION	pos;
	CMojiVect*	pcMoji;
	int			idx;
	int			code;

	if(m_pcMojiTop == NULL){
		return(TRUE);
	}
	if(m_pcMojiTop != NULL){
		pcMoji = m_pcMojiTop;
		while(1){
			if(pcMoji == NULL){
				break;
			}
			if(pcMoji->CheckNullData() == TRUE){
				cDelPtrList.AddTail(pcMoji);
			}
			else{
				cLnkPtrList.AddTail(pcMoji);
			}
			pcMoji = pcMoji->GetNextMoji();
		}
		pos = cDelPtrList.GetHeadPosition();
		while (pos != NULL)
		{
			pcMoji = cDelPtrList.GetNext(pos);
			if(pcMoji == NULL){
				break;
			}
			delete(pcMoji);
		}

		m_pcMojiTop = NULL;
		idx = 0;
		m_dCharCount = 0;
		pos = cLnkPtrList.GetHeadPosition();
		while (pos != NULL)
		{
			pcMoji = cLnkPtrList.GetNext(pos);
			if(pcMoji == NULL){
				break;
			}
			code = pcMoji->m_nBaseCode;
			if(code < 0x0100){
				m_sString[idx++] = code;
			}
			else{
				m_sString[idx++] = (code >> 8) & 0x00ff;
				m_sString[idx++] = code & 0x00ff;
			}
			m_dCharCount++;		
			SetMojiLink(pcMoji);
		}
	}
	m_sString[idx+0] = _T('\0');
	m_sString[idx+1] = _T('\0');
	return(FALSE);
}

//P 文字を文字列の最後に追加する
int CStrgVect::SetMojiLink(CMojiVect *pcSetMoji)
{
	CMojiVect*	pcMoji;
	CMojiVect*	pcPreMoji;
	int		idx;

	idx = 0;
	pcMoji = m_pcMojiTop;
	if(pcMoji == NULL){
		m_pcMojiTop = pcSetMoji;
		return(idx);
	}
	while(1){
		idx++;
		pcPreMoji = pcMoji;
		pcMoji = (CMojiVect *)pcPreMoji->GetNextMoji();
		if(pcMoji == NULL){
			pcPreMoji->SetNextMoji(pcSetMoji);
			return(idx);
			break;
		}
	}
	return(0);
}

void CStrgVect::SetSpaceKind(int flag)
{
	if(flag == 0){
		m_nStrgFlag &=  ~SPACEKINDFLAG;
	}
	else{
		m_nStrgFlag |=  SPACEKINDFLAG;
	}
}

int CStrgVect::CheckSpaceKind()
{
	if(m_nStrgFlag & SPACEKINDFLAG){
		return(ON);
	}
	else{
		return(OFF);
	}
}

void CStrgVect::SetTateYoko(int flag)
{
	m_nStrgFlag &= 0xf0;
	m_nStrgFlag |= flag;
}

int CStrgVect::CheckTateYoko()
{
	return(m_nStrgFlag & 0x0f);
}

void CStrgVect::SetYokoMojiFlag(int flag)
{
	m_nYokoGumi = flag;
}

int CStrgVect::CheckYokoMojiFlag()
{
	return(m_nYokoGumi);
}

void CStrgVect::SetMaxCharCount(DBL cnt)
{
	m_dCharCount = cnt;
}

DBL CStrgVect::GetMaxCharCount()
{
	return(m_dCharCount);
}

void CStrgVect::SetFontName(LPTSTR sFname)
{
	_tcscpy_s(m_sFontName, sFname);
}

void CStrgVect::GetFontName(LPTSTR sFname)
{
	int len = _tcslen(m_sFontName) + 1;
	_tcscpy_s(sFname, len, m_sFontName);
}

void CStrgVect::SetFontSize(DBL dFWsize, DBL dFHsize)
{
	SetDXY(0, dFWsize, dFHsize);
}

void CStrgVect::GetFontSize(DBL *dFWsize, DBL *dFHsize)
{
	GetDXY(0, dFWsize, dFHsize);
}

void CStrgVect::SetSpaceEnv(DBL dMSpace, DBL dLSpace)
{
	SetDXY(1, dMSpace, dLSpace);
}

void CStrgVect::GetSpaceEnv(DBL *dMSpace, DBL *dLSpace)
{
	GetDXY(1, dMSpace, dLSpace);
}

void CStrgVect::SetStrStartXY(DBL dSx, DBL dSy)
{
	SetDXY(2, dSx, dSy);
}

void CStrgVect::GetStrStartXY(DBL *dSx, DBL *dSy)
{
	GetDXY(2, dSx, dSy);
}

void CStrgVect::SetStrLen(DBL dStrLen)
{
	SetDXY(3, dStrLen, dStrLen);
}

void CStrgVect::GetStrLen(DBL *dStrLen)
{
	DBL		dummy;

	GetDXY(3, dStrLen, &dummy);
}

void CStrgVect::SetTrnsPar(DBL par)
{
	m_dTrns = par;
	mojienvsettrnslimit(m_dTrns);
}

void CStrgVect::GetTrnsPar(LPDBL par)
{
	*par = m_dTrns;
}

void CStrgVect::ChangeFont(LPTSTR sFname)
{
	TCHAR	szStr[RECMAX];

	SetFontName(sFname);
	GetString(szStr);
	SetString(szStr);
}

void CStrgVect::AllFree()
{
	AllMojiFree();
}

void CStrgVect::AllMojiFree()
{
	CMojiVect*	pcMojiVect;
	CMojiVect*	nextpcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		nextpcMojiVect = pcMojiVect->GetNextMoji();
		delete(pcMojiVect);
		pcMojiVect = nextpcMojiVect;
	}
	m_pcMojiTop = NULL;
}

CData *CStrgVect::GetIdxMoji(int nidx)
{
	int			idx;
	CMojiVect*	pcMojiVect;

	idx = 0;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(idx == nidx){
			return(pcMojiVect);
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
		idx++;
	}
	return(NULL);
}

void CStrgVect::CheckFlagDraw(int flag, CDC *pcDC)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->CheckFlagDraw(flag, pcDC);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->CheckFlagDrawSetClr(flag, pcDC, clr1, clr2, thick);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::AtntionDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	scrnsetslctpen(pcDC->m_hDC);
	if(m_pcMojiTop == NULL){
		StringBoxsDraw(pcDC);
	}
	else{
		StockClrDraw(pcDC);
	}
	scrnpenend(pcDC->m_hDC);
}

void CStrgVect::AtntionMojiDraw(CDC *pcDC, int nidx)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = (CMojiVect*)GetIdxMoji(nidx);
	if(pcMojiVect != NULL){
		//scrnsetslctpen(pcDC->m_hDC);
		scrnsetatnpen(pcDC->m_hDC);
		pcMojiVect->StockClrDraw(pcDC);
		scrnpenend(pcDC->m_hDC);
	}
}

void CStrgVect::NoactDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	scrnsetnoslctpen(pcDC->m_hDC);
	if(m_pcMojiTop == NULL){
		StringBoxsDraw(pcDC);
	}
	else{
		StockClrDraw(pcDC);
	}
	scrnpenend(pcDC->m_hDC);
}

void CStrgVect::DelDraw(CDC *pcDC)
{
	scrnsetdelpen(pcDC->m_hDC);
	if(m_pcMojiTop == NULL){
		StringBoxsDraw(pcDC);
	}
	else{
		StockClrDraw(pcDC);
	}
	scrnpenend(pcDC->m_hDC);
}

void CStrgVect::OnDraw(CDC *pcDC)
{
	CMojiVect*	pcMojiVect;
	int		penno;
	int		rval, gval, bval;
	int		mode;
	int		nlimit;
	DBL		dAddX, dAddY;
	DBL		dLenX, dLenY;
	int		nCount;

	nlimit = senvgetplgnlimit();
	if(senvchckstopflag() == ON){
		return;
	}
	if(m_pcMojiTop == NULL){
		mode = senvcheckdrawmode(); 
		if(mode != PRINTVIEW
		&& mode != DIRCTVIEW
		&& mode != METAFVIEW){
			penno = GetOutLinePen();
			if(penno == 0){
				rval = 0; gval = 0; bval = 0;
			}
			else{
				senvgetpenrgb(penno, &rval, &gval, &bval);
			}
			scrnsetpreviewpen(pcDC->m_hDC, rval, gval, bval, nlimit);
			StringBoxsDraw(pcDC);
			scrnpenend(pcDC->m_hDC);
		}
		return;
	}
	dAddX = cos(m_dShadowRoundRag*M_1RAG) * m_dShadowDistance;
	dAddY = sin(m_dShadowRoundRag*M_1RAG) * m_dShadowDistance;
	dLenX = dAddX; dLenY = dAddY;
	scrndlenmmtorltv(&dLenX);
	scrndlenmmtorltv(&dLenY);
	if(dLenX > dLenY){
		nCount = (int)dLenX;
	}else{
		nCount = (int)dLenY;
	}
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(senvchckstopflag() == ON){
			return;
		}
		if(m_nShadowKind == 1){
			pcMojiVect->Shadow1Draw(pcDC, m_clrShadow, nCount, dAddX, dAddY);
		}else if(m_nShadowKind == 2){
			pcMojiVect->Shadow2Draw(pcDC, m_clrShadow, nCount, dAddX, dAddY);
		}
		pcMojiVect->OnDraw(pcDC);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}
void CStrgVect::GdiplusFillDraw(CGdiPlusEx *pcGpe)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while (1) {
		if (pcMojiVect == NULL) {
			break;
		}
		if (senvchckstopflag() == ON) {
			break;
		}
		pcMojiVect->GdiplusFillDraw(pcGpe);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}
void CStrgVect::OutlineDraw(CDC *pcDC)
{
	CMojiVect*	pcMojiVect;

	if(m_pcMojiTop == NULL){
		return;
	}

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->OutlineDraw(pcDC);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::SetString(TCHAR *pstr)
{
	int		len, idx;
	int		flag;
	int		code;
	int		codetbl[STRGMAX];
	CMojiVect*	pcMojiVect;
	DBL		dsloprag;
	DBL		sx, sy;
	DBL		ddx, ddy;

	if(m_sString[0] != _T('\0')){
		AllMojiFree();
		m_sString[0] = _T('\0');
	}
	len = _tcslen(pstr);
	if(len <= 0 || 240 < len){
		return;
	}

	_tcscpy_s(m_sString, pstr);
	len = basesetcodetbl(pstr, codetbl);
	mojienvgetgyakukind(&flag);
	if(flag == ON){
		SetMirrerString(codetbl, len);
	}
	codetbl[len] = 0;
	flag = CheckTateYoko();
	idx = 0;
	while(1){
		if(idx >= len){
			break;
		}
		if(codetbl[idx] == 0){
			break;
		}
		if (codetbl[idx] < 0x0100) {
#ifdef UNICODE
			code = baseCodeHanToZen(codetbl[idx]);
#else
			code = _mbbtombc(codetbl[idx]);
#endif
		}
		else {
			code = codetbl[idx];
		}

		pcMojiVect = (CMojiVect*)NewDataCreate(MOJIVECTID);
		pcMojiVect->m_nBaseCode = codetbl[idx];
		pcMojiVect->m_nCode = code;

		if(flag == TATEGAKI || flag == YOKOOKI){
			if(m_nYokoGumi == ON && pcMojiVect->IsNumCode() == TRUE){
				flag = YOKOGAKI;
			}
		}
		pcMojiVect->SetTateYoko(flag);
		m_nFontType = FontSelect(m_sFontName, flag);
		pcMojiVect->FontLoad(m_nFontType);

		SetMojiLink(pcMojiVect);

		idx++;
	}

	StringElementInit();
	mojienvgetstrgslop(&dsloprag);
	if(dsloprag != 0){
		DataChange();
		GetStrStartXY(&sx, &sy);
		if(flag == TATEGAKI || flag == YOKOOKI){
			ddx = 0;
			ddy = tan(dsloprag*M_1RAG);
			SlopTrns(sx, sy, ddx, ddy);
		}
		else{
			ddx = tan(dsloprag*M_1RAG);
			ddy = 0;
			SlopTrns(sx, sy, ddx, ddy);
		}
	}
}

void CStrgVect::GetString(TCHAR *pstr)
{
	int len = _tcslen(m_sString) + 1;
	_tcscpy_s(pstr, len, m_sString);
}

void CStrgVect::SetStringUnLoad(TCHAR *pstr)
{
	_tcscpy_s(m_sString, pstr);
}

void CStrgVect::PlotOut(int nComID)
{
	int		pno;
	CMojiVect*	pcMojiVect;

	baseDispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}
	// ペン番号を設定
	if(poutpaintmodechk() == ON){
		pno = GetFillPen();
		poutpenoutinit(pno);
	}

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->PlotOut(nComID);
		pcMojiVect = pcMojiVect->GetNextMoji();
		baseDispatch();
		if(senvchckflag(STOPFLAG) == ON){
			poutpenoutend();
			return;
		}
	}

	if(poutpaintmodechk() == ON){
		poutpenoutend();
	}
}

int CStrgVect::GetBeforeMojiIdx(int mojiidx)
{
	return(mojiidx-1);
}

int CStrgVect::GetAfterMojiIdx(int mojiidx)
{
	int			idx;
	CMojiVect*	pcMojiVect;

	idx = mojiidx+1;
	pcMojiVect = (CMojiVect*)GetIdxMoji(idx);
	if(pcMojiVect == NULL){
		idx = -1;
	}
	return(idx);
}

void CStrgVect::MoveAfterMoji(int mojiidx, DBL movex, DBL movey)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = (CMojiVect*)GetIdxMoji(mojiidx);
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->MoveTrns(movex, movey);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::GetMojiWakuMiniMax(int idx, DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = (CMojiVect*)GetIdxMoji(idx);
	if(pcMojiVect != NULL){
		pcMojiVect->GetWakuDMiniMax(minix, miniy, maxx, maxy);
	}
}

void CStrgVect::GetMojiMiniMax(int idx, DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = (CMojiVect*)GetIdxMoji(idx);
	if(pcMojiVect != NULL){
		pcMojiVect->GetDataDMiniMax(minix, miniy, maxx, maxy);
	}
}

void CStrgVect::GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	int		fSpaceKind;

	mojienvgetspacekind(&fSpaceKind);
	if(fSpaceKind == ON){
		GetDataDMiniMax(minix, miniy, maxx, maxy);
	}
	else{
		GetWakuDMiniMax(minix, miniy, maxx, maxy);
	}
}

void CStrgVect::GetWakuDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	CMojiVect*	pcMojiVect;
	DBL			tminix, tminiy, tmaxx, tmaxy;

	(*minix) = INTMAX;
	(*miniy) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->GetWakuDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
		(*minix) = min((*minix), tminix);
		(*miniy) = min((*miniy), tminiy);
		(*maxx) = max((*maxx), tmaxx);
		(*maxy) = max((*maxy), tmaxy);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::GetDataDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	CMojiVect*	pcMojiVect;
	DBL			tminix, tminiy, tmaxx, tmaxy;

	(*minix) = INTMAX;
	(*miniy) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->GetDataDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
		(*minix) = min((*minix), tminix);
		(*miniy) = min((*miniy), tminiy);
		(*maxx) = max((*maxx), tmaxx);
		(*maxy) = max((*maxy), tmaxy);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::GetDataSumLen(DBL *xsumlen, DBL *ysumlen)
{
	CMojiVect*	pcMojiVect;
	int			nYokoMojiIn;
	DBL			dW, dH;
	DBL			tminix, tminiy, tmaxx, tmaxy;
	DBL			txlen, tylen;
	int			ftate;

	(*xsumlen) = 0;
	(*ysumlen) = 0;
	nYokoMojiIn = OFF;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(pcMojiVect->IsNumCode() == TRUE && m_nYokoGumi == ON){
			if(nYokoMojiIn == OFF){
				nYokoMojiIn = ON;
				pcMojiVect->GetFontSize(&dW, &dH);
				ftate = CheckTateYoko();
				if(ftate == TATEGAKI || ftate == TATEOKI){
					(*xsumlen) += dW;
					(*ysumlen) += dW;
				}
				else{
					(*xsumlen) += dH;
					(*ysumlen) += dH;
				}
			}
		}
		else{
			nYokoMojiIn = OFF;
			pcMojiVect->GetDataDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
			txlen = tmaxx - tminix;
			tylen = tmaxy - tminiy;
			(*xsumlen) += txlen;
			(*ysumlen) += tylen;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::GetWakuSumLen(DBL *xsumlen, DBL *ysumlen)
{
	CMojiVect*	pcMojiVect;
	int			nYokoMojiIn;
	DBL			dW, dH;
	DBL			tminix, tminiy, tmaxx, tmaxy;
	DBL			txlen, tylen;
	int			ftate;

	(*xsumlen) = 0;
	(*ysumlen) = 0;
	nYokoMojiIn = OFF;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(pcMojiVect->IsNumCode() == TRUE && m_nYokoGumi == ON){
			if(nYokoMojiIn == OFF){
				nYokoMojiIn = ON;
				pcMojiVect->GetFontSize(&dW, &dH);
				ftate = CheckTateYoko();
				if(ftate == TATEGAKI || ftate == YOKOOKI){
					(*xsumlen) += dW;
					(*ysumlen) += dW;
				}
				else{
					(*xsumlen) += dH;
					(*ysumlen) += dH;
				}
			}
		}
		else{
			nYokoMojiIn = OFF;
			pcMojiVect->GetWakuDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
			txlen = tmaxx - tminix;
			tylen = tmaxy - tminiy;
			(*xsumlen) += txlen;
			(*ysumlen) += tylen;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::RenewMiniMax()
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->RenewMiniMax();
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

CData * CStrgVect::CopyData(DBL movex, DBL movey)
{
	CStrgVect *pcnewData;
	DBL		dstartx, dstarty;
	DBL		dwidth, dhight;
	DBL		d_tcslen;
	DBL		dmojispace, dstrgspace;
	CMojiVect	*pcMojiVect;
	CMojiVect	*pcnewMojiVect;

	pcnewData = (CStrgVect*)NewDataCreate(STRGVECTID);

	GetSpaceEnv(&dmojispace, &dstrgspace);
	GetStrStartXY(&dstartx, &dstarty);
	GetFontSize(&dwidth, &dhight);
	GetStrLen(&d_tcslen);

	pcnewData->SetSpaceEnv(dmojispace, dstrgspace);
	pcnewData->SetStrStartXY(dstartx+movex, dstarty+movey);
	pcnewData->SetFontSize(dwidth, dhight);
	pcnewData->SetStrLen(d_tcslen);
	pcnewData->m_nStrgFlag = m_nStrgFlag;
	pcnewData->m_nYokoGumi = m_nYokoGumi;
	pcnewData->SetFontName(m_sFontName);
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcnewMojiVect = (CMojiVect*)pcMojiVect->CopyData(movex, movey);
		pcnewMojiVect->BaseFontLink(m_pcFSF1Font);
		pcnewMojiVect->WNTTFontLink(m_pcWNTTFont);
		pcnewMojiVect->ESF0FontLink(m_pcESF0Font);
		pcnewMojiVect->ESF1FontLink(m_pcESF1Font);
		pcnewMojiVect->FBU0FontLink(m_pcFBU0Font);
		pcnewMojiVect->ARCFontLink(m_pcARCFont);
		pcnewMojiVect->ARKFontLink(m_pcARKFont);
		pcnewMojiVect->SCRFontLink(m_pcSCRFont);
		pcnewData->SetData(pcnewMojiVect);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	pcnewData->SetStringUnLoad(m_sString);
	pcnewData->SetFillPen(m_nFillPen);
	pcnewData->SetOutLinePen(m_nOutLinePen);
	pcnewData->SetOutLineWidth(m_dOutLineWidth);
	pcnewData->RenewMiniMax();
	return((CData*)pcnewData);
}

void CStrgVect::MoveTrns(DBL movex, DBL movey)
{
	CMojiVect*	pcMojiVect;
	DBL			dSx, dSy;

	GetStrStartXY(&dSx, &dSy);
	dSx = dSx + movex;
	dSy = dSy + movey;
	SetStrStartXY(dSx, dSy);
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->MoveTrns(movex, movey);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	RenewMiniMax();
}

void CStrgVect::IdxMoveTrns(int idx, DBL movex, DBL movey)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = (CMojiVect*)GetIdxMoji(idx);
	if(pcMojiVect != NULL){
		pcMojiVect->MoveTrns(movex, movey);
	}
}

void CStrgVect::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	CMojiVect*	pcMojiVect;
	DBL			dW, dH;
	DBL			dM, dL;
	DBL			dSx, dSy;
	DBL			dSL;
	int			ftate;

	GetFontSize(&dW, &dH);
	dW = dW * timex;
	dH = dH * timey;
	SetFontSize(dW, dH);

	GetStrLen(&dSL);
	GetSpaceEnv(&dM, &dL);
	ftate = CheckTateYoko();
	if(ftate == TATEGAKI || ftate == YOKOOKI){
		dM = dM * timey;
		dL = dL * timex;
		dSL = dSL * timey;
	}
	else{
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
}

void CStrgVect::IdxSizeTrns(int idx, DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = (CMojiVect*)GetIdxMoji(idx);
	if(pcMojiVect != NULL){
		pcMojiVect->SizeTrns(cntrx, cntry, timex, timey);
	}
}

void CStrgVect::SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->SlopTrns(cntrx, cntry, ddx, ddy);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::MirrorTrns(DBL a, DBL b, DBL t1, DBL t2)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->MirrorTrns(a, b, t1, t2);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	RenewMiniMax();
}

void CStrgVect::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->RoundTrns(cntrx, cntry, tsin, tcos);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	RenewMiniMax();
}

void CStrgVect::IdxRoundTrns(int idx, DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = (CMojiVect*)GetIdxMoji(idx);
	if(pcMojiVect != NULL){
		pcMojiVect->RoundTrns(cntrx, cntry, tsin, tcos);
	}
}

void CStrgVect::StrctBoxsTrns()
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->StrctBoxsTrns();
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	RenewMiniMax();
}

void CStrgVect::SetOutLineWidth(DBL dThick)
{
	CMojiVect*	pcMojiVect;

	CVect::SetOutLineWidth(dThick);
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->SetOutLineWidth(dThick);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::SetOutLinePen(int nPen)
{
	CMojiVect*	pcMojiVect;

	CVect::SetOutLinePen(nPen);
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->SetOutLinePen(nPen);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::SetFillPen(int nPen)
{
	CMojiVect*	pcMojiVect;

	CVect::SetFillPen(nPen);
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->SetFillPen(nPen);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::OnFlag(int bitflag)
{
	CMojiVect*	pcMojiVect;

	CVect::OnFlag(bitflag);
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->OnFlag(bitflag);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::OffFlag(int bitflag)
{
	CMojiVect*	pcMojiVect;

	CVect::OffFlag(bitflag);
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->OffFlag(bitflag);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::DataChange()
{
	CMojiVect*	pcMojiVect;
	TCHAR		sFaceName[FACEMAX];

	mojienvflistopen(_T("rb"));
	mojienvgetfonttypename(m_sFontName, &m_nFontType, sFaceName);
	mojienvflistclose();

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->DataChange();
		switch(m_nFontType){
		case ES0FONT:
		case ES1FONT:
		case FB0FONT:
		case ARCFONT:
		case ARKFONT:
		case MAXFONT:
			pcMojiVect->ReverceRejion();
			break;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::DataChangeSetDataGrup(CDataGrup *pcDataGrup)
{
	CMojiVect*	pcMojiVect;
	CDataList*	pcDataList;

	DataChange();
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if((pcMojiVect->m_pcDataList) != NULL){
			pcDataList = (CDataList*)(pcMojiVect->m_pcDataList)->CopyData(0.0, 0.0);
			pcDataList->RenewMiniMax();
			pcDataGrup->SetData(pcDataList);
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

BOOL CStrgVect::CheckDataChange()
{
	return(TRUE);
}
void CStrgVect::SetIdxOutLinePen(int idx, int nPen)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = (CMojiVect*)GetIdxMoji(idx);
	if(pcMojiVect != NULL){
		pcMojiVect->SetOutLinePen(nPen);
	}
}

void CStrgVect::SetIdxFillPen(int idx, int nPen)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = (CMojiVect*)GetIdxMoji(idx);
	if(pcMojiVect != NULL){
		pcMojiVect->SetFillPen(nPen);
	}
}

int CStrgVect::GetIdxOutLinePen(int idx)
{
	int			nPen;
	CMojiVect*	pcMojiVect;

	nPen = 0;
	pcMojiVect = (CMojiVect*)GetIdxMoji(idx);
	if(pcMojiVect != NULL){
		nPen = pcMojiVect->GetOutLinePen();
	}
	return(nPen);
}

int CStrgVect::GetIdxFillPen(int idx)
{
	int			nPen;
	CMojiVect*	pcMojiVect;

	nPen = 0;
	pcMojiVect = (CMojiVect*)GetIdxMoji(idx);
	if(pcMojiVect != NULL){
		nPen = pcMojiVect->GetFillPen();
	}
	return(nPen);
}

void CStrgVect::FreeIdxObject(int idx)
{
	CMojiVect*	prepcMojiVect;
	CMojiVect*	pcMojiVect;
	CMojiVect*	nextpcMojiVect;

	pcMojiVect = (CMojiVect*)GetIdxMoji(idx);
	if(pcMojiVect != NULL){
		prepcMojiVect = (CMojiVect*)GetIdxMoji(idx-1);
		nextpcMojiVect = pcMojiVect->GetNextMoji();
		if(prepcMojiVect == NULL){
			m_pcMojiTop = nextpcMojiVect;
		}
		else{
			prepcMojiVect->SetNextMoji(nextpcMojiVect);
		}
		delete(pcMojiVect);
	}
}

int CStrgVect::CheckIdxFlag(int idx, int flag)
{
	CMojiVect*	pcMojiVect;
	int			stat;

	stat = OFF;
	pcMojiVect = (CMojiVect*)GetIdxMoji(idx);
	if(pcMojiVect != NULL){
		stat = pcMojiVect->CheckFlag(flag);
	}
	return(stat);
}

void CStrgVect::OnIdxFlag(int idx, int flag)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = (CMojiVect*)GetIdxMoji(idx);
	if(pcMojiVect != NULL){
		pcMojiVect->OnFlag(flag);
	}
}

void CStrgVect::OffIdxFlag(int idx, int flag)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = (CMojiVect*)GetIdxMoji(idx);
	if(pcMojiVect != NULL){
		pcMojiVect->OffFlag(flag);
	}
}

void CStrgVect::StringBoxsDraw(CDC *pcDC)
{
	DBL		sx, sy, ex, ey;
	DBL		dw, dh, dm, dl;
	DBL		dsl;
	DBL		donedot;
	int		ftate;

	donedot = 1;
	scrndlenrltvtomm(&donedot);
	if(donedot < 1){
		donedot = 1;
	}
	GetStrStartXY(&sx, &sy);
	GetFontSize(&dw, &dh);
	GetSpaceEnv(&dm, &dl);
	GetStrLen(&dsl);
	ftate = CheckTateYoko();
	if(ftate == TATEGAKI || ftate == YOKOOKI){
		ex = sx + dw;
		if(dsl != 0){
			ey = sy - dsl;
			BoxsDraw(pcDC, sx, sy, ex, ey);
		}
		ey = sy - dh;
		BoxsDraw(pcDC, sx+donedot, sy-donedot, ex-donedot, ey+donedot);
		sy = ey - dm;
		ey = sy - dh;
		BoxsDraw(pcDC, sx+donedot, sy-donedot, ex-donedot, ey+donedot);
	}
	else{
		ey = sy - dh;
		if(dsl != 0){
			ex = sx + dsl;
			BoxsDraw(pcDC, sx, sy, ex, ey);
		}
		ex = sx + dw;
		BoxsDraw(pcDC, sx+donedot, sy-donedot, ex-donedot, ey+donedot);
		sx = ex + dm;
		ex = sx + dw;
		BoxsDraw(pcDC, sx+donedot, sy-donedot, ex-donedot, ey+donedot);
	}
}

void CStrgVect::StockClrDraw(CDC *pcDC)
{
	CMojiVect*	pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->StockClrDraw(pcDC);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::StrgFiting()
{
	StringElementInit();
}

void CStrgVect::StringElementInit()
{
	int		ftate;

	ftate = CheckTateYoko();
	if(ftate == TATEGAKI || ftate == YOKOOKI){
		if(CheckSpaceKind() == ON){
			TategakiElementInitData();
		}
		else{
			TategakiElementInitWaku();
		}
	}
	else{
		if(CheckSpaceKind() == ON){
			YokogakiElementInitData();
		}
		else{
			YokogakiElementInitWaku();
		}
	}
}

// 内部関数
void CStrgVect::TategakiElementInitWaku()
{
	SetingElementType	SET;
	CMojiVect*	pcMojiVect;
	int		pen1, pen2;
	int		nYokoMojiIn;
	CMojiVect*	pcSMojiVect;
	CMojiVect*	pcEMojiVect;

	pen1 = GetOutLinePen();
	pen2 = GetFillPen();
	GetStrStartXY(&(SET.dSX), &(SET.dSY));
	GetFontSize(&(SET.dW), &(SET.dH));
	GetSpaceEnv(&(SET.dM), &(SET.dL));
	GetStrLen(&(SET.dSL));
	GeCharCount(&(SET.hcnt), &(SET.zcnt));
	SET.lasthan = IsLastHankaku();
	if(SET.dSL != 0){	//均等割り付け
		TategakiEqualGetElementWaku(&SET);
	}
	else{
		SET.sx = SET.dSX;
		SET.sy = SET.dSY;
		SET.space = SET.dM;
		SET.width = SET.dW;
		SET.height = SET.dH;
	}

	nYokoMojiIn = OFF;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(pcMojiVect->IsZenKaku() == TRUE
		|| (TTFONT <= m_nFontType && m_nFontType <= TT4FONT)){
			if(nYokoMojiIn == OFF){
				if(m_nYokoGumi == ON){
					if(pcMojiVect->IsNumCode() == TRUE){
						nYokoMojiIn = ON;
						pcSMojiVect = pcMojiVect;
					}
				}
			}
			else if(m_nYokoGumi == ON){
				if(pcMojiVect->IsNumCode() == FALSE){
					nYokoMojiIn = OFF;
					pcEMojiVect = pcMojiVect;
					YokoMojiElementInitWaku(&SET, pcSMojiVect, pcEMojiVect);					
					SET.sy -= (SET.height + SET.space);
				}
			}

			if(nYokoMojiIn == OFF){
				if(pcMojiVect->m_nBaseCode == 0x20){
					pcMojiVect->SetStartXY(SET.sx, SET.sy-SET.height/2);
					pcMojiVect->SetFontSize(SET.width, SET.height/2);
					pcMojiVect->SetOutLinePen(pen1);
					pcMojiVect->SetFillPen(pen2);
					SET.sy -= (SET.height + SET.space)/2;
				}
				else{
					pcMojiVect->SetStartXY(SET.sx, SET.sy-SET.height);
					pcMojiVect->SetFontSize(SET.width, SET.height);
					pcMojiVect->SetOutLinePen(pen1);
					pcMojiVect->SetFillPen(pen2);
					SET.sy -= (SET.height + SET.space);
				}
			}
		}
		else{
			if(nYokoMojiIn == OFF){
				if(m_nYokoGumi == ON){
					if(pcMojiVect->IsNumCode() == TRUE){
						nYokoMojiIn = ON;
						pcSMojiVect = pcMojiVect;
					}
				}
			}
			else if(m_nYokoGumi == ON){
				if(pcMojiVect->IsNumCode() == FALSE){
					nYokoMojiIn = OFF;
					pcEMojiVect = pcMojiVect;
					YokoMojiElementInitWaku(&SET, pcSMojiVect, pcEMojiVect);					
					SET.sy -= (SET.height + SET.space);
				}
			}

			if(nYokoMojiIn == OFF){
				pcMojiVect->SetStartXY(SET.sx, SET.sy-SET.height/2);
				pcMojiVect->SetFontSize(SET.width, SET.height/2);
				pcMojiVect->SetOutLinePen(pen1);
				pcMojiVect->SetFillPen(pen2);
				SET.sy -= (SET.height + SET.space)/2;
			}
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	// 最後が数字の時の処理
	if(nYokoMojiIn == ON && m_nYokoGumi == ON){
		pcEMojiVect = NULL;
		YokoMojiElementInitWaku(&SET, pcSMojiVect, pcEMojiVect);
	}
}

void CStrgVect::TategakiElementInitData()
{
	SetingElementType	SET;
	int		nYokoMojiIn;
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;
	int		pen1, pen2;
	CMojiVect*	pcSMojiVect;
	CMojiVect*	pcEMojiVect;

	pen1 = GetOutLinePen();
	pen2 = GetFillPen();
	GetStrStartXY(&(SET.dSX), &(SET.dSY));
	GetFontSize(&(SET.dW), &(SET.dH));
	GetSpaceEnv(&(SET.dM), &(SET.dL));
	GetStrLen(&(SET.dSL));
	GeCharCount(&(SET.hcnt), &(SET.zcnt));
	SET.lasthan = IsLastHankaku();
	StrgDataSumLenGet(SET.dW, SET.dH, &(SET.dXLen), &(SET.dYLen));

	if(SET.dSL != 0){	//均等割り付け
		TategakiEqualGetElementData(&SET);
	}
	else{
		SET.sx = SET.dSX;
		SET.sy = SET.dSY;
		SET.space = SET.dM;
		SET.width = SET.dW;
		SET.height = SET.dH;
	}

	nYokoMojiIn = OFF;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(pcMojiVect->IsZenKaku() == TRUE
		|| (TTFONT <= m_nFontType && m_nFontType <= TT4FONT)){
			if(nYokoMojiIn == OFF){
				if(m_nYokoGumi == ON){
					if(pcMojiVect->IsNumCode() == TRUE){
						nYokoMojiIn = ON;
						pcSMojiVect = pcMojiVect;
					}
				}
			}
			else if(m_nYokoGumi == ON){
				if(pcMojiVect->IsNumCode() == FALSE){
					nYokoMojiIn = OFF;
					pcEMojiVect = pcMojiVect;
					YokoMojiElementInitData(&SET, pcSMojiVect, pcEMojiVect);					
					SET.sy -= (SET.height + SET.space);
				}
			}
			if(nYokoMojiIn == OFF){
				pcMojiVect->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
				if(pcMojiVect->m_nBaseCode == 0x20){
					minix *= ((SET.width/2)/FSF1MESH);
					miniy *= ((SET.height/2)/FSF1MESH);
					maxx *= ((SET.width/2)/FSF1MESH);
					maxy *= ((SET.height/2)/FSF1MESH);
				}
				else{
					minix *= (SET.width/FSF1MESH);
					miniy *= (SET.height/FSF1MESH);
					maxx *= (SET.width/FSF1MESH);
					maxy *= (SET.height/FSF1MESH);
				}
				if((SET.zcnt+SET.hcnt) == 1){
					maxy = SET.height;
				}
				pcMojiVect->SetStartXY(SET.sx, SET.sy-maxy);
				pcMojiVect->SetFontSize(SET.width, SET.height);
				pcMojiVect->SetOutLinePen(pen1);
				pcMojiVect->SetFillPen(pen2);
				SET.sy -= ((maxy - miniy) + SET.space);
			}
		}
		else{
			if(nYokoMojiIn == OFF){
				if(m_nYokoGumi == ON){
					if(pcMojiVect->IsNumCode() == TRUE){
						nYokoMojiIn = ON;
						pcSMojiVect = pcMojiVect;
					}
				}
			}
			else if(m_nYokoGumi == ON){
				if(pcMojiVect->IsNumCode() == FALSE){
					nYokoMojiIn = OFF;
					pcEMojiVect = pcMojiVect;
					YokoMojiElementInitData(&SET, pcSMojiVect, pcEMojiVect);					
					SET.sy -= (SET.height + SET.space);
				}
			}
			if(nYokoMojiIn == OFF){
				pcMojiVect->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
				minix *= ((SET.width)/FSF1MESH);
				miniy *= ((SET.height/2)/FSF1MESH);
				maxx *= ((SET.width)/FSF1MESH);
				maxy *= ((SET.height/2)/FSF1MESH);
				if((SET.zcnt+SET.hcnt) == 1){
					maxy = 0;
				}
				pcMojiVect->SetStartXY(SET.sx, SET.sy-maxy);
				pcMojiVect->SetFontSize(SET.width, SET.height/2);
				pcMojiVect->SetOutLinePen(pen1);
				pcMojiVect->SetFillPen(pen2);
				SET.sy -= ((maxy - miniy) + SET.space);
			}
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	// 最後が数字の時の処理
	if(nYokoMojiIn == ON && m_nYokoGumi == ON){
		pcEMojiVect = NULL;
		YokoMojiElementInitData(&SET, pcSMojiVect, pcEMojiVect);					
	}
}

void CStrgVect::TategakiEqualGetElementWaku(SetingElementType *SET)
{
	DBL		charcnt, bytecnt;
	DBL		splen;

	charcnt = SET->hcnt + SET->zcnt;
	if(1.0 < m_dCharCount){
		charcnt = 2;
	}

	if(charcnt == 1){
		if(fabs(SET->dSL) < fabs(SET->dH)){
			SET->sy = SET->dSY;
			SET->sx = SET->dSX;
			SET->width = SET->dW;
			SET->height = SET->dSL;
			SET->space = SET->dM;
		}
		else{
			SET->sy = SET->dSY - (SET->dSL - SET->dH) / 2;
			SET->sx = SET->dSX;
			SET->width = SET->dW;
			SET->height = SET->dH;
			SET->space = SET->dM;
		}
	}
	else{
		if(1.0 < m_dCharCount){
			bytecnt = m_dCharCount * 2;
		}
		else{
			bytecnt = SET->hcnt + SET->zcnt*2;
		}
		splen = (DBL)(SET->hcnt + SET->zcnt - 1) * SET->dM;
		if(fabs(SET->dSL) > (bytecnt * (fabs(SET->dH))/2 + splen)){
			// 最低文字間隔よりも文字列ザイズが長いとき
			if(bytecnt == 2){
				SET->space = (fabs(SET->dSL) - (bytecnt * fabs(SET->dH) / 2))*2;
			}
			else{
				if(SET->lasthan == TRUE){
					SET->space = ((fabs(SET->dSL) - (bytecnt * fabs(SET->dH) / 2)) / (DBL)(bytecnt-1)) * 2.0;
				}
				else{
					SET->space = ((fabs(SET->dSL) - (bytecnt * fabs(SET->dH) / 2)) / (DBL)(bytecnt-2)) * 2.0;
				}
			}
			SET->height = fabs(SET->dH);
			if(SET->dSL < 0){
				SET->height *= -1;
				SET->space *= -1;
			}
		}
		else{
			// 文字を平体で処理する
			SET->height = (fabs(SET->dSL)-splen) / (DBL)(bytecnt / 2);
			if(m_dTrns > (SET->height / fabs(SET->dW) * 100.0)){
				SET->height = fabs(m_dTrns * SET->dW / 100.0);
				if(SET->lasthan == TRUE){
					SET->space = ((fabs(SET->dSL) - (bytecnt * SET->height / 2)) / (DBL)(bytecnt-1)) * 2.0;
				}
				else{
					SET->space = ((fabs(SET->dSL) - (bytecnt * SET->height / 2)) / (DBL)(bytecnt-2)) * 2.0;
				}
			}
			else{
				SET->space = SET->dM;
			}
			if(SET->dSL < 0){
				SET->height *= -1;
				SET->space *= -1;
			}
		}
		SET->width = SET->dW;
		SET->sx = SET->dSX;
		SET->sy = SET->dSY;
	}	
}

void CStrgVect::TategakiEqualGetElementData(SetingElementType *SET)
{
	int		bytecnt;
	DBL		splen, tylen;

	bytecnt = SET->hcnt + SET->zcnt;

	if((SET->hcnt + SET->zcnt) == 1){	//１文字の時
		if(fabs(SET->dSL) < fabs(SET->dH)){
			SET->sy = SET->dSY;
			SET->sx = SET->dSX;
			SET->width = SET->dW;
			SET->height = SET->dSL;
			SET->space = SET->dM;
		}
		else{
			SET->sy = SET->dSY - (SET->dSL - SET->dH) / 2;
			SET->sx = SET->dSX;
			SET->width = SET->dW;
			SET->height = SET->dH;
			SET->space = SET->dM;
		}
	}
	else{
		splen = (DBL)(SET->hcnt + SET->zcnt - 1) * SET->dM;
		if(fabs(SET->dSL) > (fabs(SET->dYLen) + splen)){
			SET->space = (fabs(SET->dSL) - fabs(SET->dYLen)) / (DBL)(SET->hcnt + SET->zcnt - 1);
			SET->height = fabs(SET->dH);
			if(SET->dSL < 0){
				SET->height *= -1;
				SET->space *= -1;
			}
		}
		else{
			tylen = (fabs(SET->dSL) - splen);
			if(m_dTrns == 0){
				SET->height =  fabs(fabs(SET->dH) * tylen / fabs(SET->dYLen));
				SET->space = SET->dM;
			}
			else if(m_dTrns > (tylen / fabs(SET->dYLen) * 100.0)){
				SET->height = fabs(SET->dH) * m_dTrns / 100.0;
				tylen = fabs(SET->dYLen) * m_dTrns / 100.0;
				SET->space = (fabs(SET->dSL) - tylen) /  (DBL)((bytecnt-2)/2);
			}
			else{
				SET->height = (fabs(SET->dSL) - splen) / fabs(SET->dYLen) * fabs(SET->dH);
				SET->space = SET->dM;
			}
			if(SET->dSL < 0){
				SET->height *= -1;
				SET->space *= -1;
			}
		}
		SET->width = SET->dW;
		SET->sx = SET->dSX;
		SET->sy = SET->dSY;
	}
}

void CStrgVect::YokogakiElementInitWaku()
{
	SetingElementType	SET;
	CMojiVect*	pcMojiVect;
	int		pen1, pen2;

	GetStrStartXY(&(SET.dSX), &(SET.dSY));
	GetFontSize(&(SET.dW), &(SET.dH));
	GetSpaceEnv(&(SET.dM), &(SET.dL));
	GetStrLen(&(SET.dSL));
	SET.lasthan = IsLastHankaku();

	int bflag = m_nYokoGumi;
	m_nYokoGumi = OFF;
	GeCharCount(&(SET.hcnt), &(SET.zcnt));
	m_nYokoGumi = bflag;

	if(SET.dSL != 0){
		YokogakiEqualGetElementWaku(&SET);
	}
	else{
		SET.sx = SET.dSX;
		SET.sy = SET.dSY;
		SET.width = SET.dW;
		SET.height = SET.dH;
		SET.space = SET.dM;
	}

	pen1 = GetOutLinePen();
	pen2 = GetFillPen();
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(pcMojiVect->IsZenKaku() == TRUE
		|| (TTFONT <= m_nFontType && m_nFontType <= TT4FONT)){
			if(pcMojiVect->m_nBaseCode == 0x20){
				pcMojiVect->SetStartXY(SET.sx, SET.sy-SET.height);
				pcMojiVect->SetFontSize(SET.width/2, SET.height);
				pcMojiVect->SetOutLinePen(pen1);
				pcMojiVect->SetFillPen(pen2);
				SET.sx += (SET.width + SET.space)/2;
			}
			else{
				pcMojiVect->SetStartXY(SET.sx, SET.sy-SET.height);
				pcMojiVect->SetFontSize(SET.width, SET.height);
				pcMojiVect->SetOutLinePen(pen1);
				pcMojiVect->SetFillPen(pen2);
				SET.sx += (SET.width + SET.space);
			}
		}
		else{
			pcMojiVect->SetStartXY(SET.sx, SET.sy-SET.height);
			pcMojiVect->SetFontSize(SET.width/2, SET.height);
			pcMojiVect->SetOutLinePen(pen1);
			pcMojiVect->SetFillPen(pen2);
			SET.sx += (SET.width + SET.space)/2;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::YokogakiElementInitData()
{
	SetingElementType	SET;
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;
	int		pen1, pen2;

	GetStrStartXY(&(SET.dSX), &(SET.dSY));
	GetFontSize(&(SET.dW), &(SET.dH));
	GetSpaceEnv(&(SET.dM), &(SET.dL));
	GetStrLen(&(SET.dSL));
	SET.lasthan = IsLastHankaku();

	int bflag = m_nYokoGumi;
	m_nYokoGumi = OFF;
	GeCharCount(&(SET.hcnt), &(SET.zcnt));
	StrgDataSumLenGet(SET.dW, SET.dH, &(SET.dXLen), &(SET.dYLen));
	m_nYokoGumi = bflag;

	if(SET.dSL != 0){
		YokogakiEqualGetElementData(&SET);
	}
	else{
		SET.sx = SET.dSX;
		SET.sy = SET.dSY;
		SET.space = SET.dM;
		SET.width = SET.dW;
		SET.height = SET.dH;
	}

	pen1 = GetOutLinePen();
	pen2 = GetFillPen();
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(pcMojiVect->IsZenKaku() == TRUE
		|| (TTFONT <= m_nFontType && m_nFontType <= TT4FONT)){
			pcMojiVect->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
			if(pcMojiVect->m_nBaseCode == 0x0020){
				minix *= ((SET.width/2)/FSF1MESH);
				miniy *= (SET.height/FSF1MESH);
				maxx *= ((SET.width/2)/FSF1MESH);
				maxy *= (SET.height/FSF1MESH);
			}
			else{
				minix *= (SET.width/FSF1MESH);
				miniy *= (SET.height/FSF1MESH);
				maxx *= (SET.width/FSF1MESH);
				maxy *= (SET.height/FSF1MESH);
			}
			pcMojiVect->SetStartXY(SET.sx-minix, SET.sy-SET.height);
			pcMojiVect->SetFontSize(SET.width, SET.height);
			pcMojiVect->SetOutLinePen(pen1);
			pcMojiVect->SetFillPen(pen2);
			SET.sx += ((maxx - minix) + SET.space);
		}
		else{
			pcMojiVect->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
			minix *= ((SET.width/2)/FSF1MESH);
			miniy *= (SET.height/FSF1MESH);
			maxx *= ((SET.width/2)/FSF1MESH);
			maxy *= (SET.height/FSF1MESH);

			pcMojiVect->SetStartXY(SET.sx-minix, SET.sy-SET.height);
			pcMojiVect->SetFontSize(SET.width/2, SET.height);
			pcMojiVect->SetOutLinePen(pen1);
			pcMojiVect->SetFillPen(pen2);
			SET.sx += ((maxx - minix) + SET.space);
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::YokogakiEqualGetElementWaku(SetingElementType *SET)
{
	DBL		charcnt, bytecnt;
	DBL		splen;

	charcnt = SET->hcnt + SET->zcnt;
	if(1.0 < m_dCharCount){
		charcnt = 2;
	}
	if(charcnt == 1){
		if(fabs(SET->dSL) < fabs(SET->dW)){
			SET->sx = SET->dSX;
			SET->sy = SET->dSY;
			SET->width = SET->dSL;
			SET->height = SET->dH;
			SET->space = SET->dM;
		}
		else{
			if(SET->zcnt == 1
			&& (TTFONT > m_nFontType || m_nFontType > TT4FONT)){
				SET->sx = SET->dSX + (SET->dSL - SET->dW) / 2;
			}
			else{
				SET->sx = SET->dSX + (SET->dSL - SET->dW) / 2;
			}
			SET->sy = SET->dSY;
			SET->width = SET->dW;
			SET->height = SET->dH;
			SET->space = SET->dM;
		}
	}
	else{
		if(1.0 < m_dCharCount){
			bytecnt = m_dCharCount * 2;
		}
		else{
			bytecnt = (DBL)SET->hcnt + (DBL)SET->zcnt * 2;
		}
		splen = (DBL)(SET->hcnt + SET->zcnt - 1) * SET->dM;
		if(fabs(SET->dSL) > (bytecnt*fabs(SET->dW)/2) + splen){
			if(bytecnt == 2){// 半角２文字のとき
				SET->space = (fabs(SET->dSL) - (bytecnt * fabs(SET->dW) / 2))*2;
			}
			else{
				if(SET->lasthan == TRUE){
					SET->space = ((fabs(SET->dSL) - (bytecnt * fabs(SET->dW) / 2)) / (DBL)(bytecnt-1)) * 2.0;
				}
				else{
					SET->space = ((fabs(SET->dSL) - (bytecnt * fabs(SET->dW) / 2)) / (DBL)(bytecnt-2)) * 2.0;
				}
			}
			SET->width = fabs(SET->dW);
			if(SET->dSL < 0){
				SET->width *= -1;
				SET->space *= -1;
			}
		}
		else{
			SET->width = (fabs(SET->dSL)-splen) / (DBL)(bytecnt / 2);
			if(m_dTrns > (SET->width / fabs(SET->dH) * 100.0)){
				SET->width = fabs(m_dTrns * fabs(SET->dH) / 100.0);
				if(SET->lasthan == TRUE){
					SET->space = ((fabs(SET->dSL) - (bytecnt * SET->width / 2)) / (DBL)(bytecnt-1)) * 2.0;
				}
				else{
					SET->space = ((fabs(SET->dSL) - (bytecnt * SET->width / 2)) / (DBL)(bytecnt-2)) * 2.0;
				}
			}
			else{
				SET->space = SET->dM;
			}
			if(SET->dSL < 0){
				SET->width *= -1;
				SET->space *= -1;
			}
		}
		SET->height = SET->dH;
		SET->sx = SET->dSX;
		SET->sy = SET->dSY;
	}
}

void CStrgVect::YokogakiEqualGetElementData(SetingElementType *SET)
{
	int		bytecnt;
	DBL		splen, txlen;

	bytecnt = SET->hcnt + SET->zcnt;
	if((SET->hcnt + SET->zcnt) == 1){
		if(fabs(SET->dSL) < fabs(SET->dW)){
			SET->sx = SET->dSX;
			SET->sy = SET->dSY;
			SET->width = SET->dSL;
			SET->height = SET->dH;
			SET->space = SET->dM;
		}
		else{
			if(SET->zcnt == 1
			&& (TTFONT > m_nFontType || m_nFontType > TT4FONT)){
				SET->sx = SET->dSX + (SET->dSL - SET->dW) / 2;
			}
			else{
				SET->sx = SET->dSX + (SET->dSL - SET->dW) / 2;
			}
			SET->sy = SET->dSY;
			SET->width = SET->dW;
			SET->height = SET->dH;
			SET->space = SET->dM;
		}
	}
	else{
		splen = (DBL)(SET->hcnt + SET->zcnt - 1) * SET->dM;
		if(fabs(SET->dSL) > (fabs(SET->dXLen) + splen)){
			SET->space = (fabs(SET->dSL) - fabs(SET->dXLen)) / (DBL)(SET->hcnt + SET->zcnt - 1);
			SET->width = fabs(SET->dW);
			if(SET->dSL < 0){
				SET->width *= -1;
				SET->space *= -1;
			}
		}
		else{
			txlen = fabs(SET->dSL) - splen;
			SET->width = txlen / fabs(SET->dXLen)  * fabs(SET->dW);
			if(m_dTrns > (txlen / fabs(SET->dXLen) * 100.0)){
				SET->width = fabs(SET->dW) * m_dTrns / 100.0;
				txlen = fabs(SET->dXLen) * m_dTrns / 100.0;
				SET->space = (fabs(SET->dSL) - txlen) /  (DBL)((bytecnt-2)/2);
			}
			else{
				SET->space = SET->dM;
			}
			if(SET->dSL < 0){
				SET->width *= -1;
				SET->space *= -1;
			}
		}
		SET->height = SET->dH;
		SET->sx = SET->dSX;
		SET->sy = SET->dSY;
	}
}

void CStrgVect::StrgDataSumLenGet(DBL width, DBL hight, DBL *xlen, DBL *ylen)
{
	int		flag;
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;
	int		nYokoMojiIn;

	nYokoMojiIn = OFF;
	pcMojiVect = m_pcMojiTop;
	*xlen = 0;
	*ylen = 0;
	flag = CheckTateYoko();
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		minix = 0;	maxx = 0;
		miniy = 0;	maxy = 0;
		if(pcMojiVect->IsZenKaku() == TRUE
		|| (TTFONT <= m_nFontType && m_nFontType <= TT4FONT)){
			if(nYokoMojiIn == OFF){
				if(m_nYokoGumi == ON
				&& (flag == TATEGAKI || flag == YOKOOKI)){
					if(pcMojiVect->IsNumCode() == TRUE){
						nYokoMojiIn = ON;
					}
				}
			}
			else if(m_nYokoGumi == ON
				 && (flag == TATEGAKI || flag == YOKOOKI)){
				if(pcMojiVect->IsNumCode() == FALSE){
					nYokoMojiIn = OFF;
					(*xlen) += width;
					(*ylen) += hight;
				}
			}
			if(nYokoMojiIn == OFF){
				pcMojiVect->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
				if(pcMojiVect->m_nBaseCode == 0x0020){
					minix *= ((width/2)/FSF1MESH);
					miniy *= ((hight/2)/FSF1MESH);
					maxx *= ((width/2)/FSF1MESH);
					maxy *= ((hight/2)/FSF1MESH);
				}
				else{
					minix *= (width/FSF1MESH);
					miniy *= (hight/FSF1MESH);
					maxx *= (width/FSF1MESH);
					maxy *= (hight/FSF1MESH);
				}
			}
		}
		else{
			if(nYokoMojiIn == OFF){
				if(m_nYokoGumi == ON
				&& (flag == TATEGAKI || flag == YOKOOKI)){
					if(pcMojiVect->IsNumCode() == TRUE){
						nYokoMojiIn = ON;
					}
				}
			}
			else if(m_nYokoGumi == ON
				 && (flag == TATEGAKI || flag == YOKOOKI)){
				if(pcMojiVect->IsNumCode() == FALSE){
					nYokoMojiIn = OFF;
					(*xlen) += width;
					(*ylen) += hight;
				}
			}
			if(nYokoMojiIn == OFF){
				pcMojiVect->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
				minix *= ((width/2)/FSF1MESH);
				miniy *= ((hight/2)/FSF1MESH);
				maxx *= ((width/2)/FSF1MESH);
				maxy *= ((hight/2)/FSF1MESH);
			}
		}
		(*xlen) += (maxx - minix);
		(*ylen) += (maxy - miniy);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	// 最後が数字の時の処理
	if(nYokoMojiIn == ON && m_nYokoGumi == ON){
		(*xlen) += width;
		(*ylen) += hight;
	}
}

void CStrgVect::AreaDataSumLenGet(CMojiVect *pcSMojiVect, CMojiVect *pcEMojiVect, DBL width, DBL hight, DBL *xlen, DBL *ylen)
{
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;

	pcMojiVect = pcSMojiVect;
	*xlen = 0;
	*ylen = 0;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(pcMojiVect == pcEMojiVect){
			break;
		}
		minix = 0;	maxx = 0;
		miniy = 0;	maxy = 0;
		if(pcMojiVect->IsZenKaku() == TRUE
		|| (TTFONT <= m_nFontType && m_nFontType <= TT4FONT)){
			pcMojiVect->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
			if(pcMojiVect->m_nBaseCode == 0x0020){
				minix *= ((width/2)/FSF1MESH);
				miniy *= (hight/FSF1MESH);
				maxx *= ((width/2)/FSF1MESH);
				maxy *= (hight/FSF1MESH);
			}
			else{
				minix *= (width/FSF1MESH);
				miniy *= (hight/FSF1MESH);
				maxx *= (width/FSF1MESH);
				maxy *= (hight/FSF1MESH);
			}
		}
		else{
			pcMojiVect->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
			minix *= ((width/2)/FSF1MESH);
			miniy *= (hight/FSF1MESH);
			maxx *= ((width/2)/FSF1MESH);
			maxy *= (hight/FSF1MESH);
		}
		(*xlen) += (maxx - minix);
		(*ylen) += (maxy - miniy);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

//P 全角と半角の文字の数を求める
void CStrgVect::GeCharCount(int* hcnt, int* zcnt)
{
	CMojiVect*	pcMojiVect;
	int		nYokoMojiIn;

	nYokoMojiIn = OFF;
	pcMojiVect = m_pcMojiTop;
	(*hcnt) = 0;
	(*zcnt) = 0;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(pcMojiVect->IsZenKaku() == TRUE
		|| (TTFONT <= m_nFontType && m_nFontType <= TT4FONT)){
			if(nYokoMojiIn == OFF){
				if(m_nYokoGumi == ON){
					if(pcMojiVect->IsNumCode() == TRUE){
						nYokoMojiIn = ON;
					}
				}
			}
			else if(m_nYokoGumi == ON){
				if(pcMojiVect->IsNumCode() == FALSE){
					nYokoMojiIn = OFF;
					(*zcnt)++;
				}
			}
			if(nYokoMojiIn == OFF){
				if(pcMojiVect->m_nBaseCode == 0x20){
					(*hcnt)++;
				}
				else{
					(*zcnt)++;
				}
			}
		}
		else{
			if(nYokoMojiIn == OFF){
				if(m_nYokoGumi == ON){
					if(pcMojiVect->IsNumCode() == TRUE){
						nYokoMojiIn = ON;
					}
				}
			}
			else if(m_nYokoGumi == ON){
				if(pcMojiVect->IsNumCode() == FALSE){
					nYokoMojiIn = OFF;
					(*zcnt)++;
				}
			}
			if(nYokoMojiIn == OFF){
				(*hcnt)++;
			}
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}

	// 最後が数字の時の処理
	if(nYokoMojiIn == ON && m_nYokoGumi == ON){
		(*zcnt)++;
	}
}

void CStrgVect::GetAreaCharCount(CMojiVect *pcSMojiVect, CMojiVect *pcEMojiVect, int *hcnt, int *zcnt)
{
	CMojiVect	*pcMojiVect;

	(*hcnt) = 0;
	(*zcnt) = 0;
	pcMojiVect = pcSMojiVect;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(pcMojiVect == pcEMojiVect){
			break;
		}
		if(pcMojiVect->IsZenKaku() == TRUE
		|| (TTFONT <= m_nFontType && m_nFontType <= TT4FONT)){
			(*zcnt)++;
		}
		else{
			(*hcnt)++;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::BoxsDraw(CDC *pcDC, DBL sx, DBL sy, DBL ex, DBL ey)
{
	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)sx, (int)ey);
	pcDC->LineTo((int)ex, (int)ey);
	pcDC->LineTo((int)ex, (int)sy);
	pcDC->LineTo((int)sx, (int)sy);
}

void CStrgVect::WakuSave(FILE * fp)
{
	return;
}

CData* CStrgVect::SrchData(DBL dx, DBL dy, DBL * retlen)
{
	int			idx;
	CMojiVect*	pcMojiVect;
	DBL		len;
	DBL		sx, sy, ex, ey;
	DBL		wd, hi, sln;
	int		flag;

	*retlen = INTMAX;
	if(m_sString[0] == _T('\0')){
		GetStrStartXY(&sx, &sy);
		GetFontSize(&wd, &hi);
		GetStrLen(&sln);
		flag = CheckTateYoko();
		if(flag == TATEGAKI || flag == YOKOOKI){
			ex = sx + wd;
			ey = sy - sln;
		}
		else{
			ex = sx + sln;
			ey = sy - hi;
		}
		if(sx <= dx && dx <= ex
		&& ey <= dy && dy <= sy){
			*retlen = 0;
		}
		else{
			*retlen = GetPreLengthBoxs(dx, dy, sx, sy, ex, ey);
		}
		return(this);
	}
	idx = 0;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		len = INTMAX;
		pcMojiVect->SrchData(dx, dy, &len);
		if(*retlen > len){
			*retlen = len;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
		idx++;
	}
	return(this);	
}

CData* CStrgVect::SrchDataBoxs(DBL dx, DBL dy, DBL * retlen)
{
	int			idx;
	CMojiVect*	pcMojiVect;
	DBL		len;
	DBL		sx, sy, ex, ey;
	DBL		wd, hi, sln;
	DBL		xx, yy;
	int		flag;

	*retlen = INTMAX;
	if(m_sString[0] == _T('\0')){
		GetStrStartXY(&sx, &sy);
		GetFontSize(&wd, &hi);
		GetStrLen(&sln);
		flag = CheckTateYoko();
		if(flag == TATEGAKI || flag == YOKOOKI){
			ex = sx + wd;
			ey = sy - sln;
		}
		else{
			ex = sx + sln;
			ey = sy - hi;
		}

		if(sx > ex){
			xx = sx;
			sx = ex;
			ex = xx;
		}
		if(sy > ey){
			yy = sy;
			sy = ey;
			ey = yy;
		}

		if(sx <= dx && dx <= ex
		&& ey <= dy && dy <= sy){
			*retlen = 0;
		}
		else{
			*retlen = GetPreLengthBoxs(dx, dy, sx, sy, ex, ey);
		}
		return(this);
	}
	idx = 0;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		len = INTMAX;
		pcMojiVect->SrchDataBoxs(dx, dy, &len);
		if(*retlen > len){
			*retlen = len;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
		idx++;
	}
	return(this);	
}

CData* CStrgVect::SrchNearOnLinePoint(DBL xx, DBL yy, DBL *retlen, DBL *retx, DBL *rety, int *retpp)
{
	CMojiVect*	pcMojiVect;
	CData*	retData;
	CData*  tpcData;
	DBL		len;
	DBL		x, y;
	int		pp;

	retData = NULL;
	*retlen = INTMAX;
	*retpp = 0;
	if(senvchknoselectstrgvect() == ON){
		return(retData);
	}
	if(m_sString[0] == _T('\0')){
		return(retData);
	}
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		tpcData = pcMojiVect->SrchNearOnLinePoint(xx, yy, &len, &x, &y, &pp);
		if(*retlen > len){
			*retlen = len;
			*retx = x;
			*rety = y;
			*retpp = pp;
			retData = tpcData;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	return(retData);	
}

CData* CStrgVect::SrchNearPoint(DBL xx, DBL yy, DBL *retlen, int *retpp)
{
	CMojiVect*	pcMojiVect;
	DBL		len;
	int		pp;
	CData*	pcData;
	CData*	retpcData;

	*retlen = INTMAX;
	*retpp = 0;
	retpcData = NULL;
	if(m_sString[0] == _T('\0')){
		return(retpcData);
	}
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		len = INTMAX;
		pcData = pcMojiVect->SrchNearPoint(xx, yy, &len, &pp);
		if(*retlen > len){
			*retpp = pp;
			*retlen = len;
			retpcData = pcData;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	return(retpcData);	
}

CData* CStrgVect::SrchMoji(DBL x, DBL y, DBL *retlen, int *retmidx)
{
	int			idx;
	CMojiVect*	pcMojiVect;
	DBL		len;

	*retlen = INTMAX;
	*retmidx = -1;
	if(m_sString[0] == _T('\0')){
		return(this);
	}
	idx = 0;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		len = INTMAX;
		pcMojiVect->SrchData(x, y, &len);
		if(*retlen > len){
			*retmidx = idx;
			*retlen = len;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
		idx++;
	}
	return(this);	
}

void CStrgVect::SetDataList(CDataList * pcDataList)
{
	CMojiVect*	pcMojiVect;
	CDataList*	pcSrcList;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcSrcList = (CDataList*)pcMojiVect->m_pcDataList;
		if(pcSrcList == NULL){
			pcMojiVect->DataChange();	
			pcSrcList = (CDataList*)pcMojiVect->m_pcDataList;
		}
		if(pcSrcList == NULL){
			break;
		}
		pcDataList->LinkCopy(pcSrcList);
		pcSrcList->m_pcDataTop = NULL;
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::SetDefOutLineClr()
{
	CMojiVect*	pcMojiVect;

	CVect::SetDefOutLineClr();
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->SetDefOutLineClr();
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::SetDefFillClr()
{
	CMojiVect*	pcMojiVect;

	CVect::SetDefFillClr();
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->SetDefFillClr();
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::GetSpaceMojiCount(LPINT retzen, LPINT rethan)
{
	int			zen, han;
	CMojiVect*	pcMojiVect;
	CMojiVect*	pcNextMojiVect;

	zen = 0;
	han = 0;
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcNextMojiVect = pcMojiVect->GetNextMoji();
		if(pcNextMojiVect == NULL){
			// 最後の文字のスペースはカウントしない
			break;
		}
		if(pcMojiVect->m_nZenKaku == ON){
			zen++;
		}
		else{
			han++;
		}
		pcMojiVect = pcNextMojiVect;
	}
	*retzen = zen;
	*rethan = han;
}

void CStrgVect::SetData(CMojiVect * pcSetMojiVect)
{
	CMojiVect	*pcMojiVect;
	CMojiVect	*pcNextMojiVect;

	if(m_pcMojiTop == NULL){
		m_pcMojiTop = pcSetMojiVect;
		return;
	}
	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcNextMojiVect = pcMojiVect->GetNextMoji();
		if(pcNextMojiVect == NULL){
			pcMojiVect->SetNextMoji(pcSetMojiVect);
			return;
		}
		pcMojiVect = pcNextMojiVect;
	}
}

int CStrgVect::FontSelect(LPTSTR sFontName, int fTateGaki)
{
	TCHAR	sGJFaceName[QRECMAX];
	TCHAR	sFaceName[QRECMAX];
	TCHAR	sTategakiFaceName[QRECMAX];

	mojienvflistopen(_T("rb"));
	mojienvgetfonttypename(sFontName, &m_nFontType, sFaceName);
	mojienvflistclose();
	if(m_nFontType == 0){
		_tcscpy_s(sFaceName, sFontName);
		m_nFontType = TT1FONT;
	}
	switch(m_nFontType){
	case ES0FONT:
		m_pcESF0Font->FontNameSet(sFaceName);
		m_pcESF0Font->SetTateYoko(fTateGaki);
		break;
	case ES1FONT:
		m_pcESF1Font->FontNameSet(sFaceName);
		m_pcESF1Font->SetTateYoko(fTateGaki);
		break;
	case FB0FONT:
		m_pcFBU0Font->FontNameSet(sFaceName);
		m_pcFBU0Font->SetTateYoko(fTateGaki);
		break;
	case TT1FONT:
	case TT2FONT:
	case TT3FONT:
		_tcscpy_s(sGJFaceName, sFontName);
		m_pcESF1Font->FontNameSet(sGJFaceName);
		if(fTateGaki == TATEGAKI || fTateGaki == YOKOOKI){
			if(m_nFontType == TT1FONT){
				_stprintf_s(sTategakiFaceName, QRECMAX, _T("@%s"), sFaceName);
			}
			else{
				_stprintf_s(sTategakiFaceName, QRECMAX, _T("%s"), sFaceName);
			}
			m_pcWNTTFont->FontNameSet(sTategakiFaceName);
			m_pcESF1Font->SetTateYoko(fTateGaki);
			m_pcWNTTFont->SetTateYoko(fTateGaki);
		}
		else{
			m_pcWNTTFont->FontNameSet(sFaceName);
			m_pcESF1Font->SetTateYoko(fTateGaki);
			m_pcWNTTFont->SetTateYoko(fTateGaki);
		}
		break;
	case ARCFONT:
	case MAXFONT:
		m_pcESF1Font->FontNameSet(sFaceName);
		m_pcARCFont->FontNameSet(sFaceName, m_nFontType);
		m_pcESF1Font->SetTateYoko(fTateGaki);
		m_pcARCFont->SetTateYoko(fTateGaki);
		break;
	case ARKFONT:
		m_pcESF1Font->FontNameSet(sFaceName);
		m_pcARKFont->FontNameSet(sFaceName, m_nFontType);
		m_pcESF1Font->SetTateYoko(fTateGaki);
		m_pcARKFont->SetTateYoko(fTateGaki);
		break;
	case SCRFONT:
		m_pcSCRFont->FontNameSet(sFaceName);
		m_pcSCRFont->SetTateYoko(fTateGaki);
		break;
	}
	return(m_nFontType);
}

void CStrgVect::DeleteData(CData *delData)
{
	CMojiVect	*pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->DeleteData(delData);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::SaveEpsFile()
{
	CMojiVect	*pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->SaveEpsFile();
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

int CStrgVect::CheckOneMojiTrns()
{
	CMojiVect	*pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(pcMojiVect->CheckOneMojiTrns() == ON){
			return(ON);
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	return(OFF);
}

void CStrgVect::ReverceRejion()
{
	CMojiVect	*pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->ReverceRejion();
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::SetUseClrPenno(int notbl[], LPINT cnt)
{
	CMojiVect	*pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->SetUseClrPenno(notbl, cnt);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::PointPicup()
{
	CMojiVect	*pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->PointPicup();
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::AllOffPointFlag(int bitflag)
{
	CMojiVect	*pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->AllOffPointFlag(bitflag);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::AllOnPointFlag(int bitflag)
{
	CMojiVect	*pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->AllOnPointFlag(bitflag);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::PointXLegal(DBL xx)
{
	CMojiVect	*pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->PointXLegal(xx);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::PointYLegal(DBL yy)
{
	CMojiVect	*pcMojiVect;

	pcMojiVect = m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->PointYLegal(yy);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

BOOL CStrgVect::IsLastHankaku()
{
	CMojiVect	*pcMoji;
	CMojiVect	*pcPreMoji;

	pcPreMoji = NULL;
	pcMoji = m_pcMojiTop;
	while(1){
		if(pcMoji == NULL){
			break;
		}
		pcPreMoji = pcMoji;
		pcMoji = pcMoji->GetNextMoji();
	}
	if(pcPreMoji == NULL){
		return(FALSE);
	}
	if(pcPreMoji->IsZenKaku() == TRUE){
		return(FALSE);
	}
	else{
		return(TRUE);
	}
}

BOOL CStrgVect::IsAreaLastHankaku(CMojiVect *pcSMoji, CMojiVect *pcEMoji)
{
	CMojiVect	*pcMoji;
	CMojiVect	*pcPreMoji;

	pcPreMoji = NULL;
	pcMoji = pcSMoji;
	while(1){
		if(pcMoji == NULL){
			break;
		}
		if(pcMoji == pcEMoji){
			break;
		}
		pcPreMoji = pcMoji;
		pcMoji = pcMoji->GetNextMoji();
	}
	if(pcPreMoji == NULL){
		return(FALSE);
	}
	if(pcPreMoji->IsZenKaku() == TRUE){
		return(FALSE);
	}
	else{
		return(TRUE);
	}
}

void CStrgVect::YokoMojiElementInitWaku(SetingElementType *SrcSET, CMojiVect *pcSMojiVect, CMojiVect *pcEMojiVect)
{
	SetingElementType	SET;
	CMojiVect*	pcMojiVect;
	DBL		add;
	int		pen1, pen2;

	SET.dSX = SrcSET->sx;
	SET.dSY = SrcSET->sy;
	SET.dH = SrcSET->height;
	SET.dW = SrcSET->dW;
	SET.dSL = SrcSET->dW;
	SET.dM = 0;
	SET.dL = 0;
	SET.lasthan = IsAreaLastHankaku(pcSMojiVect, pcEMojiVect);
	GetAreaCharCount(pcSMojiVect, pcEMojiVect, &(SET.hcnt), &(SET.zcnt));
	if(SET.zcnt == 2){
		add = SET.dSL * 0.2;
		SET.dSX = SrcSET->sx + add / 2;
		SET.dSL = SrcSET->dW - add;
	}

	YokogakiEqualGetElementWaku(&SET);

	// 開始位置と終了位置が違う以外「YokogakiElementInitWaku」と同一処理
	pen1 = GetOutLinePen();
	pen2 = GetFillPen();
	pcMojiVect = pcSMojiVect;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(pcMojiVect == pcEMojiVect){
			break;
		}
		if(pcMojiVect->IsZenKaku() == TRUE
		|| (TTFONT <= m_nFontType && m_nFontType <= TT4FONT)){
			if(pcMojiVect->m_nBaseCode == 0x20){
				pcMojiVect->SetStartXY(SET.sx, SET.sy-SET.height);
				pcMojiVect->SetFontSize(SET.width/2, SET.height);
				pcMojiVect->SetOutLinePen(pen1);
				pcMojiVect->SetFillPen(pen2);
				SET.sx += (SET.width + SET.space)/2;
			}
			else{
				pcMojiVect->SetStartXY(SET.sx, SET.sy-SET.height);
				pcMojiVect->SetFontSize(SET.width, SET.height);
				pcMojiVect->SetOutLinePen(pen1);
				pcMojiVect->SetFillPen(pen2);
				SET.sx += (SET.width + SET.space);
			}
		}
		else{
			pcMojiVect->SetStartXY(SET.sx, SET.sy-SET.height);
			pcMojiVect->SetFontSize(SET.width/2, SET.height);
			pcMojiVect->SetOutLinePen(pen1);
			pcMojiVect->SetFillPen(pen2);
			SET.sx += (SET.width + SET.space)/2;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}

void CStrgVect::YokoMojiElementInitData(SetingElementType *SrcSET, CMojiVect *pcSMojiVect, CMojiVect *pcEMojiVect)
{
	SetingElementType	SET;
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;
	int		pen1, pen2;
	DBL		add;

	SET.dSX = SrcSET->sx;
	SET.dSY = SrcSET->sy;
	SET.dH = SrcSET->height;
	SET.dW = SrcSET->dW;
	SET.dSL = SrcSET->dW;
	SET.dM = SrcSET->dW * 0.02;
	//SET.dM = 0;
	SET.dL = 0;
	SET.lasthan = IsAreaLastHankaku(pcSMojiVect, pcEMojiVect);
	GetAreaCharCount(pcSMojiVect, pcEMojiVect, &(SET.hcnt), &(SET.zcnt));
	AreaDataSumLenGet(pcSMojiVect, pcEMojiVect, SET.dW, SET.dH, &(SET.dXLen), &(SET.dYLen));
	if(SET.zcnt == 2){
		add = SET.dSL * 0.2;
		SET.dSX = SrcSET->sx + add / 2;
		SET.dSL = SrcSET->dW - add;
	}

	YokogakiEqualGetElementData(&SET);

	// 開始位置と終了位置が違う以外「YokogakiElementInitData」と同一処理
	pen1 = GetOutLinePen();
	pen2 = GetFillPen();
	pcMojiVect = pcSMojiVect;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(pcMojiVect == pcEMojiVect){
			break;
		}
		if(pcMojiVect->IsZenKaku() == TRUE
		|| (TTFONT <= m_nFontType && m_nFontType <= TT4FONT)){
			pcMojiVect->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
			if(pcMojiVect->m_nBaseCode == 0x0020){
				minix *= ((SET.width/2)/FSF1MESH);
				miniy *= (SET.height/FSF1MESH);
				maxx *= ((SET.width/2)/FSF1MESH);
				maxy *= (SET.height/FSF1MESH);
			}
			else{
				minix *= (SET.width/FSF1MESH);
				miniy *= (SET.height/FSF1MESH);
				maxx *= (SET.width/FSF1MESH);
				maxy *= (SET.height/FSF1MESH);
			}
			if((SET.zcnt+SET.hcnt) == 1){
				minix = 0;
			}
			pcMojiVect->SetStartXY(SET.sx-minix, SET.sy-SET.height);
			pcMojiVect->SetFontSize(SET.width, SET.height);
			pcMojiVect->SetOutLinePen(pen1);
			pcMojiVect->SetFillPen(pen2);
			SET.sx += ((maxx - minix) + SET.space);
		}
		else{
			pcMojiVect->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
			minix *= ((SET.width/2)/FSF1MESH);
			miniy *= (SET.height/FSF1MESH);
			maxx *= ((SET.width/2)/FSF1MESH);
			maxy *= (SET.height/FSF1MESH);

			if((SET.zcnt+SET.hcnt) == 1){
				minix = 0;
			}
			pcMojiVect->SetStartXY(SET.sx-minix, SET.sy-SET.height);
			pcMojiVect->SetFontSize(SET.width/2, SET.height);
			pcMojiVect->SetOutLinePen(pen1);
			pcMojiVect->SetFillPen(pen2);
			SET.sx += ((maxx - minix) + SET.space);
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
}


