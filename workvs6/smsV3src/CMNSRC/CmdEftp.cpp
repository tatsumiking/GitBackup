
/////////////////////////////////////////////////////////////////////////////
// CCmdEFtp

#include "stdafx.h"
#include "math.h"
#include "mapi.h"
#include "io.h"
#include "direct.h"
#include <afxinet.h>   
#include "..\imgkit\imagekit.h"

#include "cmd_base.h"
#include "cmdbsbmp.h"
#include "cmdeftp.h"
#include "bmpfile.h"
#include "DSlctFtpAdrs.h"
#include "zip32.h"

extern "C" {
	void senvgettmppath(LPSTR path);
	void senvgetexepath(LPSTR path);
	void scrngetmmsize(DBL *scrnxsize, DBL *scrnysize);
	void poutcntrkindget(int *kind);
}
typedef int (WINAPI *ZPINITFUNC)(LPZIPUSERFUNCTIONS);
typedef BOOL (WINAPI *ZPSETOPTIONSFUNC)(LPZPOPT);
typedef int (WINAPI *ZPARCHIVEFUNC)(ZCL);

/** Zip32.dllから呼び出されるコールバック関数 */
int WINAPI defaultEPrintProc(LPSTR, unsigned long);
int WINAPI defaultEPasswordProc(LPSTR, int, LPCSTR, LPCSTR);
int WINAPI defaultECommentProc(LPSTR);

CCmdEFtp::CCmdEFtp(CScrollView* pcview)
:CCmdBaseBmp(pcview)
{
#ifdef _DEBUG
	strcpy(m_strFtpRoot, "sv58.wadax.ne.jp");
	strcpy(m_strFtpPath, "public_html/upload");
	strcpy(m_strFtpID, "c-alfo-com");
	strcpy(m_strFtpPW, "VTqR7aAQ");
#else
	
	strcpy(m_strFtpRoot, "soa-nahuda.sakura.ne.jp");
	strcpy(m_strFtpPath, "www/uploadE");
	strcpy(m_strFtpID, "soa-nahuda");
	strcpy(m_strFtpPW, "w5xa7trte9");
#endif	
}

