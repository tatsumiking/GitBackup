
//
// Easy Scanner System
//
// object scrfont.cpp  ÇRÉoÉCÉgÇrÇbÇqèëëÃèàóùä÷êî
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include <stdio.h>
#include "scrfont.h"

extern "C" {
	void mojienvgetfontpath(LPSTR fontpath);
	void basesjtokuten(int far sjis, int far *ku, int far *ten);
}

#define	SCRFIDXSIZE	6

IMPLEMENT_SERIAL(CSCRFont, CObject, 0)
CSCRFont::CSCRFont()
{
}

CSCRFont::CSCRFont(int nDummy)
{
	m_fileIdx = NULL;
	m_fileDat = NULL;
	m_pFSF1Font = NULL;
	m_nDataLen = 0;
	m_nTateYoko = 0;
	m_sBaseFname[0] = '\0';
}

CSCRFont::~CSCRFont()
{
}

void CSCRFont::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// ar << 
	}
	else
	{
		// ar >>
	}
}

LPBYTE CSCRFont::GetbFBufAdrs()
{
	return(m_bFBuf);
}

int CSCRFont::FontMeshGet()
{
	return(SCRMESH);
}

void CSCRFont::FSF1ClassSet(CFSF1Font *pFSF1Font)
{
	m_pFSF1Font = pFSF1Font;
}

void CSCRFont::FontNameSet(LPSTR pFont)
{
	char	fontpath[FNAMEMAX];

	mojienvgetfontpath(fontpath);
	sprintf(m_sBaseFname, "%s\\%s", fontpath, pFont);
}

void CSCRFont::SetTateYoko(int flag)
{
	m_nTateYoko = flag;
}

void CSCRFont::SF1TrnsDataLoad(UINT sjis)
{
	DataLoad(sjis);
	TrnsSCRToFSF1();
}

void CSCRFont::SF1TrnsDataSave(UINT sjis)
{
	TrnsCopyFSF1FBuf();
	UserDataSave(sjis);
}

void CSCRFont::DataLoad(UINT sjis)
{
	if(sjis < 0x8140 || 0xffff < sjis){
		m_nDataLen = 0;
		return;
	}
	if(0x9fff < sjis && sjis < 0xe021){
		m_nDataLen = 0;
		return;
	}
	if(EsysDataLoad(sjis) == FAIL){
		m_nDataLen = 0;
		return;
	}
}

int CSCRFont::UserDataSave(UINT sjis)
{
	return(FAIL);
}

int CSCRFont::UserDataExist(UINT sjis)
{
	return(FAIL);
}

int CSCRFont::UserDataDelete(UINT sjis)
{
	return(FAIL);
}

int CSCRFont::UserDataLoad(UINT sjis)
{
	return(FAIL);
}

int CSCRFont::EsysDataLoad(UINT sjis)
{
	long 		seekpoint;
	long		seekp;
	UINT		jiscode;
	UCHAR		ubuf[16];

	if(EsysFontOpen(sjis) == FAIL){
		return(FAIL);
	}

	GetSeekPoint(sjis, &seekpoint);
	if(fseek(m_fileIdx, seekpoint, 0) != 0){
		FontClose();
		m_nDataLen = 0;
		return(0);
	}
	fread(ubuf, SCRFIDXSIZE, 1, m_fileIdx);
	jiscode = ubuf[0] <<  8;
	jiscode |= ubuf[1];
	seekp = (long)ubuf[2] << 24;
	seekp |= (long)ubuf[3] << 16;
	seekp |= (long)ubuf[4] <<  8;
	seekp |= (long)ubuf[5];
	if(seekp == -1L){
		FontClose();
		m_nDataLen = 0;
		return(0);
	}
	m_nDataLen = 0;
	fseek(m_fileDat, seekp, 0);
	while(1){
		if(fread(ubuf, 3, 1, m_fileDat) != 1){
			break;
		}
		m_bFBuf[m_nDataLen++] = ubuf[0];
		m_bFBuf[m_nDataLen++] = ubuf[1];
		m_bFBuf[m_nDataLen++] = ubuf[2];
		if(ubuf[0] & SCRENDMASK){
			break;
		}
	}
	FontClose();
	return(0);
}

