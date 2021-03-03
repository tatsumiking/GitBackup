//
// Easy Scanner System
//
// object fsf1font.cpp  ÇrÇeÇPÉ^ÉCÉvÇdÇrèëëÃèàóùä÷êî
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include <memory.h>
#include <math.h>

#pragma optimize( "", off )

#include "..\object\datalist.h"
#include "..\object\fsf1font.h"

#define	NODBG	1

#define	DIVCALC	1

extern "C" {
	void Dispatch();
	int senvchckflag(int flag);
	int curve3pcenterget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL FAR *x0, DBL FAR *y0);
	int curve3pragrget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0, DBL FAR *rag01, DBL FAR *rag03, DBL FAR *r);
	void curvebezecontrolpointget(DBL x1, DBL y1, DBL x2, DBL y2, DBL rag, DBL *xp, DBL *yp);
	int curvecenterget(DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL *x0, DBL *y0);
	int curvedivpointget(DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL rag2, DBL *x12, DBL *y12);
	BOOL curvedivragchk(DBL rag1, DBL rag2);
	void curvenoragtocurverag(DBL *rag1, DBL *rag2);
	int curveragnoget(DBL rag);
	int curveragrget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x0, DBL y0, DBL *rag01, DBL *rag02, DBL *r);
	int curveonlinechk(DBL x1, DBL y1, DBL x2, DBL y2, DBL x, DBL y);
	BOOL  pdrwatrarcchk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrbezechk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrcurvechk(int atr1);
	void  pdrwmove(HDC PaintDC, DBL x1, DBL y1);
	void  pdrwline(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2);
	void  pdrwarc(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void  pdrwbeze(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	void  pdrwfreecurve(HDC PaintDC, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL   pdrwendragget(int atr1, int atr2, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL   pdrwstartragget(int atr0, int atr1, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2);

	void  plgnfilldraw(HDC PaintDC);
	void  plgnfilloutline(HDC PaintDC, DBL sx, DBL sy, DBL ex, DBL ey);
	void  plgnfillrejionend();
	void  plgnfillrejionstart();
	void  plgnfillstart();
	void  plgnfillmove(HDC PaintDC, DBL x1, DBL y1);
	void  plgnfillline(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2);
	void  plgnfillarc(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void  plgnfillbeze(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	void  plgnfillfreecurve(HDC PaintDC,int atr0,int atr1,int atr2,DBL x0,DBL y0,DBL x1,DBL y1,DBL x2,DBL y2,DBL x3,DBL y3);
	void poutsetoverlapflag(int flag);
	int poutcheckoverlapend();
	int   poutomitmodecheck();
	void  plotdxymmtostep(LPDBL x, LPDBL y);
	int	  poutpaintmodechk();
	void  poutarc(int nComID, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	int   poutareachk(DBL sx, DBL sy, DBL ex, DBL ey);
	void  poutbeze(int nComID, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	void  poutblockend(int nComID);
	void  poutblockstart(int nComID);
	int   poutfillflagchk();
	void  poutfreecurve(int nComID, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void  poutline(int nComID, DBL sx, DBL sy, DBL ex, DBL ey);
	void  poutmove(int nComID, DBL dx, DBL dy);
	void  poutoutline(int nComID, DBL sx, DBL sy, DBL ex, DBL ey);
	void  poutrejionend(int nComID);
	void  poutrejionstart(int nComID);
	int poutchkdeepcut(DBL x0,DBL y0,DBL x1,DBL y1,DBL x2,DBL y2);
	void poutstartdeepcut(DBL x1, DBL y1, DBL x2, DBL y2);
	void poutenddeepcut(DBL x1, DBL y1, DBL x2, DBL y2);
	void  scrndxymmtorltv(LPDBL x, LPDBL y);
	void  plotdxymmtostep(LPDBL x, LPDBL y);
	void  scrnrevercecheck(int *flagx, int *flagy);
	int   scrnrltvareachk(DBL sx, DBL sy, DBL ex, DBL ey);
	int   scrnrltvarealegal(LPDBL sx, LPDBL sy, LPDBL ex, LPDBL ey);
	void  senvgettmppath(LPSTR path);
	HBRUSH senvgetconrbrush(int *nthick);
	HPEN senvgetconrpen();
	int	senvcheckconrmode();
	int senvgetplgnlimit();
	void scrnsetpreviewpen(HDC PaintDC, int r, int g, int b, int thick);
	void scrnpenend(HDC PaintDC);

	void scrnsetrltvsize(DBL sizex, DBL sizey);
	void scrngetrltvsize(LPDBL sizex, LPDBL sizey);
	void scrnsetrltvtop(DBL topx, DBL topy);
	void scrngetrltvtop(LPDBL topx, LPDBL topy);
	void scrnsetrltvmove(DBL mvx, DBL mvy);
	void scrngetrltvmove(LPDBL mvx, LPDBL mvy);
	DBL scrnsetxytime(DBL timex, DBL timey);
	void scrnrltvareaset(DBL rsx, DBL rsy, DBL rex, DBL rey);
	void scrnrltvareaget(LPDBL rsx, LPDBL rsy, LPDBL rex, LPDBL rey);
	void scrnsetdisprltvmove(DBL dmvx, DBL dmvy);
	void scrngetdisprltvmove(LPDBL dmvx, LPDBL dmvy);
}

typedef struct {
	int	n000,n001,n002,n003,n004,n005,n006,n007,n008,n009;
	int	n010,n011,n012,n013,n014,n015,n016,n017,n018,n019;
	int	n020,n021,n022,n023,n024,n025,n026,n027,n028,n029;
	int	n030,n031,n032,n033,n034,n035,n036,n037,n038,n039;
	int	n040,n041,n042,n043,n044,n045,n046,n047,n048,n049;
	int	n050,n051,n052,n053,n054,n055,n056,n057,n058,n059;
	int	n060,n061,n062,n063,n064,n065,n066,n067,n068,n069;
	int	n070,n071,n072,n073,n074,n075,n076,n077,n078,n079;
	int	n080,n081,n082,n083,n084,n085,n086,n087,n088,n089;
	int	n090,n091,n092,n093,n094,n095,n096,n097,n098,n099;
	int	n100,n101,n102,n103,n104,n105,n106,n107,n108,n109;
	int	n110,n111,n112,n113,n114,n115,n116,n117,n118,n119;
	int	n120,n121,n122,n123,n124,n125,n126,n127,n128,n129;
	int	n130,n131,n132,n133,n134,n135,n136,n137,n138,n139;
	int	n140,n141,n142,n143,n144,n145,n146,n147,n148,n149;
	int	n150,n151,n152,n153,n154,n155,n156,n157,n158,n159;
	int	n160,n161,n162,n163,n164,n165,n166,n167,n168,n169;
	int	n170,n171,n172,n173,n174,n175,n176,n177,n178,n179;
	int	n180,n181,n182,n183,n184,n185,n186,n187,n188,n189;
	int	n190,n191,n192,n193,n194,n195,n196,n197,n198,n199;
	int	n200,n201,n202,n203,n204,n205,n206,n207,n208,n209;
	int	n210,n211,n212,n213,n214,n215,n216,n217,n218,n219;
	int	n220,n221,n222,n223,n224,n225,n226,n227,n228,n229;
	int	n230,n231,n232,n233,n234,n235,n236,n237,n238,n239;
	int	n240,n241,n242,n243,n244,n245,n246,n247,n248,n249;
	int	n250,n251,n252,n253,n254,n255;
} ntbltype;

IMPLEMENT_SERIAL(CFSF1Font, CObject, 0)
CFSF1Font::CFSF1Font()
{
}

CFSF1Font::CFSF1Font(int nDummy)
{
	m_pcBoldLib = new CBoldLib(0);
	m_nDataLen = 0;
	m_sFontfile[0] = '\0';
	m_fRevercex = 0;
	m_fRevercey = 0;
	m_nDataSx = 0;
	m_nDataSy = 0;
	m_nDataEx = 0;
	m_nDataEy = 0;
	m_fileFont = NULL;
	m_fileDFont = NULL;
	m_dStartX = 0;
	m_dStartY = 0;
	m_dWidth = 0;
	m_dHight = 0;
	m_dCrtX = 0;
	m_dCrtY = 0;
	m_nCrtPP = 0;
	m_nDataNo = 0;

	if(nDummy == 0){
		OpenFileInitTable();
	}
	else{
		OpenFileInitTable1();
	}
	m_pcMemDC = NULL;
	m_pcBitmap = NULL;
	m_pcBoldMemDC = NULL;
	m_pcBoldBitmap = NULL;
}

CFSF1Font::~CFSF1Font()
{
	CloseFile();
	if(m_pcBitmap != NULL && m_pcMemDC != NULL){
		DeleteDIBDC();
	}
	if(m_pcBoldBitmap != NULL && m_pcBoldMemDC != NULL){
		DeleteBoldDIBDC();
	}
	delete(m_pcBoldLib);
}

void CFSF1Font::Serialize(CArchive& ar)
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

void CFSF1Font::OpenFileInitTable()
{
	char	szTmpPath[FNAMEMAX];
	int		byte;

	senvgettmppath(szTmpPath);
	wsprintf(m_sFontfile, "%s\\fsf1file.sf1", szTmpPath);
	FileNullSet();
	m_fileFont = fopen(m_sFontfile, "r+b");
	if(m_fileFont == NULL){
		return;
	}
	m_nUseMaxIdx = 1;
	for(byte = 0; byte < MOJIFLAGMAX; byte++){
		m_fFontUse[byte] = 0;
	}
}

void CFSF1Font::OpenFileInitTable1()
{
	char	szTmpPath[FNAMEMAX];
	int		byte;

	senvgettmppath(szTmpPath);
	wsprintf(m_sFontfile, "%s\\fsf1file1.sf1", szTmpPath);
	FileNullSet();
	m_fileFont = fopen(m_sFontfile, "r+b");
	if(m_fileFont == NULL){
		return;
	}
	m_nUseMaxIdx = 1;
	for(byte = 0; byte < MOJIFLAGMAX; byte++){
		m_fFontUse[byte] = 0;
	}
}

void CFSF1Font::CloseFile()
{
	if(m_fileFont != NULL){
		fclose(m_fileFont);
	}
}

void CFSF1Font::FileNullSet()
{
	BYTE	nullbuf[1024];
	int		block, rest, i;

	memset(nullbuf, 0, 1024);
	block = (int)(FLIBDATATOP / 1024L);
	rest = (int)(FLIBDATATOP % 1024L);
	m_fileFont = fopen(m_sFontfile, "wb");
	for(i = 0; i < block; i++){
		fwrite(nullbuf, 1, 1024, m_fileFont);
	}
	if(rest != 0){
		fwrite(nullbuf, 1, rest, m_fileFont);
	}
	fclose(m_fileFont);
}

int CFSF1Font::DFileReadOpen(LPSTR fname)
{
	m_fileDFont = fopen(fname, "r+b");
	if(m_fileDFont == NULL){
		return(FAIL);
	}
	return(0);
}

void CFSF1Font::DFileWriteOpen(LPSTR fname)
{
	DFileNullSet(fname);
	m_fileDFont = fopen(fname, "r+b");
	m_nDataNo = 0;
}

int CFSF1Font::DFileClose()
{
	if(m_fileDFont == NULL){
		return(FAIL);
	}
	fclose(m_fileDFont);
	m_fileDFont = NULL;
	return(0);
}

void CFSF1Font::DFileNullSet(LPSTR fname)
{
	BYTE	nullbuf[1024];
	int		block, rest, i;

	memset(nullbuf, 0, 1024);
	block = (int)(FLIBDATATOP / 1024L);
	rest = (int)(FLIBDATATOP % 1024L);
	m_fileDFont = fopen(fname, "wb");
	for(i = 0; i < block; i++){
		fwrite(nullbuf, 1, 1024, m_fileDFont);
	}
	if(rest != 0){
		fwrite(nullbuf, 1, rest, m_fileDFont);
	}
	fclose(m_fileDFont);
}

int CFSF1Font::GetNextDataNo()
{
	return(m_nDataNo);
}

void CFSF1Font::DataNoInc()
{
	m_nDataNo++;
}

int CFSF1Font::DFileDataLoad(int datano)
{
	long	idxseekp, datseekp;
	int			datlen;
	f1idxtype	f1idx;
	f1dhtype	f1dhead;

	if(m_fileDFont == NULL){
		return(FAIL);
	}
	IdxSeekPointGet(datano, &idxseekp);

	fseek(m_fileDFont, idxseekp, 0);
	fread(&f1idx, sizeof(f1idxtype), 1, m_fileDFont);
	DataLenSeekPointGet(f1idx, &datlen, &datseekp);
	if(datlen == 0){
		m_nDataLen = 0;
		return(0);
	}

	fseek(m_fileDFont, datseekp, 0);
	if(fread(&f1dhead, sizeof(f1dhtype), 1, m_fileDFont) == 0){
		m_nDataLen = 0;
		return(0);
	}

	m_nDataLen = f1dhead.datalen;
	m_nDataSx = f1dhead.sx * MINIMAXTIME;
	m_nDataSy = f1dhead.sy * MINIMAXTIME;
	m_nDataEx = f1dhead.ex * MINIMAXTIME;
	m_nDataEy = f1dhead.ey * MINIMAXTIME;

	if(m_nDataLen + (int)sizeof(f1dhtype) != datlen){
		m_nDataLen = 0;
		return(0);
	}
	if(fread(m_bFBuf, 1, datlen, m_fileDFont) == 0){
		m_nDataLen = 0;
		return(0);
	}
	return(0);
}

void CFSF1Font::DFileDataSave(int datano)
{
	long	idxseekp, datseekp;
	int			datlen;
	f1idxtype	f1idx;
	f1dhtype	f1dhead;

	if(m_fileDFont == NULL){
		return;
	}
	IdxSeekPointGet(datano, &idxseekp);
	fseek(m_fileDFont, idxseekp, 0);
	if(m_nDataLen == 0){
		datlen = 0;
		datseekp = 0L;
		DataLenSeekPointSet(&f1idx, datlen, datseekp);
		fwrite(&f1idx, sizeof(f1idxtype), 1, m_fileDFont);
		return;
	}
	fread(&f1idx, sizeof(f1idxtype), 1, m_fileDFont);
	DataLenSeekPointGet(f1idx, &datlen, &datseekp);
	if((m_nDataLen + (int)sizeof(f1dhtype)) > datlen){
		fseek(m_fileDFont, 0L, 2);
		datseekp = ftell(m_fileDFont);
	}

	f1dhead.headlen = 0;
	f1dhead.datalen = m_nDataLen;
	f1dhead.sx = m_nDataSx / MINIMAXTIME;
	f1dhead.sy = m_nDataSy / MINIMAXTIME;
	f1dhead.ex = m_nDataEx / MINIMAXTIME;
	f1dhead.ey = m_nDataEy / MINIMAXTIME;

	DataLenSeekPointSet(&f1idx, m_nDataLen+sizeof(f1dhtype), datseekp);

	fseek(m_fileDFont, idxseekp, 0);
	fwrite(&f1idx, sizeof(f1idxtype), 1, m_fileDFont);
	fseek(m_fileDFont, datseekp, 0);
	if(fwrite(&f1dhead, sizeof(f1dhtype), 1, m_fileDFont) == 0){
		return;
	}
	if(fwrite(m_bFBuf, 1, m_nDataLen, m_fileDFont) == 0){
		return;
	}
	return;
}

int CFSF1Font::Create()
{
	int		nMask, nByte;
	int		idx;

	nMask = 0x0080;
	nByte = 0;
	for(idx = 1; idx < m_nUseMaxIdx; idx++){
		if(idx >= FSF1FONTMAX){
			return(FAIL);
		}
		if((m_fFontUse[nByte] & nMask) == 0){
			break;
		}
		nMask >>= 1;
		if(nMask == 0){
			nByte++;
			nMask = 0x0080;
		}
	}
	if(idx >= FSF1FONTMAX){
		return(FAIL);
	}
	m_fFontUse[nByte] |= nMask;
	if(idx >= m_nUseMaxIdx){
		m_nUseMaxIdx++;
	}
	return(idx);
}

void CFSF1Font::Free(int nFidx)
{
	int		nMask, nByte;

	if(nFidx < 1 && FSF1FONTMAX < nFidx){
		return;
	}
	nMask = 0x0080 >> (nFidx - 1) % 8;
	nByte = (nFidx - 1) / 8;
	m_fFontUse[nByte] &= ~nMask;
}

LPBYTE CFSF1Font::FBufAdrsGet()
{
	return(m_bFBuf);
}

void CFSF1Font::GetMojiMiniMax(int nFidx, int w, int h, int *minix, int *miniy, int *maxx, int *maxy)
{
	DataLoad(nFidx);
	if(m_nDataLen == 0){
		*minix = 0;
		*miniy = 0;
		*maxx = w;
		*maxy = h;
		return;
	}
	*minix = (int)((long)m_nDataSx * (long)w / (long)FSF1MESH);
	*miniy = (int)((long)m_nDataSy * (long)h / (long)FSF1MESH);
	*maxx = (int)((long)m_nDataEx * (long)w / (long)FSF1MESH);
	*maxy = (int)((long)m_nDataEy * (long)h / (long)FSF1MESH);
}

int CFSF1Font::Copy(int nFidx)
{
	int	nnFidx;

	DataLoad(nFidx);
	nnFidx = Create();
	DataSave(nnFidx);
	return(nnFidx);
}

void CFSF1Font::DataMiniMaxRenew(int nFidx)
{
	DataLoad(nFidx);
	PointMiniMaxGet();
	DataSave(nFidx);
}

void CFSF1Font::DataSave(int nFidx)
{
	long		idxseekp, datseekp;
	int			datlen;
	f1idxtype	f1idx;
	f1dhtype	f1dhead;

	if(m_fileFont == NULL){
		return;
	}

	IdxSeekPointGet(nFidx, &idxseekp);
	fseek(m_fileFont, idxseekp, 0);
	if(m_nDataLen == 0){
		datlen = 0;
		datseekp = 0L;
		DataLenSeekPointSet(&f1idx, datlen, datseekp);
		fwrite(&f1idx, sizeof(f1idxtype), 1, m_fileFont);
		return;
	}
	fread(&f1idx, sizeof(f1idxtype), 1, m_fileFont);
	DataLenSeekPointGet(f1idx, &datlen, &datseekp);
	if((m_nDataLen + (int)sizeof(f1dhtype)) > datlen){
		fseek(m_fileFont, 0L, 2);
		datseekp = ftell(m_fileFont); 
	}

	f1dhead.headlen = 0;
	f1dhead.datalen = m_nDataLen;
	f1dhead.sx = m_nDataSx / MINIMAXTIME;
	f1dhead.sy = m_nDataSy / MINIMAXTIME;
	f1dhead.ex = m_nDataEx / MINIMAXTIME;
	f1dhead.ey = m_nDataEy / MINIMAXTIME;

	DataLenSeekPointSet(&f1idx, m_nDataLen+sizeof(f1dhtype), datseekp);

	fseek(m_fileFont, idxseekp, 0);
	fwrite(&f1idx, sizeof(f1idxtype), 1, m_fileFont);
	fseek(m_fileFont, datseekp, 0);
	if(fwrite(&f1dhead, sizeof(f1dhtype), 1, m_fileFont) == 0){
		return;
	}
	if(fwrite(m_bFBuf, 1, m_nDataLen, m_fileFont) == 0){
		return;
	}
	return;
}

void CFSF1Font::DataLoad(int nFidx)
{
	long	idxseekp, datseekp;
	int			datlen;
	f1idxtype	f1idx;
	f1dhtype	f1dhead;

	if(m_fileFont == NULL){
		m_nDataLen = 0;
		return;
	}
	IdxSeekPointGet(nFidx, &idxseekp);
	fseek(m_fileFont, idxseekp, 0);
	fread(&f1idx, sizeof(f1idxtype), 1, m_fileFont);
	DataLenSeekPointGet(f1idx, &datlen, &datseekp);
	if(datlen == 0){
		m_nDataLen = 0;
		return;
	}

	fseek(m_fileFont, datseekp, 0);
	if(fread(&f1dhead, sizeof(f1dhtype), 1, m_fileFont) == 0){
		m_nDataLen = 0;
		return;
	}

	m_nDataLen = f1dhead.datalen;
	m_nDataSx = f1dhead.sx * MINIMAXTIME;
	m_nDataSy = f1dhead.sy * MINIMAXTIME;
	m_nDataEx = f1dhead.ex * MINIMAXTIME;
	m_nDataEy = f1dhead.ey * MINIMAXTIME;
	if(m_nDataLen + (int)sizeof(f1dhtype) != datlen){
		m_nDataLen = 0;
		return;
	}
	if(fread(m_bFBuf, 1, datlen, m_fileFont) == 0){
		m_nDataLen = 0;
		return;
	}
	return;
}

void CFSF1Font::PointMiniMaxGet()
{
	int		pcnt;
	int		pp, atr;
	DBL		x, y;

	pcnt = PointCountGet();
	if(pcnt <= 0){
		m_nDataSx = 0;
		m_nDataSy = 0;
		m_nDataEx = FSF1MESH-1;
		m_nDataEy = FSF1MESH-1;
		return;
	}
	m_nDataSx = FSF1MESH * 2;
	m_nDataSy = FSF1MESH * 2;
	m_nDataEx = -FSF1MESH;
	m_nDataEy = -FSF1MESH;
	for(pp = 0; pp < pcnt; pp++){
		AtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
		if(x < m_nDataSx){
			m_nDataSx = (int)x;
		}
		if(y < m_nDataSy){
			m_nDataSy = (int)y;
		}
		if(m_nDataEx < x){
			m_nDataEx = (int)x;
		}
		if(m_nDataEy < y){
			m_nDataEy = (int)y;
		}
	}
}

void CFSF1Font::PointSetStart()
{
	m_nCrtPP = 0;
}

void CFSF1Font::MovePointSet(DBL dx, DBL dy)
{
	m_dCrtX = dx; m_dCrtY = dy;
}

void CFSF1Font::CurvePointSet(DBL x1, DBL y1, DBL x2, DBL y2)
{
	int		atr;

	if(m_nCrtPP >= FSF1POINTMAX){
		return;
	}
	if(RejionChk(m_nCrtPP-1) == OFF && m_nCrtPP != 0){
		AtrGet(m_bFBuf, m_nCrtPP-1, &atr);
		atr |= CURVATR;
		AtrSet(m_bFBuf, m_nCrtPP-1, atr);
	}
	AtrDXYSet(m_bFBuf, m_nCrtPP, CURVATR|CONTATR, x1, y1);
	m_nCrtPP++;
	m_dCrtX = x2; m_dCrtY = y2;
}

void CFSF1Font::LinePointSet(DBL x1, DBL y1, DBL x2, DBL y2)
{
	if(m_nCrtPP >= FSF1POINTMAX){
		return;
	}
	AtrDXYSet(m_bFBuf, m_nCrtPP, 0, x1, y1);
	m_nCrtPP++;
	m_dCrtX = x2; m_dCrtY = y2;
}

void CFSF1Font::BezePointSet(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	if(m_nCrtPP >= FSF1POINTMAX-2){
		return;
	}
	AtrDXYSet(m_bFBuf, m_nCrtPP, CURVATR|BEZEATR, x1, y1);
	m_nCrtPP++;
	AtrDXYSet(m_bFBuf, m_nCrtPP, BEZEATR, x2, y2);
	m_nCrtPP++;
	AtrDXYSet(m_bFBuf, m_nCrtPP, BEZEATR, x3, y3);
	m_nCrtPP++;
	m_dCrtX = x4; m_dCrtY = y4;
}

void CFSF1Font::CrtContAtrOff()
{
	int		atr;

	if(m_nCrtPP >= FSF1POINTMAX){
		return;
	}
	AtrGet(m_bFBuf, m_nCrtPP-1, &atr);
	atr &= ~CONTATR;
	AtrSet(m_bFBuf, m_nCrtPP-1, atr);
}

void CFSF1Font::CurveToStartPointSet()
{
	if(m_nCrtPP >= FSF1POINTMAX){
		return;
	}
	AtrDXYSet(m_bFBuf, m_nCrtPP, CURVATR, m_dCrtX, m_dCrtY);
	m_nCrtPP++;
}

void CFSF1Font::CurveToPointSet(DBL x, DBL y)
{
	if(m_nCrtPP >= FSF1POINTMAX){
		return;
	}
	AtrDXYSet(m_bFBuf, m_nCrtPP, CURVATR|CONTATR, x, y);
	m_nCrtPP++;
	m_dCrtX = x; m_dCrtY = y;
}

void CFSF1Font::CurveToEndPointSet(DBL x, DBL y)
{
	m_dCrtX = x; m_dCrtY = y;
}

void CFSF1Font::RejionSet()
{
	EorOn(m_nCrtPP-1);
	RejionOn(m_nCrtPP-1);
}

void CFSF1Font::NoRejionSet()
{
	EorOn(m_nCrtPP-1);
	RejionOff(m_nCrtPP-1);
}

void CFSF1Font::PointSetEnd()
{
	m_nDataLen = m_nCrtPP * FSF1SIZADD;
}

void CFSF1Font::GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	GetWakuDMiniMax(minix, miniy, maxx, maxy);
}

void CFSF1Font::GetWakuDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	DBL		x, y;

	x = 0;
	y = 0;
	FontScaleTrns(x, y, &x, &y);
	*minix = min(x, (*minix));
	*miniy = min(y, (*miniy));
	*maxx = max(x, (*maxx));
	*maxy = max(y, (*maxy));

	x = FSF1MESH;
	y = FSF1MESH;
	FontScaleTrns(x, y, &x, &y);
	*minix = min(x, (*minix));
	*miniy = min(y, (*miniy));
	*maxx = max(x, (*maxx));
	*maxy = max(y, (*maxy));
}

void CFSF1Font::GetDataDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	int		pcntmax, pp, atr;
	DBL		x, y;

	*minix = INTMAX;
	*miniy = INTMAX;
	*maxx = -INTMAX;
	*maxy = -INTMAX;
	pcntmax = PointCountGet();
	if(pcntmax == 0){
		GetWakuDMiniMax(minix, miniy, maxx, maxy);
		return;
	}
	for(pp = 0; pp < pcntmax; pp++){
		AtrDXYGet(m_bFBuf, pp, &atr, &x, &y);
		FontScaleTrns(x, y, &x, &y);
		*minix = min(x, (*minix));
		*miniy = min(y, (*miniy));
		*maxx = max(x, (*maxx));
		*maxy = max(y, (*maxy));
	}
}

void CFSF1Font::TrnsPlotOut(int nComID, DBL sx, DBL sy, DBL nW, DBL nH, int nFidx)
{
	Dispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}
	m_fRevercex = OFF;
	m_fRevercey = OFF;
	DataLoad(nFidx);
	TrnsElementSet(sx, sy, nW, nH);
	poutblockstart(nComID);
	DataPlotOut(nComID);
	poutblockend(nComID);
}

void CFSF1Font::TrnsFillDraw(CDC *pcDC, DBL dsx, DBL dsy, DBL dw, DBL dh, int nFidx)
{
	int		rag;

	rag = 0;
	//scrnrevercecheck(&m_fRevercex, &m_fRevercey);
	m_fRevercex = OFF;
	m_fRevercey = OFF;
	DataLoad(nFidx);
	TrnsElementSet(dsx, dsy, dw, dh);
	plgnfillstart();
	FillDrawOut(pcDC);
	plgnfilldraw(pcDC->m_hDC);
}

void CFSF1Font::TrnsPlgnDraw(CDC *pcDC, DBL dsx, DBL dsy, DBL dw, DBL dh, int nFidx)
{
	int		rag;

	rag = 0;
	//scrnrevercecheck(&m_fRevercex, &m_fRevercey);
	m_fRevercex = OFF;
	m_fRevercey = OFF;
	DataLoad(nFidx);
	TrnsElementSet(dsx, dsy, dw, dh);
	PlgnDrawOut(pcDC);
}

void CFSF1Font::TrnsElementSet(DBL dsx, DBL dsy, DBL dw, DBL dh)
{
	m_dStartX = dsx;
	m_dStartY = dsy;
	m_dWidth = dw;
	m_dHight = dh;
}

void CFSF1Font::FontScaleTrns(DBL dx, DBL dy, DBL *retdx, DBL *retdy)
{
	DBL		tx, ty;

	if(m_fRevercex == ON){
		tx = (-dx) * (m_dWidth / 1024.0);
	}
	else{
		tx = dx * (m_dWidth / 1024.0);
	}
	if(m_fRevercey == ON){
		ty = -(1024.0 - dy) * (m_dHight / 1024.0);
	}
	else{
		ty = (1024.0 - dy) * (m_dHight / 1024.0);
	}
	*retdx = tx + m_dStartX;
	*retdy = ty + m_dStartY;
}

void CFSF1Font::FBufScaleTrns(DBL dx, DBL dy, DBL *retdx, DBL *retdy)
{
	DBL		tx, ty;

	tx = dx - m_dStartX;
	ty = dy - m_dStartY;
	if(m_fRevercex == ON){
		tx = -(tx * (1024.0 / m_dWidth));
	}
	else{
		tx = tx * (1024.0 / m_dWidth);
	}
	if(m_fRevercey == ON){
		ty = -(1024.0 - dy * (1024.0 / m_dHight));
	}
	else{
		ty = 1024.0 - ty * (1024.0 / m_dHight);
	}
	*retdx = tx;
	*retdy = ty;
}

int CFSF1Font::RejionEndGet(int pcnt, int sp)
{
	int		pp;

	for(pp = sp; pp < pcnt; pp++){
		if(EorChk(pp) == ON){
			return(pp+1);
		}
	}
	return(pcnt);
}

int CFSF1Font::PointLegal(int pcnt, int pp)
{
	pp %= pcnt;
	pp += pcnt;
	pp %= pcnt;
	return(pp);
}

int CFSF1Font::EorChk(int pp)
{
	int		bytep;

	bytep = pp * FSF1SIZADD;
	if(m_bFBuf[bytep+FSF1ATRADD] & FEORMASK){
		return(ON);
	}
	else{
		return(OFF);
	}
}

void CFSF1Font::EorOn(int pp)
{
	int		bytep;

	bytep = pp * FSF1SIZADD;
	m_bFBuf[bytep+FSF1ATRADD] |= FEORMASK;
}

int CFSF1Font::RejionChk(int pp)
{
	int		bytep;

	bytep = pp * FSF1SIZADD;
	if(m_bFBuf[bytep+FSF1ATRADD] & FREJIONMASK){
		return(ON);
	}
	else{
		return(OFF);
	}
}

void CFSF1Font::RejionOn(int pp)
{
	int		bytep;

	bytep = pp * FSF1SIZADD;
	m_bFBuf[bytep+FSF1ATRADD] |= FREJIONMASK;
}

void CFSF1Font::RejionOff(int pp)
{
	int		bytep;

	bytep = pp * FSF1SIZADD;
	m_bFBuf[bytep+FSF1ATRADD] &= ~FREJIONMASK;
}

void CFSF1Font::AtrDXYCrtSet(int atr, DBL x, DBL y)
{
	if(FSF1POINTMAX <= m_nCrtPP){
		return;
	}
	AtrDXYSet(m_bFBuf, m_nCrtPP, atr, x, y);
}

void CFSF1Font::CrtPointInc()
{
	m_nCrtPP++;
	if(m_nCrtPP >= FSF1POINTMAX){
		m_nCrtPP = FSF1POINTMAX-1;
	}
}

void CFSF1Font::AtrDXYGet(BYTE fBuf[], int pp, int *atr, DBL *x, DBL *y)
{
	int		bytep;
	short	*xt, *yt;
	double	dx, dy;

	bytep = pp * FSF1SIZADD;
	xt = (short *)(&(fBuf[bytep+FSF1XCDADD]));
	yt = (short *)(&(fBuf[bytep+FSF1YCDADD]));
	*atr = (fBuf[bytep+FSF1ATRADD] & 0x0e);
	dx = *xt;
	dy = *yt;
	*x = dx;
	*y = dy;
}

void CFSF1Font::AtrDXYSet(BYTE fBuf[], int pp, int atr, DBL x, DBL y)
{
	int		bytep;
	short	*xt, *yt;

	bytep = pp * FSF1SIZADD;
	xt = (short *)(&(fBuf[bytep+FSF1XCDADD]));
	yt = (short *)(&(fBuf[bytep+FSF1YCDADD]));
	*xt = (short)(x);
	*yt = (short)(y);
	fBuf[bytep+FSF1ATRADD] = atr & 0x0e;
}

void CFSF1Font::DXYGet(BYTE fBuf[], int pp, DBL *x, DBL *y)
{
	int		bytep;
	short	*xt, *yt;
	double	dx, dy;

	bytep = pp * FSF1SIZADD;
	xt = (short *)(&(fBuf[bytep+FSF1XCDADD]));
	yt = (short *)(&(fBuf[bytep+FSF1YCDADD]));
	dx = *xt;
	dy = *yt;
	*x = dx;
	*y = dy;
}

void CFSF1Font::DXYSet(BYTE fBuf[], int pp, DBL x, DBL y)
{
	int		bytep;
	short	*xt, *yt;

	bytep = pp * FSF1SIZADD;
	xt = (short *)(&(fBuf[bytep+FSF1XCDADD]));
	yt = (short *)(&(fBuf[bytep+FSF1YCDADD]));
	*xt = (short)(x);
	*yt = (short)(y);
}

void CFSF1Font::AtrGet(BYTE fBuf[], int pp, int *atr)
{
	int		bytep;

	bytep = pp * FSF1SIZADD;
	*atr = (fBuf[bytep+FSF1ATRADD] & 0x0e);
}

void CFSF1Font::AtrSet(BYTE fBuf[], int pp, int atr)
{
	int		bytep;

	bytep = pp * FSF1SIZADD;
	fBuf[bytep+FSF1ATRADD] = atr;
}

int CFSF1Font::PointCountGet()
{
	int 	pcnt;

	pcnt = m_nDataLen / FSF1SIZADD;
	return(pcnt);
}

void CFSF1Font::IdxSeekPointGet(int nFidx, long *idxseekp)
{
	*idxseekp = (long)nFidx * (long)(sizeof(f1idxtype))
			  + (long)(sizeof(f1fhtype));
}

void CFSF1Font::DataLenSeekPointGet(f1idxtype f1idx, int *datlen, long *datseekp)
{
	*datlen = f1idx.dlen;
	*datseekp = f1idx.dseekp + FLIBDATATOP;
}

void CFSF1Font::DataLenSeekPointSet(f1idxtype *f1idx, int datlen, long datseekp)
{
	if(datlen < 0){
		datlen = 0;
	}
	if(datseekp < FLIBDATATOP){
		datseekp = FLIBDATATOP;
	}
	f1idx->dlen = datlen;
	f1idx->dseekp = datseekp - FLIBDATATOP;
}

void CFSF1Font::DataPlotOut(int nComID)
{
	int		pcntmax;
	int		spp, epp, pcnt;

	Dispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}
	pcntmax = PointCountGet();
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
		if(RejionChk(epp - 1) == ON){
			if(poutpaintmodechk() == ON){
				poutrejionstart(nComID);
				if(poutomitmodecheck() == ON){
					poutsetoverlapflag(OFF);
					RejionPlotOutOmit(nComID, &(m_bFBuf[spp*FSF1SIZADD]), pcnt);
					poutsetoverlapflag(ON);
					RejionPlotOutOmit(nComID, &(m_bFBuf[spp*FSF1SIZADD]), pcnt);
					poutsetoverlapflag(OFF);
				}
				else{
					poutsetoverlapflag(OFF);
					RejionPlotOut(nComID, &(m_bFBuf[spp*FSF1SIZADD]), pcnt);
					poutsetoverlapflag(ON);
					RejionPlotOut(nComID, &(m_bFBuf[spp*FSF1SIZADD]), pcnt);
					poutsetoverlapflag(OFF);
				}
				poutrejionend(nComID);
				Dispatch();
				if(senvchckflag(STOPFLAG) == ON){
					return;
				}
			}
		}
		else{
			if(poutpaintmodechk() == OFF){
				poutrejionstart(nComID);
				NoRejionPlotOut(nComID, &(m_bFBuf[spp*FSF1SIZADD]), pcnt);
				poutrejionend(nComID);
			}
		}
		spp = epp;
	}
}

void CFSF1Font::RejionPlotOut(int nComID, BYTE fbuf[], int pcnt)
{
	int		pp, p0, p1, p2, p3, p4, p5;
	int		atr0, atr1, atr2, atr3, atr4, atr5;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;

	if(pcnt <= 1){
		return;
	}
	if(pcnt == 2){
		AtrDXYGet(fbuf, 0, &atr1, &x1, &y1);
		AtrDXYGet(fbuf, 1, &atr2, &x2, &y2);
		FontScaleTrns(x1, y1, &x1, &y1);
		FontScaleTrns(x2, y2, &x2, &y2);
		plotdxymmtostep(&x1, &y1);
		plotdxymmtostep(&x2, &y2);
		poutmove(nComID, x1, y1);
		poutline(nComID, x1, y1, x2, y2);
		return;
	}

	p0 = pcnt-1; p1 = 0; p2 = 1;
	AtrDXYGet(fbuf, p0, &atr0, &x0, &y0);
	AtrDXYGet(fbuf, p1, &atr1, &x1, &y1);
	FontScaleTrns(x0, y0, &x0, &y0);
	FontScaleTrns(x1, y1, &x1, &y1);
	plotdxymmtostep(&x0, &y0);
	plotdxymmtostep(&x1, &y1);
	AtrGet(fbuf, p2, &atr2);
	if(poutcheckoverlapend() == OFF){
		if(pdrwatrarcchk(atr0, atr1, atr2) == ON){
			poutmove(nComID, x0, y0);
		}
		else{
			poutmove(nComID, x1, y1);
		}
	}
	for(pp = 0; pp < pcnt;){
		Dispatch();
		if(senvchckflag(STOPFLAG) == ON){
			return;
		}
		if(poutcheckoverlapend() == ON){
			return;
		}
		p0 = PointLegal(pcnt, pp-1);
		p1 = PointLegal(pcnt, pp);
		p2 = PointLegal(pcnt, pp+1);
		p3 = PointLegal(pcnt, pp+2);
		p4 = PointLegal(pcnt, pp+3);
		p5 = PointLegal(pcnt, pp+4);
		AtrDXYGet(fbuf, p0, &atr0, &x0, &y0);
		AtrDXYGet(fbuf, p1, &atr1, &x1, &y1);
		AtrDXYGet(fbuf, p2, &atr2, &x2, &y2);
		AtrDXYGet(fbuf, p3, &atr3, &x3, &y3);
		AtrDXYGet(fbuf, p4, &atr4, &x4, &y4);
		AtrDXYGet(fbuf, p5, &atr5, &x5, &y5);
		FontScaleTrns(x0, y0, &x0, &y0);
		FontScaleTrns(x1, y1, &x1, &y1);
		FontScaleTrns(x2, y2, &x2, &y2);
		FontScaleTrns(x3, y3, &x3, &y3);
		FontScaleTrns(x4, y4, &x4, &y4);
		FontScaleTrns(x5, y5, &x5, &y5);
		plotdxymmtostep(&x0, &y0);
		plotdxymmtostep(&x1, &y1);
		plotdxymmtostep(&x2, &y2);
		plotdxymmtostep(&x3, &y3);
		plotdxymmtostep(&x4, &y4);
		plotdxymmtostep(&x5, &y5);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				poutline(nComID, x1, y1, x2, y2);
				poutline(nComID, x2, y2, x3, y3);
			}
			else if(p2 == 0){
				poutline(nComID, x1, y1, x2, y2);
			}
			else{
				if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
					poutstartdeepcut(x1, y1, x2, y2);
				}
				poutbeze(nComID, x1, y1, x2, y2, x3, y3, x4, y4);
				if(poutchkdeepcut(x3, y3, x4, y4, x5, y5) == ON){
					poutenddeepcut(x3, y3, x4, y4);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
					poutstartdeepcut(x1, y1, x2, y2);
				}
				poutarc(nComID, x1, y1, x2, y2, x3, y3);
				if(poutchkdeepcut(x2, y2, x3, y3, x4, y4) == ON){
					poutenddeepcut(x2, y2, x3, y3);
				}
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					poutarc(nComID, x0, y0, x1, y1, x2, y2);
				}
				else{
					if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
						poutstartdeepcut(x1, y1, x2, y2);
					}
					poutfreecurve(nComID, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				}
			}
			else{
				if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
					poutstartdeepcut(x1, y1, x2, y2);
				}
				poutline(nComID, x1, y1, x2, y2);
			}
			if(poutchkdeepcut(x1, y1, x2, y2, x3, y3) == ON){
				poutenddeepcut(x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
}

void CFSF1Font::RejionPlotOutOmit(int nComID, BYTE fbuf[], int pcnt)
{
	int		pp, p0, p1, p2, p3, p4, p5;
	int		atr0, atr1, atr2, atr3, atr4, atr5;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;

	if(pcnt <= 1){
		return;
	}
	if(pcnt == 2){
		AtrDXYGet(fbuf, 0, &atr1, &x1, &y1);
		AtrDXYGet(fbuf, 1, &atr2, &x2, &y2);
		FontScaleTrns(x1, y1, &x1, &y1);
		FontScaleTrns(x2, y2, &x2, &y2);
		plotdxymmtostep(&x1, &y1);
		plotdxymmtostep(&x2, &y2);
		poutmove(nComID, x1, y1);
		poutline(nComID, x1, y1, x2, y2);
		poutline(nComID, x2, y2, x1, y1);
		return;
	}

	AtrDXYGet(fbuf, 0, &atr1, &x1, &y1);
	FontScaleTrns(x1, y1, &x1, &y1);
	plotdxymmtostep(&x1, &y1);
	if(poutcheckoverlapend() == OFF){
		poutmove(nComID, x1, y1);
	}
	for(pp = 0; pp < pcnt;){
		Dispatch();
		if(senvchckflag(STOPFLAG) == ON){
			return;
		}
		if(poutcheckoverlapend() == ON){
			return;
		}
		p0 = PointLegal(pcnt, pp-1);
		p1 = PointLegal(pcnt, pp);
		p2 = PointLegal(pcnt, pp+1);
		p3 = PointLegal(pcnt, pp+2);
		p4 = PointLegal(pcnt, pp+3);
		p5 = PointLegal(pcnt, pp+4);
		AtrDXYGet(fbuf, p0, &atr0, &x0, &y0);
		AtrDXYGet(fbuf, p1, &atr1, &x1, &y1);
		AtrDXYGet(fbuf, p2, &atr2, &x2, &y2);
		AtrDXYGet(fbuf, p3, &atr3, &x3, &y3);
		AtrDXYGet(fbuf, p4, &atr4, &x4, &y4);
		AtrDXYGet(fbuf, p5, &atr5, &x5, &y5);
		FontScaleTrns(x0, y0, &x0, &y0);
		FontScaleTrns(x1, y1, &x1, &y1);
		FontScaleTrns(x2, y2, &x2, &y2);
		FontScaleTrns(x3, y3, &x3, &y3);
		FontScaleTrns(x4, y4, &x4, &y4);
		FontScaleTrns(x5, y5, &x5, &y5);
		plotdxymmtostep(&x0, &y0);
		plotdxymmtostep(&x1, &y1);
		plotdxymmtostep(&x2, &y2);
		plotdxymmtostep(&x3, &y3);
		plotdxymmtostep(&x4, &y4);
		plotdxymmtostep(&x5, &y5);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				poutline(nComID, x1, y1, x2, y2);
				poutline(nComID, x2, y2, x3, y3);
			}
			else if(p2 == 0){
				poutline(nComID, x1, y1, x2, y2);
			}
			else{
				if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
					poutstartdeepcut(x1, y1, x2, y2);
				}
				poutline(nComID, x1, y1, x4, y4);
				if(poutchkdeepcut(x1, y1, x4, y4, x5, y5) == ON){
					poutenddeepcut(x1, y1, x4, y4);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 == 0){
				poutline(nComID, x1, y1, x2, y2);
			}
			else{
				if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
					poutstartdeepcut(x1, y1, x2, y2);
				}
				poutline(nComID, x1, y1, x2, y2);
				poutline(nComID, x2, y2, x3, y3);
				if(poutchkdeepcut(x2, y2, x3, y3, x4, y4) == ON){
					poutenddeepcut(x2, y2, x3, y3);
				}
			}
			pp += 2;
		}
		else{
			if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
				poutstartdeepcut(x1, y1, x2, y2);
			}
			poutline(nComID, x1, y1, x2, y2);
			if(poutchkdeepcut(x1, y1, x2, y2, x3, y3) == ON){
				poutenddeepcut(x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
}

void CFSF1Font::NoRejionPlotOut(int nComID, BYTE fbuf[], int pcnt)
{
	int		pp, p0, p1, p2, p3, p4, p5;
	int		atr0, atr1, atr2, atr3, atr4, atr5;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;

	if(pcnt <= 1){
		return;
	}
	if(pcnt == 2){
		AtrDXYGet(fbuf, 0, &atr1, &x1, &y1);
		AtrDXYGet(fbuf, 1, &atr2, &x2, &y2);
		FontScaleTrns(x1, y1, &x1, &y1);
		FontScaleTrns(x2, y2, &x2, &y2);
		plotdxymmtostep(&x1, &y1);
		plotdxymmtostep(&x2, &y2);
		poutmove(nComID, x1, y1);
		poutline(nComID, x1, y1, x2, y2);
		return;
	}

	AtrDXYGet(fbuf, 0, &atr1, &x1, &y1);
	FontScaleTrns(x1, y1, &x1, &y1);
	plotdxymmtostep(&x1, &y1);
	poutmove(nComID, x1, y1);

	for(pp = 0; pp < pcnt-1;){
		Dispatch();
		if(senvchckflag(STOPFLAG) == ON){
			return;
		}
		p0 = PointLegal(pcnt, pp-1);
		p1 = PointLegal(pcnt, pp);
		p2 = PointLegal(pcnt, pp+1);
		p3 = PointLegal(pcnt, pp+2);
		p4 = PointLegal(pcnt, pp+3);
		p5 = PointLegal(pcnt, pp+4);
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
		AtrDXYGet(fbuf, p5, &atr5, &x5, &y5);
		FontScaleTrns(x0, y0, &x0, &y0);
		FontScaleTrns(x1, y1, &x1, &y1);
		FontScaleTrns(x2, y2, &x2, &y2);
		FontScaleTrns(x3, y3, &x3, &y3);
		FontScaleTrns(x4, y4, &x4, &y4);
		FontScaleTrns(x5, y5, &x5, &y5);
		plotdxymmtostep(&x0, &y0);
		plotdxymmtostep(&x1, &y1);
		plotdxymmtostep(&x2, &y2);
		plotdxymmtostep(&x3, &y3);
		plotdxymmtostep(&x4, &y4);
		plotdxymmtostep(&x5, &y5);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				poutline(nComID, x1, y1, x2, y2);
				poutline(nComID, x2, y2, x3, y3);
			}
			else if(p3 == 0){
				poutline(nComID, x1, y1, x2, y2);
			}
			else{
				if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
					poutstartdeepcut(x1, y1, x2, y2);
				}
				poutbeze(nComID, x1, y1, x2, y2, x3, y3, x4, y4);
				if(poutchkdeepcut(x3, y3, x4, y4, x5, y5) == ON){
					poutenddeepcut(x3, y3, x4, y4);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				poutline(nComID, x1, y1, x2, y2);
			}
			else{
				if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
					poutstartdeepcut(x1, y1, x2, y2);
				}
				poutarc(nComID, x1, y1, x2, y2, x3, y3);
				if(poutchkdeepcut(x2, y2, x3, y3, x4, y4) == ON){
					poutenddeepcut(x2, y2, x3, y3);
				}
			}
			pp += 2;
		}
		else{
			if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
				poutstartdeepcut(x1, y1, x2, y2);
			}
			if(pdrwatrcurvechk(atr1) == ON){
				poutfreecurve(nComID, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
			}
			else{
				poutline(nComID, x1, y1, x2, y2);
			}
			if(poutchkdeepcut(x1, y1, x2, y2, x3, y3) == ON){
				poutenddeepcut(x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
}

void CFSF1Font::NoRejionPlotOutOmit(int nComID, BYTE fbuf[], int pcnt)
{
	int		pp, p0, p1, p2, p3, p4, p5;
	int		atr0, atr1, atr2, atr3, atr4, atr5;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;

	if(pcnt <= 1){
		return;
	}
	if(pcnt == 2){
		AtrDXYGet(fbuf, 0, &atr1, &x1, &y1);
		AtrDXYGet(fbuf, 1, &atr2, &x2, &y2);
		FontScaleTrns(x1, y1, &x1, &y1);
		FontScaleTrns(x2, y2, &x2, &y2);
		plotdxymmtostep(&x1, &y1);
		plotdxymmtostep(&x2, &y2);
		poutmove(nComID, x1, y1);
		poutline(nComID, x1, y1, x2, y2);
		return;
	}

	AtrDXYGet(fbuf, 0, &atr1, &x1, &y1);
	FontScaleTrns(x1, y1, &x1, &y1);
	plotdxymmtostep(&x1, &y1);
	poutmove(nComID, x1, y1);

	for(pp = 0; pp < pcnt-1;){
		Dispatch();
		if(senvchckflag(STOPFLAG) == ON){
			return;
		}
		p0 = PointLegal(pcnt, pp-1);
		p1 = PointLegal(pcnt, pp);
		p2 = PointLegal(pcnt, pp+1);
		p3 = PointLegal(pcnt, pp+2);
		p4 = PointLegal(pcnt, pp+3);
		p5 = PointLegal(pcnt, pp+4);
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
		AtrDXYGet(fbuf, p5, &atr5, &x5, &y5);
		FontScaleTrns(x0, y0, &x0, &y0);
		FontScaleTrns(x1, y1, &x1, &y1);
		FontScaleTrns(x2, y2, &x2, &y2);
		FontScaleTrns(x3, y3, &x3, &y3);
		FontScaleTrns(x4, y4, &x4, &y4);
		FontScaleTrns(x5, y5, &x5, &y5);
		plotdxymmtostep(&x0, &y0);
		plotdxymmtostep(&x1, &y1);
		plotdxymmtostep(&x2, &y2);
		plotdxymmtostep(&x3, &y3);
		plotdxymmtostep(&x4, &y4);
		plotdxymmtostep(&x5, &y5);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p4 == 0){
				poutline(nComID, x1, y1, x2, y2);
				poutline(nComID, x2, y2, x3, y3);
			}
			else if(p3 == 0){
				poutline(nComID, x1, y1, x2, y2);
			}
			else{
				if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
					poutstartdeepcut(x1, y1, x2, y2);
				}
				poutline(nComID, x1, y1, x4, y4);
				if(poutchkdeepcut(x1, y1, x4, y4, x5, y5) == ON){
					poutenddeepcut(x1, y1, x4, y4);
				}
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
				poutstartdeepcut(x1, y1, x2, y2);
			}
			poutline(nComID, x1, y1, x2, y2);
			poutline(nComID, x2, y2, x3, y3);
			if(poutchkdeepcut(x2, y2, x3, y3, x4, y4) == ON){
				poutenddeepcut(x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(poutchkdeepcut(x0, y0, x1, y1, x2, y2) == ON){
				poutstartdeepcut(x1, y1, x2, y2);
			}
			poutline(nComID, x1, y1, x2, y2);
			if(poutchkdeepcut(x1, y1, x2, y2, x3, y3) == ON){
				poutenddeepcut(x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
}

void CFSF1Font::FillDrawOut(CDC *pcDC)
{
	int		pcntmax;
	int		spp, epp, pcnt;

	pcntmax = PointCountGet();
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
		if(RejionChk(epp - 1) == ON){
			plgnfillrejionstart();
			FillRejionDrawOut(pcDC, &(m_bFBuf[spp*FSF1SIZADD]), pcnt);
			plgnfillrejionend();
		}
		else{
			PlgnNoRejionDrawOut(pcDC, &(m_bFBuf[spp*FSF1SIZADD]), pcnt);
		}
		spp = epp;
	}
}

void CFSF1Font::FillRejionDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt)
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
		FontScaleTrns(x1, y1, &x1, &y1);
		FontScaleTrns(x2, y2, &x2, &y2);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);

		plgnfillmove(pcDC->m_hDC, x1, y1);
		plgnfillline(pcDC->m_hDC, x1, y1, x2, y2);
		plgnfillline(pcDC->m_hDC, x2, y2, x1, y1);
		return;
	}

	p0 = pcnt-1; p1 = 0; p2 = 1;
	AtrDXYGet(fbuf, p0, &atr0, &x0, &y0);
	AtrDXYGet(fbuf, p1, &atr1, &x1, &y1);
	FontScaleTrns(x0, y0, &x0, &y0);
	FontScaleTrns(x1, y1, &x1, &y1);
	scrndxymmtorltv(&x0, &y0);
	scrndxymmtorltv(&x1, &y1);
	AtrGet(fbuf, p2, &atr2);
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
		FontScaleTrns(x0, y0, &x0, &y0);
		FontScaleTrns(x1, y1, &x1, &y1);
		FontScaleTrns(x2, y2, &x2, &y2);
		FontScaleTrns(x3, y3, &x3, &y3);
		FontScaleTrns(x4, y4, &x4, &y4);
		scrndxymmtorltv(&x0, &y0);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);
		scrndxymmtorltv(&x3, &y3);
		scrndxymmtorltv(&x4, &y4);
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

