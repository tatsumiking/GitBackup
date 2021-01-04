// MyEdit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyEdit

CMyEdit::CMyEdit()
{
	m_pcMyDialog = NULL;
	m_pcMyDialogBar = NULL;
	m_nIMEMode = ON;
	m_nFirstFlag = ON;
	//IMEOn();
}

CMyEdit::~CMyEdit()
{
	if(m_nIMEMode == ON){
		if(m_nFirstFlag	== OFF){
			IMEOff();
		}
	}
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit メッセージ ハンドラ

void CMyEdit::OnKeyUp(UINT nTCHAR, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch(nTCHAR){
	case VK_RETURN:
		if(m_pcMyDialog != NULL){
			m_pcMyDialog->KeyIn(nTCHAR, m_nID);
		}
		if(m_pcMyDialogBar != NULL){
			m_pcMyDialogBar->KeyIn(nTCHAR, m_nID);
		}
		break;
	}
	CEdit::OnKeyUp(nTCHAR, nRepCnt, nFlags);
}

void CMyEdit::OnKeyDown(UINT nTCHAR, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch(nTCHAR){
 	case VK_UP:
		if(m_pcMyDialog != NULL){
			m_pcMyDialog->KeyIn(nTCHAR, m_nID);
		}
		if(m_pcMyDialogBar != NULL){
			m_pcMyDialogBar->KeyIn(nTCHAR, m_nID);
		}
		break;
	case VK_DOWN:
		if(m_pcMyDialog != NULL){
			m_pcMyDialog->KeyIn(nTCHAR, m_nID);
		}
		if(m_pcMyDialogBar != NULL){
			m_pcMyDialogBar->KeyIn(nTCHAR, m_nID);
		}
		break;
	}
	CEdit::OnKeyDown(nTCHAR, nRepCnt, nFlags);
}

BOOL CMyEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_pcMyDialog = (CMyDialog *)pParentWnd;
	m_nID = nID;
	return CEdit::Create(dwStyle, rect, pParentWnd, nID);
}

void CMyEdit::SetIMEMode(int flag) 
{
	m_nIMEMode = flag;
}

void CMyEdit::SetItemID(UINT nID) 
{
	m_nID = nID;
}

void CMyEdit::SetMyDialog(CMyDialog* pDlg) 
{
	m_pcMyDialog = (CMyDialog *)pDlg;
}

void CMyEdit::SetMyDialogBar(CMyDialogBar* pDlgBar) 
{
	m_pcMyDialogBar = (CMyDialogBar *)pDlgBar;
}

void CMyEdit::IMEOn()
{
	COMPOSITIONFORM cf;

	if(m_nFirstFlag	== ON){
		m_hImc = ::ImmCreateContext();
		m_hOldImc = ::ImmAssociateContext(m_hWnd, m_hImc);
		::ImmSetOpenStatus(m_hImc, true);	
		m_nFirstFlag = OFF;
	}
	::ImmGetCompositionWindow(m_hImc, &cf);
	cf.dwStyle = CFS_POINT;
	cf.ptCurrentPos = GetCaretPos();
	::ImmSetCompositionWindow(m_hImc, &cf);
}

void CMyEdit::IMEOff()
{
	::ImmAssociateContext(m_hWnd, m_hOldImc);
	::ImmDestroyContext(m_hImc);
}

void CMyEdit::OnSetFocus(CWnd* pOldWnd) 
{
	if(m_pcMyDialog != NULL){
		m_pcMyDialog->SelectEdit(m_nID);
	}
	if(m_pcMyDialogBar != NULL){
		m_pcMyDialogBar->SelectEdit(m_nID);
	}
	CEdit::OnSetFocus(pOldWnd);
	if(m_nIMEMode == ON){
		IMEOn();
	}
}

void CMyEdit::OnKillFocus(CWnd* pNewWnd) 
{
	if(m_nIMEMode == ON){
		//IMEOff();
	}
	CEdit::OnKillFocus(pNewWnd);
}

