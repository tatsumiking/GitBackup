
#include	<windows.h>

#include	<stdio.h>
#include	<math.h>

#include	"..\stdef.h"
#include	"dlldef.h"

#include	"pdrwlib.fnc"
#include	"curvelib.fnc"
#include	"baselib.fnc"
#include	"fdxflib.fnc"

#define		DXFLINELIMIT	1.0

extern DBL	pai, hpai, degree1, degree01, degree5;
FILE	*dxffp;
int		dxflinemode = OFF;

DllExport void fdxflinemodeset(int mode)
{
	dxflinemode = mode;
}

DllExport int fdxffileopen(char *filename)
{
	if((dxffp = fopen(filename, "w")) == NULL){
		return(FALSE);
	}
	return(TRUE);
}

DllExport int fdxffileclose()
{
	fclose(dxffp);
	return(0);
}

DllExport void fdxfstartfile()
{
	char	buffer[RECMAX];

	sprintf(buffer, "0\n");
	fdxfputs(buffer);
	sprintf(buffer, "SECTION\n");
	fdxfputs(buffer);
	sprintf(buffer, "2\n");
	fdxfputs(buffer);
	sprintf(buffer, "ENTITIES\n");
	fdxfputs(buffer);
}

DllExport void fdxfendfile()
{
	char	buffer[RECMAX];

	sprintf(buffer, "0\n");
	fdxfputs(buffer);
	sprintf(buffer, "ENDSEC\n");
	fdxfputs(buffer);
	sprintf(buffer, "0\n");
	fdxfputs(buffer);
	sprintf(buffer, "EOF\n");
	fdxfputs(buffer);
}

DllExport void fdxflinesave(DBL x1, DBL y1, DBL x2, DBL y2)
{
	char	buffer[RECMAX];

	sprintf(buffer, "0\n");
	fdxfputs(buffer);
	sprintf(buffer, "LINE\n");
	fdxfputs(buffer);
	sprintf(buffer, "8\n");
	fdxfputs(buffer);
	sprintf(buffer, "0\n");
	fdxfputs(buffer);
	sprintf(buffer, "62\n");
	fdxfputs(buffer);
	sprintf(buffer, "7\n");
	fdxfputs(buffer);

	sprintf(buffer, "10\n");
	fdxfputs(buffer);
	fdxfputDBL(x1);
	sprintf(buffer, "20\n");
	fdxfputs(buffer);
	fdxfputDBL(y1);

	sprintf(buffer, "11\n");
	fdxfputs(buffer);
	fdxfputDBL(x2);
	sprintf(buffer, "21\n");
	fdxfputs(buffer);
	fdxfputDBL(y2);
}

DllExport void fdxfarcsave(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	len;
	DBL	x0, y0;
	DBL	rag01, rag03, r;
	int		stat;

	len = hypot(x2-x1, y2-y1) + hypot(x3-x2, y3-y2);
	if(len < DXFLINELIMIT){
		fdxflinesave(x1, y1, x2, y2);
		fdxflinesave(x2, y2, x3, y3);
		return;
	}
	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(x1,y1,x2,y2,x3,y3,x0,y0,&rag01,&rag03,&r);
	}
	if(stat == FAIL){
		fdxflinesave(x1, y1, x2, y2);
		fdxflinesave(x2, y2, x3, y3);
		return;
	}

	fdxfenkosave(x0, y0, r, rag01, rag03);
}

DllExport void fdxfbezesave(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL	ax, bx, cx, dx;
	DBL	ay, by, cy, dy;
	DBL	t, x, y;

	fdxfsaveplgnstart(dxffp);

	ax = -x1 + 3.0 * x2 - 3.0 * x3 + x4;
	bx = 3.0 * (x1 - 2.0 * x2 + x3);
	cx = 3.0 * (-x1 + x2);
	dx = x1;

	ay = -y1 + 3.0 * y2 - 3.0 * y3 + y4;
	by = 3.0 * (y1 - 2.0 * y2 + y3);
	cy = 3.0 * (-y1 + y2);
	dy = y1;

	for(t = 0.0; t <= 1.0; t += 0.1){
		x = ax*t*t*t + bx*t*t + cx*t + dx;
		y = ay*t*t*t + by*t*t + cy*t + dy;
		fdxfsaveplgnxy(x, y);
	}
	fdxfsaveplgnend(dxffp);
}

DllExport void fdxffreecurvesave(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	rag1, rag2;

	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);
	if(rag1 == NORAG && rag2 == NORAG){
		fdxflinesave(x1, y1, x2, y2);
	}
	else{
		fdxfsaveragcurve(rag1, x1, y1, x2, y2, rag2);
	}
}

