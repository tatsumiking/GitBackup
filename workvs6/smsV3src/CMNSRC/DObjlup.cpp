// DObjLup.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DObjLup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MODAL		0
#define	MODALESS	1

#define	BMPXSIZE	80
#define BMPYSIZE	80
/////////////////////////////////////////////////////////////////////////////
// CDialogObjLineup ダイアログ

CDialogObjLineup::CDialogObjLineup(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogObjLineup::IDD, pParent)
{
	m_nMode = MODAL;
	//{{AFX_DATA_INIT(CDialogObjLineup)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDialogObjLineup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogObjLineup)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogObjLineup, CDialog)
	//{{AFX_MSG_MAP(CDialogObjLineup)
	ON_BN_CLICKED(IDC_MODE1, OnMode1)
	ON_BN_CLICKED(IDC_MODE2, OnMode2)
	ON_BN_CLICKED(IDC_MODE3, OnMode3)
	ON_BN_CLICKED(IDC_MODE4, OnMode4)
	ON_BN_CLICKED(IDC_MODE5, OnMode5)
	ON_BN_CLICKED(IDC_MODE6, OnMode6)
	ON_BN_CLICKED(IDC_MODE7, OnMode7)
	ON_BN_CLICKED(IDC_MODE8, OnMode8)
	ON_BN_CLICKED(IDC_MODE9, OnMode9)
	ON_BN_CLICKED(IDC_MODE10, OnMode10)
	ON_BN_CLICKED(IDC_MODE11, OnMode11)
	ON_BN_CLICKED(IDC_MODE12, OnMode12)
	ON_BN_CLICKED(IDC_MODE13, OnMode13)
	ON_BN_CLICKED(IDC_MODE14, OnMode14)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogObjLineup メッセージ ハンドラ
void CDialogObjLineup::SetModaless()
{
	m_nMode = MODALESS;
}

void CDialogObjLineup::SetModal()
{
	m_nMode = MODAL;
}

void CDialogObjLineup::OnMode1() 
{
	m_nSlctMode = LINEUPUP;
	EndProcess();
	CDialog::OnOK();
}

void CDialogObjLineup::OnMode2() 
{
	m_nSlctMode = LINEUPUDCNTR;
	CDialog::OnOK();
}

void CDialogObjLineup::OnMode3() 
{
	m_nSlctMode = LINEUPDOWN;
	CDialog::OnOK();
}

void CDialogObjLineup::OnMode4() 
{
	m_nSlctMode = LINEUPLEFT;
	CDialog::OnOK();
}

void CDialogObjLineup::OnMode5() 
{
	m_nSlctMode = LINEUPLRCNTR;
	CDialog::OnOK();
}

void CDialogObjLineup::OnMode6() 
{
	m_nSlctMode = LINEUPRIGHT;
	CDialog::OnOK();
}

void CDialogObjLineup::OnMode7() 
{
	m_nSlctMode = LINEUPXCNTR;
	CDialog::OnOK();
}

void CDialogObjLineup::OnMode8() 
{
	m_nSlctMode = LINEUPYCNTR;
	CDialog::OnOK();
}

void CDialogObjLineup::OnMode9() 
{
	m_nSlctMode = LINEUPXYCNTR;
	CDialog::OnOK();
}

void CDialogObjLineup::OnMode10() 
{
	m_nSlctMode = LINEUPXSIZEEQUL;
	CDialog::OnOK();
}

void CDialogObjLineup::OnMode11() 
{
	m_nSlctMode = LINEUPYSIZEEQUL;
	CDialog::OnOK();
}

void CDialogObjLineup::OnMode12() 
{
	m_nSlctMode = LINEUPXYSIZEEQUL;
	CDialog::OnOK();
}

void CDialogObjLineup::OnMode13() 
{
	m_nSlctMode = LINEUPXSPEQUL;
	CDialog::OnOK();
}

void CDialogObjLineup::OnMode14() 
{
	m_nSlctMode = LINEUPYSPEQUL;
}

void CDialogObjLineup::EndProcess()
{
	if(m_nMode == MODAL){
		CDialog::OnOK();
	}
	else{
		switch(m_nSlctMode){
		case LINEUPUP:
			break;
		case LINEUPUDCNTR:
		case LINEUPDOWN:
		case LINEUPLEFT:
		case LINEUPLRCNTR:
		case LINEUPRIGHT:
		case LINEUPXSIZEEQUL:
		case LINEUPYSIZEEQUL:
		case LINEUPXYSIZEEQUL:
		case LINEUPXCNTR:
		case LINEUPYCNTR:
		case LINEUPXYCNTR:
		case LINEUPXSPEQUL:
		case LINEUPYSPEQUL:
			break;
		}
	}
}
