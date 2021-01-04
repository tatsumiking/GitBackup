// MyFileDl.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MyFileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFileDialog

IMPLEMENT_DYNAMIC(CMyFileDialog, CFileDialog)

CMyFileDialog::CMyFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	m_ofn.Flags |= OFN_HIDEREADONLY;
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

void CMyFileDialog::SetTitle(LPTSTR lpStr)
{
	m_ofn.lpstrTitle = lpStr;
}

void CMyFileDialog::SetInitPath(LPTSTR lpStr)
{
	SetCurrentDirectory(lpStr);
	m_ofn.lpstrInitialDir = lpStr;
}

void CMyFileDialog::SetBtnText(LPTSTR msg)
{
	m_lpBtnMsg = msg;
}

void CMyFileDialog::OnFileNameChange()
{
	CString	csPath;

	csPath = GetPathName();

}
