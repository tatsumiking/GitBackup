
/////////////////////////////////////////////////////////////////////////////
// CBmpFile

#include "stdafx.h"
#include "stdio.h"
#include "math.h"

#include "cmd_base.h"
#include "bmpfile.h"

#define DRAWBLOCK	512

extern "C" {
	void senvgettmppath(LPSTR path);
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
	DBL scrnsetxytime(DBL timex, DBL timey);
	void scrngetxytime(LPDBL timex, LPDBL timey);
	void scrnsetrltvelement();
	void scrnsetrltvtop(DBL xTop, DBL yTop);
	void scrngetrltvtop(LPDBL xTop, LPDBL yTop);
	void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	void scrnsetdisprltvmove(DBL xmove, DBL ymove);
	void scrngetdisprltvmove(LPDBL xmove, LPDBL ymove);
	int senvsetdrawmode(int mode);
}

CBmpFile::CBmpFile(CScrollView* pcview)
:CCmdBase(pcview)
{
}

CBmpFile::~CBmpFile()
{
}

int CBmpFile::CreateBmpInit(int xdot, int ydot)
{
	DBL		xmm, ymm;
	DBL		bmpx, bmpy;

	m_nXDotMax = xdot;
	m_nYDotMax = ydot;
	bmpx = m_nXDotMax;
	bmpy = m_nYDotMax;
	scrngetmmsize(&xmm, &ymm);
	m_dXTime = (bmpx / xmm);
	m_dYTime = (bmpy / ymm);
	if(m_dXTime < m_dYTime){
		m_dYTime = m_dXTime;
	}
	else{
		m_dXTime = m_dYTime;
	}

	int	ret = MemDCInit(m_nXDotMax, m_nYDotMax);
	return(ret);
}


int CBmpFile::CreateBmpInitHalf(int xdot, int ydot)
{
	DBL		xmm, ymm;
	DBL		bmpx, bmpy;

	m_nXDotMax = xdot;
	m_nYDotMax = ydot;
	bmpx = m_nXDotMax;
	bmpy = m_nYDotMax;
	scrngetmmsize(&xmm, &ymm);
	m_dXTime = (bmpx / xmm);
	m_dYTime = (bmpy / ymm);
	if(m_dXTime < m_dYTime){
		m_dYTime = m_dXTime;
	}
	else{
		m_dXTime = m_dYTime;
	}

	int	ret = MemDCInit(m_nXDotMax/2, m_nYDotMax);
	return(ret);
}

void CBmpFile::CreateBmpEnd()
{
	MemDCEnd();
}

void CBmpFile::GetMoveXY(int *movex, int *movey)
{
	DBL		xmm, ymm;

	scrngetmmsize(&xmm, &ymm);
	*movex = (int)((m_nXDotMax - xmm * m_dXTime) / 2);
	*movey = (int)((m_nYDotMax - ymm * m_dYTime) / 2);
}

void CBmpFile::CreateBmpImage(int movex, int movey, COLORREF clr, LPSTR fname)
{
	DBL		sizex, sizey;
	int		bmode;
	DBL		btime;
	DBL		bmvx, bmvy;

	sizex = m_nXDotMax;
	sizey = m_nYDotMax;
	scrngetdisprltvmove(&bmvx, &bmvy);
	bmode = senvsetdrawmode(DIRCTVIEW);
	btime = scrnsetxytime(1.0 / m_dXTime, 1.0 / m_dYTime);
	scrnsetrltvelement();
	scrnsetrltvtop(0, 0);
	scrnrltvareaset(0.0, 0.0, sizex, sizey);
	scrnsetdisprltvmove((DBL)movex, (DBL)movey);

	m_cdcMemCDC.BitBlt(0, 0, (int)sizex, (int)sizey, NULL, 0, 0, WHITENESS);
	if(clr != 0xffffffff){
		m_cdcMemCDC.FillSolidRect(0, 0, (int)sizex, (int)sizey, clr);
	}
	((CTonView*)m_pcView)->DirectDraw(&m_cdcMemCDC);

	scrnsetxytime(btime, btime);
	senvsetdrawmode(bmode);
	scrnsetrltvelement();
	scrnsetrltvtop(0.0, 0.0);
	scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
	scrnsetdisprltvmove(bmvx, bmvy);

	BmpFileOut(fname, m_nXDotMax, m_nYDotMax);
}

