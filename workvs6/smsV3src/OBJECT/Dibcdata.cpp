
//
// Easy Scanner System
//
// object dibdata.cpp  ÇaÇlÇoÉfÅ[É^ä÷åWä÷êî
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include "math.h"
#include "memory.h"
#include "io.h"

#include "dibcdata.h"

#define OUTXSIZE	64
#define OUTYSIZE	64

extern "C" {
	int	senvchckstopflag();
	void scrnsetslctpen(HDC PaintDC);
	void scrnsetnoslctpen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	int senvcheckoptslct();
	int	senvcheckdrawmode();
	int	 scrncheckundoflag();
	void scrndxymmtorltv(DBL *x, DBL *y);
	void scrndlenmmtorltv(LPDBL thick);
	void scrndlenrltvtomm(LPDBL len);
	void scrngetwinarea(int *sx, int *sy, int *ex, int *ey);
	void scrnrltvareaget(DBL *sx, DBL *sy, DBL *ex, DBL *ey);
	void diblibcreatebmpfile(LPSTR bmpfilename);
	void diblibGetDotRGB(int nbitcnt, LPRGBQUAD lpRGBQuad, LPBYTE lpLineImg, int x, int *r, int *g, int *b);
	void diblibSetDotRGB(int nbitcnt, LPRGBQUAD lpRGBQuad, LPBYTE lpLineImg, int x, int r, int g, int b);
}

IMPLEMENT_SERIAL(CDibCData, CObject, DIBCDATAID)
CDibCData::CDibCData()
{
}

CDibCData::CDibCData(int nDummy)
:CData(nDummy)
{
	m_dDstSx = 0;
	m_dDstSy = 0;
	m_dDstWidth = 0;
	m_dDstHight = 0;
	m_dSrcSx = 0;
	m_dSrcSy = 0;
	m_dSrcWidth = 0;
	m_dSrcHight = 0;
	m_dSrcXDPI = 72;
	m_dSrcYDPI = 72;
	m_dXByte = 0;
	m_nStat = 1;
	m_lImageTopSeek = 0;
	m_lFileSize = 0;
	m_szBmpFname[0] = '\0';
	m_nBitCount = 0;
	m_hDib = 0;
	m_hBkDib = 0;
	m_lStretchMode = SRCCOPY;
	m_dOutTrnsx = 1.0;
	m_dOutTrnsy = 1.0;
	m_hOutDib = 0;
}

CDibCData::~CDibCData()
{
	if(m_hDib != 0){
		GlobalFree(m_hDib);
	}
	if(m_hOutDib != 0){
		GlobalFree(m_hOutDib);
	}
}

int CDibCData::CheckID()
{
	return(DIBCDATAID);
}

void CDibCData::Serialize(CArchive& ar)
{
	FILE	*fp;
	UCHAR	ch;
	ULONG	idx;

	if (ar.IsStoring()){
		ar << m_dDstSx;
		ar << m_dDstSy;
		ar << m_dDstWidth;
		ar << m_dDstHight;
		if((_access(m_szBmpFname, 0 )) != 0){
			m_nStat = 0;
			ar << m_nStat;
			return;			
		}
		else{
			m_nStat = 1;
			ar << m_nStat;
		}
		ar << CString(m_szBmpFname);

		if(scrncheckundoflag() == OFF){
			ar << m_lFileSize;
			FileSave();
			if((fp = fopen(m_szBmpFname, "rb")) != NULL){
				for(idx = 0; idx < m_lFileSize; idx++){
					fread(&ch, 1, 1, fp);
					ar << ch;
				}
				fclose(fp);
			}
		}
	}
	else{
		ar >> m_dDstSx;
		ar >> m_dDstSy;
		ar >> m_dDstWidth;
		ar >> m_dDstHight;
		ar >> m_nStat;
		if(m_nStat == 0){
			m_szBmpFname[0] = '\0';
			return;
		}
		CString csBmpFname;
		ar >> csBmpFname;
		strcpy(m_szBmpFname, csBmpFname.GetBuffer(FNAMEMAX));
		if(scrncheckundoflag() == OFF){
			// êÃÇÃÉtÉ@ÉCÉãñºÇÕñ≥éãÇ∑ÇÈ
			diblibcreatebmpfile(m_szBmpFname);
			fp = fopen(m_szBmpFname, "wb");
			ar >> m_lFileSize;
			for(idx = 0; idx < m_lFileSize; idx++){
				ar >> ch;
				if(fp != NULL){
					fwrite(&ch, 1, 1, fp);
				}
			}
			if(fp != NULL){
				fclose(fp);
			}
		}
		DibReload();
	}
	CData::Serialize(ar);
	if (!(ar.IsStoring()))
	{
		RenewMiniMax();
	}
}

//p ì‡ïîÉfÅ[É^Å[Ç™ãÛÇÃèÛë‘Ç≈Ç†ÇÍÇŒçÌèúÇµÇ∑Ç◊ÇƒÇ™ãÛÇÃÇ∆Ç´TRUEÇï‘Ç∑
int CDibCData::CheckNullData()
{
	if(m_hDib == 0){
		return(TRUE);
	}
	return(FALSE);
}

BOOL CDibCData::IsDibCObject()
{
	return(TRUE);
}

int CDibCData::SetStretchMode(int mode)
{
	int	backmode;

	backmode = m_lStretchMode;
	m_lStretchMode = mode;
	return(backmode);
}

void CDibCData::SetBmpFname(LPSTR bmpfname)
{
	strcpy(m_szBmpFname, bmpfname);
}

void CDibCData::SetBkBmpFname(LPSTR bmpfname)
{
	strcpy(m_szBkBmpFname, bmpfname);
}

void CDibCData::GetBmpFname(LPSTR bmpfname)
{
	strcpy(bmpfname, m_szBmpFname);
}

