
/////////////////////////////////////////////////////////////////////////////
// CCmdSend

#include "stdafx.h"
#include "math.h"
#include "mapi.h"
#include "direct.h"

#include "cmd_base.h"
#include "cmdbsbmp.h"
#include "cmd_send.h"
#include "bmpfile.h"
#include "DSlctMAdrs.h"
#include "..\ses\fileeps.h"

extern "C" {
	void senvgettmppath(LPSTR path);
	void senvgetexepath(LPSTR path);
	void scrngetmmsize(DBL *scrnxsize, DBL *scrnysize);
}

typedef int (*ZipProc)(const HWND hwnd, LPCSTR szCmdLine, LPSTR szOutput, const DWORD dwSize);
typedef int (*TarProc)(const HWND hwnd, LPCSTR szCmdLine, LPSTR szOutput, const DWORD dwSize);

#define	MAILMAX	20

CCmdSend::CCmdSend(CScrollView* pcview)
:CCmdBaseBmp(pcview)
{
	//m_pcView = (CTonView*)pcview;
}

UINT CCmdSend::ExecEpsSend(UINT event, DBL dx, DBL dy)
{
	char	exepath[FNAMEMAX];
	char	szSendEnvFile[FNAMEMAX];
	FILE	*fp;
	char	szBuf[RECMAX];
	char	tmppath[FNAMEMAX];
	char	szName[QRECMAX];
	char	szFileName[FNAMEMAX];
	char	szMailAdrs[RECMAX];
	char	szSubject[RECMAX];
	char	szMsg[RECMAX];
	int		ret;

	senvgetexepath(exepath);
	sprintf(szSendEnvFile, "%s\\SendMail.env", exepath);
	fp = fopen(szSendEnvFile, "r");
	if(fp == NULL){
		strcpy(szMailAdrs, "mobile@c-alfo.com");
	}else{
		fgets(szBuf, RECMAX, fp);
		szBuf[strlen(szBuf)-1] = '\0';
		strcpy(szMailAdrs, szBuf);
		fclose(fp);
	}
	ULONG (PASCAL *lpfnSendMail)(ULONG, ULONG, MapiMessage*, FLAGS, ULONG);
	HMODULE hInstMail;
	MapiMessage message;
	MapiFileDesc fileDesc;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	strcpy(szName, "test.eps");
	senvgettmppath(tmppath);
	sprintf(szFileName, "%s\\test.eps", tmppath, szName);

	SaveEpsFile(szFileName);

	memset(&fileDesc, 0, sizeof(fileDesc));
	fileDesc.nPosition = (ULONG)-1;
	fileDesc.lpszPathName = szFileName;
	fileDesc.lpszFileName = szName;

	memset(&message, 0, sizeof(message));
	message.nFileCount = 1;
	message.lpFiles = &fileDesc;

	MapiRecipDesc aRecips;
	message.nRecipCount = 1;
	memset((void*)&aRecips, 0, sizeof(MapiRecipDesc));
	aRecips.ulRecipClass = 1;
	aRecips.lpszName = szMailAdrs;
	aRecips.lpszAddress = szMailAdrs;
	message.lpRecips = &aRecips;

	strcpy(szSubject, "test");
	message.lpszSubject = szSubject;
	strcpy(szMsg, "お疲れ様です\n" );
	message.lpszNoteText = szMsg;

	hInstMail = LoadLibrary("MAPI32.DLL");
	(FARPROC&)lpfnSendMail = GetProcAddress(hInstMail, "MAPISendMail");
	ret = lpfnSendMail(0L, (ULONG)(m_pcView->m_hWnd),
		&message, MAPI_LOGON_UI|MAPI_DIALOG, 0L);
	FreeLibrary(hInstMail);

	return(ENDEVENT);
}

