
#include	<windows.h>

#include	<stdio.h>
#include	<math.h>

#include	"..\stdef.h"
#include	"dlldef.h"

#include	"pdrwlib.fnc"
#include	"curvelib.fnc"
#include	"baselib.fnc"
#include	"senvlib.fnc"
#include	"epsflib.fnc"

#define		EPSLINELIMIT	0.1

#define	INTI	25.4

char	defepsfhead[] = "posthead.def";		//ds ‚d‚o‚rƒtƒ@ƒCƒ‹‚g‚d‚`‚c
char	defepsftail[] = "posttail.def";		//ds ‚d‚o‚rƒtƒ@ƒCƒ‹‚s‚`‚h‚k
int		epsfmoveflag;
int		nFill, nOutline;

extern DBL	pai, hpai, degree1, degree01, degree5;
FILE	*epsfp;

DllExport int epsffileopen(char *filename)
{
	if((epsfp = fopen(filename, "wb")) == NULL){
		return(FALSE);
	}
	return(TRUE);
}

DllExport int epsffileclose()
{
	fclose(epsfp);
	return(TRUE);
}

DllExport int epsfstartfile(char *fname, DBL maxx, DBL maxy)
{
	char	exepath[FNAMEMAX];
	char	deffname[FNAMEMAX];
	char	buffer[RECMAX];

	epsfheadwrite(fname, maxx, maxy);
	senvgetexepath(exepath);
	sprintf(deffname, "%s\\%s", exepath, defepsfhead);
	if(epsffilecopy(deffname) == FALSE){
		return(FALSE);
	}
	sprintf(buffer, "[]0 d");
	epsfputs(buffer);
	sprintf(buffer, "0.0 G");
	epsfputs(buffer);
	sprintf(buffer, "1 w");
	epsfputs(buffer);
	return(TRUE);
}

DllExport int epsfboxsstrgsave(LPSTR strg, int ty, int od, DBL fl, DBL sp, DBL sx, DBL sy, DBL ex, DBL ey)
{
	char	buffer[RECMAX];

	epsfstepxyget(&sx, &sy);
	epsfstepxyget(&ex, &ey);
	sprintf(buffer, "1 To");
	epsfputs(buffer);
	sprintf(buffer, "1 0 0 1 %4.1lf %4.1lf 0 Tp", sx, sy);
	epsfputs(buffer);
	sprintf(buffer, "%d Tv", ty);
	epsfputs(buffer);
	sprintf(buffer, "0 i");
	epsfputs(buffer);
	sprintf(buffer, "0 J 0 j 1 w 4 M []0 d");
	epsfputs(buffer);
	sprintf(buffer, "0 D");
	epsfputs(buffer);
	sprintf(buffer, "%4.1lf %4.1lf m", ex, sy);
	epsfputs(buffer);
	sprintf(buffer, "%4.1lf %4.1lf L", ex, ey);
	epsfputs(buffer);
	sprintf(buffer, "%4.1lf %4.1lf L", sx, ey);
	epsfputs(buffer);
	sprintf(buffer, "%4.1lf %4.1lf L", sx, sy);
	epsfputs(buffer);
	sprintf(buffer, "%4.1lf %4.1lf L", ex, sy);
	epsfputs(buffer);
	sprintf(buffer, "n");
	epsfputs(buffer);
	sprintf(buffer, "TP");
	epsfputs(buffer);
	sprintf(buffer, "0 Tr");
	epsfputs(buffer);
	sprintf(buffer, "0 O");
	epsfputs(buffer);
	sprintf(buffer, "0 0 0 1 k");
	epsfputs(buffer);
	if(ty == 0){
		sprintf(buffer, "/_MS-Mincho %4.1lf Tf", ey - sy);
	}
	else{
		sprintf(buffer, "/_MS-Mincho %4.1lf Tf", ex - sx);
	}
	epsfputs(buffer);
	sprintf(buffer, "0 Ts");
	epsfputs(buffer);
	sprintf(buffer, "%4.1lf 100 Tz", fl);
	epsfputs(buffer);
	sprintf(buffer, "%4.1lf Tt", sp);
	epsfputs(buffer);
	sprintf(buffer, "0 TA");
	epsfputs(buffer);
	sprintf(buffer, "%d TV", ty);
	epsfputs(buffer);
	sprintf(buffer, "0 0 5 TC");
	epsfputs(buffer);
	sprintf(buffer, "100 100 200 TW");
	epsfputs(buffer);
	sprintf(buffer, "0 0 0 Ti");
	epsfputs(buffer);
	sprintf(buffer, "%d Ta", od);
	epsfputs(buffer);
	sprintf(buffer, "0 Tq");
	epsfputs(buffer);
	sprintf(buffer, "0 0 Tl");
	epsfputs(buffer);
	sprintf(buffer, "0 Tc");
	epsfputs(buffer);
	sprintf(buffer, "0 Tw");
	epsfputs(buffer);
	epsfstrgout(strg);
	sprintf(buffer, "TO");
	epsfputs(buffer);
	return(TRUE);
}

