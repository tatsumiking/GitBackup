#ifndef __METADATA_H
#define __METADATA_H

#include "data.h"

class AFX_CLASS_EXPORT CMetaData : public CData
{
protected:
	CMetaData();
	DECLARE_SERIAL(CMetaData)
public:
	CMetaData(int nDummy);
	~CMetaData();
	virtual int CheckID();
	virtual void Serialize(CArchive& ar);

// �֐�
public:
	virtual void InitElement();
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void RenewMiniMax();
	virtual void MoveTrns(DBL movex, DBL movey);
	virtual void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	virtual void RoundTrns(DBL cntrx, DBL cntry, DBL rag);
	virtual CData* SrchData(DBL xx, DBL yy, DBL *retlen);
	virtual void AtntionDraw(CDC *pcDC);
	virtual void StockClrDraw(CDC *pcDC);
	virtual void OnDraw(CDC *pcDC);
	virtual CData * CopyData(DBL movex, DBL movey);

	void GetDstElement(LPDBL dstsx, LPDBL dstsy, LPDBL dstwd, LPDBL dsthi);
	void SetDstElement(DBL dstsx, DBL dstsy, DBL dstwd, DBL dsthi);
	void GetSrcElement(LPDBL srcxdpi, LPDBL srcydpi, LPDBL srcwd, LPDBL srchi);
	void DrowAreaBoxsCross(CDC *pcDC);
// �f�[�^�����o�[
public:
	HENHMETAFILE	m_hEmf;
	DBL m_dDstSx, m_dDstSy;	//��ʏ�̃r�b�g�}�b�v�̊J�n�ʒumm
	DBL m_dDstWidth, m_dDstHeight;	//��ʏ�̃r�b�g�}�b�v�T�C�Ymm
	DBL m_dSrcSx, m_dSrcSy;	// ���ۂ̃r�b�g�}�b�v�ʒu(0,0)dot
	DBL m_dSrcWidth, m_dSrcHeight;	// ���ۂ̃r�b�g�}�b�v�T�C�Ydot
	DBL	m_dSrcXDPI, m_dSrcYDPI;
	// �����g�p
public:
	virtual void LegalElement();

private:
};

#endif

