
//
// Easy Scanner System
//
// object esf1font.cpp  ５バイトＥＳ書体処理関数
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include <stdio.h>
#include "esf1font.h"

extern "C" {
	void basesjtokuten(int far sjis, int far *ku, int far *ten);
	void mojienvgetfontpath(LPSTR fontpath);
	BOOL  pdrwatrarcchk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrbezechk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrcurvechk(int atr1);
	int plgnfillerrorchk();
	void plgnfillerror(int flag);
	void plgnfillstart();
	void plgnfillrejionstart();
	void plgnfillrejionend();
	void plgnfilldraw(HDC PaintDC);
	void plgnfillmove(HDC PaintDC, DBL x1, DBL y1);
	void plgnfillline(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2);
	void  plgnfillarc(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void  plgnfillbeze(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	void  plgnfillfreecurve(HDC PaintDC,int atr0,int atr1,int atr2,DBL x0,DBL y0,DBL x1,DBL y1,DBL x2,DBL y2,DBL x3,DBL y3);
}

IMPLEMENT_SERIAL(CESF1Font, CObject, 0)
CESF1Font::CESF1Font()
{
}

CESF1Font::CESF1Font(int nDummy)
{
	m_fileSF1 = NULL;
	m_pFSF1Font = NULL;
	m_nDataLen = 0;
	m_nTateYoko = YOKOGAKI;
	m_sBaseFname[0] = '\0';

}

CESF1Font::~CESF1Font()
{
}

void CESF1Font::Serialize(CArchive& ar)
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

LPBYTE CESF1Font::GetbFBufAdrs()
{
	return(m_bFBuf);
}

int CESF1Font::FontMeshGet()
{
	return(ESF1MESH);
}

void CESF1Font::FSF1ClassSet(CFSF1Font* pFSF1Font)
{
	m_pFSF1Font = pFSF1Font;
}

void CESF1Font::FontNameSet(LPSTR pFont)
{
	char	fontpath[FNAMEMAX];
	int		num;

	mojienvgetfontpath(fontpath);
	if(strncmp("AR", pFont, 2) == 0){
		if(strncmp("AR英数", pFont, 6) == 0){
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

void CESF1Font::SetTateYoko(int flag)
{
	m_nTateYoko = flag;
}

int CESF1Font::SF1TrnsDataLoad(UINT sjis)
{
	int		ret;

	ret = DataLoad(sjis);
	TrnsESF1ToFSF1();
	return(ret);
}

int CESF1Font::SF1TrnsGaijiDataLoad(UINT sjis)
{
	int		ret;

	ret = GaijiDataLoad(sjis);
	if(ret == FAIL){
		return(FAIL);
	}
	TrnsESF1ToFSF1();
	return(0);
}

void CESF1Font::SF1TrnsDataSave(UINT sjis)
{
	TrnsCopyFSF1FBuf();
	UserDataSave(sjis);
}

int CESF1Font::DataLoad(UINT sjis)
{
	int	ku, ten;

	if(sjis < 0x0100){
		m_nDataLen = 0;
		return(FAIL);
	}
	if(sjis < 0x8140 || 0xffff < sjis){
		m_nDataLen = 0;
		return(0);
	}
	if(0x9fff < sjis && sjis < 0xe021){
		m_nDataLen = 0;
		return(0);
	}
	basesjtokuten(sjis, &ku, &ten);
	if(UserDataLoad(ku, ten) == FAIL){
		if(EsysDataLoad(ku, ten) == FAIL){
			m_nDataLen = 0;
			return(FAIL);
		}
	}
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		TateGakiTrns(sjis);
	}
	return(0);
}

int CESF1Font::GaijiDataLoad(UINT sjis)
{
	int	ku, ten;

	if(sjis < 0x8140 || 0xffff < sjis){
		return(FAIL);
	}
	if(0x9fff < sjis && sjis < 0xe021){
		return(FAIL);
	}
	basesjtokuten(sjis, &ku, &ten);
	if(UserDataLoad(ku, ten) == FAIL){
		return(FAIL);
	}
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		TateGakiTrns(sjis);
	}
	return(0);
}

int CESF1Font::UserDataSave(UINT sjis)
{
	int			ku, ten;
	long 		idxseekp, datseekp;
	int			datlen;
	e1idxtype	f1idx;
	e1dhtype	f1dhead;

	if(m_nDataLen == 0){
		return(0);
	}
	basesjtokuten(sjis, &ku, &ten);
	if(UserFontWriteOpen(ku, ten) == FAIL){
		return(FAIL);
	}

	IdxSeekPointGet(ku, ten, &idxseekp);
	fseek(m_fileSF1, idxseekp, 0);
	fread(&f1idx, sizeof(e1idxtype), 1, m_fileSF1);
	DataLenSeekPointGet(f1idx, &datlen, &datseekp);

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

	DataLenSeekPointSet(&f1idx, m_nDataLen + sizeof(e1dhtype), datseekp);
	fseek(m_fileSF1, datseekp, 0);
	if(fwrite(&f1dhead, sizeof(e1dhtype), 1, m_fileSF1) < 1){
		FontClose();
		return(FAIL);
	}
	if(fwrite(m_bFBuf, 1, m_nDataLen, m_fileSF1) < (UINT)m_nDataLen){
		FontClose();
		return(FAIL);
	}
	fseek(m_fileSF1, idxseekp, 0);
	if(fwrite(&f1idx, sizeof(e1idxtype), 1, m_fileSF1) < 1){
		FontClose();
		return(FAIL);
	}
	FontClose();
	return(0);
}

int CESF1Font::UserDataExist(UINT sjis)
{
	int			ku, ten;
	long 		idxseekp, datseekp;
	int			datlen;
	e1idxtype	f1idx;

	basesjtokuten(sjis, &ku, &ten);
	if(UserFontWriteOpen(ku, ten) == FAIL){
		return(FAIL);
	}

	IdxSeekPointGet(ku, ten, &idxseekp);
	fseek(m_fileSF1, idxseekp, 0);
	fread(&f1idx, sizeof(e1idxtype), 1, m_fileSF1);
	DataLenSeekPointGet(f1idx, &datlen, &datseekp);
	if(datlen == 0){
		FontClose();
		return(FAIL);
	}
	FontClose();
	return(datlen);
}

int CESF1Font::UserDataDelete(UINT sjis)
{
	int			ku, ten;
	long 		idxseekp, datseekp;
	int			datlen;
	e1idxtype	f1idx;

	basesjtokuten(sjis, &ku, &ten);
	if(UserFontWriteOpen(ku, ten) == FAIL){
		return(FAIL);
	}

	IdxSeekPointGet(ku, ten, &idxseekp);
	fseek(m_fileSF1, idxseekp, 0);
	fread(&f1idx, sizeof(e1idxtype), 1, m_fileSF1);
	datlen = 0;
	datseekp = 0L;
	DataLenSeekPointSet(&f1idx, datlen, datseekp);
	fseek(m_fileSF1, idxseekp, 0);
	if(fwrite(&f1idx, sizeof(e1idxtype), 1, m_fileSF1) < 1){
		FontClose();
		return(FAIL);
	}
	FontClose();
	return(0);
}

int CESF1Font::UserDataLoad(int ku, int ten)
{
	long 		idxseekp, datseekp;
	int			datlen;
	e1idxtype	f1idx;
	e1dhtype	f1dhead;

	if(UserFontOpen(ku, ten) == FAIL){
		return(FAIL);
	}

	IdxSeekPointGet(ku, ten, &idxseekp);
	if(fseek(m_fileSF1, idxseekp, 0) != 0){
		FontClose();
		return(FAIL);
	}
	fread(&f1idx, sizeof(e1idxtype), 1, m_fileSF1);
	DataLenSeekPointGet(f1idx, &datlen, &datseekp);
	if(datlen == 0){
		FontClose();
		return(FAIL);
	}
	if(fseek(m_fileSF1, datseekp, 0) != 0){
		FontClose();
		return(FAIL);
	}
	if(fread(&f1dhead, sizeof(e1dhtype), 1, m_fileSF1) != 1){
		FontClose();
		return(FAIL);
	}
	m_nDataLen = f1dhead.datalen;
	if(fread(m_bFBuf, 1, m_nDataLen, m_fileSF1) < (UINT)m_nDataLen){
		FontClose();
		return(FAIL);
	}
	FontClose();
	return(0);
}

int CESF1Font::EsysDataLoad(int ku, int ten)
{
	long 		idxseekp, datseekp;
	int			datlen;
	e1idxtype	f1idx;
	e1dhtype	f1dhead;

	if(EsysFontOpen(ku, ten) == FAIL){
		return(FAIL);
	}

	IdxSeekPointGet(ku, ten, &idxseekp);
	if(fseek(m_fileSF1, idxseekp, 0) != 0){
		FontClose();
		return(FAIL);
	}
	fread(&f1idx, sizeof(e1idxtype), 1, m_fileSF1);
	DataLenSeekPointGet(f1idx, &datlen, &datseekp);
	if(datlen == 0){
		FontClose();
		return(FAIL);
	}
	if(fseek(m_fileSF1, datseekp, 0) != 0){
		FontClose();
		return(FAIL);
	}
	if(fread(&f1dhead, sizeof(e1dhtype), 1, m_fileSF1) != 1){
		FontClose();
		return(FAIL);
	}
	m_nDataLen = f1dhead.datalen;
	if(fread(m_bFBuf, 1, m_nDataLen, m_fileSF1) < (UINT)m_nDataLen){
		FontClose();
		return(FAIL);
	}
	FontClose();
	return(0);
}

int CESF1Font::UserFontWriteOpen(int ku, int ten)
{
	char	ffname[FNAMEMAX];

	if(ku < ESG1KUTOP){
		sprintf(ffname, "%s.uf1", m_sBaseFname);
	}
	else{
		sprintf(ffname, "%s.ug1", m_sBaseFname);
	}
	if((m_fileSF1 = fopen(ffname, "r+b")) == NULL){
		UserFontNullSet();
		if((m_fileSF1 = fopen(ffname, "r+b")) == NULL){
			return(FAIL);
		}
	}
	return(0);
}

int CESF1Font::UserFontOpen(int ku, int ten)
{
	char	ffname[FNAMEMAX];

	if(ku < ESG1KUTOP){
		sprintf(ffname, "%s.uf1", m_sBaseFname);
	}
	else{
		sprintf(ffname, "%s.ug1", m_sBaseFname);
	}
	if((m_fileSF1 = fopen(ffname, "rb")) == NULL){
		return(FAIL);
	}
	return(0);
}

int CESF1Font::EsysFontOpen(int ku, int ten)
{
	char	ffname[FNAMEMAX];

	if(ku < ESG1KUTOP){
		sprintf(ffname, "%s.sf1", m_sBaseFname);
	}
	else{
		sprintf(ffname, "%s.sg1", m_sBaseFname);
	}
	if((m_fileSF1 = fopen(ffname, "rb")) == NULL){
		return(FAIL);
	}
	return(0);
}

void CESF1Font::FontClose()
{
	fclose(m_fileSF1);
}

int CESF1Font::UserFontNullSet()
{
	char	ffname[FNAMEMAX];
	char	buf[10];
	long	len;

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

	sprintf(ffname, "%s.ug1", m_sBaseFname);
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

void CESF1Font::IdxSeekPointGet(int ku, int ten, long *seekp)
{
	if(ku < ESG1KUTOP){
		*seekp = (long)((ku-1)*100+(ten-1)) * (long)(sizeof(f1idxtype)) + sizeof(e1fhtype);
	}
	else{
		*seekp = (long)((ku-ESG1KUTOP)*100+(ten-1)) * (long)(sizeof(f1idxtype)) + sizeof(e1fhtype);
	}
}

void CESF1Font::DataLenSeekPointGet(e1idxtype f1idx, int *dlen, long *dseekp)
{
	*dlen = f1idx.dlen;
	*dseekp = f1idx.dseekp + ESF1DSEEKP;
}

void CESF1Font::DataLenSeekPointSet(e1idxtype *f1idx, int dlen, long dseekp)
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

void CESF1Font::TateGakiTrns(UINT sjis)
{
	int		eofflag;
	int		pcnt, pp, atr;
	DBL		x, y, xx, yy;
	DBL		minx, maxx;
	DBL		movex;

	pcnt = m_nDataLen / ESF1SIZADD;
	if(0x815C <= sjis && sjis <= 0x8186){
		for(pp = 0; pp < pcnt; pp++){
			eofflag = EorChk(m_bFBuf, pp);
			AtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
			xx = ESF1MESH - y;
			yy = x;
			AtrDXYSet(m_bFBuf, pp, atr, xx, yy);
			if(eofflag == ON){
				EorOn(m_bFBuf, pp);
				if(RejionChk(m_bFBuf, pp) == ON){
					RejionOn(m_bFBuf, pp);
				}
				else{
					RejionOff(m_bFBuf, pp);
				}
			}
		}
	}
	else if(sjis == 0x8141 || sjis == 0x8142){
		minx = ESF1MESH;
		maxx = 0;
		for(pp = 0; pp < pcnt; pp++){
			AtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
			if(maxx < x){
				maxx = x;
			}
			if(minx > x){
				minx = x;
			}
		}
		movex = ESF1MESH - maxx - minx;
		for(pp = 0; pp < pcnt; pp++){
			eofflag = EorChk(m_bFBuf, pp);
			AtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
			xx = x + movex;
			yy = y + movex;
			AtrDXYSet(m_bFBuf, pp, atr, xx, yy);
			if(eofflag == ON){
				EorOn(m_bFBuf, pp);
				if(RejionChk(m_bFBuf, pp) == ON){
					RejionOn(m_bFBuf, pp);
				}
				else{
					RejionOff(m_bFBuf, pp);
				}
			}
		}
	}
	else if(sjis == 0x815b){
		for(pp = 0; pp < pcnt; pp++){
			eofflag = EorChk(m_bFBuf, pp);
			AtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
			xx = y;
			yy = x;
			AtrDXYSet(m_bFBuf, pp, atr, xx, yy);
			if(eofflag == ON){
				EorOn(m_bFBuf, pp);
				if(RejionChk(m_bFBuf, pp) == ON){
					RejionOn(m_bFBuf, pp);
				}
				else{
					RejionOff(m_bFBuf, pp);
				}
			}
		}
	}
}

void CESF1Font::TrnsESF1ToFSF1()
{
	int		pcnt, flag, pp, atr;
	DBL		x, y;

	m_pFSF1Font->PointSetStart();
	pcnt = m_nDataLen / ESF1SIZADD;
	for(pp = 0; pp < pcnt; pp++){
		AtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
		m_pFSF1Font->AtrDXYCrtSet(atr, x, y);
		if(EorChk(m_bFBuf, pp) == ON){
			flag = RejionChk(m_bFBuf, pp);
			m_pFSF1Font->EorOn(pp);
			// 便宜的に閉図形のみ扱いとする
			// 変更の際はソアで使用している書体も変更が必要
			m_pFSF1Font->RejionOn(pp);
			/*
			if(flag == ON){
				m_pFSF1Font->RejionOn(pp);
			}
			else{
				m_pFSF1Font->RejionOff(pp);
			}
			*/
		}
		m_pFSF1Font->CrtPointInc();
	}
	m_pFSF1Font->PointSetEnd();
}

void CESF1Font::SetDataLen(int pp)
{
	m_nDataLen = pp * ESF1SIZADD;
}

void CESF1Font::AtrDXYGet(BYTE fBuf[], int pp, int *atr, DBL *x, DBL *y)
{
	int		bytep;
	short	*xt, *yt;
	double	dx, dy;

	bytep = pp * ESF1SIZADD;
	xt = (short *)(&(fBuf[bytep+ESF1XCDADD]));
	yt = (short *)(&(fBuf[bytep+ESF1YCDADD]));
	*atr = (fBuf[bytep+ESF1ATRADD] & 0x0e);
	dx = *xt;
	dy = *yt;
	*x = dx;
	*y = dy;
}

void CESF1Font::AtrDXYSet(BYTE fBuf[], int pp, int atr, DBL x, DBL y)
{
	int		bytep;
	short	*xt, *yt;

	bytep = pp * ESF1SIZADD;
	xt = (short *)(&(fBuf[bytep+ESF1XCDADD]));
	yt = (short *)(&(fBuf[bytep+ESF1YCDADD]));
	*xt = (short)(x);
	*yt = (short)(y);
	fBuf[bytep+ESF1ATRADD] = atr & 0x0e;
}

int CESF1Font::EorChk(BYTE fBuf[], int pp)
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

void CESF1Font::EorOn(BYTE fBuf[], int pp)
{
	int		bytep;

	bytep = pp * ESF1SIZADD;
	fBuf[bytep+ESF1ATRADD] |= ESF1EORMASK;
}

int CESF1Font::RejionChk(BYTE fBuf[], int pp)
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

void CESF1Font::RejionOff(BYTE fBuf[], int pp)
{
	int		bytep;

	bytep = pp * ESF1SIZADD;
	fBuf[bytep+ESF1ATRADD] &= ~ESF1REJIONMASK;
}

void CESF1Font::RejionOn(BYTE fBuf[], int pp)
{
	int		bytep;

	bytep = pp * ESF1SIZADD;
	fBuf[bytep+ESF1ATRADD] |= ESF1REJIONMASK;
}

void CESF1Font::TrnsCopyFSF1FBuf()
{
	LPBYTE	FSF1FBuf;
	int		pcnt, pp;
	int		atr;
	DBL		dx, dy;

	FSF1FBuf = m_pFSF1Font->FBufAdrsGet();
	pcnt = m_pFSF1Font->PointCountGet();
	for(pp = 0; pp < pcnt; pp++){
		m_pFSF1Font->AtrDXYGet(FSF1FBuf, pp, &atr, &dx, &dy);
		AtrDXYSet(m_bFBuf, pp, atr, dx, dy);
		if(m_pFSF1Font->EorChk(pp) == ON){
			EorOn(m_bFBuf, pp);
			RejionOn(m_bFBuf, pp);
		}
	}
	SetDataLen(pp);
}

int CESF1Font::DFileReadOpen(char basefname [ ])
{
	char	ffname[FNAMEMAX];

	sprintf(ffname, "%s.sf1", basefname);
	if((m_fileSF1 = fopen(ffname, "rb")) == NULL){
		return(FAIL);
	}
	return(0);
}

void CESF1Font::DFileLoadSF1Trns(int datano)
{
	DFileDataLoad(datano);
	TrnsESF1ToFSF1();
}

void CESF1Font::DFileDataLoad(int datano)
{
	long 		idxseekp, datseekp;
	int			datlen;
	e1idxtype	f1idx;
	e1dhtype	f1dhead;

	idxseekp = (long)datano * (long)(sizeof(f1idxtype))
			  + (long)(sizeof(f1fhtype));
	if(fseek(m_fileSF1, idxseekp, 0) != 0){
		m_nDataLen = 0;
		return;
	}
	fread(&f1idx, sizeof(e1idxtype), 1, m_fileSF1);
	DFileDataLenSeekPointGet(f1idx, &datlen, &datseekp);
	if(datlen == 0){
		m_nDataLen = 0;
		return;
	}
	if(fseek(m_fileSF1, datseekp, 0) != 0){
		m_nDataLen = 0;
		return;
	}
	if(fread(&f1dhead, sizeof(e1dhtype), 1, m_fileSF1) != 1){
		m_nDataLen = 0;
		return;
	}
	m_nDataLen = f1dhead.datalen;
	if(fread(m_bFBuf, 1, m_nDataLen, m_fileSF1) < (UINT)m_nDataLen){
		m_nDataLen = 0;
		return;
	}
}

void CESF1Font::DFileDataLenSeekPointGet(e1idxtype f1idx, int *dlen, long *dseekp)
{
	*dlen = f1idx.dlen;
	*dseekp = f1idx.dseekp + DFILEDSEEKP;
}

void CESF1Font::DFileClose()
{
	if(m_fileSF1 != NULL){
		fclose(m_fileSF1);
		m_fileSF1 = NULL;
	}
}

void CESF1Font::SetBold(int flag)
{
	m_nBold = flag;
}

void CESF1Font::SetSlope(int flag)
{
	m_nSlope = flag;
}

// 文字表示関数
void CESF1Font::DrawOut(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL rag)
{
	int		pcntmax;
	int		spp, epp, pcnt;
	CPen	*pcoldPen, chPen;
	COLORREF	clr;
	int		thick;

	pcoldPen = NULL;
	if(m_nBold == ON){
		thick = (int)(wd / 50.0);
		if(thick >= 3){
			clr = pcDC->GetTextColor();
			chPen.CreatePen(PS_SOLID, thick, clr);
			pcoldPen = pcDC->SelectObject(&chPen);
		}
	}
	else{
		chPen.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		pcoldPen = pcDC->SelectObject(&chPen);
	}
	TrnsElementInit(sx, sy, wd, hi, rag);

	plgnfillstart();
	pcntmax = m_nDataLen / FSF1SIZADD;
	spp = 0;
	while(1){
		if(pcntmax <= spp){
			break;
		}
		epp = RejionEndGet(pcntmax, spp);
		pcnt = epp - spp;
		if(pcnt <= 0){
			break;
		}
		plgnfillrejionstart();
		RejionDrawOut(pcDC, &(m_bFBuf[spp*FSF1SIZADD]), pcnt);
		plgnfillrejionend();
		spp = epp;
	}

	plgnfilldraw(pcDC->m_hDC);

	if(pcoldPen != NULL){
		pcDC->SelectObject(pcoldPen);
		chPen.DeleteObject();
	}
}

void CESF1Font::TrnsElementInit(DBL sx, DBL sy, DBL wd, DBL hi, DBL rag)
{
	m_dSX = sx;
	m_dSY = sy;
	m_dXTime = wd / ESF1MESH;
	m_dYTime = hi / ESF1MESH;
	m_dSin = sin(rag);
	m_dCos = cos(rag);
}

int CESF1Font::RejionEndGet(int pcnt, int sp)
{
	int		pp;

	for(pp = sp; pp < pcnt; pp++){
		if(EorChk(pp) == ON){
			return(pp+1);
		}
	}
	return(pcnt);
}

int CESF1Font::EorChk(int pp)
{
	int		bytep;

	bytep = pp * ESF1SIZADD;
	if(m_bFBuf[bytep+ESF1ATRADD] & FEORMASK){
		return(ON);
	}
	else{
		return(OFF);
	}
}

void CESF1Font::RejionDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt)
{
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;

	if(pcnt <= 1){
		return;
	}
	if(pcnt == 2){
		AtrDXYGet(fbuf, 0, &atr1, &x1, &y1);
		AtrDXYGet(fbuf, 1, &atr2, &x2, &y2);
		TrnsXY(&x1, &y1);
		TrnsXY(&x2, &y2);
		plgnfillmove(pcDC->m_hDC, x1, y1);
		plgnfillline(pcDC->m_hDC, x1, y1, x2, y2);
		plgnfillline(pcDC->m_hDC, x2, y2, x1, y1);
		return;
	}

	p0 = pcnt-1; p1 = 0; p2 = 1;
	AtrDXYGet(fbuf, p0, &atr0, &x0, &y0);
	AtrDXYGet(fbuf, p1, &atr1, &x1, &y1);
	TrnsXY(&x0, &y0);
	TrnsXY(&x1, &y1);
	AtrDXYGet(fbuf, p2, &atr2, &x2, &y2);
	if(pdrwatrarcchk(atr0, atr1, atr2) == ON){
		plgnfillmove(pcDC->m_hDC, x0, y0);
	}
	else{
		plgnfillmove(pcDC->m_hDC, x1, y1);
	}

	for(pp = 0; pp < pcnt;){
		p0 = PointLegal(pcnt, pp-1);
		p1 = PointLegal(pcnt, pp);
		p2 = PointLegal(pcnt, pp+1);
		p3 = PointLegal(pcnt, pp+2);
		p4 = PointLegal(pcnt, pp+3);
		AtrDXYGet(fbuf, p0, &atr0, &x0, &y0);
		AtrDXYGet(fbuf, p1, &atr1, &x1, &y1);
		AtrDXYGet(fbuf, p2, &atr2, &x2, &y2);
		AtrDXYGet(fbuf, p3, &atr3, &x3, &y3);
		AtrDXYGet(fbuf, p4, &atr4, &x4, &y4);
		TrnsXY(&x0, &y0);
		TrnsXY(&x1, &y1);
		TrnsXY(&x2, &y2);
		TrnsXY(&x3, &y3);
		TrnsXY(&x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				plgnfillline(pcDC->m_hDC, x1, y1, x2, y2);
				plgnfillline(pcDC->m_hDC, x2, y2, x3, y3);
			}
			else if(p2 == 0){
				plgnfillline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			else{
				plgnfillbeze(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				plgnfillarc(pcDC->m_hDC, x1, y1, x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					plgnfillarc(pcDC->m_hDC, x0, y0, x1, y1, x2, y2);
				}
				else{
					plgnfillfreecurve(pcDC->m_hDC, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				}
			}
			else{
				plgnfillline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
}

void CESF1Font::TrnsXY(DBL *x, DBL *y)
{
	DBL		xx, yy;
	DBL		tx, ty;

	xx = (*x) - ESF1MESH / 2;
	yy = (*y) - ESF1MESH / 2;
	tx = (xx * m_dCos - yy * m_dSin + ESF1MESH / 2);
	ty = (xx * m_dSin + yy * m_dCos + ESF1MESH / 2);
	if(m_nTateYoko == YOKOGAKI || m_nTateYoko == TATEGAKI){
		if(m_nSlope == OFF){
			*x = tx * m_dXTime + m_dSX;
			*y = ty * m_dYTime + m_dSY;
		}
		else{
			*x = (tx - ty * 0.5) * m_dXTime + m_dSX + ESF1MESH*m_dXTime*0.3;
			*y = ty * m_dYTime + m_dSY;
		}
	}
	else{
		if(m_nSlope == OFF){
			*x = tx * m_dXTime + m_dSX;
			*y = ty * m_dYTime + m_dSY;
		}
		else{
			*x = tx * m_dXTime + m_dSX;
			*y = (ty + tx * 0.5) * m_dYTime + m_dSY - ESF1MESH*m_dYTime*0.3;
		}
	}
}

int CESF1Font::PointLegal(int pcnt, int pp)
{
	pp %= pcnt;
	pp += pcnt;
	pp %= pcnt;
	return(pp);
}

