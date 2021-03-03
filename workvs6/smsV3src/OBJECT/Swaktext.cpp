//
// Easy Scanner System
//
// object swaktext.cpp  枠内配置文字列関係関数
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include <math.h>

#include <mbstring.h>
#include "datafdib.h"
#include "swaktext.h"

extern "C" {
	int scrngetwakunodisp();
	int	senvchckstopflag();
	int	senvgetcrtpenno();
	void senvgetpenrgb(int pno, LPINT rval, LPINT gval, LPINT bval);
	void senvgetwakuclr(LPINT rval, LPINT gval, LPINT bval);
	int	senvgetwakuno();
	int	 scrncheckundoflag();
	int	 scrnchecknoactflag();
	void scrnpeninit(HDC PaintDC, long lClr, long nthick);
	void scrnsetdelpen(HDC PaintDC);
	void scrnsetatnpen(HDC PaintDC);
	void scrnsetnoslctpen(HDC PaintDC);
	void scrnsetfrpen(HDC PaintDC);
	void scrnsetslctpen(HDC PaintDC);
	void scrnsetwakupen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	void scrndxymmtorltv(DBL *x, DBL *y);
	int senvcheckdrawmode();
	int senvcheckverjion();
	BOOL  pdrwatrarcchk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrbezechk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrcurvechk(int atr1);
	DBL   pdrwendragget(int atr1, int atr2, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL   pdrwstartragget(int atr0, int atr1, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2);
	void  pdrwmove(HDC PaintDC, DBL x1, DBL y1);
	void  pdrwline(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2);
	int curve3pcenterget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL FAR *x0, DBL FAR *y0);
	int curve3pragrget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0, DBL FAR *rag01, DBL FAR *rag03, DBL FAR *r);
	void curvebezecontrolpointget(DBL x1, DBL y1, DBL x2, DBL y2, DBL rag, DBL *xp, DBL *yp);
	void curvenoragtocurverag(DBL *rag1, DBL *rag2);
	int curveperpendicular(DBL sx, DBL sy, DBL ex, DBL ey, DBL x, DBL y, DBL *cx, DBL *cy);
	int curveonlinechk(DBL x1, DBL y1, DBL x2, DBL y2, DBL x, DBL y);
	void curveroundtrns(DBL srcx, DBL srcy, DBL x0, DBL y0, DBL rag, DBL *dstx, DBL *dsty);
	int basesetcodetbl(LPSTR strg, UINT code[]);
	int epsfboxsstrgsave(LPSTR strg, int ty, int od, DBL fl, DBL sp, DBL sx, DBL sy, DBL ex, DBL ey);
}

IMPLEMENT_SERIAL(CSWakText, CObject, 3)
CSWakText::CSWakText()
{
}

CSWakText::CSWakText(int nDummy)
	: CStrgText(nDummy)
{
	int ver = senvcheckverjion();
	m_nWakuNo = 0;
	m_nFildNo = -1;
	strcpy(m_szWakuName, "未設定");
	m_nWakuKind = DEFWAKU;
	m_nPosLock = 0;
	m_nCntntLock = 0;
	m_nPrntOut = 1;
	m_pcDataFDib = NULL;
	m_pcFitingVect = NULL;
	m_nSpaceKind = SESPOFF | SPACEWAKU;
	m_nRoundKind = ON;
	m_dHrgnPar = 100.0;
	m_dKtknPar = 100.0;
	m_nOder = ODEREQUAL;
	m_nBlnceKind = -1;
	m_dBlncePar = 100;
	m_dSpacePar = 0;
	m_dFlatPar = 100;
}

CSWakText::~CSWakText()
{
	if(m_pcFitingVect != NULL){
		delete(m_pcFitingVect);
	}
	if(m_pcDataFDib != NULL){
		delete(m_pcDataFDib);
	}
}

int CSWakText::CheckID()
{
	return(SWAKTEXTID);
}

void CSWakText::Serialize(CArchive& ar)
{
	CString	csName;
	int		flag;
	int		pno, r,g, b;
	int		ver;

	if (ar.IsStoring())
	{
		ver = senvcheckverjion();
		ar << m_nFildNo;
		csName = _T(m_szWakuName);
		ar << csName;
		ar << m_nWakuKind;
		ar << m_nPosLock;
		ar << m_nCntntLock;
		ar << m_nPrntOut;
		ar << m_nSpaceKind;
		ar << m_nRoundKind;
		ar << m_dHrgnPar;
		ar << m_dKtknPar;
		ar << m_nOder;
		ar << m_nBlnceKind;
		ar << m_dBlncePar;
		m_pcFitingVect->Serialize(ar);
		if(m_pcDataFDib == NULL){
			flag = 0;
			ar << flag;
		}
		else{
			flag = 1;
			ar << flag;
			m_pcDataFDib->Serialize(ar);
		}
	}
	else
	{
		ver = senvcheckverjion();
		ar >> m_nFildNo;
		ar >> csName;
		strcpy(m_szWakuName, csName.GetBuffer(QRECMAX));
		if(strncmp(m_szWakuName, "未設定", 6) == 0){
			m_nFildNo = -1;
		}
		ar >> m_nWakuKind;
		ar >> m_nPosLock;
		ar >> m_nCntntLock;
		ar >> m_nPrntOut;
		ar >> m_nSpaceKind;
		ar >> m_nRoundKind;
		ar >> m_dHrgnPar;
		ar >> m_dKtknPar;
		ar >> m_nOder;
		ar >> m_nBlnceKind;
		ar >> m_dBlncePar;
		m_pcFitingVect = (CVect*)NewDataCreate(BASEVECTID);
		m_pcFitingVect->Serialize(ar);
		ar >> flag;
		if(flag != 0){
			m_pcDataFDib = (CDataFDib*)NewDataCreate(DATAFDIBID);
			m_pcDataFDib->Serialize(ar);
		}
		m_dTKtknPar = m_dKtknPar; 
		m_dTHrgnPar = m_dHrgnPar; 
	}
	CStrgText::Serialize(ar);
	if (!(ar.IsStoring()))
	{
		// 2002.01.15 UNDO、REDOで色が元に戻らない
		if(flag != 0){
			if(m_pcDataFDib->m_pcDibCData != NULL){
				pno = GetFillPen();
				if(pno == 0){
					r = 0; g = 0; b = 0;
				}
				else{
					senvgetpenrgb(pno, &r, &g, &b);
				}
				m_pcDataFDib->m_pcDibCData->SetRGBQuadData(0, r, g, b);
			}
		}
		// 2001.07.11 名詞工場(ver1.01.71）で縦書き保存すると読み込んだ時逆に表示される
		StrgFiting();
		RenewMiniMax();
	}
}

