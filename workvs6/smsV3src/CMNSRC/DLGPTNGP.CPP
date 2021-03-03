// DlgPtnGp.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgPtnGp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnGrup ダイアログ

extern "C" {
	void senvgetwakpath(LPSTR wakpath);
	void ptnenvgetpaperelement();
	void ptnenvsetpaperelement();
	void ptnenvsetgrupname(LPSTR name);
	void ptnenvgetgrupname(LPSTR name);
	void ptnenvsetpapersize(DBL xsize, DBL ysize);
	void ptnenvgetpapersize(DBL *xsize, DBL *ysize);
	LPSTR basecsvstrdivcpy(LPSTR sp, LPSTR retstr);
}

CDialogPtnGrup::CDialogPtnGrup(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPtnGrup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPtnGrup)
	m_csGrupName = _T("");
	m_dPaperSizeX = 0.0;
	m_dPaperSizeY = 0.0;
	//}}AFX_DATA_INIT
}


void CDialogPtnGrup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPtnGrup)
	DDX_Control(pDX, IDC_GRUPNAME, m_cbGrupName);
	DDX_CBString(pDX, IDC_GRUPNAME, m_csGrupName);
	DDX_Text(pDX, IDC_PAPERSIZEX, m_dPaperSizeX);
	DDX_Text(pDX, IDC_PAPERSIZEY, m_dPaperSizeY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPtnGrup, CDialog)
	//{{AFX_MSG_MAP(CDialogPtnGrup)
	ON_CBN_SELCHANGE(IDC_GRUPNAME, OnSelchangeGrupname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnGrup メッセージ ハンドラ

BOOL CDialogPtnGrup::OnInitDialog() 
{
	char	wakpath[FNAMEMAX];
	char	paperlistfname[FNAMEMAX];
	char	buf[RECMAX];
	char	*sp;
	FILE	*fp;
	char	grupname[HRECMAX];
	DBL		xsize, ysize;

	CDialog::OnInitDialog();

	senvgetwakpath(wakpath);
	sprintf(paperlistfname, "%s\\paperlst.lst", wakpath);
	if((fp = fopen(paperlistfname, "r")) == NULL){
		return TRUE;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, grupname);
		m_cbGrupName.AddString(grupname);
	}
	fclose(fp);

	ptnenvgetgrupname(grupname);
	ptnenvgetpaperelement();
	ptnenvgetpapersize(&xsize, &ysize);
	m_csGrupName = _T(grupname);
	m_dPaperSizeX = xsize;
	m_dPaperSizeY = ysize;
	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogPtnGrup::OnSelchangeGrupname() 
{
	int		idx;
	char	grupname[RECMAX];

	idx = m_cbGrupName.GetCurSel();
	m_cbGrupName.GetLBText(idx, grupname);
	UpdateData(TRUE);
	ptnenvsetgrupname(grupname);
	ptnenvgetpaperelement();
	ptnenvgetpapersize(&m_dPaperSizeX, &m_dPaperSizeY);
	m_csGrupName = _T(grupname);
	UpdateData(FALSE);
}

void CDialogPtnGrup::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CDialog::OnOK();
}
