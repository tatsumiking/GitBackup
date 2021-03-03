//
// Easy Scanner System Ver1.0
//
// ess  curvelib.c	‹Èüˆ—ŠÖ”
//
//  Copyright (c) 1998 by THEON


#include	<windows.h>

#include	<stdio.h>
#include	<math.h>

#include	"..\stdef.h"
#include	"dlldef.h"

#include	"curvelib.fnc"

DBL	pai, hpai, degree1, degree01, degree5;

//P ‰Šú‰»ˆ—
DllExport void curveinit()
{
	pai = M_PI;
	hpai = M_PI_2;
	degree1 = M_PI_2 / 90.0;
	degree01 = M_PI_2 / 900.0;
	degree5 = degree1 * 5.0;
}

//P ‚P“x‚ÌŠÖ”‚Åg—p‚·‚éŠp“x‚ğ‹‚ß‚é
DllExport DBL curvedegree1get()
{
	return(degree1);
}

//P ‚Q‚Â‚ÌÚüŠp“x‚©‚ç•ªŠ„‚ª•K—v‚©ƒ`ƒFƒbƒN‚·‚é
DllExport BOOL curvedivragchk(DBL rag1, DBL rag2)
{
	if(rag1 < 0.0){
		rag1 = -rag1;
	}
	if(rag2 < 0.0){
		rag2 = -rag2;
	}
	if(rag1 < rag2+degree5 && rag1 > rag2-degree5){
		return(ON);
	}
	return(OFF);
}

//P ‰ñ“]•ÏŒ`‚·‚é
DllExport void curveroundtrns(DBL srcx, DBL srcy, DBL x0, DBL y0, DBL rag, DBL *dstx, DBL *dsty)
{
	DBL	tx, ty, tcos, tsin;

	tx = srcx - x0; ty = srcy - y0;
	tsin = sin(rag);
	tcos = cos(rag);
	*dstx = tx * tcos - ty * tsin + x0;
	*dsty = tx * tsin + ty * tcos + y0;
}

//P HH
DBL curve2divcrnr(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	rag1, rag2, rag;

	if(curvenoragchk(x1, y1, x2, y2, x3, y3, &rag)){
		return(rag);
	}
	rag1 = atan2(y2-y1, x2-x1);
	rag2 = atan2(y3-y2, x3-x2);
	rag = (rag1-rag2);
	if(rag > pai){
		rag -= pai*2.0;
	}
	else if(rag < -pai){
		rag += pai*2.0;
	}
	rag /= 2.0;
	return(rag);
}

//P HH
DBL curvevertical(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	rag1, rag2, rag;

	if(curvenoragchk(x1, y1, x2, y2, x3, y3, &rag)){
		return(rag);
	}
	rag1 = atan2(y2-y1, x2-x1);
	rag2 = atan2(y3-y2, x3-x2);
	rag = (rag1-rag2);
	return(rag);
}

int curvenoragchk(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL *rag)
{
	if(EQUAL(x2, x1) && EQUAL(y2, y1)){
		if(EQUAL(x3, x2) && EQUAL(y3, y2)){
			*rag = NORAG;
			return(ON);
		}
		*rag = atan2(y3-y2, x3-x2);
		return(ON);
	}
	if(EQUAL(x3, x2) && EQUAL(y3, y2)){
		*rag = NORAG;
		return(ON);
	}
	return(OFF);
}

//p Šp“x‚ğ|‚X‚O‚©‚ç‚X‚O“x¡‚ß‚éŠÖ”
DBL curveraglegalhpai(DBL rag)
//a DBL rag		//Šp“x
//r DBL 			//Šp“x
{
	if(pai < rag){
		rag = rag - pai*2;
	}
	if(-pai > rag){
		rag = rag + pai*2;
	}
	if(hpai < rag){
		rag = rag - pai;
	}
	if(-hpai > rag){
		rag = rag + pai;
	}
	return(rag);
}

//p Šp“x‚ğ|‚P‚W‚O‚©‚ç‚P‚W‚O“x¡‚ß‚éŠÖ”
DllExport DBL curveraglegal(DBL rag)
{
	if(pai < rag){
		rag = rag - pai*2;
	}
	if(-pai > rag){
		rag = rag + pai*2;
	}
	return(rag);
}

