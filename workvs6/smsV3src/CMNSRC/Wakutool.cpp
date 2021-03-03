// WakuTool.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "WakuTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
	void mojienvgettategakiflag(LPINT tateflg);
	void mojienvgetstartkind(LPINT nkind);
	void senvgetwakuconerkind(LPINT nkind);
	void mojienvgetstart(LPDBL dstartx, LPDBL dstarty);
	void mojienvgetsize(LPDBL dw, LPDBL dh);
	void mojienvgetstrlen(LPDBL dmmflen);
	void mojienvgetmojicount(LPDBL dmojicount);

	void mojienvsettategakiflag(int tateflg);
	void mojienvsetstartkind(int nkind);
	void senvsetwakuconerkind(int nkind);
	void mojienvsetstart(DBL dstartx, DBL dstarty);
	void mojienvsetsize(DBL dw, DBL dh);
	void mojienvsetstrlen(DBL dmmflen);
	void mojienvsetmojicount(DBL dmojicount);

	void poutcntrkindset(int kind);
	void poutcntrkindget(int *kind);
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
}

/////////////////////////////////////////////////////////////////////////////
// CWakuTool

CWakuTool::CWakuTool()
{
}

CWakuTool::~CWakuTool()
{
}


BEGIN_MESSAGE_MAP(CWakuTool, CToolBar)
	//{{AFX_MSG_MAP(CWakuTool)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWakuTool メッセージ ハンドラ

int CWakuTool::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1){
		return -1;
	}

	return 0;
}

void CWakuTool::OnDestroy() 
{
	CToolBar::OnDestroy();
}

