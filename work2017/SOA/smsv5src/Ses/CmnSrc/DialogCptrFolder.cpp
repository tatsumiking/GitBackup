// DCptrFolder.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogCptrFolder.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Wakuenv.h"
//}
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogCptrFolder ダイアログ

int CALLBACK CDialogCptrFolder::BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if((BFFM_INITIALIZED == uMsg) && lpData){
		::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
	}
	return 0;
}

CDialogCptrFolder::CDialogCptrFolder(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogCptrFolder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogCptrFolder)
	m_csSrcFolder = _T("");
	m_csDstFolder = _T("");
	m_csSrcXSize = _T("");
	m_csSrcYSize = _T("");
	m_csDstXSize = _T("");
	m_csDstYSize = _T("");
	//}}AFX_DATA_INIT
}


void CDialogCptrFolder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogCptrFolder)
	DDX_Text(pDX, IDC_EDSRCFOLDER, m_csSrcFolder);
	DDX_Text(pDX, IDC_EDDSTFOLDER, m_csDstFolder);
	DDX_Text(pDX, IDC_EDSRCXSIZE, m_csSrcXSize);
	DDX_Text(pDX, IDC_EDSRCYSIZE, m_csSrcYSize);
	DDX_Text(pDX, IDC_EDDSTXSIZE, m_csDstXSize);
	DDX_Text(pDX, IDC_EDDSTYSIZE, m_csDstYSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogCptrFolder, CDialog)
	//{{AFX_MSG_MAP(CDialogCptrFolder)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPNSRCXSIZE, OnDeltaposSpnsrcxsize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPNSRCYSIZE, OnDeltaposSpnsrcysize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPNDSTXSIZE, OnDeltaposSpndstxsize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPNDSTYSIZE, OnDeltaposSpndstysize)
	ON_BN_CLICKED(IDC_BTNDSTFOLDER, OnBtndstfolder)
	ON_BN_CLICKED(IDC_BTNSRCFOLDER, OnBtnsrcfolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogCptrFolder メッセージ ハンドラ

BOOL CDialogCptrFolder::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitElement();
	return TRUE;
}

void CDialogCptrFolder::InitElement()
{
	TCHAR	paperpath[FNAMEMAX];
	DBL		xsize, ysize;

	wakuenvgetfullpaperpath(paperpath);
	wakuenvgetpaperarea(&xsize, &ysize);
	m_csSrcFolder = paperpath;
	m_csDstFolder = paperpath;
	m_csSrcXSize.Format(_T("%1.1lf"), xsize);
	m_csSrcYSize.Format(_T("%1.1lf"), ysize);
	m_csDstXSize.Format(_T("%1.1lf"), xsize);
	m_csDstYSize.Format(_T("%1.1lf"), ysize);
	UpdateData(FALSE);
}

void CDialogCptrFolder::OnDeltaposSpnsrcxsize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = _ttof(m_csSrcXSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csSrcXSize.Format(_T("%1.1lf"), num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogCptrFolder::OnDeltaposSpnsrcysize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = _ttof(m_csSrcYSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csSrcYSize.Format(_T("%1.1lf"), num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogCptrFolder::OnDeltaposSpndstxsize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = _ttof(m_csDstXSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csDstXSize.Format(_T("%1.1lf"), num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogCptrFolder::OnDeltaposSpndstysize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = _ttof(m_csDstYSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csDstYSize.Format(_T("%1.1lf"), num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogCptrFolder::OnBtndstfolder() 
{
	TCHAR	szPath[FNAMEMAX];

	UpdateData(TRUE);
	_tcscpy_s(szPath, m_csDstFolder.GetBuffer(FNAMEMAX));
	GetSelectDir(_T("コピー先フォルダー位置設定"), szPath);
	if(szPath[0] != _T('\0')){
		m_csDstFolder = szPath;
	}
	UpdateData(FALSE);
}

void CDialogCptrFolder::OnBtnsrcfolder() 
{
	TCHAR	szPath[FNAMEMAX];
	TCHAR	szParentPath[FNAMEMAX];
	TCHAR	szPaperName[FNAMEMAX];

	UpdateData(TRUE);
	_tcscpy_s(szPath, m_csSrcFolder.GetBuffer(FNAMEMAX));
	GetSelectDir(_T("コピー元フォルダー位置設定"), szPath);
	if(szPath[0] != _T('\0')){
		m_csSrcFolder = szPath;
		_tcscpy_s(szParentPath, szPath);
		basegetparentpath(szParentPath);
		_tcscpy_s(szPaperName, &(szPath[_tcslen(szParentPath)+1]));
		wakuenvsetpapername(szPaperName);
		wakuenvgetpaperelement();
		InitElement();
	}
	UpdateData(FALSE);
}

void CDialogCptrFolder::OnOK() 
{
	DBL		dSrcXSize, dSrcYSize;
	DBL		dDstXSize, dDstYSize;

	UpdateData(TRUE);
	
	dSrcXSize = _ttof(m_csSrcXSize);
	dSrcYSize = _ttof(m_csSrcYSize);
	dDstXSize = _ttof(m_csDstXSize);
	dDstYSize = _ttof(m_csDstYSize);
	if(dSrcXSize == 0 || dSrcYSize == 0){
		m_dXTime = 1.0;
		m_dYTime = 1.0;
	}else if(dDstXSize == 0 || dDstYSize == 0){
		::AfxMessageBox(_T("サイズを正しく入力してください"), MB_OK);
		return;
	}else{
		m_dXTime = dDstXSize / dSrcXSize;
		m_dYTime = dDstYSize / dSrcYSize;
	}
	if(m_csSrcFolder == m_csDstFolder){
		::AfxMessageBox(_T("同じ場所にコピーできません"), MB_OK);
		return;
	}
	CDialog::OnOK();
}

void CDialogCptrFolder::GetSelectDir(LPTSTR lpTitle, LPTSTR lpPath)
{
	BROWSEINFO	bInfo;
	LPITEMIDLIST	lpFoldPointer;

	memset(&bInfo, 0, sizeof(BROWSEINFO));
	bInfo.hwndOwner = this->m_hWnd;
	bInfo.pidlRoot = 0;
	bInfo.lpszTitle = lpTitle;
	bInfo.ulFlags = 1;
	bInfo.lpfn   = BrowseCallbackProc; // コールバーク関数
	bInfo.lParam = (LPARAM)(lpPath);       // 初期フォルダ
	lpFoldPointer = SHBrowseForFolder(&bInfo);
	SHGetPathFromIDList(lpFoldPointer, lpPath);
}

