// DlgScrnE.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgScrnE.h"
#include "DlgDirSl.h"
#include	<winspool.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogScrnEnv ダイアログ

extern "C" {
	void mojienvgetfontpath(LPSTR fontpath);
	void mojienvsetfontpath(LPSTR fontpath);
	void senvgetexepath(LPSTR path);
	void senvgetwakpath(LPSTR path);
	void senvsetwakpath(LPSTR path);
	void senvgetkmnpath(LPSTR path);
	void senvsetkmnpath(LPSTR path);
	void senvgetdatpath(LPSTR path);
	void senvsetdatpath(LPSTR path);
	void senvgetoutpath(LPSTR path);
	void senvsetoutpath(LPSTR path);
	void senvgetpartpath(LPSTR path);
	void senvsetpartpath(LPSTR path);
	void senvgettmppath(LPSTR path);
	void senvsettmppath(LPSTR path);
	void senvgetbtppath(LPSTR path);
	void senvsetbtppath(LPSTR path);
	void senvsetareadotsize(int dotsize);
	void senvgetareadotsize(int *dotsize);
	void senvsetpointdotsize(int dotsize);
	void senvgetpointdotsize(int *dotsize);
	void scrngetmmsize(LPDBL pdmmxsize, LPDBL pdmmysize);
	void scrnsetmmsize(DBL dmmxsize, DBL dmmysize);
	void poutmmsizeset(DBL xsize, DBL ysize);
	void poutmmsizeget(DBL *xsize, DBL *ysize);
	void pouttypeset(int type);
	void pouttypeget(LPINT type);
	void poutstepset(DBL step);
	void poutstepget(DBL *step);
	void poutmmstartset(DBL sx, DBL sy);
	void poutmmstartget(DBL *sx, DBL *sy);
	void poutcntrkindset(int kind);
	void poutcntrkindget(int *kind);
	void poutportnameset(LPSTR port);
	void poutportnameget(LPSTR port);
	void poutploternameset(LPSTR pltname);
	void poutploternameget(LPSTR pltname);
	void poutarcflagset(int flag);
	void poutarcflagget(int *flag);
	void poutfillflagset(int flag);
	void poutfillflagget(int *flag);
	void poutgetdeepcut(LPDBL mm);
	void poutsetdeepcut(DBL mm);
}

int CALLBACK CDialogScrnEnv::BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if((BFFM_INITIALIZED == uMsg) && lpData){
		::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
	}
	return 0;
}

CDialogScrnEnv::CDialogScrnEnv(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogScrnEnv::IDD, pParent)
{
	m_pcParent = pParent;
	//{{AFX_DATA_INIT(CDialogScrnEnv)
	m_csKmonPath = _T("");
	m_csCntrKind = _T("");
	m_csDataPath = _T("");
	m_csFontPath = _T("");
	m_csPlotName = _T("");
	m_dPlotStep = 0;
	m_dPlotSx = 0;
	m_dPlotSy = 0;
	m_dPlotXSize = 0;
	m_dPlotYSize = 0;
	m_csOutDPath = _T("");
	m_csPortName = _T("");
	m_csTBMPPath = _T("");
	m_csTempPath = _T("");
	m_csWakuPath = _T("");
	m_dWorkXSize = 0;
	m_dWorkYSize = 0;
	m_csDeepCut = _T("");
	m_csAreaDot = _T("");
	m_csPointDot = _T("");
	m_csPartPath = _T("");
	//}}AFX_DATA_INIT
}