BOOL CWakuTool::SetToolItem(CSize textsize)
{
	int		sx, sy, ex, ey;
	int		width;
	
	sx = 0; sy = 0; ex = LF_FACESIZE*textsize.cx; ey = 200;
	CRect rect(sx,sy,ex,ey);
	if (!m_cFontCBox.Create(COMBOSTYLE,	rect, this, IDC_CBFONTNAME))
	{
		TRACE0("Failed to create combo-box\n");
		return FALSE;
	}
	width = (ex - sx);
	SetButtonInfo(0, IDC_CBFONTNAME, TBBS_SEPARATOR, width);

	sx = ex+SEPARATORDOT; sy = 0; ex = sx + textsize.cx*2; ey = 24;
	rect.SetRect(sx, sy, ex, ey);
	width = (ex - sx);
	SetButtonInfo(1, ID_WAKU_TATEYOKO, TBBS_BUTTON, 0);

	sx = ex+SEPARATORDOT; sy = 0; ex = sx + textsize.cx*2; ey = 24;
	rect.SetRect(sx, sy, ex, ey);
	width = (ex - sx);
	SetButtonInfo(2, ID_WAKU_STARTKIND, TBBS_BUTTON, 0);

	sx = ex+SEPARATORDOT; sy = 0; ex = sx + textsize.cx*2; ey = 24;
	rect.SetRect(sx, sy, ex, ey);
	width = (ex - sx);
	SetButtonInfo(3, ID_WAKU_CONERKIND, TBBS_BUTTON, 0);

	sx = ex+SEPARATORDOT; sy = 0; ex = sx + textsize.cx*2; ey = 24;
	rect.SetRect(sx, sy, ex, ey);
	width = (ex - sx);
	SetButtonInfo(4, ID_WAKU_CNTRKIND, TBBS_BUTTON, 0);

	sx = ex+SEPARATORDOT; sy = 0; ex = sx + textsize.cx*2; ey = 24;
	rect.SetRect(sx, sy, ex, ey);
	width = (ex - sx);
	SetButtonInfo(5, ID_WAKU_BTNDSPWAKU, TBBS_BUTTON, 0);

	sx = ex+SEPARATORDOT; sy = 0; ex = sx + textsize.cx*2; ey = 24;
	rect.SetRect(sx, sy, ex, ey);
	width = (ex - sx);
	SetButtonInfo(6, ID_WAKU_BTNSETWAKU, TBBS_BUTTON, 0);

	sx = ex+SEPARATORDOT; ex = sx + textsize.cx*8;
	rect.SetRect(sx, sy, ex, ey);
	if (!m_cEditStartX.Create(EDITSTYLE, rect, this, IDC_EDSTARTX))
	{
		TRACE0("Failed to create fontsize spincntrl\n");
		return FALSE;
	}
	if (!m_cSpinStartX.Create(SPINSTYLE, rect, this, IDC_SPSTARTX))
	{
		TRACE0("Failed to create fontsize spincntrl\n");
		return FALSE;
	}
	width = (ex - sx);
	SetButtonInfo(7, IDC_EDSTARTX, TBBS_SEPARATOR, width);

	sx = ex+SEPARATORDOT; ex = sx + textsize.cx*8;
	rect.SetRect(sx, sy, ex, ey);
	if (!m_cEditStartY.Create(EDITSTYLE, rect, this, IDC_EDSTARTY))
	{
		TRACE0("Failed to create fontsize spincntrl\n");
		return FALSE;
	}
	if (!m_cSpinStartY.Create(SPINSTYLE, rect, this, IDC_SPSTARTY))
	{
		TRACE0("Failed to create fontsize spincntrl\n");
		return FALSE;
	}
	width = (ex - sx);
	SetButtonInfo(8, IDC_EDSTARTY, TBBS_SEPARATOR, width);

	sx = ex+SEPARATORDOT; ex = sx + textsize.cx*8;
	rect.SetRect(sx, sy, ex, ey);
	if (!m_cEditWidth.Create(EDITSTYLE, rect, this, IDC_EDWIDTH))
	{
		TRACE0("Failed to create fontsize spincntrl\n");
		return FALSE;
	}
	if (!m_cSpinWidth.Create(SPINSTYLE, rect, this, IDC_SPWIDTH))
	{
		TRACE0("Failed to create fontsize spincntrl\n");
		return FALSE;
	}
	width = (ex - sx);
	SetButtonInfo(9, IDC_EDWIDTH, TBBS_SEPARATOR, width);

	sx = ex+SEPARATORDOT; ex = sx + textsize.cx*8;
	rect.SetRect(sx, sy, ex, ey);
	if (!m_cEditHight.Create(EDITSTYLE, rect, this, IDC_EDHIGHT))
	{
		TRACE0("Failed to create fontsize spincntrl\n");
		return FALSE;
	}
	if (!m_cSpinHight.Create(SPINSTYLE, rect, this, IDC_SPHIGHT))
	{
		TRACE0("Failed to create fontsize spincntrl\n");
		return FALSE;
	}
	width = (ex - sx);
	SetButtonInfo(10, IDC_EDHIGHT, TBBS_SEPARATOR, width);

	sx = ex; ex = sx + textsize.cx*6;
	rect.SetRect(sx, sy, ex, ey);
	if (!m_cEditMojiCount.Create(EDITSTYLE, rect, this, IDC_EDMOJICOUNT))
	{
		TRACE0("Failed to create flatpar spincntrl\n");
		return FALSE;
	}
	if (!m_cSpinMojiCount.Create(SPINSTYLE, rect, this, IDC_SPMOJICOUNT))
	{
		TRACE0("Failed to create flatpar spincntrl\n");
		return FALSE;
	}
	width = (ex - sx);
	SetButtonInfo(11, IDC_EDMOJICOUNT, TBBS_SEPARATOR, width);

	return TRUE;
}

void CWakuTool::InitItemElement()
{
	m_cFontCBox.LimitText(LF_FACESIZE);
	m_cFontCBox.InitItemElement();
}

