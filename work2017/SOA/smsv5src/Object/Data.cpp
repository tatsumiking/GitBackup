
//
// Easy Scanner System
//
// object data.cpp  データ構造のベースクラス関係関数
//
//  Copyright (c) 1998 by THEON

#include "StdAfx.h"

#include "FSF1Font.h"
#include "WinTTFont.h"
#include "ESF0Font.h"
#include "ESF1Font.h"
#include "FBU0Font.h"
#include "ARCFont.h"
#include "ARKFont.h"
#include "MX2Font.h"
#include "SCRFont.h"

#include "Data.h"
#include "Vect.h"
#include "DibCData.h"
#include "DataList.h"
#include "MojiVect.h"
#include "StrgVect.h"
#include "WakuVect.h"
#include "MsurVect.h"
#include "DataGrup.h"
#include "DataFDib.h"
#include "DataLogo.h"
#include "SPtnVect.h"
#include "SWakVect.h"
#include "ClipData.h"
#include "MetaData.h"

#include	"../ObjLib/Curvelib.h"
#include	"../ObjLib/Scrnlib.h"

#define	DIVCALC	1
#define	RLTVLIMIT 32

IMPLEMENT_SERIAL(CData, CObject, 3)
CData::CData()
{
}

void CData::Serialize(CArchive& ar)
{
	int		nID;

	if (ar.IsStoring())
	{
		ar << m_nFlag;
		ar << m_dMinix;
		ar << m_dMiniy;
		ar << m_dMaxx;
		ar << m_dMaxy;
		ar << m_rectData;
		if(m_pcDataNext != NULL){
			nID = m_pcDataNext->CheckID();
			ar << nID;
			m_pcDataNext->Serialize(ar);
		}
		else{
			nID = NOOPDATAID;
			ar << nID;
		}
	}
	else
	{
		ar >> m_nFlag;
		ar >> m_dMinix;
		ar >> m_dMiniy;
		ar >> m_dMaxx;
		ar >> m_dMaxy;
		ar >> m_rectData;
		ar >> nID;
		if(nID != NOOPDATAID && (1 < nID && nID < 100)){
			m_pcDataNext = NewDataCreate(nID);
			m_pcDataNext->Serialize(ar);
		}
		else{
			m_pcDataNext = NULL;
		}

		if(ar.m_pDocument == NULL){
			m_pcWnd = NULL;
		}
		else{
			POSITION pos = ar.m_pDocument->GetFirstViewPosition();
			CView *pcView = (CView *)(ar.m_pDocument->GetNextView(pos));
			m_pcWnd = (CWnd*)pcView;
		}
	}
	if (!(ar.IsStoring()))
	{
		RenewMiniMax();
	}
}

CData::CData(int nDummy)
{
	int	nminx, nminy, nmaxx, nmaxy;

	m_nFlag = REJIONFLAG | VUSEFLAG | ACTIVEFLAG;
	nminx = 0; nminy = 0;
	nmaxx = 0; nmaxy = 0;
	m_rectData.SetRect(nminx, nminy, nmaxx, nmaxy);
	m_pcDataNext = NULL;
	m_dMinix = 0;
	m_dMiniy = 0;
	m_dMaxx = 0;
	m_dMaxy = 0;
	m_pcFSF1Font = NULL;
}

CData::~CData()
{
}

int CData::CheckNullData()
{
	return(FALSE);
}

int CData::CheckID()
{
	return(NOOPDATAID);
}

void CData::BaseFontLink(CFSF1Font *pFSF1Font)
{
	m_pcFSF1Font = pFSF1Font;
}

void CData::WNTTFontLink(CWinTTFont *pWNTTFont)
{
	m_pcWNTTFont = pWNTTFont;
}

void CData::ESF0FontLink(CESF0Font *pESF0Font)
{
	m_pcESF0Font = pESF0Font;
}

void CData::ESF1FontLink(CESF1Font *pESF1Font)
{
	m_pcESF1Font = pESF1Font;
}

void CData::FBU0FontLink(CFBU0Font *pFBU0Font)
{
	m_pcFBU0Font = pFBU0Font;
}

