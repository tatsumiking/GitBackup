//
// Easy Scanner System Ver1.0
//
// ess  crosslib.c	曲線処理関数
//
//  Copyright (c) 1998 by THEON


#include	<windows.h>

#include	<stdio.h>
#include	<math.h>

#include	"..\stdef.h"
#include	"dlldef.h"

#include	"pdrwlib.fnc"
#include	"curvelib.fnc"
#include	"crosslib.fnc"

#define	BEZEDIVMAX	10

extern DBL	pai, hpai, degree1, degree01, degree5;

DllExport void crossinit()
{
}

DllExport dbezetype TransFreecurveBezier(dcurvetype *pFc)
{
	dbezetype result;
	DBL rag1, rag2, xp1, yp1, xp2, yp2;

	rag1 = pdrwstartragget(pFc->atr0, pFc->atr1, pFc->x0, pFc->y0, pFc->x1, pFc->y1, pFc->x2, pFc->y2);
	rag2 = pdrwendragget(pFc->atr1, pFc->atr2, pFc->x1, pFc->y1, pFc->x2, pFc->y2, pFc->x3, pFc->y3);

	if(rag1 == NORAG && rag2 == NORAG){
		exit(1);
	}

	curvenoragtocurverag(&rag1, &rag2);

	curvebezecontrolpointget(pFc->x1, pFc->y1, pFc->x2, pFc->y2, rag1, &xp1, &yp1);
	curvebezecontrolpointget(pFc->x2, pFc->y2, pFc->x1, pFc->y1, rag2, &xp2, &yp2);

	(result.x1) = (pFc->x1);
	(result.y1) = (pFc->y1);
	(result.x2) = xp1;
	(result.y2) = yp1;
	(result.x3) = xp2;
	(result.y3) = yp2;
	(result.x4) = (pFc->x2);
	(result.y4) = (pFc->y2);

	return(result);
}

DllExport dbezetype TransArcBezier(darctype *ac)
{
	int		stat;
	DBL		x0, y0;
	DBL		rag1, rag2, dr;
	DBL		len;
	DBL		xp2, yp2, xp3, yp3;
	dbezetype bz;

	stat = curve3pcenterget(ac->x1, ac->y1, ac->x2, ac->y2, ac->x3, ac->y3, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(ac->x1, ac->y1, ac->x2, ac->y2, ac->x3, ac->y3, x0, y0, &rag1, &rag2, &dr);
	}
	if(stat == FAIL){
		bz.x1 = ac->x1; bz.y1 = ac->y1;
		bz.x2 = ac->x1; bz.y2 = ac->y1;
		bz.x3 = ac->x3; bz.y3 = ac->y3;
		bz.x4 = ac->x3; bz.y4 = ac->y3;
	}
	
	len = hypot((ac->x3)-(ac->x1), (ac->y3)-(ac->y1) );
	len = len*2.0/3.0;

	if((rag2-rag1) < 0.0)
	{
		xp2 = len*cos(rag1-M_PI_2);
		yp2 = len*sin(rag1-M_PI_2);
		xp3 = len*cos(M_PI_2+rag2);
		yp3 = len*sin(M_PI_2+rag2);
	}
	else
	{
		xp2 = len*cos(M_PI_2+rag1);
		yp2 = len*sin(M_PI_2+rag1);
		xp3 = len*cos(rag2-M_PI_2);
		yp3 = len*sin(rag2-M_PI_2);
	}

	xp2 = (ac->x1) + xp2;
	yp2 = (ac->y1) + yp2;
	xp3 = (ac->x3) + xp3;
	yp3 = (ac->y3) + yp3;

	bz.x1 = ac->x1;
	bz.y1 = ac->y1;
	bz.x2 = xp2;
	bz.y2 = yp2;
	bz.x3 = xp3;
	bz.y3 = yp3;
	bz.x4 = ac->x3;
	bz.y4 = ac->y3;

	return(bz);
}

