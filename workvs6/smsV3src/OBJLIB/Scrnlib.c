//
// Easy Scanner System Ver1.0
//
// ess  scrnlib.c  環境関係関数
//
//  Copyright (c) 1998 by THEON

#include	<windows.h>
#include	<stdio.h>
#include	<math.h>

#include	"..\stdef.h"
#include	"dlldef.h"

#include	"plgnfill.fnc"
#include	"senvlib.fnc"
#include	"scrnlib.fnc"

DBL		dmmxsize, dmmysize;		// 実際の作図サイズ
DBL 	dmmworksx, dmmworksy;		// 枠作成開始位置
DBL		dmmworkxsize, dmmworkysize; // 枠作成領域サイズ

DBL		dxtime, dytime;			// 現在の倍率
DBL		dtime;				// 全体表示倍率
DBL		drltvxtop, drltvytop;		// 表示開始位置
DBL		drltvxarea, drltvyarea;		// 相対全体サイズ
DBL		drevercexsize, dreverceysize;	// 反転位置
DBL		dmoverltvx, dmoverltvy;	// プリンター出力時位置補正値
DBL		ddispmoverltvx, ddispmoverltvy;	// 画面表示位置補正値
DBL		drltvsx, drltvsy, drltvex, drltvey;		//クリッピング領域
int		winsx, winsy, winex, winey;
int		prntcntrmoveflag, prntstartmoveflag;
DBL		prntstartxmm, prntstartymm;

int		nswflag;		// 0 UNDO(ON,OFF) 1 CLIP(ON,OFF) 2 MJONE(ON,OFF)
int		nundoflag;
int		noactflag;
int		wakunodisp;
int		namedisp;

extern int		bkrval, bkgval, bkbval;
extern int		frrval, frgval, frbval;
extern int		linerval, linegval, linebval;
extern int		curvrval, curvgval, curvbval;
extern int		sublrval, sublgval, sublbval;
extern int		slctrval, slctgval, slctbval;
extern int		noactrval, noactgval, noactbval;
extern int		imagerval, imagegval, imagebval;
extern int		nPlgnThick;
DBL		dPlgnThick;

DllExport void scrninit()
{
	drltvxtop = 0;
	drltvytop = 0;
	drevercexsize = 0;
	dreverceysize = 0;
	dmoverltvx = 0;
	dmoverltvy = 0;
	ddispmoverltvx = 0;
	ddispmoverltvy = 0;
	nundoflag = OFF;
	noactflag = OFF;
	wakunodisp = ON;
	namedisp = ON;
	dxtime = 0;
	dytime = 0;
}

DllExport void scrnend()
{
}

DllExport void scrnsetwakunodisp(int flag)
{
	wakunodisp = flag;
}

DllExport int scrngetwakunodisp()
{
	return(wakunodisp);
}

DllExport void scrnsetnamedisp(int flag)
{
	namedisp = flag;
}

DllExport int scrngetnamedisp()
{
	return(namedisp);
}

DllExport void scrnonswflag(int bit)
{
	nswflag |= bit;
}

DllExport void scrnoffswflag(int bit)
{
	nswflag &= ~bit;
}

DllExport int scrncheckswflag(int bit)
{
	if((nswflag & bit) == bit){
		return(ON);
	}
	else{
		return(OFF);
	}
}

DllExport void scrnonundoflag()
{
	nundoflag = ON;
}

DllExport void scrnoffundoflag()
{
	nundoflag = OFF;
}

DllExport int scrncheckundoflag()
{
	return(nundoflag);
}

DllExport void scrnonnoactflag()
{
	noactflag = ON;
}

DllExport void scrnoffnoactflag()
{
	noactflag = OFF;
}

DllExport int scrnchecknoactflag()
{
	return(noactflag);
}

DllExport void scrnsetwinarea(int sx, int sy, int ex, int ey)
{
	winsx = sx;
	winsy = sy;
	winex = ex;
	winey = ey;
}