void CBmpFile::CreateBmpImageHalf(int movex, int movey, COLORREF clr, LPSTR fname)
{
	DBL		sizex, sizey;
	int		bmode;
	DBL		btime;
	DBL		bmvx, bmvy;

	sizex = m_nXDotMax;
	sizey = m_nYDotMax;
	scrngetdisprltvmove(&bmvx, &bmvy);
	bmode = senvsetdrawmode(DIRCTVIEW);
	btime = scrnsetxytime(1.0 / m_dXTime, 1.0 / m_dYTime);
	scrnsetrltvelement();
	scrnsetrltvtop(0, 0);
	scrnrltvareaset(0.0, 0.0, sizex, sizey);
	scrnsetdisprltvmove((DBL)movex, (DBL)movey);

	m_cdcMemCDC.BitBlt(0, 0, (int)sizex, (int)sizey, NULL, 0, 0, WHITENESS);
	if(clr != 0xffffffff){
		m_cdcMemCDC.FillSolidRect(0, 0, (int)sizex, (int)sizey, clr);
	}
	((CTonView*)m_pcView)->DirectDraw(&m_cdcMemCDC);

	scrnsetxytime(btime, btime);
	senvsetdrawmode(bmode);
	scrnsetrltvelement();
	scrnsetrltvtop(0.0, 0.0);
	scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
	scrnsetdisprltvmove(bmvx, bmvy);

	BmpFileOutHalf(fname, m_nXDotMax, m_nYDotMax);
}

void CBmpFile::CreateBmpImage(LPSTR fname)
{
	AreaOnDraw(0, 0, m_nXDotMax, m_nYDotMax);

	BmpFileOut(fname, m_nXDotMax, m_nYDotMax);
}

void CBmpFile::CreateBmpImageRv(LPSTR fname)
{
	AreaOnDraw(0, 0, m_nXDotMax, m_nYDotMax);

	BmpFileOutRv(fname, m_nXDotMax, m_nYDotMax);
}

void CBmpFile::CreateBmpImageDivRv(LPSTR fname)
{
	char	tmppath[FNAMEMAX];
	char	tmpfile1[FNAMEMAX];
	char	tmpfile2[FNAMEMAX];

	senvgettmppath(tmppath);
	sprintf(tmpfile1, "%s\\temp1.bmp", tmppath);
	sprintf(tmpfile2, "%s\\temp2.bmp", tmppath);

	AreaOnDraw1(0, 0, m_nXDotMax, m_nYDotMax);
	BmpFileOutRv(tmpfile1, m_nXDotMax/2, m_nYDotMax);
	BmpFileOutRv1(fname, m_nXDotMax, m_nYDotMax);
	AreaOnDraw2(0, 0, m_nXDotMax, m_nYDotMax);
	BmpFileOutRv(tmpfile2, m_nXDotMax/2, m_nYDotMax);
	BmpFileOutRv2(fname, m_nXDotMax, m_nYDotMax);
}

void CBmpFile::CreateBmpImageDiv(LPSTR fname)
{
	char	tmppath[FNAMEMAX];
	char	tmpfile1[FNAMEMAX];
	char	tmpfile2[FNAMEMAX];

	senvgettmppath(tmppath);
	sprintf(tmpfile1, "%s\\temp1.bmp", tmppath);
	sprintf(tmpfile2, "%s\\temp2.bmp", tmppath);

	AreaOnDraw1(0, 0, m_nXDotMax, m_nYDotMax);
	BmpFileOut(tmpfile1, m_nXDotMax/2, m_nYDotMax);
	BmpFileOut1(fname, m_nXDotMax, m_nYDotMax);
	AreaOnDraw2(0, 0, m_nXDotMax, m_nYDotMax);
	BmpFileOut(tmpfile2, m_nXDotMax/2, m_nYDotMax);
	BmpFileOut2(fname, m_nXDotMax, m_nYDotMax);
}

