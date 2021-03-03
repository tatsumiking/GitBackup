
#ifndef __SWAKTEXT_H
#define __SWAKTEXT_H

#include	"strgtext.h"

class AFX_CLASS_EXPORT CSWakText : public CStrgText
{
protected:
	CSWakText();
	DECLARE_SERIAL(CSWakText)
public:
	virtual int CheckID();
	virtual void Serialize(CArchive& ar);

public:
	int		m_nWakuNo;
	int		m_nFildNo;		//データベースの項目番号
	char	m_szWakuName[HQRECMAX];	//枠の名前
	int		m_nPosLock;		// ON　位置、サイズ変更不可
	int		m_nCntntLock;	// ON　内容変更不可
	int		m_nPrntOut;	// ON　印刷対象 OFF 印刷除外
	int		m_nWakuKind;	// 標準枠(DEFWAKU), 氏名枠(NAMEWAKU), 画像枠(BMPWAKU), ロゴ枠(LOGOWAKU), ＰＯＰ枠(POPWAKU)
	CDataFDib	*m_pcDataFDib;		// 写真枠、ロゴ枠のとき使用
	CVect	*m_pcFitingVect;// 線上配置基準線
	int		m_nSpaceKind;	// 文字枠(SPACEWAKU), データ(SPACEDATA)
							// 枠の先頭と最後のスペース明ける(SESPON) 通常(SESPOFF)
	int		m_nRoundKind;	// 線上で回転するかしないか
	DBL		m_dHrgnPar;		// ひらがな縮小パーセント
	DBL		m_dKtknPar;		// カタカナ縮小パーセント
	int		m_nOder;		// 均等(ODEREQUAL)、中央(ODERCNTR)、左上(ODERLEFT)、右下(ODERRIGHT)
	int		m_nBlnceKind;	// バランスなし-1 上のみ0　両方1　下のみ2
	DBL		m_dBlncePar;	// 大きくするパーセント
	DBL		m_dDataTime;	// 一時的に使用均等割付で平体になったとき１以外の値になる
public:
	CSWakText(int nDummy);
	~CSWakText();
public:
	DBL GetDataTime();
	void SaveEpsFile();
	virtual void SetUseClrPenno(int notbl[], LPINT cnt);
	void ResetMojiSize();
	void LegalMojiSize();
	int GetMojiCount();
	DBL GetSpace(CMojiText *pcMoji, POSITION pos);
	DBL GetSumSpace();
	virtual int CheckNullData();
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void GetWakuDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void RenewMiniMax();
	virtual CData* CopyData(DBL movex, DBL movey);
	virtual void CopyDataElement(CSWakText *pcnewData, DBL movex, DBL movey);
	virtual void AllFree();
	virtual void NoSelectDraw(CDC *pcDC, int no);
	virtual void SelectDraw(CDC *pcDC, int no);
	virtual void OnDraw(CDC *pcDC);

	DBL GetMiniSpace(DBL pretbl[], DBL posttbl[]);
	DBL GetPropStrglen();
	void DeleteData(CData *delData);
	CData* SrchDataBoxs(DBL dx, DBL dy, DBL *retlen);
	void WakuBoxsDraw(CDC *pcDC);
	void GetFitingBoxs(LPDBL sx, LPDBL sy, LPDBL ex, LPDBL ey);

	virtual int DrawNo(CDC *pcDC, int noidx);
	virtual void OutlineDraw(CDC *pcDC);
	virtual void AtntionDraw(CDC *pcDC);
	virtual void NoactDraw(CDC *pcDC);
	virtual void DelDraw(CDC *pcDC);
	virtual void MoveTrns(DBL movex, DBL movey);
	virtual void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual void SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy);
	virtual void MirrorTrns(DBL a, DBL b, DBL t1, DBL t2);
	virtual void RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	virtual void StrctBoxsTrns();
	virtual void AllOffPointFlag(int bitflag);
	virtual void AllOnPointFlag(int bitflag);
	virtual void PointXLegal(DBL xx);
	virtual void PointYLegal(DBL yy);
	virtual CVect* GetFitingVect();
	void SetFitingVect(CVect* pcVect);
	void GetWakuName(LPSTR name);
	void SetWakuName(LPSTR name);
	void GetWakuKind(LPINT nkind);
	void SetWakuKind(int nkind);
	void GetSpaceFlag(LPINT nflag);
	void SetSpaceFlag(int nflag);
	void GetSpaceKind(LPINT nkind);
	void SetSpaceKind(int nkind);
	void GetSESpKind(LPINT nkind);
	void SetSESpKind(int nkind);
	void GetRoundKind(LPINT nkind);
	void SetRoundKind(int nkind);
	void GetHrgnPar(LPDBL dpar);
	void SetHrgnPar(DBL dpar);
	void GetKtknPar(LPDBL dpar);
	void SetKtknPar(DBL dpar);
	void GetBlncePar(LPDBL dpar);
	void SetBlncePar(DBL dpar);
	void GetOderKind(int *nkind);
	void SetOderKind(int nkind);
	void GetBlnceKind(int *nkind);
	void SetBlnceKind(int nkind);
	void StringElementInit();
	virtual void StrgFiting();
	void StrgLineStruct();
	virtual BOOL CheckDataChange();
private:
	void OnDrawPop(CDC *pcDC);
	void SetLenStrgLineStructLeftData();
	void SetLenStrgLineStructLeftWaku();
	void SetLenStrgLineStructCntrData();
	void SetLenStrgLineStructCntrWaku();
	void SetLenStrgLineStructRightData();
	void SetLenStrgLineStructRightWaku();
	void SetLenStrgLineStructEqualData();
	void SetLenStrgLineStructEqualWaku();
	void GetTopLastSpace(LPDBL top, LPDBL last);
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
	DBL	m_dLastX1;
	DBL	m_dLastY1;
	DBL	m_dLastX2;
	DBL	m_dLastY2;
};

#endif

