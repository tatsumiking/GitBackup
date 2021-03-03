
/////////////////////////////////////////////////////////////////////////////
// CCmdOutLine

#include "stdafx.h"
#include "math.h"

#include "cmd_base.h"
#include "curvutil.h"
#include "dlgoutln.h"
#include "cmdnoutl.h"

#define		THICKTIME	1.0

#define	NOCHKPROG		1
//#define	CHKPROG		1
//#define	TCHKPROG		1

extern "C" {
	void senvgetlslctclr(int *rval, int *gval, int *bval);
	void scrndxymmtorltv(LPDBL x, LPDBL y);
	void scrndlenmmtorltv(LPDBL thick);
	int	senvcheckdrawmode();
	void senvsetdrawmode(int mode);
	void curvenoragtocurverag(DBL *rag1, DBL *rag2);
	int curvecrosslnlnxyget(dlinetype line1, dlinetype line2, DBL *x, DBL *y);
	int curvecrossonlinechk(dlinetype line, DBL x, DBL y);
	int curve3pcenterget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL FAR *x0, DBL FAR *y0);
	int curve3pragrget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0, DBL FAR *rag01, DBL FAR *rag03, DBL FAR *r);
	void curvebezecontrolpointget(DBL x1, DBL y1, DBL x2, DBL y2, DBL rag, DBL *xp, DBL *yp);
	void curveonpointtocntrlpoint(dbezetype *dbz, DBL tp1, DBL tp2);
	int curvenkindget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void curvebezetpointget(dbezetype bz, DBL t, DBL *x, DBL *y);
	int crossbezebezecrs(dbezetype *bz1, dbezetype *bz2, DBL *xtbl, DBL *ytbl);
	int crossbezelinecrs(dbezetype *bz1, dlinetype *ln2, DBL *xtbl, DBL *ytbl);
	int crosslinebezecrs(dlinetype *ln1, dbezetype *bz2, DBL *xtbl, DBL *ytbl);
	int crosslinelinecrs(dlinetype *ln1, dlinetype *ln2, DBL *xtbl, DBL *ytbl);
	DBL crossgetbezett(dbezetype *bz, DBL xx, DBL yy);
	DBL crossgetlinett(dlinetype *ln, DBL xx, DBL yy);
	int crossbezettdiv(dbezetype *sbz, DBL tt, dbezetype *dbz1, dbezetype *dbz2);
	int crossbezestetdiv(dbezetype *sbz, DBL st, DBL et, dbezetype *dbz);
	void plgnfillstart();
	void plgnfillrejionstart();
	void plgnfillrejionend();
	void plgnfilldraw(HDC PaintDC);
	void scrnsetclrbrushandpen(HDC PaintDC, COLORREF fillclr, COLORREF lineclr, int nthick);
	void scrnpenend(HDC PaintDC);
	DBL pdrwendragget(int atr1, int atr2, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL pdrwstartragget(int atr0, int atr1, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2);
	BOOL  pdrwatrbezechk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrarcchk(int atr1, int atr2, int atr3);
}

CCmdOutLine::CCmdOutLine(CScrollView* pcview)
:CCmdBase(pcview)
{
	m_dOutLineMM = 1;
	m_nOutLineKind = 1;
}

// 簡略縁取り処理関数
UINT CCmdOutLine::ExecOmitOutLine(UINT event, DBL dx, DBL dy)
{
	CDataList*	pcDataList;
	DBL			thick;

	if(event != INITEVENT){
		return(0);
	}
	CTonDoc *pcDoc = m_pcView->GetDocument();
	if(pcDoc->DataActiveCheck() == FALSE){
		return(ENDEVENT);
	}

	SaveUndo();
	if(DialogOutLineInit() == FALSE){
		return(ENDEVENT);
	}
	m_pcDialogProgresBar = new CDialogProgresBar(m_pcView);
	m_pcDialogProgresBar->Create(IDD_PROGBAR, m_pcView);
	m_pcDialogProgresBar->ShowWindow(SW_SHOW);

	StrgDataChange();	// 文字列はベクトルデータに変換
	pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	CopyDataSetList(pcDataList);	// ベクトルの曲線をＢＥＺＥに変換しコピーを作成
	DelNearPoint(pcDataList);
	BitMapDrawInit(pcDataList);
	thick = 0.0;
	BitMapDraw(pcDataList, thick);
	RejionLegal(pcDataList);
#ifdef _DEBUG
	m_pcCmdSpcl->BmpFileOut("c:\\test0001.bmp", m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, m_pcCmdSpcl->m_nXSize, m_pcCmdSpcl->m_nYSize);
#endif
	BitMapDrawEnd();
	if(pcDataList->m_pcDataTop == NULL){
		delete(pcDataList);
		return(ENDEVENT);
	}
	SetOnlyLineOrBeze(pcDataList); // ベクトルの曲線をＢＥＺＥに変換
#ifdef	NOCHKPROG
	SetOutLine(pcDataList);
	CrossSetPoint(pcDataList);
	DelNearPoint(pcDataList);
	SetRestoreCurve(pcDataList);
#endif
//	LegalDataList(pcDataList);
	pcDataList->RenewMiniMax();
	pcDoc->AddActiveBeforeLink(pcDataList);
	pcDoc->OffFlag(ACTIVEFLAG);
	pcDataList->OnFlag(ACTIVEFLAG);
	m_pcDialogProgresBar->DestroyWindow();
	return(ENDEVENT);
}

UINT CCmdOutLine::ExecOutLine(UINT event, DBL dx, DBL dy)
{
	CDataList	*pcDataList;
	CDataList	*pcTDataList;
	DBL			thick, sub;
	CVect		*pcVect;

	if(event != INITEVENT){
		return(0);
	}
	CTonDoc *pcDoc = m_pcView->GetDocument();
	if(pcDoc->DataActiveCheck() == FALSE){
		return(ENDEVENT);
	}

	SaveUndo();
	if(DialogOutLineInit() == FALSE){
		return(ENDEVENT);
	}
	m_pcDialogProgresBar = new CDialogProgresBar(m_pcView);
	m_pcDialogProgresBar->Create(IDD_PROGBAR, m_pcView);
	m_pcDialogProgresBar->ShowWindow(SW_SHOW);

	HlpMsgDsp("縁取り前処理");
	m_pcDialogProgresBar->m_ProgBar.SetPos(30);
	StrgDataChange();	// 文字列はベクトルデータに変換

	pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	CopyDataSetList(pcDataList);
	DelNearPoint(pcDataList);
	HlpMsgDsp("回転方向正規化処理");
	m_pcDialogProgresBar->m_ProgBar.SetPos(60);
	BitMapDrawInit(pcDataList);
	thick = 0.0;
	pcTDataList = (CDataList*)(pcDataList->CopyData(0.0, 0.0));
	BitMapDraw(pcDataList, thick);
	RejionLegal(pcDataList);
	BitMapDrawEnd();
	if(pcDataList->m_pcDataTop == NULL){
		delete(pcDataList);
		return(ENDEVENT);
	}
	HlpMsgDsp("交点付加処理");
	m_pcDialogProgresBar->m_ProgBar.SetPos(100);
	SetOnlyLineOrBeze(pcDataList); // ベクトルの曲線をＢＥＺＥに変換

	SetOutLine(pcDataList);

	CrossSetPoint(pcDataList);

	// DelSamePoint(pcDataList);

	SetRestoreCurve(pcDataList);

	HlpMsgDsp("不用線削除処理");
	m_pcDialogProgresBar->m_ProgBar.SetPos(900);
	BitMapDrawInit(pcDataList);
	//thick = m_dOutLineMM / m_pcCmdSpcl->m_dTrnsTime;
	// 交点付加処理
	// 線分割処理
	CopyDivDataList(pcDataList);
	// 重複点削除、点ベクトル削除処理
	DelSamePoint();
	// 塗りつぶしに掛かる線削除処理

	sub =  5.0 * m_pcCmdSpcl->m_dTrnsTime;
	if(0 < m_dOutLineMM){
		thick = m_dOutLineMM * THICKTIME;
		thick -= sub;
	}
	else{
		thick = m_dOutLineMM * THICKTIME;
		thick += sub;
	}
	BitMapDraw(pcTDataList, thick);

	DelInpointVect();

	BitMapDrawEnd();

	delete(pcTDataList);
		// 単独閉図形
	HlpMsgDsp("線抽出処理");
	m_pcDialogProgresBar->m_ProgBar.SetPos(950);
#ifdef	NOCHKPROG
	SetOnevectLoop(pcDataList);
		// 外側の線からトレースするためにデータをソートする
	SortPtrList();
		// 線分トレース処理
	//DelNotConectVect();
	OutLineTrace(pcDataList);
#else
	GetDivRestVect(pcDataList);	//　デバッグ用処理
#endif

#ifdef	NOCHKPROG
	pcVect = (CVect*)(pcDataList->m_pcDataTop);
	while(1){
		if(pcVect == NULL){
			break;
		}
		pcVect->OnFlag(REJIONFLAG);
		pcVect = (CVect*)(pcVect->GetNext());
	}
	LegalDataList(pcDataList);
#endif
	pcDataList->RenewMiniMax();
	pcDoc->AddActiveBeforeLink(pcDataList);
	pcDoc->OffFlag(ACTIVEFLAG);
	pcDataList->OnFlag(ACTIVEFLAG);
	HlpMsgDsp("縁取り終了");
	m_pcDialogProgresBar->m_ProgBar.SetPos(1000);
	m_pcDialogProgresBar->DestroyWindow();
	return(ENDEVENT);
}

//p 文字列はベクトルデータに変換
void CCmdOutLine::StrgDataChange()
{
	POSITION	pos;
	CData	*pcData;
	CMojiVect*	pcMojiVect;

	SetDataListSelectID(STRGVECTID, ACTIVEFLAG);
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_cPtrList.GetNext(pos);
		if(pcData == NULL){
			break;
		}
		//((CStrgVect*)pcData)->DataChange();
		pcMojiVect = ((CStrgVect*)pcData)->m_pcMojiTop;
		while(1){
			if(pcMojiVect == NULL){
				break;
			}
			pcMojiVect->DataChange();
			pcMojiVect = pcMojiVect->GetNextMoji();
		}
	}
}

int CCmdOutLine::DialogOutLineInit()
{
	CDialogOutLine	*pcOutLine;
	int		ret;

	pcOutLine = new CDialogOutLine(m_pcView);
	pcOutLine->SetTitle("縁取り");
	pcOutLine->m_dOutLineMM = m_dOutLineMM;
	pcOutLine->m_nOutLineKind = m_nOutLineKind;
	ret = pcOutLine->DoModal();
	if(ret == IDOK){
		m_dOutLineMM = pcOutLine->m_dOutLineMM;
		m_nOutLineKind = pcOutLine->m_nOutLineKind;
		delete(pcOutLine);
		return(TRUE);
	}
	delete(pcOutLine);
	return(FALSE);
}

// ベクトルの曲線をＢＥＺＥに変換しコピーを作成
int CCmdOutLine::CopyDataSetList(CDataList *pcDataList)
{
	POSITION	pos;
	CData	*pcData;
	CData	*pcCopyData;
	int		nCount, pcnt;

	SetDataListSelectID(BASEVECTID, ACTIVEFLAG);

	nCount = 0;
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_cPtrList.GetNext(pos);
		if(pcData == NULL){
			break;
		}
		pcnt = ((CVect*)pcData)->GetPointCount();
		if(2 < pcnt){
			pcCopyData = pcData->CopyData(0.0, 0.0);
			pcDataList->SetData(pcCopyData);
			nCount++;
		}
	}
	pcDataList->RenewMiniMax();
	return(nCount);
}

void CCmdOutLine::RejionLegal(CDataList *pcDataList)
{
	CVect	*pcVect;

	pcVect = (CVect*)(pcDataList->m_pcDataTop);
	while(1){
		if(pcVect == NULL){
			break;
		}
		if(RejionLegalVectXAdd(pcVect) == FALSE){
			RejionLegalVectYAdd(pcVect);
		}
		pcVect = (CVect*)(pcVect->GetNext());
	}
}

void CCmdOutLine::SetOutLine(CDataList *pcDataList)
{
	CVect	*pcVect;
	int		pcnt, pp, dp;
	CVect	*pcDstVect;
	int		atr1, atr2;
	DBL		x1, y1;
	DBL		x2, y2;

	pcDstVect = new CVect(BASEVECTID);

	m_dVectCount = 0;
	pcVect = (CVect*)(pcDataList->m_pcDataTop);
	while(1){
		if(pcVect == NULL){
			break;
		}
		pcnt = pcVect->GetPointCount();
		if(pcnt <= 2){
			continue;
		}
		pcnt = SetOutlineVect(pcVect, pcDstVect);
		pcDstVect->SetPointCount(pcnt);
		dp = 0;
		pcDstVect->GetFAtrDXY(0, &atr1, &x1, &y1);
		for(pp = 1; pp < pcnt; pp++){
			pcDstVect->GetFAtrDXY(pp, &atr2, &x2, &y2);
			if(hypot(x1-x2, y1-y2) > 0.2){
				pcVect->SetFAtrDXY(dp++, atr1, x1, y1);
				atr1 = atr2;
				x1 = x2; y1 = y2;
			}
			else{
				atr1 = atr2;
			}
		}
		pcDstVect->GetFAtrDXY(0, &atr2, &x2, &y2);
		if(hypot(x1-x2, y1-y2) > 0.2){
			pcVect->SetFAtrDXY(dp++, atr1, x1, y1);
		}
		pcVect->SetPointCount(dp);
		pcVect = (CVect*)(pcVect->GetNext());
		m_dVectCount++;
	}
	delete(pcDstVect);
}

