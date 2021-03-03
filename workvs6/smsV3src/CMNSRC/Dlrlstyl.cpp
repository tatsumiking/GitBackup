// DlRlStyl.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlRlStyl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogRuleStyle ダイアログ

extern "C" {
	void senvgetexepath(LPSTR path);
}

CDialogRuleStyle::CDialogRuleStyle(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogRuleStyle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogRuleStyle)
	m_csStyle = _T("");
	//}}AFX_DATA_INIT
	m_pcView = (CTonView*)pParent;
}

void CDialogRuleStyle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogRuleStyle)
	DDX_Control(pDX, IDC_CBSTYLE, m_cbStyle);
	DDX_CBString(pDX, IDC_CBSTYLE, m_csStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogRuleStyle, CDialog)
	//{{AFX_MSG_MAP(CDialogRuleStyle)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_CBN_SELCHANGE(IDC_CBSTYLE, OnSelchangeCbstyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogRuleStyle メッセージ ハンドラ

void CDialogRuleStyle::OnReset() 
{
	m_pcView->CommandExec(DLGRESETEVENT, 0, 0);
}

void CDialogRuleStyle::OnSelchangeCbstyle() 
{
	UpdateData(TRUE);
	m_nStyle = m_cbStyle.GetCurSel();
}

void CDialogRuleStyle::InitStyleList()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	int		no;
	long	ptn;
	int		type;
	char	name[RECMAX];

	m_cbStyle.AddString("実線");
	senvgetexepath(exepath);
	sprintf(fname, "%s\\ptnstyle.env", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		while(1){
			if(fgets(buf, RECMAX, fp) == NULL){
				break;
			}
			sscanf(buf, "%d %lx %d %s", &no, &ptn, &type, &name);
			m_cbStyle.AddString(name);
		}
		fclose(fp);
	}
	m_cbStyle.SetCurSel(0);
	m_nStyle = 0;
}

void CDialogRuleStyle::GetItemElement()
{

}

BOOL CDialogRuleStyle::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitStyleList();
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
