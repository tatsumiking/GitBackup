
#pragma once


//G�@�T�v
//	�t���J���[���͂Q�l�̃r�b�g�}�b�v�C���[�W��������
//�@�f�o�C�X�R���e�L�X�g�N���X
//�@�f�[�^�̃T���l�C�����쐬���邽�߂Ɏg�p
//�@�x�N�g�������̕⏕�Ƃ��Ẵr�b�g�}�b�v
//  2001.09.27 �S�̂̍\����ύX 
//  CCmdDibC->CCmdSpcl->CCmdOutLine->CCmdBmpBase

#define	BITMAPAREASIZE	1024
//#define	BITMAPAREASIZE	4096

class CCmdBaseBmp : public CCmdBase
{
public:
	CCmdBaseBmp(CScrollView* pcview);
	~CCmdBaseBmp();
public:
	// �ȉ��_�C�A���O�Őݒ肷����e
	DBL m_dXTime, m_dYTime;			// �f�[�^�ϊ��{��
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
	// ���o�b�N�A�b�v�p
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
	void DrawAndCreateFullclrFile(LPTSTR fname);
	void InitDrawArea(DBL dminix, DBL dminiy, DBL dmaxx, DBL dmaxy);
	void EndDrawArea();
	void InitDrawAddArea(DBL dminix, DBL dminiy, DBL dmaxx, DBL dmaxy);
	void EndDrawAddArea();
	void SetDotSize(DBL xsize, DBL ysize);
	void InitBitMap();
	int BitOnOffCheck(BYTE *lpBits, int x, int y);
	void BitOnSet(BYTE *lpBits, int x, int y);
	void BitOffSet(BYTE *lpBits, int x, int y);
	void CreateBmpFile(LPTSTR fname, BYTE far *lpBits);
private:
	void InitFullclrMemDC();
	void EndFullclrMemDC();
	void OnDrawArea();
	void CreateFullclrFile(LPTSTR fname);

};

