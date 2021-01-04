//
// Easy Scanner System Ver1.0
//
// ess  diblib.c	ＢＭＰファイル処理関数
//
//  Copyright (c) 1998 by THEON

#include	"stdafx.h"
#include	"Baselib.h"
#include	"Diblib.h"
#include	"Senvlib.h"
#include	"Cmprlib.h"

#define		ONELINEMAX	0x008000
//#define		ONELINEMAX	0x002000

int	imgfileno;
HANDLE	hOneLineReadBuf;
HANDLE	hOneLineWriteBuf;
HANDLE	hReadRGBQUAD;
HANDLE	hWriteRGBQUAD;
LPBYTE	lpOneLineReadBuf;
LPBYTE	lpOneLineWriteBuf;
LPBYTE	lpReadRGBQUAD;
LPBYTE	lpWriteRGBQUAD;

DllExport void diblibinit()
{
	long	size;

	size = sizeof(BYTE) * ONELINEMAX;
	hOneLineReadBuf = GlobalAlloc(GHND, size);
	hOneLineWriteBuf = GlobalAlloc(GHND, size);
	size = sizeof(RGBQUAD) * 256;
	hReadRGBQUAD = GlobalAlloc(GHND, size);
	hWriteRGBQUAD = GlobalAlloc(GHND, size);
	imgfileno = 0;
}

DllExport void diblibend()
{
	if(hWriteRGBQUAD != 0){
		GlobalFree(hWriteRGBQUAD);
	}
	if(hReadRGBQUAD != 0){
		GlobalFree(hReadRGBQUAD);
	}
	if(hOneLineWriteBuf != 0){
		GlobalFree(hOneLineWriteBuf);
	}
	if(hOneLineReadBuf != 0){
		GlobalFree(hOneLineReadBuf);
	}
}

DllExport void diblibResetFileNo()
{
	imgfileno = 0;
}

DllExport void diblibRGBtoHLS(int r, int g, int b, LPDBL H, LPDBL L, LPDBL S)
{
	DBL		dr, dg, db;
	DBL		Cr, Cg, Cb;
	DBL		nmin, nmax;

	dr = (DBL)r / 255.0;
	dg = (DBL)g / 255.0;
	db = (DBL)b / 255.0;
	nmin = dr;
	nmin = min(nmin, dg);
	nmin = min(nmin, db);
	nmax = dr;
	nmax = min(nmax, dg);
	nmax = min(nmax, db);

	*L = (nmin + nmax) / 2;
	if(nmin == nmax){
		*S = 0;
		*H = -1;
	}
	else{
		if(*L <= 0.5){
			*S = (nmax - nmin) / (nmax + nmin);
		}
		else{
			*S = (nmax - nmin) / (2 - nmax - nmin);
		}
		Cr = (nmax - dr) / (nmax - nmin);
		Cg = (nmax - dg) / (nmax - nmin);
		Cb = (nmax - db) / (nmax - nmin);
		if(dr == nmax){
			*H = (Cb - Cg) * 60;
		}
		else if(dg == nmax){
			*H = (2 + Cr - Cb) * 60;
		}
		else{	// if(db == nmax)
			*H = (4 + Cg - Cr) * 60;
		}

		if((*H) < 360){
			(*H) = (*H) + 360;
		}
	}
}

DllExport int diblibGetClrHLSLen(DBL chkH, DBL chkL, DBL chkS, DBL srcH, DBL srcL, DBL srcS)
{
	int		nlen;
	DBL		dlen;

	if(chkH == -1 || srcH == -1){
		nlen = (int)(fabs(chkL - srcL) * 255.0);
	}
	else{
		dlen = fabs(chkH - srcH);
		if(180 < dlen){
			dlen = 360 - dlen; 
		}
		nlen = (int)(dlen / 180.0 * 255.0);
	}
	return(nlen);
}