void fdxfsaveragcurve(DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL rag2)
{
	int	rag1no, rag2no;

	curvenoragtocurverag(&rag1, &rag2);
	rag1no = curveragnoget(rag1);
	rag2no = curveragnoget(rag2);
	if(rag1no == rag2no || rag1no == HALFPAI || rag2no == HALFPAI){
		fdxfsaveragbeze(rag1, x1, y1, x2, y2, rag2);
	}
	else{
		fdxfsave2ragenko(rag1, x1, y1, x2, y2, rag2);
	}
}

void fdxfsave2ragenko(DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL rag2)
{
	if(fdxfdivragchk(rag1, rag2)){
		fdxfsaveragenko(rag1, x1, y1, x2, y2);
	}
	else{
		fdxfsave2divenko(rag1, x1, y1, x2, y2, rag2);
	}
}

int fdxfdivragchk(DBL rag1, DBL rag2)
{
	if(rag1 < 0.0){
		rag1 = -rag1;
	}
	if(rag2 < 0.0){
		rag2 = -rag2;
	}
	if(rag1 <= rag2+degree5 && rag1 >= rag2-degree5){
		return(ON);
	}
	return(OFF);
}

void fdxfsaveragenko(DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL	len;
	DBL	x0, y0;
	DBL	rag01, rag02;
	DBL	r;

	len = hypot(x2-x1, y2-y1);
	if(len < DXFLINELIMIT){
		fdxflinesave(x1, y1, x2, y2);
		return;
	}
	if(curvecenterget(rag1, x1, y1, x2, y2, &x0, &y0) == FAIL){
		fdxflinesave(x1, y1, x2, y2);
		return;
	}
	curveragrget(x1, y1, x2, y2, x0, y0, &rag01, &rag02, &r);
	if(rag01 == rag02){
		fdxflinesave(x1, y1, x2, y2);
		return;
	}

	fdxfenkosave(x0, y0, r, rag01, rag02);
}

void fdxfsavearc(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	len;
	DBL	x0, y0;
	DBL	rag01, rag03, r;
	int		stat;

	len = hypot(x2-x1, y2-y1) + hypot(x3-x2, y3-y2);
	if(len < DXFLINELIMIT){
		fdxflinesave(x1, y1, x2, y2);
		fdxflinesave(x2, y2, x3, y3);
		return;
	}
	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(x1,y1,x2,y2,x3,y3,x0,y0,&rag01,&rag03,&r);
	}
	if(stat == FAIL){
		fdxflinesave(x1, y1, x2, y2);
		fdxflinesave(x2, y2, x3, y3);
		return;
	}
	fdxfenkosave(x0, y0, r, rag01, rag03);
}

void fdxfsave2divenko(DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL rag2)
{
	DBL	rag, ragp;
	DBL	x12, y12;

	if(curvedivpointget(rag1, x1, y1, x2, y2, rag2, &x12, &y12) == FAIL){
		fdxflinesave(x1, y1, x2, y2);
		return;
	}
	if(x1 != x12 || y1 != y12){
		ragp = atan2(y12 - y1, x12 - x1) - atan2(y2 - y1, x2 - x1);
		rag = rag1-ragp;
		fdxfsaveragenko(rag, x1, y1, x12, y12);
	}
	if(x2 != x12 || y2 != y12){
		ragp = atan2(y12 - y2, x12 - x2) - atan2(y1 - y2, x1 - x2);
		rag = ragp-rag2;
		fdxfsaveragenko(rag, x12, y12, x2, y2);
	}
}

void fdxfsaveragbeze(DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL rag2)
{
	DBL	xp1, yp1;
	DBL	xp2, yp2;

	curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
	curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
	fdxfsavebeze(x1, y1, xp1, yp1, xp2, yp2, x2, y2);
}

void fdxfsavebeze(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL	ax, bx, cx, dx;
	DBL	ay, by, cy, dy;
	DBL	t, x, y;

	fdxfsaveplgnstart(dxffp);

	ax = -x1 + 3.0 * x2 - 3.0 * x3 + x4;
	bx = 3.0 * (x1 - 2.0 * x2 + x3);
	cx = 3.0 * (-x1 + x2);
	dx = x1;

	ay = -y1 + 3.0 * y2 - 3.0 * y3 + y4;
	by = 3.0 * (y1 - 2.0 * y2 + y3);
	cy = 3.0 * (-y1 + y2);
	dy = y1;

	for(t = 0.0; t <= 1.0; t += 0.1){
		x = ax*t*t*t + bx*t*t + cx*t + dx;
		y = ay*t*t*t + by*t*t + cy*t + dy;
		fdxfsaveplgnxy(x, y);
	}
	fdxfsaveplgnend(dxffp);
}

