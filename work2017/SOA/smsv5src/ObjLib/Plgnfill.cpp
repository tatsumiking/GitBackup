//
// Easy Scanner System Ver1.0
//
// ess  plgnfill.c	ëΩäpå`ìhí◊èàóùä÷êî
//
//  Copyright (c) 1998 by THEON

#include	"stdafx.h"

#include	"Curvelib.h"
#include	"Scrnlib.h"
#include	"Senvlib.h"
#include	"Pdrwlib.h"
#include	"Plgnfill.h"

//#define		REJIONCNYMAX	256
//#define		PLGNPOINTMAX	8192

#define		REJIONCNYMAX	1024
#define		PLGNPOINTMAX	0x0ffff /* 0x7fff */
#define		DIVCALC		1

extern HBRUSH	hMainBrush;

HANDLE		hArry = 0;
HANDLE		hPolyCount = 0;
POINT		*pArry = NULL;
int			*wPolyCount = NULL;
DXYType		*pdArry = NULL;
int			nDArryMax;
int			nDIdx;
int			nRejionCount;
int			nArryIdx, nSIdx;
DBL			dRltvSx, dRltvSy;
DBL			dRltvEx, dRltvEy;
int			nFillCrtx, nFillCrty;
int			nErrorFlag;
int			callcount = 0;

FILE		*m_fpPlgn;

DllExport int plgnfillinit()
{
	long	size;

	// _tfopen_s(&m_fpPlgn, _T("c:\\plgnfill.txt"), _T("w"));

	if(hPolyCount == 0){
		size = sizeof(int) * (REJIONCNYMAX+16);
		hPolyCount = GlobalAlloc(GHND, size);
		size = sizeof(POINT) * (PLGNPOINTMAX+16);
		hArry = GlobalAlloc(GHND, size);
		nDArryMax = size / (sizeof(DBL) * 2);
		if(hArry != 0){
			pArry = (POINT *)GlobalLock(hArry);
		}
		if(hPolyCount != 0){
			wPolyCount = (int *)GlobalLock(hPolyCount);
		}
		callcount = 0;
	}
	else{
		callcount++;
	}
	return(0);
}

DllExport void plgnfillend()
{
	//fclose(m_fpPlgn);

	if(callcount <= 0){
		//nErrorFlag = OFF;
		if(pArry != NULL){
			GlobalUnlock(hArry);
		}
		if(hPolyCount != NULL){
			GlobalUnlock(wPolyCount);
		}
		if(hArry != 0){
			GlobalFree(hArry);
		}
		if(hPolyCount != 0){
			GlobalFree(hPolyCount);
		}
	}
	else{
		callcount--;
	}
}

DllExport void plgndbllock()
{
	pdArry = (DXYType*)GlobalLock(hArry);
}

DllExport void plgndblunlock()
{
	if(pdArry != NULL){
		GlobalUnlock(hArry);
		pdArry = NULL;
	}
}

DllExport void plgnsetdpointmax(int idx)
{
	nDIdx = idx;
}

DllExport int plgngetdpointmax()
{
	return(nDIdx);
}

DllExport void plgnsetdpoint(DBL x, DBL y)
{
	if(pdArry == NULL || nDArryMax <= nDIdx){
		return;
	}
	if(nDIdx == 0 || !(pdArry[nDIdx-1].x == x && pdArry[nDIdx-1].y == y)){
		pdArry[nDIdx].x = x;
		pdArry[nDIdx].y = y;
		nDIdx++;
	}
}

DllExport void plgngetdpoint(int idx, DBL *x, DBL *y)
{
	if(pdArry == NULL || nDArryMax <= idx){
		*x = 0;
		*y = 0;
		return;
	}
	*x = pdArry[idx].x;
	*y = pdArry[idx].y;
}

DllExport int plgnfillerrorchk()
{
	return(nErrorFlag);
}

DllExport int plgnfillerror(int flag)
{
	nErrorFlag = flag;
	return(TRUE);
}