DllExport void diblibcreateimgfilename(LPTSTR imgfilename)
{
	TCHAR		szSafix[QRECMAX];
	TCHAR		tmpbmppath[FNAMEMAX];

	if (imgfilename[0] == _T('\n')) {
		_tcscpy_s(szSafix, _T("bmp"));
	}
	else {
		_tcscpy_s(szSafix, QRECMAX, basegetsafix(imgfilename));
	}
	senvgetbtppath(tmpbmppath);
	_stprintf_s(imgfilename, FNAMEMAX, _T("%s\\tbmp%04d."), tmpbmppath, imgfileno);
	imgfileno++;
	_tcscat_s(imgfilename, FNAMEMAX, szSafix);
}

DllExport void diblibBmpSmallPictureCreate(LPTSTR lpSrcFname, LPTSTR lpDstFname, DBL xsize, DBL ysize)
{
    FILE*	srcfp;
    FILE*	dstfp;
	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bminfo;
	BYTE	tmpbuf[32];
    int     ret;
	long	srcxsize, srcysize;
	long	dstxsize, dstysize;
	DBL		xtime, ytime;
	int		bitcount;
	long	yseektop, seekp;
	int		RGBQUADsize;
	long	xsrcbyte, xdstbyte;
	long	srcx, srcy;
	long	dstx, dsty;
	int		sift, img;
	
	dstxsize = (long)xsize;
	dstysize = (long)ysize;
	_tfopen_s(&srcfp, lpSrcFname, _T("r+b"));
	if(srcfp == NULL){
		return;
	}
	_tfopen_s(&dstfp, lpDstFname, _T("w+b"));
	if(dstfp == NULL){
		return;
	}

	ret = diblibGlobalLock();
	if(ret == FALSE){
		goto errorend;
	}

	//BMPファイルかチェック
	ret = fread(&bmf, sizeof(BITMAPFILEHEADER), 1, srcfp);
	if(ret != sizeof(bmf) || bmf.bfType != 'BM'){
        fclose(srcfp);
		return;
	}
	ret = fwrite(&bmf, sizeof(BITMAPFILEHEADER), 1, dstfp);

	ret = fread(&bminfo, sizeof(BITMAPINFOHEADER), 1, srcfp);
	srcxsize = bminfo.biWidth;
	srcysize = bminfo.biHeight;
	bitcount = bminfo.biBitCount;
	bminfo.biWidth = dstxsize;
	bminfo.biHeight = dstysize;
	ret = fwrite(&bminfo, sizeof(BITMAPINFOHEADER), 1, dstfp);

	seekp = fseek(srcfp, 0L, 1);
	RGBQUADsize = bmf.bfOffBits - seekp;
	ret = fread(lpReadRGBQUAD, RGBQUADsize, 1, srcfp);
	ret = fwrite(lpReadRGBQUAD, RGBQUADsize, 1, dstfp);

	xsrcbyte = srcxsize;
	xsrcbyte = (((DWORD)(xsrcbyte * bitcount + 7) / 8) + 3L) / 4L * 4L;
	xdstbyte = dstxsize;
	xdstbyte = (((DWORD)(xdstbyte * bitcount + 7) / 8) + 3L) / 4L * 4L;

	xtime = (DBL)srcxsize / (DBL)dstxsize;
	ytime = (DBL)srcysize / (DBL)dstysize;
	for(dsty = 0; dsty < dstysize; dsty++){
		srcy = (int)((DBL)dsty * ytime);
		memset(lpOneLineWriteBuf, 0, xdstbyte);
		yseektop = bmf.bfOffBits + xsrcbyte * srcy;
		for(dstx = 0; dstx < dstxsize; dstx++){
			srcx = (int)((DBL)dstx * xtime);
			switch(bitcount){
			case 24:
				seekp = yseektop + (srcx * 3);
				fseek(srcfp, seekp, 0);
				ret = fread(tmpbuf, 3, 1, srcfp);
				lpOneLineWriteBuf[dstx * 3 + 0] = tmpbuf[0];
				lpOneLineWriteBuf[dstx * 3 + 1] = tmpbuf[1];
				lpOneLineWriteBuf[dstx * 3 + 2] = tmpbuf[2];
				break;
			case 8:
				seekp = yseektop + srcx;
				fseek(srcfp, seekp, 0);
				ret = fread(tmpbuf, 1, 1, srcfp);
				lpOneLineWriteBuf[dstx] = tmpbuf[0];
				break;
			case 4:
				seekp = yseektop + (srcx / 2);
				fseek(srcfp, seekp, 0);
				ret = fread(tmpbuf, 1, 1, srcfp);
				sift = 4 * (1 - (srcx % 2));
				img = (tmpbuf[0] >> sift) & 0x0f;
				sift = 4 * (1 - (dstx % 2));
				lpOneLineWriteBuf[dstx/2] |= (img << sift);
				break;
			case 1:
				seekp = yseektop + (srcx / 8);
				fseek(srcfp, seekp, 0);
				ret = fread(tmpbuf, 1, 1, srcfp);
				sift = (7 - (srcx % 8));
				img = (tmpbuf[0] >> sift) & 0x01;
				sift = (7 - (dstx % 8));
				lpOneLineWriteBuf[dstx/8] |= (img << sift);
				break;
			}
		}
		ret = fwrite(lpOneLineWriteBuf, xdstbyte, 1, dstfp);
	}

errorend:
	diblibGlobalUnlock();
    fclose(srcfp);
    fclose(dstfp);
}

