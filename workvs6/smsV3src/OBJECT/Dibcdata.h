
#ifndef __DIBCDATA_H
#define __DIBCDATA_H

#include "data.h"
// 内部使用予定クラス宣言

#define	RGBSIZE	4
#define	INITSETX	50
#define INITSETY	50

class AFX_CLASS_EXPORT CDibCData : public CData
{
public:
	DBL m_dDstSx, m_dDstSy;	//画面上のビットマップの開始位置mm
	DBL m_dDstWidth, m_dDstHight;	//画面上のビットマップサイズmm
	DBL m_dSrcSx, m_dSrcSy;	// 実際のビットマップ位置(0,0)dot
	DBL m_dSrcWidth, m_dSrcHight;	// 実際のビットマップサイズdot
	DBL	m_dSrcXDPI, m_dSrcYDPI;
	DBL	m_dXByte;
	DBL	m_nStat;
public:
	ULONG		m_lImageTopSeek;
	ULONG		m_lFileSize;
	char		m_szBmpFname[FNAMEMAX];
	char		m_szBkBmpFname[FNAMEMAX];
	int			m_nBitCount;
	HANDLE		m_hDib;
	long		m_lStretchMode;		//

	DBL m_dBkSrcWidth, m_dBkSrcHight;	// 実際のビットマップサイズdot
	ULONG		m_lBkImageTopSeek;
	ULONG		m_lBkFileSize;
	HANDLE		m_hBkDib;
	long		m_lStretchBkMode;
private:
	int		m_nOutXByte;
	HANDLE	m_hOutDib;
	DBL		m_dOutTrnsx, m_dOutTrnsy;
	int		m_nOW, m_nOH;
	
protected:
	CDibCData();
	DECLARE_SERIAL(CDibCData)
public:
	virtual void Serialize(CArchive& ar);
	virtual void AtntionDraw(CDC *pcDC);
	virtual void NoactDraw(CDC *pcDC);
	virtual void FillDraw(CDC *pcDC);
	virtual void StockClrDraw(CDC *pcDC);
public:
	void DelArea(DBL sx, DBL sy, DBL ex, DBL ey);
	void SetArea(DBL sx, DBL sy, DBL ex, DBL ey);
	void RasterRevace();
	void SetClrTable(COLORREF clr);
	void ChengeBaseClr(int idx, COLORREF clr);
	virtual CData* CopyData(DBL movex, DBL movey);
	virtual int CheckNullData();
	void RoundDib(int mode);
	void FileSave();
	virtual void SaveEpsFile();
	virtual void OnDraw(CDC *pcDC);
	virtual void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual void MoveTrns(DBL movex, DBL movey);
	virtual void RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	void SetRGBQuadData(int plt, int rval, int gval, int bval);
	void GetRGBQuadData(int plt, LPINT rval, LPINT gval, LPINT bval);
	int SetStretchMode(int mode);
	void GetBmpFname(LPSTR srcfname);
	void BmpAreaLegal(LPDBL dsx, LPDBL dsy);
	COLORREF GetNearClr(DBL dsx, DBL dsy);
	COLORREF GetSrcXYClr(DBL dsx, DBL dsy);
	COLORREF GetPicupClr(DBL dx, DBL dy);
	virtual CData* SrchData(DBL xx, DBL yy, DBL *retlen);
	virtual CData* SrchDataBoxs(DBL xx, DBL yy, DBL *retlen);
	virtual CData* SrchDataLimitArea(DBL xx, DBL yy, DBL *retlen, DBL limit);
	virtual void RenewMiniMax();
	void SetBmpFname(LPSTR bmpfname);
	int DibLoad();
	int DibReload();
	void SetBkBmpFname(LPSTR bmpfname);
	int DibBkLoad();
	int DibBkReload();
	void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	void DrowAreaBoxsCross(CDC *pcDC);
	int PaintStretchBlt(CDC *pcDC);
	int PaintStretchDIBits(CDC *pcDC, int mode);
	int PaintBkStretchDIBits(CDC *pcDC, int mode);
	void PaintDibDirect(CDC *pcDC);
	void OutBlockCreate(LPBYTE lpSrcTop, int nsx, int nsy, LPBYTE lpDstTop);
	void OutBlockCreate32(LPBYTE lpSrcTop, int nsx, int nsy, LPBYTE lpDstTop);
	void OutBlockCreate24(LPBYTE lpSrcTop, int nsx, int nsy, LPBYTE lpDstTop);
	void OutBlockCreate8(LPBYTE lpSrcTop, int nsx, int nsy, LPBYTE lpDstTop);
	void OutBlockCreate1(LPBYTE lpSrcTop, int nsx, int nsy, LPBYTE lpDstTop);
	void SetDstElement(DBL dstsx, DBL dstsy, DBL dstwd, DBL dsthi);
	void GetDstElement(LPDBL dstsx, LPDBL dstsy, LPDBL dstwd, LPDBL dsthi);
	void GetSrcElement(LPDBL srcxdpi, LPDBL srcydpi, LPDBL srcwd, LPDBL srchi);
	void SetSrcElement(DBL srcxdpi, DBL srcydpi, DBL srcwd, DBL srchi);
	virtual BOOL IsDibCObject();
	virtual int CheckID();
	CDibCData(int nDummy);
	~CDibCData();
};

#endif