void fdxfenkosave(DBL x0, DBL y0, DBL r, DBL srag, DBL erag)
{
	DBL		subrag;
	char	buffer[RECMAX];

	if(dxflinemode == ON){
		fdxflineenkosave(x0, y0, r, srag, erag);
		return;
	}
	srag = srag/degree1;
	erag = erag/degree1;
	sprintf(buffer, "0\n");
	fdxfputs(buffer);
	sprintf(buffer, "ARC\n");
	fdxfputs(buffer);
	sprintf(buffer, "8\n");
	fdxfputs(buffer);
	sprintf(buffer, "0\n");
	fdxfputs(buffer);
	sprintf(buffer, "62\n");
	fdxfputs(buffer);
	sprintf(buffer, "7\n");
	fdxfputs(buffer);

	sprintf(buffer, "10\n");
	fdxfputs(buffer);
	fdxfputDBL(x0);
	sprintf(buffer, "20\n");
	fdxfputs(buffer);
	fdxfputDBL(y0);

	subrag = erag - srag;
	if(subrag > 0){
		if(srag < 0){
			srag = 360 + srag;
		}
		if(erag < 0){
			erag = 360 + erag;
		}
		sprintf(buffer, "40\n");
		fdxfputs(buffer);
		fdxfputDBL(r);
		sprintf(buffer, "50\n");
		fdxfputs(buffer);
		fdxfputDBL(srag);
		sprintf(buffer, "51\n");
		fdxfputs(buffer);
		fdxfputDBL(erag);
	}
	else{
		if(srag < 0){
			srag = 360 + srag;
		}
		if(erag < 0){
			erag = 360 + erag;
		}
		sprintf(buffer, "40\n");
		fdxfputs(buffer);
		fdxfputDBL(r);
		sprintf(buffer, "50\n");
		fdxfputs(buffer);
		fdxfputDBL(erag);
		sprintf(buffer, "51\n");
		fdxfputs(buffer);
		fdxfputDBL(srag);
	}
}

void fdxflineenkosave(DBL x0, DBL y0, DBL r, DBL srag, DBL erag)
{
	DBL	subrag, steprag, rag;
	int	cnt, i;
	DBL	x, y;

	subrag = erag - srag;
	cnt = (int)(fabs(subrag) / (degree1 * 5));
	steprag = subrag / (DBL)cnt;
	fdxfsaveplgnstart(dxffp);
	cnt--;
	rag = srag;
	x = cos(srag) * r + x0;
	y = sin(srag) * r + y0;
	fdxfsaveplgnxy(x, y);
	for(i = 1; i < cnt; i++){
		rag = steprag * (DBL)i + srag;
		x = cos(rag) * r + x0;
		y = sin(rag) * r + y0;
		fdxfsaveplgnxy(x, y);
	}
	x = cos(erag) * r + x0;
	y = sin(erag) * r + y0;
	fdxfsaveplgnxy(x, y);
	fdxfsaveplgnend(dxffp);

}

void fdxfsaveplgnstart()
{
	char	buffer[RECMAX];

	sprintf(buffer, "0\n");
	fdxfputs(buffer);
	sprintf(buffer, "POLYLINE\n");
	fdxfputs(buffer);
	sprintf(buffer, "8\n");
	fdxfputs(buffer);
	sprintf(buffer, "0\n");
	fdxfputs(buffer);
	sprintf(buffer, "62\n");
	fdxfputs(buffer);
	sprintf(buffer, "7\n");
	fdxfputs(buffer);
	sprintf(buffer, "66\n");
	fdxfputs(buffer);
	sprintf(buffer, "1\n");
	fdxfputs(buffer);
}

void fdxfsaveplgnxy(DBL x, DBL y)
{
	char	buffer[RECMAX];

	sprintf(buffer, "0\n");
	fdxfputs(buffer);
	sprintf(buffer, "VERTEX\n");
	fdxfputs(buffer);
	sprintf(buffer, "8\n");
	fdxfputs(buffer);
	sprintf(buffer, "0\n");
	fdxfputs(buffer);
	sprintf(buffer, "10\n");
	fdxfputs(buffer);
	fdxfputDBL(x);
	sprintf(buffer, "20\n");
	fdxfputs(buffer);
	fdxfputDBL(y);
}

void fdxfsaveplgnend()
{
	char	buffer[RECMAX];

	sprintf(buffer, "0\n");
	fdxfputs(buffer);
	sprintf(buffer, "SEQEND\n");
	fdxfputs(buffer);
}

void fdxfputDBL(DBL num)
{
	char	buffer[RECMAX];

	sprintf(buffer, "%lf\n", num);
	fdxfputs(buffer);
}

void fdxfputs(LPSTR buffer)
{
	int		len;

	len = strlen(buffer);
	fwrite(buffer, 1, len, dxffp);
}