void CFSF1Font::PlgnDrawOut(CDC *pcDC)
{
	int		pcntmax;
	int		spp, epp, pcnt;
	HPEN	pen, oldpen;
	HBRUSH	brush, oldbrush;
	int		nthick;

	pcntmax = PointCountGet();
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
		if(RejionChk(epp - 1) == ON){
			PlgnRejionDrawOut(pcDC, &(m_bFBuf[spp*FSF1SIZADD]), pcnt);
			brush = senvgetconrbrush(&nthick);
			if(senvcheckconrmode() == ON
			&& brush != NULL && nthick >= 3){
				pen = senvgetconrpen();
				oldpen = (HPEN)SelectObject(pcDC->m_hDC, pen);
				oldbrush = (HBRUSH)SelectObject(pcDC->m_hDC, brush);
				PlgnRejionConrDrawOut(pcDC, &(m_bFBuf[spp*FSF1SIZADD]), pcnt, nthick);
				SelectObject(pcDC->m_hDC, oldbrush);
				SelectObject(pcDC->m_hDC, oldpen);
			}
		}
		else{
			PlgnNoRejionDrawOut(pcDC, &(m_bFBuf[spp*FSF1SIZADD]), pcnt);
			brush = senvgetconrbrush(&nthick);
			if(senvcheckconrmode() == ON
			&& brush != NULL && nthick >= 3){
				pen = senvgetconrpen();
				oldpen = (HPEN)SelectObject(pcDC->m_hDC, pen);
				oldbrush = (HBRUSH)SelectObject(pcDC->m_hDC, brush);
				PlgnNoRejionConrDrawOut(pcDC, &(m_bFBuf[spp*FSF1SIZADD]), pcnt, nthick);
				SelectObject(pcDC->m_hDC, oldbrush);
				SelectObject(pcDC->m_hDC, oldpen);
			}
		}
		spp = epp;
	}
}

