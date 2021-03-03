
/////////////////////////////////////////////////////////////////////////////
// CCmdLaser

#include "stdafx.h"
#include "math.h"

#include "dlginnum.h"
#include "cmd_base.h"
#include "cmdlaser.h"
extern "C" {
	BOOL  pdrwatrarcchk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrbezechk(int atr1, int atr2, int atr3);
	int curve3pcenterget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL FAR *x0, DBL FAR *y0);
	int curve3pragrget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0, DBL FAR *rag01, DBL FAR *rag03, DBL FAR *r);
	void plgnfillstart();
	void plgnfillrejionstart();
	void plgnfillrejionend();
	void plgnfilldraw(HDC PaintDC);
	void scrnsetclrbrushandpen(HDC PaintDC, COLORREF fillclr, COLORREF lineclr, int nthick);
	void scrnpenend(HDC PaintDC);
	void scrndlenmmtorltv(LPDBL len);
	int curvenkindget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
}

CCmdLaser::CCmdLaser(CScrollView* pcview)
:CCmdBase(pcview)
{
}

// レーザー出力のために開始点にひげを付加する
UINT CCmdLaser::ExecHige(UINT event, DBL dx, DBL dy)
{

	if(event != INITEVENT){
		return(0);
	}

	SaveUndo();

	if(VectRoundLegal() == FALSE){
		return(ENDEVENT);
	}

	AddHigePoint();

	return(ENDEVENT);
}

// 指定された図形にひげを付加する
UINT CCmdLaser::ExecSlctHige(UINT event, DBL dx, DBL dy)
{
	POSITION	pos;
	int			pp;
	CData		*pcData;

	CTonDoc *pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		m_nMouseSetCount = 1;
		ScrnAllRedraw();
		break;
	case SETEVENT:
		pcData = SrchDataFromList(dx, dy, &pos, &pp);
		if(pcData != NULL){
			SaveUndo();
			if(VectRoundLegal() == FALSE){
				return(ENDEVENT);
			}
			VectDivSetDataList();
			if(CheckResetHigePoint(pcData) == TRUE){
				pcData = SrchDataFromList(dx, dy, &pos, &pp);
			}
			AddHigePointData(pcData, pp);
			m_pcPtrList->RemoveAt(pos);
			pcDoc->DataAddTail(pcData);
			ScrnDataRedraw(pcData);
		}
		break;
	case REDRAWEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawModeSET();
			ListRedraw();
			EndDraw();
		}
 		break;
	case RESETEVENT:
	case ENDEVENT:
		m_nMouseSetCount = 0;
		SetDataToDoc();
		return(ENDEVENT);
	}
	return(0);
}

// 小さな図形から出力するように図形の描画順番を変更する
UINT CCmdLaser::ExecSmallToBig(UINT event, DBL dx, DBL dy)
{
	CData	*pcData1, *pcData2;
	POSITION	pos1, tpos1;
	POSITION	pos2, tpos2;
	DBL		minx1, miny1, maxx1, maxy1;
	DBL		minx2, miny2, maxx2, maxy2;
	DBL		size1, size2;

	if(event != INITEVENT){
		return(0);
	}
	SaveUndo();

	VectDivSetDataList();

	pos1 = m_pcPtrList->GetHeadPosition();
	while (pos1 != NULL)
	{
		tpos1 = pos1;
		pcData1 = m_pcPtrList->GetNext(pos1);
		pcData1->GetDMiniMax(&minx1, &miny1, &maxx1, &maxy1);
		size1 = maxx1-minx1 + maxy1-miny1;
		pos2 = tpos1;
		while(pos2){
			tpos2 = pos2;
			pcData2 = m_cPtrList.GetNext(pos2);
			pcData2->GetDMiniMax(&minx2, &miny2, &maxx2, &maxy2);
			size2 = maxx2-minx2 + maxy2-miny2;
			if(size2 < size1){
				m_cPtrList.SetAt(tpos1, pcData2);
				m_cPtrList.SetAt(tpos2, pcData1);
				pcData1 = pcData2;
				size1 = size2;
			}
		}
	}

	SetDataToDoc();
	return(ENDEVENT);
}	