void CData::ARCFontLink(CARCFont *pARCFont)
{
	m_pcARCFont = pARCFont;
}

void CData::ARKFontLink(CARKFont *pARKFont)
{
	m_pcARKFont = pARKFont;
}

void CData::MX2FontLink(CMX2Font *pMX2Font)
{
	m_pcMX2Font = pMX2Font;
}

void CData::SCRFontLink(CSCRFont *pSCRFont)
{
	m_pcSCRFont = pSCRFont;
}

void CData::SetNext(CData *pData)
{
	m_pcDataNext = pData;
}

CData* CData::GetNext()
{
	return(m_pcDataNext);
}
void CData::ResetVectLink()
{
}

void CData::GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	*minix = INTMAX;
	*miniy = INTMAX;
	*maxx = -INTMAX;
	*maxy = -INTMAX;
}

void CData::GetMojiWakuMiniMax(int mojiidx, DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	*minix = INTMAX;
	*miniy = INTMAX;
	*maxx = -INTMAX;
	*maxy = -INTMAX;
}

void CData::RenewMiniMax()
{
}

void CData::MoveTrns(DBL movex, DBL movey)
{
}

void CData::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
}

void CData::SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
{
}

void CData::MirrorTrns(DBL a, DBL b, DBL t1, DBL t2)
{
}

void CData::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
}

void CData::StrctBoxsTrns()
{
}

void CData::MtrxTrns()
{
}

CData *CData::GetData(int linkno)
{
	return(NULL);
}

CData *CData::SrchData(DBL xx, DBL yy, DBL *retlen)
{
	*retlen = INTMAX;
	return(this);
}

CData *CData::SrchIdsData(int *nIDary, int nIDcnt, DBL xx, DBL yy, DBL *retlen)
{
	*retlen = INTMAX;
	return(this);
}

CData *CData::SrchDataBoxs(DBL xx, DBL yy, DBL *retlen)
{
	*retlen = INTMAX;
	return(this);
}

CData *CData::SrchIdsDataBoxs(int *nIDary, int nIDcnt, DBL xx, DBL yy, DBL *retlen)
{
	*retlen = INTMAX;
	return(this);
}

void CData::CheckFlagDraw(int flag, CDC *pcDC)
{
}

void CData::CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick)
{
}

void CData::CheckFlagOutlineDraw(int flag, CDC *pcDC)
{
}

void CData::AtntionDraw(CDC *pcDC)
{
}

void CData::NoactDraw(CDC *pcDC)
{
}

void CData::AtntionMojiDraw(CDC *pcDC, int idx)
{
}

void CData::DelDraw(CDC *pcDC)
{
}

void CData::StockClrDraw(CDC *pcDC)
{
}

void CData::PlotOut(int nComID)
{
}

void CData::OnDraw(CDC *pcDC)
{
}
void CData::OutlineDraw(CDC *pcDC)
{
}
void CData::PlgnDraw(CDC *pcDC)
{
}
void CData::FillDraw(CDC *pcDC)
{
}
void CData::GdiplusFillDraw(CGdiPlusEx *pcGpe)
{
}
void CData::GdiplusPlgnDraw(CGdiPlusEx *pcGpe)
{
}
void CData::PLDraw(CDC *pcDC)
{
}
int CData::GetFlag()
{
	return(m_nFlag);
}
void CData::SetFlag(int nFlag)
{
	m_nFlag = nFlag;
}

int CData::CheckFlag(int bitflag)
{
	if(m_nFlag & bitflag){
		return(ON);
	}
	else{
		return(OFF);
	}
}

void CData::AllOnPointFlag(int bitflag)
{
}

void CData::AllOffPointFlag(int bitflag)
{
}

void CData::OnFlag(int bitflag)
{
	m_nFlag |= bitflag;
}

void CData::OffFlag(int bitflag)
{
	m_nFlag &= ~bitflag;
}

void CData::ChngFlag(int bitflag)
{
	if(m_nFlag & bitflag){
		m_nFlag &= ~bitflag;
	}
	else{
		m_nFlag |= bitflag;
	}
}