void CBmpFile::CreateBmpOnebitImage(LPSTR fname)
{
	AreaOnDraw(0, 0, m_nXDotMax, m_nYDotMax);

	BmpOnebitFileOut(fname, m_nXDotMax, m_nYDotMax);
}

void CBmpFile::CreateBmpOnebitImageRV(LPSTR fname)
{
	AreaOnDraw(0, 0, m_nXDotMax, m_nYDotMax);

	BmpOnebitFileOutRV(fname, m_nXDotMax, m_nYDotMax);
}

void CBmpFile::AreaOnDraw(int sx, int sy, int ex, int ey)
{
	DBL		sizex, sizey;
	int		bmode;
	DBL		btime;
	DBL		bmvx, bmvy;
	DBL		bsx, bsy;

	sizex = ex - sx;
	sizey = ey - sy;
	scrngetdisprltvmove(&bmvx, &bmvy);
	scrngetrltvtop(&bsx, &bsy);
	bmode = senvsetdrawmode(DIRCTVIEW);
	btime = scrnsetxytime(1.0 / m_dXTime, 1.0 / m_dYTime);
	scrnsetrltvelement();
	scrnsetrltvtop(sx, sy);
	scrnrltvareaset(0.0, 0.0, sizex, sizey);
	scrnsetdisprltvmove(0.0, 0.0);

	m_cdcMemCDC.BitBlt(0, 0, (int)sizex, (int)sizey, NULL, 0, 0, WHITENESS);
	((CTonView*)m_pcView)->DirectDraw(&m_cdcMemCDC);

#ifdef _DEBUG
	BeginDraw();
	DrawModeSET();
	m_pcDC->BitBlt(0, 0, (int)sizex, (int)sizey, &m_cdcMemCDC, 0, 0, SRCCOPY); 
	EndDraw();
	//char msgbuf[RECMAX];
	//sprintf(msgbuf, "確認　sx %4.1lf sy %4.1lf ex %4.1lf ey %4.1lf", sx, sy, ex, ey); 
	//AfxMessageBox(msgbuf, MB_OK);
#endif

	scrnsetxytime(btime, btime);
	senvsetdrawmode(bmode);
	scrnsetrltvelement();
	scrnsetrltvtop(0.0, 0.0);
	scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
	scrnsetrltvtop(bsx, bsy);
	scrnsetdisprltvmove(bmvx, bmvy);
}

void CBmpFile::AreaOnDraw1(int sx, int sy, int ex, int ey)
{
	DBL		sizex, sizey;
	int		bmode;
	DBL		btime;
	DBL		bmvx, bmvy;
	DBL		bsx, bsy;

	sizex = ex - sx;
	sizey = ey - sy;
	scrngetdisprltvmove(&bmvx, &bmvy);
	scrngetrltvtop(&bsx, &bsy);
	bmode = senvsetdrawmode(DIRCTVIEW);
	btime = scrnsetxytime(1.0 / m_dXTime, 1.0 / m_dYTime);
	scrnsetrltvelement();
	scrnsetrltvtop(sx, sy);
	scrnrltvareaset(0.0, 0.0, sizex, sizey);
	scrnsetdisprltvmove(0.0, 0.0);

	m_cdcMemCDC.BitBlt(0, 0, (int)sizex/2, (int)sizey, NULL, 0, 0, WHITENESS);
	((CTonView*)m_pcView)->DirectDraw(&m_cdcMemCDC);

#ifdef _DEBUG
	BeginDraw();
	DrawModeSET();
	m_pcDC->BitBlt(0, 0, (int)sizex/2, (int)sizey, &m_cdcMemCDC, 0, 0, SRCCOPY); 
	EndDraw();
	//char msgbuf[RECMAX];
	//sprintf(msgbuf, "確認　sx %4.1lf sy %4.1lf ex %4.1lf ey %4.1lf", sx, sy, ex, ey); 
	//AfxMessageBox(msgbuf, MB_OK);
#endif

	scrnsetxytime(btime, btime);
	senvsetdrawmode(bmode);
	scrnsetrltvelement();
	scrnsetrltvtop(0.0, 0.0);
	scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
	scrnsetrltvtop(bsx, bsy);
	scrnsetdisprltvmove(bmvx, bmvy);
}