DllExport void plgnfillstart()
{
	//nErrorFlag = OFF;
	nArryIdx = 0;
	nRejionCount = 0;
	nFillCrtx = -10;
	nFillCrty = -10;
}

DllExport void plgnfilldraw(HDC PaintDC)
{
	//TCHAR	msg[RECMAX];
	HPEN	hOldPen;

	if(nArryIdx != 0){
		hOldPen = (HPEN)SelectObject(PaintDC, GetStockObject(NULL_PEN));
		PolyPolygon(PaintDC, pArry, wPolyCount, nRejionCount);
		SelectObject(PaintDC, hOldPen);
		//_stprintf_s(msg, "ëçì_å¬êî %d ÉäÅ[ÉWÉáÉìêî %d\n", nArryIdx, nRejionCount);
		//baselibFputs(msg, m_fpPlgn);
	}
}

DllExport void plgnfillmove(HDC PaintDC, DBL x1, DBL y1)
{
	plgnfillmoveout(PaintDC, x1, y1);
}

DllExport void plgnfillline(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2)
{
	plgnfilllineout(PaintDC, x1, y1, x2, y2);
}

DllExport void plgnfillbeze(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	int	stat1, stat2, stat3, stat4;
	int	mode;

	stat1 = scrnrltvareachk(x1, y1, x2, y2);
	stat2 = scrnrltvareachk(x2, y2, x3, y3);
	stat3 = scrnrltvareachk(x3, y3, x4, y4);
	stat4 = scrnrltvareachk(x1, y1, x4, y4);
	if(stat1 == OFF && stat2 == OFF 
	&& stat3 == OFF && stat4 == OFF){
		plgnfilloutline(PaintDC, x1, y1, x4, y4);
		return;
	}

	mode = senvcheckdrawmode(); 
	if(mode == BITMAPVIEW){
		plgnfillbeze10(PaintDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}	
	else{
		plgnfillbezediv(PaintDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}
}

DllExport void plgnfillbeze10(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL	sx, sy, ex, ey;
	DBL	t, t1, b1, b2, b3, b4;

	sx = x1; sy = y1;
	for(t = 0.1; t < 1.0; t += 0.1){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		ex = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ey = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		plgnfilllineout(PaintDC, sx, sy, ex, ey);
		sx = ex; sy = ey;
	}
	plgnfilllineout(PaintDC, sx, sy, x4, y4);
}

DllExport void plgnfillbezediv(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL	divrag, divmini, divmax;
	DBL	degree1;
	DBL	rag0, rag1, rag2, rag3;
	DBL	subrag1, subrag2, subrag3, subrag4, subrag;
	int	count;
	DBL	step;
	DBL	t, t1, b1, b2, b3, b4;
	DBL	sx, sy, ex, ey;

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
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		ex = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ey = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		plgnfilllineout(PaintDC, sx, sy, ex, ey);
		sx = ex; sy = ey;
	}
	plgnfilllineout(PaintDC, sx, sy, x4, y4);
}

DllExport void plgnfillarc(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	int		stat, stat1, stat2;
	DBL		x0, y0;
	DBL		rag01, rag03, dr;

	stat1 = scrnrltvareachk(x1, y1, x2, y2);
	stat2 = scrnrltvareachk(x2, y2, x3, y3);
	if(stat1 == OFF && stat2 == OFF){
		plgnfilloutline(PaintDC, x1, y1, x3, y3);
		return;
	}

	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat == FAIL){
		plgnfilllineout(PaintDC, x1, y1, x2, y2);
		plgnfilllineout(PaintDC, x2, y2, x3, y3);
		return;
	}
	stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &rag01, &rag03, &dr);
	if(stat == FAIL){
		plgnfilllineout(PaintDC, x1, y1, x2, y2);
		plgnfilllineout(PaintDC, x2, y2, x3, y3);
		return;
	}

	plgnfillenko(PaintDC, x0, y0, x1, y1, x3, y3, dr, rag01, rag03);
}