//p 内部データーが空の状態であれば削除しすべてが空のときTRUEを返す
int CSWakText::CheckNullData()
{
	// 文字列を正規化する
	CStrgText::CheckNullData();
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

void CSWakText::GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	CMojiText	*pcMoji;
	DBL		tminix, tminiy, tmaxx, tmaxy;

	GetWakuDMiniMax(minix, miniy, maxx, maxy);
	POSITION pos = m_cMojiTextList.GetHeadPosition();
	if(pos == NULL){
		return;
	}
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetDataDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
		(*minix) = min((*minix), tminix);
		(*miniy) = min((*miniy), tminiy);
		(*maxx) = max((*maxx), tmaxx);
		(*maxy) = max((*maxy), tmaxy);
	}
}

void CSWakText::GetWakuDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	DBL		sx, sy, ex, ey;
	DBL		add;

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
		add = m_dSize / 2;
		(*minix) -= add;
		(*miniy) -= add;
		(*maxx) += add;
		(*maxy) += add;
	}
}

void CSWakText::RenewMiniMax()
{
	DBL		minix, miniy, maxx, maxy;

	CStrgText::RenewMiniMax();
	m_pcFitingVect->RenewMiniMax();
	m_pcFitingVect->GetDMiniMax(&minix, &miniy, &maxx, &maxy);
	m_dMinix = min(minix, m_dMinix);
	m_dMiniy = min(miniy, m_dMiniy);
	m_dMaxx = max(maxx, m_dMaxx);
	m_dMaxy = max(maxy, m_dMaxy);
	m_dSX = m_dMinix;
	m_dSY = m_dMiniy;
}

CData* CSWakText::CopyData(DBL movex, DBL movey)
{
	CSWakText *pcnewData;

	pcnewData = (CSWakText*)NewDataCreate(SWAKTEXTID);
	CopyDataElement(pcnewData, movex, movey);
	pcnewData->RenewMiniMax();
	return((CData*)pcnewData);
}

void CSWakText::CopyDataElement(CSWakText *pcnewData, DBL movex, DBL movey)
{
	CVect* pcnewVect;

	pcnewVect = (CVect*)m_pcFitingVect->CopyData(movex, movey);
	pcnewData->SetFitingVect(pcnewVect);
	pcnewData->m_nSpaceKind = m_nSpaceKind;
	pcnewData->SetRoundKind(m_nRoundKind);
	pcnewData->SetHrgnPar(m_dHrgnPar);
	pcnewData->SetKtknPar(m_dKtknPar);
	pcnewData->SetOderKind(m_nOder);
	pcnewData->SetBlnceKind(m_nBlnceKind);
	pcnewData->SetBlncePar(m_dBlncePar);

	CStrgText::CopyDataElement((CStrgText*)pcnewData, movex, movey);
}

void CSWakText::AllFree()
{
	delete(m_pcFitingVect);
	m_pcFitingVect = NULL;
	CStrgText::AllFree();
}

void CSWakText::NoSelectDraw(CDC *pcDC, int no)
{
	if(m_nWakuNo == no){
		scrnsetfrpen(pcDC->m_hDC);
		WakuBoxsDraw(pcDC);
		scrnpenend(pcDC->m_hDC);
	}
}

void CSWakText::SelectDraw(CDC *pcDC, int no)
{
	if(m_nWakuNo == no){
		scrnsetatnpen(pcDC->m_hDC);
		WakuBoxsDraw(pcDC);
		scrnpenend(pcDC->m_hDC);
	}
}

void CSWakText::OnDraw(CDC *pcDC)
{
	int		rval, gval, bval;
	int		mode, penno, crtno;

	mode = senvcheckdrawmode();
	if(mode == PRINTVIEW){
		if(m_nPrntOut == OFF){
			return;
		}
	}
	if(senvchckstopflag() == ON){
		return;
	}
	if(mode != BLACKVIEW
	&& mode != DIRCTVIEW
	&& mode != PRINTVIEW){
		if(CheckFlag(ACTIVEFLAG) == ON){
			senvgetwakuclr(&rval, &gval, &bval);
			if(rval == -1 && gval == -1 && bval == -1){
				scrnsetfrpen(pcDC->m_hDC);
			}
			else{
				scrnsetwakupen(pcDC->m_hDC);
			}
		}
		else{
			scrnsetnoslctpen(pcDC->m_hDC);
		}
		WakuBoxsDraw(pcDC);
		scrnpenend(pcDC->m_hDC);
	}

	if(m_nWakuKind == BMPWAKU || m_nWakuKind == LOGOWAKU){
		if(m_pcDataFDib == NULL){
			return;
		}
		crtno = senvgetcrtpenno();
		if(mode == PRINTVIEW && crtno != -1){
			if(m_nWakuKind == LOGOWAKU){
				penno = GetFillPen();
				if(penno != crtno){	// 単色カラー使用時以外
					return;
				}
			}
			else{	// BMPWAKU
				if(crtno != 1){		// カラーリボン使用時以外
					return;
				}
			}
		}

		if(m_nWakuKind == LOGOWAKU 
		&& m_pcDataFDib->m_pcDibCData != NULL
		&& m_pcDataFDib->m_pcDibCData->m_nBitCount == 1){
			penno = GetFillPen();
			if(penno == 0){
				rval = 0; gval = 0; bval = 0;
			}
			else{
				senvgetpenrgb(penno, &rval, &gval, &bval);
			}
			m_pcDataFDib->m_pcDibCData->SetRGBQuadData(0, rval, gval, bval);
		}
		m_pcDataFDib->OnDraw(pcDC);
	}
	else if(m_nWakuKind == POPWAKU){
		if(!(m_cMojiTextList.IsEmpty())){
			OnDrawPop(pcDC);
		}
	}
	else{	// DEFWAKU NAMEWAKU POPWAKU
		if(!(m_cMojiTextList.IsEmpty())){
			CStrgText::OnDraw(pcDC);
		}
	}
}

