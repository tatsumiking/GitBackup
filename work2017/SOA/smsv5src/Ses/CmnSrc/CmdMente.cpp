
/////////////////////////////////////////////////////////////////////////////
// CCmdMente

#include "stdafx.h"
#include "io.h"
#include "direct.h"

#include "CmdBase.h"
#include "CmdMente.h"

#include "DialogCptrFolder.h"
#include "DialogWakuCopyTrns.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Wakuenv.h"
//}

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
	TCHAR	szWakuSrcList[FNAMEMAX];
	TCHAR	szWakuDstList[FNAMEMAX];
	FILE	*fp;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	pcDialogCptrFolder = new CDialogCptrFolder(m_pcView);
	ret = pcDialogCptrFolder->DoModal();
	if(ret == IDOK){
		_tcscpy_s(m_szSrcPath, pcDialogCptrFolder->m_csSrcFolder.GetBuffer(FNAMEMAX));
		_tcscpy_s(m_szDstPath, pcDialogCptrFolder->m_csDstFolder.GetBuffer(FNAMEMAX));
		m_dXTime = pcDialogCptrFolder->m_dXTime;
		m_dYTime = pcDialogCptrFolder->m_dYTime;
		m_dDstXSize = _ttof(pcDialogCptrFolder->m_csDstXSize);
		m_dDstYSize = _ttof(pcDialogCptrFolder->m_csDstYSize);
		SaveUndo();
		scrngetmmsize(&dWorkXSize, &dWorkYSize);
		SaveWakuElement();
		_stprintf_s(szWakuSrcList, _T("%s\\wakuname.lst"), m_szSrcPath);
		_tfopen_s(&fp, szWakuSrcList, _T("r"));
		if(fp == NULL){
			m_pcApp->CreateWakuNameList(m_szSrcPath);
		}else{
			fclose(fp);
		}
		_stprintf_s(szWakuDstList, _T("%s\\wakuname.lst"), m_szDstPath);
		ret = TRUE;
		if(_taccess_s(szWakuDstList, 0) == 0){
			ret = AfxMessageBox(_T("枠が存在します。\n上書してもよろしいですか"), MB_OKCANCEL);
		}
		if(ret == TRUE){
			WakuCopyTrnsLoop(szWakuSrcList);
		}
		scrnsetmmsize(dWorkXSize, dWorkYSize);
		m_pcView->WorkSizeReset();
		m_pcView->WorkAreaReset();
		LoadUndo();
		if(ret == TRUE){
			::AfxMessageBox(_T("フォルダーのコピー変形を終了しました"), MB_OK);
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
	TCHAR	tmppath[FNAMEMAX];
	TCHAR	szWakuSrcList[FNAMEMAX];

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	wakuenvgetfullpaperpath(m_szSrcPath);
	wakuenvgetpaperarea(&xsize, &ysize);
	pcDialogWakuCopyTrns = new CDialogWakuCopyTrns(m_pcView);
	pcDialogWakuCopyTrns->m_csDstFolder = m_szSrcPath;
	pcDialogWakuCopyTrns->m_csDstXSize.Format(_T("%1.1lf"), xsize);
	pcDialogWakuCopyTrns->m_csDstYSize.Format(_T("%1.1lf"), ysize);
	ret = pcDialogWakuCopyTrns->DoModal();
	if(ret == IDOK){
		_tcscpy_s(m_szDstPath, pcDialogWakuCopyTrns->m_csDstFolder.GetBuffer(FNAMEMAX));
		m_dDstXSize = _ttof(pcDialogWakuCopyTrns->m_csDstXSize);
		m_dDstYSize = _ttof(pcDialogWakuCopyTrns->m_csDstYSize);
		m_dXTime = m_dDstXSize / xsize;
		m_dYTime = m_dDstYSize / ysize;

		SaveUndo();
		scrngetmmsize(&dWorkXSize, &dWorkYSize);

		SaveWakuElement();
		senvgettmppath(tmppath);
		_stprintf_s(szWakuSrcList, _T("%s\\wakuname.lst"), tmppath);
		WakuCopyTrnsLoop(szWakuSrcList);
		
		scrnsetmmsize(dWorkXSize, dWorkYSize);
		m_pcView->WorkSizeReset();
		m_pcView->WorkAreaReset();
		LoadUndo();
		::AfxMessageBox(_T("フォルダーのコピー変形を終了しました"), MB_OK);
		ScrnAllRedraw();
	}
	return(ENDEVENT);
}

void CCmdMente::SaveWakuElement()
{
	TCHAR	szPath[FNAMEMAX];
	TCHAR	szPaperName[RECMAX];

	_tcscpy_s(szPath, m_szDstPath);
	basegetparentpath(szPath);
	_tcscpy_s(szPaperName, &(m_szDstPath[_tcslen(szPath)+1]));
	wakuenvsetpapername(szPaperName);
	wakuenvsetpapersize(m_dDstXSize, m_dDstYSize);
	wakuenvsetpaperstart(0.0, 0.0);
	wakuenvsetpaperarea(m_dDstXSize, m_dDstYSize);
	wakuenvsetpaperpath(szPaperName);
	wakuenvsetpaperelement();
}

void CCmdMente::WakuCopyTrnsLoop(LPTSTR lpWakuSrcList)
{
	TCHAR	szWakuDstList[FNAMEMAX];
	FILE	*ifp, *ofp;
	TCHAR	szWakuName[FNAMEMAX];
	TCHAR	szBuf[RECMAX];

	_stprintf_s(szWakuDstList, _T("%s\\wakuname.lst"), m_szDstPath);
	_tfopen_s(&ifp, lpWakuSrcList, _T("r"));
	_tfopen_s(&ofp, szWakuDstList, _T("w"));
	while(1){
		if(baselibCrDelFgets(szBuf, RECMAX, ifp) == NULL){
			break;
		}
		_stprintf_s(szWakuName, &(szBuf[1]));
		szWakuName[_tcslen(szWakuName) - 2] = _T('\0');
		WakuCopyTrns(szWakuName);
		baselibFputs(szBuf, ofp);
	}
	fclose(ifp);
	fclose(ofp);
}

void CCmdMente::WakuCopyTrns(LPTSTR lpWakuName)
{
	TCHAR	szWakuSrcName[FNAMEMAX];
	TCHAR	szWakuDstName[FNAMEMAX];
	DBL		dXSize, dYSize;

	_tmkdir(m_szDstPath);
	_stprintf_s(szWakuSrcName, _T("%s\\%s"), m_szSrcPath, lpWakuName);
	_stprintf_s(szWakuDstName, _T("%s\\%s"), m_szDstPath, lpWakuName);
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

void CCmdMente::WakuLoad(LPTSTR lpWakuName)
{
	TCHAR	szEnvFileName[FNAMEMAX];

	_tcscpy_s(szEnvFileName, lpWakuName);
	basechangesafix(szEnvFileName, _T("env"));
    if((_taccess_s(szEnvFileName, 0 )) == 0){
		wakuenvloadoutputenv(szEnvFileName);
		m_pcView->WorkSizeReset();
		m_pcView->WorkAreaReset();
	}
	m_pcDoc->WakuLoad(lpWakuName);
	/*
	TCHAR	szBinFileName[FNAMEMAX];
	_tcscpy_s(szBinFileName, lpWakuName);
	basechangesafix(szBinFileName, _T("bin"));
    if((_taccess_s(szBinFileName, 0 )) != 0){
		_stprintf_s(szBinFileName, _T("%s\\defprn.bin"), m_szSrcPath);
	}
	m_pcView->PrintEnvLoad(szBinFileName);
	*/
}

void CCmdMente::WakuSave(LPTSTR lpWakuFileName)
{
	TCHAR	szEnvFileName[FNAMEMAX];

	m_pcDoc->WakuSave(lpWakuFileName);

	_tcscpy_s(szEnvFileName, lpWakuFileName);
	basechangesafix(szEnvFileName, _T("env"));
	wakuenvsaveoutputenv(szEnvFileName);
	/*
	TCHAR	szBinFileName[FNAMEMAX];
	_tcscpy_s(szBinFileName, lpWakuFileName);
	basechangesafix(szBinFileName, _T("bin"));
	m_pcView->PrintEnvSave(szBinFileName);
	*/
}
