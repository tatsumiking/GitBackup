
//
// Easy Scanner System
//
// object arcfont.cpp  ÉAÅ[ÉNèëëÃèàóùä÷êî
//
//  Copyright (c) 1998 by THEON

#include	 "stdafx.h"
#include	<stdio.h>
#include	<mbstring.h>
#include	 "arcfont.h"

extern "C" {
	void basesjtokuten(int far sjis, int far *ku, int far *ten);
	BOOL  pdrwatrarcchk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrbezechk(int atr1, int atr2, int atr3);
	void mojienvgetfontpath(LPSTR fontpath);
	void curveonpointtocntrlpoint(dbezetype *dbz, DBL tp1, DBL tp2);
	int curve3pcenterget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL FAR *x0, DBL FAR *y0);
	int curve3pragrget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0, DBL FAR *rag01, DBL FAR *rag03, DBL FAR *r);
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

IMPLEMENT_SERIAL(CARCFont, CObject, 0)
CARCFont::CARCFont()
{
}

CARCFont::CARCFont(int nDummy)
{
	m_fileARC = NULL;
	m_pFSF1Font = NULL;
	m_nDataLen = 0;
	m_nTateYoko = YOKOGAKI;
	m_sBaseFname[0] = '\0';
	m_dTime = FSF1MESH / ARCMESH;
	m_nSlope = OFF;
	m_nBold = OFF;
	m_dMoveX = 0;
	m_dMoveY = 0;
	int size = 10000;	//
	m_hFBuf = ::GlobalAlloc(GHND, size);
	m_lpFBuf = (LPBYTE)::GlobalLock(m_hFBuf);
}

CARCFont::~CARCFont()
{
	if(m_lpFBuf != NULL){
		::GlobalUnlock(m_hFBuf);
		m_lpFBuf = NULL;
	}
	if(m_hFBuf != 0){
		::GlobalFree(m_hFBuf);
		m_hFBuf = 0;
	}
}

void CARCFont::Serialize(CArchive& ar)
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

void CARCFont::FSF1ClassSet(CFSF1Font* pFSF1Font)
{
	m_pFSF1Font = pFSF1Font;
}

void CARCFont::FontNameSet(LPSTR pFont, int type)
{
	char	fontpath[FNAMEMAX];
	int		num;

	m_nNewType = OFF;
	m_nType = type;
	mojienvgetfontpath(fontpath);
	if(strncmp("âpêî", pFont, 4) == 0){
		num = atoi(&(pFont[4]));
		sprintf(m_sBaseFname, "%s\\FT2\\EI400\\E%03d.ft2", fontpath, num);
	}
	else if(strncmp("SPOT", pFont, 4) == 0){
		num = atoi(&(pFont[4]));
		sprintf(m_sBaseFname, "%s\\FT2\\SP70\\SP%03d.ft2", fontpath, num);
	}
	//else if(strncmp("FT", pFont, 2) == 0){
	else{
		if(type == ARCFONT){
			if(strncmp(pFont, "FT601", 5) == 0){
				m_nNewType = ON;
			}
			sprintf(m_sBaseFname, "%s\\%s.ft2", fontpath, pFont);
		}
		else{
			sprintf(m_sBaseFname, "%s\\%s.max", fontpath, pFont);
		}
	}
	/*
	else{
		sprintf(m_sBaseFname, "%s\\FT2\\WA\\%s.ft2", fontpath, pFont);
	}
	*/
}

void CARCFont::SetTateYoko(int flag)
{
	m_nTateYoko = flag;
}

int CARCFont::SF1TrnsDataLoad(UINT sjis)
{
	int		ret;
	DBL		rag;

	rag = 0;
	if(m_nTateYoko == YOKOOKI){
		rag = -M_PI_2;
	}
	if(m_nTateYoko == TATEOKI){
		rag = M_PI_2;
	}
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		if(sjis == 0x817c){
			rag += M_PI_2;
		}
	}
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		if(sjis < 0x0100){
			rag += M_PI_2;
		}
	}
	TrnsElementInit(0.0, 0.0, FSF1MESH, FSF1MESH, rag);
	ret = DataLoad(sjis);
	TrnsARCToFSF1();
	return(ret);
}

int CARCFont::SF1TrnsDataLoadNON(UINT sjis)
{
	int		ret;

	if(m_nTateYoko == YOKOGAKI){
		TrnsElementInit(0.0, 0.0, FSF1MESH, FSF1MESH, 0.0);
	}
	else if(m_nTateYoko == TATEGAKI){
		TrnsElementInit(0.0, 0.0, FSF1MESH, FSF1MESH, 0.0);
	}
	else if(m_nTateYoko == YOKOOKI){
		TrnsElementInit(0.0, 0.0, FSF1MESH, FSF1MESH, -M_PI_2);
	}
	else if(m_nTateYoko == TATEOKI){
		TrnsElementInit(0.0, 0.0, FSF1MESH, FSF1MESH, -M_PI_2);
	}
	ret = DataLoad(sjis);
	TrnsARCToFSF1NON();
	return(ret);
}

