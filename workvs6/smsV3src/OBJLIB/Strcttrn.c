
/*
	Easy Scanner システム共通ソース	1992.12.01	by stan
base		strcttrn.c	四角円弧変形処理関数
	Copyright 株）ハリマ
*/

#include	<windows.h>

#include	<stdio.h>
#include	<math.h>

#include	"..\stdef.h"
#include	"dlldef.h"

#include	"curvelib.fnc"
#include	"scrnlib.fnc"
#include	"dltlib.fnc"
#include	"strcttrn.fnc"

extern DBL	pai, hpai, degree1, degree01, degree5;

DBL	stbx[10], stby[10];		//ds 作業用変形要素点テーブル
DBL	stdx[10], stdy[10];		//ds 変形要素点テーブル

HDC	strctPaintDC;

DllExport void strctpaintdc(HDC hDC)
{
	strctPaintDC = hDC;
}

DllExport void strctboxsbasepointset(DBL x[], DBL y[])
{
	stbx[1] = x[0]; stby[1] = y[0];
	stbx[2] = x[1]; stby[2] = y[1];
	stbx[4] = x[2]; stby[4] = y[2];
	stbx[3] = x[3]; stby[3] = y[3];
}

DllExport void strctboxstrnspointset(DBL x[], DBL y[])
{
	stdx[1] = x[0]; stdy[1] = y[0];
	stdx[2] = x[1]; stdy[2] = y[1];
	stdx[4] = x[2]; stdy[4] = y[2];
	stdx[3] = x[3]; stdy[3] = y[3];
}

DllExport int strcttrnsboxspoint(DBL *x, DBL *y)
{
	DBL	dx, dy, tx, ty, dlt;
	DBL	x1, y1, x2, y2, x3, y3, x4, y4;
	int	ret;

	tx = *x - stbx[1];
	dx = stbx[2] - stbx[1];
	dlt = tx / dx;
	x1 = dlt * (stdx[2] - stdx[1]) + stdx[1];
	y1 = dlt * (stdy[2] - stdy[1]) + stdy[1];
	x2 = dlt * (stdx[4] - stdx[3]) + stdx[3];
	y2 = dlt * (stdy[4] - stdy[3]) + stdy[3];
	ty = *y - stby[1];
	dy = stby[3] - stby[1];
	dlt = ty / dy;
	x3 = dlt * (stdx[3] - stdx[1]) + stdx[1];
	y3 = dlt * (stdy[3] - stdy[1]) + stdy[1];
	x4 = dlt * (stdx[4] - stdx[2]) + stdx[2];
	y4 = dlt * (stdy[4] - stdy[2]) + stdy[2];
	ret = strclinecrossget(x1, y1, x2, y2, x3, y3, x4, y4, &dx, &dy);
	*x = dx; *y = dy;
	return(ret);
}


DllExport int strcconrarc(DBL *xa, DBL *ya, DBL *xb, DBL *yb, DBL *xc, DBL *yc, DBL *xd, DBL *yd, DBL cnrr)
{
	DBL		x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		x, y, x0, y0, xt, yt;
	DBL		rag12, rag43, rag;
	int		minus;
	double	len, dlen, ddotr, ttan;

	x1 = *xa; y1 = *ya;
	x2 = *xb; y2 = *yb;
	x3 = *xc; y3 = *yc;
	x4 = *xd; y4 = *yd;
	if(cnrr < 0.0){
		minus = ON;
		ddotr = cnrr * -1.0;
	}
	else{
		minus = OFF;
		ddotr = cnrr;
	}

	if(strclinecrossget(x1, y1, x2, y2, x3, y3, x4, y4, &x, &y) == FAIL){
		return(FAIL);
	}

	rag12 = atan2(y1 - y2, x1 - x2);
	rag43 = atan2(y4 - y3, x4 - x3);

	rag = rag43 - rag12;
	if(rag > pai){
		rag = rag - 2.0 * pai;
	}
	if(rag < -pai){
		rag = rag + 2.0 * pai;
	}
	rag = rag / 2.0;
	ttan = fabs(tan(rag));
	dlen = ddotr / ttan;

	len = hypot(x1 - x, y1 - y);
	x2 = ((dlen * (x1 - x)) / len) + x;
	y2 = ((dlen * (y1 - y)) / len) + y;

	len = hypot(x4 - x, y4 - y);
	x3 = ((dlen * (x4 - x)) / len) + x;
	y3 = ((dlen * (y4 - y)) / len) + y;

	xt = (x2 + x3) / 2;
	yt = (y2 + y3) / 2;
	len = hypot(x2 - xt, y2 - yt);
	dlen = ttan * len;
	len = hypot(xt - x, yt - y);
	x0 = (((dlen + len) * (xt - x)) / len) + x;
	y0 = (((dlen + len) * (yt - y)) / len) + y;


	if(minus == ON){
		x0 = xt * 2.0 - x0;
		y0 = yt * 2.0 - y0;
	}

	if(conrarcpoint(x2, y2, x3, y3, x0, y0, ddotr, &x, &y) == FAIL){
		return(FAIL);
	}

	*xa = x2; *ya = y2;
	*xb = x3; *yb = y3;
	*xc = x; *yc = y;
	return(0);
}