//P ‚R“_‚©‚ç‰~‚Ì’†S‚ğ‹‚ß‚é
DllExport int curve3pcenterget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL FAR *x0, DBL FAR *y0)
{
	DBL	x12, y12, x23, y23;
	DBL	rag12, rag23;
	DBL	tan12, tan23;

	if(EQUAL(y2, y1) && EQUAL(x2, x1)){
		return(FAIL);
	}
	if(EQUAL(y3, y2) && EQUAL(x3, x2)){
		return(FAIL);
	}
	x12 = (x1 + x2) / 2.0; y12 = (y1 + y2) / 2.0;
	x23 = (x2 + x3) / 2.0; y23 = (y2 + y3) / 2.0;
	rag12 = atan2(y2 - y1, x2 - x1);
	rag23 = atan2(y3 - y2, x3 - x2);
	if(curvenocrosschk(rag12, rag23) == ON){
		return(FAIL);
	}
	rag12 = (rag12 >= hpai) ? (rag12 - hpai) : (rag12 + hpai);
	rag23 = (rag23 >= hpai) ? (rag23 - hpai) : (rag23 + hpai);
	if(curvetanerrchk(rag12) == ON){
		tan23 = tan(rag23);
		*x0 = x12;
		*y0 = tan23 * (x12 - x23) + y23;
		return(0);
	}
	tan12 = tan(rag12);
	if(curvetanerrchk(rag23) == ON){
		*x0 = x23;
		*y0 = tan12 * (x23 - x12) + y12;
		return(0);
	}
	tan23 = tan(rag23);
	if(EQUAL(tan12, tan23)){
		return(FAIL);
	}
	*x0 = (tan23 * x23 - tan12 * x12 + y12 - y23) / (tan23 - tan12);
	if(-0.0001 < tan12 && tan12 < 0.0001){
		*y0 = y12;
		return(0);
	}
	tan12 = 1.0 / tan12;
	if(-0.0001 < tan23 && tan23 < 0.0001){
		*y0 = y23;
		return(0);
	}
	tan23 = 1.0 / tan23;
	if(EQUAL(tan12, tan23)){
		return(FAIL);
	}
	*y0 = (tan23 * y23 - tan12 * y12 + x12 - x23) / (tan23 - tan12);
	return(0);
}

//P …•½‚©…•½‚É‹ß‚¢‚©‚ğƒ`ƒFƒbƒN‚·‚é
int curvenocrosschk(DBL rag1, DBL rag2)
{
	if(rag1 < (rag2 + degree1) && (rag2 - degree1) < rag1){
		return(ON);
	}
	return(OFF);
}

int curvetanerrchk(DBL rag)
{
	rag = fabs(rag);
	if(rag > hpai - degree01 && rag < hpai + degree01){
		return(ON);
	}
	return(OFF);
}

int curveroundchk(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0)
{
	DBL	rag01, rag02, rag03;
	DBL	subrag;

	if((EQUAL(x1, x0) && EQUAL(y1, y0))
	|| (EQUAL(x2, x0) && EQUAL(y2, y0))
	|| (EQUAL(x2, x0) && EQUAL(y2, y0))){
		return(1);
	}
	rag01 = atan2(y1 - y0, x1 - x0);
	rag02 = atan2(y2 - y0, x2 - x0);
	rag03 = atan2(y3 - y0, x3 - x0);
	if(EQUAL(rag01, rag02) || EQUAL(rag02, rag03)){
		return(1);
	}
	if(rag01 > rag02){
		if(rag02 < rag03){
			rag03 = (rag03) - pai*2.0;
			subrag = rag01 - rag03;
			if(subrag > pai*2.0){
				rag03 = (rag03) + pai*4;
			}
		}
	}
	else{
		if(rag02 > rag03){
			rag03 = (rag03) + pai*2.0;
			subrag = rag03 - rag01;
			if(subrag > pai*2.0){
				rag03 = (rag03) - pai*4;
			}
		}
	}
	if(rag03 > rag01){
		return(1);
	}
	return(-1);
}

/*92.09.30*/
//p ‚R“_‰~ŒÊ‚ÌŠp“x‚ğ‹‚ß‚éŠÖ”
DllExport int curve3pragrget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0, DBL FAR *rag01, DBL FAR *rag03, DBL FAR *r)
{
	DBL	len01, len02, subrag;
	DBL	rag02;

	if((EQUAL(x1, x0) && EQUAL(y1, y0))
	|| (EQUAL(x2, x0) && EQUAL(y2, y0))
	|| (EQUAL(x2, x0) && EQUAL(y2, y0))){
		*r = 0.0;
		*rag01 = 0.0;
		*rag03 = 0.0;
		return(FAIL);
	}
	*rag01 = atan2(y1 - y0, x1 - x0);
	rag02 = atan2(y2 - y0, x2 - x0);
	*rag03 = atan2(y3 - y0, x3 - x0);
	if(EQUAL(*rag01, rag02) || EQUAL(rag02, *rag03)){
		*r = hypot(x1 - x0, y1 - y0);
		*rag01 = *rag03;
		return(FAIL);
	}
	if(*rag01 > rag02){
		if(rag02 < *rag03){
			*rag03 = (*rag03) - pai*2.0;
			subrag = *rag01 - *rag03;
			if(subrag > pai*2.0){
				*rag03 = (*rag03) + pai*4;
			}
		}
	}
	else{
		if(rag02 > *rag03){
			*rag03 = (*rag03) + pai*2.0;
			subrag = *rag03 - *rag01;
			if(subrag > pai*2.0){
				*rag03 = (*rag03) - pai*4;
			}
		}
	}
	len01 = hypot(x1 - x0, y1 - y0);
	len02 = hypot(x2 - x0, y2 - y0);
	*r = (len01 + len02)/2.0;
	return(0);
}