void CARCFont::SF1TrnsDataSave(UINT sjis)
{
	TrnsCopyFSF1FBuf();
	DataSave(sjis);
}

int CARCFont::DataLoad(UINT sjis)
{
	int	ku, ten;

	m_dTrnsTimeX = 1.0;
	m_dTrnsTimeY = 1.0;
	if(m_nNewType == ON){
		if(sjis < 0x0100){
			if(0x021 <= sjis && sjis <= 0x7e){
				sjis = sjis - 0x21 + 0x859f;
			}
			if(0x00a1 <= sjis && sjis <= 0x00DF){
				sjis = sjis - 0x00a1 + 0x8640;
			}
		}
	}
	else{
		if(sjis < 0x0100){
			sjis = _mbbtombc(sjis);
			m_dTrnsTimeX = 0.5;
			m_dTrnsTimeY = 1.0;
		}
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

	// ED40Å`EEFCÇ™FA5CÅ`FA40Ç…ïœä∑Ç≥ÇÍÇƒÇµÇ‹Ç§ÇΩÇﬂñﬂÇ∑èàóù
	if(115 == ku){
		if(1 <= ten && ten <= 10){
			ku -= 23;
			ten += 80;
		}
		else if(21 == ten){
			ku = 92;
			ten = 91;
		}
		else if(22 == ten){
			ku = 92;
			ten = 92;
		}
		else if(29 <= ten){
			ten -= 28;	// 29->1
			ku -= 26;	// 115->89
		}
	}
	else if(115 < ku){
		ten -= 28;	// 29->1
		if(1 <= ten){
			ku -= 26;	// 116->90
		}
		else{
			ku -= 27;	// 116->89
			ten += 94;	// 01->67
		}
	}
	if(ArcDataLoad(ku, ten) == FAIL){
		m_nDataLen = 0;
		return(FAIL);
	}
	return(0);
}

int CARCFont::ArcDataLoad(int ku, int ten)
{
	long 		idxseekp, datseekp;
	arcidxtype	arcidx;
	maxidxtype	maxidx;
	archtype	archead;
	arcpointype	*dbuf;
	int			idx;

	if(FontOpen(ku, ten) == FAIL){
		return(FAIL);
	}

	if(m_nType == ARCFONT){
		ArcIdxSeekPointGet(ku, ten, &idxseekp);
		fseek(m_fileARC, idxseekp, 0);
		fread(&(arcidx.ku), sizeof(UCHAR), 1, m_fileARC);
		fread(&(arcidx.ten), sizeof(UCHAR), 1, m_fileARC);
		fread(&(arcidx.seekp), sizeof(long), 1, m_fileARC);
		fread(&(arcidx.useekp), sizeof(long), 1, m_fileARC);
		ArcDataLenSeekPointGet(arcidx, &datseekp);
		if(arcidx.ku != ku || arcidx.ten != ten){
			FontClose();
			return(FAIL);
		}
	}
	else{
		MaxIdxSeekPointGet(ku, ten, &idxseekp);
		fseek(m_fileARC, idxseekp, 0);
		fread(&(maxidx.seekp), sizeof(long), 1, m_fileARC);
		fread(&(maxidx.useekp), sizeof(long), 1, m_fileARC);
		MaxDataLenSeekPointGet(maxidx, &datseekp);
	}
	if(datseekp == 0){
		FontClose();
		return(FAIL);
	}
	if(fseek(m_fileARC, datseekp, 0) != 0){
		FontClose();
		return(FAIL);
	}
	if(fread(&archead, sizeof(archtype), 1, m_fileARC) != 1){
		FontClose();
		return(FAIL);
	}
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		m_dMoveX = archead.x3;
		m_dMoveY = archead.y3;
	}
	else{
		m_dMoveX = archead.x1;
		m_dMoveY = archead.y1;
	}
	dbuf = (arcpointype*)m_lpFBuf;
	int		limit = 10000 / sizeof(arcpointype);
	idx = 0;
	while(1){
		fread(&(dbuf[idx].atr), sizeof(short), 1, m_fileARC);
		fread(&(dbuf[idx].x), sizeof(short), 1, m_fileARC);
		fread(&(dbuf[idx].y), sizeof(short), 1, m_fileARC);
		if(dbuf[idx].atr == -1){
			idx++;
			break;
		}
		if(limit-10 < idx){
			dbuf[idx].atr = -1;
			break;
		}
		idx++;
	}
	FontClose();
	m_nDataLen = idx;
	return(0);
}

