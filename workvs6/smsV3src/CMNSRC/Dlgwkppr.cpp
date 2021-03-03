// DlgWkPpr.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgWkPpr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
void senvgetexepath(LPSTR execpath);
void scrngetmmsize(LPDBL dmmxsize, LPDBL dmmysize);
}

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuPaper ダイアログ


CDialogWakuPaper::CDialogWakuPaper(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWakuPaper::IDD, pParent)
{
	m_pcView = (CTonView*)pParent;
	//{{AFX_DATA_INIT(CDialogWakuPaper)
	m_dPaperHight = 0.0;
	m_dPaperSizeX = 0.0;
	m_dPaperSizeY = 0.0;
	m_dPaperSX = 0.0;
	m_dPaperSY = 0.0;
	m_dPaperWidth = 0.0;
	m_csPaperName = _T("");
	//}}AFX_DATA_INIT
}


void CDialogWakuPaper::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuPaper)
	DDX_Text(pDX, IDC_PAPERHIGHT, m_dPaperHight);
	DDX_Text(pDX, IDC_PAPERSIZEX, m_dPaperSizeX);
	DDX_Text(pDX, IDC_PAPERSIZEY, m_dPaperSizeY);
	DDX_Text(pDX, IDC_PAPERSX, m_dPaperSX);
	DDX_Text(pDX, IDC_PAPERSY, m_dPaperSY);
	DDX_Text(pDX, IDC_PAPERWIDTH, m_dPaperWidth);
	DDX_CBString(pDX, IDC_PAPERNAME, m_csPaperName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogWakuPaper, CDialog)
	//{{AFX_MSG_MAP(CDialogWakuPaper)
	ON_CBN_SELCHANGE(IDC_PAPERNAME, OnSelchangePapername)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuPaper メッセージ ハンドラ

BOOL CDialogWakuPaper::OnInitDialog() 
{
	CRect	rect;
	int		sx, sy, sizex, sizey;
	DBL		mmx, mmy;

	CDialog::OnInitDialog();
	
	GetWindowRect(&rect);
	sx = rect.left;
	sy = rect.top;
	sizex = rect.Width();
	sizey = rect.Height();
	::SetWindowPos(m_hWnd, HWND_TOPMOST, sx, sy, sizex, sizey, 0);

	CBInitPaperName();
	scrngetmmsize(&mmx, &mmy);
	m_dPaperSizeX = mmx;
	m_dPaperSizeY = mmy;
	m_dPaperSX = 0;
	m_dPaperSY = 0;
	m_dPaperWidth = mmx;
	m_dPaperHight = mmy;

	UpdateData(FALSE);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogWakuPaper::CBInitPaperName()
{
	char	exepath[RECMAX];
	char	fname[RECMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	papername[QRECMAX];

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_PAPERNAME);
	pCB->ResetContent();
	senvgetexepath(exepath);
	sprintf(fname, "%s\\papersiz.env", exepath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%s", papername);
		if(papername[0] == '\0'){
			break;
		}
		pCB->AddString(papername);
	}
	fclose(fp);
}

void CDialogWakuPaper::OnSelchangePapername() 
{
	int		idx;
	char	pname[RECMAX];

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_PAPERNAME);
	idx = pCB->GetCurSel();
	pCB->GetLBText(idx, pname);
	UpdateData(TRUE);
	GetPaperElement(pname);
}

void CDialogWakuPaper::OnSave() 
{
	char		pname[FNAMEMAX];

	// TODO: この位置にその他の検証用のコードを追加してください
	UpdateData(TRUE);
	strcpy(pname, m_csPaperName.GetBuffer(RECMAX));
	SavePaperElement(pname); 
	CDialog::OnOK();
}

void CDialogWakuPaper::OnDelete() 
{
	char	pname[RECMAX];
	char	buf[RECMAX];
	int		ret;

	UpdateData(TRUE);
	strcpy(pname, m_csPaperName.GetBuffer(RECMAX));
	sprintf(buf, "[%s]を削除してよろしいですか", pname);
	ret = AfxMessageBox(buf, MB_OKCANCEL);
	if(ret == IDOK){
		DelPaperName(pname);
	}
	CDialog::OnOK();
}

