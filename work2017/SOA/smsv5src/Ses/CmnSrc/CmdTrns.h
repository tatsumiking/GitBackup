
#pragma once

#include "../../Object/DibCData.h"

//C 変形関係コマンド処理クラス
class CCmdTrns : public CCmdBase
{
public:
	CCmdTrns(CScrollView* pcview);
	UINT ExecTrnsMirr(UINT event, DBL dx, DBL dy);
	UINT ExecTrnsRound(UINT event, DBL dx, DBL dy);
	UINT ExecTrnsSize(UINT event, DBL dx, DBL dy);
	UINT ExecTrnsSizeTime(UINT event, DBL dx, DBL dy);
	UINT ExecTrnsSlop(UINT event, DBL dx, DBL dy);
	UINT ExecTrnsBoxs(UINT event, DBL dx, DBL dy);
	UINT ExecTrnsArc(UINT event, DBL dx, DBL dy);
	UINT ExecTrnsSlopRag(UINT event, DBL dx, DBL dy);
	UINT ExecTrnsSlopRagUd(UINT event, DBL dx, DBL dy);
	UINT ExecTrnsSizeDisp(UINT event, DBL dx, DBL dy);
	UINT ExecTrns2PointSizeDisp(UINT event, DBL dx, DBL dy);
	UINT ExecTrnsSizeXY(UINT event, DBL dx, DBL dy);
	UINT ExecTrnsSizeY(UINT event, DBL dx, DBL dy);
	UINT ExecTrnsSizeX(UINT event, DBL dx, DBL dy);
	UINT ExecTrnsSize2Point(UINT event, DBL dx, DBL dy);
	UINT ExecTrnsRoundRag(UINT event, DBL dx, DBL dy);
	UINT ExecTrnsRound2Point(UINT event, DBL dx, DBL dy);
	UINT ExecTrnsMirrorConer(UINT event, DBL dx, DBL dy);
public:
	virtual DBL CntrKindInit(DBL dx, DBL dy);
	virtual void CntrKindMove(DBL dx, DBL dy);
	void DrawSlctBoxs(DBL dx, DBL dy);
	void BoxsTrnsVectMain();
	void InitCreateBoxsTrnsVect();
	void ArcTrnsVectMain();
	void ArcTrnsVect(CVect *pcVect, DBL stdx[], DBL stdy[]);
	void ArcTrnsXY(DBL stdx[], DBL stdy[], LPDBL dx, LPDBL dy);
	void ArcTrnsGetPUW(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL dd, LPDBL xuw, LPDBL yuw);
	void InitCreateArcTrnsVect();
	void Main2PointRoundTrns();
	void Main2PointSizeTrns();
	void MainSlopTrns();
	void MainSizeTrns();
	void MainRoundTrns();
	void MainMirrTrns();
	void RoundTrnsData(DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	void RoundAllDibC(DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	void RoundDibC(CDibCData *pcDibCData, DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	void GetSlopElement(DBL dx, DBL dy, LPDBL cx, LPDBL cy, LPDBL ddx, LPDBL ddy);
	void DrawSlopBoxs(CDC *pcDC);
	void SlopTrnsPoint(LPDBL dx, LPDBL dy, DBL cx, DBL cy, DBL ddx, DBL ddy);
	void SlopCntrKindMove(DBL dx, DBL dy);
	DBL SlopCntrKindInit(DBL dx, DBL dy);
	void DrawRoundLine(CDC *pcDC);
	void SetRoundLine(DBL dx, DBL dy);
	void DispSize();
	void Disp2PointSize();
private:
	CVect*	m_pcVect;
	int		m_nPP;
};

