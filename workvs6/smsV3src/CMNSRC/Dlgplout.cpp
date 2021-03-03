// DlgPlOut.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgPlOut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPlotOut ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
}

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
	char	exepath[FNAMEMAX];
	char	filename[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	name[RECMAX];
	char	file[RECMAX];

	senvgetexepath(exepath);
	sprintf(filename, "%s\\amadfile.lst", exepath);
	if((fp = fopen(filename, "r")) == NULL){
		m_cmbFile.ShowWindow(SW_HIDE);
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%s %s", name, file);
		m_cmbFile.AddString(name);
	}
	fclose(fp);
	m_cmbFile.SetCurSel(m_nFileNo);
	m_cmbFile.GetLBText(m_nFileNo, file);
	m_csCmbFile = _T(file);
}

void CDialogPlotOut::InitDataCmb()
{
	char	exepath[FNAMEMAX];
	char	filename[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	name[RECMAX];
	char	data[RECMAX];

	senvgetexepath(exepath);
	sprintf(filename, "%s\\amaddata.lst", exepath);
	if((fp = fopen(filename, "r")) == NULL){
		m_cmbData.ShowWindow(SW_HIDE);
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%s %s", name, data);
		m_cmbData.AddString(name);
	}
	fclose(fp);
	m_cmbData.SetCurSel(m_nDataNo);
	m_cmbData.GetLBText(m_nDataNo, data);
	m_csCmbData = _T(data);
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
