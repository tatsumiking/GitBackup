
//
// Easy Scanner System
//
// object BoldLib.cpp  書体をボールドに変更する関数
//
//  Copyright (c) 1998 by THEON

#include "StdAfx.h"

#define	POINTMAX	1024

#include "BoldLib.h"

#include	 "../ObjLib/Senvlib.h"
#include	 "../ObjLib/Mdfylib.h"
#include	 "../ObjLib/Curvelib.h"
#include	 "../ObjLib/Baselib.h"


IMPLEMENT_SERIAL(CBoldLib, CObject, 3)
CBoldLib::CBoldLib()
{
}

void CBoldLib::Serialize(CArchive& ar)
{
}

CBoldLib::CBoldLib(int nDummy)
{
	m_lpCusTbl = (LPBYTE)malloc(POINTMAX);
	m_lpXYTbl1 = (XYType *) malloc(sizeof(XYType) * (POINTMAX));
	m_lpXYTbl2 = (XYType *) malloc(sizeof(XYType) * (POINTMAX));
}

CBoldLib::~CBoldLib()
{
	if(m_lpXYTbl2 != NULL){
		free(m_lpXYTbl2);
	}
	if(m_lpXYTbl1 != NULL){
		free(m_lpXYTbl1);
	}
	if(m_lpCusTbl != NULL){
		free(m_lpCusTbl);
	}
}

void CBoldLib::BmpFileOut(LPTSTR fname, BYTE far *m_lpBits, int xbyte, int xsize, int ysize)
{
	FILE				*fp;
	BITMAPFILEHEADER	bif;
	BITMAPINFOHEADER	bih;
	RGBQUAD				clr[2];
	int					headsize;
	BYTE				*sp;
	int					y;
	int					txbyte;

	memset((LPTSTR)(&bif), 0, sizeof(BITMAPFILEHEADER));
	memset((LPTSTR)(&bih), 0, sizeof(BITMAPINFOHEADER));
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

	_tfopen_s(&fp, fname, _T("wb"));
	fwrite(&bif, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(clr, sizeof(RGBQUAD), 2, fp);
	txbyte = (((long)xsize + 7L) / 8L + 3L) / 4L * 4L;
	for(y = 0; y < ysize; y++){
		sp = m_lpBits + (long)y * (long)txbyte;
		fwrite(sp, 1, xbyte, fp);
	}
	fclose(fp);
}

void CBoldLib::BitMapOutLineSet(BYTE *lpBits, BYTE *lpTBits, int xbyte, int xsize, int ysize)
{
	int		x, y;

	m_nXByte = xbyte;
	m_nXSize = xsize;
	m_nYSize = ysize;
	// アウトラインドットを取り出す
	for(y = 0; y < ysize; y++){
		for(x = 0; x < xsize; x++){
			if(BitOnOffCheck(lpBits, xbyte, x, y) == 1){
				if(BitOnOffCheck(lpBits, xbyte, x-1, y) == 0){
					BitOnSet(lpTBits, xbyte, x, y);
				}
				else if(BitOnOffCheck(lpBits, xbyte, x+1, y) == 0){
					BitOnSet(lpTBits, xbyte, x, y);
				}
				else if(BitOnOffCheck(lpBits, xbyte, x, y-1) == 0){
					BitOnSet(lpTBits, xbyte, x, y);
				}
				else if(BitOnOffCheck(lpBits, xbyte, x, y+1) == 0){
					BitOnSet(lpTBits, xbyte, x, y);
				}
			}
		}
	}
}

void CBoldLib::BitMapTrace(BYTE *lpBits, BYTE *lpTBits, int xbyte, int xsize, int ysize)
{
	int		x, y;

	// アウトライントレース処理
	for(y = 0; y < ysize; y++){
		for(x = 0; x < xsize; x++){
			if(BitOnOffCheck(lpTBits, xbyte, x, y) == 1){
				// BmpFileOut(_T("c:\\image.bmp"), lpTBits, xbyte, xsize, ysize);
				BitMapTarceSetPlgn(lpBits, lpTBits, 0, xbyte, x, y);
			}
		}
	}
}

void CBoldLib::TrnsDataList(DBL sx, DBL sy, DBL xtime, DBL ytime)
{
	CVect*	pcVect;
	int		pcnt, pp, atr;
	DBL		dx, dy;

	pcVect = (CVect*)(m_pcDataList->m_pcDataTop);
	while(1){
		if(pcVect == NULL){
			break;
		}
		pcnt = pcVect->GetPointCount();
		for(pp = 0; pp < pcnt; pp++){
			pcVect->GetAtrDXY(pp, &atr, &dx, &dy);
			dx = (dx - sx) * xtime;
			dy = (dy - sy) * ytime;
			pcVect->SetAtrDXY(pp, atr, dx, dy);
		}
		pcVect = (CVect*)pcVect->GetNext();
	}
}

// 内部関数
void CBoldLib::BitMapTarceSetPlgn(BYTE *lpBits, BYTE *lpTBits, int scus, int xbyte, int x, int y)
{
	int		rsx, rsy;
	int		tx, ty;
	int		tsx, tsy;
	int		cus, bcus, tcus;
	long	idx;
	FILE	*fp;
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	TCHAR	buf[RECMAX];

	SetPlgnInit();
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\pointdel.lst"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	rsx = x; rsy = y;
	tsx = x; tsy = y;
	tx = x; ty = y;
	FirstCusGet(lpBits, xbyte, x, y, &cus, &bcus);
	_stprintf_s(buf, _T("%d %d cus %d\n"), x, y, cus);
	baselibFputs(buf, fp);
	idx = 0;
	while(1){
		tx = x; ty = y;
		tcus = cus;
		NextCusGet(lpBits, xbyte, &x, &y, bcus, &cus);
		_stprintf_s(buf, _T("%d %d cus %d\n"), tx, ty, cus);
		baselibFputs(buf, fp);
		bcus = cus;
		if(cus == -1){
			break;
		}
		m_lpCusTbl[idx] = tcus;
		idx++;
		if(POINTMAX/4 <= idx){
			SetXYTblSetPlgn(lpTBits, xbyte, idx, tsx, tsy);
			idx = 0;
			tsx = x;
			tsy = y;
		}
		if(x == rsx && y == rsy){
			if(idx != 0){
				SetXYTblSetPlgn(lpTBits, xbyte, idx, tsx, tsy);
			}
			break;
		}
	}
	fclose(fp);
	_tfopen_s(&fp, fname, _T("r"));
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		_stscanf_s(buf, _T("%d %d"), &tx, &ty);
		BitOffSet(lpTBits, xbyte, tx, ty);
	}
	fclose(fp);
	
	SetPlgnEnd();
}