DllExport int curveragnoget(DBL rag)
{
	if((rag > -degree1) && (rag < degree1)){
		return(HALFPAI);
	}
	if(rag >= pai && rag <= hpai*3){
		return(HALFPAI);
	}
	if(rag <= -pai && rag >= -hpai*3){
		return(HALFPAI);
	}
	if((rag > 0.0 && rag < pai)
	|| (rag < -pai && rag > -pai*2)){
		return(OFF);
	}
	return(ON);
}

//p ‰~ŒÊ‚ÌŠp“x‚Æ”¼Œa‚ğ‹‚ß‚éŠÖ”
DllExport int curveragrget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x0, DBL y0, DBL *rag01, DBL *rag02, DBL *r)
{
	DBL	len01, len02, rag;

	if((EQUAL(x1, x0) && EQUAL(y1, y0))
	|| (EQUAL(x2, x0) && EQUAL(y2, y0))){
		*r = 0.0;
		*rag01 = 0.0;
		*rag02 = 0.0;
		return(FAIL);
	}
	*rag01 = atan2(y1 - y0, x1 - x0);
	*rag02 = atan2(y2 - y0, x2 - x0);
	if(EQUAL(*rag01, *rag02)){
		*r = hypot(x1 - x0, y1 - y0);
		return(FAIL);
	}
	rag = (*rag02) - (*rag01);
	if(rag > pai){
		(*rag02) -= pai*2.0;
	}
	else if(rag < -pai){
		(*rag02) += pai*2.0;
	}
	len01 = hypot(x1 - x0, y1 - y0);
	len02 = hypot(x2 - x0, y2 - y0);
	*r = (len01 + len02)/2.0;
	return(0);
}

DllExport int curvecenterget(DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL *x0, DBL *y0)
{
	DBL	tan1, tcos, tsin, xx0, yy0, xx, rag;

	rag = atan2(y2-y1, x2-x1);
	tcos = cos(-rag);
	tsin = sin(-rag);
	xx = (x2-x1) * tcos - (y2-y1) * tsin;
	if(rag1 > 0.0){
		tan1 = tan(rag1 - hpai);
	}
	else{
		tan1 = tan(rag1 + hpai);
	}
	if(NEQUAL(tan1, 0.0)){
		xx0 = xx/2.0;
		yy0 = tan1*xx0;
	}
	else{
		return(FAIL);
	}
	tcos = cos(rag);
	tsin = sin(rag);
	*x0 = xx0 * tcos - yy0 * tsin + x1;
	*y0 = yy0 * tcos + xx0 * tsin + y1;
	return(0);
}

DllExport int curvedivpointget(DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL rag2, DBL *x12, DBL *y12)
{
	DBL	xx1, yy1, xx2, yy2, xp, yp, xxp1, yyp1, xxx, yyy;
	DBL	tsin, tcos, tan1, tan2;
	DBL	rag, ragp;
	DBL	lena, lenb, lenc, len, len1;

	rag = atan2(y2-y1, x2-x1);
	tcos = cos(-rag);
	tsin = sin(-rag);
	xx1 = 0.0; yy1 = 0.0;
	xx2 = (x2-x1) * tcos - (y2-y1) * tsin;
	yy2 = 0.0;
	tan1 = tan(rag1); tan2 = tan(rag2);
	if(NEQUAL(tan1, tan2)){
		xp = (-tan2*xx2) / (tan1-tan2);	
		yp = tan1*xp;
	}
	else{
		return(FAIL);
	}
	lena = hypot(xx1 - xp, yy1 - yp);
	lenb = hypot(xx2 - xp, yy2 - yp);
	lenc = hypot(xx1 - xx2, yy1 - yy2);

	len = lena+lenb+lenc;
	if(EQUAL(len, 0.0)){
		return(FAIL);
	}
	len1 = lena * lenc / len;

	xxp1 = cos(rag1) * len1 + xx1;
	yyp1 = sin(rag1) * len1 + yy1;

	ragp = atan2(yy1 - yyp1, xx1 - xxp1);

	tcos = cos(-ragp);
	tsin = sin(-ragp);
	xxx = (xx1-xxp1) * tcos - (yy1-yyp1) * tsin + xxp1;
	yyy = (yy1-yyp1) * tcos + (xx1-xxp1) * tsin + yyp1;
	tcos = cos(rag);
	tsin = sin(rag);
	*x12 = xxx * tcos - yyy * tsin + x1;
	*y12 = yyy * tcos + xxx * tsin + y1;
	return(0);
}

