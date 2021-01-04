
#pragma once

#include "Data.h"
#include "Vect.h"
#include "StrgVect.h"

class AFX_CLASS_EXPORT CSPtnVect : public CStrgVect
{
protected:
	CSPtnVect();
	DECLARE_SERIAL(CSPtnVect)
public:
	virtual void Serialize(CArchive& ar);

public:
	CVect	*m_pcFitingVect;// 線上配置基準線
	int		m_nSpaceKind;	// 配置するとき文字間を保持するかしないか
	int		m_nRoundKind;	// 線上で回転するかしないか
	int		m_nSetMode;		// 左下基準(LEFTDOWN), 中央基準(CNTRCNTR), 右上基準(RIGHTUP)
	DBL		m_dPitch;		// 文字間隔（文字サイズに対する割合）
	DBL		m_dFlatPar;		// 長体、平体（文字サイズに対する割合）
	DBL		m_dThickPar;	// 太字パーセント（文字サイズに対する割合）
	DBL		m_dSlopRag;		// 斜形角度
	int		m_nOder;		// 均等(ODEREQUAL)、中央(ODERCNTR)、左(ODERLEFT)、右(ODERRIGHT)
	DBL		m_dLastX1;
	DBL		m_dLastY1;
	DBL		m_dLastX2;
	DBL		m_dLastY2;

public:
	CSPtnVect(int nDummy);
	~CSPtnVect();
public:
	CData* SrchDataBoxs(DBL dx, DBL dy, DBL * retlen);
	virtual void DeleteData(CData * delData);
	virtual int CheckNullData();
	void CopyString(CSPtnVect *pcnewVect, DBL movex, DBL movey);
	virtual void SaveStrgStoreFile(FILE *fp, int id);
	virtual void LoadStrgStoreFile(FILE *fp, int id);
	virtual int CheckID();
	virtual void AllOffPointFlag(int bitflag);
	virtual void AllOnPointFlag(int bitflag);
	virtual void PointXLegal(DBL xx);
	virtual void PointYLegal(DBL yy);
	virtual CData* CopyData(DBL movex, DBL movey);
	virtual void AllFree();
	virtual void OnDraw(CDC *pcDC);
	virtual void OutlineDraw(CDC *pcDC);
	virtual void AtntionDraw(CDC *pcDC);
	virtual void NoactDraw(CDC *pcDC);
	virtual void DelDraw(CDC *pcDC);
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void GetStringDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void RenewMiniMax();
	virtual void MoveTrns(DBL movex, DBL movey);
	virtual void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual void SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy);
	virtual void MirrorTrns(DBL a, DBL b, DBL t1, DBL t2);
	virtual void RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	virtual void StrctBoxsTrns();
	CData* SrchNearPoint(DBL xx, DBL yy, DBL *retlen, int *retpp);
	virtual CVect* GetFitingVect();
	void SetFitingVect(CVect* pcVect);
	void GetSpaceKind(int *flag);
	void SetSpaceKind(int flag);
	void GetRoundKind(int *flag);
	void SetRoundKind(int flag);
	void GetSetMode(int *mode);
	void SetSetMode(int mode);
	void GetPitch(LPDBL dpitch);
	void SetPitch(DBL dpitch);
	void GetFlatPar(LPDBL dflatpar);
	void SetFlatPar(DBL dflatpar);
	void GetThickPar(LPDBL dthickpar);
	void SetThickPar(DBL dthickpar);
	void GetSlopRag(LPDBL dsloprag);
	void SetSlopRag(DBL dsloprag);
	void GetOderKind(int *noder);
	void SetOderKind(int noder);
	virtual void StrgFiting();
	void StrgLineStruct();
private:
	void SetLenStrgLineStructLeftData();
	void SetLenStrgLineStructLeftWaku();
	void SetLenStrgLineStructCntrData();
	void SetLenStrgLineStructCntrWaku();
	void SetLenStrgLineStructRightData();
	void SetLenStrgLineStructRightWaku();
	void SetLenStrgLineStructSpace();
	void SetLenStrgLineStructEqual();
	void StrgLineStructTrns();

	DBL GetVectLength();
	DBL GetRejionLength();
	DBL GetNorejionLength();
	DBL GetBezeLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	DBL GetArcLength(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetFreeCurveLength(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetLineLength(DBL x1, DBL y1, DBL x2, DBL y2);

	DBL GetVectPointLength(DBL xx, DBL yy);
	DBL GetRejionPointLength(int *flag, DBL xx, DBL yy);
	DBL GetNorejionPointLength(int *flag, DBL xx, DBL yy);
	DBL GetBezePointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	DBL GetArcPointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetFreeCurvePointLength(int *flag, DBL xx, DBL yy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetLinePointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2);

	DBL GetVectPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy);
	DBL GetRejionPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy);
	DBL GetNorejionPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy);
	DBL GetBezePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	DBL GetArcPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetFreeCurvePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetLinePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2);

private:
	DBL	m_dLinelen;
	DBL	m_dStrglen;
	DBL	m_dStartlen;
	DBL	m_dTrnstime;
	DBL	m_dHalffontsize;
};


