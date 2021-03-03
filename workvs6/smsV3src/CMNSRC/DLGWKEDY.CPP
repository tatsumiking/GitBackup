
// DialogWakuEdiy.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgWkEdy.h"
#include "DlgWkAtr.h"
#include "DlgWkPpr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuEdiy ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
	void poutploternameget(LPSTR plotername);
	void poutploternameset(LPSTR plotername);
	void poutcntrkindset(int cntrkind);
	void poutcntrkindget(int *cntrkind);

	void poutmmsizeset(DBL xsize, DBL ysize);
	void pouttypeset(int type);
	void poutstepset(DBL step);
	void poutmmstartset(DBL sx, DBL sy);
	void poutarcflagset(int flag);
	void poutfillflagset(int flag);

	void wakuenvlistgetpapername(int idx, LPSTR papername);
	void wakuenvgetpapername(LPSTR papername);
	void wakuenvsetpapername(LPSTR papername);
	void wakuenvgetfixflag(int *flag);
	void wakuenvgettrnspar(LPDBL par);
	void wakuenvgetmatching(LPSTR mstr);
	void wakuenvsetfixflag(int flag);
	void wakuenvsettrnspar(DBL par);
	void wakuenvsetmatching(LPSTR mstr);
	void mojienvgetflist(int idx, LPINT type, LPSTR sfname, LPSTR ffname);
	void mojienvgetfontname(LPSTR fontname);
	void mojienvgettategakiflag(LPINT tateflg);
	void mojienvgetspacekind(LPINT spkflg);
	void mojienvgetstart(LPDBL dstartx, LPDBL dstarty);
	void mojienvgetsize(LPDBL dw, LPDBL dh);
	void mojienvgetspace(LPDBL dmsp, LPDBL dlsp);
	void mojienvgetstrlen(LPDBL dlen);
	void mojienvgetgyakukind(LPINT nkind);
	void mojienvgetsmpltext(LPSTR smpl);
	void mojienvgetarngkind(LPINT nkind);
	void mojienvgetorder(LPINT nkind);
	void mojienvgetstrgslop(LPDBL dslop);
	void mojienvgetmojiround(LPDBL dround);
	void mojienvgetstring(LPSTR instr);

	void mojienvsetfontname(LPSTR fontname);
	void mojienvsettategakiflag(int tateflg);
	void mojienvsetspacekind(int spkflg);
	void mojienvsetstart(DBL dstartx, DBL dstarty);
	void mojienvsetsize(DBL dw, DBL dh);
	void mojienvsetspace(DBL dmsp, DBL dlsp);
	void mojienvsetstrlen(DBL dlen);
	void mojienvsetstrgslop(DBL slop);
	void mojienvsetmojiround(DBL round);
	void mojienvsetgyakukind(int nkind);
	void mojienvsetsmpltext(LPSTR smpl);
	void mojienvsetarngkind(int nkind);
	void mojienvsetorder(LPINT nkind);
	void mojienvsetstrgslop(DBL dslop);
	void mojienvsetmojiround(DBL dround);
	void mojienvsetstring(LPSTR instr);

	DBL scrnsetxytime(DBL timex, DBL timey);
	void scrngetmmsize(LPDBL bxmm, LPDBL bymm);
	void scrnsetmmsize(DBL xsize, DBL ysize);
	void scrnsetrltvelement();
	void scrngetrltvtop(LPDBL sx, LPDBL sy); 
	void scrnsetrltvtop(DBL sx, DBL sy); 
	void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey);
}

CDialogWakuEdiy::CDialogWakuEdiy(CWnd* pParent /*=NULL*/)
	: CMyDialog(CDialogWakuEdiy::IDD, pParent)
{
	m_pcView = (CTonView*)pParent;
	//{{AFX_DATA_INIT(CDialogWakuEdiy)
	m_dHight = 0.0;
	m_csText = _T("");
	m_dWidth = 0.0;
	m_dWorkXSize = 0.0;
	m_dWorkYSize = 0.0;
	m_csFont = _T("");
	m_csPloter = _T("");
	m_csPaperName = _T("");
	m_nSpaceKind = FALSE;
	m_dMojiSpace = 0.0;
	m_nYokoMoji = FALSE;
	//}}AFX_DATA_INIT
}