DllExport void curvebezecontrolpointget(DBL x1, DBL y1, DBL x2, DBL y2, DBL rag, DBL *xp, DBL *yp)
{
	DBL	tsin, tcos;
	DBL	xt, yt;

	if((-2.0*pai) < rag && rag < (2.0*pai)){
		xt = (x2-x1) * 2.0 / 5.0;
		yt = (y2-y1) * 2.0 / 5.0;
		tcos = cos(rag);
		tsin = sin(rag);
		*xp = (xt * tcos - yt * tsin) + x1;
		*yp = (yt * tcos + xt * tsin) + y1;
	}
	else{
		*xp = x1;
		*yp = y1;
	}
}

/*92.09.29*/
//p ŒvZ‚Å‚«‚È‚¢Šp“x‚É‘Î‚µ‚Ä”½‘Î‘¤‚ÌŠp“x‚ğ”½‰f‚³‚¹‚éŠÖ”
DllExport void curvenoragtocurverag(DBL *rag1, DBL *rag2)
{
	if(EQUAL(*rag1, NORAG)){
		*rag1 = -(*rag2);
	}
	if(EQUAL(*rag2, NORAG)){
		*rag2 = -(*rag1);
	}
}

/*92.10.5*/
//p ‚a‚d‚y‚d‹Èü‚Ì‚Ó‚­‚ç‚İ‚ğƒŠƒ~ƒbƒg‚Æ”äŠr‚·‚éŠÖ”
int curvebezelinechk(dbezetype beze, DBL limit)
{
	DBL	sx, sy, ex, ey;
	DBL	tx1, ty1, tx2, ty2, t, tt;
	DBL	a, b, c, aabb, cmp, len;

	sx = beze.x1; sy = beze.y1;
	ex = beze.x4; ey = beze.y4;

	tx1 = beze.x2; ty1 = beze.y2;
	tx2 = beze.x3; ty2 = beze.y3;

	if(EQUAL(sx, tx1) && EQUAL(sy, ty1) && EQUAL(ex, tx2) && EQUAL(ey, ty2)){
		return(ON);
	}
	len = fabs(sx - tx1) + fabs(tx1 - tx2) + fabs(tx2 - ex)
	    + fabs(sy - ty1) + fabs(ty1 - ty2) + fabs(ty2 - ey);

	if(len > limit * 100.0){
		return(OFF);
	}
	a = ey - sy;
	b = sx - ex;
	c = -(a*sx + b*sy);
	aabb = a*a + b*b;

	cmp = limit*limit*aabb;

	t = a*tx1 + b*ty1 + c;
	tt = t*t;
	if(tt > cmp){
		return(OFF);
	}

	t = a*tx2 + b*ty2 + c;
	tt = t*t;
	if(tt > cmp){
		return(OFF);
	}

	return(ON);
}

/*92.10.5*/
//p ‚a‚d‚y‚d‹Èü‚ğ‚Q‚Â‚Ì‚a‚d‚y‚d‹Èü‚É•ªŠ„‚·‚éŠÖ”
void curvebezedivbeze(dbezetype *sbeze, dbezetype *d1beze, dbezetype *d2beze)
{
	DBL	x12, y12, x23, y23, x34, y34;
	DBL	x123, y123, x234, y234;
	DBL	x1234, y1234;

	x12 = (sbeze->x1 + sbeze->x2) / 2.0;
	y12 = (sbeze->y1 + sbeze->y2) / 2.0;
	x23 = (sbeze->x2 + sbeze->x3) / 2.0;
	y23 = (sbeze->y2 + sbeze->y3) / 2.0;
	x34 = (sbeze->x3 + sbeze->x4) / 2.0;
	y34 = (sbeze->y3 + sbeze->y4) / 2.0;

	x123 = (x12 + x23) / 2.0;
	y123 = (y12 + y23) / 2.0;
	x234 = (x23 + x34) / 2.0;
	y234 = (y23 + y34) / 2.0;

	x1234 = (x123 + x234) / 2.0;
	y1234 = (y123 + y234) / 2.0;

	d1beze->x1 = sbeze->x1;
	d1beze->y1 = sbeze->y1;
	d1beze->x2 = x12;
	d1beze->y2 = y12;
	d1beze->x3 = x123;
	d1beze->y3 = y123;
	d1beze->x4 = x1234;
	d1beze->y4 = y1234;

	d2beze->x1 = x1234;
	d2beze->y1 = y1234;
	d2beze->x2 = x234;
	d2beze->y2 = y234;
	d2beze->x3 = x34;
	d2beze->y3 = y34;
	d2beze->x4 = sbeze->x4;
	d2beze->y4 = sbeze->y4;
}

