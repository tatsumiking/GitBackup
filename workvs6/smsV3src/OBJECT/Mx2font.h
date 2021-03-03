
#ifndef __MX2FONT_H
#define __MX2FONT_H               // Prevent multiple includes

#include	"fsf1font.h"

#define	MX2HEADBYTE	64
#define	CHARHEAD	24

#define	MX2KU		128
#define MX2IDXSIZE	8	// long long

#define MX2MESH		10000.0

#define	MOVETO		0
#define	LINETO		1
#define	ARCTO		2
#define	ARCEND		3
#define	ENDOF		-1

typedef	struct {
	ULONG	seekp;
	ULONG	useekp;
} mx2idxtype;

typedef struct {
	short	x1, y1;				// 横書き移動値
	short	x2, y2;				// 横書きボディーサイズ
	short	x3, y3;				// 縦書き移動値
	short	x4, y4;				// 縦書きボディーサイズ
} mx2htype;

typedef	struct {
	short	atr;
	short	x;
	short	y;
} mx2pointype;

class AFX_CLASS_EXPORT CMX2Font : public CObject
{
protected:
	CMX2Font();
	DECLARE_SERIAL(CMX2Font)
public:
	CMX2Font(int nDummy);
	~CMX2Font();
	virtual void Serialize(CArchive& ar);
private:
	FILE		*m_fileMX2;
	CFSF1Font	*m_pFSF1Font;
	//HANDLE	m_hFBuf;
	//BYTE		m_lpFBuf;
	BYTE		m_bFBuf[4096];
	int			m_nDataLen;
	int			m_nTateYoko;
	char		m_sFaceName[RECMAX];
	char		m_sBaseFname[FNAMEMAX];
	DBL			m_dMoveX, m_dMoveY;
	DBL			m_dTime;

public:
	int DataSave(UINT sjis);
	void TrnsCopyFSF1FBuf();
	void FaceNameSet(LPSTR name);
	int FontIdxInit();
	void SF1TrnsDataSave(UINT sjis);
	void FSF1ClassSet(CFSF1Font* pFSF1Font);
	void FontNameSet(LPSTR pFont);
	void SetTateYoko(int flag);
	int SF1TrnsDataLoad(UINT sjis);
	int SF1TrnsDataLoadNON(UINT sjis);
	int DataLoad(UINT sjis);
	int DataLoad(int ku, int ten);
	int DataSave(int ku, int ten);
	int FontOpen(int ku, int ten);
	int FontSaveOpen(int ku, int ten);
	void FontClose();
	void IdxSeekPointGet(int ku, int ten, long *seekp);
	void DataLenSeekPointGet(mx2idxtype mx2idx, long *dseekp);
	void DataLenSeekPointSet(mx2idxtype *mx2idx, long dseekp);
	void TrnsMX2ToFSF1();
	void TrnsMX2ToFSF1NON();
	void AtrDXYGet(mx2pointype dbuf[], int pp, int *atr, DBL *x, DBL *y);
	int ArcTrnsBezeSet(int bp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	int ArcSet(int bp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);

	int TrnsCopyRejionFSF1FBuf(BYTE fbuf[], int pcnt, int dp);
	int EofSet(mx2pointype dbuf[], int dp);
	int MoveToSet(mx2pointype dbuf[], int dp, DBL x1, DBL y1);
	int EorSet(mx2pointype dbuf[], int dp);
	int LineToSet(mx2pointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2);
	int BezeToArcToSet(mx2pointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	int ArcToSet(mx2pointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void AtrDXYSet(mx2pointype dbuf[], int dp, int mode, DBL x, DBL y);
};

#endif

