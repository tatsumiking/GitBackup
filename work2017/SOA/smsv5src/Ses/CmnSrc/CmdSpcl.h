
#pragma once


//C 特殊処理（影付けなど）関係コマンド処理クラス
class CCmdSpcl : public CCmdBase
{
public:
	CCmdSpcl(CScrollView* pcview);

public:
	DBL m_dRoundRag;
	DBL m_dDistance;
	int m_nKind;
	DBL m_dOutLineMM;
	int	m_nOutLineKind;
	DBL m_dBsx, m_dBsy;
	DBL m_dBxmm, m_dBymm;
	DBL m_dBtime;
	DBL	m_dCntrX, m_dCntrY;
	DBL	m_dTrnsTime;
	int	m_nBMode;

	CDataList*	m_pcDataList;
	CVect*	m_pcVect;
	int		m_nPcnt;
	int		m_nXSize, m_nYSize;
	int		m_nXByte;
	DBL		m_dFureLimit;
	long	m_lBitsSize;
	CDC		*m_pMemDC;
	HANDLE	m_hBits, m_hTBits;
	BYTE	*m_lpBits, *m_lpTBits;
	CBitmap	*m_pBitmap, *m_oldBitmap;
	LPBYTE	m_lpCusTbl;
	XYType	*m_lpXYTbl1, *m_lpXYTbl2;
	TCHAR	m_szTitle[RECMAX];
private:
	DBL		m_dRltvTopx, m_dRltvTopy;
public:
	void BitmapFileOut();
	UINT ExecOutLineOmit(UINT event, DBL dx, DBL dy);
	UINT ExecShadow(UINT event, DBL dx, DBL dy);
	UINT ExecOutLine(UINT event, DBL dx, DBL dy);
	UINT ExecMarge(UINT event, DBL dx, DBL dy);
	UINT ExecRuleOutLine(UINT event, DBL dx, DBL dy);
	UINT ExecAreaSlct(UINT event, DBL dx, DBL dy);
	UINT ExecBezeToArc(UINT event, DBL dx, DBL dy);
	int DialogOutLineInit();
	void OutLineDataDraw();
	void OutLineGetDMiniMax(LPDBL minix, LPDBL miniy, LPDBL maxx, LPDBL maxy);
	void OutLineDataTrns();
	void OutLineMain();
	void RuleOutLineMain();

	int DialogShadowInit();
	void ShadowGetDMiniMax(LPDBL minix, LPDBL miniy, LPDBL maxx, LPDBL maxy);
	void ShadowDataTrns();
	void ShadowMain();
	void ShadowDataTrns1();
	void ShadowProg1();
	void ShadowDataTrns2();
	void ShadowProg2();
	void ShadowDataTrns3();
	void ShadowProg3();
	void ShadowDataTrns4();
	void ShadowProg4();

	void AreaSlctTrace(DBL dx, DBL dy);

	void BitMapInit();
	void BitMapTraceMain();
	void MemoryEnd();
	void MemoryInit(DBL xytime);
	void ActiveObjectCopy();
	void DrawAreaEnd();
	void DrawAreaInit(DBL minix, DBL miniy, DBL maxx, DBL maxy);
	int BitOnOffCheck(BYTE *lpBits, int xbyte, int x, int y);
	void BitOnSet(BYTE *lpBits, int xbyte, int x, int y);
	void BitOffSet(BYTE *lpBits, int xbyte, int x, int y);
	void BmpFileOut(LPTSTR fname, BYTE far *lpBits, int xbyte, int xsize, int ysize);
private:
	void BitMapOutLineSet(BYTE *lpBits, BYTE *lpTBits, int xbyte, int xsize, int ysize);
	void BitMapTrace(BYTE *lpBits, BYTE *lpTBits, int xbyte, int xsize, int ysize);
	void BitMapTarceSetPlgn(BYTE *lpBits, BYTE *lpTBits, int scus, int xbyte, int x, int y);
	void NextCusGet(BYTE *lpTBits, int xbyte, int *retx, int *rety, int *retcus);
	int FirstCusGet(BYTE *lpBits, int scus, int xbyte, int x, int y);
	void CusAddGet(int cus, int *addx, int *addy);
	int CusLegal(int cus);
	void SetDataListPolygon(XYType *lpXYTbl1, XYType *lpXYTbl2, int points);
	void SetXYTblSetPlgn(BYTE *lpTBits, int xbyte, int idx, int tsx, int tsy);
	void SetPlgnEnd();
	void SetPlgnInit();
	void PlgnPointSet(XYType *lpXYTbl1, int points, int tx, int ty);
	void PlgnLineToArcRejionSet(CVect *pcVect);
	int PlgnCurveChk(CVect *pcVect, int pp1, DBL *retx, DBL *rety, int *retpp);
	int PlgnCrclDastance(DBL tx, DBL ty, DBL x0, DBL y0, DBL r, DBL flimit);
	void BezeToArcMain(CVect *pcVect);
	int BezeToArcFigu(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4, CVect *pcVect, int dp);
};

