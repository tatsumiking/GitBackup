// DWkBarSD.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogWakuBarSide.h"

#include "DialogWakuPaper.h"
#include "DialogSetSpace.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Pout.h"
//}
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBarSide ダイアログ

CDialogWakuBarSide::CDialogWakuBarSide(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	m_pcApp = (CTonApp*)::AfxGetApp();
	m_pcWnd = NULL;
	m_pcView = NULL;
	m_nFirstFlag = ON;
	//{{AFX_DATA_INIT(CDialogWakuBarSide)
	m_csPaperName = _T("");
	m_csPloter = _T("");
	m_csWorkXSize = _T("");
	m_csWorkYSize = _T("");
	//}}AFX_DATA_INIT
}


void CDialogWakuBarSide::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuBarSide)
	DDX_CBString(pDX, IDC_CMBPAPERNAME, m_csPaperName);
	DDX_CBString(pDX, IDC_CMBPLOTER, m_csPloter);
	DDX_Text(pDX, IDC_EDWORKXSIZE, m_csWorkXSize);
	DDX_Text(pDX, IDC_EDWORKYSIZE, m_csWorkYSize);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogWakuBarSide, CDialogBar)
	//{{AFX_MSG_MAP(CDialogWakuBarSide)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_CMBPAPERNAME, OnSelchangeCmbpapername)
	ON_CBN_SELCHANGE(IDC_CMBPLOTER, OnSelchangeCmbploter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBarSide メッセージ ハンドラ

void CDialogWakuBarSide::OnPaint() 
{
	int		cntrkind;

	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	if(m_nFirstFlag == ON){
		m_nFirstFlag = OFF;
		m_pcWnd = (CMainFrame*)::AfxGetMainWnd();
		m_btnCntrKind.SubclassDlgItem(ID_FILE_CENTERKIND, this);
		InitItemElement();
	}
	poutcntrkindget(&cntrkind);
	CenterKindBottonSet(cntrkind);
}

void CDialogWakuBarSide::OnSelchangeCmbploter() 
{
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBPLOTER);
	int idx = pcCB->GetCurSel();
	pcCB->GetLBText(idx, m_csPloter);
	ResetPlotElement();
}

void CDialogWakuBarSide::OnSelchangeCmbpapername() 
{
	int		idx;
	TCHAR	papername[RECMAX];
	DBL		xmm, ymm;

	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBPAPERNAME);
	idx = pcCB->GetCurSel();
	pcCB->GetLBText(idx, papername);
	GetPaperSize(papername, &xmm, &ymm);
	scrnsetmmsize(xmm, ymm);
	m_csPaperName = papername;
	m_csWorkXSize.Format(_T("%1.1lf"), xmm);
	m_csWorkYSize.Format(_T("%1.1lf"), ymm);
	UpdateData(FALSE);
	((CTonView*)m_pcView)->WorkSizeReset();
	((CTonView*)m_pcView)->WorkAreaReset();
	m_pcView->Invalidate(TRUE);
	Invalidate(TRUE);
}

void CDialogWakuBarSide::InitItemElement()
{
	InitCmbPloter();
	InitCmbPaperName();
	UpdateData(FALSE);
}

void CDialogWakuBarSide::GetItemElement()
{
	UpdateData(TRUE);
	CSysEnv *pcSysEnv = m_pcApp->m_pcSysEnv;
	poutploternameset(m_csPloter.GetBuffer(RECMAX));
}

void CDialogWakuBarSide::SetItemElement()
{
	TCHAR	szPloterName[RECMAX];
	int		cntrkind;
	DBL		xmm, ymm;
	TCHAR	szPaperName[RECMAX];

	CSysEnv *pcSysEnv = m_pcApp->m_pcSysEnv;
	poutploternameget(szPloterName);
	m_csPloter = szPloterName;
	poutcntrkindget(&cntrkind);
	CenterKindBottonSet(cntrkind);
	scrngetmmsize(&xmm, &ymm);
	GetPaperName(xmm, ymm, szPaperName);
	m_csPaperName = szPaperName;
	m_csWorkXSize.Format(_T("%1.1lf"), xmm);
	m_csWorkYSize.Format(_T("%1.1lf"), ymm);
	UpdateData(FALSE);
}

