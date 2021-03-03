#ifndef __FILEEPS_H
#define __FILEEPS_H

#define	GRATBLMAX	128
#define	GRAPOINTMAX	128
#define QCLIPTBLMAX	32

class CFileEps : public CCmdBase
{
public:
	CFileEps(CScrollView* pcview);
	void SaveData(LPSTR filename);
	void LoadData(LPSTR fname);
	CData *LoadDataNotAdd(LPSTR fname);
	void GrupReset();
public:
	CDataGrup*	m_pcTopDataGrup;
	DBL		m_dBindSX, m_dBindSY;
	DBL		m_dBindEX, m_dBindEY;
	DBL		m_dPaperXSize, m_dPaperYSize;
	DBL		m_dPaperXMM, m_dPaperYMM;
	DBL		m_dOffsetX, m_dOffsetY;
private:
	// グラデーションパターンテーブル
	char	m_szGraName[GRATBLMAX][RECMAX];
	long	m_lGraSeekPoint[GRATBLMAX];
	int		m_nGraTblMax;
	COLORREF	m_clrGra[GRAPOINTMAX];
	DBL		m_dCGra[GRAPOINTMAX];
	DBL		m_dPGra[GRAPOINTMAX];
	int		m_nPcnt;
	int		m_nGraKind;

	// BMP使用
	FILE*	m_pFile;
	long	m_lFileSize;
	long	m_lBitsSize;
	HANDLE		m_hBits;
	LPBYTE		m_lpBits;
	LPBYTE		m_lpCrtBits;
	long		m_lXByte;

	// ｸﾘｯﾋﾟﾝｸﾞで使用
	CRgn		m_crgnClipClip;
	long	m_lClipFileSize;
	long	m_lClipBitsSize;
	HANDLE		m_hClipBits;
	LPBYTE		m_lpClipBits;
	long		m_lClipXByte;
	CBitmap		m_cbmpClipCBitmap;
	CDC			m_cdcClipMemCDC;
	int			m_nClipBmpStat;
	CBitmap		*m_pcClipBackbmp;
	// ｸﾞﾗﾃﾞｰｼｮﾝで使用
	CRgn		m_crgnGraClip;
	long	m_lGraFileSize;
	long	m_lGraBitsSize;
	HANDLE		m_hGraBits;
	LPBYTE		m_lpGraBits;
	long		m_lGraXByte;
	CBitmap		m_cbmpGraCBitmap;
	CDC			m_cdcGraMemCDC;
	int			m_nGraBmpStat;
	CBitmap		*m_pcGraBackbmp;

	int		m_nRound;
	int		m_nXDotSize, m_nYDotSize, m_nByte;
	int		m_nRGBCrt;
	int		m_nXCrt;
	int		m_nLineCrt;
	char	m_szRGB[3];

	int			m_nPassFlag;
	CDataGrup*	m_pcCrtDataGrup;
	int		m_nQIdx;
	CDataGrup*	m_pcQClipGrup[QCLIPTBLMAX];
	CDataGrup*	m_pcQDataGrup[QCLIPTBLMAX];
	CClipData*	m_pcQClipData[QCLIPTBLMAX];
	CDataList*	m_pcCrtDataList;
	CVect*		m_pcCrtVect;
	CClipData*	m_pcCrtClipData;
	CDataGrup*	m_pcCrtClipGrup;
private:	//ｼｽﾃﾑ関係属性 
	int			m_nFillPenno;
	int			m_nOutlinePenno;
	DBL			m_dOutlineThick;
private:	//VECT関係属性
	int			m_nPP;
	DBL			m_dCrtx, m_dCrty;
private:	//TEXT関係属性
	DBL			m_dBaseX, m_dBaseY;
	DBL			m_dTopX, m_dTopY;
	char		m_szFont[RECMAX];
	char		m_szString[RECMAX];
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
	void ClrRGBTrns(DBL r, DBL g, DBL b, int *rval, int *gval, int *bval);
	void GetLPBits();
	COLORREF GetSetGraClr(DBL par);
	void SetGraClrTable(FILE *fp, LPSTR graname);
	long GetSeekpoint(LPSTR graname);
	void CreateGraPattren(FILE *fp, LPSTR graname, DBL sx, DBL sy, DBL rag, DBL vlen);
	void GetGraElement(LPSTR prm, DBL *sx, DBL *sy, DBL *rag, DBL *vlen);
	LPSTR SetGraName(LPSTR cmdstr, LPSTR graname);
	void EpsGraCommandLoop(FILE *fp, LPSTR cmdstr);
	void CreateGraPattern(FILE *fp, DBL sx, DBL sy, DBL rag, DBL vlen);
	void CreateBmpFile(LPSTR bmpfname);
	void SetAddDataGrup();
	void SetAddClipGrup();
	void MemDCInit();
	void MemDCEnd();
	void SetDibCData(CDataFDib *pcDataFDib, CDataGrup *pcDataGrup);

	void SkipEpsHead(FILE *fp);
	void EpsLoadLoop(FILE *fp);
	void EpsCommandLoop(FILE *fp, LPSTR buffer);
	LPSTR GetEpsCommandString(LPSTR cmdstr, LPSTR sp);
	void EpsPlgnCommand(LPSTR cmdstr);
	void EpsClipCommand(LPSTR cmdstr);
	void EpsEnvCommand(LPSTR cmdstr);
	void EpsClrCommand(LPSTR cmdstr);
	void EpsStrgCommand(LPSTR cmdstr);
	void EpsPassCommand(LPSTR cmdstr);
	void AnyStringCreate(LPSTR strg);
	void StringCreate(LPSTR strg);
	void GetCodeToStrg(LPSTR cmdstr, LPSTR strg);
	int CheckNumStrg(LPSTR str, int len);
	void SetVectData(CVect *pcVect, int pp);
	BOOL EPSCrDelFgets(LPSTR buf, int max, FILE *fp);
	void EpsBmpCreate(FILE *fp);
	void EpsBmpFileBitSet(UINT tt);
	void EpsBmpFileInit(LPSTR fname, int xsize, int ysize);
	void EpsBmpFileEnd();
	void TrnsPointToMMXY(LPDBL x, LPDBL y);
	void TrnsPointToMMLen(LPDBL len);
private:
};

#endif
