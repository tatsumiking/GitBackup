
//
// Easy Scanner System
//
// object esf0font.cpp  ÇRÉoÉCÉgÇdÇrèëëÃèàóùä÷êî
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"

#include "ESF0Font.h"

#include	"../ObjLib/Mojienv.h"
#include	"../ObjLib/Pdrwlib.h"
#include	"../ObjLib/Plgnfill.h"

#define	FIDXSIZE	6

IMPLEMENT_SERIAL(CESF0Font, CObject, 0)
CESF0Font::CESF0Font()
{
}

CESF0Font::CESF0Font(int nDummy)
{
	m_fileIdx = NULL;
	m_fileDat = NULL;
	m_pFSF1Font = NULL;
	m_nDataLen = 0;
	m_nTateYoko = YOKOGAKI;
	m_sBaseFname[0] = _T('\0');
}

CESF0Font::~CESF0Font()
{
}

void CESF0Font::Serialize(CArchive& ar)
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

LPBYTE CESF0Font::GetbFBufAdrs()
{
	return(m_bFBuf);
}

int CESF0Font::FontMeshGet()
{
	return(ESF0MESH);
}

void CESF0Font::FSF1ClassSet(CFSF1Font *pFSF1Font)
{
	m_pFSF1Font = pFSF1Font;
}

void CESF0Font::FontNameSet(LPTSTR pFont)
{
	TCHAR	fontpath[FNAMEMAX];
	int		num, len1, len2;

	mojienvgetfontpath(fontpath);
	len1 = _tcslen(_T("Ç`"));
	len2 = _tcslen(_T("Ç`âp"));
	if(_tcsnccmp(_T("Ç`"), pFont, len1) == 0){
		if(_tcsnccmp(_T("Ç`âp"), pFont, len2) == 0){
			num = _ttoi(&(pFont[len2]));
			if(num <= 60){
				_stprintf_s(m_sBaseFname, FNAMEMAX, _T("%s\\HE1\\%s"), fontpath, pFont);
			}
			else{
				_stprintf_s(m_sBaseFname, FNAMEMAX, _T("%s\\HE2\\%s"), fontpath, pFont);
			}
		}
		else{
			_stprintf_s(m_sBaseFname, FNAMEMAX, _T("%s\\HK\\%s"), fontpath, pFont);
		}
	}
	else{
		_stprintf_s(m_sBaseFname, FNAMEMAX, _T("%s\\%s"), fontpath, pFont);
	}
}

void CESF0Font::SetTateYoko(int flag)
{
	m_nTateYoko = flag;
}

int CESF0Font::SF1TrnsDataLoad(int sjis)
{
	int		ret;

	ret = DataLoad(sjis);
	TrnsESF0ToFSF1();
	return(ret);
}

void CESF0Font::SF1TrnsDataSave(int sjis)
{
	TrnsCopyFSF1FBuf();
	UserDataSave(sjis);
}

int CESF0Font::DFileReadOpen(TCHAR basefname[])
{
	TCHAR	idxfname[FNAMEMAX], datfname[FNAMEMAX];

	_stprintf_s(idxfname, FNAMEMAX, _T("%s.tbl"), basefname);
	_stprintf_s(datfname, FNAMEMAX, _T("%s.dat"), basefname);
	_tfopen_s(&m_fileIdx, idxfname, _T("rb"));
	if(m_fileIdx == NULL){
		m_fileIdx = NULL;
		m_fileDat = NULL;
		return(FAIL);
	}
	_tfopen_s(&m_fileDat, datfname, _T("rb"));
	if(m_fileDat == NULL){
		fclose(m_fileIdx);
		m_fileIdx = NULL;
		m_fileDat = NULL;
		return(FAIL);
	}
	return(0);
}

void CESF0Font::DFileLoadSF1Trns(int datano)
{
	DFileDataLoad(datano);
	TrnsESF0ToFSF1();
}

