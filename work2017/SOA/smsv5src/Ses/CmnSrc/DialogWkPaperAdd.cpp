// DWkPpAdd.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogWkPaperAdd.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Wakuenv.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWkPaperAdd ダイアログ

CDialogWkPaperAdd::CDialogWkPaperAdd(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWkPaperAdd::IDD, pParent)
{
	m_pcView = (CTonView*)pParent;
	//{{AFX_DATA_INIT(CDialogWkPaperAdd)
	m_csPaperPath = _T("");
	m_csPaperName = _T("");
	//}}AFX_DATA_INIT
}

void CDialogWkPaperAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWkPaperAdd)
	DDX_Text(pDX, IDC_PATH, m_csPaperPath);
	DDX_CBString(pDX, IDC_PAPERNAME, m_csPaperName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogWkPaperAdd, CDialog)
	//{{AFX_MSG_MAP(CDialogWkPaperAdd)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_CBN_SELCHANGE(IDC_PAPERNAME, OnSelchangePapername)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWkPaperAdd メッセージ ハンドラ

BOOL CDialogWkPaperAdd::OnInitDialog() 
{
	CRect	rect;
	int		sx, sy, sizex, sizey;
	TCHAR	papername[RECMAX];
	TCHAR	paperpath[RECMAX];

	CDialog::OnInitDialog();

	GetWindowRect(&rect);
	sx = rect.left;
	sy = rect.top;
	sizex = rect.Width();
	sizey = rect.Height();
	::SetWindowPos(m_hWnd, HWND_TOPMOST, sx, sy, sizex, sizey, 0);

	CBInitPaperName();
	wakuenvgetpaperelement();
	wakuenvgetpapername(papername);
	m_csPaperName = papername;
	wakuenvgetfullpaperpath(paperpath);
	m_csPaperPath = paperpath;
	UpdateData(FALSE);
	
	return TRUE;
}

void CDialogWkPaperAdd::CBInitPaperName()
{
	TCHAR	wakpath[FNAMEMAX];
	TCHAR	paperlistfname[FNAMEMAX];
	TCHAR	buf[RECMAX];
	TCHAR	*sp;
	TCHAR	pname[HRECMAX];
	FILE	*fp;

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_PAPERNAME);
	senvgetwakpath(wakpath);
	_stprintf_s(paperlistfname, _T("%s\\paperlst.lst"), wakpath);
	_tfopen_s(&fp, paperlistfname, _T("r"));
	if(fp == NULL){
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, pname);
		pCB->AddString(pname);
	}
	fclose(fp);
}

void CDialogWkPaperAdd::OnSelchangePapername() 
{
	TCHAR	buf[RECMAX];
	int		idx;
	TCHAR	pname[RECMAX];
	TCHAR	paperpath[RECMAX];

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_PAPERNAME);
	idx = pCB->GetCurSel();
	pCB->GetLBText(idx, pname);
	UpdateData(TRUE);
	wakuenvsetpapername(pname);
	wakuenvgetpaperelement();
	wakuenvgetpapername(buf);
	m_csPaperName = buf;
	wakuenvgetfullpaperpath(paperpath);
	m_csPaperPath = paperpath;
	UpdateData(FALSE);
}

void CDialogWkPaperAdd::OnSave() 
{
	TCHAR		paperpath[FNAMEMAX];
	TCHAR		printenvfname[FNAMEMAX];

	UpdateData(TRUE);
	if(CheckFileName(m_csPaperName.GetBuffer(RECMAX)) == FALSE){
		::AfxMessageBox(_T("型名には下記の文字は使えません。\n\t\\/:,;*?\"<> |"), MB_OK, 0);
		return;
	}
	wakuenvsetpapername(m_csPaperName.GetBuffer(RECMAX));
	_tcscpy_s(paperpath, m_csPaperName.GetBuffer(RECMAX));
	wakuenvsetpaperpath(paperpath);
	wakuenvsetpaperelement();

	wakuenvgetfullpaperpath(paperpath);
	_stprintf_s(printenvfname, _T("%s\\printenv.bin"), paperpath);
	m_pcView->PrintEnvSave(printenvfname);

	wakuenvend();
	CDialog::OnOK();
}

BOOL CDialogWkPaperAdd::CheckFileName(LPTSTR lpFileName)
{
	int		hc;
	TCHAR	badTCHAR[16];
	BOOL	flag;
	int		i;
	int		cnt;
	LPTSTR	sp;

	_tcscpy_s(badTCHAR, _T("\\/:,;*?\"<> |"));
	cnt = _tcslen(badTCHAR);

	flag = TRUE;
	sp = lpFileName;
	while(1){
		if((*sp) == _T('\0')){
			break;
		}
#ifdef UNICODE
		for (i = 0; i < cnt; i++) {
			if (*sp == badTCHAR[i]) {
				flag = FALSE;
			}
		}
		sp++;
#else
		hc = (*sp) & 0x00ff;
		if (baseiskanji(hc) == ON) {
			sp += 2;
		}
		else {
			for (i = 0; i < cnt; i++) {
				if (*sp == badTCHAR[i]) {
					flag = FALSE;
				}
			}
			sp++;
		}
#endif
	}
	return(flag);
}
