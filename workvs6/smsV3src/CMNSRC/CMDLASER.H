#ifndef __CMDLASER_H
#define __CMDLASER_H

class CCmdLaser : public CCmdBase
{
public:
	CCmdLaser(CScrollView* pcview);
public:
	DBL		m_dMM;
	CCmdOutLine	*m_pcCmdOutLine;
	CCmdSpcl	*m_pcCmdSpcl;
public:
	void ReverceNorejion(CVect *pcVect);
	int CheckResetHigePoint(CData *pcData);
	void ListRedraw();
	UINT ExecHige(UINT event, DBL dx, DBL dy);
	UINT ExecSlctHige(UINT event, DBL dx, DBL dy);
	UINT ExecSmallToBig(UINT event, DBL dx, DBL dy);
	UINT ExecBezeToArc(UINT event, DBL dx, DBL dy);
	UINT ExecRejionReverse(UINT event, DBL dx, DBL dy);
	UINT ExecVectCount(UINT event, DBL dx, DBL dy);
	int VectRoundLegal();
	void VectDivSetDataList();
	void SetDataToDoc();
	CData *SrchDataFromList(DBL dx, DBL dy, POSITION *retpos, int *retpp);
	void AddHigePointData(CData *pcData, int p1);
	int BezeToArcFigu(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4, CVect *pcVect, int dp);
	void BezeToArcMain(CVect *pcVect);
	void GetTreePoint(CVect *pcVect, DBL *x0, DBL *y0, DBL *x1, DBL *y1, DBL *x2, DBL *y2);
	void BitMapDrawInit();
	void BitMapDraw();
	void RejionLegal();
	void AddHigePoint();
	void StratPointLegal(CVect *pcVect);
	void AddHigePointVect(CVect *pcVect);
private:

};

#endif