UINT CCmdSend::ExecMprnSend(UINT event, DBL dx, DBL dy)
{
	CDialogSlctMailAdrs	*pcDialogSlctMailAdrs;
	char	szBuf[RECMAX];
	char	tmppath[FNAMEMAX];
	char	szName[QRECMAX];
	char	szFileNameGz[FNAMEMAX];
	char	szFileNameZip[FNAMEMAX];
	char	szFileNameTar[FNAMEMAX];
	char	szFileNameCmp[FNAMEMAX];
	char	szFileNameBmp[FNAMEMAX];
	char	szMailAdrs[RECMAX];
	char	szSubject[RECMAX];
	char	szMsg[RECMAX];
	int		ret;
	ULONG (PASCAL *lpfnSendMail)(ULONG, ULONG, MapiMessage*, FLAGS, ULONG);
	HMODULE hInstMail;
	MapiMessage message;
	MapiFileDesc fileDesc;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();

	pcDialogSlctMailAdrs = new CDialogSlctMailAdrs(m_pcView);
	pcDialogSlctMailAdrs->m_csSubject = m_pcDoc->GetTitle();
	ret = pcDialogSlctMailAdrs->DoModal();
	if(ret == IDOK){
		strcpy(szMailAdrs,pcDialogSlctMailAdrs->m_csAdrs.GetBuffer(RECMAX));
		strcpy(szSubject,pcDialogSlctMailAdrs->m_csSubject.GetBuffer(RECMAX));
	}
	delete(pcDialogSlctMailAdrs);
	if(ret != IDOK){
		return(ENDEVENT);
	}

	senvgettmppath(tmppath);
	//chdir(tmppath);
	strcpy(szName, "test.cmp");
	sprintf(szFileNameCmp, "%s\\%s", tmppath, szName);
	sprintf(szFileNameBmp, "%s\\test.bmp", tmppath);
	SaveBmpFile(szFileNameBmp, 150.0);
	//BmpGzConv(szFileNameBmp, szFileNameGz);
	//BmpTarConv(szFileNameBmp, szFileNameTar);
	//BmpZipConv(szFileNameBmp, szFileNameZip);
	BmpCmpConvBytelen(szFileNameBmp, szFileNameCmp);
#ifdef _DEBUG
	char	szBmpFile[FNAMEMAX];
	sprintf(szBmpFile, "%s\\testcmp.bmp", tmppath);
	CmpBmpConvBytelen(szFileNameCmp, szBmpFile);
#endif

	memset(&fileDesc, 0, sizeof(fileDesc));

	fileDesc.nPosition = (ULONG)-1;
	fileDesc.lpszPathName = szFileNameCmp;
	fileDesc.lpszFileName = szName;

	memset(&message, 0, sizeof(message));
	message.nFileCount = 1;
	message.lpFiles = &fileDesc;

	MapiRecipDesc aRecips;
	message.nRecipCount = 1;
	memset((void*)&aRecips, 0, sizeof(MapiRecipDesc));
	aRecips.ulRecipClass = 1;
	aRecips.lpszName = szMailAdrs;
	aRecips.lpszAddress = szMailAdrs;
	message.lpRecips = &aRecips;

	message.lpszSubject = szSubject;
	strcpy(szMsg, "お疲れ様です\n" );
	message.lpszNoteText = szMsg;

	hInstMail = LoadLibrary("MAPI32.DLL");
	(FARPROC&)lpfnSendMail = GetProcAddress(hInstMail, "MAPISendMail");
	ret = lpfnSendMail(0L, (ULONG)(m_pcView->m_hWnd),
		&message, MAPI_LOGON_UI|MAPI_DIALOG, 0L);
	FreeLibrary(hInstMail);

	return(ENDEVENT);
}


