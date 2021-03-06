
// DialogWakuEdit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogWakuEdit.h"

#include "DialogWakuAddAtr.h"
#include "DialogWakuPaper.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Pout.h"
#include "../../ObjLib/Mojienv.h"
#include "../../ObjLib/Wakuenv.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuEdit ダイアログ

CDialogWakuEdit::CDialogWakuEdit(CWnd* pParent /*=NULL*/)
	: CMyDialog(CDialogWakuEdit::IDD, pParent)
{
	m_pcView = (CTonView*)pParent;
	//{{AFX_DATA_INIT(CDialogWakuEdit)
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


void CDialogWakuEdit::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuEdit)
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


BEGIN_MESSAGE_MAP(CDialogWakuEdit, CMyDialog)
	//{{AFX_MSG_MAP(CDialogWakuEdit)
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
// CDialogWakuEdit メッセージ ハンドラ

void CDialogWakuEdit::InitItemElement()
{
	InitFontList();
	InitPaperName();
	InitPlotName();
	//UpdateData(FALSE);
}

void CDialogWakuEdit::SetItemElement()
{
	DBL		xmm, ymm;
	DBL		dw, dh, dlen;
	DBL		dlinespace;
	TCHAR	buf[RECMAX];
	int		flag;

	mojienvgetfontname(buf);
	m_csFont = buf;
	poutploternameget(buf);
	m_csPloter = buf;
	//wakuenvgetpapername(buf);
	//m_csPaperNmae = _T(buf);
	
	scrngetmmsize(&xmm, &ymm);
	m_dWorkXSize = xmm;
	m_dWorkYSize = ymm;
	mojienvgettategakiflag(&flag);
	mojienvgetsize(&dw, &dh);
	mojienvgetstrlen(&dlen);
	mojienvgetspace(&m_dMojiSpace, &dlinespace);
	m_nTateYoko = flag;
	if(m_nTateYoko == ON){
		m_btnTategaki.SetWindowText(_T("縦"));
		xmm = dw;
		ymm = dlen;
	}
	else{
		m_btnTategaki.SetWindowText(_T("横"));
		xmm = dlen;
		ymm = dh;
	}
	m_dWidth = FLOOR100(xmm);
	m_dHight = FLOOR100(ymm);

	mojienvgetspacekind(&m_nSpaceKind);
	mojienvgetyokomojiflag(&m_nYokoMoji);

	mojienvgetstring(buf);
	m_csText = buf;
	UpdateData(FALSE);
}

void CDialogWakuEdit::GetItemElement()
{
	DBL		dw, dh, dlen;
	DBL		dmojispace, dlinespace;
	TCHAR	buf[RECMAX];

	UpdateData(TRUE);

	_tcscpy_s(buf, m_csFont.GetBuffer(RECMAX));
	mojienvsetfontname(buf);
	_tcscpy_s(buf, m_csPloter.GetBuffer(RECMAX));
	poutploternameset(buf);
	_tcscpy_s(buf, m_csPaperName.GetBuffer(RECMAX));

	scrnsetmmsize(m_dWorkXSize, m_dWorkYSize);
	mojienvsettategakiflag(m_nTateYoko);
	mojienvgetspace(&dmojispace, &dlinespace);
	mojienvsetspace(m_dMojiSpace, dlinespace);
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

	mojienvsetspacekind(m_nSpaceKind);
	mojienvsetyokomojiflag(m_nYokoMoji);

	_tcscpy_s(buf, m_csText.GetBuffer(RECMAX));
	mojienvsetstring(buf);
}

void CDialogWakuEdit::OnBtntategaki() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(m_nTateYoko == ON){
		m_nTateYoko = OFF;
		m_btnTategaki.SetWindowText(_T("横"));
	}
	else{
		m_nTateYoko = ON;
		m_btnTategaki.SetWindowText(_T("縦"));
	}
}

