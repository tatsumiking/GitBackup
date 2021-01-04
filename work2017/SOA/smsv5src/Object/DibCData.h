
#pragma once

#include "Data.h"

// �����g�p�\��N���X�錾

#define	RGBSIZE		4
#define	INITSETX	50
#define INITSETY	50

class AFX_CLASS_EXPORT CDibCData : public CData
{
public:
	CDibCData(int nDummy);
	~CDibCData();
public:
	int			m_nStat; // 0:�t�@�C���Ȃ��@1:�ʏ�摜�@2:�ό`�ݒ肠��(m_nTrnsMode)
	int			m_nTrnsMode;
	TCHAR		m_szOrgFileName[FNAMEMAX];
	TCHAR		m_szImgFileName[FNAMEMAX];
	Bitmap		*m_pcGPBitmap;
	int	m_nBitCount;
	DBL	m_dSrcXDPI, m_dSrcYDPI;
	DBL m_dSrcSx, m_dSrcSy;	// ���ۂ̃r�b�g�}�b�v�ʒu(0,0)dot
	DBL m_dSrcWidth, m_dSrcHight;	// ���ۂ̃r�b�g�}�b�v�T�C�Ydot

	DBL m_dDstSx, m_dDstSy;	//��ʏ�̃r�b�g�}�b�v�̊J�n�ʒumm
	DBL m_dDstWidth, m_dDstHight;	//��ʏ�̃r�b�g�}�b�v�T�C�Ymm
public:
	CLSID		m_clsid;
	ULONG		m_lFileSize;
	HANDLE		m_hDib;
	ULONG		m_lImageTopSeek;
	DBL			m_dXByte;

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
	void SetTrnsMode(int mode);
	void GetFileName(LPTSTR imgfilename);
	void SetFileName(LPTSTR imgfilename);
	int InitElement();
	int FileLoad();

	virtual CData* CopyData(DBL movex, DBL movey);
	virtual int CheckNullData();
	virtual void SaveEpsFile();
	virtual void OnDraw(CDC *pcDC);
	virtual void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual void MoveTrns(DBL movex, DBL movey);
	void ImageAreaLegal(int *nsx, int *nsy);
	COLORREF GetPicupClr(DBL dx, DBL dy);
	virtual CData* SrchData(DBL xx, DBL yy, DBL *retlen);
	virtual CData* SrchDataBoxs(DBL xx, DBL yy, DBL *retlen);
	virtual CData* SrchDataLimitArea(DBL xx, DBL yy, DBL *retlen, DBL limit);
	virtual void RenewMiniMax();
	void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	void DrowAreaBoxsCross(CDC *pcDC);
	void SetDstElement(DBL dstsx, DBL dstsy, DBL dstwd, DBL dsthi);
	void GetDstElement(LPDBL dstsx, LPDBL dstsy, LPDBL dstwd, LPDBL dsthi);
	void GetSrcElement(LPDBL srcxdpi, LPDBL srcydpi, LPDBL srcwd, LPDBL srchi);
	void SetSrcElement(DBL srcxdpi, DBL srcydpi, DBL srcwd, DBL srchi);
	virtual BOOL IsDibCObject();
	virtual int CheckID();
	int SetRGBQuadData(int idx, int rval, int gval, int bval);
	void DelArea(DBL sx, DBL sy, DBL ex, DBL ey);
	void SetArea(DBL sx, DBL sy, DBL ex, DBL ey);
	int FileDibSave();
	int FileTrnsSave();
	int getImageEncodeClsid(LPTSTR lpFormat, CLSID* pcClsid);
};