UINT CCmdSend::ExecPrnSend(UINT event, DBL dx, DBL dy)
{
	DBL		xsize, ysize;
	char	exepath[FNAMEMAX];
	char	szSendEnvFile[FNAMEMAX];
	FILE	*fp;
	char	szBuf[RECMAX];
	char	tmppath[FNAMEMAX];
	char	szName1[QRECMAX];
	char	szFileName1[FNAMEMAX];
	char	szName2[QRECMAX];
	char	szFileNameBmp[FNAMEMAX];
	char	szFileName2[FNAMEMAX];
	char	szMailAdrs[RECMAX];
	char	szSubject[RECMAX];
	char	szMsg[RECMAX];
	int		ret;

	scrngetmmsize(&xsize, &ysize);
	senvgetexepath(exepath);
	sprintf(szSendEnvFile, "%s\\SendMail.env", exepath);
	fp = fopen(szSendEnvFile, "r");
	if(fp == NULL){
		strcpy(szMailAdrs, "mobile@c-alfo.com");
	}else{
		fgets(szBuf, RECMAX, fp);
		szBuf[strlen(szBuf)-1] = '\0';
		strcpy(szMailAdrs, szBuf);
		fclose(fp);
	}
	ULONG (PASCAL *lpfnSendMail)(ULONG, ULONG, MapiMessage*, FLAGS, ULONG);
	HMODULE hInstMail;
	MapiMessage message;
	MapiFileDesc fileDesc[3];

	if(event != INITEVENT){
		return(0);
	}
	InitClass();

	senvgettmppath(tmppath);

	strcpy(szName1, "test.prn");
	sprintf(szFileName1, "%s\\%s", tmppath, szName1);
	strcpy(szName2, "test.cmp");
	sprintf(szFileName2, "%s\\%s", tmppath, szName2);
	sprintf(szFileNameBmp, "%s\\test.bmp", tmppath);

	SavePrnFile(szFileName1);
	//if(ysize > 297){
	//	SaveBmpFile(szFileNameBmp, 20.0);
	//}else{
		SaveBmpFile(szFileNameBmp, 50.0);
	//}
	BmpCmpConvBytelen(szFileNameBmp, szFileName2);
	memset(&fileDesc, 0, sizeof(fileDesc));

	fileDesc[0].nPosition = (ULONG)-1;
	fileDesc[0].lpszPathName = szFileName1;
	fileDesc[0].lpszFileName = szName1;

	fileDesc[1].nPosition = (ULONG)-1;
	fileDesc[1].lpszPathName = szFileName2;
	fileDesc[1].lpszFileName = szName2;

	memset(&message, 0, sizeof(message));
	message.nFileCount = 2;
	message.lpFiles = fileDesc;

	MapiRecipDesc aRecips[2];
	message.nRecipCount = 1;
	memset((void*)aRecips, 0, sizeof(MapiRecipDesc)*2);
	aRecips[0].ulRecipClass = 1;
	aRecips[0].lpszName = szMailAdrs;
	aRecips[0].lpszAddress = szMailAdrs;
	message.lpRecips = aRecips;

	strcpy(szSubject, "test");
	message.lpszSubject = szSubject;
	strcpy(szMsg, "お疲れ様です\n" );
	message.lpszNoteText = szMsg;

	hInstMail = LoadLibrary("MAPI32.DLL");
	(FARPROC&)lpfnSendMail = GetProcAddress(hInstMail, "MAPISendMail");
	ret = lpfnSendMail(0L, (ULONG)(m_pcView->m_hWnd),
		&message, MAPI_LOGON_UI|MAPI_DIALOG, 0L);
	FreeLibrary(hInstMail);

	return(ENDEVENT);
}

void CCmdSend::SaveBmpFile(LPSTR lpFileName, DBL dpi)
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

void CCmdSend::SaveEpsFile(LPSTR fname)
{
	SaveUndo();
	ActiveDataChange(SWAKVECTID);
	CFileEps*	pcFileEps;
	pcFileEps = new CFileEps(m_pcView);
	pcFileEps->SaveData(fname);
	delete(pcFileEps);
	LoadUndo();
}

void CCmdSend::SavePrnFile(LPSTR fname)
{
	HANDLE	hDevMode;
	HANDLE	hDevNames;
	DEVMODE		*lpDevMode;
	DEVNAMES	*lpDevNames;
	char	szDevice[QRECMAX];
	char	szDriver[QRECMAX];
	char	szOutput[QRECMAX];
	CDC		cDC;
	int		ret;

	hDevMode = m_pcApp->GetDevMode();
	hDevNames = m_pcApp->GetDevNames();
	if(hDevNames == 0){
		return;
	}
	lpDevNames = (DEVNAMES *)GlobalLock(hDevNames);
	if(lpDevNames == NULL){
		return;
	}
	strcpy(szDevice, ((LPSTR)(lpDevNames) + lpDevNames->wDeviceOffset));
	strcpy(szDriver, ((LPSTR)(lpDevNames) + lpDevNames->wDriverOffset));
	strcpy(szOutput, ((LPSTR)(lpDevNames) + lpDevNames->wOutputOffset));
	strcpy(szOutput, fname);
	GlobalUnlock(hDevNames);
	lpDevMode = (DEVMODE *)GlobalLock(hDevMode);
	if(lpDevMode == NULL){
		return;
	}
	ret = cDC.CreateDC(szDriver, szDevice, szOutput, lpDevMode);
	GlobalUnlock(hDevMode);
	if(ret == 0){
		return;
	}
	m_pcView->PrintOutDC(&cDC);
	cDC.DeleteDC();
}

