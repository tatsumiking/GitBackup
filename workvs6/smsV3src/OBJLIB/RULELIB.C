//
// Easy Scanner System Ver1.0
//
// ess  rulelib.c  罫線形状データ関数
//
//  Copyright (c) 1998 by THEON

#include	<windows.h>

#include	<stdio.h>
#include	<math.h>
#include	"..\stdef.h"
#include	"dlldef.h"

DBL curve2divcrnr(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
DBL curvevertical(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);

#define		DIVCALC		1

#include	"curvelib.fnc"
#include	"pdrwlib.fnc"
#include	"senvlib.fnc"
#include	"scrnlib.fnc"
#include	"rulelib.fnc"

extern char	exepath[FNAMEMAX+2];

int	nFirstFlag;

DllExport void rulelibgetkind(int kindno, long *pattern, int *style, char *stylename)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	int		tno, no;
	long	ptn;
	int		type;
	char	name[RECMAX];

	*pattern = 0;
	*style = 0;
	sprintf(fname, "%s\\ptnstyle.env", exepath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	tno = 0;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%d %lx %d %s", &no, &ptn, &type, &name);
		if(tno == kindno){
			*pattern = ptn;
			*style = type;
			strcpy(stylename, name);
		}
		tno++;
	}
	fclose(fp);
}

DllExport void rulelibmove(HDC PaintDC, DBL x1, DBL y1, long ptn, int style, DBL *sum)
{
	nFirstFlag = ON;
}

DllExport void rulelibline(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, long ptn, int style, DBL *sum)
{
	HPEN	pen;

	pen = senvgetmainpen();
	if(pen != NULL){
		SelectObject(PaintDC, pen);
	}

	ruleliblineout(PaintDC, x1, y1, x2, y2, ptn, style, sum);

	if(pen != NULL){
		pen = senvgetmainpen();
		SelectObject(PaintDC, pen);
	}
}

DllExport void rulelibarc(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, long ptn, int style, DBL *sum)
{
	DBL		x0, y0, rag01, rag03, dr;
	int		stat;

	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat == FAIL){
		ruleliblineout(PaintDC, x1, y1, x2, y2, ptn, style, sum);
		ruleliblineout(PaintDC, x2, y2, x3, y3, ptn, style, sum);
		return;
	}
	stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &rag01, &rag03, &dr);
	if(stat == FAIL){
		ruleliblineout(PaintDC, x1, y1, x2, y2, ptn, style, sum);
		ruleliblineout(PaintDC, x2, y2, x3, y3, ptn, style, sum);
		return;
	}
	rulelibenko(PaintDC, x0, y0, x1, y1, x3, y3, dr, rag01, rag03, ptn, style, sum);
}

DllExport void rulelibbeze(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4, long ptn, int style, DBL *sum)
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
		ruleliblineout(PaintDC, sx, sy, ex, ey, ptn, style, sum);
		sx = ex; sy = ey;
	}
	ruleliblineout(PaintDC, sx, sy, x4, y4, ptn, style, sum);
}

DllExport void rulelibfreecurve(HDC PaintDC, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, long ptn, int style, DBL *sum)
{
	DBL	rag1, rag2;
	int	rag1no, rag2no;
	DBL	xp1, yp1, xp2, yp2;

	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);

	if(rag1 == NORAG && rag2 == NORAG){
		ruleliblineout(PaintDC, x1, y1, x2, y2, ptn, style, sum);
	}
	else{
		curvenoragtocurverag(&rag1, &rag2);
		rag1no = curveragnoget(rag1);
		rag2no = curveragnoget(rag2);

		if(rag1no == rag2no || rag1no == HALFPAI || rag2no == HALFPAI){
			curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
			curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
			rulelibbeze(PaintDC, x1, y1, xp1, yp1, xp2, yp2, x2, y2, ptn, style, sum);
		}
		else{
			if(curvedivragchk(rag1, rag2)){
				rulelibragenko(PaintDC, rag1, x1, y1, x2, y2, ptn, style, sum);
			}
			else{
				rulelibdivragenko(PaintDC, rag1, x1, y1, x2, y2, rag2, ptn, style, sum);
			}
		}
	}
}

void rulelibenko(HDC PaintDC, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL dr, DBL srag, DBL erag, long ptn, int style, DBL *sum)
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
		ruleliblineout(PaintDC, sx, sy, ex, ey, ptn, style, sum);
		sx = ex; sy = ey;
	}
	ruleliblineout(PaintDC, sx, sy, x2, y2, ptn, style, sum);
}

void rulelibragenko(HDC PaintDC, DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, long ptn, int style, DBL *sum)
{
	DBL	x0, y0, rag01, rag02, r;

	if(curvecenterget(rag1, x1, y1, x2, y2, &x0, &y0) == FAIL){
		ruleliblineout(PaintDC, x1, y1, x2, y2, ptn, style, sum);
		return;
	}
	if(curveragrget(x1, y1, x2, y2, x0, y0, &rag01, &rag02, &r) == FAIL){
		ruleliblineout(PaintDC, x1, y1, x2, y2, ptn, style, sum);
		return;
	}
	rulelibenko(PaintDC, x0, y0, x1, y1, x2, y2, r, rag01, rag02, ptn, style, sum);
}