UINT CCmdEFtp::ExecMltprnFtp(UINT event, DBL dx, DBL dy)
{
	int		cntrkind;
	char	tmppath[FNAMEMAX];
	char	szFileNameCmp[FNAMEMAX];
	char	szFileNameBmp[FNAMEMAX];
	char	szFileNameDspBmp[FNAMEMAX];
	char	szFileNameZip[FNAMEMAX];
	char	szFileNameDspZip[FNAMEMAX];
	char	szFileNameCrct[FNAMEMAX];
	char	szBaseName[RECMAX];
	int		ret;
	DBL		dWorkXSize, dWorkYSize;
	DBL		dpi;
	HANDLE		hndle, nhndle;
	int		mode;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	
	CDialogSlctFtpAdrs *pcDialogSlctFtpAdrs = new CDialogSlctFtpAdrs(m_pcView);
	pcDialogSlctFtpAdrs->m_csFileName = m_pcDoc->GetTitle();
	ret = pcDialogSlctFtpAdrs->DoModal();
	if(ret == IDOK){
		strcpy(m_szFtpAdrs,pcDialogSlctFtpAdrs->m_csFtpFolder.GetBuffer(QRECMAX));
		strcpy(m_szBaseName,pcDialogSlctFtpAdrs->m_csFileName.GetBuffer(QRECMAX));
	}
	delete(pcDialogSlctFtpAdrs);
	if(ret != IDOK){
		return(ENDEVENT);
	}
	poutcntrkindget(&cntrkind);

	sprintf(szBaseName, "%s00", m_szBaseName);
	
	senvgettmppath(tmppath);
	scrngetmmsize(&dWorkXSize, &dWorkYSize);
	sprintf(szFileNameBmp, "%s\\%srv.bmp", tmppath, szBaseName);
	SaveBmpFile(szFileNameBmp, 360.0);
	if(m_pcView->m_bEpsonYoko == TRUE){
		if(dWorkXSize > dWorkYSize){
			hndle = ImgKitFileLoad(szFileNameBmp, "", "");
			nhndle = ImgKitRotation(hndle, "", "", 2);
			ImgKitBmpFileSave(szFileNameBmp, nhndle, "", "", 0);
		}
	}
	sprintf(szFileNameCmp, "%s\\%s.cmp", tmppath, szBaseName);
	BmpToCmpBytelen(szFileNameBmp, szFileNameCmp);

	sprintf(szFileNameDspBmp, "%s\\%s.bmp", tmppath, szBaseName);
	dpi = 700 * 25.4 / dWorkYSize;
	SaveDspBmpFile(szFileNameDspBmp, dpi);
#ifdef _DEBUG
	char	szBmpFile[FNAMEMAX];
	sprintf(szBmpFile, "%s\\testcmp.bmp", tmppath);
	CmpToBmpBytelen(szFileNameCmp, szBmpFile);
#endif
	chdir(tmppath);
	sprintf(szFileNameZip, "%s.zip", szBaseName);
	sprintf(szFileNameDspZip, "%sDSP.zip", szBaseName);
	SrcZipConv(tmppath, szFileNameCmp, szFileNameZip);
	SrcZipConv(tmppath, szFileNameDspBmp, szFileNameDspZip);
	strcpy(szFileNameCrct, "crct.lst");
	FILE *fp;
	fp = fopen(szFileNameCrct, "w");
	if(fp == NULL){
		return(ENDEVENT);
	}
	fprintf(fp, "%s\n", szBaseName);
	fclose(fp);
	SendFileForFtp(szFileNameCrct, m_szFtpAdrs);
	SendFileForFtp(szFileNameZip, m_szFtpAdrs);
	SendFileForFtp(szFileNameDspZip, m_szFtpAdrs);
	ScrnAllRedraw();
	::AfxMessageBox("FTP転送が終了しました", MB_OK);
	return(ENDEVENT);
}

void CCmdEFtp::SmsoutCrctFileInit()
{
	char	tmppath[FNAMEMAX];
	char	szFileNameCrct[FNAMEMAX];
	int		ret;

	InitClass();
	senvgettmppath(tmppath);
	chdir(tmppath);
	m_nCrtNo = 1;
	strcpy(szFileNameCrct, "crct.lst");
	FILE *fp;
	fp = fopen(szFileNameCrct, "w");
	if(fp == NULL){
		return;
	}
	fclose(fp);

	CDialogSlctFtpAdrs *pcDialogSlctFtpAdrs = new CDialogSlctFtpAdrs(m_pcView);
	pcDialogSlctFtpAdrs->m_csFileName = _T("");
	ret = pcDialogSlctFtpAdrs->DoModal();
	if(ret == IDOK){
		strcpy(m_szFtpAdrs,pcDialogSlctFtpAdrs->m_csFtpFolder.GetBuffer(RECMAX));
		strcpy(m_szBaseName,pcDialogSlctFtpAdrs->m_csFileName.GetBuffer(RECMAX));
	}
	delete(pcDialogSlctFtpAdrs);

}

