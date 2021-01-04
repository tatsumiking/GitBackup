
/////////////////////////////////////////////////////////////////////////////
// CCmdText

#include "stdafx.h"
#include "math.h"

#include "CmdBase.h"
#include "CmdBaseBmp.h"
#include "CmdPart.h"
#include "DialogTextInput.h"
#include "DialogPartLoad.h"
#include "DialogPartSave.h"

//extern "C" {
	#include "../../ObjLib/Senvlib.h"
//}

#define	MOJISIZE	102.4	// MESHSIZE 1024

CCmdPart::CCmdPart(CScrollView* pcview)
:CCmdBaseBmp(pcview)
{
	//m_pcView = (CTonView*)pcview;
}

UINT CCmdPart::ExecPartLoad(UINT event, DBL dx, DBL dy)
{
	CDialogPartLoad	*pcDialogPartLoad;
	TCHAR	szName[RECMAX];
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	pcDialogPartLoad = new CDialogPartLoad(m_pcView);
	ret = pcDialogPartLoad->DoModal();
	if(ret == IDOK){
		_tcscpy_s(szName, pcDialogPartLoad->m_szName);
		LoadPart(szName);
	}
	delete(pcDialogPartLoad);
	return(ENDEVENT);
}

UINT CCmdPart::ExecPartSave(UINT event, DBL dx, DBL dy)
{
	CDialogPartSave	*pcDialogPartSave;
	TCHAR	szName[RECMAX];
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	pcDialogPartSave = new CDialogPartSave(m_pcView);
	ret = pcDialogPartSave->DoModal();
	if(ret == IDOK){
		_tcscpy_s(szName, pcDialogPartSave->m_szName);
		SavePart(szName);
	}
	delete(pcDialogPartSave);
	return(ENDEVENT);
}

void CCmdPart::SavePart(LPTSTR lpName)
{
	CDialogTextInput	*pcDialogTextInput;
	int		ret;
	DBL		sx, sy, ex, ey;
	DBL		cx, cy;
	DBL		movex, movey;
	TCHAR	szPartPath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];

	if(lpName[0] == _T('\0')){
		pcDialogTextInput = new CDialogTextInput(m_pcView);
		pcDialogTextInput->SetDialogTitle(_T("•”•i–¼Ý’è"));
		pcDialogTextInput->SetTitleText(_T("•”•i–¼‚ð“ü—Í‚µ‚Ä‚­‚¾‚³‚¢"));
		ret = pcDialogTextInput->DoModal();
		if(ret == IDOK){
			_tcscpy_s(lpName, RECMAX, pcDialogTextInput->m_csText.GetBuffer(RECMAX));
		}
		delete(pcDialogTextInput);
		if(ret != IDOK){
			return;
		}
		AddPartList(lpName);
	}
	m_pcDoc->GetDMiniMax(ACTIVEFLAG, &sx, &sy, &ex, &ey);
	cx = (sx + ex) / 2.0;
	cy = (sy + ey) / 2.0;
	sx = cx - MOJISIZE / 2.0;
	ex = cx + MOJISIZE / 2.0;
	sy = cy - MOJISIZE / 2.0;
	ey = cy + MOJISIZE / 2.0;

	senvgetpartpath(szPartPath);
	_stprintf_s(szFileName, _T("%s\\%s.bmp"), szPartPath, lpName);
	InitFullClrDrawArea(64, 64, sx, sy, ex, ey);
	DrawAndCreateFullclrFile(szFileName);
	EndFullclrImage();

	SaveUndo();
	m_pcDoc->MoveTrns(MOJISIZE / 2.0 - cx, MOJISIZE / 2.0 - cy); 
	_stprintf_s(szFileName, _T("%s\\%s.iti"), szPartPath, lpName);
	m_pcDoc->m_nActiveSave = ON;
	m_pcDoc->m_nNoMsgOut = TRUE;
	m_pcDoc->SaveFile(szFileName);
	m_pcDoc->m_nNoMsgOut = FALSE;
	m_pcDoc->m_nActiveSave = OFF;
	LoadUndo();
}

void CCmdPart::AddPartList(LPTSTR lpName)
{
	TCHAR	szPartPath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];
	FILE	*fp;

	senvgetpartpath(szPartPath);
	_stprintf_s(szFileName, _T("%s\\partlist.txt"), szPartPath);
	_tfopen_s(&fp, szFileName, _T("r+"));
	if (fp == NULL) {
		return;
	}
	fseek(fp, 0L, 2);
	_ftprintf_s(fp, _T("%s\n"), lpName);
	fclose(fp);
}

void CCmdPart::LoadPart(LPTSTR lpName)
{
	TCHAR	szPartPath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];

	senvgetpartpath(szPartPath);
	_stprintf_s(szFileName, _T("%s\\%s.iti"), szPartPath, lpName);
	m_pcDoc->LoadFile(szFileName);
}