DllExport void plgnfillfreecurve(HDC PaintDC, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	int		stat;
	DBL		rag1, rag2;
	int		rag1no, rag2no;
	DBL		xp1, yp1, xp2, yp2;

	stat = scrnrltvareachk(x1, y1, x2, y2);
	if(stat == OFF){
		plgnfilloutline(PaintDC, x1, y1, x2, y2);
		return;
	}

	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);

	if(rag1 == NORAG && rag2 == NORAG){
		plgnfilllineout(PaintDC, x1, y1, x2, y2);
	}
	else{
		curvenoragtocurverag(&rag1, &rag2);
		rag1no = curveragnoget(rag1);
		rag2no = curveragnoget(rag2);

		if(rag1no == rag2no || rag1no == HALFPAI || rag2no == HALFPAI){
			curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
			curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
			plgnfillbeze(PaintDC, x1, y1, xp1, yp1, xp2, yp2, x2, y2);
		}
		else{
			if(curvedivragchk(rag1, rag2)){
				plgnfillragenko(PaintDC, rag1, x1, y1, x2, y2);
			}
			else{
				plgnfilldivragenko(PaintDC, rag1, x1, y1, x2, y2, rag2);
			}
		}
	}
}

void plgnfillenko(HDC PaintDC, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL dr, DBL srag, DBL erag)
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
		plgnfilllineout(PaintDC, sx, sy, ex, ey);
		sx = ex; sy = ey;
	}
	plgnfilllineout(PaintDC, sx, sy, x2, y2);
}

void plgnfillragenko(HDC PaintDC, DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL	x0, y0, rag01, rag02, r;

	if(curvecenterget(rag1, x1, y1, x2, y2, &x0, &y0) == FAIL){
		plgnfilllineout(PaintDC, x1, y1, x2, y2);
		return;
	}
	if(curveragrget(x1, y1, x2, y2, x0, y0, &rag01, &rag02, &r) == FAIL){
		plgnfilllineout(PaintDC, x1, y1, x2, y2);
		return;
	}
	plgnfillenko(PaintDC, x0, y0, x1, y1, x2, y2, r, rag01, rag02);
}

void plgnfilldivragenko(HDC PaintDC, DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL rag2)
{
	DBL		rag, ragp, x12, y12;

	if(curvedivpointget(rag1, x1, y1, x2, y2, rag2, &x12, &y12) == FAIL){
		plgnfilllineout(PaintDC, x1, y1, x2, y2);
		return;
	}
	ragp = atan2(y12 - y1, x12 - x1) - atan2(y2 - y1, x2 - x1);
	rag = rag1-ragp;
	if(x1 != x12 || y1 != y12){
		plgnfillragenko(PaintDC, rag, x1, y1, x12, y12);
	}
	ragp = atan2(y12 - y2, x12 - x2) - atan2(y1 - y2, x1 - x2);
	rag = ragp-rag2;
	if(x2 != x12 || y2 != y12){
		plgnfillragenko(PaintDC, rag, x12, y12, x2, y2);
	}
}

void plgnfillmoveout(HDC PaintDC, DBL dx, DBL dy)
{
	plgnfillmoveto(PaintDC, dx, dy);
}

void plgnfilllineout(HDC PaintDC, DBL sx, DBL sy, DBL ex, DBL ey)
{
	int		stat, ret;
	DBL		x1, y1, x2, y2;

	x1 = sx; y1 = sy;
	x2 = ex; y2 = ey;
	stat = scrnrltvareachk(sx, sy, ex, ey);
	if(stat == OFF){
		plgnfilloutline(PaintDC, sx, sy, ex, ey);
		return;
	}
	if(stat == CROSS){
		ret = scrnrltvarealegal(&sx, &sy, &ex, &ey);
		if(ret == OFF){
			plgnfilloutline(PaintDC, x1, y1, x2, y2);
			return;
		}
	}

	if(x1 != sx || y1 != sy){
		plgnfilloutline(PaintDC, x1, y1, sx, sy);
	}
	plgnfilllineto(PaintDC, sx, sy);
	plgnfilllineto(PaintDC, ex, ey);
	if(x2 != ex || y2 != ey){
		plgnfilloutline(PaintDC, ex, ey, x2, y2);
	}
}

