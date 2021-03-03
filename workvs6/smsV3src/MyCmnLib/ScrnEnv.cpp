// scrnenv.cpp : アプリケーション用環境定義
//

#include "stdafx.h"

#include "scrnenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
	void scrninit();
	void scrnend();
	void scrnsetwinarea(int sx, int sy, int ex, int ey);
	void scrninitsizeelement(LPINT width, LPINT hight);
	void scrnsetrltvelement();
	void scrngetmmsize(LPDBL pdmmxsize, LPDBL pdmmysize);
	void scrngetrltvsize(LPDBL pdRXsize, LPDBL pdRYsize);
	void scrnsetrltvtop(DBL xTop, DBL yTop);
	void scrngetrltvtop(LPDBL xTop, LPDBL yTop);
	void scrnsetrltvmove(DBL x, DBL y);
	void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	DBL scrnsetxytime(DBL timex, DBL timey);
	void scrngetxytime(LPDBL timex, LPDBL timey);
	void scrnonundoflag();
	void scrnoffundoflag();
	int scrncheckundoflag();
	void scrndxymmtorltv(LPDBL x, LPDBL y);
	void scrndxyrltvtomm(LPDBL x, LPDBL y);
	void scrndlenmmtorltv(LPDBL len);
	void scrndlenrltvtomm(LPDBL len);
	void scrnsetdisprltvmove(DBL xmove, DBL ymove);
	void scrngetdisprltvmove(LPDBL xmove, LPDBL ymove);
	int senvsetdrawmode(int mode);
	int	senvcheckdrawmode();
}

CScrnEnv::CScrnEnv()
{
	scrninit();
}

CScrnEnv::~CScrnEnv()
{
}

int CScrnEnv::SetDrawMode(int mode)
{
	return(senvsetdrawmode(mode));
}

int CScrnEnv::CheckDrawMode()
{
	return(senvcheckdrawmode());
}

void CScrnEnv::GetMMSize(LPDBL xsize, LPDBL ysize)
{
	scrngetmmsize(xsize, ysize);
}

DBL CScrnEnv::SetXYTime(DBL xtime, DBL ytime)
{
	return(scrnsetxytime(xtime, ytime));
}

void CScrnEnv::SetRltvElement()
{
	scrnsetrltvelement();
}

void CScrnEnv::SetRltvTop(DBL topx, DBL topy)
{
	scrnsetrltvtop(topx, topy);
}

void CScrnEnv::GetRltvTop(LPDBL topx, LPDBL topy)
{
	scrngetrltvtop(topx, topy);
}

void CScrnEnv::SetRltvMove(DBL movex, DBL movey)
{
	scrnsetrltvmove(movex, movey);
}

void CScrnEnv::SetDispRltvMove(DBL movex, DBL movey)
{
	scrnsetdisprltvmove(movex, movey);
}

void CScrnEnv::SetRltvArea(DBL sx, DBL sy, DBL ex, DBL ey)
{
	scrnrltvareaset(sx, sy, ex, ey);
}

void CScrnEnv::GetRltvSize(LPDBL xsize, LPDBL ysize)
{
	scrngetrltvsize(xsize, ysize);
}

void CScrnEnv::SetWinArea(int sx, int sy, int ex, int ey)
{
	scrnsetwinarea(sx, sy, ex, ey);
}

void CScrnEnv::InitSizeElement(LPINT w, LPINT h)
{
	scrninitsizeelement(w, h);
}

int CScrnEnv::CheckUndo()
{
	return(scrncheckundoflag());
}

void CScrnEnv::OnUndo()
{
	scrnonundoflag();
}

void CScrnEnv::OffUndo()
{
	scrnoffundoflag();
}

void CScrnEnv::TrnsXYMMToRltv(LPDBL xx, LPDBL yy)
{
	scrndxymmtorltv(xx, yy);
}

void CScrnEnv::TrnsXYRltvToMM(LPDBL xx, LPDBL yy)
{
	scrndxyrltvtomm(xx, yy);
}

void CScrnEnv::TrnsLenMMToRltv(LPDBL len)
{
	scrndlenmmtorltv(len);
}

void CScrnEnv::TrnsLenRltvToMM(LPDBL len)
{
	scrndlenrltvtomm(len);
}
