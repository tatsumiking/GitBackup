/*
	Easy Scanner システム共通ソース	1992.12.01	by stan
base		outlnlib.c	縁取り関係関数
	Copyright 株）ハリマ
*/

#include	<windows.h>
#include	<stdio.h>
#include	<math.h>
#include	"..\stdef.h"
#include	"dlldef.h"

#include    "plgnfill.fnc"
#include	"outlnlib.fnc"

DllExport int outlnlibcheckragsharp(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL		rag, rag1, rag2;
	DBL		degree60;

	degree60 = M_PI / 3.0;
	if((x1 == x2 && y1 == y2)
	|| (x2 == x3 && y2 == y3)){
		return(ON);
	}
	rag1 = atan2(y1-y2, x1-x2);
	rag2 = atan2(y3-y2, x3-x2);
	rag = rag2 - rag1;
	if(rag > M_PI){
		rag = rag - M_PI * 2.0;
	}
	if(rag < -M_PI){
		rag = rag + M_PI * 2.0;
	}
	if((-degree60) < rag && rag < (degree60)){
		return(ON);
	}
	return(OFF);
}

//p 開始縁取り点を求める関数
DllExport void outlnlibstartpoint(DBL dot, DBL x1, DBL y1, DBL x2, DBL y2, DBL *x, DBL *y)
//a DBL dot	//縁取り幅
//a DBL x1,y1 	//１番目の点の座標
//a DBL x2,y2	//２番目の点の座標
//a DBL *x, *y	//縁取り点
//r void		//なし
{
	DBL	x21, y21;
	DBL	rag21;

	outlnlibgetroundrag(x1, y1, x2, y2, &rag21);
	outlnlibsetroundpoint(x1, y1, rag21, 0.0, dot, &x21,  &y21);
	*x = x21; *y = y21;
}

//p 終了縁取り点を求める関数
DllExport void outlnlibendpoint(DBL dot, DBL x1, DBL y1, DBL x2, DBL y2, DBL *x, DBL *y)
//a DBL dot	//縁取り幅
//a DBL x1,y1 	//１番目の点の座標
//a DBL x2,y2	//２番目の点の座標
//a DBL *x, *y	//縁取り点
//r void		//なし
{
	DBL	x21, y21;
	DBL	rag21;

	outlnlibgetroundrag(x1, y1, x2, y2, &rag21);
	outlnlibsetroundpoint(x2, y2, rag21, 0.0, dot, &x21,  &y21);
	*x = x21; *y = y21;
}

//p ふちどりの点を設定する関数
DllExport void outlnlibtreepoint(int atr, DBL dot, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL *x, DBL *y)
//a DBL dot		//ドット数
//a DBL x1,y1 	//１番目の点の座標
//a DBL x2,y2	//２番目の点の座標
//a DBL x3,y3	//３番目の点の座標
//a DBL *x,*y	//ふちどり点の座標
//r void		//なし
{
	DBL	x21, y21, x23, y23, dx, dy;
	DBL	rag21, rag23;

	outlnlibgetroundrag(x1, y1, x2, y2, &rag21);
	outlnlibsetroundpoint(x2, y2, rag21, 0.0, dot, &x21,  &y21);
    if(x2 == x3 && y2 == y3){
        *x = x21;
        *y = y21;
        return;
    }
	outlnlibgetroundrag(x2, y2, x3, y3, &rag23);
	outlnlibsetroundpoint(x2, y2, rag23, 0.0, dot, &x23, &y23);
    if(x1 == x2 && y1 == y2){
        *x = x23;
        *y = y23;
        return;
    }
	outlnlibgetcrosspoint(x21, y21, rag21, x23, y23, rag23, &dx, &dy);
	outlnliblegallength(atr, dot, x2, y2, &dx, &dy);
	*x = dx; *y = dy;
}

