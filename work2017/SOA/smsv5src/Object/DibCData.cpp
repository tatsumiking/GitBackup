
//
// Easy Scanner System
//
// object dibdata.cpp  ÇaÇlÇoÉfÅ[É^ä÷åWä÷êî
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"

#include "memory.h"
#include "io.h"

#include "Data.h"
#include "Vect.h"
#include "BoldLib.h"
#include "DibCData.h"

#include	"../ObjLib/Baselib.h"
#include	"../ObjLib/Senvlib.h"
#include	"../ObjLib/Scrnlib.h"
#include	"../ObjLib/Diblib.h"

#define OUTXSIZE	64
#define OUTYSIZE	64

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
	m_nTrnsMode = 0;
	m_nStat = 1;
	m_lFileSize = 0;
	m_szOrgFileName[0] = _T('\0');
	m_szImgFileName[0] = _T('\0');
	m_nBitCount = 0;
	m_hDib = 0;
	m_pcGPBitmap = NULL;
}

CDibCData::~CDibCData()
{
	if(m_hDib != 0){
		GlobalFree(m_hDib);
	}
}

int CDibCData::CheckID()
{
	return(DIBCDATAID);
}

void CDibCData::Serialize(CArchive& ar)
{
	FILE	*fp;
	BYTE	ch;
	ULONG	idx;

	if (ar.IsStoring()){
		ar << m_dDstSx;
		ar << m_dDstSy;
		ar << m_dDstWidth;
		ar << m_dDstHight;
		if((_taccess_s(m_szOrgFileName, 0)) != 0){
			m_nStat = 0;
			ar << m_nStat;
			return;			
		}
		else{
			m_nStat = 2; // m_nTrnsModeÇí«â¡ÇµÇΩDibCData
			ar << m_nStat;
			ar << m_nTrnsMode;
		}
		ar << CString(m_szOrgFileName);

		if(scrncheckundoflag() == OFF){
			ar << m_lFileSize;
			if (m_pcGPBitmap != NULL) {
				delete(m_pcGPBitmap);
			}
			_tfopen_s(&fp, m_szImgFileName, _T("rb"));
			if(fp != NULL){
				for(idx = 0; idx < m_lFileSize; idx++){
					fread(&ch, 1, 1, fp);
					ar << ch;
				}
				fclose(fp);
			}
			if (m_pcGPBitmap != NULL) {
				m_pcGPBitmap = new Bitmap(m_szImgFileName);
			}
		}
		RenewMiniMax();
	}
	else{
		ar >> m_dDstSx;
		ar >> m_dDstSy;
		ar >> m_dDstWidth;
		ar >> m_dDstHight;
		ar >> m_nStat;
		if(m_nStat == 0){
			m_szOrgFileName[0] = _T('\0');
			return;
		}
		else if(m_nStat == 2) {
			ar >> m_nTrnsMode;
		}
		CString csOrgFileName;
		ar >> csOrgFileName;
		_tcscpy_s(m_szOrgFileName, csOrgFileName.GetBuffer(FNAMEMAX));
		if(scrncheckundoflag() == OFF){
			// êÃÇÃÉtÉ@ÉCÉãñºÇÕñ≥éãÇ∑ÇÈ
			if (m_pcGPBitmap != NULL) {
				delete(m_pcGPBitmap);
			}
			_tfopen_s(&fp, m_szImgFileName, _T("wb"));
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
			if (m_pcGPBitmap != NULL) {
				m_pcGPBitmap = new Bitmap(m_szImgFileName);
			}
		}
		InitElement();
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

void CDibCData::SetFileName(LPTSTR imgfilename)
{
	_tcscpy_s(m_szOrgFileName, imgfilename);
	_tcscpy_s(m_szImgFileName, imgfilename);
	diblibcreateimgfilename(m_szImgFileName);
	CopyFile(m_szOrgFileName, m_szImgFileName, FALSE);
}
void CDibCData::GetFileName(LPTSTR imgfilename)
{
	int len = _tcslen(m_szOrgFileName) + 1;
	_tcscpy_s(imgfilename, len, m_szImgFileName);
}
void CDibCData::SetTrnsMode(int mode)
{
	m_nTrnsMode = mode;
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
	CGdiPlusEx *pcGpe;
	DBL dsx, dsy, dex, dey;
	DBL dw, dh;

	if(m_szImgFileName[0] == _T('\0')){
		return;
	}

	pcGpe = new CGdiPlusEx();
	pcGpe->InitGraphics(pcDC);
	GetDstElement(&dsx, &dsy, &dw, &dh);
	dex = dsx + dw;
	dey = dsy + dh;
	scrndxymmtorltv(&dsx, &dsy);
	scrndxymmtorltv(&dex, &dey);
	dw = dex - dsx;
	dh = dsy - dey;
	//m_pcGPBitmap->RotateFlip(Rotate180FlipX);
	pcGpe->DrawImage(m_pcGPBitmap, dsx, dey, dw, dh);
	delete(pcGpe);
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
int CDibCData::InitElement()
{
	int		ret;

	ret = FileLoad();

	if (m_pcGPBitmap != NULL) {
		m_dDstWidth = m_dSrcWidth / m_dSrcXDPI * 25.4;
		m_dDstHight = m_dSrcHight / m_dSrcYDPI * 25.4;
		m_dDstSx = INITSETX;
		m_dDstSy = INITSETY;
	}
	return(0);
}
int CDibCData::FileLoad()
{
	LPTSTR	lpSafix;
	CFile	cFile;
	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bminfo;
	LPBYTE	lpDib;

	if (m_pcGPBitmap != NULL) {
		delete(m_pcGPBitmap);
	}

	if (m_hDib != 0) {
		GlobalFree(m_hDib);
	}

	lpSafix = basegetsafix(m_szImgFileName);

	cFile.Open(m_szImgFileName, CFile::modeRead | CFile::typeBinary);
	m_lFileSize = (ULONG)cFile.GetLength();
	m_hDib = 0;
	m_nBitCount = 24;
	if (_tcsicmp(lpSafix, _T("png")) == 0) {
		getImageEncodeClsid(_T("image/png"), &m_clsid);
		m_nBitCount = 32;
	}
	else if (_tcsicmp(lpSafix, _T("jpg")) == 0 || _tcsicmp(lpSafix, _T("jpeg")) == 0) {
		getImageEncodeClsid(_T("image/jpeg"), &m_clsid);
		m_nBitCount = 24;
	}
	else if (_tcsicmp(lpSafix, _T("tif")) == 0 || _tcsicmp(lpSafix, _T("tiff")) == 0) {
		getImageEncodeClsid(_T("image/tiff"), &m_clsid);
		m_nBitCount = 24;
	}
	else if (_tcsicmp(lpSafix, _T("gif")) == 0 || _tcsicmp(lpSafix, _T("gif")) == 0) {
		getImageEncodeClsid(_T("image/gif"), &m_clsid);
		m_nBitCount = 24;
	}
	else if (_tcsicmp(lpSafix, _T("bmp")) == 0) {
		getImageEncodeClsid(_T("image/bmp"), &m_clsid);
		cFile.Read(&bmf, sizeof(BITMAPFILEHEADER));
		cFile.Read(&bminfo, sizeof(BITMAPINFOHEADER));
		m_nBitCount = bminfo.biBitCount;
		m_dSrcWidth = bminfo.biWidth;
		m_dSrcHight = bminfo.biHeight;
		m_lImageTopSeek = bmf.bfOffBits;
		m_dXByte = (((DWORD)(m_dSrcWidth * m_nBitCount + 7) / 8) + 3L) / 4L * 4L;

		if (m_nBitCount == 1) {
			cFile.SeekToBegin();
			m_hDib = GlobalAlloc(GHND, m_lFileSize);
			if (m_hDib != 0) {
				lpDib = (LPBYTE)GlobalLock(m_hDib);
				if (lpDib != NULL) {
					cFile.Read(lpDib, m_lFileSize);
				}
				GlobalUnlock(m_hDib);
			}
		}
	}
	cFile.Close();

	m_pcGPBitmap = new Bitmap(m_szImgFileName);
	if (m_pcGPBitmap != NULL) {
		m_dSrcXDPI = m_pcGPBitmap->GetHorizontalResolution();
		m_dSrcYDPI = m_pcGPBitmap->GetVerticalResolution();
		m_dSrcWidth = m_pcGPBitmap->GetWidth();
		m_dSrcHight = m_pcGPBitmap->GetHeight();
	}
	return 0;
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
int CDibCData::SetRGBQuadData(int plt, int rval, int gval, int bval)
{
	LPBYTE	lpDib;
	RGBQUAD FAR *lpRGBQuad;
	CFile	cFile;

	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if (lpDib == NULL) {
		return 1;
	}
	if (m_pcGPBitmap != NULL) {
		delete(m_pcGPBitmap);
	}
	lpRGBQuad = (RGBQUAD FAR *)(lpDib + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	lpRGBQuad[plt].rgbBlue = bval;
	lpRGBQuad[plt].rgbGreen = gval;
	lpRGBQuad[plt].rgbRed = rval;

	cFile.Open(m_szImgFileName, CFile::modeWrite | CFile::typeBinary);
	cFile.Write(lpDib, m_lFileSize);
	cFile.Close();

	GlobalUnlock(m_hDib);

	if (m_pcGPBitmap != NULL) {
		m_pcGPBitmap = new Bitmap(m_szImgFileName);
	}

	return 0;
}
COLORREF CDibCData::GetPicupClr(DBL dx, DBL dy)
{
	DBL		trnsx, trnsy;
	int		nsx, nsy;
	Gdiplus::Color gpclr;
	COLORREF	clr;

	dx = dx - m_dDstSx;
	dy = dy - m_dDstSy;
	trnsx = m_dSrcWidth / m_dDstWidth;
	trnsy = m_dSrcHight / m_dDstHight;
	nsx = (int)(dx * trnsx);
	nsy = (int)(m_dSrcHight - dy * trnsy);
	nsy = (int)(m_dSrcHight - nsy);
	ImageAreaLegal(&nsy, &nsy);
	m_pcGPBitmap->GetPixel(nsy, nsy, &gpclr);
	clr = gpclr.ToCOLORREF();
	return(clr);
}

void CDibCData::ImageAreaLegal(int *nsx, int *nsy)
{
	int		nx, ny;

	nx = *nsx;
	ny = *nsy;
	if(ny < 0){
		ny = 0;
	}
	if(m_dSrcHight <= ny){
		ny = (int)(m_dSrcHight-1);
	}

	if(nx < 0){
		nx = 0;
	}
	if(m_dSrcWidth <= nx){
		nx = (int)(m_dSrcWidth-1);
	}
	*nsx = nx;
	*nsy = ny;
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
CData* CDibCData::CopyData(DBL movex, DBL movey)
{
	CDibCData*	pcnewData;
	TCHAR		newfname[FNAMEMAX];

	pcnewData = (CDibCData*)NewDataCreate(DIBCDATAID);
	_tcscpy_s(newfname, m_szOrgFileName);
	diblibcreateimgfilename(newfname);
	CopyFile(m_szImgFileName, newfname, FALSE);
	pcnewData->SetFileName(newfname);
	pcnewData->InitElement();
	pcnewData->m_dDstHight = m_dDstHight;
	pcnewData->m_dDstWidth = m_dDstWidth;
	pcnewData->m_dDstSx = m_dDstSx + movex;
	pcnewData->m_dDstSy = m_dDstSy + movey;
	pcnewData->RenewMiniMax();
	// ÉoÉbÉNÉrÉbÉgÉ}ÉbÉvÇÃÉRÉsÅ[Ç‡ïKóv
	return((CData*)pcnewData);
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

	if (sx > ex) {
		SWAP(sx, ex, xy);
	}
	if (sy > ey) {
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
	if (nsx < 0) {
		nsx = 0;
	}
	if (nex > (int)(m_dSrcWidth)) {
		nex = (int)(m_dSrcWidth - 1);
	}
	if (nsy < 0) {
		nsy = 0;
	}
	if (ney > (int)(m_dSrcHight)) {
		ney = (int)(m_dSrcHight - 1);
	}

	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if (lpDib == NULL) {
		return;
	}
	lpbits = lpDib + m_lImageTopSeek;
	for (y = nsy; y < ney; y++) {
		lpoffset = lpbits + (ULONG)(y * m_dXByte);
		byte = nsx / 8;
		mask = 0x0080 >> (nsx % 8);
		for (x = nsx; x < nex; x++) {
			lpoffset[byte] |= mask;
			mask >>= 1;
			if (mask == 0) {
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

	if (sx > ex) {
		SWAP(sx, ex, xy);
	}
	if (sy > ey) {
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
	if (nsx < 0) {
		nsx = 0;
	}
	if (nex > (int)(m_dSrcWidth)) {
		nex = (int)(m_dSrcWidth - 1);
	}
	if (nsy < 0) {
		nsy = 0;
	}
	if (ney > (int)(m_dSrcHight)) {
		ney = (int)(m_dSrcHight - 1);
	}

	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if (lpDib == NULL) {
		return;
	}
	lpbits = lpDib + m_lImageTopSeek;
	for (y = nsy; y < ney; y++) {
		lpoffset = lpbits + (ULONG)(y * m_dXByte);
		byte = nsx / 8;
		mask = 0x0080 >> (nsx % 8);
		for (x = nsx; x < nex; x++) {
			lpoffset[byte] &= ~mask;
			mask >>= 1;
			if (mask == 0) {
				byte++;
				mask = 0x0080;
			}
		}
	}
	GlobalUnlock(m_hDib);
}
int CDibCData::FileDibSave()
{
	CFile	cpFile;
	LPBYTE	lpDib;

	if (m_pcGPBitmap != NULL) {
		delete(m_pcGPBitmap);
	}

	if ((cpFile.Open(m_szImgFileName, CFile::modeWrite)) == FALSE) {
		AfxMessageBox(_T("ÇaÇlÇoÉtÉ@ÉCÉãÇ™äJÇØÇ‹ÇπÇÒ"), MB_OK);
	}
	else {
		lpDib = (LPBYTE)GlobalLock(m_hDib);
		cpFile.Write(lpDib, m_lFileSize);
		GlobalUnlock(m_hDib);
		cpFile.Close();
	}
	if (m_pcGPBitmap != NULL) {
		m_pcGPBitmap = new Bitmap(m_szImgFileName);
	}
	return(0);
}
int CDibCData::FileTrnsSave()
{
	if (m_pcGPBitmap != NULL) {
		delete(m_pcGPBitmap);
	}

	Bitmap *bitmap = Bitmap::FromFile(m_szImgFileName, TRUE);
	switch (m_nTrnsMode) {
	case 1:	//ÇPÇWÇOÅãâÒì]
		bitmap->RotateFlip(Rotate180FlipNone);
		break;
	case 2:	//ç∂ÇXÇOÅãâÒì]
		bitmap->RotateFlip(Rotate270FlipNone);
		break;
	case 3:	//âEÇXÇOÅãâÒì]
		bitmap->RotateFlip(Rotate90FlipNone);
		break;
	case 4:	//è„â∫îΩì]
		bitmap->RotateFlip(RotateNoneFlipY);
		break;
	case 5:	//ç∂âEîΩì]
		bitmap->RotateFlip(RotateNoneFlipX);
		break;
	}
	m_nTrnsMode = 0;
	bitmap->Save(m_szImgFileName, &m_clsid);

	if (m_pcGPBitmap != NULL) {
		m_pcGPBitmap = new Bitmap(m_szImgFileName);
	}
	return 0;
}
int CDibCData::getImageEncodeClsid(LPTSTR lpFormat, CLSID* pcClsid)
{
	UINT  num = 0;
	UINT  size = 0;
	ImageCodecInfo* pcImageCodecInfo;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return(0);
	pcImageCodecInfo = (ImageCodecInfo*)new char[size];
	if (pcImageCodecInfo == NULL)
		return(0);
	GetImageEncoders(num, size, pcImageCodecInfo);
	for (UINT n = 0; n < num; ++n) {
		if (_tcscmp(lpFormat, pcImageCodecInfo[n].MimeType) == 0) {
			*pcClsid = pcImageCodecInfo[n].Clsid;
			free(pcImageCodecInfo);
			return(n + 1);
		}
	}
	free(pcImageCodecInfo);
	return 0;
}