void CDialogWakuEdiy::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuEdiy)
	DDX_Control(pDX, IDC_EDMOJISPACE, m_edMojiSpace);
	DDX_Control(pDX, IDC_CNTRKIND, m_btnCntrKind);
	DDX_Control(pDX, IDC_EDWORKYSIZE, m_edWorkYSize);
	DDX_Control(pDX, IDC_EDWORKXSIZE, m_edWorkXSize);
	DDX_Control(pDX, IDC_EDWIDTH, m_edWidth);
	DDX_Control(pDX, IDC_EDHIGHT, m_edHight);
	DDX_Control(pDX, IDC_EDTEXT, m_edText);
	DDX_Control(pDX, IDC_BTNTATEGAKI, m_btnTategaki);
	DDX_Control(pDX, IDC_CMBPAPERNAME, m_cmbPaperName);
	DDX_Control(pDX, IDC_CMBPLOTER, m_cmbPloter);
	DDX_Control(pDX, IDC_CMBFONT, m_cmbFont);
	DDX_Text(pDX, IDC_EDHIGHT, m_dHight);
	DDX_Text(pDX, IDC_EDTEXT, m_csText);
	DDX_Text(pDX, IDC_EDWIDTH, m_dWidth);
	DDX_Text(pDX, IDC_EDWORKXSIZE, m_dWorkXSize);
	DDX_Text(pDX, IDC_EDWORKYSIZE, m_dWorkYSize);
	DDX_CBString(pDX, IDC_CMBFONT, m_csFont);
	DDX_CBString(pDX, IDC_CMBPLOTER, m_csPloter);
	DDX_CBString(pDX, IDC_CMBPAPERNAME, m_csPaperName);
	DDX_Check(pDX, IDC_CKMOJIDATA, m_nSpaceKind);
	DDX_Text(pDX, IDC_EDMOJISPACE, m_dMojiSpace);
	DDX_Check(pDX, IDC_CHKYOKOMOJI, m_nYokoMoji);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogWakuEdiy, CMyDialog)
	//{{AFX_MSG_MAP(CDialogWakuEdiy)
	ON_BN_CLICKED(IDC_BTNTATEGAKI, OnBtntategaki)
	ON_BN_CLICKED(IDC_BTNOPTION, OnBtnoption)
	ON_BN_CLICKED(IDC_BTNPAPERSAVE, OnBtnpapersave)
	ON_BN_CLICKED(IDC_BTNPRINT, OnBtnprint)
	ON_CBN_SELCHANGE(IDC_CMBPAPERNAME, OnSelchangeCmbpapername)
	ON_BN_CLICKED(IDC_BTNSET, OnBtnset)
	ON_BN_CLICKED(IDC_BTNRESET, OnBtnreset)
	ON_EN_CHANGE(IDC_EDWORKXSIZE, OnChangeEdworkxsize)
	ON_EN_CHANGE(IDC_EDWORKYSIZE, OnChangeEdworkysize)
	ON_EN_CHANGE(IDC_EDHIGHT, OnChangeEdhight)
	ON_EN_CHANGE(IDC_EDWIDTH, OnChangeEdwidth)
	ON_EN_CHANGE(IDC_EDTEXT, OnChangeEdtext)
	ON_BN_CLICKED(IDC_CNTRKIND, OnCntrkind)
	ON_CBN_SELCHANGE(IDC_CMBPLOTER, OnSelchangeCmbploter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuEdiy メッセージ ハンドラ

void CDialogWakuEdiy::InitItemElement()
{
	InitFontList();
	InitPaperName();
	InitPlotName();
	//UpdateData(FALSE);
}

void CDialogWakuEdiy::SetItemElement()
{
	DBL		xmm, ymm;
	DBL		dw, dh, dlen;
	char	buf[RECMAX];
	int		flag;

	mojienvgetfontname(buf);
	m_csFont = _T(buf);
	poutploternameget(buf);
	m_csPloter = _T(buf);
	//wakuenvgetpapername(buf);
	//m_csPaperNmae = _T(buf);
	
	scrngetmmsize(&xmm, &ymm);
	m_dWorkXSize = xmm;
	m_dWorkYSize = ymm;
	mojienvgettategakiflag(&flag);
	mojienvgetsize(&dw, &dh);
	mojienvgetstrlen(&dlen);
	m_nTateYoko = flag;
	if(m_nTateYoko == ON){
		m_btnTategaki.SetWindowText("縦");
		xmm = dw;
		ymm = dlen;
	}
	else{
		m_btnTategaki.SetWindowText("横");
		xmm = dlen;
		ymm = dh;
	}
	m_dWidth = FLOOR100(xmm);
	m_dHight = FLOOR100(ymm);
	
	mojienvgetstring(buf);
	m_csText = _T(buf);
	UpdateData(FALSE);
}

void CDialogWakuEdiy::GetItemElement()
{
	DBL		dw, dh, dlen;
	char	buf[RECMAX];

	UpdateData(TRUE);

	strcpy(buf, m_csFont.GetBuffer(RECMAX));
	mojienvsetfontname(buf);
	strcpy(buf, m_csPloter.GetBuffer(RECMAX));
	poutploternameset(buf);
	strcpy(buf, m_csPaperName.GetBuffer(RECMAX));

	scrnsetmmsize(m_dWorkXSize, m_dWorkYSize);
	mojienvsettategakiflag(m_nTateYoko);

	mojienvgetsize(&dw, &dh);
	if(m_nTateYoko == ON){
		dw = m_dWidth;
		dlen = m_dHight;
	}
	else{
		dlen = m_dWidth;
		dh = m_dHight;
	}
	mojienvsetsize(dw, dh);
	mojienvsetstrlen(dlen);

	strcpy(buf, m_csText.GetBuffer(RECMAX));
	mojienvsetstring(buf);
}

void CDialogWakuEdiy::OnBtntategaki() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(m_nTateYoko == ON){
		m_nTateYoko = OFF;
		m_btnTategaki.SetWindowText("横");
	}
	else{
		m_nTateYoko = ON;
		m_btnTategaki.SetWindowText("縦");
	}
}

