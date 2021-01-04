
#include	"stdafx.h"

#include	"Mathlib.h"


#define	MTRXCNT		4

DBL			x11, x12, x13, x14;
DBL			y11, y12, y13, y14;
DBL			x21, x22, x23, x24;
DBL			y21, y22, y23, y24;
DBL			xa, xb, xc, xd;
DBL			ya, yb, yc, yd;
int			areaflag;

DllExport void mathmtrxtrns(DBL *x, DBL *y)
{
	DBL	tx, ty;
	DBL	dx, dy;
	int	flag;

	tx = *x; ty = *y;

	flag = mathareachek(tx, ty);
	if(flag == areaflag){
		dx = xa * tx + xb * ty + xc * tx * ty + xd;
		dy = ya * tx + yb * ty + yc * tx * ty + yd;
	}
	else{
		dx = tx;
		dy = ty;
	}
	*x = dx; *y = dy;
}

void mathconstinit(DBL x1[], DBL y1[], DBL x2[], DBL y2[])
{
	DBL		mtrx[MTRXCNT*MTRXCNT];

	x11 = x1[0]; x12 = x1[1]; x13 = x1[2]; x14 = x1[3];
	y11 = y1[0]; y12 = y1[1]; y13 = y1[2]; y14 = y1[3];
	x21 = x2[0]; x22 = x2[1]; x23 = x2[2]; x24 = x2[3];
	y21 = y2[0]; y22 = y2[1]; y23 = y2[2]; y24 = y2[3];

	mtrx[0*MTRXCNT+0]=x11;
	mtrx[0*MTRXCNT+1]=y11;
	mtrx[0*MTRXCNT+2]=x11*y11;
	mtrx[0*MTRXCNT+3]=1.0;

	mtrx[1*MTRXCNT+0]=x12;
	mtrx[1*MTRXCNT+1]=y12;
	mtrx[1*MTRXCNT+2]=x12*y12;
	mtrx[1*MTRXCNT+3]=1.0;

	mtrx[2*MTRXCNT+0]=x13;
	mtrx[2*MTRXCNT+1]=y13;
	mtrx[2*MTRXCNT+2]=x13*y13;
	mtrx[2*MTRXCNT+3]=1.0;

	mtrx[3*MTRXCNT+0]=x14;
	mtrx[3*MTRXCNT+1]=y14;
	mtrx[3*MTRXCNT+2]=x14*y14;
	mtrx[3*MTRXCNT+3]=1.0;

	mathgetgyakumtrx(mtrx, MTRXCNT);

	xa = mtrx[0*MTRXCNT+0]*x21+mtrx[0*MTRXCNT+1]*x22
		+mtrx[0*MTRXCNT+2]*x23+mtrx[0*MTRXCNT+3]*x24;

	xb = mtrx[1*MTRXCNT+0]*x21+mtrx[1*MTRXCNT+1]*x22
		+mtrx[1*MTRXCNT+2]*x23+mtrx[1*MTRXCNT+3]*x24;

	xc = mtrx[2*MTRXCNT+0]*x21+mtrx[2*MTRXCNT+1]*x22
		+mtrx[2*MTRXCNT+2]*x23+mtrx[2*MTRXCNT+3]*x24;

	xd = mtrx[3*MTRXCNT+0]*x21+mtrx[3*MTRXCNT+1]*x22
		+mtrx[3*MTRXCNT+2]*x23+mtrx[3*MTRXCNT+3]*x24;


	ya = mtrx[0*MTRXCNT+0]*y21+mtrx[0*MTRXCNT+1]*y22
		+mtrx[0*MTRXCNT+2]*y23+mtrx[0*MTRXCNT+3]*y24;

	yb = mtrx[1*MTRXCNT+0]*y21+mtrx[1*MTRXCNT+1]*y22
		+mtrx[1*MTRXCNT+2]*y23+mtrx[1*MTRXCNT+3]*y24;

	yc = mtrx[2*MTRXCNT+0]*y21+mtrx[2*MTRXCNT+1]*y22
		+mtrx[2*MTRXCNT+2]*y23+mtrx[2*MTRXCNT+3]*y24;

	yd = mtrx[3*MTRXCNT+0]*y21+mtrx[3*MTRXCNT+1]*y22
		+mtrx[3*MTRXCNT+2]*y23+mtrx[3*MTRXCNT+3]*y24;

	areaflag = 0;
	areaflag = mathareachek((x11+x13)/2.0, (y11+y13)/2.0);
}

void mathgetgyakumtrx(DBL mtrx[], int cnt)
{
	int		tt, row, clm, cnt2;
	DBL		mt[MTRXCNT*MTRXCNT*2];
	DBL		tc;

	cnt2 = cnt*2;

	for(row = 0; row < cnt; row++){
		for(clm = 0; clm < cnt; clm++){
			mt[row*cnt2+clm] = mtrx[row*cnt+clm];
			if(row == clm){
				mt[row*cnt2 + cnt + clm] = 1;
			}
			else{
				mt[row*cnt2 + cnt + clm] = 0;
			}
		}
	}

	for(tt = 0; tt < cnt; tt++){
		row = tt; clm = tt;
		if(mt[tt*cnt2 + tt] == 0){
			return;
		}
		tc = 1.0 / mt[tt*cnt2 + tt];
		for(clm = 0; clm < cnt2; clm++){
			mt[row*cnt2 + clm] *= tc;
		}
		for(row = 0; row < cnt; row++){
			if(tt == row){
				continue;
			}
			clm = tt;
			tc = -mt[row*cnt2+tt];
			for(clm = tt; clm < cnt2; clm++){
				mt[row*cnt2+clm] += (tc*mt[tt*cnt2+clm]);
			}
		}
	}

	for(row = 0; row < cnt; row++){
		for(clm = 0; clm < cnt; clm++){
			mtrx[row*cnt+clm] = mt[row*cnt2+cnt+clm];
		}
	}
}

int mathareachek(DBL tx, DBL ty)
{
	DBL	ans;
	int	nz, tnz;

	ans = (x12-x11)*(ty-y12)-(y12-y11)*(tx-x12);
	nz = mathareasigneget(ans);
	ans = (x13-x12)*(ty-y13)-(y13-y12)*(tx-x13);
	tnz = mathareasigneget(ans);
	if(nz != tnz){
		return(0);
	}
	ans = (x14-x13)*(ty-y14)-(y14-y13)*(tx-x14);
	tnz = mathareasigneget(ans);
	if(nz != tnz){
		return(0);
	}
	ans = (x11-x14)*(ty-y11)-(y11-y14)*(tx-x11);
	tnz = mathareasigneget(ans);
	if(nz != tnz){
		return(0);
	}
	return(nz);
}

int mathareasigneget(DBL ans)
{
	if(ans > 0){
		return(1);
	}
	else if(ans < 0){
		return(-1);
	}
	else{
		return(0);
	}
}
