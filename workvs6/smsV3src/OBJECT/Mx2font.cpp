
//
// Easy Scanner System
//
// object mx2font.cpp  マックス書体処理関数
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include <stdio.h>
#include "mx2font.h"

extern "C" {
	void basesjtokuten(int far sjis, int far *ku, int far *ten);
	BOOL  pdrwatrarcchk(int atr1, int atr2, int atr3);
	BOOL  pdrwatrbezechk(int atr1, int atr2, int atr3);
	void mojienvgetfontpath(LPSTR fontpath);
	void curveonpointtocntrlpoint(dbezetype *dbz, DBL tp1, DBL tp2);
	int curve3pcenterget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL FAR *x0, DBL FAR *y0);
	int curve3pragrget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0, DBL FAR *rag01, DBL FAR *rag03, DBL FAR *r);
}

IMPLEMENT_SERIAL(CMX2Font, CObject, 0)
CMX2Font::CMX2Font()
{
}

CMX2Font::CMX2Font(int nDummy)
{
	m_fileMX2 = NULL;
	m_pFSF1Font = NULL;
	m_nDataLen = 0;
	m_nTateYoko = 0;
	m_sBaseFname[0] = '\0';
	m_dTime = FSF1MESH / MX2MESH;
	m_dMoveX = 0;
	m_dMoveY = 0;
}

CMX2Font::~CMX2Font()
{
}

void CMX2Font::Serialize(CArchive& ar)
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

void CMX2Font::FSF1ClassSet(CFSF1Font* pFSF1Font)
{
	m_pFSF1Font = pFSF1Font;
}

void CMX2Font::FontNameSet(LPSTR pFont)
{
	char	fontpath[FNAMEMAX];

	mojienvgetfontpath(fontpath);
	sprintf(m_sBaseFname, "%s\\%s.ft2", fontpath, pFont);
}

void CMX2Font::SetTateYoko(int flag)
{
	m_nTateYoko = flag;
}

int CMX2Font::SF1TrnsDataLoad(UINT sjis)
{
	int		ret;

	ret = DataLoad(sjis);
	TrnsMX2ToFSF1();
	return(ret);
}

int CMX2Font::SF1TrnsDataLoadNON(UINT sjis)
{
	int		ret;

	ret = DataLoad(sjis);
	TrnsMX2ToFSF1NON();
	return(ret);
}

void CMX2Font::SF1TrnsDataSave(UINT sjis)
{
	TrnsCopyFSF1FBuf();
	DataSave(sjis);
}

int CMX2Font::DataLoad(UINT sjis)
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
	if(DataLoad(ku, ten) == FAIL){
		m_nDataLen = 0;
		return(FAIL);
	}
	return(0);
}

int CMX2Font::DataLoad(int ku, int ten)
{
	long 		idxseekp, datseekp;
	mx2idxtype	mx2idx;
	mx2htype	mx2head;
	mx2pointype	*dbuf;
	int			idx;

	if(FontOpen(ku, ten) == FAIL){
		return(FAIL);
	}

	IdxSeekPointGet(ku, ten, &idxseekp);
	fseek(m_fileMX2, idxseekp, 0);
	fread(&(mx2idx.seekp), sizeof(long), 1, m_fileMX2);
	fread(&(mx2idx.useekp), sizeof(long), 1, m_fileMX2);
	DataLenSeekPointGet(mx2idx, &datseekp);

	if(datseekp == 0){
		FontClose();
		return(FAIL);
	}
	if(fseek(m_fileMX2, datseekp, 0) != 0){
		FontClose();
		return(FAIL);
	}
	if(fread(&mx2head, sizeof(mx2htype), 1, m_fileMX2) != 1){
		FontClose();
		return(FAIL);
	}
	if(m_nTateYoko == ON){
		m_dMoveX = mx2head.x3;
		m_dMoveY = mx2head.y3;
	}
	else{
		m_dMoveX = mx2head.x1;
		m_dMoveY = mx2head.y1;
	}
	dbuf = (mx2pointype*)m_bFBuf;
	idx = 0;
	while(1){
		fread(&(dbuf[idx].atr), sizeof(short), 1, m_fileMX2);
		fread(&(dbuf[idx].x), sizeof(short), 1, m_fileMX2);
		fread(&(dbuf[idx].y), sizeof(short), 1, m_fileMX2);
		if(dbuf[idx].atr == -1){
			idx++;
			break;
		}
		idx++;
	}
	FontClose();
	m_nDataLen = idx;
	return(0);
}

