
//
// Easy Scanner System
//
// object arkfont.cpp  アーク書体処理関数
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include <stdio.h>
#include "arkfont.h"

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
}

IMPLEMENT_SERIAL(CARKFont, CObject, 0)
CARKFont::CARKFont()
{
}

CARKFont::CARKFont(int nDummy)
{
	m_fileARK = NULL;
	m_pFSF1Font = NULL;
	m_nDataLen = 0;
	m_nTateYoko = YOKOGAKI;
	m_sBaseFname[0] = '\0';
	m_dTime = FSF1MESH / ARKMESH;
	m_nSlope = OFF;
	m_nBold = OFF;
	m_dMoveX = 0;
	m_dMoveY = 0;
}

CARKFont::~CARKFont()
{
}

void CARKFont::Serialize(CArchive& ar)
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

void CARKFont::FSF1ClassSet(CFSF1Font* pFSF1Font)
{
	m_pFSF1Font = pFSF1Font;
}

void CARKFont::FontNameSet(LPSTR pFont, int type)
{
	char	fontpath[FNAMEMAX];

	m_nType = type;
	mojienvgetfontpath(fontpath);
	sprintf(m_sBaseFname, "%s\\%s.fnt", fontpath, pFont);
}

void CARKFont::SetTateYoko(int flag)
{
	m_nTateYoko = flag;
}

int CARKFont::SF1TrnsDataLoad(UINT sjis)
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
	TrnsARKToFSF1();
	return(ret);
}

int CARKFont::SF1TrnsDataLoadNON(UINT sjis)
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
	TrnsARKToFSF1NON();
	return(ret);
}

void CARKFont::SF1TrnsDataSave(UINT sjis)
{
	TrnsCopyFSF1FBuf();
	DataSave(sjis);
}

int CARKFont::DataLoad(UINT sjis)
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
	if(ArcDataLoad(ku, ten) == FAIL){
		m_nDataLen = 0;
		return(FAIL);
	}
	return(0);
}

int CARKFont::ArcDataLoad(int ku, int ten)
{
	long 		idxseekp, datseekp;
	arkidxtype	arkidx;
	arkhtype	arkhead;
	arkpointype	*dbuf;
	int			idx;
	int			atr;
	DBL			x, y;

	if(FontOpen(ku, ten) == FAIL){
		return(FAIL);
	}
	fread(m_bFBuf, 1, 256, m_fileARK);
	if(m_bFBuf[0x42] == 0x32){
		m_nByteAdd = 2;
	}
	else{
		m_nByteAdd = 0;
	}
	ArkIdxSeekPointGet(ku, ten, &idxseekp);
	fseek(m_fileARK, idxseekp, 0);
	fread(&(arkidx.ku), sizeof(UCHAR), 1, m_fileARK);
	fread(&(arkidx.ten), sizeof(UCHAR), 1, m_fileARK);
	fread(&(arkidx.seekp), sizeof(long), 1, m_fileARK);
	fread(&(arkidx.useekp), sizeof(long), 1, m_fileARK);
	ArkDataLenSeekPointGet(arkidx, &datseekp);
	if(datseekp == m_nByteAdd){
		FontClose();
		return(FAIL);
	}
	if(fseek(m_fileARK, datseekp, 0) != 0){
		FontClose();
		return(FAIL);
	}
	if(fread(&arkhead, sizeof(arkhtype), 1, m_fileARK) != 1){
		FontClose();
		return(FAIL);
	}
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		m_dMoveX = arkhead.x3;
		m_dMoveY = -arkhead.y3;
	}
	else{
		m_dMoveX = arkhead.x1;
		m_dMoveY = -arkhead.y1;
	}
	dbuf = (arkpointype*)m_bFBuf;
	idx = 0;
	while(1){
		fread(&(dbuf[idx]), sizeof(arkpointype), 1, m_fileARK); 
		if(dbuf[idx].x == 0xffff 
		&& dbuf[idx].y == 0xffff){
			idx++;
			break;
		}
		AtrDXYGet(dbuf, idx, &atr, &x, &y);
		idx++;
	}
	FontClose();
	m_nDataLen = idx;
	return(0);
}