int	CCmdOutLine::SetOutlineVect(CVect *pcSrcVect, CVect *pcDstVect)
{
	int		pcnt, pp, np, dp;
	int		atr1, atr2;
	DBL		dx, dy;
	int		i;

	pcnt = pcSrcVect->GetPointCount();
	pcDstVect->SetPointCount(pcnt);
	dp = 0;
	np= pcSrcVect->NextPointGet(0);
	pcSrcVect->GetFAtrDXY(0, &atr1, &dx, &dy);
	m_nKind1 = SetFigu(pcSrcVect, 0, &m_sBz1, &m_sLn1);
	SetOutlineFigu(&m_nKind1, &m_sBz1, &m_sLn1, m_sTbl1, &m_nMax1);
	for(pp = np; pp < pcnt;){
		np = pcSrcVect->NextPointGet(pp);
		pcSrcVect->GetFAtrDXY(pp, &atr2, &dx, &dy);
		m_nKind2 = SetFigu(pcSrcVect, pp, &m_sBz2, &m_sLn2);
		SetOutlineFigu(&m_nKind2, &m_sBz2, &m_sLn2, m_sTbl2, &m_nMax2);
		dp = SetOutlineAcutFigu(atr1, dx, dy, pcDstVect, dp);
		atr1 = atr2;
		m_nKind1 = m_nKind2;
		m_sBz1 = m_sBz2; m_sLn1 = m_sLn2;
		for(i = 0; i < m_nMax2; i++){
			m_sTbl1[i] = m_sTbl2[i];
		}
		m_nMax1 = m_nMax2;
		pp = np;
	}
	pcSrcVect->GetFAtrDXY(0, &atr2, &dx, &dy);
	m_nKind2 = SetFigu(pcDstVect, 0, &m_sBz2, &m_sLn2);
	dp = SetOutlineAcutFigu(atr1, dx, dy, pcDstVect, dp);
	if(m_nKind2 == BEZEKIND){
		pcDstVect->GetFAtrDXY(3, &atr1, &dx, &dy);
		SetOutlineBezeFigu(pcDstVect, atr2, m_sBz2, 0);
		pcDstVect->SetFAtrDXY(3, atr1, dx, dy);
	}
	else{
		SetOutlineLineFigu(pcDstVect, m_sLn2, 0);
	}

	dp = SetOutLineLastPointLegal(pcDstVect, dp);
	return(dp);
}

int	CCmdOutLine::SetOutlineFigu(int *kind1, dbezetype *bz1, dlinetype *ln1, DXYType tbl1[], int *max)
{
	DBL		mm;

	mm = m_dOutLineMM;
	if(*kind1 == BEZEKIND){
		*kind1 = SetOutlineBezePoint(mm, bz1, tbl1, max);
	}
	else{
		SetOutlineLinePoint(mm, ln1);
	}
	return(0);
}

/*
int	CCmdOutLine::SetOutlineBezePoint(DBL mm, dbezetype *bz, DXYType tbl[], int *max)
{
	dlinetype	ln1, ln2, ln3;
	DBL			rag;

	ln1.x1 = bz->x1; ln1.y1 = bz->y1;
	curvebezetpointget(*bz, 0.333, &ln1.x2, &ln1.y2);
	ln2.x1 = ln1.x1; ln2.y1 = ln1.y1;
	curvebezetpointget(*bz, 0.666, &ln2.x2, &ln2.y2);
	ln3.x1 = ln2.x1; ln3.y1 = ln2.y1;
	ln3.x2 = bz->x4; ln3.y2 = bz->y4;

	GetRoundRag(ln1.x1, ln1.y1, ln1.x2, ln1.y2, &rag);
	SetRoundPoint(ln1.x1, ln1.y1, rag, 0.0, mm, &(ln1.x1), &(ln1.y1));
	SetRoundPoint(ln1.x2, ln1.y2, rag, 0.0, mm, &(ln1.x2), &(ln1.y2));
	GetRoundRag(ln2.x1, ln2.y1, ln2.x2, ln2.y2, &rag);
	SetRoundPoint(ln2.x1, ln2.y1, rag, 0.0, mm, &(ln2.x1), &(ln2.y1));
	SetRoundPoint(ln2.x2, ln2.y2, rag, 0.0, mm, &(ln2.x2), &(ln2.y2));
	GetRoundRag(ln3.x1, ln3.y1, ln3.x2, ln3.y2, &rag);
	SetRoundPoint(ln3.x1, ln3.y1, rag, 0.0, mm, &(ln3.x1), &(ln3.y1));
	SetRoundPoint(ln3.x2, ln3.y2, rag, 0.0, mm, &(ln3.x2), &(ln3.y2));

	bz->x1 = ln1.x1; bz->y1 = ln1.y1;
	if(curvecrosslnlnxyget(ln1, ln2, &(bz->x2), &(bz->y2)) == FAIL){
		bz->x2 = ln2.x1; bz->y2 = ln2.y1;
	}
	if(curvecrosslnlnxyget(ln2, ln3, &(bz->x3), &(bz->y3)) == FAIL){
		bz->x3 = ln2.x2; bz->y3 = ln2.y2;	
	}
	bz->x4 = ln3.x2; bz->y4 = ln3.y2;

	curveonpointtocntrlpoint(bz, 0.333, 0.666);

	return(BEZEKIND);
}
*/

int	CCmdOutLine::SetOutlineBezePoint(DBL mm, dbezetype *bz, DXYType tbl[], int *max)
{
	DBL rag, rag1, rag2, rag3;
	dbezetype	tbz;
	dlinetype	ln1, ln2, ln3;
	DBL	dlen, len1, len2;
	DBL	subrag1, subrag2;

	tbz = *bz;

	GetRoundRag(bz->x1, bz->y1, bz->x2, bz->y2, &rag);
	SetRoundPoint(bz->x1, bz->y1, rag, 0.0, mm, &(ln1.x1), &(ln1.y1));
	SetRoundPoint(bz->x2, bz->y2, rag, 0.0, mm, &(ln1.x2), &(ln1.y2));
	GetRoundRag(bz->x2, bz->y2, bz->x3, bz->y3, &rag);
	SetRoundPoint(bz->x2, bz->y2, rag, 0.0, mm, &(ln2.x1), &(ln2.y1));
	SetRoundPoint(bz->x3, bz->y3, rag, 0.0, mm, &(ln2.x2), &(ln2.y2));
	GetRoundRag(bz->x3, bz->y3, bz->x4, bz->y4, &rag);
	SetRoundPoint(bz->x3, bz->y3, rag, 0.0, mm, &(ln3.x1), &(ln3.y1));
	SetRoundPoint(bz->x4, bz->y4, rag, 0.0, mm, &(ln3.x2), &(ln3.y2));

	GetRoundRag(ln1.x1, ln1.y1, ln1.x2, ln1.y2, &rag1);
	GetRoundRag(ln1.x2, ln1.y2, ln3.x1, ln3.y1, &rag2);
	GetRoundRag(ln3.x1, ln3.y1, ln3.x2, ln3.y2, &rag3);
	subrag1 = GetSubRag(rag1, rag2);
	subrag2 = GetSubRag(rag2, rag3);
	if((-M_PI_2 > subrag1 || subrag1 > M_PI_2)
	|| (-M_PI_2 > subrag2 || subrag2 > M_PI_2)){
		*max = SetOutlinePlgnPoint(mm, &tbz, tbl);
		return(PLGNKIND);
	}

	GetRoundRag(bz->x1, bz->y1, bz->x2, bz->y2, &rag1);
	GetRoundRag(bz->x2, bz->y2, bz->x3, bz->y3, &rag2);
	GetRoundRag(bz->x3, bz->y3, bz->x4, bz->y4, &rag3);
	subrag1 = GetSubRag(rag1, rag2);
	subrag2 = GetSubRag(rag2, rag3);
	if((-M_PI_2 > subrag1 || subrag1 > M_PI_2)
	|| (-M_PI_2 > subrag2 || subrag2 > M_PI_2)){
		*max = SetOutlinePlgnPoint(mm, &tbz, tbl);
		return(PLGNKIND);
	}

	bz->x1 = ln1.x1; bz->y1 = ln1.y1;
	if(curvecrosslnlnxyget(ln1, ln2, &(bz->x2), &(bz->y2)) == FAIL){
		bz->x2 = ln2.x1; bz->y2 = ln2.y1;
	}
	if(curvecrosslnlnxyget(ln2, ln3, &(bz->x3), &(bz->y3)) == FAIL){
		bz->x3 = ln2.x2; bz->y3 = ln2.y2;	
	}
	bz->x4 = ln3.x2; bz->y4 = ln3.y2;
	if(mm > 0){
		curvebezetpointget(tbz, 0.5, &(ln1.x1), &(ln1.y1));
		curvebezetpointget(*bz, 0.5, &(ln1.x2), &(ln1.y2));
		dlen = hypot(ln1.x1-ln1.x2, ln1.y1-ln1.y2);
		len1 = hypot(tbz.x1-tbz.x2, tbz.y1-tbz.y2);
		len2 = hypot(bz->x1-bz->x2, bz->y1-bz->y2);
		if(len1 < len2){
			bz->x2 = (bz->x2 - bz->x1) * fabs(mm)/dlen*1.0 + bz->x1;
			bz->y2 = (bz->y2 - bz->y1) * fabs(mm)/dlen*1.0 + bz->y1;
		}
		len1 = hypot(tbz.x3-tbz.x4, tbz.y3-tbz.y4);
		len2 = hypot(bz->x3-bz->x4, bz->y3-bz->y4);
		if(len1 < len2){
			bz->x3 = (bz->x3 - bz->x4) * fabs(mm)/dlen*1.0 + bz->x4;
			bz->y3 = (bz->y3 - bz->y4) * fabs(mm)/dlen*1.0 + bz->y4;
		}
	}
	return(BEZEKIND);
}

int	CCmdOutLine::SetOutlinePlgnPoint(DBL mm, dbezetype *bz, DXYType tbl[])
{
	dlinetype	ln1, ln2;
	dlinetype	ln[12];
	int		i, j;
	int		divcnt;
	DBL		step; 
	DBL		dlen;
	DBL		tx, ty;
	DBL		rag1, rag2, subrag;

	dlen = 0;
	ln1.x1 = bz->x1; ln1.y1 = bz->y1;
	for(i = 1; i <= 10; i++){
		curvebezetpointget(*bz, (DBL)i * 0.1, &(ln1.x2), &(ln1.y2));
		dlen += GetLength(ln1.x1, ln1.y1, ln1.x2, ln1.y2);
	}
	divcnt = (int)(dlen / (mm*2));
	if(divcnt < 1){
		divcnt = 1;
	}
	if(divcnt > 10){
		divcnt = 10;
	}

	step = 1.0 /  divcnt;

	ln[0].x1 = bz->x1; ln[0].y1 = bz->y1;
	for(i = 1; i < divcnt; i++){
		curvebezetpointget(*bz, (DBL)i * step, &(ln[i-1].x2), &(ln[i-1].y2));
		ln[i].x1 = ln[i-1].x2;  ln[i].y1 = ln[i-1].y2;  
	}
	ln[i-1].x2 = bz->x4; ln[i-1].y2 = bz->y4;

	ln1 = ln[0];
	SetOutlineLinePoint(mm, &ln1);
	j = 0;
	tbl[j].x = ln1.x1; tbl[j].y = ln1.y1;
	j++;
	for(i = 1; i < divcnt; i++){
		ln2 = ln[i];
		SetOutlineLinePoint(mm, &ln2);

		GetRoundRag(ln1.x1, ln1.y1, ln1.x2, ln1.y2, &rag1);
		GetRoundRag(ln2.x1, ln2.y1, ln1.x2, ln1.y2, &rag2);
		subrag = GetSubRag(rag1, rag2);
		if(-M_PI_2 < subrag && subrag < M_PI_2){
			tx = (ln1.x2 + ln2.x1) / 2.0;
			ty = (ln1.y2 + ln2.y1) / 2.0;
		}
		else{
			if(curvecrosslnlnxyget(ln1, ln2, &tx, &ty) == FAIL){
				tx = (ln1.x2 + ln2.x1) / 2.0;
				ty = (ln1.y2 + ln2.y1) / 2.0;
			}
		}
		tbl[j].x = tx; tbl[j].y = ty;
		j++;
		ln1 = ln2;
	}
	//tbl[j-1].x = ln1.x2; tbl[j-1].y = ln1.y2;

	tbl[j].x = ln1.x2; tbl[j].y = ln1.y2;
	j++;
	return(j);
}

int	CCmdOutLine::SetOutlineLinePoint(DBL mm, dlinetype *ln)
{
	DBL rag;

	GetRoundRag(ln->x1, ln->y1, ln->x2, ln->y2, &rag);
	SetRoundPoint(ln->x1, ln->y1, rag, 0.0, mm, &(ln->x1),  &(ln->y1));
	SetRoundPoint(ln->x2, ln->y2, rag, 0.0, mm, &(ln->x2),  &(ln->y2));
	return(TRUE);
}