void CCmdSend::BmpCmpConvBytelen(LPSTR lpBmpFName, LPSTR lpCmpFName)
{
	FILE	*ifp;
	FILE	*ofp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	RGBQUAD	clr[2];
	int		x, y;
	int		xdot, ydot, xbyte;
	BYTE	imgSrc[RECMAX2];
	BYTE	imgDst[RECMAX2];
	int		btDst, cnt, flag;
	int		btSrc;

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
	fread(clr, sizeof(RGBQUAD), 2, ifp);

	xdot = bmpinfo.bmiHeader.biWidth;
	ydot = bmpinfo.bmiHeader.biHeight;
	xbyte = xdot / 8;

	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, ofp);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, ofp);
	fwrite(clr, sizeof(RGBQUAD), 2, ofp);
	for(y = 0; y < ydot; y++){
		fread(imgSrc, 1, xbyte, ifp);
		flag = 1;
		cnt = 0;
		btSrc = 0;
		btDst = 1;
		for(btSrc = 0; btSrc < xbyte; btSrc++){
			if(imgSrc[btSrc] == 0){
				if(flag == -1){
					cnt++;
				}else if(flag == 1){
					imgDst[btDst++] = 0xff;
					imgDst[btDst++] = cnt;
					cnt = 1;
				}else{
					cnt = 1;
				}
				flag = -1;
			}else if(imgSrc[btSrc] == 0xff){
				if(flag == 1){
					cnt++;
				}else if(flag == -1){
					imgDst[btDst++] = 0x00;
					imgDst[btDst++] = cnt;
					cnt = 1;
				}else{
					cnt = 1;
				}
				flag = 1;
			}else{
				if(flag == -1){
					imgDst[btDst++] = 0x00;
					imgDst[btDst++] = cnt;
					cnt= 0;
				}else if(flag == 1){
					imgDst[btDst++] = 0xff;
					imgDst[btDst++] = cnt;
					cnt = 0;
				}
				imgDst[btDst++] = imgSrc[btSrc];
				flag = 0;
			}
		}
		if(flag == -1){
			imgDst[btDst++] = 0x00;
			imgDst[btDst++] = cnt;
		}else if(flag == 1){
			imgDst[btDst++] = 0xff;
			imgDst[btDst++] = cnt;
		}
		imgDst[0] = btDst-1;
		fwrite(imgDst, 1, btDst, ofp);
	}
	fclose(ifp);
	fclose(ofp);
}

void CCmdSend::CmpBmpConvBytelen(LPSTR lpCmpFName, LPSTR lpBmpFName)
{
	FILE	*ifp;
	FILE	*ofp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	RGBQUAD	clr[2];
	int		xdot, ydot, xbyte;
	int		x, y;
	BYTE	imgSrc[RECMAX2];
	BYTE	imgDst[RECMAX2];
	int		nXCnt, nCnt, nRep;
	int		btSrc, btDst;
	int		i, j;

	ifp = fopen(lpCmpFName, "rb");
	if(ifp == NULL){
		return;
	}
	ofp = fopen(lpBmpFName, "wb");
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

	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, ofp);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, ofp);
	fwrite(clr, sizeof(RGBQUAD), 2, ofp);
	for(y = 0; y < ydot; y++){
		fread(&nXCnt, 1, 1, ifp);
		nXCnt &= 0x00ff;
		if(nXCnt == 0){
			fread(&nRep, 1, 1, ifp);
			nRep &= 0x00ff;
			for(i = 0; i < nRep; i++){
				fwrite(imgDst, 1, xbyte, ofp);
			}
			y += (nRep-1);
		}else{
			fread(imgSrc, 1, nXCnt, ifp);
			memset(imgDst, 0xff, RECMAX2);
			btSrc = 0;
			btDst = 0;
			for(i = 0; i < nXCnt; i++){
				if(imgSrc[i] == 0xff){
					i++;
					nCnt = (imgSrc[i] & 0x00ff);
					for(j = 0; j < nCnt; j++){
						imgDst[btDst++] = 0xff;
					}
				}else if(imgSrc[i] == 0x00){
					i++;
					nCnt = (imgSrc[i] & 0x00ff);
					for(j = 0; j < nCnt; j++){
						imgDst[btDst++] = 0x00;
					}
				}else{
					imgDst[btDst++] = imgSrc[i];
				}
			}
			fwrite(imgDst, 1, xbyte, ofp);
		}
	}
	fclose(ifp);
	fclose(ofp);
}

