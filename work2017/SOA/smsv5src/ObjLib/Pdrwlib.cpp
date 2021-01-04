//
// Easy Scanner System Ver1.0
//
// ess  pdrwblib.c	ï`âÊèàóùä÷êî
//
//  Copyright (c) 1998 by THEON

#include	"stdafx.h"

#include	"Curvelib.h"
#include	"Senvlib.h"
#include	"Scrnlib.h"
#include	"Pdrwlib.h"


#define		DIVCALC		1

extern DBL		dRltvSx, dRltvSy;
extern DBL		dRltvEx, dRltvEy;
extern int		nFirstFlag;

int		clipflag = OFF;

DllExport int pdrawchkclipmode()
{
	return(clipflag);
}

DllExport void pdrawclipmode(int flag)
{
	clipflag = flag;
}

DllExport void pdrwpoint(HDC PaintDC, DBL x, DBL y)
{
	int		ndot, nhdot;
	int		sx, sy, ex, ey;

	senvgetpointdotsize(&ndot);
	nhdot = ndot/2;
	sx = (int)(x - (DBL)nhdot);
	sy = (int)(y - (DBL)nhdot);
	ex = sx + ndot;
	ey = sy + ndot;
	pdrwmoveto(PaintDC, sx, sy);
	pdrwlineout(PaintDC, sx, sy, sx, ey);
	pdrwlineout(PaintDC, sx, ey, ex, ey);
	pdrwlineout(PaintDC, ex, ey, ex, sy);
	pdrwlineout(PaintDC, ex, sy, sx, sy);
}

DllExport void pdrwmove(HDC PaintDC, DBL x1, DBL y1)
{
	pdrwmoveto(PaintDC, x1, y1);
}

DllExport void pdrwline(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2)
{
	HPEN	pen;

	pen = senvgetlinepen();
	if(pen != NULL){
		SelectObject(PaintDC, pen);
	}

	pdrwlineout(PaintDC, x1, y1, x2, y2);

	if(pen != NULL){
		pen = senvgetmainpen();
		SelectObject(PaintDC, pen);
	}
}

DllExport void pdrwarc(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL		x0, y0, rag01, rag03, dr;
	int		stat;

	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat == FAIL){
		pdrwlineout(PaintDC, x1, y1, x2, y2);
		pdrwlineout(PaintDC, x2, y2, x3, y3);
		return;
	}
	stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &rag01, &rag03, &dr);
	if(stat == FAIL){
		pdrwlineout(PaintDC, x1, y1, x2, y2);
		pdrwlineout(PaintDC, x2, y2, x3, y3);
		return;
	}
	pdrwenko(PaintDC, x0, y0, x1, y1, x3, y3, dr, rag01, rag03);
}

DllExport void pdrwbezecntrl(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	int		x, y;
	HPEN	pen;
	POINT	point;

	pen = senvgetsubpen();
	if(pen != NULL){
		SelectObject(PaintDC, pen);
		x = (int)x1; y = (int)y1;
		MoveToEx(PaintDC, x, y, &point);
		x = (int)x2; y = (int)y2;
	 	LineTo(PaintDC, x, y);
		x = (int)x3; y = (int)y3;
		MoveToEx(PaintDC, x, y, &point);
		x = (int)x4; y = (int)y4;
	 	LineTo(PaintDC, x, y);
		pen = senvgetmainpen();
		SelectObject(PaintDC, pen);
	}
}

DllExport void pdrwbeze(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	HPEN	pen;

	pen = senvgetbezepen();
	if(pen != NULL){
		SelectObject(PaintDC, pen);
	}
	pdrwbezelow(PaintDC, x1, y1, x2, y2, x3, y3, x4, y4);
	pen = senvgetmainpen();
	SelectObject(PaintDC, pen);
}