void CFSF1Font::PlgnRejionDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt)
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
		FontScaleTrns(x1, y1, &x1, &y1);
		FontScaleTrns(x2, y2, &x2, &y2);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);

		pdrwmove(pcDC->m_hDC, x1, y1);
		pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
		return;
	}

	p0 = pcnt-1; p1 = 0; p2 = 1;
	AtrDXYGet(fbuf, p0, &atr0, &x0, &y0);
	AtrDXYGet(fbuf, p1, &atr1, &x1, &y1);
	FontScaleTrns(x0, y0, &x0, &y0);
	FontScaleTrns(x1, y1, &x1, &y1);
	scrndxymmtorltv(&x0, &y0);
	scrndxymmtorltv(&x1, &y1);
	AtrGet(fbuf, p2, &atr2);
	if(pdrwatrarcchk(atr0, atr1, atr2) == ON){
		pdrwmove(pcDC->m_hDC, x0, y0);
	}
	else{
		pdrwmove(pcDC->m_hDC, x1, y1);
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
		FontScaleTrns(x0, y0, &x0, &y0);
		FontScaleTrns(x1, y1, &x1, &y1);
		FontScaleTrns(x2, y2, &x2, &y2);
		FontScaleTrns(x3, y3, &x3, &y3);
		FontScaleTrns(x4, y4, &x4, &y4);
		scrndxymmtorltv(&x0, &y0);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);
		scrndxymmtorltv(&x3, &y3);
		scrndxymmtorltv(&x4, &y4);
		if(pdrwatrbezechk(atr1, atr2, atr3) == ON){
			if(p3 == 0){
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
				pdrwline(pcDC->m_hDC, x2, y2, x3, y3);
			}
			else if(p2 == 0){
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			else{
				pdrwbeze(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				pdrwarc(pcDC->m_hDC, x1, y1, x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					pdrwarc(pcDC->m_hDC, x0, y0, x1, y1, x2, y2);
				}
				else{
					pdrwfreecurve(pcDC->m_hDC, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				}
			}
			else{
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
}

void CFSF1Font::PlgnNoRejionDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt)
{
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;

	if(pcnt <= 1){
		return;
	}

	AtrDXYGet(fbuf, 0, &atr0, &x0, &y0);
	FontScaleTrns(x0, y0, &x0, &y0);
	scrndxymmtorltv(&x0, &y0);

	pdrwmove(pcDC->m_hDC, x0, y0);

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
		FontScaleTrns(x0, y0, &x0, &y0);
		FontScaleTrns(x1, y1, &x1, &y1);
		FontScaleTrns(x2, y2, &x2, &y2);
		FontScaleTrns(x3, y3, &x3, &y3);
		FontScaleTrns(x4, y4, &x4, &y4);
		scrndxymmtorltv(&x0, &y0);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);
		scrndxymmtorltv(&x3, &y3);
		scrndxymmtorltv(&x4, &y4);
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

void CFSF1Font::PlgnRejionConrDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt, int nthick)
{
	int		pp, p1, p2, p3;
	int		atr1, atr2, atr3;
	DBL		x1, y1, x2, y2, x3, y3;
	DBL		x21, y21, x23, y23, dx, dy;
	DBL		xt, yt;
	DBL		rag21, rag23;
	DBL		dthick;
	POINT		pArry[4];
	int			wPolyCount[2];
	int			nRejionCount;


	if(pcnt <= 2){
		return;
	}
	for(pp = 0; pp < pcnt;){
		p1 = PointLegal(pcnt, pp-1);
		p2 = PointLegal(pcnt, pp);
		p3 = PointLegal(pcnt, pp+1);
		AtrDXYGet(fbuf, p1, &atr1, &x1, &y1);
		AtrDXYGet(fbuf, p2, &atr2, &x2, &y2);
		AtrDXYGet(fbuf, p3, &atr3, &x3, &y3);
		FontScaleTrns(x1, y1, &x1, &y1);
		FontScaleTrns(x2, y2, &x2, &y2);
		FontScaleTrns(x3, y3, &x3, &y3);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);
		scrndxymmtorltv(&x3, &y3);
		pp += 1;
		if((x1 == x2 && y1 == y2)
		|| (x2 == x3 && y2 == y3)){
			continue;
		}
		if((atr2 & CONTATR) != 0){
			continue;
		}
		if(atr2 & BEZEATR){
			continue;
		}
		dthick = nthick / 2;
		GetRoundRag(x1, y1, x2, y2, &rag21);
		SetRoundPoint(x2, y2, rag21, 0.0, dthick, &x21,  &y21);
		GetRoundRag(x2, y2, x3, y3, &rag23);
		SetRoundPoint(x2, y2, rag23, 0.0, dthick, &x23, &y23);
		GetConerPoint(x21, y21, rag21, x23, y23, rag23, &dx, &dy);
		SetRoundPoint(x3, y3, rag23, 0.0, dthick, &xt, &yt);
		if(curveonlinechk(x23, y23, xt, yt, dx, dy) == OFF){
			pArry[0].x = (long)x21; pArry[0].y = (long)y21;
			pArry[1].x = (long)x23; pArry[1].y = (long)y23;
			pArry[2].x = (long)dx; pArry[2].y = (long)dy;
			wPolyCount[0] = 3;
			nRejionCount = 1;
			PolyPolygon(pcDC->m_hDC, pArry, wPolyCount, nRejionCount);
			// éOäpå`ÇìhÇËÇ¬Ç‘Çµ
		}
		dthick = -nthick / 2;
		GetRoundRag(x1, y1, x2, y2, &rag21);
		SetRoundPoint(x2, y2, rag21, 0.0, dthick, &x21,  &y21);
		GetRoundRag(x2, y2, x3, y3, &rag23);
		SetRoundPoint(x2, y2, rag23, 0.0, dthick, &x23, &y23);
		GetConerPoint(x21, y21, rag21, x23, y23, rag23, &dx, &dy);
		SetRoundPoint(x3, y3, rag23, 0.0, dthick, &xt, &yt);
		if(curveonlinechk(x23, y23, xt, yt, dx, dy) == OFF){
			pArry[0].x = (long)x21; pArry[0].y = (long)y21;
			pArry[1].x = (long)x23; pArry[1].y = (long)y23;
			pArry[2].x = (long)dx; pArry[2].y = (long)dy;
			wPolyCount[0] = 3;
			nRejionCount = 1;
			PolyPolygon(pcDC->m_hDC, pArry, wPolyCount, nRejionCount);
			// éOäpå`ÇìhÇËÇ¬Ç‘Çµ
		}
	}
}

void CFSF1Font::PlgnNoRejionConrDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt, int nthick)
{
	int		pp, p1, p2, p3;
	int		atr1, atr2, atr3;
	DBL		x1, y1, x2, y2, x3, y3;
	DBL		x21, y21, x23, y23, dx, dy;
	DBL		xt, yt;
	DBL		rag21, rag23;
	DBL		dthick;
	POINT		pArry[4];
	int			wPolyCount[2];
	int			nRejionCount;

	if(pcnt <= 2){
		return;
	}
	for(pp = 1; pp < pcnt-1;){
		p1 = PointLegal(pcnt, pp-1);
		p2 = PointLegal(pcnt, pp);
		p3 = PointLegal(pcnt, pp+1);
		AtrDXYGet(fbuf, p1, &atr1, &x1, &y1);
		AtrDXYGet(fbuf, p2, &atr2, &x2, &y2);
		AtrDXYGet(fbuf, p3, &atr3, &x3, &y3);
		FontScaleTrns(x1, y1, &x1, &y1);
		FontScaleTrns(x2, y2, &x2, &y2);
		FontScaleTrns(x3, y3, &x3, &y3);
		scrndxymmtorltv(&x1, &y1);
		scrndxymmtorltv(&x2, &y2);
		scrndxymmtorltv(&x3, &y3);
		pp += 1;
		if((x1 == x2 && y1 == y2)
		|| (x2 == x3 && y2 == y3)){
			continue;
		}
		if((atr2 & CONTATR) != 0){
			continue;
		}
		if(atr2 & BEZEATR){
			continue;
		}
		dthick = nthick / 2;
		GetRoundRag(x1, y1, x2, y2, &rag21);
		SetRoundPoint(x2, y2, rag21, 0.0, dthick, &x21,  &y21);
		GetRoundRag(x2, y2, x3, y3, &rag23);
		SetRoundPoint(x2, y2, rag23, 0.0, dthick, &x23, &y23);
		GetConerPoint(x21, y21, rag21, x23, y23, rag23, &dx, &dy);
		SetRoundPoint(x3, y3, rag23, 0.0, dthick, &xt, &yt);
		if(curveonlinechk(x23, y23, xt, yt, dx, dy) == OFF){
			pArry[0].x = (long)x21; pArry[0].y = (long)y21;
			pArry[1].x = (long)x23; pArry[1].y = (long)y23;
			pArry[2].x = (long)dx; pArry[2].y = (long)dy;
			wPolyCount[0] = 3;
			nRejionCount = 1;
			PolyPolygon(pcDC->m_hDC, pArry, wPolyCount, nRejionCount);
			// éOäpå`ÇìhÇËÇ¬Ç‘Çµ
		}
		dthick = -nthick / 2;
		GetRoundRag(x1, y1, x2, y2, &rag21);
		SetRoundPoint(x2, y2, rag21, 0.0, dthick, &x21,  &y21);
		GetRoundRag(x2, y2, x3, y3, &rag23);
		SetRoundPoint(x2, y2, rag23, 0.0, dthick, &x23, &y23);
		GetConerPoint(x21, y21, rag21, x23, y23, rag23, &dx, &dy);
		SetRoundPoint(x3, y3, rag23, 0.0, dthick, &xt, &yt);
		if(curveonlinechk(x23, y23, xt, yt, dx, dy) == OFF){
			pArry[0].x = (long)x21; pArry[0].y = (long)y21;
			pArry[1].x = (long)x23; pArry[1].y = (long)y23;
			pArry[2].x = (long)dx; pArry[2].y = (long)dy;
			wPolyCount[0] = 3;
			nRejionCount = 1;
			PolyPolygon(pcDC->m_hDC, pArry, wPolyCount, nRejionCount);
			// éOäpå`ÇìhÇËÇ¬Ç‘Çµ
		}
	}
}

