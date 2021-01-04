
/////////////////////////////////////////////////////////////////////////////
// CCmdSWak

#include "stdafx.h"
#include "CmdBase.h"
#include "CmdSWak.h"

#include "DialogWakuAtrChange.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Mojienv.h"
//}

#define	SLOPRAG		15

CCmdSWak::CCmdSWak(CScrollView* pcview)
:CCmdBase(pcview)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	TCHAR	buf[RECMAX];
	FILE	*fp;

	_tcscpy_s(m_szFont, _T("ＭＳ明朝"));
	m_pcSWakVect = NULL;
	m_nSpaceKind = SPACEWAKU;
	m_nRoundKind = 1;
	m_nTateYoko = TATEGAKI;
	m_dSpacePar = 0;
	m_dFlatPar = MJFREEFLATPAR;			// 自動
	m_dKanaPar = 100;
	m_nOderKind = ODEREQUAL;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\defenv\\cmd_swak.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp != NULL){
		baselibCrDelFgets(buf, RECMAX, fp);
		//_stscanf_s(buf, "%s", m_szFont);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d %d"), &m_nSpaceKind, &m_nRoundKind);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d"), &m_nOderKind);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%d %lf %lf"), &m_nTateYoko, &m_dFlatPar, &m_dSpacePar);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%lf"), &m_dKanaPar);
		fclose(fp);
	}
	mojienvgetfontname(m_szFont);
}

CCmdSWak::~CCmdSWak()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	GetItemElement();
	mojienvgetfontname(m_szFont);
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\defenv\\cmd_swak.env"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	if(fp != NULL){
		_stprintf_s(buf, _T("%s\n"), m_szFont);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d %d\n"), m_nSpaceKind, m_nRoundKind);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d\n"), m_nOderKind);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%d %4.1lf %4.1lf\n"), m_nTateYoko, m_dFlatPar, m_dSpacePar);
		baselibFputs(buf, fp);
		_stprintf_s(buf, _T("%4.1lf\n"), m_dKanaPar);
		baselibFputs(buf, fp);
		fclose(fp);
	}
}