//p ’¼ü‚ğƒŠƒ~ƒbƒg’l‘å‚«‚­‚µ‚½”ÍˆÍ‚É“_‚ª“à‘¤‚É‚ ‚é‚©”»’è‚·‚éŠÖ”
int curvelimitareachk(DBL x1, DBL y1, DBL x2, DBL y2, DBL limit, DBL tx, DBL ty)
//a DBL x1, y1		//’¼ü‚Ìn“_À•W
//a DBL x2, y2		//’¼ü‚ÌI“_À•W
//a DBL limit		//ƒŠƒ~ƒbƒg’l
//a DBL tx, ty			//“_‚ÌÀ•W
//r int				//¸”s = FAIL  ‚à‚Æ‚Ü‚Á‚½ = 0
{
	DBL	sx, sy, ex, ey;

	if(x1 < x2){
		sx = x1;
		ex = x2;
	}
	else{
		sx = x2;
		ex = x1;
	}

	if(y1 < y2){
		sy = y1;
		ey = y2;
	}
	else{
		sy = y2;
		ey = y1;
	}
	if((sx - limit) < tx && tx < (ex + limit)
	&& (sy - limit) < ty && ty < (ey + limit)){
		return(ON);
	}
	return(OFF);
}

//p ’¼ü‚É‘Î‚·‚é”CˆÓ‚Ì“_‚©‚ç‚Ì‚üŒğ“_‚ğ‹‚ß‚éŠÖ”
DllExport int curveperpendicular(DBL sx, DBL sy, DBL ex, DBL ey, DBL x, DBL y, DBL *cx, DBL *cy)
//a DBL sx, sy		//’¼ü‚Ìn“_À•W
//a DBL ex, ey		//’¼ü‚ÌI“_À•W
//a DBL x, y			//“_‚ÌÀ•W
//a DBL *cx, *cy		//’¼ü‚Æ‚ü‚Æ‚ÌŒğ“_À•W
//r int				//¸”s = FAIL  ‚à‚Æ‚Ü‚Á‚½ = 0
{
	DBL	d, ss, a, b, c;

	if(EQUAL(sx, x) && EQUAL(sy, y)){
		*cx = x; *cy = y;
		return(0);
	}
	if(EQUAL(ex, x) && EQUAL(ey, y)){
		*cx = x; *cy = y;
		return(0);
	}
	if(EQUAL(sx, ex) && EQUAL(sy, ey)){
		return(FAIL);
	}
	if(EQUAL(sx, ex)){
		*cx = sx; *cy = y;
		return(0);
	}
	if(EQUAL(sy, ey)){
		*cx = x; *cy = sy;
		return(0);
	}
	d = sx*ey - ex*sy;
	if(EQUAL(d, 0)){
		return(FAIL);
	}
	a = (sy - ey) / d;
	b = (ex - sx) / d;
	c = 1;
	ss = a*a + b*b;
	if(EQUAL(ss, 0)){
		return(FAIL);
	}
	if(EQUAL(b, 0)){
		return(FAIL);
	}
	(*cx) = (b*b*x - a*b*y - a*c) / ss;
	(*cy) = -(a/b)*(*cx) - (c/b);
	return(0);
}