void CFSF1Font::GetRoundRag(DBL x0, DBL y0, DBL x1, DBL y1, DBL *rag)
{
	if(x0 == x1 && y0 == y1){
		*rag = 0.0;
	}
	else{
		*rag = atan2(y1-y0, x1-x0);
	}
}

void CFSF1Font::SetRoundPoint(DBL x0, DBL y0, DBL rag, DBL x1, DBL y1, DBL *x, DBL *y)
{
	DBL	tcos, tsin;

	tcos = cos(rag);
	tsin = sin(rag);
	*x = x1 * tcos - y1 * tsin + x0;
	*y = y1 * tcos + x1 * tsin + y0;
}

void CFSF1Font::GetConerPoint(DBL x1, DBL y1, DBL rag1, DBL x2, DBL y2, DBL rag2, DBL *x, DBL *y)
{
	DBL	a1, a2, b1, b2;

	a1 = tan(rag1);
	a2 = tan(rag2);
	if(a1 > 100000.0 || a1 < -100000.0){
		b1 = x1;
		if(a2 > 100000.0 || a2 < -100000.0){
			*x = (x1 + x2) / 2.0;
			*y = (y1 + y2) / 2.0;
			return;
		}
		b2 = y2 - a2*x2;
		b1 = x1;
		*x = b1;
		*y = a2 * b1 + b2;
		return;
	}
	if(a2 > 100000.0 || a2 < -100000.0){
		b1 = y1 - a1*x1;
		b2 = x2;
		*x = b2;
		*y = a1 * b2 + b1;
		return;
	}
	b1 = y1 - a1*x1;
	b2 = y2 - a2*x2;
	if(a1 == a2){
		*x = (x1 + x2) / 2.0;
		*y = (y1 + y2) / 2.0;
	}
	else{
		*x = (b1 - b2) / (a2 - a1);
		*y = (b1*a2 - b2*a1)/(a2-a1);
	}
	if((*x) < -50000.0 || 50000.0 < (*x)
	|| (*y) < -50000.0 || 50000.0 < (*y)){
		*x = (x1 + x2) / 2.0;
		*y = (y1 + y2) / 2.0;
	}
}

