
/*
	Easy Scanner システム共通ソース	1992.12.01	by stan
sbas	sbasmdfy.c	線抽出正規化処理
	Copyright 株）ハリマ
*/

#include	<windows.h>
#include	<stdio.h>
#include	<math.h>

#include	"..\stdef.h"
#include	"dlldef.h"

#include	"mdfylib.fnc"

#define	iabs(x)	(((x) > 0) ? (x) : (-(x)))

DBL	degree1;

int	furelimit = 1;	//ds 粗さ初期値の定義
DBL	flimit;		//d 粗さ基準値
int	limit;			//ds リミット値
int	linelimit; 		//ds ラインリミット値

DllExport void mdfyinit(DBL setlimit)
{
	degree1 = M_PI_2 / 90.0;
	furelimit = (int)setlimit;
	flimit = setlimit;
	limit = (int)setlimit;
	linelimit = (int)setlimit * 2;
}

DllExport int mdfyfirst(XYType sp[], int smax, XYType dp[])
{
	int		di, i, ret;
	int		sx, sy, ex, ey;
	int		subx, suby;

	di = 0;
	for(i = 0; i < smax; i++){
		dp[di].x = sp[i].x;
		dp[di].y = sp[i].y;
		di++;
		ret = mdfystartendget(sp, smax, i); 
		if(ret){
			if(mdfychkline(sp, i, ret) == OFF){
				i = ret - 1;
			}
		}
	}

	sx = dp[0].x;
	sy = dp[0].y;
	for(i = 1; i < di; i++){
		ex = dp[i].x;
		ey = dp[i].y;
		subx = ex - sx;
		suby = ey - sy;
		if(abs(subx) == 1 && abs(suby) > 2){
			dp[i].x = dp[i-1].x;					
		}
		else if(abs(subx) > 2 && abs(suby) == 1){
			dp[i].y = dp[i-1].y;					
		}
		sx = ex;
		sy = ey;
	}
	return(di);
}

//p 点抽出処理２関数
DllExport int mdfysecond(XYType sp[], int smax, XYType dp[])
{
	int	di, i, ret;

	di = 0;
	for(i = 0; i < smax; i++){
		dp[di].x = sp[i].x;
		dp[di].y = sp[i].y;
		di++;
		ret = mdfystartendget(sp, smax, i); 
		if(ret){
			if(mdfychkline(sp, i, ret) == OFF){
				i = ret - 1;
			}
		}
	}
	return(di);
}

//p 直線性をチェックする関数
int mdfychkline(XYType sp[], int si, int ei)
{
	int	subx, suby;

	if(ei - si < 3){
		return(OFF);
	}
	subx = iabs(sp[si+1].x - sp[si].x);
	suby = iabs(sp[si+1].y - sp[si].y);
	if(subx <= limit && suby <= limit){
		if(sp[si+1].x == sp[ei-1].x
		&& iabs(sp[si+1].y - sp[ei-1].y) > linelimit){
			return(ON);
		}
		else if(sp[si+1].y == sp[ei-1].y
		&& iabs(sp[si+1].x - sp[ei-1].x) > linelimit){
			return(ON);
		}
	}
	return(OFF);
}

//p 直線の始点終点抽出処理関数
int mdfystartendget(XYType sp[], int smax, int si)
{
	int	start, end;
	int	sx, sy, ex, ey;
	int	i, ret, cnt;

	start = si;
	sx = sp[start].x;
	sy = sp[start].y;
	end = 0;
	if(start + 2 >= smax){
		return(0);
	}
	for(i = start+2, cnt = 0; i < smax; i++, cnt++){
		ex = sp[i].x;
		ey = sp[i].y;
		ret = mdfychkfure(sx, sy, ex, ey, sp, start, i);
		if(ret == ON){
			end = i;
		}
		else if(ret == -1){
			break;
		}
		if(cnt == 10){
			break;
		}
	}
	return(end);
}

