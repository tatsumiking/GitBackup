#pragma once

//C 交点検出処理クラス
class CCrossLib
{
public:
	CCrossLib(CScrollView* pcview);
public:
	CTonView*	m_pcView;
	CTonDoc*	m_pcDoc;

	CVect		*m_pcVect;
	CTypedPtrList<CObList,CData*>	*m_pcPtrList;
	typedef CTypedPtrList<CPtrList, CFigure*> m_cFigureList;

public:
	void SetDataList(CTypedPtrList<CObList,CData*> *pList);
	void GetCrossPoint(dlinetype ln1, dcrostype * crs, int *crsmax, int limit);
	void GetCrossPointVect(CVect *pcVect, dlinetype ln1, dcrostype * crs, int *crsmax);
private:
	void GetRejionCrossPoint(CVect *pcVect, dlinetype ln1, dcrostype * crs, int *crsmax);
	void GetNoRejionCrossPoint(CVect *pcVect, dlinetype ln1, dcrostype * crs, int *crsmax);
	void GetCrossPointFigu(dlinetype ln1, dlinetype ln2, int pp, dcrostype * crs, int *crsmax);
	void GetCrossPointFigu(dlinetype ln1, darctype ac2, int pp, dcrostype * crs, int *crsmax);
	void GetCrossPointFigu(dlinetype ln1, dbezetype bz2, int pp, dcrostype * crs, int *crsmax);
};