DllExport void pdrwbezelow(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL	divrag, divmini, divmax;
	DBL	degree1;
	DBL	rag0, rag1, rag2, rag3;
	DBL	subrag1, subrag2, subrag3, subrag4, subrag;
	int	count;
	DBL	step;
	DBL	t, t1, b1, b2, b3, b4;
	DBL	sx, sy, ex, ey;

#ifdef DIVCALC
	senvgetdrawdivbeze(&divrag, &divmini, &divmax);
	degree1 = M_PI / 180.0;
	if(x1 == x4 && y1 == y4){
		rag0 = 0;
	}
	else{
		rag0 = atan2(y4-y1, x4-x1);
	}

	if(x1 == x2 && y1 == y2){
		rag1 = rag0;
	}
	else{
		rag1 = atan2(y2-y1, x2-x1);
	}
	if(x2 == x3 && y2 == y3){
		rag2 = rag0;
	}
	else{
		rag2 = atan2(y3-y2, x3-x2);
	}
	if(x4 == x3 && y4 == y3){
		rag3 = rag0;
	}
	else{
		rag3 = atan2(y4-y3, x4-x3);
	}
	subrag1 = rag1 - rag0;
	subrag2 = rag2 - rag1;
	subrag3 = rag3 - rag2;
	subrag4 = rag3 - rag0;
	subrag1 = curveraglegal(subrag1);
	subrag2 = curveraglegal(subrag2);
	subrag3 = curveraglegal(subrag3);
	subrag4 = curveraglegal(subrag4);
	subrag = fabs(subrag1) + fabs(subrag2) + fabs(subrag3) + fabs(subrag4);

	count = (int)((subrag/degree1) / divrag);
	if(count < (int)divmini){
		count = (int)divmini;
	}
	if(count > (int)divmax){
		count = (int)divmax;
	}
	step = 1.0 / (DBL)count;
	sx = x1; sy = y1;
	for(t = step; t < 1.0; t += step){
#else
	sx = x1; sy = y1;
	for(t = 0.1; t < 1.0; t += 0.1){
#endif
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		ex = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ey = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		pdrwlineout(PaintDC, sx, sy, ex, ey);
		sx = ex; sy = ey;
	}
	pdrwlineout(PaintDC, sx, sy, x4, y4);
}

DllExport void pdrwfreecurve(HDC PaintDC, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	rag1, rag2;
	int	rag1no, rag2no;
	DBL	xp1, yp1, xp2, yp2;

	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);

	if(rag1 == NORAG && rag2 == NORAG){
		pdrwlineout(PaintDC, x1, y1, x2, y2);
	}
	else{
		curvenoragtocurverag(&rag1, &rag2);
		rag1no = curveragnoget(rag1);
		rag2no = curveragnoget(rag2);

		if(rag1no == rag2no || rag1no == HALFPAI || rag2no == HALFPAI){
			curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
			curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
			pdrwbezelow(PaintDC, x1, y1, xp1, yp1, xp2, yp2, x2, y2);
		}
		else{
			if(curvedivragchk(rag1, rag2)){
				pdrwragenko(PaintDC, rag1, x1, y1, x2, y2);
			}
			else{
				pdrwdivragenko(PaintDC, rag1, x1, y1, x2, y2, rag2);
			}
		}
	}
}

void pdrwenko(HDC PaintDC, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL dr, DBL srag, DBL erag)
{
	DBL	divrag, divmini, divmax;
	DBL	degree1;
	DBL	subrag;
	DBL	rag, steprag;
	DBL	sx, sy, ex, ey;
	int	count, idx;

#ifdef DIVCALC
	senvgetdrawdivarc(&divrag, &divmini, &divmax);
	degree1 = M_PI / 180.0;
	subrag = fabs(erag - srag) / degree1;
	count = (int)(subrag / divrag);
	if(count < (int)divmini){
		count = (int)divmini;
	}
	if(count > (int)divmax){
		count = (int)divmax;
	}

#else
	count = (int)(fabs(erag - srag) / M_5RAG);
	if(count < 2){
		count = 2;
	}
#endif
	steprag = (erag - srag) / (DBL)count;
	sx = x1; sy = y1;
	for(idx = 1, rag = srag+steprag; idx < count; idx++, rag += steprag){
		ex = cos(rag)*dr+x0;
		ey = sin(rag)*dr+y0;
		pdrwlineout(PaintDC, sx, sy, ex, ey);
		sx = ex; sy = ey;
	}
	pdrwlineout(PaintDC, sx, sy, x2, y2);
}

