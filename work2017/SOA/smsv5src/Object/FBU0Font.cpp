
//
// Easy Scanner System
//
// object fbu9font.cpp  ÇTÉoÉCÉgÇeÇaèëëÃèàóùä÷êî
// 
//  Copyright (c) 2020 by TecDev

#include "stdafx.h"

#include "FBU0Font.h"

#include	"../ObjLib/Mojienv.h"
#include	"../ObjLib/Pdrwlib.h"
#include	"../ObjLib/Plgnfill.h"

IMPLEMENT_SERIAL(CFBU0Font, CObject, 0)
CFBU0Font::CFBU0Font()
{
}

CFBU0Font::CFBU0Font(int nDummy)
{
	m_fileIdx = NULL;
	m_fileDat = NULL;
	m_pFSF1Font = NULL;
	m_nDataLen = 0;
	m_nTateYoko = YOKOGAKI;
	m_sBaseName[0] = _T('\0');
}

CFBU0Font::~CFBU0Font()
{
}
void CFBU0Font::Serialize(CArchive& ar)
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

LPBYTE CFBU0Font::GetbFBufAdrs()
{
	return(m_bFBuf);
}
int CFBU0Font::FontMeshGet()
{
	return(FBU0MESH);
}
void CFBU0Font::FSF1ClassSet(CFSF1Font *pFSF1Font)
{
	m_pFSF1Font = pFSF1Font;
}
void CFBU0Font::FontNameSet(LPTSTR pFont)
{
	TCHAR	fontpath[FNAMEMAX];

	mojienvgetfontpath(fontpath);
	_stprintf_s(m_sBaseName, FNAMEMAX, _T("%s\\%s"), fontpath, pFont);

}

void CFBU0Font::SetTateYoko(int flag)
{
	m_nTateYoko = flag;
}

