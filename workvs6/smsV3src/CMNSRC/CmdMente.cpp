
/////////////////////////////////////////////////////////////////////////////
// CCmdMente

#include "stdafx.h"
#include "math.h"
#include "io.h"
#include "direct.h"

#include "cmd_base.h"
#include "cmdmente.h"
#include "DCptrFolder.h"
#include "DWkCptr.h"

extern "C" {
	void scrngetmmsize(LPDBL xmm, LPDBL ymm);
	void scrnsetmmsize(DBL xsize, DBL ysize);
	void senvgettmppath(LPSTR path);
	int basegetparentpath(LPSTR lpPath);
	void wakuenvgetfullpaperpath(LPSTR paperpath);
	void wakuenvgetpaperarea(LPDBL paperwidth, LPDBL paperhight);
	void wakuenvsetpapername(LPSTR name);
	void wakuenvsetpapersize(DBL xsize, DBL ysize);
	void wakuenvsetpaperstart(DBL sx, DBL sy);
	void wakuenvsetpaperarea(DBL width, DBL hight);
	void wakuenvsetpaperpath(LPSTR path);
	void wakuenvsetpaperelement();
	void basechangesafix(LPSTR FileName, LPSTR Safix);
	void wakuenvsaveoutputenv(LPSTR wakusavefname);
	void wakuenvloadoutputenv(LPSTR wakuloadfname);
}

CCmdMente::CCmdMente(CScrollView* pcview)
:CCmdBase(pcview)
{
	//m_pcView = (CTonView*)pcview;
}

UINT CCmdMente::ExecCopyTrnsFolder(UINT event, DBL dx, DBL dy)
{
	CDialogCptrFolder	*pcDialogCptrFolder;
	int		ret;
	DBL		dWorkXSize, dWorkYSize;
	char	szWakuSrcList[FNAMEMAX];
	char	szWakuDstList[FNAMEMAX];
	FILE	*fp;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	pcDialogCptrFolder = new CDialogCptrFolder(m_pcView);
	ret = pcDialogCptrFolder->DoModal();
	if(ret == IDOK){
		strcpy(m_szSrcPath, pcDialogCptrFolder->m_csSrcFolder.GetBuffer(FNAMEMAX));
		strcpy(m_szDstPath, pcDialogCptrFolder->m_csDstFolder.GetBuffer(FNAMEMAX));
		m_dXTime = pcDialogCptrFolder->m_dXTime;
		m_dYTime = pcDialogCptrFolder->m_dYTime;
		m_dDstXSize = atof(pcDialogCptrFolder->m_csDstXSize);
		m_dDstYSize = atof(pcDialogCptrFolder->m_csDstYSize);
		SaveUndo();
		scrngetmmsize(&dWorkXSize, &dWorkYSize);
		SaveWakuElement();
		sprintf(szWakuSrcList, "%s\\wakuname.lst", m_szSrcPath);
		fp = fopen(szWakuSrcList, "r");
		if(fp == NULL){
			m_pcApp->CreateWakuNameList(m_szSrcPath);
		}else{
			fclose(fp);
		}
		sprintf(szWakuDstList, "%s\\wakuname.lst", m_szDstPath);
		ret = TRUE;
		if(_access(szWakuDstList, 0) == 0){
			ret = AfxMessageBox("枠が存在します。\n上書してもよろしいですか", MB_OKCANCEL);
		}
		if(ret == TRUE){
			WakuCopyTrnsLoop(szWakuSrcList);
		}
		scrnsetmmsize(dWorkXSize, dWorkYSize);
		m_pcView->WorkSizeReset();
		m_pcView->WorkAreaReset();
		LoadUndo();
		if(ret == TRUE){
			::AfxMessageBox("フォルダーのコピー変形を終了しました", MB_OK);
		}
		ScrnAllRedraw();
	}
	return(ENDEVENT);
}

UINT CCmdMente::ExecCopyTrnsFile(UINT event, DBL dx, DBL dy)
{
	CDialogWakuCopyTrns *pcDialogWakuCopyTrns;
	int		ret;
	DBL		xsize, ysize;
	DBL		dWorkXSize, dWorkYSize;
	char	tmppath[FNAMEMAX];
	char	szWakuSrcList[FNAMEMAX];

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	wakuenvgetfullpaperpath(m_szSrcPath);
	wakuenvgetpaperarea(&xsize, &ysize);
	pcDialogWakuCopyTrns = new CDialogWakuCopyTrns(m_pcView);
	pcDialogWakuCopyTrns->m_csDstFolder = _T(m_szSrcPath);
	pcDialogWakuCopyTrns->m_csDstXSize.Format("%1.1lf", xsize);
	pcDialogWakuCopyTrns->m_csDstYSize.Format("%1.1lf", ysize);
	ret = pcDialogWakuCopyTrns->DoModal();
	if(ret == IDOK){
		strcpy(m_szDstPath, pcDialogWakuCopyTrns->m_csDstFolder.GetBuffer(FNAMEMAX));
		m_dDstXSize = atof(pcDialogWakuCopyTrns->m_csDstXSize);
		m_dDstYSize = atof(pcDialogWakuCopyTrns->m_csDstYSize);
		m_dXTime = m_dDstXSize / xsize;
		m_dYTime = m_dDstYSize / ysize;

		SaveUndo();
		scrngetmmsize(&dWorkXSize, &dWorkYSize);

		SaveWakuElement();
		senvgettmppath(tmppath);
		sprintf(szWakuSrcList, "%s\\wakuname.lst", tmppath);
		WakuCopyTrnsLoop(szWakuSrcList);
		
		scrnsetmmsize(dWorkXSize, dWorkYSize);
		m_pcView->WorkSizeReset();
		m_pcView->WorkAreaReset();
		LoadUndo();
		::AfxMessageBox("フォルダーのコピー変形を終了しました", MB_OK);
		ScrnAllRedraw();
	}
	return(ENDEVENT);
}