CRect* CData::GetBoundingRect()
{
	DBL	dminx, dminy, dmaxx, dmaxy;
	int	nminx, nminy, nmaxx, nmaxy;

	dminx = m_dMinix;
	dminy = m_dMiniy;
	dmaxx = m_dMaxx;
	dmaxy = m_dMaxy;
	scrndxymmtorltv(&dminx, &dminy);
	scrndxymmtorltv(&dmaxx, &dmaxy);
	nminx = (int)dminx;
	nminy = (int)dminy;
	nmaxx = (int)dmaxx;
	nmaxy = (int)dmaxy;
	m_rectData.SetRect(nminx, nminy, nmaxx, nmaxy);
	return(&m_rectData);
}

int CData::GetFillPen()
{
	return(-1);
}

int CData::GetOutLinePen()
{
	return(-1);
}

DBL CData::GetOutLineWidth()
{
	return(0.0);
}

void CData::WakuSave(FILE * fp)
{
	return;
}

void CData::WakuLoad(FILE * fp, LPTSTR buf)
{
	return;
}

void CData::InAreaDataAtntionOn(DBL sx, DBL sy, DBL ex, DBL ey)
{
	return;
}


void CData::SetAtnVectFSF1fBuf(CFSF1Font * pcFSF1Font)
{

}

CData *CData::SrchNearOnLinePoint(DBL xx, DBL yy, DBL * retlen, DBL * retx, DBL * rety, int * retpp)
{
	*retlen = INTMAX;
	*retx = xx;
	*rety = yy;
	*retpp = 0;
	return(this);
}

CData* CData::SrchNearPoint(DBL xx, DBL yy, DBL * retlen, int * retpp)
{
	*retlen = INTMAX;
	*retpp = 0;
	return(this);
}

void CData::DeleteData(CData * pData)
{

}

void CData::FlagOnDelData(int flag)
{
	
}

CData* CData::NewDataCreate(int nItemID)
{
	CData	*pcData;

	pcData = NULL;
	switch(nItemID){
	case BASEDATAID:
		pcData = new CData(nItemID);
		break;
	case BASEVECTID:
		pcData = (CData *)(new CVect(nItemID));
		break;
	case MOJIVECTID:
		pcData = (CData *)(new CMojiVect(nItemID));
		break;
	case DIBCDATAID:
		pcData = (CData *)(new CDibCData(nItemID));
		break;
	case STRGVECTID:
		pcData = (CData *)(new CStrgVect(nItemID));
		break;
	case WAKUVECTID:
		pcData = (CData *)(new CWakuVect(nItemID));
		break;
	case MSURVECTID:
		pcData = (CData *)(new CMsurVect(nItemID));
		break;
	case DATALISTID:
		pcData = (CData *)(new CDataList(nItemID));
		break;
	case DATAGRUPID:
		pcData = (CData *)(new CDataGrup(nItemID));
		break;
	case DATAFDIBID:
		pcData = (CData *)(new CDataFDib(nItemID));
		break;
	case DATALOGOID:
		pcData = (CData *)(new CDataLogo(nItemID));
		break;
	case SPTNVECTID:
		pcData = (CData *)(new CSPtnVect(nItemID));
		break;
	case SWAKVECTID:
		pcData = (CData *)(new CSWakVect(nItemID));
		break;
	case CLIPDATAID:
		pcData = (CData *)(new CClipData(nItemID));
		break;
	case METADATAID:
		pcData = (CData *)(new CMetaData(nItemID));
		break;
	}
	if(pcData != NULL){
		pcData->BaseFontLink(m_pcFSF1Font);
		pcData->WNTTFontLink(m_pcWNTTFont);
		pcData->ESF0FontLink(m_pcESF0Font);
		pcData->ESF1FontLink(m_pcESF1Font);
		pcData->FBU0FontLink(m_pcFBU0Font);
		pcData->ARCFontLink(m_pcARCFont);
		pcData->ARKFontLink(m_pcARKFont);
		pcData->MX2FontLink(m_pcMX2Font);
		pcData->SCRFontLink(m_pcSCRFont);
		pcData->m_pcWnd = m_pcWnd;
	}
	return(pcData);
}