DllExport int crossbezebezediv(dbezetype *bz1, dbezetype *bz2, dbezetype *bz1d, dbezetype *bz2d)
{
	DBL		xtbl[16], ytbl[16];
	dbezetype	bz11, bz12;
	dbezetype	bz21, bz22;

	if(crossbezebezecrs(bz1, bz2, xtbl, ytbl) < 1){
		return(FALSE);
	}
	crossbezediv(bz1, xtbl[0], ytbl[0], &bz11, &bz12);
	(*bz1) = bz11; (*bz1d) = bz12;
	crossbezediv(bz2, xtbl[0], ytbl[0], &bz21, &bz22);
	(*bz2) = bz21; (*bz2d) = bz22;
	return(TRUE);
}

DllExport int crossbezearcdiv(dbezetype *bz1, darctype *ac2, dbezetype *bz1d, darctype *ac2d)
{
	dlinetype	ln1;
	int		divcnt, i;
	DBL		add, tt;
	DBL		xtbl[16], ytbl[16];
	dbezetype	bz11, bz12;
	darctype	ac21, ac22;

	ln1.x1 = bz1->x1; ln1.y1 = bz1->y1;
	divcnt = BEZEDIVMAX;
	add = 1.0 / (DBL)divcnt;
	for(i = 1; i <= divcnt; i++){
		tt = add * (DBL)i;
		curvebezetpointget(*bz1, tt, &ln1.x2, &ln1.y2);
		if(crossarclinecrs(ac2, &ln1, xtbl, ytbl) >= 1){
			crossbezediv(bz1, xtbl[0], ytbl[0], &bz11, &bz12);
			(*bz1) = bz11; (*bz1d) = bz12;
			crossarcdiv(ac2, xtbl[0], ytbl[0], &ac21, &ac22); 
			(*ac2) = ac21; (*ac2d) = ac22;
			return(ON);
		}
		ln1.x1 = ln1.x2; ln1.y1 = ln1.y2;
	}
	return(OFF);
}

DllExport int crossarcarcdiv(darctype *ac1, darctype *ac2, darctype *ac1d, darctype *ac2d)
{
	int		stat;
	DBL		x0, y0;
	DBL		srag, erag;
	DBL		dr;
	dlinetype	ln1;
	DBL		subrag, divrag, rag;
	int		divcnt, i;
	DBL		xtbl[16], ytbl[16];
	darctype	ac11, ac12;
	darctype	ac21, ac22;

	stat = curve3pcenterget(ac1->x1, ac1->y1, ac1->x2, ac1->y2, ac1->x3, ac1->y3, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(ac1->x1, ac1->y1, ac1->x2, ac1->y2, ac1->x3, ac1->y3, x0, y0, &srag, &erag, &dr);
	}
	if(stat == FAIL){
		ln1.x1 = ac1->x1; ln1.y1 = ac1->y1;
		ln1.x2 = ac1->x3; ln1.y2 = ac1->y3;
		if(crossarclinecrs(ac2, &ln1, xtbl, ytbl) >= 1){
			crossarcdiv(ac1, xtbl[0], ytbl[0], &ac11, &ac12); 
			(*ac1) = ac11; (*ac1d) = ac12;
			crossarcdiv(ac2, xtbl[0], ytbl[0], &ac21, &ac22); 
			(*ac2) = ac21; (*ac2d) = ac22;
			return(ON);
		}
	}
	subrag = erag - srag;
	divcnt = (int)(subrag / degree5);
	divrag = subrag / (DBL)divcnt;

	ln1.x1 = ac1->x1; ln1.y1 = ac1->y1;
	for(i = 1; i <= divcnt; i++){
		rag = divrag*(DBL)i + srag;
		ln1.x2 = cos(rag)*dr+x0;
		ln1.y2 = sin(rag)*dr+y0;
		if(crossarclinecrs(ac2, &ln1, xtbl, ytbl) >= 1){
			crossarcdiv(ac1, xtbl[0], ytbl[0], &ac11, &ac12); 
			(*ac1) = ac11; (*ac1d) = ac12;
			crossarcdiv(ac2, xtbl[0], ytbl[0], &ac21, &ac22); 
			(*ac2) = ac21; (*ac2d) = ac22;
			return(ON);
		}
		ln1.x1 = ln1.x2; ln1.y1 = ln1.y2;
	}
	return(OFF);
}