DllExport void diblibBmpOnePlanePictureCreate(LPTSTR lpSrcFname, LPTSTR lpDstFname, COLORREF chkclr, int nlimit)
{
	FILE*		srcfp;
	FILE*		dstfp;
	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bminfo;
    int     ret;
	long	xsize, ysize;
	int		bitcount;
	long	yseektop, seekp;
	int		tmpbfOffBits;
	int		RGBQUADsize;
	long	xsrcbyte, xdstbyte;
	long	xx, yy;
	LPRGBQUAD lpclr;
	int		chkr, chkg, chkb;
	int		srcr, srcg, srcb;
	int		clrlen;
	int		sift, img;
	
	chkr = GetRValue(chkclr); 
	chkg = GetGValue(chkclr); 
	chkb = GetBValue(chkclr); 

	_tfopen_s(&srcfp, lpSrcFname, _T("r+b"));
	if(srcfp == NULL){
		return;
	}
	_tfopen_s(&dstfp, lpDstFname, _T("w+b"));
	if(dstfp == NULL){
		return;
	}

	ret = diblibGlobalLock();
	if(ret == FALSE){
		fclose(srcfp);
		fclose(dstfp);
		return;
	}

	//BMPファイルかチェック
	ret = fread(&bmf, sizeof(BITMAPFILEHEADER), 1, srcfp);
	if(ret != 1 || bmf.bfType != 0x4d42){
        fclose(srcfp);
		return;
	}
	tmpbfOffBits = bmf.bfOffBits;
	bmf.bfOffBits =  sizeof(BITMAPFILEHEADER)
					+ sizeof(BITMAPINFOHEADER)
					+ sizeof(RGBQUAD)*2;
	ret = fwrite(&bmf, sizeof(BITMAPFILEHEADER), 1, dstfp);

	ret = fread(&bminfo, sizeof(BITMAPINFOHEADER), 1, srcfp);
	xsize = bminfo.biWidth;
	ysize = bminfo.biHeight;
	bitcount = bminfo.biBitCount;
	bminfo.biBitCount = 1;
	ret = fwrite(&bminfo, sizeof(BITMAPINFOHEADER), 1, dstfp);

	seekp = fseek(srcfp, 0L, 1);
	RGBQUADsize = tmpbfOffBits - seekp;
	ret = fread(lpReadRGBQUAD, RGBQUADsize, 1, srcfp);

	lpclr = (LPRGBQUAD)lpWriteRGBQUAD;
	lpclr[0].rgbBlue = 0;
	lpclr[0].rgbGreen = 0;
	lpclr[0].rgbRed = 0;
	lpclr[0].rgbReserved = 0;
	lpclr[1].rgbBlue = 255;
	lpclr[1].rgbGreen = 255;
	lpclr[1].rgbRed = 255;
	lpclr[1].rgbReserved = 0;

	RGBQUADsize = sizeof(RGBQUAD) * 2;
	ret = fwrite(lpWriteRGBQUAD, RGBQUADsize, 1, dstfp);

	xsrcbyte = (((DWORD)(xsize * bitcount + 7) / 8) + 3L) / 4L * 4L;
	xdstbyte = (((DWORD)(xsize * 1 + 7) / 8) + 3L) / 4L * 4L;

	for(yy = 0; yy < ysize; yy++){
		memset(lpOneLineWriteBuf, 0, xdstbyte);
		yseektop = tmpbfOffBits + xsrcbyte * yy;
		fseek(srcfp, yseektop, 0);
		fread(lpOneLineReadBuf, xsrcbyte, 1, srcfp);
		for(xx = 0; xx < xsize; xx++){
			diblibGetDotRGB(bitcount, (LPRGBQUAD)lpReadRGBQUAD, lpOneLineReadBuf, xx, &srcr, &srcg, &srcb);
			clrlen = (abs(chkr - srcr) +  abs(chkg - srcg) + abs(chkb - srcb)) / 3;
			if(255 < clrlen){
				clrlen = 255;
			}
			if(nlimit < clrlen){
				sift = (7 - (xx % 8));
				img = 0x01;
				lpOneLineWriteBuf[xx/8] |= (img << sift);
			}
		}
		ret = fwrite(lpOneLineWriteBuf, xdstbyte, 1, dstfp);
	}

	diblibGlobalUnlock();

    fclose(srcfp);
    fclose(dstfp);
}

