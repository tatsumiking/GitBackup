
//
// Easy Scanner System Ver1.0
//
// ess  esf0font.cpp  ÇRÉoÉCÉgÇdÇrèëëÃèàóùä÷êî
//
//  Copyright (c) 1998 by THEON

#include	<windows.h>
#include	<stdio.h>
#include	<math.h>

#include	"..\stdef.h"
#include	"dlldef.h"
#include	"esf0font.h"
#include	"fsf1font.h"

#include	"esf0font.fnc"
#include	"fsf1font.fnc"

#define	FIDXSIZE	6

FILE		*m_fileIdx, *m_fileDat;
BYTE		m_bEBuf[3072];
int			m_nEDataLen;

void smslibGetFontPath(LPSTR fontpath);

void esf0fontFontNameSet(LPSTR pFont)
{
	char	fontpath[FNAMEMAX];
	int		num;

	smslibGetFontPath(fontpath);
	if(strncmp("Ç`", pFont, 2) == 0){
		if(strncmp("Ç`âpêî", pFont, 6) == 0){
			num = atoi(&(pFont[6]));
			if(num <= 60){
				sprintf(m_sBaseFname, "%s\\HE1\\%s", fontpath, pFont);
			}
			else{
				sprintf(m_sBaseFname, "%s\\HE2\\%s", fontpath, pFont);
			}
		}
		else{
			sprintf(m_sBaseFname, "%s\\HK\\%s", fontpath, pFont);
		}
	}
	else{
		sprintf(m_sBaseFname, "%s\\%s", fontpath, pFont);
	}
}

void esf0fontSF1TrnsDataLoad(UINT sjis)
{
	esf0fontDataLoad(sjis);
	esf0fontTrnsESF0ToFSF1();
}

void esf0fontDFileReadOpen(char basefname[])
{
	char	idxfname[FNAMEMAX], datfname[FNAMEMAX];

	sprintf(idxfname, "%s.tbl", basefname);
	sprintf(datfname, "%s.dat", basefname);
	if((m_fileIdx = fopen(idxfname, "rb")) == NULL){
		m_fileIdx = NULL;
		m_fileDat = NULL;
		return;
	}
	if((m_fileDat = fopen(datfname, "rb")) == NULL){
		fclose(m_fileIdx);
		m_fileIdx = NULL;
		m_fileDat = NULL;
		return;
	}
	return;
}

void esf0fontDFileDataLoad(int datano)
{
	long	seekpoint;
	fidxtype	fidx;

	seekpoint = (long)datano * (long)(FIDXSIZE);
	fseek(m_fileIdx, seekpoint, 0);
	fread(&fidx, 1, FIDXSIZE, m_fileIdx);
	if(fidx.len == 0){
		m_nEDataLen = 0;
		return;
	}
	m_nEDataLen = fidx.len;
	fseek(m_fileDat, fidx.seekp, 0);
	if(fread(m_bEBuf, 1, m_nEDataLen, m_fileDat) < (UINT)m_nEDataLen){
		m_nEDataLen = 0;
		return;
	}
}

void esf0fontDFileClose()
{
	if(m_fileIdx != NULL){
		fclose(m_fileIdx);
	}
	if(m_fileDat != NULL){
		fclose(m_fileDat);
	}
}

void esf0fontDataLoad(UINT sjis)
{
	if(sjis < 0x8140 || 0xffff < sjis){
		m_nEDataLen = 0;
		return;
	}
	if(0x9fff < sjis && sjis < 0xe021){
		m_nEDataLen = 0;
		return;
	}
	if(esf0fontUserDataLoad(sjis) == FAIL){
		if(esf0fontEsysDataLoad(sjis) == FAIL){
			m_nEDataLen = 0;
			return;
		}
	}
}

int esf0fontUserDataSave(UINT sjis)
{
	long 		seekpoint;
	fidxtype	fidx;

	if(esf0fontUserFontWriteOpen(sjis) == FAIL){
		return(FAIL);
	}

	esf0fontGetSeekPoint(sjis, &seekpoint);
	fseek(m_fileIdx, seekpoint, 0);
	fread(&fidx, 1, FIDXSIZE, m_fileIdx);
	if(fidx.len < m_nEDataLen){
		fseek(m_fileDat, 0L, 2);
		fidx.seekp = ftell(m_fileDat);
	}
	fidx.len = m_nEDataLen;
	fseek(m_fileDat, fidx.seekp, 0);
	if(fwrite(m_bEBuf, 1, m_nEDataLen, m_fileDat) < (UINT)m_nEDataLen){
		esf0fontFontClose();
		return(FAIL);
	}
	fseek(m_fileIdx, seekpoint, 0);
	if(fwrite(&fidx, 1, FIDXSIZE, m_fileIdx) < FIDXSIZE){
		esf0fontFontClose();
		return(FAIL);
	}
	esf0fontFontClose();
	return(0);
}