void CDibCData::SetRGBQuadData(int plt, int rval, int gval, int bval)
{
	LPBYTE	lpDib;
	RGBQUAD FAR *lpRGBQuad;

	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if(lpDib == NULL){
		AfxMessageBox("ÇaÇlÇoópÇÃÉÅÉÇÉäÇ™ÇkÇnÇbÇjÇ≈Ç´Ç‹ÇπÇÒ", MB_OK);
		return;
	}
	lpRGBQuad = (RGBQUAD FAR *)(lpDib + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	lpRGBQuad[plt].rgbBlue = bval;
	lpRGBQuad[plt].rgbGreen = gval;
	lpRGBQuad[plt].rgbRed = rval;

	GlobalUnlock(m_hDib);
}

void CDibCData::GetRGBQuadData(int plt, LPINT rval, LPINT gval, LPINT bval)
{
	LPBYTE	lpDib;
	RGBQUAD FAR *lpRGBQuad;

	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if(lpDib == NULL){
		AfxMessageBox("ÇaÇlÇoópÇÃÉÅÉÇÉäÇ™ÇkÇnÇbÇjÇ≈Ç´Ç‹ÇπÇÒ", MB_OK);
		return;
	}
	lpRGBQuad = (RGBQUAD FAR *)(lpDib + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	*bval = lpRGBQuad[plt].rgbBlue;
	*gval = lpRGBQuad[plt].rgbGreen;
	*rval = lpRGBQuad[plt].rgbRed;

	GlobalUnlock(m_hDib);
}

void CDibCData::OnDraw(CDC *pcDC)
{
	if(senvchckstopflag() == ON){
		return;
	}
	FillDraw(pcDC);
}

void CDibCData::StockClrDraw(CDC *pcDC)
{
	DrowAreaBoxsCross(pcDC);
}

void CDibCData::FillDraw(CDC *pcDC)
{
	int		mode;
	int		rastercaps;
	int		ret;

	if(m_szBmpFname[0] == '\0'){
		return;
	}
	if(m_hDib == 0){
		return;
	}
	// 1999.03.25 èCê≥ 
	// 1999.04.02 Ç‚Ç¡ÇœÇËPC-60Ç≈ÇÕPaintDibDirectÇ™ìÆçÏÇµÇ»Ç©Ç¡ÇΩ
#ifdef ACELE
	PaintStretchDIBits(pcDC);
#else
	mode = senvcheckdrawmode(); 
	if(mode == BLACKVIEW
	|| mode == DIRCTVIEW
	|| mode == PRINTVIEW){
		rastercaps = GetDeviceCaps(pcDC->m_hDC, RASTERCAPS);
		if(rastercaps & RC_STRETCHDIB){
			ret = PaintStretchDIBits(pcDC, SRCCOPY);
		}
		else if(rastercaps & RC_STRETCHBLT){
			ret = PaintStretchBlt(pcDC);
		}
		else{
			ret = FALSE;
		}
	}
	else if(mode == METAFVIEW){
		ret = PaintStretchBlt(pcDC);
	}
	else{
		PaintDibDirect(pcDC);
		/*
		if(m_nBitCount == 24 || m_nBitCount == 8 || m_nBitCount == 1){
			ret = FALSE;
			PaintDibDirect(pcDC);
		}
		else{
			rastercaps = GetDeviceCaps(pcDC->m_hDC, RASTERCAPS);
			if(rastercaps & RC_STRETCHDIB){
				ret = PaintStretchDIBits(pcDC, SRCCOPY);
			}
			else{
				ret = FALSE;
			}

			if(ret == FALSE){
				ret = PaintStretchBlt(pcDC);
			}
		}
		*/
	}
#endif
}

void CDibCData::AtntionDraw(CDC *pcDC)
{
	scrnsetslctpen(pcDC->m_hDC);
	DrowAreaBoxsCross(pcDC);
	scrnpenend(pcDC->m_hDC);
}

void CDibCData::NoactDraw(CDC *pcDC)
{
	scrnsetnoslctpen(pcDC->m_hDC);
	DrowAreaBoxsCross(pcDC);
	scrnpenend(pcDC->m_hDC);
}

int CDibCData::DibBkLoad()
{
	int		ret;

	ret = DibBkReload();
	return(ret);
}

int CDibCData::DibBkReload()
{
	CFile	cpFile;
	LPBYTE	lpDib;
	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bminfo;
	int		ret;
	int		r0, g0, b0;
	int		r1, g1, b1;

	if((cpFile.Open(m_szBkBmpFname, CFile::modeRead)) == FALSE){
		AfxMessageBox("ÇaÇlÇoÉtÉ@ÉCÉãÇ™äJÇØÇ‹ÇπÇÒ", MB_OK);
		return(FAIL);
	}

	//BMPÉtÉ@ÉCÉãÇ©É`ÉFÉbÉN
	ret = cpFile.Read(&bmf, sizeof(bmf));
	if(ret != sizeof(bmf) || bmf.bfType != 0x4d42){
		ret = FAIL;
		AfxMessageBox("ÇaÇlÇoÉtÉ@ÉCÉãÇ≈ÇÕÇ†ÇËÇ‹ÇπÇÒ", MB_OK);
		goto ErrorEnd;
	}

	ret = cpFile.Read(&bminfo, sizeof(BITMAPINFOHEADER));
	m_lBkFileSize = cpFile.GetLength();

	if(m_hBkDib != 0){
		GlobalFree(m_hBkDib);
	}

	m_hBkDib = GlobalAlloc(GHND, m_lBkFileSize);
	if(m_hBkDib == 0){
		ret = FAIL;
		AfxMessageBox("ÇaÇlÇoópÇÃÉÅÉÇÉäÇ™ämï€Ç≈Ç´Ç‹ÇπÇÒ", MB_OK);
		goto ErrorEnd;
	}

	cpFile.SeekToBegin();
	lpDib = (LPBYTE)GlobalLock(m_hBkDib);
	if(lpDib == NULL){
		ret = FAIL;
		AfxMessageBox("ÇaÇlÇoópÇÃÉÅÉÇÉäÇ™ÇkÇnÇbÇjÇ≈Ç´Ç‹ÇπÇÒ", MB_OK);
		goto ErrorEnd;
	}
	if(cpFile.ReadHuge(lpDib, m_lBkFileSize) != m_lBkFileSize){
		ret = FAIL;
		AfxMessageBox("ÇaÇlÇoÉCÉÅÅ[ÉWÇ™ì«Ç›çûÇﬂÇ‹ÇπÇÒÇ≈ÇµÇΩ", MB_OK);
		GlobalUnlock(m_hBkDib);
		goto ErrorEnd;
	}
	GlobalUnlock(m_hBkDib);
	m_lBkImageTopSeek = bmf.bfOffBits;
	m_dBkSrcWidth = bminfo.biWidth;
	m_dBkSrcHight = bminfo.biHeight;
	ret = 0;
ErrorEnd:
	cpFile.Close();
	return(ret);
}

int CDibCData::DibLoad()
{
	int		ret;

	ret = DibReload();
	if(ret != FAIL){
		m_dDstWidth = m_dSrcWidth/m_dSrcXDPI*25.4;
		m_dDstHight = m_dSrcHight/m_dSrcYDPI*25.4;
		m_dDstSx = INITSETX;
		m_dDstSy = INITSETY;
	}
	return(ret);
}

int CDibCData::DibReload()
{
	CFile	cpFile;
	LPBYTE	lpDib;
	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bminfo;
	int		ret;
	int		r0, g0, b0;
	int		r1, g1, b1;

	if((cpFile.Open(m_szBmpFname, CFile::modeRead)) == FALSE){
		AfxMessageBox("ÇaÇlÇoÉtÉ@ÉCÉãÇ™äJÇØÇ‹ÇπÇÒ", MB_OK);
		return(FAIL);
	}

	//BMPÉtÉ@ÉCÉãÇ©É`ÉFÉbÉN
	ret = cpFile.Read(&bmf, sizeof(bmf));
	if(ret != sizeof(bmf) || bmf.bfType != 0x4d42){
		ret = FAIL;
		AfxMessageBox("ÇaÇlÇoÉtÉ@ÉCÉãÇ≈ÇÕÇ†ÇËÇ‹ÇπÇÒ", MB_OK);
		goto ErrorEnd;
	}

	ret = cpFile.Read(&bminfo, sizeof(BITMAPINFOHEADER));
	m_nBitCount = bminfo.biBitCount;

	m_lFileSize = cpFile.GetLength();

	if(m_hDib != 0){
		GlobalFree(m_hDib);
	}

	m_hDib = GlobalAlloc(GHND, m_lFileSize);
	if(m_hDib == 0){
		ret = FAIL;
		AfxMessageBox("ÇaÇlÇoópÇÃÉÅÉÇÉäÇ™ämï€Ç≈Ç´Ç‹ÇπÇÒ", MB_OK);
		goto ErrorEnd;
	}

	cpFile.SeekToBegin();
	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if(lpDib == NULL){
		ret = FAIL;
		AfxMessageBox("ÇaÇlÇoópÇÃÉÅÉÇÉäÇ™ÇkÇnÇbÇjÇ≈Ç´Ç‹ÇπÇÒ", MB_OK);
		goto ErrorEnd;
	}
	if(cpFile.ReadHuge(lpDib, m_lFileSize) != m_lFileSize){
		ret = FAIL;
		AfxMessageBox("ÇaÇlÇoÉCÉÅÅ[ÉWÇ™ì«Ç›çûÇﬂÇ‹ÇπÇÒÇ≈ÇµÇΩ", MB_OK);
		GlobalUnlock(m_hDib);
		goto ErrorEnd;
	}
	GlobalUnlock(m_hDib);

	m_lImageTopSeek = bmf.bfOffBits;
	m_dSrcWidth = bminfo.biWidth;
	m_dSrcHight = bminfo.biHeight;
	m_dSrcXDPI = bminfo.biXPelsPerMeter;
	m_dSrcYDPI = bminfo.biYPelsPerMeter;
	/*
	if(m_dSrcXDPI > 10000 || m_dSrcYDPI > 10000){
		m_dSrcXDPI /= 1000;
		m_dSrcYDPI /= 1000;
	}
	if((m_dSrcXDPI != m_dSrcYDPI) 
	|| (m_dSrcXDPI == 0 || m_dSrcYDPI == 0)){
		m_dSrcXDPI = 72.0/25.4;
		m_dSrcYDPI = 72.0/25.4;
	}
	*/
	if((m_dSrcXDPI != m_dSrcYDPI) 
	|| (m_dSrcXDPI == 0 || m_dSrcYDPI == 0)
	|| (m_dSrcXDPI > 2400 || m_dSrcYDPI > 2400)){
		m_dSrcXDPI = 72;
		m_dSrcYDPI = 72;
	}

	m_dXByte = (((DWORD)(m_dSrcWidth * m_nBitCount + 7) / 8) + 3L) / 4L * 4L;
	ret = 0;
	cpFile.Close();


	// ï\é¶ópÇcÇhÇaÉfÅ[É^ê∂ê¨
	long	allocsize;
	LPBYTE	lpOutDib;

	m_nOutXByte = (((OUTXSIZE*m_nBitCount + 7) / 8) + 3L) / 4L * 4L;
	allocsize = m_lImageTopSeek + m_nOutXByte * OUTYSIZE;
	m_hOutDib = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, allocsize);
	lpDib = (LPBYTE)GlobalLock(m_hDib);
	lpOutDib = (LPBYTE)GlobalLock(m_hOutDib);
	memcpy(lpOutDib, lpDib, m_lImageTopSeek);
	GlobalUnlock(m_hOutDib);
	GlobalUnlock(m_hDib);

	// 2002.01.18 ÇQílâÊëúÇ≈îíçïÇ™îΩì]ÇµÇƒÇ¢ÇÈÉtÉ@ÉCÉãÇ…ëŒÇ∑ÇÈèàóù
	if(m_nBitCount == 1){
		GetRGBQuadData(0, &r0, &g0, &b0);
		GetRGBQuadData(1, &r1, &g1, &b1);
		if(r0 == 255 && g0 == 255 && b0 == 255){
			RasterRevace();
			SetRGBQuadData(0, r1, g1, b1);
			SetRGBQuadData(1, r0, g0, b0);
		}
	}

	return(ret);
ErrorEnd:
	cpFile.Close();
	return(ret);
}

void CDibCData::GetSrcElement(LPDBL srcxdpi, LPDBL srcydpi, LPDBL srcwd, LPDBL srchi)
{
	*srcxdpi = m_dSrcXDPI;
	*srcydpi = m_dSrcYDPI;
	*srcwd = m_dSrcWidth;
	*srchi = m_dSrcHight;
}

void CDibCData::SetSrcElement(DBL srcxdpi, DBL srcydpi, DBL srcwd, DBL srchi)
{
	m_dSrcXDPI = srcxdpi;
	m_dSrcYDPI = srcydpi;
	m_dSrcWidth = srcwd;
	m_dSrcHight = srchi;
}

void CDibCData::GetDstElement(LPDBL dstsx, LPDBL dstsy, LPDBL dstwd, LPDBL dsthi)
{
	*dstsx = m_dDstSx;
	*dstsy = m_dDstSy;
	*dstwd = m_dDstWidth;
	*dsthi = m_dDstHight;
}

void CDibCData::SetDstElement(DBL dstsx, DBL dstsy, DBL dstwd, DBL dsthi)
{
	m_dDstSx = dstsx;
	m_dDstSy = dstsy;
	m_dDstWidth = dstwd;
	m_dDstHight = dsthi;
}

void CDibCData::DrowAreaBoxsCross(CDC *pcDC)
{
	DBL		dsx, dsy, dw, dh;
	DBL		dex, dey;
	int		sx, sy, ex, ey;

	GetDstElement(&dsx, &dsy, &dw, &dh);
	dsx = dsx;
	dsy = dsy;
	dex = dsx + dw;
	dey = dsy + dh;
	scrndxymmtorltv(&dsx, &dsy);
	scrndxymmtorltv(&dex, &dey);
	sx = (int)dsx - 1; sy = (int)dsy - 1;
	ex = (int)dex + 1; ey = (int)dey + 1;
	pcDC->MoveTo(sx, sy);
	pcDC->LineTo(sx, ey);
	pcDC->LineTo(ex, ey);
	pcDC->LineTo(ex, sy);
	pcDC->LineTo(sx, sy);

	pcDC->MoveTo(sx, sy);
	pcDC->LineTo(ex, ey);
	pcDC->MoveTo(sx, ey);
	pcDC->LineTo(ex, sy);
}

int CDibCData::PaintStretchBlt(CDC *pcDC)
{
	DBL		ddsx, ddsy, ddex, ddey;
	DBL		ddw, ddh;
	DBL		dssx, dssy, dsex, dsey;
	DBL		dsw, dsh, dsxdpi, dsydpi;
	DBL		dtdsx, dtdsy;
	DBL		timex, timey;
	DBL		minx, miny, maxx, maxy;
	int		ndsx, ndsy, ndw, ndh;
	int		nssx, nssy, nsw, nsh;
	int		ret;
	HDC		hMemDC;
	HBITMAP	hBitmap, hOldBitmap;
	LPBYTE	lpDib;
	LPBYTE 	lpInfo;
	LPSTR	lpBits;
	CRect	rect;

	GetDstElement(&ddsx, &ddsy, &ddw, &ddh);
	ddsx = ddsx;
	ddsy = ddsy;
	ddex = ddsx + ddw;
	ddey = ddsy + ddh;
	scrndxymmtorltv(&ddsx, &ddsy);
	scrndxymmtorltv(&ddex, &ddey);

	dtdsy = ddsy;
	ddsy = ddey;
	ddey = dtdsy;

	ddw = ddex - ddsx;
	ddh = ddey - ddsy;
	dtdsx = ddsx;
	dtdsy = ddsy;

	GetSrcElement(&dsxdpi, &dsydpi, &dsw, &dsh);

	timex = dsw / ddw;
	timey = dsh / ddh;
	
	dssx = 0.0; dssy = 0.0;
	dsex = dsw; dsey = dsh;

	scrnrltvareaget(&minx, &miny, &maxx, &maxy);

	if(ddsx < minx){
		dssx = (minx-dtdsx) * timex;
		ddsx = minx;
	}
	else{
		dssx = 0.0;
	}
	if(ddsy < miny){
		dssy = (miny-dtdsy) * timey;
		ddsy = miny;
	}
	else{
		dssy = 0.0;
	}

	if(ddex > maxx){
		dsex = (maxx-dtdsx) * timex;
		ddex = maxx;
	}
	else{
		dsex = dsw;
	}
	if(ddey > maxy){
		dsey = (maxy-dtdsy) * timey;
		ddey = maxy;
	}
	else{
		dsey = dsh;
	}
	ddw = ddex - ddsx;
	ddh = ddey - ddsy;
	dsw = dsex - dssx;
	dsh = dsey - dssy;

	ndsx = (int)ddsx; ndsy = (int)ddsy; ndw = (int)ddw; ndh = (int)ddh;
	nssx = (int)dssx; nssy = (int)dssy; nsw = (int)dsw; nsh = (int)dsh;

	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if(lpDib == NULL){
		AfxMessageBox("ÇaÇlÇoópÇÃÉÅÉÇÉäÇ™ÇkÇnÇbÇjÇ≈Ç´Ç‹ÇπÇÒ", MB_OK);
		return(FALSE);
	}
	lpInfo = lpDib + sizeof(BITMAPFILEHEADER);
	lpBits = (LPSTR)(lpDib + m_lImageTopSeek);
	hMemDC = CreateCompatibleDC(pcDC->m_hDC);
	if(hMemDC == 0){
		GlobalUnlock(m_hDib);
		return(FALSE);
	}
	hBitmap = CreateDIBitmap(pcDC->m_hDC, (LPBITMAPINFOHEADER)lpInfo,
						 CBM_INIT, lpBits, (LPBITMAPINFO)lpInfo, 0);
	if(hBitmap == 0){
		DeleteDC(hMemDC);
		GlobalUnlock(m_hDib);
		return(FALSE);
	}
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	ret = StretchBlt(pcDC->m_hDC,
			ndsx, ndsy, ndw, ndh, hMemDC,
			nssx, nssy, nsw, nsh, m_lStretchMode);
	if(ret == 0){
		ret = FALSE;
	}
	else{
		ret = TRUE;
	}
	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);
	GlobalUnlock(m_hDib);
	return(ret);
}