//　円弧に近似できるベーゼ曲線を円弧に変換する
UINT CCmdLaser::ExecBezeToArc(UINT event, DBL dx, DBL dy)
{
	CVect	*pcVect;
	POSITION	pos;

	if(event != INITEVENT){
		return(0);
	}
	SaveUndo();
	SetDataListSelectID(BASEVECTID, ACTIVEFLAG);
	CTonDoc *pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pos = m_pcPtrList->GetHeadPosition();
	while (pos != NULL)
	{
		pcVect = (CVect*)m_pcPtrList->GetNext(pos);
		BezeToArcMain(pcVect);
	}

	return(ENDEVENT);
}	

// 指定された図形の回転方向を変更する
UINT CCmdLaser::ExecRejionReverse(UINT event, DBL dx, DBL dy)
{
	CVect		*pcVect;
	int			retpp;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		pcVect = (CVect*)SrchDataPoint(dx, dy, &retpp);
		if(pcVect != NULL && pcVect->CheckID() == BASEVECTID){
			SaveUndo();
			if(pcVect->CheckFlag(REJIONFLAG) == ON){
				pcVect->ReverceRejion();
			}
			else{
				ReverceNorejion(pcVect);
			}
			ScrnDataRedraw(pcVect);
		}
		break;
	case RESETEVENT:
	case ENDEVENT:
		return(ENDEVENT);
	}
	return(0);
}

UINT CCmdLaser::ExecVectCount(UINT event, DBL dx, DBL dy)
{
	int		cnt;

	if(event != INITEVENT){
		return(0);
	}
	SetDataListSelectID(BASEVECTID, ACTIVEFLAG);
	cnt = m_pcPtrList->GetCount();
	sprintf(m_szMsgBuf, "出力対象の図形数 %d個", cnt);
	::AfxMessageBox(m_szMsgBuf, MB_OK);
	return(ENDEVENT);
}

// 図形の回転方向を正規化する
int CCmdLaser::VectRoundLegal()
{
	CDialogInputNum	*pDialogInputNum;

	m_dMM = 2;
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle("レーザー用開始位置設定");
	pDialogInputNum->SetUnitText("mm");
	pDialogInputNum->SetTitleText("何mm離す");
	pDialogInputNum->m_dInputNum = FLOOR100(m_dMM);
	m_dMM = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		m_dMM = pDialogInputNum->m_dInputNum;
	}
	delete(pDialogInputNum);
	if(m_dMM <= 0){
		return(FALSE);
	}
	m_pcCmdSpcl = m_pcCmdOutLine->m_pcCmdSpcl;
	m_pcCmdOutLine->StrgDataChange();
	CTonDoc *pcDoc = (CTonDoc*)(m_pcView->GetDocument());

	SetDataListSelectID(BASEVECTID, ACTIVEFLAG);
	BitMapDrawInit();
	BitMapDraw();
	RejionLegal();
	m_pcCmdOutLine->BitMapDrawEnd();
	return(TRUE);
}

// すべてのアクティブなベクトル構造を個別データに変換しリスト構造にセットする
void CCmdLaser::VectDivSetDataList()
{
	POSITION	pos, tpos;
	CData		*pcData;
	CDataList	*pcDataList;
	CData		*pcNewData;

	CTonDoc *pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	SetDataListSelectID(BASEVECTID, ACTIVEFLAG);
	pos = m_pcPtrList->GetHeadPosition();
	while (pos != NULL)
	{
		tpos = pos;
		pcData = m_pcPtrList->GetNext(pos);
		pcDataList = (CDataList*)pcDoc->NewData(DATALISTID);
		pcNewData = pcData->CopyData(0.0, 0.0);
		pcDataList->SetData(pcNewData);
		pcDataList->RenewMiniMax();
		m_pcPtrList->SetAt(tpos, pcDataList);
	}
	pcDoc->DataAllCls();
}

// リスト構造内のデータをドキュメントのデータ構造にセットする
void CCmdLaser::SetDataToDoc()
{
	POSITION	pos;
	CData		*pcData;

	CTonDoc *pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pos = m_pcPtrList->GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_pcPtrList->GetNext(pos);
		pcDoc->DataAddTail(pcData);
	}
}