void CCmdSend::BmpCmpConvBitlen(LPSTR lpBmpFName, LPSTR lpCmpFName)
{
	FILE	*ifp;
	FILE	*ofp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	RGBQUAD	clr[2];
	int		x, y;
	int		xdot, ydot, xbyte;
	BYTE	imgSrc[RECMAX4];
	BYTE	imgDst[RECMAX4];
	int		btDst, cnt, flag;
	int		btSrc, mask;

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
	fread(clr, sizeof(RGBQUAD), 2, ifp);

	xdot = bmpinfo.bmiHeader.biWidth;
	ydot = bmpinfo.bmiHeader.biHeight;
	xbyte = xdot / 8;

	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, ofp);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, ofp);
	fwrite(clr, sizeof(RGBQUAD), 2, ofp);
	for(y = 0; y < ydot; y++){
		fread(imgSrc, 1, xbyte, ifp);
		flag = ON;
		cnt = 0;
		btSrc = 0;
		btDst = 0;
		mask = 0x080;
		for(x = 0; x < xdot; x++){
			if((imgSrc[btSrc] & mask) == 0){
				if(flag == OFF){
					cnt++;
				}else{
					imgDst[btDst] = cnt;
					imgDst[btDst] |= 0x080;
					btDst++;
					flag = OFF;
					cnt = 1;
				}
			}else{
				if(flag == ON){
					cnt++;
				}else{
					imgDst[btDst] = cnt;
					btDst++;
					flag = ON;
					cnt = 1;
				}
			}
			if(120 <= cnt){
				imgDst[btDst] = cnt;
				if(flag == ON){
					imgDst[btDst] |= 0x080;
				}
				btDst++;
				cnt = 1;
			}
			mask >>= 1;
			if(mask == 0){
				btSrc++;
				mask = 0x080;
			}
		}
		imgDst[btDst] = 0xff;
		btDst++;
		fwrite(&btDst, 2, 1, ofp);
		fwrite(imgDst, btDst, 1, ofp);
	}
	fclose(ifp);
	fclose(ofp);
}

void CCmdSend::CmpBmpConvBitlen(LPSTR lpCmpFName, LPSTR lpBmpFName)
{
	FILE	*ifp;
	FILE	*ofp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	RGBQUAD	clr[2];
	int		xdot, ydot, xbyte;
	int		cnt, idx, len, i;
	int		byte, mask, x, y;
	BYTE	cmp[RECMAX2];
	BYTE	img[RECMAX2];
	CString	csStr;

	ifp = fopen(lpCmpFName, "rb");
	if(ifp == NULL){
		return;
	}
	ofp = fopen(lpBmpFName, "wb");
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

	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, ofp);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, ofp);
	fwrite(clr, sizeof(RGBQUAD), 2, ofp);
	for(y = 0; y < ydot; y++){
		cnt = 0;
		fread(&cnt, 1, 2, ifp);
		fread(cmp, 1, cnt, ifp);
		cnt = cnt & 0x00ffff; 
		//csStr.Format(CString("%d %d"), y, cnt);
		//::MessageBox(NULL, csStr, NULL, MB_OK);
		memset(img, 0, RECMAX2);
		byte = 0;
		mask = 0x080;
		for(idx = 0; idx < cnt; idx++){
			if(cmp[idx] == 0xff){
				break;
			}
			len = cmp[idx] & 0x7f;
			for(i = 0; i < len; i++){
				if((cmp[idx] & 0x080) == 0){
					img[byte] |= mask;
				}
				mask >>= 1;
				if(mask == 0){
					byte++;
					mask = 0x080;
				}
			}
		}
		for(byte = 0; byte < xbyte; byte++){
			img[byte] = ~img[byte];
		}
		fwrite(img, 1, xbyte, ofp);
	}
	fclose(ifp);
	fclose(ofp);
}

void CCmdSend::BmpZipConv(LPSTR lpBmpFile, LPSTR lpZipFile)
{
	HINSTANCE	hZip;
	ZipProc		procZip;
	char		szCmdLine[FNAMEMAX*2+16];
	char		szBuf[RECMAX4];
	int			ret;

	hZip = LoadLibrary("zip32j.dll");
	procZip = (ZipProc)GetProcAddress(hZip, "Zip");
	sprintf(szCmdLine, "-r \"%s\" \"%s\"", lpZipFile, lpBmpFile);  
	ret = (*procZip)(NULL, szCmdLine, szBuf,RECMAX4);
	FreeLibrary(hZip);
}