DllExport void scrnsetwintop(DBL topx, DBL topy)
{
	int		w, h;

	w = winex - winsx;
	h = winey - winsy;
	winsx = (int)topx;
	winsy = (int)topy;
	winex = winsx + w;
	winey = winsy + h;
}

DllExport void scrngetwinarea(int *sx, int *sy, int *ex, int *ey)
{
	*sx = winsx;
	*sy = winsy;
	*ex = winex;
	*ey = winey;
}

DllExport void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize)
{
	*pdmmxsize = dmmxsize;
	*pdmmysize = dmmysize;
}

DllExport void scrnsetmmsize(DBL Xsize, DBL Ysize)
{
	dmmxsize = Xsize;
	dmmysize = Ysize;
}

DllExport void scrnsetmmworkstart(DBL sx, DBL sy)
{
	dmmworksx = sx;
	dmmworksy = sy;
}

DllExport void scrnsetmmworksize(DBL xsize, DBL ysize)
{
	dmmworkxsize = xsize;
	dmmworkysize = ysize;
}

DllExport void scrngetmmworkstart(DBL *sx, DBL *sy)
{
	*sx = dmmworksx;
	*sy = dmmworksy;
}

DllExport void scrngetmmworksize(DBL *xsize, DBL *ysize)
{
	*xsize = dmmworkxsize;
	*ysize = dmmworkysize;
}

DllExport void scrninitsizeelement(LPINT width, LPINT hight)
{
	DBL	xtime, ytime;

	if(dmmxsize == 0 && dmmysize == 0){
		return;
	}
	xtime = dmmxsize / (DBL)(*width);
	ytime = dmmysize / (DBL)(*hight);

	if(xtime > ytime){
		dtime = xtime;
	}
	else{
		dtime = ytime;
	}
	dxtime = dtime;
	dytime = dtime;

	scrnsetrltvelement();

	*width = (int)(dmmxsize / dxtime);
	*hight = (int)(dmmysize / dytime);
}

DllExport void scrnsetrltvelement()
{
	drltvxtop = 0;
	drltvytop = 0;
	drevercexsize = 0;
	dreverceysize = 0;
	drltvxarea = (int)(dmmxsize/dxtime);
	drltvyarea = (int)(dmmysize/dytime);
	// ぼっちゃんで付加？？
	//dxtime = dmmxsize / drltvxarea;
	//dytime = dmmysize / drltvyarea;
}

DllExport void scrngetrltvsize(DBL *pdRXsize, DBL *pdRYsize)
{
	*pdRXsize = drltvxarea;
	*pdRYsize = drltvyarea;
}

DllExport void scrnsetrltvsize(DBL dRXsize, DBL dRYsize)
{
	drltvxarea = dRXsize;
	drltvyarea = dRYsize;
}

DllExport void scrnsetcntrmoveflag(int flag)
{
	prntcntrmoveflag = flag;
}

DllExport int scrngetcntrmoveflag()
{
	if(prntcntrmoveflag == 0){
		return(OFF);
	}
	else{
		return(ON);
	}
}

DllExport void scrnsetstartmoveflag(int flag)
{
	prntstartmoveflag = flag;
}

DllExport int scrngetstartmoveflag()
{
	if(prntstartmoveflag == 0){
		return(OFF);
	}
	else{
		return(ON);
	}
}

DllExport void scrnsetstartmovemm(DBL xmm, DBL ymm)
{
	prntstartxmm = xmm;
	prntstartymm = ymm;
}

DllExport void scrngetstartmovemm(LPDBL xmm, LPDBL ymm)
{
	*xmm = prntstartxmm;
	*ymm = prntstartymm;
}

DllExport void scrnsetrltvmove(DBL xmove, DBL ymove)
{
	dmoverltvx = xmove;
	dmoverltvy = ymove;
}

DllExport void scrngetrltvmove(LPDBL xmove, LPDBL ymove)
{
	*xmove = dmoverltvx;
	*ymove = dmoverltvy;
}

DllExport void scrnsetdisprltvmove(DBL xmove, DBL ymove)
{
	ddispmoverltvx = xmove;
	ddispmoverltvy = ymove;
}