void CBoldLib::SetXYTblSetPlgn(BYTE *lpTBits, int xbyte, int idx, int tsx, int tsy)
{
	int		tx, ty;
	int		bx, by;
	int		point;
	int		cus;
	int		addx, addy;
	int		i;

	tx = tsx;
	ty = tsy;
	bx = tsx;
	by = tsy;
	point = 0;
	PlgnPointSet(m_lpXYTbl1, point, tx, ty);
	point++;
	for(i = 0; i < idx; i++){
		cus = m_lpCusTbl[i];
		CusAddGet(cus, &addx, &addy);
		tx += addx;
		ty += addy;
		if(bx != tx && by != ty){
			PlgnPointSet(m_lpXYTbl1, point, tx, ty);
			point++;
			bx = tx;
			by = ty;
		}
	}	
	PlgnPointSet(m_lpXYTbl1, point, tx, ty);
	point++;
	SetDataListPolygon(m_lpXYTbl1, m_lpXYTbl2, point);
}

void CBoldLib::NextCusGet(BYTE *lpTBits, int xbyte, int *retx, int *rety, int bcus, int *retcus)
{
	int		cus;
	int		x, y;
	int		addx, addy;
	int		i;

	x = *retx;
	y = *rety;
	cus = (bcus + 4 + 1) % 8;
	*retcus = -1;
	for(i = 0; i < 8; i++){
		CusAddGet(cus, &addx, &addy);
		if(BitOnOffCheck(lpTBits, xbyte, x+addx, y+addy) == 1){
			*retx = x+addx;
			*rety = y+addy;
			*retcus = cus;
			break;
		}
		cus = CusLegal(cus+1);
	}
}

int CBoldLib::FirstCusGet(BYTE *lpBits, int xbyte, int x, int y, int *ncus, int *bcus)
{
	int	tcus, cus, idx;
	int	addx, addy;

	for(tcus = 0; tcus < 8; tcus++){
		CusAddGet(tcus, &addx, &addy);
		if(BitOnOffCheck(lpBits, xbyte, x+addx, y+addy) == 0){
			break;
		}
	}
	if(tcus == 8){
		return(FALSE);
	}
	cus = tcus;
	for(idx = 0; idx < 8; idx++){
		CusAddGet(cus, &addx, &addy);
		if(BitOnOffCheck(lpBits, xbyte, x+addx, y+addy) == 1){
			break;
		}
		cus++;
		if(cus > 7){
			cus = 0;
		}
	}
	*ncus = cus;
	cus = tcus;
	for(idx = 0; idx < 8; idx++){
		CusAddGet(cus, &addx, &addy);
		if(BitOnOffCheck(lpBits, xbyte, x+addx, y+addy) == 1){
			break;
		}
		cus--;
		if(cus < 0){
			cus = 7;
		}
	}
	*bcus = (cus + 4) % 8;
	return(TRUE);
}