void CSWakText::OnDrawPop(CDC *pcDC)
{
	POSITION	pos;
	CMojiText*	pcMoji;

	if(senvchckstopflag() == ON){
		return;
	}
	pos = m_cMojiTextList.GetTailPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetPrev(pos);
		if(senvchckstopflag() == ON){
			return;
		}
		pcMoji->m_nPop = ON;
		pcMoji->OnDraw(pcDC);
		pcMoji->m_nPop = OFF;
	}
}

void CSWakText::WakuBoxsDraw(CDC *pcDC)
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

void CSWakText::GetFitingBoxs(LPDBL sx, LPDBL sy, LPDBL ex, LPDBL ey)
{
	DBL		x1, y1, x2, y2;
	DBL		wd, hi;

	m_pcFitingVect->GetDXY(0, &x1, &y1);
	m_pcFitingVect->GetDXY(1, &x2, &y2);
	GetFontSize(&wd, &hi);
	if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
		*sx = x1 - wd/2;
		*sy = y1;
		*ex = x2 + wd/2;
		*ey = y2;
	}
	else{
		*sx = x1;
		*sy = y1 + hi/2;
		*ex = x2;
		*ey = y2 - hi/2;
	}
}

int CSWakText::DrawNo(CDC *pcDC, int noidx)
{
	DBL		sx, sy;
	DBL		ex, ey;
	char	buf[16];
	int		ver;

	if(scrngetwakunodisp() == OFF){
		return(noidx);
	}
	if(pcDC != NULL){
		if(CheckFlag(ACTIVEFLAG) == ON){
			scrnsetfrpen(pcDC->m_hDC);
		}
		else{
			scrnsetnoslctpen(pcDC->m_hDC);
		}
		ver = senvcheckverjion();
		sprintf(buf, "%d", m_nWakuNo);
		GetFitingBoxs(&sx, &sy, &ex, &ey);
		scrndxymmtorltv(&sx, &sy);
		::TextOut(pcDC->m_hDC,(int)sx, (int)sy-WAKUNOTEXTSIZE,  buf, strlen(buf));
		scrnpenend(pcDC->m_hDC);
	}
	noidx++;
	return(noidx);
}

void CSWakText::OutlineDraw(CDC *pcDC)
{
	CStrgText::OutlineDraw(pcDC);
}

void CSWakText::AtntionDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	scrnsetslctpen(pcDC->m_hDC);
	WakuBoxsDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CSWakText::NoactDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	scrnsetnoslctpen(pcDC->m_hDC);
	WakuBoxsDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
	CStrgText::NoactDraw(pcDC);
}

void CSWakText::DelDraw(CDC *pcDC)
{
	scrnsetdelpen(pcDC->m_hDC);
	WakuBoxsDraw(pcDC);
	scrnpenend(pcDC->m_hDC);
	CStrgText::DelDraw(pcDC);
}

void CSWakText::MoveTrns(DBL movex, DBL movey)
{
	m_pcFitingVect->MoveTrns(movex, movey);
	CStrgText::MoveTrns(movex, movey);
	if(m_pcDataFDib != NULL){
		m_pcDataFDib->MoveTrns(movex, movey);
	}
}

void CSWakText::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	m_pcFitingVect->SizeTrns(cntrx, cntry, timex, timey);
	CStrgText::SizeTrns(cntrx, cntry, timex, timey);
	if(m_pcDataFDib != NULL){
		m_pcDataFDib->SizeTrns(cntrx, cntry, timex, timey);
	}
}

void CSWakText::SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
{
	if(m_pcDataFDib != NULL){
		m_pcDataFDib->SizeTrns(cntrx, cntry, ddx, ddy);
	}
	else{
		//m_pcFitingVect->SlopTrns(cntrx, cntry, ddx, ddy);
		CStrgText::SlopTrns(cntrx, cntry, ddx, ddy);
	}
}

void CSWakText::MirrorTrns(DBL a, DBL b, DBL t1, DBL t2)
{
	if(m_pcDataFDib != NULL){
		//m_pcDataFDib->MirrorTrns(a, b, t1, t2);
	}
	else{
		m_pcFitingVect->MirrorTrns(a, b, t1, t2);
		CStrgText::MirrorTrns(a, b, t1, t2);
	}
}

void CSWakText::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	if(m_pcDataFDib != NULL){
		m_pcDataFDib->RoundTrns(cntrx, cntry, tsin, tcos);
	}
	else{
		m_pcFitingVect->RoundTrns(cntrx, cntry, tsin, tcos);
		CStrgText::RoundTrns(cntrx, cntry, tsin, tcos);
	}
}

void CSWakText::StrctBoxsTrns()
{
	if(m_pcDataFDib == NULL){
		m_pcFitingVect->StrctBoxsTrns();
		CStrgText::StrctBoxsTrns();
	}
}

void CSWakText::AllOnPointFlag(int bitflag)
{
	m_pcFitingVect->AllOnPointFlag(bitflag);
	CStrgText::AllOnPointFlag(bitflag);
}

void CSWakText::AllOffPointFlag(int bitflag)
{
	m_pcFitingVect->AllOffPointFlag(bitflag);
	CStrgText::AllOffPointFlag(bitflag);
}

void CSWakText::PointXLegal(DBL xx)
{
	m_pcFitingVect->PointXLegal(xx);
	CStrgText::PointXLegal(xx);
}

void CSWakText::PointYLegal(DBL yy)
{
	m_pcFitingVect->PointYLegal(yy);
	CStrgText::PointYLegal(yy);
}

