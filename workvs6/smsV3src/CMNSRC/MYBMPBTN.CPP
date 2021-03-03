// MyBmpBtn.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "..\cmnsrc\MyBmpBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyBitmapButton

CMyBitmapButton::CMyBitmapButton()
{
	m_nCheck = 0;
}

CMyBitmapButton::~CMyBitmapButton()
{
}


BEGIN_MESSAGE_MAP(CMyBitmapButton, CBitmapButton)
	//{{AFX_MSG_MAP(CMyBitmapButton)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyBitmapButton メッセージ ハンドラ

void CMyBitmapButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if(m_nCheck == 0){
		lpDrawItemStruct->itemState = ODS_DEFAULT;
	}
	else if(m_nCheck == 1){
		lpDrawItemStruct->itemState = ODS_SELECTED;
	}
	else{
		lpDrawItemStruct->itemState = ODS_DISABLED;
	}
	CBitmapButton::DrawItem(lpDrawItemStruct);
}

void CMyBitmapButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CBitmapButton::OnLButtonDown(nFlags, point);
}

void CMyBitmapButton::ChangeCheck()
{
	if(m_nCheck == -1){
		m_nCheck = 0;
	}
	else if(m_nCheck == 1){
		m_nCheck = 0;
	}
	else{
		m_nCheck = 1;
	}
	Invalidate(TRUE);
}

void CMyBitmapButton::NonsetCheck()
{
	m_nCheck = -1;
}

void CMyBitmapButton::OffCheck()
{
	m_nCheck = 0;
}

void CMyBitmapButton::OnCheck()
{
	m_nCheck = 1;
}

int CMyBitmapButton::CheckCheck()
{
	return(m_nCheck);
}