void CBmpFile::AreaOnDraw2(int sx, int sy, int ex, int ey)
{
	DBL		sizex, sizey;
	int		bmode;
	DBL		btime;
	DBL		bmvx, bmvy;
	DBL		bsx, bsy;

	sizex = ex - sx;
	sizey = ey - sy;
	scrngetdisprltvmove(&bmvx, &bmvy);
	scrngetrltvtop(&bsx, &bsy);
	bmode = senvsetdrawmode(DIRCTVIEW);
	btime = scrnsetxytime(1.0 / m_dXTime, 1.0 / m_dYTime);
	scrnsetrltvelement();
	scrnsetrltvtop(sx, sy);
	scrnrltvareaset(0.0, 0.0, sizex, sizey);
	scrnsetdisprltvmove(-sizex/2, 0.0);

	m_cdcMemCDC.BitBlt(0, 0, (int)sizex/2, (int)sizey, NULL, 0, 0, WHITENESS);
	((CTonView*)m_pcView)->DirectDraw(&m_cdcMemCDC);

#ifdef _DEBUG
	BeginDraw();
	DrawModeSET();
	m_pcDC->BitBlt(0, 0, (int)sizex/2, (int)sizey, &m_cdcMemCDC, 0, 0, SRCCOPY); 
	EndDraw();
	//char msgbuf[RECMAX];
	//sprintf(msgbuf, "確認　sx %4.1lf sy %4.1lf ex %4.1lf ey %4.1lf", sx, sy, ex, ey); 
	//AfxMessageBox(msgbuf, MB_OK);
#endif

	scrnsetxytime(btime, btime);
	senvsetdrawmode(bmode);
	scrnsetrltvelement();
	scrnsetrltvtop(0.0, 0.0);
	scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
	scrnsetrltvtop(bsx, bsy);
	scrnsetdisprltvmove(bmvx, bmvy);
}

int CBmpFile::MemDCInit(int xdot, int ydot)
{
	int		ret;

	BeginDraw();
	DrawModeSET();
	m_nBmpStat = m_cbmpCBitmap.CreateCompatibleBitmap(m_pcDC, xdot, ydot);
	ret = m_cdcMemCDC.CreateCompatibleDC(m_pcDC);
	EndDraw();

	m_pcbackbmp = (CBitmap*)m_cdcMemCDC.SelectObject(&m_cbmpCBitmap);

	m_crgnClip.CreateRectRgn(0, 0, xdot, ydot);
	//m_crgnClip.CreateRectRgn(-INTMAX, -INTMAX, INTMAX, INTMAX);
	m_cdcMemCDC.SelectClipRgn(&m_crgnClip);
	CRect	rectClip;
	m_cdcMemCDC.GetClipBox(&rectClip);

	m_lXByte = (xdot * 3 + 3) / 4L * 4L;
	m_lBitsSize = m_lXByte * ydot;
	m_hBits = GlobalAlloc(GHND, m_lBitsSize+256);
	m_lpBits = (BYTE *) GlobalLock(m_hBits);

	if(m_nBmpStat == 0){
		return(FALSE);
	}
	return(TRUE);
}
	
void CBmpFile::MemDCEnd()
{
	GlobalUnlock(m_hBits);
	GlobalFree(m_hBits);

	m_crgnClip.DeleteObject();

	m_cdcMemCDC.SelectObject(m_pcbackbmp);

	m_cbmpCBitmap.DeleteObject();

	m_cdcMemCDC.DeleteDC();
}