// リスト構造からいちばん近い図形を検索する
CData *CCmdLaser::SrchDataFromList(DBL dx, DBL dy, POSITION *retpos, int *retpp)
{
	POSITION	pos, tpos;
	DBL		dlen, tlen;
	int		tpp;
	CData	*pcData;
	CData	*retData;

	retData = NULL;
	dlen = INTMAX;
	pos = m_pcPtrList->GetHeadPosition();
	while (pos != NULL)
	{
		tpos = pos;
		pcData = m_pcPtrList->GetNext(pos);
		pcData->SrchNearPoint(dx, dy, &tlen, &tpp);
		if(dlen > tlen){
			dlen = tlen;
			*retpp = tpp;
			retData = pcData;
			*retpos = tpos;
		}
	}
	scrndlenmmtorltv(&dlen);
	if(HNEARLIMIT < dlen){
		return(NULL);
	}
	return(retData);
}

// ひげを図形の外側付加する
void CCmdLaser::AddHigePointData(CData *pcData, int p1)
{
	CVect	*pcVect;
	CVect	*pcTVect;
	int		pcnt, pp, dp, atr;
	DBL		dx, dy;

	pcTVect = new CVect(BASEVECTID);
	pcVect = (CVect *)(((CDataList *)pcData)->m_pcDataTop);
	pcnt = pcVect->GetPointCount();
	for(pp = p1, dp = 0; pp < pcnt; pp++){
		pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
		pcTVect->SetAtrDXY(dp++, atr, dx, dy);
	}
	for(pp = 0; pp < p1; pp++){
		pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
		pcTVect->SetAtrDXY(dp++, atr, dx, dy);
	}

	for(pp = 0; pp < pcnt; pp++){
		pcTVect->GetAtrDXY(pp, &atr, &dx, &dy);
		pcVect->SetAtrDXY(pp, atr, dx, dy);
	}
	delete(pcTVect);

	AddHigePointVect(pcVect);
}

// ビットマップイメージ初期化
void CCmdLaser::BitMapDrawInit()
{
	DBL		tminix, tminiy, tmaxx, tmaxy;
	DBL		dminix, dminiy, dmaxx, dmaxy;
	DBL		xytime;
	CVect	*pcVect;

	dminix = INTMAX;
	dminiy = INTMAX;
	dmaxx = -INTMAX;
	dmaxy = -INTMAX;
	POSITION pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcVect = (CVect*)m_cPtrList.GetNext(pos);
		pcVect->RenewMiniMax();
		pcVect->GetDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
		dminix = min(dminix, tminix);
		dminiy = min(dminiy, tminiy);
		dmaxx = max(dmaxx, tmaxx);
		dmaxy = max(dmaxy, tmaxy);
	}
	xytime = (dmaxy - dminiy) / (dmaxx - dminix);
	m_pcCmdSpcl->MemoryInit(xytime);
	m_pcCmdSpcl->DrawAreaInit(dminix, dminiy, dmaxx, dmaxy);
	m_pcCmdSpcl->BitMapInit();
}

// リスト構造にリンクしている図形をビットマップイメージに描画する
void CCmdLaser::BitMapDraw()
{
	COLORREF	clr1;
	long		lBitsSize;
	CVect		*pcVect;

	m_pcCmdSpcl = m_pcCmdOutLine->m_pcCmdSpcl;
	clr1 = RGB(255, 255, 255);
	m_pcCmdSpcl->BitMapInit();
	m_pcCmdSpcl->m_pMemDC->SetROP2(R2_XORPEN);
	scrnsetclrbrushandpen(m_pcCmdSpcl->m_pMemDC->m_hDC, clr1, clr1, 1);
	POSITION pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcVect = (CVect*)m_cPtrList.GetNext(pos);
		plgnfillstart();
		plgnfillrejionstart();
		pcVect->FillDraw(m_pcCmdSpcl->m_pMemDC);
		plgnfillrejionend();
		plgnfilldraw(m_pcCmdSpcl->m_pMemDC->m_hDC);
		pcVect = (CVect *)pcVect->GetNext();
	}	
	scrnpenend(m_pcCmdSpcl->m_pMemDC->m_hDC);

	m_pcCmdSpcl->m_nXByte = (((long)m_pcCmdSpcl->m_nXSize + 7L) / 8L + 3L) / 4L * 4L;
	lBitsSize = (long)m_pcCmdSpcl->m_nYSize * m_pcCmdSpcl->m_nXByte;
	m_pcCmdSpcl->m_pBitmap->GetBitmapBits(lBitsSize, m_pcCmdSpcl->m_lpBits);
}