void rulelibdivragenko(HDC PaintDC, DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL rag2, long ptn, int style, DBL *sum)
{
	DBL		rag, ragp, x12, y12;

	if(curvedivpointget(rag1, x1, y1, x2, y2, rag2, &x12, &y12) == FAIL){
		ruleliblineout(PaintDC, x1, y1, x2, y2, ptn, style, sum);
		return;
	}
	ragp = atan2(y12 - y1, x12 - x1) - atan2(y2 - y1, x2 - x1);
	rag = rag1-ragp;
	if(x1 != x12 || y1 != y12){
		rulelibragenko(PaintDC, rag, x1, y1, x12, y12, ptn, style, sum);
	}
	ragp = atan2(y12 - y2, x12 - x2) - atan2(y1 - y2, x1 - x2);
	rag = ragp-rag2;
	if(x2 != x12 || y2 != y12){
		rulelibragenko(PaintDC, rag, x12, y12, x2, y2, ptn, style, sum);
	}
}

void ruleliblineout(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, long ptn, int style, DBL *sum)
{
	int		ret;
	int		x, y;
	POINT	point;
	DBL		len;

	ret = scrnrltvareachk(x1, y1, x2, y2);
	if(ret != INAREA){
		len = hypot(x1-x2, y1-y2);
		(*sum) += len;
		nFirstFlag = ON;
		return;
	}
	if(nFirstFlag == ON){
		x = (int)x1; y = (int)y1;
		MoveToEx(PaintDC, x, y, &point);
		nFirstFlag = OFF;
	}
	switch(style){
	case 0:
		x = (int)x2; y = (int)y2;
		LineTo(PaintDC, x, y);
		break;
	case 1:
		rulelibdotlineout(PaintDC, x1, y1, x2, y2, ptn, sum);
		break;
	case 2:
		rulelibwavelineout(PaintDC, x1, y1, x2, y2, ptn, sum);
		break;
	}
}

void rulelibdotlineout(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, long ptn, DBL *sum)
{
	DBL		restlen;
	int		nthick;
	DBL		div;
	ULONG	maskbit, crtbit;
	int		flag;
	DBL		cnt;
	long	lsum;
	int		sx, sy, ex, ey;
	POINT	point;

	restlen = hypot(x2-x1, y2-y1);	// 線分の長さ
	if(restlen == 0){
		return;
	}
	nthick = senvgetplgnthick();	// １ドットの長さ
	if(nthick < 1){
		nthick = 1;
	}
	div = (DBL)(nthick) / restlen;	// 線の長さに対する１ドットの長さの比率
									// 現在の位置を計算するのに使用する
	maskbit = 0x80000000;
	flag = OFF;				// 現在のドット位置
	cnt = 0;
	if(restlen < nthick){
		// １ドットの長さよりも短いとき
		lsum = (long)(*sum);
		crtbit = maskbit >> ((long)(lsum / nthick) % 32);
		if(ptn & crtbit){
			sx = (int)x1; sy = (int)y1;
			ex = (int)x2; ey = (int)y2;
			MoveToEx(PaintDC, sx, sy, &point);
			LineTo(PaintDC, ex, ey);
		}
		(*sum) += restlen;
		return;
	}
	
	sx = (int)x1; sy = (int)y1;
	while(1){
		if(restlen < nthick){
			break;
		}
		lsum = (long)(*sum);
		crtbit = maskbit >> ((long)(lsum / (DBL)nthick) % 32);
		if(ptn & crtbit){
			if(flag == OFF){
				sx = (int)((x2-x1) * div * cnt + x1);
				sy = (int)((y2-y1) * div * cnt + y1);
				flag = ON;
			}
		}
		else{
			if(flag == ON){
				ex = (int)((x2-x1) * div * cnt + x1);
				ey = (int)((y2-y1) * div * cnt + y1);
				MoveToEx(PaintDC, sx, sy, &point);
				LineTo(PaintDC, ex, ey);
				flag = OFF;
			}
		}
		(*sum) += (DBL)nthick;
		restlen -= (DBL)nthick;
		cnt += 1;
	}
	if(flag == ON){
		ex = (int)x2;
		ey = (int)y2;
		MoveToEx(PaintDC, sx, sy, &point);
		LineTo(PaintDC, ex, ey);
		(*sum) += restlen;
	}
}

void rulelibwavelineout(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, long ptn, DBL *sum)
{
	DBL		restlen;
	int		nthick;
	DBL		div;
	long	piclen;
	DBL		cnt;
	DBL		sx, sy, ex, ey;
	DBL		cx, cy, csx, csy, cex, cey;
	POINT	point;

	restlen = hypot(x2-x1, y2-y1);	// 線分の長さ
	nthick = senvgetplgnthick();	// １ドットの長さ
	if(nthick < 1){
		nthick = 1;
	}
	piclen = nthick * ptn;	// パターン１単位の長さ
	div = (DBL)piclen / restlen;	// 線の長さに対する１ドットの長さの比率
									// 現在の位置を計算するのに使用する
	sx = x1; sy = y1;
	cnt = 1;
	while(1){
		if(restlen < piclen){
			break;
		}
		ex = (x2-x1) * div * cnt + x1;
		ey = (y2-y1) * div * cnt + y1;

		cx = (sx + ex) /2;
		cy = (sy + ey) /2;
		csx = (sy-cy) + cx;
		csy = -(sx-cx) + cy;
		cex = (ey-cy) + cx;
		cey = -(ex-cx) + cy;

		pdrwbezelow(PaintDC, sx, sy, csx, csy, cex, cey, ex, ey);

		sx = ex; sy = ey;
		(*sum) += (DBL)piclen;
		restlen -= (DBL)piclen;
		cnt += 1;
	}
	ex = x2; ey = y2;
	MoveToEx(PaintDC, (int)sx, (int)sy, &point);
	LineTo(PaintDC, (int)ex, (int)ey);
	(*sum) += restlen;
}

