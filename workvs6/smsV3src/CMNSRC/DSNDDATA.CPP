// DSndData.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <shlobj.h>
#include "math.h"
#include "DSndData.h"
#include "DlSndDtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSendData ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
	void senvgetdatpath(LPSTR path);
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
}

CDialogSendData::CDialogSendData(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSendData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSendData)
	m_bChkX = FALSE;
	m_bChkY = FALSE;
	m_dXMMOut = 0.0;
	m_dYMMOut = 0.0;
	m_dXMMPaper = 0.0;
	m_dYMMPaper = 0.0;
	m_dXMMNori = 0.0;
	m_dYMMNori = 0.0;
	m_nCopyCount = 0;
	m_csSendPath = _T("");
	m_csSaverPath = _T("");
	//}}AFX_DATA_INIT
	m_nFirst = TRUE;
}

void CDialogSendData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSendData)
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
	DDX_Text(pDX, IDC_ED_SETPATH, m_csSendPath);
	DDX_Text(pDX, IDC_ED_SVPATH, m_csSaverPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSendData, CDialog)
	//{{AFX_MSG_MAP(CDialogSendData)
	ON_BN_CLICKED(IDC_BTNDETAILS, OnBtndetails)
	ON_EN_CHANGE(IDC_EDXMMOUT, OnChangeEdxmmout)
	ON_EN_CHANGE(IDC_EDYMMOUT, OnChangeEdymmout)
	ON_CBN_SELCHANGE(IDC_CBCMDKIND, OnSelchangeCbcmdkind)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_SETSLCT, OnBtnSetslct)
	ON_BN_CLICKED(IDC_BTN_SVSLCT, OnBtnSvslct)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSendData メッセージ ハンドラ

void CDialogSendData::OnBtndetails() 
{
	CDialogSendDetails *pDialogSendDetails;

	// ダイアログで出力機器　のりしろ　紙サイズ　
	pDialogSendDetails = new CDialogSendDetails(this);
	strcpy(pDialogSendDetails->m_szCmdKind, m_szCmdKind);
	pDialogSendDetails->m_nProcess = m_nProcess;
	strcpy(pDialogSendDetails->m_szPaperName, m_szPaperName);
	pDialogSendDetails->m_dSendTime = m_dSendTime;
	if(pDialogSendDetails->DoModal() == IDOK){
		strcpy(m_szPaperName, pDialogSendDetails->m_szPaperName);
		m_dRgnum = pDialogSendDetails->m_dRgnum;
		m_dGgnum = pDialogSendDetails->m_dGgnum;
		m_dBgnum = pDialogSendDetails->m_dBgnum;
		m_dRadd = pDialogSendDetails->m_dRadd;
		m_dGadd = pDialogSendDetails->m_dGadd;
		m_dBadd = pDialogSendDetails->m_dBadd;
		m_dRcntrst = pDialogSendDetails->m_dRcntrst;
		m_dGcntrst = pDialogSendDetails->m_dGcntrst;
		m_dBcntrst = pDialogSendDetails->m_dBcntrst;
		m_nProcess = pDialogSendDetails->m_nProcess;
		m_dSendTime = pDialogSendDetails->m_dSendTime;
	}
}

void CDialogSendData::OnChangeEdxmmout() 
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

void CDialogSendData::OnChangeEdymmout() 
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

void CDialogSendData::OnSelchangeCbcmdkind() 
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

BOOL CDialogSendData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	name[32];
	int		kind, xdpi, ydpi;
	DBL		step;
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
		sscanf(buf, "%s %d %d %d %lf", name, &kind, &xdpi, &ydpi, &step);
		if(m_nCmdKind == kind){
			strcpy(m_szCmdKind, name);
			selno = idx;
			m_nXdpi = xdpi;
			m_nYdpi = ydpi;
			m_dStep = step;
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

void CDialogSendData::PaperDefLoad(LPSTR szCmdname)
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

void CDialogSendData::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	if(m_nFirst == TRUE){
		m_nFirst = FALSE;
	}
}

void CDialogSendData::OnBtnSetslct() 
{
	BROWSEINFO bi;
	TCHAR szDir[FNAMEMAX];
	LPITEMIDLIST pidl;

	ZeroMemory(&bi,sizeof(bi));
	bi.lpszTitle = "保存位置を選択してください";
	pidl = SHBrowseForFolder(&bi);
	if(pidl){
		if (SHGetPathFromIDList(pidl,szDir)) {
			m_csSendPath = _T(szDir);
			UpdateData(FALSE);
		}
	}
}

void CDialogSendData::OnBtnSvslct() 
{
	BROWSEINFO bi;
	TCHAR szDir[FNAMEMAX];
	LPITEMIDLIST pidl;

	ZeroMemory(&bi,sizeof(bi));
	bi.lpszTitle = "サーバ位置を選択してください";
	pidl = SHBrowseForFolder(&bi);
	if(pidl){
		if (SHGetPathFromIDList(pidl,szDir)) {
			m_csSaverPath = _T(szDir);
			UpdateData(FALSE);
		}
	}
}