void CBmpFile::BmpFileOut(LPSTR fname, int xdot, int ydot)
{
	FILE	*fp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	long	dwsize;

	if((fp = fopen(fname, "wb")) == NULL){
		return;
	}
	dwsize = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + m_lBitsSize;
	bmpfh.bfType = 0x4d42;
	bmpfh.bfSize = dwsize;
	bmpfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = xdot;
	bmpinfo.bmiHeader.biHeight = ydot;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = m_lBitsSize;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	::GetDIBits(m_cdcMemCDC.m_hDC, (HBITMAP)(m_cbmpCBitmap.m_hObject), 0, ydot, m_lpBits, &bmpinfo, DIB_RGB_COLORS);

	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(m_lpBits, m_lBitsSize, 1, fp);
	fclose(fp);
}
void CBmpFile::BmpFileOut1(LPSTR fname, int xdot, int ydot)
{
	FILE	*fp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	LPBYTE	lpByte;
	long	xbyte, xhbyte, y, ret;
	long	imgsize, dwsize;
	int		xwrtiebyte, xrestbyte;

	BYTE	*nullimg = new BYTE[5000];
	// 210/25.4*360*3 = 8929.2;

	memset(nullimg, 0, 5000);
	xbyte = ((xdot * 3) + 3L) / 4L * 4L;
	imgsize = xbyte*ydot;
	xhbyte = ((xdot/2 * 3) + 3L) / 4L * 4L;
	xwrtiebyte = xdot/2*3;
	xrestbyte = xbyte-xwrtiebyte;
	if((fp = fopen(fname, "wb")) == NULL){
		return;
	}
	dwsize = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + imgsize;
	bmpfh.bfType = 0x4d42;
	bmpfh.bfSize = dwsize;
	bmpfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = xdot/2;
	bmpinfo.bmiHeader.biHeight = ydot;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = m_lBitsSize;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	::GetDIBits(m_cdcMemCDC.m_hDC, (HBITMAP)(m_cbmpCBitmap.m_hObject), 0, ydot, m_lpBits, &bmpinfo, DIB_RGB_COLORS);

	bmpinfo.bmiHeader.biWidth = xdot;
	bmpinfo.bmiHeader.biSizeImage = imgsize;
	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	for(y = 0; y < ydot; y++){
		lpByte = m_lpBits + y * xhbyte;
		ret = fwrite(lpByte, 1, xwrtiebyte, fp);
		ret = fwrite(nullimg, 1, xrestbyte, fp);
	}
	fclose(fp);
}

void CBmpFile::BmpFileOut2(LPSTR fname, int xdot, int ydot)
{
	FILE	*fp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	LPBYTE	lpByte;
	long	xbyte, xhbyte, xwbyte, y, ret;
	long	imgsize, dwsize;
	int		xwrtiebyte, xrestbyte;
	long	seekp;

	xbyte = ((xdot * 3) + 3L) / 4L * 4L;
	imgsize = xbyte*ydot;
	xhbyte = ((xdot/2 * 3) + 3L) / 4L * 4L;
	xwrtiebyte = xdot/2*3;
	xrestbyte = xbyte-xwrtiebyte;
	if((fp = fopen(fname, "r+b")) == NULL){
		return;
	}
	dwsize = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + imgsize;

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = xdot/2;
	bmpinfo.bmiHeader.biHeight = ydot;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = m_lBitsSize;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	::GetDIBits(m_cdcMemCDC.m_hDC, (HBITMAP)(m_cbmpCBitmap.m_hObject), 0, ydot, m_lpBits, &bmpinfo, DIB_RGB_COLORS);
	seekp = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + xwrtiebyte;
	for(y = 0; y < ydot; y++){
		lpByte = m_lpBits + y * xhbyte;
		fseek(fp, seekp, 0);
		ret = fwrite(lpByte, 1, xwrtiebyte, fp);
		seekp += xbyte;
	}
	fclose(fp);
}