int CFBU0Font::SF1TrnsDataLoad(int code)
{
	int		ret;

	ret = DataLoad(code);
	if (ret == 0) {
		TrnsFBU0ToFSF1();
	}
 	return(ret);
}
void CFBU0Font::SF1TrnsDataSave(int code)
{
	TrnsFSF1ToFBU0();
	UserDataSave(code);
}
void CFBU0Font::SF1TrnsDataReadSave(int code)
{
	TrnsFSF1ToFBU0();
	DataRealSave(code);
}
int CFBU0Font::FontNullSet()
{
	TCHAR		sIdxFileName[FNAMEMAX];
	TCHAR		sDatFileName[FNAMEMAX];
	int ret;

	_stprintf_s(sIdxFileName, FNAMEMAX, _T("%s.ftg"), m_sBaseName);
	_stprintf_s(sDatFileName, FNAMEMAX, _T("%s.fdg"), m_sBaseName);
	ret = FontTblDatNullSet(sIdxFileName, sDatFileName);
	if (ret == FAIL) {
		return(FAIL);
	}
	_stprintf_s(sIdxFileName, FNAMEMAX, _T("%s.ftt"), m_sBaseName);
	_stprintf_s(sDatFileName, FNAMEMAX, _T("%s.fdt"), m_sBaseName);
	ret = FontTblDatNullSet(sIdxFileName, sDatFileName);
	if (ret == FAIL) {
		return(FAIL);
	}
	_stprintf_s(sIdxFileName, FNAMEMAX, _T("%s.fty"), m_sBaseName);
	_stprintf_s(sDatFileName, FNAMEMAX, _T("%s.fdy"), m_sBaseName);
	ret = FontTblDatNullSet(sIdxFileName, sDatFileName);
	if (ret == FAIL) {
		return(FAIL);
	}
	return(0);
}
int CFBU0Font::UserFontNullSet()
{
	_tmkdir(m_sBaseName);
	return(0);
}
int CFBU0Font::DataLoad(int code)
{
	if(UserDataLoad(code) == FAIL){
		if(0x0E000 <= code && code <= 0x0F8FF){
			if(GaijiDataLoad(code) == FAIL){
				m_nDataLen = 0;
				return(FAIL);
			}
		}
		else if(m_nTateYoko == YOKOGAKI){
			if(YokogakiDataLoad(code) == FAIL){
				if(TategakiDataLoad(code) == FAIL){
					m_nDataLen = 0;
					return(FAIL);
				}
			}
		}else{
			if(TategakiDataLoad(code) == FAIL){
				if(YokogakiDataLoad(code) == FAIL){
					m_nDataLen = 0;
					return(FAIL);
				}
			}
		}
	}
	return(0);
}
int CFBU0Font::DataSave(int code)
{
	int ret;
	
	ret = UserDataSave(code);
	return(ret);
}
int CFBU0Font::DataRealSave(int code)
{
	if(0x0E000 <= code && code <= 0x0F8FF){
		if(GaijiDataSave(code) == FAIL){
			return(FAIL);
		}
	}
	else if(m_nTateYoko == YOKOGAKI){
		if(YokogakiDataSave(code) == FAIL){
			return(FAIL);
		}
	}else{
		if(TategakiDataSave(code) == FAIL){
			return(FAIL);
		}
	}
	return(0);
}
void CFBU0Font::TrnsCopyFSF1FBuf()
{
	TrnsFSF1ToFBU0();
}
int CFBU0Font::UserDataDelete(int code)
{
	TCHAR	szGaijiFileName[FNAMEMAX];

	_stprintf_s(szGaijiFileName, FNAMEMAX, _T("%s\\%04X.fbu"), m_sBaseName, code);
	_tremove(szGaijiFileName);
	return(0);
}
int CFBU0Font::UserDataLoad(int code)
{
	TCHAR	szGaijiFileName[FNAMEMAX];

	_stprintf_s(szGaijiFileName, FNAMEMAX, _T("%s\\%04X.fbu"), m_sBaseName, code);
	_tfopen_s(&m_fileDat, szGaijiFileName, _T("rb"));
	if(m_fileDat == NULL){
		m_nDataLen = 0;
		return(FAIL);
	}
	m_nDataLen = fread(m_bFBuf, 1, FBUBUFMAX, m_fileDat);
	fclose(m_fileDat);
	return(0);
}
int CFBU0Font::UserDataSave(int code)
{
	TCHAR	szGaijiFileName[FNAMEMAX];

	if(m_nDataLen == 0){
		return(0);
	}
	_tmkdir(m_sBaseName);
	_stprintf_s(szGaijiFileName, FNAMEMAX, _T("%s\\%04X.fbu"), m_sBaseName, code);
	_tfopen_s(&m_fileDat, szGaijiFileName, _T("wb"));
	if(m_fileDat == NULL){
		return(FAIL);
	}
	if(fwrite(m_bFBuf, 1, m_nDataLen, m_fileDat) < m_nDataLen){
		fclose(m_fileDat);
		return(FAIL);
	}
	fclose(m_fileDat);
	return(0);
}
int CFBU0Font::UserDataExist(int code)
{
	TCHAR	szGaijiFileName[FNAMEMAX];

	_stprintf_s(szGaijiFileName, FNAMEMAX, _T("%s\\%04X.fbu"), m_sBaseName, code);
	if ((_taccess_s(szGaijiFileName, 0)) != 0) {
		return(FAIL);
	}
	return(0);
}
int CFBU0Font::GaijiDataLoad(int code)
{
	int ret;

	_stprintf_s(m_sIdxFileName, FNAMEMAX, _T("%s.ftg"), m_sBaseName);
	_stprintf_s(m_sDatFileName, FNAMEMAX, _T("%s.fdg"), m_sBaseName);
	ret = FileDataLoad(code);
	return(ret);
}
int CFBU0Font::TategakiDataLoad(int code)
{
	int ret;

	_stprintf_s(m_sIdxFileName, FNAMEMAX, _T("%s.ftt"), m_sBaseName);
	_stprintf_s(m_sDatFileName, FNAMEMAX, _T("%s.fdt"), m_sBaseName);
	ret = FileDataLoad(code);
	return(ret);
}
int CFBU0Font::YokogakiDataLoad(int code)
{
	int ret;

	_stprintf_s(m_sIdxFileName, FNAMEMAX, _T("%s.fty"), m_sBaseName);
	_stprintf_s(m_sDatFileName, FNAMEMAX, _T("%s.fdy"), m_sBaseName);
	ret = FileDataLoad(code);
	return(ret);
}
int CFBU0Font::FileDataLoad(int code)
{
	long 		seekpoint;
	fbuidxtype	fidx;

	m_nDataLen = 0;
	_tfopen_s(&m_fileIdx, m_sIdxFileName, _T("rb"));
	if(m_fileIdx == NULL){
		return(FAIL);
	}
	seekpoint = sizeof(fbuidxtype) * code;
	fseek(m_fileIdx, seekpoint, 0);
	fread(&(fidx), sizeof(fbuidxtype), 1, m_fileIdx);
	if(fidx.len == 0){
		return(FAIL);
	}

	_tfopen_s(&m_fileDat, m_sDatFileName, _T("rb"));
	if(m_fileDat == NULL){
		fclose(m_fileIdx);
		return(FAIL);
	}
	m_nDataLen = fidx.len;
	fseek(m_fileDat, fidx.seekp, 0);
	if(fread(m_bFBuf, 1, m_nDataLen, m_fileDat) < m_nDataLen){
		fclose(m_fileIdx);
		fclose(m_fileDat);
		return(FAIL);
	}
	fclose(m_fileIdx);
	fclose(m_fileDat);
	return(0);
}
int CFBU0Font::GaijiDataSave(int code)
{
	int ret;

	_stprintf_s(m_sIdxFileName, FNAMEMAX, _T("%s.ftg"), m_sBaseName);
	_stprintf_s(m_sDatFileName, FNAMEMAX, _T("%s.fdg"), m_sBaseName);
	ret = FileDataSave(code);
	return(ret);
}
int CFBU0Font::TategakiDataSave(int code)
{
	int ret;

	_stprintf_s(m_sIdxFileName, FNAMEMAX, _T("%s.ftt"), m_sBaseName);
	_stprintf_s(m_sDatFileName, FNAMEMAX, _T("%s.fdt"), m_sBaseName);
	ret = FileDataSave(code);
	return(ret);
}
int CFBU0Font::YokogakiDataSave(int code)
{
	int ret;

	_stprintf_s(m_sIdxFileName, FNAMEMAX, _T("%s.fty"), m_sBaseName);
	_stprintf_s(m_sDatFileName, FNAMEMAX, _T("%s.fdy"), m_sBaseName);
	ret = FileDataSave(code);
	return(ret);
}
int CFBU0Font::FileDataSave(int code)
{
	long 		seekpoint;
	fbuidxtype	fidx;
	int			len1, len2;

	_tfopen_s(&m_fileIdx, m_sIdxFileName, _T("r+b"));
	if(m_fileIdx == NULL){
		return(FAIL);
	}
	seekpoint = sizeof(fbuidxtype) * code;
	fseek(m_fileIdx, seekpoint, 0);
	fread(&(fidx), sizeof(fbuidxtype), 1, m_fileIdx);

	_tfopen_s(&m_fileDat, m_sDatFileName, _T("r+b"));
	if(m_fileDat == NULL){
		fclose(m_fileIdx);
		return(FAIL);
	}
	if(fidx.len < m_nDataLen){
		fseek(m_fileDat, 0L, 2);
		fidx.seekp = ftell(m_fileDat);
	}else{
		fseek(m_fileDat, fidx.seekp, 0);
	}
	fidx.len = m_nDataLen;
	len1 = fwrite(m_bFBuf, 1, m_nDataLen, m_fileDat);
	if(len1 != m_nDataLen){
		fidx.len = 0;
	}
	fseek(m_fileIdx, seekpoint, 0);
	len2 = fwrite(&(fidx), sizeof(fbuidxtype), 1, m_fileIdx);

	fclose(m_fileIdx);
	fclose(m_fileDat);
	if(len1 != m_nDataLen || len2 != 1){
		return(FAIL);
	}
	return(0);
}
int CFBU0Font::FontTblDatNullSet(TCHAR *lpIdxFname, TCHAR *lpDatFname)
{
	fbuidxtype	fidx;
	char	buf[5];
	int		len;

	fidx.len = 0;
	fidx.seekp = 0;
	_tfopen_s(&m_fileIdx, lpIdxFname, _T("wb"));
	if(m_fileIdx == NULL){
		return(FAIL);
	}
	len = 0x10000;
	while(len--){
		fwrite(&(fidx), sizeof(fbuidxtype), 1, m_fileIdx);
	}
	fclose(m_fileIdx);
	buf[0] = 0; buf[1] = 0; buf[2] = 0; buf[3] = 0;	buf[4] = 0;
	_tfopen_s(&m_fileDat, lpDatFname, _T("wb"));
	if(m_fileDat == NULL){
		return(FAIL);
	}
	len = 256;
	while(len--){
		if(fwrite(buf, 1, 1, m_fileDat) != 1){
			fclose(m_fileDat);
			return(FAIL);
		}
	}
	fclose(m_fileDat);
	return(0);
}
void CFBU0Font::TrnsFBU0ToFSF1()
{
	int		pcnt, flag, pp, atr;
	DBL		x, y;

	m_pFSF1Font->PointSetStart();
	pcnt = m_nDataLen / FBU0SIZADD;
	for(pp = 0; pp < pcnt; pp++){
		AtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
		m_pFSF1Font->AtrDXYCrtSet(atr, x, y);
		if(EorChk(m_bFBuf, pp) == ON){
			flag = RejionChk(m_bFBuf, pp);
			m_pFSF1Font->EorOn(pp);
			m_pFSF1Font->RejionOn(pp);
		}
		m_pFSF1Font->CrtPointInc();
	}
	m_pFSF1Font->PointSetEnd();
}
void CFBU0Font::TrnsFSF1ToFBU0()
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
void CFBU0Font::SetDataLen(int pp)
{
	m_nDataLen = pp * FBU0SIZADD;
}
void CFBU0Font::AtrDXYGet(BYTE fBuf[], int pp, int *atr, DBL *x, DBL *y)
{
	int		bytep;
	short	*xt, *yt;
	double	dx, dy;

	bytep = pp * FBU0SIZADD;
	xt = (short *)(&(fBuf[bytep+FBU0XCDADD]));
	yt = (short *)(&(fBuf[bytep+FBU0YCDADD]));
	*atr = (fBuf[bytep+FBU0ATRADD] & 0x0e);
	dx = *xt;
	dy = *yt;
	*x = dx;
	*y = dy;
}
void CFBU0Font::AtrDXYSet(BYTE fBuf[], int pp, int atr, DBL x, DBL y)
{
	int		bytep;
	short	*xt, *yt;

	bytep = pp * FBU0SIZADD;
	xt = (short *)(&(fBuf[bytep+FBU0XCDADD]));
	yt = (short *)(&(fBuf[bytep+FBU0YCDADD]));
	*xt = (short)(x);
	*yt = (short)(y);
	fBuf[bytep+FBU0ATRADD] = atr & 0x0e;
}
int CFBU0Font::EorChk(BYTE fBuf[], int pp)
{
	int		bytep;

	bytep = pp * FBU0SIZADD;
	if(fBuf[bytep+FBU0ATRADD] & FBU0EORMASK){
		return(ON);
	}
	else{
		return(OFF);
	}
}
void CFBU0Font::EorOn(BYTE fBuf[], int pp)
{
	int		bytep;

	bytep = pp * FBU0SIZADD;
	fBuf[bytep+FBU0ATRADD] |= FBU0EORMASK;
}
int CFBU0Font::RejionChk(BYTE fBuf[], int pp)
{
	int		bytep;

	bytep = pp * FBU0SIZADD;
	if(fBuf[bytep+FBU0ATRADD] & FBU0REJIONMASK){
		return(ON);
	}
	else{
		return(OFF);
	}
}
void CFBU0Font::RejionOff(BYTE fBuf[], int pp)
{
	int		bytep;

	bytep = pp * FBU0SIZADD;
	fBuf[bytep+FBU0ATRADD] &= ~FBU0REJIONMASK;
}