void CFSF1Font::CreateDIBDC(CDC *pcDC)
{
	m_pcBitmap = new CBitmap();
	m_pcBitmap->CreateBitmap(FDIBCSIZE, FDIBCSIZE, 1, 1, NULL);
	m_pcMemDC = new CDC();
	m_pcMemDC->CreateCompatibleDC(pcDC);
	m_pcoldBitmap = m_pcMemDC->SelectObject(m_pcBitmap);

	m_nXByte = (((FDIBCSIZE + 31) / 32) * 32) / 8;
	m_dwSize = FDIBCSIZE * m_nXByte;
	m_hDIB = GlobalAlloc(GHND, m_dwSize+m_nXByte*2);
	m_hTDIB = GlobalAlloc(GHND, m_dwSize+m_nXByte*2);
}

void CFSF1Font::CreateBoldDIBDC(CDC *pcDC)
{
	m_pcBoldBitmap = new CBitmap();
	m_pcBoldBitmap->CreateBitmap(BOLDSIZE, BOLDSIZE, 1, 1, NULL);
	m_pcBoldMemDC = new CDC();
	m_pcBoldMemDC->CreateCompatibleDC(pcDC);
	m_pcoldBoldBitmap = m_pcBoldMemDC->SelectObject(m_pcBoldBitmap);

	m_nBoldXByte = (((BOLDSIZE + 31) / 32) * 32) / 8;
	m_dwBoldSize = BOLDSIZE * m_nBoldXByte;
	m_hBoldDIB = GlobalAlloc(GHND, m_dwBoldSize+m_nBoldXByte*2);
	m_hBoldTDIB = GlobalAlloc(GHND, m_dwBoldSize+m_nBoldXByte*2);
}