void CDialogWakuBarSide::SetCrtView(CTonView * pcView)
{
	m_pcView = pcView;
}

void CDialogWakuBarSide::InitCmbPloter()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	plotername[QRECMAX];

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\plotname.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		return;
	}
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBPLOTER);
	pcCB->ResetContent();
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		_stscanf_s(buf, _T("%s"), plotername, QRECMAX);
		pcCB->AddString(plotername);
	}
	fclose(fp);
	poutploternameget(plotername);
	m_csPloter = plotername;
}

void CDialogWakuBarSide::InitCmbPaperName()
{
	int		idx;
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	papername[QRECMAX];
	TCHAR	buf[RECMAX];

	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBPAPERNAME);
	pcCB->ResetContent();
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\papersiz.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		return;
	}
	idx = 0;
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		_stscanf_s(buf, _T("%s"), papername, QRECMAX);
		if(papername[0] == _T('\0')){
			break;
		}
		pcCB->AddString(papername);
		idx++;
	}
	fclose(fp);
}

void CDialogWakuBarSide::SetPrint() 
{
	CTonApp*	pcApp;

	m_csPloter = _T("プリンタ");
	pcApp = (CTonApp*)AfxGetApp();
	pcApp->OnFilePrintSetup();
	UpdateData(FALSE);
}

void CDialogWakuBarSide::SetPaperSave() 
{
	CDialogWakuPaper*	pcDialogWakuPaper;
	int		ret;
	TCHAR	papername[RECMAX];
	DBL		xmm, ymm;

	pcDialogWakuPaper = new CDialogWakuPaper(m_pcView);
	pcDialogWakuPaper->m_csPaperName = m_csPaperName;
	ret = pcDialogWakuPaper->DoModal();
	if(ret == IDOK){
		InitCmbPaperName();
		m_csPaperName = pcDialogWakuPaper->m_csPaperName;
		_tcscpy_s(papername, m_csPaperName.GetBuffer(RECMAX));
		GetPaperSize(papername, &xmm, &ymm);
		m_csWorkXSize.Format(_T("%1.1lf"), xmm);
		m_csWorkYSize.Format(_T("%1.1lf"), ymm);
		scrnsetmmsize(xmm, ymm);
		UpdateData(FALSE);
		((CTonView*)m_pcView)->WorkSizeReset();
		((CTonView*)m_pcView)->WorkAreaReset();
		m_pcView->Invalidate(TRUE);
	}
	delete(pcDialogWakuPaper);
	UpdateData(FALSE);
}

void CDialogWakuBarSide::SetPageSpace() 
{
	CDialogSetSpace	*pcDialogSetSpace;

	pcDialogSetSpace = new CDialogSetSpace(this);
	pcDialogSetSpace->SetCrtView(m_pcView);
	pcDialogSetSpace->DoModal();
	delete(pcDialogSetSpace);
}
void CDialogWakuBarSide::CenterkindChange()
{
	int		cntrkind;

	poutcntrkindget(&cntrkind);
	switch(cntrkind){
	case LEFTDOWN:
		cntrkind = LEFTUP;
		break;
	case LEFTUP:
		cntrkind = RIGHTUP;
		break;
	case RIGHTUP:
		cntrkind = RIGHTDOWN;
		break;
	case RIGHTDOWN:
		cntrkind = LEFTDOWN;
		break;
	}
	poutcntrkindset(cntrkind);
	Invalidate(TRUE);
}

void CDialogWakuBarSide::CenterKindBottonSet(int cntrkind)
{
	switch(cntrkind){
	case LEFTDOWN:
		m_btnCntrKind.LoadBitmaps(IDB_OLEFTDOWN, NULL, NULL);
		break;
	case LEFTUP:
		m_btnCntrKind.LoadBitmaps(IDB_OLEFTUP, NULL, NULL);
		break;
	case RIGHTUP:
		m_btnCntrKind.LoadBitmaps(IDB_ORIGHTUP, NULL, NULL);
		break;
	case RIGHTDOWN:
		m_btnCntrKind.LoadBitmaps(IDB_ORIGHTDOWN, NULL, NULL);
		break;
	}
}

