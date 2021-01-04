// DlgWkGrp.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <direct.h>
#include "DialogWakuGrup.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Wakuenv.h"
//}
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuGrup ダイアログ


CDialogWakuGrup::CDialogWakuGrup(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWakuGrup::IDD, pParent)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	int		len;

	//{{AFX_DATA_INIT(CDialogWakuGrup)
	m_csWakuGrupPath = _T("");
	m_csWakuGrupName = _T("");
	//}}AFX_DATA_INIT
	m_szGrupPath[0] = _T('\0');
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\gruppath.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		return;
	}
	baselibCrDelFgets(m_szGrupPath, FNAMEMAX, fp);
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
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	*sp;
	TCHAR	grupname[QRECMAX];
	TCHAR	gruppath[QRECMAX];

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_WKGRUPNAME);
	if(m_szGrupPath[0] == _T('\0')){
		senvgetexepath(exepath);
		_stprintf_s(fname, _T("%s\\wakugrup.lst"), exepath);
	}
	else{
		_stprintf_s(fname, _T("%s\\wakugrup.lst"), m_szGrupPath);
	}
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, grupname);
		sp = basecsvstrdivcpy(sp, gruppath);
		_stprintf_s(buf, _T("%s"), grupname);
		pCB->AddString(grupname);
	}
	fclose(fp);

	wakuenvgetgrupname(grupname);
	m_csWakuGrupName = grupname;
	senvgetwakpath(gruppath);
	m_csWakuGrupPath = gruppath;
}

void CDialogWakuGrup::OnOK() 
{
	TCHAR	exepath[FNAMEMAX];
	BOOL	setflag;
	TCHAR	grupname[QRECMAX];
	TCHAR	gruppath[FNAMEMAX];
	TCHAR	tgrupname[QRECMAX];
	TCHAR	tgruppath[FNAMEMAX];
	TCHAR	srcfname[FNAMEMAX];
	TCHAR	dstfname[FNAMEMAX];
	TCHAR	buf[RECMAX];
	TCHAR	*sp;
	FILE	*sfp, *dfp;
	int		len;
	// TODO: この位置にその他の検証用のコードを追加してください
	
	setflag = FALSE;
	UpdateData(TRUE);

	_stprintf_s(grupname, _T("%s"), m_csWakuGrupName.GetBuffer(RECMAX));
	_stprintf_s(gruppath, _T("%s"), m_csWakuGrupPath.GetBuffer(RECMAX));

	CheckAddWakuGrup(gruppath);
	
	if(m_szGrupPath[0] == _T('\0')){
		senvgetexepath(exepath);
		_stprintf_s(srcfname, _T("%s\\wakugrup.lst"), exepath);
		_stprintf_s(dstfname, _T("%s\\wakugrup.bak"), exepath);
	}
	else{
		_stprintf_s(srcfname, _T("%s\\wakugrup.lst"), m_szGrupPath);
		_stprintf_s(dstfname, _T("%s\\wakugrup.bak"), m_szGrupPath);
	}
	CopyFile(srcfname, dstfname, FALSE);
	_tfopen_s(&sfp, dstfname, _T("r"));
	_tfopen_s(&dfp, srcfname, _T("w"));
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, sfp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, tgrupname);
		sp = basecsvstrdivcpy(sp, tgruppath);
		if(_tcscmp(tgrupname, grupname) == 0){
			_tcscpy_s(gruppath, gruppath);
			setflag = TRUE;
		}
		_stprintf_s(buf, _T("\"%s\", \"%s\"\n"), tgrupname, tgruppath);
		baselibFputs(buf, dfp);
	}
	
	if(setflag == FALSE){
		_stprintf_s(buf, _T("\"%s\", \"%s\"\n"), grupname, gruppath);
		baselibFputs(buf, dfp);
	}
	
	fclose(sfp);
	fclose(dfp);
	
	len = _tcslen(_T("\\\\"));
	if(gruppath[1] != _T(':') && _tcsnccmp(gruppath, _T("\\\\"), len) != 0){
		if(m_szGrupPath[0] == _T('\0')){
			senvgetexepath(exepath);
			_stprintf_s(tgruppath, _T("%s\\%s"), exepath, gruppath);
		}
		else{
			_stprintf_s(tgruppath, _T("%s\\%s"), m_szGrupPath, gruppath);
		}
		_tcscpy_s(gruppath, tgruppath);
	}
	wakuenvsetgrupname(grupname);
	senvsetwakpath(gruppath);

	CDialog::OnOK();
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
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	*sp;
	TCHAR	grupname[QRECMAX];
	TCHAR	tgrupname[QRECMAX];
	TCHAR	tgruppath[QRECMAX];

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_WKGRUPNAME);
	idx = pCB->GetCurSel();
	pCB->GetLBText(idx, grupname);
	if(m_szGrupPath[0] == _T('\0')){
		senvgetexepath(exepath);
		_stprintf_s(fname, _T("%s\\wakugrup.lst"), exepath);
	}
	else{
		_stprintf_s(fname, _T("%s\\wakugrup.lst"), m_szGrupPath);
	}
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		return;
	}

	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, tgrupname);
		sp = basecsvstrdivcpy(sp, tgruppath);
		if(_tcscmp(tgrupname, grupname) == 0){
			m_csWakuGrupName = tgrupname;
			m_csWakuGrupPath = tgruppath;
			break;
		}
	}
	fclose(fp);
		
	UpdateData(FALSE);
}

void CDialogWakuGrup::CheckAddWakuGrup(LPTSTR gruppath)
{
	FILE	*fp;
	TCHAR	fname[RECMAX];

	_stprintf_s(fname, _T("%s\\paperlst.lst"), gruppath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		CreateDirectory(gruppath, NULL);
		_tfopen_s(&fp, fname, _T("w"));
		if(fp != NULL){
			fclose(fp);
		}
	}
	else{
		fclose(fp);
	}

}