CVect* CSWakText::GetFitingVect()
{
	return(m_pcFitingVect);
}

void CSWakText::SetFitingVect(CVect* pcVect)
{
	m_pcFitingVect = pcVect;
	pcVect->SetFillPen(-1);
	pcVect->SetOutLinePen(-1);
	pcVect->SetOutLineWidth(0.0);
}

void CSWakText::GetWakuName(LPSTR name)
{
	strcpy(name, m_szWakuName);
}

void CSWakText::SetWakuName(LPSTR name)
{
	strcpy(m_szWakuName, name);
}

void CSWakText::GetWakuKind(LPINT kind)
{
	*kind = m_nWakuKind;
}

void CSWakText::SetWakuKind(int kind)
{
	m_nWakuKind = kind;
}

void CSWakText::GetSpaceFlag(LPINT flag)
{
	*flag = m_nSpaceKind;
}

void CSWakText::SetSpaceFlag(int flag)
{
	m_nSpaceKind = flag;
}

void CSWakText::GetSpaceKind(LPINT kind)
{
	*kind = (m_nSpaceKind & 0x0f);
}

void CSWakText::SetSpaceKind(int kind)
{
	m_nSpaceKind &= 0xf0;
	m_nSpaceKind |= kind;
}

void CSWakText::GetSESpKind(LPINT kind)
{
	if((m_nSpaceKind & 0xf0) == 0){
		*kind = OFF;
	}
	else{
		*kind = ON;
	}
}

void CSWakText::SetSESpKind(int kind)
{
	if(kind == 0){
		m_nSpaceKind &= 0x0f;
	}
	else{
		m_nSpaceKind |= SESPON;
	}
}

void CSWakText::GetRoundKind(LPINT flag)
{
	*flag = m_nRoundKind;
}

void CSWakText::SetRoundKind(int flag)
{
	m_nRoundKind = flag;
}

void CSWakText::GetHrgnPar(LPDBL dpar)
{
	*dpar = m_dHrgnPar;
}

void CSWakText::SetHrgnPar(DBL dpar)
{
	m_dHrgnPar = dpar;
}

void CSWakText::GetKtknPar(LPDBL dpar)
{
	*dpar = m_dKtknPar;
}

void CSWakText::SetKtknPar(DBL dpar)
{
	m_dKtknPar = dpar;
}

void CSWakText::GetBlncePar(LPDBL dpar)
{
	*dpar = m_dBlncePar;
}

void CSWakText::SetBlncePar(DBL dpar)
{
	m_dBlncePar = dpar;
}

void CSWakText::GetOderKind(int *kind)
{
	*kind = m_nOder;
}

void CSWakText::SetOderKind(int kind)
{
	m_nOder = kind;
}

void CSWakText::GetBlnceKind(int *kind)
{
	*kind = m_nBlnceKind;
}

void CSWakText::SetBlnceKind(int kind)
{
	m_nBlnceKind = kind;
}

void CSWakText::StringElementInit()
{
	DBL		sx, sy;
	DBL		size, wd, hi;
	DBL		hpar, kpar, bpar, fpar;
	DBL		kaddx, kaddy;
	DBL		haddx, haddy;
	DBL		baddx, baddy;
	DBL		kwd, khi;
	DBL		hwd, hhi;
	DBL		bwd, bhi;
	UINT	code;
	POSITION	pos;
	CMojiText*	pcMoji;
	CMojiText*	pcFirstMoji;
	CMojiText*	pcLastMoji;

	GetHrgnPar(&hpar);
	GetKtknPar(&kpar);
	GetBlncePar(&bpar);
	GetFlatPar(&fpar);
	sx = m_dSX;
	sy = m_dSY;

	GetFontSize(&wd, &hi);

	pos = m_cMojiTextList.GetHeadPosition();
	if(pos == NULL){
		return;
	}
	pcFirstMoji = m_cMojiTextList.GetAt(pos);
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);

		pcMoji->GetBaseFontSize(&size);
		wd = size;
		hi = size;
		if(fpar != NOSETPAR){
			if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
				hi = wd * fpar / 100.0;
			}
			else{
				wd = hi * fpar / 100.0;
			}
		}

		kwd = wd * kpar / 100.0;
		khi = hi * kpar / 100.0;
		kaddx = (wd - kwd) / 2;
		kaddy = (hi - khi) / 2;

		hwd = wd * hpar / 100.0;
		hhi = hi * hpar / 100.0;
		haddx = (wd - hwd) / 2;
		haddy = (hi - hhi) / 2;

		pcMoji->GetCode(&code);
		if(pcMoji->IsZenkaku()){
			if(0x829f <= code && code <= 0x82f1){
				pcMoji->SetStartXY(sx+haddx, sy-haddy);
				pcMoji->SetFontSize(hwd, hhi);
			}
			else if(0x8340 <= code && code <= 0x8396){
				pcMoji->SetStartXY(sx+kaddx, sy-kaddy);
				pcMoji->SetFontSize(kwd, khi);
			}
			else{
				pcMoji->SetStartXY(sx, sy);
				pcMoji->SetFontSize(wd, hi);
			}
			if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
				sy -= hi;
			}
			else{
				sx += wd;
			}
		}
		else{
			pcMoji->SetStartXY(sx, sy);
			pcMoji->SetFontSize(wd, hi);
			if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
				sy -= hi/2;
			}
			else{
				sx += wd/2;
			}
		}
		pcLastMoji = pcMoji;
	}
	if(m_nBlnceKind == 0 || m_nBlnceKind == 1){
		pcFirstMoji->GetFontSize(&wd, &hi);
		pcFirstMoji->GetStartXY(&sx, &sy);
		bwd = wd * bpar / 100.0;
		bhi = hi * bpar / 100.0;
		baddx = (wd - bwd) / 2;
		baddy = (hi - bhi) / 2;
		pcFirstMoji->SetFontSize(bwd, bhi);
		pcFirstMoji->SetStartXY(sx+baddx, sy-baddy);
	}
	if(m_nBlnceKind == 1 || m_nBlnceKind == 2){
		pcLastMoji->GetFontSize(&wd, &hi);
		pcLastMoji->GetStartXY(&sx, &sy);
		bwd = wd * bpar / 100.0;
		bhi = hi * bpar / 100.0;
		baddx = (wd - bwd) / 2;
		baddy = (hi - bhi) / 2;
		pcLastMoji->SetFontSize(bwd, bhi);
		pcLastMoji->SetStartXY(sx+baddx, sy-baddy);
	}
}

