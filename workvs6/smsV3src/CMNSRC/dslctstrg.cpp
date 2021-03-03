// DSlctStrg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DSlctStrg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSlctStrg ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
}

CDialogSlctStrg::CDialogSlctStrg(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSlctStrg::IDD, pParent)
{
	m_pcView = (CTonView*)pParent;
	//{{AFX_DATA_INIT(CDialogSlctStrg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDialogSlctStrg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSlctStrg)
	DDX_Control(pDX, IDC_LSTSTRG1, m_lstStrg1);
	DDX_Control(pDX, IDC_LSTSTRG2, m_lstStrg2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSlctStrg, CDialog)
	//{{AFX_MSG_MAP(CDialogSlctStrg)
	ON_LBN_DBLCLK(IDC_LSTSTRG1, OnDblclkLststrg1)
	ON_LBN_DBLCLK(IDC_LSTSTRG2, OnDblclkLststrg2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSlctStrg メッセージ ハンドラ

BOOL CDialogSlctStrg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	LoadStrg1List();
	LoadStrg2List();
	return TRUE;
}

void CDialogSlctStrg::LoadStrg1List()
{
	char	exepath[FNAMEMAX];
	char	szFileName[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];

	m_lstStrg1.ResetContent();
	senvgetexepath(exepath);
	sprintf(szFileName, "%s\\SlctStrg1.txt", exepath);
	fp = fopen(szFileName, "r");
	if(fp == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		buf[strlen(buf)-1] = '\0';
		m_lstStrg1.AddString(buf);
	}
	fclose(fp);
}

void CDialogSlctStrg::LoadStrg2List()
{
	char	exepath[FNAMEMAX];
	char	szFileName[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];

	m_lstStrg2.ResetContent();
	senvgetexepath(exepath);
	sprintf(szFileName, "%s\\SlctStrg2.txt", exepath);
	fp = fopen(szFileName, "r");
	if(fp == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		buf[strlen(buf)-1] = '\0';
		m_lstStrg2.AddString(buf);
	}
	fclose(fp);

}

void CDialogSlctStrg::OnDblclkLststrg1() 
{
	int		idx;
	char	szStr[RECMAX];

	idx = m_lstStrg1.GetCurSel();
	m_lstStrg1.GetText(idx, szStr);
	m_pcView->SetString(szStr);
}

void CDialogSlctStrg::OnDblclkLststrg2() 
{
	int		idx;
	char	szStr[RECMAX];

	idx = m_lstStrg2.GetCurSel();
	m_lstStrg2.GetText(idx, szStr);
	m_pcView->SetString(szStr);
}
