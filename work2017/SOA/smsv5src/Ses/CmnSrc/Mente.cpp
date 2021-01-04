// Mente.cpp : インプリメンテーション ファイル
//

#include "io.h"
#include "direct.h"
#include "stdafx.h"
#include "Mente.h"

//extern "C" {
#include "../../ObjLib/Senvlib.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CMente

CMente::CMente()
{
	TCHAR	szSrcPath[FNAMEMAX];
	TCHAR	szDstPath[FNAMEMAX];

	senvgetwakpath(szSrcPath);
	SrchBackUpFolder(szDstPath);
	CopyFolder(szSrcPath, szDstPath);
}

CMente::~CMente()
{
}

void CMente::SrchBackUpFolder(LPTSTR lpBackupPath)
{
	TCHAR	szDrv[FNAMEMAX];
	int		idx;
	TCHAR	szKeyFileName[FNAMEMAX];
	int		len;

	_tcscpy_s(szDrv, _T("D:\\"));
	for (idx = 0; idx < 22; idx++ )
	{
		szDrv[0] = 'D' + idx;
		_stprintf_s(szKeyFileName, _T("%s\\soabackupwaku"), szDrv);
		if(_taccess_s(szKeyFileName, 0) == 0){
			len = _tcslen(szKeyFileName) + 1;
			_tcscpy_s(lpBackupPath, len, szKeyFileName);
			return;
		}
	}
	len = _tcslen(_T("c:\\soabackupwaku")) + 1;
	_tcscpy_s(lpBackupPath, len, _T("c:\\soabackupwaku"));
}

void CMente::CopyFolder(LPTSTR lpSrcPath, LPTSTR lpDstPath)
{
	TCHAR	szSrchFileName[FNAMEMAX];
	TCHAR	szFolderName[QRECMAX];
	TCHAR	szSrcFileName[FNAMEMAX];
	TCHAR	szDstFileName[FNAMEMAX];
	CFileFind finder;
	CString csName;

	_tmkdir(lpDstPath);
	_stprintf_s(szSrchFileName, _T("%s\\*.*"), lpSrcPath);
	BOOL bWorking = finder.FindFile((LPCTSTR)szSrchFileName);
	while (bWorking){
		bWorking = finder.FindNextFile();
		if(finder.IsDots() == FALSE){
			csName = finder.GetFileName();
			_stprintf_s(szFolderName, _T("%s"), csName.GetBuffer());
			_stprintf_s(szSrcFileName, _T("%s\\%s"), lpSrcPath, szFolderName);
			_stprintf_s(szDstFileName, _T("%s\\%s"), lpDstPath, szFolderName);
			if(finder.IsDirectory() == TRUE){
				CopyFolder(szSrcFileName, szDstFileName);
			}else{
				CopyFile(szSrcFileName, szDstFileName, FALSE);
			}
		}
	}
	finder.Close();
}