DllExport int epsfendfile()
{
	char	exepath[FNAMEMAX];
	char	deffname[FNAMEMAX];

	senvgetexepath(exepath);
	sprintf(deffname, "%s\\%s", exepath, defepsftail);
	if(epsffilecopy(deffname) == FALSE){
		return(FALSE);
	}
	return(TRUE);
}

DllExport int epsfstartblock()
{
	char	buffer[RECMAX];

	sprintf(buffer, "*u");
	epsfputs(buffer);
	epsfmoveflag = OFF;
	return(TRUE);
}

DllExport int epsfoutlinethick(DBL thickmm)
{
	DBL		thickpt;
	char	buffer[RECMAX];

	thickpt = thickmm * MMTOPOINT;
	if(thickpt < 0.01){
		thickpt = 0.01;
	}
	sprintf(buffer, "%4.1lf w", thickpt);
	epsfputs(buffer);
	return(TRUE);
}

DllExport int epsffillclrset(int rval, int gval, int bval)
{
	char	buffer[RECMAX];
	DBL		r, g, b;
	DBL		c, m, y, k;

	if(rval == -1 && gval == -1 && bval == -1){
		nFill = OFF;
		return(FALSE);
	}
	r = rval; g = gval; b = bval;
	nFill = ON;
	c = 1.0 - r / 255;
	m = 1.0 - g / 255;
	y = 1.0 - b / 255;
	k = min(c, m);
	k = min(k, y);
	c = c - k;
	m = m - k;
	y = y - k;
	sprintf(buffer, "%0.3lf %0.3lf %0.3lf %0.3lf k", c, m, y, k); 
	epsfputs(buffer);
	return(TRUE);
}

DllExport int epsfoutlineclrset(int rval, int gval, int bval)
{
	char	buffer[RECMAX];
	DBL		r, g, b;
	DBL		c, m, y, k;

	if(rval == -1 && gval == -1 && bval == -1){
		nOutline = OFF;
		return(FALSE);
	}
	nOutline = ON;
	r = rval; g = gval; b = bval;
	c = 1.0 - r / 255;
	m = 1.0 - g / 255;
	y = 1.0 - b / 255;
	k = min(c, m);
	k = min(k, y);
	c = c - k;
	m = m - k;
	y = y - k;
	sprintf(buffer, "%0.3lf %0.3lf %0.3lf %0.3lf K", c, m, y, k); 
	epsfputs(buffer);
	return(TRUE);
}

DllExport int epsfendblock()
{
	char	buffer[RECMAX];

	sprintf(buffer, "*U");
	epsfputs(buffer);
	epsfmoveflag = OFF;
	return(TRUE);
}

DllExport int epsfstartrejion()
{
	epsfmoveflag = OFF;
	return(TRUE);
}

DllExport int epsfendrejion()
{
	char	buffer[RECMAX];

	if(nFill == ON && nOutline == ON){
		sprintf(buffer, "b");
	}
	else if(nFill == ON && nOutline == OFF){
		sprintf(buffer, "f");
	}
	else{
		sprintf(buffer, "s");
	}
	epsfputs(buffer);
	return(TRUE);
}

