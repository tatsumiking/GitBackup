
/////////////////////////////////////////////////////////////////////////////
// CCmdMoji

#include "stdafx.h"
#include "CmdBase.h"
#include "CmdMoji.h"

#include "DialogInputNum.h"
#include "DialogMojiLoad.h"
#include "DialogMojiSave.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Mojienv.h"
//}

#define	MOJISIZE	102.4	// MESHSIZE 1024

CCmdMoji::CCmdMoji(CScrollView* pcview)
:CCmdBase(pcview)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	TCHAR	buf[RECMAX];
	FILE	*fp;

	m_dStepmm = 10.0;
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\defenv\\cmd_moji.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp != NULL){
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf"), &m_dStepmm);
		fclose(fp);
	}
}

CCmdMoji::~CCmdMoji()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\defenv\\cmd_moji.env"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	if(fp != NULL){
		_stprintf_s(buf, _T("%4.1lf\n"), m_dStepmm);
		baselibFputs(buf, fp);
		fclose(fp);
	}
}

UINT CCmdMoji::ExecMojiSize(UINT event, DBL dx, DBL dy)
{
	CData*	pcData;

	switch(event){
	case INITEVENT:
		m_pcDialogMojiSize = new CDialogMojiSize(m_pcView);
		m_pcDialogMojiSize->m_edStepmm = m_dStepmm;
		if(m_pcDialogMojiSize->Create(IDD_MOJI_SIZE, m_pcView)){
			LocateMojiSizeDlg();
		}
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0
		|| m_nMouseSetCount == 1){
			if(m_nMouseSetCount == 1){
				BeginDraw();
				m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
				EndDraw();
				ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
				ScrnDataRedraw(m_pcData);
			}
			pcData = SrchMoji(dx, dy, &m_nMjidx);
			if(pcData != NULL){
				SaveUndo();
				m_pcData = (CStrgVect*)pcData;
				m_pcData->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
				BeginDraw();
				m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
				EndDraw();
				m_nMouseSetCount = 1;
			}
			else{
				m_nMouseSetCount = 0;
			}
		}
		break;
	case USER1EVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			m_pcDialogMojiSize->GetItemElement();
			m_dStepmm = m_pcDialogMojiSize->m_edStepmm;
			SizeMojiBig();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
		}
		break;
	case USER2EVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			m_pcDialogMojiSize->GetItemElement();
			m_dStepmm = m_pcDialogMojiSize->m_edStepmm;
			SizeMojiSmall();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case DLGSETEVENT:
		if(m_nMouseSetCount == 1){
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnDataRedraw(m_pcData);
		}
		m_nMouseSetCount = 0;
		break;
	case DLGRESETEVENT:
		if(m_nMouseSetCount == 1){
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnDataRedraw(m_pcData);
		}
		m_pcDialogMojiSize->DestroyWindow();
		return(ENDEVENT);
	case RESETEVENT:
		break;
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnDataRedraw(m_pcData);
		}
		m_pcDialogMojiSize->DestroyWindow();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdMoji::ExecMojiMove(UINT event, DBL dx, DBL dy)
{
	CData*	pcData;

	switch(event){
	case INITEVENT:
		m_pcDialogMojiMove = new CDialogMojiMove(m_pcView);
		m_pcDialogMojiMove->m_edStepmm = m_dStepmm;
		if(m_pcDialogMojiMove->Create(IDD_MOJI_MOVE, m_pcView)){
			LocateMojiMoveDlg();
		}
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0
		|| m_nMouseSetCount == 1){
			if(m_nMouseSetCount == 1){
				BeginDraw();
				m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
				EndDraw();
				ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
				ScrnDataRedraw(m_pcData);
			}
			pcData = SrchMoji(dx, dy, &m_nMjidx);
			if(pcData != NULL){
				SaveUndo();
				m_pcData = (CStrgVect*)pcData;
				m_pcData->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
				BeginDraw();
				m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
				EndDraw();
				m_nMouseSetCount = 1;
			}
			else{
				m_nMouseSetCount = 0;
			}
		}
		break;
	case USER1EVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			m_pcDialogMojiMove->GetItemElement();
			m_dStepmm = m_pcDialogMojiMove->m_edStepmm;
			MoveMojiLeft();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
		}
		break;
	case USER2EVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			m_pcDialogMojiMove->GetItemElement();
			m_dStepmm = m_pcDialogMojiMove->m_edStepmm;
			MoveMojiRight();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
		}
		break;
	case USER3EVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			m_pcDialogMojiMove->GetItemElement();
			m_dStepmm = m_pcDialogMojiMove->m_edStepmm;
			MoveMojiUp();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
		}
		break;
	case USER4EVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			m_pcDialogMojiMove->GetItemElement();
			m_dStepmm = m_pcDialogMojiMove->m_edStepmm;
			MoveMojiDown();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case DLGSETEVENT:
		if(m_nMouseSetCount == 1){
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnDataRedraw(m_pcData);
		}
		m_nMouseSetCount = 0;
		break;
	case DLGRESETEVENT:
		m_pcDialogMojiMove->DestroyWindow();
		if(m_nMouseSetCount == 1){
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnDataRedraw(m_pcData);
		}
		m_nMouseSetCount = 0;
		return(ENDEVENT);
	case RESETEVENT:
		break;
	case ENDEVENT:
		m_pcDialogMojiMove->DestroyWindow();
		if(m_nMouseSetCount == 1){
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnDataRedraw(m_pcData);
		}
		m_nMouseSetCount = 0;
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdMoji::ExecMojiFiting(UINT event, DBL dx, DBL dy)
{
	CData*	pcData;

	switch(event){
	case INITEVENT:
		m_pcDialogMojiFiting = new CDialogMojiFiting(m_pcView);
		m_pcDialogMojiFiting->m_dStep = m_dStepmm;
		if(m_pcDialogMojiFiting->Create(IDD_MOJI_FITING, m_pcView)){
			LocateMojiFitingDlg();
		}
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0
		|| m_nMouseSetCount == 1){
			if(m_nMouseSetCount == 1){
				BeginDraw();
				m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
				EndDraw();
				ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
				ScrnDataRedraw(m_pcData);
			}
			pcData = SrchMoji(dx, dy, &m_nMjidx);
			if(pcData != NULL){
				SaveUndo();
				m_pcData = (CStrgVect*)pcData;
				m_pcData->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
				SetMojiSizeToDialog();
				BeginDraw();
				m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
				EndDraw();
				m_nMouseSetCount = 1;
			}
			else{
				m_nMouseSetCount = 0;
			}
		}
		break;
	case USER1EVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			m_pcDialogMojiFiting->GetItemElement();
			m_dStepmm = m_pcDialogMojiFiting->m_dStep;
			SizeMojiBig();
			SetMojiSizeToDialog();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
		}
		break;
	case USER2EVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			m_pcDialogMojiFiting->GetItemElement();
			m_dStepmm = m_pcDialogMojiFiting->m_dStep;
			SizeMojiSmall();
			SetMojiSizeToDialog();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
		}
		break;
	case USER3EVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			m_pcDialogMojiFiting->GetItemElement();
			m_dStepmm = m_pcDialogMojiFiting->m_dStep;
			SizeMojiRLBig();
			SetMojiSizeToDialog();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
		}
		break;
	case USER4EVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			m_pcDialogMojiFiting->GetItemElement();
			m_dStepmm = m_pcDialogMojiFiting->m_dStep;
			SizeMojiRLSmall();
			SetMojiSizeToDialog();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
		}
		break;
	case USER5EVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			m_pcDialogMojiFiting->GetItemElement();
			m_dStepmm = m_pcDialogMojiFiting->m_dStep;
			SizeMojiUDBig();
			SetMojiSizeToDialog();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
		}
		break;
	case USER6EVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			m_pcDialogMojiFiting->GetItemElement();
			m_dStepmm = m_pcDialogMojiFiting->m_dStep;
			SizeMojiUDSmall();
			SetMojiSizeToDialog();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
		}
		break;
	case USER7EVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			m_pcDialogMojiFiting->GetItemElement();
			m_dStepmm = m_pcDialogMojiFiting->m_dStep;
			MoveMojiUp();
			SetMojiSizeToDialog();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
		}
		break;
	case USER8EVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			m_pcDialogMojiFiting->GetItemElement();
			m_dStepmm = m_pcDialogMojiFiting->m_dStep;
			MoveMojiDown();
			SetMojiSizeToDialog();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
		}
		break;
	case USER9EVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			m_pcDialogMojiFiting->GetItemElement();
			m_dStepmm = m_pcDialogMojiFiting->m_dStep;
			MoveMojiLeft();
			SetMojiSizeToDialog();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
		}
		break;
	case USER10EVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			m_pcDialogMojiFiting->GetItemElement();
			m_dStepmm = m_pcDialogMojiFiting->m_dStep;
			MoveMojiRight();
			SetMojiSizeToDialog();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case DLGSETEVENT:
		if(m_nMouseSetCount == 1){
			GetMojiSizeFromDialog();
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnDataRedraw(m_pcData);
		}
		m_nMouseSetCount = 0;
		break;
	case DLGRESETEVENT:
		if(m_nMouseSetCount == 1){
			LoadUndo();
			ScrnAllRedraw();
		}
		m_nMouseSetCount = 0;
		break;
	case RESETEVENT:
		break;
	case DLGENDEVENT:
	case ENDEVENT:
		if(m_nMouseSetCount == 1){
			LoadUndo();
			ScrnAllRedraw();
		}
		m_pcDialogMojiFiting->DestroyWindow();
		m_nMouseSetCount = 0;
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdMoji::ExecMojiRound(UINT event, DBL dx, DBL dy)
{
	CData*	pcData;

	switch(event){
	case INITEVENT:
		break;
	case SETEVENT:
		pcData = SrchMoji(dx, dy, &m_nMjidx);
		if(pcData != NULL){ 
			SaveUndo();
			m_pcData = (CStrgVect*)pcData;
			m_pcData->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);

			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();

			RoundMojiMain();				

			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnDataRedraw(m_pcData);
			m_nMouseSetCount = 1;
		}
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdMoji::ExecMojiLoad(UINT event, DBL dx, DBL dy)
{
	int		ret;

	switch(event){
	case INITEVENT:
		InitPointSet();
		//SaveUndo();
		ret = MojiLoadMain();
		if(ret == FALSE){
			return(ENDEVENT);
		}
		BeginDraw();
		DrawBoxs(m_pcDC);
		EndDraw();
		break;
	case SETEVENT:
		BeginDraw();
		DrawBoxs(m_pcDC);
		m_pcDataGrup->MoveTrns(m_dSX - m_dBX, m_dSY - m_dBY);
		m_pcDataGrup->RenewMiniMax();
		DrawModeSET();
		m_pcDataGrup->OnDraw(m_pcDC);
		EndDraw();
		DeleteGrup(m_pcDataGrup);
		return(ENDEVENT);
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		BeginDraw();
		DrawBoxs(m_pcDC);
		MovePointSet(dx, dy);
		DrawBoxs(m_pcDC);
		EndDraw();
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		break;
	case ENDEVENT:
		BeginDraw();
		DrawBoxs(m_pcDC);
		EndDraw();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdMoji::ExecMojiSave(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		InitPointSet();
		BeginDraw();
		DrawBoxs(m_pcDC);
		EndDraw();
		break;
	case SETEVENT:
		BeginDraw();
		DrawBoxs(m_pcDC);
		MovePointSet(dx, dy);
		InAreaDataAtntionOn();
		((CTonView*)m_pcView)->AtntionDraw(m_pcDC);
		MojiSaveMain();
		EndDraw();
		return(ENDEVENT);
	case MOVEEVENT:
	case UPMOVEEVENT:
		BeginDraw();
		DrawBoxs(m_pcDC);
		MovePointSet(dx, dy);
		DrawBoxs(m_pcDC);
		EndDraw();
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdMoji::ExecMojiSaveDel(UINT event, DBL dx, DBL dy)
{
	int		ret;

	switch(event){
	case INITEVENT:
		InitPointSet();
		SaveUndo();
		ret = MojiDelLoadMain();
		if(ret == FALSE){
			return(ENDEVENT);
		}
		BeginDraw();
		DrawBoxs(m_pcDC);
		EndDraw();
		break;
	case SETEVENT:
		BeginDraw();
		DrawBoxs(m_pcDC);
		m_pcDataGrup->MoveTrns(m_dSX - m_dBX, m_dSY - m_dBY);
		m_pcDataGrup->RenewMiniMax();
		DrawModeSET();
		m_pcDataGrup->OnDraw(m_pcDC);
		EndDraw();
		if(AfxMessageBox(_T("‚±‚Ì“o˜^•¶Žš‚ðíœ‚µ‚Ä‚æ‚ë‚µ‚¢‚Å‚·‚©"), MB_OKCANCEL, -1) == IDOK){
			MojiSaveDelete(m_szFontName, m_nCode);			
		}
		DeleteGrup(m_pcDataGrup);
		return(ENDEVENT);
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		BeginDraw();
		DrawBoxs(m_pcDC);
		MovePointSet(dx, dy);
		DrawBoxs(m_pcDC);
		EndDraw();
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		break;
	case ENDEVENT:
		BeginDraw();
		DrawBoxs(m_pcDC);
		EndDraw();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdMoji::ExecFontChange(UINT event, DBL dx, DBL dy)
{
	CData*	pcData;
	CTonDoc	*pcDoc;

	switch(event){
	case INITEVENT:
		m_pcDialogFontChange = new CDialogFontChange(m_pcView);
		if(m_pcDialogFontChange->Create(IDD_FONTCHNG, m_pcView)){
			LocateFontChangeDlg();
		}
		pcDoc = (CTonDoc*)m_pcView->GetDocument();
		pcDoc->OffFlag(ATNTION1FLAG);
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		pcData = SrchMoji(dx, dy, &m_nMjidx);
		if(pcData != NULL){ 
			SaveUndo();
			m_pcData = pcData;
			//m_pcData->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
			BeginDraw();
			m_pcData->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
			m_pcData->OnIdxFlag(m_nMjidx, ATNTION1FLAG);
			m_nMouseSetCount = 1;
		}
		break;
	case DLGSETEVENT:
		if(m_nMouseSetCount == 1){
			FlagCheckMojiChange(ATNTION1FLAG);
		}
		m_nMouseSetCount = 0;
		pcDoc = (CTonDoc*)m_pcView->GetDocument();
		pcDoc->OffFlag(ATNTION1FLAG);
		ScrnRedraw();
		//return(REDRAWEVENT);
		break;
	case DLGRESETEVENT:
	case ENDEVENT:
		m_pcDialogFontChange->DestroyWindow();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

// “à•”ŠÖ”
void CCmdMoji::LocateMojiSizeDlg()
{
	CTonApp*	theApp;
	CMainFrame*	theWnd;
	CRect	prect;
	CRect	crect;
	int		x, y, cx, cy;
	int		ret;

	m_pcView->GetWindowRect(&prect);
	m_pcDialogMojiSize->GetWindowRect(&crect);
	x = prect.right - crect.Width();
	y = prect.top;
	cx = crect.Width();
	cy = crect.Height();

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	ret = m_pcDialogMojiSize->SetWindowPos(theWnd, x, y, cx, cy, 0);
	m_pcDialogMojiSize->ShowWindow(SW_SHOW);
}

void CCmdMoji::LocateMojiMoveDlg()
{
	CTonApp*	theApp;
	CMainFrame*	theWnd;
	CRect	prect;
	CRect	crect;
	int		x, y, cx, cy;
	int		ret;

	m_pcView->GetWindowRect(&prect);
	m_pcDialogMojiMove->GetWindowRect(&crect);
	x = prect.right - crect.Width();
	y = prect.top;
	cx = crect.Width();
	cy = crect.Height();

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	ret = m_pcDialogMojiMove->SetWindowPos(theWnd, x, y, cx, cy, 0);
	m_pcDialogMojiMove->ShowWindow(SW_SHOW);
}

void CCmdMoji::LocateMojiFitingDlg()
{
	CTonApp*	theApp;
	CMainFrame*	theWnd;
	CRect	prect;
	CRect	crect;
	int		x, y, cx, cy;
	int		ret;

	m_pcView->GetWindowRect(&prect);
	m_pcDialogMojiFiting->GetWindowRect(&crect);
	x = prect.right - crect.Width();
	y = prect.top;
	cx = crect.Width();
	cy = crect.Height();

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	ret = m_pcDialogMojiFiting->SetWindowPos(theWnd, x, y, cx, cy, 0);
	m_pcDialogMojiFiting->ShowWindow(SW_SHOW);
}

void CCmdMoji::LocateFontChangeDlg()
{
	CTonApp*	theApp;
	CMainFrame*	theWnd;
	CRect	prect;
	CRect	crect;
	int		x, y, cx, cy;
	int		ret;

	m_pcView->GetWindowRect(&prect);
	m_pcDialogFontChange->GetWindowRect(&crect);
	x = prect.right - crect.Width();
	y = prect.top;
	cx = crect.Width();
	cy = crect.Height();

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	ret = m_pcDialogFontChange->SetWindowPos(theWnd, x, y, cx, cy, 0);
	m_pcDialogFontChange->ShowWindow(SW_SHOW);
}

void CCmdMoji::SetMojiSizeToDialog()
{
	CMojiVect*	pcMojiVect;
	int		fSpaceKind;
	DBL		minix, miniy, maxx, maxy;
	DBL		xsize, ysize;

	pcMojiVect = (CMojiVect*)m_pcData->GetIdxMoji(m_nMjidx);
	mojienvgetspacekind(&fSpaceKind);
	if(fSpaceKind == ON){
		pcMojiVect->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
	}
	else{
		pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
	}
	xsize = maxx - minix;
	ysize = maxy - miniy;
	m_pcDialogMojiFiting->m_dXSize = FLOOR100(xsize);
	m_pcDialogMojiFiting->m_dYSize = FLOOR100(ysize);
	m_pcDialogMojiFiting->SetItemElement();
}

void CCmdMoji::GetMojiSizeFromDialog()
{
	DBL		xsize, ysize;
	CMojiVect*	pcMojiVect;
	int		fSpaceKind;
	DBL		minix, miniy, maxx, maxy;
	DBL		timex, timey;
	DBL		cx, cy;

	m_pcDialogMojiFiting->GetItemElement();
	xsize = m_pcDialogMojiFiting->m_dXSize;
	ysize = m_pcDialogMojiFiting->m_dYSize;
	pcMojiVect = (CMojiVect*)m_pcData->GetIdxMoji(m_nMjidx);
	mojienvgetspacekind(&fSpaceKind);
	if(fSpaceKind == ON){
		pcMojiVect->GetDataDMiniMax(&minix, &miniy, &maxx, &maxy);
	}
	else{
		pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
	}
	cx = (maxx + minix) / 2.0;
	cy = (maxy + miniy) / 2.0;
	timex = xsize / (maxx - minix);
	timey = ysize / (maxy - miniy);
	pcMojiVect->SizeTrns(cx, cy, timex, timey);
	m_pcData->RenewMiniMax();
}

void CCmdMoji::SizeMojiSmall()
{
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;
	DBL		size, time;
	DBL		cx, cy;

	pcMojiVect = (CMojiVect*)m_pcData->GetIdxMoji(m_nMjidx);
	pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
	if(m_pcData->CheckTateYoko() == ON){
		size = maxy - miniy;
	}
	else{
		size = maxx - minix;
	}
	time = (size - m_dStepmm) / size;
	cx = (maxx + minix) / 2.0;
	cy = (maxy + miniy) / 2.0;
	pcMojiVect->SizeTrns(cx, cy, time, time);
	m_pcData->RenewMiniMax();
}

void CCmdMoji::SizeMojiBig()
{
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;
	DBL		size, time;
	DBL		cx, cy;

	pcMojiVect = (CMojiVect*)m_pcData->GetIdxMoji(m_nMjidx);
	pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
	if(m_pcData->CheckTateYoko() == ON){
		size = maxy - miniy;
	}
	else{
		size = maxx - minix;
	}
	time = (size + m_dStepmm) / size;
	cx = (maxx + minix) / 2.0;
	cy = (maxy + miniy) / 2.0;
	pcMojiVect->SizeTrns(cx, cy, time, time);
	m_pcData->RenewMiniMax();
}

void CCmdMoji::SizeMojiRLSmall()
{
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;
	DBL		size, time;
	DBL		cx, cy;

	pcMojiVect = (CMojiVect*)m_pcData->GetIdxMoji(m_nMjidx);
	pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
	size = maxx - minix;
	time = (size - m_dStepmm) / size;
	cx = (maxx + minix) / 2.0;
	cy = (maxy + miniy) / 2.0;
	pcMojiVect->SizeTrns(cx, cy, time, 1.0);
	m_pcData->RenewMiniMax();
}

void CCmdMoji::SizeMojiRLBig()
{
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;
	DBL		size, time;
	DBL		cx, cy;

	pcMojiVect = (CMojiVect*)m_pcData->GetIdxMoji(m_nMjidx);
	pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
	size = maxx - minix;
	time = (size + m_dStepmm) / size;
	cx = (maxx + minix) / 2.0;
	cy = (maxy + miniy) / 2.0;
	pcMojiVect->SizeTrns(cx, cy, time, 1.0);
	m_pcData->RenewMiniMax();
}

void CCmdMoji::SizeMojiUDSmall()
{
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;
	DBL		size, time;
	DBL		cx, cy;

	pcMojiVect = (CMojiVect*)m_pcData->GetIdxMoji(m_nMjidx);
	pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
	size = maxy - miniy;
	time = (size - m_dStepmm) / size;
	cx = (maxx + minix) / 2.0;
	cy = (maxy + miniy) / 2.0;
	pcMojiVect->SizeTrns(cx, cy, 1.0, time);
	m_pcData->RenewMiniMax();
}

void CCmdMoji::SizeMojiUDBig()
{
	CMojiVect*	pcMojiVect;
	DBL		minix, miniy, maxx, maxy;
	DBL		size, time;
	DBL		cx, cy;

	pcMojiVect = (CMojiVect*)m_pcData->GetIdxMoji(m_nMjidx);
	pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
	size = maxy - miniy;
	time = (size + m_dStepmm) / size;
	cx = (maxx + minix) / 2.0;
	cy = (maxy + miniy) / 2.0;
	pcMojiVect->SizeTrns(cx, cy, 1.0, time);
	m_pcData->RenewMiniMax();
}

void CCmdMoji::MoveMojiLeft()
{
	CMojiVect*	pcMojiVect;
	DBL	movex, movey;

	pcMojiVect = (CMojiVect*)m_pcData->GetIdxMoji(m_nMjidx);
	movex = -m_dStepmm;
	movey = 0;
	pcMojiVect->MoveTrns(movex, movey);
	m_pcData->RenewMiniMax();
}

void CCmdMoji::MoveMojiRight()
{
	CMojiVect*	pcMojiVect;
	DBL	movex, movey;

	pcMojiVect = (CMojiVect*)m_pcData->GetIdxMoji(m_nMjidx);
	movex = m_dStepmm;
	movey = 0;
	pcMojiVect->MoveTrns(movex, movey);
	m_pcData->RenewMiniMax();
}

void CCmdMoji::MoveMojiUp()
{
	CMojiVect*	pcMojiVect;
	DBL	movex, movey;

	pcMojiVect = (CMojiVect*)m_pcData->GetIdxMoji(m_nMjidx);
	movex = 0;
	movey = m_dStepmm;
	pcMojiVect->MoveTrns(movex, movey);
	m_pcData->RenewMiniMax();
}

void CCmdMoji::MoveMojiDown()
{
	CMojiVect*	pcMojiVect;
	DBL	movex, movey;

	pcMojiVect = (CMojiVect*)m_pcData->GetIdxMoji(m_nMjidx);
	movex = 0;
	movey = -m_dStepmm;
	pcMojiVect->MoveTrns(movex, movey);
	m_pcData->RenewMiniMax();
}

void CCmdMoji::RoundMojiMain()
{
	CMojiVect*	pcMojiVect;
	CDialogInputNum	*pDialogInputNum;
	DBL		rag;
	DBL		tsin, tcos;
	DBL		cx, cy;
	DBL		minix, miniy, maxx, maxy;

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("Šp“xŽw’è‰ñ“]•ÏX"));
	pDialogInputNum->SetUnitText(_T("“x"));
	pDialogInputNum->SetTitleText(_T("‰ñ“]Šp“x"));
	pDialogInputNum->m_dInputNum = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		rag = pDialogInputNum->m_dInputNum;
	}
	delete(pDialogInputNum);

	pcMojiVect = (CMojiVect*)m_pcData->GetIdxMoji(m_nMjidx);
	pcMojiVect->GetWakuDMiniMax(&minix, &miniy, &maxx, &maxy);
	cx = (maxx + minix) / 2.0;
	cy = (maxy + miniy) / 2.0;
	tsin = sin(rag*M_1RAG);
	tcos = cos(rag*M_1RAG);
	pcMojiVect->RoundTrns(cx, cy, tsin, tcos);
}

void CCmdMoji::DrawBoxs(CDC *pcDC)
{
	DBL		dsx, dsy;
	DBL		dex, dey;
	int		sx, sy, ex, ey;

	dsx = m_dSX; dsy = m_dSY;
	dex = dsx + m_dWD; dey = dsy + m_dHI;

	scrndxymmtorltv(&dsx, &dsy);
	scrndxymmtorltv(&dex, &dey);

	scrnsetslctpen(pcDC->m_hDC);

	sx = (int)dsx; sy = (int)dsy;
	ex = (int)dex; ey = (int)dey;
	pcDC->MoveTo(sx, sy);
	pcDC->LineTo(ex, sy);
	pcDC->LineTo(ex, ey);
	pcDC->LineTo(sx, ey);
	pcDC->LineTo(sx, sy);

	scrnpenend(pcDC->m_hDC);

	dsx = m_dSX - MOJISIZE/4;
	dsy = m_dSY - MOJISIZE/4;
	dex = m_dSX + m_dWD + MOJISIZE/4;
	dey = m_dSY + m_dHI + MOJISIZE/4;

	scrndxymmtorltv(&dsx, &dsy);
	scrndxymmtorltv(&dex, &dey);

	scrnsetslctdotpen(pcDC->m_hDC);

	sx = (int)dsx; sy = (int)dsy;
	ex = (int)dex; ey = (int)dey;
	pcDC->MoveTo(sx, sy);
	pcDC->LineTo(ex, sy);
	pcDC->LineTo(ex, ey);
	pcDC->LineTo(sx, ey);
	pcDC->LineTo(sx, sy);

	scrnpenend(pcDC->m_hDC);
}

void CCmdMoji::MovePointSet(DBL dx, DBL dy)
{
	m_dSX = dx - MOJISIZE/2.0;
	m_dSY = dy - MOJISIZE/2.0;
}

void CCmdMoji::InitPointSet()
{
	DBL		mmxsize, mmysize;

	scrngetmmsize(&mmxsize, &mmysize);
	m_dSX = mmxsize / 2 - MOJISIZE/2;
	m_dSY = mmysize / 2 - MOJISIZE/2;
	m_dWD = MOJISIZE;
	m_dHI = MOJISIZE;
	m_dBX = m_dSX;
	m_dBY = m_dSY;
}

BOOL CCmdMoji::MojiLoadMain()
{
	CDialogMojiLoad*	cpDialogMojiLoad;
	int		ret;
	TCHAR	fontname[RECMAX];
	TCHAR	codestr[QRECMAX];
	int		hc, lc;
	int		sjis;

	cpDialogMojiLoad = new CDialogMojiLoad(m_pcView);
	ret = cpDialogMojiLoad->DoModal();
	if(ret == IDOK){
		_tcscpy_s(fontname, cpDialogMojiLoad->m_csFontName.GetBuffer(RECMAX));
		_tcscpy_s(codestr, cpDialogMojiLoad->m_csMojiCode.GetBuffer(QRECMAX));
		hc = (codestr[0] & 0x00ff);
		if(baseiskanji(hc) == ON){
			lc = codestr[1];
			sjis = ((hc << 8) & 0x00ff00) | (lc & 0x00ff);
		}
		else{
			_stscanf_s(codestr, _T("%x"), &sjis);
		}
		MojiLoadSetVect(fontname, sjis);
	}
	else{
		sjis = 0;
	}
	delete(cpDialogMojiLoad);
	if(sjis == 0){
		return(FALSE);
	}
	return(TRUE);
}

BOOL CCmdMoji::MojiSaveMain()
{
	CDialogMojiSave*	cpDialogMojiSave;
	int		ret;
	TCHAR	fontname[RECMAX];
	TCHAR	codestr[QRECMAX];
	int		code;


	cpDialogMojiSave = new CDialogMojiSave(m_pcView);
	ret = cpDialogMojiSave->DoModal();
	if(ret == IDOK){
		_tcscpy_s(fontname, cpDialogMojiSave->m_csFontName.GetBuffer(RECMAX));
		_tcscpy_s(codestr, cpDialogMojiSave->m_csMojiCode.GetBuffer(QRECMAX));
#ifdef UNICODE
		int sjis;
		code = codestr[0] & 0x00ffff;
		if (code < 0x100) {
			_stscanf_s(codestr, _T("%x"), &sjis);
			code = baselibSjisToU16(sjis);
		}
#else
		int hc;
		hc = (codestr[0] & 0x00ff);
		if(baseiskanji(hc) == ON){
			code = ((hc << 8) & 0x00ff00) | (codestr[1] & 0x00ff);
		}
		else{
			_stscanf_s(codestr, _T("%x"), &code);
		}
#endif
		MojiSaveAtnVect(fontname, code, m_dSX, m_dSY, MOJISIZE);
	}
	else{
		code = 0;
	}
	delete(cpDialogMojiSave);
	if(code == 0){
		return(FALSE);
	}
	return(TRUE);
}

BOOL CCmdMoji::MojiDelLoadMain()
{
	CDialogMojiLoad*	cpDialogMojiLoad;
	int		ret;
	TCHAR	codestr[QRECMAX];

	cpDialogMojiLoad = new CDialogMojiLoad(m_pcView);
	cpDialogMojiLoad->SetDialogTitle(_T("“o˜^íœ"));
	ret = cpDialogMojiLoad->DoModal();
	if (ret == IDOK) {
		_tcscpy_s(m_szFontName, cpDialogMojiLoad->m_csFontName.GetBuffer(RECMAX));
		_tcscpy_s(codestr, cpDialogMojiLoad->m_csMojiCode.GetBuffer(QRECMAX));
#ifdef UNICODE
		int		sjis;
		m_nCode = (codestr[0] & 0x0000ffff);
		if (m_nCode < 0x100) {
			_stscanf_s(codestr, _T("%x"), &sjis);
			m_nCode = baselibSjisToU16(sjis);
		}
#else
		int		hc;
		hc = (codestr[0] & 0x0000ff);
		if (baseiskanji(hc) == ON) {
			m_nCode = ((hc << 8) & 0x00ff00) | (codestr[1] & 0x00ff);
		}
		else {
			_stscanf_s(codestr, _T("%x"), &m_nCode);
		}
#endif
		MojiLoadSetVect(m_szFontName, m_nCode);
	}
	else{
		m_nCode = 0;
	}
	delete(cpDialogMojiLoad);
	if(m_nCode == 0){
		return(FALSE);
	}
	return(TRUE);
}

void CCmdMoji::MojiLoadSetVect(LPTSTR fontname, int code)
{
	int		ftategaki;
	CTonDoc*	pcDoc;
	TCHAR	strgbuf[10];
	CStrgVect	*pcStrgVect;
	CDataList	*pcDataList;

	mojienvgettategakiflag(&ftategaki);
	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcStrgVect = (CStrgVect*)pcDoc->NewData(STRGVECTID);

	pcStrgVect->SetFontName(fontname);
	pcStrgVect->SetSpaceEnv(0.0, 0.0);
	pcStrgVect->SetStrStartXY(m_dSX, m_dSY+MOJISIZE);
	pcStrgVect->SetFontSize(MOJISIZE, MOJISIZE);
	pcStrgVect->SetTateYoko(ftategaki);
#ifdef UNICODE
	strgbuf[0] = code;
	strgbuf[1] = _T('\0');
#else
	strgbuf[0] = ((code >> 8) & 0x00ff);
	strgbuf[1] = (code & 0x00ff);
	strgbuf[2] = _T('\0');
#endif

	pcStrgVect->SetString(strgbuf);
	pcStrgVect->DataChange();

	pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	pcStrgVect->SetDataList(pcDataList);
	delete(pcStrgVect);

	m_pcDataGrup = (CDataGrup*)(pcDoc->NewData(DATAGRUPID));
	// m_pcDataGrup->SetData(pcDataList);
	// pcDoc->DataAddTail(pcDataList);
	// 2002.12.11 ƒ\ƒA´…—l—v–]‚É‚æ‚è•œŠˆ
	int backfillpenno = senvgetcrtfillpenno();
	int backlinepenno = senvgetcrtlinepenno();
	int backlinethick = (int)senvgetcrtlinethick();
	senvsetcrtfillpenno(0);
	senvsetcrtlinepenno(0);
	senvsetcrtlinethick(0.1);
	ListResetLoop(m_pcDataGrup, pcDataList);
	senvsetcrtfillpenno(backfillpenno);
	senvsetcrtlinepenno(backlinepenno);
	senvsetcrtlinethick(backlinethick);

	//CreateSetTomboData();
}

void CCmdMoji::ListResetLoop(CDataGrup *pcSetDataGrup, CDataList *pcGetDataList)
{
	CTonDoc*	pcDoc;
	CData*		pcData;
	CData*		pcpreData;
	CDataList*	pcTmpcDataList;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcData = pcGetDataList->m_pcDataTop;
	while(pcData != NULL){
		pcTmpcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
		pcTmpcDataList->SetData(pcData);
		pcDoc->DataAddTail(pcTmpcDataList);
		pcTmpcDataList->SetDefFillClr();
		pcTmpcDataList->SetDefOutLineClr();

		pcSetDataGrup->SetData(pcTmpcDataList);

		pcpreData = pcData;
		pcData = pcpreData->GetNext();
		pcpreData->SetNext(NULL);
	}
}

void CCmdMoji::InAreaDataAtntionOn()
{
	CTonDoc*	pcDoc;
	DBL			sx, sy, ex, ey;

	sx = m_dSX - MOJISIZE/4;
	sy = m_dSY - MOJISIZE/4;
	ex = m_dSX + m_dWD + MOJISIZE/4;
	ey = m_dSY + m_dHI + MOJISIZE/4;
	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->OffFlag(ATNTION1FLAG);
	pcDoc->InAreaDataAtntionOn(sx, sy, ex, ey);
}

void CCmdMoji::MojiSaveAtnVect(LPTSTR fontname, int code, DBL sx, DBL sy, DBL size)
{
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->MojiSaveAtnVect(fontname, code, sx, sy, size);
}

void CCmdMoji::MojiSaveDelete(LPTSTR fontname, UINT code)
{
	CTonDoc*	pcDoc;
	CESF0Font*  pcESF0Font;
	CESF1Font*  pcESF1Font;
	CFBU0Font*  pcFBU0Font;
	int		nFontType;
	TCHAR	sGJFaceName[FACEMAX];
	TCHAR	sFaceName[FACEMAX];
	int		sjis;

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
#ifdef UNICODE
	sjis = baselibU16ToSjis(code);
#else
	sjis = code;
#endif
	pcESF0Font = pcDoc->m_pcESF0Font;
	pcESF1Font = pcDoc->m_pcESF1Font;
	pcFBU0Font = pcDoc->m_pcFBU0Font;
	mojienvflistopen(_T("rb"));
	mojienvgetfonttypename(fontname, &nFontType, sFaceName);
	mojienvflistclose();
	switch(nFontType){
	case ES0FONT:
		pcESF0Font->FontNameSet(sFaceName);
		pcESF0Font->UserDataDelete(sjis);
		break;
	case ES1FONT:
		pcESF1Font->FontNameSet(sFaceName);
		pcESF1Font->UserDataDelete(sjis);
		break;
	case FB0FONT:
		pcFBU0Font->FontNameSet(sFaceName);
		pcFBU0Font->UserDataDelete(code);
		break;
	case TT1FONT:
	case TT2FONT:
	case TT3FONT:
		_tcscpy_s(sGJFaceName, fontname);
		sGJFaceName[8] = _T('\0');
		pcESF1Font->FontNameSet(sGJFaceName);
		pcESF1Font->UserDataDelete(sjis);
		break;
	}
}

void CCmdMoji::DeleteGrup(CDataGrup * pcGetDataGrup)
{
	CData	*pcData;
	CData	*pcpreData;

	pcData = pcGetDataGrup->m_pcDataTop;
	while(pcData != NULL){
		pcpreData = pcData;
		pcData = pcpreData->GetNext();
		pcpreData->SetNext(NULL);
	}
	pcGetDataGrup->m_pcDataTop = NULL;
	delete(pcGetDataGrup);
}

void CCmdMoji::CreateSetTomboData()
{
	CTonDoc*	pcDoc;
	CDataList *pcDataList;
	CVect	*pcVect;
	DBL		sx1, sy1;
	DBL		ex1, ey1;
	DBL		sx2, sy2;
	DBL		ex2, ey2;
	DBL		sx3, sy3;
	DBL		ex3, ey3;

	sx1 = m_dSX;
	sy1 = m_dSY;
	ex1 = m_dSX + m_dWD;
	ey1 = m_dSY + m_dHI;

	sx2 = m_dSX - MOJISIZE/4;
	sy2 = m_dSY - MOJISIZE/4;
	ex2 = m_dSX + m_dWD + MOJISIZE/4;
	ey2 = m_dSY + m_dHI + MOJISIZE/4;

	sx3 = m_dSX - MOJISIZE/3;
	sy3 = m_dSY - MOJISIZE/3;
	ex3 = m_dSX + m_dWD + MOJISIZE/3;
	ey3 = m_dSY + m_dHI + MOJISIZE/3;


	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDataList = (CDataList*)(pcDoc->NewData(DATALISTID));
	pcDoc->DataAddTail(pcDataList);
	m_pcDataGrup->SetData((CData*)pcDataList);
	//CreateSetKagi(m_pcDataList, sx3, sy3, sx1, sy1);
	pcVect = new CVect(BASEVECTID);
	pcDataList->SetData((CData*)pcVect);
	pcVect->OffFlag(REJIONFLAG);
	pcVect->SetAtrDXY(0, 0, sx3, sy1);
	pcVect->SetAtrDXY(1, 0, sx1, sy1);
	pcVect->SetAtrDXY(2, 0, sx1, sy3);
	pcVect->SetPointCount(3);

	pcVect = new CVect(BASEVECTID);
	pcDataList->SetData((CData*)pcVect);
	pcVect->OffFlag(REJIONFLAG);
	pcVect->SetAtrDXY(0, 0, ex3, ey1);
	pcVect->SetAtrDXY(1, 0, ex1, ey1);
	pcVect->SetAtrDXY(2, 0, ex1, ey3);
	pcVect->SetPointCount(3);

	pcVect = new CVect(BASEVECTID);
	pcDataList->SetData((CData*)pcVect);
	pcVect->OffFlag(REJIONFLAG);
	pcVect->SetAtrDXY(0, 0, sx3, ey1);
	pcVect->SetAtrDXY(1, 0, sx1, ey1);
	pcVect->SetAtrDXY(2, 0, sx1, ey3);
	pcVect->SetPointCount(3);

	pcVect = new CVect(BASEVECTID);
	pcDataList->SetData((CData*)pcVect);
	pcVect->OffFlag(REJIONFLAG);
	pcVect->SetAtrDXY(0, 0, ex3, sy1);
	pcVect->SetAtrDXY(1, 0, ex1, sy1);
	pcVect->SetAtrDXY(2, 0, ex1, sy3);
	pcVect->SetPointCount(3);

	pcVect = new CVect(BASEVECTID);
	pcDataList->SetData((CData*)pcVect);
	pcVect->OffFlag(REJIONFLAG);
	pcVect->SetAtrDXY(0, 0, sx3, sy2);
	pcVect->SetAtrDXY(1, 0, sx2, sy2);
	pcVect->SetAtrDXY(2, 0, sx2, sy3);
	pcVect->SetPointCount(3);

	pcVect = new CVect(BASEVECTID);
	pcDataList->SetData((CData*)pcVect);
	pcVect->OffFlag(REJIONFLAG);
	pcVect->SetAtrDXY(0, 0, ex3, ey2);
	pcVect->SetAtrDXY(1, 0, ex2, ey2);
	pcVect->SetAtrDXY(2, 0, ex2, ey3);
	pcVect->SetPointCount(3);

	pcVect = new CVect(BASEVECTID);
	pcDataList->SetData((CData*)pcVect);
	pcVect->OffFlag(REJIONFLAG);
	pcVect->SetAtrDXY(0, 0, sx3, ey2);
	pcVect->SetAtrDXY(1, 0, sx2, ey2);
	pcVect->SetAtrDXY(2, 0, sx2, ey3);
	pcVect->SetPointCount(3);

	pcVect = new CVect(BASEVECTID);
	pcDataList->SetData((CData*)pcVect);
	pcVect->OffFlag(REJIONFLAG);
	pcVect->SetAtrDXY(0, 0, ex3, sy2);
	pcVect->SetAtrDXY(1, 0, ex2, sy2);
	pcVect->SetAtrDXY(2, 0, ex2, sy3);
	pcVect->SetPointCount(3);
}

void CCmdMoji::FlagCheckMojiChange(int flag)
{
	CTonDoc*	pcDoc;
	CData* pcData;
	POSITION pos;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pcData = pcObjDataList->GetNext(pos);
		if(pcData == NULL){
			break;
		}
		pcData->FlagCheckMojiChange(flag);
	}
}