// 図形回転方向の正規化
void CCmdLaser::RejionLegal()
{
	CVect	*pcVect;

	POSITION pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcVect = (CVect*)m_cPtrList.GetNext(pos);
		if(m_pcCmdOutLine->RejionLegalVectXAdd(pcVect) == FALSE){
			m_pcCmdOutLine->RejionLegalVectYAdd(pcVect);
		}
	}
}

// リスト構造にリンクしている図形にひげを付加する
void CCmdLaser::AddHigePoint()
{
	CVect	*pcVect;

	POSITION pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcVect = (CVect*)m_cPtrList.GetNext(pos);
		StratPointLegal(pcVect);
		AddHigePointVect(pcVect);
	}
}

// ベクトル構造にひげを付加する
void CCmdLaser::AddHigePointVect(CVect *pcVect)
{
	CVect	*pcTVect;
	int		pcnt, pp, dp, atr;
	DBL		x0, y0, x1, y1, x2, y2;
	DBL		rag1, rag2, subrag, rag, mm;
	int		nkind;

	mm = m_dMM;
	pcnt = pcVect->GetPointCount();
	pcTVect = new CVect(BASEVECTID);
	GetTreePoint(pcVect, &x0, &y0, &x1, &y1, &x2, &y2);
	dp = 0;
	m_pcCmdOutLine->GetRoundRag(x1, y1, x0, y0, &rag1);
	m_pcCmdOutLine->GetRoundRag(x1, y1, x2, y2, &rag2);
	subrag = rag1 - rag2;
	rag = subrag / 2 + rag2;
	m_pcCmdOutLine->SetRoundPoint(x1, y1, rag, mm, 0.0, &x0, &y0);
	nkind = curvenkindget(x1, y1, x2, y2, x0, y0);
	if(nkind < 0){
		x0 = x1 * 2 - x0;
		y0 = y1 * 2 - y0;
	}
	pcTVect->SetAtrDXY(dp++, 0, x0, y0);
	for(pp = 0; pp < pcnt; pp++){
		pcVect->GetAtrDXY(pp, &atr, &x1, &y1);
		if(pp == 0){
			atr &= ~CONTATR;
		}
		pcTVect->SetAtrDXY(dp++, atr, x1, y1);
	}
	pcVect->GetAtrDXY(0, &atr, &x1, &y1);
	pcTVect->SetAtrDXY(dp++, 0, x1, y1);
	pcnt = dp;
	for(pp = 0; pp < pcnt; pp++){
		pcTVect->GetAtrDXY(pp, &atr, &x1, &y1);
		pcVect->SetAtrDXY(pp, atr, x1, y1);
	}
	pcVect->SetPointCount(pcnt);
	pcVect->OffFlag(REJIONFLAG);
	pcVect->RenewMiniMax();
	delete(pcTVect);
}

// 始点と重複しない前後の点をセットする
void CCmdLaser::GetTreePoint(CVect *pcVect, DBL *retx0, DBL *rety0, DBL *retx1, DBL *rety1, DBL *retx2, DBL *rety2)
{
	int		pcnt, pp, atr;
	DBL		x0, y0, x1, y1, x2, y2;

	pcnt = pcVect->GetPointCount();
	pcVect->GetAtrDXY(0, &atr, &x1, &y1);
	for(pp = 1; pp < pcnt; pp++){
		pcVect->GetAtrDXY(pp, &atr, &x2, &y2);
		if(!(x1 == x2 && y1 == y2)){
			break;
		}
	}
	for(pp = pcnt-1; pp >= 0; pp--){
		pcVect->GetAtrDXY(pp, &atr, &x0, &y0);
		if(!(x1 == x0 && y1 == y0)){
			break;
		}
	}
	*retx0 = x0; *rety0 = y0;
	*retx1 = x1; *rety1 = y1;
	*retx2 = x2; *rety2 = y2;
}