DllExport int crossbezebezecrs(dbezetype *bz1, dbezetype *bz2, DBL *xtbl, DBL *ytbl)
{
	dlinetype	ln2;
	int		divcnt, i;
	DBL		add, tt;
	int		sumcnt, cnt, idx;
	DBL		xtbl2[12], ytbl2[12];

	sumcnt = 0;
	idx = 0;
	ln2.x1 = bz2->x1; ln2.y1 = bz2->y1;
	divcnt = BEZEDIVMAX;
	add = 1.0 / (DBL)divcnt;
	for(i = 1; i <= divcnt; i++){
		tt = add * (DBL)i;
		curvebezetpointget(*bz2, tt, &ln2.x2, &ln2.y2);
		cnt = crossbezelinecrs(bz1, &ln2, xtbl2, ytbl2);
		for(idx = 0; idx < cnt; idx++){
			if(sumcnt == 0){
				xtbl[sumcnt] = xtbl2[idx];
				ytbl[sumcnt] = ytbl2[idx];
				sumcnt++;
			}
			else if(NEQUAL(xtbl[sumcnt-1], xtbl2[idx]) || NEQUAL(ytbl[sumcnt-1], ytbl2[idx])){
				if(sumcnt >= 9){
					return(0);
				}
				xtbl[sumcnt] = xtbl2[idx];
				ytbl[sumcnt] = ytbl2[idx];
				sumcnt++;
			}
		}
		ln2.x1 = ln2.x2; ln2.y1 = ln2.y2;
	}
	return(sumcnt);
}

DllExport int crossbezelinecrs(dbezetype *bz1, dlinetype *ln2, DBL *xtbl, DBL *ytbl)
{
	dlinetype	ln1;
	int		divcnt, i;
	DBL		add, tt;
	int		idx;
	DBL		xx, yy;

	idx = 0;
	ln1.x1 = bz1->x1; ln1.y1 = bz1->y1;
	divcnt = BEZEDIVMAX;
	add = 1.0 / (DBL)divcnt;
	for(i = 1; i <= divcnt; i++){
		tt = add * (DBL)i;
		curvebezetpointget(*bz1, tt, &ln1.x2, &ln1.y2);
		if(curvecrosslineinareachk(ln1, *ln2) == ON){
			if(curvecrosslinelinexyget(ln1, *ln2, &xx, &yy) == ON){
				if(idx == 0){
					xtbl[idx] = xx;
					ytbl[idx] = yy;
					idx++;
				}
				else if(NEQUAL(xtbl[idx-1], xx) || NEQUAL(ytbl[idx-1], yy)){
					if(idx >= 3){
						return(0);
					}
					xtbl[idx] = xx;
					ytbl[idx] = yy;
					idx++;
				}
			}
		}
		ln1.x1 = ln1.x2; ln1.y1 = ln1.y2;
	}
	return(idx);
}

