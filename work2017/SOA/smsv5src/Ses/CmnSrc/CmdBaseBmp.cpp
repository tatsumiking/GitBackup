
/////////////////////////////////////////////////////////////////////////////
// CCmdBaseBmp

#include "stdafx.h"
#include "stdio.h"

#include "CmdBase.h"
#include "CmdBaseBmp.h"

//extern "C" {
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
//}

CCmdBaseBmp::CCmdBaseBmp(CScrollView* pcview)
:CCmdBase(pcview)
{
}

CCmdBaseBmp::~CCmdBaseBmp()
{
}


void CCmdBaseBmp::InitFullClrDrawArea(int xdot, int ydot, DBL sx, DBL sy, DBL ex, DBL ey)
{
	DBL		bmpx, bmpy;

	m_dSX = sx;
	m_dSY = sy;
	m_dEX = ex;
	m_dEY = ey;
	m_nXSize = xdot;
	m_nYSize = ydot;
	bmpx = m_nXSize;
	bmpy = m_nYSize;
	m_dXTime = (bmpx / (ex - sx));
	m_dYTime = (bmpy / (ey - sy));
	if(m_dXTime < m_dYTime){
		m_dYTime = m_dXTime;
	}
	else{
		m_dXTime = m_dYTime;
	}

	InitFullclrMemDC();
}

// フルカラー用画像の初期処理
void CCmdBaseBmp::InitFullclrImage(int xdot, int ydot)
{
	DBL		xmm, ymm;
	DBL		bmpx, bmpy;

	scrngetmmsize(&xmm, &ymm);
	m_dSX = 0;
	m_dSY = 0;
	m_dEX = xmm;
	m_dEY = ymm;
	m_nXSize = xdot;
	m_nYSize = ydot;
	bmpx = m_nXSize;
	bmpy = m_nYSize;
	m_dXTime = (bmpx / xmm);
	m_dYTime = (bmpy / ymm);
	if(m_dXTime < m_dYTime){
		m_dYTime = m_dXTime;
	}
	else{
		m_dXTime = m_dYTime;
	}
	InitFullclrMemDC();
}

// フルカラー用画像の後処理
void CCmdBaseBmp::EndFullclrImage()
{
	EndFullclrMemDC();
}

// データを描画しＢＭＰファイルを作成
void CCmdBaseBmp::DrawAndCreateFullclrFile(LPTSTR fname)
{
	OnDrawArea();
	CreateFullclrFile(fname);
}

void CCmdBaseBmp::InitDrawArea(DBL dminix, DBL dminiy, DBL dmaxx, DBL dmaxy)
{
	DBL xsize, ysize;

	scrngetrltvtop(&m_dBsx, &m_dBsy); 
	scrnsetrltvtop(0, 0);
	scrndxymmtorltv(&dminix, &dminiy);
	scrndxymmtorltv(&dmaxx, &dmaxy);
	scrnsetrltvtop(dminix, dmaxy);
	xsize = dmaxx - dminix;
	ysize = dminiy - dmaxy;
	SetDotSize(xsize, ysize);
}

void CCmdBaseBmp::EndDrawArea()
{
	scrnsetrltvtop(m_dBsx, m_dBsy); 
}