int CDibCData::PaintStretchDIBits(CDC *pcDC, int mode)
{
	DBL		ddsx, ddsy, ddex, ddey;
	DBL		ddw, ddh;
	DBL		dssx, dssy, dsex, dsey;
	DBL		dsw, dsh, dsth, dsxdpi, dsydpi;
	DBL		dtdsx, dtdsy;
	DBL		timex, timey;
	DBL		minx, miny, maxx, maxy;
	int		ndsx, ndsy, ndw, ndh;
	int		nssx, nssy, nsw, nsh;
	int		ret;
	LPBYTE	lpDib;
	LPBYTE 	lpInfo;
	LPSTR	lpBits;
	CRect	rect;

	GetDstElement(&ddsx, &ddsy, &ddw, &ddh);
	ddsx = ddsx;
	ddsy = ddsy;
	ddex = ddsx + ddw;
	ddey = ddsy + ddh;
	scrndxymmtorltv(&ddsx, &ddsy);
	scrndxymmtorltv(&ddex, &ddey);

	dtdsy = ddsy;
	ddsy = ddey;
	ddey = dtdsy;

	ddw = ddex - ddsx;
	ddh = ddey - ddsy;
	dtdsx = ddsx;
	dtdsy = ddsy;

	GetSrcElement(&dsxdpi, &dsydpi, &dsw, &dsh);
	dsth = dsh;

	timex = dsw / ddw;
	timey = dsh / ddh;
	
	dssx = 0.0; dssy = 0.0;
	dsex = dsw; dsey = dsh;

	scrnrltvareaget(&minx, &miny, &maxx, &maxy);

	if(ddsx < minx){
		dssx = (minx-dtdsx) * timex;
		ddsx = minx;
	}
	else{
		dssx = 0.0;
	}
	if(ddsy < miny){
		dssy = (miny-dtdsy) * timey;
		ddsy = miny;
	}
	else{
		dssy = 0.0;
	}

	if(ddex > maxx){
		dsex = (maxx-dtdsx) * timex;
		ddex = maxx;
	}
	else{
		dsex = dsw;
	}
	if(ddey > maxy){
		dsey = (maxy-dtdsy) * timey;
		ddey = maxy;
	}
	else{
		dsey = dsh;
	}
	ddw = ddex - ddsx;
	ddh = ddey - ddsy;
	dsw = dsex - dssx;
	dsh = dsey - dssy;

	ndsx = (int)ddsx; ndsy = (int)ddsy; ndw = (int)ddw; ndh = (int)ddh;
	nssx = (int)dssx; nssy = (int)(dsth - dsey); nsw = (int)dsw; nsh = (int)dsh;
	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if(lpDib == NULL){
		AfxMessageBox("ÇaÇlÇoópÇÃÉÅÉÇÉäÇ™ÇkÇnÇbÇjÇ≈Ç´Ç‹ÇπÇÒ", MB_OK);
		return(FALSE);
	}
	lpInfo = lpDib + sizeof(BITMAPFILEHEADER);
	lpBits = (LPSTR)(lpDib + m_lImageTopSeek);
	ret = StretchDIBits(pcDC->m_hDC,
				ndsx, ndsy, ndw, ndh, 
				nssx, nssy, nsw, nsh, 
				lpBits, (LPBITMAPINFO)lpInfo, DIB_RGB_COLORS, mode);
	if(ret == 0){
		GlobalUnlock(m_hDib);
		return(FALSE);
	}
	GlobalUnlock(m_hDib);
	return(TRUE);
}

