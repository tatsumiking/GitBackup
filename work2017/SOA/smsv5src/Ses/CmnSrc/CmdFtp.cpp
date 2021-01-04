
/////////////////////////////////////////////////////////////////////////////
// CCmdFtp

#include "stdafx.h"
#include <afxinet.h>
#include "mapi.h"
#include "io.h"
#include "direct.h"
#include "zip32.h"


#include "CmdBase.h"
#include "CmdBaseBmp.h"
#include "CmdFtp.h"

#include "BmpFile.h"
#include "DialogSlctFtpAdrs.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
//}

typedef int (WINAPI *ZPINITFUNC)(LPZIPUSERFUNCTIONS);
typedef BOOL (WINAPI *ZPSETOPTIONSFUNC)(LPZPOPT);
typedef int (WINAPI *ZPARCHIVEFUNC)(ZCL);

/** Zip32.dllから呼び出されるコールバック関数 */
int WINAPI defaultPrintProc(LPTSTR, unsigned long);
int WINAPI defaultPasswordProc(LPTSTR, int, LPCSTR, LPCSTR);
int WINAPI defaultCommentProc(LPTSTR);

CCmdFtp::CCmdFtp(CScrollView* pcview)
:CCmdBaseBmp(pcview)
{
#ifdef _DEBUG
	_tcscpy_s(m_strFtpRoot, _T("sv58.wadax.ne.jp"));
	_tcscpy_s(m_strFtpPath, _T("public_html/upload"));
	_tcscpy_s(m_strFtpID, _T("c-alfo-com"));
	_tcscpy_s(m_strFtpPW, _T("VTqR7aAQ"));
#else
	//m_pcView = (CTonView*)pcview;
	
	_tcscpy_s(m_strFtpRoot, _T("soa-nahuda.sakura.ne.jp"));
	_tcscpy_s(m_strFtpPath, _T("www/upload"));
	_tcscpy_s(m_strFtpID, _T("soa-nahuda"));
	_tcscpy_s(m_strFtpPW, _T("w5xa7trte9"));
#endif	
}

