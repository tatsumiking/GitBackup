
//
// Easy Scanner System Ver1.0
//
// ess  outlib.c  出力関係関数
//
//  Copyright (c) 1998 by THEON

#include	"stdafx.h"

#include	<winspool.h>

#include	"Baselib.h"
#include	"Pout.h"
#include	"Senvlib.h"
#include	"Outlib.h"

#define	CANNOT	0
#define	RS232C	1
#define CENTRO	2
#define	FILEOUT	3
#define	SPOOL	4
#define	MAXSPOOL 5
#define	DIRECT	8

#define		DEFFAULT_TIMEOUTS	5000	/* sec */

// SPOOLで使用
HANDLE	hPrinter;
DOC_INFO_1 DOC1;
DWORD	dwJobID;
DWORD	dwWritten;

int		nAccessMode;
HDC		hPloterDC;
HANDLE	hPortID;
FILE	*fpOut;
OVERLAPPED  osWrite;
int     busymask, outcmd01, outcmd02;
int		portstatadrs, portdataadrs, portcmmdadrs, delaywait;
int		wait1, wait2;

DllExport int outlibOpen()
{
	int		len1, len2, len3;
	HANDLE				hInfo;
	LPPRINTER_INFO_5	lpINFO5 = NULL;		//PRINT_INFO_5構造体
	DWORD				dwNeeded;			//PRINT_INFO_X構造体に必要なｻｲｽﾞ
	DWORD				dwReturned;			//PRINT_INFO_X構造体の個数
	TCHAR	szPrinterName[RECMAX];
	TCHAR	szTitle[QRECMAX];

	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	TCHAR	szPortPrinter[RECMAX];
	TCHAR	outportname[RECMAX];
	TCHAR	DcbString[128];
	DCB		dcb;
	COMMTIMEOUTS	CommTimeOuts;
	int		i;

	nAccessMode = CANNOT;
	poutportnameget(outportname);
	len1 = _tcslen(_T("COM"));
	len2 = _tcslen(_T("LPT"));
	len3 = _tcslen(_T("FILE"));

	if(_tcsnccmp(outportname, _T("COM"), len1) == 0 && outportname[len1+1] == ':'){
		nAccessMode = RS232C;
		GetProfileString(_T("Ports"), outportname, _T(""), DcbString, 128-1);
	}else if(_tcsnccmp(outportname, _T("LPT"), len2) == 0 && outportname[len2+1] == ':'){
		if(nAccessMode == CANNOT){
			nAccessMode = CENTRO;
		}
		outportname[len2 + 1] = _T('\0');
	}else if(_tcsnccmp(outportname, _T("FILE"), len3) == 0){
		nAccessMode = FILEOUT;
		senvgetexepath(exepath);
		_stprintf_s(fname, _T("%s\\plotout.txt"), exepath);
		_tfopen_s(&fpOut, (LPTSTR)fname, _T("w"));
		if(fpOut == NULL){
			nAccessMode = CANNOT;
			return(FAIL);
		}
		return(0);
	}else{
		_tcscpy_s(szTitle, _T("DirectPrint"));
		szPrinterName[0] = _T('\0');
		EnumPrinters(PRINTER_ENUM_LOCAL,NULL,5,NULL,0,&dwNeeded,&dwReturned);

		hInfo = GlobalAlloc(GHND, dwNeeded);
		lpINFO5 = (LPPRINTER_INFO_5)GlobalLock(hInfo);
		EnumPrinters(PRINTER_ENUM_LOCAL,NULL,5,(LPBYTE)lpINFO5,dwNeeded,&dwNeeded,&dwReturned);
		for(i = 0;i < (int)dwReturned; i++){
			_stprintf_s(szPortPrinter, _T("%s (%s)"), lpINFO5[i].pPortName, lpINFO5[i].pPrinterName);
			if(_tcscmp(szPortPrinter, outportname) == 0){
				_tcscpy_s(szPrinterName, lpINFO5[i].pPrinterName);
				break;
			}
		}
		GlobalUnlock(hInfo);
		GlobalFree(hInfo);

		len1 = _tcslen(_T("MAX"));
		if(_tcsnccmp(szPrinterName, _T("MAX"), len1) == 0){
			if(!OpenPrinter(szPrinterName, &hPrinter, NULL)){
				nAccessMode = CANNOT;
				return(FAIL);
			}
			ZeroMemory(&DOC1, sizeof(DOC_INFO_1));
			DOC1.pDocName = szTitle;
			dwJobID = StartDocPrinter(hPrinter, 1, (LPBYTE)&DOC1);
			StartPagePrinter(hPrinter);
			//nAccessMode = MAXSPOOL;
			nAccessMode = SPOOL;
			return(0);
		}else if(szPrinterName[0] != _T('\0')){
			if(!OpenPrinter(szPrinterName, &hPrinter, NULL)){
				nAccessMode = CANNOT;
				return(FAIL);
			}
			ZeroMemory(&DOC1, sizeof(DOC_INFO_1));
			DOC1.pDocName = szTitle;
			dwJobID = StartDocPrinter(hPrinter, 1, (LPBYTE)&DOC1);
			StartPagePrinter(hPrinter);
			nAccessMode = SPOOL;
			return(0);
		}else{
			nAccessMode = FILEOUT;
			_tfopen_s(&fpOut, (LPTSTR)outportname, _T("w"));
			if(fpOut == NULL){
				nAccessMode = CANNOT;
				return(FAIL);
			}
			return(0);
		}
	}

	for(i = 0; i < 16; i++){
		hPortID = CreateFile(outportname,
						GENERIC_WRITE,
						0,					// exclusive access
						NULL,					// no security attrs
						OPEN_EXISTING,
						0,
						NULL);

		if(hPortID != (HANDLE) -1){
			break;
		}
		baseDispatch();
		Sleep(8000);	// 2秒Waitを8秒に変更
		nAccessMode = CANNOT;
	}

	if(nAccessMode == CANNOT){
		return(FAIL);
	}

	if(nAccessMode != RS232C){
		return(0);
	}

    SetCommMask( hPortID, EV_RXCHAR ) ;
    SetupComm( hPortID, 4096, 4096 ) ;
    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
    CommTimeOuts.ReadTotalTimeoutConstant = 0 ;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0 ;
    CommTimeOuts.WriteTotalTimeoutConstant = DEFFAULT_TIMEOUTS ;
    SetCommTimeouts( hPortID, &CommTimeOuts ) ;
	
	GetCommState(hPortID, &dcb);
	if (BuildCommDCB(DcbString, &dcb) != TRUE) {
		outlibClose();
		nAccessMode = CANNOT;
	 	return(FAIL);
	}
	dcb.XonLim = 2048;
	dcb.XoffLim = 512;
	//dcb.fOutX = 1;
	//dcb.fInX = 1;
	if(dcb.fRtsControl == 3){
		dcb.fOutxCtsFlow = 1;		// Hardware
		dcb.fRtsControl = 2;	// RTS_CONTROL_HANDSHAKE -> 2
	}
	//MyBuildCommDCB2(&dcb,&dcb);	// Xonoff

	if (SetCommState(hPortID, &dcb) < 0) {	//Win32
		outlibClose();
		nAccessMode = CANNOT;
	 	return(FAIL);
	}
	
	EscapeCommFunction(hPortID, SETDTR);
	EscapeCommFunction(hPortID, SETRTS);
	return(0);
}