UINT CCmdSWak::ExecCreate(UINT event, DBL dx, DBL dy)
{
	DBL		tt;

	switch(event){
	case INITEVENT:
		m_dKanaPar = 100;
		m_dTCHARCount = 0;
		m_nOderKind = ODEREQUAL;
		senvsetcrtfillpenno(0);
		senvsetcrtlinepenno(0);
		senvsetcrtlinethick(0.0);
		m_nMouseSetCount = 0;
		m_pcSWakVect = NULL;
		mojienvgetfontname(m_szFont);
		InitClass();
		break;
	case SETEVENT:
	case SETEVENT2:
		if(m_nMouseSetCount == 0){
			m_nMouseSetCount = 1;
			m_pcView->StartTimer();
			m_dSX = dx; m_dSY = dy;
			m_dEX = dx; m_dEY = dy;
			SetSWakLocate();
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		else if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
			m_dEX = dx; m_dEY = dy;
			int	ret = m_pcView->EndTimer();
			if(ret <= 1){	//ドラッグしなかった時
				return(ENDEVENT);
			}	
			if(m_dSX > m_dEX){
				tt = m_dSX;
				m_dSX = m_dEX;
				m_dEX = tt;
			}
			if(m_dSY < m_dEY){
				tt = m_dSY;
				m_dSY = m_dEY;
				m_dEY = tt;
			}
			SetSWakLocate();
			SaveUndo();
			m_pcDoc->GetDMiniMax(ACTIVEFLAG, &m_dBSX, &m_dBSY, &m_dBEX, &m_dBEY);
			ActiveDataAllOff();
			GetItemElement();
			CreateSWakVect();
			m_szStrg[0] = _T('\0');
			SetItemElement();
			ScrnAreaRedraw(m_dBSX, m_dBSY, m_dBEX, m_dBEY);
			ScrnDataRedraw(m_pcSWakVect);
			m_nMouseSetCount = 0;
		}
		break;
	case MOVEEVENT:
		if(m_nMouseSetCount == 1){
			BeginDraw();
			DrawAtnBoxs(m_pcDC);
			m_dEX = dx; m_dEY = dy;
			SetSWakLocate();
			DrawAtnBoxs(m_pcDC);
			EndDraw();
		}
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		break;
	case DLGSETEVENT:
		if(m_pcSWakVect == NULL){
			return(0);
		}
		GetItemElement();
		m_pcSWakVect->GetString(m_szStrg);
		SetDataElement();
		ScrnDataRedraw(m_pcSWakVect);
		break;
	case DLGENDEVENT:
	case ENDEVENT:
		m_pcSWakVect = NULL;
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdSWak::ExecAnyInput(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		InitClass();
		SaveUndo();
		m_pcDoc->OnFlag(ACTIVEFLAG);
		ScrnAllRedraw();
		SetDataListSelectID(SWAKVECTID, ACTIVEFLAG);
		m_nMouseSetCount = 0;
		//m_pcView->DrawWakuNo(NULL, pcDoc); 
		SaveWakuString();
		m_pcDialogWakuInput = new CDialogWakuInput(m_pcView);
		if(m_pcDialogWakuInput->Create(IDD_WAKU_INPUT, m_pcView)){
			LocateRightFitDialog((CDialog*)m_pcDialogWakuInput);
		}
		m_pcDialogSlctStrg = new CDialogSlctStrg(m_pcView);
		if(m_pcDialogSlctStrg->Create(IDD_SLCTSTRG, m_pcView)){
			LocateRightBottomFitDialog((CDialog*)m_pcDialogSlctStrg);
		}
		m_pcDialogCrctDisp = new CDialogCrctDisp(m_pcView);
		if(m_pcDialogCrctDisp->Create(IDD_CRCTDISP, m_pcView)){
			LocateRightBottomFitDialog((CDialog*)m_pcDialogCrctDisp);
			m_pcDialogCrctDisp->ShowWindow(SW_HIDE);
		}
		m_pcDialogWakuInput->SetSubDialog(m_pcDialogSlctStrg);
		m_pcDialogWakuInput->m_pcDialogCrctDisp = m_pcDialogCrctDisp;
		break;
	case SETEVENT:
		break;
	case MOVEEVENT:
		break;
	case REDRAWEVENT:	// 要修正	
		SetDataListSelectID(SWAKVECTID, ACTIVEFLAG);
		SaveWakuString();
		break;
	case RESETEVENT:
		break;
	case USER1EVENT:
		m_pcDialogWakuInput->SetString(m_pcView->m_szStockStr);
		break;
	case DLGDRAWEVENT:
		SetDataListSelectID(SWAKVECTID, ACTIVEFLAG);
		LoadWakuString();
		ScrnAllRedraw();
		break;
	case DLGSETEVENT:
		m_pcDoc->OnFlag(ACTIVEFLAG);
		SetDataListSelectID(SWAKVECTID, ACTIVEFLAG);
		LoadWakuString();
		ScrnAllRedraw();
		break;
	case DLGENDEVENT:
	case DLGRESETEVENT:
	case ENDEVENT:
		LoadWakuString();
		m_pcDoc->SetModifiedFlag();
		m_pcDialogWakuInput->DestroyWindow();
		m_pcDialogSlctStrg->DestroyWindow();
		m_pcDialogCrctDisp->DestroyWindow();
		ScrnAllRedraw();
		delete(m_pcDialogSlctStrg);
		delete(m_pcDialogWakuInput);
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}
UINT CCmdSWak::ExecAnyInSekiji(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		InitClass();
		SaveUndo();
		// m_pcDoc->OnFlag(ACTIVEFLAG);
		ScrnAllRedraw();
		SetDataListSelectID(DATAGRUPID, ACTIVEFLAG);
		m_nMouseSetCount = 0;
		//m_pcView->DrawWakuNo(NULL, pcDoc); 
		SaveGroupString();
		m_pcDialogWakuInSekiji = new CDialogWakuInSekiji(m_pcView);
		m_pcDialogWakuInSekiji->SetCmdBase(this);
		if(m_pcDialogWakuInSekiji->Create(IDD_WAKU_INSEKIJI, m_pcView)){
			LocateRightFitDialog((CDialog*)m_pcDialogWakuInSekiji);
		}
		m_pcDialogSlctStrg = new CDialogSlctStrg(m_pcView);
		if(m_pcDialogSlctStrg->Create(IDD_SLCTSTRG, m_pcView)){
			LocateRightBottomFitDialog((CDialog*)m_pcDialogSlctStrg);
		}
		break;
	case SETEVENT:
		break;
	case MOVEEVENT:
		break;
	case REDRAWEVENT:	// 要修正	
		SetDataListSelectID(DATAGRUPID, ACTIVEFLAG);
		SaveGroupString();
		break;
	case RESETEVENT:
		break;
	case USER1EVENT:
		m_pcDialogWakuInSekiji->SetString(m_pcView->m_szStockStr);
		break;
	case DLGDRAWEVENT:
		SetDataListSelectID(DATAGRUPID, ACTIVEFLAG);
		LoadGroupString();
		ScrnAllRedraw();
		break;
	case DLGSETEVENT:
		m_pcDoc->OnFlag(ACTIVEFLAG);
		SetDataListSelectID(DATAGRUPID, ACTIVEFLAG);
		LoadGroupString();
		ScrnAllRedraw();
		break;
	case DLGENDEVENT:
	case DLGRESETEVENT:
	case ENDEVENT:
		SetDataListSelectID(DATAGRUPID, ACTIVEFLAG);
		LoadGroupString();
		ResetGrupNo();
		m_pcDoc->SetModifiedFlag();
		m_pcDialogWakuInSekiji->DestroyWindow();
		m_pcDialogSlctStrg->DestroyWindow();
		ScrnAllRedraw();
		delete(m_pcDialogSlctStrg);
		delete(m_pcDialogWakuInSekiji);
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}
UINT CCmdSWak::ExecWakuChange(UINT event, DBL dx, DBL dy)
{
	CDialogWakuAtrChange* pcDialogWakuAtrChange;
	POSITION	pos;
	CData*		pcData;
	CSWakVect	*pcSWakVect;
	CVect		*pcVect;
	TCHAR		fontname[RECMAX];
	TCHAR		szStr[RECMAX];

	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	pcDialogWakuAtrChange = new CDialogWakuAtrChange(m_pcView);
	if(pcDialogWakuAtrChange->DoModal() == IDOK){
		mojienvgetfontname(fontname);

		SetDataListSelectID(SWAKVECTID, ACTIVEFLAG);
		pos = m_cPtrList.GetHeadPosition();
		while (pos != NULL)
		{
			pcData = m_cPtrList.GetNext(pos);
			if(pcData == NULL){
				break;
			}
			pcSWakVect = (CSWakVect *)pcData;
			pcSWakVect->GetString(szStr);
			pcSWakVect->GetFontSize(&m_dWidth, &m_dWidth);
			m_nTateYoko = pcSWakVect->CheckTateYoko();
			pcVect = pcSWakVect->GetFitingVect();
			GetFitingVectElement(m_nTateYoko, m_dWidth, m_dHight, pcVect);
			pcSWakVect->SetFontName(fontname);
			SetFitingVectElement(pcVect);
			pcSWakVect->SetString(szStr);
			pcSWakVect->RenewMiniMax();
			if(szStr[0] != _T('\0')){
				pcSWakVect->StrgLineStruct();
				pcSWakVect->RenewMiniMax();
			}
		}
	}
	delete(pcDialogWakuAtrChange);
	ScrnAllRedraw();
	return(ENDEVENT);
}	

void CCmdSWak::LoadWakuString()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	CSWakVect	*pcSWakVect;
	POSITION pos;
	TCHAR	szStr[STRGMAX];
	TCHAR	szSetStr[STRGMAX];
	TCHAR	szFildName[RECMAX];	
	TCHAR	buf[RECMAXAD];

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\string.lst"), exepath);
	_tfopen_s(&fp, fname, _T("rb"));
	if(fp == NULL){
		return;
	}
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcSWakVect = (CSWakVect *)m_cPtrList.GetNext(pos);
		pcSWakVect->GetString(szStr);

		fread(buf, RECMAX, 1, fp);
		_tcscpy_s(szSetStr, buf);
		if(_tcscmp(szSetStr, szStr) != 0){
			if(pcSWakVect->m_nFixFlag == OFF){
				UpdateStringList(szSetStr);
				pcSWakVect->GetFildName(szFildName);
				if(szFildName[0] != _T('\0')
				&& m_pcDoc->m_pcDialogDBEdit != NULL){
					m_pcDoc->m_pcDialogDBEdit->GetFildElement(szFildName, szSetStr);
				}
				pcSWakVect->SetString(szSetStr);
				pcSWakVect->StrgLineStruct();
				pcSWakVect->RenewMiniMax();
				ScrnDataRedraw((CData*)pcSWakVect);
			}
		}
	}
	fclose(fp);
}