// 図形の始点を角に設定する
void CCmdLaser::StratPointLegal(CVect *pcVect)
{
	CVect	*pcTVect;
	int		pcnt, pp, p1, dp, atr;
	DBL		x1, y1;

	pcnt = pcVect->GetPointCount();
	for(pp = 0; pp < pcnt; pp++){
		pcVect->GetAtrDXY(pp, &atr, &x1, &y1);
		if((atr & CONTATR) == 0){
			break;
		}
	}
	if(pp == pcnt){
		return;
	}
	pcTVect = new CVect(BASEVECTID);
	dp = 0;
	p1 = pp;
	for(;pp < pcnt; pp++){
		pcVect->GetAtrDXY(pp, &atr, &x1, &y1);
		pcTVect->SetAtrDXY(dp++, atr, x1, y1);
	}
	for(pp = 0; pp < p1; pp++){
		pcVect->GetAtrDXY(pp, &atr, &x1, &y1);
		pcTVect->SetAtrDXY(dp++, atr, x1, y1);
	}
	for(pp = 0; pp < pcnt; pp++){
		pcTVect->GetAtrDXY(pp, &atr, &x1, &y1);
		pcVect->SetAtrDXY(pp, atr, x1, y1);
	}
	delete(pcTVect);
}

//　図形の円弧に近似できるベーゼ曲線を円弧に変換する
void CCmdLaser::BezeToArcMain(CVect *pcVect)
{
	CVect	*pcTVect;
	int pcnt;
	int dp, pp;
	int p0, p1, p2, p3, p4;
	int atr, atr0, atr1, atr2, atr3, atr4;
	double x0, x1, x2, x3, x4;
	double y0, y1, y2, y3, y4;

	pcTVect = new CVect(BASEVECTID);
	pcnt = pcVect->GetPointCount();
	pcVect->GetAtrDXY(0, &atr1, &x1, &y1);
	pcVect->GetAtrDXY(pcnt-1, &atr2, &x2, &y2);
	if(EQUAL(x1, x2) && EQUAL(y1, y2)){
		pcVect->OnFlag(REJIONFLAG);
		pcnt--;
	}
	dp = 0;
	for(pp = 0; pp < pcnt;){
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
			if(p3 == 0){
				pcTVect->SetAtrDXY(dp++, 0, x1, y1);
				pcTVect->SetAtrDXY(dp++, 0, x2, y2);
			}
			else if(p2 == 0){
				pcTVect->SetAtrDXY(dp++, 0, x1, y1);
			}
			else{
				dp = BezeToArcFigu(x1, y1, x2, y2, x3, y3, x4, y4, pcTVect, dp);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 == 0){
				pcTVect->SetAtrDXY(dp++, 0, x1, y1);
			}
			else{
				pcTVect->SetAtrDXY(dp++, atr1, x1, y1);
				pcTVect->SetAtrDXY(dp++, atr2, x2, y2);
			}
			pp += 2;
		}
		else{
			pcTVect->SetAtrDXY(dp++, atr1, x1, y1);
			pp += 1;
		}
	}

	for(pp = 0; pp < dp; pp++){
		pcTVect->GetAtrDXY(pp, &atr, &x1, &y1);
		pcVect->SetAtrDXY(pp, atr, x1, y1);
	}
	pcVect->SetPointCount(dp);
	delete(pcTVect);
}