void CDialogScrnEnv::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogScrnEnv)
	DDX_Control(pDX, IDC_PAPERSIZE, m_cbPaperSize);
	DDX_Text(pDX, IDC_KMONPATH, m_csKmonPath);
	DDX_CBString(pDX, IDC_CNTRKIND, m_csCntrKind);
	DDX_Text(pDX, IDC_DATAPATH, m_csDataPath);
	DDX_Text(pDX, IDC_FONTPATH, m_csFontPath);
	DDX_CBString(pDX, IDC_PLOTNAME, m_csPlotName);
	DDX_Text(pDX, IDC_PLOTSTEP, m_dPlotStep);
	DDX_Text(pDX, IDC_PLOTSX, m_dPlotSx);
	DDX_Text(pDX, IDC_PLOTSY, m_dPlotSy);
	DDX_Text(pDX, IDC_PLOTXSIZE, m_dPlotXSize);
	DDX_Text(pDX, IDC_PLOTYSIZE, m_dPlotYSize);
	DDX_Text(pDX, IDC_OUTDPATH, m_csOutDPath);
	DDX_CBString(pDX, IDC_PORTNAME, m_csPortName);
	DDX_Text(pDX, IDC_TBMPPATH, m_csTBMPPath);
	DDX_Text(pDX, IDC_TEMPPATH, m_csTempPath);
	DDX_Text(pDX, IDC_WAKUPATH, m_csWakuPath);
	DDX_Text(pDX, IDC_WORKXSIZE, m_dWorkXSize);
	DDX_Text(pDX, IDC_WORKYSIZE, m_dWorkYSize);
	DDX_Text(pDX, IDC_EDDEEPCUT, m_csDeepCut);
	DDX_Text(pDX, IDC_EDAREADOT, m_csAreaDot);
	DDX_Text(pDX, IDC_EDPOINTDOT, m_csPointDot);
	DDX_Text(pDX, IDC_PARTPATH, m_csPartPath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogScrnEnv, CDialog)
	//{{AFX_MSG_MAP(CDialogScrnEnv)
	ON_CBN_SELCHANGE(IDC_PLOTNAME, OnSelchangePlotname)
	ON_BN_CLICKED(IDC_BTN_BTMP, OnBtnBtmp)
	ON_BN_CLICKED(IDC_BTN_DATA, OnBtnData)
	ON_BN_CLICKED(IDC_BTN_FONT, OnBtnFont)
	ON_BN_CLICKED(IDC_BTN_KMN, OnBtnKmn)
	ON_BN_CLICKED(IDC_BTN_OUT, OnBtnOut)
	ON_BN_CLICKED(IDC_BTN_TEMP, OnBtnTemp)
	ON_BN_CLICKED(IDC_BTN_WAKU, OnBtnWaku)
	ON_BN_CLICKED(IDC_PLOTSIZE, OnPlotsize)
	ON_CBN_SELCHANGE(IDC_PAPERSIZE, OnSelchangePapersize)
	ON_BN_CLICKED(IDC_BTN_PART, OnBtnPart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogScrnEnv メッセージ ハンドラ

int CDialogScrnEnv::DoModal() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	return CDialog::DoModal();
}

void CDialogScrnEnv::CBInitPaperSize()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	papername[NRECMAX];
	
	m_cbPaperSize.AddString("USERSIZE");
	senvgetexepath(exepath);
	sprintf(fname, "%s\\papersiz.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		while(1){
			if(CrDelFgets(buf, RECMAX, fp) == NULL){
				break;
			}
			sscanf(buf, "%s", papername);
			m_cbPaperSize.AddString(papername);
		}
		fclose(fp);
	}
	m_cbPaperSize.SetCurSel(0);
}

void CDialogScrnEnv::OnSelchangePapersize() 
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	int		slctidx, idx;
	char	buf[RECMAX];
	char	papername[NRECMAX];
	DBL		xsize, ysize;
	
	slctidx = m_cbPaperSize.GetCurSel();
	senvgetexepath(exepath);
	sprintf(fname, "%s\\papersiz.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		idx = 1;
		while(1){
			if(CrDelFgets(buf, RECMAX, fp) == NULL){
				break;
			}
			if(slctidx == idx){	
				sscanf(buf, "%s %lf %lf", papername, &xsize, &ysize);
				m_dWorkXSize = xsize;
				m_dWorkYSize = ysize;
				UpdateData(FALSE);
				break;
			}
			idx++;
		}
		fclose(fp);
	}
	
}

void CDialogScrnEnv::CBInitCntrKind()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	szcntrkind[NRECMAX];

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_CNTRKIND);
	senvgetexepath(exepath);
	sprintf(fname, "%s\\cntrkind.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		while(1){
			if(CrDelFgets(buf, RECMAX, fp) == NULL){
				break;
			}
			sscanf(buf, "%s", szcntrkind);
			pCB->AddString(szcntrkind);
		}
		fclose(fp);
	}
	else{
		pCB->AddString("左下原点");
		pCB->AddString("左上原点");
		pCB->AddString("右上原点");
		pCB->AddString("右下原点");
	}
}