DllExport int epsfstartnorejion()
{
	epsfmoveflag = OFF;
	return(TRUE);
}

DllExport int epsfendnorejion()
{
	char	buffer[RECMAX];

	if(nFill == ON && nOutline == ON){
		sprintf(buffer, "B");
	}
	else if(nFill == ON && nOutline == OFF){
		sprintf(buffer, "F");
	}
	else{
		sprintf(buffer, "S");
	}
	epsfputs(buffer);
	return(TRUE);
}

DllExport int epsflinesave(DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		smx, smy, emx, emy;
	char	buffer[RECMAX];

	smx = x1; smy = y1;
	emx = x2; emy = y2;
	epsfstepxyget(&smx, &smy);
	epsfstepxyget(&emx, &emy);
	if(epsfmoveflag == OFF){
		epsfmoveflag = ON;
		sprintf(buffer, "%1.0lf %1.0lf m", smx, smy);
		epsfputs(buffer);
	}
	sprintf(buffer, "%1.0lf %1.0lf l", emx, emy);
	epsfputs(buffer);
	return(TRUE);
}

DllExport int epsfarcsave(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL		len;
	DBL		x0, y0;
	DBL		rag01, rag03, r;
	DBL		subrag, divrag;
	DBL		sx, sy, ex, ey;
	DBL		srag, erag;
	int		stat;
	int		divcnt, i;

	len = hypot(x2-x1, y2-y1) + hypot(x3-x2, y3-y2);
	if(len < EPSLINELIMIT){
		epsflinesave(x1, y1, x2, y2);
		epsflinesave(x2, y2, x3, y3);
		return(TRUE);
	}
	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat != FAIL){
		stat = curve3pragrget(x1,y1,x2,y2,x3,y3,x0,y0,&rag01,&rag03,&r);
	}
	if(stat == FAIL){
		epsflinesave(x1, y1, x2, y2);
		epsflinesave(x2, y2, x3, y3);
		return(TRUE);
	}
	subrag = rag03 - rag01;
	if(fabs(subrag) > M_PI_2){
		divcnt = (int)(fabs(subrag) / M_PI_2);
		divcnt++;
	}
	else{
		divcnt = 1;
	}
	divrag = subrag / (DBL)divcnt;
	sx = x1; sy = y1; srag = rag01;
	for(i = 1; i < divcnt; i++){
		erag = rag01 + divrag * (DBL)i;
		ex = cos(erag) * r + x0;
		ey = sin(erag) * r + y0;
		epsfenkotobeze(sx, sy, ex, ey, x0, y0, r, srag, erag);
		sx = ex; sy = ey; srag = erag;
	}
	if(rag03 != srag){
		ex = x3; ey = y3; erag = rag03;
		epsfenkotobeze(sx, sy, ex, ey, x0, y0, r, srag, erag);
	}
	return(TRUE);
}

//p ‰~ŒÊ‚ð‚a‚d‚y‚d‹Èü‚É•ÏŠ·‚µ•Û‘¶‚·‚éŠÖ”
int epsfenkotobeze(double sx, double sy, double ex, double ey, double x0, double y0, double r, double srag, double erag)
//a double sx, sy	//Žn“_À•W
//a double ex, ey	//I“_À•W
//a double x0, y0	//‰~ŒÊ’†SÀ•W
//a double r		//‰~ŒÊ”¼Œa
//a double srag, erag	//ŠJŽnŠp“xAI—¹Šp“x
//r void		//‚È‚µ
{
	double	subrag, ctrlen;
	double	x1, y1, x2, y2, x3, y3, x4, y4;
	double	tsin, tcos;

	subrag = erag - srag;
	//ctrlen = fabs(r * (4.0 / 3.0) * (subrag / pai));
	ctrlen = fabs(r * (4.0 / 3.6) * (subrag / pai));
	if(subrag > 0){
		srag += hpai;
		erag -= hpai;
	}
	else{
		srag -= hpai;
		erag += hpai;
	}

	x1 = sx;
	y1 = sy;

	tcos = cos(srag);
	tsin = sin(srag);
	x2 = ctrlen * tcos + sx;
	y2 = ctrlen * tsin + sy;

	tcos = cos(erag);
	tsin = sin(erag);
	x3 = ctrlen * tcos + ex;
	y3 = ctrlen * tsin + ey;

	x4 = ex;
	y4 = ey;
	epsfbezesave(x1, y1, x2, y2, x3, y3, x4, y4);
	return(TRUE);
}