void CCmdEFtp::SmsoutSendData()
{
	int		cntrkind;
	char	tmppath[FNAMEMAX];
	char	szFileNameCmp[FNAMEMAX];
	char	szFileNameBmp[FNAMEMAX];
	char	szFileNameDspBmp[FNAMEMAX];
	char	szFileNameZip[FNAMEMAX];
	char	szFileNameDspZip[FNAMEMAX];
	char	szFileNameCrct[FNAMEMAX];
	char	szBaseName[RECMAX];
	int		ret;
	DBL		dWorkXSize, dWorkYSize;
	DBL		dpi;
	HANDLE		hndle, nhndle;
	int		mode;

	poutcntrkindget(&cntrkind);
	sprintf(szBaseName, "%s%02d", m_szBaseName, m_nCrtNo);
	m_nCrtNo++;
	senvgettmppath(tmppath);
	scrngetmmsize(&dWorkXSize, &dWorkYSize);
	sprintf(szFileNameCmp, "%s\\%s.cmp", tmppath, szBaseName);
	sprintf(szFileNameBmp, "%s\\%s.bmp", tmppath, szBaseName);
	SaveBmpFile(szFileNameBmp, 360.0);
	if(m_pcView->m_bEpsonYoko == TRUE){
		if(dWorkXSize > dWorkYSize){
			hndle = ImgKitFileLoad(szFileNameBmp, "", "");
			nhndle = ImgKitRotation(hndle, "", "", 2);
			ImgKitBmpFileSave(szFileNameBmp, nhndle, "", "", 0);
		}
	}
	BmpToCmpBytelen(szFileNameBmp, szFileNameCmp);
	sprintf(szFileNameDspBmp, "%s\\%s.bmp", tmppath, szBaseName);
	dpi = 700 * 25.4 / dWorkYSize;
	SaveDspBmpFile(szFileNameDspBmp, dpi);
	chdir(tmppath);
	sprintf(szFileNameZip, "%s.zip", szBaseName);
	sprintf(szFileNameDspZip, "%sDSP.zip", szBaseName);
	SrcZipConv(tmppath, szFileNameCmp, szFileNameZip);
	SrcZipConv(tmppath, szFileNameDspBmp, szFileNameDspZip);

	SendFileForFtp(szFileNameZip, m_szFtpAdrs);
	SendFileForFtp(szFileNameDspZip, m_szFtpAdrs);

	strcpy(szFileNameCrct, "crct.lst");
	FILE *fp;
	fp = fopen(szFileNameCrct, "r+");
	if(fp == NULL){
		return;
	}
	fseek(fp, 0L, 2);
	fprintf(fp, "%s\n", szBaseName);
	fclose(fp);
}

void CCmdEFtp::SmsoutCrctFileEnd()
{
	char	tmppath[FNAMEMAX];
	char	szFileNameCrct[FNAMEMAX];

	chdir(tmppath);
	strcpy(szFileNameCrct, "crct.lst");
	SendFileForFtp(szFileNameCrct, m_szFtpAdrs);
	::AfxMessageBox("FTP転送が終了しました", MB_OK);
}

void CCmdEFtp::SaveBmpFile(LPSTR lpFileName, DBL dpi)
{
	DBL		xsize, ysize;
	int		xdot, ydot;
	CBmpFile	*pcBmpFile;

	scrngetmmsize(&xsize, &ysize);
	xdot = (int)(xsize / 25.4 * dpi);
	ydot = (int)(ysize / 25.4 * dpi);
	pcBmpFile = new CBmpFile(m_pcView);
	pcBmpFile->CreateBmpInitHalf(xdot, ydot);
	pcBmpFile->CreateBmpImageDiv(lpFileName);
	pcBmpFile->CreateBmpEnd();
	delete(pcBmpFile);
}

void CCmdEFtp::SaveBmpFileRv(LPSTR lpFileName, DBL dpi)
{
	DBL		xsize, ysize;
	int		xdot, ydot;
	CBmpFile	*pcBmpFile;

	scrngetmmsize(&xsize, &ysize);
	xdot = (int)(xsize / 25.4 * dpi);
	ydot = (int)(ysize / 25.4 * dpi);
	pcBmpFile = new CBmpFile(m_pcView);
	pcBmpFile->CreateBmpInitHalf(xdot, ydot);
	pcBmpFile->CreateBmpImageDivRv(lpFileName);
	pcBmpFile->CreateBmpEnd();
	delete(pcBmpFile);
}