int CARKFont::DataSave(UINT sjis)
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

int CARKFont::ArcDataSave(int ku, int ten)
{
	long 		idxseekp, datseekp;
	arkidxtype	arkidx;
	arkhtype	arkhead;
	arkpointype	*dbuf;
	int		idx, len;

	if(FontSaveOpen(ku, ten) == FAIL){
		return(FAIL);
	}

	ArkIdxSeekPointGet(ku, ten, &idxseekp);
	fseek(m_fileARK, idxseekp, 0);
	fread(&(arkidx.ku), sizeof(UCHAR), 1, m_fileARK);
	fread(&(arkidx.ten), sizeof(UCHAR), 1, m_fileARK);
	fread(&(arkidx.seekp), sizeof(long), 1, m_fileARK);
	fread(&(arkidx.useekp), sizeof(long), 1, m_fileARK);
	ArkDataLenSeekPointGet(arkidx, &datseekp);

	fseek(m_fileARK, 0L, 2);
	datseekp = ftell(m_fileARK);
	arkhead.x1 = 0;
	arkhead.y1 = 0;
	arkhead.x2 = (int)ARKMESH; 
	arkhead.y2 = (int)ARKMESH; 
	arkhead.x3 = 0;
	arkhead.y3 = 0;
	arkhead.x4 = (int)ARKMESH; 
	arkhead.y4 = (int)ARKMESH; 
	if(fwrite(&arkhead, sizeof(arkhtype), 1, m_fileARK) != 1){
		FontClose();
		return(FAIL);
	}
	dbuf = (arkpointype*)m_bFBuf;
	len = m_nDataLen / sizeof(arkpointype);
	for(idx = 0; idx < len; idx++){
		fread(&(dbuf[idx]), sizeof(arkpointype), 1, m_fileARK); 
	}

	arkidx.ku = ku;
	arkidx.ten = ten;
	ArkDataLenSeekPointSet(&arkidx, datseekp);
	fseek(m_fileARK, idxseekp, 0);
	fwrite(&(arkidx.ku), sizeof(UCHAR), 1, m_fileARK);
	fwrite(&(arkidx.ten), sizeof(UCHAR), 1, m_fileARK);
	fwrite(&(arkidx.seekp), sizeof(long), 1, m_fileARK);
	fwrite(&(arkidx.useekp), sizeof(long), 1, m_fileARK);

	FontClose();

	return(0);
}

int CARKFont::FontOpen(int ku, int ten)
{
	if((m_fileARK = fopen(m_sBaseFname, "rb")) == NULL){
		return(FAIL);
	}
	return(0);
}

int CARKFont::FontSaveOpen(int ku, int ten)
{
	if((m_fileARK = fopen(m_sBaseFname, "r+b")) == NULL){
		if(FontIdxInit() == FAIL){
			return(FAIL);
		}
	}
	return(0);
}

int CARKFont::FontIdxInit()
{
	char	buf[RECMAX];
	int		len, blk, rest, i;

	if((m_fileARK = fopen(m_sBaseFname, "wb")) == NULL){
		return(FAIL);
	}
	memset(buf, '\0', ARKHEADBYTE);
	strcpy(buf, m_sBaseFname);
	fwrite(buf, ARKHEADBYTE, 1, m_fileARK);
	len = (MAXKU * 94 + 1) * FT1IDXSIZE;

	memset(buf, '\0', RECMAX);
	blk = len / RECMAX;
	rest = len % RECMAX;
	for(i = 0; i < blk; i++){
		fwrite(buf, RECMAX, 1, m_fileARK);
	}
	if(rest != 0){
		fwrite(buf, rest, 1, m_fileARK);
	}
	fclose(m_fileARK);

	if((m_fileARK = fopen(m_sBaseFname, "r+b")) == NULL){
		return(FAIL);
	}
	return(0);
}