void CSWakText::StrgFiting()
{
	DBL		sx, sy, ex, ey;
	DBL		xdpi, ydpi, swd, shi;
	DBL		dwd, dhi;
	DBL		xtime, ytime;
	CVect	*pcVect;

	GetDMiniMax(&sx, &sy, &ex, &ey);
	m_dSX = sx;
	m_dSY = sy;
	if(m_nWakuKind == DEFWAKU
	|| m_nWakuKind == NAMEWAKU
	|| m_nWakuKind == MAEWAKU1
	|| m_nWakuKind == MAEWAKU2
	|| m_nWakuKind == POPWAKU){
		StringElementInit();
		StrgLineStruct();
	}
	else{
		if(m_pcDataFDib != NULL && m_pcDataFDib->m_pcDibCData != NULL){
			GetDMiniMax(&sx, &sy, &ex, &ey);
			m_pcDataFDib->m_pcDibCData->GetSrcElement(&xdpi, &ydpi, &swd, &shi);
			dwd = ex - sx;
			dhi = ey - sy;
			xtime = dwd / swd;
			ytime = dhi / shi;
			if(fabs(xtime) < fabs(ytime)){
				switch(m_nOder){
				case ODEREQUAL:
					break;
				case ODERCNTR:
					dhi = shi * xtime;
					sy = sy + ((ey-sy)-dhi)/2;
					break;
				case ODERLEFT:
					dhi = shi * xtime;
					sy = ey - dhi;
					break;
				case ODERRIGHT:
					dhi = shi * xtime;
					break;
				}
			}
			else{
				switch(m_nOder){
				case ODEREQUAL:
					break;
				case ODERCNTR:
					dwd = swd * ytime;
					sx = sx + ((ex-sx)-dwd)/2;
					break;
				case ODERLEFT:
					dwd = swd * ytime;
					break;
				case ODERRIGHT:
					dwd = swd * ytime;
					sx = ex - dwd;
					break;
				}
			}
			m_pcDataFDib->m_pcDibCData->SetDstElement(sx, sy, dwd, dhi);
			pcVect = (CVect *)(((CDataList*)(m_pcDataFDib->m_pcDataTop))->m_pcDataTop);
			pcVect->SetAtrDXY(0, 0, sx, sy);
			pcVect->SetAtrDXY(1, 0, sx, ey);
			pcVect->SetAtrDXY(2, 0, ex, ey);
			pcVect->SetAtrDXY(3, 0, ex, sy);
		}
	}
	RenewMiniMax();
}

void CSWakText::StrgLineStruct()
{
	int		spkind;

	m_dDataTime = 1.0;
	GetSpaceKind(&spkind);
	if(m_cMojiTextList.IsEmpty()){
		return;
	}
	switch(m_nOder){
	case ODERLEFT:
		switch(spkind){
		case SPACEWAKU:
			SetLenStrgLineStructLeftWaku();
			break;
		case SPACEDATA:
			SetLenStrgLineStructLeftData();
			break;
		}
		break;
	case ODERCNTR:
		switch(spkind){
		case SPACEWAKU:
			SetLenStrgLineStructCntrWaku();
			break;
		case SPACEDATA:
			SetLenStrgLineStructCntrData();
			break;
		}
		break;
	case ODERRIGHT:
		switch(spkind){
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
		switch(spkind){
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
}

void CSWakText::SetLenStrgLineStructLeftData()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		size, space;
	DBL		sp, ep;
	DBL		sumlen, sumspace;
	DBL		setlen;

	sumlen = GetDataSumLength();
	sumspace = GetSumSpace();
	sumlen = sumlen + sumspace;
	m_dLinelen = GetVectLength();
	if(m_dLinelen < sumlen || m_dFlatPar == MJFREEFLATPAR){
		SetLenStrgLineStructEqualData();
		return;
	}

	setlen = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetFontWidth(&size);
		pcMoji->GetDataSEP(&sp, &ep);
		setlen = setlen - sp + size/2;
		pcMoji->SetTT(setlen/m_dLinelen);
		space = GetSpace(pcMoji, pos);
		setlen = setlen - size/2 + ep + space;
	}
}

void CSWakText::SetLenStrgLineStructLeftWaku()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		top, last;
	DBL		size, space;
	DBL		sumlen, sumspace;
	DBL		setlen;
	int		sespkind;

	GetSESpKind(&sespkind);
	if(sespkind == OFF){
		GetTopLastSpace(&top, &last);
	}
	else{
		top = 0;
		last = 0;
	}
	sumlen = GetWakuSumLength();
	sumspace = GetSumSpace();
	//sumlen = sumlen + sumspace - top - last;
	sumlen = sumlen + sumspace - top;
	m_dLinelen = GetVectLength();
	if(m_dLinelen < sumlen || m_dFlatPar == MJFREEFLATPAR){
		SetLenStrgLineStructEqualWaku();
		return;
	}
	setlen = -top;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetFontWidth(&size);
		pcMoji->SetTT((setlen+size/2)/m_dLinelen);
		space = GetSpace(pcMoji, pos);
		pcMoji->GetFontWakuWidth(&size);
		setlen = setlen + size + space;
	}
}