int CDibCData::PaintBkStretchDIBits(CDC *pcDC, int mode)
{
	DBL		ddsx, ddsy, ddex, ddey;
	DBL		ddw, ddh;
	DBL		dssx, dssy, dsex, dsey;
	DBL		dsw, dsh, dsth, dsxdpi, dsydpi;
	DBL		dtdsx, dtdsy;
	DBL		timex, timey;
	DBL		minx, miny, maxx, maxy;
	int		ndsx, ndsy, ndw, ndh;
	int		nssx, nssy, nsw, nsh;
	int		ret;
	LPBYTE	lpDib;
	LPBYTE 	lpInfo;
	LPSTR	lpBits;
	CRect	rect;

	GetDstElement(&ddsx, &ddsy, &ddw, &ddh);
	ddsx = ddsx;
	ddsy = ddsy;
	ddex = ddsx + ddw;
	ddey = ddsy + ddh;
	scrndxymmtorltv(&ddsx, &ddsy);
	scrndxymmtorltv(&ddex, &ddey);

	dtdsy = ddsy;
	ddsy = ddey;
	ddey = dtdsy;

	ddw = ddex - ddsx;
	ddh = ddey - ddsy;
	dtdsx = ddsx;
	dtdsy = ddsy;

	GetSrcElement(&dsxdpi, &dsydpi, &dsw, &dsh);
	dsw = m_dBkSrcWidth;
	dsh = m_dBkSrcHight;
	dsth = dsh;

	timex = dsw / ddw;
	timey = dsh / ddh;
	
	dssx = 0.0; dssy = 0.0;
	dsex = dsw; dsey = dsh;

	scrnrltvareaget(&minx, &miny, &maxx, &maxy);

	if(ddsx < minx){
		dssx = (minx-dtdsx) * timex;
		ddsx = minx;
	}
	else{
		dssx = 0.0;
	}
	if(ddsy < miny){
		dssy = (miny-dtdsy) * timey;
		ddsy = miny;
	}
	else{
		dssy = 0.0;
	}

	if(ddex > maxx){
		dsex = (maxx-dtdsx) * timex;
		ddex = maxx;
	}
	else{
		dsex = dsw;
	}
	if(ddey > maxy){
		dsey = (maxy-dtdsy) * timey;
		ddey = maxy;
	}
	else{
		dsey = dsh;
	}
	ddw = ddex - ddsx;
	ddh = ddey - ddsy;
	dsw = dsex - dssx;
	dsh = dsey - dssy;

	ndsx = (int)ddsx; ndsy = (int)ddsy; ndw = (int)ddw; ndh = (int)ddh;
	nssx = (int)dssx; nssy = (int)(dsth - dsey); nsw = (int)dsw; nsh = (int)dsh;
	lpDib = (LPBYTE)GlobalLock(m_hBkDib);
	if(lpDib == NULL){
		AfxMessageBox("ÇaÇlÇoópÇÃÉÅÉÇÉäÇ™ÇkÇnÇbÇjÇ≈Ç´Ç‹ÇπÇÒ", MB_OK);
		return(FALSE);
	}
	lpInfo = lpDib + sizeof(BITMAPFILEHEADER);
	lpBits = (LPSTR)(lpDib + m_lImageTopSeek);
	ret = StretchDIBits(pcDC->m_hDC,
				ndsx, ndsy, ndw, ndh, 
				nssx, nssy, nsw, nsh, 
				lpBits, (LPBITMAPINFO)lpInfo, DIB_RGB_COLORS, mode);
	if(ret == 0){
		GlobalUnlock(m_hDib);
		return(FALSE);
	}
	GlobalUnlock(m_hDib);
	return(TRUE);
}