void CBmpFile::BmpFileOutRv(LPSTR fname, int xdot, int ydot)
{
	FILE	*fp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	LPBYTE	lpByte;
	long	xbyte, y, ret;
	long	dwsize;

	xbyte = ((xdot * 3) + 3L) / 4L * 4L;
	if((fp = fopen(fname, "wb")) == NULL){
		return;
	}
	dwsize = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + m_lBitsSize;
	bmpfh.bfType = 0x4d42;
	bmpfh.bfSize = dwsize;
	bmpfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = xdot;
	bmpinfo.bmiHeader.biHeight = ydot;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = m_lBitsSize;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	::GetDIBits(m_cdcMemCDC.m_hDC, (HBITMAP)(m_cbmpCBitmap.m_hObject), 0, ydot, m_lpBits, &bmpinfo, DIB_RGB_COLORS);

	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	for(y = ydot-1; y >= 0; y--){
		lpByte = m_lpBits + y * xbyte;
		ret = fwrite(lpByte, 1, xbyte, fp);
	}
	fclose(fp);
}

void CBmpFile::BmpFileOutRv1(LPSTR fname, int xdot, int ydot)
{
	FILE	*fp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	LPBYTE	lpByte;
	long	xbyte, xhbyte, y, ret;
	long	imgsize, dwsize;
	int		xwrtiebyte, xrestbyte;

	BYTE	*nullimg = new BYTE[5000];
	// 210/25.4*360*3 = 8929.2;

	memset(nullimg, 0, 5000);
	xbyte = ((xdot * 3) + 3L) / 4L * 4L;
	imgsize = xbyte*ydot;
	xhbyte = ((xdot/2 * 3) + 3L) / 4L * 4L;
	xwrtiebyte = xdot/2*3;
	xrestbyte = xbyte-xwrtiebyte;
	if((fp = fopen(fname, "wb")) == NULL){
		return;
	}
	dwsize = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + imgsize;
	bmpfh.bfType = 0x4d42;
	bmpfh.bfSize = dwsize;
	bmpfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = xdot/2;
	bmpinfo.bmiHeader.biHeight = ydot;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = m_lBitsSize;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	::GetDIBits(m_cdcMemCDC.m_hDC, (HBITMAP)(m_cbmpCBitmap.m_hObject), 0, ydot, m_lpBits, &bmpinfo, DIB_RGB_COLORS);

	bmpinfo.bmiHeader.biWidth = xdot;
	bmpinfo.bmiHeader.biSizeImage = imgsize;
	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	for(y = ydot-1; y >= 0; y--){
		lpByte = m_lpBits + y * xhbyte;
		ret = fwrite(lpByte, 1, xwrtiebyte, fp);
		ret = fwrite(nullimg, 1, xrestbyte, fp);
	}
	fclose(fp);
}

void CBmpFile::BmpFileOutRv2(LPSTR fname, int xdot, int ydot)
{
	FILE	*fp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	LPBYTE	lpByte;
	long	xbyte, xhbyte, xwbyte, y, ret;
	long	imgsize, dwsize;
	int		xwrtiebyte, xrestbyte;
	long	seekp;

	xbyte = ((xdot * 3) + 3L) / 4L * 4L;
	imgsize = xbyte*ydot;
	xhbyte = ((xdot/2 * 3) + 3L) / 4L * 4L;
	xwrtiebyte = xdot/2*3;
	xrestbyte = xbyte-xwrtiebyte;
	if((fp = fopen(fname, "r+b")) == NULL){
		return;
	}
	dwsize = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + imgsize;

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = xdot/2;
	bmpinfo.bmiHeader.biHeight = ydot;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = m_lBitsSize;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	::GetDIBits(m_cdcMemCDC.m_hDC, (HBITMAP)(m_cbmpCBitmap.m_hObject), 0, ydot, m_lpBits, &bmpinfo, DIB_RGB_COLORS);
	seekp = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + xwrtiebyte;
	for(y = ydot-1; y >= 0; y--){
		lpByte = m_lpBits + y * xhbyte;
		fseek(fp, seekp, 0);
		ret = fwrite(lpByte, 1, xwrtiebyte, fp);
		seekp += xbyte;
	}
	fclose(fp);
}