void CDialogWakuEdiy::OnBtnoption() 
{
	CDialogWakuAddAtr*	pDialogWakuAddAtr;
	int		flag;
	DBL		par;
	char	mstr[RECMAX];
	int		ret;
		
	pDialogWakuAddAtr = new CDialogWakuAddAtr(m_pcView);

	wakuenvgetfixflag(&flag);
	wakuenvgettrnspar(&par);
	wakuenvgetmatching(mstr);
	
	pDialogWakuAddAtr->m_nFixFlag = flag;
	pDialogWakuAddAtr->m_dTrns = par;
	pDialogWakuAddAtr->m_csInputStrg = m_csText;
	pDialogWakuAddAtr->m_csMatching  = _T(mstr);
	ret = pDialogWakuAddAtr->DoModal();
	if(ret == IDOK){
		flag = pDialogWakuAddAtr->m_nFixFlag;
		par = pDialogWakuAddAtr->m_dTrns;
		strcpy(mstr, pDialogWakuAddAtr->m_csMatching.GetBuffer(RECMAX));
		m_csText = pDialogWakuAddAtr->m_csInputStrg;
		wakuenvsetfixflag(flag);
		wakuenvsettrnspar(par);
		wakuenvsetmatching(mstr);
	}
	delete(pDialogWakuAddAtr);
}