DllExport void diblibBmpOnePlanePictureCreateES98(LPTSTR lpSrcFname, LPTSTR lpDstFname, COLORREF chkclr, int nlimit)
{
	FILE*		srcfp;
	FILE*		dstfp;
	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bminfo;
    int     ret;
	long	xsize, ysize;
	int		bitcount;
	long	yseektop, seekp;
	int		tmpbfOffBits;
	int		RGBQUADsize;
	long	xsrcbyte, xdstbyte;
	long	xx, yy;
	LPRGBQUAD lpclr;
	int		chkr, chkg, chkb;
	int		srcr, srcg, srcb;
	int		clrlen;
	int		sift, img;
	
	chkr = GetRValue(chkclr); 
	chkg = GetGValue(chkclr); 
	chkb = GetBValue(chkclr); 

	_tfopen_s(&srcfp, lpSrcFname, _T("r+b"));
	if(srcfp == NULL){
		return;
	}
	_tfopen_s(&dstfp, lpDstFname, _T("w+b"));
	if(dstfp == NULL){
		return;
	}

	ret = diblibGlobalLock();
	if(ret == FALSE){
		fclose(srcfp);
		fclose(dstfp);
		return;
	}

	//BMPファイルかチェック
	ret = fread(&bmf, sizeof(BITMAPFILEHEADER), 1, srcfp);
	if(ret != 1 || bmf.bfType != 0x4d42){
        fclose(srcfp);
		return;
	}
	tmpbfOffBits = bmf.bfOffBits;
	bmf.bfOffBits =  sizeof(BITMAPFILEHEADER)
					+ sizeof(BITMAPINFOHEADER)
					+ sizeof(RGBQUAD)*2;
	ret = fwrite(&bmf, sizeof(BITMAPFILEHEADER), 1, dstfp);

	ret = fread(&bminfo, sizeof(BITMAPINFOHEADER), 1, srcfp);
	xsize = bminfo.biWidth;
	ysize = bminfo.biHeight;
	bitcount = bminfo.biBitCount;
	bminfo.biBitCount = 1;
	ret = fwrite(&bminfo, sizeof(BITMAPINFOHEADER), 1, dstfp);

	seekp = fseek(srcfp, 0L, 1);
	RGBQUADsize = tmpbfOffBits - seekp;
	ret = fread(lpReadRGBQUAD, RGBQUADsize, 1, srcfp);

	lpclr = (LPRGBQUAD)lpWriteRGBQUAD;
	lpclr[0].rgbBlue = 0;
	lpclr[0].rgbGreen = 0;
	lpclr[0].rgbRed = 0;
	lpclr[0].rgbReserved = 0;
	lpclr[1].rgbBlue = 255;
	lpclr[1].rgbGreen = 255;
	lpclr[1].rgbRed = 255;
	lpclr[1].rgbReserved = 0;

	RGBQUADsize = sizeof(RGBQUAD) * 2;
	ret = fwrite(lpWriteRGBQUAD, RGBQUADsize, 1, dstfp);

	xsrcbyte = (((DWORD)(xsize * bitcount + 7) / 8) + 3L) / 4L * 4L;
	xdstbyte = (((DWORD)(xsize * 1 + 7) / 8) + 3L) / 4L * 4L;

	for(yy = 0; yy < ysize; yy++){
		memset(lpOneLineWriteBuf, 0, xdstbyte);
		yseektop = tmpbfOffBits + xsrcbyte * yy;
		fseek(srcfp, yseektop, 0);
		fread(lpOneLineReadBuf, xsrcbyte, 1, srcfp);
		for(xx = 0; xx < xsize; xx++){
			diblibGetDotRGB(bitcount, (LPRGBQUAD)lpReadRGBQUAD, lpOneLineReadBuf, xx, &srcr, &srcg, &srcb);
			clrlen = (abs(chkr - srcr) +  abs(chkg - srcg) + abs(chkb - srcb)) / 3;
			if(255 < clrlen){
				clrlen = 255;
			}
			if(nlimit > clrlen){
				sift = (7 - (xx % 8));
				img = 0x01;
				lpOneLineWriteBuf[xx/8] |= (img << sift);
			}
		}
		ret = fwrite(lpOneLineWriteBuf, xdstbyte, 1, dstfp);
	}

	diblibGlobalUnlock();

    fclose(srcfp);
    fclose(dstfp);
}

