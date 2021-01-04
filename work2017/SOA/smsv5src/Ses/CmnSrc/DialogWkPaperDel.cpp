// DWkPpDel.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogWkPaperDel.h"


//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Wakuenv.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWkPaperDel ダイアログ

CDialogWkPaperDel::CDialogWkPaperDel(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWkPaperDel::IDD, pParent)
{
	m_pcView = (CTonView*)pParent;
	//{{AFX_DATA_INIT(CDialogWkPaperDel)
	m_csPaperPath = _T("");
	m_csPaperName = _T("");
	//}}AFX_DATA_INIT
}


void CDialogWkPaperDel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWkPaperDel)
	DDX_Text(pDX, IDC_PATH, m_csPaperPath);
	DDX_CBString(pDX, IDC_PAPERNAME, m_csPaperName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogWkPaperDel, CDialog)
	//{{AFX_MSG_MAP(CDialogWkPaperDel)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_CBN_SELCHANGE(IDC_PAPERNAME, OnSelchangePapername)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWkPaperDel メッセージ ハンドラ

BOOL CDialogWkPaperDel::OnInitDialog() 
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

void CDialogWkPaperDel::CBInitPaperName()
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

void CDialogWkPaperDel::OnSelchangePapername() 
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

void CDialogWkPaperDel::OnDelete() 
{
	TCHAR	papername[RECMAX];
	TCHAR	buf[RECMAX];
	int		ret;

	UpdateData(TRUE);
	_tcscpy_s(papername, m_csPaperName.GetBuffer(RECMAX));
	_stprintf_s(buf, _T("[%s]を削除してよろしいですか\n削除を行うと枠種類[%s]のレイアウトはすべて削除されます"), papername, papername);
	ret = MessageBox(buf, _T("枠種類削除"), MB_OKCANCEL);
	if(ret == IDOK){
		wakuenvdelpaper(papername);
	}
	CDialog::OnOK();
}