void CDibCData::PaintDibDirect(CDC *pcDC)
{
	LPBYTE	lpDib, lpOutDib; 
	LPBYTE 	lpInfo;
	LPBYTE	lpSrcTop;
	LPBYTE	lpDstTop;
	HDC		hMemDC;
	HBITMAP	hBitmap, hOldBitmap;
	DBL		ddw, ddh;
	DBL		dtopx, dtopy;
	int		ndw, ndh;
	int		nsw, nsh;
	int		xdivcnt, ydivcnt, xdivrst, ydivrst;
	int		x, y;
	int		nsx, nsy;
	DBL		ddx, ddy;
	int		nminx, nminy, nmaxx, nmaxy;
	DBL		minx, miny, maxx, maxy;
	int		ndx, ndy;
	int		outdibcsize;
	int		ret;
	int		rastercaps;
	int		mode;
	RGBQUAD FAR *lpRGBQuad;

	rastercaps = GetDeviceCaps(pcDC->m_hDC, RASTERCAPS);
	lpDib = (LPBYTE)GlobalLock(m_hDib);
	lpOutDib = (LPBYTE)GlobalLock(m_hOutDib);
	hMemDC = CreateCompatibleDC(pcDC->m_hDC);

	memcpy(lpOutDib, lpDib, m_lImageTopSeek);

	lpInfo = lpOutDib + sizeof(BITMAPFILEHEADER);
	lpSrcTop = lpDib + m_lImageTopSeek;
	lpDstTop = lpOutDib + m_lImageTopSeek;
	lpRGBQuad = (RGBQUAD*)(lpOutDib + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	lpRGBQuad = (RGBQUAD*)(lpOutDib + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD));
	ddw = m_dDstWidth;
	ddh = m_dDstHight;
	scrndlenmmtorltv(&ddw);
	scrndlenmmtorltv(&ddh);
	ndw = (int)ddw; ndh = (int)ddh;
	dtopx = m_dDstSx; dtopy = m_dDstSy;
	scrndxymmtorltv(&dtopx, &dtopy);
	dtopy = dtopy - ddh;

	nsw = (int)m_dSrcWidth; nsh = (int)m_dSrcHight;
	m_dOutTrnsx = ddw / m_dSrcWidth;
	m_dOutTrnsy = ddh / m_dSrcHight;

	xdivcnt = ndw / OUTXSIZE;
	xdivrst = ndw % OUTXSIZE;
	ydivcnt = ndh / OUTXSIZE;
	ydivrst = ndh % OUTXSIZE;
	
	mode = senvcheckdrawmode(); 
	if(mode != PRINTVIEW
	&& mode != DIRCTVIEW
	&& mode != METAFVIEW){
		scrngetwinarea(&nminx, &nminy, &nmaxx, &nmaxy);
		minx = nminx; miny = nminy;
		maxx = nmaxx; maxy = nmaxy;
	}
	else{
		scrnrltvareaget(&minx, &miny, &maxx, &maxy);
	}
	// î˜í≤êÆÇÃÇΩÇﬂï\é¶ÉGÉäÉAÇägëÂ
	minx -= 16; miny -= 16; maxx += 16; maxy += 16;
	for(y = 0; y < ydivcnt; y++){
		m_nOH = OUTYSIZE;
		ddy = y * OUTYSIZE + dtopy;
		if((ddy+m_nOH) < miny || maxy < ddy){
			continue;
		}
		ndy = (int)ddy;
		nsy = (int)(y * OUTYSIZE / m_dOutTrnsy);

		m_nOW = OUTXSIZE;
		m_nOutXByte = (((m_nOW*m_nBitCount + 7) / 8) + 3L) / 4L * 4L;
		outdibcsize = m_lImageTopSeek + m_nOutXByte * m_nOH;
		for(x = 0; x < xdivcnt; x++){
			ddx = x * OUTXSIZE + dtopx;
			if((ddx+m_nOW) < minx || maxx < ddx){
				continue;
			}
			ndx = (int)ddx;
			nsx = (int)(x * OUTXSIZE / m_dOutTrnsx);
			OutBlockCreate(lpSrcTop, nsx, nsy, lpDstTop);

			((LPBITMAPINFOHEADER)lpInfo)->biWidth = m_nOW;
			((LPBITMAPINFOHEADER)lpInfo)->biHeight = m_nOH;
			((LPBITMAPINFOHEADER)lpInfo)->biSizeImage = outdibcsize;

			if(rastercaps & RC_STRETCHDIB){
				ret = StretchDIBits(pcDC->m_hDC, ndx, ndy, m_nOW, m_nOH,
						0, 0, m_nOW, m_nOH, lpDstTop, (LPBITMAPINFO)lpInfo, DIB_RGB_COLORS, m_lStretchMode);
			}
			else{
				hBitmap = CreateDIBitmap(pcDC->m_hDC, (LPBITMAPINFOHEADER)lpInfo,
						 CBM_INIT, lpDstTop, (LPBITMAPINFO)lpInfo, 0);
				hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
				ret = BitBlt(pcDC->m_hDC, ndx, ndy, m_nOW, m_nOH, hMemDC, 0, 0, m_lStretchMode);
				SelectObject(hMemDC, hOldBitmap);
				DeleteObject(hBitmap);
			}
		}

		m_nOW = xdivrst;
		m_nOutXByte = (((m_nOW*m_nBitCount + 7) / 8) + 3L) / 4L * 4L;
		outdibcsize = m_lImageTopSeek + m_nOutXByte * m_nOH;
		if(xdivrst != 0){
			ddx = x * OUTXSIZE + dtopx;
			if((ddx+m_nOW) < minx || maxx < ddx){
				continue;
			}
			ndx = (int)ddx;
			nsx = (int)(x * OUTXSIZE / m_dOutTrnsx);
			OutBlockCreate(lpSrcTop, nsx, nsy, lpDstTop);

			((LPBITMAPINFOHEADER)lpInfo)->biWidth = m_nOW;
			((LPBITMAPINFOHEADER)lpInfo)->biHeight = m_nOH;
			((LPBITMAPINFOHEADER)lpInfo)->biSizeImage = outdibcsize;

			if(rastercaps & RC_STRETCHDIB){
				ret = StretchDIBits(pcDC->m_hDC, ndx, ndy, m_nOW, m_nOH,
					0, 0, m_nOW, m_nOH, lpDstTop, (LPBITMAPINFO)lpInfo, DIB_RGB_COLORS, m_lStretchMode);
			}
			else{
				hBitmap = CreateDIBitmap(pcDC->m_hDC, (LPBITMAPINFOHEADER)lpInfo,
						 CBM_INIT, lpDstTop, (LPBITMAPINFO)lpInfo, 0);
				hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
				ret = BitBlt(pcDC->m_hDC, ndx, ndy, m_nOW, m_nOH, hMemDC, 0, 0, m_lStretchMode);
				SelectObject(hMemDC, hOldBitmap);
				DeleteObject(hBitmap);
			}
		}
	}
	if(ydivrst != 0){
		m_nOH = ydivrst;
		ddy = y * OUTYSIZE + dtopy;
		if((ddy+m_nOH) < miny || maxy < ddy){
			goto End;
		}
		ndy = (int)ddy;
		nsy = (int)(y * OUTYSIZE / m_dOutTrnsy);

		m_nOW = OUTXSIZE;
		m_nOutXByte = (((m_nOW*m_nBitCount + 7) / 8) + 3L) / 4L * 4L;
		outdibcsize = m_lImageTopSeek + m_nOutXByte * m_nOH;
		for(x = 0; x < xdivcnt; x++){
			ddx = x * OUTXSIZE + dtopx;
			if((ddx+m_nOW) < minx || maxx < ddx){
				continue;
			}
			ndx = (int)ddx;
			nsx = (int)(x * OUTXSIZE / m_dOutTrnsx);
			OutBlockCreate(lpSrcTop, nsx, nsy, lpDstTop);

			((LPBITMAPINFOHEADER)lpInfo)->biWidth = m_nOW;
			((LPBITMAPINFOHEADER)lpInfo)->biHeight = m_nOH;
			((LPBITMAPINFOHEADER)lpInfo)->biSizeImage = outdibcsize;
			if(rastercaps & RC_STRETCHDIB){
				ret = StretchDIBits(pcDC->m_hDC, ndx, ndy, m_nOW, m_nOH,
						0, 0, m_nOW, m_nOH, lpDstTop, (LPBITMAPINFO)lpInfo, DIB_RGB_COLORS, m_lStretchMode);
			}
			else{
				hBitmap = CreateDIBitmap(pcDC->m_hDC, (LPBITMAPINFOHEADER)lpInfo,
							 CBM_INIT, lpDstTop, (LPBITMAPINFO)lpInfo, 0);
				hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
				ret = BitBlt(pcDC->m_hDC, ndx, ndy, m_nOW, m_nOH, hMemDC, 0, 0, m_lStretchMode);
				SelectObject(hMemDC, hOldBitmap);
				DeleteObject(hBitmap);
			}
		}

		m_nOW = xdivrst;
		m_nOutXByte = (((m_nOW*m_nBitCount + 7) / 8) + 3L) / 4L * 4L;
		outdibcsize = m_lImageTopSeek + m_nOutXByte * m_nOH;
		if(xdivrst != 0){
			ddx = x * OUTXSIZE + dtopx;
			if((ddx+m_nOW) < minx || maxx < ddx){
				goto End;
			}
			ndx = (int)ddx;
			nsx = (int)(x * OUTXSIZE / m_dOutTrnsx);
			OutBlockCreate(lpSrcTop, nsx, nsy, lpDstTop);

			((LPBITMAPINFOHEADER)lpInfo)->biWidth = m_nOW;
			((LPBITMAPINFOHEADER)lpInfo)->biHeight = m_nOH;
			((LPBITMAPINFOHEADER)lpInfo)->biSizeImage = outdibcsize;
			if(rastercaps & RC_STRETCHDIB){
				ret = StretchDIBits(pcDC->m_hDC, ndx, ndy, m_nOW, m_nOH,
						0, 0, m_nOW, m_nOH, lpDstTop, (LPBITMAPINFO)lpInfo, DIB_RGB_COLORS, m_lStretchMode);
			}
			else{
				hBitmap = CreateDIBitmap(pcDC->m_hDC, (LPBITMAPINFOHEADER)lpInfo,
							 CBM_INIT, lpDstTop, (LPBITMAPINFO)lpInfo, 0);
				hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
				ret = BitBlt(pcDC->m_hDC, ndx, ndy, m_nOW, m_nOH, hMemDC, 0, 0, m_lStretchMode);
				SelectObject(hMemDC, hOldBitmap);
				DeleteObject(hBitmap);
			}
		}
	}
End:	
	DeleteDC(hMemDC);
	GlobalUnlock(m_hOutDib);
	GlobalUnlock(m_hDib);
}

