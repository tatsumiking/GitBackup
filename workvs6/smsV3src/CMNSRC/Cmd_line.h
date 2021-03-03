#ifndef __CMD_LINE_H
#define __CMD_LINE_H

#include "dlginnum.h"

class CCrossPoint;
class CFigure;

//C 線操作関係コマンド処理クラス
class CCmdLine : public CCmdBase
{
public:
	CDialogInputNum	*m_pcDialogInputNum;
	typedef CTypedPtrList<CPtrList, CFigure*> m_cFigureList;
	typedef CTypedPtrList<CPtrList, CCrossPoint*> m_cCrosList;
	typedef CTypedPtrList<CPtrList, CCrossPoint*> *m_pcCrsLst;
	CVect	*m_pcVect;
	CVect	*m_pcVect2;
	CVect	*m_pcCopyVect;
	int		m_nPP;
	int		m_nPP2;
	int		m_nAtr;
	DBL		m_dConerrmm;
	int		m_nSelectPoint;
	int		m_nLocalCommand;

	CCmdLine(CScrollView* pcview);
	UINT ExecLineMove(UINT event, DBL dx, DBL dy);
	UINT ExecLineLineing(UINT event, DBL dx, DBL dy);
	UINT ExecLineCurve(UINT event, DBL dx, DBL dy);
	UINT ExecLineCut(UINT event, DBL dx, DBL dy);
	UINT ExecLineJoin(UINT event, DBL dx, DBL dy);
	UINT ExecLineCutJoin(UINT event, DBL dx, DBL dy);
	UINT ExecLineBeze(UINT event, DBL dx, DBL dy);
	UINT ExecLineHalfArc(UINT event, DBL dx, DBL dy);
	UINT ExecLineArc(UINT event, DBL dx, DBL dy);
	UINT ExecLineCrcl(UINT event, DBL dx, DBL dy);
	UINT ExecLineConerset(UINT event, DBL dx, DBL dy);
	UINT ExecLineConerarc(UINT event, DBL dx, DBL dy);
	UINT ExecLineDiv(UINT event, DBL dx, DBL dy);
public:
	void ToBeze();
	void ToBezeVect(CVect *pcVect);
	int CrossPointAfter(DBL len, DBL xx, DBL yy, CVect *srcVect, int pp);
	int SetDivFiguAfter(DBL len, DBL xx, DBL yy, CVect *srcVect, int pp, CVect *dstVect, int dp);
	int CrossPointBefore(DBL len, DBL xx, DBL yy, CVect *srcVect, int pp);
	int SetDivFiguBefore(DBL len, DBL xx, DBL yy, CVect *srcVect, int pp, CVect *dstVect, int dp);
	int ReadLineDivCount();
	void LineDivMain(int ndivcnt);
	UINT ExecLineDel(UINT event, DBL dx, DBL dy);
	int GetConerAreaOutPoint(DBL dotr, int *retpp1, int *retpp2);
	void ConersetMain();
	void ConerarcMain();
	void SelectVectCrcling();
	void ArcSelectPoint(DBL dx, DBL dy);
	void BezeSelectPoint(DBL dx, DBL dy);
	void BezeMovePoint(DBL dx, DBL dy);
	void ArcMovePoint(DBL dx, DBL dy);
	void DrawSelectBeze(CDC *pcDC);
	void DrawSelectArc(CDC *pcDC);
	void BezeSelectVect();
	void ArcSelectVect();
	void HalfArcRevace();
	void HalfArcSelectVect();
	UINT SelectCommand(UINT event, DBL dx, DBL dy);
	UINT BezeCommand(UINT event, DBL dx, DBL dy);
	UINT ArcCommand(UINT event, DBL dx, DBL dy);
	UINT HalfArcCommand(UINT event, DBL dx, DBL dy);
	void OneVectCutJoin(DBL mm, dcrostype crs1, dcrostype crs2);
	void JoinVect(CVect *pcVect11, CVect *pcVect12, CVect *pcVect21, CVect *pcVect22);
	void JoinCloseClose(CVect *pcVect11, CVect *pcVect21);
	void JoinOpenClose(CVect *pcVect11, CVect *pcVect12, CVect *pcVect21);
	void JoinOpenOpen(CVect *pcVect11, CVect *pcVect12, CVect *pcVect21, CVect *pcVect22);
	void CreateNewVect(CVect *pcVect);
	void CutVect(DBL mm, dcrostype crs);
	void SortCrossPoint(dcrostype *crs, int crsmax);
	void LineCutJointMain(DBL mm);
	void LineJoinMain();
	void LineCutMain();
	void LineCurveMain();
	void LineLineingMain();
private:

};

#endif