DllExport void plgnfillareaset(DBL sx, DBL sy, DBL ex, DBL ey)
{
	dRltvSx = sx;
	dRltvSy = sy;
	dRltvEx = ex;
	dRltvEy = ey;
}

DllExport void plgnfillmoveto(HDC PaintDC, DBL dx, DBL dy)
{
	int		x, y;
	int		flagx, flagy;

	flagx = plgnfillareasignget(dx, dRltvSx, dRltvEx);
	flagy = plgnfillareasignget(dy, dRltvSy, dRltvEy);
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
	nSIdx = nArryIdx;
	plgnfillarrypointset(x, y);
}

DllExport void plgnfilloutline(HDC PaintDC, DBL sx, DBL sy, DBL ex, DBL ey)
{
	int	flagx1, flagy1, flagx2, flagy2;
	int	x1, y1, x12, y12, x2, y2;
	DBL	ans1, ans2;

	flagx1 = plgnfillareasignget(sx, dRltvSx, dRltvEx);
	flagy1 = plgnfillareasignget(sy, dRltvSy, dRltvEy);
	flagx2 = plgnfillareasignget(ex, dRltvSx, dRltvEx);
	flagy2 = plgnfillareasignget(ey, dRltvSy, dRltvEy);

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
		plgnfillarrypointset(x1, y1);
		plgnfillarrypointset(x2, y2);
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
		plgnfillarrypointset(x1, y1);
		plgnfillarrypointset(x12, y12);
		plgnfillarrypointset(x2, y2);
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
		plgnfillarrypointset(x1, y1);
		plgnfillarrypointset(x12, y12);
		plgnfillarrypointset(x2, y2);
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
		plgnfillarrypointset(x1, y1);
		plgnfillarrypointset(x12, y12);
		plgnfillarrypointset(x2, y2);
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
		plgnfillarrypointset(x1, y1);
		plgnfillarrypointset(x12, y12);
		plgnfillarrypointset(x2, y2);
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
		plgnfillarrypointset(x1, y1);
		plgnfillarrypointset(x12, y12);
		plgnfillarrypointset(x2, y2);
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
		plgnfillarrypointset(x1, y1);
		plgnfillarrypointset(x12, y12);
		plgnfillarrypointset(x2, y2);
	}
}

int plgnfillareasignget(DBL num, DBL mini, DBL max)
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

DllExport void plgnfilllineto(HDC PaintDC, DBL dx, DBL dy)
{
	int		x, y;

	x = (int)dx; y = (int)dy;
	plgnfillarrypointset(x, y);
}

DllExport void plgnfillrejionstart()
{
	nSIdx = nArryIdx;
	nFillCrtx = -10;
	nFillCrty = -10;
}

DllExport void plgnfillrejionend()
{
	if(nRejionCount >= REJIONCNYMAX){
		return;
	}
	if(nArryIdx == 0){
		return;
	}
	wPolyCount[nRejionCount] = nArryIdx - nSIdx;
	if(wPolyCount[nRejionCount] < 2){
		nArryIdx -= wPolyCount[nRejionCount];
	}
	else{
		nRejionCount++;
	}
}

void plgnfillarrypointset(int x, int y)
{
	if(nArryIdx >= PLGNPOINTMAX){
		if(nErrorFlag == OFF){
			MessageBox( NULL, _T("ì_ÇÃëçêîÇ™ëΩÇ¢ÇΩÇﬂìhÇËÇ¬Ç‘ÇµÇ≈Ç´Ç‹ÇπÇÒ"), NULL, MB_OK );
			nErrorFlag = ON;
		}
		return;
	}
	if(nFillCrtx == x
	&& nFillCrty == y){
		return;
	}
	pArry[nArryIdx].x = x;
	pArry[nArryIdx].y = y;
	nFillCrtx = x;
	nFillCrty = y;
	nArryIdx++;
}