DllExport void scrngetdisprltvmove(LPDBL xmove, LPDBL ymove)
{
	*xmove = ddispmoverltvx;
	*ymove = ddispmoverltvy;
}

DllExport void scrnsetrltvtop(DBL xTop, DBL yTop)
{
	drltvxtop = xTop;
	drltvytop = yTop;
	drevercexsize = 0;
	dreverceysize = 0;
}

DllExport void scrngetrltvtop(LPDBL xTop, LPDBL yTop)
{
	*xTop = drltvxtop;
	*yTop = drltvytop;
}

DllExport void scrnsetrltvtopreverce(DBL xTop, DBL yTop, DBL xSize, DBL ySize)
{
	drltvxtop = xTop;
	drltvytop = yTop;
	drevercexsize = xSize;
	dreverceysize = ySize;
}

DllExport void scrnrevercecheck(int *flagx, int *flagy)
{
	if(drevercexsize != 0){
		*flagx = ON;
	}
	else{
		*flagx = OFF;
	}

	if(dreverceysize != 0){
		*flagy = ON;
	}
	else{
		*flagy = OFF;
	}
}

DllExport DBL scrngetbasetime()
{
	return(dtime);
}

DllExport void scrnsetbasetime(DBL time)
{
	dtime = time;
}

DllExport DBL scrnsetxytime(DBL xtime, DBL ytime)
{
	DBL	rettime;

	rettime = dxtime;
	dxtime = xtime;
	dytime = ytime;
	return(rettime);
}

DllExport void scrngetxytime(LPDBL xtime, LPDBL ytime)
{
	*xtime = dxtime;
	*ytime = dxtime;
}

DllExport void scrnresetzoomtime()
{
	dxtime = dtime;
	dytime = dtime;
}

DllExport BOOL scrncheckzoomall()
{
	if(dxtime == dtime && dytime == dtime){
		return(TRUE);
	}
	return(FALSE);
}

DllExport void scrnenvbasetimezoomup()
{
	DBL		rltvxsize, rltvysize;

	dxtime /= 1.5;
	rltvxsize = dmmxsize / dxtime;
	rltvysize = dmmysize / dxtime;
	if(rltvxsize >= AREAMAXLIMIT || rltvysize >= AREAMAXLIMIT){
		// 倍率をもとに戻す
		dxtime = dytime;
	}
	else{
		// 倍率を変更
		dytime = dxtime;
	}
}

DllExport void scrnenvbasetimezoomdown()
{
	dxtime *= 1.5;
	if(dtime < dxtime){
		dxtime = dtime;
		dytime = dtime;
	}
	else{
		dytime = dxtime;
	}
}

DllExport void scrngetwinunitsize(int *pnUnitXsize, int *pnUnitYsize)
{
	DBL		rltvxsize, rltvysize;

	rltvxsize = dmmxsize / dxtime;
	rltvysize = dmmxsize / dxtime;
	if(rltvxsize <= (DBL)(*pnUnitXsize)){
		(*pnUnitXsize) = (int)rltvxsize;
	}
	else{
		(*pnUnitXsize) /= 4;
	}
	if(rltvysize <= (DBL)(*pnUnitYsize)){
		(*pnUnitYsize) = (int)rltvysize;
	}
	else{
		(*pnUnitYsize) /= 4;
	}
}

DllExport void scrndxyrltvtomm(LPDBL x, LPDBL y)
{
	DBL	rltvx, rltvy;
	DBL	mmx, mmy;

	rltvx = (*x)+drltvxtop;
	rltvy = drltvyarea - ((*y)+drltvytop);
	mmx = rltvx * dxtime;
	mmy = rltvy * dytime;
	*x = mmx;
	*y = mmy;
}

