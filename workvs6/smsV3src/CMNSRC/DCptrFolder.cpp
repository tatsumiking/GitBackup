// DCptrFolder.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DCptrFolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogCptrFolder ダイアログ
extern "C" {
	int basegetparentpath(LPSTR lpPath);
	void wakuenvsetpapername(LPSTR name);
	void wakuenvgetpaperelement();
	void wakuenvgetfullpaperpath(LPSTR paperpath);
	void wakuenvgetpaperarea(LPDBL paperwidth, LPDBL paperhight);
}

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
	char	paperpath[FNAMEMAX];
	DBL		xsize, ysize;

	wakuenvgetfullpaperpath(paperpath);
	wakuenvgetpaperarea(&xsize, &ysize);
	m_csSrcFolder = _T(paperpath);
	m_csDstFolder = _T(paperpath);
	m_csSrcXSize.Format("%1.1lf", xsize);
	m_csSrcYSize.Format("%1.1lf", ysize);
	m_csDstXSize.Format("%1.1lf", xsize);
	m_csDstYSize.Format("%1.1lf", ysize);
	UpdateData(FALSE);
}

void CDialogCptrFolder::OnDeltaposSpnsrcxsize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = atof(m_csSrcXSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csSrcXSize.Format("%1.1lf", num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogCptrFolder::OnDeltaposSpnsrcysize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = atof(m_csSrcYSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csSrcYSize.Format("%1.1lf", num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogCptrFolder::OnDeltaposSpndstxsize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = atof(m_csDstXSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csDstXSize.Format("%1.1lf", num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogCptrFolder::OnDeltaposSpndstysize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = atof(m_csDstYSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csDstYSize.Format("%1.1lf", num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogCptrFolder::OnBtndstfolder() 
{
	char	szPath[FNAMEMAX];

	UpdateData(TRUE);
	strcpy(szPath, m_csDstFolder.GetBuffer(FNAMEMAX));
	GetSelectDir("コピー先フォルダー位置設定", szPath);
	if(szPath[0] != '\0'){
		m_csDstFolder = _T(szPath);
	}
	UpdateData(FALSE);
}

void CDialogCptrFolder::OnBtnsrcfolder() 
{
	char	szPath[FNAMEMAX];
	char	szParentPath[FNAMEMAX];
	char	szPaperName[FNAMEMAX];

	UpdateData(TRUE);
	strcpy(szPath, m_csSrcFolder.GetBuffer(FNAMEMAX));
	GetSelectDir("コピー元フォルダー位置設定", szPath);
	if(szPath[0] != '\0'){
		m_csSrcFolder = _T(szPath);
		strcpy(szParentPath, szPath);
		basegetparentpath(szParentPath);
		strcpy(szPaperName, &(szPath[strlen(szParentPath)+1]));
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
	
	dSrcXSize = atof(m_csSrcXSize);
	dSrcYSize = atof(m_csSrcYSize);
	dDstXSize = atof(m_csDstXSize);
	dDstYSize = atof(m_csDstYSize);
	if(dSrcXSize == 0 || dSrcYSize == 0){
		m_dXTime = 1.0;
		m_dYTime = 1.0;
	}else if(dDstXSize == 0 || dDstYSize == 0){
		::AfxMessageBox("サイズを正しく入力してください", MB_OK);
		return;
	}else{
		m_dXTime = dDstXSize / dSrcXSize;
		m_dYTime = dDstYSize / dSrcYSize;
	}
	if(m_csSrcFolder == m_csDstFolder){
		::AfxMessageBox("同じ場所にコピーできません", MB_OK);
		return;
	}
	CDialog::OnOK();
}

void CDialogCptrFolder::GetSelectDir(LPSTR lpTitle, LPSTR lpPath)
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