void CESF0Font::DFileDataLoad(int datano)
{
	long	seekpoint;
	fidxtype	fidx;

	seekpoint = (long)datano * (long)(FIDXSIZE);
	fseek(m_fileIdx, seekpoint, 0);
	fread(&(fidx.len), sizeof(short), 1, m_fileIdx); 
	fseek(m_fileIdx, seekpoint+2, 0);
	fread(&(fidx.seekp), sizeof(long), 1, m_fileIdx); 
	if(fidx.len == 0){
		m_nDataLen = 0;
		return;
	}
	m_nDataLen = fidx.len;
	fseek(m_fileDat, fidx.seekp, 0);
	if(fread(m_bFBuf, 1, m_nDataLen, m_fileDat) < (UINT)m_nDataLen){
		m_nDataLen = 0;
		return;
	}
}

void CESF0Font::DFileClose()
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

int CESF0Font::DataLoad(int sjis)
{
	if(sjis < 0x8140 || 0xffff < sjis){
		m_nDataLen = 0;
		return(0);
	}
	if(0x9fff < sjis && sjis < 0xe021){
		m_nDataLen = 0;
		return(0);
	}
	if(UserDataLoad(sjis) == FAIL){
		if(EsysDataLoad(sjis) == FAIL){
			m_nDataLen = 0;
			return(FAIL);
		}
	}
	return(0);
}

int CESF0Font::UserDataSave(int sjis)
{
	long 		seekpoint;
	fidxtype	fidx;

	if(m_nDataLen == 0){
		return(0);
	}
	if(UserFontWriteOpen(sjis) == FAIL){
		return(FAIL);
	}

	GetSeekPoint(sjis, &seekpoint);
	fseek(m_fileIdx, seekpoint, 0);
	fread(&(fidx.len), sizeof(short), 1, m_fileIdx); 
	fseek(m_fileIdx, seekpoint+2, 0);
	fread(&(fidx.seekp), sizeof(long), 1, m_fileIdx); 
	if(fidx.len < m_nDataLen){
		fseek(m_fileDat, 0L, 2);
		fidx.seekp = ftell(m_fileDat);
	}
	fidx.len = m_nDataLen;
	fseek(m_fileDat, fidx.seekp, 0);
	if(fwrite(m_bFBuf, 1, m_nDataLen, m_fileDat) < m_nDataLen){
		FontClose();
		return(FAIL);
	}
	fseek(m_fileIdx, seekpoint, 0);
	fwrite(&(fidx.len), sizeof(short), 1, m_fileIdx); 
	fseek(m_fileIdx, seekpoint+2, 0);
	fwrite(&(fidx.seekp), sizeof(long), 1, m_fileIdx); 
	FontClose();
	return(0);
}

int CESF0Font::UserDataExist(int sjis)
{
	long 		seekpoint;
	fidxtype	fidx;

	if(UserFontWriteOpen(sjis) == FAIL){
		return(FAIL);
	}

	GetSeekPoint(sjis, &seekpoint);
	fseek(m_fileIdx, seekpoint, 0);
	fread(&(fidx.len), sizeof(short), 1, m_fileIdx); 
	fseek(m_fileIdx, seekpoint+2, 0);
	fread(&(fidx.seekp), sizeof(long), 1, m_fileIdx); 
	if(fidx.len == 0){
		FontClose();
		return(FAIL);
	}
	FontClose();
	return(0);
}

int CESF0Font::UserDataDelete(int sjis)
{
	long 		seekpoint;
	fidxtype	fidx;

	if(UserFontWriteOpen(sjis) == FAIL){
		return(FAIL);
	}
	GetSeekPoint(sjis, &seekpoint);
	fidx.len = 0;
	fidx.seekp = 0L;
	fseek(m_fileIdx, seekpoint, 0);
	fwrite(&(fidx.len), sizeof(short), 1, m_fileIdx); 
	fseek(m_fileIdx, seekpoint+2, 0);
	fwrite(&(fidx.seekp), sizeof(long), 1, m_fileIdx); 
	FontClose();
	return(0);
}