void CDialogWakuEdiy::OnBtnpapersave() 
{
	CDialogWakuPaper*	pDialogWakuPaper;
	int		ret;
	char	papername[RECMAX];
	DBL		mmxsize, mmysize;
	CTonApp*	theApp;
	CMainFrame*	theWnd;

	pDialogWakuPaper = new CDialogWakuPaper(m_pcView);
	pDialogWakuPaper->m_csPaperName = m_csPaperName;
	ret = pDialogWakuPaper->DoModal();
	if(ret == IDOK){
		InitPaperName();
		m_csPaperName = pDialogWakuPaper->m_csPaperName;
		
		strcpy(papername, m_csPaperName.GetBuffer(RECMAX));
		GetPaperSize(papername, &mmxsize, &mmysize);
		m_dWorkXSize = mmxsize;
		m_dWorkYSize = mmysize;
		scrnsetmmsize(m_dWorkXSize, m_dWorkYSize);
		UpdateData(FALSE);
		((CTonView*)m_pcView)->WorkSizeReset();
		((CTonView*)m_pcView)->WorkAreaReset();
		m_pcView->Invalidate(TRUE);
	}
	delete(pDialogWakuPaper);
	UpdateData(FALSE);
}

void CDialogWakuEdiy::OnBtnprint() 
{
	CTonApp*	theApp;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_csPloter = _T("プリンタ");
	theApp = (CTonApp*)AfxGetApp();
	theApp->OnFilePrintSetup();
	UpdateData(FALSE);
}

BOOL CDialogWakuEdiy::OnInitDialog() 
{
	int		cntrkind;

	m_edText.SetMyDialog(this);
	m_edWorkYSize.SetMyDialog(this);
	m_edWorkXSize.SetMyDialog(this);
	m_edWidth.SetMyDialog(this);
	m_edHight.SetMyDialog(this);
	m_edText.SetItemID(IDC_EDTEXT);
	m_edWorkYSize.SetItemID(IDC_EDWORKYSIZE);
	m_edWorkXSize.SetItemID(IDC_EDWORKXSIZE);
	m_edWidth.SetItemID(IDC_EDWIDTH);
	m_edHight.SetItemID(IDC_EDHIGHT);
	m_edText.SetIMEMode(ON);
	m_edWorkYSize.SetIMEMode(OFF);
	m_edWorkXSize.SetIMEMode(OFF);
	m_edWidth.SetIMEMode(OFF);
	m_edHight.SetIMEMode(OFF);


	CDialog::OnInitDialog();
	InitFontList();
	InitPaperName();
	InitPlotName();
	scrngetmmsize(&m_dWorkXSize, &m_dWorkYSize);
	poutcntrkindget(&cntrkind);
	CntrKindBottonSet(cntrkind);
	//UpdateData(FALSE);
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogWakuEdiy::InitFontList()
{
	int		idx;
	int		fonttype;
	char	fontname[FNAMEMAX];
	char	accessname[FNAMEMAX];

	m_cmbFont.ResetContent();
	idx = 0;
	mojienvgetfontname(fontname);
	m_csFont = _T(fontname);
	while(1){
		mojienvgetflist(idx, &fonttype, fontname, accessname);
		if(fontname[0] == '\0' && accessname[0] == '\0'){
			break;
		}
		m_cmbFont.AddString(fontname);
		idx++;
	}
}

void CDialogWakuEdiy::InitPlotName()
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
	m_cmbPloter.ResetContent();
	while(1){
		if(CrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%s", plotername);
		m_cmbPloter.AddString(plotername);
	}
	fclose(fp);
	poutploternameget(plotername);
	m_csPloter = _T(plotername);
}

void CDialogWakuEdiy::InitPaperName()
{
	int		idx;
	char	exepath[RECMAX];
	char	fname[RECMAX];
	FILE	*fp;
	char	papername[QRECMAX];
	char	buf[RECMAX];

	m_cmbPaperName.ResetContent();
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
		m_cmbPaperName.AddString(papername);
		idx++;
	}
	fclose(fp);
	m_csPaperName = _T("USERSIZE");
}

LPSTR CDialogWakuEdiy::CrDelFgets(char * buf, int max, FILE * fp)
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

