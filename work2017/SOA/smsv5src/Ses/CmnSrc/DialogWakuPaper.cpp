// DlgWkPpr.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogWakuPaper.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuPaper ダイアログ


CDialogWakuPaper::CDialogWakuPaper(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWakuPaper::IDD, pParent)
{
	m_pcView = (CTonView*)pParent;
	//{{AFX_DATA_INIT(CDialogWakuPaper)
	m_dPaperHight = 0.0;
	m_dPaperSizeX = 0.0;
	m_dPaperSizeY = 0.0;
	m_dPaperSX = 0.0;
	m_dPaperSY = 0.0;
	m_dPaperWidth = 0.0;
	m_csPaperName = _T("");
	//}}AFX_DATA_INIT
}


void CDialogWakuPaper::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuPaper)
	DDX_Text(pDX, IDC_PAPERHIGHT, m_dPaperHight);
	DDX_Text(pDX, IDC_PAPERSIZEX, m_dPaperSizeX);
	DDX_Text(pDX, IDC_PAPERSIZEY, m_dPaperSizeY);
	DDX_Text(pDX, IDC_PAPERSX, m_dPaperSX);
	DDX_Text(pDX, IDC_PAPERSY, m_dPaperSY);
	DDX_Text(pDX, IDC_PAPERWIDTH, m_dPaperWidth);
	DDX_CBString(pDX, IDC_PAPERNAME, m_csPaperName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogWakuPaper, CDialog)
	//{{AFX_MSG_MAP(CDialogWakuPaper)
	ON_CBN_SELCHANGE(IDC_PAPERNAME, OnSelchangePapername)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuPaper メッセージ ハンドラ

BOOL CDialogWakuPaper::OnInitDialog() 
{
	CRect	rect;
	int		sx, sy, sizex, sizey;
	DBL		mmx, mmy;

	CDialog::OnInitDialog();
	
	GetWindowRect(&rect);
	sx = rect.left;
	sy = rect.top;
	sizex = rect.Width();
	sizey = rect.Height();
	::SetWindowPos(m_hWnd, HWND_TOPMOST, sx, sy, sizex, sizey, 0);

	CBInitPaperName();
	scrngetmmsize(&mmx, &mmy);
	m_dPaperSizeX = mmx;
	m_dPaperSizeY = mmy;
	m_dPaperSX = 0;
	m_dPaperSY = 0;
	m_dPaperWidth = mmx;
	m_dPaperHight = mmy;

	UpdateData(FALSE);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogWakuPaper::CBInitPaperName()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	papername[QRECMAX];

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_PAPERNAME);
	pCB->ResetContent();
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\papersiz.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		_stscanf_s(buf, _T("%s"), papername, QRECMAX);
		if(papername[0] == _T('\0')){
			break;
		}
		pCB->AddString(papername);
	}
	fclose(fp);
}

void CDialogWakuPaper::OnSelchangePapername() 
{
	int		idx;
	TCHAR	pname[RECMAX];

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_PAPERNAME);
	idx = pCB->GetCurSel();
	pCB->GetLBText(idx, pname);
	UpdateData(TRUE);
	GetPaperElement(pname);
}

void CDialogWakuPaper::OnSave() 
{
	TCHAR		pname[FNAMEMAX];

	// TODO: この位置にその他の検証用のコードを追加してください
	UpdateData(TRUE);
	_tcscpy_s(pname, m_csPaperName.GetBuffer(RECMAX));
	SavePaperElement(pname); 
	CDialog::OnOK();
}

void CDialogWakuPaper::OnDelete() 
{
	TCHAR	pname[RECMAX];
	TCHAR	buf[RECMAX];
	int		ret;

	UpdateData(TRUE);
	_tcscpy_s(pname, m_csPaperName.GetBuffer(RECMAX));
	_stprintf_s(buf, _T("[%s]を削除してよろしいですか"), pname);
	ret = AfxMessageBox(buf, MB_OKCANCEL);
	if(ret == IDOK){
		DelPaperName(pname);
	}
	CDialog::OnOK();
}

void CDialogWakuPaper::GetPaperElement(LPTSTR srchname)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	papername[QRECMAX];
	TCHAR	buf[RECMAX];
	DBL		mmx, mmy;
	DBL		sx, sy;
	DBL		width, hight;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\papersiz.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		_stscanf_s(buf, _T("%s"), papername, QRECMAX);
		if(_tcscmp(papername, srchname) == 0){
			_stscanf_s(buf, _T("%s %lf %lf %lf %lf %lf %lf")
				, papername, QRECMAX, &mmx, &mmy, &sx, &sy, &width, &hight);
			m_dPaperSizeX = mmx;
			m_dPaperSizeY = mmy;
			m_dPaperSX = sx;
			m_dPaperSY = sy;
			m_dPaperWidth = width;
			m_dPaperHight = hight;
			break;
		}
	}
	fclose(fp);
}

