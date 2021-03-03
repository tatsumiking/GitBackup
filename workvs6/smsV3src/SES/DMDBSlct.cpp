// DMDBSlct.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DMDBSlct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
	int basegetparentpath(LPSTR lpPath);
}

/////////////////////////////////////////////////////////////////////////////
// CDialogMDBSelect ダイアログ

CDialogMDBSelect::CDialogMDBSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMDBSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMDBSelect)
	m_nTBLorQRY = 0;
	//}}AFX_DATA_INIT
}

void CDialogMDBSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMDBSelect)
	DDX_Control(pDX, IDC_LSTNAME, m_lstName);
	DDX_Radio(pDX, IDC_RDTABLE, m_nTBLorQRY);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogMDBSelect, CDialog)
	//{{AFX_MSG_MAP(CDialogMDBSelect)
	ON_BN_CLICKED(IDC_RDTABLE, OnRdtable)
	ON_BN_CLICKED(IDC_RDQUERY, OnRdquery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMDBSelect メッセージ ハンドラ

void CDialogMDBSelect::OnRdtable() 
{
	UpdateData(TRUE);
	InitNameList();
}

void CDialogMDBSelect::OnRdquery() 
{
	UpdateData(TRUE);
	InitNameList();
}

void CDialogMDBSelect::OnOK() 
{
	int		idx;

	UpdateData(TRUE);
	idx = m_lstName.GetCurSel();
	if(idx == -1){
		::AfxMessageBox("接続先が指定されていません", MB_OK);
		return;
	}
	m_lstName.GetText(idx, m_csTableName);
	CDialog::OnOK();
}

BOOL CDialogMDBSelect::OnInitDialog() 
{
	char	szDBFname[FNAMEMAX];
	char	szTitle[RECMAX];

	CDialog::OnInitDialog();
	strcpy(szDBFname, m_csDBFname.GetBuffer(FNAMEMAX));
	int idx = basegetparentpath(szDBFname);
	strcpy(szDBFname, m_csDBFname.GetBuffer(FNAMEMAX));
	strcpy(szTitle, &(szDBFname[idx+1]));
	SetWindowText(szTitle);
	InitNameList();
	return TRUE;
}

void CDialogMDBSelect::InitNameList()
{
	COleVariant		var;
	int		max, idx;
	CDaoTableDefInfo	dtdiDef;
	CDaoQueryDefInfo	dqdiDef;

	try{
		m_pcDaoDatabase = new CDaoDatabase();
		m_pcDaoDatabase->Open(m_csDBFname);
	}
	catch (CDaoException* e1){
		e1->Delete();
		return;
	}

	try{
		m_lstName.ResetContent();
		if(m_nTBLorQRY == 0){
			max = m_pcDaoDatabase->GetTableDefCount();
			for(idx = 0; idx < max; idx++){
				m_pcDaoDatabase->GetTableDefInfo(idx, dtdiDef, AFX_DAO_PRIMARY_INFO);
				if((dtdiDef.m_lAttributes & 0x80000000) == 0
				&& (dtdiDef.m_lAttributes & 0x00000002) == 0){
					m_lstName.AddString(dtdiDef.m_strName);
				}
			}
		}else{
			max = m_pcDaoDatabase->GetQueryDefCount();
			for(idx = 0; idx < max; idx++){
				m_pcDaoDatabase->GetQueryDefInfo(idx, dqdiDef, AFX_DAO_PRIMARY_INFO);
				if(dqdiDef.m_strName.GetAt(0) != '~'){
					m_lstName.AddString(dqdiDef.m_strName);
				}
			}
		}
	}
	catch (CDaoException* e2){
		e2->Delete();
	}

	try{
		m_pcDaoDatabase->Close();
		delete(m_pcDaoDatabase);
	}
	catch (CDaoException* e3){
		e3->Delete();
	}
	m_lstName.SetCurSel(0);

}