void CCmdSWak::SaveWakuString()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	CSWakVect	*pcSWakVect;
	POSITION pos;
	TCHAR	szStr[RECMAXAD];
	TCHAR	buf[RECMAXAD];

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\string.lst"), exepath);
	_tfopen_s(&fp, fname, _T("wb"));
	if(fp == NULL){
		return;
	}
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcSWakVect = (CSWakVect *)m_cPtrList.GetNext(pos);
		pcSWakVect->GetString(szStr);
		memset(buf, _T(' '), RECMAXAD);
		_stprintf_s(buf, _T("%s"), szStr);
		buf[RECMAX - 2] = _T('\r');
		buf[RECMAX - 1] = _T('\n');
		fwrite(buf, RECMAX, 1, fp);
	}
	fclose(fp);
}
void CCmdSWak::LoadGroupString()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	CDataGrup	*pcDataGrup;
	CSWakVect	*pcSWakVect;
	POSITION pos;
	TCHAR	buf[RECMAXAD];
	LPTSTR	csvsp[RECMAX];
	int		cnt;
	int		no, flag;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\sekiji.lst"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		return;
	}
	baselibCrDelFgets(buf, RECMAX, fp); // 最初の行はスキップ
	no = 1;
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcDataGrup = (CDataGrup *)m_cPtrList.GetNext(pos);
		if(m_pcView->CheckSekijiGrup(pcDataGrup) == TRUE){
			pcDataGrup->m_no = no;
			baselibCrDelFgets(buf, RECMAX, fp);
			cnt = basedivcsvdata(csvsp, buf);
			flag = _ttoi(csvsp[0]);
			if(flag == ON){
				pcDataGrup->OnFlag(KATAGAKIFLAG);
			}else{
				pcDataGrup->OffFlag(KATAGAKIFLAG);
			}
			pcSWakVect = (CSWakVect*)pcDataGrup->GetData(0);
			SetSWakString(pcSWakVect, csvsp[1]);
			pcSWakVect = (CSWakVect*)pcDataGrup->GetData(1);
			SetSWakString(pcSWakVect, csvsp[2]);
			pcSWakVect = (CSWakVect*)pcDataGrup->GetData(2);
			SetSWakString(pcSWakVect, csvsp[3]);
			no++;
		}
	}
	fclose(fp);
}
void CCmdSWak::SaveGroupString()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	CDataGrup	*pcDataGrup;
	CSWakVect	*pcSWakVect;
	POSITION pos;
	TCHAR	szStr1[RECMAXAD];
	TCHAR	szStr2[RECMAXAD];
	TCHAR	szStr3[RECMAXAD];
	int		no, flag;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\sekiji.lst"), exepath);
	_tfopen_s(&fp, fname, _T("w"));
	if(fp == NULL){
		return;
	}
	_stprintf_s(buf, _T("// aaaa\n"));
	baselibFputs(buf, fp);
	no = 1;
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcDataGrup = (CDataGrup *)m_cPtrList.GetNext(pos);
		if(m_pcView->CheckSekijiGrup(pcDataGrup) == TRUE){
			pcDataGrup->m_no = no;
			flag = pcDataGrup->CheckFlag(KATAGAKIFLAG);
			pcSWakVect = (CSWakVect*)pcDataGrup->GetData(0);
			pcSWakVect->GetString(szStr1);
			pcSWakVect = (CSWakVect*)pcDataGrup->GetData(1);
			pcSWakVect->GetString(szStr2);
			pcSWakVect = (CSWakVect*)pcDataGrup->GetData(2);
			pcSWakVect->GetString(szStr3);
			_stprintf_s(buf, _T("%d,%s,%s,%s,\n"), flag, szStr1, szStr2, szStr3);
			baselibFputs(buf, fp);
			no++;
		}
	}
	fclose(fp);
}
void CCmdSWak::ResetGrupNo()
{
	CDataGrup	*pcDataGrup;
	POSITION pos;
	int		no;

	no = 0;
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcDataGrup = (CDataGrup *)m_cPtrList.GetNext(pos);
		if(m_pcView->CheckSekijiGrup(pcDataGrup) == TRUE){
			pcDataGrup->m_no = no;
		}
	}
}
void CCmdSWak::SetSWakString(CSWakVect *pcSWakVect, LPTSTR lpSetStr)
{
	TCHAR	szStr[STRGMAX];

	pcSWakVect->GetString(szStr);
	if(_tcscmp(lpSetStr, szStr) != 0){
		if(pcSWakVect->m_nFixFlag == OFF){
			UpdateStringList(lpSetStr);
			pcSWakVect->SetString(lpSetStr);
			pcSWakVect->StrgLineStruct();
			pcSWakVect->RenewMiniMax();
			ScrnDataRedraw((CData*)pcSWakVect);
		}
	}
}
void CCmdSWak::SetItemElement()
{
	CSysEnv	*pcSysEnv;

	pcSysEnv = m_pcApp->m_pcSysEnv;
	pcSysEnv->m_dWidth = FLOOR10(m_dEX - m_dSX);
	pcSysEnv->m_dHight = FLOOR10(m_dEY - m_dSY);
	pcSysEnv->m_dSX = FLOOR10(m_dSX);
	pcSysEnv->m_dSY = FLOOR10(m_dSY);
	_tcscpy_s(pcSysEnv->m_szStrg, m_szStrg);
	_tcscpy_s(pcSysEnv->m_szFont, m_szFont);
	_tcscpy_s(pcSysEnv->m_szFildName, m_szFildName);
	pcSysEnv->m_dFlatPar = m_dFlatPar;
	if(m_dSpacePar == MJFREESPACEPAR){
		pcSysEnv->m_dSpaceMM = MJEQUALSP;
	}else{
		if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
			pcSysEnv->m_dSpaceMM = (int)(m_dWidth * m_dSpacePar / 100.0);
		}
		else{
			pcSysEnv->m_dSpaceMM = (int)(m_dHight * m_dSpacePar / 100.0);
		}
	}
	pcSysEnv->m_nTateYoko = m_nTateYoko;
	pcSysEnv->m_nSpaceKind = m_nSpaceKind;
	pcSysEnv->m_dTCHARCount = m_dTCHARCount;
	pcSysEnv->m_dKanaPar = m_dKanaPar;
	pcSysEnv->m_nOderKind = m_nOderKind;
	pcSysEnv->m_nYokoGumi = m_nYokoGumi;
	pcSysEnv->m_nFixFlag = m_nFixFlag;
	pcSysEnv->m_nMirror = m_nMirror;
	pcSysEnv->m_nShadowKind = m_nShadowKind;
	if(m_nShadowKind == 0){
		pcSysEnv->m_clrShadow = m_clrShadow;
		pcSysEnv->m_dShadowDistance = m_dShadowDistance;
		pcSysEnv->m_dShadowRoundRag = m_dShadowRoundRag;
	}
	SetToolBarElement();
}