void CCmdBaseBmp::InitDrawAddArea(DBL dminix, DBL dminiy, DBL dmaxx, DBL dmaxy)
{
	DBL		addx, addy;
	DBL		minix, miniy, maxx, maxy;
	DBL		areaxmm, areaymm;
	DBL		xtime, ytime, time;
	DBL		addrltv;

	addx = (dmaxx - dminix) / 8; 
	addy = (dmaxy - dminiy) / 8; 
	minix = dminix - addx;
	miniy = dminiy - addy;
	maxx = dmaxx + addx;
	maxy = dmaxy + addy;

	areaxmm = maxx - minix;
	areaymm = maxy - miniy;

	xtime = areaxmm / m_nXSize;
	ytime = areaymm / m_nYSize;

	if(xtime > ytime){
		time = xtime;
		addrltv = (areaymm - areaxmm) / time;
	}
	else{
		time = ytime;
		addrltv = 0;
	}
	m_nBMode = senvcheckdrawmode();
	senvsetdrawmode(PREVIEW);
	m_dBtime = scrnsetxytime(time, time);
	m_dTrnsTime = time;
	scrngetmmsize(&m_dBxmm, &m_dBymm);
	scrngetrltvtop(&m_dBsx, &m_dBsy); 
	scrngetdisprltvmove(&m_dBmvx, &m_dBmvy);
	scrnsetrltvtop(0.0, 0.0);
	scrnsetdisprltvmove(0.0, 0.0);
	scrnsetmmsize(areaxmm, areaymm);
	scrnsetrltvelement();
	scrndxymmtorltv(&minix, &miniy);
	scrndxymmtorltv(&maxx, &maxy);
	m_dCntrX = minix;
	m_dCntrY = miniy;
	m_dCntrY -= addrltv;
	scrndxyrltvtomm(&m_dCntrX, &m_dCntrY);

	scrnsetrltvtop(minix, maxy);

	scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
}

void CCmdBaseBmp::EndDrawAddArea()
{
	scrnsetxytime(m_dBtime, m_dBtime);
	scrnsetmmsize(m_dBxmm, m_dBymm);
	scrnsetrltvelement();
	scrnsetrltvtop(m_dBsx, m_dBsy); 
	scrnsetdisprltvmove(m_dBmvx, m_dBmvy);
	((CTonView*)m_pcView)->InitScrollElement();
	senvsetdrawmode(m_nBMode);
}

void CCmdBaseBmp::InitActiveDrawArea()
{
	DBL		minx, miny, maxx, maxy;
	DBL		xytime, time;
	DBL		sumarea, areay;

	m_pcDoc->GetDMiniMax(ACTIVEFLAG, &minx, &miny, &maxx, &maxy);
	xytime = (maxy - miny) / (maxx - minx);
	sumarea = BITMAPAREASIZE * BITMAPAREASIZE;
	m_nXSize = (int)(sqrt(sumarea / xytime));
	m_nYSize = (int)(m_nXSize * xytime);
	m_nXByte = ((((long)m_nXSize + 7) / 8L + 3L) / 4L)*4L;
	m_nXSize = m_nXByte * 8;
	m_lBitsSize = m_nXByte * m_nYSize;
	m_dAreaXMM = maxx - minx;
	m_dAreaYMM = maxy - miny;
	time = m_dAreaYMM / m_nYSize;
	m_dAreaXMM = m_nXSize * time;

	m_nBMode = senvcheckdrawmode();
	senvsetdrawmode(PREVIEW);
	m_dBtime = scrnsetxytime(time, time);
	scrngetmmsize(&m_dBxmm, &m_dBymm);
	scrngetrltvtop(&m_dBsx, &m_dBsy); 
	scrngetdisprltvmove(&m_dBmvx, &m_dBmvy);

	scrnsetrltvtop(0.0, 0.0);
	scrnsetdisprltvmove(0.0, 0.0);
	scrnsetmmsize(m_dAreaXMM, m_dAreaYMM);
	scrnsetrltvelement();
	scrndxymmtorltv(&minx, &miny);
	scrndxymmtorltv(&maxx, &maxy);
	areay = m_dAreaYMM;
	scrndlenmmtorltv(&areay);
	scrnsetrltvtopreverce(minx, maxy, 0, areay); 
	//scrnsetrltvtop(topx, topy);
	scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
}

void CCmdBaseBmp::EndActiveDrawArea()
{
	scrnsetxytime(m_dBtime, m_dBtime);
	scrnsetmmsize(m_dBxmm, m_dBymm);
	scrnsetrltvelement();
	scrnsetrltvtop(m_dBsx, m_dBsy); 
	scrnsetdisprltvmove(m_dBmvx, m_dBmvy);
	((CTonView*)m_pcView)->InitScrollElement();
	senvsetdrawmode(m_nBMode);
}

