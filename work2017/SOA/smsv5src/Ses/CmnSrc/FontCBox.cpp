// FontCBox.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "FontCBox.h"

//extern "C" {
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Mojienv.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFontCBox


BEGIN_MESSAGE_MAP(CFontCBox, CComboBox)
	//{{AFX_MSG_MAP(CFontCBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontCBox メッセージ ハンドラ

CFontCBox::CFontCBox()
{
}

void CFontCBox::LimitText(int nMaxTCHAR)
{
	CComboBox::LimitText(nMaxTCHAR);
}

void CFontCBox::InitItemElement()
{
	SetFontList();
}

void CFontCBox::SetItemElement()
{
	TCHAR	fontname[FNAMEMAX];

	mojienvgetfontname(fontname);
	SetWindowText(fontname);
}

void CFontCBox::GetItemElement()
{
	CString	cfontname;
	TCHAR	fontname[FNAMEMAX];

	UpdateData(TRUE);
	GetWindowText(cfontname);
	_tcscpy_s(fontname, LPCTSTR(cfontname));
	mojienvsetfontname(fontname);
}

void CFontCBox::SetFontList()
{
	int		idx;
	int		fonttype;
	TCHAR	fontname[FNAMEMAX];
	TCHAR	accessname[FNAMEMAX];

	idx = 0;
	mojienvflistopen(_T("rb"));
	mojienvgetfontname(fontname);
	SetWindowText(fontname);
	while(1){
		mojienvgetflist(idx, &fonttype, fontname, accessname);
		if(fontname[0] == _T('\0') && accessname[0] == _T('\0')){
			break;
		}
		AddString(fontname);
		idx++;
	}
	mojienvflistclose();
}

int CFontCBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: この位置に固有の作成用コードを追加してください

	return 0;
}