void CDialogScrnEnv::CBInitPlotName()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	plotname[QRECMAX];

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_PLOTNAME);
	senvgetexepath(exepath);

	sprintf(fname, "%s\\plotname.env", exepath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	while(1){
		if(CrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%s", plotname);
		pCB->AddString(plotname);
	}
	fclose(fp);
}

void CDialogScrnEnv::CBInitPortName()
{
	char	szPortList[1024+2];
	LPSTR	lpSp;
	int		idx, len;
	HANDLE				hInfo;
	LPPRINTER_INFO_5	lpINFO5 = NULL;		//PRINT_INFO_5構造体
	DWORD				dwNeeded;			//PRINT_INFO_X構造体に必要なｻｲｽﾞ
	DWORD				dwReturned;			//PRINT_INFO_X構造体の個数
	int		i, ret;

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_PORTNAME);
	pCB->ResetContent();
	EnumPrinters(PRINTER_ENUM_LOCAL,NULL,5,NULL,0,&dwNeeded,&dwReturned);
	hInfo = GlobalAlloc(GHND, dwNeeded);
	lpINFO5 = (LPPRINTER_INFO_5)GlobalLock(hInfo);
	EnumPrinters(PRINTER_ENUM_LOCAL,NULL,5,(LPBYTE)lpINFO5,dwNeeded,&dwNeeded,&dwReturned);
	for(i = 0;i < (int)dwReturned; i++){
		sprintf(szPortList, "%s (%s)", lpINFO5[i].pPortName, lpINFO5[i].pPrinterName);
		ret = pCB->FindString(0, szPortList);
		if(ret == -1){
			pCB->AddString(szPortList);
		}
	}
	GlobalUnlock(hInfo);
	GlobalFree(hInfo);

	GetProfileString("ports", NULL, "", szPortList, 1024);
	idx = 0;
	lpSp = szPortList;
	while(1){
		if(*lpSp == '\0'){
			break;
		}
		ret = pCB->FindString(0, lpSp);
		if(ret == -1){
			pCB->AddString(lpSp);
		}
		len = strlen(lpSp);
		lpSp = lpSp+len+1;
	}
}

void CDialogScrnEnv::OnPlotsize() 
{
	int		selno;

	UpdateData(TRUE);
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_CNTRKIND);
	selno = pCB->GetCurSel();
	switch(selno){
	case 0:
	case 2:
		m_dWorkXSize = m_dPlotXSize;
		m_dWorkYSize = m_dPlotYSize;
		break;
	case 1:
	case 3:
		m_dWorkXSize = m_dPlotYSize;
		m_dWorkYSize = m_dPlotXSize;
		break;
	}
	UpdateData(FALSE);
}

void CDialogScrnEnv::CntrKindToStrg(int kind, char *buf)
{
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_CNTRKIND);
	switch(kind){
	case LEFTDOWN:
		pCB->GetLBText(0, buf);
		break;
	case LEFTUP:
		pCB->GetLBText(1, buf);
		break;
	case RIGHTUP:
		pCB->GetLBText(2, buf);
		break;
	case RIGHTDOWN:
		pCB->GetLBText(3, buf);
		break;
	default:
		pCB->GetLBText(4, buf);
		break;
	}
}

void CDialogScrnEnv::CntrStrgToKind(int *kind, char *srchbuf)
{
	char	cntrname[QRECMAX];

	*kind = LEFTDOWN;
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_CNTRKIND);
	pCB->GetLBText(0, cntrname);
	if(strcmp(cntrname, srchbuf) == 0){
		*kind = LEFTDOWN;
	}
	pCB->GetLBText(1, cntrname);
	if(strcmp(cntrname, srchbuf) == 0){
		*kind = LEFTUP;
	}
	pCB->GetLBText(2, cntrname);
	if(strcmp(cntrname, srchbuf) == 0){
		*kind = RIGHTUP;
	}
	pCB->GetLBText(3, cntrname);
	if(strcmp(cntrname, srchbuf) == 0){
		*kind = RIGHTDOWN;
	}
}

void CDialogScrnEnv::OnSelchangePlotname() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int		idx;
	char	pData[RECMAX];

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_PLOTNAME);
	idx = pCB->GetCurSel();
	pCB->GetLBText(idx, pData );
	PlotElementGet(pData);
}

void CDialogScrnEnv::PlotElementGet(LPSTR SrchPlotName)
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	plotername[QRECMAX];
	int		setflag;

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_PLOTNAME);
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
			if(strcmp(plotername, SrchPlotName) == 0){
				setflag = ON;
				break;
			}
		}
		fclose(fp);
	}
	if(setflag == OFF){
		strcpy(m_stPlotEnv.cmdtstr, "HPGL");
		m_stPlotEnv.cmdtype = HPGL;
		m_stPlotEnv.sizex = 590;
		m_stPlotEnv.sizey = 430;
		m_stPlotEnv.step = 0.1;
		m_stPlotEnv.sx = -295;
		m_stPlotEnv.sy = -215;
		m_stPlotEnv.xtime = 1.0;
		m_stPlotEnv.ytime = 1.0;
		m_stPlotEnv.arcflg = OFF;
		m_stPlotEnv.outflg = OFF;
	}
	m_csPlotName = _T(SrchPlotName);
	m_nPlotType = m_stPlotEnv.cmdtype;
	m_dPlotXSize = m_stPlotEnv.sizex;
	m_dPlotYSize = m_stPlotEnv.sizey;
	m_dPlotStep = m_stPlotEnv.step;
	m_dPlotSx = m_stPlotEnv.sx;
	m_dPlotSy = m_stPlotEnv.sy;
	m_fPlotArc = m_stPlotEnv.arcflg;
	m_fPlotFill = m_stPlotEnv.outflg;
	UpdateData(FALSE);
}