int conrarcpoint(DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0, DBL r, DBL *x, DBL *y)
{
	double	rag02, rag03;
	double	tsin, tcos;
	double	subrag, steprag, rag;
	double	tx, ty;

	if(y2 == y0 && x2 == x0){
		return(FAIL);
	}
	if(y3 == y0 && x3 == x0){
		return(FAIL);
	}
	rag02 = atan2(y2 - y0, x2 - x0);
	rag03 = atan2(y3 - y0, x3 - x0);
	subrag = rag03 - rag02;
	if(subrag > pai){
		subrag = subrag - 2.0 * pai;
	}
	else if(subrag < -pai){
		subrag = subrag + 2.0 * pai;
	}
	steprag = subrag / 2.0;
	rag = rag02+steprag;
	tsin = sin(rag);
	tcos = cos(rag);
	tx = tcos*r+x0;
	ty = tsin*r+y0;

	*x = tx;
	*y = ty;
	return(0);
}

DllExport int strclinecrossget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4, DBL *x, DBL *y)
{
	DBL	dx, a21, b21, a43, b43;
	/* y = a21 * x + b21 と y = a43 * x + b43 の交差する座標 */

	if(x2 == x1){
		if(x4 == x3){
			*x = 0; *y = 0;
			return(FAIL);
		}
		else{
			a43 = (y4 - y3) / (x4 - x3);
			b43 = y4 - a43*x4;
			*x = x2;
			*y = a43 * x2 + b43;
		}
	}
	else{
		if(x4 == x3){
			a21 = (y2 - y1) / (x2 - x1);
			b21 = y2 - a21*x2;
			*x = x4;
			*y = a21 * x4 + b21;
		}
		else{
			a21 = (y2 - y1) / (x2 - x1);
			b21 = y2 - a21*x2;
			a43 = (y4 - y3) / (x4 - x3);
			b43 = y4 - a43*x4;
			if(a21 == a43){
				return(FAIL);
			}
			dx = (b43 - b21) / (a21 - a43);
			*x = dx;
			*y = a21*(dx) + b21; 
		}
	}
	return(0);
}

void strctcrcllinecrosschk(DBL x0, DBL y0, DBL rag1, DBL rag2, DBL x1, DBL y1, DBL x3, DBL y3, DBL dx1, DBL dy1, DBL dx2, DBL dy2, DBL *dx, DBL *dy)
{
	DBL	tt, rag, mrag, prag;

	if(x1 > x3){
		tt = x1;
		x1 = x3;
		x3 = tt;
	}
	if(y1 > y3){
		tt = y1;
		y1 = y3;
		y3 = tt;
	}

	if(x1 > dx1 || x3 < dx1	|| y1 > dy1 || y3 < dy1){
		*dx = dx2;
		*dy = dy2;
	}
	else if(x1 > dx2 || x3 < dx2 || y1 > dy2 || y3 < dy2){
		*dx = dx1;
		*dy = dy1;
	}
	else{
		rag = atan2(dy1 - y0, dx1 - x0);
		mrag = rag - (pai * 2);
		prag = rag + (pai * 2);

		if(rag1 < rag2){
			if(rag2 > pai && rag < rag1){
				rag = rag + (pai * 2);
			}
			//if((rag1 <= rag && rag <= rag2)
			//|| (rag1 <= prag && prag <= rag2)
			//|| (rag1 <= mrag && mrag <= rag2)){
			if(rag1 <= rag && rag <= rag2){
				*dx = dx1;
				*dy = dy1;
			}
			else{
				*dx = dx2;
				*dy = dy2;
			}
		}
		else{
			if(rag2 < -pai && rag > rag1){
				rag = rag - (pai * 2);
			}
			//if((rag1 >= rag && rag >= rag2)
			//|| (rag1 >= prag && prag >= rag2)
			//|| (rag1 >= mrag && mrag >= rag2)){
			if(rag1 >= rag && rag >= rag2){
				*dx = dx1;
				*dy = dy1;
			}
			else{
				*dx = dx2;
				*dy = dy2;
			}
		}
	}
	return;
}