void CDialogWakuEdiy::OnSelchangeCmbpapername() 
{
	int		idx;
	char	papername[RECMAX];
	CTonApp*	theApp;
	CMainFrame*	theWnd;
	DBL			mmxsize, mmysize;

	idx = m_cmbPaperName.GetCurSel();
	m_cmbPaperName.GetLBText(idx, papername);

	GetPaperSize(papername, &mmxsize, &mmysize);
	m_dWorkXSize = mmxsize;
	m_dWorkYSize = mmysize;
	scrnsetmmsize(m_dWorkXSize, m_dWorkYSize);
	UpdateData(FALSE);

	m_pcView->WorkSizeReset();
	m_pcView->WorkAreaReset();
	m_pcView->Invalidate(TRUE);
}

void CDialogWakuEdiy::KeyIn(UINT nChar, UINT nID)
{
	if(nChar == VK_RETURN){
		switch(m_nChangeEditID){
		case IDC_EDWORKXSIZE:
		case IDC_EDWORKYSIZE:
			GetItemElement();
			m_pcView->WorkSizeReset();
			m_pcView->WorkAreaReset();
			m_pcView->Invalidate(TRUE);
			break;
		case IDC_EDWIDTH:
		case IDC_EDHIGHT:
			m_pcView->CommandExec(DLGDRAWEVENT, 0, 0);
			break;
		}
	}
}

void CDialogWakuEdiy::OnBtnset() 
{
	m_pcView->CommandExec(RESETEVENT, 0, 0);
}

void CDialogWakuEdiy::OnBtnreset() 
{
	m_pcView->CommandExec(ENDEVENT, 0, 0);
}

void CDialogWakuEdiy::OnChangeEdworkxsize() 
{
	m_nChangeEditID = IDC_EDWORKXSIZE;
}

void CDialogWakuEdiy::OnChangeEdworkysize() 
{
	m_nChangeEditID = IDC_EDWORKYSIZE;
}

void CDialogWakuEdiy::OnChangeEdhight() 
{
	m_nChangeEditID = IDC_EDHIGHT;
}

void CDialogWakuEdiy::OnChangeEdwidth() 
{
	m_nChangeEditID = IDC_EDWIDTH;
}

void CDialogWakuEdiy::OnChangeEdtext() 
{
	m_nChangeEditID = IDC_EDTEXT;
}

void CDialogWakuEdiy::GetPaperSize(LPSTR srchname, DBL *mmxsize, DBL *mmysize)
{
	char	exepath[RECMAX];
	char	fname[RECMAX];
	FILE	*fp;
	char	papername[QRECMAX];
	char	buf[RECMAX];
	DBL		mmx, mmy;
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
				, papername, &mmx, &mmy, &sx, &sy, &width, &hight);
			*mmxsize = mmx;
			*mmysize = mmy;
			break;
		}
	}
	fclose(fp);
}

void CDialogWakuEdiy::OnCntrkind() 
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
	CntrKindBottonSet(cntrkind);
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDialogWakuEdiy::CntrKindBottonSet(int cntrkind)
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

void CDialogWakuEdiy::OnSelchangeCmbploter() 
{
	int		idx;
	char	szPloterName[RECMAX];

	idx = m_cmbPloter.GetCurSel();
	m_cmbPloter.GetLBText(idx, szPloterName);
	m_csPloter = _T(szPloterName);
	PlotElementGet(szPloterName);
	UpdateData(FALSE);
}

void CDialogWakuEdiy::PlotElementGet(LPSTR SrchPlotName)
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	plotername[QRECMAX];
	pltenvtype m_stPlotEnv;
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
			if(strcmp(plotername, SrchPlotName) == 0){
				setflag = ON;
				break;
			}
		}
		fclose(fp);
	}
	if(setflag == OFF){
		strcpy(SrchPlotName, "HPGL");
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
	pouttypeset(m_stPlotEnv.cmdtype);
	poutmmsizeset(m_stPlotEnv.sizex, m_stPlotEnv.sizey);
	poutstepset(m_stPlotEnv.step);
	poutmmstartset(m_stPlotEnv.sx, m_stPlotEnv.sy);
	poutploternameset(SrchPlotName);
	poutarcflagset(m_stPlotEnv.arcflg);
	poutfillflagset(m_stPlotEnv.outflg);

	UpdateData(FALSE);
}

