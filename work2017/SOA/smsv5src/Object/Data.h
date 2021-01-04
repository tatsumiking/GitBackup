#pragma once

#include "FSF1Font.h"
#include "WinTTFont.h"
#include "ESF0Font.h"
#include "ESF1Font.h"
#include "FBU0Font.h"
#include "MX2Font.h"
#include "ARCFont.h"
#include "ARKFont.h"
#include "SCRFont.h"

#include "GdiPlusEx.h"

class CData;
class CVect;
class CDataGrup;

typedef struct {
	double	xx, yy;
	double	len;
	CData	*pcData;
	int		pp;
} dcrostype;

class AFX_CLASS_EXPORT CData : public CObject
{
protected:
	CData();
	DECLARE_SERIAL(CData)
public:
	virtual void Serialize(CArchive& ar);
public:
	CData*	m_pcDataNext;
	CRect	m_rectData;
	int		m_nFlag;
	DBL		m_dMinix, m_dMiniy;
	DBL		m_dMaxx, m_dMaxy;
	CWnd	*m_pcWnd;
public:
	CFSF1Font*	m_pcFSF1Font;
	CWinTTFont*	m_pcWNTTFont;
	CESF0Font*	m_pcESF0Font;
	CESF1Font*	m_pcESF1Font;
	CFBU0Font*	m_pcFBU0Font;
	CMX2Font*	m_pcMX2Font;
	CARCFont*	m_pcARCFont;
	CARKFont*	m_pcARKFont;
	CSCRFont*	m_pcSCRFont;

public:
	DBL GetSrchNearMM();
	virtual void ResetVectLink();
	virtual void CnvBezeToLine(int divcnt);
	virtual void SetUseClrPenno(int notbl[], LPINT cnt);
	virtual int CheckOneMojiTrns();
	virtual void ReverceRejion();
	virtual int CheckNullData();
	virtual void OutLineOmit(DBL mm);
	virtual void SaveDxfFile();
	virtual void SaveEpsFile();
	virtual void OutlineDraw(CDC *pcDC);
	virtual void PlgnDraw(CDC *pcDC);
	virtual void FillDraw(CDC *pcDC);
	virtual void GdiplusFillDraw(CGdiPlusEx *pcGpe);
	virtual void GdiplusPlgnDraw(CGdiPlusEx *pcGpe);
	virtual void OnDraw(CDC *pcDC);
	virtual void PLDraw(CDC *pcDC);
	virtual void SetDefFillClr();
	virtual void SetDefOutLineClr();
	virtual int SetData(CData* pcData);
	virtual CData * SrchMoji(DBL x, DBL y, DBL *len, int *linkno);
	virtual int GetNearOnlinePoint(DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL *retlen, DBL *retx, DBL *rety);
	virtual DBL GetPreLengthBoxs(DBL x, DBL y, DBL sx, DBL sy, DBL ex, DBL ey);
	virtual void PointPicup();
	virtual void PointXLegal(DBL xx);
	virtual void PointYLegal(DBL yy);
	virtual void NoSelectDraw(CDC *pcDC);
	virtual void NoSelectDraw(CDC *pcDC, int no);
	virtual void SelectDraw(CDC *pcDC);
	virtual void SelectDraw(CDC *pcDC, int no);
	virtual int DrawNo(CDC *pcDC, int noidx);
	virtual void DrawSekijiNo(CDC *pcDC);
	virtual CData * CopyData(DBL movex, DBL movey);
	CData(int nDummy);
	~CData();
	virtual void SaveStrgStoreFile(FILE *fp, int id);
	virtual void LoadStrgStoreFile(FILE *fp, int id);
	virtual int CheckID();
	virtual void BaseFontLink(CFSF1Font *pFSF1Font);
	virtual void WNTTFontLink(CWinTTFont *pWNTTFont);
	virtual void ESF0FontLink(CESF0Font *pESF0Font);
	virtual void ESF1FontLink(CESF1Font *pESF1Font);
	virtual void FBU0FontLink(CFBU0Font *pFBU0Font);
	virtual void ARCFontLink(CARCFont *pARCFont);
	virtual void ARKFontLink(CARKFont *pARKFont);
	virtual void MX2FontLink(CMX2Font *pMX2Font);
	virtual void SCRFontLink(CSCRFont *pSCRFont);
	virtual CData* NewDataCreate(int nID);
	virtual void FlagOnDelData(int flag);
	virtual void DeleteData(CData *pData);
	virtual CData* SrchNearPoint(DBL xx, DBL yy, DBL *retlen, int *retpp);
	virtual CData *SrchNearOnLinePoint(DBL xx, DBL yy, DBL *retlen, DBL *retx, DBL *rety, int *retpp);
	virtual void SetAtnVectFSF1fBuf(CFSF1Font *pcFSF1Font);
	virtual void InAreaDataAtntionOn(DBL sx, DBL sy, DBL ex, DBL ey);
	virtual void WakuLoad(FILE *fp, LPTSTR buf);
	virtual void WakuSave(FILE *fp);
	virtual int GetOutLinePen();
	virtual int GetFillPen();
	virtual DBL GetOutLineWidth();
	virtual void SetNext(CData *pData);
	virtual CData* GetNext();
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void GetMojiWakuMiniMax(int mojiidx, DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void RenewMiniMax();
	virtual void MoveTrns(DBL movex, DBL movey);
	virtual void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual void SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy);
	virtual void MirrorTrns(DBL a, DBL b, DBL t1, DBL t2);
	virtual void RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	virtual void StrctBoxsTrns();
	virtual void MtrxTrns();
	virtual CData* GetData(int idx);
	virtual CData* SrchData(DBL xx, DBL yy, DBL *retlen);
	virtual CData* SrchIdsData(int *nIDary, int nIDcnt, DBL xx, DBL yy, DBL *retlen);
	virtual CData* SrchDataBoxs(DBL xx, DBL yy, DBL *retlen);
	virtual CData* SrchIdsDataBoxs(int *nIDary, int nIDcnt, DBL xx, DBL yy, DBL *retlen);
	virtual void CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick);
	virtual void CheckFlagDraw(int flag, CDC *pcDC);
	virtual void CheckFlagOutlineDraw(int flag, CDC *pcDC);
	virtual void AtntionDraw(CDC *pcDC);
	virtual void NoactDraw(CDC *pcDC);
	virtual void AtntionMojiDraw(CDC *pcDC, int idx);
	virtual void DelDraw(CDC *pcDC);
	virtual void StockClrDraw(CDC *pcDC);
	virtual void PlotOut(int nComID);
	virtual int GetFlag();
	virtual void SetFlag(int nFlag);
	virtual int CheckFlag(int bitflag);
	virtual void AllOnPointFlag(int bitflag);
	virtual void AllOffPointFlag(int bitflag);
	virtual void OnFlag(int bitflag);
	virtual void OffFlag(int bitflag);
	virtual void ChngFlag(int bitflag);
	virtual CRect* GetBoundingRect();
	virtual int CheckIdxFlag(int idx, int flag);
	virtual void OnIdxFlag(int idx, int flag);
	virtual void OffIdxFlag(int idx, int flag);
	virtual int CheckTateYoko();
	virtual CData *GetIdxMoji(int nidx);
	virtual void FlagCheckMojiChange(int flag);
	virtual void StrgFiting();
	virtual CVect* GetFitingVect();
	virtual void SetMirrerString(int codetbl[], int len);
	virtual void DataChange();
	virtual void DataChangeSetDataGrup(CDataGrup *pcDataGrup);
	virtual BOOL CheckDataChange();
	virtual void RenewMiniMaxPoint(DBL dx, DBL dy);
	virtual void RenewMiniPoint(DBL minix, DBL miniy);
	virtual void RenewMaxPoint(DBL maxx, DBL maxy);
	virtual void AddSubRag(DBL *sumrag, DBL dx, DBL dy, DBL x1, DBL y1, DBL x2, DBL y2);
	virtual int SrchNearOnPointLine(dfigutype fg, DBL xx, DBL yy, DBL *retlen, DBL *retx, DBL *rety);
private:
};

