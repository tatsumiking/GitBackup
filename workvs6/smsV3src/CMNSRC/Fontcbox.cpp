// FontCBox.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "FontCBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
	void senvgetexepath(LPSTR path);
	void mojienvgetfontname(LPSTR fontname);
	void mojienvsetfontname(LPSTR fontname);
	void mojienvgetflist(int idx, LPINT type, LPSTR sfname, LPSTR ffname);
}

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

void CFontCBox::LimitText(int nMaxChar)
{
	CComboBox::LimitText(nMaxChar);
}

void CFontCBox::InitItemElement()
{
	SetFontList();
}

void CFontCBox::SetItemElement()
{
	char	fontname[FNAMEMAX];

	mojienvgetfontname(fontname);
	SetWindowText(fontname);
}

void CFontCBox::GetItemElement()
{
	CString	cfontname;
	char	fontname[FNAMEMAX];

	UpdateData(TRUE);
	GetWindowText(cfontname);
	strcpy(fontname, LPCTSTR(cfontname));
	mojienvsetfontname(fontname);
}

void CFontCBox::SetFontList()
{
	int		idx;
	int		fonttype;
	char	fontname[FNAMEMAX];
	char	accessname[FNAMEMAX];

	idx = 0;
	mojienvgetfontname(fontname);
	SetWindowText(fontname);
	while(1){
		mojienvgetflist(idx, &fonttype, fontname, accessname);
		if(fontname[0] == '\0' && accessname[0] == '\0'){
			break;
		}
		AddString(fontname);
		idx++;
	}
}

int CFontCBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: この位置に固有の作成用コードを追加してください

	return 0;
}