void CWakuTool::SetItemElement()
{
	int		ftategaki;
	int		ncntrkind;
	int		nstartkind, nconerkind;
	DBL		workmmx, workmmy;
	DBL		addx, addy;
	DBL		dmmsx, dmmsy;
	DBL		dmmfwidth, dmmfhight;
	DBL		dmmflen;
	DBL		dmojicount;
	DBL		dwidth, dhight;
	char	numstr[RECMAX];

	static UDACCEL aSpinAccel[] = {
		{1,1},
		{2,5},
		{4,10},
		{8,50},
		{12,100},
	};

	poutcntrkindget(&ncntrkind);
	mojienvgettategakiflag(&ftategaki);
	mojienvgetstartkind(&nstartkind);
	senvgetwakuconerkind(&nconerkind);
	mojienvgetstart(&dmmsx, &dmmsy);
	mojienvgetsize(&dmmfwidth, &dmmfhight);
	mojienvgetstrlen(&dmmflen);
	mojienvgetmojicount(&dmojicount);
	//EncodeStartXY(&dmmsx, &dmmsy);

	if(ftategaki == ON){
		dwidth = dmmfwidth;
		dhight = dmmflen;
	}
	else{
		dwidth = dmmflen;
		dhight = dmmfhight;
	}


	scrngetmmsize(&workmmx, &workmmy);

	switch(nconerkind){
	case CNTRCNTR:
		addx = dwidth / 2;
		addy = dhight / 2;
		break;
	case LEFTUP: // 枠支点
		addx = 0;
		addy = 0;
		break;
	case LEFTDOWN:
		addx = 0;
		addy = dhight;
		break;
	case RIGHTDOWN:
		addx = dwidth;
		addy = dhight;
		break;
	case RIGHTUP:
		addx = dwidth;
		addy = 0;
		break;
	}

	switch(nstartkind){
	case RIGHTDOWN:
		dmmsx = workmmx - dmmsx;
		dmmsx -= addx;
		dmmsy -= addy;
		break;
	case RIGHTUP:
		dmmsx = workmmx - dmmsx;
		dmmsy = workmmy - dmmsy;
		dmmsx -= addx;
		dmmsy += addy;
		break;
	case LEFTUP:
		dmmsy = workmmy - dmmsy;
		dmmsx += addx;
		dmmsy += addy;
		break;
	case LEFTDOWN:	//作図原点
		dmmsx += addx;
		dmmsy -= addy;
		break;
	case CNTRCNTR:
		dmmsx = workmmx / 2 - dmmsx;
		dmmsy = workmmy / 2 - dmmsy;
		dmmsx -= addx;
		dmmsy += addy;
		dmmsx *= -1;
		dmmsy *= -1;
		break;
	}

	m_cFontCBox.SetItemElement();
	TateYokoFlagBottonSet(ftategaki);
	StartKindBottonSet(nstartkind);
	ConerKindBottonSet(nconerkind);
	CntrKindBottnSet(ncntrkind);
	SetButtonInfo(5, ID_WAKU_BTNDSPWAKU, TBBS_BUTTON, 16);
	SetButtonInfo(6, ID_WAKU_BTNSETWAKU, TBBS_BUTTON, 17);

	sprintf(numstr, "%4.1lf", dmmsx);
	m_cEditStartX.SetWindowText(numstr);
	m_cSpinStartX.SetRange(-30000, 30000);
	m_cSpinStartX.SetPos((int)dmmsx);
	m_cSpinStartX.SetAccel(5, aSpinAccel);

	sprintf(numstr, "%4.1lf", dmmsy);
	m_cEditStartY.SetWindowText(numstr);
	m_cSpinStartY.SetRange(-30000, 30000);
	m_cSpinStartY.SetPos((int)dmmsy);
	m_cSpinStartY.SetAccel(5, aSpinAccel);

	sprintf(numstr, "%4.1lf", dwidth);
	m_cEditWidth.SetWindowText(numstr);
	m_cSpinWidth.SetRange(-30000, 30000);
	m_cSpinWidth.SetPos((int)dwidth);
	m_cSpinWidth.SetAccel(5, aSpinAccel);

	sprintf(numstr, "%4.1lf", dhight);
	m_cEditHight.SetWindowText(numstr);
	m_cSpinHight.SetRange(-30000, 30000);
	m_cSpinHight.SetPos((int)dhight);
	m_cSpinHight.SetAccel(5, aSpinAccel);

	sprintf(numstr, "%4.1lf", dmojicount);
	m_cEditMojiCount.SetWindowText(numstr);
	m_cSpinMojiCount.SetRange(0, 300);
	m_cSpinMojiCount.SetPos((int)dmojicount);
	m_cSpinMojiCount.SetAccel(5, aSpinAccel);


	UpdateData(FALSE);
}

