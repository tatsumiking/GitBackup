
#pragma once

#define		VARSIZE		16
#define		FREJIONMASK		0x10
#define		FEORMASK		0x01

#define	MINIMAXTIME		8

#define	FSF1POINTMAX	4096

#define	FSF1MESH		1024
#define	FSF1ATRADD		0
#define	FSF1XCDADD		1
#define	FSF1YCDADD		3
#define	FSF1SIZADD		5

typedef struct {
	TCHAR	version[VARSIZE];
	long	idxsseekp;
	long	idxeseekp;
	long	datsseekp;
	long	dateseekp;
} f1fhtype;

typedef struct {
	long	dseekp;
	long	dlen;
} f1idxtype;

typedef struct {
	short	headlen;
	short	datalen;
	TCHAR	sx, sy;
	TCHAR	ex, ey;
} f1dhtype;

#define	FSF1FONTMAX		10000

#define FSF1ISEEKP	0x00000020L
#define FSF1DSEEKP	0x000138A0L

//#define	FLIBDATATOP	0x00003ea0L	/* 2000(文字) * 8 + 32(f1fhtype) byte */
#define	FLIBDATATOP	0x000138A0L	/* 10000(文字) * 8 + 32(f1fhtype) byte */
#define MOJIFLAGMAX 1258 /* 10000(文字)/8 byte + 8 */

class AFX_CLASS_EXPORT CFSF1Font : public CObject
{
protected:
	CFSF1Font();
	DECLARE_SERIAL(CFSF1Font)
public:
	virtual void Serialize(CArchive& ar);
	//BYTE		m_bFBuf[(long)(FSF1POINTMAX)*(long)FSF1SIZADD];
	// 2000.06.01
	BYTE		m_bFBuf[(long)(FSF1POINTMAX+16)*(long)FSF1SIZADD];
	int			m_nDataLen;
	int			m_fRevercex, m_fRevercey;
	// プロポーショナルおよびボールド処理用の変数
	CDC		*m_pcMemDC;
	CBitmap	*m_pcBitmap, *m_pcoldBitmap;
	HANDLE	m_hDIB;
	HANDLE	m_hTDIB;
	LPBYTE	m_lpBits;
	LPBYTE	m_lpTBits;
	int		m_nXByte;
	long	m_dwSize;
	CDC		*m_pcBoldMemDC;
	CBitmap	*m_pcBoldBitmap, *m_pcoldBoldBitmap;
	HANDLE	m_hBoldDIB;
	HANDLE	m_hBoldTDIB;
	LPBYTE	m_lpBoldBits;
	LPBYTE	m_lpBoldTBits;
	int		m_nBoldXByte;
	long	m_dwBoldSize;

