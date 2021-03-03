#ifndef __CMDBSBMP_H
#define __CMDBSBMP_H

//G　概要
//	フルカラー又は２値のビットマップイメージを持った
//　デバイスコンテキストクラス
//　データのサムネイルを作成するために使用
//　ベクトル処理の補助としてのビットマップ
//  2001.09.27 全体の構成を変更 
//  CCmdDibC->CCmdSpcl->CCmdOutLine->CCmdBmpBase

#define	BITMAPAREASIZE	1024
//#define	BITMAPAREASIZE	4096

class CCmdBaseBmp : public CCmdBase
{
public:
	CCmdBaseBmp(CScrollView* pcview);
	~CCmdBaseBmp();
public:
	// 以下ダイアログで設定する内容
	DBL m_dXTime, m_dYTime;			// データ変換倍率
	int m_nBmpStat;
	int	m_nXSize, m_nYSize, m_nXByte;
	long	m_lBitsSize;
	CRgn		m_crgnClip;
	CBitmap		*m_pcBitmap, *m_oldBitmap;
	CDC			*m_pcMemDC;
	HANDLE		m_hBits;
	BYTE		*m_lpBits;
	DBL			m_dAreaXMM, m_dAreaYMM;

	DBL	m_dCntrX, m_dCntrY;
	DBL	m_dTrnsTime;
private:
	// 環境バックアップ用
	DBL m_dBsx, m_dBsy;
	DBL m_dBxmm, m_dBymm;
	DBL m_dBtime;
	int	m_nBMode;
public:
	void InitFullClrDrawArea(int xdot, int ydot, DBL sx, DBL sy, DBL ex, DBL ey);
	void InitActiveDrawArea();
	void EndActiveDrawArea();
	void InitImage();
	void EndImage();
	void InitBits();
	void EndBits();

	void InitFullclrImage(int xdot, int ydot);
	void EndFullclrImage();
	void DrawAndCreateFullclrFile(LPSTR fname);
	void InitDrawArea(DBL dminix, DBL dminiy, DBL dmaxx, DBL dmaxy);
	void EndDrawArea();
	void InitDrawAddArea(DBL dminix, DBL dminiy, DBL dmaxx, DBL dmaxy);
	void EndDrawAddArea();
	void SetDotSize(DBL xsize, DBL ysize);
	void InitBitMap();
	int BitOnOffCheck(BYTE *lpBits, int x, int y);
	void BitOnSet(BYTE *lpBits, int x, int y);
	void BitOffSet(BYTE *lpBits, int x, int y);
	void CreateBmpFile(LPSTR fname, BYTE far *lpBits);
private:
	void InitFullclrMemDC();
	void EndFullclrMemDC();
	void OnDrawArea();
	void CreateFullclrFile(LPSTR fname);

};

#endif