void CWakuTool::GetItemElement()
{
	int		ftategaki;
	int		nstartkind, nconerkind;
	DBL		workmmx, workmmy;
	DBL		addx, addy;
	DBL		dmmsx, dmmsy;
	DBL		dmmfwidth, dmmfhight;
	DBL		dmmflen;
	DBL		dmojicount;
	DBL		dwidth, dhight;
	char	numstr[RECMAX];

	UpdateData(TRUE);

	m_cFontCBox.GetItemElement();

	m_cEditStartX.GetWindowText(numstr, RECMAX);
	sscanf(numstr, "%lf", &dmmsx);
	m_cEditStartY.GetWindowText(numstr, RECMAX);
	sscanf(numstr, "%lf", &dmmsy);
	m_cEditWidth.GetWindowText(numstr, RECMAX);
	sscanf(numstr, "%lf", &dwidth);
	m_cEditHight.GetWindowText(numstr, RECMAX);
	sscanf(numstr, "%lf", &dhight);
	m_cEditMojiCount.GetWindowText(numstr, RECMAX);
	sscanf(numstr, "%lf", &dmojicount);

	mojienvgettategakiflag(&ftategaki);
	mojienvgetstartkind(&nstartkind);
	senvgetwakuconerkind(&nconerkind);
	mojienvgetsize(&dmmfwidth, &dmmfhight);
	mojienvgetstrlen(&dmmflen);
	//DecodeStartXY(&dmmsx, &dmmsy);
	scrngetmmsize(&workmmx, &workmmy);

	switch(nconerkind){
	case CNTRCNTR:
		addx = dwidth / 2;
		addy = dhight / 2;
		break;
	case LEFTUP: // 枠支点
		addx = 0;
		addy = 0;
		break;
	case LEFTDOWN:
		addx = 0;
		addy = dhight;
		break;
	case RIGHTDOWN:
		addx = dwidth;
		addy = dhight;
		break;
	case RIGHTUP:
		addx = dwidth;
		addy = 0;
		break;
	}

	switch(nstartkind){
	case RIGHTDOWN:
		dmmsx += addx;
		dmmsy += addy;
		dmmsx = workmmx - dmmsx;
		break;
	case RIGHTUP:
		dmmsx += addx;
		dmmsy -= addy;
		dmmsx = workmmx - dmmsx;
		dmmsy = workmmy - dmmsy;
		break;
	case LEFTUP:
		dmmsx -= addx;
		dmmsy -= addy;
		dmmsy = workmmy - dmmsy;
		break;
	case LEFTDOWN:	//作図原点
		dmmsx -= addx;
		dmmsy += addy;
		break;
	case CNTRCNTR:
		dmmsx *= -1;
		dmmsy *= -1;
		dmmsx += addx;
		dmmsy -= addy;
		dmmsx = workmmx / 2 - dmmsx;
		dmmsy = workmmy / 2 - dmmsy;
		break;
	}
	if(ftategaki == ON){
		dmmfwidth = dwidth;
		dmmfhight = dwidth;
		dmmflen = dhight;
	}
	else{
		dmmflen = dwidth;
		dmmfwidth = dhight;
		dmmfhight = dhight;
	}
	
	mojienvsetstart(dmmsx, dmmsy);
	mojienvsetsize(dmmfwidth, dmmfhight);
	mojienvsetstrlen(dmmflen);
	mojienvsetmojicount(dmojicount);
}

void CWakuTool::TateyokoChange() 
{
	int	ftategaki;

	mojienvgettategakiflag(&ftategaki);
	if(ftategaki == ON){
		ftategaki = OFF;
	}
	else{
		ftategaki = ON;
	}
	mojienvsettategakiflag(ftategaki);
	TateYokoFlagBottonSet(ftategaki);
}

void CWakuTool::TateYokoFlagBottonSet(int ftategaki)
{
	if(ftategaki == ON){
		SetButtonInfo(1, ID_WAKU_TATEYOKO, TBBS_BUTTON, 0);
	}
	else{
		SetButtonInfo(1, ID_WAKU_TATEYOKO, TBBS_BUTTON, 1);
	}
}