DllExport int crossbezelinecrsex(dbezetype *bz1, dlinetype *ln2, DBL *xtbl, DBL *ytbl, DBL *ttbl)
{
	dlinetype	ln1;
	int		divcnt, i;
	DBL		add, tt;
	DBL		len, tlen;
	int		idx;
	DBL		xx, yy;


	idx = 0;
	ln1.x1 = bz1->x1; ln1.y1 = bz1->y1;
	divcnt = BEZEDIVMAX;
	add = 1.0 / (DBL)divcnt;
	for(i = 1; i <= divcnt; i++){
		tt = add * (DBL)i;
		curvebezetpointget(*bz1, tt, &ln1.x2, &ln1.y2);
		len = hypot(ln1.x2-ln1.x1, ln1.y2-ln1.y1);
		if(curvecrosslineinareachk(ln1, *ln2) == ON){
			if(curvecrosslinelinexyget(ln1, *ln2, &xx, &yy) == ON){
				tlen = hypot(xx - ln1.x1, yy - ln1.y1);
				if(idx == 0){
					xtbl[idx] = xx;
					ytbl[idx] = yy;
					ttbl[idx] = tt + add * (tlen / len) - add;
					idx++;
				}
				else if(NEQUAL(xtbl[idx-1], xx) || NEQUAL(ytbl[idx-1], yy)){
					if(idx >= 3){
						return(0);
					}
					xtbl[idx] = xx;
					ytbl[idx] = yy;
					ttbl[idx] = tt + add * (tlen / len) - add;
					idx++;
				}
			}
		}
		ln1.x1 = ln1.x2; ln1.y1 = ln1.y2;
	}
	return(idx);
}

DllExport int crosslinebezecrs(dlinetype *ln1, dbezetype *bz2, DBL *xtbl, DBL *ytbl)
{
	dlinetype	ln2;
	int		divcnt, i;
	DBL		add, tt;
	int		idx;
	DBL		xx, yy;

	idx = 0;
	ln2.x1 = bz2->x1; ln2.y1 = bz2->y1;
	divcnt = BEZEDIVMAX;
	add = 1.0 / (DBL)divcnt;
	for(i = 1; i <= divcnt; i++){
		tt = add * (DBL)i;
		curvebezetpointget(*bz2, tt, &ln2.x2, &ln2.y2);
		if(curvecrosslineinareachk(*ln1, ln2) == ON){
			if(curvecrosslinelinexyget(*ln1, ln2, &xx, &yy) == ON){
				if(idx == 0){
					xtbl[idx] = xx;
					ytbl[idx] = yy;
					idx++;
				}
				else if(NEQUAL(xtbl[idx-1], xx) || NEQUAL(ytbl[idx-1], yy)){
					if(idx >= 3){
						return(0);
					}
					xtbl[idx] = xx;
					ytbl[idx] = yy;
					idx++;
				}
			}
		}
		ln2.x1 = ln2.x2; ln2.y1 = ln2.y2;
	}
	return(idx);
}

DllExport int crosslinelinecrs(dlinetype *ln1, dlinetype *ln2, DBL *xtbl, DBL *ytbl)
{
	int		ret;
	DBL		xx[4], yy[4], tt[4];
	DBL		x, y, t;
	int		i, j;
	DBL		k1, d1;
	DBL		k2, d2;

	if(curvecrosslineinareachk(*ln1, *ln2) == ON){
		ret = curvecrosslinelinexyget(*ln1, *ln2, &(xtbl[0]), &(ytbl[0]));
		if(ret == ON){
			return(1);
		}
		if(ret == FAIL){
			if(ln1->x2 == ln2->x1 && ln1->y2 == ln2->y1){
				return(0);
			}
			curvecrosslinekdget(ln1->x1, ln1->y1, ln1->x2, ln1->y2, &k1, &d1);
			curvecrosslinekdget(ln2->x1, ln2->y1, ln2->x2, ln2->y2, &k2, &d2);
			if(EQUAL(k1, k2) && EQUAL(d1, d2)){
				xx[0] = ln1->x1; yy[0] = ln1->y1;
				xx[1] = ln1->x2; yy[1] = ln1->y2;
				xx[2] = ln2->x1; yy[2] = ln2->y1;
				xx[3] = ln2->x2; yy[3] = ln2->y2;
				if(EQUAL(ln1->x1, ln1->x2)){
					tt[0] = yy[0];
					tt[1] = yy[1];
					tt[2] = yy[2];
					tt[3] = yy[3];
				}
				else{
					tt[0] = xx[0];
					tt[1] = xx[1];
					tt[2] = xx[2];
					tt[3] = xx[3];
				}
				for(i = 0; i < 3; i++){
					for(j = i+1; j < 4; j++){
						if(tt[i] > tt[j]){
							t = tt[i];
							x = xx[i];
							y = yy[i];
							tt[i] = tt[j];
							xx[i] = xx[j];
							yy[i] = yy[j];
							tt[j] = t;
							xx[j] = x;
							yy[j] = y;
						}
					}
				}
				xtbl[0] = xx[1];
				ytbl[0] = yy[1];
				xtbl[1] = xx[2];
				ytbl[1] = yy[2];
				return(2);
			}
		}
	}
	return(0);
}