void pdrwragenko(HDC PaintDC, DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL	x0, y0, rag01, rag02, r;

	if(curvecenterget(rag1, x1, y1, x2, y2, &x0, &y0) == FAIL){
		pdrwlineout(PaintDC, x1, y1, x2, y2);
		return;
	}
	if(curveragrget(x1, y1, x2, y2, x0, y0, &rag01, &rag02, &r) == FAIL){
		pdrwlineout(PaintDC, x1, y1, x2, y2);
		return;
	}
	pdrwenko(PaintDC, x0, y0, x1, y1, x2, y2, r, rag01, rag02);
}

void pdrwdivragenko(HDC PaintDC, DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL rag2)
{
	DBL		rag, ragp, x12, y12;

	if(curvedivpointget(rag1, x1, y1, x2, y2, rag2, &x12, &y12) == FAIL){
		pdrwlineout(PaintDC, x1, y1, x2, y2);
		return;
	}
	ragp = atan2(y12 - y1, x12 - x1) - atan2(y2 - y1, x2 - x1);
	rag = rag1-ragp;
	if(x1 != x12 || y1 != y12){
		pdrwragenko(PaintDC, rag, x1, y1, x12, y12);
	}
	ragp = atan2(y12 - y2, x12 - x2) - atan2(y1 - y2, x1 - x2);
	rag = ragp-rag2;
	if(x2 != x12 || y2 != y12){
		pdrwragenko(PaintDC, rag, x12, y12, x2, y2);
	}
}

void pdrwlineout(HDC PaintDC, DBL sx, DBL sy, DBL ex, DBL ey)
{
	int		stat, ret;
	DBL		x1, y1, x2, y2;

	if(clipflag == OFF){
		pdrwlinetonoclip(PaintDC, sx, sy, ex, ey);
		return;
	}
	x1 = sx; y1 = sy;
	x2 = ex; y2 = ey;
	stat = scrnrltvareachk(sx, sy, ex, ey);
	if(stat == OFF){
		pdrwlineto(PaintDC, sx, sy, ex, ey);
		return;
	}
	if(stat == CROSS){
		ret = scrnrltvarealegal(&sx, &sy, &ex, &ey);
		if(ret == OFF){
			pdrwlineto(PaintDC, x1, y1, x2, y2);
			return;
		}
	}

	if(x1 != sx || y1 != sy){
		pdrwlineto(PaintDC, x1, y1, sx, sy);
	}
	LineTo(PaintDC, (int)sx, (int)sy);
	LineTo(PaintDC, (int)ex, (int)ey);
	if(x2 != ex || y2 != ey){
		pdrwlineto(PaintDC, ex, ey, x2, y2);
	}
}

DllExport void pdrwmoveto(HDC PaintDC, DBL dx, DBL dy)
{
	int		x, y;
	int		flagx, flagy;
	POINT	point;

	if(clipflag == OFF){
		nFirstFlag = ON;
		return;
	}

	flagx = pdrwareasignget(dx, dRltvSx, dRltvEx);
	flagy = pdrwareasignget(dy, dRltvSy, dRltvEy);
	if(flagx == 0){
		x = (int)dx;
	}
	else if(flagx < 0){
		x = (int)dRltvSx;
	}
	else if(flagx > 0){
		x = (int)dRltvEx;
	}

	if(flagy == 0){
		y = (int)dy;
	}
	else if(flagy < 0){
		y = (int)dRltvSy;
	}
	else if(flagy > 0){
		y = (int)dRltvEy;
	}
	MoveToEx(PaintDC, x, y, &point);
}

void pdrwlinetonoclip(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2)
{
	int		ret;
	int		x, y;
	POINT	point;

	ret = scrnrltvareachk(x1, y1, x2, y2);
	if(ret == INAREA){
		if(nFirstFlag == ON){
			x = (int)x1; y = (int)y1;
			MoveToEx(PaintDC, x, y, &point);
			nFirstFlag = OFF;
		}
		x = (int)x2; y = (int)y2;
		LineTo(PaintDC, x, y);
	}
	else if(ret == CROSS){
		ret = scrnrltvarealegal(&x1, &y1, &x2, &y2);
		if(ret == ON){
			x = (int)x1; y = (int)y1;
			MoveToEx(PaintDC, x, y, &point);
			x = (int)x2; y = (int)y2;
			LineTo(PaintDC, x, y);
			nFirstFlag = ON;
		}
	}
	else if(ret == OFF){
		nFirstFlag = ON;
	}
}