void CBmpFile::BmpOnebitFileOutRV(LPSTR fname, int xdot, int ydot)
{
	FILE	*fp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	RGBQUAD	clr[2];
	long	headsize, dwsize;
	long	xbyte, imgsize;
	int		mask, byte;
	BYTE	img[RECMAX4];
	int		x, y;
	BYTE	*topsp;

	xbyte = ((xdot+7)/8+3)/4*4;
	imgsize = xbyte * ydot; 
	if((fp = fopen(fname, "wb")) == NULL){
		return;
	}
	headsize = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + sizeof(RGBQUAD)*2;
	dwsize = headsize + imgsize;

	bmpfh.bfType = 0x4d42;
	bmpfh.bfSize = dwsize;
	bmpfh.bfOffBits = headsize;

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = xdot;
	bmpinfo.bmiHeader.biHeight = ydot;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = imgsize;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	clr[0].rgbBlue = 0;
	clr[0].rgbGreen = 0;
	clr[0].rgbRed = 0;
	clr[0].rgbReserved = 0;
	clr[1].rgbBlue = 255;
	clr[1].rgbGreen = 255;
	clr[1].rgbRed = 255;
	clr[1].rgbReserved = 0;

	::GetDIBits(m_cdcMemCDC.m_hDC, (HBITMAP)(m_cbmpCBitmap.m_hObject), 0, ydot, m_lpBits, &bmpinfo, DIB_RGB_COLORS);

	bmpinfo.bmiHeader.biWidth = xbyte*8;
	bmpinfo.bmiHeader.biBitCount = 1;
	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(clr, sizeof(RGBQUAD), 2, fp);
	for(y = 0; y < ydot; y++){
		topsp = m_lpBits +  m_lXByte * (ydot-y-1);
		memset(img, 0, xbyte);
		byte = 0;
		mask = 0x080;
		for(x = 0; x < xdot; x++){
			if(topsp[x*3] == 0){
				img[byte] |= mask;
			}
			mask >>= 1;
			if(mask == 0){
				byte++;
				mask = 0x080;
			}
		}
		for(byte = 0; byte < xbyte; byte++){
			img[byte] = ~img[byte];
		}
		fwrite(img, 1, xbyte, fp);
	}
	fclose(fp);
}

void CBmpFile::BmpOnebitFileOut(LPSTR fname, int xdot, int ydot)
{
	FILE	*fp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	RGBQUAD	clr[2];
	long	headsize, dwsize;
	long	xbyte, imgsize;
	int		mask, byte;
	BYTE	img[RECMAX4];
	int		x, y;
	BYTE	*topsp;

	xbyte = ((xdot+7)/8+3)/4*4;
	imgsize = xbyte * ydot; 
	if((fp = fopen(fname, "wb")) == NULL){
		return;
	}
	headsize = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + sizeof(RGBQUAD)*2;
	dwsize = headsize + imgsize;

	bmpfh.bfType = 0x4d42;
	bmpfh.bfSize = dwsize;
	bmpfh.bfOffBits = headsize;

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = xdot;
	bmpinfo.bmiHeader.biHeight = ydot;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = imgsize;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	clr[0].rgbBlue = 0;
	clr[0].rgbGreen = 0;
	clr[0].rgbRed = 0;
	clr[0].rgbReserved = 0;
	clr[1].rgbBlue = 255;
	clr[1].rgbGreen = 255;
	clr[1].rgbRed = 255;
	clr[1].rgbReserved = 0;

	::GetDIBits(m_cdcMemCDC.m_hDC, (HBITMAP)(m_cbmpCBitmap.m_hObject), 0, ydot, m_lpBits, &bmpinfo, DIB_RGB_COLORS);

	bmpinfo.bmiHeader.biWidth = xbyte*8;
	bmpinfo.bmiHeader.biBitCount = 1;
	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(clr, sizeof(RGBQUAD), 2, fp);
	for(y = 0; y < ydot; y++){
		topsp = m_lpBits +  m_lXByte * y;
		memset(img, 0, xbyte);
		byte = 0;
		mask = 0x080;
		for(x = 0; x < xdot; x++){
			if(topsp[x*3] == 0){
				img[byte] |= mask;
			}
			mask >>= 1;
			if(mask == 0){
				byte++;
				mask = 0x080;
			}
		}
		for(byte = 0; byte < xbyte; byte++){
			img[byte] = ~img[byte];
		}
		fwrite(img, 1, xbyte, fp);
	}
	fclose(fp);
}