void CCmdSWak::GetItemElement()
{
	CSysEnv	*pcSysEnv;

	GetToolBarElement();
	pcSysEnv = m_pcApp->m_pcSysEnv;
	m_dSX = pcSysEnv->m_dSX;
	m_dSY = pcSysEnv->m_dSY;
	m_dEX = m_dSX + pcSysEnv->m_dWidth;
	m_dEY = m_dSY + pcSysEnv->m_dHight;
	m_dWidth = m_dEX - m_dSX;
	m_dHight = m_dEY - m_dSY;
	_tcscpy_s(m_szStrg, pcSysEnv->m_szStrg);
	_tcscpy_s(m_szFont, pcSysEnv->m_szFont);
	_tcscpy_s(m_szFildName, pcSysEnv->m_szFildName);
	m_dFlatPar = pcSysEnv->m_dFlatPar;
	m_nTateYoko = pcSysEnv->m_nTateYoko;
	if(m_pcApp->m_pcSysEnv->m_dSpaceMM == MJEQUALSP){
		m_dSpacePar = MJFREESPACEPAR;
	}
	else{
		if(m_nTateYoko == TATEGAKI || m_nTateYoko == YOKOOKI){
			m_dSpacePar = (pcSysEnv->m_dSpaceMM+0.5) / m_dWidth * 100.0;
		}
		else{
			m_dSpacePar = (pcSysEnv->m_dSpaceMM+0.5) / m_dHight * 100.0;
		}
	}
	m_nSpaceKind = pcSysEnv->m_nSpaceKind;
	m_dTCHARCount = pcSysEnv->m_dTCHARCount;
	m_dKanaPar = pcSysEnv->m_dKanaPar;
	m_nOderKind = pcSysEnv->m_nOderKind;
	m_nYokoGumi = pcSysEnv->m_nYokoGumi;
	m_nFixFlag = pcSysEnv->m_nFixFlag;
	m_nMirror = pcSysEnv->m_nMirror;
	m_nShadowKind = pcSysEnv->m_nShadowKind;
	m_clrShadow = pcSysEnv->m_clrShadow;
	m_dShadowDistance = pcSysEnv->m_dShadowDistance;
	m_dShadowRoundRag = pcSysEnv->m_dShadowRoundRag;
}