DllExport void scrndxymmtorltv(LPDBL x, LPDBL y)
{
	DBL	rltvx, rltvy;

	//rltvx = (*x)+drltvxtop;
	//rltvy = drltvyarea - ((*y)+drltvytop);

	rltvx = (*x) / dxtime;
	rltvy = (*y) / dytime;
	rltvx = rltvx - drltvxtop;
	rltvy = (drltvyarea - rltvy)  - drltvytop;
	if(drevercexsize != 0){
		rltvx = drevercexsize - rltvx;
	}
	if(dreverceysize != 0){
		rltvy = dreverceysize - rltvy;
	}
	*x = rltvx + dmoverltvx + ddispmoverltvx;
	*y = rltvy + dmoverltvy + ddispmoverltvy;
}

DllExport void scrndlenrltvtomm(LPDBL len)
{
	DBL	rltvlen;
	DBL	abslen;

	rltvlen = *len;
	abslen = rltvlen * dxtime;
	*len = abslen;
}

DllExport void scrndlenmmtorltv(LPDBL len)
{
	DBL	rltvlen;
	DBL	abslen;

	abslen = *len;
	rltvlen = abslen / dxtime;
	*len = rltvlen;
}

DllExport void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey)
{
	drltvsx = sx;
	drltvsy = sy;
	drltvex = ex;
	drltvey = ey;
	plgnfillareaset(drltvsx, drltvsy, drltvex, drltvey);
}

DllExport void scrnrltvareaget(DBL *sx, DBL *sy, DBL *ex, DBL *ey)
{
	*sx = drltvsx;
	*sy = drltvsy;
	*ex = drltvex;
	*ey = drltvey;
}

DllExport int scrnrltvareaychk(DBL y)
{
	if((drltvsy-nPlgnThick) < y && y < (drltvey+nPlgnThick)){
		return(INAREA);
	}
	return(OFF);
}

DllExport int scrnrltvareaxychk(DBL x, DBL y)
{
	dPlgnThick = nPlgnThick;
	if((drltvsx-dPlgnThick) < x && x < (drltvex+dPlgnThick)
	&& (drltvsy-dPlgnThick) < y && y < (drltvey+dPlgnThick)){
		return(INAREA);
	}
	return(OFF);
}

DllExport int scrnrltvareachk(DBL sx, DBL sy, DBL ex, DBL ey)
{
	DBL	tt;
	int	flagx1, flagy1, flagx2, flagy2;

	dPlgnThick = nPlgnThick;
	if(sx > ex){
		tt = sx;
		sx = ex;
		ex = tt;
	}
	if(sy > ey){
		tt = sy;
		sy = ey;
		ey = tt;
	}
	flagx1 = scrnareasignget(sx, (drltvsx-dPlgnThick), (drltvex+dPlgnThick));
	flagy1 = scrnareasignget(sy, (drltvsy-dPlgnThick), (drltvey+dPlgnThick));
	flagx2 = scrnareasignget(ex, (drltvsx-dPlgnThick), (drltvex+dPlgnThick));
	flagy2 = scrnareasignget(ey, (drltvsy-dPlgnThick), (drltvey+dPlgnThick));
	if(flagx1 == 0 && flagy1 == 0 && flagx2 == 0 && flagy2 == 0){
		return(INAREA);
	}
	if((flagx1 * flagx2) > 0 || (flagy1 * flagy2) > 0){
		return(OFF);
	}
	return(CROSS);
}

