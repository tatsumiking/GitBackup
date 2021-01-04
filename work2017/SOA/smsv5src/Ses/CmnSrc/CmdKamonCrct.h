
#pragma once


class CCmdKamonCrct : public CCmdBase
{
public:
	CCmdKamonCrct(CScrollView* pcview);

public:
	UINT ExecKamonLoad(UINT event, DBL dx, DBL dy);
	void KamonLoadSetVect(DBL size, LPTSTR fontname, LPTSTR lpch);

private:
	void TTGlyphToDataList(CDataList *pcDataList);
	int MovetoSet(CVect *pcVect, int dp, LPPOINTFX lpcood);
	int PolygonSet(CVect *pcVect, int dp, LPPOINTFX lpcood, WORD count);
	int ConicSet(CVect *pcVect, int dp, LPPOINTFX lpcood, WORD count);
	int BezeSet(CVect *pcVect, int dp, LPPOINTFX lpcood, WORD count);
	int PolyCurveSet(CVect *pcVect, int dp, LPPOINTFX lpcood, WORD count);
	int B_splinePointSet(CVect *pcVect, int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void LastjoinSet(CVect *pcVect, int dp);
	void TrnsPoint(DBL *retx, DBL *rety);

private:
	CWinTTFont* m_pcWNTTFont;
	HANDLE		m_hGlyphBuf;
	LPBYTE		m_lpGlyphBuf;
	long		m_lGlyphLen;
	DBL			m_dVectCrtX, m_dVectCrtY;
	DBL			m_dCrtx, m_dCrty;
	DBL			m_dRsx, m_dRsy;
	DBL			m_dXaddtime, m_dYaddtime;
	DBL			m_dXaddmove, m_dYaddmove;
	DBL			m_dBezeaddtime;
	DBL			m_dTrnsTime;
};