	DBL		m_dSX, m_dSY;
	DBL		m_dXTime, m_dYTime;
	DBL		m_dSin,	m_dCos;
	int		m_nTateYoko;
	int		m_nSlope;
	int		m_nBold;

private:
	TCHAR		m_sFontfile[FNAMEMAX];
	BYTE		m_fFontUse[MOJIFLAGMAX+8];
	int			m_nUseMaxIdx;
	int			m_fFlag;
	int			m_nDataSx, m_nDataSy;
	int			m_nDataEx, m_nDataEy;
	FILE		*m_fileFont, *m_fileDFont;
	DBL			m_dStartX, m_dStartY;
	DBL			m_dWidth, m_dHight;
	DBL			m_dCrtX, m_dCrtY;
	int			m_nCrtPP;
	int			m_nDataNo;

public:
	CFSF1Font(int nDummy);
	~CFSF1Font();

public:
	void OpenFileInitTable();
	void OpenFileInitTable0();
	void OpenFileInitTable1();
	void OpenFileInitTable2();
	void OpenFileInitTable3();
	void OpenFileInitTable4();
	void OpenFileInitTableMain();
	void ReOpen();
	void TrnsHanKaku();
	int CheckTateYoko();
	void SetTateYoko(int flag);
	void CloseFile();
	void FileNullSet();
	int DFileReadOpen(LPTSTR fname);
	void DFileWriteOpen(LPTSTR fname);
	int DFileClose();
	void DFileNullSet(LPTSTR fname);
	int  GetNextDataNo();
	void DataNoInc();
	int DFileDataLoad(int datano);
	void DFileDataSave(int datano);
	int Create();
	void Free(int nFidx);
	LPBYTE FBufAdrsGet();
	void GetMojiMiniMax(int nFidx, int w, int h, int *minix, int *miniy, int *maxx, int *maxy);
	int Copy(int nFidx);
	void DataMiniMaxRenew(int nFidx);
	void DataSave(int nFidx);
	void DataLoad(int nFidx);
	void PointMiniMaxGet();
	void PointSetStart();
	void MovePointSet(DBL dx, DBL dy);
	void LinePointSet(DBL x1, DBL y1, DBL x2, DBL y2);
	void CurvePointSet(DBL x1, DBL y1, DBL x2, DBL y2);
	void BezePointSet(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	void CrtContAtrOff();
	void CurveToStartPointSet();
	void CurveToPointSet(DBL x, DBL y);
	void CurveToEndPointSet(DBL x, DBL y);
	void RejionSet();
	void NoRejionSet();
	void PointSetEnd();
	void GetWakuDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	void GetDataDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	void TrnsPlotOut(int nComID, DBL sx, DBL sy, DBL nW, DBL nH, int nFidx);
	void TrnsFillDraw(CDC *pcDC, DBL sx, DBL sy, DBL nW, DBL nH, int nFidx);
	void TrnsPlgnDraw(CDC *pcDC, DBL sx, DBL sy, DBL nW, DBL nH, int nFidx);
	void TrnsElementSet(DBL sx, DBL sy, DBL nW, DBL nH);
	void FontScaleTrns(DBL dx, DBL dy, DBL *retdx, DBL *retdy);
	void FBufScaleTrns(DBL dx, DBL dy, DBL *retdx, DBL *retdy);
	int RejionEndGet(int pcnt, int sp);
	int PointLegal(int pcnt, int pp);
	int EorChk(int pp);
	void EorOn(int pp);
	int RejionChk(int pp);
	void RejionOn(int pp);
	void RejionOff(int pp);
	void AtrDXYCrtSet(int atr, DBL x, DBL y);
	void CrtPointInc();
	void AtrDXYGet(BYTE fBuf[], int pp, int *atr, double *x, double *y);
	void AtrDXYSet(BYTE fBuf[], int pp, int atr, double x, double y);
	void DXYGet(BYTE fBuf[], int pp, double *x, double *y);
	void DXYSet(BYTE fBuf[], int pp, double x, double y);
	void AtrGet(BYTE fBuf[], int pp, int *atr);
	void AtrSet(BYTE fBuf[], int pp, int atr);
	int PointCountGet();
	void IdxSeekPointGet(int nFidx, long *idxseekp);
	void DataLenSeekPointGet(f1idxtype f1idx, int *datlen, long *datseekp);
	void DataLenSeekPointSet(f1idxtype *f1idx, int datlen, long datseekp);

	void DataPlotOut(int nComID);
	void RejionPlotOut(int nComID, BYTE fbuf[], int pcnt);
	void RejionPlotOutOmit(int nComID, BYTE fbuf[], int pcnt);
	void NoRejionPlotOut(int nComID, BYTE fbuf[], int pcnt);
	void NoRejionPlotOutOmit(int nComID, BYTE fbuf[], int pcnt);

	void FillDrawOut(CDC *pcDC);
	void FillRejionDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt);

	void PlgnDrawOut(CDC *pcDC);
	void PlgnRejionDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt);
	void PlgnNoRejionDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt);
	void PlgnRejionConrDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt, int nthick);
	void PlgnNoRejionConrDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt, int nthick);
	void GetRoundRag(DBL x0, DBL y0, DBL x1, DBL y1, DBL *rag);
	void SetRoundPoint(DBL x0, DBL y0, DBL rag, DBL x1, DBL y1, DBL *x, DBL *y);
	void GetConerPoint(DBL x1, DBL y1, DBL rag1, DBL x2, DBL y2, DBL rag2, DBL *x, DBL *y);
	// プロポーショナル処理用の関数
	void CreateDIBDC(CDC *pcDC);
	void CreateBoldDIBDC(CDC *pcDC);
	void DeleteDIBDC();
	void DeleteBoldDIBDC();
	void PlgnDrawDIBC(int nTateYako, int hFno, DBL sx, DBL sy, DBL timex, DBL timey);
	void GetPrePostTable(int pre[], int post[]);
	int PreFirstYOnBit(int x);
	int PostFirstYOnBit(int x);
	int PreFirstXOnBit(int y);
	int PostFirstXOnBit(int y);
	//太字生成処理関数
	void PreBoldTrns(int hFno, DBL par);
	void PostBoldTrns(int hFno, DBL par);
	void BmpFileOut(LPTSTR fname, int xsize, int ysize);

	void TrnsElementInit(DBL sx, DBL sy, DBL wd, DBL hi, DBL rag);
	void SetSlope(int flag);
	void SetBold(int flag);
	void FillDraw(CDC *pcDC);
	void PlgnDraw(CDC *pcDC);
	void RejionPlgnDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt);
	void NoRejionPlgnDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt);
	void RejionFillDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt);
	void TrnsXY(DBL *x, DBL *y);
};

