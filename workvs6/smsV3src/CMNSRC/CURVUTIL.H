#ifndef __CURVUTIL_H
#define __CURVUTIL_H

class CCurveUtil
{
public:
	CTonView	*m_pcView;
	CCurveUtil(CScrollView* pcView);
public:
	int SetDivBeze(dbezetype bz, CVect *pcVect, int dp);
	int CheckBezeOverLimit(dbezetype bz);
	void ConvertBezeDispath(CData *pcData);
	void ConvertBezeDataGrup(CDataGrup *pcDataGrup);
	void ConvertBezeDataList(CDataList *pcDataList);
	void ConvertBezeVect(CVect* pcVect);
	void StartArcLegal(CVect *pcVect);
private:
	int ConvertBezeVectArea(CVect* pcVect, int sp, int ep, CVect *pcTempVect, int dp);
	int ConvertBezeArc(CVect *pcVect, int sp, int ep, CVect *pcTempVect, int dp);
	int ConvertBezeFreeArea(CVect *pcVect, int sp, int ep, CVect *pcTempVect, int dp);
	int ConvertBezeFreeCurve(CVect *pcVect, int sp, int ep, CVect *pcTempVect, int dp);
	DBL GetFreeCurveAreaLength(CVect* pcVect, int sp, int ep);
	DBL GetFreeCurveLength(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetBezeLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	void GetFreeCurveOnPoint(DBL slen, DBL *retx, DBL *rety, CVect* pcVect, int sp, int ep);
	DBL GetFreeCurveOnXY(DBL setlen, LPDBL dx, LPDBL dy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetBezeOnXY(DBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	DBL GetLineOnXY(DBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2);
	int SetFigu(CVect *pcVect, int pp, dbezetype *bz, dlinetype *ln);
};

#endif