int CESF0Font::UserDataLoad(int sjis)
{
	long 		seekpoint;
	fidxtype	fidx;

	if(UserFontOpen(sjis) == FAIL){
		return(FAIL);
	}

	GetSeekPoint(sjis, &seekpoint);
	if(fseek(m_fileIdx, seekpoint, 0) != 0){
		FontClose();
		return(FAIL);
	}
	fread(&(fidx.len), sizeof(short), 1, m_fileIdx); 
	if(fseek(m_fileIdx, seekpoint+2, 0) != 0){
		FontClose();
		return(FAIL);
	}
	fread(&(fidx.seekp), sizeof(long), 1, m_fileIdx); 
	if(fidx.len == 0){
		FontClose();
		return(FAIL);
	}
	m_nDataLen = fidx.len;
	if(fseek(m_fileDat, fidx.seekp, 0) != 0){
		FontClose();
		return(FAIL);
	}
	if(fread(m_bFBuf, 1, m_nDataLen, m_fileDat) < m_nDataLen){
		FontClose();
		return(FAIL);
	}
	if(m_nTateYoko == TATEGAKI){
		TateGakiTrns(sjis);
	}
	FontClose();
	return(0);
}

int CESF0Font::EsysDataLoad(int sjis)
{
	long 		seekpoint;
	fidxtype	fidx;

	if(EsysFontOpen(sjis) == FAIL){
		return(FAIL);
	}

	GetSeekPoint(sjis, &seekpoint);
	if(fseek(m_fileIdx, seekpoint, 0) != 0){
		FontClose();
		m_nDataLen = 0;
		return(0);
	}
	fread(&(fidx.len), sizeof(short), 1, m_fileIdx); 
	if(fseek(m_fileIdx, seekpoint+2, 0) != 0){
		FontClose();
		m_nDataLen = 0;
		return(0);
	}
	fread(&(fidx.seekp), sizeof(long), 1, m_fileIdx); 
	if(fidx.len == 0){
		FontClose();
		m_nDataLen = 0;
		return(0);
	}
	m_nDataLen = fidx.len;
	if(m_nDataLen < 0 || 4096 < m_nDataLen){
		FontClose();
		m_nDataLen = 0;
		return(0);
	}
	fseek(m_fileDat, fidx.seekp, 0);
	if(fread(m_bFBuf, 1, m_nDataLen, m_fileDat) < m_nDataLen){
		FontClose();
		m_nDataLen = 0;
		return(0);
	}
	if(m_nTateYoko == TATEGAKI){
		TateGakiTrns(sjis);
	}
	FontClose();
	return(0);
}

int CESF0Font::UserFontWriteOpen(int sjis)
{
	TCHAR	idxfname[FNAMEMAX], datfname[FNAMEMAX];

	if(sjis < 0x989e){
		_stprintf_s(idxfname, FNAMEMAX, _T("%s.tbu"), m_sBaseFname);
		_stprintf_s(datfname, FNAMEMAX, _T("%s.dau"), m_sBaseFname);
	}
	else if(sjis < ESF0LASTSJIS){
		_stprintf_s(idxfname, FNAMEMAX, _T("%s.t2u"), m_sBaseFname);
		_stprintf_s(datfname, FNAMEMAX, _T("%s.d2u"), m_sBaseFname);
	}
	else {
		_stprintf_s(idxfname, FNAMEMAX, _T("%s.tgu"), m_sBaseFname);
		_stprintf_s(datfname, FNAMEMAX, _T("%s.dgu"), m_sBaseFname);
	}
	_tfopen_s(&m_fileIdx, idxfname, _T("r+b"));
	if(m_fileIdx == NULL){
		FontTblDatNullSet(idxfname, datfname);
		_tfopen_s(&m_fileIdx, idxfname, _T("r+b"));
		if(m_fileIdx == NULL){
			return(FAIL);
		}
	}
	_tfopen_s(&m_fileDat, datfname, _T("r+b"));
	if(m_fileDat == NULL){
		fclose(m_fileIdx);
		return(FAIL);
	}
	return(0);
}