DllExport void diblibBmp256PlanePictureCreate(LPTSTR lpSrcFname, LPTSTR lpDstFname, DBL xsize, DBL ysize, COLORREF chkclr)
{
	FILE*		srcfp;
	FILE*		dstfp;
	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bminfo;
    int     ret;
	long	srcxsize, srcysize;
	long	dstxsize, dstysize;
	DBL		xtime, ytime;
	int		bitcount;
	long	yseektop, seekp;
	int		tmpbfOffBits;
	int		RGBQUADsize;
	long	xsrcbyte, xdstbyte;
	long	srcx, srcy;
	long	dstx, dsty;
	LPRGBQUAD lpclr;
	int		idx;
	long	crty;
	int		chkr, chkg, chkb;
	int		srcr, srcg, srcb;
	int		clrlen;
	
	chkr = GetRValue(chkclr); 
	chkg = GetGValue(chkclr); 
	chkb = GetBValue(chkclr);
	//diblibRGBtoHLS(chkr, chkg, chkb, &chkH, &chkL, &chkS);

	dstxsize = (long)xsize;
	dstysize = (long)ysize;
	_tfopen_s(&srcfp, lpSrcFname, _T("r+b"));
	if(srcfp == NULL){
		return;
	}
	_tfopen_s(&dstfp, lpDstFname, _T("w+b"));
	if(dstfp == NULL){
		fclose(srcfp);
		return;
	}

	ret = diblibGlobalLock();
	if(ret == FALSE){
		fclose(srcfp);
		fclose(dstfp);
		return;
	}

	//BMPファイルかチェック
	ret = fread(&bmf, sizeof(BITMAPFILEHEADER), 1, srcfp);
	if(ret != 1 || bmf.bfType != 0x4d42){
        fclose(srcfp);
		fclose(dstfp);
		return;
	}
	tmpbfOffBits = bmf.bfOffBits;

	ret = fread(&bminfo, sizeof(BITMAPINFOHEADER), 1, srcfp);
	srcxsize = bminfo.biWidth;
	srcysize = bminfo.biHeight;
	bitcount = bminfo.biBitCount;
	bminfo.biWidth = dstxsize;
	bminfo.biHeight = dstysize;
	bminfo.biBitCount = 8;
	bminfo.biClrUsed = 256;
	xsrcbyte = srcxsize;
	xsrcbyte = (((DWORD)(xsrcbyte * bitcount + 7) / 8) + 3L) / 4L * 4L;
	if(xsrcbyte > ONELINEMAX){
		MessageBox(NULL, _T("ファイルが大きすぎるため処理できません\n解像度をおとしてください\n"), _T("警告"), MB_OK);
        fclose(srcfp);
		fclose(dstfp);
		return;
	}
	xdstbyte = dstxsize;
	xdstbyte = (((DWORD)(xdstbyte * 8 + 7) / 8) + 3L) / 4L * 4L;

	bmf.bfOffBits =  sizeof(BITMAPFILEHEADER)
					+ sizeof(BITMAPINFOHEADER)
					+ sizeof(RGBQUAD)*256;
	bmf.bfSize = bmf.bfOffBits + xdstbyte * dstysize;

	ret = fwrite(&bmf, sizeof(BITMAPFILEHEADER), 1, dstfp);

	ret = fwrite(&bminfo, sizeof(BITMAPINFOHEADER), 1, dstfp);

	seekp = fseek(srcfp, 0L, 1);
	RGBQUADsize = tmpbfOffBits - seekp;
	ret = fread(lpReadRGBQUAD, RGBQUADsize, 1, srcfp);

	lpclr = (LPRGBQUAD)lpWriteRGBQUAD;
	for(idx = 0; idx < 256; idx++){
		lpclr[idx].rgbBlue = idx;
		lpclr[idx].rgbGreen = idx;
		lpclr[idx].rgbRed = idx;
		lpclr[idx].rgbReserved = 0;
	}
	RGBQUADsize = sizeof(RGBQUAD) * 256;
	ret = fwrite(lpWriteRGBQUAD, RGBQUADsize, 1, dstfp);

	xtime = (DBL)srcxsize / (DBL)dstxsize;
	ytime = (DBL)srcysize / (DBL)dstysize;

	crty = -1;
	for(dsty = 0; dsty < dstysize; dsty++){
		srcy = (int)((DBL)dsty * ytime);
		memset(lpOneLineWriteBuf, 0, xdstbyte);
		if(srcy != crty){
			yseektop = tmpbfOffBits + xsrcbyte * srcy;
			fseek(srcfp, yseektop, 0);
			fread(lpOneLineReadBuf, xsrcbyte, 1, srcfp);
		}
		for(dstx = 0; dstx < dstxsize; dstx++){
			srcx = (int)((DBL)dstx * xtime);
			diblibGetDotRGB(bitcount, (LPRGBQUAD)lpReadRGBQUAD, lpOneLineReadBuf, srcx, &srcr, &srcg, &srcb);
			//diblibRGBtoHLS(srcr, srcg, srcb, &srcH, &srcL, &srcS);
			//clrlen = diblibGetClrHLSLen(chkH, chkL, chkS, srcH, srcL, srcS);
			clrlen = (abs(chkr - srcr) +  abs(chkg - srcg) + abs(chkb - srcb)) / 3;
			if(clrlen < 3){
				clrlen = 0;
			}
			if(253 < clrlen){
				clrlen = 255;
			}
			//clrlen = 255 - clrlen;
			lpOneLineWriteBuf[dstx] = clrlen;
		}
		ret = fwrite(lpOneLineWriteBuf, xdstbyte, 1, dstfp);
	}

	diblibGlobalUnlock();

    fclose(srcfp);
    fclose(dstfp);

}