//p 直線からの距離をチェックする関数
int mdfychkfure(int sx, int sy, int ex, int ey, XYType sp[], int start, int end)
{
	int	brag, rag, subrag;
	DBL		a, b;
	DBL		c;
	DBL		aabb, cmp, t, tt;
	int	i, cnt, flag;

	a = ey - sy;
	b = sx - ex;
	c = -(a*sx + b*sy);
	if((ey - sy) != 0 && (ex - sx) != 0){
		brag = (int)(atan2((DBL)(ey - sy), (DBL)(ex - sx))/degree1);
	}
	else{
		brag = 0;
	}
	flag = 5;
	aabb = a*a + b*b;
	cmp = (DBL)limit*(DBL)limit*aabb;
	cnt = 0;
	for(i = start+1, cnt = 0; i < end; i++, cnt++){
		if((sp[i].y - sy) != 0 && (sp[i].y - sx) != 0){
			rag = (int)(atan2((DBL)(sp[i].y-sy), (DBL)(sp[i].x-sx))/degree1);
		}
		else{
			rag = 0;
		}
		subrag = (rag - brag) % 360;
		t = a*(DBL)(sp[i].x) + b*(DBL)(sp[i].y) + c;
		tt = t*t;
		if(tt > cmp){
			return(-1);
		}
		if((subrag < 90 && subrag > 0)
		|| (subrag < -270 && subrag > -360)){
			if(flag < 0){
				flag = 0;
			}
			flag++;	
		}
		else if((subrag > 270 && subrag < 360)
		     || (subrag > -90 && subrag < 0)){
			if(flag > 0){
				flag = 0;
			}
			flag--;
		}
		else if(subrag == 0){
			flag = 0;
		}
		else{
			return(-1);
		}
	}
	if(cnt == 1){
		if(flag == 0){
			return(ON);
		}
		return(OFF);
	}
	if(flag > 1 || flag < -1){
		return(OFF);
	}
	return(ON);
}

int mdfyoverlap(XYType sp[], int smax, XYType dp[])
{
	int	si, di;
	int	x, y, rsx, rsy, bx, by;

	rsx = bx = sp[0].x;
	rsy = by = sp[0].y;
	dp[0].x = bx;
	dp[0].y = by;
	for(si = 1, di = 1; si < smax; si++){
		x = sp[si].x;
		y = sp[si].y;
		if(x != bx || y != by){
			dp[di].x = x;
			dp[di].y = y;
			di++;
		}
		bx = x; by = y;
	}
	if(dp[di-1].x == rsx && dp[di-1].y == rsy){
		di--;
	}
	return(di);
}

int mdfycorner(XYType sp[], int smax, XYType dp[])
{
	int	i, j, si, di;
	int	skipcnt;
	int	x1, y1, x2, y2, subx, suby;

	if(smax <= 4){
		for(i = 0; i < smax; i++){
			dp[i].x = sp[i].x;
			dp[i].y = sp[i].y;
		}
		return(smax);
	}

	for(si = 0; si < smax-1; si++){
		subx = iabs(sp[si+1].x - sp[si].x);
		suby = iabs(sp[si+1].y - sp[si].y);
		if(subx > limit || suby > limit){
			break;
		}
	}
	if(si == smax - 1){
		for(i = 0; i < smax; i++){
			dp[i].x = sp[i].x;
			dp[i].y = sp[i].y;
		}
		return(smax);
	}
	else if(si != 0){
		for(i = 0, j = si; j < smax; i++, j++){
			dp[i].x = sp[j].x;
			dp[i].y = sp[j].y;
		}
		for(j = 0; j < si; i++, j++){
			dp[i].x = sp[j].x;
			dp[i].y = sp[j].y;
		}
		for(i = 0; i < smax; i++){
			sp[i].x = dp[i].x;
			sp[i].y = dp[i].y;
		}
	}
	
	di = 0;
	for(i = 0; i < smax; i++){
		skipcnt = mdfycornerchk(sp,smax,dp,di,i,&x1,&y1,&x2,&y2);
		if(skipcnt != 0){
			dp[di].x = x1; dp[di].y = y1;
			i += skipcnt;
			sp[i].x = x1; sp[i].y = y1;
			if(x2 != -1){
				di++;
				dp[di].x = x2; dp[di].y = y2;
				sp[i].x = x2; sp[i].y = y2;
			}
		}
		else{
			dp[di].x = sp[i].x; dp[di].y = sp[i].y;
		}
		di++;
	}
	if(i == smax+1){
		dp[0].x = dp[di-1].x;
		dp[0].y = dp[di-1].y;
		di--;
	}
	return(di);
}