//p ü‚Ìã‚É“_‚ª‚ ‚é‚©ƒ`ƒFƒbƒN‚·‚éŠÖ”
DllExport int curveonlinechk(DBL x1, DBL y1, DBL x2, DBL y2, DBL x, DBL y)
//a DBL x1, y1		//’¼ü‚Ìn“_À•W
//a DBL x2, y2		//’¼ü‚ÌI“_À•W
//a DBL x, y			//Œğ“_‚ÌÀ•W
//r int				//Œğ“_‚ª’¼ü‚Ì’†‚É‚ ‚é‚Æ‚« ‚n‚m@‚È‚¢‚Æ‚« ‚n‚e‚e
{
	DBL	t;

	if(fabs(x1-x2) < 0.01){
		if(EQUAL(y2, y1)){
			if(EQUAL(y1, y)){
				return(ON);
			}
			else{
				return(OFF);
			}
		}
		else{
			if(fabs(x1-x) > 0.01){
				return(OFF);
			}
			t = (y-y1) / (y2-y1);
		}
	}
	else{
		if(EQUAL(x2, x1)){
			if(EQUAL(x1, x)){
				return(ON);
			}
			else{
				return(OFF);
			}
		}
		else{
			t = (x-x1) / (x2-x1);
		}
	}
	if((0 <= t) && (t <= 1)){
		return(ON);
	}
	else{
		return(OFF);
	}
}

DllExport void curve3pragget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x0, DBL y0, DBL *r1, DBL *r12, DBL *r23, DBL *r3)
{
	DBL	rag1, rag2;

	rag1 = atan2(x1 - x0, y1 - y0);
	rag2 = atan2(x2 - x1, y2 - y1);
	*r1 = curveraglegalhpai(rag1 - hpai - rag2);
	rag1 = atan2(x2 - x0, y2 - y0);
	rag2 = atan2(x1 - x2, y1 - y2);
	*r12 = curveraglegalhpai(rag1 + hpai - rag2);
	rag2 = atan2(x3 - x2, y3 - y2);
	*r23 = curveraglegalhpai(rag1 - hpai - rag2);
	rag1 = atan2(x3 - x0, y3 - y0);
	rag2 = atan2(x2 - x3, y2 - y3);
	*r3 = curveraglegalhpai(rag1 + hpai - rag2);
}

DllExport void curvebezettdivget(dbezetype sbz, DBL st, DBL et, dbezetype *dbz)
{
	DBL	tp1, tp2;
	DBL	x1, y1, x4, y4;

	curvebezetpointget(sbz, st, &x1, &y1);
	curvebezetpointget(sbz, et, &x4, &y4);

	tp1 = (et - st) * 0.3 + st;
	tp2 = (et - st) * 0.6 + st;

	dbz->x1 = x1; dbz->y1 = y1;
	dbz->x4 = x4; dbz->y4 = y4;
	curvebezetpointget(sbz, tp1, &(dbz->x2), &(dbz->y2));
	curvebezetpointget(sbz, tp2, &(dbz->x3), &(dbz->y3));

	tp1 = 0.3;
	tp2 = 0.6;

	curveonpointtocntrlpoint(dbz, tp1, tp2);
}

DllExport void curveonpointtocntrlpoint(dbezetype *dbz, DBL tp1, DBL tp2)
{
	DBL	x1, y1, x2, y2, x3, y3, x4, y4;
	DBL	ta;
	DBL	b1p1, b2p1, b3p1, b4p1;
	DBL	b1p2, b2p2, b3p2, b4p2;
	DBL	a, b, c, d, e, f;
	DBL	ans1, ans2;
	int	ret;

	x1 = dbz->x1; y1 = dbz->y1;
	x2 = dbz->x2; y2 = dbz->y2;
	x3 = dbz->x3; y3 = dbz->y3;
	x4 = dbz->x4; y4 = dbz->y4;
	
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

	a = b2p1;
	b = b3p1;
	c = x2 - b1p1*x1 - b4p1*x4;
	d = b2p2;
	e = b3p2;
	f = x3 - b1p2*x1 - b4p2*x4;
	ret = curvesmltqu(a, b, c, d, e, f, &ans1, &ans2);
	if(ret == 0){
		dbz->x2 = ans1;
		dbz->x3 = ans2;
	}
	a = b2p1;
	b = b3p1;
	c = y2 - b1p1*y1 - b4p1*y4;
	d = b2p2;
	e = b3p2;
	f = y3 - b1p2*y1 - b4p2*y4;
	ret = curvesmltqu(a, b, c, d, e, f, &ans1, &ans2);
	if(ret == 0){
		dbz->y2 = ans1;
		dbz->y3 = ans2;
	}
}

int curvesmltqu(DBL a, DBL b, DBL c, DBL d, DBL e, DBL f, DBL *x, DBL *y)
{
	if(EQUAL((a*e - b*d), 0)){
		*x = 0; *y = 0;
		return(FAIL);
	}
	*x = (c*e - b*f) / (a*e - b*d);
	*y = (a*f - c*d) / (a*e - b*d);
	return(0);
}