void CCmdSWak::GetDataElement()
{
	CVect*		pcVect;
	int			pno;
	DBL			thick;

	pcVect = m_pcSWakVect->GetFitingVect();
	pno = m_pcSWakVect->GetFillPen();
	senvsetcrtfillpenno(pno);
	pno = m_pcSWakVect->GetOutLinePen();
	senvsetcrtlinepenno(pno);
	thick = m_pcSWakVect->GetOutLineWidth();
	senvsetcrtlinethick(thick);
	
	m_pcSWakVect->GetFlatPar(&m_dFlatPar);
	m_pcSWakVect->GetSpaceKind(&m_nSpaceKind);
	m_pcSWakVect->GetSpacePar(&m_dSpacePar);
	m_pcSWakVect->GetKanaPar(&m_dKanaPar);
	m_pcSWakVect->GetOderKind(&m_nOderKind);
	m_dTCHARCount = m_pcSWakVect->GetMaxCharCount();
	m_pcSWakVect->GetFontName(m_szFont);
	m_pcSWakVect->GetFildName(m_szFildName);
	m_nTateYoko = m_pcSWakVect->CheckTateYoko();
	m_nYokoGumi = m_pcSWakVect->m_nYokoGumi;
	m_nFixFlag = m_pcSWakVect->m_nFixFlag;
	m_nMirror = m_pcSWakVect->m_nMirror;	
	m_pcSWakVect->GetFontSize(&m_dWidth, &m_dHight);
	if(m_pcSWakVect->m_nShadowKind != 0){
		m_nShadowKind = m_pcSWakVect->m_nShadowKind;
		m_clrShadow = m_pcSWakVect->m_clrShadow;
		m_dShadowDistance = m_pcSWakVect->m_dShadowDistance;
		m_dShadowRoundRag = m_pcSWakVect->m_dShadowRoundRag;
	}
	GetFitingVectElement(m_nTateYoko, m_dWidth, m_dHight, pcVect);
	m_dWidth = m_dEX - m_dSX;
	m_dHight = m_dEY - m_dSY;
	m_pcSWakVect->GetString(m_szStrg);
}

void CCmdSWak::SetDataElement()
{
	CVect*		pcVect;
	DBL			sx, sy;
	int			pno;
	DBL			thick;
	TCHAR		szFildName[RECMAX];

	pcVect = m_pcSWakVect->GetFitingVect();
	if(m_szFont[0] == _T('\0')){
		mojienvgetfontname(m_szFont);
	}
	if(m_nOderKind == -1){
		m_nOderKind = ODEREQUAL;
	}
	if(m_dFlatPar == NOSETPAR){
		m_dFlatPar = MJFREEFLATPAR;
	}
	if(m_nSpaceKind == -1){
		m_nSpaceKind = SPACEWAKU;
	}
	if(m_dSpacePar == NOSETPAR){
		m_dSpacePar = MJFREESPACEPAR;
	}
	if(m_dTCHARCount == -1){
		m_dTCHARCount = 0;
	}
	if(m_dKanaPar == NOSETPAR){
		m_dKanaPar = 100;
	}

	if(m_nYokoGumi == -1){
		m_nYokoGumi = OFF;
	}
	if(m_nFixFlag == -1){
		m_nFixFlag = OFF;
	}
	if(m_nMirror == -1){
		m_nMirror = OFF;
	}
	
	pno = senvgetcrtfillpenno();
	m_pcSWakVect->SetFillPen(pno);
	pno = senvgetcrtlinepenno();
	m_pcSWakVect->SetOutLinePen(pno);
	thick = senvgetcrtlinethick();
	m_pcSWakVect->SetOutLineWidth(thick);

	m_pcSWakVect->SetFlatPar(m_dFlatPar);
	m_pcSWakVect->SetSpaceKind(m_nSpaceKind);
	m_pcSWakVect->SetSpacePar(m_dSpacePar);
	m_pcSWakVect->SetKanaPar(m_dKanaPar);
	m_pcSWakVect->SetOderKind(m_nOderKind);
	m_pcSWakVect->SetMaxCharCount(m_dTCHARCount);
	m_pcSWakVect->SetTateYoko(m_nTateYoko);
	if(m_nTateYoko == TATEGAKI || m_nTateYoko == TATEOKI){
		m_pcSWakVect->SetFontSize(m_dWidth, m_dWidth);
		m_pcSWakVect->SetStrLen(m_dHight);
	}else{
		m_pcSWakVect->SetFontSize(m_dHight, m_dHight);
		m_pcSWakVect->SetStrLen(m_dWidth);
	}
	m_pcSWakVect->SetStrLen(0.0);
	m_pcSWakVect->m_nYokoGumi = m_nYokoGumi;
	m_pcSWakVect->m_nFixFlag = m_nFixFlag;
	m_pcSWakVect->m_nMirror = m_nMirror;	

	SetFitingVectElement(pcVect);
	pcVect->GetDXY(0, &sx, &sy);
	m_pcSWakVect->SetStrStartXY(sx, sy);
	m_pcSWakVect->SetFontName(m_szFont);
	mojienvsetfontname(m_szFont);
	m_pcSWakVect->GetFildName(szFildName);
	if(m_szFildName[0] == _T('\0')){
		m_pcSWakVect->SetFildName(m_szFildName);
		if(m_pcSWakVect->m_nFixFlag == ON){
			m_pcSWakVect->GetString(m_szStrg);
		}
	}else if(_tcscmp(szFildName, m_szFildName) == 0){
		m_pcDoc->m_pcDialogDBEdit->GetFildElement(m_szFildName, m_szStrg);
	}else{
		m_pcSWakVect->SetFildName(m_szFildName);
		m_pcDoc->m_pcDialogDBEdit->GetFildElement(m_szFildName, m_szStrg);
	}
	m_pcSWakVect->m_nShadowKind = m_nShadowKind;
	if(m_nShadowKind != 0){
		m_pcSWakVect->m_clrShadow = m_clrShadow;
		m_pcSWakVect->m_dShadowDistance = m_dShadowDistance;
		m_pcSWakVect->m_dShadowRoundRag = m_dShadowRoundRag;
	}
	m_szStrg[RECMAX] = _T('\0');
	m_pcSWakVect->SetString(m_szStrg);
	m_pcSWakVect->RenewMiniMax();

	if(m_szStrg[0] != _T('\0')){
		m_pcSWakVect->StrgLineStruct();
		m_pcSWakVect->RenewMiniMax();
	}
}