int mdfycornerchk(XYType sp[], int smax, XYType dp[], int dmax, int p1, int *x, int *y, int *xx, int *yy)
{
	int	p2, p3;
	int	x0, y0, x1, y1, x2, y2, x3, y3;
	int	subx0, suby0, subx1, suby1;
	int	rag0, rag1, subrag;
	double	len0, len1;
	int	skipcnt;

	p2 = nextpget(sp, smax, dp, dmax, p1);
	if(p2 == p1){
		return(0);
	}
	if(p2 == 0){
		skipcnt = smax - p1;
	}
	else{
		skipcnt = p2 - p1;
	}
	if(dmax != 0){
		x0 = dp[dmax-1].x;
		y0 = dp[dmax-1].y;
	}
	else{
		x0 = sp[p1-1].x;
		y0 = sp[p1-1].y;
	}
	x1 = sp[p1].x; y1 = sp[p1].y;
	p3 = p2 + 1;
	if(p2 == smax-1){
		x2 = sp[p2].x; y2 = sp[p2].y;
		if(dmax != 0){
			x3 = dp[0].x; y3 = dp[0].y;
		}
		else{
			x3 = sp[0].x; y3 = sp[0].y;
		}
	}
	else if(p2 == 0){
		if(dmax != 0){
			x2 = dp[0].x; y2 = dp[0].y;
		}
		else{
			x2 = sp[0].x; y2 = sp[0].y;
		}
		if(dmax > 1){
			x3 = dp[1].x; y3 = dp[1].y;
		}
		else{
			x3 = sp[1].x; y3 = sp[1].y;
		}
	}
	else{
		x2 = sp[p2].x; y2 = sp[p2].y;
		x3 = sp[p3].x; y3 = sp[p3].y;
	}
	subx0 = x1 - x0; suby0 = y1 - y0;
	subx1 = x3 - x2; suby1 = y3 - y2;
	if(suby0 != 0 && subx0 != 0){
		rag0 = (int)(atan2((DBL)suby0, (DBL)subx0)/degree1);
	}
	else{
		rag0 = 0;
	}
	if(suby1 != 0 && subx1 != 0){
		rag1 = (int)(atan2((DBL)suby1, (DBL)subx1)/degree1);
	}
	else{
		rag1 = 0;
	}
	subx0 = iabs(subx0);
	suby0 = iabs(suby0);
	subx1 = iabs(subx1);
	suby1 = iabs(suby1);
	*xx = *yy = -1;
	if(subx0 <= linelimit && suby0 <= linelimit){
		return(0);
	}
	if(subx0 <= limit && suby1 <= limit){
		*x = x0; *y = y3;
		return(skipcnt);
	}
	if(suby0 <= limit && subx1 <= limit){
		*x = x3; *y = y0;
		return(skipcnt);
	}
	if(subx0 == 0 || suby0 == 0){
		*x = x1; *y = y1;
		return(skipcnt);
	}
	if(subx1 == 0 || suby1 == 0){
		*x = x2; *y = y2;
		return(skipcnt);
	}
	subrag = iabs(rag1 - rag0);
	if(subrag > 100 && subrag < 260){
		*x = x1; *y = y1;
		*xx = x2; *yy = y2;
		return(skipcnt);
	}
	len0 = hypot((double)(subx0), (double)(suby0));
	len1 = hypot((double)(subx1), (double)(suby1));
	if(len0 < len1){
		*x = x2; *y = y2;
		return(skipcnt);
	}
	else{
		*x = x1; *y = y1;
		return(skipcnt);
	}
}

int nextpget(XYType sp[], int smax, XYType dp[], int dmax, int p1)
{
	int	i, x0, y0, x1, y1, subx, suby;

	x0 = sp[p1].x; y0 = sp[p1].y;
	for(i = p1+1; i < smax; i++){
		subx = iabs(sp[i].x - x0);
		suby = iabs(sp[i].y - y0);
		if(subx > linelimit || suby > linelimit){
			return(i-1);
		}
	}
	if(i == smax){
		if(dmax != 0){
			x1 = dp[0].x; y1 = dp[0].y;
		}
		else{
			x1 = sp[0].x; y1 = sp[0].y;
		}
		subx = iabs(x1 - x0);
		suby = iabs(y1 - y0);
		if(subx > linelimit || suby > linelimit){
			return(smax-1);
		}
		return(0);
	}
	return(0);
}

