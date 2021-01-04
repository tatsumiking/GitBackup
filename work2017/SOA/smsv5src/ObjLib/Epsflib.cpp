
#include	"stdafx.h"
#include	"Pdrwlib.h"
#include	"Curvelib.h"
#include	"Baselib.h"
#include	"Senvlib.h"
#include	"Epsflib.h"

#define		EPSLINELIMIT	0.1

#define	INTI	25.4

TCHAR	defepsfhead[] = _T("posthead.def");		//ds ‚d‚o‚rƒtƒ@ƒCƒ‹‚g‚d‚`‚c
TCHAR	defepsftail[] = _T("posttail.def");		//ds ‚d‚o‚rƒtƒ@ƒCƒ‹‚s‚`‚h‚k
int		epsfmoveflag;
int		nFill, nOutline;

extern DBL	pai, hpai, degree1, degree01, degree5;
FILE	*epsfp;

DllExport int epsffileopen(TCHAR *filename)
{
	_tfopen_s(&epsfp, filename, _T("wb"));
	if(epsfp == NULL){
		return(FALSE);
	}
	return(TRUE);
}

DllExport int epsffileclose()
{
	fclose(epsfp);
	return(TRUE);
}

DllExport int epsfstartfile(TCHAR *fname, DBL maxx, DBL maxy)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	deffname[FNAMEMAX];
	TCHAR	buffer[RECMAX];

	epsfheadwrite(fname, maxx, maxy);
	senvgetexepath(exepath);
	_stprintf_s(deffname, RECMAX, _T("%s\\%s"), exepath, defepsfhead);
	if(epsffilecopy(deffname) == FALSE){
		return(FALSE);
	}
	_stprintf_s(buffer, RECMAX, _T("[]0 d"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("0.0 G"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("1 w"));
	epsbaselibFputs(buffer);
	return(TRUE);
}

DllExport int epsfboxsstrgsave(LPTSTR strg, int ty, int od, DBL fl, DBL sp, DBL sx, DBL sy, DBL ex, DBL ey)
{
	TCHAR	buffer[RECMAX];

	epsfstepxyget(&sx, &sy);
	epsfstepxyget(&ex, &ey);
	_stprintf_s(buffer, RECMAX, _T("1 To"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("1 0 0 1 %4.1lf %4.1lf 0 Tp"), sx, sy);
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("%d Tv"), ty);
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("0 i"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("0 J 0 j 1 w 4 M []0 d"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("0 D"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("%4.1lf %4.1lf m"), ex, sy);
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("%4.1lf %4.1lf L"), ex, ey);
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("%4.1lf %4.1lf L"), sx, ey);
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("%4.1lf %4.1lf L"), sx, sy);
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("%4.1lf %4.1lf L"), ex, sy);
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("n"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("TP"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("0 Tr"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("0 O"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("0 0 0 1 k"));
	epsbaselibFputs(buffer);
	if(ty == 0){
		_stprintf_s(buffer, RECMAX, _T("/_MS-Mincho %4.1lf Tf"), ey - sy);
	}
	else{
		_stprintf_s(buffer, RECMAX, _T("/_MS-Mincho %4.1lf Tf"), ex - sx);
	}
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("0 Ts"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("%4.1lf 100 Tz"), fl);
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("%4.1lf Tt"), sp);
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("0 TA"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("%d TV"), ty);
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("0 0 5 TC"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("100 100 200 TW"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("0 0 0 Ti"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("%d Ta"), od);
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("0 Tq"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("0 0 Tl"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("0 Tc"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("0 Tw"));
	epsbaselibFputs(buffer);
	epsfstrgout(strg);
	_stprintf_s(buffer, RECMAX, _T("TO"));
	epsbaselibFputs(buffer);
	return(TRUE);
}

DllExport int epsfendfile()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	deffname[FNAMEMAX];

	senvgetexepath(exepath);
	_stprintf_s(deffname, FNAMEMAX, _T("%s\\%s"), exepath, defepsftail);
	if(epsffilecopy(deffname) == FALSE){
		return(FALSE);
	}
	return(TRUE);
}

DllExport int epsfstartblock()
{
	TCHAR	buffer[RECMAX];

	_stprintf_s(buffer, RECMAX, _T("*u"));
	epsbaselibFputs(buffer);
	epsfmoveflag = OFF;
	return(TRUE);
}

DllExport int epsfoutlinethick(DBL thickmm)
{
	DBL		thickpt;
	TCHAR	buffer[RECMAX];

	thickpt = thickmm * MMTOPOINT;
	if(thickpt < 0.01){
		thickpt = 0.01;
	}
	_stprintf_s(buffer, RECMAX, _T("%4.1lf w"), thickpt);
	epsbaselibFputs(buffer);
	return(TRUE);
}

DllExport int epsffillclrset(int rval, int gval, int bval)
{
	TCHAR	buffer[RECMAX];
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
	_stprintf_s(buffer, RECMAX, _T("%0.3lf %0.3lf %0.3lf %0.3lf k"), c, m, y, k);
	epsbaselibFputs(buffer);
	return(TRUE);
}

DllExport int epsfoutlineclrset(int rval, int gval, int bval)
{
	TCHAR	buffer[RECMAX];
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
	_stprintf_s(buffer, RECMAX, _T("%0.3lf %0.3lf %0.3lf %0.3lf K"), c, m, y, k);
	epsbaselibFputs(buffer);
	return(TRUE);
}

DllExport int epsfendblock()
{
	TCHAR	buffer[RECMAX];

	_stprintf_s(buffer, RECMAX, _T("*U"));
	epsbaselibFputs(buffer);
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
	TCHAR	buffer[RECMAX];

	if(nFill == ON && nOutline == ON){
		_stprintf_s(buffer, RECMAX, _T("b"));
	}
	else if(nFill == ON && nOutline == OFF){
		_stprintf_s(buffer, RECMAX, _T("f"));
	}
	else{
		_stprintf_s(buffer, RECMAX, _T("s"));
	}
	epsbaselibFputs(buffer);
	return(TRUE);
}

DllExport int epsfstartnorejion()
{
	epsfmoveflag = OFF;
	return(TRUE);
}

DllExport int epsfendnorejion()
{
	TCHAR	buffer[RECMAX];

	if(nFill == ON && nOutline == ON){
		_stprintf_s(buffer, RECMAX, _T("B"));
	}
	else if(nFill == ON && nOutline == OFF){
		_stprintf_s(buffer, RECMAX, _T("F"));
	}
	else{
		_stprintf_s(buffer, RECMAX, _T("S"));
	}
	epsbaselibFputs(buffer);
	return(TRUE);
}

DllExport int epsflinesave(DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		smx, smy, emx, emy;
	TCHAR	buffer[RECMAX];

	smx = x1; smy = y1;
	emx = x2; emy = y2;
	epsfstepxyget(&smx, &smy);
	epsfstepxyget(&emx, &emy);
	if(epsfmoveflag == OFF){
		epsfmoveflag = ON;
		_stprintf_s(buffer, RECMAX, _T("%1.0lf %1.0lf m"), smx, smy);
		epsbaselibFputs(buffer);
	}
	_stprintf_s(buffer, RECMAX, _T("%1.0lf %1.0lf l"), emx, emy);
	epsbaselibFputs(buffer);
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
	TCHAR	buffer[RECMAX];

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
		_stprintf_s(buffer, RECMAX, _T("%1.0lf %1.0lf m"), xm1, ym1);
		epsbaselibFputs(buffer);
	}
	_stprintf_s(buffer, RECMAX, _T("%1.0lf %1.0lf %1.0lf %1.0lf %1.0lf %1.0lf c"), xm2, ym2, xm3, ym3, xm4, ym4);
	epsbaselibFputs(buffer);
	return(TRUE);
}

int epsfenkosave(DBL sx, DBL sy, DBL x0, DBL y0, DBL r, DBL srag, DBL erag)
{
	DBL		smx, smy, xm0, ym0, rm, subrag;
	TCHAR	buffer[RECMAX];

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
		_stprintf_s(buffer, RECMAX, _T("%1.0lf %1.0lf m"), smx, smy);
		epsbaselibFputs(buffer);
	}
	if(subrag > 0){
		_stprintf_s(buffer, RECMAX, _T("%1.0lf %1.0lf %1.0lf %1.0lf %1.0lf arc"),
						xm0, ym0, rm, srag, erag);
		epsbaselibFputs(buffer);
	}
	else{
		_stprintf_s(buffer, RECMAX, _T("%1.0lf %1.0lf %1.0lf %1.0lf %1.0lf arcn"),
						xm0, ym0, rm, srag, erag);
		epsbaselibFputs(buffer);
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

int epsfheadwrite(TCHAR *fname, DBL maxx, DBL maxy)
{
	TCHAR	buffer[RECMAX];
	int		yy, mm, dd, ww;
	int		hh, tmm, ss, tss;

	basedateget(&yy, &mm, &dd, &ww);
	basetimeget(&hh, &tmm, &ss, &tss);
	epsfmmtostep(&maxx);
	epsfmmtostep(&maxy);
	_stprintf_s(buffer, RECMAX, _T("%%!PS-Adobe-3.0 EPSF-3.0"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("%%%%Creator: Adobe Illustrator(TM) 3.2"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("%%%%For: (\\212\\224\\216\\256\\211\\357\\216\\320\\203A\\203C\\203\\217\\201[\\203h)"));
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("%%%%Title: (%s)"), fname);
	epsbaselibFputs(buffer);
	if(hh > 12){
		_stprintf_s(buffer, RECMAX, _T("%%CreationDate: (%02d.%d.%d) (%d:%d PM)"), (yy-1900), mm, dd, (hh-12), tmm);
	}
	else{
		_stprintf_s(buffer, RECMAX, _T("%%%%CreationDate: (%02d.%d.%d) (%d:%d AM)"), (yy-1900), mm, dd, hh, tmm);
	}
	epsbaselibFputs(buffer);
	_stprintf_s(buffer, RECMAX, _T("%%%%BoundingBox: 0 0 %4.0lf %4.0lf"), maxx, maxy);
	epsbaselibFputs(buffer);
	return(TRUE);
}

int epsffilecopy(TCHAR fname[])
{
	FILE	*sfp;
	TCHAR	buffer[1024];
	int		len;

	if(epsfp == NULL){
		return(FALSE);
	}
	_tfopen_s(&sfp, fname, _T("rb"));
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

int epsfstrgout(LPTSTR sp)
{
	int		idx, ch;
	TCHAR	setstr[RECMAX];
	TCHAR	buffer[RECMAX];

	idx = 0;
	while(1){
		if(*sp == _T('\0')){
			break;
		}
		ch = *sp++;
		ch &= 0x00ff;
		setstr[idx++] = _T('\\');
		setstr[idx++] = (ch / 64) + 0x30;
		setstr[idx++] = (ch % 64) / 8 + 0x30;
		setstr[idx++] = (ch % 64) % 8 + 0x30;
		if(idx > HRECMAX){
			setstr[idx] = _T('\0');
			_stprintf_s(buffer, RECMAX, _T("(%s) Tx 1 0 Tk"), setstr);
			epsbaselibFputs(buffer);
			idx = 0;
		}
	}
	if(idx != 0){
		setstr[idx] = _T('\0');
		_stprintf_s(buffer, RECMAX, _T("(%s) Tx 1 0 Tk"), setstr);
		epsbaselibFputs(buffer);
	}
	return(TRUE);
}

int epsbaselibFputs(TCHAR *buffer)
{
	int		len;

	if(epsfp == NULL){
		return(FALSE);
	}
	len = _tcslen(buffer);
	buffer[len] = 0x0d;
	len++;
	buffer[len] = 0x0a;
	len++;
	buffer[len] = _T('\0');
	fwrite(buffer, 1, len, epsfp);
	return(TRUE);
}