void CCmdSWak::CreateSWakVect()
{
	CTonDoc*	pcDoc;
	CVect*		pcVect;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcSWakVect = (CSWakVect*)(pcDoc->NewData(SWAKVECTID));
	pcVect = (CVect*)(pcDoc->NewData(BASEVECTID));
	if((m_dEX - m_dSX) < (m_dEY - m_dSY)){
		m_nTateYoko = TATEGAKI;
	}
	else{
		m_nTateYoko = YOKOGAKI;
	}
	m_pcSWakVect->SetFitingVect(pcVect);
	m_pcSWakVect->SetFillPen(0);

	SetDataElement();

	if(m_pcSWakVect->m_nFixFlag == ON){
		m_pcSWakVect->SetString(m_szStrg);
		m_szStrg[RECMAX] = _T('\0');
		m_pcSWakVect->RenewMiniMax();
		if(m_szStrg[0] != _T('\0')){
			m_pcSWakVect->StrgLineStruct();
			m_pcSWakVect->RenewMiniMax();
		}
	}
	AddLinkDoc(m_pcSWakVect);
}

void CCmdSWak::SetFitingVectElement(CVect *pcVect)
{
	DBL		x1, y1, x2, y2;

	if(m_nTateYoko == YOKOGAKI || m_nTateYoko == YOKOOKI){
		x1 = m_dSX;
		y1 = (m_dSY + m_dEY) / 2;
		x2 = m_dEX;
		y2 = y1;
	}
	else{
		x1 = (m_dSX + m_dEX) / 2;
		y1 = m_dEY;
		x2 = x1;
		y2 = m_dSY;
	}
	if(m_nMirror == NOMIRROR){
		pcVect->SetAtrDXY(0, 0, x1, y1);
		pcVect->SetAtrDXY(1, 0, x2, y2);
	}
	else{
		pcVect->SetAtrDXY(0, 0, x2, y2);
		pcVect->SetAtrDXY(1, 0, x1, y1);
	}
	pcVect->SetPointCount(2);
	pcVect->OffFlag(REJIONFLAG);
}