int CESF0Font::UserFontOpen(int sjis)
{
	TCHAR	idxfname[FNAMEMAX], datfname[FNAMEMAX];

	if(sjis < 0x989e){
		_stprintf_s(idxfname, FNAMEMAX, _T("%s.tbu"), m_sBaseFname);
		_stprintf_s(datfname, FNAMEMAX, _T("%s.dau"), m_sBaseFname);
	}
	else if(sjis < ESF0LASTSJIS){
		_stprintf_s(idxfname, FNAMEMAX, _T("%s.t2u"), m_sBaseFname);
		_stprintf_s(datfname, FNAMEMAX, _T("%s.d2u"), m_sBaseFname);
	}
	else {
		_stprintf_s(idxfname, FNAMEMAX, _T("%s.tgu"), m_sBaseFname);
		_stprintf_s(datfname, FNAMEMAX, _T("%s.dgu"), m_sBaseFname);
	}
	_tfopen_s(&m_fileIdx, idxfname, _T("rb"));
	if(m_fileIdx == NULL){
		return(FAIL);
	}
	_tfopen_s(&m_fileDat, datfname, _T("rb"));
	if(m_fileDat == NULL){
		fclose(m_fileIdx);
		return(FAIL);
	}
	return(0);
}

int CESF0Font::EsysFontOpen(int sjis)
{
	TCHAR	idxfname[FNAMEMAX], datfname[FNAMEMAX];

	if(sjis < 0x989e){
		_stprintf_s(idxfname, FNAMEMAX, _T("%s.tbl"), m_sBaseFname);
		_stprintf_s(datfname, FNAMEMAX, _T("%s.dat"), m_sBaseFname);
	}
	else if(sjis < ESF0LASTSJIS){
		_stprintf_s(idxfname, FNAMEMAX, _T("%s.tb2"), m_sBaseFname);
		_stprintf_s(datfname, FNAMEMAX, _T("%s.da2"), m_sBaseFname);
	}
	else {
		_stprintf_s(idxfname, FNAMEMAX, _T("%s.tbg"), m_sBaseFname);
		_stprintf_s(datfname, FNAMEMAX, _T("%s.dag"), m_sBaseFname);
	}
	_tfopen_s(&m_fileIdx, idxfname, _T("rb"));
	if(m_fileIdx == NULL){
		return(FAIL);
	}
	_tfopen_s(&m_fileDat, datfname, _T("rb"));
	if(m_fileDat == NULL){
		fclose(m_fileIdx);
		return(FAIL);
	}
	return(0);
}

void CESF0Font::FontClose()
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
int CESF0Font::FontNullSet()
{
	TCHAR	lpIdxFname[FNAMEMAX], lpDatFname[FNAMEMAX];
	int ret;

	_stprintf_s(lpIdxFname, FNAMEMAX, _T("%s.tbl"), m_sBaseFname);
	_stprintf_s(lpDatFname, FNAMEMAX, _T("%s.dat"), m_sBaseFname);
	ret = FontTblDatNullSet(lpIdxFname, lpDatFname);
	if (ret == FAIL) {
		return(FAIL);
	}
	_stprintf_s(lpIdxFname, FNAMEMAX, _T("%s.tb2"), m_sBaseFname);
	_stprintf_s(lpDatFname, FNAMEMAX, _T("%s.da2"), m_sBaseFname);
	ret = FontTblDatNullSet(lpIdxFname, lpDatFname);
	return(ret);
}
int CESF0Font::UserFontNullSet()
{
	TCHAR	lpIdxFname[FNAMEMAX], lpDatFname[FNAMEMAX];
	int ret;

	_stprintf_s(lpIdxFname, FNAMEMAX, _T("%s.tbu"), m_sBaseFname);
	_stprintf_s(lpDatFname, FNAMEMAX, _T("%s.dau"), m_sBaseFname);
	ret = FontTblDatNullSet(lpIdxFname, lpDatFname);
	if (ret == FAIL) {
		return(FAIL);
	}
	_stprintf_s(lpIdxFname, FNAMEMAX, _T("%s.t2u"), m_sBaseFname);
	_stprintf_s(lpDatFname, FNAMEMAX, _T("%s.d2u"), m_sBaseFname);
	ret = FontTblDatNullSet(lpIdxFname, lpDatFname);
	if (ret == FAIL) {
		return(FAIL);
	}
	_stprintf_s(lpIdxFname, FNAMEMAX, _T("%s.tgu"), m_sBaseFname);
	_stprintf_s(lpDatFname, FNAMEMAX, _T("%s.dgu"), m_sBaseFname);
	ret = FontTblDatNullSet(lpIdxFname, lpDatFname);
	return(ret);
}

