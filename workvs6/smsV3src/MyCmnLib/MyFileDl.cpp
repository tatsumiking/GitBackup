// MyFileDl.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MyFileDl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFileDialog

IMPLEMENT_DYNAMIC(CMyFileDialog, CFileDialog)

CMyFileDialog::CMyFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	m_lpBtnMsg = NULL;
}


BEGIN_MESSAGE_MAP(CMyFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(CMyFileDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMyFileDialog::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	if(m_lpBtnMsg != NULL){
		SetControlText(IDOK, m_lpBtnMsg);
	}
	return TRUE;
}

void CMyFileDialog::SetTitle(LPSTR lpstr)
{
	m_ofn.lpstrTitle = lpstr;
}

void CMyFileDialog::SetCrtPath(LPSTR lppath)
{
	m_ofn.lpstrInitialDir = lppath;
}

void CMyFileDialog::SetBtnText(LPSTR msg)
{
	m_lpBtnMsg = msg;
}