// ベーゼ曲線を円弧に近似する
int CCmdLaser::BezeToArcFigu(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4, CVect *pcVect, int dp)
{
	DBL		t, t1;
	int		i, cnt, stat;
	DBL		b1, b2, b3, b4;
	DBL		x0, y0;
	DBL		rag, rag01, rag03, steprag;
	DBL		dr;
	DBL		tx1, ty1, tx2, ty2;
	DBL		step, len;

	t = 0.5;
	t1 = 1 - t;
	b1 = t1 * t1 * t1;
	b2 = 3 * t * t1 * t1;
	b3 = 3 * t * t * t1;
	b4 = t * t * t;
	tx2 = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
	ty2 = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
	stat = curve3pcenterget(x1, y1, tx2, ty2, x4, y4, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(x1, y1, tx2, ty2, x4, y4, x0, y0, &rag01, &rag03, &dr);
	}
	if(stat == FAIL){
		pcVect->SetAtrDXY(dp++, 0, x1, y1);
		return(dp);
	}
	cnt = 4;
	steprag = (rag03 - rag01) / (DBL)cnt;
	step = 1.0 / (DBL)cnt;
	for(i = 1; i < cnt; i++){
		rag = steprag * i + rag01;
		tx1 = cos(rag)*dr+x0;
		ty1 = sin(rag)*dr+y0;
		t = step * i;
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		tx2 = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ty2 = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		len = hypot(tx1-tx2, ty1-ty2);
		if(dr * 0.02 < len){
			pcVect->SetAtrDXY(dp++, BEZEATR|CURVATR, x1, y1);
			pcVect->SetAtrDXY(dp++, BEZEATR, x2, y2);
			pcVect->SetAtrDXY(dp++, BEZEATR, x3, y3);
			return(dp);
		}
	}
	pcVect->SetAtrDXY(dp++, CURVATR, x1, y1);
	rag = (rag03 - rag01) / 2 + rag01;
	x2 = cos(rag)*dr+x0;
	y2 = sin(rag)*dr+y0;
	pcVect->SetAtrDXY(dp++, CURVATR|CONTATR, x2, y2);
	return(dp);
}

// リスト構造を描画する
void CCmdLaser::ListRedraw()
{
	CData	*pcData;

	POSITION pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_cPtrList.GetNext(pos);
		pcData->OnDraw(m_pcDC);
	}
}

// ひげデータを削除する
int CCmdLaser::CheckResetHigePoint(CData *pcData)
{
	CVect	*pcVect;
	CVect	*pcTVect;
	int		pcnt, pp, dp, atr;
	DBL		dx, dy, x0, y0, xx, yy;

	pcTVect = new CVect(BASEVECTID);
	pcVect = (CVect *)(((CDataList *)pcData)->m_pcDataTop);
	pcnt = pcVect->GetPointCount();
	pcVect->GetAtrDXY(1, &atr, &x0, &y0);
	pcVect->GetAtrDXY(pcnt-1, &atr, &xx, &yy);
	if(x0 != xx || y0 != yy){
		return(FALSE);
	}
	for(pp = 1, dp = 0; pp < pcnt-1; pp++){
		pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
		pcTVect->SetAtrDXY(dp++, atr, dx, dy);
	}
	pcnt = dp;
	for(pp = 0; pp < pcnt; pp++){
		pcTVect->GetAtrDXY(pp, &atr, &dx, &dy);
		pcVect->SetAtrDXY(pp, atr, dx, dy);
	}
	pcVect->SetPointCount(pcnt);
	pcVect->OnFlag(REJIONFLAG);
	delete(pcTVect);
	return(TRUE);
}

// ひげを含む開放図形の回転方向を変更する
void CCmdLaser::ReverceNorejion(CVect *pcVect)
{
	CVect	*pcTmpVect;
	int		pcnt, pp, atr;
	DBL		dx, dy;
	DBL		x1, y1, x2, y2;

	pcTmpVect = new CVect(BASEVECTID);
	pcnt = pcVect->GetPointCount();
	for(pp = 1; pp < pcnt; pp++){
		pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
		pcTmpVect->SetAtrDXY(pp-1, atr, dx, dy);
	}
	pcTmpVect->SetPointCount(pcnt-1);
	pcTmpVect->OnFlag(REJIONFLAG);
	pcTmpVect->ReverceRejion();
	for(pp = 0; pp < pcnt-1; pp++){
		pcTmpVect->GetAtrDXY(pp, &atr, &dx, &dy);
		pcVect->SetAtrDXY(pp+1, atr, dx, dy);
	}
	pcVect->GetAtrDXY(0, &atr, &x1, &y1);
	pcVect->GetAtrDXY(1, &atr, &x2, &y2);
	dx = x2*2-x1;
	dy = y1*2-y1;
	pcVect->SetAtrDXY(0, 0, dx, dy);
	pcVect->OffFlag(REJIONFLAG);
	pcVect->SetPointCount(pcnt);
	delete(pcTmpVect);
}
