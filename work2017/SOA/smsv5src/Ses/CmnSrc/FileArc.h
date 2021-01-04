
#pragma once

class CFileArc
{
public:
	CFileArc(CScrollView* pcview);
public:
	CTonView*	m_pcView;
	CTonDoc*	m_pcDoc;
public:
	void LoadData(LPTSTR fname);
private:
	CDataList	*m_pcDataList;
	CVect	*m_pcVect;
private:
};


