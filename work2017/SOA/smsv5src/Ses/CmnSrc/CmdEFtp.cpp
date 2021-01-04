
/////////////////////////////////////////////////////////////////////////////
// CCmdEFtp

#include "stdafx.h"
#include <afxinet.h>   
#include "mapi.h"
#include "io.h"
#include "direct.h"
#include "zip32.h"
//#include "..\imgkit\imagekit.h"

#include "CmdBase.h"
#include "CmdBaseBmp.h"
#include "CmdEFtp.h"

#include "BmpFile.h"
#include "DialogSlctFtpAdrs.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Pout.h"
//}

typedef int (WINAPI *ZPINITFUNC)(LPZIPUSERFUNCTIONS);
typedef BOOL (WINAPI *ZPSETOPTIONSFUNC)(LPZPOPT);
typedef int (WINAPI *ZPARCHIVEFUNC)(ZCL);

/** Zip32.dllから呼び出されるコールバック関数 */
int WINAPI defaultEPrintProc(LPTSTR, unsigned long);
int WINAPI defaultEPasswordProc(LPTSTR, int, LPCSTR, LPCSTR);
int WINAPI defaultECommentProc(LPTSTR);

CCmdEFtp::CCmdEFtp(CScrollView* pcview)
:CCmdBaseBmp(pcview)
{
#ifdef _DEBUG
	_tcscpy_s(m_strFtpRoot, _T("sv58.wadax.ne.jp"));
	_tcscpy_s(m_strFtpPath, _T("public_html/upload"));
	_tcscpy_s(m_strFtpID, _T("c-alfo-com"));
	_tcscpy_s(m_strFtpPW, _T("VTqR7aAQ"));
#else
	
	_tcscpy_s(m_strFtpRoot, _T("soa-nahuda.sakura.ne.jp"));
	_tcscpy_s(m_strFtpPath, _T("www/uploadE"));
	_tcscpy_s(m_strFtpID, _T("soa-nahuda"));
	_tcscpy_s(m_strFtpPW, _T("w5xa7trte9"));
#endif	
}

UINT CCmdEFtp::ExecMltprnFtp(UINT event, DBL dx, DBL dy)
{
	int		cntrkind;
	TCHAR	tmppath[FNAMEMAX];
	TCHAR	szFileNameCmp[FNAMEMAX];
	TCHAR	szFileNameBmp[FNAMEMAX];
	TCHAR	szFileNameDspBmp[FNAMEMAX];
	TCHAR	szFileNameZip[FNAMEMAX];
	TCHAR	szFileNameDspZip[FNAMEMAX];
	TCHAR	szFileNameCrct[FNAMEMAX];
	TCHAR	szBaseName[RECMAX];
	int		ret;
	DBL		dWorkXSize, dWorkYSize;
	DBL		dpi;
	TCHAR	buf[RECMAX];

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	
	CDialogSlctFtpAdrs *pcDialogSlctFtpAdrs = new CDialogSlctFtpAdrs(m_pcView);
	pcDialogSlctFtpAdrs->m_csFileName = m_pcDoc->GetTitle();
	ret = pcDialogSlctFtpAdrs->DoModal();
	if(ret == IDOK){
		_tcscpy_s(m_szFtpAdrs,pcDialogSlctFtpAdrs->m_csFtpFolder.GetBuffer(QRECMAX));
		_tcscpy_s(m_szBaseName,pcDialogSlctFtpAdrs->m_csFileName.GetBuffer(QRECMAX));
	}
	delete(pcDialogSlctFtpAdrs);
	if(ret != IDOK){
		return(ENDEVENT);
	}
	poutcntrkindget(&cntrkind);

	_stprintf_s(szBaseName, _T("%s00"), m_szBaseName);
	
	senvgettmppath(tmppath);
	scrngetmmsize(&dWorkXSize, &dWorkYSize);
	_stprintf_s(szFileNameBmp, _T("%s\\%srv.bmp"), tmppath, szBaseName);
	SaveBmpFile(szFileNameBmp, 360.0);
	if(m_pcView->m_bEpsonYoko == TRUE){
		if(dWorkXSize > dWorkYSize){
			/*
			HANDLE		hndle, nhndle;
			int		mode;

			hndle = ImgKitFileLoad(szFileNameBmp, "", "");
			nhndle = ImgKitRotation(hndle, "", "", 2);
			ImgKitBmpFileSave(szFileNameBmp, nhndle, "", "", 0);
			*/
		}
	}
	_stprintf_s(szFileNameCmp, _T("%s\\%s.cmp"), tmppath, szBaseName);
	BmpToCmpBytelen(szFileNameBmp, szFileNameCmp);

	_stprintf_s(szFileNameDspBmp, _T("%s\\%s.bmp"), tmppath, szBaseName);
	dpi = 700 * 25.4 / dWorkYSize;
	SaveDspBmpFile(szFileNameDspBmp, dpi);
#ifdef _DEBUG
	TCHAR	szBmpFile[FNAMEMAX];
	_stprintf_s(szBmpFile, _T("%s\\testcmp.bmp"), tmppath);
	CmpToBmpBytelen(szFileNameCmp, szBmpFile);
#endif
	_tchdir(tmppath);
	_stprintf_s(szFileNameZip, _T("%s.zip"), szBaseName);
	_stprintf_s(szFileNameDspZip, _T("%sDSP.zip"), szBaseName);
	SrcZipConv(tmppath, szFileNameCmp, szFileNameZip);
	SrcZipConv(tmppath, szFileNameDspBmp, szFileNameDspZip);
	_stprintf_s(szFileNameCrct, _T("crct.lst"));
	FILE *fp;
	_tfopen_s(&fp, szFileNameCrct, _T("w"));
	if(fp == NULL){
		return(ENDEVENT);
	}
	_stprintf_s(buf, _T("%s\n"), szBaseName);
	baselibFputs(buf, fp);
	fclose(fp);
	SendFileForFtp(szFileNameCrct, m_szFtpAdrs);
	SendFileForFtp(szFileNameZip, m_szFtpAdrs);
	SendFileForFtp(szFileNameDspZip, m_szFtpAdrs);
	ScrnAllRedraw();
	::AfxMessageBox(_T("FTP転送が終了しました"), MB_OK);
	return(ENDEVENT);
}

