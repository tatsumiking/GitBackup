// DlgMjLd.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgMjLd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
	void mojienvgetfontname(LPSTR fontname);
	void mojienvsetfontname(LPSTR fontname);
	void mojienvgetflist(int idx, LPINT type, LPSTR sfname, LPSTR ffname);
}

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiLoad ダイアログ


CDialogMojiLoad::CDialogMojiLoad(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMojiLoad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMojiLoad)
	m_csFontName = _T("");
	m_csMojiCode = _T("");
	//}}AFX_DATA_INIT
	m_lpTitle = NULL;
}


void CDialogMojiLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMojiLoad)
	DDX_CBString(pDX, IDC_COMBO1, m_csFontName);
	DDX_Text(pDX, IDC_EDIT1, m_csMojiCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMojiLoad, CDialog)
	//{{AFX_MSG_MAP(CDialogMojiLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiLoad メッセージ ハンドラ

void CDialogMojiLoad::FontCBInit()
{
	int		idx;
	int		fonttype;
	char	fontname[FNAMEMAX];
	char	accessname[FNAMEMAX];

	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_COMBO1);
	idx = 0;
	while(1){
		mojienvgetflist(idx, &fonttype, fontname, accessname);
		if(fontname[0] == '\0' && accessname[0] == '\0'){
			break;
		}
		pcCB->AddString(fontname);
		idx++;
	}
	mojienvgetfontname(fontname);
	m_csFontName = _T(fontname);
	idx = pcCB->FindString(-1, fontname);
	if(idx == -1){
		strcpy(fontname, "翠舟楷書体");
		idx = pcCB->FindString(-1, fontname);
		if(idx == -1){
			idx = 0;
			pcCB->GetLBText(idx, fontname);
		}
		mojienvsetfontname(fontname);
		m_csFontName = _T(fontname);
	}
	pcCB->SetCurSel(idx);
}

void CDialogMojiLoad::SetDialogTitle(LPSTR lpTitle)
{
	m_lpTitle = lpTitle;
}

BOOL CDialogMojiLoad::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(m_lpTitle != NULL){
		SetWindowText(m_lpTitle);
	}
	FontCBInit();
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
