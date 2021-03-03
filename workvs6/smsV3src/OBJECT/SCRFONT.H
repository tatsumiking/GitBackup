
#ifndef __SCRFONT_H
#define __SCRFONT_H               // Prevent multiple includes

#include	"fsf1font.h"

#define	SCRMESH	1024
#define	SCRATRADD		0
#define	SCRXCDADD		1
#define	SCRYCDADD		2
#define	SCRSIZADD		3

#define	SCRPRTCT	28562L

#define SCRARCMASK		0x10
#define	SCRCCNTMASK		0x08
#define	SCRCSTARTMASK	0x04
#define	SCRRENDMASK		0x02
#define	SCRENDMASK		0x01

class CFSF1Font;
class AFX_CLASS_EXPORT CSCRFont : public CObject
{
protected:
	CSCRFont();
	DECLARE_SERIAL(CSCRFont)
public:
	CSCRFont(int nDummy);
	~CSCRFont();
	virtual void Serialize(CArchive& ar);
private:
	FILE		*m_fileIdx, *m_fileDat;
	CFSF1Font*	m_pFSF1Font;
	BYTE		m_bFBuf[3072];
	int			m_nDataLen;
	int			m_nTateYoko;
	char		m_sBaseFname[FNAMEMAX];

public:
	void DFileLoadSF1Trns(int datano);
	void TrnsCopyFSF1FBuf();
	LPBYTE GetbFBufAdrs();
	int FontMeshGet();
	void FSF1ClassSet(CFSF1Font *pFSF1Font);
	void FontNameSet(LPSTR pFont);
	void SetTateYoko(int flag);
	void SF1TrnsDataLoad(UINT sjis);
	void SF1TrnsDataSave(UINT sjis);
	int DFileReadOpen(char basefname[]);
	void DFileDataLoad(int datano);
	void DFileClose();

	void DataLoad(UINT sjis);
	int UserDataSave(UINT sjis);
	int UserDataExist(UINT sjis);
	int UserDataDelete(UINT sjis);
	int UserDataLoad(UINT sjis);
	int EsysDataLoad(UINT sjis);
	int UserFontWriteOpen(UINT sjis);
	int UserFontOpen(UINT sjis);
	int EsysFontOpen(UINT sjis);
	void FontClose();
	int UserFontNullSet(char idxfname[], char datfname[]);
	void GetSeekPoint(UINT sjis, long *seekp);
	void TateGakiTrns(UINT sjis);
	void TrnsSCRToFSF1();
	void SetDataLen(int pp);
	void AtrDXYGet(BYTE fBuf[], int pp, int *atr, DBL *x, DBL *y);
	int EorChk(BYTE fBuf[], int pp);
};

#endif