void CDialogWakuBarSide::GetPaperName(DBL srcxmm, DBL srcymm, LPTSTR retpapername)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	papername[QRECMAX];
	TCHAR	buf[RECMAX];
	DBL		xmm, ymm;
	DBL		sx, sy;
	DBL		width, hight;
	int		len;

	senvgetexepath(exepath);
	len = _tcslen(_T("USERSIZE")) + 1;
	_tcscpy_s(retpapername, len, _T("USERSIZE"));
	_stprintf_s(fname, _T("%s\\papersiz.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		_stscanf_s(buf, _T("%s %lf %lf %lf %lf %lf %lf")
			, papername, QRECMAX, &xmm, &ymm, &sx, &sy, &width, &hight);
		if(srcxmm == xmm && srcymm == ymm){
			len = _tcslen(papername) + 1;
			_tcscpy_s(retpapername, len, papername);
			break;
		}
	}
	fclose(fp);
}

void CDialogWakuBarSide::GetPaperSize(LPTSTR srchname, DBL *retxmm, DBL *retymm)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	papername[QRECMAX];
	TCHAR	buf[RECMAX];
	DBL		xmm, ymm;
	DBL		sx, sy;
	DBL		width, hight;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\papersiz.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		_stscanf_s(buf, _T("%s"), papername, QRECMAX);
		if(_tcscmp(papername, srchname) == 0){
			_stscanf_s(buf, _T("%s %lf %lf %lf %lf %lf %lf")
				, papername, QRECMAX, &xmm, &ymm, &sx, &sy, &width, &hight);
			*retxmm = xmm;
			*retymm = ymm;
			break;
		}
	}
	fclose(fp);
}

BOOL CDialogWakuBarSide::PreTranslateMessage(MSG* pMsg) 
{
	DBL		xmm, ymm;

	if(pMsg->message == WM_KEYDOWN){
		if(pMsg->wParam == VK_RETURN){
			UpdateData(TRUE);
			m_csPaperName = _T("USERSIZE");
			xmm = _ttof(m_csWorkXSize);
			ymm = _ttof(m_csWorkYSize);
			scrnsetmmsize(xmm, ymm);
			((CTonView*)m_pcView)->WorkSizeReset();
			((CTonView*)m_pcView)->WorkAreaReset();
			m_pcView->Invalidate(TRUE);
			UpdateData(FALSE);
		}
	}
	return CDialogBar::PreTranslateMessage(pMsg);
}

void CDialogWakuBarSide::ResetPlotElement()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	plotername[QRECMAX];
	int		setflag;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\plotname.env"), exepath);
	setflag = OFF;
	_tfopen_s(&fp, fname, _T("r"));
	if(fp != NULL){
		while(1){
			if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
				break;
			}
			_stscanf_s(buf, _T("%s %s %d %lf %lf %lf %lf %lf %lf %lf %d %d"),
					plotername, QRECMAX, m_stPlotEnv.cmdtstr, QRECMAX, &(m_stPlotEnv.cmdtype),
					&(m_stPlotEnv.sizex), &(m_stPlotEnv.sizey), &(m_stPlotEnv.step),
					&(m_stPlotEnv.sx), &(m_stPlotEnv.sy),
					&(m_stPlotEnv.xtime), &(m_stPlotEnv.ytime),
					&(m_stPlotEnv.arcflg), &(m_stPlotEnv.outflg));
			if(m_csPloter == plotername){
				setflag = ON;
				break;
			}
		}
		fclose(fp);
	}
	if(setflag == ON){
		pouttypeset(m_stPlotEnv.cmdtype);
		poutmmsizeset(m_stPlotEnv.sizex, m_stPlotEnv.sizey);
		poutstepset(m_stPlotEnv.step);
		poutmmstartset(m_stPlotEnv.sx, m_stPlotEnv.sy);
		poutploternameset(m_csPloter.GetBuffer(RECMAX));
		poutarcflagset(m_stPlotEnv.arcflg);
		poutfillflagset(m_stPlotEnv.outflg);
	}
}