void CSWakText::SetLenStrgLineStructCntrData()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		size, space;
	DBL		sp, ep;
	DBL		sumlen, sumspace;
	DBL		setlen;

	sumlen = GetDataSumLength();
	sumspace = GetSumSpace();
	sumlen = sumlen + sumspace;
	m_dLinelen = GetVectLength();
	if(m_dLinelen < sumlen || m_dFlatPar == MJFREEFLATPAR){
		SetLenStrgLineStructEqualData();
		return;
	}

	setlen = (m_dLinelen - sumlen) / 2;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetFontWidth(&size);
		pcMoji->GetDataSEP(&sp, &ep);
		setlen = setlen + size/2 - sp;
		pcMoji->SetTT(setlen/m_dLinelen);
		space = GetSpace(pcMoji, pos);
		setlen = setlen - size/2 + ep + space;
	}
}

void CSWakText::SetLenStrgLineStructCntrWaku()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		top, last;
	DBL		size, space;
	DBL		sumlen, sumspace;
	DBL		setlen;
	int sespkind;

	GetSESpKind(&sespkind);
	if(sespkind == OFF){
		GetTopLastSpace(&top, &last);
	}
	else{
		top = 0;
		last = 0;
	}
	sumlen = GetWakuSumLength();
	sumspace = GetSumSpace();
	//sumlen = sumlen + sumspace - top - last;
	sumlen = sumlen + sumspace - top;
	m_dLinelen = GetVectLength();
	if(m_dLinelen < sumlen || m_dFlatPar == MJFREEFLATPAR){
		SetLenStrgLineStructEqualWaku();
		return;
	}
	setlen = (m_dLinelen - sumlen) / 2 - top;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetFontWidth(&size);
		pcMoji->SetTT((setlen+size/2)/m_dLinelen);
		space = GetSpace(pcMoji, pos);
		pcMoji->GetMojiWakuSize(&size);
		setlen = setlen + size + space;
	}
}

void CSWakText::SetLenStrgLineStructRightData()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		size, space;
	DBL		sumlen, sumspace;
	DBL		sp, ep;
	DBL		setlen;

	sumlen = GetDataSumLength();
	sumspace = GetSumSpace();
	sumlen = sumlen + sumspace;
	m_dLinelen = GetVectLength();
	if(m_dLinelen < sumlen || m_dFlatPar == MJFREEFLATPAR){
		SetLenStrgLineStructEqualData();
		return;
	}

	setlen = m_dLinelen - sumlen;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetFontWidth(&size);
		pcMoji->GetDataSEP(&sp, &ep);
		setlen = setlen + size/2 - sp;
		pcMoji->SetTT(setlen/m_dLinelen);
		space = GetSpace(pcMoji, pos);
		setlen = setlen - size/2 + ep + space;
	}
}

void CSWakText::SetLenStrgLineStructRightWaku()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		top, last;
	DBL		size, space;
	DBL		sumlen, sumspace;
	DBL		setlen;
	int		sespkind;

	GetSESpKind(&sespkind);
	if(sespkind == OFF){
		GetTopLastSpace(&top, &last);
	}
	else{
		top = 0;
		last = 0;
	}
	sumlen = GetWakuSumLength();
	sumspace = GetSumSpace();
	//sumlen = sumlen + sumspace - top - last;
	sumlen = sumlen + sumspace - top;
	m_dLinelen = GetVectLength();
	if(m_dLinelen < sumlen || m_dFlatPar == MJFREEFLATPAR){
		SetLenStrgLineStructEqualWaku();
		return;
	}

	setlen = m_dLinelen - sumlen - top;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetFontWidth(&size);
		pcMoji->SetTT((setlen + size/2)/m_dLinelen);
		space = GetSpace(pcMoji, pos);
		pcMoji->GetMojiWakuSize(&size);
		setlen = setlen + size + space;
	}
}

void CSWakText::SetLenStrgLineStructEqualData()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		size, space;
	int		mojicnt;
	DBL		sumlen, sumspace;
	DBL		sp, ep;
	DBL		datatime, spacetime, addspace;
	DBL		setlen;
	DBL		wd, hi;

	sumlen = GetDataSumLength();
	if(sumlen == 0) return;
	sumspace = GetSumSpace();
	mojicnt = GetMojiCount();
	m_dLinelen = GetVectLength();
	if(m_dFlatPar == MJFREEFLATPAR){
		datatime = (m_dLinelen-sumspace) / sumlen;
		spacetime = 1.0;
		addspace = 0.0;
	}
	else{
		if(m_dLinelen < (sumlen + sumspace)){
			datatime = m_dLinelen / (sumlen+sumspace);
			spacetime = datatime;
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
				addspace = (m_dLinelen - (sumlen+sumspace)) / (mojicnt-1);
			}
		}
	}
	if(mojicnt == 1){
		setlen = m_dLinelen / 2;
	}
	else{
		setlen = 0;
	}
	m_dDataTime = datatime;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetFontSize(&wd, &hi);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			hi = hi * datatime;
			size = hi;
		}
		else{
			wd = wd * datatime;
			size = wd;
		}
		pcMoji->SetFontSize(wd, hi);
		pcMoji->GetDataSEP(&sp, &ep);
		if(mojicnt != 1){
			setlen = setlen + size/2 - sp;
		}
		pcMoji->SetTT(setlen/m_dLinelen);
		space = GetSpace(pcMoji, pos);
		space = space * spacetime + addspace;
		setlen = setlen - size/2 + ep + space;
	}
}