void CCmdBaseBmp::SetDotSize(DBL xsize, DBL ysize)
{
	m_nXSize = (int)xsize;
	m_nYSize = (int)ysize;
	m_nXByte = ((((long)m_nXSize + 7) / 8L + 3L) / 4L)*4L;
	m_lBitsSize = m_nXByte * m_nYSize;
}

void CCmdBaseBmp::InitBits()
{
	m_hBits = GlobalAlloc(GHND, m_lBitsSize);
	if(m_hBits == 0){
		return;
	}
	m_lpBits = (BYTE *) GlobalLock(m_hBits);
	if(m_lpBits == NULL){
		GlobalFree(m_hBits);
		return;
	}
	memset(m_lpBits, 0, m_lBitsSize);
}

void CCmdBaseBmp::InitImage()
{
	CDC		*pcDC;
	UINT	unPlanes, unBitCnt;

	m_pcMemDC = NULL;
	m_pcBitmap = NULL;
	unPlanes = 1;
	unBitCnt = 1;
	//メモリデバイスコンテクストの生成
	m_pcBitmap = new CBitmap();
	m_pcBitmap->CreateBitmap(m_nXSize, m_nYSize, unPlanes, unBitCnt, NULL);

	m_pcMemDC = new CDC();
	pcDC = m_pcView->GetDC();
	m_pcMemDC->CreateCompatibleDC(pcDC);
	m_pcView->ReleaseDC(pcDC);
	m_oldBitmap = m_pcMemDC->SelectObject(m_pcBitmap);
}

void CCmdBaseBmp::EndImage()
{
	m_pcMemDC->SelectObject(m_oldBitmap);
	delete(m_pcBitmap);
	delete(m_pcMemDC);
}

void CCmdBaseBmp::EndBits()
{
	if(m_lpBits != NULL){
		GlobalUnlock(m_hBits);
		m_lpBits = NULL;
	}
	if(m_hBits != 0){
		GlobalFree(m_hBits);
		m_hBits = 0;
	}
}

void CCmdBaseBmp::InitBitMap()
{
	m_pcMemDC->BitBlt( 0,0 , m_nXByte*8, m_nYSize, NULL, 0, 0, BLACKNESS);
}

int CCmdBaseBmp::BitOnOffCheck(BYTE *lpBits, int x, int y)
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
	sp = lpBits + (long)y * (long)m_nXByte + (long)nbyte;
	if(*sp & mask){
		return(1);
	}
	else{
		return(0);
	}
}

void CCmdBaseBmp::BitOnSet(BYTE *lpBits, int x, int y)
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
	sp = lpBits + (long)y * (long)m_nXByte + (long)nbyte;
	(*sp) |= mask;
}

void CCmdBaseBmp::BitOffSet(BYTE *lpBits, int x, int y)
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
	sp = lpBits + (long)y * (long)m_nXByte + (long)nbyte;
	(*sp) &= ~mask;
}

void CCmdBaseBmp::CreateBmpFile(LPTSTR fname, BYTE far *lpBits)
{
	FILE				*fp;
	BITMAPFILEHEADER	bif;
	BITMAPINFOHEADER	bih;
	RGBQUAD				clr[2];
	int					headsize;
	BYTE				*sp;
	int					y;

	memset((LPTSTR)(&bif), 0, sizeof(BITMAPFILEHEADER));
	memset((LPTSTR)(&bih), 0, sizeof(BITMAPINFOHEADER));
	headsize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*2;
	bif.bfType = 0x4D42;
	bif.bfSize = m_nXByte * m_nYSize + headsize;
	bif.bfOffBits = headsize;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = m_nXSize;
	bih.biHeight = m_nYSize;
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
	for(y = 0; y < m_nYSize; y++){
		sp = lpBits + (long)y * (long)m_nXByte;
		fwrite(sp, 1, m_nXByte, fp);
	}
	fclose(fp);
}