void CBmpFile::BmpFileOutHalf(LPSTR fname, int xdot, int ydot)
{
	int		setxdot, setydot;
	FILE	*fp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	long	dwsize, size;

	if((fp = fopen(fname, "wb")) == NULL){
		return;
	}
	dwsize = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + m_lBitsSize;
	bmpfh.bfType = 0x4d42;
	bmpfh.bfSize = dwsize;
	bmpfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = xdot;
	bmpinfo.bmiHeader.biHeight = ydot;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = m_lBitsSize;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	::GetDIBits(m_cdcMemCDC.m_hDC, (HBITMAP)(m_cbmpCBitmap.m_hObject), 0, ydot, m_lpBits, &bmpinfo, DIB_RGB_COLORS);

	ConvertHalfBmpImage(xdot, ydot);

	setxdot = xdot / 3;
	setydot = ydot / 3;

	size = ((setxdot * 3 + 3) / 4L * 4L) * setydot;
	dwsize = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + size;
	bmpfh.bfType = 0x4d42;
	bmpfh.bfSize = dwsize;
	bmpfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = setxdot;
	bmpinfo.bmiHeader.biHeight = setydot;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = size;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(m_lpBits, size, 1, fp);
	fclose(fp);
}

void CBmpFile::ConvertHalfBmpImage(int xdot, int ydot)
{
	int		setxdot, setydot;
	int		x, y, tx, ty, adx, ady;
	int		r, g, b;
	int		tr, tg, tb;
	
	setxdot = xdot / 3;
	setydot = ydot / 3;
	//setxdot = xdot / 2;
	//setydot = ydot / 2;
	m_nHalfXbyte = (setxdot * 3 + 3) / 4L * 4L;
	for(y = 0; y < setydot; y++){
		for(x = 0; x < setxdot; x++){
			tx = x*3; ty = y*3;
			//tx = x*2; ty = y*2;
			r = 0; g = 0; b = 0;
			for(ady = 0; ady < 3; ady++){
				for(adx = 0; adx < 3; adx++){
					Getrbg(tx+adx, ty+ady, &tr, &tg, &tb);
					r += tr; g += tg; b += tb;
				}
			}
			r /= 9; g /= 9; b /= 9;
			// r /= 4; g /= 4; b /= 4;
			SetHalfrgb(x, y, r, g, b);
		}
	}
}

void CBmpFile::Getrbg(int x, int y, int *r, int *g, int *b)
{
	LPBYTE	adr;

	adr = m_lpBits + (y * m_lXByte + x * 3);
	*b = (int)((*adr) & 0x00ff);
	*g = (int)((*(adr+1)) & 0x00ff);
	*r = (int)((*(adr+2)) & 0x00ff);
}

void CBmpFile::SetHalfrgb(int x, int y, int r, int g, int b)
{
	LPBYTE	adr;

	adr = m_lpBits + (y * m_nHalfXbyte + x * 3);
	*adr = b;
	*(adr+1) = g;
	*(adr+2) = r;
}

