// TitlFDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "TitlFDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitlFileDialog

IMPLEMENT_DYNAMIC(CTitlFileDialog, CFileDialog)

CTitlFileDialog::CTitlFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	//{{AFX_DATA_INIT(CTitlFileDialog)
	//}}AFX_DATA_INIT
	m_ofn.Flags |= OFN_ENABLETEMPLATE | OFN_HIDEREADONLY;
	m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_TITL_FILE);
	m_ofn.lpstrTitle = "";
}

void CTitlFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTitlFileDialog)
	DDX_Text(pDX, IDC_EDCOMENT1, m_csComent1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTitlFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(CTitlFileDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CTitlFileDialog::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	
	return TRUE;
}

void CTitlFileDialog::OnFileNameChange()
{
	CString	csPath;
	FILE	*fp;
	char	fname[FNAMEMAX];

	strcpy(fname, GetPathName().GetBuffer(FNAMEMAX));
	if((fp = fopen(fname, "rb")) != NULL){
		fclose(fp);
		FileTitlLoad(fname);
	}
}

void CTitlFileDialog::FileTitlLoad(LPSTR fname)
{
	FILE	*fp;
	char	buf[RECMAX];

	if((fp = fopen(fname, "rb")) != NULL){
		fgets(buf, RECMAX, fp);
		if(strncmp(buf, "REM", 3) == 0){
			m_csComent1 = _T(&(buf[4]));
			UpdateData(FALSE);
		}
		fclose(fp);
	}
}

void CTitlFileDialog::OnPaint() 
{
	CPaintDC dc(this);
}

void CTitlFileDialog::SetTitle(LPSTR szTitle)
{
	m_ofn.lpstrTitle = szTitle;
}