// 内部使用関数
void CCmdBaseBmp::OnDrawArea()
{
	DBL		dx, dy;
	int		bmode;
	DBL		btime;
	DBL		bmvx, bmvy;
	DBL		brltvtopx, brltvtopy;

	scrngetdisprltvmove(&bmvx, &bmvy);
	scrngetrltvtop(&brltvtopx, &brltvtopy);
	bmode = senvsetdrawmode(DIRCTVIEW);
	btime = scrnsetxytime(1.0 / m_dXTime, 1.0 / m_dYTime);
	scrnsetrltvelement();
	dx = m_dSX; dy = m_dEY;
	scrndxymmtorltv(&dx, &dy);
	scrnsetrltvtop(dx, dy);
	scrnrltvareaset(0.0, 0.0, m_nXSize, m_nYSize);
	scrnsetdisprltvmove(0.0, 0.0);

	m_pcMemDC->BitBlt(0, 0, (int)m_nXSize, (int)m_nYSize, NULL, 0, 0, WHITENESS);
	((CTonView*)m_pcView)->DirectDraw(m_pcMemDC);

#ifdef _DEBUG
	BeginDraw();
	DrawModeSET();
	m_pcDC->BitBlt(0, 0, (int)m_nXSize, (int)m_nYSize, m_pcMemDC, 0, 0, SRCCOPY); 
	EndDraw();
	//TCHAR msgbuf[RECMAX];
	//_stprintf_s(msgbuf, "確認"); 
	//AfxMessageBox(msgbuf, MB_OK);
#endif
	scrnsetxytime(btime, btime);
	senvsetdrawmode(bmode);
	scrnsetrltvelement();
	scrnsetrltvtop(0.0, 0.0);
	scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
	scrnsetdisprltvmove(bmvx, bmvy);
	scrnsetrltvtop(brltvtopx, brltvtopy);
}

void CCmdBaseBmp::InitFullclrMemDC()
{
	int		ret;

	BeginDraw();
	m_pcBitmap = new CBitmap();
	m_nBmpStat = m_pcBitmap->CreateCompatibleBitmap(m_pcDC, m_nXSize, m_nYSize);
	m_pcMemDC = new CDC();
	ret = m_pcMemDC->CreateCompatibleDC(m_pcDC);
	EndDraw();

	m_oldBitmap = (CBitmap*)m_pcMemDC->SelectObject(m_pcBitmap);

	m_crgnClip.CreateRectRgn(0, 0, m_nXSize, m_nYSize);
	//m_crgnClip.CreateRectRgn(-INTMAX, -INTMAX, INTMAX, INTMAX);
	m_pcMemDC->SelectClipRgn(&m_crgnClip);
	CRect	rectClip;
	m_pcMemDC->GetClipBox(&rectClip);

	m_nXByte = (m_nXSize * 3 + 3) / 4L * 4L;
	m_lBitsSize = m_nXByte * m_nYSize;
	m_hBits = GlobalAlloc(GHND, m_lBitsSize+1024);
	m_lpBits = (BYTE *) GlobalLock(m_hBits);
}

void CCmdBaseBmp::EndFullclrMemDC()
{
	GlobalUnlock(m_hBits);
	GlobalFree(m_hBits);

	m_crgnClip.DeleteObject();

	m_pcMemDC->SelectObject(m_oldBitmap);

	m_pcBitmap->DeleteObject();
	m_pcMemDC->DeleteDC();
	delete(m_pcBitmap);
	delete(m_pcMemDC);
}

void CCmdBaseBmp::CreateFullclrFile(LPTSTR fname)
{
	FILE	*fp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	long	dwsize;

	_tfopen_s(&fp, fname, _T("wb"));
	if(fp == NULL){
		return;
	}
	dwsize = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + m_lBitsSize;
	bmpfh.bfType = 0x4d42;
	bmpfh.bfSize = dwsize;
	bmpfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = m_nXSize;
	bmpinfo.bmiHeader.biHeight = m_nYSize;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = m_lBitsSize;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	::GetDIBits(m_pcMemDC->m_hDC, (HBITMAP)(m_pcBitmap->m_hObject), 0, m_nYSize, m_lpBits, &bmpinfo, DIB_RGB_COLORS);

	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(m_lpBits, m_lBitsSize, 1, fp);
	fclose(fp);
}