void CDialogWakuPaper::SavePaperElement(LPTSTR srchname)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	sfname[FNAMEMAX];
	TCHAR	dfname[FNAMEMAX];
	FILE	*sfp;
	FILE	*dfp;
	TCHAR	papername[QRECMAX];
	TCHAR	buf[RECMAX];
	int		flag;

	BackupPaperName();
	flag = OFF;
	senvgetexepath(exepath);
	_stprintf_s(sfname, _T("%s\\papersiz.bak"), exepath);
	_stprintf_s(dfname, _T("%s\\papersiz.env"), exepath);
	_tfopen_s(&dfp, dfname, _T("w"));
	if(dfp == NULL){
		return;
	}
	_tfopen_s(&sfp, sfname, _T("r"));
	if(sfp == NULL){
		_stprintf_s(buf, _T("%s %4.1lf %4.1lf %4.1lf %4.1lf %4.1lf %4.1lf\n"),
			srchname, m_dPaperSizeX, m_dPaperSizeY, m_dPaperSX, m_dPaperSY, m_dPaperWidth, m_dPaperHight);
		baselibFputs(buf, dfp);
		fclose(dfp);
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, sfp) == NULL){
			break;
		}
		_stscanf_s(buf, _T("%s"), papername, QRECMAX);
		if(_tcscmp(papername, srchname) == 0){
			_stprintf_s(buf, _T("%s %4.1lf %4.1lf %4.1lf %4.1lf %4.1lf %4.1lf\n"),
				srchname, m_dPaperSizeX, m_dPaperSizeY, m_dPaperSX, m_dPaperSY, m_dPaperWidth, m_dPaperHight);
			baselibFputs(buf, dfp);
			flag = ON;
		}
		else{
			_stprintf_s(buf, _T("%s"), buf);
			baselibFputs(buf, dfp);
		}
		
	}
	if(flag == OFF){
		_stprintf_s(buf, _T("%s %4.1lf %4.1lf %4.1lf %4.1lf %4.1lf %4.1lf\n"),
			srchname, m_dPaperSizeX, m_dPaperSizeY, m_dPaperSX, m_dPaperSY, m_dPaperWidth, m_dPaperHight);
		baselibFputs(buf, dfp);
	}
	fclose(sfp);
	fclose(dfp);
}

void CDialogWakuPaper::DelPaperName(LPTSTR srchname)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	sfname[FNAMEMAX];
	TCHAR	dfname[FNAMEMAX];
	FILE	*sfp;
	FILE	*dfp;
	TCHAR	papername[QRECMAX];
	TCHAR	buf[RECMAX];

	BackupPaperName();
	senvgetexepath(exepath);
	_stprintf_s(sfname, _T("%s\\papersiz.bak"), exepath);
	_stprintf_s(dfname, _T("%s\\papersiz.env"), exepath);
	_tfopen_s(&dfp, dfname, _T("w"));
	if(dfp == NULL){
		return;
	}
	_tfopen_s(&sfp, sfname, _T("r"));
	if(sfp == NULL){
		fclose(dfp);
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, sfp) == NULL){
			break;
		}
		_stscanf_s(buf, _T("%s"), papername, QRECMAX);
		if(_tcscmp(papername, srchname) != 0){
			_stprintf_s(buf, _T("%s"), buf);
			baselibFputs(buf, dfp);
		}
		
	}
	fclose(sfp);
	fclose(dfp);
}

void CDialogWakuPaper::BackupPaperName()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	sfname[FNAMEMAX];
	TCHAR	dfname[FNAMEMAX];
	FILE	*sfp;
	FILE	*dfp;
	TCHAR	buf[RECMAX];

	senvgetexepath(exepath);
	_stprintf_s(sfname, _T("%s\\papersiz.env"), exepath);
	_stprintf_s(dfname, _T("%s\\papersiz.bak"), exepath);
	_tfopen_s(&dfp, dfname, _T("w"));
	if(dfp == NULL){
		return;
	}
	_tfopen_s(&sfp, sfname, _T("r"));
	if(sfp == NULL){
		fclose(dfp);
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, sfp) == NULL){
			break;
		}
		_stprintf_s(buf, _T("%s"), buf);
		baselibFputs(buf, dfp);
	}
	
	fclose(sfp);
	fclose(dfp);
}