int	CCmdOutLine::SetOutlineAcutFigu(int atr1, DBL cx, DBL cy, CVect *pcDstVect, int dp)
{
	DBL		mm;
	DBL		xtbl[5], ytbl[5];
	int		cnt;
	DBL		rag1, rag2, rag3;
	DBL		subrag, subrag1, subrag2;
	DBL		xx, yy, dx, dy;
	DBL		len1, len2;
	int		nkind1, nkind2;

	mm = m_dOutLineMM;
	if(m_nKind1 == PLGNKIND){
		nkind1 = LINEKIND;
		m_sLn1.x1 = m_sTbl1[m_nMax1-2].x;
		m_sLn1.y1 = m_sTbl1[m_nMax1-2].y;
		m_sLn1.x2 = m_sTbl1[m_nMax1-1].x;
		m_sLn1.y2 = m_sTbl1[m_nMax1-1].y;
	}
	else{
		nkind1 = m_nKind1;
	}
	if(m_nKind2 == PLGNKIND){
		nkind2 = LINEKIND;
		m_sLn2.x1 = m_sTbl2[0].x;
		m_sLn2.y1 = m_sTbl2[0].y;
		m_sLn2.x2 = m_sTbl2[1].x;
		m_sLn2.y2 = m_sTbl2[1].y;
	}
	else{
		nkind2 = m_nKind2;
	}
	cnt = SetOutlineCrossPointSetTbl(nkind1, m_sBz1, m_sLn1, nkind2, m_sBz2, m_sLn2, xtbl, ytbl);
	if(cnt >= 1){
		// 単純に次の線分と交差する時の処理
		SetOutlineCrossDivFigu(nkind1, &m_sBz1, &m_sLn1, nkind2, &m_sBz2, &m_sLn2);
		if(m_nKind1 == PLGNKIND){
			m_sTbl1[m_nMax1-1].x = m_sLn1.x2;
			m_sTbl1[m_nMax1-1].y = m_sLn1.y2;
		}
		if(m_nKind2 == PLGNKIND){
			m_sTbl2[0].x = m_sLn2.x1;
			m_sTbl2[0].y = m_sLn2.y1;
		}
		dp = SetOutLineFiguSetPoint(m_nKind1, atr1, m_sBz1, m_sLn1, m_sTbl1, m_nMax1, pcDstVect, dp);
		return(dp);
	}

	if(m_nKind1 == BEZEKIND){
		m_sLn1.x1 = m_sBz1.x3; m_sLn1.y1 = m_sBz1.y3;
		m_sLn1.x2 = m_sBz1.x4; m_sLn1.y2 = m_sBz1.y4;
	}
	else if(m_nKind1 == PLGNKIND){
		m_sLn1.x1 = m_sTbl1[m_nMax1-2].x; m_sLn1.y1 = m_sTbl1[m_nMax1-2].y;
		m_sLn1.x2 = m_sTbl1[m_nMax1-1].x; m_sLn1.y2 = m_sTbl1[m_nMax1-1].y;
	}

	if(m_nKind2 == BEZEKIND){
		m_sLn2.x1 = m_sBz2.x1; m_sLn2.y1 = m_sBz2.y1;
		m_sLn2.x2 = m_sBz2.x2; m_sLn2.y2 = m_sBz2.y2;
	}
	else if(m_nKind2 == PLGNKIND){
		m_sLn2.x1 = m_sTbl2[0].x; m_sLn2.y1 = m_sTbl2[0].y;
		m_sLn2.x2 = m_sTbl2[1].x; m_sLn2.y2 = m_sTbl2[1].y;
	}

	// 終点と始点が同一であるとき
	if(EQUAL(m_sLn1.x2, m_sLn2.x1) && EQUAL(m_sLn1.y2, m_sLn2.y1)){
		dp = SetOutLineFiguSetPoint(m_nKind1, atr1, m_sBz1, m_sLn1, m_sTbl1, m_nMax1, pcDstVect, dp);
		return(dp);
	}

	// 片方がbeze曲線であるときには中点を終点、始点にする
	len1 = GetLength(m_sLn1.x2, m_sLn1.y2, m_sLn2.x1, m_sLn2.y1);
	if((m_nKind1 != LINEKIND && m_nKind2 != LINEKIND) && (len1 < mm)){
		dx = (m_sLn1.x2 + m_sLn2.x1) / 2.0;
		dy = (m_sLn1.y2 + m_sLn2.y1) / 2.0;
		if(m_nKind1 == BEZEKIND){
			 m_sBz1.x4 = dx; m_sBz1.y4 = dy;
		}
		else if(m_nKind1 == PLGNKIND){
			m_sTbl1[m_nMax1-1].x = dx; m_sTbl1[m_nMax1-1].y = dy;
		}
		else{
			 m_sLn1.x2 = dx; m_sLn1.y2 = dy;
		}

		if(m_nKind2 == BEZEKIND){
			m_sBz2.x1 = dx; m_sBz2.y1 = dy;
		}
		else if(m_nKind2 == PLGNKIND){
			m_sTbl2[0].x = dx; m_sTbl2[0].y = dy;
		}
		else{
			m_sLn2.x1 = dx; m_sLn2.y1 = dy;
		}
		dp = SetOutLineFiguSetPoint(m_nKind1, atr1, m_sBz1, m_sLn1, m_sTbl1, m_nMax1, pcDstVect, dp);
		return(dp);
	}

	dp = SetOutLineFiguSetPoint(m_nKind1, atr1, m_sBz1, m_sLn1, m_sTbl1, m_nMax1, pcDstVect, dp);

	if(curvecrosslnlnxyget(m_sLn1, m_sLn2, &dx, &dy) == FAIL){
		return(dp);
	}

	GetRoundRag(m_sLn1.x1, m_sLn1.y1, m_sLn1.x2, m_sLn1.y2, &rag1);
	GetRoundRag(m_sLn1.x2, m_sLn1.y2, m_sLn2.x1, m_sLn2.y1, &rag2);
	GetRoundRag(m_sLn2.x1, m_sLn2.y1, m_sLn2.x2, m_sLn2.y2, &rag3);
	subrag = GetSubRag(rag1, rag3);
	subrag1 = GetSubRag(rag1, rag2);
	subrag2 = GetSubRag(rag2, rag3);

	xx = (m_sLn1.x2 + m_sLn2.x1)/2;
	yy = (m_sLn1.y2 + m_sLn2.y1)/2;
	
	len1 = hypot(m_sLn1.x2 - m_sLn2.x1,  m_sLn1.y2 - m_sLn2.y1);
	if(len1 < 0.1){
		dp = SetOutLineFiguSetPoint(m_nKind1, atr1, m_sBz1, m_sLn1, m_sTbl1, m_nMax1, pcDstVect, dp);
		return(dp);
	}

	if((-M_PI_2 > subrag1 || subrag1 > M_PI_2)
	|| (-M_PI_2 > subrag2 || subrag2 > M_PI_2)){
		return(dp);
	}
	else if(m_nOutLineKind == 0){
		pcDstVect->SetAtrDXY(dp++, 0, dx, dy);
	}
	else if(m_nOutLineKind == 1){
		if(-M_PI_2 < subrag && subrag < M_PI_2){
			pcDstVect->SetAtrDXY(dp++, 0, dx, dy);
		}
		else{
			len1 = hypot(dx - cx, dy - cy);
			len2 = fabs(mm * 2);
			// 原因不明のバグを回避するため
			sprintf(m_szMsgBuf, "%4.1lf %4.1lf", len1, len2);
			//::AfxMessageBox(m_szMsgBuf, MB_OK);
			if(len1 < len2){
				pcDstVect->SetAtrDXY(dp++, 0, dx, dy);
			}
			else{
				len1 = hypot(m_sLn1.x1-m_sLn1.x2, m_sLn1.y1-m_sLn1.y2);
				dx = (m_sLn1.x2 - m_sLn1.x1) * (fabs(mm) + len1) / len1 + m_sLn1.x1;
				dy = (m_sLn1.y2 - m_sLn1.y1) * (fabs(mm) + len1) / len1 + m_sLn1.y1;
				pcDstVect->SetAtrDXY(dp-1, 0, dx, dy);
				len1 = hypot(m_sLn2.x1-m_sLn2.x2, m_sLn2.y1-m_sLn2.y2);
				dx = (m_sLn2.x1 - m_sLn2.x2) * (fabs(mm) + len1) / len1 + m_sLn2.x2;
				dy = (m_sLn2.y1 - m_sLn2.y2) * (fabs(mm) + len1) / len1 + m_sLn2.y2;
				m_sLn2.x1 = dx; m_sLn2.y1 = dy;
				if(m_nKind2 == BEZEKIND){
					m_sBz2.x1 = dx; m_sBz2.y1 = dy;
				}
				else if(m_nKind2 == PLGNKIND){
					m_sTbl2[0].x = dx; m_sTbl2[0].y = dy;
				}
				else{
					m_sLn2.x1 = dx; m_sLn2.y1 = dy;
				}
			}
		}
	}
	else if(m_nOutLineKind == 2){
		len1 = hypot(dx - cx, dy - cy);
		dx = (dx - cx) * fabs(mm) / len1 + cx;
		dy = (dy - cy) * fabs(mm) / len1 + cy;
		dp = SetArcToBeze(pcDstVect, dp, m_sLn1.x2, m_sLn1.y2, dx, dy, m_sLn2.x1, m_sLn2.y1);
	}
	return(dp);
}

int	CCmdOutLine::SetOutLineFiguSetPoint(int kind, int atr, dbezetype bz, dlinetype ln, DXYType tbl[], int max, CVect *pcVect, int dp)
{
	if(kind == BEZEKIND){
		dp = SetOutlineBezeFigu(pcVect, atr, bz, dp);
	}
	else if(kind == PLGNKIND){
		dp = SetOutlinePlgnFigu(pcVect, tbl, max, dp);
	}
	else{
		dp = SetOutlineLineFigu(pcVect, ln, dp);
	}
	return(dp);
}

int	CCmdOutLine::SetOutlineBezeFigu(CVect *pcVect, int atr1, dbezetype bz1, int dp)
{
	int		atr;
	DBL		xx, yy;

	if(dp != 0){
		pcVect->GetAtrDXY(dp-1, &atr, &xx, &yy);
		if(EQUAL(xx, bz1.x1) && EQUAL(yy, bz1.y1)){
			dp--;
		}
	}
	atr1 &= ~PATRMASK;
	pcVect->SetFAtrDXY(dp++, atr1|CURVATR|BEZEATR, bz1.x1, bz1.y1);
	pcVect->SetAtrDXY(dp++, BEZEATR, bz1.x2, bz1.y2);
	pcVect->SetAtrDXY(dp++, BEZEATR, bz1.x3, bz1.y3);
	pcVect->SetAtrDXY(dp++, 0, bz1.x4, bz1.y4);
	return(dp);
}

int CCmdOutLine::SetOutlinePlgnFigu(CVect *pcVect, DXYType tbl[], int max, int dp)
{
	int		atr;
	DBL		xx, yy;
	int		i;

	if(dp != 0){
		pcVect->GetAtrDXY(dp-1, &atr, &xx, &yy);
		if(EQUAL(xx, tbl[0].x) && EQUAL(yy, tbl[0].y)){
			dp--;
		}
	}
	for(i = 0; i < max; i++){
		pcVect->SetAtrDXY(dp++, 0, tbl[i].x, tbl[i].y);
	}
	return(dp);
}

int	CCmdOutLine::SetOutlineLineFigu(CVect *pcVect, dlinetype ln1, int dp)
{
	int		atr;
	DBL		xx, yy;

	if(dp != 0){
		pcVect->GetAtrDXY(dp-1, &atr, &xx, &yy);
		if(EQUAL(xx, ln1.x1) && EQUAL(yy, ln1.y1)){
			dp--;
		}
	}
	pcVect->SetAtrDXY(dp++, 0, ln1.x1, ln1.y1);
	pcVect->SetAtrDXY(dp++, 0, ln1.x2, ln1.y2);
	return(dp);
}

int	CCmdOutLine::SetOutLineLastPointLegal(CVect *pcVect, int dp)
{
	int		atr;
	DBL		x1, y1, x2, y2;

	pcVect->GetAtrDXY(0, &atr, &x1, &y1);
	pcVect->GetAtrDXY(dp-1, &atr, &x2, &y2);
	if(EQUAL(x1, x2) && EQUAL(y1, y2)){
		dp--;
	}
	return(dp);
}