DllExport void outlibPutString(char *buf)
{
	int		stat;
	int		len;
	DWORD	ret;
	int		i, j, k, kk;
	char	outbuf[RECMAX];

	if(nAccessMode == CANNOT){
		return;
	}

	len = strlen(buf);
	if(nAccessMode == SPOOL){
		WritePrinter(hPrinter, buf, len, &dwWritten);
	}else if(nAccessMode == MAXSPOOL){
		len += 8;
		outbuf[0] = len % 256;
		outbuf[1] = len / 256;
		strcpy_s(&(outbuf[2]), RECMAX, "MAXFGL1");
		strcpy_s(&(outbuf[10]), RECMAX, buf);
		WritePrinter(hPrinter, outbuf, len+2, &dwWritten);
		//ret = ExtEscape(hPloterDC, PASSTHROUGH, len+2, outbuf, 0, NULL);
	}else if(nAccessMode == RS232C){
		//stat = WriteFile( hPortID, (LPTSTR)buf, len, &ret, NULL ) ;
		for(i = 0; i < len; i++){
			while(1){
				stat = WriteFile( hPortID, (LPTSTR)(&(buf[i])), 1, &ret,  NULL) ;
				if(stat == 1){
					break;
				}
				if(ret == 1){
					break;
				}
				if (!stat && (GetLastError() == ERROR_IO_PENDING)){
					stat = WaitForSingleObject(osWrite.hEvent, 10000 );
					if(stat){
						ret = 0;
					}
					else{
						GetOverlappedResult( hPortID, &osWrite,
											&ret, FALSE ) ;
						osWrite.Offset += ret;
					}
				}
				for(j = 0; j < wait1; j++){
					for(k = 0; k < wait2; k++){
						kk = j+k;
					}
				}
			}
			baseDispatch();
			RsSetDefaultTimeOut( hPortID);
		}
	}else if(nAccessMode == CENTRO){
		WriteFile(hPortID, buf, len, &ret, NULL);
		for(j = 0; j < wait1; j++){
			for(k = 0; k < wait2; k++){
				kk = j+k;
			}
		}
		baseDispatch();
	}else{
		ret = fwrite(buf, len, 1, fpOut);
	}
}

