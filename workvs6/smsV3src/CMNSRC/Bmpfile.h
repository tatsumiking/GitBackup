#ifndef __BMPFILE_H
#define __BMPFILE_H

//G　概要
//	フルカラーのビットマップイメージを持った
//　デバイスコンテキストクラス
//　データのサムネイルを作成するために使用
//

class CBmpFile : public CCmdBase
{
public:
	CBmpFile(CScrollView* pcview);
	~CBmpFile();
	void CreateBmpImage(int movex, int movey, COLORREF clr, LPSTR fname);
	void CreateBmpImageHalf(int movex, int movey, COLORREF clr, LPSTR fname);
	void GetMoveXY(int *movex, int *movey);
	int CreateBmpInit(int xdot, int ydot);
	int CreateBmpInitHalf(int xdot, int ydot);
	void CreateBmpEnd();
	void CreateBmpImage(LPSTR fname);
	void CreateBmpImageDiv(LPSTR fname);
	void CreateBmpImageRv(LPSTR fname);
	void CreateBmpImageDivRv(LPSTR fname);
	void CreateBmpOnebitImage(LPSTR fname);
	void CreateBmpOnebitImageRV(LPSTR fname);
	void BmpFileOutHalf(LPSTR fname, int xdot, int ydot);
private:
	void SetHalfrgb(int x, int y, int r, int g, int b);
	void Getrbg(int x, int y, int *r, int *g, int *b);
	void ConvertHalfBmpImage(int xdot, int ydot);
	int MemDCInit(int xdot, int ydot);
	void MemDCEnd();
	void AreaOnDraw(int sx, int sy, int ex, int ey);
	void AreaOnDraw1(int sx, int sy, int ex, int ey);
	void AreaOnDraw2(int sx, int sy, int ex, int ey);
	void BmpFileOut(LPSTR fname, int xdot, int ydot);
	void BmpFileOut1(LPSTR fname, int xdot, int ydot);
	void BmpFileOut2(LPSTR fname, int xdot, int ydot);
	void BmpFileOutRv(LPSTR fname, int xdot, int ydot);
	void BmpFileOutRv1(LPSTR fname, int xdot, int ydot);
	void BmpFileOutRv2(LPSTR fname, int xdot, int ydot);
	void BmpOnebitFileOut(LPSTR fname, int xdot, int ydot);
	void BmpOnebitFileOutRV(LPSTR fname, int xdot, int ydot);

public:
	int		m_nXDotMax, m_nYDotMax;		// 全体データサイズ
	CDC		m_cdcMemCDC;

private:
	// 以下ダイアログで設定する内容
	DBL m_dXTime, m_dYTime;			// データ変換倍率
	int m_nBmpStat;
	CRgn		m_crgnClip;
	CBitmap		m_cbmpCBitmap;
	CBitmap		*m_pcbackbmp;
	HANDLE		m_hBits;
	LPBYTE		m_lpBits;
	long		m_lXByte;
	long		m_lBitsSize;
	long		m_nHalfXbyte;
};

#endif

