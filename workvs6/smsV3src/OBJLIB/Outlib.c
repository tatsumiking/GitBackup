
//
// Easy Scanner System Ver1.0
//
// ess  outlib.c  出力関係関数
//
//  Copyright (c) 1998 by THEON

#include	<windows.h>
#include	<conio.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<winspool.h>

#include	"..\stdef.h"
#include	"dlldef.h"

#include	"baselib.fnc"
#include	"senvlib.fnc"
#include	"pout.fnc"
#include	"outlib.fnc"

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
	HANDLE				hInfo;
	LPPRINTER_INFO_5	lpINFO5 = NULL;		//PRINT_INFO_5構造体
	DWORD				dwNeeded;			//PRINT_INFO_X構造体に必要なｻｲｽﾞ
	DWORD				dwReturned;			//PRINT_INFO_X構造体の個数
	char	szPrinterName[RECMAX];
	char	szTitle[QRECMAX];

	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	char	szPortPrinter[RECMAX];
	char	outportname[RECMAX];
	char	DcbString[128];
	DCB		dcb;
	COMMTIMEOUTS	CommTimeOuts;
	int		i;

	nAccessMode = CANNOT;
	poutportnameget(outportname);
	if(strncmp(outportname, "COM", 3) == 0 && outportname[4] == ':'){
		nAccessMode = RS232C;
		GetProfileString("Ports", outportname, "", DcbString, 128-1);
	}else if(strncmp(outportname, "LPT", 3) == 0 && outportname[4] == ':'){
		if(nAccessMode == CANNOT){
			nAccessMode = CENTRO;
		}
		outportname[4] = '\0';
	}else if(strncmp(outportname, "FILE", 4) == 0){
		nAccessMode = FILEOUT;
		senvgetexepath(exepath);
		sprintf(fname, "%s\\plotout.txt", exepath);
		if((fpOut = fopen(fname, "w")) == NULL){		
			nAccessMode = CANNOT;
			return(FAIL);
		}
		return(0);
	}else{
		strcpy(szTitle, "DirectPrint");
		szPrinterName[0] = '\0';
		EnumPrinters(PRINTER_ENUM_LOCAL,NULL,5,NULL,0,&dwNeeded,&dwReturned);

		hInfo = GlobalAlloc(GHND, dwNeeded);
		lpINFO5 = (LPPRINTER_INFO_5)GlobalLock(hInfo);
		EnumPrinters(PRINTER_ENUM_LOCAL,NULL,5,(LPBYTE)lpINFO5,dwNeeded,&dwNeeded,&dwReturned);
		for(i = 0;i < (int)dwReturned; i++){
			sprintf(szPortPrinter, "%s (%s)", lpINFO5[i].pPortName, lpINFO5[i].pPrinterName);
			if(strcmp(szPortPrinter, outportname) == 0){
				strcpy(szPrinterName, lpINFO5[i].pPrinterName);
				break;
			}
		}
		GlobalUnlock(hInfo);
		GlobalFree(hInfo);

		if(strncmp(szPrinterName, "MAX", 3) == 0){
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
		}else if(szPrinterName[0] != '\0'){
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
			if((fpOut = fopen(outportname, "w")) == NULL){
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
		Dispatch();
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
	int		ret;
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
		strcpy(&(outbuf[2]), "MAXFGL1");
		strcpy(&(outbuf[10]), buf);
		WritePrinter(hPrinter, outbuf, len+2, &dwWritten);
		//ret = ExtEscape(hPloterDC, PASSTHROUGH, len+2, outbuf, 0, NULL);
	}else if(nAccessMode == DIRECT){
		for(i = 0; i < len; i++){
			while((_inp((USHORT)portstatadrs)&busymask) != busymask);
			_outp((USHORT)portdataadrs, buf[i]);
			for(k = 0; k < delaywait; k++);
			_outp((USHORT)portcmmdadrs, outcmd01);
			for(k = 0; k < delaywait; k++);
			_outp((USHORT)portcmmdadrs, outcmd02);
			for(k = 0; k < delaywait; k++);
		}
	}else if(nAccessMode == RS232C){
		//stat = WriteFile( hPortID, (LPSTR)buf, len, &ret, NULL ) ;
		for(i = 0; i < len; i++){
			while(1){
				stat = WriteFile( hPortID, (LPSTR)(&(buf[i])), 1, &ret,  NULL) ;
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
			Dispatch();
			RsSetDefaultTimeOut( hPortID);
		}
	}else if(nAccessMode == CENTRO){
		WriteFile(hPortID, buf, len, &ret, NULL);
		for(j = 0; j < wait1; j++){
			for(k = 0; k < wait2; k++){
				kk = j+k;
			}
		}
		Dispatch();
	}else{
		ret = fwrite(buf, len, 1, fpOut);
	}
}

DllExport void outlibPutData(char *buf, int len)
{
	int		stat;
	int		ret;
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
		strcpy(&(outbuf[2]), "MAXFGL1");
		memcpy(&(outbuf[10]), buf, len-8);
		WritePrinter(hPrinter, outbuf, len+2, &dwWritten);
		//ret = ExtEscape(hPloterDC, PASSTHROUGH, len+2, outbuf, 0, NULL);
	}else if(nAccessMode == DIRECT){
		for(i = 0; i < len; i++){
			while((_inp((USHORT)portstatadrs)&busymask) != busymask);
			_outp((USHORT)portdataadrs, buf[i]);
			for(k = 0; k < delaywait; k++);
			_outp((USHORT)portcmmdadrs, outcmd01);
			for(k = 0; k < delaywait; k++);
			_outp((USHORT)portcmmdadrs, outcmd02);
			for(k = 0; k < delaywait; k++);
		}
	}else if(nAccessMode == RS232C){
		//stat = WriteFile( hPortID, (LPSTR)buf, len, &ret, NULL ) ;
		for(i = 0; i < len; i++){
			while(1){
				stat = WriteFile( hPortID, (LPSTR)(&(buf[i])), 1, &ret,  NULL) ;
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
				Dispatch();
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
		Dispatch();
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