UINT CCmdFtp::ExecMltprnFtp(UINT event, DBL dx, DBL dy)
{
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
	_stprintf_s(szBaseName, _T("%s00"), m_szBaseName);
	
	senvgettmppath(tmppath);
	scrngetmmsize(&dWorkXSize, &dWorkYSize);
	if(dWorkXSize > dWorkYSize){
		dpi = 500 * 25.4 / dWorkXSize;
	}else{
		dpi = 700 * 25.4 / dWorkYSize;
	}
	_stprintf_s(szFileNameCmp, _T("%s\\%s.cmp"), tmppath, szBaseName);
	_stprintf_s(szFileNameBmp, _T("%s\\%s.bmp"), tmppath, szBaseName);
	SaveBmpFile(szFileNameBmp, 150.0);
	//SaveBmpFile(szFileNameBmp, 180.0);
	BmpToCmpBytelen(szFileNameBmp, szFileNameCmp);
	_stprintf_s(szFileNameDspBmp, _T("%s\\%s.bmp"), tmppath, szBaseName);
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
	_tcscpy_s(szFileNameCrct, _T("crct.lst"));
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

void CCmdFtp::SmsoutCrctFileInit()
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

void CCmdFtp::SmsoutSendData()
{
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

	_stprintf_s(szBaseName, _T("%s%02d"), m_szBaseName, m_nCrtNo);
	m_nCrtNo++;
	senvgettmppath(tmppath);
	scrngetmmsize(&dWorkXSize, &dWorkYSize);
	if(dWorkXSize > dWorkYSize){
		dpi = 500 * 25.4 / dWorkXSize;
	}else{
		dpi = 700 * 25.4 / dWorkYSize;
	}
	_stprintf_s(szFileNameCmp, _T("%s\\%s.cmp"), tmppath, szBaseName);
	_stprintf_s(szFileNameBmp, _T("%s\\%s.bmp"), tmppath, szBaseName);
	SaveBmpFile(szFileNameBmp, 150.0);
	BmpToCmpBytelen(szFileNameBmp, szFileNameCmp);
	_stprintf_s(szFileNameDspBmp, _T("%s\\%s.bmp"), tmppath, szBaseName);
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

void CCmdFtp::SmsoutCrctFileEnd()
{
	TCHAR	tmppath[FNAMEMAX];
	TCHAR	szFileNameCrct[FNAMEMAX];

	_tchdir(tmppath);
	_tcscpy_s(szFileNameCrct, _T("crct.lst"));
	SendFileForFtp(szFileNameCrct, m_szFtpAdrs);
	::AfxMessageBox(_T("FTP転送が終了しました"), MB_OK);
}

void CCmdFtp::SaveBmpFile(LPTSTR lpFileName, DBL dpi)
{
	DBL		xsize, ysize;
	int		xdot, ydot;
	CBmpFile	*pcBmpFile;

	scrngetmmsize(&xsize, &ysize);
	xdot = (int)(xsize / 25.4 * dpi);
	ydot = (int)(ysize / 25.4 * dpi);
	pcBmpFile = new CBmpFile(m_pcView);
	pcBmpFile->CreateBmpInit(xdot, ydot);
	pcBmpFile->CreateBmpOnebitImageRV(lpFileName);
	pcBmpFile->CreateBmpEnd();
	delete(pcBmpFile);

}

void CCmdFtp::SaveDspBmpFile(LPTSTR lpFileName, DBL dpi)
{
	DBL		xsize, ysize;
	int		xdot, ydot;
	CBmpFile	*pcBmpFile;

	scrngetmmsize(&xsize, &ysize);
	xdot = (int)(xsize / 25.4 * dpi);
	ydot = (int)(ysize / 25.4 * dpi);
	pcBmpFile = new CBmpFile(m_pcView);
	pcBmpFile->CreateBmpInit(xdot, ydot);
	pcBmpFile->CreateBmpOnebitImage(lpFileName);
	pcBmpFile->CreateBmpEnd();
	delete(pcBmpFile);

}

void CCmdFtp::BmpToCmpBytelen(LPTSTR lpBmpFName, LPTSTR lpCmpFName)
{
	FILE	*ifp;
	FILE	*ofp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	RGBQUAD	clr[2];
	int		x, y, idx;
	int		xdot, ydot, xbyte;
	BYTE	imgSrc[RECMAX2];
	BYTE	imgTwo[RECMAX2];
	BYTE	imgDst[RECMAX2];
	int		dy, dstxdot, dstydot;
	int		dst, src, cnt, flag;
	int		dstmask, srcmask;
	int		linecnt, size, last, sumsize;
	TCHAR	msg[RECMAX];

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
	fread(clr, sizeof(RGBQUAD), 2, ifp);

	xdot = bmpinfo.bmiHeader.biWidth;
	ydot = bmpinfo.bmiHeader.biHeight;
	xbyte = xdot / 8;

	dstxdot = xdot * 2;
	dstydot = ydot * 2;
	Setint32(imgDst, 0, 0);
	Setint32(imgDst, 4, dstxdot);
	Setint32(imgDst, 8, dstydot);
	fwrite(imgDst, 12, 1, ofp);
	sumsize = 12;
	linecnt = 0;
	dy = 0;
	for(y = 0; y < ydot; y++){
		fread(imgSrc, 1, xbyte, ifp);
		for(x = 0; x < xbyte; x++){
			imgSrc[x] = ~(imgSrc[x]);
		}
		memset(imgTwo, 0, RECMAX2);
		_stprintf_s(msg, _T("%d/%d, %d/%d\n"), y, ydot, dy, dstydot);
		TRACE(msg);

		srcmask = 0x0080;
		dstmask = 0x0080;
		src = 0;
		dst = 0;
		last = 0;
		for(x = 0; x < xdot; x++){
			if(imgSrc[src] & srcmask){
				imgTwo[dst] |= dstmask;
				dstmask >>= 1;
				imgTwo[dst] |= dstmask;
				dstmask >>= 1;
				last = dst;
			}else{
				dstmask >>= 2;
			}
			if(dstmask == 0){
				dst++;
				dstmask = 0x0080;
			}
			srcmask >>= 1;
			if(srcmask == 0){
				src++;
				srcmask = 0x0080;
			}
		}
		if(last == 0){
			linecnt++;
			linecnt++;
			if (linecnt >= 255){ // 改行数をセット
				imgDst[0] = 0; imgDst[1] = 0; imgDst[2] = 0xff;
				fwrite(imgDst, 3, 1, ofp);
				sumsize += 3;
				linecnt = 0;
				dy += 255;
			}
		}else{
			if(linecnt != 0){
				imgDst[0] = 0; imgDst[1] = 0; imgDst[2] = (BYTE)linecnt;
				fwrite(imgDst, 3, 1, ofp);
				sumsize += 3;
				linecnt = 0;
				dy += linecnt;
			}
			last++;	// バイト位置は0から始まるためバイト数は１プラス
			flag = -1;
			cnt = 0;
			dst = 4; // 0,1は一行のサイズ, 2,3は展開した後のバイト数
			for (x = 0; x < last; x++){
				if(imgTwo[x] == 0){
					if(flag == -1){ // 前が0x00のとき
						cnt++;
						if(cnt >= 255){ // 0x00を255個
							imgDst[dst++] = 0x00; imgDst[dst++] = 0xff;
							cnt = 0;
						}
					}else if(flag == 1){ // 前が0xffのとき0xffの個数をセット
						imgDst[dst++] = 0xff; imgDst[dst++] = (byte)cnt;
						cnt = 1; // 0x00の個数
					}else{ // それ以外
						cnt = 1;
					}
					flag = -1;
				}else if(imgTwo[x] == 0xff){
					if(flag == 1){ // 前が0xffのとき
						cnt++;
						if(cnt >= 255){ // 0xffを255個
							imgDst[dst++] = 0xff; imgDst[dst++] = 0xff;
							cnt = 0;
						}
					}else if(flag == -1){ // 前が0x00のとき個数をセット
						imgDst[dst++] = 0x00; imgDst[dst++] = (byte)cnt;
						cnt = 1; // 0xffの個数
					}else{ // それ以外
						cnt = 1;
					}
					flag = 1;
				}else{
					if(flag == -1){ // 前が0x00のとき個数をセット
						imgDst[dst++] = 0x00; imgDst[dst++] = (byte)cnt;
						cnt = 1; // 個数
					}else if(flag == 1){ // 前が0xffのとき個数をセット
						imgDst[dst++] = 0xff; imgDst[dst++] = (byte)cnt;
						cnt = 1; // 個数
					}
					imgDst[dst++] = imgTwo[x];
					flag = 0;
				}
			}
			if(flag == -1){ // 前が0x00のとき個数をセット
				imgDst[dst++] = 0x00; imgDst[dst++] = (byte)cnt;
			}else if(flag == 1){ // 前が0xffのとき個数をセット
				imgDst[dst++] = 0xff; imgDst[dst++] = (byte)cnt;
			}
			size = dst - 4;
			Setint16(imgDst, 0, size);
			Setint16(imgDst, 2, last);
			fwrite(imgDst, dst, 1, ofp);
			fwrite(imgDst, dst, 1, ofp);
			sumsize += (dst * 2);
			dy += 2;
		}
	}
	if(linecnt != 0){
		imgDst[0] = 0; imgDst[1] = 0; imgDst[2] = (BYTE)linecnt;
		fwrite(imgDst, 3, 1, ofp);
		sumsize += 3;
		linecnt = 16;
		imgDst[0] = 0; imgDst[1] = 0; imgDst[2] = (BYTE)linecnt;
		fwrite(imgDst, 3, 1, ofp);
		sumsize += 3;
	}
    for (idx = 0; idx < 32; idx++)
    {
        imgDst[0] = 0x00;
    }
	fwrite(imgDst, 32, 1, ofp);
	size = 32 - (sumsize % 32);
	fwrite(imgDst, size, 1, ofp);
	fclose(ifp);
	fclose(ofp);
	_tfopen_s(&ofp, lpCmpFName, _T("r+b"));
	if(ofp == NULL){
		return;
	}
	Setint32(imgDst, 0, size);
	fwrite(imgDst, 4, 1, ofp);
	fclose(ofp);
}

void CCmdFtp::CmpToBmpBytelen(LPTSTR lpCmpFName, LPTSTR lpBmpFName)
{
	FILE	*ifp;
	FILE	*ofp;
	BITMAPFILEHEADER	bif;
	BITMAPINFOHEADER	bih;
	RGBQUAD				clr[2];
	int		xdot, ydot, xbyte;
	int		headsize;
	int		y;
	BYTE	imgSrc[RECMAX2];
	BYTE	imgDst[RECMAX2];
	int		nLen, nCnt, nRep;
	int		src, dst;
	int		i, ret;
	int		i1, i2;

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
	i1 = (imgSrc[4] & 0x00ff); i2 = (imgSrc[5] & 0x00ff);
	xdot = i1+ i2 * 0x100;
	i1 = (imgSrc[8] & 0x00ff); i2 = (imgSrc[9] & 0x00ff);
	ydot = i1+ i2 * 0x100;
	xbyte = ((xdot+7)/8+3)/4*4;
	headsize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*2;
	memset(&bif, 0, sizeof(BITMAPFILEHEADER));
	memset(&bih, 0, sizeof(BITMAPINFOHEADER));
	bif.bfType = 0x4D42;
	bif.bfSize = xbyte * ydot + headsize;
	bif.bfOffBits = headsize;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = xdot;
	bih.biHeight = ydot;
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

	fwrite(&bif, sizeof(BITMAPFILEHEADER), 1, ofp);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, ofp);
	fwrite(clr, sizeof(RGBQUAD), 2, ofp);
	y = 0;
	while(1){
       	if(y >= ydot){
       		break;
       	}
		ret = fread(imgSrc, 1, 2, ifp);
   		if(ret == 0){
   			break;
   		}
		if(imgSrc[0] == 0 && imgSrc[1] == 0){
			fread(imgSrc, 1, 1, ifp);
			nRep = ((imgSrc[0]) & 0x00ff);
			memset(imgDst, 0, xbyte);
			for(i = 0; i < nRep; i++){
				fwrite(imgDst, 1, xbyte, ofp);
			}
			y += nRep;
		}else{
			i1 = (imgSrc[0]) & 0x00ff; i2 = (imgSrc[1]) & 0x00ff;
			nCnt = i1 + i2 * 0x0100;
			fread(imgSrc, 1, 2, ifp);
			i1 = (imgSrc[0]) & 0x00ff; i2 = (imgSrc[1]) & 0x00ff;
			nLen = i1 + i2 * 0x0100;
			fread(imgSrc, 1, nCnt, ifp);
			src = 0;
			dst = 0;
			memset(imgDst, 0, xbyte);
			while(1){
				if (src >= nCnt){
					break;
				}
				if(imgSrc[src] == 0x00){
					src++;
					nRep = (imgSrc[src++]) & 0x00ff;
					for (i = 0; i < nRep; i++){
						imgDst[dst++] = 0x00;
					}
				}else if(imgSrc[src] == 0xff){
					src++;
					nRep = (imgSrc[src++]) & 0x00ff;
					for (i = 0; i < nRep; i++){
						imgDst[dst++] = 0xff;
					}
				}else{
					imgDst[dst++] = imgSrc[src++];
				}
			}
			fwrite(imgDst, 1, xbyte, ofp);
			y++;
		}
	}
	fclose(ifp);
	fclose(ofp);
}

void CCmdFtp::SrcZipConv(LPTSTR lpWorkPath, LPTSTR lpSrcFile, LPTSTR lpZipFile)
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
		ZpInit = (ZPINITFUNC)GetProcAddress(hZip, _T("ZpInit"));
		ZpSetOptions = (ZPSETOPTIONSFUNC)GetProcAddress(hZip, _T("ZpSetOptions"));
		ZpArchive = (ZPARCHIVEFUNC)GetProcAddress(hZip, _T("ZpArchive"));
		if (ZpInit != NULL){
			ZeroMemory(&sZipUserFunctions, sizeof(ZIPUSERFUNCTIONS));
			sZipUserFunctions.print = defaultPrintProc;
			sZipUserFunctions.comment = defaultCommentProc;
			sZipUserFunctions.password = defaultPasswordProc;
			ZeroMemory(&sZpOpt, sizeof(ZPOPT));
			sZpOpt.fRecurse = 1;           // ディレクトリを再帰的に処理するかのフラグ: 1 = "-r", 2 = "-R"			sZpOpt.szRootDir = lpWorkPath;

			ZeroMemory(&sZcl, sizeof(ZCL));
			sZcl.argc = 1;
			sZcl.FNV = &lpSrcFile;
			sZcl.lpszZipFN = lpZipFile;

			ZpInit(&sZipUserFunctions);
			ZpSetOptions(&sZpOpt);
			ret = ZpArchive(sZcl);
		}else{
			AfxMessageBox(_T("ZpInit()APIが存在しません。DLLがぶっ壊れています。"), MB_OK);
		}
	}else{
		AfxMessageBox(_T("ZIP32.dllがありません。"), MB_OK);
	}
	FreeLibrary(hZip);
	*/
}

void CCmdFtp::SendFileForFtp(LPTSTR lpFileName, LPTSTR lpFtpFolder)
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

void CCmdFtp::Setint32(BYTE buf[], int offset, int num)
{
	buf[offset++] = (BYTE)(num & 0x00ff);
	buf[offset++] = (BYTE)((num >> 8) & 0x00ff);
	buf[offset++] = (BYTE)((num >> 16) & 0x00ff);
	buf[offset++] = (BYTE)((num >> 24) & 0x00ff);
}

void CCmdFtp::Setint16(BYTE buf[], int offset, int num)
{
	buf[offset++] = (BYTE)(num & 0x00ff);
	buf[offset++] = (BYTE)((num >> 8) & 0x00ff);
}

int WINAPI defaultPrintProc(LPTSTR buff, unsigned long size)
{
	//printf(_T("%s"), buff);
	return (unsigned int)size;
}

int WINAPI defaultCommentProc(TCHAR* buff)
{
	buff[0] = _T('\0');
	return true;
}

int WINAPI defaultPasswordProc(LPTSTR p, int n, LPCSTR m, LPCSTR name)
{
	return 1;
}