DllExport int crossarclinecrs(darctype *ac1, dlinetype *ln2, DBL *xx, DBL *yy)
{
	int result=0, LOOPNUMBER=0, loop, stat;
	DBL	srad, erad, R, ox, oy, lx, ly, A, B, C, D, Ex, Ey, arad;
	DBL t[2];

	stat = curve3pcenterget(ac1->x1, ac1->y1, ac1->x2, ac1->y2, ac1->x3, ac1->y3, &ox, &oy);
	if(stat != FAIL){
		stat = curve3pragrget(ac1->x1, ac1->y1, ac1->x2, ac1->y2, ac1->x3, ac1->y3, ox, oy, &srad, &erad, &R);
	}
	if(stat == FAIL){
		dlinetype	ln1;
		ln1.x1 = ac1->x1; ln1.y1 = ac1->y1;
		ln1.x2 = ac1->x3; ln1.y2 = ac1->y3;
		if(curvecrosslineinareachk(ln1, *ln2) == ON){
			if(curvecrosslinelinexyget(ln1, *ln2, xx, yy) == ON){
				return(1);
			}
		}
		else
		{
			return(0);
		}
	}

	Ex = ((ln2->x2)-(ln2->x1));
	Ey = ((ln2->y2)-(ln2->y1));

	A = Ex*Ex + Ey*Ey;
	B = 2.0*Ex*((ln2->x1)-ox) + 2.0*Ey*((ln2->y1)-oy);
	C = ((ln2->x1)-ox)*((ln2->x1)-ox) + ((ln2->y1)-oy)*((ln2->y1)-oy) - R*R;
	D = B*B - 4.0*A*C;

	if(D<0.0)
	{
		return(0);
	}
	else if(D==0.0)
	{
		t[0] = (-1.0*B) / (2.0*A);
		if( 1.0>=t[0] && t[0]>=0.0)
		{
			++LOOPNUMBER;
		}
		else
		{
			return(0);
		}
	}
	else
	{
		t[0] = ( -1.0*B + sqrt(D) ) / (2.0*A);
		if( 1.0>=t[0] && t[0]>=0.0)
		{
			++LOOPNUMBER;
		}

		t[LOOPNUMBER] = ( -1.0*B - sqrt(D) ) / (2.0*A);
		if( 1.0>=t[LOOPNUMBER] && t[LOOPNUMBER]>=0.0)
		{
			++LOOPNUMBER;
		}
	}

	for(loop=0; loop<LOOPNUMBER; ++loop)
	{
		lx = Ex*t[loop] + (ln2->x1);
		ly = Ey*t[loop] + (ln2->y1);

		arad = atan2( (ly-oy), (lx-ox) );
		if( (erad-srad)>0.0 )
		{
			if( (arad-srad)<0.0 )
			{
				arad = (2*M_PI)+arad;
			}
		}
		else
		{
			if( (arad-srad)>0.0 )
			{
				arad = arad-(2*M_PI);
			}
		}

		if( fabs(erad-srad) >= fabs(arad-srad) )
		{
			xx[result] = lx;
			yy[result] = ly;
			++result;
		}
	}
	return(result);
}

DllExport DBL crossgetlinett(dlinetype *ln, DBL xx, DBL yy)
{
	DBL			tt;
	DBL			divlen, alllen;

	divlen = hypot(xx - ln->x1, yy - ln->y1);
	alllen = hypot(ln->x2 - ln->x1, ln->y2 - ln->y1);
	tt = (divlen / alllen);
	return(tt);
}