void CFBU0Font::RejionOn(BYTE fBuf[], int pp)
{
	int		bytep;

	bytep = pp * FBU0SIZADD;
	fBuf[bytep+FBU0ATRADD] |= FBU0REJIONMASK;
}
// ï∂éöï\é¶ä÷êî
void CFBU0Font::DrawOut(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL rag)
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
	pcntmax = m_nDataLen / FBU0SIZADD;
	spp = 0;
	while(1){
		if(pcntmax <= spp){
			break;
		}
		epp = RejionEndGet(m_bFBuf, spp);
		pcnt = epp - spp;
		if(pcnt <= 0){
			break;
		}
		plgnfillrejionstart();
		RejionDrawOut(pcDC, &(m_bFBuf[spp*FBU0SIZADD]), pcnt);
		plgnfillrejionend();
		spp = epp;
	}

	plgnfilldraw(pcDC->m_hDC);

	if(pcoldPen != NULL){
		pcDC->SelectObject(pcoldPen);
		chPen.DeleteObject();
	}
}
void CFBU0Font::TrnsElementInit(DBL sx, DBL sy, DBL wd, DBL hi, DBL rag)
{
	m_dSX = sx;
	m_dSY = sy;
	m_dXTime = wd / FBU0MESH;
	m_dYTime = hi / FBU0MESH;
	m_dSin = sin(rag);
	m_dCos = cos(rag);
}
int CFBU0Font::RejionEndGet(BYTE fBuf[], int sp)
{
	int		pcnt, pp;

	pcnt = m_nDataLen / FSF1SIZADD;
	for (pp = sp; pp < pcnt; pp++) {
		if (EorChk(fBuf, pp) == ON) {
			return(pp + 1);
		}
	}
	return(pcnt);
}
void CFBU0Font::RejionDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt)
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
void CFBU0Font::TrnsXY(DBL *x, DBL *y)
{
	DBL		xx, yy;
	DBL		tx, ty;

	xx = (*x) - FBU0MESH / 2;
	yy = (*y) - FBU0MESH / 2;
	tx = (xx * m_dCos - yy * m_dSin + FBU0MESH / 2);
	ty = (xx * m_dSin + yy * m_dCos + FBU0MESH / 2);
	if(m_nTateYoko == YOKOGAKI || m_nTateYoko == TATEGAKI){
		if(m_nSlope == OFF){
			*x = tx * m_dXTime + m_dSX;
			*y = ty * m_dYTime + m_dSY;
		}
		else{
			*x = (tx - ty * 0.5) * m_dXTime + m_dSX + FBU0MESH*m_dXTime*0.3;
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
			*y = (ty + tx * 0.5) * m_dYTime + m_dSY - FBU0MESH*m_dYTime*0.3;
		}
	}
}
int CFBU0Font::PointLegal(int pcnt, int pp)
{
	pp %= pcnt;
	pp += pcnt;
	pp %= pcnt;
	return(pp);
}