int CARCFont::DataSave(UINT sjis)
{
	int	ku, ten;

	if(sjis < 0x8140 || 0xffff < sjis){
		return(0);
	}
	if(0x9fff < sjis && sjis < 0xe021){
		return(0);
	}
	basesjtokuten(sjis, &ku, &ten);
	if(ArcDataSave(ku, ten) == FAIL){
		return(FAIL);
	}
	return(0);
}

int CARCFont::ArcDataSave(int ku, int ten)
{
	long 		idxseekp, datseekp;
	arcidxtype	arcidx;
	maxidxtype	maxidx;
	archtype	archead;
	arcpointype	*dbuf;
	int		idx, len;

	if(FontSaveOpen(ku, ten) == FAIL){
		return(FAIL);
	}

	if(m_nType == ARCFONT){
		ArcIdxSeekPointGet(ku, ten, &idxseekp);
		fseek(m_fileARC, idxseekp, 0);
		fread(&(arcidx.ku), sizeof(UCHAR), 1, m_fileARC);
		fread(&(arcidx.ten), sizeof(UCHAR), 1, m_fileARC);
		fread(&(arcidx.seekp), sizeof(long), 1, m_fileARC);
		fread(&(arcidx.useekp), sizeof(long), 1, m_fileARC);
		ArcDataLenSeekPointGet(arcidx, &datseekp);
	}
	else{
		MaxIdxSeekPointGet(ku, ten, &idxseekp);
		fseek(m_fileARC, idxseekp, 0);
		fread(&(maxidx.seekp), sizeof(long), 1, m_fileARC);
		fread(&(maxidx.useekp), sizeof(long), 1, m_fileARC);
		MaxDataLenSeekPointGet(maxidx, &datseekp);
	}
	fseek(m_fileARC, 0L, 2);
	datseekp = ftell(m_fileARC);
	archead.x1 = 0;
	archead.y1 = 0;
	archead.x2 = (int)ARCMESH; 
	archead.y2 = (int)ARCMESH; 
	archead.x3 = 0;
	archead.y3 = 0;
	archead.x4 = (int)ARCMESH; 
	archead.y4 = (int)ARCMESH; 
	if(fwrite(&archead, sizeof(archtype), 1, m_fileARC) != 1){
		FontClose();
		return(FAIL);
	}
	dbuf = (arcpointype*)m_lpFBuf;
	len = m_nDataLen / sizeof(arcpointype);
	for(idx = 0; idx < len; idx++){
		fwrite(&(dbuf[idx].atr), sizeof(short), 1, m_fileARC);
		fwrite(&(dbuf[idx].x), sizeof(short), 1, m_fileARC);
		fwrite(&(dbuf[idx].y), sizeof(short), 1, m_fileARC);
	}

	if(m_nType == ARCFONT){
		arcidx.ku = ku;
		arcidx.ten = ten;
		ArcDataLenSeekPointSet(&arcidx, datseekp);
		fseek(m_fileARC, idxseekp, 0);
		fwrite(&(arcidx.ku), sizeof(UCHAR), 1, m_fileARC);
		fwrite(&(arcidx.ten), sizeof(UCHAR), 1, m_fileARC);
		fwrite(&(arcidx.seekp), sizeof(long), 1, m_fileARC);
		fwrite(&(arcidx.useekp), sizeof(long), 1, m_fileARC);
	}
	else{
		MaxDataLenSeekPointSet(&maxidx, datseekp);
		fseek(m_fileARC, idxseekp, 0);
		fwrite(&(maxidx.seekp), sizeof(long), 1, m_fileARC);
		fwrite(&(maxidx.useekp), sizeof(long), 1, m_fileARC);
	}

	FontClose();

	return(0);
}

int CARCFont::FontOpen(int ku, int ten)
{
	int		len;

	len = strlen(m_sBaseFname);
	if(m_sBaseFname[len-7] == '6'
	&& m_sBaseFname[len-6] == '0'
	&& m_sBaseFname[len-5] == '1'){
		m_dAddX = ARCMESH * 0.045;
		m_dAddY = 0;
	}
	else{
		m_dAddX = 0;
		m_dAddY = 0;
	}
	if((m_fileARC = fopen(m_sBaseFname, "rb")) == NULL){
		return(FAIL);
	}
	return(0);
}