DllExport DBL crossgetbezett(dbezetype *bz, DBL xx, DBL yy)
{
	dlinetype	ln1;
	int		divcnt, i;
	DBL		add;
	DBL		tt, tt1;
	DBL		divlen, alllen;

	ln1.x1 = bz->x1; ln1.y1 = bz->y1;
	divcnt = BEZEDIVMAX;
	add = 1.0 / (DBL)divcnt;
	for(i = 1; i <= divcnt; i++){
		tt = add * (DBL)i;
		curvebezetpointget(*bz, tt, &ln1.x2, &ln1.y2);
		if(curvecrossonlinechk(ln1, xx, yy) == ON){
			if(fabs(xx - ln1.x1) < 0.000001 && fabs(yy - ln1.y1) < 0.000001 ){
				divlen = 0.0;
			}
			else{
				divlen = hypot(xx-ln1.x1, yy-ln1.y1);
			}
			alllen = hypot(ln1.x2-ln1.x1, ln1.y2-ln1.y1);
			tt1 = (tt-0.1) + 0.1 * (divlen / alllen);
			return(tt1);
		}
		ln1.x1 = ln1.x2; ln1.y1 = ln1.y2;
	}
	return(1.0);
}

DllExport int crossbezediv(dbezetype *sbz, DBL xx, DBL yy, dbezetype *dbz1, dbezetype *dbz2)
{
	dlinetype	ln1;
	int		divcnt, i;
	DBL		add;
	DBL		tt, tt1;
	DBL		divlen, alllen;

	ln1.x1 = sbz->x1; ln1.y1 = sbz->y1;
	divcnt = BEZEDIVMAX;
	add = 1.0 / (DBL)divcnt;
	for(i = 1; i <= divcnt; i++){
		tt = add * (DBL)i;
		curvebezetpointget(*sbz, tt, &ln1.x2, &ln1.y2);
		if(curvecrossonlinechk(ln1, xx, yy) == ON){
			divlen = hypot(xx-ln1.x1, yy-ln1.y1);
			alllen = hypot(ln1.x2-ln1.x1, ln1.y2-ln1.y1);
			tt1 = (tt-0.1) + 0.1 * (divlen / alllen);
			crossbezettdiv(sbz, tt1, dbz1, dbz2);
			return(ON);
		}
		ln1.x1 = ln1.x2; ln1.y1 = ln1.y2;
	}
	return(OFF);
}

DllExport int crossarcdiv(darctype *sac, DBL xx, DBL yy, darctype *dac1, darctype *dac2)
{
	int		stat;
	DBL		x0, y0;
	DBL		srag, erag;
	DBL		dr;
	dlinetype	ln1;
	DBL		subrag, divrag, rag, trag;
	int		divcnt, i;
	DBL		alllen, divlen;
	DBL		ttrag;

	stat = curve3pcenterget(sac->x1, sac->y1, sac->x2, sac->y2, sac->x3, sac->y3, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(sac->x1, sac->y1, sac->x2, sac->y2, sac->x3, sac->y3, x0, y0, &srag, &erag, &dr);
	}
	if(stat == FAIL){
		dac1->x1 = sac->x1; dac1->y1 = sac->y1;
		dac1->x2 = xx; dac1->y2 = yy;
		dac1->x3 = xx; dac1->y3 = yy;

		dac2->x1 = xx; dac2->y1 = yy;
		dac2->x2 = sac->x3; dac2->y2 = sac->y3;
		dac2->x3 = sac->x3; dac2->y3 = sac->y3;

		return(ON);
	}
	subrag = erag - srag;
	divcnt = (int)(subrag / degree5);
	divrag = subrag / (DBL)divcnt;

	ln1.x1 = sac->x1; ln1.y1 = sac->y1;
	for(i = 1; i <= divcnt; i++){
		rag = divrag*(DBL)i + srag;
		ln1.x2 = cos(rag)*dr+x0;
		ln1.y2 = sin(rag)*dr+y0;
		if(curvecrossonlinechk(ln1, xx, yy) == ON){
			divlen = hypot(xx-ln1.x1, yy-ln1.y1);
			alllen = hypot(ln1.x2-ln1.x1, ln1.y2-ln1.y1);
			trag = divrag * (divlen / alllen);
			ttrag = (rag - divrag + trag);

			trag = (ttrag - srag) / 2 + srag;
			dac1->x1 = sac->x1; dac1->y1 = sac->y1;
			dac1->x2 = cos(trag)*dr+x0;
			dac1->y2 = sin(trag)*dr+y0;
			dac1->x3 = xx; dac1->y3 = yy;

			trag = (erag - ttrag) / 2 + ttrag;
			dac2->x1 = xx; dac2->y1 = yy;
			dac2->x2 = cos(trag)*dr+x0;
			dac2->y2 = sin(trag)*dr+y0;
			dac2->x3 = sac->x3; dac2->y3 = sac->y3;

			return(ON);
		}
		ln1.x1 = ln1.x2; ln1.y1 = ln1.y2;
	}
	return(OFF);
}