void CWakuTool::StartKindChange()
{
	int	nstartkind;

	mojienvgetstartkind(&nstartkind);
	switch(nstartkind){
	case LEFTDOWN:
		nstartkind = LEFTUP;
		break;
	case LEFTUP:
		nstartkind = RIGHTUP;
		break;
	case RIGHTUP:
		nstartkind = RIGHTDOWN;
		break;
	case RIGHTDOWN:
		nstartkind = CNTRCNTR;
		break;
	case CNTRCNTR:
		nstartkind = LEFTDOWN;
		break;
	}
	mojienvsetstartkind(nstartkind);
	//StartKindBottonSet(nstartkind);
	SetItemElement();
}

void CWakuTool::StartKindBottonSet(int nstartkind)
{
	switch(nstartkind){
	case LEFTDOWN:
		SetButtonInfo(2, ID_WAKU_STARTKIND, TBBS_BUTTON, 2);
		break;
	case LEFTUP:
		SetButtonInfo(2, ID_WAKU_STARTKIND, TBBS_BUTTON, 3);
		break;
	case RIGHTUP:
		SetButtonInfo(2, ID_WAKU_STARTKIND, TBBS_BUTTON, 4);
		break;
	case RIGHTDOWN:
		SetButtonInfo(2, ID_WAKU_STARTKIND, TBBS_BUTTON, 5);
		break;
	case CNTRCNTR:
		SetButtonInfo(2, ID_WAKU_STARTKIND, TBBS_BUTTON, 6);
		break;
	}
}

void CWakuTool::ConerKindChange() 
{
	int	nconerkind;

	senvgetwakuconerkind(&nconerkind);
	switch(nconerkind){
	case LEFTDOWN:
		nconerkind = LEFTUP;
		break;
	case LEFTUP:
		nconerkind = RIGHTUP;
		break;
	case RIGHTUP:
		nconerkind = RIGHTDOWN;
		break;
	case RIGHTDOWN:
		nconerkind = CNTRCNTR;
		break;
	case CNTRCNTR:
		nconerkind = LEFTDOWN;
		break;
	}
	senvsetwakuconerkind(nconerkind);
	//ConerKindBottonSet(nconerkind);
	SetItemElement();
}

void CWakuTool::ConerKindBottonSet(int nconerkind)
{
	switch(nconerkind){
	case LEFTDOWN:
		SetButtonInfo(3, ID_WAKU_CONERKIND, TBBS_BUTTON, 7);
		break;
	case LEFTUP:
		SetButtonInfo(3, ID_WAKU_CONERKIND, TBBS_BUTTON, 8);
		break;
	case RIGHTUP:
		SetButtonInfo(3, ID_WAKU_CONERKIND, TBBS_BUTTON, 9);
		break;
	case RIGHTDOWN:
		SetButtonInfo(3, ID_WAKU_CONERKIND, TBBS_BUTTON, 10);
		break;
	case CNTRCNTR:
		SetButtonInfo(3, ID_WAKU_CONERKIND, TBBS_BUTTON, 11);
		break;
	}
}

void CWakuTool::CntrKindBottnSet(int ncntrkind)
{
	switch(ncntrkind){
	case LEFTDOWN:
		SetButtonInfo(4, ID_WAKU_CNTRKIND, TBBS_BUTTON, 12);
		break;
	case LEFTUP:
		SetButtonInfo(4, ID_WAKU_CNTRKIND, TBBS_BUTTON, 13);
		break;
	case RIGHTUP:
		SetButtonInfo(4, ID_WAKU_CNTRKIND, TBBS_BUTTON, 14);
		break;
	case RIGHTDOWN:
		SetButtonInfo(4, ID_WAKU_CNTRKIND, TBBS_BUTTON, 15);
		break;
	}
}

void CWakuTool::CntrKindChange()
{
	int	ncntrkind;

	poutcntrkindget(&ncntrkind);
	switch(ncntrkind){
	case LEFTDOWN:
		ncntrkind = LEFTUP;
		break;
	case LEFTUP:
		ncntrkind = RIGHTUP;
		break;
	case RIGHTUP:
		ncntrkind = RIGHTDOWN;
		break;
	case RIGHTDOWN:
		ncntrkind = LEFTDOWN;
		break;
	}
	poutcntrkindset(ncntrkind);
	SetItemElement();
}