int CARCFont::FontSaveOpen(int ku, int ten)
{
	if((m_fileARC = fopen(m_sBaseFname, "r+b")) == NULL){
		if(FontIdxInit() == FAIL){
			return(FAIL);
		}
	}
	return(0);
}

int CARCFont::FontIdxInit()
{
	char	buf[RECMAX];
	int		len, blk, rest, i;

	if((m_fileARC = fopen(m_sBaseFname, "wb")) == NULL){
		return(FAIL);
	}
	memset(buf, '\0', HEADBYTE);
	strcpy(buf, m_sBaseFname);
	fwrite(buf, HEADBYTE, 1, m_fileARC);
	if(m_nType == ARCFONT){
		len = (MAXKU * 94 + 1) * FT2IDXSIZE;
	}
	else{
		len = (MAXKU * 94 + 1) * MAXIDXSIZE;
	}
	memset(buf, '\0', RECMAX);
	blk = len / RECMAX;
	rest = len % RECMAX;
	for(i = 0; i < blk; i++){
		fwrite(buf, RECMAX, 1, m_fileARC);
	}
	if(rest != 0){
		fwrite(buf, rest, 1, m_fileARC);
	}
	fclose(m_fileARC);

	if((m_fileARC = fopen(m_sBaseFname, "r+b")) == NULL){
		return(FAIL);
	}
	return(0);
}

void CARCFont::FaceNameSet(LPSTR name)
{
	strcpy(m_sFaceName, name);
}

void CARCFont::FontClose()
{
	fclose(m_fileARC);
}

void CARCFont::ArcIdxSeekPointGet(int ku, int ten, long *seekp)
{
	int		idx;
	int		size;

	idx = (ku - 1) * 94 + (ten - 1);
	size = sizeof(char)+sizeof(char)+sizeof(long)+sizeof(long);
	*seekp = HEADBYTE + size * idx;
}

void CARCFont::MaxIdxSeekPointGet(int ku, int ten, long *seekp)
{
	int		idx;
	int		size;

	idx = (ku - 1) * 94 + (ten - 1);
	size = sizeof(long)+sizeof(long);
	*seekp = HEADBYTE + size * idx;
}

void CARCFont::ArcDataLenSeekPointGet(arcidxtype arcidx, long *dseekp)
{
	*dseekp = 0;
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		*dseekp = arcidx.useekp;
	}
	if((*dseekp) == 0){
		*dseekp = arcidx.seekp;
	}
}

void CARCFont::MaxDataLenSeekPointGet(maxidxtype maxidx, long *dseekp)
{
	*dseekp = 0;
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		*dseekp = maxidx.useekp;
	}
	if((*dseekp) == 0){
		*dseekp = maxidx.seekp;
	}
}

void CARCFont::ArcDataLenSeekPointSet(arcidxtype *arcidx, long dseekp)
{
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		arcidx->useekp = dseekp;
	}
	else{
		arcidx->seekp = dseekp;
	}
}

void CARCFont::MaxDataLenSeekPointSet(maxidxtype *maxidx, long dseekp)
{
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		maxidx->useekp = dseekp;
	}
	else{
		maxidx->seekp = dseekp;
	}
}

void CARCFont::TrnsARCToFSF1()
{
	int		pcnt, pp;
	int		dp;
	int		atr;
	DBL		x, y;
	DBL		rx, ry;
	DBL		sx, sy;
	DBL		ax1, ay1;
	DBL		ax2, ay2;
	int		aflag;
	arcpointype	*dbuf;

	m_pFSF1Font->PointSetStart();
	dbuf = (arcpointype*)m_lpFBuf;
	pcnt = m_nDataLen;
	aflag = OFF;
	for(pp = 0, dp = 0; pp < pcnt; pp++){
		AtrDXYGet(dbuf, pp, &atr, &x, &y);
		TrnsXY(&x, &y);
		if(atr == ENDOF || atr == MOVETO){
			if(pp != 0){
				if(aflag == ON){
					dp = ArcTrnsBezeSet(dp, ax1, ay1, ax2, ay2, rx, ry);
					aflag = OFF;
				}
				m_pFSF1Font->AtrDXYCrtSet(0, sx, sy);
				m_pFSF1Font->RejionOn(dp);
				m_pFSF1Font->EorOn(dp);
				m_pFSF1Font->CrtPointInc();
				dp++;
			}
		}
		switch(atr){
		case ENDOF:
			break;
		case MOVETO:
			rx = x; ry = y;
			sx = x; sy = y;
			break;
		case LINETO:
			if(aflag == ON){
				dp = ArcTrnsBezeSet(dp, ax1, ay1, ax2, ay2, sx, sy);
				aflag = OFF;
			}
			m_pFSF1Font->AtrDXYCrtSet(0, sx, sy);
			m_pFSF1Font->CrtPointInc();
			dp++;
			sx = x; sy = y;
			break;
		case ARCTO:
			if(aflag == ON){
				dp = ArcTrnsBezeSet(dp, ax1, ay1, ax2, ay2, sx, sy);
				aflag = OFF;
			}
			ax1 = sx; ay1 = sy;
			sx = x; sy = y;
			break;
		case ARCEND:
			if(aflag == ON){
				m_pFSF1Font->AtrDXYCrtSet(0, ax1, ay1);
				m_pFSF1Font->CrtPointInc();
				dp++;
				ax1 = ax2; ay1 = ay2;
			}
			ax2 = sx; ay2 = sy;
			aflag = ON;
			sx = x; sy = y;
			break;
		}
	}
	m_pFSF1Font->PointSetEnd();
}