void CCmdEFtp::SmsoutCrctFileInit()
{
	TCHAR	tmppath[FNAMEMAX];
	TCHAR	szFileNameCrct[FNAMEMAX];
	int		ret;

	InitClass();
	senvgettmppath(tmppath);
	_tchdir(tmppath);
	m_nCrtNo = 1;
	_tcscpy_s(szFileNameCrct, _T("crct.lst"));
	FILE *fp;
	_tfopen_s(&fp, szFileNameCrct, _T("w"));
	if(fp == NULL){
		return;
	}
	fclose(fp);

	CDialogSlctFtpAdrs *pcDialogSlctFtpAdrs = new CDialogSlctFtpAdrs(m_pcView);
	pcDialogSlctFtpAdrs->m_csFileName = _T("");
	ret = pcDialogSlctFtpAdrs->DoModal();
	if(ret == IDOK){
		_tcscpy_s(m_szFtpAdrs,pcDialogSlctFtpAdrs->m_csFtpFolder.GetBuffer(RECMAX));
		_tcscpy_s(m_szBaseName,pcDialogSlctFtpAdrs->m_csFileName.GetBuffer(RECMAX));
	}
	delete(pcDialogSlctFtpAdrs);

}

void CCmdEFtp::SmsoutSendData()
{
	int		cntrkind;
	TCHAR	tmppath[FNAMEMAX];
	TCHAR	szFileNameCmp[FNAMEMAX];
	TCHAR	szFileNameBmp[FNAMEMAX];
	TCHAR	szFileNameDspBmp[FNAMEMAX];
	TCHAR	szFileNameZip[FNAMEMAX];
	TCHAR	szFileNameDspZip[FNAMEMAX];
	TCHAR	szFileNameCrct[FNAMEMAX];
	TCHAR	szBaseName[RECMAX];
	DBL		dWorkXSize, dWorkYSize;
	DBL		dpi;
	TCHAR	buf[RECMAX];

	poutcntrkindget(&cntrkind);
	_stprintf_s(szBaseName, _T("%s%02d"), m_szBaseName, m_nCrtNo);
	m_nCrtNo++;
	senvgettmppath(tmppath);
	scrngetmmsize(&dWorkXSize, &dWorkYSize);
	_stprintf_s(szFileNameCmp, _T("%s\\%s.cmp"), tmppath, szBaseName);
	_stprintf_s(szFileNameBmp, _T("%s\\%s.bmp"), tmppath, szBaseName);
	SaveBmpFile(szFileNameBmp, 360.0);
	if(m_pcView->m_bEpsonYoko == TRUE){
		if(dWorkXSize > dWorkYSize){
			/* // 修正必要
			HANDLE		hndle, nhndle;
			int		mode;
			hndle = ImgKitFileLoad(szFileNameBmp, "", "");
			nhndle = ImgKitRotation(hndle, "", "", 2);
			ImgKitBmpFileSave(szFileNameBmp, nhndle, "", "", 0);
			*/
		}
	}
	BmpToCmpBytelen(szFileNameBmp, szFileNameCmp);
	_stprintf_s(szFileNameDspBmp, _T("%s\\%s.bmp"), tmppath, szBaseName);
	dpi = 700 * 25.4 / dWorkYSize;
	SaveDspBmpFile(szFileNameDspBmp, dpi);
	_tchdir(tmppath);
	_stprintf_s(szFileNameZip, _T("%s.zip"), szBaseName);
	_stprintf_s(szFileNameDspZip, _T("%sDSP.zip"), szBaseName);
	SrcZipConv(tmppath, szFileNameCmp, szFileNameZip);
	SrcZipConv(tmppath, szFileNameDspBmp, szFileNameDspZip);

	SendFileForFtp(szFileNameZip, m_szFtpAdrs);
	SendFileForFtp(szFileNameDspZip, m_szFtpAdrs);

	_tcscpy_s(szFileNameCrct, _T("crct.lst"));
	FILE *fp;
	_tfopen_s(&fp, szFileNameCrct, _T("r+"));
	if(fp == NULL){
		return;
	}
	fseek(fp, 0L, 2);
	_stprintf_s(buf, _T("%s\n"), szBaseName);
	baselibFputs(buf, fp);
	fclose(fp);
}