void CDialogWakuPaper::GetPaperElement(LPSTR srchname)
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
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%s", papername);
		if(strcmp(papername, srchname) == 0){
			sscanf(buf, "%s %lf %lf %lf %lf %lf %lf"
				, papername, &mmx, &mmy, &sx, &sy, &width, &hight);
			m_dPaperSizeX = mmx;
			m_dPaperSizeY = mmy;
			m_dPaperSX = sx;
			m_dPaperSY = sy;
			m_dPaperWidth = width;
			m_dPaperHight = hight;
			break;
		}
	}
	fclose(fp);
}

void CDialogWakuPaper::SavePaperElement(LPSTR srchname)
{
	char	exepath[RECMAX];
	char	sfname[RECMAX];
	char	dfname[RECMAX];
	FILE	*sfp;
	FILE	*dfp;
	char	papername[QRECMAX];
	char	buf[RECMAX];
	int		flag;

	BackupPaperName();
	flag = OFF;
	senvgetexepath(exepath);
	sprintf(sfname, "%s\\papersiz.bak", exepath);
	sprintf(dfname, "%s\\papersiz.env", exepath);
	if((dfp = fopen(dfname, "w")) == NULL){
		return;
	}
	if((sfp = fopen(sfname, "r")) == NULL){
		fprintf(dfp, "%s %4.1lf %4.1lf %4.1lf %4.1lf %4.1lf %4.1lf\n",
			srchname, m_dPaperSizeX, m_dPaperSizeY, m_dPaperSX, m_dPaperSY, m_dPaperWidth, m_dPaperHight);
		fclose(dfp);
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, sfp) == NULL){
			break;
		}
		sscanf(buf, "%s", papername);
		if(strcmp(papername, srchname) == 0){
			fprintf(dfp, "%s %4.1lf %4.1lf %4.1lf %4.1lf %4.1lf %4.1lf\n",
				srchname, m_dPaperSizeX, m_dPaperSizeY, m_dPaperSX, m_dPaperSY, m_dPaperWidth, m_dPaperHight);
			flag = ON;
		}
		else{
			fprintf(dfp, "%s", buf);
		}
	}
	if(flag == OFF){
		fprintf(dfp, "%s %4.1lf %4.1lf %4.1lf %4.1lf %4.1lf %4.1lf\n",
			srchname, m_dPaperSizeX, m_dPaperSizeY, m_dPaperSX, m_dPaperSY, m_dPaperWidth, m_dPaperHight);
	}
	fclose(sfp);
	fclose(dfp);
}

void CDialogWakuPaper::DelPaperName(LPSTR srchname)
{
	char	exepath[RECMAX];
	char	sfname[RECMAX];
	char	dfname[RECMAX];
	FILE	*sfp;
	FILE	*dfp;
	char	papername[QRECMAX];
	char	buf[RECMAX];

	BackupPaperName();
	senvgetexepath(exepath);
	sprintf(sfname, "%s\\papersiz.bak", exepath);
	sprintf(dfname, "%s\\papersiz.env", exepath);
	if((dfp = fopen(dfname, "w")) == NULL){
		return;
	}
	if((sfp = fopen(sfname, "r")) == NULL){
		fclose(dfp);
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, sfp) == NULL){
			break;
		}
		sscanf(buf, "%s", papername);
		if(strcmp(papername, srchname) != 0){
			fprintf(dfp, "%s", buf);
		}
	}
	fclose(sfp);
	fclose(dfp);
}

void CDialogWakuPaper::BackupPaperName()
{
	char	exepath[RECMAX];
	char	sfname[RECMAX];
	char	dfname[RECMAX];
	FILE	*sfp;
	FILE	*dfp;
	char	buf[RECMAX];

	senvgetexepath(exepath);
	sprintf(sfname, "%s\\papersiz.env", exepath);
	sprintf(dfname, "%s\\papersiz.bak", exepath);
	if((dfp = fopen(dfname, "w")) == NULL){
		return;
	}
	if((sfp = fopen(sfname, "r")) == NULL){
		fclose(dfp);
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, sfp) == NULL){
			break;
		}
		fprintf(dfp, "%s", buf);
	}
	fclose(sfp);
	fclose(dfp);
}