void CARCFont::TrnsARCToFSF1NON()
{
	int		pcnt, pp;
	int		dp;
	int		atr;
	DBL		x, y;
	DBL		rx, ry;
	DBL		sx, sy;
	DBL		ax1, ay1;
	DBL		ax2, ay2;
	int		aflag;
	arcpointype	*dbuf;

	m_pFSF1Font->PointSetStart();
	dbuf = (arcpointype*)m_lpFBuf;
	pcnt = m_nDataLen;
	aflag = OFF;
	for(pp = 0, dp = 0; pp < pcnt; pp++){
		AtrDXYGet(dbuf, pp, &atr, &x, &y);
		if(atr == ENDOF || atr == MOVETO){
			if(pp != 0){
				if(aflag == ON){
					dp = ArcSet(dp, ax1, ay1, ax2, ay2, rx, ry);
					aflag = OFF;
				}
				m_pFSF1Font->AtrDXYCrtSet(0, sx, sy);
				m_pFSF1Font->RejionOn(dp);
				m_pFSF1Font->EorOn(dp);
				m_pFSF1Font->CrtPointInc();
				dp++;
			}
		}
		switch(atr){
		case ENDOF:
			break;
		case MOVETO:
			rx = x; ry = y;
			sx = x; sy = y;
			break;
		case LINETO:
			if(aflag == ON){
				dp = ArcSet(dp, ax1, ay1, ax2, ay2, sx, sy);
				aflag = OFF;
			}
			m_pFSF1Font->AtrDXYCrtSet(0, sx, sy);
			m_pFSF1Font->CrtPointInc();
			dp++;
			sx = x; sy = y;
			break;
		case ARCTO:
			if(aflag == ON){
				dp = ArcSet(dp, ax1, ay1, ax2, ay2, sx, sy);
				aflag = OFF;
			}
			ax1 = sx; ay1 = sy;
			sx = x; sy = y;
			break;
		case ARCEND:
			if(aflag == ON){
				m_pFSF1Font->AtrDXYCrtSet(0, ax1, ay1);
				m_pFSF1Font->CrtPointInc();
				dp++;
				ax1 = ax2; ay1 = ay2;
			}
			ax2 = sx; ay2 = sy;
			aflag = ON;
			sx = x; sy = y;
			break;
		}
	}
	m_pFSF1Font->PointSetEnd();
}

void CARCFont::AtrDXYGet(arcpointype dbuf[], int pp, int *atr, DBL *x, DBL *y)
{
	*atr = dbuf[pp].atr;
	*x = dbuf[pp].x * m_dTrnsTimeX;
	*y = dbuf[pp].y * m_dTrnsTimeY;
}

int CARCFont::ArcTrnsBezeSet(int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	int	stat;
	DBL	x0, y0, srag, erag, dr;
	DBL	rag;
	DBL	t1, t2;
	dbezetype bz;

	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &srag, &erag, &dr);
	}
	if(stat == FAIL){
		m_pFSF1Font->AtrDXYCrtSet(0, x1, y1);
		m_pFSF1Font->CrtPointInc();
		dp++;
		m_pFSF1Font->AtrDXYCrtSet(0, x2, y2);
		m_pFSF1Font->CrtPointInc();
		dp++;
		return(dp);
	}

	bz.x1 = x1; bz.y1 = y1;
	bz.x4 = x3; bz.y4 = y3;
	rag = (erag - srag) / 3.0 + srag;
	bz.x2 = cos(rag)*dr+x0;
	bz.y2 = sin(rag)*dr+y0;
	rag = (erag - srag) / 3.0 * 2.0 + srag;
	bz.x3 = cos(rag)*dr+x0;
	bz.y3 = sin(rag)*dr+y0;

	t1 = 1.0 / 3.0; t2 = 1.0 / 3.0 * 2.0;
	curveonpointtocntrlpoint(&bz, t1, t2);
	
	m_pFSF1Font->AtrDXYCrtSet(BEZEATR | CURVATR, x1, y1);
	m_pFSF1Font->CrtPointInc();
	dp++;
	m_pFSF1Font->AtrDXYCrtSet(BEZEATR, bz.x2, bz.y2);
	m_pFSF1Font->CrtPointInc();
	dp++;
	m_pFSF1Font->AtrDXYCrtSet(BEZEATR, bz.x3, bz.y3);
	m_pFSF1Font->CrtPointInc();
	dp++;
	return(dp);
}