void strctcrclcrosschk(DBL xx0, DBL xy0, DBL xrag1, DBL xrag2, DBL yx0, DBL yy0, DBL yrag1, DBL yrag2, DBL dx1, DBL dy1, DBL dx2, DBL dy2, DBL *dx, DBL *dy)
{
	DBL	xrag, xmrag, xprag;
	DBL	yrag, ymrag, yprag;

	xrag = atan2(dy1 - xy0, dx1 - xx0);
	xmrag = xrag - (pai * 2);
	xprag = xrag + (pai * 2);
	yrag = atan2(dy1 - yy0, dx1 - yx0);
	ymrag = yrag - (pai * 2);
	yprag = yrag + (pai * 2);
	if(xrag1 < xrag2){
		if(xrag2 > pai && xrag < xrag1){
			xrag = xrag + (pai * 2);
		}
		//if((xrag1 <= xrag && xrag <= xrag2)
		//|| (xrag1 <= xprag && xprag <= xrag2)
		//|| (xrag1 <= xmrag && xmrag <= xrag2)){
		if(xrag1 <= xrag && xrag <= xrag2){
			*dx = dx1;
			*dy = dy1;
			return;
		}
	}
	else{
		if(xrag2 < -pai && xrag > xrag1){
			xrag = xrag - (pai * 2);
		}
		//if((xrag1 >= xrag && xrag >= xrag2)
		//|| (xrag1 >= xprag && xprag >= xrag2)
		//|| (xrag1 >= xmrag && xmrag >= xrag2)){
		if(xrag1 >= xrag && xrag >= xrag2){
			*dx = dx1;
			*dy = dy1;
			return;
		}
	}
	if(yrag1 < yrag2){
		if(yrag2 > pai && yrag < yrag1){
			yrag = yrag + (pai * 2);
		}
		//if((yrag1 <= yrag && yrag <= yrag2)
		//|| (yrag1 <= yprag && yprag <= yrag2)
		//|| (yrag1 <= ymrag && ymrag <= yrag2)){
		if(yrag1 <= yrag && yrag <= yrag2){
			*dx = dx1;
			*dy = dy1;
			return;
		}
	}
	else{
		if(yrag2 < -pai && yrag > yrag1){
			yrag = yrag - (pai * 2);
		}
		//if((yrag1 >= yrag && yrag >= yrag2)
		//|| (yrag1 >= yprag && yprag >= yrag2)
		//|| (yrag1 >= ymrag && ymrag >= yrag2)){
		if(yrag1 >= yrag && yrag >= yrag2){
			*dx = dx1;
			*dy = dy1;
			return;
		}
	}
	*dx = dx2;
	*dy = dy2;
}

void strctcrclcdtrns(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL dlt, DBL *dx, DBL *dy)
{
	DBL	x0, y0, rag1, rag2, rag, r, tcos, tsin;
	int		stat;

	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &rag1, &rag2, &r);
	}
	if(stat == FAIL){
		*dx = dlt * (x3 - x1) + x1;
		*dy = dlt * (y3 - y1) + y1;
	}
	else{
		rag = dlt * (rag2 - rag1) + rag1;
		tcos = cos(rag);
		tsin = sin(rag);
		*dx = r * tcos + x0;
		*dy = r * tsin + y0;
	}
}