DllExport int epsffreecurvesave(int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	DBL	rag1, rag2;

	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);
	if(rag1 == NORAG && rag2 == NORAG){
		epsflinesave(x1, y1, x2, y2);
	}
	else{
		epsfsaveragcurve(rag1, x1, y1, x2, y2, rag2);
	}
	return(TRUE);
}

int epsfsaveragcurve(DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL rag2)
{
	curvenoragtocurverag(&rag1, &rag2);
	epsfsaveragbeze(rag1, x1, y1, x2, y2, rag2);
	return(TRUE);
}

int epsfsaveragbeze(DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL rag2)
{
	DBL	xp1, yp1;
	DBL	xp2, yp2;

	curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
	curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
	epsfbezesave(x1, y1, xp1, yp1, xp2, yp2, x2, y2);
	return(TRUE);
}

DllExport int epsfbezesave(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL		xm1, ym1, xm2, ym2, xm3, ym3, xm4, ym4;
	char	buffer[RECMAX];

	xm1 = x1; ym1 = y1;
	xm2 = x2; ym2 = y2;
	xm3 = x3; ym3 = y3;
	xm4 = x4; ym4 = y4;
	epsfstepxyget(&xm1, &ym1);
	epsfstepxyget(&xm2, &ym2);
	epsfstepxyget(&xm3, &ym3);
	epsfstepxyget(&xm4, &ym4);
	if(epsfmoveflag == OFF){
		epsfmoveflag = ON;
		sprintf(buffer, "%1.0lf %1.0lf m", xm1, ym1);
		epsfputs(buffer);
	}
	sprintf(buffer, "%1.0lf %1.0lf %1.0lf %1.0lf %1.0lf %1.0lf c", xm2, ym2, xm3, ym3, xm4, ym4);
	epsfputs(buffer);
	return(TRUE);
}

int epsfenkosave(DBL sx, DBL sy, DBL x0, DBL y0, DBL r, DBL srag, DBL erag)
{
	DBL		smx, smy, xm0, ym0, rm, subrag;
	char	buffer[RECMAX];

	xm0 = x0; ym0 = y0;
	smx = sx; smy = sy;
	rm = r;
	epsfstepxyget(&xm0, &ym0);
	epsfstepxyget(&smx, &smy);
	epsfdottostep(&rm);
	srag = -srag/degree1; erag = -erag/degree1;
	subrag = erag - srag;
	if(epsfmoveflag == OFF){
		epsfmoveflag = ON;
		sprintf(buffer, "%1.0lf %1.0lf m", smx, smy);
		epsfputs(buffer);
	}
	if(subrag > 0){
		sprintf(buffer, "%1.0lf %1.0lf %1.0lf %1.0lf %1.0lf arc",
						xm0, ym0, rm, srag, erag);
		epsfputs(buffer);
	}
	else{
		sprintf(buffer, "%1.0lf %1.0lf %1.0lf %1.0lf %1.0lf arcn",
						xm0, ym0, rm, srag, erag);
		epsfputs(buffer);
	}
	return(TRUE);
}

DllExport void epsfmmxyget(DBL *x, DBL *y)
{
	DBL	mx, my;

	mx = *x; my = *y;
	*x = mx * INTI / 72.0;
	*y = my * INTI / 72.0;
}

DllExport void epsfmmget(DBL *len)
{
	DBL	mlen;

	mlen = *len;
	*len = mlen * INTI / 72.0;
}