void CCmdSend::BmpTarConv(LPSTR lpBmpFile, LPSTR lpTarFile)
{
	HINSTANCE	hTar;
	TarProc		procTar;
	char		szCmdLine[FNAMEMAX*2+16];
	char		szBuf[RECMAX4];
	int			ret;

	hTar = LoadLibrary("tar32.dll");
	procTar = (TarProc)GetProcAddress(hTar, "Tar");
	sprintf(szCmdLine, "-pxvf \"%s\" \"%s\"", lpTarFile, lpBmpFile);  
	ret = (*procTar)(NULL, szCmdLine, szBuf, RECMAX4);
	FreeLibrary(hTar);
}

typedef LONG (__stdcall *pInitCompression)();
typedef LONG (__stdcall *pCreateCompression)(LONG *context, LONG flags);
typedef LONG (__stdcall *pCompress)(
                            LONG context,
                            const BYTE *in,
                            LONG insize,
                            BYTE *out,
                            LONG outsize,
                            LONG *inused,
                            LONG *outused,
                            LONG compression_level);
typedef LONG (__stdcall *pDestroyCompression)(LONG context);
typedef LONG (__stdcall *pDeInitCompression)();

int CCmdSend::BmpGzConv(LPSTR lpBmpFile, LPSTR lpGzFile)
{
	CFile	cpFile;
	HANDLE	hIn, hOut;
	BYTE	*pcIn, *pcOut;
	const LONG GZIP_LVL = 1;
	LONG	ctx = 0;
	LONG	inused = 0;
	LONG	outused = 0;
	LONG	nSize;

	if((cpFile.Open(lpBmpFile, CFile::modeRead)) == FALSE){
		AfxMessageBox("ＢＭＰファイルが開けません", MB_OK);
		return(FAIL);
	}
	nSize = cpFile.GetLength();
	hIn = GlobalAlloc(GHND, nSize);
	if(hIn == 0){
		return(FALSE);
	}
	hOut= GlobalAlloc(GHND, nSize);
	if(hOut == 0){
		GlobalFree(hIn);
		return(FALSE);
	}
	pcIn = (LPBYTE)GlobalLock(hIn);
	if(pcIn == NULL){
		GlobalFree(hIn);
		GlobalFree(hOut);
		return(FALSE);
	}
	pcOut = (LPBYTE)GlobalLock(hOut);
	if(pcIn == NULL){
		GlobalUnlock(hIn);
		GlobalFree(hIn);
		GlobalFree(hOut);
		return(FALSE);
	}
	if(cpFile.ReadHuge(pcIn, nSize) != nSize){
		GlobalUnlock(hIn);
		GlobalUnlock(hOut);
		GlobalFree(hIn);
		GlobalFree(hOut);
		return(FALSE);
	}
	cpFile.Close();

	memset(pcOut, 0, nSize);

    if (HINSTANCE hInst = LoadLibrary("gzip.dll"))
    {
        pInitCompression InitCompression = (pInitCompression)GetProcAddress(hInst, "InitCompression");
        pCreateCompression CreateCompression = (pCreateCompression)GetProcAddress(hInst, "CreateCompression");
        pCompress Compress = (pCompress)GetProcAddress(hInst, "Compress");
        pDestroyCompression DestroyCompression = (pDestroyCompression)GetProcAddress(hInst, "DestroyCompression");
        pDeInitCompression DeInitCompression = (pDeInitCompression)GetProcAddress(hInst, "DeInitCompression");
        if (InitCompression && CreateCompression && Compress && DestroyCompression && DeInitCompression)
        {
            InitCompression();
            CreateCompression(&ctx, GZIP_LVL);
            if (ctx)
            {
                Compress(ctx, pcIn, nSize, pcOut, nSize, &inused, &outused, 9L);
                DestroyCompression(ctx);
            }
            DeInitCompression();
        }
        FreeLibrary(hInst);
    }
	if((cpFile.Open(lpGzFile, CFile::modeCreate|CFile::modeWrite)) == FALSE){
		GlobalUnlock(hIn);
		GlobalUnlock(hOut);
		GlobalFree(hIn);
		GlobalFree(hOut);
		return(FALSE);
	}
	cpFile.WriteHuge(pcOut, inused);
	cpFile.Close();
	GlobalUnlock(hIn);
	GlobalUnlock(hOut);
	GlobalFree(hIn);
	GlobalFree(hOut);
	return(TRUE);
}
	