void CSWakText::SetLenStrgLineStructEqualWaku()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		top, last;
	DBL		size, space;
	int		mojicnt;
	DBL		sumlen, sumspace;
	DBL		datatime, spacetime, addspace;
	DBL		setlen;
	DBL		wd, hi;
	DBL		sx, sy;
	int sespkind;

	GetSESpKind(&sespkind);
	if(sespkind == OFF){
		GetTopLastSpace(&top, &last);
	}
	else{
		top = 0;
		last = 0;
	}
	sumlen = GetWakuSumLength();
	if(sumlen == 0) return;
	sumspace = GetSumSpace();
	mojicnt = GetMojiCount();
	sumlen -= (top + last);
	//sumlen -= (top);
	m_dLinelen = GetVectLength();
	if(m_dFlatPar == MJFREEFLATPAR){
		datatime = (m_dLinelen-sumspace) / sumlen;
		spacetime = 1.0;
		addspace = 0.0;
	}
	else{
		if(m_dLinelen < (sumlen+sumspace)){
			datatime = m_dLinelen / (sumlen+sumspace);
			spacetime = datatime;
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
				addspace = (m_dLinelen - (sumlen+sumspace)) / (mojicnt-1);
			}
		}
	}
	//SetFlatPar(datatime*100.0);
	if(mojicnt == 1){
		if(m_dLinelen > sumlen){
			setlen = (m_dLinelen-sumlen) / 2 - top*datatime;
		}
		else{
			setlen = - top*datatime;
		}
	}
	else{
		setlen = - top*datatime;
	}
	m_dDataTime = datatime;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetFontSize(&wd, &hi);
		pcMoji->GetFontWidth(&size);
		pcMoji->GetStartXY(&sx, &sy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			sy -= hi*(1.0-datatime);
			hi = hi * datatime;
			//sy -= size*(1.0-datatime);
			size = size* datatime;
		}
		else{
			sx -= wd*(1.0-datatime);
			wd = wd * datatime;
			//sx -= size*(1.0-datatime);
			size = size * datatime;
		}
		pcMoji->SetStartXY(sx, sy);
		pcMoji->SetFontSize(wd, hi);
		pcMoji->RenewMiniMax();
		pcMoji->SetTT((setlen + size/2)/m_dLinelen);
		space = GetSpace(pcMoji, pos);
		space = space * spacetime + addspace;
		pcMoji->GetFontWakuWidth(&size);
		setlen = setlen + size + space;
	}
}

void CSWakText::GetTopLastSpace(LPDBL top, LPDBL last)
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		sp, ep;
	DBL		mojisize;

	pos = m_cMojiTextList.GetHeadPosition();
	if(pos != NULL){
		pcMoji = m_cMojiTextList.GetAt(pos);
		pcMoji->GetDataSEP(&sp, &ep);
		*top = sp;
	}
	else{
		*top = 0;
		*last = 0;
		return;
	}
	pos = m_cMojiTextList.GetTailPosition();
	pcMoji = m_cMojiTextList.GetAt(pos);
	pcMoji->GetDataSEP(&sp, &ep);
	// 2001.07.03 プロポーショナル書体がはみ出すを修正
	pcMoji->GetMojiWakuSize(&mojisize);
	*last = (mojisize) - ep;
}

void CSWakText::StrgLineStructTrns()
{
	POSITION	pos;
	CMojiText*	pcMoji;
	DBL		tt;
	DBL		setlen;
	DBL		mminix, mminiy, mmaxx, mmaxy;
	DBL		cx, cy;
	DBL		movex, movey;
	DBL		tsin, tcos;
	DBL		rag, dx, dy;
	DBL		dw, dh;

	GetFontSize(&dw, &dh);
	m_dLinelen = GetVectLength();
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		//pcMoji->GetWakuDMiniMax(&mminix, &mminiy, &mmaxx, &mmaxy);
		pcMoji->GetDMiniMax(&mminix, &mminiy, &mmaxx, &mmaxy);
		pcMoji->GetTT(&tt);
		setlen = m_dLinelen * tt;
		rag = GetVectPointRagAndCood(&setlen, &dx, &dy);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			rag += M_PI_2;
		}
		cx = (mminix + mmaxx) / 2;
		cy = (mminiy + mmaxy) / 2;
		movex = dx - cx;
		movey = dy - cy;
		tsin = sin(rag);
		tcos = cos(rag);
		pcMoji->MoveTrns(movex, movey);
		if(m_nRoundKind == ON){
			pcMoji->RoundTrns(dx, dy, tsin, tcos);
		}
	}
}

DBL CSWakText::GetVectLength()
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

DBL CSWakText::GetRejionLength()
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

DBL CSWakText::GetNorejionLength()
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

DBL CSWakText::GetBezeLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
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

DBL CSWakText::GetArcLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

DBL CSWakText::GetFreeCurveLength(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

DBL CSWakText::GetLineLength(DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		len;

	len = hypot(x1-x2, y1-y2);
	return(len);
}

// 点までの長さを求める
DBL CSWakText::GetVectPointLength(DBL xx, DBL yy)
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

DBL CSWakText::GetRejionPointLength(int *flag, DBL xx, DBL yy)
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

DBL CSWakText::GetNorejionPointLength(int *flag, DBL xx, DBL yy)
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

DBL CSWakText::GetBezePointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
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

DBL CSWakText::GetArcPointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

DBL CSWakText::GetFreeCurvePointLength(int *flag, DBL xx, DBL yy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

DBL CSWakText::GetLinePointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2)
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
DBL CSWakText::GetVectPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy)
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

DBL CSWakText::GetRejionPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy)
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

DBL CSWakText::GetNorejionPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy)
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

DBL CSWakText::GetBezePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
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

DBL CSWakText::GetArcPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

DBL CSWakText::GetFreeCurvePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

DBL CSWakText::GetLinePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2)
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

void CSWakText::DeleteData(CData *delData)
{
	CVect	*pcVect;

	if(m_pcFitingVect == delData){
		pcVect = (CVect*)(m_pcFitingVect->CopyData(0, 0));
		m_pcFitingVect = pcVect;
		return;
	}
	CStrgText::DeleteData(delData);
}

CData* CSWakText::SrchDataBoxs(DBL dx, DBL dy, DBL *retlen)
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

DBL CSWakText::GetPropStrglen()
{
	POSITION	pos;
	CMojiText	*pcMoji;
	DBL		sumlen;
	DBL		size, wd, hi;
	DBL		posttbl[FPROPSIZE];
	DBL		npretbl[FPROPSIZE], nposttbl[FPROPSIZE];
	int		i;
	DBL		mini;

	sumlen = 0;
	for(i = 0; i < FPROPSIZE; i++){
		posttbl[i] = 0;
	}
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetPrePostTable(npretbl, nposttbl);
		pcMoji->GetFontSize(&wd, &hi);
		if(CheckTateYoko() == TATEGAKI || CheckTateYoko() == TATEOKI){
			size = hi;
		}
		else{
			size = wd;
		}
		mini = GetMiniSpace(npretbl, posttbl);
		sumlen += (size - mini);
		for(i = 0; i < FPROPSIZE; i++){
			posttbl[i] = nposttbl[i];
		}
	}
	for(i = 0; i < FPROPSIZE; i++){
		npretbl[i] = 0;
	}
	mini = GetMiniSpace(npretbl, posttbl);
	sumlen -= mini;
	return(sumlen);
}

