
#pragma once

class CFileDxf
{
public:
	CFileDxf(CScrollView* pcview);
public:
	CTonView*	m_pcView;
	CTonDoc*	m_pcDoc;
public:
	void SaveData(LPTSTR filename);
	void LoadData(LPTSTR fname);
private:
	FILE*	m_pFile;
	CDataGrup	*m_pcDataGrup;
	CDataList	*m_pcDataList;
	CVect	*m_pcVect;
	int		m_nPointCount;
	DBL		m_dCrtX, m_dCrtY;
	DBL		m_dLimit;
private:
	void SetVect();
	int PointContinueChk(DBL x, DBL y);
	int CreateVect();
	int LineLoad(FILE *fp);
	int ArcLoad(FILE *fp);
	int CircleLoad(FILE *fp);
	int PolyLineLoad(FILE *fp);
	int TextLoad(FILE *fp);
	LPTSTR FDxfFGets(LPTSTR retbuf, int len, FILE *fp);
};