void CCmdSWak::SetAnyWakuAtr()
{
	int		nfirstflag;
	TCHAR	szFont2[RECMAX];
	TCHAR	szFildName2[RECMAX];
	DBL		dwd, dhi, par;
	DBL		dFlatPar2;
	int		nSpaceKind2;
	DBL		dSpaceMM2;
	int		nTateGaki2;
	DBL		dKanaPar2;
	int		nOderKind2;
	DBL		dTCHARCount2;
	int		nYokoGumi2;
	int		nFixFlag2;
	int		nMirror2;
	CSysEnv	*pcSysEnv;

	pcSysEnv = m_pcApp->m_pcSysEnv;

	pcSysEnv->m_szStrg[0] = _T('\0');
	_tcscpy_s(pcSysEnv->m_szFont, m_szFont);
	if(m_szFildName[0] == _T('\0')){
		_tcscpy_s(pcSysEnv->m_szFildName, _T("未設定"));
	}else{
		_tcscpy_s(pcSysEnv->m_szFildName, m_szFildName);
	}
	pcSysEnv->m_dFlatPar = m_dFlatPar;
	pcSysEnv->m_nSpaceKind = SPACEWAKU;
	pcSysEnv->m_dSpaceMM = MJEQUALSP;
	pcSysEnv->m_nTateYoko = -1;
	pcSysEnv->m_dKanaPar = m_dKanaPar;
	pcSysEnv->m_nOderKind = m_nOderKind;
	pcSysEnv->m_dTCHARCount = m_dTCHARCount;
	pcSysEnv->m_nYokoGumi = m_nYokoGumi;
	pcSysEnv->m_nFixFlag = m_nFixFlag;
	pcSysEnv->m_nMirror = m_nMirror;
	pcSysEnv->m_dSX = NOSETMM;
	pcSysEnv->m_dSY = NOSETMM;
	pcSysEnv->m_dWidth = NOSETMM;
	pcSysEnv->m_dHight = NOSETMM;

	nfirstflag = ON;
	POSITION pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		m_pcSWakVect = (CSWakVect *)m_cPtrList.GetNext(pos);
		m_pcSWakVect->GetFontSize(&dwd, &dhi);
		m_pcSWakVect->GetSpaceKind(&nSpaceKind2);
		m_pcSWakVect->GetFlatPar(&dFlatPar2);
		m_pcSWakVect->GetKanaPar(&dKanaPar2);
		m_pcSWakVect->GetOderKind(&nOderKind2);
		dTCHARCount2 = m_pcSWakVect->GetMaxCharCount();
		m_pcSWakVect->GetFontName(szFont2);
		m_pcSWakVect->GetFildName(szFildName2);
		m_pcSWakVect->GetSpacePar(&par);
		nTateGaki2 = m_pcSWakVect->CheckTateYoko();
		if(par == MJFREESPACEPAR){
			dSpaceMM2 = MJEQUALSP;
		}
		else{
			if(nTateGaki2 == TATEGAKI){
				 dSpaceMM2 = dwd * par / 100.0;
			}
			else{
				dSpaceMM2 = dhi * par / 100.0;
			}
		}
		nYokoGumi2 = m_pcSWakVect->m_nYokoGumi;
		nFixFlag2 = m_pcSWakVect->m_nFixFlag;
		nMirror2 = m_pcSWakVect->m_nMirror;	
		if(nfirstflag == ON){
			nfirstflag = OFF;
			_tcscpy_s(pcSysEnv->m_szFont, szFont2);
			_tcscpy_s(pcSysEnv->m_szFildName, szFildName2);
			pcSysEnv->m_nTateYoko = -1;
			pcSysEnv->m_nOderKind = nOderKind2;
			pcSysEnv->m_dFlatPar = dFlatPar2;
			pcSysEnv->m_nSpaceKind = nSpaceKind2;
			pcSysEnv->m_dTCHARCount = dTCHARCount2;
			pcSysEnv->m_dSpaceMM = dSpaceMM2;
			pcSysEnv->m_dKanaPar = dKanaPar2;
			pcSysEnv->m_nYokoGumi = nYokoGumi2;
			pcSysEnv->m_nFixFlag = nFixFlag2;
			pcSysEnv->m_nMirror = nMirror2;
		}
		else{
			if(_tcscmp(pcSysEnv->m_szFont, szFont2) != 0){
				pcSysEnv->m_szFont[0] = _T('\0');
			}
			if(_tcscmp(pcSysEnv->m_szFildName, szFildName2) != 0){
				pcSysEnv->m_szFildName[0] = _T('\0');
			}
			if(pcSysEnv->m_nTateYoko != nTateGaki2){
				pcSysEnv->m_nTateYoko = -1;
			}
			if(pcSysEnv->m_nOderKind != nOderKind2){
				pcSysEnv->m_nOderKind = -1;
			}
			if(pcSysEnv->m_dFlatPar != dFlatPar2){
				pcSysEnv->m_dFlatPar = NOSETPAR;
			}
			if(pcSysEnv->m_nSpaceKind != nSpaceKind2){
				pcSysEnv->m_nSpaceKind = -1;
			}
			if(pcSysEnv->m_dSpaceMM != dSpaceMM2){
				pcSysEnv->m_dSpaceMM = NOSETMM;
			}
			if(pcSysEnv->m_dTCHARCount != dTCHARCount2){
				pcSysEnv->m_dTCHARCount = NOSETMM;
			}
			if(pcSysEnv->m_dKanaPar != dKanaPar2){
				pcSysEnv->m_dKanaPar = NOSETPAR;
			}
			if(pcSysEnv->m_nYokoGumi != nYokoGumi2){
				pcSysEnv->m_nYokoGumi = -1;
			}
			if(pcSysEnv->m_nFixFlag != nFixFlag2){
				pcSysEnv->m_nFixFlag = -1;
			}
			if(pcSysEnv->m_nMirror != nMirror2){
				pcSysEnv->m_nMirror = -1;
			}
		}
	}
}