void CCmdEFtp::SaveDspBmpFile(LPSTR lpFileName, DBL dpi)
{
	DBL		xsize, ysize;
	int		xdot, ydot;
	CBmpFile	*pcBmpFile;

	scrngetmmsize(&xsize, &ysize);
	xdot = (int)(xsize / 25.4 * dpi);
	ydot = (int)(ysize / 25.4 * dpi);
	pcBmpFile = new CBmpFile(m_pcView);
	pcBmpFile->CreateBmpInit(xdot, ydot);
	pcBmpFile->CreateBmpImage(lpFileName);
	pcBmpFile->CreateBmpEnd();
	delete(pcBmpFile);
}
void CCmdEFtp::SaveDspBmpFileRv(LPSTR lpFileName, DBL dpi)
{
	DBL		xsize, ysize;
	int		xdot, ydot;
	CBmpFile	*pcBmpFile;

	scrngetmmsize(&xsize, &ysize);
	xdot = (int)(xsize / 25.4 * dpi);
	ydot = (int)(ysize / 25.4 * dpi);
	pcBmpFile = new CBmpFile(m_pcView);
	pcBmpFile->CreateBmpInit(xdot, ydot);
	pcBmpFile->CreateBmpImageRv(lpFileName);
	pcBmpFile->CreateBmpEnd();
	delete(pcBmpFile);
}
void CCmdEFtp::BmpToCmpBytelen(LPSTR lpBmpFName, LPSTR lpCmpFName)
{
	FILE	*ifp;
	FILE	*ofp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	int		x, y, dst;
	int		xdot, ydot, xbyte;
	BYTE	*lpSrc;
	BYTE	*lpDst;
	int		dstx, dstxdot, dstydot, dstxbyte;

	ifp = fopen(lpBmpFName, "rb");
	if(ifp == NULL){
		return;
	}
	ofp = fopen(lpCmpFName, "wb");
	if(ofp == NULL){
		fclose(ifp);
		return;
	}

	fread(&bmpfh, sizeof(BITMAPFILEHEADER), 1, ifp);
	fread(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, ifp);
	xdot = bmpinfo.bmiHeader.biWidth;
	ydot = bmpinfo.bmiHeader.biHeight;
	xbyte = (((xdot * 24 + 7) / 8) + 3) / 4 * 4;
	dstxbyte = ((xdot * 4) + 4) / 8;
	dstxdot = xdot;
	dstydot = ydot;
	lpSrc = new BYTE[xbyte+16];
	lpDst = new BYTE[dstxbyte+16];

	Setint32(lpDst, 0, dstxbyte);
	Setint32(lpDst, 4, dstxdot);
	Setint32(lpDst, 8, dstydot);
	fwrite(lpDst, 12, 1, ofp);
	for(y = 0; y < ydot; y++){
		fread(lpSrc, 1, xbyte, ifp);
		for(x = 0; x < xbyte; x++){
			lpSrc[x] = ~(lpSrc[x]);
		}
		for(x = 0; x < dstxbyte; x++){
			lpDst[x] = 0;
		}
		for(dst = 0, x = xdot; x > 0; x--, dst++){
			dstx = dst / 2;
		//for(x = 0; x < xdot; x++){
			//dstx = x / 2;
			if(lpSrc[x*3] == 0xff
			&& lpSrc[x*3+1] == 0xff
			&& lpSrc[x*3+2] == 0xff){
				if((x % 2) == 0){
					lpDst[dstx] |= 0x80;
				}else{
					lpDst[dstx] |= 0x08;
				}
			}else{
				if(lpSrc[x*3] == 0xff){// Yellow
					if((x % 2) == 0){
						lpDst[dstx] |= 0x40;
					}else{
						lpDst[dstx] |= 0x04;
					}
				}
				if(lpSrc[x*3+1] == 0xff){// Magenda
					if((x % 2) == 0){
						lpDst[dstx] |= 0x20;
					}else{
						lpDst[dstx] |= 0x02;
					}
				}
				if(lpSrc[x*3+2] == 0xff){// Cyan
					if((x % 2) == 0){
						lpDst[dstx] |= 0x10;
					}else{
						lpDst[dstx] |= 0x01;
					}
				}
			}
		}
		fwrite(lpDst, 1, dstxbyte, ofp);
	}
	delete(lpSrc);
	delete(lpDst);

	fclose(ifp);
	fclose(ofp);
}