void CCmdOutLine::SetOutlineCrossDivFigu(int kind1, dbezetype *bz1, dlinetype *ln1, int kind2, dbezetype *bz2, dlinetype *ln2)
{
	int		cnt, idx;
	DBL		xtbl[10], ytbl[10];
	DBL		tt;
	dbezetype	tbz;

	cnt = SetOutlineCrossPointSetTbl(kind1, *bz1, *ln1, kind2, *bz2, *ln2, xtbl, ytbl);
	if(cnt == 0){
		return;
	}
	if(kind1 == BEZEKIND){
		if(kind2 == BEZEKIND){
			idx = SetOutlineNearPoint(bz1->x1, bz1->y1, xtbl, ytbl, cnt);
			tt = crossgetbezett(bz1, xtbl[idx], ytbl[idx]);
			if(NEQUAL(tt, 1.0)){	// if(tt != 0.1)
				crossbezestetdiv(bz1, 0.0, tt, &tbz);
				*bz1 = tbz;
			}

			idx = SetOutlineNearPoint(bz2->x4, bz2->y4, xtbl, ytbl, cnt);
			tt = crossgetbezett(bz2, xtbl[idx], ytbl[idx]);
			if(NEQUAL(tt, 0.0)){	// if(tt != 0.1)
				crossbezestetdiv(bz2, tt, 1.0, &tbz);
				*bz2 = tbz;
			}
		}
		else{
			idx = SetOutlineNearPoint(bz1->x1, bz1->y1, xtbl, ytbl, cnt);
			tt = crossgetbezett(bz1, xtbl[idx], ytbl[idx]);
			if(NEQUAL(tt, 1.0)){	// if(tt != 0.1)
				crossbezestetdiv(bz1, 0.0, tt, &tbz);
				*bz1 = tbz;
			}

			idx = SetOutlineNearPoint(ln2->x2, ln2->y2, xtbl, ytbl, cnt);
			ln2->x1 = xtbl[idx]; ln2->y1 = ytbl[idx];
		}
	}
	else{
		if(kind2 == BEZEKIND){
			idx = SetOutlineNearPoint(ln1->x2, ln1->y2, xtbl, ytbl, cnt);
			ln1->x2 = xtbl[idx]; ln1->y2 = ytbl[idx];

			idx = SetOutlineNearPoint(bz2->x4, bz2->y4, xtbl, ytbl, cnt);
			tt = crossgetbezett(bz2, xtbl[idx], ytbl[idx]);
			if(NEQUAL(tt, 0.0)){	// if(tt != 0.1)
				crossbezestetdiv(bz2, tt, 1.0, &tbz);
				*bz2 = tbz;
			}
		}
		else{
			idx = SetOutlineNearPoint(ln1->x2, ln1->y2, xtbl, ytbl, cnt);
			ln1->x2 = xtbl[idx]; ln1->y2 = ytbl[idx];

			idx = SetOutlineNearPoint(ln2->x2, ln2->y2, xtbl, ytbl, cnt);
			ln2->x1 = xtbl[idx]; ln2->y1 = ytbl[idx];
		}
	}
}

int CCmdOutLine::SetOutlineCrossPointSetTbl(int kind1, dbezetype bz1, dlinetype ln1, int kind2, dbezetype bz2, dlinetype ln2, DBL xtbl[], DBL ytbl[])
{
	int		cnt;

	if(kind1 == BEZEKIND){
		if(kind2 == BEZEKIND){
			cnt = crossbezebezecrs(&bz1, &bz2, xtbl, ytbl);
		}
		else{
			cnt = crossbezelinecrs(&bz1, &ln2, xtbl, ytbl);
		}
	}
	else{
		if(kind2 == BEZEKIND){
			cnt = crosslinebezecrs(&ln1, &bz2, xtbl, ytbl);
		}
		else{
			cnt = crosslinelinecrs(&ln1, &ln2, xtbl, ytbl);
		}
	}
	return(cnt);
}

int CCmdOutLine::SetOutlineNearPoint(DBL dx, DBL dy, DBL xtbl[], DBL ytbl[], int cnt)
{
	DBL		dlen, tlen;
	int		retidx, idx;

	dlen = INTMAX;
	for(idx = 0; idx < cnt; idx++){
		tlen = hypot(xtbl[idx]-dx, ytbl[idx]-dy);
		if(dlen >= tlen){
			retidx = idx;
			dlen = tlen;
		}
	}
	return(retidx);
}

DBL CCmdOutLine::GetLength(DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		dlen;

	if(EQUAL(x1, x2) && EQUAL(y1, y2)){
		dlen = 0.0;
	}
	else{
		dlen = hypot(x1 - x2, y1 - y2);
	}
	return(dlen);
}

DBL CCmdOutLine::GetSubRag(DBL rag1, DBL rag2)
{
	DBL		subrag;

	subrag = rag1 - rag2;
	if(subrag > M_PI){
		subrag -= (M_PI*2);
	}
	if(subrag < -M_PI){
		subrag += (M_PI*2);
	}
	return(subrag);
}

void CCmdOutLine::GetRoundRag(DBL x0, DBL y0, DBL x1, DBL y1, DBL *rag)
{
	if(EQUAL(x0, x1) && EQUAL(y0, y1)){
		*rag = 0.0;
	}
	else{
		*rag = atan2(y1-y0, x1-x0);
	}
}

void CCmdOutLine::SetRoundPoint(DBL x0, DBL y0, DBL rag, DBL x1, DBL y1, DBL *x, DBL *y)
{
	DBL	tcos, tsin;

	tcos = cos(rag);
	tsin = sin(rag);
	*x = x1 * tcos - y1 * tsin + x0;
	*y = y1 * tcos + x1 * tsin + y0;
}

int CCmdOutLine::SetArcToBeze(CVect *pcVect, int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	int	stat;
	DBL	x0, y0, srag, erag, dr;
	DBL	rag;
	DBL	t1, t2;
	dbezetype bz;

	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &srag, &erag, &dr);
	}
	if(stat == FAIL){
		pcVect->SetAtrDXY(dp, 0, x1, y1);
		dp++;
		return(dp);
	}

	bz.x1 = x1; bz.y1 = y1;
	bz.x4 = x3; bz.y4 = y3;
	rag = (erag - srag) / 3.0 + srag;
	bz.x2 = cos(rag)*dr+x0;
	bz.y2 = sin(rag)*dr+y0;
	rag = (erag - srag) / 3.0 * 2.0 + srag;
	bz.x3 = cos(rag)*dr+x0;
	bz.y3 = sin(rag)*dr+y0;

	t1 = 1.0 / 3.0; t2 = 1.0 / 3.0 * 2.0;
	curveonpointtocntrlpoint(&bz, t1, t2); 
	pcVect->SetAtrDXY(dp, BEZEATR | CURVATR, x1, y1);
	dp++;
	pcVect->SetAtrDXY(dp, BEZEATR, bz.x2, bz.y2);
	dp++;
	pcVect->SetAtrDXY(dp, BEZEATR, bz.x3, bz.y3);
	dp++;
	pcVect->SetAtrDXY(dp, 0, bz.x4, bz.y4);
	dp++;
	return(dp);
}

void CCmdOutLine::CrossSetPoint(CDataList *pcDataList)
{
	CVect	*pcVect1;
	CVect	*pcVect2;
	DBL		sum, crt;

	sum = m_dVectCount*m_dVectCount / 2.0 + m_dVectCount * 2;
	crt = 0;
	m_cCrsLst.RemoveAll();
	pcVect1 = (CVect*)(pcDataList->m_pcDataTop);
	while(1){
		if(pcVect1 == NULL){
			break;
		}
		pcVect2 = pcVect1;
		//pcVect2 = (CVect*)(pcVect1->GetNext());
		while(1){
			if(pcVect2 == NULL){
				break;
			}
			CrossSetPointVect(pcVect1, pcVect2);
			pcVect2 = (CVect*)(pcVect2->GetNext());
			crt++;
			sprintf(m_szMsgBuf, "交点付加 %4.1lf％", crt/sum * 100.0);
			HlpMsgDsp(m_szMsgBuf);
			m_pcDialogProgresBar->m_ProgBar.SetPos((int)(crt/sum*800.0+100.0));
		}
		pcVect1 = (CVect*)(pcVect1->GetNext());
	}

	SortCrossPoint();
	pcVect1 = (CVect*)(pcDataList->m_pcDataTop);
	pcVect1->AllOffPointFlag(PATN0ATR);
	while(1){
		if(pcVect1 == NULL){
			break;
		}
		AddCrossPoint(pcVect1);
		pcVect1 = (CVect*)(pcVect1->GetNext());
		crt++;
		sprintf(m_szMsgBuf, "交点付加 %4.1lf％", crt/sum * 100.0);
		HlpMsgDsp(m_szMsgBuf);
	}
}

void CCmdOutLine::CrossSetPointVect(CVect *pcVect1, CVect *pcVect2)
{
	int		pcnt1, pcnt2;
	int		pp1, pp2;
	int		np1, np2;
	dlinetype	ln1, ln2;
	dbezetype	bz1, bz2;
	int		kind1, kind2;

	pcnt1 = pcVect1->GetPointCount();
	pcnt2 = pcVect2->GetPointCount();
	for(pp1 = 0; pp1 < pcnt1;){
		np1 = pcVect1->NextPointGet(pp1);
		kind1 = SetFigu(pcVect1, pp1, &bz1, &ln1);
		if(pcVect1 == pcVect2){
			pp2 = np1;
		}
		else{
			pp2 = 0;
		}
		for(; pp2 < pcnt2;){
			np2 = pcVect2->NextPointGet(pp2);
			kind2 = SetFigu(pcVect2, pp2, &bz2, &ln2);
			/*
			if(pcVect1 == pcVect2
			&& (pp1 == pp2 || np1 == pp2 || pp1 == np2)){
				pp2 = np2;
				continue;
			}
			*/
			m_pcVect1 = pcVect1;
			m_pcVect2 = pcVect2;
			m_nPP1 = pp1;
			m_nPP2 = pp2;
			if(kind1 == BEZEKIND){
				if(kind2 == BEZEKIND){
					//DrawLine(bz1.x1, bz1.y1, bz1.x4, bz1.y4, RGB(255, 255, 0));
					//DrawLine(bz2.x1, bz2.y1, bz2.x4, bz2.y4, RGB(255, 255, 0));
					GetCrossPointFigu(bz1, bz2);
					//DrawLine(bz1.x1, bz1.y1, bz1.x4, bz1.y4, RGB(255, 255, 0));
					//DrawLine(bz2.x1, bz2.y1, bz2.x4, bz2.y4, RGB(255, 255, 0));
				}
				else{
					//DrawLine(bz1.x1, bz1.y1, bz1.x4, bz1.y4, RGB(255, 255, 0));
					//DrawLine(ln2.x1, ln2.y1, ln2.x2, ln2.y2, RGB(255, 255, 0));
					GetCrossPointFigu(bz1, ln2);
					//DrawLine(bz1.x1, bz1.y1, bz1.x4, bz1.y4, RGB(255, 255, 0));
					//DrawLine(ln2.x1, ln2.y1, ln2.x2, ln2.y2, RGB(255, 255, 0));
				}
			}
			else{
				if(kind2 == BEZEKIND){
					//DrawLine(bz2.x1, bz2.y1, bz2.x4, bz2.y4, RGB(255, 255, 0));
					//DrawLine(ln1.x1, ln1.y1, ln1.x2, ln1.y2, RGB(255, 255, 0));
					GetCrossPointFigu(ln1, bz2);
					//DrawLine(bz2.x1, bz2.y1, bz2.x4, bz2.y4, RGB(255, 255, 0));
					//DrawLine(ln1.x1, ln1.y1, ln1.x2, ln1.y2, RGB(255, 255, 0));
				}
				else{
					//DrawLine(ln1.x1, ln1.y1, ln1.x2, ln1.y2, RGB(255, 255, 0));
					//DrawLine(ln2.x1, ln2.y1, ln2.x2, ln2.y2, RGB(255, 255, 0));
					GetCrossPointFigu(ln1, ln2);
					//DrawLine(ln1.x1, ln1.y1, ln1.x2, ln1.y2, RGB(255, 255, 0));
					//DrawLine(ln2.x1, ln2.y1, ln2.x2, ln2.y2, RGB(255, 255, 0));
				}
			}
			pp2 = np2;
		}
		pp1 = np1;
	}
}

void CCmdOutLine::GetCrossPointFigu(dlinetype ln1, dlinetype ln2)
{
	DBL		xtbl[3];
	DBL		ytbl[3];
	int		cnt, i;
	DBL		tt;
	CCrossPoint	*pcCrs;

	cnt = crosslinelinecrs(&ln1, &ln2, xtbl, ytbl);
	for(i = 0; i < cnt; i++){
		if(CheckStartEndPoint(xtbl[i], ytbl[i], ln1.x1, ln1.y1, ln1.x2, ln1.y2, ln2.x1, ln2.y1, ln2.x2, ln2.y2) == TRUE){
			continue;
		}
		tt = crossgetlinett(&ln1, xtbl[i], ytbl[i]);
		pcCrs = new CCrossPoint();
		pcCrs->m_pcVect = m_pcVect1;
		pcCrs->m_npp = m_nPP1;
		pcCrs->m_dtt = tt;
		pcCrs->m_dxx = xtbl[i];
		pcCrs->m_dyy = ytbl[i];
		m_cCrsLst.AddTail(pcCrs);
		tt = crossgetlinett(&ln2, xtbl[i], ytbl[i]);
		pcCrs = new CCrossPoint();
		pcCrs->m_pcVect = m_pcVect2;
		pcCrs->m_npp = m_nPP2;
		pcCrs->m_dtt = tt;
		pcCrs->m_dxx = xtbl[i];
		pcCrs->m_dyy = ytbl[i];
		m_cCrsLst.AddTail(pcCrs);
	}
}