char* CDialogScrnEnv::CrDelFgets(char *buf, int max, FILE *fp)
{
	char	*ret;
	int		len;

	ret = fgets(buf, max, fp);
	if(ret != NULL){
		len = strlen(buf);
		if(buf[len-1] == '\n'){
			buf[len-1] = '\0';
		}
	}
	return(ret);

}

void CDialogScrnEnv::OnOK() 
{
	int		kind;
	DBL		dDeepCut;
	int		nDot;
	char	numstr[QRECMAX];

	// TODO: この位置にその他の検証用のコードを追加してください
	UpdateData(TRUE);
	scrnsetmmsize(m_dWorkXSize, m_dWorkYSize);
	mojienvsetfontpath(m_csFontPath.GetBuffer(RECMAX));
	senvsetdatpath(m_csDataPath.GetBuffer(RECMAX));
	senvsetwakpath(m_csWakuPath.GetBuffer(RECMAX));
	senvsetkmnpath(m_csKmonPath.GetBuffer(RECMAX));
	senvsetoutpath(m_csOutDPath.GetBuffer(RECMAX));
	senvsetpartpath(m_csPartPath.GetBuffer(RECMAX));
	senvsetbtppath(m_csTBMPPath.GetBuffer(RECMAX));
	senvsettmppath(m_csTempPath.GetBuffer(RECMAX));
	pouttypeset(m_nPlotType);
	poutmmsizeset(m_dPlotXSize, m_dPlotYSize);
	poutstepset(m_dPlotStep);
	poutmmstartset(m_dPlotSx, m_dPlotSy);
	CntrStrgToKind(&kind, m_csCntrKind.GetBuffer(RECMAX));
	poutcntrkindset(kind);
	poutportnameset(m_csPortName.GetBuffer(RECMAX));
	poutploternameset(m_csPlotName.GetBuffer(RECMAX));
	poutarcflagset(m_fPlotArc);
	poutfillflagset(m_fPlotFill);
	strcpy(numstr, m_csDeepCut.GetBuffer(QRECMAX));
	dDeepCut = atof(numstr);
	poutsetdeepcut(dDeepCut);
	nDot = atoi(m_csAreaDot);
	senvsetareadotsize(nDot);
	nDot = atoi(m_csPointDot);
	senvsetpointdotsize(nDot);
	CDialog::OnOK();
}


BOOL CDialogScrnEnv::OnInitDialog() 
{
	char	buf[HRECMAX];
	int		kind;
	char	numstr[QRECMAX];
	DBL		dDeepCut;
	int		nDot;

	CDialog::OnInitDialog();
	// TODO: この位置に初期化の補足処理を追加してください

	CBInitPaperSize();
	CBInitCntrKind();
	CBInitPlotName();
	CBInitPortName();

	scrngetmmsize(&m_dWorkXSize, &m_dWorkYSize);
	mojienvgetfontpath(buf);
	m_csFontPath = _T(buf);
	senvgetdatpath(buf);
	m_csDataPath = _T(buf);
	senvgetwakpath(buf);
	m_csWakuPath = _T(buf);
	senvgetkmnpath(buf);
	m_csKmonPath = _T(buf);
	senvgetoutpath(buf);
	m_csOutDPath = _T(buf);
	senvgetpartpath(buf);
	m_csPartPath = _T(buf);
	senvgetbtppath(buf);
	m_csTBMPPath = _T(buf);
	senvgettmppath(buf);
	m_csTempPath = _T(buf);
	poutmmsizeget(&m_dPlotXSize, &m_dPlotYSize);
	poutstepget(&m_dPlotStep);
	poutmmstartget(&m_dPlotSx, &m_dPlotSy);
	poutcntrkindget(&kind);
	CntrKindToStrg(kind, buf);
	m_csCntrKind = _T(buf);
	poutportnameget(buf);
	m_csPortName = _T(buf);
	poutploternameget(buf);
	m_csPlotName = _T(buf);
	pouttypeget(&m_nPlotType);
	poutarcflagget(&m_fPlotArc);
	poutfillflagget(&m_fPlotFill);
	poutgetdeepcut(&dDeepCut);
	sprintf(numstr, "%1.1lf", dDeepCut);
	m_csDeepCut = _T(numstr);
	senvgetareadotsize(&nDot);
	sprintf(numstr, "%d", nDot);
	m_csAreaDot = _T(numstr);
	senvgetpointdotsize(&nDot);
	sprintf(numstr, "%d", nDot);
	m_csPointDot = _T(numstr);

	UpdateData(FALSE);

	return TRUE;
}