void CCmdEFtp::CmpToBmpBytelen(LPSTR lpCmpFName, LPSTR lpBmpFName)
{
	FILE	*ifp;
	FILE	*ofp;
	BITMAPFILEHEADER	bif;
	BITMAPINFOHEADER	bih;
	BYTE	imgSrc[64];
	int		srcxdot, srcydot, srcxbyte;
	int		xdot, ydot, xbyte;
	int		headsize;
	int		x, y, srcx;
	BYTE	*lpSrc;
	BYTE	*lpDst;
	int		i1, i2;
	BYTE	img[0x100];

	ifp = fopen(lpCmpFName, "rb");
	if(ifp == NULL){
		return;
	}
	ofp = fopen(lpBmpFName, "wb");
	if(ofp == NULL){
		fclose(ifp);
		return;
	}
	fread(imgSrc, 1, 12, ifp);
	i1 = (imgSrc[0] & 0x00ff); i2 = (imgSrc[1] & 0x00ff);
	srcxbyte = i1+ i2 * 0x100;
	i1 = (imgSrc[4] & 0x00ff); i2 = (imgSrc[5] & 0x00ff);
	srcxdot = i1+ i2 * 0x100;
	i1 = (imgSrc[8] & 0x00ff); i2 = (imgSrc[9] & 0x00ff);
	srcydot = i1+ i2 * 0x100;
	xdot = srcxdot;
	ydot = srcydot;
	xbyte = (((xdot * 24 + 7) / 8) + 3) / 4 * 4;

	lpSrc = new BYTE[srcxbyte+16];
	lpDst = new BYTE[xbyte+16];

	headsize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	memset(&bif, 0, sizeof(BITMAPFILEHEADER));
	memset(&bih, 0, sizeof(BITMAPINFOHEADER));
	bif.bfType = 0x4D42;
	bif.bfSize = xbyte * ydot + headsize;
	bif.bfOffBits = headsize;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = xdot;
	bih.biHeight = ydot;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = 0;

	fwrite(&bif, sizeof(BITMAPFILEHEADER), 1, ofp);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, ofp);
	for(y = 0; y < ydot; y++){
		fread(lpSrc, 1, srcxbyte, ifp);
		//memcpy(img, lpSrc, srcxbyte);
		memset(lpDst, '\0', xbyte);
		for(x = 0; x < xdot; x++){
			srcx = x / 2;
			if((x % 2) == 0){
				if(lpSrc[srcx] & 0x80){
					lpDst[x*3] = 0xff;
					lpDst[x*3+1] = 0xff;
					lpDst[x*3+2] = 0xff;
				}else{
					if(lpSrc[srcx] & 0x40){
						lpDst[x*3] |= 0xff;
					}
					if(lpSrc[srcx] & 0x20){
						lpDst[x*3+1] |= 0xff;
					}
					if(lpSrc[srcx] & 0x10){
						lpDst[x*3+2] |= 0xff;
					}
				}
			}else{
				if(lpSrc[srcx] & 0x08){
					lpDst[x*3] = 0xff;
					lpDst[x*3+1] = 0xff;
					lpDst[x*3+2] = 0xff;
				}else{
					if(lpSrc[srcx] & 0x04){
						lpDst[x*3] |= 0xff;
					}
					if(lpSrc[srcx] & 0x02){
						lpDst[x*3+1] |= 0xff;
					}
					if(lpSrc[srcx] & 0x01){
						lpDst[x*3+2] |= 0xff;
					}
				}
			}
		}
		//memcpy(img, lpDst, xbyte);

		for(x = 0; x < xbyte; x++){
			lpDst[x] = ~(lpDst[x]);
		}
		fwrite(lpDst, 1, xbyte, ofp);
	}
	delete(lpSrc);
	delete(lpDst);

	fclose(ifp);
	fclose(ofp);
}