void CCmdOutLine::GetCrossPointFigu(dbezetype bz1, dlinetype ln2)
{
	DBL		xtbl[5];
	DBL		ytbl[5];
	int		cnt, i;
	DBL		tt;
	CCrossPoint	*pcCrs;

	cnt = crossbezelinecrs(&bz1, &ln2, xtbl, ytbl);
	for(i = 0; i < cnt; i++){
		if(CheckStartEndPoint(xtbl[i], ytbl[i], bz1.x1, bz1.y1, bz1.x2, bz1.y2, ln2.x1, ln2.y1, ln2.x2, ln2.y2) == TRUE){
			continue;
		}
		tt = crossgetbezett(&bz1, xtbl[i], ytbl[i]);
		pcCrs = new CCrossPoint();
		pcCrs->m_pcVect = m_pcVect1;
		pcCrs->m_npp = m_nPP1;
		pcCrs->m_dtt = tt;
		pcCrs->m_dxx = xtbl[i];
		pcCrs->m_dyy = ytbl[i];
		m_cCrsLst.AddTail(pcCrs);
		tt = crossgetlinett(&ln2, xtbl[i], ytbl[i]);
		pcCrs = new CCrossPoint();
		pcCrs->m_pcVect = m_pcVect2;
		pcCrs->m_npp = m_nPP2;
		pcCrs->m_dtt = tt;
		pcCrs->m_dxx = xtbl[i];
		pcCrs->m_dyy = ytbl[i];
		m_cCrsLst.AddTail(pcCrs);
	}
}


void CCmdOutLine::GetCrossPointFigu(dlinetype ln1, dbezetype bz2)
{
	DBL		xtbl[5];
	DBL		ytbl[5];
	int		cnt, i;
	DBL		tt;
	CCrossPoint	*pcCrs;

	cnt = crossbezelinecrs(&bz2, &ln1, xtbl, ytbl);
	for(i = 0; i < cnt; i++){
		if(CheckStartEndPoint(xtbl[i], ytbl[i], ln1.x1, ln1.y1, ln1.x2, ln1.y2, bz2.x1, bz2.y1, bz2.x2, bz2.y2) == TRUE){
			continue;
		}
		tt = crossgetlinett(&ln1, xtbl[i], ytbl[i]);
		pcCrs = new CCrossPoint();
		pcCrs->m_pcVect = m_pcVect1;
		pcCrs->m_npp = m_nPP1;
		pcCrs->m_dtt = tt;
		pcCrs->m_dxx = xtbl[i];
		pcCrs->m_dyy = ytbl[i];
		m_cCrsLst.AddTail(pcCrs);
		tt = crossgetbezett(&bz2, xtbl[i], ytbl[i]);
		pcCrs = new CCrossPoint();
		pcCrs->m_pcVect = m_pcVect2;
		pcCrs->m_npp = m_nPP2;
		pcCrs->m_dtt = tt;
		pcCrs->m_dxx = xtbl[i];
		pcCrs->m_dyy = ytbl[i];
		m_cCrsLst.AddTail(pcCrs);
	}
}

void CCmdOutLine::GetCrossPointFigu(dbezetype bz1, dbezetype bz2)
{
	DBL		xtbl[12];
	DBL		ytbl[12];
	int		cnt, i;
	DBL		tt;
	CCrossPoint	*pcCrs;

	cnt = crossbezebezecrs(&bz1, &bz2, xtbl, ytbl);
	//DrawLine(bz1.x1, bz1.y1, bz1.x4, bz1.y4, RGB(255, 255, 0));
	//DrawLine(bz2.x1, bz2.y1, bz2.x4, bz2.y4, RGB(255, 255, 0));
	for(i = 0; i < cnt; i++){
		if(CheckStartEndPoint(xtbl[i], ytbl[i], bz1.x1, bz1.y1, bz1.x2, bz1.y2, bz2.x1, bz2.y1, bz2.x2, bz2.y2) == TRUE){
			continue;
		}
		tt = crossgetbezett(&bz1, xtbl[i], ytbl[i]);
		pcCrs = new CCrossPoint();
		pcCrs->m_pcVect = m_pcVect1;
		pcCrs->m_npp = m_nPP1;
		pcCrs->m_dtt = tt;
		pcCrs->m_dxx = xtbl[i];
		pcCrs->m_dyy = ytbl[i];
		m_cCrsLst.AddTail(pcCrs);
		tt = crossgetbezett(&bz2, xtbl[i], ytbl[i]);
		pcCrs = new CCrossPoint();
		pcCrs->m_pcVect = m_pcVect2;
		pcCrs->m_npp = m_nPP2;
		pcCrs->m_dtt = tt;
		pcCrs->m_dxx = xtbl[i];
		pcCrs->m_dyy = ytbl[i];
		m_cCrsLst.AddTail(pcCrs);
	}
}

void CCmdOutLine::SortCrossPoint()
{
	CCrossPoint	*pcCrs1;
	CCrossPoint	*pcCrs2;
	CCrossPoint	cTCrs;
	POSITION	pos1, tpos1;
	POSITION	pos2, tpos2;

	pos1 = m_cCrsLst.GetHeadPosition();
	while(pos1){
		tpos1 = pos1;
		pcCrs1 = m_cCrsLst.GetNext(tpos1);
		pos2 = tpos1;
		while(pos2){
			tpos2 = pos2;
			pcCrs2 = m_cCrsLst.GetNext(tpos2);
			if(pcCrs1->m_pcVect < pcCrs2->m_pcVect){
				pos2 = tpos2;
				continue;
			}
			if(pcCrs1->m_pcVect == pcCrs2->m_pcVect
			&& pcCrs1->m_npp < pcCrs2->m_npp){
				pos2 = tpos2;
				continue;
			}
			if(pcCrs1->m_pcVect == pcCrs2->m_pcVect
			&& pcCrs1->m_npp == pcCrs2->m_npp
			&& pcCrs1->m_dtt < pcCrs2->m_dtt){
				pos2 = tpos2;
				continue;
			}
			cTCrs = *pcCrs1;
			*pcCrs1 = *pcCrs2;
			*pcCrs2 = cTCrs;
			pos2 = tpos2;
		}
		pos1 = tpos1;
	}
}

void CCmdOutLine::AddCrossPoint(CVect *pcVect)
{
	CVect	*pcTVect;
	int		pcnt, pp, np, dp;
	int		atr1;
	DBL		x1;
	DBL		y1;
	dbezetype	bz;
	dlinetype	ln;
	int			kind;

	m_pcVect1 = pcVect;
	dp = 0;
	pcTVect	= new CVect(BASEVECTID);
	pcnt = pcVect->GetPointCount();
	for(pp = 0; pp < pcnt;){
		m_nPP1 = pp;
		np = pcVect->NextPointGet(pp);
		pcVect->GetAtr(pp, &atr1);
		kind = SetFigu(pcVect, pp, &bz, &ln);
		if(kind == BEZEKIND){
			dp = AddCrossPoint(atr1, bz, pcTVect, dp);
		}
		else{
			pcTVect->SetAtrDXY(dp++, 0, ln.x1, ln.y1);
			dp = AddCrossPoint(atr1, ln, pcTVect, dp);
		}
		pp = np;
	}
	pcnt = dp;
	for(pp = 0; pp < pcnt; pp++){
		pcTVect->GetFAtrDXY(pp, &atr1, &x1, &y1);
		m_pcVect1->SetFAtrDXY(pp, atr1, x1, y1);
	}
	delete(pcTVect);
	m_pcVect1->SetPointCount(pcnt);
	m_pcVect1->RenewMiniMax();
}

int CCmdOutLine::AddCrossPoint(int atr1, dlinetype ln, CVect *pcVect, int dp)
{
	POSITION	pos;
	CCrossPoint	*pcCrs;
	int			atr;
	DBL			xx, yy;

	xx = ln.x1;
	yy = ln.y1;
	DrawLine(ln.x1, ln.y1, ln.x2, ln.y2, RGB(255, 255, 0));
	pos = m_cCrsLst.GetHeadPosition();
	DrawLine(ln.x1, ln.y1, ln.x2, ln.y2, RGB(255, 255, 0));
	while(pos){
		pcCrs = m_cCrsLst.GetNext(pos);
		if(pcCrs->m_pcVect == m_pcVect1
		&& pcCrs->m_npp == m_nPP1){
			if(EQUAL(xx, pcCrs->m_dxx) && EQUAL(yy, pcCrs->m_dyy)){
				dp--;
				pcVect->GetAtr(dp, &atr);
			}
			else{
				atr = 0;
			}
			atr |= PATN0ATR;
			pcVect->SetAtrDXY(dp, 0, pcCrs->m_dxx, pcCrs->m_dyy);
			pcVect->SetAtr(dp++, atr);
		}
	}
	return(dp);
}

int CCmdOutLine::AddCrossPoint(int atr1, dbezetype bz, CVect *pcVect, int dp)
{
	POSITION	pos;
	CCrossPoint	*pcCrs;
	DBL			st, et;
	DBL			xx, yy;
	int			firstflag;
	dbezetype	divbz;
	
	firstflag = ON;
	st = 0;
	pos = m_cCrsLst.GetHeadPosition();
	xx = bz.x1;
	yy = bz.y1;
	while(pos){
		pcCrs = m_cCrsLst.GetNext(pos);
		if(pcCrs->m_pcVect == m_pcVect1
		&& pcCrs->m_npp == m_nPP1){
			et = pcCrs->m_dtt;
			if(0.1 < (et-st)){
				crossbezestetdiv(&bz, st, et, &divbz);
				pcVect->SetAtrDXY(dp++, CURVATR|BEZEATR, xx, yy);
				if(firstflag == OFF){
					pcVect->SetAtr(dp-1, CURVATR|BEZEATR|PATN0ATR);
				}
				pcVect->SetAtrDXY(dp++, BEZEATR, divbz.x2, divbz.y2);
				pcVect->SetAtrDXY(dp++, BEZEATR, divbz.x3, divbz.y3);
			}
			else{
			//else if(NEQUAL(et, st)){
				pcVect->SetAtrDXY(dp++, 0, xx, yy);
				if(firstflag == OFF){
					pcVect->SetAtr(dp-1, PATN0ATR);
				}
			}
		
			xx = pcCrs->m_dxx;
			yy = pcCrs->m_dyy;
			st = et;
			firstflag = OFF;
		}
	}
	et = 1.0;
	if(0.1 < (et-st)){
		crossbezestetdiv(&bz, st, et, &divbz);
		pcVect->SetAtrDXY(dp++, CURVATR|BEZEATR, xx, yy);
		if(firstflag == OFF){
			pcVect->SetAtr(dp-1, CURVATR|BEZEATR|PATN0ATR);
		}
		else{
			atr1 &= ~PATRMASK;
			pcVect->SetAtr(dp-1, atr1|CURVATR|BEZEATR);
		}
		pcVect->SetAtrDXY(dp++, BEZEATR, divbz.x2, divbz.y2);
		pcVect->SetAtrDXY(dp++, BEZEATR, divbz.x3, divbz.y3);
	}
	else {
	//else if(NEQUAL(et, st)){
		pcVect->SetAtrDXY(dp++, 0, xx, yy);
		if(firstflag == OFF){
			pcVect->SetAtr(dp-1, PATN0ATR);
		}
	}
	return(dp);
}

int CCmdOutLine::SetFigu(CVect *pcVect, int pp, dbezetype *bz, dlinetype *ln)
{
	int		pcnt, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, x1, x2, x3, x4;
	DBL		y0, y1, y2, y3, y4;
	DBL		rag1, rag2;
	int	stat;
	DBL	srag, erag, dr;
	DBL	rag;
	DBL	t1, t2;

	pcnt = pcVect->GetPointCount();
	p0 = pcVect->PointLegal(pcnt, pp-1);
	p1 = pcVect->PointLegal(pcnt, pp+0);
	p2 = pcVect->PointLegal(pcnt, pp+1);
	p3 = pcVect->PointLegal(pcnt, pp+2);
	p4 = pcVect->PointLegal(pcnt, pp+3);
	pcVect->GetAtrDXY(p0, &atr0, &x0, &y0);
	pcVect->GetAtrDXY(p1, &atr1, &x1, &y1);
	pcVect->GetAtrDXY(p2, &atr2, &x2, &y2);
	pcVect->GetAtrDXY(p3, &atr3, &x3, &y3);
	pcVect->GetAtrDXY(p4, &atr4, &x4, &y4);
	if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
		bz->x1 = x1; bz->y1 = y1;
		bz->x2 = x2; bz->y2 = y2;
		bz->x3 = x3; bz->y3 = y3;
		bz->x4 = x4; bz->y4 = y4;
		return(BEZEKIND);
	}
	else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
		stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
		if(stat != FAIL){
			stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &srag, &erag, &dr);
		}
		if(stat == FAIL){
			ln->x1 = x1; ln->y1 = y1;
			ln->x2 = x3; ln->y2 = y3;
			return(LINEKIND);
		}
		bz->x1 = x1; bz->y1 = y1;
		bz->x4 = x3; bz->y4 = y3;
		rag = (erag - srag) / 3.0 + srag;
		bz->x2 = cos(rag)*dr+x0;
		bz->y2 = sin(rag)*dr+y0;
		rag = (erag - srag) / 3.0 * 2.0 + srag;
		bz->x3 = cos(rag)*dr+x0;
		bz->y3 = sin(rag)*dr+y0;

		t1 = 1.0 / 3.0; t2 = 1.0 / 3.0 * 2.0;
		curveonpointtocntrlpoint(bz, t1, t2); 
		return(BEZEKIND);
	}
	else if(atr1 & CURVATR){
		rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
		rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);
		if(rag1 == NORAG && rag2 == NORAG){
			ln->x1 = x1; ln->y1 = y1;
			ln->x2 = x2; ln->y2 = y2;
			return(LINEKIND);
		}
		curvenoragtocurverag(&rag1, &rag2);
		bz->x1 = x1; bz->y1 = y1;
		bz->x4 = x2; bz->y4 = y2;
		curvebezecontrolpointget(x1, y1, x2, y2, rag1, &(bz->x2), &(bz->y2));
		curvebezecontrolpointget(x2, y2, x1, y1, rag2, &(bz->x3), &(bz->y3));
		return(BEZEKIND);
	}
	else{
		ln->x1 = x1; ln->y1 = y1;
		ln->x2 = x2; ln->y2 = y2;
		return(LINEKIND);
	}
}