void CBoldLib::CusAddGet(int cus, int *addx, int *addy)
{
	switch(cus){
	case 7:
	case 0:
	case 1:
		*addx = -1;
		break;
	case 2:
	case 6:
		*addx = 0;
		break;
	case 3:
	case 4:
	case 5:
		*addx = 1;
		break;
	}
	switch(cus){
	case 1:
	case 2:
	case 3:
		*addy = 1;
		break;
	case 0:
	case 4:
		*addy = 0;
		break;
	case 5:
	case 6:
	case 7:
		*addy = -1;
		break;
	}
}

int CBoldLib::CusLegal(int cus)
{
	if(cus < 0){
		cus += 8;
	}
	if(7 < cus){
		cus -= 8;
	}
	return(cus);
}

int CBoldLib::BitOnOffCheck(BYTE *lpBits, int xbyte, int x, int y)
{
	int		nbyte;
	int		mask;
	BYTE	*sp;

	if(x < 0){
		return(0);
	}
	if(m_nXSize <= x){
		return(0);
	}
	if(y < 0){
		return(0);
	}
	if(m_nYSize <= y){
		return(0);
	}
	nbyte = x / 8;
	mask = 0x0080 >> (x % 8);
	sp = lpBits + (long)y * (long)xbyte + (long)nbyte;
	if(*sp & mask){
		return(1);
	}
	else{
		return(0);
	}
}

void CBoldLib::BitOnSet(BYTE *lpBits, int xbyte, int x, int y)
{
	int		nbyte;
	int		mask;
	BYTE	*sp;

	if(x < 0){
		return;
	}
	if(m_nXSize <= x){
		return;
	}
	if(y < 0){
		return;
	}
	if(m_nYSize <= y){
		return;
	}
	nbyte = x / 8;
	mask = 0x0080 >> (x % 8);
	sp = lpBits + (long)y * (long)xbyte + (long)nbyte;
	(*sp) |= mask;
}

void CBoldLib::BitOffSet(BYTE *lpBits, int xbyte, int x, int y)
{
	int		nbyte;
	int		mask;
	BYTE	*sp;

	if(x < 0){
		return;
	}
	if(m_nXSize <= x){
		return;
	}
	if(y < 0){
		return;
	}
	if(m_nYSize <= y){
		return;
	}
	nbyte = x / 8;
	mask = 0x0080 >> (x % 8);
	sp = lpBits + (long)y * (long)xbyte + (long)nbyte;
	(*sp) &= ~mask;
}

void CBoldLib::PlgnPointSet(XYType *lpXYTbl, int points, int tx, int ty)
{
	lpXYTbl[points].x = tx;
	lpXYTbl[points].y = ty;
}

void CBoldLib::SetDataListPolygon(XYType *lpXYTbl1, XYType *lpXYTbl2, int points)
{
	int		cnt1, cnt2;
	int		tp, pp;
	DBL		dx, dy;

	cnt1 = points;
	m_dFureLimit = 1;
	mdfyinit(m_dFureLimit);
	cnt2 = mdfyfirst(lpXYTbl1, cnt1, lpXYTbl2);
	if(cnt2 == 0){
		return;
	}
	cnt1 = mdfysecond(lpXYTbl2, cnt2, lpXYTbl1);
	if(cnt1 == 0){
		return;
	}
	for(tp = 0, pp = m_nPcnt; pp < m_nPcnt + cnt1; tp++, pp++){
		dx = lpXYTbl1[tp].x;
		dy = lpXYTbl1[tp].y;
		dy = m_nYSize - dy;
		m_pcVect->SetAtrDXY(pp, 0, dx, dy);
	}
	m_nPcnt = m_nPcnt + cnt1;
}

void CBoldLib::SetPlgnInit()
{
	m_nPcnt = 0;
	m_pcVect = new CVect(BASEVECTID);
}

void CBoldLib::SetPlgnEnd()
{
	m_pcVect->SetPointCount(m_nPcnt);
	PlgnLineToArcRejionSet(m_pcVect);
	m_nPcnt = m_pcVect->GetPointCount();
	if(2 < m_nPcnt){
		m_pcVect->RenewMiniMax();
		m_pcVect->OnFlag(ATNTION1FLAG);
		m_pcDataList->SetData((CData*)m_pcVect);
	}
	else{
		delete(m_pcVect);
		m_pcVect = NULL;
	}
}

