
#pragma once

class CFileEps
{
public:
	CFileEps(CView* pcview);
	void SaveData(LPTSTR filename);
	void LoadData(LPTSTR fname);
	void GrupReset();
public:
	CTonView*	m_pcView;
	CTonDoc*	m_pcDoc;
	CDataGrup*	m_pcDataGrup;
private:
	FILE*	m_pFile;
	long	m_lFileSize;
	long	m_lBitsSize;
	HANDLE		m_hBits;
	LPBYTE		m_lpBits;
	LPBYTE		m_lpCrtBits;
	int		m_nRound;
	int		m_nXDotSize, m_nYDotSize, m_nByte;
	int		m_nRGBCrt;
	int		m_nXCrt;
	int		m_nLineCrt;
	BYTE	m_szRGB[3];

	int			m_nPassFlag;
	CDataList*	m_pcDataList;
	CVect*		m_pcVect;
	CSPtnVect*	m_pcSPtnVect;
private:	//ºΩ√—ä÷åWëÆê´ 
	int		m_nOutLinePenno;
	int		m_nFillPenno;
	DBL		m_dOutLineThick;
private:	//VECTä÷åWëÆê´
	int			m_nPP;
	DBL			m_dCrtx, m_dCrty;
private:	//TEXTä÷åWëÆê´
	DBL			m_dTopX, m_dTopY;
	TCHAR		m_szFont[RECMAX];
	TCHAR		m_szString[RECMAX];
	int			m_nStrIdx;
	int			m_nTateYoko;
	int			m_nOder;
	int			m_nSetMode;
	DBL			m_dA1, m_dB1, m_dA2, m_dB2;
	DBL			m_dXX;
	DBL			m_dSX, m_dSY;
	DBL			m_dW, m_dH;
	DBL			m_dMS, m_dLS;
	DBL			m_dLXS, m_dLYS;
	DBL			m_dSPF;
	DBL			m_dFltPar;

public:
	void SkipEpsHead(FILE *fp);
	void EpsLoadLoop(FILE *fp);
	void EpsCommandLoop(LPTSTR buffer);
	LPTSTR GetEpsCommandString(LPTSTR cmdstr, LPTSTR sp);
	void PlgnEpsCommand(LPTSTR cmdstr);
	void EnvEpsCommand(LPTSTR cmdstr);
	void EpsClrCommand(LPTSTR cmdstr);
	void StrgEpsCommand(LPTSTR cmdstr);
	void PassEpsCommand(LPTSTR cmdstr);
	void AnyStringCreate(LPTSTR strg);
	void StringCreate(LPTSTR strg);
	void GetCodeToStrg(LPTSTR cmdstr, LPTSTR strg);
	int CheckNumStrg(LPTSTR str, int len);
	void SetVectData(CVect *pcVect, int pp);
	void EpsBmpCreate(FILE *fp);
	void EpsBmpFileBitSet(UINT tt);
	void EpsBmpFileInit(LPTSTR fname, int xsize, int ysize);
	void EpsBmpFileEnd();
	void TrnsPointToMMXY(LPDBL x, LPDBL y);
	void TrnsPointToMMLen(LPDBL len);
private:
};