void CFSF1Font::DeleteDIBDC()
{
	GlobalFree(m_hTDIB);
	GlobalFree(m_hDIB);
	m_pcMemDC->SelectObject(m_pcoldBitmap);
	m_pcBitmap->DeleteObject();
	delete(m_pcBitmap);
	m_pcMemDC->DeleteDC();
	delete(m_pcMemDC);
}

void CFSF1Font::DeleteBoldDIBDC()
{
	GlobalFree(m_hBoldTDIB);
	GlobalFree(m_hBoldDIB);
	m_pcBoldMemDC->SelectObject(m_pcoldBoldBitmap);
	m_pcBoldBitmap->DeleteObject();
	delete(m_pcBoldBitmap);
	m_pcBoldMemDC->DeleteDC();
	delete(m_pcBoldMemDC);
}

void CFSF1Font::PlgnDrawDIBC(int nTateYoko, int hFno, DBL sx, DBL sy, DBL timex, DBL timey)
{
	DBL		wd, hi;

	wd = FDIBCSIZE*timex;
	hi = FDIBCSIZE*timey;
	SetTateYoko(nTateYoko);
	m_pcMemDC->BitBlt(0, 0, FDIBCSIZE, FDIBCSIZE, NULL, 0, 0, WHITENESS);

	DBL		sizex, sizey;
	DBL		topx, topy;
	DBL		mvx, mvy;
	DBL		time;
	DBL		rsx, rsy, rex, rey;
	DBL		dmvx, dmvy;

	scrngetrltvsize(&sizex, &sizey);
	scrngetrltvtop(&topx, &topy);
	scrngetrltvmove(&mvx, &mvy);
	scrnrltvareaget(&rsx, &rsy, &rex, &rey);
	scrngetdisprltvmove(&dmvx, &dmvy);

	scrnsetrltvsize(FDIBCSIZE, FDIBCSIZE);
	scrnsetrltvtop(0, 0);
	scrnsetrltvmove(0, 0);
	time = scrnsetxytime(1.0, 1.0);
	scrnrltvareaset(0, 0, FDIBCSIZE, FDIBCSIZE);
	scrnsetdisprltvmove(0, 0);

	TrnsPlgnDraw(m_pcMemDC, sx, sy, wd, hi, hFno);

	scrnsetrltvsize(sizex, sizey);
	scrnsetrltvtop(topx, topy);
	scrnsetrltvmove(mvx, mvy);
	scrnsetxytime(time, time);
	scrnrltvareaset(rsx, rsy, rex, rey);
	scrnsetdisprltvmove(dmvx, dmvy);
}

