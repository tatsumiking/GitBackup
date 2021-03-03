// DlWkAChg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlWkAChg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuAtrChange ダイアログ

extern "C" {
	void mojienvsetfontname(LPSTR fontname);
	void mojienvgetfontname(LPSTR fontname);
	void mojienvgetflist(int idx, LPINT type, LPSTR sfname, LPSTR ffname);
}

CDialogWakuAtrChange::CDialogWakuAtrChange(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWakuAtrChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogWakuAtrChange)
	m_csFont = _T("");
	//}}AFX_DATA_INIT
}


void CDialogWakuAtrChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuAtrChange)
	DDX_Control(pDX, IDC_CMBFONT, m_cmbFont);
	DDX_CBString(pDX, IDC_CMBFONT, m_csFont);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogWakuAtrChange, CDialog)
	//{{AFX_MSG_MAP(CDialogWakuAtrChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuAtrChange メッセージ ハンドラ

BOOL CDialogWakuAtrChange::OnInitDialog() 
{
	char	buf[RECMAX];

	CDialog::OnInitDialog();
	InitFontList();
	mojienvgetfontname(buf);
	m_csFont = _T(buf);
	UpdateData(FALSE);
	return TRUE;
}

void CDialogWakuAtrChange::InitFontList()
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

void CDialogWakuAtrChange::OnOK() 
{
	char	buf[RECMAX];

	UpdateData(TRUE);

	strcpy(buf, m_csFont.GetBuffer(RECMAX));
	mojienvsetfontname(buf);
	CDialog::OnOK();
}