DllExport int scrnrltvarealegal(LPDBL sx, LPDBL sy, LPDBL ex, LPDBL ey)
{
	DBL	clipsx, clipsy, clipex, clipey;
	DBL	x1, y1, x2, y2;
	DBL	sub1, sub2, sub3;

	dPlgnThick = nPlgnThick;
	clipsx = drltvsx-dPlgnThick;
	clipsy = drltvsy-dPlgnThick;
	clipex = drltvex+dPlgnThick;
	clipey = drltvey+dPlgnThick;
	x1 = *sx; y1 = *sy;
	x2 = *ex; y2 = *ey;
	if(x1 < x2){
		if(x2 < clipsx)	return(OFF);
		if(x1 > clipex)	return(OFF);
		if(x1 <= clipsx){
			sub3 = clipsx - x1;
			sub1 = x2 - x1;
			if(y1 < y2){
				sub2 = y2 - y1;
				y1 += ((sub2 * sub3) / sub1);
			}
			else if(y1 > y2){
				sub2 = y1 - y2;
				y1 -= ((sub2 * sub3) / sub1);
			}
			x1 = clipsx;
		}
		if(x2 >= clipex){
			sub3 = x2 - clipex;
			sub1 = x2 - x1;
			if(y2 < y1){
				sub2 = y1 - y2;
				y2 += ((sub2 * sub3) / sub1);
			}
			else if(y2 > y1){
				sub2 = y2 - y1;
				y2 -= ((sub2 * sub3) / sub1);
			}
			x2 = clipex;
		}
	}
	else if(x1 > x2){
		if(x1 < clipsx)	return(OFF);
		if(x2 > clipex)	return(OFF);
		if(x2 <= clipsx){
			sub3 = clipsx - x2;
			sub1 = x1 - x2;
			if(y2 < y1){
				sub2 = y1 - y2;
				y2 += ((sub2 * sub3) / sub1);
			}
			else if(y2 > y1){
				sub2 = y2 - y1;
				y2 -= ((sub2 * sub3) / sub1);
			}
			x2 = clipsx;
		}
		if(x1 >= clipex){
			sub3 = x1 - clipex;
			sub1 = x1 - x2;
			if(y1 < y2){
				sub2 = y2 - y1;
				y1 += ((sub2 * sub3) / sub1);
			}
			else if(y1 > y2){
				sub2 = y1 - y2;
				y1 -= ((sub2 * sub3) / sub1);
			}
			x1 = clipex;
		}
	}
	else if(x1 == x2){
		if(x1 > clipex) return(OFF);
		if(x1 < clipsx) return(OFF);
	}
	if(y1 < y2){
		if(y2 < clipsy)	return(OFF);
		if(y1 > clipey)	return(OFF);

		if(y1 <= clipsy){
			sub3 = clipsy - y1;
			sub1 = y2 - y1;
			if(x1 < x2){
				sub2 = x2 - x1;
				x1 += ((sub2 * sub3) / sub1);
			}
			else if(x1 > x2){
				sub2 = x1 - x2;
				x1 -= ((sub2 * sub3) / sub1);
			}
			y1 = clipsy;
		}
		if(y2 >= clipey){
			sub3 = y2 - clipey;
			sub1 = y2 - y1;
			if(x2 < x1){
				sub2 = x1 - x2;
				x2 += ((sub2 * sub3) / sub1);
			}
			else if(x2 > x1){
				sub2 = x2 - x1;
				x2 -= ((sub2 * sub3) / sub1);
			}
			y2 = clipey;
		}
	}
	else if(y1 > y2){
		if(y1 < clipsy)	return(OFF);
		if(y2 > clipey)	return(OFF);
		if(y2 <= clipsy){
			sub3 = clipsy - y2;
			sub1 = y1 - y2;
			if(x2 < x1){
				sub2 = x1 - x2;
				x2 += ((sub2 * sub3) / sub1);
			}
			else if(x2 > x1){
				sub2 = x2 - x1;
				x2 -= ((sub2 * sub3) / sub1);
			}
			y2 = clipsy;
		}
		if(y1 >= clipey){
			sub3 = y1 - clipey;
			sub1 = y1 - y2;
			if(x1 < x2){
				sub2 = x2 - x1;
				x1 += ((sub2 * sub3) / sub1);
			}
			else if(x1 > x2){
				sub2 = x1 - x2;
				x1 -= ((sub2 * sub3) / sub1);
			}
			y1 = clipey;
		}
	}
	else if(y1 == y2){
		if(y1 > clipey) return(OFF);
		if(y1 < clipsy) return(OFF);
	}
	*sx = x1; *sy = y1;
	*ex = x2; *ey = y2;
	return(ON);
}

int scrnareasignget(DBL num, DBL mini, DBL max)
{
	if(num < mini){
		return(-1);
	}
	else if(num > max){
		return(1);
	}
	else{
		return(0);
	}
}
