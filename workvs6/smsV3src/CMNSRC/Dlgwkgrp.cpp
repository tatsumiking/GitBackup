// DlgWkGrp.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <direct.h>
#include "DlgWkGrp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
	void senvgetexepath(LPSTR path);
	void senvgetwakpath(LPSTR gruppath);
	void senvsetwakpath(LPSTR gruppath);
	LPSTR basecsvstrdivcpy(LPSTR sp, LPSTR retstr);
	void wakuenvgetgrupname(LPSTR grupname);
	void wakuenvsetgrupname(LPSTR grupname);
}
/////////////////////////////////////////////////////////////////////////////
// CDialogWakuGrup ダイアログ


CDialogWakuGrup::CDialogWakuGrup(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWakuGrup::IDD, pParent)
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	int		len;

	//{{AFX_DATA_INIT(CDialogWakuGrup)
	m_csWakuGrupPath = _T("");
	m_csWakuGrupName = _T("");
	//}}AFX_DATA_INIT
	m_szGrupPath[0] = '\0';
	senvgetexepath(exepath);
	sprintf(fname, "%s\\gruppath.env", exepath);
	fp = fopen(fname, "r");
	if(fp == NULL){
		return;
	}
	fgets(m_szGrupPath, FNAMEMAX, fp);
	len = strlen(m_szGrupPath);
	m_szGrupPath[len-1] = '\0';	// CR delete
	fclose(fp);
}

void CDialogWakuGrup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuGrup)
	DDX_Text(pDX, IDC_EDIT2, m_csWakuGrupPath);
	DDX_CBString(pDX, IDC_WKGRUPNAME, m_csWakuGrupName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogWakuGrup, CDialog)
	//{{AFX_MSG_MAP(CDialogWakuGrup)
	ON_CBN_SELCHANGE(IDC_WKGRUPNAME, OnSelchangeWkgrupname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuGrup メッセージ ハンドラ

void CDialogWakuGrup::InitWakuGrupCombo()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	*sp;
	char	grupname[QRECMAX];
	char	gruppath[QRECMAX];

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_WKGRUPNAME);
	if(m_szGrupPath[0] == '\0'){
		senvgetexepath(exepath);
		sprintf(fname, "%s\\wakugrup.lst", exepath);
	}
	else{
		sprintf(fname, "%s\\wakugrup.lst", m_szGrupPath);
	}
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	while(1){
		if(CrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, grupname);
		sp = basecsvstrdivcpy(sp, gruppath);
		sprintf(buf, "%s", grupname);
		pCB->AddString(grupname);
	}
	fclose(fp);

	wakuenvgetgrupname(grupname);
	m_csWakuGrupName = _T(grupname);
	senvgetwakpath(gruppath);
	m_csWakuGrupPath = _T(gruppath);
}

void CDialogWakuGrup::OnOK() 
{
	char	exepath[FNAMEMAX];
	BOOL	setflag;
	char	grupname[QRECMAX];
	char	gruppath[FNAMEMAX];
	char	tgrupname[QRECMAX];
	char	tgruppath[FNAMEMAX];
	char	srcfname[FNAMEMAX];
	char	dstfname[FNAMEMAX];
	char	buf[RECMAX];
	char	*sp;
	FILE	*sfp, *dfp;
	// TODO: この位置にその他の検証用のコードを追加してください
	
	setflag = FALSE;
	UpdateData(TRUE);

	sprintf(grupname, "%s", m_csWakuGrupName.GetBuffer(RECMAX));
	sprintf(gruppath, "%s", m_csWakuGrupPath.GetBuffer(RECMAX));

	CheckAddWakuGrup(gruppath);
	
	if(m_szGrupPath[0] == '\0'){
		senvgetexepath(exepath);
		sprintf(srcfname, "%s\\wakugrup.lst", exepath);
		sprintf(dstfname, "%s\\wakugrup.bak", exepath);
	}
	else{
		sprintf(srcfname, "%s\\wakugrup.lst", m_szGrupPath);
		sprintf(dstfname, "%s\\wakugrup.bak", m_szGrupPath);
	}
	CopyFile(srcfname, dstfname, FALSE);
	sfp = fopen(dstfname, "r");
	dfp = fopen(srcfname, "w");
	while(1){
		if(CrDelFgets(buf, RECMAX, sfp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, tgrupname);
		sp = basecsvstrdivcpy(sp, tgruppath);
		if(strcmp(tgrupname, grupname) == 0){
			strcpy(gruppath, gruppath);
			setflag = TRUE;
		}
		fprintf(dfp, "\"%s\", \"%s\"\n", tgrupname, tgruppath);
	}
	if(setflag == FALSE){
		fprintf(dfp, "\"%s\", \"%s\"\n", grupname, gruppath);
	}
	fclose(sfp);
	fclose(dfp);
	
	if(gruppath[1] != ':' && strncmp(gruppath, "\\\\", 2) != 0){
		if(m_szGrupPath[0] == '\0'){
			senvgetexepath(exepath);
			sprintf(tgruppath, "%s\\%s", exepath, gruppath);
		}
		else{
			sprintf(tgruppath, "%s\\%s", m_szGrupPath, gruppath);
		}
		strcpy(gruppath, tgruppath);
	}
	wakuenvsetgrupname(grupname);
	senvsetwakpath(gruppath);

	CDialog::OnOK();
}

char* CDialogWakuGrup::CrDelFgets(char * buf, int max, FILE * fp)
{
	char	*ret;
	int		len;

	ret = fgets(buf, max, fp);
	if(ret != NULL){
		len = strlen(buf);
		if(buf[len-1] == '\n'){
			buf[len-1] = '\0';
		}
	}
	return(ret);
}

BOOL CDialogWakuGrup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	InitWakuGrupCombo();

	UpdateData(FALSE);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogWakuGrup::OnSelchangeWkgrupname() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int		idx;
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	*sp;
	char	grupname[QRECMAX];
	char	tgrupname[QRECMAX];
	char	tgruppath[QRECMAX];

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_WKGRUPNAME);
	idx = pCB->GetCurSel();
	pCB->GetLBText(idx, grupname);
	if(m_szGrupPath[0] == '\0'){
		senvgetexepath(exepath);
		sprintf(fname, "%s\\wakugrup.lst", exepath);
	}
	else{
		sprintf(fname, "%s\\wakugrup.lst", m_szGrupPath);
	}
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}

	while(1){
		if(CrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, tgrupname);
		sp = basecsvstrdivcpy(sp, tgruppath);
		if(strcmp(tgrupname, grupname) == 0){
			m_csWakuGrupName = _T(tgrupname);
			m_csWakuGrupPath = _T(tgruppath);
			break;
		}
	}
	fclose(fp);
		
	UpdateData(FALSE);
}

void CDialogWakuGrup::CheckAddWakuGrup(LPSTR gruppath)
{
	FILE	*fp;
	char	fname[RECMAX];

	sprintf(fname, "%s\\paperlst.lst", gruppath);
	if((fp = fopen(fname, "r")) == NULL){
		CreateDirectory(gruppath, NULL);
		if((fp = fopen(fname, "w")) != NULL){
			fclose(fp);
		}
	}
	else{
		fclose(fp);
	}

}