void CFSF1Font::GetPrePostTable(int pre[], int post[])
{
	int		flag;
	int		x, y;

	m_lpBits = (LPBYTE)GlobalLock(m_hDIB);
	if(m_lpBits == NULL){
		return;
	}
	m_pcBitmap->GetBitmapBits(m_dwSize, m_lpBits);
	flag = CheckTateYoko();
	if(flag == TATEGAKI || flag == YOKOOKI){
		for(x = 0; x < FDIBCSIZE; x++){
			pre[x] = PreFirstYOnBit(x);
			post[x] = PostFirstYOnBit(x);
		}
	}
	else{
		for(y = 0; y < FDIBCSIZE; y++){
			pre[y] = PreFirstXOnBit(y);
			post[y] = PostFirstXOnBit(y);
		}
	}
	GlobalUnlock(m_hDIB);
}

int CFSF1Font::PreFirstYOnBit(int x)
{
	int			byte;
	LPBYTE		adrs;
	int		y, mask;

	mask = 0x0080 >> (x % 8);
	byte = x / 8;
	for(y = 0; y < FDIBCSIZE; y++){
		adrs = &(m_lpBits[m_nXByte * y + byte]);
		if(((*adrs) & mask) == 0){
			break;
		}
	}
	return(y);
}

int CFSF1Font::PostFirstYOnBit(int x)
{
	int		byte;
	LPBYTE	adrs;
	int		y, mask;

	mask = 0x0080 >> (x % 8);
	byte = x / 8;
	for(y = FDIBCSIZE-1; y >= 0; y--){
		adrs = &(m_lpBits[m_nXByte * y + byte]);
		if(((*adrs) & mask) == 0){
			break;
		}
	}
	if(y < 0){
		y = 0;
	}
	return(FDIBCSIZE-y);
}

int CFSF1Font::PreFirstXOnBit(int y)
{
	int		byte, bytemax;
	LPBYTE	adrs;
	int		i, mask;
	int		ret;

	bytemax = FDIBCSIZE / 8;
	byte = m_nXByte * y;
	adrs = &(m_lpBits[byte]);
	for(i = 0; i < bytemax; i++){
		if(adrs[i] != 0x00ff){
			break;
		}
	}
	if(i == bytemax){
		return(FDIBCSIZE);
	}
	byte = i;
	mask = 0x0080;
	for(i = 0; i < 8; i++){
		if((adrs[byte] & mask) == 0){
			break;
		}
		mask >>= 1;
	}
	ret = byte * 8 + i;
	return(ret);
}

int CFSF1Font::PostFirstXOnBit(int y)
{
	int		byte, ibyte, bytemax;
	LPBYTE	adrs;
	int		i, mask;
	int		ret;

	bytemax = FDIBCSIZE / 8;
	byte = m_nXByte * y;
	adrs = &(m_lpBits[byte]);
	for(i = bytemax-1, byte = 0; i >= 0; i--, byte++){
		if(adrs[i] != 0x00ff){
			ibyte = i;
			break;
		}
	}
	if(i <= -1){
		return(FDIBCSIZE);
	}
	mask = 0x0001;
	for(i = 0; i < 8; i++){
		if((adrs[ibyte] & mask) == 0){
			break;
		}
		mask <<= 1;
	}
	ret = byte * 8 + i;
	return(ret);
}

void CFSF1Font::TrnsHanKaku()
{
	int		pcnt, pp;
	DBL		x, y;
	DBL		xx, yy;

	pcnt = PointCountGet();
	for(pp = 0; pp < pcnt; pp++){
		DXYGet(m_bFBuf, pp, &x, &y);
		xx = FSF1MESH - y;
		yy = x;
		DXYSet(m_bFBuf, pp, xx, yy);
	}
}