DllExport void outlibPutData(char *buf, int len)
{
	int		stat;
	DWORD	ret;
	int		i, j, k, kk;
	char	outbuf[RECMAX];
	
	if(nAccessMode == CANNOT){
		return;
	}

	if(nAccessMode == SPOOL){
		WritePrinter(hPrinter, buf, len, &dwWritten);
	}else if(nAccessMode == MAXSPOOL){
		len += 8;
		outbuf[0] = len % 256;
		outbuf[1] = len / 256;
		strcpy_s(&(outbuf[2]), RECMAX, "MAXFGL1");
		memcpy_s(&(outbuf[10]), RECMAX, buf, len-8);
		WritePrinter(hPrinter, outbuf, len+2, &dwWritten);
		//ret = ExtEscape(hPloterDC, PASSTHROUGH, len+2, outbuf, 0, NULL);
	}else if(nAccessMode == RS232C){
		//stat = WriteFile( hPortID, (LPTSTR)buf, len, &ret, NULL ) ;
		for(i = 0; i < len; i++){
			while(1){
				stat = WriteFile( hPortID, (LPTSTR)(&(buf[i])), 1, &ret,  NULL) ;
				if(stat == 1){
					break;
				}
				if(ret == 1){
					break;
				}
				if (!stat && (GetLastError() == ERROR_IO_PENDING)){
					stat = WaitForSingleObject(osWrite.hEvent, 10000 );
					if(stat){
						ret = 0;
					}
					else{
						GetOverlappedResult( hPortID, &osWrite,
											&ret, FALSE ) ;
						osWrite.Offset += ret;
					}
				}
				for(j = 0; j < wait1; j++){
					for(k = 0; k < wait2; k++){
						kk = j+k;
					}
				}
				baseDispatch();
				RsSetDefaultTimeOut( hPortID);
			}
		}
	}else if(nAccessMode == CENTRO){
		WriteFile(hPortID, buf, len, &ret, NULL);
		for(j = 0; j < wait1; j++){
			for(k = 0; k < wait2; k++){
				kk = j+k;
			}
		}
		baseDispatch();
	}else{
		ret = fwrite(buf, len, 1, fpOut);
	}
}

