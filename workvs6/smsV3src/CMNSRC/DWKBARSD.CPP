// DWkBarSD.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DWkBarSD.h"
#include "DlgWkPpr.h"
#include "DSetSp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBarSide ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
	void scrngetmmsize(LPDBL bxmm, LPDBL bymm);
	void scrnsetmmsize(DBL xsize, DBL ysize);
	void poutcntrkindset(int kind);
	void poutcntrkindget(int *kind);
	void poutploternameget(LPSTR plotername);
	void poutploternameset(LPSTR plotername);
	void pouttypeset(int type);
	void poutmmsizeset(DBL xsize, DBL ysize);
	void poutstepset(DBL step);
	void poutmmstartset(DBL sx, DBL sy);
	void poutploternameset(LPSTR pltname);
	void poutarcflagset(int flag);
	void poutfillflagset(int flag);
}

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
	char	papername[RECMAX];
	DBL		xmm, ymm;

	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBPAPERNAME);
	idx = pcCB->GetCurSel();
	pcCB->GetLBText(idx, papername);
	GetPaperSize(papername, &xmm, &ymm);
	scrnsetmmsize(xmm, ymm);
	m_csPaperName = _T(papername);
	m_csWorkXSize.Format("%1.1lf", xmm);
	m_csWorkYSize.Format("%1.1lf", ymm);
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
	char	szPloterName[RECMAX];
	int		cntrkind;
	DBL		xmm, ymm;
	char	szPaperName[RECMAX];

	CSysEnv *pcSysEnv = m_pcApp->m_pcSysEnv;
	poutploternameget(szPloterName);
	m_csPloter = _T(szPloterName);
	poutcntrkindget(&cntrkind);
	CenterKindBottonSet(cntrkind);
	scrngetmmsize(&xmm, &ymm);
	GetPaperName(xmm, ymm, szPaperName);
	m_csPaperName = _T(szPaperName);
	m_csWorkXSize.Format("%1.1lf", xmm);
	m_csWorkYSize.Format("%1.1lf", ymm);
	UpdateData(FALSE);
}

void CDialogWakuBarSide::SetCrtView(CTonView * pcView)
{
	m_pcView = pcView;
}

void CDialogWakuBarSide::InitCmbPloter()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	plotername[QRECMAX];

	senvgetexepath(exepath);
	sprintf(fname, "%s\\plotname.env", exepath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBPLOTER);
	pcCB->ResetContent();
	while(1){
		if(CrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%s", plotername);
		pcCB->AddString(plotername);
	}
	fclose(fp);
	poutploternameget(plotername);
	m_csPloter = _T(plotername);
}

void CDialogWakuBarSide::InitCmbPaperName()
{
	int		idx;
	char	exepath[RECMAX];
	char	fname[RECMAX];
	FILE	*fp;
	char	papername[QRECMAX];
	char	buf[RECMAX];

	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBPAPERNAME);
	pcCB->ResetContent();
	senvgetexepath(exepath);
	sprintf(fname, "%s\\papersiz.env", exepath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	idx = 0;
	while(1){
		if(CrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%s", papername);
		if(papername[0] == '\0'){
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
	char	papername[RECMAX];
	DBL		xmm, ymm;

	pcDialogWakuPaper = new CDialogWakuPaper(m_pcView);
	pcDialogWakuPaper->m_csPaperName = m_csPaperName;
	ret = pcDialogWakuPaper->DoModal();
	if(ret == IDOK){
		InitCmbPaperName();
		m_csPaperName = pcDialogWakuPaper->m_csPaperName;
		strcpy(papername, m_csPaperName.GetBuffer(RECMAX));
		GetPaperSize(papername, &xmm, &ymm);
		m_csWorkXSize.Format("%1.1lf", xmm);
		m_csWorkYSize.Format("%1.1lf", ymm);
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

LPSTR CDialogWakuBarSide::CrDelFgets(char * buf, int max, FILE * fp)
{
	char	*ret;
	int		len;

	ret = fgets(buf, max, fp);
	if(ret != NULL){
		len = strlen(buf);
		if(buf[len-1] == 0x0a){
			if(buf[len-2] == 0x0d){
				if(buf[len-3] == 0x0d){
					buf[len-3] = '\0';
				}
				else{
					buf[len-2] = '\0';
				}
			}
			else{
				buf[len-1] = '\0';
			}
		}
		else if(buf[len-1] == 0x0d){
			buf[len-1] = '\0';
		}
	}
	return(ret);
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

void CDialogWakuBarSide::GetPaperName(DBL srcxmm, DBL srcymm, LPSTR retpapername)
{
	char	exepath[RECMAX];
	char	fname[RECMAX];
	FILE	*fp;
	char	papername[QRECMAX];
	char	buf[RECMAX];
	DBL		xmm, ymm;
	DBL		sx, sy;
	DBL		width, hight;

	senvgetexepath(exepath);
	strcpy(retpapername, "USERSIZE");
	sprintf(fname, "%s\\papersiz.env", exepath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	while(1){
		if(CrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%s %lf %lf %lf %lf %lf %lf"
			, papername, &xmm, &ymm, &sx, &sy, &width, &hight);
		if(srcxmm == xmm && srcymm == ymm){
			strcpy(retpapername, papername); 
			break;
		}
	}
	fclose(fp);
}

void CDialogWakuBarSide::GetPaperSize(LPSTR srchname, DBL *retxmm, DBL *retymm)
{
	char	exepath[RECMAX];
	char	fname[RECMAX];
	FILE	*fp;
	char	papername[QRECMAX];
	char	buf[RECMAX];
	DBL		xmm, ymm;
	DBL		sx, sy;
	DBL		width, hight;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\papersiz.env", exepath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	while(1){
		if(CrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%s", papername);
		if(strcmp(papername, srchname) == 0){
			sscanf(buf, "%s %lf %lf %lf %lf %lf %lf"
				, papername, &xmm, &ymm, &sx, &sy, &width, &hight);
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
			xmm = atof(m_csWorkXSize);
			ymm = atof(m_csWorkYSize);
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
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	plotername[QRECMAX];
	int		setflag;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\plotname.env", exepath);
	setflag = OFF;
	if((fp = fopen(fname, "r")) != NULL){
		while(1){
			if(CrDelFgets(buf, RECMAX, fp) == NULL){
				break;
			}
			sscanf(buf, "%s %s %d %lf %lf %lf %lf %lf %lf %lf %d %d",
					plotername, m_stPlotEnv.cmdtstr, &(m_stPlotEnv.cmdtype),
					&(m_stPlotEnv.sizex), &(m_stPlotEnv.sizey), &(m_stPlotEnv.step),
					&(m_stPlotEnv.sx), &(m_stPlotEnv.sy),
					&(m_stPlotEnv.xtime), &(m_stPlotEnv.ytime),
					&(m_stPlotEnv.arcflg), &(m_stPlotEnv.outflg));
			if(m_csPloter == _T(plotername)){
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