void CDialogScrnEnv::GetSelectDir(LPSTR lpTitle, LPSTR lpPath)
{
	BROWSEINFO	bInfo;
	LPITEMIDLIST	lpFoldPointer;

	memset(&bInfo, 0, sizeof(BROWSEINFO));
	bInfo.hwndOwner = this->m_hWnd;
	bInfo.pidlRoot = 0;
	bInfo.lpszTitle = lpTitle;
	bInfo.ulFlags = 1;
	bInfo.lpfn   = BrowseCallbackProc; // コールバーク関数
	bInfo.lParam = (LPARAM)(lpPath);       // 初期フォルダ
	lpFoldPointer = SHBrowseForFolder(&bInfo);
	SHGetPathFromIDList(lpFoldPointer, lpPath);
}

void CDialogScrnEnv::OnBtnBtmp() 
{
	char	szPath[RECMAX];

	UpdateData(TRUE);
	strcpy(szPath, m_csTBMPPath.GetBuffer(FNAMEMAX));
	GetSelectDir("画像作業位置設定", szPath);
	if(szPath[0] != '\0'){
		m_csTBMPPath = _T(szPath);
		UpdateData(FALSE);
	}
}

void CDialogScrnEnv::OnBtnData() 
{
	char	szPath[RECMAX];

	UpdateData(TRUE);
	strcpy(szPath, m_csDataPath.GetBuffer(FNAMEMAX));
	GetSelectDir("データ位置設定", szPath);
	if(szPath[0] != '\0'){
		m_csDataPath = _T(szPath);
		UpdateData(FALSE);
	}
}

void CDialogScrnEnv::OnBtnFont() 
{
	char	szPath[RECMAX];

	UpdateData(TRUE);
	strcpy(szPath, m_csFontPath.GetBuffer(FNAMEMAX));
	GetSelectDir("書体位置設定", szPath);
	if(szPath[0] != '\0'){
		m_csFontPath = _T(szPath);
		UpdateData(FALSE);
	}
}

void CDialogScrnEnv::OnBtnKmn() 
{
	char	szPath[RECMAX];

	UpdateData(TRUE);
	strcpy(szPath, m_csKmonPath.GetBuffer(FNAMEMAX));
	GetSelectDir("家紋位置指定", szPath);
	if(szPath[0] != '\0'){
		m_csKmonPath = _T(szPath);
		UpdateData(FALSE);
	}
}

void CDialogScrnEnv::OnBtnPart() 
{
	char	szPath[RECMAX];

	UpdateData(TRUE);
	strcpy(szPath, m_csPartPath.GetBuffer(FNAMEMAX));
	GetSelectDir("部品位置指定", szPath);
	if(szPath[0] != '\0'){
		m_csPartPath = _T(szPath);
		UpdateData(FALSE);
	}
}

void CDialogScrnEnv::OnBtnOut() 
{
	char	szPath[RECMAX];

	UpdateData(TRUE);
	strcpy(szPath, m_csOutDPath.GetBuffer(FNAMEMAX));
	GetSelectDir("保管位置指定", szPath);
	if(szPath[0] != '\0'){
		m_csOutDPath = _T(szPath);
		UpdateData(FALSE);
	}
}

void CDialogScrnEnv::OnBtnTemp() 
{
	char	szPath[RECMAX];

	UpdateData(TRUE);
	strcpy(szPath, m_csTempPath.GetBuffer(FNAMEMAX));
	GetSelectDir("作業位置指定", szPath);
	if(szPath[0] != '\0'){
		m_csTempPath = _T(szPath);
		UpdateData(FALSE);
	}
}

void CDialogScrnEnv::OnBtnWaku() 
{
	char	szPath[RECMAX];

	UpdateData(TRUE);
	strcpy(szPath, m_csWakuPath.GetBuffer(FNAMEMAX));
	GetSelectDir("枠位置指定", szPath);
	if(szPath[0] != '\0'){
		m_csWakuPath = _T(szPath);
		UpdateData(FALSE);
	}
}
