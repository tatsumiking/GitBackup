
#ifndef __ESF1FONT_H
#define __ESF1FONT_H               // Prevent multiple includes

#include	"fsf1font.h"

#define		VARSIZE		16
#define		FREJIONMASK		0x10
#define		FEORMASK		0x01

typedef struct {
	char	version[VARSIZE];
	long	idxsseekp;
	long	idxeseekp;
	long	datsseekp;
	long	dateseekp;
} e1fhtype;

typedef struct {
	long	dseekp;
	long	dlen;
} e1idxtype;

typedef struct {
	short	headlen;
	short	datalen;
	char	sx, sy;
	char	ex, ey;
} e1dhtype;

#define	DFILEDSEEKP	0x00003ea0L	/* 2000(•¶Žš) * 8 + 32(f1fhtype) byte */
#define ESF1ISEEKP	0x00000020L
#define ESF1DSEEKP	0x000138A0L
#define ESF1PRTCT	0x94E0L	/* (47*100 + 60) * 8 + 32 */
#define	ESG1KUTOP	100

#define	MINIMAXTIME		8
#define	ESF1MESH		1024
#define	ESF1ATRADD		0
#define	ESF1XCDADD		1
#define	ESF1YCDADD		3
#define	ESF1SIZADD		5
#define	ESF1POINTMAX	1000

#define		ESF1REJIONMASK		0x10
#define		ESF1EORMASK		0x01

class AFX_CLASS_EXPORT CESF1Font : public CObject
{
protected:
	CESF1Font();
	DECLARE_SERIAL(CESF1Font)
public:
	virtual void Serialize(CArchive& ar);
private:
	FILE		*m_fileSF1;
	CFSF1Font	*m_pFSF1Font;
	BYTE		m_bFBuf[(long)ESF1POINTMAX*(long)ESF1SIZADD];
	int			m_nDataLen;
	int			m_nTateYoko;
	int			m_nBold;
	int			m_nSlope;
	char		m_sBaseFname[FNAMEMAX];

	DBL		m_dSX, m_dSY;
	DBL		m_dXTime, m_dYTime;
	DBL		m_dSin, m_dCos;

public:
	void SF1TrnsDataSave(UINT sjis);
	void DFileClose();
	void DFileDataLenSeekPointGet(e1idxtype f1idx, int *dlen, long *dseekp);
	void DFileDataLoad(int datano);
	void DFileLoadSF1Trns(int datano);
	int DFileReadOpen(char basefname[]);
	void TrnsCopyFSF1FBuf();
	CESF1Font(int nDummy);
	~CESF1Font();
	LPBYTE GetbFBufAdrs();
	int FontMeshGet();
	void FSF1ClassSet(CFSF1Font* pFSF1Font);
	void FontNameSet(LPSTR pFont);
	void SetTateYoko(int flag);
	int SF1TrnsDataLoad(UINT sjis);
	int SF1TrnsGaijiDataLoad(UINT sjis);

	int DataLoad(UINT sjis);
	int GaijiDataLoad(UINT sjis);
	int UserDataSave(UINT sjis);
	int UserDataExist(UINT sjis);
	int UserDataDelete(UINT sjis);
	int UserDataLoad(int ku, int ten);
	int EsysDataLoad(int ku, int ten);
	int UserFontWriteOpen(int ku, int ten);
	int UserFontOpen(int ku, int ten);
	int EsysFontOpen(int ku, int ten);
	void FontClose();
	int UserFontNullSet();
	void IdxSeekPointGet(int ku, int ten, long *seekp);
	void DataLenSeekPointGet(e1idxtype f1idx, int *dlen, long *dseekp);
	void DataLenSeekPointSet(e1idxtype *f1idx, int dlen, long dseekp);
	void TateGakiTrns(UINT sjis);
	void TrnsESF1ToFSF1();
	void SetDataLen(int pp);
	void AtrDXYGet(BYTE fBuf[], int pp, int *atr, DBL *x, DBL *y);
	void AtrDXYSet(BYTE fBuf[], int pp, int atr, DBL x, DBL y);
	int EorChk(BYTE fBuf[], int pp);
	void EorOn(BYTE fBuf[], int pp);
	int RejionChk(BYTE fBuf[], int pp);
	void RejionOff(BYTE fBuf[], int pp);
	void RejionOn(BYTE fBuf[], int pp);

	void DrawOut(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL rag);
	void RejionDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt);
	void SetSlope(int flag);
	void SetBold(int flag);
	void TrnsElementInit(DBL sx, DBL sy, DBL wd, DBL hi, DBL rag);
	int RejionEndGet(int pcnt, int sp);
	int EorChk(int pp);
	void TrnsXY(DBL *x, DBL *y);
	int PointLegal(int pcnt, int pp);
};

#endif