int CESF0Font::FontTblDatNullSet(TCHAR *lpIdxFname, TCHAR *lpDatFname)
{
	TCHAR	buf[10];
	int		len;

	buf[0] = _T('\0');

	_tfopen_s(&m_fileIdx, lpIdxFname, _T("wb"));
	if(m_fileIdx == NULL){
		return(FAIL);
	}
	len = 30000;
	while(len--){
		if(fwrite(buf, 1, 1, m_fileIdx) != 1){
			fclose(m_fileIdx);
			return(FAIL);
		}
	}
	fclose(m_fileIdx);

	_tfopen_s(&m_fileDat, lpDatFname, _T("wb"));
	if(m_fileDat == NULL){
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

void CESF0Font::GetSeekPoint(int sjis, long *seekp)
{
	long	seekpoint, hcd, lcd;

	hcd = (sjis >> 8) & 0xff;
	lcd = (sjis) & 0xff;

	if(sjis < 0x0000989e){
		seekpoint = ((hcd-0x0081)*0x00c0+lcd-0x0040)* FIDXSIZE;
	}
	else if(hcd < 0x00e0){
		seekpoint = ((hcd-0x0098)*0x00c0+lcd-0x0040)* FIDXSIZE;
	}
	else if(hcd < 0x00f2){
		seekpoint = ((hcd-0x00d8)*0x00c0+lcd-0x0040)* FIDXSIZE;
	}
	else{
		seekpoint = ((hcd-0x00f2)*0x00c0+lcd-0x0040)* FIDXSIZE;
	}
	*seekp = seekpoint;
}

void CESF0Font::TateGakiTrns(int sjis)
{
	int		eofflag, pcnt, pp, atr;
	DBL		x, y, xx, yy;
	DBL		minx, miny, maxx, maxy;
	DBL		movex, movey;

	pcnt = m_nDataLen / ESF0SIZADD;
	if(sjis == 0x008141 
	|| sjis == 0x008142 
	|| sjis == 0x008143 
	|| sjis == 0x008144){
		minx = ESF0MESH;
		miny = ESF0MESH;
		maxx = 0;
		maxy = 0;
		for(pp = 0; pp < pcnt; pp++){
			AtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
			if(maxx < x){
				maxx = x;
			}
			if(minx > x){
				minx = x;
			}
			if(maxy < y){
				maxy = y;
			}
			if(miny > y){
				miny = y;
			}
		}
		movex = ESF0MESH - maxx - minx;
		movey = ESF0MESH - maxy - miny;
		for(pp = 0; pp < pcnt; pp++){
			eofflag = EorChk(m_bFBuf, pp);
			AtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
			xx = x + movex;
			yy = y + movey;
			AtrDXYSet(m_bFBuf, pp, atr, xx, yy);
			if(eofflag == ON){
				EorOn(m_bFBuf, pp);
			}
		}
	}
	else if(0x008150 == sjis || sjis == 0x008151
	     || 0x008163 == sjis || sjis == 0x008164
		 ||(0x008169 <= sjis && sjis <= 0x008186)){

		for(pp = 0; pp < pcnt; pp++){
			eofflag = EorChk(m_bFBuf, pp);
			AtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
			xx = ESF0MESH - y;
			yy = x;
			AtrDXYSet(m_bFBuf, pp, atr, xx, yy);
			if(eofflag == ON){
				EorOn(m_bFBuf, pp);
			}
		}
	}
	else if(sjis == 0x00815b || sjis == 0x00815c
		 || sjis == 0x00815d || sjis == 0x008160
		 || sjis == 0x008161 || sjis == 0x008162){
		for(pp = 0; pp < pcnt; pp++){
			eofflag = EorChk(m_bFBuf, pp);
			AtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
			xx = y;
			yy = x;
			AtrDXYSet(m_bFBuf, pp, atr, xx, yy);
			if(eofflag == ON){
				EorOn(m_bFBuf, pp);
			}
		}
	}
}

void CESF0Font::TrnsESF0ToFSF1()
{
	int		pcnt, flag, pp, tp, atr;
	DBL		x, y;

	m_pFSF1Font->PointSetStart();
	pcnt = m_nDataLen / ESF0SIZADD;
	for(pp = 0; pp < pcnt; pp++){
		AtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
		if(EorChk(m_bFBuf, pp) == ON){
			tp = pp;
			flag = RejionChk(m_bFBuf, &tp);
			if(tp == pp){
				m_pFSF1Font->AtrDXYCrtSet(atr, x, y);
				m_pFSF1Font->CrtPointInc();
			}
			if(flag == ON){
				m_pFSF1Font->RejionSet();
			}
			else{
				m_pFSF1Font->NoRejionSet();
			}
		}
		else{
			m_pFSF1Font->AtrDXYCrtSet(atr, x, y);
			m_pFSF1Font->CrtPointInc();
		}
	}
	//m_pFSF1Font->RejionSet();
	m_pFSF1Font->PointSetEnd();
}

void CESF0Font::SetDataLen(int pp)
{
	m_nDataLen = pp * ESF0SIZADD;
}

void CESF0Font::AtrDXYGet(BYTE fBuf[], int pp, int *atr, DBL *x, DBL *y)
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

void CESF0Font::AtrDXYSet(BYTE fBuf[], int pp, int atr, DBL x, DBL y)
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

int CESF0Font::EorChk(BYTE fBuf[], int pp)
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

void CESF0Font::EorOn(BYTE fBuf[], int pp)
{
	int		bytep;

	bytep = pp * ESF0SIZADD;
	fBuf[bytep+ESF0ATRADD] |= F0EORMASK;
}

int CESF0Font::RejionChk(BYTE fBuf[], int *pp)
{
	int		p0, p1;
	int		atr0, atr1;
	DBL		x0, y0, x1, y1;

	p0 = (*pp) - 1;
	p1 = (*pp);
	AtrDXYGet(fBuf, p1, &atr1, &x1, &y1);
	if(atr1 == 0x0e){
		AtrDXYGet(fBuf, p0, &atr0, &x0, &y0);
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

int CESF0Font::RejionOff(BYTE fBuf[], int pp)
{
	int		p0, p1;
	int		atr;
	DBL		x, y;

	p0 = pp;
	p1 = pp + 1;
	AtrDXYGet(fBuf, p0, &atr, &x, &y);
	atr = 0x0e;
	AtrDXYSet(fBuf, p1, atr, x, y);
	return(pp+1);
}

int CESF0Font::RejionOn(BYTE fBuf[], int pp)
{
	return(pp);
}

void CESF0Font::TrnsCopyFSF1FBuf()
{
	LPBYTE	FSF1FBuf;
	int		pcnt, pp, dp, atr;
	DBL		dx, dy;

	FSF1FBuf = m_pFSF1Font->FBufAdrsGet();
	pcnt = m_pFSF1Font->PointCountGet();
	for(pp = 0, dp = 0; pp < pcnt; pp++, dp++){
		m_pFSF1Font->AtrDXYGet(FSF1FBuf, pp, &atr, &dx, &dy);
		AtrDXYSet(m_bFBuf, dp, atr, dx, dy);
		if(m_pFSF1Font->EorChk(pp) == ON){
			// 2002.07.08 óßêg çÇÉVÉìëŒâû
			if(m_pFSF1Font->RejionChk(pp) == ON){
				dp = RejionOn(m_bFBuf, dp);
			}
			else{
				dp = RejionOff(m_bFBuf, dp);
			}
			EorOn(m_bFBuf, dp);
		}
	}
	// 2002.07.09 óßêg çÇÉVÉìëŒâû
	SetDataLen(dp);
}

void CESF0Font::SetBold(int flag)
{
	m_nBold = flag;
}

void CESF0Font::SetSlope(int flag)
{
	m_nSlope = flag;
}

// ï∂éöï\é¶ä÷êî
void CESF0Font::DrawOut(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL rag)
{
	int		pcntmax;
	int		spp, epp, tpp, pcnt;
	CPen	*pcoldPen, chPen;
	COLORREF	clr;
	int		thick;
	int		flag;

	pcoldPen = NULL;
	thick = (int)(wd / 50.0);
	if(m_nBold == ON && thick >= 3){
			clr = pcDC->GetTextColor();
			chPen.CreatePen(PS_SOLID, thick, clr);
			pcoldPen = pcDC->SelectObject(&chPen);
	}
	else{
		clr = pcDC->GetTextColor();
		chPen.CreatePen(PS_SOLID, 1, clr);
		pcoldPen = pcDC->SelectObject(&chPen);
	}
	TrnsElementInit(sx, sy, wd, hi, rag);

	plgnfillstart();
	pcntmax = m_nDataLen / ESF0SIZADD;
	spp = 0;
	while(1){
		if(pcntmax <= spp){
			break;
		}
		epp = RejionEndGet(m_bFBuf, pcntmax, spp);
		tpp = epp;
		flag = RejionChk(m_bFBuf, &tpp);
		pcnt = tpp - spp;
		if(flag == ON){
			plgnfillrejionstart();
			RejionDrawOut(pcDC, &(m_bFBuf[spp*ESF0SIZADD]), pcnt);
			plgnfillrejionend();
		}
		else{
			NoRejionDrawOut(pcDC, &(m_bFBuf[spp*ESF0SIZADD]), pcnt);
		}
		spp = epp;
	}

	plgnfilldraw(pcDC->m_hDC);

	if(pcoldPen != NULL){
		pcDC->SelectObject(pcoldPen);
		chPen.DeleteObject();
	}
}

void CESF0Font::TrnsElementInit(DBL sx, DBL sy, DBL wd, DBL hi, DBL rag)
{
	m_dSX = sx;
	m_dSY = sy;
	m_dXTime = wd / ESF0MESH;
	m_dYTime = hi / ESF0MESH;
	m_dSin = sin(rag);
	m_dCos = cos(rag);
}

int CESF0Font::RejionEndGet(BYTE fbuf[], int pcnt, int sp)
{
	int		pp;

	for(pp = sp; pp < pcnt; pp++){
		if(EorChk(fbuf, pp) == ON){
			return(pp+1);
		}
	}
	return(pcnt);
}

void CESF0Font::RejionDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt)
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

void CESF0Font::NoRejionDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt)
{
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;

	if(pcnt <= 1){
		return;
	}

	AtrDXYGet(fbuf, 0, &atr1, &x1, &y1);
	TrnsXY(&x1, &y1);
	pdrwmove(pcDC->m_hDC, x1, y1);

	for(pp = 0; pp < pcnt-1;){
		p0 = PointLegal(pcnt, pp-1);
		p1 = PointLegal(pcnt, pp);
		p2 = PointLegal(pcnt, pp+1);
		p3 = PointLegal(pcnt, pp+2);
		p4 = PointLegal(pcnt, pp+3);
		if(pp == 0){
			p0 = p1;
		}
		if(p3 == 0){
			p3 = p2;
		}
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
			if(p4 == 0){
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
				pdrwline(pcDC->m_hDC, x2, y2, x3, y3);
			}
			else if(p3 == 0){
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			else{
				pdrwbeze(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			else{
				pdrwarc(pcDC->m_hDC, x1, y1, x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				pdrwfreecurve(pcDC->m_hDC, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
			}
			else{
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
}

void CESF0Font::TrnsXY(DBL *x, DBL *y)
{
	DBL		xx, yy;
	DBL		tx, ty;

	xx = (*x) - ESF0MESH / 2;
	yy = (*y) - ESF0MESH / 2;
	tx = (xx * m_dCos - yy * m_dSin + ESF0MESH / 2);
	ty = (xx * m_dSin + yy * m_dCos + ESF0MESH / 2);
	if(m_nTateYoko == YOKOGAKI || m_nTateYoko == TATEGAKI){
		if(m_nSlope == OFF){
			*x = tx * m_dXTime + m_dSX;
			*y = ty * m_dYTime + m_dSY;
		}
		else{
			*x = (tx - ty * 0.5) * m_dXTime + m_dSX + ESF0MESH*m_dXTime*0.3;
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
			*y = (ty + tx * 0.5) * m_dYTime + m_dSY - ESF0MESH*m_dYTime*0.3;
		}
	}
}

int CESF0Font::PointLegal(int pcnt, int pp)
{
	pp %= pcnt;
	pp += pcnt;
	pp %= pcnt;
	return(pp);
}