void CData::SaveStrgStoreFile(FILE * fp, int id)
{

}

void CData::LoadStrgStoreFile(FILE * fp, int id)
{

}
CData * CData::CopyData(DBL movex, DBL movey)
{
	return(NULL);
}

int CData::DrawNo(CDC *pcDC, int noidx)
{
	return(noidx);
}
void CData::DrawSekijiNo(CDC *pcDC)
{
}

void CData::SelectDraw(CDC *pcDC, int no)
{

}

void CData::SelectDraw(CDC *pcDC)
{

}

void CData::NoSelectDraw(CDC *pcDC, int no)
{

}

void CData::NoSelectDraw(CDC *pcDC)
{

}

void CData::PointYLegal(DBL yy)
{

}

void CData::PointXLegal(DBL xx)
{

}

void CData::PointPicup()
{

}


DBL CData::GetPreLengthBoxs(DBL x, DBL y, DBL sx, DBL sy, DBL ex, DBL ey)
{
	DBL		retlen, len;
	DBL		tt, xx, yy;

	retlen = INTMAX;
	if(sx > ex){
		tt = sx;
		sx = ex;
		ex = tt;
	}
	if(sy > ey){
		tt = sy;
		sy = ey;
		ey = tt;
	}
	if(sx < x && x < ex && sy < y && y < ey){
		retlen = HNEARLIMIT;
		scrndlenrltvtomm(&retlen);
	}
	if(GetNearOnlinePoint(x, y, sx, sy, sx, ey, &len, &xx, &yy) != FAIL){
		if(retlen > len){
			retlen = len;
		}
	}
	if(GetNearOnlinePoint(x, y, sx, ey, ex, ey, &len, &xx, &yy) != FAIL){
		if(retlen > len){
			retlen = len;
		}
	}
	if(GetNearOnlinePoint(x, y, ex, ey, ex, sy, &len, &xx, &yy) != FAIL){
		if(retlen > len){
			retlen = len;
		}
	}
	if(GetNearOnlinePoint(x, y, ex, sy, sx, sy, &len, &xx, &yy) != FAIL){
		if(retlen > len){
			retlen = len;
		}
	}
	return(retlen);
}

int CData::GetNearOnlinePoint(DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL *retlen, DBL *retx, DBL *rety)
{
	int		ret;
	DBL		x, y;
	DBL		len1, len2;

	ret = curveperpendicular(x1, y1, x2, y2, xx, yy, &x, &y);
	if(ret == FAIL){
		return(FAIL);
	}
	if(curveonlinechk(x1, y1, x2, y2, x, y) == ON){
		*retlen = hypot(x - xx, y - yy);
		*retx = x;
		*rety = y;
		return(ON);
	}
	else{
		len1 = hypot(xx - x1, yy - y1);
		len2 = hypot(xx - x2, yy - y2);
		if(len1 < len2){
			*retlen = len1;
			*retx = x1;
			*rety = y1;
		}
		else{
			*retlen = len2;
			*retx = x2;
			*rety = y2;
		}
		return(OFF);
	}
}

CData * CData::SrchMoji(DBL x, DBL y, DBL * len, int * linkno)
{
	*len = INTMAX;
	*linkno = -1;
	return(NULL);
}

int CData::SetData(CData * pcData)
{
	return(0);
}

void CData::SetDefOutLineClr()
{
	return;
}

void CData::SetDefFillClr()
{
	return;
}

void CData::SaveEpsFile()
{
	return;
}

void CData::SaveDxfFile()
{
	return;
}

void CData::OutLineOmit(DBL mm)
{
	return;
}

int CData::CheckIdxFlag(int idx, int flag)
{
	return(OFF);
}

void CData::OnIdxFlag(int idx, int flag)
{
	return;
}

void CData::OffIdxFlag(int idx, int flag)
{
	return;
}

int CData::CheckTateYoko()
{
	return(0);
}