DllExport void pdrwlineto(HDC PaintDC, DBL sx, DBL sy, DBL ex, DBL ey)
{
	int	flagx1, flagy1, flagx2, flagy2;
	int	x1, y1, x12, y12, x2, y2;
	DBL	ans1, ans2;

	flagx1 = pdrwareasignget(sx, dRltvSx, dRltvEx);
	flagy1 = pdrwareasignget(sy, dRltvSy, dRltvEy);
	flagx2 = pdrwareasignget(ex, dRltvSx, dRltvEx);
	flagy2 = pdrwareasignget(ey, dRltvSy, dRltvEy);

	if(flagx1 == 0){
		x1 = (int)sx;
	}
	else if(flagx1 < 0){
		x1 = (int)dRltvSx;
	}
	else{
		x1 = (int)dRltvEx;
	}

	if(flagy1 == 0){
		y1 = (int)sy;
	}
	else if(flagy1 < 0){
		y1 = (int)dRltvSy;
	}
	else{
		y1 = (int)dRltvEy;
	}

	if(flagx2 == 0){
		x2 = (int)ex;
	}
	else if(flagx2 < 0){
		x2 = (int)dRltvSx;
	}
	else{
		x2 = (int)dRltvEx;
	}

	if(flagy2 == 0){
		y2 = (int)ey;
	}
	else if(flagy2 < 0){
		y2 = (int)dRltvSy;
	}
	else{
		y2 = (int)dRltvEy;
	}

	if(flagx1 == flagx2 || flagy1 == flagy2){
		LineTo(PaintDC, x1, y1);
		LineTo(PaintDC, x2, y2);
	}
	else if(flagx1 == 0){
		if(flagx2 < 0){
			x12 = (int)dRltvSx;
		}
		else{
			x12 = (int)dRltvEx;
		}
		if(flagy1 < 0){
			y12 = (int)dRltvSy;
		}
		else if(flagy1 > 0){
			y12 = (int)dRltvEy;
		}
		else{
			y12 = y1;
		}
		LineTo(PaintDC, x1, y1);
		LineTo(PaintDC, x12, y12);
		LineTo(PaintDC, x2, y2);
	}
	else if(flagx2 == 0){
		if(flagx1 < 0){
			x12 = (int)dRltvSx;
		}
		else{
			x12 = (int)dRltvEx;
		}
		if(flagy2 < 0){
			y12 = (int)dRltvSy;
		}
		else if(flagy2 > 0){
			y12 = (int)dRltvEy;
		}
		else{
			y12 = y2;
		}
		LineTo(PaintDC, x1, y1);
		LineTo(PaintDC, x12, y12);
		LineTo(PaintDC, x2, y2);
	}
	else if(flagy1 == 0){
		if(flagy2 < 0){
			y12 = (int)dRltvSy;
		}
		else{
			y12 = (int)dRltvEy;
		}
		if(flagx1 < 0){
			x12 = (int)dRltvSx;
		}
		else if(flagx1 > 0){
			x12 = (int)dRltvEx;
		}
		else{
			x12 = x1;
		}
		LineTo(PaintDC, x1, y1);
		LineTo(PaintDC, x12, y12);
		LineTo(PaintDC, x2, y2);
	}
	else if(flagy2 == 0){
		if(flagy1 < 0){
			y12 = (int)dRltvSy;
		}
		else{
			y12 = (int)dRltvEy;
		}
		if(flagx2 < 0){
			x12 = (int)dRltvSx;
		}
		else if(flagx2 > 0){
			x12 = (int)dRltvEx;
		}
		else{
			x12 = x2;
		}
		LineTo(PaintDC, x1, y1);
		LineTo(PaintDC, x12, y12);
		LineTo(PaintDC, x2, y2);
	}
	else if(flagx1 == flagy1 && flagx2 == flagy2){
		ans1 = dRltvEx * (sy - ey) + dRltvSy * (ex - sx) + sx * ey - ex * sy;
		ans2 = dRltvSx * (sy - ey) + dRltvEy * (ex - sx) + sx * ey - ex * sy;
		if(fabs(ans1) < fabs(ans2)){
			x12 = (int)dRltvEx;
			y12 = (int)dRltvSy;
		}
		else{
			x12 = (int)dRltvSx;
			y12 = (int)dRltvEy;
		}
		LineTo(PaintDC, x1, y1);
		LineTo(PaintDC, x12, y12);
		LineTo(PaintDC, x2, y2);
	}
	else if(flagx1 != flagy1 && flagx2 != flagy2){
		ans1 = dRltvSx * (sy - ey) + dRltvSy * (ex - sx) + sx * ey - ex * sy;
		ans2 = dRltvEx * (sy - ey) + dRltvEy * (ex - sx) + sx * ey - ex * sy;
		if(fabs(ans1) < fabs(ans2)){
			x12 = (int)dRltvSx;
			y12 = (int)dRltvSx;
		}
		else{
			x12 = (int)dRltvEx;
			y12 = (int)dRltvEy;
		}
		LineTo(PaintDC, x1, y1);
		LineTo(PaintDC, x12, y12);
		LineTo(PaintDC, x2, y2);
	}
}