DllExport void diblibCmprimgToBmp(LPTSTR dotfname, LPTSTR bmpfname)
{
	FILE*		srcfp;
	FILE*		dstfp;
	char		tmp[QRECMAX];
	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bminfo;
	int		RGBQUADsize;
	LPRGBQUAD lpclr;
	int	dotxlen, dotylen, dotdpi;
	int	xbyte, y;
	int	len, ret;
	int	byte;
	long	seekp;
	LPBYTE	rbuf;
	LPBYTE	wbuf;

	_tfopen_s(&srcfp, dotfname, _T("r+b"));
	if(srcfp == NULL){
		return;
	}
	_tfopen_s(&dstfp, bmpfname, _T("w+b"));
	if(dstfp == NULL){
		return;
	}
	ret = diblibGlobalLock();
	if(ret == FALSE){
		fclose(srcfp);
		fclose(dstfp);
		return;
	}

	rbuf = lpOneLineReadBuf;
	wbuf = lpOneLineWriteBuf;
	fgets((LPSTR)(rbuf), RECMAX, srcfp);
	sscanf_s((LPSTR)(rbuf), "%s %d %d %d\n", tmp, QRECMAX, &dotxlen, &dotylen, &dotdpi);

	xbyte = (((DWORD)(dotxlen * 1 + 7) / 8) + 3L) / 4L * 4L;
	memset(&bmf, _T('\0'), sizeof(BITMAPFILEHEADER));
	bmf.bfType = 0x4d42;
	bmf.bfSize = bmf.bfOffBits + xbyte * dotylen;
	bmf.bfOffBits =  sizeof(BITMAPFILEHEADER)
					+ sizeof(BITMAPINFOHEADER)
					+ sizeof(RGBQUAD)*2;
	ret = fwrite(&bmf, sizeof(BITMAPFILEHEADER), 1, dstfp);

	memset(&bminfo, _T('\0'), sizeof(BITMAPINFOHEADER));
	bminfo.biSize = sizeof(BITMAPINFOHEADER);
	bminfo.biWidth = dotxlen;
	bminfo.biHeight = dotylen;
	bminfo.biPlanes = 1;
	bminfo.biBitCount = 1;
	bminfo.biCompression = BI_RGB;
    bminfo.biSizeImage = 0;
    bminfo.biXPelsPerMeter = dotdpi;
    bminfo.biYPelsPerMeter = dotdpi;
    bminfo.biClrUsed = 0;
    bminfo.biClrImportant = 0;

	ret = fwrite(&bminfo, sizeof(BITMAPINFOHEADER), 1, dstfp);

	lpclr = (LPRGBQUAD)lpWriteRGBQUAD;
	lpclr[0].rgbBlue = 0;
	lpclr[0].rgbGreen = 0;
	lpclr[0].rgbRed = 0;
	lpclr[0].rgbReserved = 0;
	lpclr[1].rgbBlue = 255;
	lpclr[1].rgbGreen = 255;
	lpclr[1].rgbRed = 255;
	lpclr[1].rgbReserved = 255;

	RGBQUADsize = sizeof(RGBQUAD) * 2;
	ret = fwrite(lpWriteRGBQUAD, RGBQUADsize, 1, dstfp);

	cmprlibrstorinit(dotxlen/8, lpOneLineReadBuf);
	for(y = 0; y < dotylen; y++){
		fread(rbuf, 1, 2, srcfp);
		len = ((rbuf[0] << 8) & 0xff00) + (rbuf[1] & 0xff);
		fread(rbuf, 1, len, srcfp);
		cmprlibrstor1(&(rbuf[1]), (int)(rbuf[0]));
		cmprlibrstor2(wbuf);

		seekp = (dotylen - y - 1) * xbyte + bmf.bfOffBits;
		fseek(dstfp, seekp, 0);
		for(byte = 0; byte < xbyte; byte++){
			wbuf[byte] = (~wbuf[byte]);
		}
		fwrite(wbuf, 1, xbyte, dstfp);
	}
	diblibGlobalUnlock();

    fclose(srcfp);
    fclose(dstfp);
}

