// DSlctFtpAdrs.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DSlctFtpAdrs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSlctFtpAdrs ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
}

CDialogSlctFtpAdrs::CDialogSlctFtpAdrs(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSlctFtpAdrs::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSlctFtpAdrs)
	m_csFtpFolder = _T("");
	m_csFileName = _T("");
	//}}AFX_DATA_INIT
}


void CDialogSlctFtpAdrs::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSlctFtpAdrs)
	DDX_Control(pDX, IDC_CMBFTPFOLDER, m_cmbFtpFolder);
	DDX_CBString(pDX, IDC_CMBFTPFOLDER, m_csFtpFolder);
	DDX_Text(pDX, IDC_EDFILENAME, m_csFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSlctFtpAdrs, CDialog)
	//{{AFX_MSG_MAP(CDialogSlctFtpAdrs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSlctFtpAdrs メッセージ ハンドラ

BOOL CDialogSlctFtpAdrs::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitCmbFtpFolder();
	InitFileName();
	UpdateData(FALSE);
	return TRUE;
}

void CDialogSlctFtpAdrs::InitCmbFtpFolder()
{
	char	exepath[FNAMEMAX];
	char	szFileName[FNAMEMAX];
	FILE	*fp;
	char	szBuf[RECMAX];

	m_cmbFtpFolder.ResetContent();
	senvgetexepath(exepath);
	sprintf(szFileName, "%s\\ftpfolder.txt", exepath);
	fp = fopen(szFileName, "r");
	if(fp == NULL){
		m_cmbFtpFolder.AddString("001");
		m_cmbFtpFolder.AddString("002");
		m_cmbFtpFolder.AddString("003");
		m_cmbFtpFolder.AddString("004");
		m_cmbFtpFolder.AddString("005");
		return;
	}
	while(1){
		if(fgets(szBuf, RECMAX, fp) == NULL){
			break;
		}
		if(szBuf[0] != '\0' && szBuf[1] != '\0'){
			szBuf[strlen(szBuf)-1] = '\0'; // \nを削除
			m_cmbFtpFolder.AddString(szBuf);
		}
	}
	fclose(fp);
}

void CDialogSlctFtpAdrs::InitFileName()
{
	COleDateTime	cOleTime;
	int		yy, mm, dd, hh, mn;

	cOleTime = COleDateTime::GetCurrentTime();
	yy = cOleTime.GetYear();
	mm = cOleTime.GetMonth();
	dd = cOleTime.GetDay();
	hh = cOleTime.GetHour();
	mn = cOleTime.GetMinute();
	m_csFileName.Format("%04d%02d%02d%02d%02d", yy, mm, dd, hh, mn);
}