DllExport int crossbezettdiv(dbezetype *sbz, DBL tt, dbezetype *dbz1, dbezetype *dbz2)
{
	dbezetype tbz;
	DBL		tt1, tt2;

	tt1 = tt * 0.3;
	tt2 = tt * 0.6;
	tbz.x1 = sbz->x1;
	tbz.y1 = sbz->y1;
	curvebezetpointget(*sbz, tt1, &(tbz.x2), &(tbz.y2));
	curvebezetpointget(*sbz, tt2, &(tbz.x3), &(tbz.y3));
	curvebezetpointget(*sbz, tt, &(tbz.x4), &(tbz.y4));
	crossbezedivcalce(sbz, &tbz, dbz1);

	tt1 = (1.0 - tt) * 0.3 + tt;
	tt2 = (1.0 - tt) * 0.6 + tt;
	curvebezetpointget(*sbz, tt, &(tbz.x1), &(tbz.y1));
	curvebezetpointget(*sbz, tt1, &(tbz.x2), &(tbz.y2));
	curvebezetpointget(*sbz, tt2, &(tbz.x3), &(tbz.y3));
	tbz.x4 = sbz->x4;
	tbz.y4 = sbz->y4;
	crossbezedivcalce(sbz, &tbz, dbz2);
	return(0);
}

DllExport int crossbezestetdiv(dbezetype *sbz, DBL st, DBL et, dbezetype *dbz)
{
	curvebezettdivget(*sbz, st, et, dbz);
	/*

	dbezetype tbz;
	DBL		tt1, tt2, tt3, tt4;

	tt1 = st;
	tt2 = (et-st) * 0.3;
	tt3 = (et-st) * 0.6;
	tt4 = et;
	curvebezetpointget(*sbz, tt1, &(tbz.x1), &(tbz.y1));
	curvebezetpointget(*sbz, tt2, &(tbz.x2), &(tbz.y2));
	curvebezetpointget(*sbz, tt3, &(tbz.x3), &(tbz.y3));
	curvebezetpointget(*sbz, tt4, &(tbz.x4), &(tbz.y4));
	crossbezedivcalce(sbz, &tbz, dbz);
	*/
	return(0);
}