int CARCFont::ArcSet(int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	m_pFSF1Font->AtrDXYCrtSet(CURVATR, x1, y1);
	m_pFSF1Font->CrtPointInc();
	dp++;
	m_pFSF1Font->AtrDXYCrtSet(CURVATR|CONTATR, x2, y2);
	m_pFSF1Font->CrtPointInc();
	dp++;
	return(dp);
}

void CARCFont::TrnsCopyFSF1FBuf()
{
	arcpointype	*dbuf;
	LPBYTE	FSF1FBuf;
	int		pcntmax;
	int		spp, epp, pcnt;
	int		dp;

	dbuf = (arcpointype*)m_lpFBuf;
	FSF1FBuf = m_pFSF1Font->FBufAdrsGet();
	pcntmax = m_pFSF1Font->PointCountGet();
	spp = 0;
	dp = 0;
	while(1){
		if(pcntmax <= spp){
			break;
		}
		epp = m_pFSF1Font->RejionEndGet(pcntmax, spp);
		pcnt = epp - spp;
		if(pcnt <= 0){
			break;
		}
		if(m_pFSF1Font->RejionChk(epp - 1)){
			dp = TrnsCopyRejionFSF1FBuf(&(FSF1FBuf[spp*FSF1SIZADD]), pcnt, dp);
		}
		spp = epp;
	}
	dp = EofSet(dbuf, dp);
	m_nDataLen = dp * sizeof(arcpointype); 

}

int CARCFont::TrnsCopyRejionFSF1FBuf(BYTE fbuf[], int pcnt, int dp)
{
	arcpointype	*dbuf;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;

	if(pcnt <= 1){
		return(dp);
	}
	dbuf = (arcpointype*)m_lpFBuf;
	m_pFSF1Font->AtrDXYGet(fbuf, 0, &atr0, &x0, &y0);
	dp = MoveToSet(dbuf, dp, x0, y0);	
	for(pp = 0; pp < pcnt;){
		p0 = m_pFSF1Font->PointLegal(pcnt, pp-1);
		p1 = m_pFSF1Font->PointLegal(pcnt, pp);
		p2 = m_pFSF1Font->PointLegal(pcnt, pp+1);
		p3 = m_pFSF1Font->PointLegal(pcnt, pp+2);
		p4 = m_pFSF1Font->PointLegal(pcnt, pp+3);
		m_pFSF1Font->AtrDXYGet(fbuf, p0, &atr0, &x0, &y0);
		m_pFSF1Font->AtrDXYGet(fbuf, p1, &atr1, &x1, &y1);
		m_pFSF1Font->AtrDXYGet(fbuf, p2, &atr2, &x2, &y2);
		m_pFSF1Font->AtrDXYGet(fbuf, p3, &atr3, &x3, &y3);
		m_pFSF1Font->AtrDXYGet(fbuf, p4, &atr4, &x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			dp = BezeToArcToSet(dbuf, dp, x1, y1, x2, y2, x3, y3, x4, y4);
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			dp = ArcToSet(dbuf, dp, x1, y1, x2, y2, x3, y3);
			pp += 2;
		}
		else{
			dp = LineToSet(dbuf, dp, x1, y1, x2, y2);
			pp += 1;
		}
	}
	AtrDXYGet(dbuf, dp-2, &atr2, &x2, &y2);
	AtrDXYGet(dbuf, dp-1, &atr1, &x1, &y1);
	if(atr1 == atr2 && x1 == x2 && y1 == y2){
		dp--;
	}
	return(dp);
}

int CARCFont::EofSet(arcpointype dbuf[], int dp)
{
	DBL		x, y;

	x = 0.0;
	y = 0.0;
	AtrDXYSet(dbuf, dp, ENDOF, x, y);
	dp++;
	return(dp);
}

int CARCFont::MoveToSet(arcpointype dbuf[], int dp, DBL x1, DBL y1)
{
	AtrDXYSet(dbuf, dp, MOVETO, x1, y1);
	dp++;
	return(dp);
}