DllExport void diblibGetDotRGB(int nbitcnt, LPRGBQUAD lpRGBQuad, LPBYTE lpLineImg, int x, int *r, int *g, int *b)
{
	LPBYTE	adr;
	DWORD	dwByte;
	int		sift, mask, plt;

	if(nbitcnt == 24){
		dwByte = (DWORD)(x * 3);
		adr = lpLineImg;
		adr = adr + dwByte;
		*b = (*adr) & 0x00ff;
		adr = adr + 1L;
		*g = (*adr) & 0x00ff;
		adr = adr + 1L;
		*r = (*adr) & 0x00ff;
	}
	else{
		switch(nbitcnt){
		case  8:
			dwByte = x;
			sift = 0;
			mask = 0xff;
			break;
		case 4:
			dwByte = x / 2;
			sift = 4 * (1 - (x % 2));
			mask = 0x0f;
			break;
		case 1:
			dwByte = x / 8;
			sift = (7 - (x % 8));
			mask = 0x01;
			break;
		}
		adr = lpLineImg;
		adr = adr + dwByte;
		plt = (*adr);
		plt = (plt >> sift) & mask;
		*b = lpRGBQuad[plt].rgbBlue;
		*g = lpRGBQuad[plt].rgbGreen;
		*r = lpRGBQuad[plt].rgbRed;
	}
}