DBL CSWakText::GetMiniSpace(DBL pretbl[], DBL posttbl[])
{
	int		i;
	DBL		mini, tmini;
	DBL		postmini, premini;

	mini = INTMAX;
	for(i = 0; i < FPROPSIZE; i++){
		if(pretbl[i] != INTMAX && posttbl[i] != INTMAX){
			tmini = pretbl[i] + posttbl[i];
			if(mini > tmini){
				mini = tmini;
			}
		}
	}
	if(mini == INTMAX){
		premini = INTMAX;
		postmini = INTMAX;
		for(i = 0; i < FPROPSIZE; i++){
			if(pretbl[i] != INTMAX){
				if(premini > pretbl[i]){
					premini = pretbl[i];
				}
			}
			if(posttbl[i] != INTMAX){
				if(postmini > posttbl[i]){
					postmini = posttbl[i];
				}
			}
		}
		if(premini == INTMAX){
			premini = 0;
		}
		if(postmini == INTMAX){
			postmini = 0;
		}
		mini = premini + postmini;
	}
	return(mini);
}

DBL CSWakText::GetSumSpace()
{
	POSITION	pos;
	CMojiText	*pcMoji;
	DBL		sumspace;
	DBL		space;

	sumspace = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		space = GetSpace(pcMoji, pos);
		sumspace += space;
	}
	return(sumspace);
}

DBL CSWakText::GetSpace(CMojiText *pcMoji, POSITION pos)
{
	POSITION	tpos;
	CMojiText	*pcNextMoji;
	DBL		size;
	DBL		space;

	tpos = pos;
	if(tpos == NULL){
		return(0.0);
	}
	pcNextMoji = m_cMojiTextList.GetNext(pos);
	pcMoji->GetFontBody(&size);
	if(pcMoji->m_dSpacePar == NOSETPAR){
		space = size * m_dSpacePar / 100.0;
	}
	else{
		space = size * pcMoji->m_dSpacePar / 100.0;
	}
	/*
	if(!(pcMoji->IsZenkaku() && pcNextMoji->IsZenkaku())){
		space /= 2;
	}
	*/
	return(space);
}

int CSWakText::GetMojiCount()
{
	POSITION	pos;
	CMojiText	*pcMoji;
	int		mojicnt;

	mojicnt = 0;
	pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		mojicnt++;
	}
	return(mojicnt);
}

void CSWakText::ResetMojiSize()
{
	CMojiText	*pcMoji;

	POSITION pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->SetFontSize(m_dSize, m_dSize);
		// 2001.07.01
		pcMoji->SetBaseFontSize(m_dSize);
	}
}

void CSWakText::LegalMojiSize()
{
	CMojiText	*pcMoji;
	DBL		par, tpar;
	DBL		wd, hi;

	POSITION pos = m_cMojiTextList.GetHeadPosition();
	while(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
		pcMoji->GetFontSize(&wd, &hi);
		pcMoji->GetFlatPar(&tpar);
		if(tpar == NOSETPAR){
			tpar = 100.0;
		}
		par = m_dFlatPar * tpar / 100.0;
		if(CheckTateYoko() == TATEGAKI){
			hi = wd * par / 100.0;
		}
		else{
			wd = hi * par / 100.0;
		}
		pcMoji->SetFontSize(wd, hi);
	}
}

void CSWakText::SetUseClrPenno(int notbl[], LPINT cnt)
{
	int		penno, i;

	if(m_nPrntOut == OFF){
		return;
	}
	if(m_nWakuKind == LOGOWAKU){
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
	else if(m_nWakuKind == BMPWAKU){
		penno = 1;
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
	else if(m_nWakuKind == DEFWAKU 
		 || m_nWakuKind == NAMEWAKU 
		 || m_nWakuKind == MAEWAKU1
		 || m_nWakuKind == MAEWAKU2
		 || m_nWakuKind == POPWAKU){
		CStrgText::SetUseClrPenno(notbl, cnt);
	}
}

BOOL CSWakText::CheckDataChange()
{
	if(m_nWakuKind == BMPWAKU || m_nWakuKind == LOGOWAKU){
		return(FALSE);
	}
	return(TRUE);
}

void CSWakText::SaveEpsFile()
{
	char	strg[RECMAX];
	DBL		sx, sy, ex, ey;
	DBL		wd, hi, fl, sp;
	int		od, ty;

	GetString(strg);
	GetDMiniMax(&sx, &sy, &ex, &ey);
	switch(m_nOder){
	case ODERLEFT:
		od = 0;
		break;
	case ODERCNTR:
		od = 1;
		break;
	case ODERRIGHT:
		od = 2;
		break;
	case ODEREQUAL:
		od = 4;
		break;
	}
	ty = CheckTateYoko();
	fl = 100;
	sp = 0;
	CMojiText *pcMoji = NULL;
	POSITION pos = m_cMojiTextList.GetHeadPosition();
	if(pos != NULL){
		pcMoji = m_cMojiTextList.GetNext(pos);
	}
	if(pcMoji != NULL){
		pcMoji->GetFontSize(&wd, &hi);
		if(ty == 0){
			fl = wd / hi * 100;
		}
		else{
			fl = hi / wd * 100;
		}
		if(fl == 0){
			fl = 100;
		}
		pcMoji->GetSpacePar(&sp);
		if(sp == NOSETPAR){
			sp = 0;
		}
		sp *= 10;
	}
	epsfboxsstrgsave(strg, ty, od, fl, sp, sx, sy, ex, ey); 
}

DBL CSWakText::GetDataTime()
{
	return(m_dDataTime);
}
