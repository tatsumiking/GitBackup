// DlgFChng.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogFontChange.h"

//extern "C" {
#include "../../ObjLib/Mojienv.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogFontChange ダイアログ

CDialogFontChange::CDialogFontChange(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogFontChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogFontChange)
	m_csFontName = _T("");
	//}}AFX_DATA_INIT
	m_pcView = (CTonView*)pParent;
}

void CDialogFontChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogFontChange)
	DDX_CBString(pDX, IDC_COMBO1, m_csFontName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogFontChange, CDialog)
	//{{AFX_MSG_MAP(CDialogFontChange)
	ON_BN_CLICKED(IDC_DLGRESET, OnDlgreset)
	ON_BN_CLICKED(IDC_DLGSET, OnDlgset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogFontChange メッセージ ハンドラ
void CDialogFontChange::FontCBInit()
{
	int		idx;
	int		fonttype;
	TCHAR	fontname[FNAMEMAX];
	TCHAR	accessname[FNAMEMAX];

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_COMBO1);
	idx = 0;
	mojienvflistopen(_T("rb"));
	mojienvgetfontname(fontname);
	pCB->SetWindowText(fontname);
	while(1){
		mojienvgetflist(idx, &fonttype, fontname, accessname);
		if(fontname[0] == _T('\0') && accessname[0] == _T('\0')){
			break;
		}
		pCB->AddString(fontname);
		idx++;
	}
	mojienvflistclose();
}

BOOL CDialogFontChange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	FontCBInit();
	
	return TRUE;
}

void CDialogFontChange::OnDlgreset() 
{
	m_pcView->CommandExec(DLGRESETEVENT, 0, 0);
}

void CDialogFontChange::OnDlgset() 
{
	TCHAR	fontname[RECMAX];

	UpdateData(TRUE);
	_tcscpy_s(fontname, m_csFontName.GetBuffer(RECMAX));
	mojienvsetfontname(fontname);
	m_pcView->CommandExec(DLGSETEVENT, 0, 0);
}
