// DialogDirectOut.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "math.h"
#include "DlDirect.h"
#include "DlDrtDtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDirectOut ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
	void senvgetdatpath(LPSTR path);
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
}

CDialogDirectOut::CDialogDirectOut(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDirectOut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDirectOut)
	m_bChkX = FALSE;
	m_bChkY = FALSE;
	m_dXMMOut = 0.0;
	m_dYMMOut = 0.0;
	m_dXMMPaper = 0.0;
	m_dYMMPaper = 0.0;
	m_dXMMNori = 0.0;
	m_dYMMNori = 0.0;
	m_nCopyCount = 0;
	m_bChkMark = FALSE;
	m_nPrintCut = -1;
	//}}AFX_DATA_INIT
	m_nFirst = TRUE;
}


void CDialogDirectOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDirectOut)
	DDX_Control(pDX, IDC_SPYMMPAPER, m_spYMMPaper);
	DDX_Control(pDX, IDC_SPYMMOUT, m_spYMMOut);
	DDX_Control(pDX, IDC_SPYMMNORI, m_spYMMNori);
	DDX_Control(pDX, IDC_SPXMMPAPER, m_spXMMPaper);
	DDX_Control(pDX, IDC_SPXMMOUT, m_spXMMOut);
	DDX_Control(pDX, IDC_SPXMMNORI, m_spXMMNori);
	DDX_Control(pDX, IDC_SPCOPY, m_spCopy);
	DDX_Control(pDX, IDC_EDYMMOUT, m_m_edYMMOut);
	DDX_Control(pDX, IDC_EDXMMOUT, m_edXMMOut);
	DDX_Control(pDX, IDC_CBCMDKIND, m_cbCmdKind);
	DDX_Check(pDX, IDC_CHKX, m_bChkX);
	DDX_Check(pDX, IDC_CHKY, m_bChkY);
	DDX_Text(pDX, IDC_EDXMMOUT, m_dXMMOut);
	DDX_Text(pDX, IDC_EDYMMOUT, m_dYMMOut);
	DDX_Text(pDX, IDC_EDXMMPAPER, m_dXMMPaper);
	DDX_Text(pDX, IDC_EDYMMPAPER, m_dYMMPaper);
	DDX_Text(pDX, IDC_EDXMMNORI, m_dXMMNori);
	DDX_Text(pDX, IDC_EDYMMNORI, m_dYMMNori);
	DDX_Text(pDX, IDC_EDCOPY, m_nCopyCount);
	DDX_Check(pDX, IDC_CHKMARK, m_bChkMark);
	DDX_Radio(pDX, IDC_RDOPRINT, m_nPrintCut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogDirectOut, CDialog)
	//{{AFX_MSG_MAP(CDialogDirectOut)
	ON_BN_CLICKED(IDC_BTNDETAILS, OnBtndetails)
	ON_EN_CHANGE(IDC_EDXMMOUT, OnChangeEdxmmout)
	ON_EN_CHANGE(IDC_EDYMMOUT, OnChangeEdymmout)
	ON_CBN_SELCHANGE(IDC_CBCMDKIND, OnSelchangeCbcmdkind)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDirectOut メッセージ ハンドラ

void CDialogDirectOut::OnBtndetails() 
{
	CDialogDirectDetails *pDialogDirectDetails;

	// ダイアログで出力機器　のりしろ　紙サイズ　
	pDialogDirectDetails = new CDialogDirectDetails(this);
	strcpy(pDialogDirectDetails->m_szCmdKind, m_szCmdKind);
	pDialogDirectDetails->m_nXdpi = m_nXdpi;
	pDialogDirectDetails->m_nYdpi = m_nYdpi;
	pDialogDirectDetails->m_dStep = m_dStep;
	pDialogDirectDetails->m_nProcess = m_nProcess;
	pDialogDirectDetails->m_dSendTime = m_dSendTime;
	strcpy(pDialogDirectDetails->m_szPaperName, m_szPaperName);
	if(pDialogDirectDetails->DoModal() == IDOK){
		strcpy(m_szPaperName, pDialogDirectDetails->m_szPaperName);
		m_dRgnum = pDialogDirectDetails->m_dRgnum;
		m_dGgnum = pDialogDirectDetails->m_dGgnum;
		m_dBgnum = pDialogDirectDetails->m_dBgnum;
		m_dRadd = pDialogDirectDetails->m_dRadd;
		m_dGadd = pDialogDirectDetails->m_dGadd;
		m_dBadd = pDialogDirectDetails->m_dBadd;
		m_dRcntrst = pDialogDirectDetails->m_dRcntrst;
		m_dGcntrst = pDialogDirectDetails->m_dGcntrst;
		m_dBcntrst = pDialogDirectDetails->m_dBcntrst;
		m_nXdpi = pDialogDirectDetails->m_nXdpi;
		m_nYdpi = pDialogDirectDetails->m_nYdpi;
		m_dStep = pDialogDirectDetails->m_dStep;
		m_nProcess = pDialogDirectDetails->m_nProcess;
		m_dSendTime = pDialogDirectDetails->m_dSendTime;
	}
}

