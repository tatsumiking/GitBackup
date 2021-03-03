//
// Easy Scanner System Ver1.0
//
// ess  esf1font.cpp  ÇTÉoÉCÉgÇdÇrèëëÃèàóùä÷êî
//
//  Copyright (c) 1998 by THEON

#include	<windows.h>
#include	<stdio.h>
#include	<math.h>

#include	"..\stdef.h"
#include	"dlldef.h"
#include	"esf1font.h"
#include	"fsf1font.h"

#include	"esf1font.fnc"

FILE		*m_fileSF1;

void smslibGetFontPath(LPSTR fontpath);
DllExport void basesjtokuten(int far sjis, int far *ku, int far *ten);

void esf1fontFontNameSet(LPSTR pFont)
{
	char	fontpath[FNAMEMAX];
	int		num;

	smslibGetFontPath(fontpath);
	if(strncmp("AR", pFont, 2) == 0){
		if(strncmp("ARâpêî", pFont, 6) == 0){
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

int esf1fontDataLoad(UINT sjis)
{
	int	ku, ten;

	if(sjis < 0x8140 || 0xffff < sjis){
		m_nDataLen = 0;
		return(0);
	}
	if(0x9fff < sjis && sjis < 0xe021){
		m_nDataLen = 0;
		return(0);
	}
	basesjtokuten(sjis, &ku, &ten);
	if(esf1fontUserDataLoad(ku, ten) == FAIL){
		if(esf1fontEsysDataLoad(ku, ten) == FAIL){
			m_nDataLen = 0;
			return(FAIL);
		}
	}
	if(m_fTateGaki == ON){
		esf1fontTateGakiTrns(sjis);
	}
	return(0);
}

int esf1fontGaijiDataLoad(UINT sjis)
{
	int	ku, ten;

	if(sjis < 0x8140 || 0xffff < sjis){
		m_nDataLen = 0;
		return(0);
	}
	if(0x9fff < sjis && sjis < 0xe021){
		m_nDataLen = 0;
		return(0);
	}
	basesjtokuten(sjis, &ku, &ten);
	if(esf1fontUserDataLoad(ku, ten) == FAIL){
		m_nDataLen = 0;
		return(0);
	}
	if(m_fTateGaki == ON){
		esf1fontTateGakiTrns(sjis);
	}
	return(0);
}

int esf1fontUserDataSave(UINT sjis)
{
	int			ku, ten;
	long 		idxseekp, datseekp;
	int			datlen;
	e1idxtype	f1idx;
	e1dhtype	f1dhead;

	basesjtokuten(sjis, &ku, &ten);
	if(esf1fontUserFontWriteOpen() == FAIL){
		return(FAIL);
	}

	esf1fontIdxSeekPointGet(ku, ten, &idxseekp);
	fseek(m_fileSF1, idxseekp, 0);
	fread(&f1idx, sizeof(e1idxtype), 1, m_fileSF1);
	esf1fontDataLenSeekPointGet(f1idx, &datlen, &datseekp);

	if(datlen < (m_nDataLen + (int)sizeof(e1dhtype))){
		fseek(m_fileSF1, 0L, 2);
		datseekp = ftell(m_fileSF1);
	}
	f1dhead.headlen = 0;
	f1dhead.datalen = m_nDataLen;
	f1dhead.sx = 0;
	f1dhead.sy = 0;
	f1dhead.ex = ESF1MESH / MINIMAXTIME - 1;
	f1dhead.ey = ESF1MESH / MINIMAXTIME - 1;

	esf1fontDataLenSeekPointSet(&f1idx, m_nDataLen + sizeof(e1dhtype), datseekp);
	fseek(m_fileSF1, datseekp, 0);
	if(fwrite(&f1dhead, sizeof(e1dhtype), 1, m_fileSF1) < 1){
		esf1fontFontClose();
		return(FAIL);
	}
	if(fwrite(m_bFBuf, 1, m_nDataLen, m_fileSF1) < (UINT)m_nDataLen){
		esf1fontFontClose();
		return(FAIL);
	}
	fseek(m_fileSF1, idxseekp, 0);
	if(fwrite(&f1idx, sizeof(e1idxtype), 1, m_fileSF1) < 1){
		esf1fontFontClose();
		return(FAIL);
	}
	esf1fontFontClose();
	return(0);
}

int esf1fontUserDataExist(UINT sjis)
{
	int			ku, ten;
	long 		idxseekp, datseekp;
	int			datlen;
	e1idxtype	f1idx;

	basesjtokuten(sjis, &ku, &ten);
	if(esf1fontUserFontWriteOpen() == FAIL){
		return(FAIL);
	}

	esf1fontIdxSeekPointGet(ku, ten, &idxseekp);
	fseek(m_fileSF1, idxseekp, 0);
	fread(&f1idx, sizeof(e1idxtype), 1, m_fileSF1);
	esf1fontDataLenSeekPointGet(f1idx, &datlen, &datseekp);
	if(datlen == 0){
		esf1fontFontClose();
		return(FAIL);
	}
	esf1fontFontClose();
	return(0);
}

int esf1fontUserDataDelete(UINT sjis)
{
	int			ku, ten;
	long 		idxseekp, datseekp;
	int			datlen;
	e1idxtype	f1idx;

	basesjtokuten(sjis, &ku, &ten);
	if(esf1fontUserFontWriteOpen() == FAIL){
		return(FAIL);
	}

	esf1fontIdxSeekPointGet(ku, ten, &idxseekp);
	fseek(m_fileSF1, idxseekp, 0);
	fread(&f1idx, sizeof(e1idxtype), 1, m_fileSF1);
	datlen = 0;
	esf1fontDataLenSeekPointSet(&f1idx, datlen, datseekp);
	fseek(m_fileSF1, idxseekp, 0);
	if(fwrite(&f1idx, sizeof(e1idxtype), 1, m_fileSF1) < 1){
		esf1fontFontClose();
		return(FAIL);
	}
	esf1fontFontClose();
	return(0);
}

int esf1fontUserDataLoad(int ku, int ten)
{
	long 		idxseekp, datseekp;
	int			datlen;
	e1idxtype	f1idx;
	e1dhtype	f1dhead;

	if(esf1fontUserFontOpen() == FAIL){
		return(FAIL);
	}

	esf1fontIdxSeekPointGet(ku, ten, &idxseekp);
	fseek(m_fileSF1, idxseekp, 0);
	fread(&f1idx, sizeof(e1idxtype), 1, m_fileSF1);
	esf1fontDataLenSeekPointGet(f1idx, &datlen, &datseekp);
	if(datlen == 0){
		esf1fontFontClose();
		return(FAIL);
	}
	fseek(m_fileSF1, datseekp, 0);
	if(fread(&f1dhead, sizeof(e1dhtype), 1, m_fileSF1) != 1){
		esf1fontFontClose();
		return(FAIL);
	}
	m_nDataLen = f1dhead.datalen;
	if(fread(m_bFBuf, 1, m_nDataLen, m_fileSF1) < (UINT)m_nDataLen){
		esf1fontFontClose();
		return(FAIL);
	}
	esf1fontFontClose();
	return(0);
}

int esf1fontEsysDataLoad(int ku, int ten)
{
	long 		idxseekp, datseekp;
	int			datlen;
	e1idxtype	f1idx;
	e1dhtype	f1dhead;

	if(esf1fontEsysFontOpen() == FAIL){
		return(FAIL);
	}

	esf1fontIdxSeekPointGet(ku, ten, &idxseekp);
	fseek(m_fileSF1, idxseekp, 0);
	fread(&f1idx, sizeof(e1idxtype), 1, m_fileSF1);
	esf1fontDataLenSeekPointGet(f1idx, &datlen, &datseekp);
	if(datlen == 0){
		esf1fontFontClose();
		return(FAIL);
	}
	fseek(m_fileSF1, datseekp, 0);
	if(fread(&f1dhead, sizeof(e1dhtype), 1, m_fileSF1) != 1){
		esf1fontFontClose();
		return(FAIL);
	}
	m_nDataLen = f1dhead.datalen;
	if(fread(m_bFBuf, 1, m_nDataLen, m_fileSF1) < (UINT)m_nDataLen){
		esf1fontFontClose();
		return(FAIL);
	}
	esf1fontFontClose();
	return(0);
}

int esf1fontUserFontWriteOpen()
{
	char	ffname[FNAMEMAX];

	sprintf(ffname, "%s.uf1", m_sBaseFname);
	if((m_fileSF1 = fopen(ffname, "r+b")) == NULL){
		esf1fontUserFontNullSet();
		if((m_fileSF1 = fopen(ffname, "r+b")) == NULL){
			return(FAIL);
		}
	}
	return(0);
}

int esf1fontUserFontOpen()
{
	char	ffname[FNAMEMAX];

	sprintf(ffname, "%s.uf1", m_sBaseFname);
	if((m_fileSF1 = fopen(ffname, "rb")) == NULL){
		return(FAIL);
	}
	return(0);
}

int esf1fontEsysFontOpen()
{
	char	ffname[FNAMEMAX];

	sprintf(ffname, "%s.sf1", m_sBaseFname);
	if((m_fileSF1 = fopen(ffname, "rb")) == NULL){
		return(FAIL);
	}
	return(0);
}

void esf1fontFontClose()
{
	fclose(m_fileSF1);
}

int esf1fontUserFontNullSet()
{
	char	ffname[FNAMEMAX];
	char	buf[10], ebuf[10];
	long	len;

	ebuf[0] = 'E';
	buf[0] = '\0';

	sprintf(ffname, "%s.uf1", m_sBaseFname);
	if((m_fileSF1 = fopen(ffname, "wb")) == NULL){
		return(FAIL);
	}
	len = ESF1DSEEKP;
	while(len--){
		if(fwrite(buf, 1, 1, m_fileSF1) != 1){
			fclose(m_fileSF1);
			return(FAIL);
		}
	}
	fclose(m_fileSF1);
	return(0);
}

void esf1fontIdxSeekPointGet(int ku, int ten, long *seekp)
{
	*seekp = (long)((ku-1)*100+(ten-1)) * (long)(sizeof(e1idxtype)) + sizeof(e1fhtype);
}

void esf1fontDataLenSeekPointGet(e1idxtype f1idx, int *dlen, long *dseekp)
{
	*dlen = f1idx.dlen;
	*dseekp = f1idx.dseekp + ESF1DSEEKP;
}

void esf1fontDataLenSeekPointSet(e1idxtype *f1idx, int dlen, long dseekp)
{
	if(dlen < 0){
		dlen = 0;
	}
	if(dseekp < ESF1DSEEKP){
		dseekp = ESF1DSEEKP;
	}
	f1idx->dlen = dlen;
	f1idx->dseekp = dseekp - ESF1DSEEKP;
}

void esf1fontTateGakiTrns(UINT sjis)
{
	int		eofflag, pcnt, pp, atr;
	DBL		x, y, xx, yy, minix, maxx, movex;

	pcnt = m_nDataLen / ESF1SIZADD;
	if(0x815C <= sjis && sjis <= 0x8186){
		for(pp = 0; pp < pcnt; pp++){
			eofflag = esf1fontEorChk(m_bFBuf, pp);
			esf1fontAtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
			xx = ESF1MESH - y;
			yy = x;
			esf1fontAtrDXYSet(m_bFBuf, pp, atr, xx, yy);
			if(eofflag == ON){
				esf1fontEorOn(m_bFBuf, pp);
			}
		}
	}
	else if(sjis == 0x8141 || sjis == 0x8142){
		minix = ESF1MESH;
		maxx = 0;
		for(pp = 0; pp < pcnt; pp++){
			esf1fontAtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
			if(maxx < x){
				maxx = x;
			}
			if(minix > x){
				minix = x;
			}
		}
		movex = ESF1MESH - maxx - minix;
		for(pp = 0; pp < pcnt; pp++){
			eofflag = esf1fontEorChk(m_bFBuf, pp);
			esf1fontAtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
			xx = x + movex;
			yy = y + movex;
			esf1fontAtrDXYSet(m_bFBuf, pp, atr, xx, yy);
			if(eofflag == ON){
				esf1fontEorOn(m_bFBuf, pp);
			}
		}
	}
	else if(sjis == 0x815b){
		for(pp = 0; pp < pcnt; pp++){
			eofflag = esf1fontEorChk(m_bFBuf, pp);
			esf1fontAtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
			xx = y;
			yy = x;
			esf1fontAtrDXYSet(m_bFBuf, pp, atr, xx, yy);
			if(eofflag == ON){
				esf1fontEorOn(m_bFBuf, pp);
			}
		}
	}
}

void esf1fontSetDataLen(int pp)
{
	m_nDataLen = pp * ESF1SIZADD;
}

void esf1fontAtrDXYGet(BYTE fBuf[], int pp, int *atr, DBL *x, DBL *y)
{
	int		bytep;
	int		*xt, *yt;
	double	dx, dy;

	bytep = pp * ESF1SIZADD;
	xt = (int *)(&(fBuf[bytep+ESF1XCDADD]));
	yt = (int *)(&(fBuf[bytep+ESF1YCDADD]));
	*atr = (fBuf[bytep+ESF1ATRADD] & 0x0e);
	dx = *xt;
	dy = *yt;
	*x = dx;
	*y = dy;
}

void esf1fontAtrDXYSet(BYTE fBuf[], int pp, int atr, DBL x, DBL y)
{
	int		bytep;
	int		*xt, *yt;

	bytep = pp * ESF1SIZADD;
	xt = (int *)(&(fBuf[bytep+ESF1XCDADD]));
	yt = (int *)(&(fBuf[bytep+ESF1YCDADD]));
	*xt = (int)(x);
	*yt = (int)(y);
	fBuf[bytep+ESF1ATRADD] = atr & 0x0e;
}

int esf1fontEorChk(BYTE fBuf[], int pp)
{
	int		bytep;

	bytep = pp * ESF1SIZADD;
	if(fBuf[bytep+ESF1ATRADD] & ESF1EORMASK){
		return(ON);
	}
	else{
		return(OFF);
	}
}

void esf1fontEorOn(BYTE fBuf[], int pp)
{
	int		bytep;

	bytep = pp * ESF1SIZADD;
	fBuf[bytep+ESF1ATRADD] |= ESF1EORMASK;
}

int esf1fontRejionChk(BYTE fBuf[], int pp)
{
	int		bytep;

	bytep = pp * ESF1SIZADD;
	if(fBuf[bytep+ESF1ATRADD] & ESF1REJIONMASK){
		return(ON);
	}
	else{
		return(OFF);
	}
}

void esf1fontRejionOff(BYTE fBuf[], int pp)
{
	int		bytep;

	bytep = pp * ESF1SIZADD;
	fBuf[bytep+ESF1ATRADD] &= ~ESF1REJIONMASK;
}

void esf1fontRejionOn(BYTE fBuf[], int pp)
{
	int		bytep;

	bytep = pp * ESF1SIZADD;
	fBuf[bytep+ESF1ATRADD] |= ESF1REJIONMASK;
}