int CSCRFont::UserFontWriteOpen(UINT sjis)
{
	char	idxfname[FNAMEMAX], datfname[FNAMEMAX];

	if(sjis < 0x989e){
		sprintf(idxfname, "%s.iu1", m_sBaseFname);
		sprintf(datfname, "%s.du1", m_sBaseFname);
	}
	else {
		sprintf(idxfname, "%s.iu2", m_sBaseFname);
		sprintf(datfname, "%s.du2", m_sBaseFname);
	}
	if((m_fileIdx = fopen(idxfname, "r+b")) == NULL){
		UserFontNullSet(idxfname, datfname);
		if((m_fileIdx = fopen(idxfname, "r+b")) == NULL){
			return(FAIL);
		}
	}
	if((m_fileDat = fopen(datfname, "r+b")) == NULL){
		fclose(m_fileIdx);
		return(FAIL);
	}
	return(0);
}

int CSCRFont::UserFontOpen(UINT sjis)
{
	char	idxfname[FNAMEMAX], datfname[FNAMEMAX];

	if(sjis < 0x989e){
		sprintf(idxfname, "%s.iu1", m_sBaseFname);
		sprintf(datfname, "%s.du1", m_sBaseFname);
	}
	else {
		sprintf(idxfname, "%s.iu2", m_sBaseFname);
		sprintf(datfname, "%s.du2", m_sBaseFname);
	}
	if((m_fileIdx = fopen(idxfname, "rb")) == NULL){
		return(FAIL);
	}
	if((m_fileDat = fopen(datfname, "rb")) == NULL){
		fclose(m_fileIdx);
		return(FAIL);
	}
	return(0);
}

int CSCRFont::EsysFontOpen(UINT sjis)
{
	char	idxfname[FNAMEMAX], datfname[FNAMEMAX];

	if(sjis < 0x989e){
		sprintf(idxfname, "%s.id1", m_sBaseFname);
		sprintf(datfname, "%s.da1", m_sBaseFname);
	}
	else {
		sprintf(idxfname, "%s.id2", m_sBaseFname);
		sprintf(datfname, "%s.da2", m_sBaseFname);
	}
	if((m_fileIdx = fopen(idxfname, "rb")) == NULL){
		return(FAIL);
	}
	if((m_fileDat = fopen(datfname, "rb")) == NULL){
		fclose(m_fileIdx);
		return(FAIL);
	}
	return(0);
}

void CSCRFont::FontClose()
{
	if(m_fileIdx != NULL){
		fclose(m_fileIdx);
	}
	if(m_fileDat != NULL){
		fclose(m_fileDat);
	}
	m_fileIdx = NULL;
	m_fileDat = NULL;
}

int CSCRFont::UserFontNullSet(char idxfname[], char datfname[])
{
	char	buf[10], ebuf[10];
	int		len;

	ebuf[0] = 'E';
	buf[0] = '\0';

	if((m_fileIdx = fopen(idxfname, "wb")) == NULL){
		return(FAIL);
	}
	len = 30000;
	while(len--){
		if(1440 > len && len > 1310){
			if(fwrite(ebuf, 1, 1, m_fileIdx) != 1){
				fclose(m_fileIdx);
				return(FAIL);
			}
		}
		else{
			if(fwrite(buf, 1, 1, m_fileIdx) != 1){
				fclose(m_fileIdx);
				return(FAIL);
			}
		}
	}
	fclose(m_fileIdx);

	if((m_fileDat = fopen(datfname, "wb")) == NULL){
		return(FAIL);
	}
	len = 10;
	while(len--){
		if(fwrite(buf, 1, 1, m_fileDat) != 1){
			fclose(m_fileDat);
			return(FAIL);
		}
	}
	fclose(m_fileDat);

	return(0);
}

