// StrgTool.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "StrgTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
	void mojienvgettategakiflag(LPINT tateflg);
	void mojienvgetspacekind(LPINT spkflg);
	void mojienvgetsize(LPDBL dw, LPDBL dh);
	void mojienvgetspace(LPDBL dmsp, LPDBL dlsp);
	void mojienvgetstrlen(LPDBL dlen);
	void mojienvsettategakiflag(int tateflg);
	void mojienvsetspacekind(int spkflg);
	void mojienvsetsize(DBL dw, DBL dh);
	void mojienvsetspace(DBL dmsp, DBL dlsp);
	void mojienvsetstrlen(DBL dlen);
}

/////////////////////////////////////////////////////////////////////////////
// CStrgTool

CStrgTool::CStrgTool()
{
}

CStrgTool::~CStrgTool()
{
}


BEGIN_MESSAGE_MAP(CStrgTool, CToolBar)
	//{{AFX_MSG_MAP(CStrgTool)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStrgTool メッセージ ハンドラ

int CStrgTool::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1){
		return -1;
	}

	return 0;
}

void CStrgTool::OnDestroy() 
{
	CToolBar::OnDestroy();
}

BOOL CStrgTool::SetToolItem(CSize textsize)
{
	int		sx, sy, ex, ey;
	int		width;
	
	sx = 0; sy = 0; ex = LF_FACESIZE*textsize.cx; ey = 200;
	CRect rect(sx,sy,ex,ey);
	if (!m_cFontCBox.Create(COMBOSTYLE,	rect, this, ID_STRG_FONT))
	{
		TRACE0("Failed to create combo-box\n");
		return FALSE;
	}
	width = (ex - sx);
	SetButtonInfo(0, ID_STRG_FONT, TBBS_SEPARATOR, width);

	sx = ex+SEPARATORDOT; sy = 0; ex = sx + textsize.cx*2; ey = 24;
	rect.SetRect(sx, sy, ex, ey);
	width = (ex - sx);
	SetButtonInfo(2, ID_STRG_TATEYOKO, TBBS_BUTTON, 0);

	sx = ex+SEPARATORDOT; ex = sx + textsize.cx*8;
	rect.SetRect(sx, sy, ex, ey);
	if (!m_cEditFontSize.Create(EDITSTYLE, rect, this, ID_STRG_MOJISIZE))
	{
		TRACE0("Failed to create fontsize spincntrl\n");
		return FALSE;
	}
	if (!m_cSpinFontSize.Create(SPINSTYLE, rect, this, IDC_SPFONTSIZE))
	{
		TRACE0("Failed to create fontsize spincntrl\n");
		return FALSE;
	}
	width = (ex - sx);
	SetButtonInfo(3, ID_STRG_MOJISIZE, TBBS_SEPARATOR, width);
	//SetButtonInfo(3, ID_SPFONTSIZE, TBBS_SEPARATOR, width);

	sx = ex; ex = sx + textsize.cx*6;
	rect.SetRect(sx, sy, ex, ey);
	if (!m_cEditFlatPar.Create(EDITSTYLE, rect, this, ID_STRG_MOJIFLATPAR))
	{
		TRACE0("Failed to create flatpar spincntrl\n");
		return FALSE;
	}
	if (!m_cSpinFlatPar.Create(SPINSTYLE, rect, this, IDC_SPFLATPAR))
	{
		TRACE0("Failed to create flatpar spincntrl\n");
		return FALSE;
	}
	width = (ex - sx);
	SetButtonInfo(4, ID_STRG_MOJIFLATPAR, TBBS_SEPARATOR, width);
	//SetButtonInfo(4, ID_SPFLATPAR, TBBS_SEPARATOR, width);

	sx = ex+SEPARATORDOT; ex = sx + textsize.cx*2;
	rect.SetRect(sx, sy, ex, ey);
	width = (ex - sx);
	SetButtonInfo(6, ID_STRG_SPACEKIND, TBBS_BUTTON, 2);

	sx = ex+8; ex = sx + textsize.cx*8;
	rect.SetRect(sx, sy, ex, ey);
	if (!m_cEditMojiSpace.Create(EDITSTYLE, rect, this, ID_STRG_MOJISPACE))
	{
		TRACE0("Failed to create fontsize spincntrl\n");
		return FALSE;
	}
	if (!m_cSpinMojiSpace.Create(SPINSTYLE, rect, this, IDC_SPMOJISPACE))
	{
		TRACE0("Failed to create fontsize spincntrl\n");
		return FALSE;
	}
	width = (ex - sx);
	SetButtonInfo(7, ID_STRG_MOJISPACE, TBBS_SEPARATOR, width);
	//SetButtonInfo(7, ID_SPMOJISPACE, TBBS_SEPARATOR, width);

	sx = ex; ex = sx + textsize.cx*8;
	rect.SetRect(sx, sy, ex, ey);
	if (!m_cEditStrgSpace.Create(EDITSTYLE, rect, this, ID_STRG_STRGSPACE))
	{
		TRACE0("Failed to create fontsize spincntrl\n");
		return FALSE;
	}
	if (!m_cSpinStrgSpace.Create(SPINSTYLE, rect, this, IDC_SPSTRGSPACE))
	{
		TRACE0("Failed to create fontsize spincntrl\n");
		return FALSE;
	}
	width = (ex - sx);
	SetButtonInfo(8, ID_STRG_STRGSPACE, TBBS_SEPARATOR, width);
	//SetButtonInfo(8, ID_SPSTRGSPACE, TBBS_SEPARATOR, width);

	sx = ex; ex = sx + textsize.cx*8;
	rect.SetRect(sx, sy, ex, ey);
	if (!m_cEditStrgSize.Create(EDITSTYLE, rect, this, ID_STRG_STRGSIZE))
	{
		TRACE0("Failed to create fontsize spincntrl\n");
		return FALSE;
	}
	if (!m_cSpinStrgSize.Create(SPINSTYLE, rect, this, IDC_SPSTRGSIZE))
	{
		TRACE0("Failed to create fontsize spincntrl\n");
		return FALSE;
	}
	width = (ex - sx);
	SetButtonInfo(9, ID_STRG_STRGSIZE, TBBS_SEPARATOR, width);

	return TRUE;
}

