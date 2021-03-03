
#ifndef __MOJITEXT_H
#define __MOJITEXT_H

#include	"vect.h"
#include	"datalist.h"

class AFX_CLASS_EXPORT CMojiText : public CVect
{
protected:
	CMojiText();
	DECLARE_SERIAL(CMojiText)
public:
	virtual void Serialize(CArchive& ar);
	CMojiText(int nDummy);
	~CMojiText();

public:
	CDataList	*m_pcDataList;
	char		m_szFontName[FACEMAX];
	int		m_nTateYoko;	// 縦書きON　横書きOFF
	UINT	m_nCode;		// 文字コード
	DBL		m_dSize;		// ベース文字サイズ
	DBL		m_dSpacePar;	// 文字間隔
	DBL		m_dFlatPar;		// 平体率
	DBL		m_dMoveXPar;	// 横幅に対する移動量
	DBL		m_dMoveYPar;	// 高さに対する移動量
	DBL		m_dTrnsPar;		// ひらがな縮小、カタカナ縮小、バランスに対応
	int		m_nBold;		// 太字 ON OFF
	int		m_nSlope;		// 斜形 ON OFF
	DBL		m_dTT;			// 媒介変数
	int		m_nPop;			// ポップ文字対応
	// 文字サイズを持たないデータがかな縮小でどんどん小さくなるバグ回避のため	
	DBL		m_dTKtknPar;
	DBL		m_dTHrgnPar;

private:
	int		m_nFontType;	// 
	DBL		m_dHWTime;		// 
	DBL		m_dSEPWD, m_dSEPHI;
	DBL		m_dSEPSP, m_dSEPEP;

public:
	void GetDrawElement(DBL *sx, DBL *sy, DBL *wd, DBL *hi, DBL *rag);
	void SetFSF1Data();
	void GetGaijiFileName(LPSTR sGJFaceName);
	virtual void RenewMiniMax();
	void GetTrnsXYTime(LPDBL xtime, LPDBL ytime);
	virtual void SetUseClrPenno(int notbl[], LPINT cnt);
	void SetVect(LPBYTE fbuf, int pcnt, CVect *pcVect);
	void GetMojiWakuSize(LPDBL size);
	void GetDataSEP(LPDBL sp, LPDBL ep);
	void GetFontBody(DBL *size);
	void GetFontWakuWidth(DBL *size);
	void GetFontWidth(DBL *size);
	int IsZenkaku();
	virtual int CheckID();
	virtual int CheckNullData();
	virtual void GetWakuDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void GetDataDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void DeleteData(CData * pData);
	virtual CData* SrchData(DBL x, DBL y, DBL *retlen);
	virtual CData* SrchDataBoxs(DBL x, DBL y, DBL *retlen);
	virtual CData* SrchNearPoint(DBL xx, DBL yy, DBL *retlen, int *retpp);
	virtual CData* SrchNearOnLinePoint(DBL xx, DBL yy, DBL * retlen, DBL * retx, DBL * rety, int * retpp);
	virtual CData* CopyData(DBL movex, DBL movey);
	virtual void MoveTrns(DBL movex, DBL movey);
	virtual void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual void RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	virtual void SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy);
	virtual void FillClrDraw(CDC *pcDC);
	virtual void CheckFlagDraw(int flag, CDC *pcDC);
	virtual void CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick);
	virtual void DrawMojiWaku(CDC *pcDC);
	virtual void OnDraw(CDC *pcDC);
	virtual void OutlineDraw(CDC *pcDC);
	virtual void DataChange();
	virtual void SetPosition(DBL sx, DBL sy, DBL size, DBL par, DBL rag);
	virtual void GetFontBoxs(DBL x[], DBL y[]);
	virtual void SetFontSize(DBL dFWsize, DBL dFHsize);
	virtual void GetFontSize(DBL *dFWsize, DBL *dFHsize);
	virtual void GetBaseFontSize(DBL *dSize);
	virtual void SetBaseFontSize(DBL dSize);
	virtual void SetFont(LPSTR fontname);
	virtual void GetFont(LPSTR fontname);
	virtual void SetTateGaki(int tategaki);
	virtual void GetTateGaki(LPINT tategaki);
	virtual void SetCode(UINT code);
	virtual void GetCode(LPUINT code);
	virtual void SetTT(DBL tt);
	virtual void GetTT(LPDBL tt);
	virtual void SetStartXY(DBL dSx, DBL dSy);
	virtual void GetStartXY(DBL *dSx, DBL *dSy);
	virtual void GetSpacePar(LPDBL spacepar);
	virtual void SetSpacePar(DBL spacepar);
	virtual void GetFlatPar(LPDBL flatpar);
	virtual void SetFlatPar(DBL flatpar);
	virtual void GetMoveXPar(LPDBL movexpar);
	virtual void SetMoveXPar(DBL movexpar);
	virtual void GetMoveYPar(LPDBL moveypar);
	virtual void SetMoveYPar(DBL moveypar);
	virtual void SetTrnsXYTime(DBL xtime, DBL ytime);
	virtual void AddMoveXY(DBL xtime, DBL ytime);
	virtual void AddTrnsXYTime(DBL xtime, DBL ytime);
	virtual void SetTrnsPar(DBL trnspar);
	virtual void GetTrnsPar(LPDBL trnspar);
	virtual void GetBold(LPINT flag);
	virtual void SetBold(int flag);
	virtual void GetSlope(LPINT flag);
	virtual void SetSlope(int flag);
	void GetPrePostTable(DBL pre[], DBL post[]);
	void ResetFontSize(DBL size, DBL par, DBL *width, DBL *hight);
private:
	void DrawOutMemDCTrueTypeFont();
	void GetMojiWakuSizeTrueTypeFont(LPDBL mojisize);
	void DrawOutTrueTypeFont(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL fontrag);
	void FontCreate(CFont *pcFont, int wd, int hi, int round, int bold, int Slope, LPSTR name, int type);
	void DataChangeTrueTypeFont();
	void SetHWTimeTureTypeFont();

	void DrawOutMemDCARCFont();
	void DrawOutARCFont(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL fontrag);
	void DataChangeARCFont();

	int DrawOutMemDCESF0Font();
	int DrawOutESF0Font(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL fontrag);
	int DataChangeESF0Font();

	int DrawOutMemDCESF1Font();
	int DrawOutESF1Font(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL fontrag);
	int DataChangeESF1Font();
};

#endif