void CCmdEFtp::SmsoutCrctFileEnd()
{
	TCHAR	tmppath[FNAMEMAX];
	TCHAR	szFileNameCrct[FNAMEMAX];

	_tchdir(tmppath);
	_tcscpy_s(szFileNameCrct, _T("crct.lst"));
	SendFileForFtp(szFileNameCrct, m_szFtpAdrs);
	::AfxMessageBox(_T("FTP転送が終了しました"), MB_OK);
}

void CCmdEFtp::SaveBmpFile(LPTSTR lpFileName, DBL dpi)
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

void CCmdEFtp::SaveBmpFileRv(LPTSTR lpFileName, DBL dpi)
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

void CCmdEFtp::SaveDspBmpFile(LPTSTR lpFileName, DBL dpi)
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
void CCmdEFtp::SaveDspBmpFileRv(LPTSTR lpFileName, DBL dpi)
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
void CCmdEFtp::BmpToCmpBytelen(LPTSTR lpBmpFName, LPTSTR lpCmpFName)
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

	_tfopen_s(&ifp, lpBmpFName, _T("rb"));
	if(ifp == NULL){
		return;
	}
	_tfopen_s(&ofp, lpCmpFName, _T("wb"));
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

void CCmdEFtp::CmpToBmpBytelen(LPTSTR lpCmpFName, LPTSTR lpBmpFName)
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
	//BYTE	img[0x100];

	_tfopen_s(&ifp, lpCmpFName, _T("rb"));
	if(ifp == NULL){
		return;
	}
	_tfopen_s(&ofp, lpBmpFName, _T("wb"));
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
		memset(lpDst, _T('\0'), xbyte);
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

void CCmdEFtp::SrcZipConv(LPTSTR lpWorkPath, LPTSTR lpSrcFile, LPTSTR lpZipFile)
{
	// 修正必要
	/*
	HINSTANCE	hZip;
	ZPINITFUNC	ZpInit;
	ZPSETOPTIONSFUNC	ZpSetOptions;
	ZPARCHIVEFUNC	ZpArchive;
	ZIPUSERFUNCTIONS sZipUserFunctions;
	ZPOPT			sZpOpt;
	ZCL				sZcl;
	int				ret;
	
	//Zip32j.dllのロード
	hZip = LoadLibrary(_T("zip32.dll"));
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
			sZpOpt.fRecurse = 1;           // ディレクトリを再帰的に処理するかのフラグ: 1 = "-r", 2 = "-R"
			sZpOpt.szRootDir = lpWorkPath;

			ZeroMemory(&sZcl, sizeof(ZCL));
			sZcl.argc = 1;
			sZcl.FNV = &lpSrcFile;
			sZcl.lpszZipFN = lpZipFile;

			ZpInit(&sZipUserFunctions);
			ZpSetOptions(&sZpOpt);
			ret = ZpArchive(sZcl);
		}else{
			AfxMessageBox(_T("ZpInit()APIが存在しません。DLLが壊れています。"), MB_OK);
		}
	}else{
		AfxMessageBox(_T("ZIP32.dllがありません。"), MB_OK);
	}
	FreeLibrary(hZip);
	*/
}

void CCmdEFtp::SendFileForFtp(LPTSTR lpFileName, LPTSTR lpFtpFolder)
{
	CString	csPath;

//assumes server and file names have been initialized
	CInternetSession session(_T("FTP Session"));
	CFtpConnection* pcConn = NULL;

// 接続サーバ
	pcConn = session.GetFtpConnection(m_strFtpRoot, m_strFtpID, m_strFtpPW, 21, INTERNET_FLAG_PASSIVE);
	csPath.Format(_T("%s/%s"), m_strFtpPath, lpFtpFolder);
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

int WINAPI defaultEPrintProc(LPTSTR buff, unsigned long size)
{
	//printf("%s", buff);
	return (unsigned int)size;
}

int WINAPI defaultECommentProc(TCHAR* buff)
{
	buff[0] = _T('\0');
	return true;
}

int WINAPI defaultEPasswordProc(LPTSTR p, int n, LPCSTR m, LPCSTR name)
{
	return 1;
}

