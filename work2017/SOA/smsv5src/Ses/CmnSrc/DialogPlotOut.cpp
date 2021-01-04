// DlgPlOut.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogPlotOut.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPlotOut ダイアログ

CDialogPlotOut::CDialogPlotOut(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPlotOut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPlotOut)
	m_fHomeStop = FALSE;
	m_fLineOut = FALSE;
	m_fOmitOut = FALSE;
	m_csCmbData = _T("");
	m_csCmbFile = _T("");
	//}}AFX_DATA_INIT
}


void CDialogPlotOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPlotOut)
	DDX_Control(pDX, IDC_CMBFILE, m_cmbFile);
	DDX_Control(pDX, IDC_CMBDATA, m_cmbData);
	DDX_Check(pDX, IDC_CHK_HORS, m_fHomeStop);
	DDX_Check(pDX, IDC_CHK_LORC, m_fLineOut);
	DDX_Check(pDX, IDC_CHK_OORN, m_fOmitOut);
	DDX_CBString(pDX, IDC_CMBDATA, m_csCmbData);
	DDX_CBString(pDX, IDC_CMBFILE, m_csCmbFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPlotOut, CDialog)
	//{{AFX_MSG_MAP(CDialogPlotOut)
	ON_CBN_SELCHANGE(IDC_CMBDATA, OnSelchangeCmbdata)
	ON_CBN_SELCHANGE(IDC_CMBFILE, OnSelchangeCmbfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPlotOut メッセージ ハンドラ

BOOL CDialogPlotOut::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitFileCmb();
	InitDataCmb();

	UpdateData(FALSE);

	return TRUE;
}

void CDialogPlotOut::InitFileCmb()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	filename[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	name[RECMAX];
	TCHAR	file[RECMAX];

	senvgetexepath(exepath);
	_stprintf_s(filename, _T("%s\\amadfile.lst"), exepath);
	_tfopen_s(&fp, filename, _T("r"));
	if(fp == NULL){
		m_cmbFile.ShowWindow(SW_HIDE);
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		_stscanf_s(buf, _T("%s %s"), name, RECMAX, file, RECMAX);
		m_cmbFile.AddString(name);
	}
	fclose(fp);
	m_cmbFile.SetCurSel(m_nFileNo);
	m_cmbFile.GetLBText(m_nFileNo, file);
	m_csCmbFile = file;
}

void CDialogPlotOut::InitDataCmb()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	filename[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	name[RECMAX];
	TCHAR	data[RECMAX];

	senvgetexepath(exepath);
	_stprintf_s(filename, _T("%s\\amaddata.lst"), exepath);
	_tfopen_s(&fp, filename, _T("r"));
	if(fp == NULL){
		m_cmbData.ShowWindow(SW_HIDE);
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		_stscanf_s(buf, _T("%s %s"), name, RECMAX, data, RECMAX);
		m_cmbData.AddString(name);
	}
	fclose(fp);
	m_cmbData.SetCurSel(m_nDataNo);
	m_cmbData.GetLBText(m_nDataNo, data);
	m_csCmbData = data;
}

void CDialogPlotOut::OnSelchangeCmbdata() 
{
	UpdateData(TRUE);
	m_nDataNo = m_cmbData.GetCurSel();
}

void CDialogPlotOut::OnSelchangeCmbfile() 
{
	UpdateData(TRUE);
	m_nFileNo = m_cmbFile.GetCurSel();
}