BOOL RsSetTimeOut(HANDLE hCommDev, long IntervalTimeout, long ReadMultiTimeout,
 long ReadConstantTimeout, long WriteMultiTimeout, long WriteConstantTimeout)
{
	COMMTIMEOUTS CommTimeOuts;
	BOOL	stat;

	CommTimeOuts.ReadIntervalTimeout 	= IntervalTimeout ;
	CommTimeOuts.ReadTotalTimeoutMultiplier = ReadMultiTimeout ;
	CommTimeOuts.ReadTotalTimeoutConstant 	= ReadConstantTimeout ;
	CommTimeOuts.WriteTotalTimeoutMultiplier = WriteMultiTimeout ;
	CommTimeOuts.WriteTotalTimeoutConstant 	= WriteConstantTimeout ;
	stat = SetCommTimeouts( hCommDev, &CommTimeOuts ) ;
	return stat;
}

BOOL RsSetDefaultTimeOut(HANDLE hPortID)
{
	return(RsSetTimeOut(hPortID, 0xFFFFFFFF,0,0,0,DEFFAULT_TIMEOUTS));
}

DllExport void outlibClose()
{
	if(nAccessMode == SPOOL){
		EndPagePrinter(hPrinter);
		EndDocPrinter(hPrinter);
		ClosePrinter(hPrinter);
		return;
	}else if(nAccessMode == MAXSPOOL){
		EndPagePrinter(hPrinter);
		EndDocPrinter(hPrinter);
		ClosePrinter(hPrinter);
		return;
	}else if(nAccessMode == RS232C){
		SetCommMask( hPortID, 0 ) ;
		EscapeCommFunction( hPortID, CLRDTR ) ;
		PurgeComm( hPortID, PURGE_TXABORT | PURGE_RXABORT |
                                   PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
	}

	if(nAccessMode == DIRECT){
		;
	}else if(nAccessMode == RS232C || nAccessMode == CENTRO){
		outlibHndleClose();
	}else{
		if(fpOut != NULL){
			fclose(fpOut);
		}
	}
}

void outlibHndleClose()
{
	if(hPortID != (HANDLE) -1){
		CloseHandle(hPortID);
	}
	hPortID = (HANDLE) -1;
}

BOOL MyBuildCommDCB2( DCB *dcb, DCB *yourdcb )
{
    if(!dcb || !yourdcb){
   		return FALSE;
	}
   dcb->DCBlength = 28;
   dcb->BaudRate = yourdcb->BaudRate;
   dcb->fBinary = 1;
   dcb->fParity = 0;
   dcb->fOutxCtsFlow = 0;		// Hardware
   dcb->fOutxDsrFlow = 0;		// 0 -> modifyed 12.06
   dcb->fDtrControl = 1;	    // 1 -> modifyed 12.06
   					// DTRのハンドシェイクを有効にします。
   dcb->fDsrSensitivity = 0;
   dcb->fTXContinueOnXoff = 0;
   dcb->fOutX = 1;
   dcb->fInX = 1;
   dcb->fErrorChar = 0;
   dcb->fNull = 0;
   dcb->fRtsControl = 1;	// RTS_CONTROL_HANDSHAKE -> 2
   dcb->fAbortOnError = 0;
   dcb->fDummy2 = 0;
   dcb->wReserved = 0;
   //dcb->XonLim = 2048;
   //dcb->XoffLim = 512;
   dcb->XonLim = 15000;
   dcb->XoffLim = 512;

   dcb->ByteSize = yourdcb->ByteSize;
   dcb->Parity = yourdcb->Parity;
   dcb->StopBits = yourdcb->StopBits;

   dcb->XonChar = 17;
   dcb->XoffChar = 19;
   dcb->ErrorChar = 63;
   dcb->EofChar = 26;
   dcb->EvtChar = 0;
   dcb->wReserved1 = 0;
   return(TRUE);
}