int esf0fontUserDataExist(UINT sjis)
{
	long 		seekpoint;
	fidxtype	fidx;

	if(esf0fontUserFontWriteOpen(sjis) == FAIL){
		return(FAIL);
	}

	esf0fontGetSeekPoint(sjis, &seekpoint);
	fseek(m_fileIdx, seekpoint, 0);
	fread(&fidx, 1, FIDXSIZE, m_fileIdx);
	if(fidx.len == 0){
		esf0fontFontClose();
		return(FAIL);
	}
	esf0fontFontClose();
	return(0);
}

int esf0fontUserDataDelete(UINT sjis)
{
	long 		seekpoint;
	fidxtype	fidx;

	if(esf0fontUserFontWriteOpen(sjis) == FAIL){
		return(FAIL);
	}
	esf0fontGetSeekPoint(sjis, &seekpoint);
	fseek(m_fileIdx, seekpoint, 0);
	fread(&fidx, FIDXSIZE, 1, m_fileIdx);
	fidx.len = 0;
	fidx.seekp = 0L;
	fseek(m_fileIdx, seekpoint, 0);
	if(fwrite(&fidx, 1, FIDXSIZE, m_fileIdx) < FIDXSIZE){
		esf0fontFontClose();
		return(FAIL);
	}
	esf0fontFontClose();
	return(0);
}

int esf0fontUserDataLoad(UINT sjis)
{
	long 		seekpoint;
	fidxtype	fidx;

	if(esf0fontUserFontOpen(sjis) == FAIL){
		return(FAIL);
	}

	esf0fontGetSeekPoint(sjis, &seekpoint);
	fseek(m_fileIdx, seekpoint, 0);
	fread(&fidx, 1, FIDXSIZE, m_fileIdx);
	if(fidx.len == 0){
		esf0fontFontClose();
		return(FAIL);
	}
	m_nEDataLen = fidx.len;
	fseek(m_fileDat, fidx.seekp, 0);
	if(fread(m_bEBuf, 1, m_nEDataLen, m_fileDat) < (UINT)m_nEDataLen){
		esf0fontFontClose();
		return(FAIL);
	}
	if(m_fTateGaki == ON){
		esf0fontTateGakiTrns(sjis);
	}
	esf0fontFontClose();
	return(0);
}

int esf0fontEsysDataLoad(UINT sjis)
{
	long 		seekpoint;
	fidxtype	fidx;

	if(esf0fontEsysFontOpen(sjis) == FAIL){
		return(FAIL);
	}

	esf0fontGetSeekPoint(sjis, &seekpoint);
	fseek(m_fileIdx, seekpoint, 0);
	fread(&(fidx.len), sizeof(short), 1, m_fileIdx); 
	fseek(m_fileIdx, seekpoint+2, 0);
	fread(&(fidx.seekp), sizeof(long), 1, m_fileIdx); 
	if(fidx.len == 0){
		esf0fontFontClose();
		m_nEDataLen = 0;
		return(0);
	}
	m_nEDataLen = fidx.len;
	fseek(m_fileDat, fidx.seekp, 0);
	if(fread(m_bEBuf, 1, m_nEDataLen, m_fileDat) < (UINT)m_nEDataLen){
		esf0fontFontClose();
		m_nEDataLen = 0;
		return(0);
	}
	if(m_fTateGaki == ON){
		esf0fontTateGakiTrns(sjis);
	}
	esf0fontFontClose();
	return(0);
}

