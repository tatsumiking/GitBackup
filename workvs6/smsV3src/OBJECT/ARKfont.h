
#ifndef __ARKFONT_H
#define __ARKFONT_H               // Prevent multiple includes

#include	"fsf1font.h"

#define	ARKHEADBYTE	256
#define	CHARHEAD	24

#define	MAXKU		128
#define	FT1IDXSIZE	10	// char char long long

#define ARKMESH		10000.0

#define	HMOVETO		0x00
#define	HLINETO		0x01
#define	HARCTO		0x02
#define	HARCEND		0x03
#define	HENDOF		0x03

typedef	struct {
	UCHAR	ku, ten;
	ULONG	seekp;
	ULONG	useekp;
} arkidxtype;

typedef struct {
	short	x1, y1;				// 横書き移動値
	short	x2, y2;				// 横書きボディーサイズ
	short	x3, y3;				// 縦書き移動値
	short	x4, y4;				// 縦書きボディーサイズ
	short	x5, y5;
	short	x6, y6;
} arkhtype;

typedef	struct {
	USHORT	x;
	USHORT	y;
} arkpointype;

class AFX_CLASS_EXPORT CARKFont : public CObject
{
protected:
	CARKFont();
	DECLARE_SERIAL(CARKFont)
public:
	CARKFont(int nDummy);
	~CARKFont();
	virtual void Serialize(CArchive& ar);
private:
	FILE		*m_fileARK;
	CFSF1Font	*m_pFSF1Font;
	BYTE	m_bFBuf[4096];
	int		m_nByteAdd;
	int		m_nDataLen;
	int		m_nType;
	int		m_nTateYoko;
	int		m_nSlope;
	int		m_nBold;
	char	m_sFaceName[RECMAX];
	char	m_sBaseFname[FNAMEMAX];
	DBL		m_dMoveX, m_dMoveY;
	DBL		m_dTime;
	DBL		m_dSX, m_dSY;
	DBL		m_dXTime, m_dYTime;
	DBL		m_dSin, m_dCos;

public:
	void SetSlope(int flag);
	void SetBold(int flag);
	void DrawOut(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL rag);
	void DrawOutMove(CDC *pcDC, DBL x, DBL y);
	void DrawOutLine(CDC *pcDC, DBL x1, DBL y1, DBL x2, DBL y2);
	void DrawOutArc(CDC *pcDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void TrnsElementInit(DBL sx, DBL sy, DBL wd, DBL hi, DBL rag);
	void TrnsXY(LPDBL x, LPDBL y);
	int DataSave(UINT sjis);
	void TrnsCopyFSF1FBuf();
	void FaceNameSet(LPSTR name);
	int FontIdxInit();
	void SF1TrnsDataSave(UINT sjis);
	void FSF1ClassSet(CFSF1Font* pFSF1Font);
	void FontNameSet(LPSTR pFont, int type);
	void SetTateYoko(int flag);
	int SF1TrnsDataLoad(UINT sjis);
	int SF1TrnsDataLoadNON(UINT sjis);
	int DataLoad(UINT sjis);
	int ArcDataLoad(int ku, int ten);
	int ArcDataSave(int ku, int ten);
	int FontOpen(int ku, int ten);
	int FontSaveOpen(int ku, int ten);
	void FontClose();
	void ArkIdxSeekPointGet(int ku, int ten, long *seekp);
	void ArkDataLenSeekPointGet(arkidxtype arkidx, long *dseekp);
	void ArkDataLenSeekPointSet(arkidxtype *arkidx, long dseekp);
	void TrnsARKToFSF1();
	void TrnsARKToFSF1NON();
	void AtrDXYGet(arkpointype dbuf[], int pp, int *atr, DBL *x, DBL *y);
	int ArcTrnsBezeSet(int bp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	int ArcSet(int bp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);

	int TrnsCopyRejionFSF1FBuf(BYTE fbuf[], int pcnt, int dp);
	int EofSet(arkpointype dbuf[], int dp);
	int MoveToSet(arkpointype dbuf[], int dp, DBL x1, DBL y1);
	int EorSet(arkpointype dbuf[], int dp);
	int LineToSet(arkpointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2);
	int BezeToArcToSet(arkpointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	int ArcToSet(arkpointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void AtrDXYSet(arkpointype dbuf[], int dp, int mode, DBL x, DBL y);
};

#endif