void CCmdEFtp::SrcZipConv(LPSTR lpWorkPath, LPSTR lpSrcFile, LPSTR lpZipFile)
{
	HINSTANCE	hZip;
	ZPINITFUNC	ZpInit;
	ZPSETOPTIONSFUNC	ZpSetOptions;
	ZPARCHIVEFUNC	ZpArchive;
	ZIPUSERFUNCTIONS sZipUserFunctions;
	ZPOPT			sZpOpt;
	ZCL				sZcl;
	int				ret;

	//Zip32j.dllのロード
	hZip = LoadLibrary("zip32.dll");
	if (hZip!=NULL){  
		//Zip()APIの呼び出し
		ZpInit = (ZPINITFUNC)GetProcAddress(hZip, "ZpInit");
		ZpSetOptions = (ZPSETOPTIONSFUNC)GetProcAddress(hZip, "ZpSetOptions");
		ZpArchive = (ZPARCHIVEFUNC)GetProcAddress(hZip, "ZpArchive");
		if (ZpInit != NULL){
			ZeroMemory(&sZipUserFunctions, sizeof(ZIPUSERFUNCTIONS));
			sZipUserFunctions.print = defaultEPrintProc;
			sZipUserFunctions.comment = defaultECommentProc;
			sZipUserFunctions.password = defaultEPasswordProc;
			ZeroMemory(&sZpOpt, sizeof(ZPOPT));
			sZpOpt.fRecurse = 1;           /* ディレクトリを再帰的に処理するかのフラグ: 1 = "-r", 2 = "-R" */
			sZpOpt.szRootDir = lpWorkPath;

			ZeroMemory(&sZcl, sizeof(ZCL));
			sZcl.argc = 1;
			sZcl.FNV = &lpSrcFile;
			sZcl.lpszZipFN = lpZipFile;

			ZpInit(&sZipUserFunctions);
			ZpSetOptions(&sZpOpt);
			ret = ZpArchive(sZcl);
		}else{
			AfxMessageBox("ZpInit()APIが存在しません。DLLが壊れています。", MB_OK);
		}
	}else{
		AfxMessageBox("ZIP32.dllがありません。", MB_OK);
	}
	FreeLibrary(hZip);
}

void CCmdEFtp::SendFileForFtp(LPSTR lpFileName, LPSTR lpFtpFolder)
{
	CString	csPath;

//assumes server and file names have been initialized
	CInternetSession session("FTP Session");
	CFtpConnection* pcConn = NULL;

// 接続サーバ
	pcConn = session.GetFtpConnection(m_strFtpRoot, m_strFtpID, m_strFtpPW, 21, INTERNET_FLAG_PASSIVE);
	csPath.Format("%s/%s", m_strFtpPath, lpFtpFolder);
	pcConn->SetCurrentDirectory(csPath);
	int ret = pcConn->PutFile(lpFileName, lpFileName, FTP_TRANSFER_TYPE_BINARY, 1);
// 後処理
	if(pcConn != NULL){
		delete pcConn;
	}
	session.Close();
}

void CCmdEFtp::Setint32(BYTE buf[], int offset, int num)
{
	buf[offset++] = (BYTE)(num & 0x00ff);
	buf[offset++] = (BYTE)((num >> 8) & 0x00ff);
	buf[offset++] = (BYTE)((num >> 16) & 0x00ff);
	buf[offset++] = (BYTE)((num >> 24) & 0x00ff);
}

void CCmdEFtp::Setint16(BYTE buf[], int offset, int num)
{
	buf[offset++] = (BYTE)(num & 0x00ff);
	buf[offset++] = (BYTE)((num >> 8) & 0x00ff);
}

int WINAPI defaultEPrintProc(LPSTR buff, unsigned long size)
{
	//printf("%s", buff);
	return (unsigned int)size;
}

int WINAPI defaultECommentProc(char* buff)
{
	buff[0] = '\0';
	return true;
}

int WINAPI defaultEPasswordProc(LPSTR p, int n, LPCSTR m, LPCSTR name)
{
	return 1;
}

