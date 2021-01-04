
/////////////////////////////////////////////////////////////////////////////
// CCmdStrg

#include "stdafx.h"
#include "CmdBase.h"
#include "CmdStrg.h"

#include "DialogStrgLineup.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Mojienv.h"
//}

CCmdStrg::CCmdStrg(CScrollView* pcview)
:CCmdBase(pcview)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	TCHAR	buf[RECMAX];
	FILE	*fp;

	m_dStrgLen = 0;
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\defenv\\cmd_strg.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp != NULL){
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf"), &m_dStrgLen);
		fclose(fp);
	}
}

CCmdStrg::~CCmdStrg()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\defenv\\cmd_strg.env"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	if(fp != NULL){
		_stprintf_s(buf, _T("%4.1lf"), m_dStrgLen);
		baselibFputs(buf, fp);
		fclose(fp);
	}
}

UINT CCmdStrg::ExecStrgCreate(UINT event, DBL dx, DBL dy)
{

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		m_pcDataGrup = NULL;
		mojienvsetstrlen(m_dStrgLen);
		m_pcDialogStrgInput = new CDialogStrgInput(m_pcView);
		if(m_pcDialogStrgInput->Create(IDD_STRG_INPUT, m_pcView)){
			LocateStrgDlg();
			m_pcDialogStrgInput->InitItemElement();
			m_pcDialogStrgInput->SetItemElement();
		}
		((CTonView*)m_pcView)->RedrawFlagOn();
		ScrnAllRedraw();
		m_nMouseSetCount = 1;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			StrgBoxsDraw(m_pcDC);
			m_pcDialogStrgInput->GetItemElement();
			mojienvsetstart(dx, dy);
			m_pcDialogStrgInput->SetItemElement();
			StrgBoxsDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 2;
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			StrgBoxsDraw(m_pcDC);
			m_pcDialogStrgInput->GetItemElement();
			mojienvsetstart(dx, dy);
			m_pcDialogStrgInput->SetItemElement();
			StrgBoxsDraw(m_pcDC);
			EndDraw();
			//m_pcDialogStrgInput->m_btnCreate.SetFocus();
			m_pcDialogStrgInput->m_pcEdit->SetFocus();
			m_nMouseSetCount = 1;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 2){
			BeginDraw();
			StrgBoxsDraw(m_pcDC);
			m_pcDialogStrgInput->GetItemElement();
			mojienvsetstart(dx, dy);
			m_pcDialogStrgInput->SetItemElement();
			StrgBoxsDraw(m_pcDC);
			EndDraw();
		}
		break;
	case DLGSETEVENT:
		if(m_nMouseSetCount == 1){
			SaveUndo();
			BeginDraw();
			StrgBoxsDraw(m_pcDC);
			EndDraw();
			m_pcDialogStrgInput->GetItemElement();
			if(m_pcDataGrup != NULL){
				delete(m_pcDataGrup);
			}
			AnyStrgCreate();
			AddLinkDoc(m_pcDataGrup);
			ScrnDataRedraw(m_pcDataGrup);
			m_pcDataGrup = NULL;
			m_pcDialogStrgInput->SetItemElement();
			BeginDraw();
			StrgBoxsDraw(m_pcDC);
			EndDraw();
			//return(REDRAWEVENT);
		}
		break;
	case DLGRESETEVENT:
		if(m_nMouseSetCount == 1){
			SaveUndo();
			BeginDraw();
			StrgBoxsDraw(m_pcDC);
			if(m_pcDataGrup != NULL){
				m_pcDataGrup->AtntionDraw(m_pcDC);
				delete(m_pcDataGrup);
			}
			EndDraw();
			m_pcDialogStrgInput->GetItemElement();
			AnyStrgCreate();
			BeginDraw();
			m_pcDataGrup->AtntionDraw(m_pcDC);
			StrgBoxsDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		CDC	*pcDC;
		int	mode;

		pcDC = ((CTonView*)m_pcView)->m_pcDC;
		if(pcDC != NULL){
			mode = pcDC->GetROP2();
			pcDC->SetROP2(R2_XORPEN);
			StrgBoxsDraw(pcDC);
			pcDC->SetROP2(mode);
		}
		break;
	case RESETEVENT:
	case ENDEVENT:
		if(m_nMouseSetCount != 0){
			BeginDraw();
			StrgBoxsDraw(m_pcDC);
			EndDraw();
			if(m_pcDataGrup != NULL){
				delete(m_pcDataGrup);
			}
			m_pcDialogStrgInput->DestroyWindow();
			mojienvgetstrlen(&m_dStrgLen);
		}
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdStrg::ExecStrgEdit(UINT event, DBL dx, DBL dy)
{
	CData*	pcData;
	TCHAR	msgbuf[HRECMAX];

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		m_pcDataGrup = NULL;
		m_pcDialogStrgInput = new CDialogStrgInput(m_pcView);
		if(m_pcDialogStrgInput->Create(IDD_STRG_INPUT, m_pcView)){
			LocateStrgDlg();
			m_pcDialogStrgInput->InitItemElement();
			m_pcDialogStrgInput->SetItemElement();
		}
		m_nMouseSetCount = 1;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 1){
			m_nIDary[0] = STRGVECTID;
			m_nIDcnt = 1;
			pcData = SrchIdsData(m_nIDary, m_nIDcnt, dx, dy);
			if(pcData != NULL){ 
				SaveUndo();
				m_pcStrgVect = (CStrgVect*)pcData;
				m_pcStrgVect->GetWakuDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
				StrgElementRestore();
				m_pcDialogStrgInput->SetFileToStrgText();
				_stprintf_s(msgbuf, _T("文字列  %4.1lfmm"), m_dBEX - m_dBSX);
				m_pcDialogStrgInput->m_stStrgSize = msgbuf; 
				m_pcDialogStrgInput->SetItemElement();
				BeginDraw();
				StrgBoxsDraw(m_pcDC);
				EndDraw();
				m_pcDialogStrgInput->m_pcEdit->SetFocus();
				m_nMouseSetCount = 2;
			}
		}
		else if(m_nMouseSetCount == 2){
			BeginDraw();
			StrgBoxsDraw(m_pcDC);
			m_pcDialogStrgInput->GetItemElement();
			mojienvsetstart(dx, dy);
			m_pcDialogStrgInput->SetItemElement();
			StrgBoxsDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 3;
		}
		else if(m_nMouseSetCount == 3){
			BeginDraw();
			StrgBoxsDraw(m_pcDC);
			m_pcDialogStrgInput->GetItemElement();
			mojienvsetstart(dx, dy);
			m_pcDialogStrgInput->SetItemElement();
			StrgBoxsDraw(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 2;
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		if(m_nMouseSetCount == 3){
			BeginDraw();
			StrgBoxsDraw(m_pcDC);
			m_pcDialogStrgInput->GetItemElement();
			mojienvsetstart(dx, dy);
			m_pcDialogStrgInput->SetItemElement();
			StrgBoxsDraw(m_pcDC);
			EndDraw();
		}
		break;
	case DLGSETEVENT:
		if(m_nMouseSetCount == 2){
			BeginDraw();
			StrgBoxsDraw(m_pcDC);
			EndDraw();
			m_pcDialogStrgInput->GetItemElement();
			StrgElementReset();
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnDataRedraw(m_pcStrgVect);
			m_nMouseSetCount = 1;
			//return(REDRAWEVENT);
		}
		break;
	case DLGRESETEVENT:
		if(m_nMouseSetCount == 2){
			BeginDraw();
			StrgBoxsDraw(m_pcDC);

			m_pcDialogStrgInput->GetItemElement();
			StrgElementReset();

			m_pcStrgVect->AtntionDraw(m_pcDC);
			StrgBoxsDraw(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:	// 要修正
		CDC	*pcDC;
		int	mode;

		if(m_nMouseSetCount == 2){
			pcDC = ((CTonView*)m_pcView)->m_pcDC;
			if(pcDC != NULL){
				mode = pcDC->GetROP2();
				pcDC->SetROP2(R2_XORPEN);
				StrgBoxsDraw(pcDC);
				if(m_pcStrgVect != NULL){
					m_pcStrgVect->AtntionDraw(pcDC);
				}
				pcDC->SetROP2(mode);
			}
		}
		break;
	case RESETEVENT:
	case ENDEVENT:
		if(m_nMouseSetCount == 3){
			BeginDraw();
			StrgBoxsDraw(m_pcDC);
			EndDraw();
		}
		m_pcDialogStrgInput->DestroyWindow();
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdStrg::ExecStrgOrder(UINT event, DBL dx, DBL dy)
{
	CData*	pcData;
	int		ret;

	switch(event){
	case INITEVENT:
		CDialogStrgLineup*	pDialogStrgLineup;
		m_nMouseSetCount = 0;
		pDialogStrgLineup = new CDialogStrgLineup(m_pcView);
		ret = pDialogStrgLineup->DoModal();
		if(ret == IDOK){
			m_nMode = pDialogStrgLineup->m_nSlctMode;
		}
		delete(pDialogStrgLineup);
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			m_nIDary[0] = STRGVECTID;
			m_nIDcnt = 1;
			pcData = SrchIdsData(m_nIDary, m_nIDcnt, dx, dy);
			if(pcData != NULL){ 
				m_pcStrgVect = (CStrgVect*)pcData;
				StdLineInit();
				BeginDraw();
				DrawStdLine(m_pcDC);
				EndDraw();
				m_nMouseSetCount = 1;
			}
		}
		else if(m_nMouseSetCount == 1){
			m_nIDary[0] = STRGVECTID;
			m_nIDcnt = 1;
			pcData = SrchIdsData(m_nIDary, m_nIDcnt, dx, dy);
			if(pcData != NULL){ 
				SaveUndo();
				m_pcStrgVect = (CStrgVect*)pcData;
				m_pcStrgVect->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
				MoveStrgData();
				ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
				ScrnDataRedraw(m_pcStrgVect);
			}
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawStdLine(m_pcDC);
			EndDraw();
			m_nMouseSetCount = 0;
		}
		//return(REDRAWEVENT);
		break;
	case ENDEVENT:
		ScrnAllRedraw();
		m_nMouseSetCount = 0;
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdStrg::ExecStrgRound(UINT event, DBL dx, DBL dy)
{
	CData*	pcData;

	switch(event){
	case SETEVENT:
		m_nIDary[0] = STRGVECTID;
		m_nIDcnt = 1;
		pcData = SrchIdsData(m_nIDary, m_nIDcnt, dx, dy);
		if(pcData != NULL){ 
			SaveUndo();
			m_pcStrgVect = (CStrgVect*)pcData;
			m_pcStrgVect->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
			StrgRoundMain();
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnDataRedraw(m_pcStrgVect);
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

UINT CCmdStrg::ExecStrgSlop(UINT event, DBL dx, DBL dy)
{
	CData*	pcData;

	switch(event){
	case SETEVENT:
		m_nIDary[0] = STRGVECTID;
		m_nIDcnt = 1;
		pcData = SrchIdsData(m_nIDary, m_nIDcnt, dx, dy);
		if(pcData != NULL){ 
			SaveUndo();
			m_pcStrgVect = (CStrgVect*)pcData;
			m_pcStrgVect->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
			StrgSlopMain();
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnDataRedraw(m_pcStrgVect);
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

UINT CCmdStrg::ExecStrgBoxsin(UINT event, DBL dx, DBL dy)
{
	CData*	pcData;
	DBL		xx, yy;
	int		tpp;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		if(m_nMouseSetCount == 0){
			senvsetnoselectstrgvect(ON);
			pcData = SrchDataOnLinePoint(dx, dy, &xx, &yy, &tpp);
			senvsetnoselectstrgvect(ON);
			if(pcData != NULL 
			&& pcData->CheckID() == BASEVECTID){
				m_pcVect = (CVect*)pcData;
				if(m_pcVect->GetPointCount() == 4){
					BeginDraw();
					m_pcVect->OnFlag(ATNTION1FLAG);
					m_pcVect->AtntionDraw(m_pcDC);
					EndDraw();
					m_nMouseSetCount = 1;
				}
			}
		}
		else if(m_nMouseSetCount == 1){
			m_nIDary[0] = STRGVECTID;
			m_nIDcnt = 1;
			pcData = SrchIdsData(m_nIDary, m_nIDcnt, dx, dy);
			if(pcData != NULL){ 
				SaveUndo();
				m_pcStrgVect = (CStrgVect*)pcData;
				m_pcStrgVect->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
				StrgBoxsinMain();
				m_pcVect->OffFlag(ATNTION1FLAG);
				ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
				ScrnDataRedraw(m_pcStrgVect);
				m_nMouseSetCount = 0;
				//return(REDRAWEVENT);
			}
		}
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdStrg::ExecPlgning(UINT event, DBL dx, DBL dy)
{
	CData*	pcData;
	CData*	pcnewData;

	switch(event){
	case INITEVENT:
		HlpMsgDsp(_T("図形に変換する文字列を指定してください"));
		break;
	case SETEVENT:
		m_nIDary[0] = STRGVECTID;
		m_nIDary[1] = SWAKVECTID;
		m_nIDary[2] = SPTNVECTID;
		m_nIDcnt = 3;
		pcData = SrchIdsData(m_nIDary, m_nIDcnt, dx, dy);
		if(pcData != NULL){ 
			SaveUndo();
			m_pcStrgVect = (CStrgVect*)pcData;
			pcnewData = StrgDataChange();
			CTonDoc* pcDoc = (CTonDoc*)(m_pcView->GetDocument());
			pcDoc->DeleteData(pcData);
			delete(pcData);
			ScrnDataRedraw(pcnewData);
		}
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdStrg::ExecStrgSpace(UINT event, DBL dx, DBL dy)
{
	CTonDoc* pcDoc;
	CData*	pcData;
	DBL		space;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		pcDoc = (CTonDoc*)(m_pcView->GetDocument());
		pcDoc->OffFlag(ATNTION1FLAG);
		HlpMsgDsp(_T("左ボタンで文字列選択、右ボタンで文字列間隔入力です"));
		break;
	case SETEVENT:
		m_nIDary[0] = STRGVECTID;
		m_nIDcnt = 1;
		pcData = SrchIdsData(m_nIDary, m_nIDcnt, dx, dy);
		if(pcData != NULL){ 
			BeginDraw();
			if(m_nMouseSetCount == 0){
				m_cPtrList.RemoveAll();
				m_nMouseSetCount = 1;
			}
			m_cPtrList.AddTail(pcData);
			pcData->OnFlag(ATNTION1FLAG);
			pcData->AtntionDraw(m_pcDC);
			EndDraw();
		}
		break;
	case RESETEVENT:
		if(ReadStrgSpace(&space) == FALSE){
			ScrnAllRedraw();
			return(ENDEVENT);
		}
		SaveUndo();
		StrgSpaceMain(space);
		ScrnAllRedraw();
		m_nMouseSetCount = 0;
		pcDoc = (CTonDoc*)(m_pcView->GetDocument());
		pcDoc->OffFlag(ATNTION1FLAG);
		break;
	case ENDEVENT:
		ScrnAllRedraw();
		m_nMouseSetCount = 0;
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdStrg::ExecMojiDel(UINT event, DBL dx, DBL dy)
{
	CData*	pcData;

	switch(event){
	case SETEVENT:
		pcData = SrchMoji(dx, dy, &m_nMjidx);
		if(pcData != NULL 
		&& pcData->CheckID() == STRGVECTID){
			SaveUndo();
			m_pcStrgVect = (CStrgVect*)pcData;
			m_pcStrgVect->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
			MojiDelMain();
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnDataRedraw(m_pcStrgVect);
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

UINT CCmdStrg::ExecMojiInseart(UINT event, DBL dx, DBL dy)
{
	CData*	pcData;

	switch(event){
	case SETEVENT:
		pcData = SrchMoji(dx, dy, &m_nMjidx);
		if(pcData != NULL 
		&& pcData->CheckID() == STRGVECTID){
			SaveUndo();
			m_pcStrgVect = (CStrgVect*)pcData;
			m_pcStrgVect->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
			BeginDraw();
			m_pcStrgVect->AtntionMojiDraw(m_pcDC, m_nMjidx);
			EndDraw();
			StrgMojiInseart();
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnDataRedraw(m_pcStrgVect);
		}
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdStrg::ExecMojiSpace(UINT event, DBL dx, DBL dy)
{
	CData*	pcData;
	int		ret;

	switch(event){
	case SETEVENT:
		pcData = SrchMoji(dx, dy, &m_nMjidx);
		if(pcData != NULL){ 
			if(pcData->CheckID() == STRGVECTID
			|| pcData->CheckID() == SWAKVECTID){
				SaveUndo();
				m_pcStrgVect = (CStrgVect*)pcData;
				m_pcStrgVect->GetDMiniMax(&m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
				ret = MojiSpaceMain();
				ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
				ScrnDataRedraw(m_pcStrgVect);
				if(ret == 0){
					//return(REDRAWEVENT);
				}
			}
		}
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

void CCmdStrg::LocateStrgDlg()
{
	CTonApp*	theApp;
	CMainFrame*	theWnd;
	CRect	prect;
	CRect	crect;
	int		x, y, cx, cy;

	m_pcView->GetWindowRect(&prect);
	m_pcDialogStrgInput->GetWindowRect(&crect);
	x = prect.right - crect.Width();
	y = prect.top;
	cx = crect.Width();
	cy = crect.Height();

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	m_pcDialogStrgInput->ShowWindow(SW_SHOW);
	::SetWindowPos(m_pcDialogStrgInput->m_hWnd, HWND_TOP, x, y, cx, cy, 0);
}

void CCmdStrg::StrgBoxsDraw(CDC *pcDC)
{
	int		ftategaki;
	DBL		dwidth, dhight;
	DBL		d_tcslen;
	DBL		dmjsp, dlnsp;
	DBL		movex, movey;

	mojienvgettategakiflag(&ftategaki);
	mojienvgetsize(&dwidth, &dhight);
	mojienvgetstrlen(&d_tcslen);
	mojienvgetspace(&dmjsp, &dlnsp);
	mojienvgetstart(&m_dSX, &m_dSY);
	if(0 < d_tcslen){
		if(ftategaki == ON){
			m_dEX = m_dSX + dwidth;
			m_dEY = m_dSY - d_tcslen;
		}
		else{
			m_dEX = m_dSX + d_tcslen;
			m_dEY = m_dSY - dhight;
		}
		SetCntrXYCntrKind(m_dSX, m_dSY, m_dEX, m_dEY);
		movex = m_dSX - m_dCX;
		movey = m_dSY - m_dCY;
		m_dSX += movex; m_dEX += movex;
		m_dSY += movey; m_dEY += movey;
		DrawAtnBoxs(pcDC);
	}
	else{
		m_dEX = m_dSX + dwidth;
		m_dEY = m_dSY - dhight;
		if(ftategaki == ON){
			m_dEY = m_dEY - dmjsp;
			m_dEY = m_dEY - dhight;
		}
		else{
			m_dEX = m_dEX + dmjsp;
			m_dEX = m_dEX + dwidth;
		}
		SetCntrXYCntrKind(m_dSX, m_dSY, m_dEX, m_dEY);
		movex = m_dSX - m_dCX;
		movey = m_dSY - m_dCY;

		m_dSX += movex;
		m_dSY += movey;
		m_dEX = m_dSX + dwidth;
		m_dEY = m_dSY - dhight;
		DrawAtnBoxs(pcDC);
		if(ftategaki == ON){
			m_dSY = m_dEY - dmjsp;
			m_dEY = m_dSY - dhight;
		}
		else{
			m_dSX = m_dEX + dmjsp;
			m_dEX = m_dSX + dwidth;
		}
		DrawAtnBoxs(pcDC);
	}
}

void CCmdStrg::AnyStrgCreate()
{
	CTonDoc*	pcDoc;
	CStrgVect*	pcStrgVect;
	TCHAR	fontname[FNAMEMAX];
	int		ftategaki;
	DBL		dstartx, dstarty;
	DBL		dwidth, dhight;
	DBL		d_tcslen;
	DBL		dmojispace, dstrgspace;
	DBL		dlimit;
	int		skind;
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	strg[RECMAX];
	DBL		minix, miniy, maxx, maxy;
	DBL		movex, movey;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataGrup = (CDataGrup*)(pcDoc->NewData(DATAGRUPID));
	if(m_pcDataGrup == NULL){
		return;
	}

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\strgtext.lst"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		return;
	}

	mojienvgetfontname(fontname);
	mojienvgettategakiflag(&ftategaki);
	mojienvgetstart(&dstartx, &dstarty);
	mojienvgetsize(&dwidth, &dhight);
	mojienvgetstrlen(&d_tcslen);
	mojienvgetspace(&dmojispace, &dstrgspace);
	mojienvgettrnslimit(&dlimit);
	mojienvgetspacekind(&skind);
	m_dSX = dstartx;
	m_dSY = dstarty;
	while(1){
		pcStrgVect = (CStrgVect*)(pcDoc->NewData(STRGVECTID));
		if(pcStrgVect == NULL){
			break;
		}
		
		if(baselibCrDelFgets(strg, RECMAX, fp) == NULL){
			break;
		}
		if(strg[0] != _T('\0')){
			pcStrgVect->SetSpaceEnv(dmojispace, dstrgspace);
			pcStrgVect->SetStrStartXY(dstartx, dstarty);
			pcStrgVect->SetFontSize(dwidth, dhight);
			pcStrgVect->SetStrLen(d_tcslen);
			if(d_tcslen != 0){
				pcStrgVect->SetTrnsPar(dlimit);
			}
			pcStrgVect->SetSpaceKind(skind);
			pcStrgVect->SetTateYoko(ftategaki);
			if(ftategaki == TATEGAKI || ftategaki == YOKOOKI){
				dstartx = dstartx - dwidth - dstrgspace; 
			}else{
				dstarty = dstarty - dhight - dstrgspace; 
			}

			pcStrgVect->SetFontName(fontname);
			pcStrgVect->SetString(strg);

			m_pcDataGrup->SetData((CData*)pcStrgVect);
			pcStrgVect->OnFlag(ATNTION1FLAG);
		}
	}
	fclose(fp);
	mojienvsetstart(dstartx, dstarty);

	MoveStringOder();

	m_pcDataGrup->RenewMiniMax();
	m_pcDataGrup->GetDMiniMax(&minix, &miniy, &maxx, &maxy);
	SetCntrXYCntrKind(minix, maxy, maxx, miniy);
	movex = m_dSX - m_dCX;
	movey = m_dSY - m_dCY;
	m_pcDataGrup->MoveTrns(movex, movey);
	m_pcDataGrup->RenewMiniMax();

	_tfopen_s(&fp, fname, _T("w"));
	if(fp == NULL){
		return;
	}
	fclose(fp);
}

void CCmdStrg::StdLineInit()
{
	DBL		sx, sy, ex, ey;
	DBL		mmxsize, mmysize;

	scrngetmmsize(&mmxsize, &mmysize);
	m_pcStrgVect->GetDMiniMax(&sx, &sy, &ex, &ey);
	switch(m_nMode){
	case LINEUPUP:
		m_dSX = 0;
		m_dEX = mmxsize; 
		m_dSY = ey;
		m_dEY = ey;
		break;
	case LINEUPUDCNTR:
		m_dSX = 0;
		m_dEX = mmxsize; 
		m_dSY = (sy + ey) / 2.0;
		m_dEY = (sy + ey) / 2.0;
		break;
	case LINEUPDOWN:
		m_dSX = 0;
		m_dEX = mmxsize; 
		m_dSY = sy;
		m_dEY = sy;
		break;
	case LINEUPLEFT:
		m_dSX = sx;
		m_dEX = sx;
		m_dSY = 0;
		m_dEY = mmysize; 
		break;
	case LINEUPLRCNTR:
		m_dSX = (sx + ex) / 2.0;
		m_dEX = (sx + ex) / 2.0;
		m_dSY = 0;
		m_dEY = mmysize; 
		break;
	case LINEUPRIGHT:
		m_dSX = ex;
		m_dEX = ex;
		m_dSY = 0;
		m_dEY = mmysize; 
		break;
	}
}

void CCmdStrg::DrawStdLine(CDC *pcDC)
{
	DBL		sx, sy;
	DBL		ex, ey;

	sx = m_dSX; sy = m_dSY;
	ex = m_dEX; ey = m_dEY;

	scrndxymmtorltv(&sx, &sy);
	scrndxymmtorltv(&ex, &ey);

	scrnsetslctpen(pcDC->m_hDC);
	pcDC->MoveTo((int)sx, (int)sy);
	pcDC->LineTo((int)ex, (int)ey);

	scrnpenend(pcDC->m_hDC);
}

void CCmdStrg::MoveStrgData()
{
	DBL		movex, movey;
	DBL		sx, sy, ex, ey;

	m_pcStrgVect->GetDMiniMax(&sx, &sy, &ex, &ey);
	switch(m_nMode){
	case LINEUPUP:
		movex = 0;
		movey = m_dSY - ey;
		break;
	case LINEUPUDCNTR:
		movex = 0;
		movey = m_dSY - (sy+ey)/2;
		break;
	case LINEUPDOWN:
		movex = 0;
		movey = m_dSY - sy;
		break;
	case LINEUPLEFT:
		movex = m_dSX - sx;
		movey = 0;
		break;
	case LINEUPLRCNTR:
		movex = m_dSX - (sx+ex)/2;
		movey = 0;
		break;
	case LINEUPRIGHT:
		movex = m_dSX - ex;
		movey = 0;
		break;
	}
	m_pcStrgVect->MoveTrns(movex, movey);	
}

void CCmdStrg::StrgBoxsinMain()
{
	DBL		bxsx, bxsy, bxex, bxey;
	int		flag;
	TCHAR	szStr[RECMAX];
	int		tate;
	DBL		hight, width, _tcslen; 

	mojienvgetspacekind(&flag);
	mojienvsetspacekind(ON);
	m_pcVect->GetDMiniMax(&bxsx, &bxsy, &bxex, &bxey);
	m_pcStrgVect->GetString(szStr);
	m_pcStrgVect->GetFontSize(&width, &hight);
	tate = m_pcStrgVect->CheckTateYoko();
	if(tate == ON){
		_tcslen = bxey - bxsy;
		width = bxex - bxsx;
		hight= width;
	}
	else{
		_tcslen = bxex - bxsx;
		hight = bxey - bxsy;
		width = hight;
	}
	m_pcStrgVect->SetStrStartXY(bxsx, bxey);
	m_pcStrgVect->SetStrLen(_tcslen);
	m_pcStrgVect->SetFontSize(width, hight);
	m_pcStrgVect->SetString(szStr);
	mojienvsetspacekind(flag);
}

void CCmdStrg::StrgRoundMain()
{
	CDialogInputNum	*pDialogInputNum;
	DBL		rag;
	DBL		tsin, tcos;
	DBL		cx, cy;
	DBL		minix, miniy, maxx, maxy;

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("角度指定回転変更"));
	pDialogInputNum->SetUnitText(_T("度"));
	pDialogInputNum->SetTitleText(_T("回転角度"));
	pDialogInputNum->m_dInputNum = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		rag = pDialogInputNum->m_dInputNum;
	}
	else{
		rag = 0;
	}
	delete(pDialogInputNum);
	m_pcStrgVect->GetDMiniMax(&minix, &miniy, &maxx, &maxy);
	GetDataCenter(minix, miniy, maxx, maxy, &cx, &cy);
	if(rag != 0){
		tsin = sin(rag*M_1RAG);
		tcos = cos(rag*M_1RAG);
		m_pcStrgVect->RoundTrns(cx, cy, tsin, tcos);
	}
}

void CCmdStrg::StrgSlopMain()
{
	CDialogInputNum	*pDialogInputNum;
	DBL		rag;
	DBL		ddx, ddy;
	DBL		cx, cy;
	DBL		minix, miniy, maxx, maxy;

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("角度指定斜形変更"));
	pDialogInputNum->SetUnitText(_T("度"));
	pDialogInputNum->SetTitleText(_T("斜形角度"));
	pDialogInputNum->m_dInputNum = 0;
	if(pDialogInputNum->DoModal() == IDOK){
		rag = pDialogInputNum->m_dInputNum;
	}
	else{
		rag = 0;
	}
	delete(pDialogInputNum);
	m_pcStrgVect->GetDMiniMax(&minix, &miniy, &maxx, &maxy);
	GetDataCenter(minix, miniy, maxx, maxy, &cx, &cy);
	
	if(rag != 0){
		ddx = tan(rag*M_1RAG);
		ddy = 0;
		m_pcStrgVect->SlopTrns(cx, cy, ddx, ddy);
	}
}

void CCmdStrg::StrgElementRestore()
{
	DBL		dwidth, dhight;
	DBL		d_tcslen;
	DBL		dstartx, dstarty;
	DBL		dmojispace, dstrgspace;
	int		ftategaki;
	TCHAR	fontname[RECMAX];
	TCHAR	instr[RECMAX];
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	DBL		minx, miny, maxx, maxy;
	DBL		movex, movey;
	int		skind;
	int		yokomoji;

	m_pcStrgVect->GetFontName(fontname);
	mojienvsetfontname(fontname);
	m_pcStrgVect->GetString(instr);
	mojienvsetstring(instr);
	m_pcStrgVect->GetSpaceEnv(&dmojispace, &dstrgspace);
	m_pcStrgVect->GetFontSize(&dwidth, &dhight);
	m_pcStrgVect->GetStrLen(&d_tcslen);
	m_pcStrgVect->GetStrStartXY(&dstartx, &dstarty);
	ftategaki = m_pcStrgVect->CheckTateYoko();
	skind = m_pcStrgVect->CheckSpaceKind();
	yokomoji = m_pcStrgVect->CheckYokoMojiFlag();
	if(d_tcslen != 0){
		GetRealMojiSpace(ftategaki, skind, &dmojispace);
	}
	m_dSX = dstartx;
	m_dSY = dstarty;
	m_pcStrgVect->GetDMiniMax(&minx, &miny, &maxx, &maxy);
	SetCntrXYCntrKind(minx, maxy, maxx, miny);
	movex = m_dSX - m_dCX;
	movey = m_dSY - m_dCY;

	mojienvsetspacekind(skind);
	mojienvsetstart(dstartx-movex, dstarty-movey);
	mojienvsetsize(dwidth, dhight);
	mojienvsetstrlen(d_tcslen);
	mojienvsetspace(dmojispace, dstrgspace);
	mojienvsettategakiflag(ftategaki);

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\strgtext.lst"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	if(fp == NULL){
		return;
	}
	_stprintf_s(buf, _T("%s\n"), instr);
	baselibFputs(buf, fp);
	fclose(fp);
}

void CCmdStrg::StrgElementReset()
{
	DBL		dwidth, dhight;
	DBL		d_tcslen;
	DBL		dstartx, dstarty;
	DBL		dmojispace, dstrgspace;
	DBL		dlimit;
	int		ftategaki;
	int		skind;
	TCHAR	fontname[RECMAX];
	TCHAR	instr[RECMAX];
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	strg[RECMAX];
	DBL		minix, miniy, maxx, maxy;
	DBL		movex, movey;

	mojienvgetstart(&dstartx, &dstarty);
	mojienvgetsize(&dwidth, &dhight);
	mojienvgetstrlen(&d_tcslen);
	mojienvgetspace(&dmojispace, &dstrgspace);
	mojienvgetfontname(fontname);
	mojienvgetstring(instr);
	mojienvgettategakiflag(&ftategaki);
	mojienvgettrnslimit(&dlimit);
	mojienvgetspacekind(&skind);

	m_pcStrgVect->SetSpaceKind(skind);
	m_pcStrgVect->SetTateYoko(ftategaki);
	m_pcStrgVect->SetFontName(fontname);
	m_pcStrgVect->SetSpaceEnv(dmojispace, dstrgspace);
	m_pcStrgVect->SetFontSize(dwidth, dhight);
	m_pcStrgVect->SetStrLen(d_tcslen);
	if(d_tcslen != 0){
		m_pcStrgVect->SetTrnsPar(dlimit);
	}
	m_pcStrgVect->SetStrStartXY(dstartx, dstarty);
	m_dSX = dstartx;
	m_dSY = dstarty;
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\strgtext.lst"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		return;
	}
	if(baselibCrDelFgets(strg, RECMAX, fp) != NULL){
		m_pcStrgVect->SetString(_T(""));
		m_pcStrgVect->SetString(strg);
	}
	m_pcStrgVect->RenewMiniMax();
	m_pcStrgVect->GetDMiniMax(&minix, &miniy, &maxx, &maxy);
	SetCntrXYCntrKind(minix, maxy, maxx, miniy);
	movex = m_dSX - m_dCX;
	movey = m_dSY - m_dCY;
	m_pcStrgVect->MoveTrns(movex, movey);
	m_pcStrgVect->RenewMiniMax();


	fclose(fp);
}

CData* CCmdStrg::StrgDataChange()
{
	CMojiVect*	pcMojiVect;
	CDataGrup*	pcDataGrup;
	CDataList*	pcDataList;
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcDataGrup = (CDataGrup*)(pcDoc->NewData(DATAGRUPID));
	pcDoc->DataAddTail((CData *)pcDataGrup);
	pcMojiVect = m_pcStrgVect->m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->DataChange();
		if((pcMojiVect->m_pcDataList) != NULL){
			pcDataList = (CDataList*)(pcMojiVect->m_pcDataList)->CopyData(0.0, 0.0);	
			pcDataList->RenewMiniMax();
			pcDataGrup->SetData(pcDataList);
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	return(pcDataGrup);
}

void CCmdStrg::MojiDelMain()
{
	CMojiVect*	pcMojiVect;
	CMojiVect*	pcDelMojiVect;
	CMojiVect*	pcNextMojiVect;
	CTonDoc*	pcDoc;
	int			idx;
	DBL			x1, y1, x2, y2;
	DBL			movex, movey;

	pcDelMojiVect = NULL;
	pcNextMojiVect = NULL;

	idx = 0;
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcMojiVect = m_pcStrgVect->m_pcMojiTop;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(idx == m_nMjidx){
			pcDelMojiVect = pcMojiVect;
			pcNextMojiVect = pcMojiVect->GetNextMoji();
			break;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
		idx++;
	}
	if(pcNextMojiVect != NULL && pcDelMojiVect != NULL){
		pcDelMojiVect->GetStartXY(&x1, &y1);
		pcNextMojiVect->GetStartXY(&x2, &y2);
		movex = x1 - x2;
		movey = y1 - y2;
		if(m_pcStrgVect->CheckTateYoko() == ON){
			movex = 0;
		}
		else{
			movey = 0;
		}
		
		pcMojiVect = pcNextMojiVect;
		while(1){
			if(pcMojiVect == NULL){
				break;
			}
			pcMojiVect->MoveTrns(movex, movey);
			pcMojiVect = pcMojiVect->GetNextMoji();
			idx++;
		}
	}

	CMojiVect*	pcPreMojiVect;

	pcMojiVect = m_pcStrgVect->m_pcMojiTop;
	if(pcMojiVect == pcDelMojiVect){
		m_pcStrgVect->m_pcMojiTop = pcMojiVect->GetNextMoji();
		delete(pcDelMojiVect);
	}
	else{
		while(1){
			pcPreMojiVect = pcMojiVect;
			pcMojiVect = pcMojiVect->GetNextMoji();
			if(pcMojiVect == pcDelMojiVect){
				pcMojiVect = pcMojiVect->GetNextMoji();
				pcPreMojiVect->SetNextMoji(pcMojiVect);
				delete(pcDelMojiVect);
				break;
			}
			if(pcMojiVect == NULL){
				break;
			}
		}
	}

	TCHAR	szSrcStrg[RECMAX];
	TCHAR	szDstStrg[RECMAX];
	int		srcidx, dstidx;
	int		hc;

	m_pcStrgVect->GetString(szSrcStrg);
	idx = 0;
	srcidx = 0;
	dstidx = 0;
	while(1){
		if(szSrcStrg[srcidx] == 0){
			szDstStrg[dstidx] = _T('\0');
			break;
		}
#ifdef UNICODE
		if (idx != m_nMjidx) {
			szDstStrg[dstidx++] = szSrcStrg[srcidx++];
		}
		else {
			srcidx++;
		}
#else
		hc = (szSrcStrg[srcidx] & 0x00ff);
		if(baseiskanji(hc) == ON){
			if(idx != m_nMjidx){
				szDstStrg[dstidx++] = szSrcStrg[srcidx++];
				szDstStrg[dstidx++] = szSrcStrg[srcidx++];
			}
			else{
				srcidx++;
				srcidx++;
			}
		}
		else{
			if(idx != m_nMjidx){
				szDstStrg[dstidx++] = szSrcStrg[srcidx++];
			}
			else{
				srcidx++;
			}
		}
#endif
		idx++;
	}
	m_pcStrgVect->SetStringUnLoad(szDstStrg);
}

void CCmdStrg::StrgMojiInseart()
{
	TCHAR	szInsStrg[RECMAX];
	int		mode;
	CDialogStrgInseart*	pDialogStrgInseart;
	int		ret;

	pDialogStrgInseart = new CDialogStrgInseart(m_pcView);
	pDialogStrgInseart->m_csInsStrg = _T("");
	ret = pDialogStrgInseart->DoModal();
	if(ret == IDOK){
		_tcscpy_s(szInsStrg, pDialogStrgInseart->m_csInsStrg.GetBuffer(RECMAX));
		mode = pDialogStrgInseart->m_chkMode;
		if(szInsStrg[0] != _T('\0')){
			StrgInseartMain(szInsStrg, mode);
		}
	}
}

void CCmdStrg::StrgInseartMain(LPTSTR szInsStr, int mode)
{
	CStrgVect*	pcTmpStrg;
	CMojiVect*	pcTmpMoji;
	CMojiVect*	pcNext;
	CMojiVect*	pcMojiVect1;
	CMojiVect*	pcMojiVect2;
	CMojiVect*	pcMoveMoji;
	int		idx;
	int		tategaki;
	DBL		dmojispace, dstrgspace;
	DBL		dwidth, dhight;
	DBL		sx, sy, tx, ty;
	DBL		movex, movey;

	tategaki = m_pcStrgVect->CheckTateYoko();
	m_pcStrgVect->GetSpaceEnv(&dmojispace, &dstrgspace);
	m_pcStrgVect->GetFontSize(&dwidth, &dhight);
	if(mode == TRUE){
		idx = 0;
		pcMojiVect1 = NULL;
		pcMojiVect2 = m_pcStrgVect->m_pcMojiTop;
		while(1){
			if(pcMojiVect2 == NULL){
				break;
			}
			if(idx == m_nMjidx){
				break;
			}
			pcMojiVect1 = pcMojiVect2;
			pcMojiVect2 = pcMojiVect1->GetNextMoji();
			idx++;
		}
	}
	else{
		idx = 0;
		pcMojiVect1 = m_pcStrgVect->m_pcMojiTop;
		while(1){
			if(pcMojiVect1 == NULL){
				pcMojiVect2 = NULL;
				break;
			}
			if(idx == m_nMjidx){
				pcMojiVect2 = pcMojiVect1->GetNextMoji();
				break;
			}
			pcMojiVect1 = pcMojiVect1->GetNextMoji();
			idx++;
		}
	}

	if(pcMojiVect1 == NULL && pcMojiVect2 == NULL){
		return;
	}

	pcTmpStrg = (CStrgVect*)m_pcStrgVect->CopyData(0.0, 0.0);	
	pcTmpStrg->SetString(szInsStr);

	pcTmpMoji = pcTmpStrg->m_pcMojiTop;

	// データの移動
	if(pcMojiVect1 != NULL){
		pcMojiVect1->GetStartXY(&sx, &sy);
		if(tategaki == ON){
			sy = sy - dhight - dmojispace;
		}
		else{
			sx = sx + dwidth + dmojispace;
		}
		pcTmpMoji->GetStartXY(&tx, &ty);
		movex = sx - tx;
		movey = sy - ty;
		pcTmpStrg->MoveTrns(movex, movey);
	}

	if(pcMojiVect2 != NULL){
		pcNext = pcTmpMoji;
		while(1){
			if(pcNext == NULL){
				break;
			}
			pcTmpMoji = pcNext;
			pcNext = pcTmpMoji->GetNextMoji();
		}
		pcTmpMoji->GetStartXY(&sx, &sy);
		if(tategaki == ON){
			sy = sy - dhight - dmojispace;
		}
		else{
			sx = sx + dwidth + dmojispace;
		}

		pcMojiVect2->GetStartXY(&tx, &ty);
		movex = sx - tx;
		movey = sy - ty;
		pcMoveMoji = pcMojiVect2;
		while(1){
			if(pcMoveMoji == NULL){
				break;
			}
			pcMoveMoji->MoveTrns(movex, movey);
			pcMoveMoji = pcMoveMoji->GetNextMoji();
		}
	}

	//データリンク
	pcTmpMoji = pcTmpStrg->m_pcMojiTop;
	if(pcMojiVect1 != NULL){
		pcMojiVect1->SetNextMoji(pcTmpMoji);
	}
	else{
		m_pcStrgVect->m_pcMojiTop = pcTmpMoji;
	}

	if(pcMojiVect2 != NULL){
		pcNext = pcTmpMoji;
		while(1){
			if(pcNext == NULL){
				break;
			}
			pcTmpMoji = pcNext;
			pcNext = pcTmpMoji->GetNextMoji();
		}
		pcTmpMoji->SetNextMoji(pcMojiVect2);
	}
	pcTmpStrg->m_pcMojiTop = NULL;
	delete(pcTmpStrg);

	// 文字列の結合
	TCHAR	szSrcStrg[RECMAX];
	TCHAR	szDstStrg[RECMAX];
	int		srcidx, dstidx;
	int		hc;

	m_pcStrgVect->GetString(szSrcStrg);
	idx = 0;
	srcidx = 0;
	dstidx = 0;
	while(1){
		if(szSrcStrg[srcidx] == 0){
			szDstStrg[dstidx] = _T('\0');
			break;
		}
#ifdef UNICODE
		if (idx != m_nMjidx) {
			szDstStrg[dstidx++] = szSrcStrg[srcidx++];
		}
		else {
			if (mode == FALSE) {
				szDstStrg[dstidx++] = szSrcStrg[srcidx++];
			}
			break;
		}
#else
		hc = szSrcStrg[srcidx] & 0x00ff;
		if(baseiskanji(hc) == ON){
			if(idx != m_nMjidx){
				szDstStrg[dstidx++] = szSrcStrg[srcidx++];
				szDstStrg[dstidx++] = szSrcStrg[srcidx++];
			}
			else{
				if(mode == FALSE){
					szDstStrg[dstidx++] = szSrcStrg[srcidx++];
					szDstStrg[dstidx++] = szSrcStrg[srcidx++];
				}
				break;
			}
		}
		else{
			if(idx != m_nMjidx){
				szDstStrg[dstidx++] = szSrcStrg[srcidx++];
			}
			else{
				if(mode == FALSE){
					szDstStrg[dstidx++] = szSrcStrg[srcidx++];
				}
				break;
			}
		}
#endif
		idx++;
	}
	szDstStrg[dstidx] = _T('\0');
	_tcscat_s(szInsStr, RECMAX, &(szSrcStrg[srcidx]));
	_tcscat_s(szDstStrg, RECMAX, szInsStr);
	m_pcStrgVect->SetStringUnLoad(szDstStrg);
	
}

int CCmdStrg::MojiSpaceMain()
{
	CDialogInputNum	*pDialogInputNum;
	CMojiVect*	pcMojiVect;
	DBL		dSpace;
	DBL		minix1, miniy1, maxx1, maxy1;
	DBL		minix2, miniy2, maxx2, maxy2;
	int		idx;
	int		tategaki;
	DBL		movex, movey;

	tategaki = m_pcStrgVect->CheckTateYoko();
	pcMojiVect = m_pcStrgVect->m_pcMojiTop;
	idx = 0;
	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		if(idx == m_nMjidx){
			break;
		}
		pcMojiVect = pcMojiVect->GetNextMoji();
		idx++;
	}


	pcMojiVect->GetDataDMiniMax(&minix1, &miniy1, &maxx1, &maxy1);
	pcMojiVect = pcMojiVect->GetNextMoji();
	if(pcMojiVect == NULL){
		return(FALSE);
	}
	pcMojiVect->GetDataDMiniMax(&minix2, &miniy2, &maxx2, &maxy2);
	if(tategaki == ON){
		//dSpace = (miniy2 - maxy1)*-1;
		dSpace = (maxy2 - miniy1)*-1;
	}
	else{
		dSpace = minix2 - maxx1;
	}

	BeginDraw();
	m_pcStrgVect->AtntionMojiDraw(m_pcDC, m_nMjidx);
	m_pcStrgVect->AtntionMojiDraw(m_pcDC, m_nMjidx+1);
	EndDraw();

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("文字間隔変更"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("文字間隔"));
	pDialogInputNum->m_dInputNum = FLOOR100(dSpace);
	if(pDialogInputNum->DoModal() == IDOK){
		dSpace = pDialogInputNum->m_dInputNum;
		delete(pDialogInputNum);
	}
	else{
		delete(pDialogInputNum);
		return(FALSE);
	}

	if(tategaki == ON){
		dSpace *= -1;
		movex = 0;
		movey = (miniy1 + dSpace) - maxy2;
	}
	else{
		movex = (maxx1 + dSpace) - minix2;
		movey = 0;
	}

	while(1){
		if(pcMojiVect == NULL){
			break;
		}
		pcMojiVect->MoveTrns(movex, movey);
		pcMojiVect = pcMojiVect->GetNextMoji();
	}
	return(0);
}

BOOL CCmdStrg::ReadStrgSpace(LPDBL space)
{
	CDialogInputNum	*pDialogInputNum;
	DBL		mjspace, strspace;
	BOOL	ret;

	mojienvgetspace(&mjspace, &strspace);
	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle(_T("行間隔設定"));
	pDialogInputNum->SetUnitText(_T("mm"));
	pDialogInputNum->SetTitleText(_T("行間"));
	pDialogInputNum->m_dInputNum = FLOOR100(strspace);
	if(pDialogInputNum->DoModal() == IDOK){
		*space = pDialogInputNum->m_dInputNum;
		ret = TRUE;
	}
	else{
		*space = 0;
		ret = FALSE;
	}
	delete(pDialogInputNum);
	return(ret);
}

void CCmdStrg::StrgSpaceMain(DBL space)
{
	CStrgVect	*pcStrgVect;
	DBL		minx, miny, maxx, maxy;
	DBL		movex, movey;

	POSITION pos = m_cPtrList.GetHeadPosition();
	if(pos == NULL){
		return;
	}
	pcStrgVect = (CStrgVect*)(m_cPtrList.GetNext(pos));
	pcStrgVect->GetDMiniMax(&minx, &miny, &maxx, &maxy);
	pcStrgVect->OffFlag(ATNTION1FLAG);
	m_dSX = minx; m_dSY = miny;
	while (pos != NULL)
	{
		pcStrgVect = (CStrgVect*)(m_cPtrList.GetNext(pos));
		if(pcStrgVect->CheckFlag(ATNTION1FLAG) == ON){
			pcStrgVect->GetDMiniMax(&minx, &miny, &maxx, &maxy);
			if(pcStrgVect->CheckTateYoko() == ON){
				movex = m_dSX - space - maxx;
				movey = 0;
			}
			else{
				movex = 0;
				movey = m_dSY - space - maxy;
			}
			pcStrgVect->MoveTrns(movex, movey);
			pcStrgVect->RenewMiniMax();
			pcStrgVect->GetDMiniMax(&minx, &miny, &maxx, &maxy);
			m_dSX = minx; m_dSY = miny;
			pcStrgVect->OffFlag(ATNTION1FLAG);
		}
	}
}

void CCmdStrg::SetCntrXYCntrKind(DBL sx, DBL sy, DBL ex, DBL ey)
{
	senvgetmojiconerkind(&m_nCntrKind);
	switch(m_nCntrKind){
	case LEFTUP:
		m_dCX = sx;
		m_dCY = sy;
		break;
	case LEFTDOWN:
		m_dCX = sx;
		m_dCY = ey;
		break;
	case RIGHTUP:
		m_dCX = ex;
		m_dCY = sy;
		break;
	case RIGHTDOWN:
		m_dCX = ex;
		m_dCY = ey;
		break;
	case CNTRCNTR:
		m_dCX = (sx + ex) / 2;
		m_dCY = (sy + ey) / 2;
		break;
	}
}

void CCmdStrg::MoveStringOder()
{
	CStrgVect	*pcStrgVect;
	DBL	minx, miny, maxx, maxy;
	int	mode;
	DBL	cx, cy;
	DBL	tx, ty;
	DBL	movex, movey;

	pcStrgVect = (CStrgVect*)m_pcDataGrup->m_pcDataTop;
	if(pcStrgVect == NULL){
		return;
	}

	pcStrgVect->GetDMiniMax(&minx, &miny, &maxx, &maxy);
	
	mojienvgetorder(&mode);
	switch(mode){
	case ODERLEFT:
		cx = minx;
		cy = miny;
		break;
	case ODERRIGHT:
		cx = maxx;
		cy = maxy;
		break;
	case ODERCNTR:
		cx = (minx+maxx) / 2;
		cy = (miny+maxy) / 2;
		break;
	default:
		cx = minx;
		cy = miny;
		break;
	}
	while(1){
		pcStrgVect = (CStrgVect*)((CData*)pcStrgVect)->GetNext();
		if(pcStrgVect == NULL){
			break;
		}
		pcStrgVect->GetDMiniMax(&minx, &miny, &maxx, &maxy);
		switch(mode){
		case ODERLEFT:
			tx = minx;
			ty = miny;
			break;
		case ODERRIGHT:
			tx = maxx;
			ty = maxy;
			break;
		case ODERCNTR:
			tx = (minx+maxx) / 2;
			ty = (miny+maxy) / 2;
			break;
		default:
			tx = minx;
			ty = miny;
			break;
		}
		if(pcStrgVect->CheckTateYoko() == ON){
			movex = 0;
			movey = cy - ty;
		}
		else{
			movex = cx - tx;
			movey = 0;
		}
		pcStrgVect->MoveTrns(movex, movey);
	}
}

void CCmdStrg::GetRealMojiSpace(int tategaki, int spacekind, DBL *mojispace)
{
	DBL		minx, miny, maxx, maxy;
	DBL		mjminx, mjminy, mjmaxx, mjmaxy;

	if(m_pcStrgVect->m_pcMojiTop != NULL){
		m_pcStrgVect->GetMojiWakuMiniMax(0, &mjminx, &mjminy, &mjmaxx, &mjmaxy);
		if(spacekind == ON){
			m_pcStrgVect->GetMojiMiniMax(0, &mjminx, &mjminy, &mjmaxx, &mjmaxy);
		}
		else{
			m_pcStrgVect->GetMojiWakuMiniMax(0, &mjminx, &mjminy, &mjmaxx, &mjmaxy);
		}
		if((m_pcStrgVect->m_pcMojiTop)->GetNextMoji() != NULL){
			if(spacekind == ON){
				m_pcStrgVect->GetMojiMiniMax(1, &minx, &miny, &maxx, &maxy);
			}
			else{
				m_pcStrgVect->GetMojiWakuMiniMax(1, &minx, &miny, &maxx, &maxy);
			}
			if(tategaki != ON){
				*mojispace = minx - mjmaxx;
			}
			else{
				*mojispace = mjminy - maxy;
			}
		}
	}
}