void CARKFont::FaceNameSet(LPSTR name)
{
	strcpy(m_sFaceName, name);
}

void CARKFont::FontClose()
{
	fclose(m_fileARK);
}

void CARKFont::ArkIdxSeekPointGet(int ku, int ten, long *seekp)
{
	int		idx;
	int		size;

	idx = (ku - 1) * 94 + (ten - 1);
	size = sizeof(char)+sizeof(char)+sizeof(long)+sizeof(long);
	*seekp = ARKHEADBYTE + size * idx + m_nByteAdd;
}

void CARKFont::ArkDataLenSeekPointGet(arkidxtype arkidx, long *dseekp)
{
	*dseekp = 0;
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		*dseekp = arkidx.useekp + m_nByteAdd;
	}
	if((*dseekp) == 0){
		*dseekp = arkidx.seekp + m_nByteAdd;
	}
}

void CARKFont::ArkDataLenSeekPointSet(arkidxtype *arkidx, long dseekp)
{
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
		arkidx->useekp = dseekp - m_nByteAdd;
	}
	else{
		arkidx->seekp = dseekp - m_nByteAdd;
	}
}

void CARKFont::TrnsARKToFSF1()
{
	int		pcnt, pp;
	int		dp;
	int		atr;
	DBL		x, y;
	DBL		rx, ry;
	DBL		sx, sy;
	DBL		ax1, ay1;
	DBL		ax2, ay2;
	arkpointype	*dbuf;

	m_pFSF1Font->PointSetStart();
	dbuf = (arkpointype*)m_bFBuf;
	pcnt = m_nDataLen;
	dp = 0;
	pp = 0;
	while(1){
		if(pcnt <= pp){
			break;
		}
		AtrDXYGet(dbuf, pp, &atr, &x, &y);
		TrnsXY(&x, &y);
		if(atr == HENDOF || atr == HMOVETO){
			if(pp != 0){
				m_pFSF1Font->AtrDXYCrtSet(0, sx, sy);
				m_pFSF1Font->RejionOn(dp);
				m_pFSF1Font->EorOn(dp);
				m_pFSF1Font->CrtPointInc();
				dp++;
			}
		}
		pp++;
		switch(atr){
		case HENDOF:
			break;
		case HMOVETO:
			rx = x; ry = y;
			sx = x; sy = y;
			break;
		case HLINETO:
			m_pFSF1Font->AtrDXYCrtSet(0, sx, sy);
			m_pFSF1Font->CrtPointInc();
			dp++;
			sx = x; sy = y;
			break;
		case HARCTO:
			ax1 = sx; ay1 = sy;
			ax2 = x; ay2 = y;
			AtrDXYGet(dbuf, pp, &atr, &x, &y);
			pp++;
			TrnsXY(&x, &y);
			if(atr != HARCEND){
				break;
			}
			dp = ArcTrnsBezeSet(dp, ax1, ay1, ax2, ay2, x, y);
			sx = x; sy = y;
			break;
		}
	}
	m_pFSF1Font->PointSetEnd();
}

