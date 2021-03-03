#ifndef __FILEARC_H
#define __FILEARC_H

class CFileArc
{
public:
	CFileArc(CScrollView* pcview);
public:
	CTonView*	m_pcView;
	CTonDoc*	m_pcDoc;
public:
	void LoadData(LPSTR fname);
private:
	CDataList	*m_pcDataList;
	CVect	*m_pcVect;
private:
};

#endif