void CBoldLib::PlgnLineToArcRejionSet(CVect *pcVect)
{
	int		pcnt;
	int		pp1, pp3;
	int		atr;
	DBL		x1, y1, x2, y2;
	CVect	*pcTmpVect;
	int		dp;

	dp = 0;
	pcnt = pcVect->GetPointCount();
	if(pcnt < 5){
		return;
	}
	pcTmpVect = new CVect(BASEVECTID);
	for(pp1 = 0; pp1 < pcnt-1;){
		if(PlgnCurveChk(pcVect, pp1, &x2, &y2, &pp3) == ON){
			pcVect->GetDXY(pp1, &x1, &y1);
			pcTmpVect->SetAtrDXY(dp, CURVATR, x1, y1);
			dp++;
			pcTmpVect->SetAtrDXY(dp, CONTATR|CURVATR, x2, y2);
			dp++;
			pp1 = pp3;
		}
		else{
			pcVect->GetDXY(pp1, &x1, &y1);
			pcTmpVect->SetAtrDXY(dp, 0, x1, y1);
			dp++;
			pp1++;
		}
	}
	pcnt = dp;
	for(pp1 = 0; pp1 < pcnt; pp1++){
		pcTmpVect->GetAtrDXY(pp1, &atr, &x1, &y1);
		pcVect->SetAtrDXY(pp1, atr, x1, y1);
	}
	pcVect->SetPointCount(pcnt);
	delete(pcTmpVect);
}

int CBoldLib::PlgnCurveChk(CVect *pcVect, int pp1, DBL *retx, DBL *rety, int *retpp)
{
	int		pcnt;
	int		pp2, pp3, tpp;
	DBL		x1, y1, x2, y2, x3, y3;
	DBL		x0, y0, r;
	DBL		rag01, rag03;
	DBL		subrag;
	DBL		tx, ty, thx, thy;
	DBL		limit;
	DBL		len1, len2;
	int		ret;

	limit = m_dFureLimit*2.0;
	pcnt = pcVect->GetPointCount();
	if(pp1 == pcnt - 1
	|| pp1 == pcnt - 2){
		return(OFF);
	}

	for(pp3 = pp1 + 3; pp3 < pcnt; pp3++){
		pp2 = (pp1+pp3)/2;
		pcVect->GetDXY(pp1, &x1, &y1);
		pcVect->GetDXY(pp2, &x2, &y2);
		pcVect->GetDXY(pp3, &x3, &y3);
		if(curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0) == FAIL){
			return(OFF);
		}
		if(curve3pragrget(x1,y1,x2,y2,x3,y3,x0,y0,&rag01,&rag03,&r) == FAIL){
			return(OFF);
		}
		subrag = rag03 - rag01;
		if(M_PI_4 < fabs(subrag)){
			break;
		}

		ret = 0;
		pcVect->GetDXY(pp1, &x1, &y1);
		for(tpp = pp1+1; tpp < pp3; tpp++){
			pcVect->GetDXY(tpp, &tx, &ty);
			if(PlgnCrclDastance(tx, ty, x0, y0, r, limit) == FAIL){
				ret = FAIL;
				break;
			}
			thx = (x1 + tx) / 2;
			thy = (y1 + ty) / 2;
			if(PlgnCrclDastance(thx, thy, x0, y0, r, limit*2) == FAIL){
				ret = FAIL;
				break;
			}
			x1 = tx;
			y1 = ty;
		}
		thx = (x1 + x3) / 2;
		thy = (y1 + y3) / 2;
		if(PlgnCrclDastance(thx, thy, x0, y0, r, limit*2) == FAIL){
			ret = FAIL;
		}
		pcVect->GetDXY(pp3, &x3, &y3);

		if(ret == FAIL){
			break;
		}
	}
	if(pp3 >= pcnt){
		return(OFF);
	}
	pp3 = pp3-1;
	if(pp3 < pp1+3){
		return(OFF);
	}
	pp2 = (pp1+pp3)/2;
	pcVect->GetDXY(pp1, &x1, &y1);
	pcVect->GetDXY(pp2, &x2, &y2);
	pcVect->GetDXY(pp3, &x3, &y3);
	len1 = hypot(x1-x2, y1-y2);
	len2 = hypot(x2-x3, y2-y3);
	if(len1 < limit*2 && len2 < limit*2){
		return(OFF);
	}
	*retx = x2;
	*rety = y2;
	*retpp = pp3;
	return(ON);
}

int CBoldLib::PlgnCrclDastance(DBL tx, DBL ty, DBL x0, DBL y0, DBL r, DBL flimit)
{
	DBL		frx, fry, r2, deltr;

	frx = tx - x0;
	fry = ty - y0;
	r2 = sqrt(frx*frx + fry*fry);
	deltr = fabs(r2 - r);
	if(deltr > flimit){
		return(FAIL);
	}
	return(0);
}