void CDibCData::OutBlockCreate(LPBYTE lpSrcTop, int nsx, int nsy, LPBYTE lpDstTop)
{
	switch(m_nBitCount){
	case 32:
		OutBlockCreate32(lpSrcTop, nsx, nsy, lpDstTop);
		break;
	case 24:
		OutBlockCreate24(lpSrcTop, nsx, nsy, lpDstTop);
		break;
	case 8:
		OutBlockCreate8(lpSrcTop, nsx, nsy, lpDstTop);
		break;
	case  1:
		OutBlockCreate1(lpSrcTop, nsx, nsy, lpDstTop);
		break;
	}
}

void CDibCData::OutBlockCreate32(LPBYTE lpSrcTop, int nsx, int nsy, LPBYTE lpDstTop)
{
	int		x, y;
	int		xx, yy;
	int		crtx, crty;
	BYTE	r, g, b;
	LPBYTE	lpCrtDst, lpDst, lpSrc;

	crty = -1;
	for(y = 0; y < m_nOH; y++){
		lpDst = lpDstTop + (m_nOH - y - 1) * m_nOutXByte;
		yy = nsy + (long)(y / m_dOutTrnsy);
		if(crty != yy){
			lpSrc = lpSrcTop + (long)((m_dSrcHight - yy - 1) * m_dXByte);
			crtx = -1;
			for(x = 0; x < m_nOW; x++){
				xx = nsx + (long)(x / m_dOutTrnsx);
				if(xx != crtx){
					r = *(lpSrc+xx*4);
					g = *(lpSrc+xx*4+1);
					b = *(lpSrc+xx*4+2);
					crtx = xx;
				}
				lpDst[x*4] = r;
				lpDst[x*4+1] = g;
				lpDst[x*4+2] = b;
			}
			lpCrtDst = lpDst;
		}
		if(lpCrtDst != lpDst){
			memcpy(lpDst, lpCrtDst, m_nOutXByte);
		}
	}
}
void CDibCData::OutBlockCreate24(LPBYTE lpSrcTop, int nsx, int nsy, LPBYTE lpDstTop)
{
	int		x, y;
	int		xx, yy;
	int		crtx, crty;
	BYTE	r, g, b;
	LPBYTE	lpCrtDst, lpDst, lpSrc;

	crty = -1;
	for(y = 0; y < m_nOH; y++){
		lpDst = lpDstTop + (m_nOH - y - 1) * m_nOutXByte;
		yy = nsy + (long)(y / m_dOutTrnsy);
		if(crty != yy){
			lpSrc = lpSrcTop + (long)((m_dSrcHight - yy - 1) * m_dXByte);
			crtx = -1;
			for(x = 0; x < m_nOW; x++){
				xx = nsx + (long)(x / m_dOutTrnsx);
				if(xx != crtx){
					r = *(lpSrc+xx*3);
					g = *(lpSrc+xx*3+1);
					b = *(lpSrc+xx*3+2);
					crtx = xx;
				}
				lpDst[x*3] = r;
				lpDst[x*3+1] = g;
				lpDst[x*3+2] = b;
			}
			lpCrtDst = lpDst;
		}
		if(lpCrtDst != lpDst){
			memcpy(lpDst, lpCrtDst, m_nOutXByte);
		}
	}
}
void CDibCData::OutBlockCreate8(LPBYTE lpSrcTop, int nsx, int nsy, LPBYTE lpDstTop)
{
	int		x, y;
	int		xx, yy;
	int		crtx, crty;
	BYTE	img;
	LPBYTE	lpCrtDst, lpDst, lpSrc;

	crty = -1;
	for(y = 0; y < m_nOH; y++){
		lpDst = lpDstTop + (m_nOH - y - 1) * m_nOutXByte;
		yy = nsy + (long)(y / m_dOutTrnsy);
		if(crty != yy){
			lpSrc = lpSrcTop + (long)((m_dSrcHight - yy - 1) * m_dXByte);
			crtx = -1;
			for(x = 0; x < m_nOW; x++){
				xx = nsx + (long)(x / m_dOutTrnsx);
				if(xx != crtx){
					img = *(lpSrc+xx);
					crtx = xx;
				}
				lpDst[x] = img;
			}
			lpCrtDst = lpDst;
		}
		if(lpCrtDst != lpDst){
			memcpy(lpDst, lpCrtDst, m_nOutXByte);
		}
	}
}

