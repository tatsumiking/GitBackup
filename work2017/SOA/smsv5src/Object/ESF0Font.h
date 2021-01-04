
#pragma once

#include "FSF1Font.h"

#define	ESF0MESH	1024
#define	ESF0ATRADD		0
#define	ESF0XCDADD		1
#define	ESF0YCDADD		2
#define	ESF0SIZADD		3

#define	ESF0PRTCT	28562L

#define	ESF0LASTSJIS	0xf240	// Šg’£ŠOŽš 99‹æ01“_ JIS 8321

#define	F0EORMASK		0x01

typedef	struct {
	short	len;
	long	seekp;
} fidxtype;

class AFX_CLASS_EXPORT CESF0Font : public CObject
{
protected:
	CESF0Font();
	DECLARE_SERIAL(CESF0Font)
public:
	virtual void Serialize(CArchive& ar);
private:
	FILE		*m_fileIdx, *m_fileDat;
	CFSF1Font*	m_pFSF1Font;
	BYTE		m_bFBuf[3072];
	UINT		m_nDataLen;
	int			m_nTateYoko;
	int			m_nBold;
	int			m_nSlope;
	TCHAR		m_sBaseFname[FNAMEMAX];

	DBL		m_dSX, m_dSY;
	DBL		m_dXTime, m_dYTime;
	DBL		m_dSin, m_dCos;

public:
	void DFileLoadSF1Trns(int datano);
	void TrnsCopyFSF1FBuf();
	CESF0Font(int nDummy);
	~CESF0Font();
	LPBYTE GetbFBufAdrs();
	int FontMeshGet();
	void FSF1ClassSet(CFSF1Font *pFSF1Font);
	void FontNameSet(LPTSTR pFont);
	void SetTateYoko(int flag);
	int SF1TrnsDataLoad(int sjis);
	void SF1TrnsDataSave(int sjis);
	int DFileReadOpen(TCHAR basefname[]);
	void DFileDataLoad(int datano);
	void DFileClose();
	int FontNullSet();
	int UserFontNullSet();

	int DataLoad(int sjis);
	int UserDataSave(int sjis);
	int UserDataExist(int sjis);
	int UserDataDelete(int sjis);
	int UserDataLoad(int sjis);
	int EsysDataLoad(int sjis);
	int UserFontWriteOpen(int sjis);
	int UserFontOpen(int sjis);
	int EsysFontOpen(int sjis);
	void FontClose();
	int FontTblDatNullSet(TCHAR *lpIdxFname, TCHAR *lpDatFname);
	void GetSeekPoint(int sjis, long *seekp);
	void TateGakiTrns(int sjis);
	void TrnsESF0ToFSF1();
	void SetDataLen(int pp);
	void AtrDXYGet(BYTE fBuf[], int pp, int *atr, DBL *x, DBL *y);
	void AtrDXYSet(BYTE fBuf[], int pp, int atr, DBL x, DBL y);
	int EorChk(BYTE fBuf[], int pp);
	void EorOn(BYTE fBuf[], int pp);
	int RejionChk(BYTE fBuf[], int *pp);
	int RejionOff(BYTE fBuf[], int pp);
	int RejionOn(BYTE fBuf[], int pp);

	void DrawOut(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL rag);
	void RejionDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt);
	void NoRejionDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt);
	void SetSlope(int flag);
	void SetBold(int flag);
	void TrnsElementInit(DBL sx, DBL sy, DBL wd, DBL hi, DBL rag);
	int RejionEndGet(BYTE fbuf[], int pcnt, int sp);
	void TrnsXY(DBL *x, DBL *y);
	int PointLegal(int pcnt, int pp);
};