void CDialogWakuEdit::OnBtnoption() 
{
	CDialogWakuAddAtr*	pDialogWakuAddAtr;
	int		flag;
	DBL		par;
	TCHAR	mstr[RECMAX];
	int		ret;
		
	pDialogWakuAddAtr = new CDialogWakuAddAtr(m_pcView);

	wakuenvgetfixflag(&flag);
	wakuenvgettrnspar(&par);
	wakuenvgetmatching(mstr);
	
	pDialogWakuAddAtr->m_nChkFixed = flag;
	pDialogWakuAddAtr->m_dTrns = par;
	pDialogWakuAddAtr->m_csInputStrg = m_csText;
	pDialogWakuAddAtr->m_csMatching  = mstr;
	ret = pDialogWakuAddAtr->DoModal();
	if(ret == IDOK){
		flag = pDialogWakuAddAtr->m_nChkFixed;
		par = pDialogWakuAddAtr->m_dTrns;
		_tcscpy_s(mstr, pDialogWakuAddAtr->m_csMatching.GetBuffer(RECMAX));
		m_csText = pDialogWakuAddAtr->m_csInputStrg;
		wakuenvsetfixflag(flag);
		wakuenvsettrnspar(par);
		wakuenvsetmatching(mstr);
	}
	delete(pDialogWakuAddAtr);
}

void CDialogWakuEdit::OnBtnpapersave() 
{
	CDialogWakuPaper*	pDialogWakuPaper;
	int		ret;
	TCHAR	papername[RECMAX];
	DBL		mmxsize, mmysize;

	pDialogWakuPaper = new CDialogWakuPaper(m_pcView);
	pDialogWakuPaper->m_csPaperName = m_csPaperName;
	ret = pDialogWakuPaper->DoModal();
	if(ret == IDOK){
		InitPaperName();
		m_csPaperName = pDialogWakuPaper->m_csPaperName;
		
		_tcscpy_s(papername, m_csPaperName.GetBuffer(RECMAX));
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

void CDialogWakuEdit::OnBtnprint() 
{
	CTonApp*	theApp;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_csPloter = _T("プリンタ");
	theApp = (CTonApp*)AfxGetApp();
	theApp->OnFilePrintSetup();
	UpdateData(FALSE);
}

BOOL CDialogWakuEdit::OnInitDialog() 
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

void CDialogWakuEdit::InitFontList()
{
	int		idx;
	int		fonttype;
	TCHAR	fontname[FNAMEMAX];
	TCHAR	accessname[FNAMEMAX];

	m_cmbFont.ResetContent();
	idx = 0;
	mojienvflistopen(_T("rb"));
	mojienvgetfontname(fontname);
	m_csFont = fontname;
	while(1){
		mojienvgetflist(idx, &fonttype, fontname, accessname);
		if(fontname[0] == _T('\0') && accessname[0] == _T('\0')){
			break;
		}
		m_cmbFont.AddString(fontname);
		idx++;
	}
	mojienvflistclose();
}

void CDialogWakuEdit::InitPlotName()
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
	m_cmbPloter.ResetContent();
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		_stscanf_s(buf, _T("%s"), plotername, QRECMAX);
		m_cmbPloter.AddString(plotername);
	}
	fclose(fp);
	poutploternameget(plotername);
	m_csPloter = plotername;
}

void CDialogWakuEdit::InitPaperName()
{
	int		idx;
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	papername[QRECMAX];
	TCHAR	buf[RECMAX];

	m_cmbPaperName.ResetContent();
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
		m_cmbPaperName.AddString(papername);
		idx++;
	}
	fclose(fp);
	m_csPaperName = _T("USERSIZE");
}
void CDialogWakuEdit::OnSelchangeCmbpapername() 
{
	int		idx;
	TCHAR	papername[RECMAX];
	DBL			mmxsize, mmysize;

	idx = m_cmbPaperName.GetCurSel();
	m_cmbPaperName.GetLBText(idx, papername);

	GetPaperSize(papername, &mmxsize, &mmysize);
	m_dWorkXSize = mmxsize;
	m_dWorkYSize = mmysize;
	scrnsetmmsize(m_dWorkXSize, m_dWorkYSize);
	UpdateData(FALSE);
	m_pcView->RedrawFlagOn();
	m_pcView->WorkSizeReset();
	m_pcView->WorkAreaReset();
	m_pcView->Invalidate(TRUE);
}