void CCmdSWak::ResetAnyWakuAtr()
{
	int		nTateGaki;
	DBL		dwd, dhi, par;
	int		nchange;
	TCHAR	szStr[RECMAX];
	CSysEnv	*pcSysEnv;

	pcSysEnv = m_pcApp->m_pcSysEnv;
	POSITION pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		nchange = FALSE;
		m_pcSWakVect = (CSWakVect *)m_cPtrList.GetNext(pos);
		nTateGaki = m_pcSWakVect->CheckTateYoko();
		m_pcSWakVect->GetFontSize(&dwd, &dhi);
		if(pcSysEnv->m_szFont[0] != _T('\0')){
			m_pcSWakVect->SetFontName(pcSysEnv->m_szFont);
			mojienvsetfontname(pcSysEnv->m_szFont);
			nchange = TRUE;
		}
		/*
		if(pcSysEnv->m_szFildName[0] != _T('\0')){
			m_pcSWakVect->SetFontName(pcSysEnv->m_szFildName);
		}
		*/
		if(pcSysEnv->m_nOderKind != -1){
			m_pcSWakVect->SetOderKind(pcSysEnv->m_nOderKind);
			nchange = TRUE;
		}
		if(pcSysEnv->m_dFlatPar != NOSETPAR){
			m_pcSWakVect->SetFlatPar(pcSysEnv->m_dFlatPar);
			nchange = TRUE;
		}
		if(pcSysEnv->m_nSpaceKind != -1){
			m_pcSWakVect->SetSpaceKind(pcSysEnv->m_nSpaceKind);
			nchange = TRUE;
		}
		if(pcSysEnv->m_dSpaceMM != NOSETMM){
			if(pcSysEnv->m_dSpaceMM == MJEQUALSP){
				par = MJFREESPACEPAR;
			}
			else{
				if(nTateGaki == TATEGAKI){
					par = pcSysEnv->m_dSpaceMM / dwd * 100.0;
				}
				else{
					par = pcSysEnv->m_dSpaceMM / dhi * 100.0;
				}
			}
			m_pcSWakVect->SetSpacePar(par);
			nchange = TRUE;
		}
		if(pcSysEnv->m_dTCHARCount != NOSETMM){
			m_pcSWakVect->SetMaxCharCount(pcSysEnv->m_dTCHARCount);
			nchange = TRUE;
		}
		if(pcSysEnv->m_dKanaPar != NOSETPAR){
			m_pcSWakVect->SetKanaPar(pcSysEnv->m_dKanaPar);
			nchange = TRUE;
		}
		if(pcSysEnv->m_nYokoGumi != -1){
			m_pcSWakVect->m_nYokoGumi = pcSysEnv->m_nYokoGumi;
			nchange = TRUE;
		}
		if(pcSysEnv->m_nFixFlag != -1){
			m_pcSWakVect->m_nFixFlag = pcSysEnv->m_nFixFlag;
			nchange = TRUE;
		}
		if(pcSysEnv->m_nMirror != -1){
			if(m_pcSWakVect->m_nMirror != pcSysEnv->m_nMirror){
				m_pcSWakVect->m_pcFitingVect->ReverceRejion();
				m_pcSWakVect->m_nMirror = pcSysEnv->m_nMirror;
			}
			nchange = TRUE;
		}

		if(nchange == TRUE){
 			m_pcSWakVect->GetString(szStr);
			m_pcSWakVect->SetString(szStr);
			m_pcSWakVect->RenewMiniMax();
			if(szStr[0] != _T('\0')){
				m_pcSWakVect->StrgLineStruct();
				m_pcSWakVect->RenewMiniMax();
			}
		}
	}
}

void CCmdSWak::GetFitingVectElement(int ntateyoko, DBL wd, DBL hi, CVect *pcVect)
{
	DBL		sx, sy, ex, ey;

	if(m_nMirror == NOMIRROR){
		pcVect->GetDXY(0, &sx, &sy);
		pcVect->GetDXY(1, &ex, &ey);
	}
	else{
		pcVect->GetDXY(1, &sx, &sy);
		pcVect->GetDXY(0, &ex, &ey);
	}
	if(ntateyoko == YOKOGAKI || ntateyoko == TATEOKI){
		m_dSX = sx;
		m_dSY = sy - hi / 2.0;
		m_dEX = ex;
		m_dEY = sy + hi / 2.0;
	}
	else{
		m_dSX = sx - wd / 2.0;
		m_dSY = ey;
		m_dEX = sx + wd / 2.0;
		m_dEY = sy;
	}
}

void CCmdSWak::SetSWakLocate()
{
	DBL		sx, sy;
	DBL		ex, ey;

	if(m_dSX < m_dEX){
		sx = m_dSX;
		ex = m_dEX;
	}
	else{
		sx = m_dEX;
		ex = m_dSX;
	}

	if(m_dSY < m_dEY){
		sy = m_dSY;
		ey = m_dEY;
	}
	else{
		sy = m_dEY;
		ey = m_dSY;
	}
	CSysEnv	*pcSysEnv;

	pcSysEnv = m_pcApp->m_pcSysEnv;
	pcSysEnv->m_dWidth = FLOOR10(ex - sx);
	pcSysEnv->m_dHight = FLOOR10(ey - sy);
	pcSysEnv->m_dSX = FLOOR10(sx);
	pcSysEnv->m_dSY = FLOOR10(sy);

	m_pcWnd->SetSWakLocate();
}

void CCmdSWak::UpdateStringList(LPTSTR str)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	szInFile[FNAMEMAX];
	TCHAR	szOutFile[FNAMEMAX];
	FILE	*ifp, *ofp;
	TCHAR	buf[RECMAX];
	int		cnt;

	senvgetexepath(exepath);
	_stprintf_s(szInFile, _T("%s\\SlctStrg1.bak"), exepath);
	_stprintf_s(szOutFile, _T("%s\\SlctStrg1.txt"), exepath);
	_tfopen_s(&ofp, szInFile, _T("w"));
	if(ofp == NULL){
		return;
	}
	_tfopen_s(&ifp, szOutFile, _T("r"));
	if(ifp == NULL){
		return;
	}
	cnt = 0;
	while(1){
		if(cnt > 100){
			break;
		}
		if(baselibCrDelFgets(buf, RECMAX, ifp) == NULL){
			break;
		}
		if(_tcscmp(buf, str) != 0){
			if(_tcslen(buf) != 0){
				_stprintf_s(buf, _T("%s\n"), buf);
				baselibFputs(buf, ofp);
				cnt++;
			}
		}
	}	
	fclose(ifp);
	fclose(ofp);
	
	_tfopen_s(&ofp, szOutFile, _T("w"));
	if(ofp == NULL){
		return;
	}
	_tfopen_s(&ifp, szInFile, _T("r"));
	if(ifp == NULL){
		return;
	}
	_stprintf_s(buf, _T("%s\n"), str);
	baselibFputs(buf, ofp);
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, ifp) == NULL){
			break;
		}
		baselibFputs(buf, ofp);
	}
	fclose(ifp);
	fclose(ofp);
}