int mdfyburepoint(XYType sp[], int smax, XYType dp[])
{
	int	subrag0, subrag1, subrag2, subrag3;
	int	fugou0, fugou1, fugou2, fugou3;
	int	i, di, sumrag, flag;

	di = 0;
	sumrag = 0;
	flag = OFF;
	for(i = 0; i < smax; i++){
		subrag0 = mdfysubragget(sp, smax, i-1);
		subrag1 = mdfysubragget(sp, smax, i);
		subrag2 = mdfysubragget(sp, smax, i+1);
		subrag3 = mdfysubragget(sp, smax, i+2);
		fugou0 = (subrag0 < 0)?1:-1;
		fugou1 = (subrag1 < 0)?1:-1;
		fugou2 = (subrag2 < 0)?1:-1;
		fugou3 = (subrag3 < 0)?1:-1;
		if(iabs(subrag1) <= 5){
			sumrag += subrag1;
			if(iabs(sumrag) > 5){
				dp[di].x = sp[i].x;
				dp[di].y = sp[i].y;
				di++;
				sumrag = 0;
				flag = OFF;
			} 
			else{
				flag = ON;
			}
		}
		else if(fugou0 == fugou2 && fugou0 == fugou3
		&& fugou0 != fugou1){
			if(mdfyisshoutline(sp, smax, i-1) == OFF
			&& mdfyisshoutline(sp, smax, i) == OFF){
				dp[di].x = sp[i].x;
				dp[di].y = sp[i].y;
				di++;
				sumrag = 0;
				flag = OFF;
			}
			else if(flag == ON){
				dp[di].x = sp[i].x;
				dp[di].y = sp[i].y;
				di++;
				sumrag = 0;
				flag = OFF;
			}
			else{
				flag = ON;
			}
		}
		else{
			dp[di].x = sp[i].x;
			dp[di].y = sp[i].y;
			di++;
			sumrag = 0;
			flag = OFF;
		}
	}
	return(di);
}

int mdfysubragget(XYType sp[], int smax, int p1)
{
	int	p0, p2, rag1, rag2, subrag;

	if(p1 <= 0){
		p0 = p1 + smax - 1;
		if(p1 < 0){
			p1 += smax;
		}
	}
	else{
		p0 = p1 - 1;
	}
	if(p1 >= smax - 1){
		p2 = p1 - smax + 1;
		if(p1 == smax){
			if(p1 > smax){
				p0 = p1 - smax - 1;
			}
			p1 -= smax;
		}
	}
	else{
		p2 = p1 + 1;
	}
	if((sp[p1].y-sp[p0].y) != 0 && (sp[p1].x-sp[p0].x) != 0){
		rag1 = (int)(atan2((DBL)(sp[p1].y-sp[p0].y), (DBL)(sp[p1].x-sp[p0].x))/degree1);
	}
	else{
		rag1 = 0;
	}
	if((sp[p2].y-sp[p1].y) != 0 && (sp[p2].x-sp[p1].x) != 0){
		rag2 = (int)(atan2((DBL)(sp[p2].y-sp[p1].y), (DBL)(sp[p2].x-sp[p1].x))/degree1);
	}
	else{
		rag2 = 0;
	}
	subrag = rag2 - rag1;
	if(subrag > 180){
		subrag -= 360;
	}
	if(subrag < -180){
		subrag += 360;
	}
	return(subrag);
}

int mdfyisshoutline(XYType sp[], int smax, int p1)
{
	int	p2;
	double	len1;

	if(p1 >= smax - 1){
		p2 = p1 - smax + 1;
		if(p1 >= smax){
			p1 -= smax;
		}
	}
	else{
		p2 = p1 + 1;
	}
	if(p1 < 0){
		if(p1 < -1){
			p2 = p1 + smax + 1;
		}
		p1 += smax;
	}
	len1 = hypot((DBL)(sp[p2].x-sp[p1].x), (DBL)(sp[p2].y-sp[p1].y));
	if(len1 < 10.0){
		return(ON);
	}
	return(OFF);
}