DllExport void diblibSetDotRGB(int nbitcnt, LPRGBQUAD lpRGBQuad, LPBYTE lpLineImg, int x, int r, int g, int b)
{
	LPBYTE	adr;
	DWORD	dwByte;
	int		pltmax;
	int		sift, mask, plt;
	int		tb, tg, tr;
	int		len, tlen, tplt;

	if(nbitcnt == 24){
		dwByte = (DWORD)(x * 3);
		adr = lpLineImg;
		adr = adr + dwByte;
		(*adr) = b;
		adr = adr + 1L;
		(*adr) = g;
		adr = adr + 1L;
		(*adr) = r;
	}
	else{
		pltmax = 1 << nbitcnt;
		for(plt = 0; plt < pltmax; plt++){
			tb = lpRGBQuad[plt].rgbBlue;
			tg = lpRGBQuad[plt].rgbGreen;
			tr = lpRGBQuad[plt].rgbRed;
			if(tb == b && tg == g && tr == r){
				break;
			}
		}
		if(plt == pltmax){
			tlen = 256*3;
			tplt = 0;
			for(plt = 0; plt < pltmax; plt++){
				tb = lpRGBQuad[plt].rgbBlue;
				tg = lpRGBQuad[plt].rgbGreen;
				tr = lpRGBQuad[plt].rgbRed;
				len = tb - b + tg - g + tr - r;
				if(tlen > len){
					tlen = len;
					tplt = plt;
				}
			}
			plt = tplt;
		}

		switch(nbitcnt){
		case  8:
			dwByte = x;
			sift = 0;
			mask = 0xff;
			break;
		case 4:
			dwByte = x / 2;
			sift = 4 * (1 - (x % 2));
			mask = 0x0f;
			break;
		case 1:
			dwByte = x / 8;
			sift = (7 - (x % 8));
			mask = 0x01;
			break;
		}
		adr = lpLineImg;
		adr = adr + dwByte;
		(*adr) &= ~(mask << sift);
		(*adr) |= ((plt & mask) << sift);
	}
}

DllExport BOOL diblibGlobalLock()
{
	lpOneLineReadBuf = (LPBYTE)GlobalLock(hOneLineReadBuf);
	if(lpOneLineReadBuf == NULL){
		goto errorend;
	}
	lpOneLineWriteBuf = (LPBYTE)GlobalLock(hOneLineWriteBuf);
	if(lpOneLineWriteBuf == NULL){
		goto errorend;
	}
	lpReadRGBQUAD = (LPBYTE)GlobalLock(hReadRGBQUAD);
	if(lpReadRGBQUAD == NULL){
		goto errorend;
	}
	lpWriteRGBQUAD = (LPBYTE)GlobalLock(hWriteRGBQUAD);
	if(lpWriteRGBQUAD == NULL){
		goto errorend;
	}
	return(TRUE);
errorend:
	return(FALSE);
}

DllExport void diblibGlobalUnlock()
{
	if(lpOneLineReadBuf != NULL){
		GlobalUnlock(hOneLineReadBuf);
	}
	if(lpOneLineWriteBuf != NULL){
		GlobalUnlock(hOneLineWriteBuf);
	}
	if(lpReadRGBQUAD != NULL){
		GlobalUnlock(hReadRGBQUAD);
	}
	if(lpWriteRGBQUAD != NULL){
		GlobalUnlock(hWriteRGBQUAD);
	}
}
DllExport void diblibGetEncoderClsid(CString csFormat, CLSID* lpClsid)
{
	// image/bmp image/jpeg image/gif image/tiff image/png

	UINT  num = 0;
	UINT  size = 0;
	ImageCodecInfo* pImageCodecInfo;
	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return;
	pImageCodecInfo = (ImageCodecInfo*)new char[size];
	if (pImageCodecInfo == NULL)
		return;
	GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT n = 0; n < num; ++n) {
		CString csStr(pImageCodecInfo[n].MimeType);
		if(csStr.Compare(csFormat) == 0){
			*lpClsid = pImageCodecInfo[n].Clsid;
			delete pImageCodecInfo;
			return;
		}
	}
	delete pImageCodecInfo;
}

