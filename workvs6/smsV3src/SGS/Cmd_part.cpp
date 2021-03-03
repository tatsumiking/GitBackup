
/////////////////////////////////////////////////////////////////////////////
// CCmdText

#include "stdafx.h"
#include "math.h"

#include "..\cmnsrc\cmdbsbmp.h"
#include "cmd_part.h"
#include "..\cmnsrc\Dlgtxtin.h"
#include "DPrtLoad.h"
#include "DPrtSave.h"

extern "C" {
	void senvgetpartpath(LPSTR path);
}

#define	MOJISIZE	102.4	// MESHSIZE 1024

CCmdPart::CCmdPart(CScrollView* pcview)
:CCmdBaseBmp(pcview)
{
	//m_pcView = (CTonView*)pcview;
}

UINT CCmdPart::ExecPartLoad(UINT event, DBL dx, DBL dy)
{
	CDialogPartLoad	*pcDialogPartLoad;
	char	szName[RECMAX];
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	pcDialogPartLoad = new CDialogPartLoad(m_pcView);
	ret = pcDialogPartLoad->DoModal();
	if(ret == IDOK){
		strcpy(szName, pcDialogPartLoad->m_szName);
		LoadPart(szName);
	}
	delete(pcDialogPartLoad);
	return(ENDEVENT);
}

UINT CCmdPart::ExecPartSave(UINT event, DBL dx, DBL dy)
{
	CDialogPartSave	*pcDialogPartSave;
	char	szName[RECMAX];
	int		ret;

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	pcDialogPartSave = new CDialogPartSave(m_pcView);
	ret = pcDialogPartSave->DoModal();
	if(ret == IDOK){
		strcpy(szName, pcDialogPartSave->m_szName);
		SavePart(szName);
	}
	delete(pcDialogPartSave);
	return(ENDEVENT);
}

void CCmdPart::SavePart(LPSTR lpName)
{
	CDialogTextInput	*pcDialogTextInput;
	int		ret;
	DBL		sx, sy, ex, ey;
	DBL		cx, cy;
	DBL		movex, movey;
	char	szPartPath[FNAMEMAX];
	char	szFileName[FNAMEMAX];

	if(lpName[0] == '\0'){
		pcDialogTextInput = new CDialogTextInput(m_pcView);
		pcDialogTextInput->SetDialogTitle("•”•i–¼Ý’è");
		pcDialogTextInput->SetTitleText("•”•i–¼‚ð“ü—Í‚µ‚Ä‚­‚¾‚³‚¢");
		ret = pcDialogTextInput->DoModal();
		if(ret == IDOK){
			strcpy(lpName, pcDialogTextInput->m_csText.GetBuffer(RECMAX));
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
	sprintf(szFileName, "%s\\%s.bmp", szPartPath, lpName);
	InitFullClrDrawArea(64, 64, sx, sy, ex, ey);
	DrawAndCreateFullclrFile(szFileName);
	EndFullclrImage();

	SaveUndo();
	m_pcDoc->MoveTrns(MOJISIZE / 2.0 - cx, MOJISIZE / 2.0 - cy); 
	sprintf(szFileName, "%s\\%s.iti", szPartPath, lpName);
	m_pcDoc->m_nActiveSave = ON;
	m_pcDoc->m_nNoMsgOut = TRUE;
	m_pcDoc->SaveFile(szFileName);
	m_pcDoc->m_nNoMsgOut = FALSE;
	m_pcDoc->m_nActiveSave = OFF;
	LoadUndo();
}

void CCmdPart::AddPartList(LPSTR lpName)
{
	char	szPartPath[FNAMEMAX];
	char	szFileName[FNAMEMAX];
	FILE	*fp;

	senvgetpartpath(szPartPath);
	sprintf(szFileName, "%s\\partlist.txt", szPartPath);
	fp = fopen(szFileName, "r+");
	fseek(fp, 0L, 2);
	fprintf(fp, "%s\n", lpName);
	fclose(fp);
}

void CCmdPart::LoadPart(LPSTR lpName)
{
	char	szPartPath[FNAMEMAX];
	char	szFileName[FNAMEMAX];

	senvgetpartpath(szPartPath);
	sprintf(szFileName, "%s\\%s.iti", szPartPath, lpName);
	m_pcDoc->LoadFile(szFileName);
}
