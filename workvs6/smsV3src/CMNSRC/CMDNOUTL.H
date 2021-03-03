
#ifndef __CMDNOUTL_H
#define __CMDNOUTL_H

#include "crospint.h"
#include "dprogbar.h"

class CCmdOutLine : public CCmdBase
{
public:
	CDialogProgresBar *m_pcDialogProgresBar;
	CCmdOutLine(CScrollView* pcview);
	UINT ExecOmitOutLine(UINT event, DBL dx, DBL dy);
	UINT ExecOutLine(UINT event, DBL dx, DBL dy);
public:
	void StrgDataChange();
	int DialogOutLineInit();
	int CopyDataSetList(CDataList *pcDataList);
	void SetOutLine(CDataList *pcDataList);
private:
	int	SetOutlineVect(CVect *pcSrcVect, CVect *pcDstVect);
	int	SetOutlineFigu(int *kind1, dbezetype *bz1, dlinetype *ln1, DXYType tbl[], int *max);
	int	SetOutlineAcutFigu(int atr, DBL cx, DBL cy, CVect *pcDstVect, int dp);
	int	SetOutlineBezePoint(DBL mm, dbezetype *bz, DXYType tbl[], int *max);
	int	SetOutlinePlgnPoint(DBL mm, dbezetype *bz, DXYType tbl[]);
	int	SetOutlineLinePoint(DBL mm, dlinetype *ln);
	int	SetOutLineFiguSetPoint(int kind, int atr, dbezetype bz, dlinetype ln, DXYType tbl[], int max, CVect *pcVect, int dp);
	int	SetOutlineBezeFigu(CVect *pcVect, int atr, dbezetype bz1, int dp);
	int SetOutlinePlgnFigu(CVect *pcVect, DXYType tbl[], int max, int dp);
	int	SetOutlineLineFigu(CVect *pcVect, dlinetype ln1, int dp);
	int	SetOutLineLastPointLegal(CVect *pcVect, int dp);
	void SetOutlineCrossDivFigu(int kind1, dbezetype *bz1, dlinetype *ln1, int kind2, dbezetype *bz2, dlinetype *ln2);
	int SetOutlineCrossPointSetTbl(int kind1, dbezetype bz1, dlinetype ln1, int kind2, dbezetype bz2, dlinetype ln2, DBL xtbl[], DBL ytbl[]);
	int SetOutlineNearPoint(DBL dx, DBL dy, DBL xtbl[], DBL ytbl[], int cnt);
	
	int SetArcToBeze(CVect *pcVect, int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void CrossSetPoint(CDataList *pcDataList);
	void CrossSetPointVect(CVect *pcVect1, CVect *pcVect2);
	void GetCrossPointFigu(dlinetype ln1, dlinetype ln2);
	void GetCrossPointFigu(dbezetype bz1, dlinetype ln2);
	void GetCrossPointFigu(dlinetype ln1, dbezetype bz2);
	void GetCrossPointFigu(dbezetype bz1, dbezetype bz2);
	int AddCrossPoint(int atr1, dbezetype bz, CVect *pcVect, int dp);
	int AddCrossPoint(int atr1, dlinetype bz, CVect *pcVect, int dp);
	void AddCrossPoint(CVect *pcVect);
	void SortCrossPoint();
	int SetFigu(CVect *pcVect, int pp, dbezetype *bz, dlinetype *ln);
	int SetFiguOutline(DBL mm, CVect *pcVect, int pp, dbezetype *bz, dlinetype *ln);
	void DelSamePoint();
	void DelSamePoint(CDataList *pcDataList);
	void DrawLine(DBL x1, DBL y1, DBL x2, DBL y2, long clr);
	int CopyVect(CVect *pcTVect, CVect *pcVect, int dp);
	int OutLineTraceVect(CVect *pcVect, POSITION postbl[]);
	void CopyDivDataList(CDataList *pcDataList);
	void OutLineTrace(CDataList *pcDataList);
public:
	void DelNotConectVect();
	void SetRestoreCurveVect(CVect *pcVect, CVect *pcTempVect);
	void SetRestoreCurve(CDataList *pcDataList);
	void SetOnlyLineOrBeze(CDataList *pcDataList);
	int CheckStartEndPoint(DBL xx, DBL yy, DBL sx1, DBL sy1, DBL ex1, DBL ey1, DBL sx2, DBL sy2, DBL ex2, DBL ey2);
	int LegalVect(CVect *pcVect);
	void LegalDataList(CDataList *pcDataList);
	DBL GetLength(DBL x1, DBL y1, DBL x2, DBL y2);
	void GetRoundRag(DBL x0, DBL y0, DBL x1, DBL y1, DBL *rag);
	DBL GetSubRag(DBL rag1, DBL rag2);
	void SetRoundPoint(DBL x0, DBL y0, DBL rag, DBL x1, DBL y1, DBL *x, DBL *y);
	void DelNearPoint(CDataList *pcDataList);
	void SortPtrList();
	void GetDivRestVect(CDataList *pcDataList);
	int GetSinge(DBL singe);
	DBL m_dVectCount;
	int BitOnOffCheck(DBL dx, DBL dy);
	int CheckBitPointXAdd(dlinetype ln, int nx, int ny);
	int CheckBitPointYAdd(dlinetype ln, int nx, int ny);
	void RejionLegal(CDataList *pcDataList);
	int RejionLegalVectXAdd(CVect *pcVect);
	int RejionLegalVectYAdd(CVect *pcVect);
	void SetOnevectLoop(CDataList *pcDataList);
	void BitMapDrawInit(CDataList *pcDataList);
	void BitMapDraw(CDataList *pcDataList, DBL thick);
	void DelInpointVect();
	void BitMapDrawEnd();
	CTypedPtrList<CPtrList, CCrossPoint*> m_cCrsLst;
	CCmdSpcl *m_pcCmdSpcl;
	DBL	m_dOutLineMM;
	int m_nOutLineKind;
	CVect	*m_pcVect1;
	CVect	*m_pcVect2;
	int		m_nPP1;
	int		m_nPP2;
	int		m_nKind1, m_nKind2;
	dbezetype	m_sBz1, m_sBz2;
	dlinetype	m_sLn1, m_sLn2;
	DXYType		m_sTbl1[12], m_sTbl2[12];
	int		m_nMax1, m_nMax2;
};

#endif