int CCmdOutLine::SetFiguOutline(DBL mm, CVect *pcVect, int pp, dbezetype *bz, dlinetype *ln)
{
	int		pcnt, p1, p2, p3, p4;
	int		atr1, atr2, atr3, atr4;
	DBL		x1, x2, x3, x4;
	DBL		y1, y2, y3, y4;
	DBL		rag1, rag2;
	int		kind;

	pcnt = pcVect->GetPointCount();
	p1 = pcVect->PointLegal(pcnt, pp+0);
	p2 = pcVect->PointLegal(pcnt, pp+1);
	p3 = pcVect->PointLegal(pcnt, pp+2);
	p4 = pcVect->PointLegal(pcnt, pp+3);
	pcVect->GetAtrDXY(p1, &atr1, &x1, &y1);
	pcVect->GetAtrDXY(p2, &atr2, &x2, &y2);
	pcVect->GetAtrDXY(p3, &atr3, &x3, &y3);
	pcVect->GetAtrDXY(p4, &atr4, &x4, &y4);
	if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
		GetRoundRag(x1, y1, x2, y2, &rag1);
		SetRoundPoint(x1, y1, rag1, 0.0, mm, &(bz->x1), &(bz->y1));
		SetRoundPoint(x2, y2, rag1, 0.0, mm, &(bz->x2), &(bz->y2));
		GetRoundRag(x3, y3, x4, y4, &rag2);
		SetRoundPoint(x3, y3, rag2, 0.0, mm, &(bz->x3), &(bz->y3));
		SetRoundPoint(x4, y4, rag2, 0.0, mm, &(bz->x4), &(bz->y4));
		kind = BEZEKIND;
	}
	else{
		GetRoundRag(x1, y1, x2, y2, &rag1);
		SetRoundPoint(x1, y1, rag1, 0.0, mm, &(ln->x1), &(ln->y1));
		SetRoundPoint(x2, y2, rag1, 0.0, mm, &(ln->x2), &(ln->y2));
		kind = LINEKIND;
	}
	return(kind);
}

void CCmdOutLine::OutLineTrace(CDataList *pcDataList)
{
	POSITION	pos, tpos;
	POSITION	postbl[RECMAX];
	int		i;
	CVect	*pcVect;
	int		pcnt, pp, atr;
	DBL		x1, y1, x2, y2;

	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcVect = (CVect*)m_cPtrList.GetNext(pos);
		pcnt = pcVect->GetPointCount();
		pcVect->GetAtrDXY(0, &atr, &x1, &y1);
		for(pp = 1; pp < pcnt; pp++){
			pcVect->GetAtrDXY(pp, &atr, &x2, &y2);
			DrawLine(x1, y1, x2, y2, RGB(32, 32, 32));
			x1 = x2; y1 = y2;
		}
	}

	CTonDoc *pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		while(1){
			pcVect = (CVect*)(pcDoc->NewData(BASEVECTID));
			for(i = 0; i < RECMAX; i++){
				postbl[i] = 0;
			}
			postbl[0] = pos;
			if(OutLineTraceVect(pcVect, postbl) == TRUE){
				pcDataList->SetData(pcVect);
			}
			else{
				m_cPtrList.GetNext(pos);
				delete(pcVect);
				break;
			}
			if(postbl[0] != pos){
				i = 0;
			}
			else{
				i = 1;
			}
			for(; i < RECMAX; i++){
				if(postbl[i] == 0){
					break;
				}
				tpos = postbl[i];
				pcVect = (CVect*)m_cPtrList.GetNext(tpos);
				m_cPtrList.RemoveAt(postbl[i]);
				delete(pcVect);
			}
			if(postbl[0] == pos){
				m_cPtrList.GetNext(pos);
				tpos = postbl[0];
				pcVect = (CVect*)m_cPtrList.GetNext(tpos);
				m_cPtrList.RemoveAt(postbl[0]);
				break;
			}
		}
	}
}

void CCmdOutLine::CopyDivDataList(CDataList *pcDataList)
{
	CVect	*pcVect;
	CVect	*pcTVect0, *pcTVect1, *pcTVect;
	int		pcnt, tpcnt, pp, dp, atr;
	DBL		dx, dy;
	int		divflag;

	m_cPtrList.RemoveAll( );
	pcVect = (CVect*)pcDataList->m_pcDataTop;
	while(1){
		if(pcVect == NULL){
			break;
		}
		divflag = OFF;
		pcTVect0 = new CVect(BASEVECTID);
		tpcnt = 0;
		pcTVect = pcTVect0;
		pcnt = pcVect->GetPointCount();
		for(pp = 0, dp = 0; pp < pcnt; pp++){
			pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
			pcTVect->SetAtrDXY(dp++, atr, dx, dy);
			if(pcVect->CheckPointFlag(pp, PATN0ATR) == ON){
				if(pcTVect == pcTVect0){
					tpcnt = pp;
				}
				else{
					divflag = ON;
					pcTVect->SetPointCount(dp);
					pcTVect->OffFlag(REJIONFLAG);
					m_cPtrList.AddTail(pcTVect);
				}
				pcTVect1 = new CVect(BASEVECTID);
				pcTVect = pcTVect1;
				dp = 0;
				pcTVect->SetAtrDXY(dp++, atr, dx, dy);
			}
		}
		if(pcTVect != pcTVect0){
			for(pp = 0; pp <= tpcnt; pp++){
				pcTVect0->GetAtrDXY(pp, &atr, &dx, &dy);
				pcTVect->SetAtrDXY(dp++, atr, dx, dy);
			}
			delete(pcTVect0);
		}
		pcTVect->SetPointCount(dp);
		if(divflag == ON){
			pcTVect->OffFlag(REJIONFLAG);
		}
		else{
			pcTVect->OnFlag(REJIONFLAG);
		}
		m_cPtrList.AddTail(pcTVect);
		pcTVect = (CVect*)(pcVect->GetNext());
		delete(pcVect);
		pcVect = pcTVect;
	}
	pcDataList->m_pcDataTop = NULL;
}

int CCmdOutLine::OutLineTraceVect(CVect *pcVect, POSITION postbl[])
{
	POSITION	pos, tpos;
	CVect	*pcTVect;
	int		posidx;
	CVect	*pcVectTbl[8];
	int		idx, i, j;
	int		dp, atr, atr1, atr2;
	DBL		rsx, rsy, sx, sy, tsx, tsy;
	DBL		x1, y1, x2, y2;
	DBL		tx1, ty1, tx2, ty2;
	DBL		rag0, rag1, subrag, tsubrag;
	
	dp = 0;
	pos = postbl[0];
	pcTVect = (CVect*)m_cPtrList.GetNext(pos);
	dp = CopyVect(pcTVect, pcVect, dp);
	pcVect->GetAtrDXY(dp-1, &atr, &sx, &sy);
	posidx = 1;
	while(1){
		pos = postbl[0];
		idx = 0;
		while(pos != NULL){
			pcTVect = (CVect*)m_cPtrList.GetNext(pos);
			pcTVect->GetAtrDXY(0, &atr, &tsx, &tsy);
			if(EQUAL(tsx, sx) && EQUAL(tsy, sy)){
				pcVectTbl[idx] = pcTVect;
				idx++;
			}
		}
		if(idx == 0){
			return(FALSE);
		}
		pcVect->GetAtrDXY(dp-1, &atr, &x1, &y1);
		pcVect->GetAtrDXY(dp-2, &atr, &x2, &y2);
		DrawLine(x1, y1, x2, y2, RGB(255, 255, 0));
		GetRoundRag(x1, y1, x2, y2, &rag0);
		tsubrag = M_PI*0;
		tx1 = x2; ty1 = y2; tx2 = x1; ty2 = y1;
		for(i = 0; i < idx; i++){
			pcVectTbl[i]->GetAtrDXY(0, &atr1, &x1, &y1);
			pcVectTbl[i]->GetAtrDXY(1, &atr2, &x2, &y2);
			if(x1 == tx1 && y1 == ty1 && x2 == ty2 && y2 == ty2){
				continue;
			}
			DrawLine(x1, y1, x2, y2, RGB(0, 255, 255));
			GetRoundRag(x1, y1, x2, y2, &rag1);
			subrag = rag1 - rag0;
			if(subrag < 0){
				subrag = subrag + M_PI*2; 
			}
			if(subrag > tsubrag){
				pcTVect = pcVectTbl[i];
				tsubrag = subrag;
			}
			DrawLine(x1, y1, x2, y2, RGB(0, 255, 255));
		}
		DrawLine(tx1, ty1, tx2, ty2, RGB(255, 255, 0));
		tpos = m_cPtrList.Find(pcTVect);
		for(i = 0; i < posidx; i++){
			if(postbl[i] == tpos){
				break;
			}
		}
		if(i == posidx){
			postbl[posidx] = tpos;
			posidx++;
		}
		dp = CopyVect(pcTVect, pcVect, dp-1);
		pcVect->GetAtrDXY(dp-1, &atr, &sx, &sy);
		if(EQUAL(sx, 0.0) && EQUAL(sy, 0.0)){
			pcVect->GetAtrDXY(dp-2, &atr, &sx, &sy);
			if(EQUAL(sx, 0.0) && EQUAL(sy, 0.0)){
				postbl[1] = 0;
				postbl[2] = 0;
				return(FALSE);
			}
		}
		// 途中で閉じている図形のための処理
		for(i = 0; i < posidx; i++){
			pos = postbl[i];
			pcTVect = (CVect*)m_cPtrList.GetNext(pos);
			pcTVect->GetAtrDXY(0, &atr, &rsx, &rsy);
			if(EQUAL(sx, rsx) && EQUAL(sy, rsy)){
				break;
			}
		}
		if(i != posidx){
			break;
		}
	}

	if(i != 0){
		dp = 1;
		idx = 0;
		for(j = i; j < posidx; j++){
			pos = postbl[j];
			pcTVect = (CVect*)m_cPtrList.GetNext(pos);
			dp = CopyVect(pcTVect, pcVect, dp-1);
			postbl[idx] = postbl[j];
			idx++;
		}
		postbl[idx] = 0;
	}
	pcVect->SetPointCount(dp);
	pcVect->RenewMiniMax();
	return(TRUE);
}

int CCmdOutLine::CopyVect(CVect *pcTVect, CVect *pcVect, int dp)
{
	int		pcnt, pp, atr;
	DBL		dx, dy;
	DBL		tx, ty;
	int		flag;

	flag = OFF;
	pcnt = pcTVect->GetPointCount();

	pcTVect->GetAtrDXY(0, &atr, &tx, &ty);
	for(pp = 0; pp < dp; pp++){
		pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
		if(EQUAL(tx, dx) && EQUAL(ty, dy)){
			flag = ON;
		}
	}

	for(pp = 0; pp < pcnt; pp++){
		pcTVect->GetAtrDXY(pp, &atr, &dx, &dy);
		pcVect->SetAtrDXY(dp++, atr, dx, dy);
	}

	if(flag == ON){
		pcVect->SetAtrDXY(dp++, 0, 0.0, 0.0);
		pcVect->SetAtrDXY(dp++, 0, 0.0, 0.0);
	}

	return(dp);
}

void CCmdOutLine::DrawLine(DBL x1, DBL y1, DBL x2, DBL y2, long clr)
{
	scrndxymmtorltv(&x1, &y1);
	scrndxymmtorltv(&x2, &y2);
	BeginDraw();
	CPen *pcPen = new CPen(PS_SOLID, 1, clr);
	CPen *pOldPen = m_pcDC->SelectObject(pcPen);
	m_pcDC->MoveTo((int)x1, (int)y1);	
	m_pcDC->LineTo((int)x2, (int)y2);	
	m_pcDC->SelectObject(pOldPen);
	delete(pcPen);
	EndDraw();
}

void CCmdOutLine::DelSamePoint()
{
	POSITION	pos, tpos;
	CVect	*pcVect;
	int		pcnt, pp, dp, satr, eatr;
	DBL		sx, sy, ex, ey;

	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		tpos = pos;
		pcVect = (CVect*)m_cPtrList.GetNext(pos);
		pcnt = pcVect->GetPointCount();
		pcVect->GetAtrDXY(0, &satr, &sx, &sy);
		for(pp = 1, dp = 0; pp < pcnt; pp++){
			pcVect->GetAtrDXY(pp, &eatr, &ex, &ey);
			if(!(EQUAL(sx, ex) && EQUAL(sy, ey))){
				pcVect->SetAtrDXY(dp++, satr, sx, sy);
			}
			satr = eatr;
			sx = ex;
			sy = ey;
		}
		pcVect->SetAtrDXY(dp++, satr, sx, sy);
		pcVect->SetPointCount(dp);

		if(dp <= 1){
			m_cPtrList.RemoveAt(tpos);
			delete(pcVect);
		}
	}
}