void CARKFont::TrnsARKToFSF1NON()
{
	int		pcnt, pp;
	int		dp;
	int		atr;
	DBL		x, y;
	DBL		rx, ry;
	DBL		sx, sy;
	DBL		ax1, ay1;
	DBL		ax2, ay2;
	arkpointype	*dbuf;

	m_pFSF1Font->PointSetStart();
	dbuf = (arkpointype*)m_bFBuf;
	pcnt = m_nDataLen;
	for(pp = 0, dp = 0; pp < pcnt; pp++){
		AtrDXYGet(dbuf, pp, &atr, &x, &y);
		if(atr == HENDOF || atr == HMOVETO){
			if(pp != 0){
				m_pFSF1Font->AtrDXYCrtSet(0, sx, sy);
				m_pFSF1Font->RejionOn(dp);
				m_pFSF1Font->EorOn(dp);
				m_pFSF1Font->CrtPointInc();
				dp++;
			}
		}
		switch(atr){
		case HENDOF:
			break;
		case HMOVETO:
			rx = x; ry = y;
			sx = x; sy = y;
			break;
		case HLINETO:
			m_pFSF1Font->AtrDXYCrtSet(0, sx, sy);
			m_pFSF1Font->CrtPointInc();
			dp++;
			sx = x; sy = y;
			break;
		case HARCTO:
			ax1 = sx; ay1 = sy;
			ax2 = x; ay2 = y;
			AtrDXYGet(dbuf, pp, &atr, &x, &y);
			TrnsXY(&x, &y);
			if(atr != HARCEND){
				pp++;
				break;
			}
			dp = ArcSet(dp, ax1, ay1, ax2, ay2, sx, sy);
			sx = x; sy = y;
			break;
		}
	}
	m_pFSF1Font->PointSetEnd();
}

void CARKFont::AtrDXYGet(arkpointype dbuf[], int pp, int *atr, DBL *x, DBL *y)
{
	*atr = ((dbuf[pp].x & 0x8000) >> 14) | ((dbuf[pp].y & 0x8000) >> 15);
	*x = (DBL)(dbuf[pp].x & 0x7fff);
	*y = (DBL)(dbuf[pp].y & 0x7fff);
}

