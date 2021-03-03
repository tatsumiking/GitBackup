// Mente.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "io.h"
#include "direct.h"
#include "Mente.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMente
extern "C" {
	void senvgetwakpath(LPSTR path);
}

CMente::CMente()
{
	char	szSrcPath[FNAMEMAX];
	char	szDstPath[FNAMEMAX];

	senvgetwakpath(szSrcPath);
	SrchBackUpFolder(szDstPath);
	CopyFolder(szSrcPath, szDstPath);
}

CMente::~CMente()
{
}

void CMente::SrchBackUpFolder(LPSTR lpBackupPath)
{
	char	szDrv[FNAMEMAX];
	int		idx;
	char	szKeyFileName[FNAMEMAX];

	strcpy(szDrv, "D:\\");
	for (idx = 0; idx < 22; idx++ )
	{
		szDrv[0] = 'D' + idx;
		sprintf(szKeyFileName, "%s\\soabackupwaku", szDrv);
		if(_access(szKeyFileName, 0) == 0){
			strcpy(lpBackupPath, szKeyFileName);
			return;
		}
	}
	strcpy(lpBackupPath, "c:\\soabackupwaku");
}

void CMente::CopyFolder(LPSTR lpSrcPath, LPSTR lpDstPath)
{
	char	szSrchFileName[FNAMEMAX];
	char	szFolderName[QRECMAX];
	char	szSrcFileName[FNAMEMAX];
	char	szDstFileName[FNAMEMAX];
	CFileFind finder;

	_mkdir(lpDstPath);
	sprintf(szSrchFileName, "%s\\*.*", lpSrcPath);
	BOOL bWorking = finder.FindFile(szSrchFileName);
	while (bWorking){
		bWorking = finder.FindNextFile();
		if(finder.IsDots() == FALSE){
			sprintf(szFolderName, "%s", finder.GetFileName());
			sprintf(szSrcFileName, "%s\\%s", lpSrcPath, szFolderName);
			sprintf(szDstFileName, "%s\\%s", lpDstPath, szFolderName);
			if(finder.IsDirectory() == TRUE){
				CopyFolder(szSrcFileName, szDstFileName);
			}else{
				CopyFile(szSrcFileName, szDstFileName, FALSE);
			}
		}
	}
	finder.Close();
}

