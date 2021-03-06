// UsbPrt.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "malloc.h"
#include "stdio.h"
#include "io.h"
#include "direct.h"
#include "Shlobj.h"
#include "setupapi.h"
#include "Shlwapi.h"

#define	QRECMAX		64
#define	RECMAX		256
#define	RECMAX4		1024
#define	RECMAX8		2048
#define	RECMAX16	4096
#define TEMPBUFMAX	8192
#define	FNAMEMAX	MAX_PATH

#define	DllExport __declspec(dllexport)

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

void CheckDrv(LPSTR lpChkVol, LPSTR lpPath)
{
	char	szDrv[RECMAX];
	int		idx;
	char	szVol[RECMAX];
	char	szSysname[RECMAX];
	DWORD	nMaxlen;
	DWORD	nVolno;
	DWORD	nFlag;

	lpPath[0] = '\0';
	strcpy(szDrv, "D:\\");
	for (idx = 0; idx < 22; idx++ )
	{
		szDrv[0] = 'D' + idx;
		GetVolumeInformation(szDrv, szVol, RECMAX, &nVolno, &nMaxlen, &nFlag, szSysname, RECMAX);
		if(strcmp(szVol, lpChkVol) == 0){
			strcpy(lpPath, szDrv);
			return;
		}
	}
}

void GetSrchKey(LPSTR lpKeyFileName, long nID, LPSTR lpSrchKey)
{
	FILE	*fp;
	char	szReadBuf[TEMPBUFMAX];
	int		idx, setidx;
	int		sumcrt, add;

	fp = fopen(lpKeyFileName, "rb");
	if(fp == NULL){
		return;
	}
	fread(szReadBuf, 1, TEMPBUFMAX, fp);
	fclose(fp);

	idx = (nID % 256);
	setidx = 0;
	sumcrt = 0;
	while(1){
		if(szReadBuf[idx] == 'T'){
			lpSrchKey[setidx] = '\0';
			break;
		}
		add = (szReadBuf[idx]) % 10 + 4;
		sumcrt += add;
		lpSrchKey[setidx] = szReadBuf[8050-sumcrt];
		idx++;
		setidx++;
	}
}

#define HD_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) static const GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 }}

HD_DEFINE_GUID(GUID_DEVINTERFACE_USBDISK, 0x53f56307, 0xb6bf, 0x11d0
			   , 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);

long GetInterFaceName(LPSTR lpSrchKey, LPSTR lpInterFaceName)
{
	PSP_DEVICE_INTERFACE_DETAIL_DATA	interface_detail;
	SP_DEVICE_INTERFACE_DATA	devinterface;
	SP_DEVINFO_DATA	devinfo;
	GUID		intfce;
	HDEVINFO	devs;
	DWORD		devcount;
	DWORD		size;
	int			ret;
	DWORD		dataType;

    char	szDriverkey[RECMAX4];
    char	szInterfacename[RECMAX4];
	
	lpInterFaceName[0] = '\0';
	intfce = GUID_DEVINTERFACE_USBDISK;
	devs = SetupDiGetClassDevs(&intfce, 0, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if (devs == INVALID_HANDLE_VALUE) {
		return(FALSE);
	}
	devcount = 0;
	devinterface.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	while(1){
		ret = SetupDiEnumDeviceInterfaces(devs, 0, &intfce, devcount, &devinterface);
		if(ret == FALSE){
			break;
		}
		devcount++;
		size = 0;
		SetupDiGetDeviceInterfaceDetail(devs, &devinterface, 0, 0, &size, 0);
		devinfo.cbSize = sizeof(SP_DEVINFO_DATA);
		interface_detail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)calloc(1, size);
		if(interface_detail){
			interface_detail->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);
			devinfo.cbSize = sizeof(SP_DEVINFO_DATA);
			ret = SetupDiGetDeviceInterfaceDetail(devs, &devinterface, interface_detail, size, 0, &devinfo);
			if(ret == 0){
				free(interface_detail);
				break;
			}
			strcpy(szInterfacename, interface_detail->DevicePath);
			free(interface_detail);
			size = sizeof(szDriverkey);
			szDriverkey[0] = 0;
			ret = SetupDiGetDeviceRegistryProperty(devs, &devinfo, SPDRP_DRIVER, &dataType, (LPBYTE)szDriverkey, size, 0);
			if(ret == 0){
				break;
			}
			if(StrStr(szInterfacename, lpSrchKey)){
				strcpy(lpInterFaceName, szInterfacename);
			}
		}
	}
	SetupDiDestroyDeviceInfoList(devs);
	if(lpInterFaceName[0] == '\0'){
		return(FALSE);
	}
	return(TRUE);
}