int crossbezedivcalce(dbezetype *sbz, dbezetype *tbz, dbezetype *dbz)
{
	DBL		tp1, tp2, ta;
	double	b1p1, b2p1, b3p1, b4p1;
	double	b1p2, b2p2, b3p2, b4p2;
	double	a, b, c, d, e, f;
	double	ans1, ans2;
	int		ret;

	tp1 = 0.3;
	tp2 = 0.6;

	ta = 1 - tp1;
	b1p1 = ta * ta * ta;
	b2p1 = 3.0 * tp1 * ta * ta;
	b3p1 = 3.0 * tp1 * tp1 * ta;
	b4p1 = tp1 * tp1 * tp1;

	ta = 1 - tp2;
	b1p2 = ta * ta * ta;
	b2p2 = 3.0 * tp2 * ta * ta;
	b3p2 = 3.0 * tp2 * tp2 * ta;
	b4p2 = tp2 * tp2 * tp2;

	dbz->x1 = tbz->x1;
	dbz->x4 = tbz->x4;
	a = b2p1;
	b = b3p1;
	c = (tbz->x2) - b1p1*(tbz->x1) - b4p1*(tbz->x4);
	d = b2p2;
	e = b3p2;
	f = (tbz->x3) - b1p2*(tbz->x1) - b4p2*(tbz->x4);
	ret = crossmltqu(a, b, c, d, e, f, &ans1, &ans2);
	if(ret == FAIL){
		dbz->x2 = tbz->x2;
		dbz->x3 = tbz->x3;
	}
	else{
		dbz->x2 = ans1;
		dbz->x3 = ans2;
	}

	dbz->y1 = tbz->y1;
	dbz->y4 = tbz->y4;
	a = b2p1;
	b = b3p1;
	c = (tbz->y2) - b1p1*(tbz->y1) - b4p1*(tbz->y4);
	d = b2p2;
	e = b3p2;
	f = (tbz->y3) - b1p2*(tbz->y1) - b4p2*(tbz->y4);
	ret = crossmltqu(a, b, c, d, e, f, &ans1, &ans2);
	if(ret == FAIL){
		dbz->y2 = tbz->y2;
		dbz->y3 = tbz->y3;
	}
	else{
		dbz->y2 = ans1;
		dbz->y3 = ans2;
	}

	return(ret);
}

void crosscurbtobeze(DBL *x1, DBL *y1, DBL *x2, DBL *y2, DBL *x3, DBL *y3, DBL *x4, DBL *y4)
{
}

//p 連立方程式(simultaneous equations)を解く関数
int crossmltqu(double a, double b, double c, double d, double e, double f, double *x, double *y)
{
	DBL	dd, xx, yy;

	if(EQUAL((a*e - b*d), 0)){		// ちょっと心配　if((a*e-b*d) == 0){
		*x = 0; *y = 0;
		return(FAIL);
	}
	dd = (a*e - b*d);
	xx = (c*e - b*f);
	yy = (a*f - c*d);
	(*x) = (xx / dd);
	(*y) = (yy / dd);
	return(0);
}

//媒介変数を入れて円弧を分割する関数
DllExport int crossarcttdiv(darctype *sac, DBL xx, DBL yy, DBL tt, darctype *dac1, darctype *dac2)
{
	int	stat;
	DBL x0, y0;
	DBL srag, erag;
	DBL subrag, thalf0, thalf1;
	DBL dr, radian0, radian1;

	stat = curve3pcenterget(sac->x1, sac->y1, sac->x2, sac->y2, sac->x3, sac->y3, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(sac->x1, sac->y1, sac->x2, sac->y2, sac->x3, sac->y3, x0, y0, &srag, &erag, &dr);
	}
	if(stat == FAIL){
		return(ON);
	}

	subrag = (erag - srag);
	thalf0 = (tt / 2.0);
	thalf1 = ((1.0+tt) / 2.0);
	radian0 = subrag*thalf0 + srag;
	radian1 = subrag*thalf1 + srag;

	dac1->x1 = sac->x1;
	dac1->y1 = sac->y1;
	dac1->x2 = cos(radian0)*dr+x0;
	dac1->y2 = sin(radian0)*dr+y0;
	dac1->x3 = xx;
	dac1->y3 = yy;

	dac2->x1 = xx;
	dac2->y1 = yy;
	dac2->x2 = cos(radian1)*dr+x0;
	dac2->y2 = sin(radian1)*dr+y0;
	dac2->x3 = sac->x3;
	dac2->y3 = sac->y3;

	return(OFF);
}