int CARCFont::EorSet(arcpointype dbuf[], int dp)
{
	return(dp);
}

int CARCFont::LineToSet(arcpointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2)
{
	AtrDXYSet(dbuf, dp, LINETO, x2, y2);
	dp++;
	return(dp);
}

int CARCFont::BezeToArcToSet(arcpointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL	t, t1, b1, b2, b3, b4;
	DBL	xx, yy;

	t = 0.5;
	t1 = 1 - t;
	b1 = t1 * t1 * t1;
	b2 = 3 * t * t1 * t1;
	b3 = 3 * t * t * t1;
	b4 = t * t * t;
	xx = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
	yy = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);

	AtrDXYSet(dbuf, dp, ARCTO, xx, yy);
	dp++;
	AtrDXYSet(dbuf, dp, ARCEND, x4, y4);
	dp++;
	return(dp);
}

int CARCFont::ArcToSet(arcpointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	AtrDXYSet(dbuf, dp, ARCTO, x2, y2);
	dp++;
	AtrDXYSet(dbuf, dp, ARCEND, x3, y3);
	dp++;
	return(dp);
}

void CARCFont::AtrDXYSet(arcpointype dbuf[], int dp, int mode, DBL x, DBL y)
{
	dbuf[dp].atr = mode;
	dbuf[dp].x = (int)(x / m_dTime);
	dbuf[dp].y = (int)(ARCMESH - y / m_dTime);
}

// ï∂éöï\é¶ä÷êî
void CARCFont::DrawOut(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL rag)
{
	arcpointype	*dbuf;
	int		pp, pcnt, atr;
	int		aflag;
	DBL		x, y;
	DBL		x1, y1, x2, y2;
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
	TrnsElementInit(sx, sy, wd, hi, rag);
	dbuf = (arcpointype*)m_lpFBuf;
	pcnt = m_nDataLen;
	aflag = OFF;
	plgnfillerror(OFF);
	plgnfillstart();
	for(pp = 0; pp < pcnt; pp++){
		AtrDXYGet(dbuf, pp, &atr, &x, &y);
		TrnsXY(&x, &y);
		if(atr == ENDOF || atr == MOVETO){
			if(aflag == ON){
				DrawOutArc(pcDC, x1, y1, x2, y2, sx, sy);
				aflag = OFF;
			}
			if(pp != 0){
				plgnfillrejionend();
			}
		}
		switch(atr){
		case ENDOF:
			break;
		case MOVETO:
			plgnfillrejionstart();	
			DrawOutMove(pcDC, x, y);
			sx = x; sy = y;
			break;
		case LINETO:
			if(aflag == ON){
				DrawOutArc(pcDC, x1, y1, x2, y2, sx, sy);
				aflag = OFF;
			}
			DrawOutLine(pcDC, sx, sy, x, y);
			sx = x; sy = y;
			break;
		case ARCTO:
			if(aflag == ON){
				DrawOutArc(pcDC, x1, y1, x2, y2, sx, sy);
				aflag = OFF;
			}
			x1 = sx; y1 = sy;
			sx = x; sy = y;
			break;
		case ARCEND:
			if(aflag == ON){
				DrawOutLine(pcDC, sx, sy, x1, y1);
				x1 = x2; y1 = y2;
			}
			x2 = sx; y2 = sy;
			sx = x; sy = y;
			aflag = ON;
			break;
		}
	}
	plgnfilldraw(pcDC->m_hDC);
	if(pcoldPen != NULL){
		pcDC->SelectObject(pcoldPen);
		chPen.DeleteObject();
	}
}

void CARCFont::TrnsElementInit(DBL sx, DBL sy, DBL wd, DBL hi, DBL rag)
{
	m_dSX = sx;
	m_dSY = sy;
	m_dXTime = (wd / ARCMESH);
	m_dYTime = (hi / ARCMESH);
	m_dSin = sin(rag);
	m_dCos = cos(rag);
}

void CARCFont::SetBold(int flag)
{
	m_nBold = flag;
}

void CARCFont::SetSlope(int flag)
{
	m_nSlope = flag;
}