int strccrclcross(DBL x01, DBL y01, DBL r1, DBL x02, DBL y02, DBL r2, DBL *x1, DBL *y1, DBL *x2, DBL *y2)
{
	DBL	x, y, q, t;
	DBL	a, b;
	
	x = x02 - x01;
	y = y02 - y01;
	q = sqrt(x*x + y*y);
	a = atan2(y, x);
	if(q == 0.0 || r1 == 0.0){
		return(FAIL);
	}
	t = (r1*r1 + q*q - r2*r2) / (2*r1*q);
	if(t > 1.0){
		t = 1.0;
	}
	if(t < -1.0){
		t = -1.0;
	}
	b = acos(t);
	*x1 = r1 * cos(a+b) + x01;
	*y1 = r1 * sin(a+b) + y01;
	*x2 = r1 * cos(a-b) + x01;
	*y2 = r1 * sin(a-b) + y01;
	return(0);
}

int strccrcllinecross(DBL x0, DBL y0, DBL r, DBL sx, DBL sy, DBL ex, DBL ey, DBL *x1, DBL *y1, DBL *x2, DBL *y2)
{
	DBL	m, n, x, y, t, t2, t3;

	if(ex == sx){
		x = sx - x0;
		t = r*r - x*x;
		if(t < 0){
			return(FAIL);
		}
		t = sqrt(t);
		*x1 = sx;
		*y1 = t + y0;
		*x2 = sx;
		*y2 = -t + y0;
	}
	else if(sy == ey){
		y = sy - y0;
		t = r*r - y*y;
		if(t < 0){
			return(FAIL);
		}
		t = sqrt(t);
		*x1 = t + x0;
		*y1 = sy;
		*x2 = -t + x0;
		*y2 = sy;
		
	}
	else{
		m = (ey - sy) / (ex - sx);
		x = sx - x0;
		y = sy - y0;
		n = x * m - y;
		t2 = m*m + 1;
		t = r*r * t2 - n*n;
		if(t < 0 || t2 == 0){
			return(FAIL);
		}
		t = sqrt(t);
		t3 = ((m*n + t) / t2);
		*x1 = t3 + x0;
		*y1 = t3 * m - n + y0;
		t3 = ((m*n - t) / t2);
		*x2 = t3 + x0;
		*y2 = t3 * m - n + y0;
	}
	return(0);
}

void strctDrawArcLine(DBL x0, DBL y0, DBL srag, DBL erag, DBL dr)
{
	POINT	point;
	DBL		steprag, rag;
	int		idx, count;
	DBL		dx, dy;
	int		nx, ny;

	count = 10;
	steprag = (erag - srag) / count;
	rag = srag;
	dx = cos(rag)*dr+x0;
	dy = sin(rag)*dr+y0;
	scrndxymmtorltv(&dx, &dy);
	nx = (int)dx; ny = (int)dy;
	MoveToEx(strctPaintDC, nx, ny, &point);
	for(idx = 1, rag = srag+steprag; idx < count; idx++, rag += steprag){
		dx = cos(rag)*dr+x0;
		dy = sin(rag)*dr+y0;
		scrndxymmtorltv(&dx, &dy);
		nx = (int)dx; ny = (int)dy;
		LineTo(strctPaintDC, nx, ny);
	}
	rag = erag;
	dx = cos(rag)*dr+x0;
	dy = sin(rag)*dr+y0;
	scrndxymmtorltv(&dx, &dy);
	nx = (int)dx; ny = (int)dy;
	LineTo(strctPaintDC, nx, ny);
}

void strctDrawPoint(DBL x, DBL y)
{
	POINT	point;
	int		sx, sy;
	int		ex, ey;

	scrndxymmtorltv(&x, &y);
	sx = (int)x-2; sy = (int)y-2;
	ex = (int)x+2; ey = (int)y+2;
	MoveToEx(strctPaintDC, sx, sy, &point);
	LineTo(strctPaintDC, sx, ey);
	LineTo(strctPaintDC, ex, ey);
	LineTo(strctPaintDC, ex, sy);
	LineTo(strctPaintDC, sx, sy);
}