//p 頂点からの角度を求める関数
DllExport void outlnlibgetroundrag(DBL x0, DBL y0, DBL x1, DBL y1, DBL *rag)
//a DBL x0,y0	//０番目の点の座標
//a DBL x1,y1	//１番目の点の座標
//a DBL *rag		//角度
//r void		//なし
{
	if(x0 == x1 && y0 == y1){
		*rag = 0.0;
	}
	else{
		*rag = atan2(y1-y0, x1-x0);
	}
}

//p ２点を反対側に延ばし点を求める関数
DllExport void outlnlibsetroundpoint(DBL x0, DBL y0, DBL rag, DBL x1, DBL y1, DBL *x, DBL *y)
//a DBL x0, y0	//１点目の座標
//a DBL rag		//角度
//a DBL x1, y1	//２点目の座標
//a DBL *x, *y	//延ばした点の座標
//r void		//なし
{
	DBL	tcos, tsin;

	tcos = cos(rag);
	tsin = sin(rag);
	*x = x1 * tcos - y1 * tsin + x0;
	*y = y1 * tcos + x1 * tsin + y0;
}

//p 縁取り交点を求める関数
DllExport void outlnlibgetcrosspoint(DBL x1, DBL y1, DBL rag1, DBL x2, DBL y2, DBL rag2, DBL *x, DBL *y)
//a DBL x1, y1	//１点目の座標
//a DBL rag1		//角度
//a DBL x2, y2	//２点目の座標
//a DBL rag2		//角度
//a DBL *x, *y	//延ばした点の座標
//r void		//なし
{
	DBL	a1, a2, b1, b2;

	a1 = tan(rag1);
	a2 = tan(rag2);
	if(a1 > 100000.0 || a1 < -100000.0){
		b1 = x1;
		if(a2 > 100000.0 || a2 < -100000.0){
			*x = (x1 + x2) / 2.0;
			*y = (y1 + y2) / 2.0;
			return;
		}
		b2 = y2 - a2*x2;
		b1 = x1;
		*x = b1;
		*y = a2 * b1 + b2;
		return;
	}
	if(a2 > 100000.0 || a2 < -100000.0){
		b1 = y1 - a1*x1;
		b2 = x2;
		*x = b2;
		*y = a1 * b2 + b1;
		return;
	}
	b1 = y1 - a1*x1;
	b2 = y2 - a2*x2;
	if(a1 == a2){
		*x = (x1 + x2) / 2.0;
		*y = (y1 + y2) / 2.0;
	}
	else{
		*x = (b1 - b2) / (a2 - a1);
		*y = (b1*a2 - b2*a1)/(a2-a1);
	}
	if((*x) < -50000.0 || 50000.0 < (*x)
	|| (*y) < -50000.0 || 50000.0 < (*y)){
		*x = (x1 + x2) / 2.0;
		*y = (y1 + y2) / 2.0;
	}
}

//p 縁取り点範囲内に治まるように処理する関数
DllExport void outlnliblegallength(int atr, double dot, double x0, double y0, double *dx, double *dy)
//a int atr		//点属性
//a double dot	//縁取り幅
//a double x0,y0 	//元の点
//a double *dx,*dy	//縁取り点
//r void		//なし
{
	double	x, y;
	double	len;

	x = (*dx) - x0; y = (*dy) - y0;
	len = hypot(x, y);
	if(len == 0){
		return;
	}
	dot = fabs(dot);
	if(atr & CONTATR){
		*dx = ((x * dot) / len) + x0;
		*dy = ((y * dot) / len) + y0;
	}
	else if(len > fabs(dot * 2.0)){
		*dx = ((x * (dot * 2.0)) / len) + x0;
		*dy = ((y * (dot * 2.0)) / len) + y0;
	}
}

DllExport void outlnlibfilldrowoutbeze(HDC hDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL	t, t1, b1, b2, b3, b4;
	DBL	sx, sy, ex, ey;

	sx = x1; sy = y1;
	for(t = 0.1; t < 1.0; t += 0.1){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		ex = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ey = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
	 	outlnlibfilldrowoutline(hDC, sx, sy, ex, ey);
		sx = ex; sy = ey;
	}
 	outlnlibfilldrowoutline(hDC, sx, sy, x4, y4);
}