void CCmdMente::SaveWakuElement()
{
	char	szPath[FNAMEMAX];
	char	szPaperName[RECMAX];

	strcpy(szPath, m_szDstPath);
	basegetparentpath(szPath);
	strcpy(szPaperName, &(m_szDstPath[strlen(szPath)+1]));
	wakuenvsetpapername(szPaperName);
	wakuenvsetpapersize(m_dDstXSize, m_dDstYSize);
	wakuenvsetpaperstart(0.0, 0.0);
	wakuenvsetpaperarea(m_dDstXSize, m_dDstYSize);
	wakuenvsetpaperpath(szPaperName);
	wakuenvsetpaperelement();
}

void CCmdMente::WakuCopyTrnsLoop(LPSTR lpWakuSrcList)
{
	char	szWakuDstList[FNAMEMAX];
	FILE	*ifp, *ofp;
	char	szWakuName[FNAMEMAX];
	char	szBuf[RECMAX];

	sprintf(szWakuDstList, "%s\\wakuname.lst", m_szDstPath);
	ifp = fopen(lpWakuSrcList, "r");
	ofp = fopen(szWakuDstList, "w");
	while(1){
		if(fgets(szBuf, RECMAX, ifp) == NULL){
			break;
		}
		sprintf(szWakuName, &(szBuf[1]));
		szWakuName[strlen(szWakuName) - 2] = '\0';
		WakuCopyTrns(szWakuName);
		fputs(szBuf, ofp);
	}
	fclose(ifp);
	fclose(ofp);
}

void CCmdMente::WakuCopyTrns(LPSTR lpWakuName)
{
	char	szWakuSrcName[FNAMEMAX];
	char	szWakuDstName[FNAMEMAX];
	DBL		dXSize, dYSize;

	_mkdir(m_szDstPath);
	sprintf(szWakuSrcName, "%s\\%s", m_szSrcPath, lpWakuName);
	sprintf(szWakuDstName, "%s\\%s", m_szDstPath, lpWakuName);
	m_pcDoc->DataAllCls();
	WakuLoad(szWakuSrcName);

	scrngetmmsize(&dXSize, &dYSize);
	dXSize = dXSize * m_dXTime;
	dYSize = dYSize * m_dYTime;
	m_pcDoc->OnFlag(ACTIVEFLAG);
	m_pcDoc->SizeTrns(0.0, 0.0, m_dXTime, m_dYTime);
	scrnsetmmsize(dXSize, dYSize);
	m_pcView->WorkSizeReset();
	m_pcView->WorkAreaReset();
	
	WakuSave(szWakuDstName);
}

void CCmdMente::WakuLoad(LPSTR lpWakuName)
{
	char	szEnvFileName[FNAMEMAX];

	strcpy(szEnvFileName, lpWakuName); 
	basechangesafix(szEnvFileName, "env");
    if((_access(szEnvFileName, 0 )) == 0){
		wakuenvloadoutputenv(szEnvFileName);
		m_pcView->WorkSizeReset();
		m_pcView->WorkAreaReset();
	}
	m_pcDoc->WakuLoad(lpWakuName);
	/*
	char	szBinFileName[FNAMEMAX];
	strcpy(szBinFileName, lpWakuName);
	basechangesafix(szBinFileName, "bin");
    if((_access(szBinFileName, 0 )) != 0){
		sprintf(szBinFileName, "%s\\defprn.bin", m_szSrcPath);
	}
	m_pcView->PrintEnvLoad(szBinFileName);
	*/
}

void CCmdMente::WakuSave(LPSTR lpWakuFileName)
{
	char	szEnvFileName[FNAMEMAX];

	m_pcDoc->WakuSave(lpWakuFileName);

	strcpy(szEnvFileName, lpWakuFileName);
	basechangesafix(szEnvFileName, "env");
	wakuenvsaveoutputenv(szEnvFileName);
	/*
	char	szBinFileName[FNAMEMAX];
	strcpy(szBinFileName, lpWakuFileName);
	basechangesafix(szBinFileName, "bin");
	m_pcView->PrintEnvSave(szBinFileName);
	*/
}
