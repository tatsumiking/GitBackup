// DialogDirectDetails.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlDrtDtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDirectDetails ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
	int basestrgcntget(char far *strg);
}

CDialogDirectDetails::CDialogDirectDetails(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDirectDetails::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDirectDetails)
	m_dRgnum = 0.0;
	m_dGgnum = 0.0;
	m_dBgnum = 0.0;
	m_dRadd = 0.0;
	m_dGadd = 0.0;
	m_dBadd = 0.0;
	m_dRcntrst = 0.0;
	m_dGcntrst = 0.0;
	m_dBcntrst = 0.0;
	m_dStep = 0.0;
	m_nXdpi = 0;
	m_nYdpi = 0;
	//}}AFX_DATA_INIT
}


void CDialogDirectDetails::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDirectDetails)
	DDX_Control(pDX, IDC_SPYDPI, m_spYDpi);
	DDX_Control(pDX, IDC_SPXDPI, m_spXDpi);
	DDX_Control(pDX, IDC_SPRGNUM, m_spRGNum);
	DDX_Control(pDX, IDC_SPRCNTRST, m_spRCntrst);
	DDX_Control(pDX, IDC_SPRADD, m_spRAAdd);
	DDX_Control(pDX, IDC_SPGGNUM, m_spGGNum);
	DDX_Control(pDX, IDC_SPGCNTRST, m_spGCntrst);
	DDX_Control(pDX, IDC_SPGADD, m_spGAdd);
	DDX_Control(pDX, IDC_SPBGNUM, m_spBGNum);
	DDX_Control(pDX, IDC_SPBCNTRST, m_spBCntrst);
	DDX_Control(pDX, IDC_SPBADD, m_spBAdd);
	DDX_Control(pDX, IDC_CBPROCESS, m_cbProcess);
	DDX_Control(pDX, IDC_CBPAPERKIND, m_cbPaperkind);
	DDX_Text(pDX, IDC_EDRGNUM, m_dRgnum);
	DDX_Text(pDX, IDC_EDGGNUM, m_dGgnum);
	DDX_Text(pDX, IDC_EDBGNUM, m_dBgnum);
	DDX_Text(pDX, IDC_EDRADD, m_dRadd);
	DDX_Text(pDX, IDC_EDGADD, m_dGadd);
	DDX_Text(pDX, IDC_EDBADD, m_dBadd);
	DDX_Text(pDX, IDC_EDRCNTRST, m_dRcntrst);
	DDX_Text(pDX, IDC_EDGCNTRST, m_dGcntrst);
	DDX_Text(pDX, IDC_EDBCNTRST, m_dBcntrst);
	DDX_Text(pDX, IDC_EDSTEP, m_dStep);
	DDX_Text(pDX, IDC_EDXDPI, m_nXdpi);
	DDX_Text(pDX, IDC_EDYDPI, m_nYdpi);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogDirectDetails, CDialog)
	//{{AFX_MSG_MAP(CDialogDirectDetails)
	ON_CBN_SELCHANGE(IDC_CBPAPERKIND, OnSelchangeCbpaperkind)
	ON_CBN_SELCHANGE(IDC_CBPROCESS, OnSelchangeCbprocess)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDirectDetails メッセージ ハンドラ

BOOL CDialogDirectDetails::OnInitDialog() 
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	name[32];
	int		kind;
	int		idx, selno;

	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	PaperListInit();

	senvgetexepath(exepath);
	sprintf(fname, "%s\\process.env", exepath);
	if((fp = fopen(fname, "r")) == NULL){
		return TRUE;
	}
	idx = 0;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%s %d", name, &kind);
		if(m_nProcess == kind){
			selno = idx;
		}
		m_cbProcess.AddString(buf);
		idx++;
	}
	fclose(fp);
	m_cbProcess.SetCurSel(selno);

	m_spYDpi.SetRange(0, INTMAX);
	m_spXDpi.SetRange(0, INTMAX);
	m_spRGNum.SetRange(0, 255);
	m_spRCntrst.SetRange(0, 5);
	m_spRAAdd.SetRange(-255, 255);
	m_spGGNum.SetRange(0, 255);
	m_spGCntrst.SetRange(0, 5);
	m_spGAdd.SetRange(-255, 255);
	m_spBGNum.SetRange(0, 255);
	m_spBCntrst.SetRange(0, 5);
	m_spBAdd.SetRange(-255, 255);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogDirectDetails::OnSelchangeCbpaperkind() 
{
	int	selno;

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	selno = m_cbPaperkind.GetCurSel();
	m_cbPaperkind.GetLBText(selno, m_szPaperName);
	PaperDefLoad();
	UpdateData(FALSE);
}