//プロテクト千恵公益社様用
long	CheckKoudenKanri(long nID)
{
	long	ret;
	char	szSrchKey[1024];
	char	szDevName[1024];

	//プロテクト千恵公益社様用 ソニー製USBメモリーチェック
	strcpy(szSrchKey, "ven_sony&prod_storage_media&rev_0100#2a0804221");
	ret = GetInterFaceName(szSrchKey, szDevName);
	return ret;
}

__declspec(dllexport)
long __stdcall Check(long nID)
{
	LPITEMIDLIST	PidList;
	char	szVol[QRECMAX];
	char	szUSBPath[RECMAX];
	char	szPCPath[FNAMEMAX];
	char	szDirName[RECMAX];
	char	szUSBKeyFileName[FNAMEMAX];
	char	szPCKeyFileName[FNAMEMAX];
	char	szSrchKey[RECMAX4];
	char	szDevName[RECMAX4];
	int		ret;

	if(CheckKoudenKanri(nID) == TRUE){
		return(nID);
	}

	SHGetSpecialFolderLocation(NULL, 0x23, &PidList);
	SHGetPathFromIDList(PidList, szPCPath);
	sprintf(szPCKeyFileName, "%s\\.system\\.system%08ld", szPCPath, nID);

	CheckDrv("USBPROTECT", szUSBPath);
	if(szUSBPath[0] == '\0'){
		sprintf(szVol, "USB%08d", nID);
		CheckDrv(szVol, szUSBPath);
		sprintf(szUSBKeyFileName, "%s\\.system\\.system", szUSBPath);
	}else{
		sprintf(szUSBKeyFileName, "%s\\.system\\.system%08ld", szUSBPath, nID);
	}

	if(access(szPCKeyFileName, 0) != 0){
		sprintf(szDirName, "%s\\.system", szPCPath);
		mkdir(szDirName);
		CopyFile(szUSBKeyFileName, szPCKeyFileName, TRUE);
#ifdef _DEBUG
		MessageBox(NULL, szPCPath, "CopyFile USB->PC", MB_OK);
#endif
	}else if(access(szUSBKeyFileName, 0) != 0){
		sprintf(szDirName, "%s\\.system", szUSBPath);
		mkdir(szDirName); 
		CopyFile(szPCKeyFileName, szUSBKeyFileName, TRUE);
#ifdef _DEBUG
		MessageBox(NULL, szPCPath, "CopyFile PC->USB", MB_OK);
#endif
	}

	GetSrchKey(szPCKeyFileName, nID, szSrchKey);
#ifdef _DEBUG
	MessageBox(NULL, szSrchKey, "Check", MB_OK);
#endif
	ret = GetInterFaceName(szSrchKey, szDevName);
#ifdef _DEBUG
	MessageBox(NULL, szDevName, "Check", MB_OK);
#endif
	if(ret == TRUE){
#ifdef _DEBUG
		MessageBox(NULL, "Success", "Check", MB_OK);
#endif
		return(nID);
	}else{
		GetSrchKey(szUSBKeyFileName, nID, szSrchKey);
#ifdef _DEBUG
		MessageBox(NULL, szSrchKey, "Check", MB_OK);
#endif
		ret = GetInterFaceName(szSrchKey, szDevName);
#ifdef _DEBUG
		MessageBox(NULL, szDevName, "Check", MB_OK);
#endif
		if(ret == TRUE){
			sprintf(szDirName, "%s\\.system", szPCPath);
			mkdir(szDirName);
			CopyFile(szUSBKeyFileName, szPCKeyFileName, FALSE);
	#ifdef _DEBUG
			MessageBox(NULL, szPCPath, "CopyFile USB->PC", MB_OK);
	#endif
			return(nID);
		}else{
#ifdef _DEBUG
			MessageBox(NULL, "Failure", "Check", MB_OK);
#endif
			return(-1);
		}
	}
}