int CMX2Font::DataSave(UINT sjis)
{
	int	ku, ten;

	if(sjis < 0x8140 || 0xffff < sjis){
		return(0);
	}
	if(0x9fff < sjis && sjis < 0xe021){
		return(0);
	}
	basesjtokuten(sjis, &ku, &ten);
	if(DataSave(ku, ten) == FAIL){
		return(FAIL);
	}
	return(0);
}

int CMX2Font::DataSave(int ku, int ten)
{
	long 		idxseekp, datseekp;
	mx2idxtype	mx2idx;
	mx2htype	mx2head;
	mx2pointype	*dbuf;
	int		idx, len;

	if(FontSaveOpen(ku, ten) == FAIL){
		return(FAIL);
	}

	IdxSeekPointGet(ku, ten, &idxseekp);
	fseek(m_fileMX2, idxseekp, 0);
	fread(&(mx2idx.seekp), sizeof(long), 1, m_fileMX2);
	fread(&(mx2idx.useekp), sizeof(long), 1, m_fileMX2);
	DataLenSeekPointGet(mx2idx, &datseekp);

	fseek(m_fileMX2, 0L, 2);
	datseekp = ftell(m_fileMX2);
	mx2head.x1 = 0;
	mx2head.y1 = 0;
	mx2head.x2 = (int)MX2MESH; 
	mx2head.y2 = (int)MX2MESH; 
	mx2head.x3 = 0;
	mx2head.y3 = 0;
	mx2head.x4 = (int)MX2MESH; 
	mx2head.y4 = (int)MX2MESH; 
	if(fwrite(&mx2head, sizeof(mx2htype), 1, m_fileMX2) != 1){
		FontClose();
		return(FAIL);
	}
	dbuf = (mx2pointype*)m_bFBuf;
	len = m_nDataLen / sizeof(mx2pointype);
	for(idx = 0; idx < len; idx++){
		fwrite(&(dbuf[idx].atr), sizeof(short), 1, m_fileMX2);
		fwrite(&(dbuf[idx].x), sizeof(short), 1, m_fileMX2);
		fwrite(&(dbuf[idx].y), sizeof(short), 1, m_fileMX2);
	}

	DataLenSeekPointSet(&mx2idx, datseekp);
	fseek(m_fileMX2, idxseekp, 0);
	fwrite(&(mx2idx.seekp), sizeof(long), 1, m_fileMX2);
	fwrite(&(mx2idx.useekp), sizeof(long), 1, m_fileMX2);

	FontClose();

	return(0);
}

int CMX2Font::FontOpen(int ku, int ten)
{
	if((m_fileMX2 = fopen(m_sBaseFname, "rb")) == NULL){
		return(FAIL);
	}
	return(0);
}

int CMX2Font::FontSaveOpen(int ku, int ten)
{
	if((m_fileMX2 = fopen(m_sBaseFname, "r+b")) == NULL){
		if(FontIdxInit() == FAIL){
			return(FAIL);
		}
	}
	return(0);
}

int CMX2Font::FontIdxInit()
{
	char	buf[RECMAX];
	int		len, blk, rest, i;

	if((m_fileMX2 = fopen(m_sBaseFname, "wb")) == NULL){
		return(FAIL);
	}
	memset(buf, '\0', MX2HEADBYTE);
	strcpy(buf, m_sBaseFname);
	fwrite(buf, MX2HEADBYTE, 1, m_fileMX2);
	len = (MX2KU * 94 + 1) * MX2IDXSIZE;
	memset(buf, '\0', RECMAX);
	blk = len / RECMAX;
	rest = len % RECMAX;
	for(i = 0; i < blk; i++){
		fwrite(buf, RECMAX, 1, m_fileMX2);
	}
	if(rest != 0){
		fwrite(buf, rest, 1, m_fileMX2);
	}
	fclose(m_fileMX2);

	if((m_fileMX2 = fopen(m_sBaseFname, "r+b")) == NULL){
		return(FAIL);
	}
	return(0);
}

void CMX2Font::FaceNameSet(LPSTR name)
{
	strcpy(m_sFaceName, name);
}

void CMX2Font::FontClose()
{
	fclose(m_fileMX2);
}

void CMX2Font::IdxSeekPointGet(int ku, int ten, long *seekp)
{
	int		idx;
	int		size;

	idx = (ku - 1) * 94 + (ten - 1);
	size = sizeof(long)+sizeof(long);
	*seekp = MX2HEADBYTE + size * idx;
}

