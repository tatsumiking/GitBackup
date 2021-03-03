#ifndef __CROSVECT_H
#define __CROSVECT_H

//C 交点検出処理クラス
class CCrossVect
{
public:
	CCrossVect(CScrollView* pcview);
public:
	CTonView*	m_pcView;
	CTonDoc*	m_pcDoc;

	CVect		*m_pcVect;
	DBL			m_dSpaceMM;
	CTypedPtrList<CObList,CData*>	*m_pcPtrList;
public:
	void SetSpaceMM(DBL mm);
	void SetDataList(CTypedPtrList<CObList,CData*> *pList);
	void GetCrossPoint(dlinetype ln1, dcrostype * crs, int *crsmax);
	void GetCrossPointVect(CVect *pcVect, dlinetype ln1, dcrostype * crs, int *crsmax);
	int SearchVectRotateDirection(CVect *pcVect);
private:
	void GetRejionCrossPoint(CVect *pcVect, dlinetype ln1, dcrostype * crs, int *crsmax);
	void GetNoRejionCrossPoint(CVect *pcVect, dlinetype ln1, dcrostype * crs, int *crsmax);
};

#endif
