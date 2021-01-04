
/////////////////////////////////////////////////////////////////////////////
// CDibLib

#include "stdafx.h"
#include "DibLib.h"

//extern "C" {
#include "../../ObjLib/Senvlib.h"
//}

CDibLib::CDibLib(CScrollView* pcview)
{
	m_pcView = (CTonView*)pcview;
}

CDibLib::~CDibLib()
{
}

CDC *CDibLib::CreateDIBDC(int xsize, int ysize)
{
	LPBITMAPINFO		lpbi;
	LPBITMAPINFOHEADER	lpbih;
	LPRGBQUAD			lpclr;
	CDC					*pcDC;

	m_nXDotSize = xsize;
	m_nYDotSize = ysize;
	m_nXByte = (((m_nXDotSize + 31) / 32) * 32) / 8;
	m_dwFileSize = (DWORD)(sizeof(BITMAPINFOHEADER)) + (DWORD)(2 * sizeof(RGBQUAD)) + (DWORD)m_nXByte*(DWORD)m_nYDotSize;
	m_dwFileSize += (DWORD)m_nXByte*6;
	m_hDIB = GlobalAlloc(GHND, m_dwFileSize);
	if(m_hDIB == 0){
		return(NULL);
	}
	lpbi = (LPBITMAPINFO)GlobalLock(m_hDIB);
	if(lpbi == NULL){
		GlobalFree(m_hDIB);
		return(NULL);
	}
	lpbih = &(lpbi->bmiHeader);
	memset((LPTSTR)(lpbi), 0, m_dwFileSize);
	lpbih->biSize = sizeof(BITMAPINFOHEADER);
	lpbih->biWidth = m_nXDotSize;
	lpbih->biHeight = m_nYDotSize;
	lpbih->biPlanes = 1;
	lpbih->biBitCount = 1;
	lpbih->biCompression = BI_RGB;

	lpclr = lpbi->bmiColors;
	lpclr[0].rgbBlue = 0;
	lpclr[0].rgbGreen = 0;
	lpclr[0].rgbRed = 0;
	lpclr[0].rgbReserved = 0;
	lpclr[1].rgbBlue = 255;
	lpclr[1].rgbGreen = 255;
	lpclr[1].rgbRed = 255;
	lpclr[1].rgbReserved = 0;

	m_lpBits = ((LPBYTE)lpbi) + (long)(sizeof(BITMAPINFOHEADER) + 2*sizeof(RGBQUAD));

	m_pcBitmap = new CBitmap();
	m_pcBitmap->CreateBitmap(m_nXDotSize, m_nXDotSize, 1, 1, NULL);
	m_pcMemDC = new CDC();
	pcDC = m_pcView->GetDC();
	m_pcMemDC->CreateCompatibleDC(pcDC);
	m_pcView->ReleaseDC(pcDC);
	m_pcoldBitmap = m_pcMemDC->SelectObject(m_pcBitmap);

	return(m_pcMemDC);
}

void CDibLib::DeleteDIBDC()
{
	m_pcMemDC->SelectObject(m_pcoldBitmap);
	delete(m_pcBitmap);
	delete(m_pcMemDC);
	GlobalUnlock(m_hDIB);
	GlobalFree(m_hDIB);
}

void CDibLib::AreaCls(CDC *pcDC)
{
	HPEN	hP, oldhP;
	HBRUSH	hB, oldhB;

	oldhP = (HPEN)SelectObject(pcDC->m_hDC, CreatePen(PS_SOLID, 1, RGB(255, 255, 255)));
	oldhB = (HBRUSH)SelectObject(pcDC->m_hDC, CreateSolidBrush(RGB(255, 255, 255)));
	Rectangle(pcDC->m_hDC, 0, 0, (int)m_nXDotSize, (int)m_nYDotSize);
	hB = (HBRUSH)SelectObject(pcDC->m_hDC, oldhB);
	hP = (HPEN)SelectObject(pcDC->m_hDC, oldhP);
	DeleteObject(hB);
	DeleteObject(hP);
}

void CDibLib::GetBitImage()
{
	TCHAR	btppath[FNAMEMAX];
	TCHAR	bmpfname[FNAMEMAX];

	m_pcBitmap->GetBitmapBits(m_dwFileSize, m_lpBits);
	senvgetbtppath(btppath);
	_stprintf_s(bmpfname, _T("%s\\tmpsys$$.bmp"), btppath);
	BmpFileOut(bmpfname, m_lpBits, m_nXByte, m_nXDotSize, m_nYDotSize);
}

int CDibLib::PreFirstYOnBit(int x)
{
	int			byte;
	BYTE FAR	*adr;
	int		y, mask;

	mask = 0x0080 >> (x % 8);
	byte = x / 8;
	for(y = 0; y < m_nYDotSize; y++){
		adr = m_lpBits + m_nXByte * y + byte;
		if(((*adr) & mask) == 0){
			break;
		}
	}
	return(y);
}

int CDibLib::PostFirstYOnBit(int x)
{
	int			byte;
	BYTE FAR	*adr;
	int		y, mask;

	mask = 0x0080 >> (x % 8);
	byte = x / 8;
	for(y = m_nYDotSize-1; y >= 0; y--){
		adr = m_lpBits + m_nXByte * y + byte;
		if(((*adr) & mask) == 0){
			break;
		}
	}
	if(y < 0){
		y = 0;
	}
	return(y);
}

int CDibLib::PreFirstXOnBit(int y)
{
	int			byte, bytemax;
	BYTE FAR	*adr;
	int		i, mask;
	int		ret;

	bytemax = m_nXDotSize / 8;
	byte = m_nXByte * y;
	adr = m_lpBits + (long)byte;
	for(i = 0; i < bytemax; i++){
		if(adr[i] != 0xff){
			break;
		}
	}
	if(i == bytemax){
		return(m_nXDotSize);
	}
	byte = i;
	mask = 0x0080;
	for(i = 0; i < 8; i++){
		if((adr[byte] & mask) == 0){
			break;
		}
		mask >>= 1;
	}
	ret = byte * 8 + i;
	return(ret);
}

int CDibLib::PostFirstXOnBit(int y)
{
	int		byte, bytemax;
	BYTE FAR	*adr;
	int		i, mask;
	int		ret;

	bytemax = m_nXDotSize / 8;
	byte = m_nXByte * y;
	adr = m_lpBits + (long)byte;
	for(i = bytemax-1; i >= 0; i--){
		if(adr[i] != 0xff){
			break;
		}
	}
	if(i <= -1){
		return(0);
	}
	byte = i;
	mask = 0x0001;
	for(i = 0; i < 8; i++){
		if((adr[byte] & mask) == 0){
			break;
		}
		mask <<= 1;
	}
	ret = byte * 8 + (7 - i);
	return(ret);
}

void CDibLib::BmpFileOut(LPTSTR fname, BYTE far *m_lpBits, int xbyte, int xsize, int ysize)
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
	txbyte = (((long)xsize + 7L) / 8L + 1L) / 2L * 2L;
	for(y = 0; y < ysize; y++){
		sp = m_lpBits + (long)y * (long)txbyte;
		fwrite(sp, 1, xbyte, fp);
	}
	fclose(fp);
}

