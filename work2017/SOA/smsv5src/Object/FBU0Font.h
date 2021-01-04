
#pragma once

#include "FSF1Font.h"

// FTT 縦書きインデックスファイル
// FDT 縦書きデータファイル
// FTY 横書きインデックスファイル
// FDY 横書きデータファイル
// FTG 外字用インデックスファイル
// FDG 外字用データファイル
// FBU ユーザ登録文字（前半画像ファイル後半ベクトルデータファイル）

#define	FBU0MESH		1024
#define	FBU0ATRADD		0
#define	FBU0XCDADD		1
#define	FBU0YCDADD		3
#define	FBU0SIZADD		5

#define	FBU0LASTCODE	0xffff	// 

#define	FBU0REJIONMASK	0x10
#define	FBU0EORMASK		0x01

#define	FBU0POINTMAX	1000
#define FBUBUFMAX		5000

typedef	struct {
	short	len;
	long	seekp;
} fbuidxtype;

class AFX_CLASS_EXPORT CFBU0Font : public CObject
{
protected:
	CFBU0Font();
	DECLARE_SERIAL(CFBU0Font)
public:
	CFBU0Font(int nDummy);
	~CFBU0Font();
	virtual void Serialize(CArchive& ar);
private:
	FILE		*m_fileIdx, *m_fileDat;
	CFSF1Font*	m_pFSF1Font;
	BYTE		m_bFBuf[FBUBUFMAX];
	UINT		m_nDataLen;
	int			m_nTateYoko;
	int			m_nBold;
	int			m_nSlope;
	TCHAR		m_sBaseName[FNAMEMAX];
	TCHAR		m_sIdxFileName[FNAMEMAX];
	TCHAR		m_sDatFileName[FNAMEMAX];

	DBL		m_dSX, m_dSY;
	DBL		m_dXTime, m_dYTime;
	DBL		m_dSin, m_dCos;

public:
	LPBYTE GetbFBufAdrs();
	int FontMeshGet();
	void FSF1ClassSet(CFSF1Font *pFSF1Font);
	void FontNameSet(LPTSTR pFont);
	void SetTateYoko(int flag);
	int SF1TrnsDataLoad(int sjis);
	void SF1TrnsDataSave(int sjis);
	void SF1TrnsDataReadSave(int code);
	int FontNullSet();
	int UserFontNullSet();
	int DataLoad(int code);
	int DataSave(int code);
	int DataRealSave(int code);
	void TrnsCopyFSF1FBuf();
	int UserDataDelete(int code);
	int UserDataSave(int code);
	int UserDataExist(int sjis);

private:
	int UserDataLoad(int code);
	int GaijiDataLoad(int code);
	int TategakiDataLoad(int code);
	int YokogakiDataLoad(int code);
	int FileDataLoad(int code);
	int GaijiDataSave(int code);
	int TategakiDataSave(int code);
	int YokogakiDataSave(int code);
	int FileDataSave(int code);
	int FontTblDatNullSet(TCHAR *lpIdxFname, TCHAR *lpDatFname);
	void TrnsFBU0ToFSF1();
	void TrnsFSF1ToFBU0();
	void SetDataLen(int pp);
	void AtrDXYGet(BYTE fBuf[], int pp, int *atr, DBL *x, DBL *y);
	void AtrDXYSet(BYTE fBuf[], int pp, int atr, DBL x, DBL y);
	int EorChk(BYTE fBuf[], int pp);
	void EorOn(BYTE fBuf[], int pp);
	int RejionChk(BYTE fBuf[], int pp);
	void RejionOff(BYTE fBuf[], int pp);
	void RejionOn(BYTE fBuf[], int pp);
	void DrawOut(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL rag);
	void TrnsElementInit(DBL sx, DBL sy, DBL wd, DBL hi, DBL rag);
	int RejionEndGet(BYTE fBuf[], int sp);
	void RejionDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt);
	void TrnsXY(DBL *x, DBL *y);
	int PointLegal(int pcnt, int pp);
};