void CStrgTool::InitItemElement()
{
	m_cFontCBox.LimitText(LF_FACESIZE);
	m_cFontCBox.InitItemElement();
}

void CStrgTool::SetItemElement()
{
	int		ftategaki, spkflg;
	DBL		dmmfwidth, dmmfhight;
	DBL		dmmmojispace, dmmstrgspace;
	DBL		dmmstrgsize;
	DBL		num, par;
	char	numstr[RECMAX];

	static UDACCEL aSpinAccel[] = {
		{1,1},
		{2,5},
		{4,10},
		{8,50},
		{12,100},
	};

	mojienvgettategakiflag(&ftategaki);
	mojienvgetsize(&dmmfwidth, &dmmfhight);
	mojienvgetspacekind(&spkflg);
	mojienvgetstrlen(&dmmstrgsize);
	if(spkflg == ON){
		SetButtonInfo(6, ID_STRG_SPACEKIND, TBBS_BUTTON, 3);
	}
	else{
		SetButtonInfo(6, ID_STRG_SPACEKIND, TBBS_BUTTON, 2);
	}
	mojienvgetspace(&dmmmojispace, &dmmstrgspace);
	if(ftategaki == ON){
		SetButtonInfo(2, ID_STRG_TATEYOKO, TBBS_BUTTON, 0);
		num = dmmfwidth;
		par = dmmfhight / dmmfwidth * 100.0;
	}
	else{
		SetButtonInfo(2, ID_STRG_TATEYOKO, TBBS_BUTTON, 1);
		num = dmmfhight;
		par = dmmfwidth / dmmfhight * 100.0;
	}

	m_cFontCBox.SetItemElement();

	sprintf(numstr, "%4.1lf", num);  
	m_cEditFontSize.SetWindowText(numstr);
	m_cSpinFontSize.SetRange(0, 30000);
	m_cSpinFontSize.SetPos((int)num);
	m_cSpinFontSize.SetAccel(5, aSpinAccel);
	sprintf(numstr, "%4.0lf", par);  
	m_cEditFlatPar.SetWindowText(numstr);
	m_cSpinFlatPar.SetRange(10, 900);
	m_cSpinFlatPar.SetPos((int)par);
	m_cSpinFlatPar.SetAccel(5, aSpinAccel);
	sprintf(numstr, "%4.0lf", dmmmojispace);  
	m_cEditMojiSpace.SetWindowText(numstr);
	m_cSpinMojiSpace.SetRange(-30000, 30000);
	m_cSpinMojiSpace.SetPos((int)dmmmojispace);
	m_cSpinMojiSpace.SetAccel(5, aSpinAccel);
	sprintf(numstr, "%4.0lf", dmmstrgspace);  
	m_cEditStrgSpace.SetWindowText(numstr);
	m_cSpinStrgSpace.SetRange(-30000, 30000);
	m_cSpinStrgSpace.SetPos((int)dmmstrgspace);
	m_cSpinStrgSpace.SetAccel(5, aSpinAccel);
	sprintf(numstr, "%4.0lf", dmmstrgsize);  
	m_cEditStrgSize.SetWindowText(numstr);
	m_cSpinStrgSize.SetRange(-30000, 30000);
	m_cSpinStrgSize.SetPos((int)dmmstrgsize);
	m_cSpinStrgSize.SetAccel(5, aSpinAccel);
}