void CCmdOutLine::BitMapDrawInit(CDataList *pcDataList)
{
	DBL		dminix, dminiy, dmaxx, dmaxy;
	DBL		xytime;

	pcDataList->GetDMiniMax(&dminix, &dminiy, &dmaxx, &dmaxy);
	xytime = (dmaxy - dminiy) / (dmaxx - dminix);
	m_pcCmdSpcl->MemoryInit(xytime);
	m_pcCmdSpcl->DrawAreaInit(dminix, dminiy, dmaxx, dmaxy);
	m_pcCmdSpcl->BitMapInit();
}

void CCmdOutLine::BitMapDraw(CDataList *pcDataList, DBL thick)
{
	COLORREF	clr1, clr2;
	long	lBitsSize;

	m_pcCmdSpcl->BitMapInit();
	clr1 = RGB(255, 255, 255);
	if(thick < 0){
		thick *= -1;
		clr2 = RGB(0, 0, 0);
	}
	else{
		clr2 = RGB(255, 255, 255);
	}
	m_pcView->CheckFlagDrawSetClr(ACTIVEFLAG, m_pcCmdSpcl->m_pMemDC, clr1, clr2, thick);
	m_pcCmdSpcl->m_nXByte = (((long)m_pcCmdSpcl->m_nXSize + 7L) / 8L + 3L) / 4L * 4L;
	lBitsSize = (long)m_pcCmdSpcl->m_nYSize * m_pcCmdSpcl->m_nXByte;
	m_pcCmdSpcl->m_pBitmap->GetBitmapBits(lBitsSize, m_pcCmdSpcl->m_lpBits);
}

void CCmdOutLine::DelInpointVect()
{
	POSITION	pos, tpos;
	CVect	*pcVect;
	int		pcnt, pp, np, kind;
	dlinetype	ln1;
	dbezetype	bz1;
	DBL		dx, dy;
	int		flag;

	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		tpos = pos;
		pcVect = (CVect*)m_cPtrList.GetNext(pos);
		pcnt = pcVect->GetPointCount();
		flag = OFF;
		for(pp = 0; pp < pcnt;){
			np = pcVect->NextPointGet(pp);
			kind = SetFigu(pcVect, pp, &bz1, &ln1);
			if(kind == BEZEKIND){
				curvebezetpointget(bz1, 0.0, &dx, &dy);
				if(BitOnOffCheck(dx, dy) == ON){
					flag = ON;
#ifdef	NOCHKPROG
					break;
#endif
				}
				curvebezetpointget(bz1, 0.333, &dx, &dy);
				if(BitOnOffCheck(dx, dy) == ON){
					flag = ON;
#ifdef	NOCHKPROG
					break;
#endif
				}
				curvebezetpointget(bz1, 0.666, &dx, &dy);
				if(BitOnOffCheck(dx, dy) == ON){
					flag = ON;
#ifdef	NOCHKPROG
					break;
#endif
				}
			}
			else{
				if(BitOnOffCheck(ln1.x1, ln1.y1) == ON){
					flag = ON;
#ifdef	NOCHKPROG
					break;
#endif
				}
			}
			pp = np;
		}
		if(flag == ON){
			m_cPtrList.RemoveAt(tpos);
			delete(pcVect);
		}
		
	}
#ifdef _DEBUG
	m_pcCmdSpcl->BmpFileOut("c:\\test0001.bmp", m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, m_pcCmdSpcl->m_nXSize, m_pcCmdSpcl->m_nYSize);
#endif
}

int CCmdOutLine::BitOnOffCheck(DBL dx, DBL dy)
{
	int		nx, ny;

	nx = (int)((dx - m_pcCmdSpcl->m_dCntrX) / m_pcCmdSpcl->m_dTrnsTime); 
	ny = (int)(m_pcCmdSpcl->m_nYSize - (dy - m_pcCmdSpcl->m_dCntrY) / m_pcCmdSpcl->m_dTrnsTime);
	if(m_pcCmdSpcl->BitOnOffCheck(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx, ny) == ON){
#ifdef	TCHKPROG
		m_pcCmdSpcl->BitOffSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx+1, ny);
		m_pcCmdSpcl->BitOffSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx-1, ny);
		m_pcCmdSpcl->BitOffSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx, ny+1);
		m_pcCmdSpcl->BitOffSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx, ny-1);
		m_pcCmdSpcl->BitOffSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx+1, ny+1);
		m_pcCmdSpcl->BitOffSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx-1, ny+1);
		m_pcCmdSpcl->BitOffSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx+1, ny-1);
		m_pcCmdSpcl->BitOffSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx-1, ny-1);
#endif
		if(0 < m_dOutLineMM){
			return(ON);
		}
		else{
			return(OFF);
		}
	}
	else{
#ifdef	TCHKPROG
		m_pcCmdSpcl->BitOnSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx+1, ny);
		m_pcCmdSpcl->BitOnSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx-1, ny);
		m_pcCmdSpcl->BitOnSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx, ny+1);
		m_pcCmdSpcl->BitOnSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx, ny-1);
		m_pcCmdSpcl->BitOnSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx+1, ny+1);
		m_pcCmdSpcl->BitOnSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx-1, ny+1);
		m_pcCmdSpcl->BitOnSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx+1, ny-1);
		m_pcCmdSpcl->BitOnSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx-1, ny-1);
#endif
		if(0 < m_dOutLineMM){
			return(OFF);
		}
		else{
			return(ON);
		}
	}
}

void CCmdOutLine::BitMapDrawEnd()
{
	m_pcCmdSpcl->DrawAreaEnd();
	m_pcCmdSpcl->MemoryEnd();
}

void CCmdOutLine::SetOnevectLoop(CDataList *pcDataList)
{
	POSITION	pos, tpos;
	CVect	*pcVect;
	int		pcnt, atr;
	DBL		sx, sy, ex, ey;

	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		tpos = pos;
		pcVect = (CVect*)m_cPtrList.GetNext(pos);
		if(pcVect->CheckFlag(REJIONFLAG) == ON){
			pcDataList->SetData(pcVect);
			m_cPtrList.RemoveAt(tpos);
		}
	}

	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		tpos = pos;
		pcVect = (CVect*)m_cPtrList.GetNext(pos);
		pcnt = pcVect->GetPointCount();
		pcVect->GetAtrDXY(0, &atr, &sx, &sy);
		pcVect->GetAtrDXY(pcnt-1, &atr, &ex, &ey);
		if(EQUAL(sx, ex) && EQUAL(sy, ey)){
			pcDataList->SetData(pcVect);
			m_cPtrList.RemoveAt(tpos);
			//delete(pcVect);
		}
	}
}

void CCmdOutLine::GetDivRestVect(CDataList *pcDataList)
{
	POSITION	pos, tpos;
	CVect	*pcVect;

	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		tpos = pos;
		pcVect = (CVect*)m_cPtrList.GetNext(pos);
		pcDataList->SetData(pcVect);
		m_cPtrList.RemoveAt(tpos);
	}
}

// 図形のビットマップイメージからX方向に検査し回転方向を正規化する
int CCmdOutLine::RejionLegalVectXAdd(CVect *pcVect1)
{
	int		pcnt, pp, np;
	dlinetype	ln1;
	dbezetype	bz1;
	DBL		xx, yy, tt;
	int		nx, ny;
	DBL		time, sx, sy;
	int		nkind;
	int		kind;
	int		flag;

	time = m_pcCmdSpcl->m_dTrnsTime;
	sx = m_pcCmdSpcl->m_dCntrX;
	sy = m_pcCmdSpcl->m_dCntrY;

	m_pcVect1 = NULL;
	pcnt = pcVect1->GetPointCount();
	for(pp = 0; pp < pcnt;){
		m_nPP1 = pp;
		np = pcVect1->NextPointGet(pp);
		kind = SetFigu(pcVect1, pp, &bz1, &ln1);
		if(kind == BEZEKIND){
			curvebezetpointget(bz1, 0.0, &ln1.x1, &ln1.y1);
			for(tt = 0.1; tt < 1.0; tt++){
				curvebezetpointget(bz1, tt, &ln1.x2, &ln1.y2);
				xx = (ln1.x1 + ln1.x2) / 2;
				yy = (ln1.y1 + ln1.y2) / 2;
				nx = (int)((xx - sx) / time);
				ny = (int)(m_pcCmdSpcl->m_nYSize - (yy - sy) / time);
				flag = CheckBitPointXAdd(ln1, nx, ny);
				if(flag != FAIL){
					break;
				}
				ln1.x1 = ln1.x2;
				ln1.y1 = ln1.y2;
			}
			if(flag != FAIL){
				break;
			}
		}
		else{
			xx = (ln1.x1 + ln1.x2) / 2;
			yy = (ln1.y1 + ln1.y2) / 2;
			nx = (int)((xx - sx) / time);
			ny = (int)(m_pcCmdSpcl->m_nYSize - (yy - sy) / time);
			flag = CheckBitPointXAdd(ln1, nx, ny);
			if(flag != FAIL){
				break;
			}
		}
		pp = np;
	}
	if(flag == FAIL){
		return(FALSE);
	}
	xx = 0.0;
	yy = (ln1.y1 + ln1.y2) / 2;
	nkind = curvenkindget(ln1.x1, ln1.y1, ln1.x2, ln1.y2, xx, yy);
	if(nkind == 0){
		return(FALSE);
	}
	if(flag == ON && nkind > 0){
		pcVect1->ReverceRejion();
	}
	else if(flag == OFF && nkind < 0){
		pcVect1->ReverceRejion();
	}
	return(TRUE);
}

// 図形のビットマップイメージからY方向に検査し回転方向を正規化する
int CCmdOutLine::RejionLegalVectYAdd(CVect *pcVect1)
{
	int		pcnt, pp, np;
	dlinetype	ln1;
	dbezetype	bz1;
	DBL		xx, yy, tt;
	int		nx, ny;
	DBL		time, sx, sy;
	int		nkind;
	int		kind;
	int		flag;

	time = m_pcCmdSpcl->m_dTrnsTime;
	sx = m_pcCmdSpcl->m_dCntrX;
	sy = m_pcCmdSpcl->m_dCntrY;

	m_pcVect1 = NULL;
	pcnt = pcVect1->GetPointCount();
	for(pp = 0; pp < pcnt;){
		m_nPP1 = pp;
		np = pcVect1->NextPointGet(pp);
		kind = SetFigu(pcVect1, pp, &bz1, &ln1);
		if(kind == BEZEKIND){
			curvebezetpointget(bz1, 0.0, &ln1.x1, &ln1.y1);
			for(tt = 0.1; tt < 1.0; tt++){
				curvebezetpointget(bz1, tt, &ln1.x2, &ln1.y2);
				xx = (ln1.x1 + ln1.x2) / 2;
				yy = (ln1.y1 + ln1.y2) / 2;
				nx = (int)((xx - sx) / time);
				ny = (int)(m_pcCmdSpcl->m_nYSize - (yy - sy) / time);
				flag = CheckBitPointYAdd(ln1, nx, ny);
				if(flag != FAIL){
					break;
				}
				ln1.x1 = ln1.x2;
				ln1.y1 = ln1.y2;
			}
		}
		else{
			xx = (ln1.x1 + ln1.x2) / 2;
			yy = (ln1.y1 + ln1.y2) / 2;
			nx = (int)((xx - sx) / time);
			ny = (int)(m_pcCmdSpcl->m_nYSize - (yy - sy) / time);
			flag = CheckBitPointYAdd(ln1, nx, ny);
			if(flag != FAIL){
				break;
			}
		}
		pp = np;
	}
	if(flag == FAIL){
		return(FALSE);
	}
	yy = 0.0;
	xx = (ln1.x1 + ln1.x2) / 2;
	nkind = curvenkindget(ln1.x1, ln1.y1, ln1.x2, ln1.y2, xx, yy);
	if(nkind == 0){
		return(FALSE);
	}
	if(flag == ON && nkind < 0){
		pcVect1->ReverceRejion();
	}
	else if(flag == OFF && nkind > 0){
		pcVect1->ReverceRejion();
	}
	return(TRUE);
}

int CCmdOutLine::CheckBitPointXAdd(dlinetype ln, int nx, int ny)
{
	int		flag1, flag2;
	int		i;

	if(EQUAL(ln.x1, ln.x2) && EQUAL(ln.y1, ln.y2)){
		return(FAIL);
	}
	if(fabs(ln.x1 - ln.x2) < fabs(ln.y1 - ln.y2)){
		for(i = 1; i < 8; i++){
			flag1 = m_pcCmdSpcl->BitOnOffCheck(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx-i, ny);
			flag2 = m_pcCmdSpcl->BitOnOffCheck(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx+i, ny);
			if(flag1 != flag2){
/*
#ifdef	CHKPROG
				for(i = 1; i < 8; i++){
					flag1 = m_pcCmdSpcl->BitOnOffCheck(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx-i, ny);
					if(flag1 == ON){
						m_pcCmdSpcl->BitOffSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx-i, ny);
					}
					else{
						m_pcCmdSpcl->BitOnSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx-i, ny);
					}
					flag2 = m_pcCmdSpcl->BitOnOffCheck(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx+i, ny);
					if(flag2 == ON){
						m_pcCmdSpcl->BitOffSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx+i, ny);
					}
					else{
						m_pcCmdSpcl->BitOnSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx+i, ny);
					}
				}
#endif
*/
				return(flag1);
			}
		}
	}
	return(FAIL);
}

