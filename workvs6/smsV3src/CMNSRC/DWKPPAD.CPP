// DWkPpAdd.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DWkPpAd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWkPaperAdd ダイアログ
extern "C" {
void senvgetwakpath(LPSTR wakpath);
int baseiskanji(UINT ch);
LPSTR basecsvstrdivcpy(LPSTR sp, LPSTR pname);
void wakuenvinit(LPSTR envfile);
void wakuenvsetpaperelement();
void wakuenvgetpaperelement();
void wakuenvend();
void wakuenvsetpapername(LPSTR name);
void wakuenvgetpapername(LPSTR name);
void wakuenvsetpaperpath(LPSTR path);
void wakuenvgetpaperpath(LPSTR path);
void wakuenvgetfullpaperpath(LPSTR path);
void wakuenvdelpaper(LPSTR delpapername);
}

CDialogWkPaperAdd::CDialogWkPaperAdd(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWkPaperAdd::IDD, pParent)
{
	m_pcView = (CTonView*)pParent;
	//{{AFX_DATA_INIT(CDialogWkPaperAdd)
	m_csPaperPath = _T("");
	m_csPaperName = _T("");
	//}}AFX_DATA_INIT
}

void CDialogWkPaperAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWkPaperAdd)
	DDX_Text(pDX, IDC_PATH, m_csPaperPath);
	DDX_CBString(pDX, IDC_PAPERNAME, m_csPaperName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogWkPaperAdd, CDialog)
	//{{AFX_MSG_MAP(CDialogWkPaperAdd)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_CBN_SELCHANGE(IDC_PAPERNAME, OnSelchangePapername)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWkPaperAdd メッセージ ハンドラ

BOOL CDialogWkPaperAdd::OnInitDialog() 
{
	CRect	rect;
	int		sx, sy, sizex, sizey;
	char	papername[RECMAX];
	char	paperpath[RECMAX];

	CDialog::OnInitDialog();

	GetWindowRect(&rect);
	sx = rect.left;
	sy = rect.top;
	sizex = rect.Width();
	sizey = rect.Height();
	::SetWindowPos(m_hWnd, HWND_TOPMOST, sx, sy, sizex, sizey, 0);

	CBInitPaperName();
	wakuenvgetpaperelement();
	wakuenvgetpapername(papername);
	m_csPaperName = _T(papername);
	wakuenvgetfullpaperpath(paperpath);
	m_csPaperPath = _T(paperpath);
	UpdateData(FALSE);
	
	return TRUE;
}

void CDialogWkPaperAdd::CBInitPaperName()
{
	char	wakpath[FNAMEMAX];
	char	paperlistfname[FNAMEMAX];
	char	buf[RECMAX];
	char	*sp;
	char	pname[HRECMAX];
	FILE	*fp;

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_PAPERNAME);
	senvgetwakpath(wakpath);
	sprintf(paperlistfname, "%s\\paperlst.lst", wakpath);
	if((fp = fopen(paperlistfname, "r")) == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, pname);
		pCB->AddString(pname);
	}
	fclose(fp);
}

void CDialogWkPaperAdd::OnSelchangePapername() 
{
	char	buf[RECMAX];
	int		idx;
	char	pname[RECMAX];
	char	paperpath[RECMAX];

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_PAPERNAME);
	idx = pCB->GetCurSel();
	pCB->GetLBText(idx, pname);
	UpdateData(TRUE);
	wakuenvsetpapername(pname);
	wakuenvgetpaperelement();
	wakuenvgetpapername(buf);
	m_csPaperName = _T(buf);
	wakuenvgetfullpaperpath(paperpath);
	m_csPaperPath = _T(paperpath);
	UpdateData(FALSE);
}

void CDialogWkPaperAdd::OnSave() 
{
	char		paperpath[FNAMEMAX];
	char		printenvfname[FNAMEMAX];

	UpdateData(TRUE);
	if(CheckFileName(m_csPaperName.GetBuffer(RECMAX)) == FALSE){
		::AfxMessageBox("型名には下記の文字は使えません。\n\t\\/:,;*?\"<> |", MB_OK, 0);
		return;
	}
	wakuenvsetpapername(m_csPaperName.GetBuffer(RECMAX));
	strcpy(paperpath, m_csPaperName.GetBuffer(RECMAX));
	wakuenvsetpaperpath(paperpath);
	wakuenvsetpaperelement();

	wakuenvgetfullpaperpath(paperpath);
	sprintf(printenvfname, "%s\\printenv.bin", paperpath);
	m_pcView->PrintEnvSave(printenvfname);

	wakuenvend();
	CDialog::OnOK();
}

BOOL CDialogWkPaperAdd::CheckFileName(LPSTR lpFileName)
{
	int		hc;
	char	badchar[16];
	BOOL	flag;
	int		i;
	int		cnt;
	LPSTR	sp;

	strcpy(badchar, "\\/:,;*?\"<> |");
	cnt = strlen(badchar);

	flag = TRUE;
	sp = lpFileName;
	while(1){
		hc = (*sp) & 0x00ff;
		if(hc == '\0'){
			break;
		}
		if(baseiskanji(hc) == ON){
			sp += 2;
		}
		else{
			for(i = 0;i < cnt;i++){
				if(*sp == badchar[i]){
					flag = FALSE;
				}
			}
			sp++;
		}
	}
	return(flag);
}