void CSCRFont::GetSeekPoint(UINT sjis, long *seekp)
{
	int		ku, ten;
	long	seekpoint;

	basesjtokuten(sjis, &ku, &ten);

	if(sjis <= 0x81ac){
		seekpoint = ((ku-1)*94+(ten-1)) * SCRFIDXSIZE;
	}
	else if(0x824f <= sjis && sjis <= 0x8258){
		seekpoint = ((ku-2)*94+(ten-2)) * SCRFIDXSIZE;
	}
	else if(0x8260 <= sjis && sjis <= 0x8279){
		seekpoint = ((ku-2)*94+(ten-9)) * SCRFIDXSIZE;
	}
	else if(0x8281 <= sjis && sjis <= 0x829a){
		seekpoint = ((ku-2)*94+(ten-15)) * SCRFIDXSIZE;
	}
	else if(0x829f <= sjis && sjis <= 0x82f1){
		seekpoint = ((ku-2)*94+(ten-19)) * SCRFIDXSIZE;
	}
	else if(0x8340 <= sjis && sjis <= 0x8396){
		seekpoint = ((ku-2)*94+(ten-30)) * SCRFIDXSIZE;
	}
	else if(0x839f <= sjis && sjis <= 0x83b6){
		seekpoint = ((ku-2)*94+(ten-38)) * SCRFIDXSIZE;
	}
	else if(0x83bf <= sjis && sjis <= 0x83d6){
		seekpoint = ((ku-2)*94+(ten-46)) * SCRFIDXSIZE;
	}
	else if(0x8440 <= sjis && sjis <= 0x8460){
		seekpoint = ((ku-2)*94+(ten-84)) * SCRFIDXSIZE;
	}
	else if(0x8470 <= sjis && sjis <= 0x8491){
		seekpoint = ((ku-3)*94+(ten-5)) * SCRFIDXSIZE;
	}
	else if(sjis < 0x889e){
		seekpoint = 0;
	}
	else if(sjis < 0x987f){
		seekpoint = ((ku-11)*94+(ten-18)) * SCRFIDXSIZE;
	}
	else{
		seekpoint = ((ku-47)*94+(ten-1))* SCRFIDXSIZE;
	}
	*seekp = seekpoint;
}

void CSCRFont::TrnsSCRToFSF1()
{
	int		pcnt, pp, atr;
	DBL		x, y;

	m_pFSF1Font->PointSetStart();
	pcnt = m_nDataLen / SCRSIZADD;
	for(pp = 0; pp < (pcnt-1); pp++){
		AtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
		if(EorChk(m_bFBuf, pp) == ON){
			m_pFSF1Font->AtrDXYCrtSet(atr, x, y);
			m_pFSF1Font->RejionSet();
		}
		else{
			m_pFSF1Font->AtrDXYCrtSet(atr, x, y);
			m_pFSF1Font->CrtPointInc();
		}
	}
	m_pFSF1Font->RejionSet();
	m_pFSF1Font->PointSetEnd();
}

void CSCRFont::SetDataLen(int pp)
{
	m_nDataLen = pp * SCRSIZADD;
}

void CSCRFont::AtrDXYGet(BYTE fBuf[], int pp, int *atr, DBL *x, DBL *y)
{
	int		bytep;
	int		tatr;

	bytep = pp * SCRSIZADD;
	tatr = fBuf[bytep+SCRATRADD];
	*x = fBuf[bytep+SCRXCDADD];
	*y = fBuf[bytep+SCRYCDADD];
	(*x) *= 4;
	(*y) *= 4;
	tatr = tatr & ~(SCRRENDMASK);
	if(tatr == (SCRARCMASK | SCRCSTARTMASK)){
		*atr = CURVATR;
	}
	else if(tatr == SCRCCNTMASK){
		*atr = CURVATR|CONTATR;
	}
	else if(tatr == SCRCSTARTMASK){
		*atr = CURVATR;
	}
	else{
		*atr = 0;
	}
}

int CSCRFont::EorChk(BYTE fBuf[], int pp)
{
	int		bytep;
	int		atr;

	bytep = pp * SCRSIZADD;
	atr = fBuf[bytep+SCRATRADD];
	if(atr & SCRRENDMASK || atr & SCRENDMASK){
		return(ON);
	}
	return(OFF);
}

void CSCRFont::TrnsCopyFSF1FBuf()
{
}