int CCmdOutLine::CheckBitPointYAdd(dlinetype ln, int nx, int ny)
{
	int		flag1, flag2;
	int		i;

	if(EQUAL(ln.x1, ln.x2) && EQUAL(ln.y1, ln.y2)){
		return(FAIL);
	}
	if(fabs(ln.x1 - ln.x2) > fabs(ln.y1 - ln.y2)){
		for(i = 1; i < 8; i++){
			flag1 = m_pcCmdSpcl->BitOnOffCheck(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx, ny-i);
			flag2 = m_pcCmdSpcl->BitOnOffCheck(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx, ny+i);
			if(flag1 != flag2){
/*
#ifdef	CHKPROG
				for(i = 1; i < 8; i++){
					flag1 = m_pcCmdSpcl->BitOnOffCheck(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx, ny-i);
					if(flag1 == ON){
						m_pcCmdSpcl->BitOffSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx, ny-i);
					}
					else{
						m_pcCmdSpcl->BitOnSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx, ny-i);
					}
					flag2 = m_pcCmdSpcl->BitOnOffCheck(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx, ny+i);
					if(flag2 == ON){
						m_pcCmdSpcl->BitOffSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx, ny+i);
					}
					else{
						m_pcCmdSpcl->BitOnSet(m_pcCmdSpcl->m_lpBits, m_pcCmdSpcl->m_nXByte, nx, ny+i);
					}
				}
#endif
*/
				return(flag1);
			}
		}
	}
	return(FAIL);
}

int CCmdOutLine::GetSinge(DBL singe)
{
	if(singe == 0.0){
		return(0);
	}
	else if(singe < 0.0){
		return(-1);
	}
	else{
		return(1);
	}
}

void CCmdOutLine::SortPtrList()
{
	CVect		*pcVect1, *pcVect2;
	POSITION	pos1, tpos1;
	POSITION	pos2, tpos2;
	DBL			minx1, miny1, maxx1, maxy1;
	DBL			minx2, miny2, maxx2, maxy2;

	pos1 = m_cPtrList.GetHeadPosition();
	while(pos1){
		pcVect1 = (CVect*)m_cPtrList.GetNext(pos1);
		pcVect1->RenewMiniMax();
	}
	pos1 = m_cPtrList.GetHeadPosition();
	while(pos1){
		tpos1 = pos1;
		pcVect1 = (CVect*)m_cPtrList.GetNext(tpos1);
		pos2 = tpos1;
		while(pos2){
			tpos2 = pos2;
			pcVect2 = (CVect*)m_cPtrList.GetNext(tpos2);
			pcVect1->GetDMiniMax(&minx1, &miny1, &maxx1, &maxy1);
			pcVect2->GetDMiniMax(&minx2, &miny2, &maxx2, &maxy2);
			if(minx1 > minx2){
				m_cPtrList.SetAt(pos1, pcVect2);
				m_cPtrList.SetAt(pos2, pcVect1);
				pcVect1 = pcVect2;
			}
			pos2 = tpos2;
		}
		pos1 = tpos1;
	}
}

void CCmdOutLine::DelSamePoint(CDataList *pcDataList)
{
	CVect	*pcVect;
	int		pcnt, pp, dp, satr, eatr;
	DBL		sx, sy, ex, ey;

	pcVect = (CVect*)(pcDataList->m_pcDataTop);
	while(1){
		if(pcVect == NULL){
			break;
		}
		pcnt = pcVect->GetPointCount();
		pcVect->GetFAtrDXY(0, &satr, &sx, &sy);
		for(pp = 1, dp = 0; pp < pcnt; pp++){
			pcVect->GetFAtrDXY(pp, &eatr, &ex, &ey);
			if(!(EQUAL(sx, ex) && EQUAL(sy, ey))){
				pcVect->SetFAtrDXY(dp++, satr, sx, sy);
			}
			else{
				if(satr & PATN0ATR){
					eatr |= PATN0ATR;
				}
			}
			satr = eatr;
			sx = ex;
			sy = ey;
		}
		pcVect->SetAtrDXY(dp++, satr, sx, sy);
		pcVect->SetPointCount(dp);

		pcVect = (CVect*)(pcVect->GetNext());
	}
}

void CCmdOutLine::DelNearPoint(CDataList *pcDataList)
{
	DBL		dminix, dminiy, dmaxx, dmaxy;
	DBL		dlenx, dleny, dlimit, dlen;
	CVect	*pcVect;
	int		pcnt, pp, dp;
	int		atr1, atr2;
	DBL		x1, y1, x2, y2;

	pcVect = (CVect*)(pcDataList->m_pcDataTop);
	while(1){
		if(pcVect == NULL){
			break;
		}
		pcVect->GetDMiniMax(&dminix, &dminiy, &dmaxx, &dmaxy);
		dlenx = fabs(dmaxx - dminix);
		dleny = fabs(dmaxy - dminiy);
		if(dlenx < dleny){
			dlimit = dlenx / 200.0;
		}
		else{
			dlimit = dleny / 200.0;
		}
		pcnt = pcVect->GetPointCount();
		pcVect->GetFAtrDXY(0, &atr1, &x1, &y1);
		for(pp = 1, dp = 0; pp < pcnt; pp++){
			pcVect->GetFAtrDXY(pp, &atr2, &x2, &y2);
			dlen = hypot(x1-x2, y1-y2);
			if(dlen < dlimit){
				atr1 = atr2;
			}
			else{
				pcVect->SetFAtrDXY(dp++, atr1, x1, y1);
				atr1 = atr2;
				x1 = x2;
				y1 = y2;
			}
		}
		pcVect->GetFAtrDXY(0, &atr2, &x2, &y2);
		dlen = hypot(x1-x2, y1-y2);
		if(dlen < dlimit){
			atr1 = atr2;
		}
		else{
			pcVect->SetFAtrDXY(dp++, atr1, x1, y1);
		}
		pcVect->SetPointCount(dp);
		pcVect = (CVect*)(pcVect->GetNext());
	}
}

void CCmdOutLine::LegalDataList(CDataList *pcDataList)
{
	CVect	*pcVect;
	CVect	*pcNextVect;

	m_cPtrList.RemoveAll();
	pcVect = (CVect*)(pcDataList->m_pcDataTop);
	while(pcVect != NULL){
		pcNextVect = (CVect*)(pcVect->GetNext());
		if(LegalVect(pcVect) == TRUE){
			m_cPtrList.AddTail(pcVect);
		}
		else{
			delete(pcVect);
		}
		pcVect = pcNextVect;
	}
	pcDataList->m_pcDataTop = NULL;
	POSITION pos = m_cPtrList.GetHeadPosition();
	while(pos){
		pcVect = (CVect*)m_cPtrList.GetNext(pos);
		pcVect->SetNext(NULL);
		pcDataList->SetData(pcVect);
	}
}

int CCmdOutLine::LegalVect(CVect *pcVect)
{
	int		pcnt, pp, dp;
	int		atr1, atr2;
	DBL		len;
	DBL		x1, y1, x2, y2;

	pcnt = pcVect->GetPointCount();
	dp = 0;
	pcVect->GetAtrDXY(0, &atr1, &x1, &y1);
	for(pp = 1; pp < pcnt; pp++){
		pcVect->GetAtrDXY(pp, &atr2, &x2, &y2);
		len = hypot(x1-x2, y1-y2);
		if(0.1 < len || atr1 != 0){
			pcVect->SetAtrDXY(dp, atr1, x1, y1);
			dp++;
			x1 = x2; y1 = y2;
		}
		atr1 = atr2;
	}
	pcVect->GetAtrDXY(0, &atr2, &x2, &y2);
	len = hypot(x1-x2, y1-y2);
	if(0.1 < len || atr1 != 0){
		pcVect->SetAtrDXY(dp, atr1, x1, y1);
		dp++;
	}
	if(dp <= 2){
		return(FALSE);
	}
	pcVect->SetPointCount(dp);
	return(TRUE);
}

int CCmdOutLine::CheckStartEndPoint(DBL xx, DBL yy, DBL sx1, DBL sy1, DBL ex1, DBL ey1, DBL sx2, DBL sy2, DBL ex2, DBL ey2)
{
	if(xx == sx1 && yy == sy1 && xx == ex2 && yy == ey2){
		return(TRUE);
	}
	if(xx == ex1 && yy == ey1 && xx == sx2 && yy == sy2){
		return(TRUE);
	}
	return(FALSE);
}

void CCmdOutLine::SetOnlyLineOrBeze(CDataList *pcDataList)
{
	CVect	*pcVect;

	CCurveUtil *pcCurveUtil = new CCurveUtil(m_pcView);
	pcVect = (CVect*)(pcDataList->m_pcDataTop);
	while(1){
		if(pcVect == NULL){
			break;
		}
		pcCurveUtil->ConvertBezeVect(pcVect);
		pcVect = (CVect*)(pcVect->GetNext());
	}
	delete(pcCurveUtil);

}

void CCmdOutLine::SetRestoreCurve(CDataList *pcDataList)
{
	CVect	*pcVect;
	CVect	*pcTempVect;

	pcTempVect = new CVect(BASEVECTID);
	pcVect = (CVect*)(pcDataList->m_pcDataTop);
	while(1){
		if(pcVect == NULL){
			break;
		}
		SetRestoreCurveVect(pcVect, pcTempVect);
		pcVect = (CVect*)(pcVect->GetNext());
	}
	delete(pcTempVect);
}

void CCmdOutLine::SetRestoreCurveVect(CVect *pcVect, CVect *pcTempVect)
{
	int		pcnt, pp, np, dp;
	dlinetype	ln;
	dbezetype	bz;
	int		atr, kind;
	DBL		xx, yy;

	pcnt = pcVect->GetPointCount();
	dp = 0;
	for(pp = 0; pp < pcnt;){
		np = pcVect->NextPointGet(pp);
		pcVect->GetAtr(pp, &atr);
		kind = SetFigu(pcVect, pp, &bz, &ln);
		if(kind == BEZEKIND){
			if(atr & PATN1ATR){
				pcTempVect->SetAtrDXY(dp++, CURVATR, bz.x1, bz.y1);
				curvebezetpointget(bz, 0.5, &xx, &yy);
				pcTempVect->SetAtrDXY(dp++, CURVATR|CONTATR, xx, yy);
			}
			else if(atr & PATN2ATR){
				if(atr & PATN3ATR){
					pcTempVect->SetAtrDXY(dp++, CURVATR|CONTATR, bz.x1, bz.y1);
				}
				else{
					pcTempVect->SetAtrDXY(dp++, CURVATR, bz.x1, bz.y1);
				}
			}
			else{
				pcTempVect->SetFAtrDXY(dp++, atr, bz.x1, bz.y1);
				pcTempVect->SetAtrDXY(dp++, BEZEATR, bz.x2, bz.y2);
				pcTempVect->SetAtrDXY(dp++, BEZEATR, bz.x3, bz.y3);
			}
		}
		else{
			pcTempVect->SetFAtrDXY(dp++, atr, ln.x1, ln.y1);
		}
		pp = np;
	}

	for(pp = 0; pp < dp; pp++){
		pcTempVect->GetFAtrDXY(pp, &atr, &xx, &yy);
		pcVect->SetFAtrDXY(pp, atr, xx, yy);
	}
	pcVect->SetPointCount(dp);
}

void CCmdOutLine::DelNotConectVect()
{
	POSITION	pos, ppos, tpos;
	CVect	*pcVect, *pcTVect;
	int		pcnt, atr;
	DBL		sx, sy, ex, ey;
	DBL		tsx, tsy, tex, tey;
	int		sidx, eidx;

	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		sidx = 0;
		eidx = 0;
		ppos = pos;
		pcVect = (CVect*)m_cPtrList.GetNext(pos);
		pcnt = pcVect->GetPointCount();
		pcVect->GetAtrDXY(0, &atr, &sx, &sy);
		pcVect->GetAtrDXY(pcnt-1, &atr, &ex, &ey);
		tpos = pos;
		while(tpos != NULL){
			pcTVect = (CVect*)m_cPtrList.GetNext(tpos);
			pcnt = pcTVect->GetPointCount();
			pcTVect->GetAtrDXY(0, &atr, &tsx, &tsy);
			pcTVect->GetAtrDXY(pcnt-1, &atr, &tex, &tey);
			if(EQUAL(sx, tex) && EQUAL(sy, tey)){
				sidx++;
			}
			if(EQUAL(ex, tsx) && EQUAL(ey, tsy)){
				eidx++;
			}
		}
		if(sidx == 0 || eidx == 0){
			tpos = ppos;
			pcVect = (CVect*)m_cPtrList.GetNext(tpos);
			m_cPtrList.RemoveAt(ppos);
			delete(pcVect);
		}
	}
}