void CMX2Font::DataLenSeekPointGet(mx2idxtype mx2idx, long *dseekp)
{
	*dseekp = 0;
	if(m_nTateYoko == ON){
		*dseekp = mx2idx.useekp;
	}
	if((*dseekp) == 0){
		*dseekp = mx2idx.seekp;
	}
}

void CMX2Font::DataLenSeekPointSet(mx2idxtype *mx2idx, long dseekp)
{
	if(m_nTateYoko == ON){
		mx2idx->useekp = dseekp;
	}
	else{
		mx2idx->seekp = dseekp;
	}
}

void CMX2Font::TrnsMX2ToFSF1()
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
	mx2pointype	*dbuf;

	m_pFSF1Font->PointSetStart();
	dbuf = (mx2pointype*)m_bFBuf;
	pcnt = m_nDataLen;
	aflag = OFF;
	for(pp = 0, dp = 0; pp < pcnt; pp++){
		AtrDXYGet(dbuf, pp, &atr, &x, &y);
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

void CMX2Font::TrnsMX2ToFSF1NON()
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
	mx2pointype	*dbuf;

	m_pFSF1Font->PointSetStart();
	dbuf = (mx2pointype*)m_bFBuf;
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

void CMX2Font::AtrDXYGet(mx2pointype dbuf[], int pp, int *atr, DBL *x, DBL *y)
{
	DBL		dx, dy;

	*atr = dbuf[pp].atr;
	dx = dbuf[pp].x;
	dy = dbuf[pp].y;
	*x = (dx - m_dMoveX) * m_dTime;
 	*y = FSF1MESH - (dy - m_dMoveY) * m_dTime;
}

int CMX2Font::ArcTrnsBezeSet(int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
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

int CMX2Font::ArcSet(int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	m_pFSF1Font->AtrDXYCrtSet(CURVATR, x1, y1);
	m_pFSF1Font->CrtPointInc();
	dp++;
	m_pFSF1Font->AtrDXYCrtSet(CURVATR|CONTATR, x2, y2);
	m_pFSF1Font->CrtPointInc();
	dp++;
	return(dp);
}

void CMX2Font::TrnsCopyFSF1FBuf()
{
	mx2pointype	*dbuf;
	LPBYTE	FSF1FBuf;
	int		pcntmax;
	int		spp, epp, pcnt;
	int		dp;

	dbuf = (mx2pointype*)m_bFBuf;
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
	m_nDataLen = dp * sizeof(mx2pointype); 

}

int CMX2Font::TrnsCopyRejionFSF1FBuf(BYTE fbuf[], int pcnt, int dp)
{
	mx2pointype	*dbuf;
	int		pp, p0, p1, p2, p3, p4;
	int		atr0, atr1, atr2, atr3, atr4;
	DBL		x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;

	if(pcnt <= 1){
		return(dp);
	}
	dbuf = (mx2pointype*)m_bFBuf;
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

int CMX2Font::EofSet(mx2pointype dbuf[], int dp)
{
	DBL		x, y;

	x = 0.0;
	y = 0.0;
	AtrDXYSet(dbuf, dp, ENDOF, x, y);
	dp++;
	return(dp);
}

int CMX2Font::MoveToSet(mx2pointype dbuf[], int dp, DBL x1, DBL y1)
{
	AtrDXYSet(dbuf, dp, MOVETO, x1, y1);
	dp++;
	return(dp);
}

int CMX2Font::EorSet(mx2pointype dbuf[], int dp)
{
	return(dp);
}

int CMX2Font::LineToSet(mx2pointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2)
{
	AtrDXYSet(dbuf, dp, LINETO, x2, y2);
	dp++;
	return(dp);
}

int CMX2Font::BezeToArcToSet(mx2pointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
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

int CMX2Font::ArcToSet(mx2pointype dbuf[], int dp, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	AtrDXYSet(dbuf, dp, ARCTO, x2, y2);
	dp++;
	AtrDXYSet(dbuf, dp, ARCEND, x3, y3);
	dp++;
	return(dp);
}

void CMX2Font::AtrDXYSet(mx2pointype dbuf[], int dp, int mode, DBL x, DBL y)
{
	dbuf[dp].atr = mode;
	dbuf[dp].x = (int)(x / m_dTime);
	dbuf[dp].y = (int)(MX2MESH - y / m_dTime);
}

