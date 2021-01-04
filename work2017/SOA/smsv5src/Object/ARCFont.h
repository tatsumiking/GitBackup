
#pragma once

#include "FSF1Font.h"

#define	HEADBYTE	128
#define	TCHARHEAD	24

#define	MAXKU		128
#define	FT2IDXSIZE	10	// TCHAR TCHAR long long
#define MAXIDXSIZE	8	// long long

#define ARCMESH		10000.0

#define	MOVETO		0
#define	LINETO		1
#define	ARCTO		2
#define	ARCEND		3
#define	ENDOF		-1

typedef	struct {
	BYTE	ku, ten;
	ULONG	seekp;
	ULONG	useekp;
} arcidxtype;

typedef	struct {
	ULONG	seekp;
	ULONG	useekp;
} maxidxtype;

typedef struct {
	short	x1, y1;				// 横書き移動値
	short	x2, y2;				// 横書きボディーサイズ
	short	x3, y3;				// 縦書き移動値
	short	x4, y4;				// 縦書きボディーサイズ
} archtype;

typedef	struct {
	short	atr;
	short	x;
	short	y;
} arcpointype;

class AFX_CLASS_EXPORT CARCFont : public CObject
{
protected:
	CARCFont();
	DECLARE_SERIAL(CARCFont)
public:
	CARCFont(int nDummy);
	~CARCFont();
	virtual void Serialize(CArchive& ar);
private:
	FILE		*m_fileARC;
	CFSF1Font	*m_pFSF1Font;
	HANDLE	m_hFBuf;
	LPBYTE	m_lpFBuf;
	//BYTE	m_bFBuf[4096];
	int		m_nDataLen;
	int		m_nType;
	int		m_nTateYoko;
	int		m_nSlope;
	int		m_nBold;
	TCHAR	m_sFaceName[RECMAX];
	int		m_nNewType;
	TCHAR	m_sBaseFname[FNAMEMAX];
	DBL		m_dAddX, m_dAddY;
	DBL		m_dMoveX, m_dMoveY;
	DBL		m_dTime;
	DBL		m_dSX, m_dSY;
	DBL		m_dXTime, m_dYTime;
	DBL		m_dSin, m_dCos;
	DBL		m_dTrnsTimeX, m_dTrnsTimeY;

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
	void FaceNameSet(LPTSTR name);
	int FontIdxInit();
	void SF1TrnsDataSave(int sjis);
	void FSF1ClassSet(CFSF1Font* pFSF1Font);
	void FontNameSet(LPTSTR pFont, int type);
	void SetTateYoko(int flag);
	int SF1TrnsDataLoad(int sjis);
	int SF1TrnsDataLoadNON(int sjis);
	int DataLoad(int sjis);
	int ArcDataLoad(int ku, int ten);
	int ArcDataSave(int ku, int ten);
	int FontOpen(int ku, int ten);
	int FontSaveOpen(int ku, int ten);
	void FontClose();
	void ArcIdxSeekPointGet(int ku, int ten, long *seekp);
	void MaxIdxSeekPointGet(int ku, int ten, long *seekp);
	void ArcDataLenSeekPointGet(arcidxtype arcidx, long *dseekp);
	void MaxDataLenSeekPointGet(maxidxtype maxidx, long *dseekp);
	void ArcDataLenSeekPointSet(arcidxtype *arcidx, long dseekp);
	void MaxDataLenSeekPointSet(maxidxtype *maxidx, long dseekp);
	void TrnsARCToFSF1();
	void TrnsARCToFSF1NON();
	void AtrDXYGet(arcpointype dbuf[], int pp, int *atr, DBL *x, DBL *y);
	int ArcTrnsBezeSet(int bp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	int ArcSet(int bp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);

	int TrnsCopyRejionFSF1FBuf(BYTE fbuf[], int pcnt, int dp);
	int EofSet(arcpointype dbuf[], int dp);
	int MoveToSet(arcpointype dbuf[], int dp, DBL x1, DBL y1);
	int EorSet(arcpointype dbuf[], int dp);
	int LineToSet(arcpointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2);
	int BezeToArcToSet(arcpointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	int ArcToSet(arcpointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void AtrDXYSet(arcpointype dbuf[], int dp, int mode, DBL x, DBL y);
};

