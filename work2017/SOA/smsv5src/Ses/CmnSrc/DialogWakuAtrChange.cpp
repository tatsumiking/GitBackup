// DlWkAChg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogWakuAtrChange.h"

//extern "C" {
#include "../../ObjLib/Mojienv.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuAtrChange ダイアログ

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
	TCHAR	buf[RECMAX];

	CDialog::OnInitDialog();
	InitFontList();
	mojienvgetfontname(buf);
	m_csFont = buf;
	UpdateData(FALSE);
	return TRUE;
}

void CDialogWakuAtrChange::InitFontList()
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

void CDialogWakuAtrChange::OnOK() 
{
	TCHAR	buf[RECMAX];

	UpdateData(TRUE);

	_tcscpy_s(buf, m_csFont.GetBuffer(RECMAX));
	mojienvsetfontname(buf);
	CDialog::OnOK();
}

