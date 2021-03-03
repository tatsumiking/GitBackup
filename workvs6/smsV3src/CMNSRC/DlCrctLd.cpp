// DlCrctLd.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlCrctLd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogCrctLoad ダイアログ
extern "C" {
	int basedivcsvdata(LPSTR csvsp[], LPSTR csvbuf);
	void senvgetexepath(LPSTR path);
}

CDialogCrctLoad::CDialogCrctLoad(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogCrctLoad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogCrctLoad)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDialogCrctLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogCrctLoad)
	DDX_Control(pDX, IDC_LST_CRCT, m_lstCrct);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogCrctLoad, CDialog)
	//{{AFX_MSG_MAP(CDialogCrctLoad)
	ON_LBN_SELCHANGE(IDC_LST_CRCT, OnSelchangeLstCrct)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogCrctLoad メッセージ ハンドラ

BOOL CDialogCrctLoad::OnInitDialog() 
{
	char	exepath[FNAMEMAX];
	char	crctfname[FNAMEMAX];
	FILE	*crctfp;
	char	csvbuf[1024];
	LPSTR	csvsp[10];
	char	buf[1024];

	CDialog::OnInitDialog();
	senvgetexepath(exepath);
	sprintf(crctfname, "%s\\outcrect.dat", exepath);
	if((crctfp = fopen(crctfname, "r+")) == NULL){
		return TRUE;
	}
	m_lstCrct.SetHorizontalExtent(10000);
	while(1){
		if(fgets(csvbuf, 1024, crctfp) == NULL){
			break;
		}
		basedivcsvdata(csvsp, csvbuf);
		sprintf(buf, "%s %s", csvsp[2], csvsp[5]);
		m_lstCrct.AddString(buf);
	}
	m_nSlctIdx = -1;
	return TRUE;
}

void CDialogCrctLoad::OnSelchangeLstCrct() 
{
	m_nSlctIdx = m_lstCrct.GetCurSel();
}