int epsfstepxyget(DBL *x, DBL *y)
{
	DBL	mx, my;

	mx = *x; my = *y;
	*x = mx * 72.0 / INTI;
	*y = my * 72.0 / INTI;
	return(TRUE);
}

int epsfdottostep(DBL *num)
{
	DBL	mm;

	mm = *num;
	*num = mm * 72.0 / INTI;
	return(TRUE);
}

int epsfmmtostep(DBL *num)
{
	DBL	mm;

	mm = *num;
	*num = mm * 72.0 / INTI;
	return(TRUE);
}

int epsfheadwrite(char *fname, DBL maxx, DBL maxy)
{
	char	buffer[RECMAX];
	int		yy, mm, dd, ww;
	int		hh, tmm, ss, tss;

	basedateget(&yy, &mm, &dd, &ww);
	basetimeget(&hh, &tmm, &ss, &tss);
	epsfmmtostep(&maxx);
	epsfmmtostep(&maxy);
	sprintf(buffer, "%%!PS-Adobe-3.0 EPSF-3.0");
	epsfputs(buffer);
	sprintf(buffer, "%%%%Creator: Adobe Illustrator(TM) 3.2");
	epsfputs(buffer);
	sprintf(buffer, "%%%%For: (\\212\\224\\216\\256\\211\\357\\216\\320\\203A\\203C\\203\\217\\201[\\203h)");
	epsfputs(buffer);
	sprintf(buffer, "%%%%Title: (%s)", fname);
	epsfputs(buffer);
	if(hh > 12){
		sprintf(buffer, "%%CreationDate: (%02d.%d.%d) (%d:%d PM)", (yy-1900), mm, dd, (hh-12), tmm);
	}
	else{
		sprintf(buffer, "%%%%CreationDate: (%02d.%d.%d) (%d:%d AM)", (yy-1900), mm, dd, hh, tmm);
	}
	epsfputs(buffer);
	sprintf(buffer, "%%%%BoundingBox: 0 0 %4.0lf %4.0lf", maxx, maxy);
	epsfputs(buffer);
	return(TRUE);
}

int epsffilecopy(char fname[])
{
	FILE	*sfp;
	char	buffer[1024];
	int		len;

	if(epsfp == NULL){
		return(FALSE);
	}
	sfp = fopen(fname, "rb");
	if(sfp == NULL){
		return(FALSE);
	}
	while(1){
		len = fread(buffer, 1, 1024, sfp);
		if(len != 0){
			if(buffer[len-1] == 0x1a){
				len--;
			}
			fwrite(buffer, 1, len, epsfp);
		}
		if(len != 1024){
			break;
		}
	}

	fclose(sfp);
	return(TRUE);
}

int epsfstrgout(LPSTR sp)
{
	int		idx, ch;
	char	setstr[RECMAX];
	char	buffer[RECMAX];

	idx = 0;
	while(1){
		if(*sp == '\0'){
			break;
		}
		ch = *sp++;
		ch &= 0x00ff;
		setstr[idx++] = '\\';
		setstr[idx++] = (ch / 64) + 0x30;
		setstr[idx++] = (ch % 64) / 8 + 0x30;
		setstr[idx++] = (ch % 64) % 8 + 0x30;
		if(idx > HRECMAX){
			setstr[idx] = '\0';
			sprintf(buffer, "(%s) Tx 1 0 Tk", setstr);
			epsfputs(buffer);
			idx = 0;
		}
	}
	if(idx != 0){
		setstr[idx] = '\0';
		sprintf(buffer, "(%s) Tx 1 0 Tk", setstr);
		epsfputs(buffer);
	}
	return(TRUE);
}

int epsfputs(char *buffer)
{
	int		len;

	if(epsfp == NULL){
		return(FALSE);
	}
	len = strlen(buffer);
	buffer[len] = 0x0d;
	len++;
	buffer[len] = 0x0a;
	len++;
	buffer[len] = '\0';
	fwrite(buffer, 1, len, epsfp);
	return(TRUE);
}