DllExport void outlnlibfilldrowoutenko(HDC hDC, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL dr, DBL srag, DBL erag)
{
	DBL		rag, degree5, steprag;
	DBL		sx, sy, ex, ey;
	int		count, idx;

	degree5 = M_PI / 36.0;
	count = (int)((erag - srag) / degree5);
	if(count < 5){
		count = 5;
	}
	steprag = (erag - srag) / (DBL)count;

	sx = x1; sy = y1;
	for(idx = 1, rag = srag+steprag; idx < count; idx++, rag += steprag){
		ex = cos(rag)*dr+x0;
		ey = sin(rag)*dr+y0;
	 	outlnlibfilldrowoutline(hDC, sx, sy, ex, ey);
		sx = ex; sy = ey;
	}
	outlnlibfilldrowoutline(hDC, sx, sy, x2, y2);
}

DllExport void outlnlibfilldrowoutmove(HDC hDC, DBL dx, DBL dy)
{
	if(dx < -INTDLIMIT || INTDLIMIT < dx){
		return;
	}
	if(dy < -INTDLIMIT || INTDLIMIT < dy){
		return;
	}
	plgnfillmoveto(hDC, (int)dx, (int)dy);
}

DllExport void outlnlibfilldrowoutline(HDC hDC, DBL x1, DBL y1, DBL x2, DBL y2)
{
	if(x2 < -INTDLIMIT || INTDLIMIT < x2){
		return;
	}
	if(y2 < -INTDLIMIT || INTDLIMIT < y2){
		return;
	}
	plgnfilllineto(hDC, (int)x2, (int)y2);
}

DllExport void outlnlibdrowoutbeze(HDC hDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL	t, t1, b1, b2, b3, b4;
	DBL	sx, sy, ex, ey;

	sx = x1; sy = y1;
	for(t = 0.1; t < 1.0; t += 0.1){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		ex = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ey = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
	 	outlnlibdrowoutline(hDC, sx, sy, ex, ey);
		sx = ex; sy = ey;
	}
 	outlnlibdrowoutline(hDC, sx, sy, x4, y4);
}

DllExport void outlnlibdrowoutenko(HDC hDC, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL dr, DBL srag, DBL erag)
{
	DBL		rag, degree5, steprag;
	DBL		sx, sy, ex, ey;
	int		count, idx;

	degree5 = M_PI / 36.0;
	count = (int)((erag - srag) / degree5);
	if(count < 5){
		count = 5;
	}
	steprag = (erag - srag) / (DBL)count;

	sx = x1; sy = y1;
	for(idx = 1, rag = srag+steprag; idx < count; idx++, rag += steprag){
		ex = cos(rag)*dr+x0;
		ey = sin(rag)*dr+y0;
	 	outlnlibdrowoutline(hDC, sx, sy, ex, ey);
		sx = ex; sy = ey;
	}
	outlnlibdrowoutline(hDC, sx, sy, x2, y2);
}

DllExport void outlnlibdrowoutmove(HDC hDC, DBL dx, DBL dy)
{
	POINT	Point;

	if(dx < -INTDLIMIT || INTDLIMIT < dx){
		return;
	}
	if(dy < -INTDLIMIT || INTDLIMIT < dy){
		return;
	}
	MoveToEx(hDC, (long)dx, (long)dy, &Point);
}

DllExport void outlnlibdrowoutline(HDC hDC, DBL x1, DBL y1, DBL x2, DBL y2)
{
	if(x2 < -INTDLIMIT || INTDLIMIT < x2){
		return;
	}
	if(y2 < -INTDLIMIT || INTDLIMIT < y2){
		return;
	}
	LineTo(hDC, (long)x2, (long)y2);
}