DllExport void curvebezetpointget(dbezetype bz, DBL t, DBL *x, DBL *y)
{
	DBL	b1, b2, b3, b4;
	DBL	mt;

	mt = 1 - t;
	b1 = mt * mt * mt;
	b2 = 3 * t * mt * mt;
	b3 = 3 * t * t * mt;
	b4 = t * t * t;
	*x = (b1 * bz.x1 + b2 * bz.x2 + b3 * bz.x3 + b4 * bz.x4);
	*y = (b1 * bz.y1 + b2 * bz.y2 + b3 * bz.y3 + b4 * bz.y4);
}

DllExport void curvesymxytrns(DBL a, DBL b, DBL t1, DBL t2, DBL *x, DBL *y)
{
	DBL	tx, ty, xx, yy;
	DBL	t3;

	tx = *x; ty = *y;
	if(a >= 100000.0){
		xx = 2 * b - tx;
		yy = ty;
	}
	else{
		t3 = ty - b;
		xx = (t2 * t3 - (t1 - 1) * tx) / (t1 + 1); 
		yy = (t2 * tx + (t1 - 1) * t3) / (t1 + 1) + b; 
	}
	*x = xx;
	*y = yy;
}

DllExport int curvenkindget(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	nz;

	nz = (x2-x1)*(y3-y2)-(y2-y1)*(x3-x2);
	if(EQUAL(nz, 0.0)){
		return(0);
	}
	else if(nz < 0.0){
		return(-1);
	}
	else{
		return(1);
	}
}

DllExport int curvecrosslineinareachk(dlinetype line1, dlinetype line2)
{
	DBL		minix1, miniy1, maxx1, maxy1;
	DBL		minix2, miniy2, maxx2, maxy2;

	minix1 = (line1.x1 < line1.x2) ? line1.x1 : line1.x2;
	maxx1  = (line1.x1 > line1.x2) ? line1.x1 : line1.x2;
	miniy1 = (line1.y1 < line1.y2) ? line1.y1 : line1.y2;
	maxy1  = (line1.y1 > line1.y2) ? line1.y1 : line1.y2;

	minix2 = (line2.x1 < line2.x2) ? line2.x1 : line2.x2;
	maxx2  = (line2.x1 > line2.x2) ? line2.x1 : line2.x2;
	miniy2 = (line2.y1 < line2.y2) ? line2.y1 : line2.y2;
	maxy2  = (line2.y1 > line2.y2) ? line2.y1 : line2.y2;

	if(minix1 > maxx2 || miniy1 > maxy2
	|| maxx1 < minix2 || maxy1 < miniy2){
		return(OFF);
	}
	/*
	if(minix1 == maxx2 && miniy1 == maxy2){
		return(OFF);
	}
	if(minix2 == maxx1 && miniy2 == maxy1){
		return(OFF);
	}
	*/
	return(ON);
}

DllExport int curvecrosslnlnxyget(dlinetype line1, dlinetype line2, DBL *x, DBL *y)
{
	DBL		tx, ty;
	DBL		sx1, sy1, ex1, ey1;
	DBL		sx2, sy2, ex2, ey2;
	DBL		k1, d1;
	DBL		k2, d2;

	sx1 = line1.x1; sy1 = line1.y1;
	ex1 = line1.x2; ey1 = line1.y2;
	sx2 = line2.x1; sy2 = line2.y1;
	ex2 = line2.x2; ey2 = line2.y2;
	curvecrosslinekdget(sx1, sy1, ex1, ey1, &k1, &d1);
	curvecrosslinekdget(sx2, sy2, ex2, ey2, &k2, &d2);
	if(fabs(k1-k2) < 0.0001){
		return(FAIL);
	}
	curvecrosspointxyget(k1, d1, k2, d2, &tx, &ty);
	*x = tx;
	*y = ty;
	return(ON);
}

DllExport int curvecrosslinelinexyget(dlinetype line1, dlinetype line2, DBL *x, DBL *y)
{
	DBL		tx, ty;
	DBL		sx1, sy1, ex1, ey1;
	DBL		sx2, sy2, ex2, ey2;
	DBL		k1, d1;
	DBL		k2, d2;

	sx1 = line1.x1; sy1 = line1.y1;
	ex1 = line1.x2; ey1 = line1.y2;
	sx2 = line2.x1; sy2 = line2.y1;
	ex2 = line2.x2; ey2 = line2.y2;
	curvecrosslinekdget(sx1, sy1, ex1, ey1, &k1, &d1);
	curvecrosslinekdget(sx2, sy2, ex2, ey2, &k2, &d2);
	if(fabs(k1-k2) < 0.0001){
		return(FAIL);
	}
	curvecrosspointxyget(k1, d1, k2, d2, &tx, &ty);
	if(curveonlinechk(sx1, sy1, ex1, ey1, tx, ty) == ON){
		if(curveonlinechk(sx2, sy2, ex2, ey2, tx, ty) == ON){
			*x = tx;
			*y = ty;
			return(ON);
		}
	}
	return(OFF);
}