void CDibCData::OutBlockCreate1(LPBYTE lpSrcTop, int nsx, int nsy, LPBYTE lpDstTop)
{
	int		x, y;
	int		xx, yy;
	int		crtx, crty;
	LPBYTE	lpCrtDst, lpDst, lpSrc;
	int		byte, sift, mask;
	int		flag;

	crty = -1;
	for(y = 0; y < m_nOH; y++){
		lpDst = lpDstTop + (m_nOH - y - 1) * m_nOutXByte;
		yy = nsy + (long)(y / m_dOutTrnsy);
		if(crty != yy){
			lpSrc = lpSrcTop + (long)((m_dSrcHight - yy - 1) * m_dXByte);
			crtx = -1;
			for(x = 0; x < m_nOW; x++){
				if((x % 8) == 0){
					byte = x / 8;
					lpDst[byte] = 0;
				}
				xx = nsx + (long)(x / m_dOutTrnsx);
				if(xx != crtx){
					byte = xx / 8;
					sift = xx % 8;
					mask = 0x0080 >> sift;
					if((*(lpSrc+byte)) & mask){
						flag = ON;
					}
					else{
						flag = OFF;
					}
					crtx = xx;
				}
				byte = x / 8;
				sift = x % 8;
				if(flag == ON){
					mask = 0x0080 >> sift;
					lpDst[byte] |= mask;
				}
			}
			lpCrtDst = lpDst;
		}
		if(lpCrtDst != lpDst){
			memcpy(lpDst, lpCrtDst, m_nOutXByte);
		}
	}
}

void CDibCData::GetDMiniMax(DBL * minix, DBL * miniy, DBL * maxx, DBL * maxy)
{
	*minix = m_dMinix;
	*miniy = m_dMiniy;
	*maxx = m_dMaxx;
	*maxy = m_dMaxy;
}

void CDibCData::RenewMiniMax()
{
	DBL		sx, sy, ex, ey;

	sx = m_dDstSx;
	sy = m_dDstSy;
	ex = sx + m_dDstWidth;
	ey = sy + m_dDstHight;
	m_dMinix = sx;
	m_dMiniy = sy;
	m_dMaxx = ex;
	m_dMaxy = ey;

	RenewMiniMaxPoint(sx, sy);
	RenewMiniMaxPoint(ex, ey);
}

CData* CDibCData::SrchData(DBL xx, DBL yy, DBL * retlen)
{
	int		mode;

	// 2001.10.01 âÊëúÇ™â∫Ç…Ç†ÇËéûê¸ÉÇÅ[ÉhÇ≈ê¸Ç™éwíËÇ≈Ç´Ç»Ç¢
	mode = senvcheckdrawmode(); 
	if(mode == PREVIEW){
		SrchDataLimitArea(xx, yy, retlen, 0);
		return((CData*)this);
	}
	else{
		mode = senvcheckoptslct(); 
		if(mode == 1){
			SrchDataLimitArea(xx, yy, retlen, HNEARLIMIT/2);
			return((CData*)this);
		}
		else{
			SrchDataLimitArea(xx, yy, retlen, 0);
			return((CData*)this);
		}
	}
	return((CData*)this);
}

CData* CDibCData::SrchDataLimitArea(DBL xx, DBL yy, DBL * retlen, DBL rltvlimit)
{
	DBL		minix, miniy, maxx, maxy;

	*retlen = INTMAX;
	GetDMiniMax(&minix, &miniy, &maxx, &maxy);
	if(minix <= xx && xx <= maxx
	&& miniy <= yy && yy <= maxy){
		scrndlenrltvtomm(&rltvlimit);
		*retlen = rltvlimit;
		return((CData*)this);
	}
	return((CData*)this);
}

CData* CDibCData::SrchDataBoxs(DBL xx, DBL yy, DBL * retlen)
{
	DBL		minix, miniy, maxx, maxy;

	GetDMiniMax(&minix, &miniy, &maxx, &maxy);
	*retlen = GetPreLengthBoxs(xx, yy, minix, miniy, maxx, maxy);
	return((CData*)this);
}

COLORREF CDibCData::GetPicupClr(DBL dx, DBL dy)
{
	DBL		trnsx, trnsy;
	int		nsx, nsy;
	COLORREF	clr;

	dx = dx - m_dDstSx;
	dy = dy - m_dDstSy;
	trnsx = m_dSrcWidth / m_dDstWidth;
	trnsy = m_dSrcHight / m_dDstHight;
	nsx = (int)(dx * trnsx);
	nsy = (int)(m_dSrcHight - dy * trnsy);
	clr = GetSrcXYClr(nsx, nsy);
	return(clr);
}

COLORREF CDibCData::GetNearClr(DBL dsx, DBL dsy)
{
	COLORREF	clr;
	LPBYTE		lpDib;
	LPRGBQUAD	lpRGBQuad;
	LPBYTE		lpLineTop;
	int			r, g, b;

	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if(lpDib == NULL){
		AfxMessageBox("ÇaÇlÇoópÇÃÉÅÉÇÉäÇ™ÇkÇnÇbÇjÇ≈Ç´Ç‹ÇπÇÒ", MB_OK);
		return(RGB(0,0,0));
	}

	lpRGBQuad = (RGBQUAD FAR *)(lpDib + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	dsy = m_dSrcHight - dsy;
	BmpAreaLegal(&dsx, &dsy);
	lpLineTop = lpDib + (long)(dsy * m_dXByte) + m_lImageTopSeek;
	diblibGetDotRGB(m_nBitCount, lpRGBQuad, lpLineTop, (int)dsx, &r, &g, &b);
	clr = RGB(r, g, b);
	GlobalUnlock(m_hDib);
	return(clr);
}

COLORREF CDibCData::GetSrcXYClr(DBL dsx, DBL dsy)
{
	COLORREF	clr;
	LPBYTE		lpDib;
	LPRGBQUAD	lpRGBQuad;
	LPBYTE		lpLineTop;
	int			r, g, b;

	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if(lpDib == NULL){
		AfxMessageBox("ÇaÇlÇoópÇÃÉÅÉÇÉäÇ™ÇkÇnÇbÇjÇ≈Ç´Ç‹ÇπÇÒ", MB_OK);
		return(RGB(0,0,0));
	}

	lpRGBQuad = (RGBQUAD FAR *)(lpDib + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	dsy = m_dSrcHight - dsy;
	BmpAreaLegal(&dsx, &dsy);
	lpLineTop = lpDib + (long)(dsy * m_dXByte) + m_lImageTopSeek;
	diblibGetDotRGB(m_nBitCount, lpRGBQuad, lpLineTop, (int)dsx, &r, &g, &b);
	clr = RGB(r, g, b);
	GlobalUnlock(m_hDib);
	return(clr);
}

void CDibCData::BmpAreaLegal(LPDBL dsx, LPDBL dsy)
{
	DBL		dx, dy;

	dx = *dsx;
	dy = *dsy;
	if(dy < 0){
		dy = 0;
	}
	if(m_dSrcHight <= dy){
		dy = m_dSrcHight-1;
	}

	if(dx < 0){
		dx = 0;
	}
	if(m_dSrcWidth <= dx){
		dx = m_dSrcWidth-1;
	}
	*dsx = dx;
	*dsy = dy;
}

void CDibCData::MoveTrns(DBL movex, DBL movey)
{
	m_dDstSx += movex;
	m_dDstSy += movey;
	RenewMiniMax();
}

void CDibCData::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	DBL		sx, sy, ex, ey;
	DBL		tx, ty;

	sx = m_dDstSx;
	sy = m_dDstSy;
	ex = sx + m_dDstWidth;
	ey = sy + m_dDstHight;
	sx = (sx - cntrx) * timex + cntrx;
	sy = (sy - cntry) * timey + cntry;
	ex = (ex - cntrx) * timex + cntrx;
	ey = (ey - cntry) * timey + cntry;
	if(ex < sx){
		tx = sx;
		sx = ex;
		ex = tx;
	}

	if(ey < sy){
		ty = sy;
		sy = ey;
		ey = ty;
	}

	m_dDstSx = sx;
	m_dDstSy = sy;

	m_dDstWidth = ex - sx;
	m_dDstHight = ey - sy;

	RenewMiniMax();
}

void CDibCData::SaveEpsFile()
{

}

void CDibCData::FileSave()
{
	CFile	cpFile;
	LPBYTE	lpDib;

	if((cpFile.Open(m_szBmpFname, CFile::modeWrite)) == FALSE){
		AfxMessageBox("ÇaÇlÇoÉtÉ@ÉCÉãÇ™äJÇØÇ‹ÇπÇÒ", MB_OK);
		return;
	}
	lpDib = (LPBYTE)GlobalLock(m_hDib);
	cpFile.WriteHuge(lpDib, m_lFileSize);
	GlobalUnlock(m_hDib);
	cpFile.Close();
}

void CDibCData::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
}