int esf0fontUserFontWriteOpen(UINT sjis)
{
	char	idxfname[FNAMEMAX], datfname[FNAMEMAX];

	if(sjis < 0x989e){
		sprintf(idxfname, "%s.tbu", m_sBaseFname);
		sprintf(datfname, "%s.dau", m_sBaseFname);
	}
	else{
		sprintf(idxfname, "%s.t2u", m_sBaseFname);
		sprintf(datfname, "%s.d2u", m_sBaseFname);
	}
	if((m_fileIdx = fopen(idxfname, "r+b")) == NULL){
		esf0fontUserFontNullSet(idxfname, datfname);
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

int esf0fontUserFontOpen(UINT sjis)
{
	char	idxfname[FNAMEMAX], datfname[FNAMEMAX];

	if(sjis < 0x989e){
		sprintf(idxfname, "%s.tbu", m_sBaseFname);
		sprintf(datfname, "%s.dau", m_sBaseFname);
	}
	else{
		sprintf(idxfname, "%s.t2u", m_sBaseFname);
		sprintf(datfname, "%s.d2u", m_sBaseFname);
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

int esf0fontEsysFontOpen(UINT sjis)
{
	char	idxfname[FNAMEMAX], datfname[FNAMEMAX];

	if(sjis < 0x989e){
		sprintf(idxfname, "%s.tbl", m_sBaseFname);
		sprintf(datfname, "%s.dat", m_sBaseFname);
	}
	else{
		sprintf(idxfname, "%s.tb2", m_sBaseFname);
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

void esf0fontFontClose()
{
	fclose(m_fileIdx);
	fclose(m_fileDat);
}

int esf0fontUserFontNullSet(char idxfname[], char datfname[])
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

void esf0fontGetSeekPoint(UINT sjis, long *seekp)
{
	long	seekpoint, hcd, lcd;

	hcd = (sjis >> 8) & 0xff;
	lcd = (sjis) & 0xff;

	if(sjis < 0x989e){
		seekpoint = ((hcd-0x81)*0xc0+lcd-0x40)* FIDXSIZE;
	}
	else if(hcd < 0xe0){
		seekpoint = ((hcd-0x98)*0xc0+lcd-0x40)* FIDXSIZE;
	}
	else{
		seekpoint = ((hcd-0xd8)*0xc0+lcd-0x40)* FIDXSIZE;
	}
	*seekp = seekpoint;
}

void esf0fontTateGakiTrns(UINT sjis)
{
	int		eofflag, pcnt, pp, atr;
	DBL		x, y, xx, yy, minix, maxx, movex;

	pcnt = m_nEDataLen / ESF0SIZADD;
	if(0x815C <= sjis && sjis <= 0x8186){
		for(pp = 0; pp < pcnt; pp++){
			eofflag = esf0fontEorChk(m_bEBuf, pp);
			esf0fontAtrDXYGet(m_bEBuf, pp, &atr, &x, &y);
			xx = ESF0MESH - y;
			yy = x;
			esf0fontAtrDXYSet(m_bEBuf, pp, atr, xx, yy);
			if(eofflag == ON){
				esf0fontEorOn(m_bEBuf, pp);
			}
		}
	}
	else if(sjis == 0x8141 || sjis == 0x8142){
		minix = ESF0MESH;
		maxx = 0;
		for(pp = 0; pp < pcnt; pp++){
			esf0fontAtrDXYGet(m_bEBuf, pp, &atr, &x, &y);
			if(maxx < x){
				maxx = x;
			}
			if(minix > x){
				minix = x;
			}
		}
		movex = ESF0MESH - maxx - minix;
		for(pp = 0; pp < pcnt; pp++){
			eofflag = esf0fontEorChk(m_bEBuf, pp);
			esf0fontAtrDXYGet(m_bEBuf, pp, &atr, &x, &y);
			xx = x + movex;
			yy = y;
			esf0fontAtrDXYSet(m_bEBuf, pp, atr, xx, yy);
			if(eofflag == ON){
				esf0fontEorOn(m_bEBuf, pp);
			}
		}
	}
	else if(sjis == 0x815b){
		for(pp = 0; pp < pcnt; pp++){
			eofflag = esf0fontEorChk(m_bEBuf, pp);
			esf0fontAtrDXYGet(m_bEBuf, pp, &atr, &x, &y);
			xx = y;
			yy = x;
			esf0fontAtrDXYSet(m_bEBuf, pp, atr, xx, yy);
			if(eofflag == ON){
				esf0fontEorOn(m_bEBuf, pp);
			}
		}
	}
}

void esf0fontTrnsESF0ToFSF1()
{
	int		pcnt, flag, pp, tp, atr;
	DBL		x, y;

	fsf1fontPointSetStart();
	pcnt = m_nEDataLen / ESF0SIZADD;
	for(pp = 0; pp < pcnt; pp++){
		esf0fontAtrDXYGet(m_bEBuf, pp, &atr, &x, &y);
		if(esf0fontEorChk(m_bEBuf, pp) == ON){
			tp = pp;
			flag = esf0fontRejionChk(m_bEBuf, &tp);
			if(tp == pp){
				fsf1fontAtrDXYCrtSet(atr, x, y);
				fsf1fontCrtPointInc();
			}
			if(flag == ON){
				fsf1fontRejionSet();
			}
			else{
				fsf1fontNoRejionSet();
			}
		}
		else{
			fsf1fontAtrDXYCrtSet(atr, x, y);
			fsf1fontCrtPointInc();
		}
	}
	fsf1fontPointSetEnd();
}

void esf0fontSetDataLen(int pp)
{
	m_nEDataLen = pp * ESF0SIZADD;
}

void esf0fontAtrDXYGet(BYTE fBuf[], int pp, int *atr, DBL *x, DBL *y)
{
	int		bytep;
	int		bitx, bity, ix, iy;

	bytep = pp * ESF0SIZADD;
	*atr = (fBuf[bytep+ESF0ATRADD] & 0x0e);
	bitx = (fBuf[bytep+ESF0ATRADD] & 0xc0) >> 6;
	bity = (fBuf[bytep+ESF0ATRADD] & 0x30) >> 4;
	ix = fBuf[bytep+ESF0XCDADD];
	iy = fBuf[bytep+ESF0YCDADD];
	*x = (double)(((ix << 2) & 0x3fc) | (bitx & 0x003));
	*y = (double)(((iy << 2) & 0x3fc) | (bity & 0x003));
}

void esf0fontAtrDXYSet(BYTE fBuf[], int pp, int atr, DBL x, DBL y)
{
	int		bytep;
	int		bitx, bity, bitxy;
	int		ix, iy;

	if(x < 0.0){
		x = 0;
	}
	if(y < 0.0){
		y = 0;
	}
	if(1023.0 < x){
		x = 1023.0;
	}
	if(1023.0 < y){
		y = 1023.0;
	}
	bytep = pp * ESF0SIZADD;
	ix = (int)x; iy = (int)y;
	bitx = ix & 0x003;
	bity = iy & 0x003;
	bitxy = ((bitx << 6) & 0xc0) | ((bity << 4) & 0x30);
	fBuf[bytep+ESF0ATRADD] = (atr & 0x0e) | (bitxy & 0xf0);
	fBuf[bytep+ESF0XCDADD] = ((ix >> 2) & 0x00ff);
	fBuf[bytep+ESF0YCDADD] = ((iy >> 2) & 0x00ff);
}

int esf0fontEorChk(BYTE fBuf[], int pp)
{
	int		bytep;

	bytep = pp * ESF0SIZADD;
	if(fBuf[bytep+ESF0ATRADD] & F0EORMASK){
		return(ON);
	}
	else{
		return(OFF);
	}
}

void esf0fontEorOn(BYTE fBuf[], int pp)
{
	int		bytep;

	bytep = pp * ESF0SIZADD;
	fBuf[bytep+ESF0ATRADD] |= F0EORMASK;
}

int esf0fontRejionChk(BYTE fBuf[], int *pp)
{
	int		p0, p1;
	int		atr0, atr1;
	DBL		x0, y0, x1, y1;

	p0 = (*pp) - 1;
	p1 = (*pp);
	esf0fontAtrDXYGet(fBuf, p1, &atr1, &x1, &y1);
	if(atr1 == 0x0e){
		esf0fontAtrDXYGet(fBuf, p0, &atr0, &x0, &y0);
		if(x1 == x0 && y1 == y0){
			(*pp) = p0;
			return(OFF);
		}
		else{
			(*pp) = p0;
			return(ON);
		}
	}
	else{
		return(ON);
	}
}

int esf0fontRejionOff(BYTE fBuf[], int pp)
{
	int		p0, p1;
	int		atr;
	DBL		x, y;

	p0 = pp;
	p1 = pp + 1;
	esf0fontAtrDXYGet(fBuf, p0, &atr, &x, &y);
	atr = 0x0e;
	esf0fontAtrDXYSet(fBuf, p1, atr, x, y);
	return(pp+1);
}

int esf0fontRejionOn(BYTE fBuf[], int pp)
{
	return(pp);
}