DllExport void curvecrosslinekdget(DBL x1,DBL y1,DBL x2,DBL y2,DBL *k,DBL *d)
{

	if(fabs(x1-x2) < 0.01) {
 		*k=1000;
		*d=x2;
	}
	else if(fabs(y1-y2) < 0.01) {
 		*k=0.0;
		*d=y2;
	}
	else{
		*k= (y2-y1) / (x2-x1);
		if(1000 < *k || *k < -1000){
	 		*k=1000;
			*d=x2;
		}
		else if(-0.001 < *k && *k < 0.001){
	 		*k=0.0;
			*d=y2;
		}
		else{
			*d= y1 - (*k) * x1;
		}
	}
}

DllExport void curvecrosspointxyget(DBL k1, DBL d1, DBL k2, DBL d2, DBL *x, DBL *y)
{
	if(-1000 >= k1 || k1 >= 1000){
		*x = d1;
		if(-0.001 <= k2 && k2 <= 0.001){
			*y = d2;
		}
		else{
			*y = k2*(*x) + d2;
		}
	}
	else if(-0.001 <= k1 && k1 <= 0.001){
		*y = d1;
		if(-1000 >= k2 || k2 >= 1000){
			*x = d2;
		}
		else{
			*x = ((*y) - d2) / k2;
		}
	}
	else{
		if(1000 <= k2 || k2 <= -1000){
			*x = d2;
			*y = k1*(*x)+d1;
		}
		else if(-0.001 <= k2 && k2 <= 0.001){
			*y = d2;
			*x = ((*y) - d1) / k1;
		}
		else{
			*x = (d2-d1) / (k1-k2);
			*y = (k1*d2-k2*d1) / (k1-k2);
		}
	}
}

DllExport int curvecrossonlinechk(dlinetype line, DBL x, DBL y)
{
	DBL		minix, miniy, maxx, maxy;

	minix = (line.x1 < line.x2) ? line.x1 : line.x2;
	maxx  = (line.x1 > line.x2) ? line.x1 : line.x2;
	miniy = (line.y1 < line.y2) ? line.y1 : line.y2;
	maxy  = (line.y1 > line.y2) ? line.y1 : line.y2;

	if(fabs(line.x1 - x) < 0.000001 && fabs(line.y1 - y) < 0.000001){
		return(ON);
	}
	if(fabs(line.x2 - x) < 0.000001 && fabs(line.x2 - y) < 0.000001){
		return(ON);
	}
	if(minix <= x && x <= maxx
	&& miniy <= y && y <= maxy){
		return(ON);
	}
	return(OFF);
}

DllExport void cruvearctobeze(darctype arc,  dbezetype *bz)
{
	int		stat;
	DBL		x0, y0, rag1, rag2, dr;
	DBL		rag, t1, t2;

	stat = curve3pcenterget(arc.x1, arc.y1, arc.x2, arc.y2, arc.x3, arc.y3, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(arc.x1, arc.y1, arc.x2, arc.y2, arc.x3, arc.y3, x0, y0, &rag1, &rag2, &dr);
	}
	if(stat == FAIL){
		bz->x1 = arc.x1; bz->y1 = arc.y1;
		bz->x2 = arc.x2; bz->y2 = arc.y2;
		bz->x3 = arc.x2; bz->y3 = arc.y2;
		bz->x4 = arc.x3; bz->y4 = arc.y3;
		return;
	}

	bz->x1 = arc.x1; bz->y1 = arc.y1;
	bz->x4 = arc.x3; bz->y4 = arc.y3;
	rag = (rag2 - rag1) / 3.0 + rag1;
	bz->x2 = cos(rag)*dr+x0;
	bz->y2 = sin(rag)*dr+y0;
	rag = (rag2 - rag1) / 3.0 * 2.0 + rag1;
	bz->x3 = cos(rag)*dr+x0;
	bz->y3 = sin(rag)*dr+y0;

	t1 = 1.0 / 3.0; t2 = 1.0 / 3.0 * 2.0;
	curveonpointtocntrlpoint(bz, t1, t2);
}

//p ‚Q“_ŠÔ‚Ì’·‚³‚ğ‹‚ß‚é
DllExport DBL curvegetlength(DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		len;

	if(EQUAL(x1, x2) && EQUAL(y1, y2)){
		return(0.0);
	}
	len = hypot(x2-x1, y2-y1);
	return(len);
}

