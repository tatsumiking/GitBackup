// MyStatic.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MyStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyStatic

CMyStatic::CMyStatic()
{
	m_nFlag = OFF;
}

CMyStatic::~CMyStatic()
{
}


BEGIN_MESSAGE_MAP(CMyStatic, CStatic)
	//{{AFX_MSG_MAP(CMyStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyStatic メッセージ ハンドラ

void CMyStatic::OnPaint() 
{
	CPaintDC dc(this);
	int		size;
	CRect	rect;
	CBrush	*pcBrush;
	char	szStr[QRECMAX];
	int		len;

	if(m_nFlag == ON){
		GetWindowRect(&rect);
		size = rect.Width();
		rect.left = 0; rect.right = size;
		size = rect.Height();
		rect.top = 0; rect.bottom = size;
		pcBrush = new CBrush(GetSysColor(COLOR_HIGHLIGHT));
		rect.left += 1; rect.right -= 1;
		rect.top += 1; rect.bottom -= 1;
		dc.FillRect(&rect, pcBrush);
		delete(pcBrush);
		dc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	GetWindowRect(&rect);
	size = rect.Width();
	rect.left = 0; rect.right = size;
	size = rect.Height();
	rect.top = 0; rect.bottom = size;
	GetWindowText(szStr, QRECMAX);
	len = strlen(szStr);
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(szStr, len, &rect, DT_CENTER);
	dc.Draw3dRect(&rect,  GetSysColor(COLOR_3DDKSHADOW), GetSysColor(COLOR_3DHILIGHT));
}

void CMyStatic::SetFlag(int flag)
{
	m_nFlag = flag;
}

int CMyStatic::CheckFlag()
{
	return(m_nFlag);
}