void CDialogDirectOut::OnChangeEdxmmout() 
{
	DBL		xmm, ymm;

	if(m_nFirst != TRUE){
		UpdateData(TRUE);
		scrngetmmsize(&xmm, &ymm);
		if(m_bChkY == FALSE){
			m_dYMMOut = m_dXMMOut * ymm / xmm;
			m_dYMMOut = floor(m_dYMMOut*100.0) / 100.0;
			}
		UpdateData(FALSE);
	}
}

void CDialogDirectOut::OnChangeEdymmout() 
{
	DBL		xmm, ymm;

	if(m_nFirst != TRUE){
		UpdateData(TRUE);
		scrngetmmsize(&xmm, &ymm);
		if(m_bChkX == FALSE){
			m_dXMMOut = m_dYMMOut * xmm / ymm;
			m_dXMMOut = floor(m_dXMMOut*100.0) / 100.0;
		}
		UpdateData(FALSE);
	}
}

void CDialogDirectOut::OnSelchangeCbcmdkind() 
{
	char	buf[RECMAX];
	char	name[32];
	int		kind, xdpi, ydpi;
	DBL		step;
	int		selno;

	UpdateData(TRUE);
	selno = m_cbCmdKind.GetCurSel();
	m_cbCmdKind.GetLBText(selno, buf);
	sscanf(buf, "%s %d %d %d %lf", name, &kind, &xdpi, &ydpi, &step);
	m_nCmdKind = kind;
	strcpy(m_szCmdKind, name);
	m_nXdpi = xdpi;
	m_nYdpi = ydpi;
	m_dStep = step;

	PaperDefLoad(m_szCmdKind);

	UpdateData(FALSE);
}

BOOL CDialogDirectOut::OnInitDialog() 
{
	CDialog::OnInitDialog();
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	name[32];
	int		kind, xdpi, ydpi;
	DBL		step;
	int		outflag;
	int		idx, selno;

	CDialog::OnInitDialog();

	senvgetexepath(exepath);
	sprintf(fname, "%s\\directnm.env", exepath);
	if((fp = fopen(fname, "r")) == NULL){
		return TRUE;
	}
	idx = 0;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%s %d %d %d %lf %d", name, &kind, &xdpi, &ydpi, &step, &outflag);
		if(m_nCmdKind == kind){
			strcpy(m_szCmdKind, name);
			selno = idx;
			m_nXdpi = xdpi;
			m_nYdpi = ydpi;
			m_dStep = step;
			m_nOutFlag = outflag;
		}
		m_cbCmdKind.AddString(buf);
		idx++;
	}
	fclose(fp);
	m_cbCmdKind.SetCurSel(selno);
	
	PaperDefLoad(m_szCmdKind);

	m_spYMMPaper.SetRange(0, INTMAX);
	m_spYMMOut.SetRange(0, INTMAX);
	m_spYMMNori.SetRange(0, INTMAX);
	m_spXMMPaper.SetRange(0, INTMAX);
	m_spXMMOut.SetRange(0, INTMAX);
	m_spXMMNori.SetRange(0, INTMAX);
	m_spCopy.SetRange(0, 1000);
	UpdateData(FALSE);
	
	// TODO: この位置に初期化の補足処理を追加してください
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogDirectOut::PaperDefLoad(LPSTR szCmdname)
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	name[32];
	DBL		rgn, ggn, bgn;
	DBL		rad, gad, bad;
	DBL		rcr, gcr, bcr;

	m_dRgnum = 228;
	m_dGgnum = 228;
	m_dBgnum = 228;
	m_dRadd = 32;
	m_dGadd = 32;
	m_dBadd = 32;
	m_dRcntrst = 1.25;
	m_dGcntrst = 1.25;
	m_dBcntrst = 1.25;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\%s.ppf", exepath, szCmdname);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%s %lf %lf %lf %lf %lf %lf %lf %lf %lf", name, &rgn, &ggn, &bgn, &rad, &gad, &bad, &rcr, &gcr, &bcr);
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
		}
	}
	fclose(fp);
}

void CDialogDirectOut::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	if(m_nFirst == TRUE){
		m_nFirst = FALSE;
	}
}