int pdrwareasignget(DBL num, DBL mini, DBL max)
{
	if(num <= mini){
		return(-1);
	}
	else if(num >= max){
		return(1);
	}
	else{
		return(0);
	}
}

DllExport BOOL pdrwatrbezechk(int atr1, int atr2, int atr3)
{
	atr1 &= PATRMASK;
	atr2 &= PATRMASK;
	atr3 &= PATRMASK;
	if(((atr1 == (CURVATR | BEZEATR)) || (atr1 == (CONTATR | CURVATR | BEZEATR)))
	&& (atr2 == BEZEATR)
	&& (atr3 == BEZEATR)){
		return(ON);
	}
	return(OFF);
}

DllExport BOOL pdrwatrarcchk(int atr1, int atr2, int atr3)
{
	atr1 &= PATRMASK;
	atr2 &= PATRMASK;
	atr3 &= PATRMASK;
	if((atr1 == CURVATR)
	&& (atr2 == (CURVATR | CONTATR))
	&& ((atr3 & CONTATR) == 0)){
		return(ON);
	}
	return(OFF);
}

DllExport BOOL pdrwatrcurvechk(int atr1)
{
	if(atr1 & CURVATR){
		return(ON);
	}
	return(OFF);
}

DllExport BOOL pdrwatrcurvecontchk(int atr1)
{
	if(atr1 & CURVATR){
		if(atr1 & CONTATR){
			return(ON);
		}
	}
	return(OFF);
}

DllExport DBL pdrwstartragget(int atr0, int atr1, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL	rag;

	if((atr1 & CURVATR) && (atr1 & CONTATR)){
		if(atr0 & CURVATR){
			rag = curve2divcrnr(x0, y0, x1, y1, x2, y2);
		}
		else{
			rag = curvevertical(x0, y0, x1, y1, x2, y2);
		}
	}
	else{
		rag = NORAG;
	}
	return(rag);
}

DllExport DBL pdrwendragget(int atr1, int atr2, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	rag;

	if(atr1 & CURVATR){
		if((atr2 & CURVATR) && (atr2 & CONTATR)){
			if(atr2 & BEZEATR){
				rag = curvevertical(x3, y3, x2, y2, x1, y1);
			}
			else{
				rag = curve2divcrnr(x3, y3, x2, y2, x1, y1);
			}
		}
		else if(atr2 & CONTATR){
			rag = curvevertical(x3, y3, x2, y2, x1, y1);
		}
		else{
			rag = NORAG;
		}
	}
	else{
		rag = NORAG;
	}
	return(rag);
}

DllExport int pdrwreverseatrget(int atr1, int atr2)
{
	int		atr;

	if(atr1 == BEZEATR){
		if((atr2 & BEZEATR) == BEZEATR){
			atr = BEZEATR;
		}
		else{
			atr = 0;
		}
	}
	else{
		if(atr2 == BEZEATR){
			atr = BEZEATR | CURVATR;
		}
		else{
			atr = (atr1 & CONTATR) | (atr2 & CURVATR);
		}
	}
	return(atr);
}