CData *CData::GetIdxMoji(int nidx)
{
	return(NULL);
}

void CData::FlagCheckMojiChange(int flag)
{
	return;
}

void CData::ReverceRejion()
{
	return;
}

void CData::StrgFiting()
{
	return;
}

CVect* CData::GetFitingVect()
{
	return(NULL);
}

int CData::CheckOneMojiTrns()
{
	return(OFF);
}

void CData::SetMirrerString(int codetbl[], int len)
{
	int	tcodetbl[RECMAX];
	int		idx;

	for(idx = 0; idx < len; idx++){
		tcodetbl[idx] = codetbl[idx];
	}
	for(idx = 0; idx < len; idx++){
		codetbl[len - idx - 1] = tcodetbl[idx];
	}
}

void CData::DataChange()
{
}

void CData::DataChangeSetDataGrup(CDataGrup *pcDataGrup)
{
}

void CData::SetUseClrPenno(int notbl[], LPINT cnt)
{

}

BOOL CData::CheckDataChange()
{
	return(FALSE);
}

void CData::CnvBezeToLine(int divcnt)
{

}

void CData::RenewMiniMaxPoint(DBL dx, DBL dy)
{
	m_dMinix = min(dx, m_dMinix);
	m_dMiniy = min(dy, m_dMiniy);
	m_dMaxx = max(dx, m_dMaxx);
	m_dMaxy = max(dy, m_dMaxy);
}

void CData::RenewMiniPoint(DBL minix, DBL miniy)
{
	m_dMinix = min(minix, m_dMinix);
	m_dMiniy = min(miniy, m_dMiniy);
}

void CData::RenewMaxPoint(DBL maxx, DBL maxy)
{
	m_dMaxx = max(maxx, m_dMaxx);
	m_dMaxy = max(maxy, m_dMaxy);
}

void CData::AddSubRag(DBL *sumrag, DBL dx, DBL dy, DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		rag1, rag2, subrag;
	int		nrag1, nrag2, nsubrag;

	if(dx == x1 && dy == y1){
		return;
	}
	if(dx == x2 && dy == y2){
		return;
	}
	if(x1 == x2 && y1 == y2){
		return;
	}
	rag1 = atan2(y1-dy, x1-dx);
	rag2 = atan2(y2-dy, x2-dx);
	subrag = rag1 - rag2;
	if(M_PI*2 < subrag){
		subrag -= M_PI*2;
	}
	if(-M_PI*2 > subrag){
		subrag += M_PI*2;
	}
	if(M_PI < subrag){
		subrag -= M_PI*2;
	}
	if(-M_PI > subrag){
		subrag += M_PI*2;
	}
	nrag1 = (int)(rag1 / M_1RAG);
	nrag2 = (int)(rag2 / M_1RAG);
	nsubrag = (int)(subrag / M_1RAG);
	(*sumrag) += subrag;
}

int CData::SrchNearOnPointLine(dfigutype fg, DBL xx, DBL yy, DBL *retlen, DBL *retx, DBL *rety)
{
	int		ret;
	DBL		tx, ty;
	DBL		len1, len2;

	ret = curveperpendicular(fg.x1, fg.y1, fg.x2, fg.y2, xx, yy, &tx, &ty);
	if(ret != FAIL){
		if(curveonlinechk(fg.x1, fg.y1, fg.x2, fg.y2, tx, ty) == ON){
			*retlen = curvegetlength(xx, yy, tx, ty);
			*retx = tx;
			*rety = ty;
			return(TRUE);
		}
	}
	len1 = curvegetlength(xx, yy, fg.x1, fg.y1);
	len2 = curvegetlength(xx, yy, fg.x2, fg.y2);
	if(len1 < len2){
		*retlen = len1;
		*retx = fg.x1;
		*rety = fg.y1;
	}
	else{
		*retlen = len2;
		*retx = fg.x2;
		*rety = fg.y2;
	}
	return(FALSE);
}

DBL CData::GetSrchNearMM()
{
	DBL			len;

	len = HNEARLIMIT;	// 16
	scrndlenrltvtomm(&len);
	return(len);
}