void CARCFont::TrnsXY(LPDBL x, LPDBL y)
{
	DBL		xx, yy;
	DBL		tx, ty;

	xx = (*x - m_dMoveX + m_dAddX) - ARCMESH / 2;
	yy = (ARCMESH - *y + m_dMoveY + m_dAddY) - ARCMESH / 2;
	tx = (xx * m_dCos - yy * m_dSin + ARCMESH / 2);
	ty = (xx * m_dSin + yy * m_dCos + ARCMESH / 2);
	if(m_nTateYoko == YOKOGAKI || m_nTateYoko == TATEGAKI){
		if(m_nSlope == OFF){
			*x = tx * m_dXTime + m_dSX;
			*y = ty * m_dYTime + m_dSY;
		}
		else{
			*x = (tx - ty * 0.5) * m_dXTime + m_dSX + ARCMESH*m_dXTime*0.3;
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
			*y = (ty + tx * 0.5) * m_dYTime + m_dSY - ARCMESH*m_dYTime*0.3;;
		}
	}

	/*
	if(m_nTateYoko == YOKOGAKI){
		if(m_nSlope == OFF){
			*x = tx * m_dXTime + m_dSX;
			*y = ty * m_dYTime + m_dSY;
		}
		else{
			*x = (tx - ty * 0.5) * m_dXTime + m_dSX;
			*y = ty * m_dYTime + m_dSY;
		}
	}
	else if(m_nTateYoko == TATEGAKI){
		if(m_nSlope == OFF){
			*x = tx * m_dXTime + m_dSX;
			*y = ty * m_dYTime + m_dSY;
		}
		else{
			*x = tx * m_dXTime + m_dSX;
			*y = (ty - tx * 0.5) * m_dYTime + m_dSY + ARCMESH*m_dYTime*0.25;
			//*y = (ty + tx * 0.5) * m_dYTime + m_dSY;
		}
	}
	else if(m_nTateYoko == YOKOOKI){
		if(m_nSlope == OFF){
			*x = tx * m_dXTime + m_dSX;
			*y = ty * m_dYTime + m_dSY;
		}
		else{
			*x = (tx - ty * 0.5) * m_dXTime + m_dSX;
			*y = ty * m_dYTime + m_dSY;
		}
	}
	else if(m_nTateYoko == TATEOKI){
		if(m_nSlope == OFF){
			*x = tx * m_dXTime + m_dSX;
			*y = ty * m_dYTime + m_dSY;
		}
		else{
			*x = tx * m_dXTime + m_dSX;
			*y = (ty + tx * 0.5) * m_dYTime + m_dSY;
		}
	}
	*/
}

void CARCFont::DrawOutMove(CDC *pcDC, DBL x, DBL y)
{
	pcDC->MoveTo((int)x, (int)y);
	plgnfillmove(pcDC->m_hDC, x, y);
}

void CARCFont::DrawOutLine(CDC *pcDC, DBL x1, DBL y1, DBL x2, DBL y2)
{
	pcDC->LineTo((int)x2, (int)y2);
	plgnfillline(pcDC->m_hDC, x1, y1, x2, y2);
}

void CARCFont::DrawOutArc(CDC *pcDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	// ÇSï™äÑÇÃíºê¸Ç≈ï`âÊÇµÇƒÇ¢ÇÈÇÃÇ≈Ç†Ç∆Ç≈ã»ê¸Ç…Ç∑ÇÈïKóvÇ†ÇË
	int	stat;
	DBL	x0, y0, srag, erag, dr;
	DBL	rag;
	DBL	t1, t2;
	dbezetype bz;

	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &srag, &erag, &dr);
	}
	if(stat == FAIL){
		DrawOutLine(pcDC, x1, y1, x2, y2);
		DrawOutLine(pcDC, x2, y2, x3, y3);
		plgnfillline(pcDC->m_hDC, x1, y1, x2, y2);
		plgnfillline(pcDC->m_hDC, x2, y2, x3, y3);
		return;
	}
	bz.x1 = x1; bz.y1 = y1;
	bz.x4 = x3; bz.y4 = y3;
	rag = (erag - srag) / 3.0 + srag;
	bz.x2 = cos(rag)*dr+x0;
	bz.y2 = sin(rag)*dr+y0;
	rag = (erag - srag) / 3.0 * 2.0 + srag;
	bz.x3 = cos(rag)*dr+x0;
	bz.y3 = sin(rag)*dr+y0;
	t1 = 1.0 / 3.0; t2 = 1.0 / 3.0 * 2.0;
	pcDC->LineTo((int)bz.x2, (int)bz.y2);
	pcDC->LineTo((int)bz.x3, (int)bz.y3);
	pcDC->LineTo((int)bz.x4, (int)bz.y4);
	plgnfillline(pcDC->m_hDC, bz.x1, bz.y1, bz.x2, bz.y2);
	plgnfillline(pcDC->m_hDC, bz.x2, bz.y2, bz.x3, bz.y3);
	plgnfillline(pcDC->m_hDC, bz.x3, bz.y3, bz.x4, bz.y4);
	//curveonpointtocntrlpoint(&bz, t1, t2);
}

