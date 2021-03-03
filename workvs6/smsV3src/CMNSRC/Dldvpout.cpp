// DialogDivPlotOut.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlDvPOut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDivPlotOut ダイアログ


CDialogDivPlotOut::CDialogDivPlotOut(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDivPlotOut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDivPlotOut)
	m_chkAreaBoxs = FALSE;
	m_chkStopChk = FALSE;
	m_dNorisiro = 0.0;
	m_dPlotXSize = 0.0;
	m_dPlotYSize = 0.0;
	//}}AFX_DATA_INIT
	m_pcView = (CTonView*)pParent;
}


void CDialogDivPlotOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDivPlotOut)
	DDX_Check(pDX, IDC_CHKAREABOXS, m_chkAreaBoxs);
	DDX_Check(pDX, IDC_CHKSTOPCHK, m_chkStopChk);
	DDX_Text(pDX, IDC_EDNORISIRO, m_dNorisiro);
	DDX_Text(pDX, IDC_EDPLTXSIZE, m_dPlotXSize);
	DDX_Text(pDX, IDC_EDPLTYSIZE, m_dPlotYSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogDivPlotOut, CDialog)
	//{{AFX_MSG_MAP(CDialogDivPlotOut)
	ON_BN_CLICKED(ID_DLGRESET, OnDlgreset)
	ON_BN_CLICKED(ID_DLGSET, OnDlgset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDivPlotOut メッセージ ハンドラ

void CDialogDivPlotOut::OnDlgreset() 
{
	UpdateData(TRUE);
	m_pcView->CommandExec(DLGRESETEVENT, 0, 0);
}

void CDialogDivPlotOut::OnDlgset() 
{
	UpdateData(TRUE);
	m_pcView->CommandExec(DLGSETEVENT, 0, 0);
}

void CDialogDivPlotOut::OnCancel() 
{
	UpdateData(TRUE);
	m_pcView->CommandExec(ENDEVENT, 0, 0);
}