void CDialogDirectDetails::OnSelchangeCbprocess() 
{
	int		selno;
	char	buf[RECMAX];
	char	name[32];
	int		kind;

	selno = m_cbProcess.GetCurSel();
	m_cbProcess.GetLBText(selno, buf);
	sscanf(buf, "%s %d", name, &kind);
	m_nProcess = kind;		
}

void CDialogDirectDetails::PaperDefLoad()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	int		cnt;
	char	name[32];
	DBL		rgn, ggn, bgn;
	DBL		rad, gad, bad;
	DBL		rcr, gcr, bcr;
	DBL		time;

	m_dRgnum = 228;
	m_dGgnum = 228;
	m_dBgnum = 228;
	m_dRadd = 32;
	m_dGadd = 32;
	m_dBadd = 32;
	m_dRcntrst = 1.25;
	m_dGcntrst = 1.25;
	m_dBcntrst = 1.25;
	m_dSendTime = 1.0;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\%s.ppf", exepath, m_szCmdKind);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		cnt = basestrgcntget(buf);
		if(cnt == 10){
			sscanf(buf, "%s %lf %lf %lf %lf %lf %lf %lf %lf %lf", name, &rgn, &ggn, &bgn, &rad, &gad, &bad, &rcr, &gcr, &bcr);
			time = 1.0;
		}
		else{
			sscanf(buf, "%s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", name, &rgn, &ggn, &bgn, &rad, &gad, &bad, &rcr, &gcr, &bcr, &time);
		}
		if(strcmp(name, m_szPaperName) == 0){
			m_dRgnum = rgn;
			m_dGgnum = ggn;
			m_dBgnum = bgn;
			m_dRadd = rad;
			m_dGadd = gad;
			m_dBadd = bad;
			m_dRcntrst = rcr;
			m_dGcntrst = gcr;
			m_dBcntrst = bcr;
			m_dSendTime = time;
		}
	}
	fclose(fp);
}

void CDialogDirectDetails::PaperListInit()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	int		cnt;
	char	name[32];
	DBL		rgn, ggn, bgn;
	DBL		rad, gad, bad;
	DBL		rcr, gcr, bcr;
	DBL		time;
	int		idx, selno;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\%s.ppf", exepath, m_szCmdKind);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	idx = 0;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		cnt = basestrgcntget(buf);
		if(cnt == 10){
			sscanf(buf, "%s %lf %lf %lf %lf %lf %lf %lf %lf %lf", name, &rgn, &ggn, &bgn, &rad, &gad, &bad, &rcr, &gcr, &bcr);
			time = 1.0;
		}
		else{
			sscanf(buf, "%s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", name, &rgn, &ggn, &bgn, &rad, &gad, &bad, &rcr, &gcr, &bcr, &time);
		}
		m_cbPaperkind.AddString(name);
		if(strcmp(name, m_szPaperName) == 0){
			m_dRgnum = rgn;
			m_dGgnum = ggn;
			m_dBgnum = bgn;
			m_dRadd = rad;
			m_dGadd = gad;
			m_dBadd = bad;
			m_dRcntrst = rcr;
			m_dGcntrst = gcr;
			m_dBcntrst = bcr;
			m_dSendTime = time;
			selno = idx;
		}
		idx++;
	}
	fclose(fp);
	m_cbPaperkind.SetCurSel(selno);
	UpdateData(FALSE);
}