void CStrgTool::GetItemElement()
{
	int		ftategaki;
	DBL		dmmfwidth, dmmfhight;
	DBL		dmmmojispace, dmmstrgspace;
	DBL		dmmstrgsize;
	DBL		num, par;
	char	numstr[RECMAX];

	UpdateData(TRUE);

	m_cFontCBox.GetItemElement();

	m_cEditFontSize.GetWindowText(numstr, RECMAX);
	sscanf(numstr, "%lf", &num);
	m_cEditFlatPar.GetWindowText(numstr, RECMAX);
	sscanf(numstr, "%lf", &par);
	m_cEditMojiSpace.GetWindowText(numstr, RECMAX);
	sscanf(numstr, "%lf", &dmmmojispace);
	m_cEditStrgSpace.GetWindowText(numstr, RECMAX);
	sscanf(numstr, "%lf", &dmmstrgspace);
	m_cEditStrgSize.GetWindowText(numstr, RECMAX);
	sscanf(numstr, "%lf", &dmmstrgsize);

	mojienvgettategakiflag(&ftategaki);
	if(ftategaki == ON){
		dmmfwidth = num;
		dmmfhight = num * par / 100.0;
	}
	else{
		dmmfhight = num;
		dmmfwidth = num * par / 100.0;
	}
	mojienvsetsize(dmmfwidth, dmmfhight);
	mojienvsetspace(dmmmojispace, dmmstrgspace);
	mojienvsetstrlen(dmmstrgsize);
}

void CStrgTool::TateyokoChange() 
{
	int	ftategaki;

	mojienvgettategakiflag(&ftategaki);
	if(ftategaki == ON){
		ftategaki = OFF;
		SetButtonInfo(2, ID_STRG_TATEYOKO, TBBS_BUTTON, 1);
	}
	else{
		ftategaki = ON;
		SetButtonInfo(2, ID_STRG_TATEYOKO, TBBS_BUTTON, 0);
	}
	mojienvsettategakiflag(ftategaki);
}

void CStrgTool::SpacekindChange() 
{
	int	spkflg;

	mojienvgetspacekind(&spkflg);
	if(spkflg == ON){
		spkflg = OFF;
		SetButtonInfo(6, ID_STRG_SPACEKIND, TBBS_BUTTON, 2);
	}
	else{
		spkflg = ON;
		SetButtonInfo(6, ID_STRG_SPACEKIND, TBBS_BUTTON, 3);
	}
	mojienvsetspacekind(spkflg);
}