int CARKFont::ArcTrnsBezeSet(int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

int CARKFont::ArcSet(int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	m_pFSF1Font->AtrDXYCrtSet(CURVATR, x1, y1);
	m_pFSF1Font->CrtPointInc();
	dp++;
	m_pFSF1Font->AtrDXYCrtSet(CURVATR|CONTATR, x2, y2);
	m_pFSF1Font->CrtPointInc();
	dp++;
	return(dp);
}

void CARKFont::TrnsCopyFSF1FBuf()
{
	arkpointype	*dbuf;
	LPBYTE	FSF1FBuf;
	int		pcntmax;
	int		spp, epp, pcnt;
	int		dp;

	dbuf = (arkpointype*)m_bFBuf;
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
	m_nDataLen = dp * sizeof(arkpointype); 

}

int CARKFont::TrnsCopyRejionFSF1FBuf(BYTE fbuf[], int pcnt, int dp)
{
	arkpointype	*dbuf;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;

	if(pcnt <= 1){
		return(dp);
	}
	dbuf = (arkpointype*)m_bFBuf;
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

int CARKFont::EofSet(arkpointype dbuf[], int dp)
{
	DBL		x, y;

	x = 0.0;
	y = 0.0;
	AtrDXYSet(dbuf, dp, HENDOF, x, y);
	dp++;
	return(dp);
}

int CARKFont::MoveToSet(arkpointype dbuf[], int dp, DBL x1, DBL y1)
{
	AtrDXYSet(dbuf, dp, HMOVETO, x1, y1);
	dp++;
	return(dp);
}

int CARKFont::EorSet(arkpointype dbuf[], int dp)
{
	return(dp);
}

int CARKFont::LineToSet(arkpointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2)
{
	AtrDXYSet(dbuf, dp, HLINETO, x2, y2);
	dp++;
	return(dp);
}

int CARKFont::BezeToArcToSet(arkpointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
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

	AtrDXYSet(dbuf, dp, HARCTO, xx, yy);
	dp++;
	AtrDXYSet(dbuf, dp, HARCEND, x4, y4);
	dp++;
	return(dp);
}

int CARKFont::ArcToSet(arkpointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	AtrDXYSet(dbuf, dp, HARCTO, x2, y2);
	dp++;
	AtrDXYSet(dbuf, dp, HARCEND, x3, y3);
	dp++;
	return(dp);
}

void CARKFont::AtrDXYSet(arkpointype dbuf[], int dp, int mode, DBL x, DBL y)
{
	USHORT	sx, sy;

	sx = (int)(x / m_dTime);
	sy = (int)(ARKMESH - y / m_dTime);
	sx = (sx & 0x7fff) | ((mode << 14) & 0x8000);
	sy = (sy & 0x7fff) | ((mode << 15) & 0x8000);
	dbuf[dp].x = sx;
	dbuf[dp].y = sy;
}

// 文字表示関数
void CARKFont::DrawOut(CDC *pcDC, DBL sx, DBL sy, DBL wd, DBL hi, DBL rag)
{
	arkpointype	*dbuf;
	int		pp, pcnt, atr;
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
	dbuf = (arkpointype*)m_bFBuf;
	pcnt = m_nDataLen;
	plgnfillerror(OFF);
	plgnfillstart();
	for(pp = 0; pp < pcnt; pp++){
		AtrDXYGet(dbuf, pp, &atr, &x, &y);
		TrnsXY(&x, &y);
		if(atr == HENDOF || atr == HMOVETO){
			if(pp != 0){
				plgnfillrejionend();
			}
			plgnfillrejionstart();	
		}
		switch(atr){
		case HENDOF:
			break;
		case HMOVETO:
			DrawOutMove(pcDC, x, y);
			sx = x; sy = y;
			break;
		case HLINETO:
			DrawOutLine(pcDC, sx, sy, x, y);
			sx = x; sy = y;
			break;
		case HARCTO:
			x1 = sx; y1 = sy;
			x2 = x; y2 = y;
			AtrDXYGet(dbuf, pp, &atr, &x, &y);
			TrnsXY(&x, &y);
			if(atr != HARCEND){
				pp++;
				break;
			}
			DrawOutArc(pcDC, x1, y1, x2, y2, sx, sy);
			sx = x; sy = y;
			break;
		}
	}
	plgnfilldraw(pcDC->m_hDC);
	if(pcoldPen != NULL){
		pcDC->SelectObject(pcoldPen);
		chPen.DeleteObject();
	}
}

void CARKFont::TrnsElementInit(DBL sx, DBL sy, DBL wd, DBL hi, DBL rag)
{
	m_dSX = sx;
	m_dSY = sy;
	m_dXTime = wd / ARKMESH;
	m_dYTime = hi / ARKMESH;
	m_dSin = sin(rag);
	m_dCos = cos(rag);
}

void CARKFont::SetBold(int flag)
{
	m_nBold = flag;
}

void CARKFont::SetSlope(int flag)
{
	m_nSlope = flag;
}

void CARKFont::TrnsXY(LPDBL x, LPDBL y)
{
	DBL		xx, yy;
	DBL		tx, ty;

	xx = (*x - m_dMoveX) - ARKMESH / 2;
	yy = (ARKMESH - *y - m_dMoveY) - ARKMESH / 2;
	tx = (xx * m_dCos - yy * m_dSin + ARKMESH / 2);
	ty = (xx * m_dSin + yy * m_dCos + ARKMESH / 2);
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
			*y = (ty + tx * 0.5) * m_dYTime + m_dSY;
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
}

void CARKFont::DrawOutMove(CDC *pcDC, DBL x, DBL y)
{
	pcDC->MoveTo((int)x, (int)y);
	plgnfillmove(pcDC->m_hDC, x, y);
}

void CARKFont::DrawOutLine(CDC *pcDC, DBL x1, DBL y1, DBL x2, DBL y2)
{
	pcDC->LineTo((int)x2, (int)y2);
	plgnfillline(pcDC->m_hDC, x1, y1, x2, y2);
}

void CARKFont::DrawOutArc(CDC *pcDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

