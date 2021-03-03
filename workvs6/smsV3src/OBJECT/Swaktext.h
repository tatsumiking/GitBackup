
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
	int		m_nFildNo;		//�f�[�^�x�[�X�̍��ڔԍ�
	char	m_szWakuName[HQRECMAX];	//�g�̖��O
	int		m_nPosLock;		// ON�@�ʒu�A�T�C�Y�ύX�s��
	int		m_nCntntLock;	// ON�@���e�ύX�s��
	int		m_nPrntOut;	// ON�@����Ώ� OFF ������O
	int		m_nWakuKind;	// �W���g(DEFWAKU), �����g(NAMEWAKU), �摜�g(BMPWAKU), ���S�g(LOGOWAKU), �o�n�o�g(POPWAKU)
	CDataFDib	*m_pcDataFDib;		// �ʐ^�g�A���S�g�̂Ƃ��g�p
	CVect	*m_pcFitingVect;// ����z�u���
	int		m_nSpaceKind;	// �����g(SPACEWAKU), �f�[�^(SPACEDATA)
							// �g�̐擪�ƍŌ�̃X�y�[�X������(SESPON) �ʏ�(SESPOFF)
	int		m_nRoundKind;	// ����ŉ�]���邩���Ȃ���
	DBL		m_dHrgnPar;		// �Ђ炪�ȏk���p�[�Z���g
	DBL		m_dKtknPar;		// �J�^�J�i�k���p�[�Z���g
	int		m_nOder;		// �ϓ�(ODEREQUAL)�A����(ODERCNTR)�A����(ODERLEFT)�A�E��(ODERRIGHT)
	int		m_nBlnceKind;	// �o�����X�Ȃ�-1 ��̂�0�@����1�@���̂�2
	DBL		m_dBlncePar;	// �傫������p�[�Z���g
	DBL		m_dDataTime;	// �ꎞ�I�Ɏg�p�ϓ����t�ŕ��̂ɂȂ����Ƃ��P�ȊO�̒l�ɂȂ�
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

