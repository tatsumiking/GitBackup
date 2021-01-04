// DSlctFtpAdrs.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogSlctFtpAdrs.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSlctFtpAdrs ダイアログ

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
	TCHAR	exepath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];
	FILE	*fp;
	TCHAR	szBuf[RECMAX];

	m_cmbFtpFolder.ResetContent();
	senvgetexepath(exepath);
	_stprintf_s(szFileName, _T("%s\\ftpfolder.txt"), exepath);
	_tfopen_s(&fp, szFileName, _T("r"));
	if(fp == NULL){
		m_cmbFtpFolder.AddString(_T("001"));
		m_cmbFtpFolder.AddString(_T("002"));
		m_cmbFtpFolder.AddString(_T("003"));
		m_cmbFtpFolder.AddString(_T("004"));
		m_cmbFtpFolder.AddString(_T("005"));
		return;
	}
	while(1){
		if(baselibCrDelFgets(szBuf, RECMAX, fp) == NULL){
			break;
		}
		if(szBuf[0] != _T('\0') && szBuf[1] != _T('\0')){
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
	m_csFileName.Format(_T("%04d%02d%02d%02d%02d"), yy, mm, dd, hh, mn);
}