void CFSF1Font::BoldTrns(int hFno, DBL par)
{
	DBL		bsizex, bsizey;
	DBL		btopx, btopy;
	DBL		bmvx, bmvy;
	DBL		btime;
	DBL		brsx, brsy, brex, brey;
	DBL		bdmvx, bdmvy;
	DBL		dsx, dsy;
	DBL		dw, dh;
	DBL		thick;
	int		nthick;
	int		nlimit;
	DBL		xtime, ytime;
	int		nXByte, nXSize, nYSize;

	nlimit = senvgetplgnlimit();
	thick = (DBL)(BOLDSIZE-16) * (par - 100.0) / 100.0;
	nthick = (int)(thick * 2.0 + 1);
	if(nthick < nlimit){
		nthick = nlimit;
	}
	scrnsetpreviewpen(m_pcBoldMemDC->m_hDC, 255, 255, 255, nthick);

	nXByte = BOLDSIZE / 8;
	nXSize = BOLDSIZE;
	nYSize = BOLDSIZE;
	dsx = 8;
	dsy = 8;
	dw = BOLDSIZE - 16;
	dh = BOLDSIZE - 16;
	m_pcBoldMemDC->BitBlt(0, 0, BOLDSIZE, BOLDSIZE, NULL, 0, 0, BLACKNESS);

	scrngetrltvsize(&bsizex, &bsizey);
	scrngetrltvtop(&btopx, &btopy);
	scrngetrltvmove(&bmvx, &bmvy);
	scrnrltvareaget(&brsx, &brsy, &brex, &brey);
	scrngetdisprltvmove(&bdmvx, &bdmvy);

	scrnsetrltvsize(BOLDSIZE, BOLDSIZE);
	scrnsetrltvtop(0, 0);
	scrnsetrltvmove(0, 0);
	btime = scrnsetxytime(1.0, 1.0);
	scrnrltvareaset(0, 0, BOLDSIZE, BOLDSIZE);
	scrnsetdisprltvmove(0, 0);

	m_fRevercex = OFF;
	m_fRevercey = OFF;
	DataLoad(hFno);
	TrnsElementSet(dsx, dsy, dw, dh);
	plgnfillstart();
	FillDrawOut(m_pcBoldMemDC);
	plgnfilldraw(m_pcBoldMemDC->m_hDC);

	scrnsetrltvsize(bsizex, bsizey);
	scrnsetrltvtop(btopx, btopy);
	scrnsetrltvmove(bmvx, bmvy);
	scrnsetxytime(btime, btime);
	scrnrltvareaset(brsx, brsy, brex, brey);
	scrnsetdisprltvmove(bdmvx, bdmvy);

	scrnpenend(m_pcBoldMemDC->m_hDC);

	m_lpBoldBits = (LPBYTE)GlobalLock(m_hBoldDIB);
	if(m_lpBoldBits == NULL){
		return;
	}
	m_lpBoldTBits = (LPBYTE)GlobalLock(m_hBoldTDIB);
	if(m_lpBoldTBits == NULL){
		return;
	}
	m_pcBoldBitmap->GetBitmapBits(m_dwBoldSize, m_lpBoldBits);

	m_pcBoldLib->m_pcDataList = new CDataList(DATALISTID);
	m_pcBoldLib->BitMapOutLineSet(m_lpBoldBits, m_lpBoldTBits, nXByte, nXSize, nYSize);
	m_pcBoldLib->BitMapTrace(m_lpBoldBits, m_lpBoldTBits, nXByte, nXSize, nYSize);
	xtime = (DBL)(FSF1MESH) / dw;
	ytime = (DBL)(FSF1MESH) / dh;
	m_pcBoldLib->TrnsDataList(dsx, dsy, xtime, ytime);
	m_dStartX = 0;
	m_dStartY = 0;
	m_fRevercex = OFF;
	m_fRevercey = OFF;
	m_dWidth = FSF1MESH;
	m_dHight = FSF1MESH;

	PointSetStart();
	m_pcBoldLib->m_pcDataList->SetAtnVectFSF1fBuf(this);
	PointSetEnd();
	delete(m_pcBoldLib->m_pcDataList);
	DataSave(hFno);

	GlobalUnlock(m_hBoldDIB);
	GlobalUnlock(m_hBoldTDIB);
}

void CFSF1Font::BmpFileOut(LPSTR fname, int xsize, int ysize)
{
	FILE				*fp;
	BITMAPFILEHEADER	bif;
	BITMAPINFOHEADER	bih;
	RGBQUAD				clr[2];
	int					headsize;
	BYTE				*sp;
	int					y;
	int					xbyte;

	xbyte = ((xsize + 7L) / 8L + 3L) / 4L * 4L;
	memset((LPSTR)(&bif), 0, sizeof(BITMAPFILEHEADER));
	memset((LPSTR)(&bih), 0, sizeof(BITMAPINFOHEADER));
	headsize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*2;
	bif.bfType = 0x4D42;
	bif.bfSize = xbyte * ysize + headsize;
	bif.bfOffBits = headsize;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = xsize;
	bih.biHeight = ysize;
	bih.biPlanes = 1;
	bih.biBitCount = 1;
	bih.biCompression = 0;

	clr[0].rgbBlue = 0;
	clr[0].rgbGreen = 0;
	clr[0].rgbRed = 0;
	clr[0].rgbReserved = 0;
	clr[1].rgbBlue = 255;
	clr[1].rgbGreen = 255;
	clr[1].rgbRed = 255;
	clr[1].rgbReserved = 0;

	fp = fopen(fname, "wb");
	fwrite(&bif, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(clr, sizeof(RGBQUAD), 2, fp);
	for(y = 0; y < ysize; y++){
		sp = m_lpBits + (long)y * (long)xbyte;
		fwrite(sp, 1, xbyte, fp);
	}
	fclose(fp);
}

void CFSF1Font::SetBold(int flag)
{
	m_nBold = flag;
}

void CFSF1Font::SetSlope(int flag)
{
	m_nSlope = flag;
}

int CFSF1Font::CheckTateYoko()
{
	return(m_nTateYoko);
}

void CFSF1Font::SetTateYoko(int flag)
{
	m_nTateYoko = flag;
}

void CFSF1Font::TrnsElementInit(DBL sx, DBL sy, DBL wd, DBL hi, DBL rag)
{
	m_dSX = sx;
	m_dSY = sy;
	m_dXTime = wd / ESF1MESH;
	m_dYTime = hi / ESF1MESH;
	m_dSin = sin(rag);
	m_dCos = cos(rag);
}

void CFSF1Font::PlgnDraw(CDC *pcDC)
{
	int		pcntmax;
	int		spp, epp, pcnt;

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
		if(RejionChk(epp-1) == ON){
			RejionPlgnDrawOut(pcDC, &(m_bFBuf[spp*FSF1SIZADD]), pcnt);
		}
		else{
			NoRejionPlgnDrawOut(pcDC, &(m_bFBuf[spp*FSF1SIZADD]), pcnt);
		}
		spp = epp;
	}
}

void CFSF1Font::FillDraw(CDC *pcDC)
{
	int		pcntmax;
	int		spp, epp, pcnt;

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
		if(RejionChk(epp-1) == ON){
			plgnfillrejionstart();
			RejionFillDrawOut(pcDC, &(m_bFBuf[spp*FSF1SIZADD]), pcnt);
			plgnfillrejionend();
		}
		spp = epp;
	}
	plgnfilldraw(pcDC->m_hDC);
}

void CFSF1Font::RejionPlgnDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt)
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
		pdrwmove(pcDC->m_hDC, x1, y1);
		pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
		pdrwline(pcDC->m_hDC, x2, y2, x1, y1);
		return;
	}

	p0 = pcnt-1; p1 = 0; p2 = 1;
	AtrDXYGet(fbuf, p0, &atr0, &x0, &y0);
	AtrDXYGet(fbuf, p1, &atr1, &x1, &y1);
	TrnsXY(&x0, &y0);
	TrnsXY(&x1, &y1);
	AtrDXYGet(fbuf, p2, &atr2, &x2, &y2);
	if(pdrwatrarcchk(atr0, atr1, atr2) == ON){
		pdrwmove(pcDC->m_hDC, x0, y0);
	}
	else{
		pdrwmove(pcDC->m_hDC, x1, y1);
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
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
				pdrwline(pcDC->m_hDC, x2, y2, x3, y3);
			}
			else if(p2 == 0){
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			else{
				pdrwbeze(pcDC->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
			}
			pp += 3;
		}
		else if(pdrwatrarcchk(atr1, atr2, atr3) == ON){
			if(p2 != 0){
				pdrwarc(pcDC->m_hDC, x1, y1, x2, y2, x3, y3);
			}
			pp += 2;
		}
		else{
			if(pdrwatrcurvechk(atr1) == ON){
				if(p1 == 0 && pdrwatrarcchk(atr0, atr1, atr2) == ON){
					pdrwarc(pcDC->m_hDC, x0, y0, x1, y1, x2, y2);
				}
				else{
					pdrwfreecurve(pcDC->m_hDC, atr0, atr1, atr2, x0, y0, x1, y1, x2, y2, x3, y3);
				}
			}
			else{
				pdrwline(pcDC->m_hDC, x1, y1, x2, y2);
			}
			pp += 1;
		}
	}
}

void CFSF1Font::NoRejionPlgnDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt)
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

void CFSF1Font::RejionFillDrawOut(CDC *pcDC, BYTE fbuf[], int pcnt)
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

void CFSF1Font::TrnsXY(DBL *x, DBL *y)
{
	DBL		xx, yy;
	DBL		tx, ty;

	if(m_nTateYoko == YOKOGAKI){
		xx = (*x) - FSF1MESH / 2;
		yy = (*y) - FSF1MESH / 2;
	}
	else if(m_nTateYoko == TATEGAKI){
		xx = (*x) - FSF1MESH / 2;
		yy = (*y) - FSF1MESH / 2;
	}
	else if(m_nTateYoko == YOKOOKI){
		xx = (FSF1MESH - (*y)) - FSF1MESH / 2;
		yy = (*x) - FSF1MESH / 2;
	}
	else if(m_nTateYoko == TATEOKI){
		xx = (FSF1MESH - (*y)) - FSF1MESH / 2;
		yy = (*x) - FSF1MESH / 2;
	}
	tx = (xx * m_dCos - yy * m_dSin + FSF1MESH / 2);
	ty = (xx * m_dSin + yy * m_dCos + FSF1MESH / 2);
	if(m_nTateYoko == YOKOGAKI){
		if(m_nSlope == OFF){
			*x = tx * m_dXTime + m_dSX;
			*y = ty * m_dYTime + m_dSY;
		}
		else{
			*x = (tx - ty * 0.5) * m_dXTime + m_dSX + FSF1MESH*m_dXTime*0.25;
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
			*y = (ty - tx * 0.5) * m_dYTime + m_dSY + FSF1MESH*m_dYTime*0.25;
			//*y = (ty + tx * 0.5) * m_dYTime + m_dSY;
		}
	}
	else if(m_nTateYoko == YOKOOKI){
		if(m_nSlope == OFF){
			*x = tx * m_dXTime + m_dSX;
			*y = ty * m_dYTime + m_dSY;
		}
		else{
			*x = (tx + ty * 0.5) * m_dXTime + m_dSX + FSF1MESH*m_dXTime*0.25;
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
			*y = (ty + tx * 0.5) * m_dYTime + m_dSY - FSF1MESH*m_dYTime*0.25;
		}
	}
}

