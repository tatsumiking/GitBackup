// DSlctMAdrs.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DSlctMAdrs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSlctMailAdrs ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
}


CDialogSlctMailAdrs::CDialogSlctMailAdrs(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSlctMailAdrs::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSlctMailAdrs)
	m_csSubject = _T("");
	m_csMailAdrs = _T("");
	//}}AFX_DATA_INIT
}


void CDialogSlctMailAdrs::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSlctMailAdrs)
	DDX_Control(pDX, IDC_CMBMAILADRS, m_cmbMailAdrs);
	DDX_Text(pDX, IDC_EDSUBJECT, m_csSubject);
	DDX_CBString(pDX, IDC_CMBMAILADRS, m_csMailAdrs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSlctMailAdrs, CDialog)
	//{{AFX_MSG_MAP(CDialogSlctMailAdrs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSlctMailAdrs メッセージ ハンドラ

BOOL CDialogSlctMailAdrs::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitCmbMailAdrs();	
	return TRUE;
}

void CDialogSlctMailAdrs::InitCmbMailAdrs()
{
	char	exepath[FNAMEMAX];
	char	szSendEnvFile[FNAMEMAX];
	FILE	*fp;
	char	szMailAdrs[RECMAX];

	m_cmbMailAdrs.ResetContent();
	senvgetexepath(exepath);
	sprintf(szSendEnvFile, "%s\\SendMail.env", exepath);
	fp = fopen(szSendEnvFile, "r");
	if(fp == NULL){
		strcpy(szMailAdrs, "モバイル用アドレス<mobile@c-alfo.com>");
		m_cmbMailAdrs.AddString(szMailAdrs);
	}else{
		while(1){
			if(fgets(szMailAdrs, RECMAX, fp) == NULL){
				break;
			}
			szMailAdrs[strlen(szMailAdrs)-1] = '\0';
			m_cmbMailAdrs.AddString(szMailAdrs);
		}
		fclose(fp);
	}
}

void CDialogSlctMailAdrs::OnOK() 
{
	int		spos, epos;
	CString	csStr;

	UpdateData(TRUE);
	spos = m_csMailAdrs.FindOneOf(_T("<"));
	epos = m_csMailAdrs.FindOneOf(_T(">"));
	m_csAdrs = m_csMailAdrs.Mid(spos+1, epos-spos-1);
	CDialog::OnOK();
}