void CDibCData::RoundDib(int mode)
{
}

CData* CDibCData::CopyData(DBL movex, DBL movey)
{
	CDibCData*	pcnewData;
	char		newfname[FNAMEMAX];

	pcnewData = (CDibCData*)NewDataCreate(DIBCDATAID);
	diblibcreatebmpfile(newfname);
	CopyFile(m_szBmpFname, newfname, FALSE);
	pcnewData->SetBmpFname(newfname);
	pcnewData->DibLoad();
	pcnewData->m_dDstHight = m_dDstHight;
	pcnewData->m_dDstWidth = m_dDstWidth;
	pcnewData->m_dDstSx = m_dDstSx + movex;
	pcnewData->m_dDstSy = m_dDstSy + movey;
	pcnewData->RenewMiniMax();
	// ÉoÉbÉNÉrÉbÉgÉ}ÉbÉvÇÃÉRÉsÅ[Ç‡ïKóv
	return((CData*)pcnewData);
}

void CDibCData::ChengeBaseClr(int idx, COLORREF clr)
{
	
}

void CDibCData::SetClrTable(COLORREF clr)
{
	int		r, g, b;
	int		plt;
	DBL		divr, divg, divb;
	int		setr, setg, setb;
	
	r = GetRValue(clr);
	g = GetGValue(clr);
	b = GetBValue(clr);
	divr = (255 - (DBL)r) / 255.0;
	divg = (255 - (DBL)g) / 255.0;
	divb = (255 - (DBL)b) / 255.0;
	for(plt = 0; plt < 255; plt++){
		setr = r + (int)(divr * plt);
		setg = g + (int)(divg * plt);
		setb = b + (int)(divb * plt);
		if(255 < setr){
			setr = 255;
		}
		SetRGBQuadData(plt, setr, setg, setb);
	}
	setr = 255; setg = 255; setb = 255;
	SetRGBQuadData(255, setr, setg, setb);
}

void CDibCData::RasterRevace()
{
	LPBYTE	lpDib;
	LPBYTE	lpbits;
	LPBYTE	lpoffset;
	int		x, y;

	lpDib = (LPBYTE)GlobalLock(m_hDib);
	lpbits = lpDib + m_lImageTopSeek;
	for(y = 0; y < m_dSrcHight; y++){
		lpoffset = lpbits + (ULONG)(y * m_dXByte);
		for(x = 0; x < m_dXByte; x++){
			(*lpoffset) = ~(*lpoffset);
			lpoffset++;
		}
	}
	GlobalUnlock(m_hDib);
}

void CDibCData::DelArea(DBL sx, DBL sy, DBL ex, DBL ey)
{
	DBL		xy;
	DBL		trnsx, trnsy;
	int		nsx, nsy, nex, ney;
	LPBYTE	lpDib;
	LPBYTE	lpbits;
	LPBYTE	lpoffset;
	int		x, y;
	int		byte, mask;

	if(sx > ex){
		SWAP(sx, ex, xy);
	}
	if(sy > ey){
		SWAP(sy, ey, xy);
	}
	trnsx = m_dSrcWidth / m_dDstWidth;
	trnsy = m_dSrcHight / m_dDstHight;
	sx = sx - m_dDstSx;
	sy = sy - m_dDstSy;
	ex = ex - m_dDstSx;
	ey = ey - m_dDstSy;
	nsx = (int)(sx * trnsx);
	nsy = (int)(sy * trnsy);
	nex = (int)(ex * trnsx);
	ney = (int)(ey * trnsy);
	if(nsx < 0){
		nsx = 0;
	}
	if(nex > (int)(m_dSrcWidth)){
		nex = (int)(m_dSrcWidth-1);
	}
	if(nsy < 0){
		nsy = 0;
	}
	if(ney > (int)(m_dSrcHight)){
		ney = (int)(m_dSrcHight-1);
	}

	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if(lpDib == NULL){
		return;
	}
	lpbits = lpDib + m_lImageTopSeek;
	for(y = nsy; y < ney; y++){
		lpoffset = lpbits + (ULONG)(y * m_dXByte);
		byte = nsx / 8;
		mask = 0x0080 >> (nsx % 8);
		for(x = nsx; x < nex; x++){
			lpoffset[byte] |= mask;
			mask >>= 1;
			if(mask == 0){
				byte++;
				mask = 0x0080;
			}
		}
	}
	GlobalUnlock(m_hDib);
}

void CDibCData::SetArea(DBL sx, DBL sy, DBL ex, DBL ey)
{
	DBL		xy;
	DBL		trnsx, trnsy;
	int		nsx, nsy, nex, ney;
	LPBYTE	lpDib;
	LPBYTE	lpbits;
	LPBYTE	lpoffset;
	int		x, y;
	int		byte, mask;

	if(sx > ex){
		SWAP(sx, ex, xy);
	}
	if(sy > ey){
		SWAP(sy, ey, xy);
	}
	trnsx = m_dSrcWidth / m_dDstWidth;
	trnsy = m_dSrcHight / m_dDstHight;
	sx = sx - m_dDstSx;
	sy = sy - m_dDstSy;
	ex = ex - m_dDstSx;
	ey = ey - m_dDstSy;
	nsx = (int)(sx * trnsx);
	nsy = (int)(sy * trnsy);
	nex = (int)(ex * trnsx);
	ney = (int)(ey * trnsy);
	if(nsx < 0){
		nsx = 0;
	}
	if(nex > (int)(m_dSrcWidth)){
		nex = (int)(m_dSrcWidth-1);
	}
	if(nsy < 0){
		nsy = 0;
	}
	if(ney > (int)(m_dSrcHight)){
		ney = (int)(m_dSrcHight-1);
	}

	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if(lpDib == NULL){
		return;
	}
	lpbits = lpDib + m_lImageTopSeek;
	for(y = nsy; y < ney; y++){
		lpoffset = lpbits + (ULONG)(y * m_dXByte);
		byte = nsx / 8;
		mask = 0x0080 >> (nsx % 8);
		for(x = nsx; x < nex; x++){
			lpoffset[byte] &= ~mask;
			mask >>= 1;
			if(mask == 0){
				byte++;
				mask = 0x0080;
			}
		}
	}
	GlobalUnlock(m_hDib);
}