void CDialogWakuEdit::KeyIn(UINT nTCHAR, UINT nID)
{
	int		cmd;
	if(nTCHAR == VK_RETURN){
		switch(m_nChangeEditID){
		case IDC_EDWORKXSIZE:
		case IDC_EDWORKYSIZE:
			GetItemElement();
			m_pcView->RedrawFlagOn();
			m_pcView->WorkSizeReset();
			m_pcView->WorkAreaReset();
			m_pcView->Invalidate(TRUE);
			cmd = m_pcView->GetSubCommand();
			m_pcView->SetNextCommand(cmd);
			OnBtnreset();	// コマンドを終了
			break;
		case IDC_EDWIDTH:
		case IDC_EDHIGHT:
			m_pcView->CommandExec(DLGDRAWEVENT, 0, 0);
			break;
		}
	}
}

void CDialogWakuEdit::OnBtnset() 
{
	m_pcView->CommandExec(RESETEVENT, 0, 0);
}

void CDialogWakuEdit::OnBtnreset() 
{
	m_pcView->CommandExec(ENDEVENT, 0, 0);
}

void CDialogWakuEdit::OnChangeEdworkxsize() 
{
	m_nChangeEditID = IDC_EDWORKXSIZE;
}

void CDialogWakuEdit::OnChangeEdworkysize() 
{
	m_nChangeEditID = IDC_EDWORKYSIZE;
}

void CDialogWakuEdit::OnChangeEdhight() 
{
	m_nChangeEditID = IDC_EDHIGHT;
}

void CDialogWakuEdit::OnChangeEdwidth() 
{
	m_nChangeEditID = IDC_EDWIDTH;
}

void CDialogWakuEdit::OnChangeEdtext() 
{
	m_nChangeEditID = IDC_EDTEXT;
}

void CDialogWakuEdit::GetPaperSize(LPTSTR srchname, DBL *mmxsize, DBL *mmysize)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	papername[QRECMAX];
	TCHAR	buf[RECMAX];
	DBL		mmx, mmy;
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
				, papername, QRECMAX, &mmx, &mmy, &sx, &sy, &width, &hight);
			*mmxsize = mmx;
			*mmysize = mmy;
			break;
		}
	}
	fclose(fp);
}

void CDialogWakuEdit::OnCntrkind() 
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

void CDialogWakuEdit::CntrKindBottonSet(int cntrkind)
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

void CDialogWakuEdit::OnSelchangeCmbploter() 
{
	int		idx;
	TCHAR	szPloterName[RECMAX];

	idx = m_cmbPloter.GetCurSel();
	m_cmbPloter.GetLBText(idx, szPloterName);
	m_csPloter = szPloterName;
	PlotElementGet(szPloterName);
	UpdateData(FALSE);
}

void CDialogWakuEdit::PlotElementGet(LPTSTR lpSrchPlotName)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	plotername[QRECMAX];
	pltenvtype m_stPlotEnv;
	int		setflag;
	int		len;

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
			if(_tcscmp(plotername, lpSrchPlotName) == 0){
				setflag = ON;
				break;
			}
		}
		fclose(fp);
	}
	if(setflag == OFF){
		len = _tcslen(_T("HPGL")) + 1;
		_tcscpy_s(lpSrchPlotName, len, _T("HPGL"));
		_tcscpy_s(m_stPlotEnv.cmdtstr, _T("HPGL"));
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
	poutploternameset(lpSrchPlotName);
	poutarcflagset(m_stPlotEnv.arcflg);
	poutfillflagset(m_stPlotEnv.outflg);

	UpdateData(FALSE);
}

