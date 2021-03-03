//
// Easy Scanner For Windows Ver1.0
//
// eswin  senvlib.c  ÉyÉìÉÇÅ[Éhä¬ã´ä÷åWä÷êî
//
//  Copyright (c) 1996 by HARIMA

#include	<windows.h>
#include	<stdio.h>
#include	<math.h>
#include	<io.h>
#include	<direct.h>

#include	"..\stdef.h"
#include	"dlldef.h"

#include	"senvlib.fnc"

#define	NODBG	1

char	exepath[FNAMEMAX+2];
char	datpath[FNAMEMAX+2];
char	prtpath[FNAMEMAX+2];
char	outpath[FNAMEMAX+2];
char	kmnpath[FNAMEMAX+2];
char	wakpath[FNAMEMAX+2];
char	partpath[FNAMEMAX+2];
char	tmppath[FNAMEMAX+2];
char	btppath[FNAMEMAX+2];

int		wakuconerkind;
int		editconerkind;
int		mojiconerkind;
int		mojiwakuflag;

int		senvpoutdispflag;
int		clrmode = CLR;
int		progflag = 0;
int		drawmode;
int		drawpoint;
int		drawbeze;
int		drawgrid;
int		drawimage;
int		drawrejion;
int		optslct = 0;

char	pltfilename[FNAMEMAX+2];
int		pltnomax;
int		crtfillpenno;
int		crtlinepenno;
DBL		dthickmm;
int		nareadotsize;
int		npointdotsize;
int		noselectstrgvect = OFF;

int		crtoutpenno;

int		bkrval, bkgval, bkbval;
int		frrval, frgval, frbval;
int		linerval, linegval, linebval;
int		curvrval, curvgval, curvbval;
int		atnrval, atngval, atnbval;
int		slctrval, slctgval, slctbval;
int		noslctrval, noslctgval, noslctbval;
int		imagerval, imagegval, imagebval;
int		lslctrval, lslctgval, lslctbval;
int		sublrval, sublgval, sublbval;
int		arearval, areagval, areabval;
int		gridrval, gridgval, gridbval;
int		scalrval, scalgval, scalbval;
int		cntrrval, cntrgval, cntrbval;
int		asstrval, asstgval, asstbval;
int		bezerval, bezegval, bezebval;
int		wakurval, wakugval, wakubval;
int		pgsprval, pgspgval, pgspbval;

HPEN		hMainPen, hBezePen, hSubPen, hLinePen, hConrPen, oldhPen;
HBRUSH	hMainBrush, hConrBrush, oldhBrush;
COLORREF	oldClr;
int		nPlgnThick;
int		nConrFlag;
int		nSystemVerjion;
int		nDocWakuNo[100];
int		nCrtDocNo;
int		nPlgnLimit;

#define DIVBEZERAG	5
#define	DIVBEZEMINI	6
#define	DIVBEZEMAX	20
#define DIVARCRAG	5
#define	DIVARCMINI	3
#define	DIVARCMAX	72

DBL	ddrawdivbezerag;
DBL	ddrawdivbezemini;
DBL	ddrawdivbezemax;
DBL	ddrawdivarcrag;
DBL	ddrawdivarcmini;
DBL	ddrawdivarcmax;

DllExport void senvinit()
{
	int		i;

	ddrawdivbezerag = DIVBEZERAG;
	ddrawdivbezemini = DIVBEZEMINI;
	ddrawdivbezemax = DIVBEZEMAX;
	ddrawdivarcrag = DIVARCRAG;
	ddrawdivarcmini = DIVARCMINI;
	ddrawdivarcmax = DIVARCMAX;

	crtoutpenno = -1;
	nConrFlag = OFF;
	crtfillpenno = 0;
	crtlinepenno = 0;
	dthickmm = 0.1;
	senvsetdefscrnclr();
	nareadotsize = 6;
	npointdotsize = 3;
	nSystemVerjion = VER100;
	strcpy(pltfilename, "penvalue.env");
	wakuconerkind = RIGHTUP;
	editconerkind = CNTRCNTR;
	mojiconerkind = LEFTUP;
	mojiwakuflag = OFF;
	drawpoint = ON;
	drawbeze = ON;
	clrmode = CLR;
	drawrejion = OFF;
	senvpoutdispflag = OFF;
	optslct = 0;
	for(i = 0; i < 100; i++){
		nDocWakuNo[i] = 0;
	}
	nCrtDocNo = 0;
	nPlgnLimit = 1;
}

DllExport void senvend()
{
}

DllExport void senvsetplgnlimit(int nlimit)
{
	nPlgnLimit = nlimit;
}

DllExport int senvgetplgnlimit()
{
	return(nPlgnLimit);
}

DllExport void senvsetnoselectstrgvect(int flag)
{
	noselectstrgvect = flag;
}

DllExport int senvchknoselectstrgvect()
{
	return(noselectstrgvect);
}

DllExport void senvinitcrtdocno(int no)
{
	nCrtDocNo = no;
	nDocWakuNo[nCrtDocNo] = 0;
}

DllExport void senvsetcrtdocno(int no)
{
	nCrtDocNo = no;
}

DllExport int	senvgetwakuno()
{
	nDocWakuNo[nCrtDocNo]++;
	return(nDocWakuNo[nCrtDocNo]);
}

DllExport void senvsetoptslct(int mode)
{
	optslct = mode;
}

DllExport int senvcheckoptslct()
{
	return(optslct);
}

DllExport void senvsetpoutdisp(int flag)
{
	senvpoutdispflag = flag;
}

DllExport int senvcheckpoutdisp()
{
	return(senvpoutdispflag);
}

DllExport int senvcheckrejionmode()
{
	return(drawrejion);
}

DllExport void senvsetrejionmode(int flag)
{
	drawrejion = flag;
}

DllExport void senvsetclrmode()
{
	clrmode = CLR;
}

DllExport void senvsetblackmode()
{
	clrmode = BLACK;
}

DllExport int senvgetclrmode()
{
	return(clrmode);
}

DllExport void senvsetcrtpenno(int penno)
{
	crtoutpenno = penno;
}

DllExport int senvgetcrtpenno()
{
	return(crtoutpenno);
}

DllExport void senvsetmojiwaku(int flag)
{
	mojiwakuflag = flag;
}

DllExport int senvcheckmojiwaku()
{
	return(mojiwakuflag);
}

DllExport void senvsetconerkind(int kind)
{
	wakuconerkind = kind;
	editconerkind = kind;
	mojiconerkind = kind;
}

DllExport void senvgetconerkind(LPINT kind)
{
	if(wakuconerkind != RIGHTDOWN
	&& wakuconerkind != RIGHTUP
	&& wakuconerkind != LEFTUP
	&& wakuconerkind != LEFTDOWN
	&& wakuconerkind != CNTRCNTR){
		wakuconerkind = LEFTDOWN;	
	}
	*kind = wakuconerkind;
}

DllExport void senvsetwakuconerkind(int kind)
{
	wakuconerkind = kind;
}

DllExport void senvgetwakuconerkind(LPINT kind)
{
	if(wakuconerkind != RIGHTDOWN
	&& wakuconerkind != RIGHTUP
	&& wakuconerkind != LEFTUP
	&& wakuconerkind != LEFTDOWN
	&& wakuconerkind != CNTRCNTR){
		wakuconerkind = LEFTDOWN;	
	}
	*kind = wakuconerkind;
}

DllExport void senvseteditconerkind(int kind)
{
	editconerkind = kind;
}

DllExport void senvgeteditconerkind(LPINT kind)
{
	if(editconerkind != RIGHTDOWN
	&& editconerkind != RIGHTUP
	&& editconerkind != LEFTUP
	&& editconerkind != LEFTDOWN
	&& editconerkind != CNTRCNTR){
		editconerkind = LEFTDOWN;	
	}
	*kind = editconerkind;
}

DllExport void senvsetmojiconerkind(int kind)
{
	mojiconerkind = kind;
}

DllExport void senvgetmojiconerkind(LPINT kind)
{
	if(mojiconerkind != RIGHTDOWN
	&& mojiconerkind != RIGHTUP
	&& mojiconerkind != LEFTUP
	&& mojiconerkind != LEFTDOWN
	&& mojiconerkind != CNTRCNTR){
		mojiconerkind = LEFTDOWN;	
	}
	*kind = mojiconerkind;
}

DllExport int senvsetverjion(int flag)
{
	nSystemVerjion = flag;
	return(TRUE);
}

DllExport int senvcheckverjion()
{
	return(nSystemVerjion);
}

DllExport void senvsetdefscrnclr()
{
	bkrval = 250; bkgval = 250; bkbval = 250;
	frrval = 0; frgval = 0; frbval = 0;
	linerval = 0; linegval = 0; linebval = 0;
	curvrval = 200; curvgval = 0; curvbval = 0;
	atnrval = 255; atngval = 0; atnbval = 0;
	slctrval = 100; slctgval = 100; slctbval = 80;
	noslctrval = 180; noslctgval = 180; noslctbval = 180;
	imagerval = 100; imagegval = 100; imagebval = 160;
	lslctrval = 0; lslctgval = 128; lslctbval = 128;
	sublrval = 0; sublgval = 150; sublbval = 150;
	arearval = 100; areagval = 100; areabval = 100;
	gridrval = 200; gridgval = 200; gridbval = 200;
	scalrval = 100; scalgval = 100; scalbval = 100;
	cntrrval = 0; cntrgval = 85; cntrbval = 0;
	asstrval = 85; asstgval = 85; asstbval = 0;
	bezerval = 200; bezegval = 0; bezebval = 0;
	wakurval = 0; wakugval = 0; wakubval = 255;
	pgsprval = 0; pgspgval = 255; pgspbval = 0;
}

DllExport void senvsetareadotsize(int dotsize)
{
	nareadotsize = dotsize;
}

DllExport void senvgetareadotsize(int *dotsize)
{
	*dotsize = nareadotsize;
}

DllExport void senvsetpointdotsize(int dotsize)
{
	npointdotsize = dotsize;
}

DllExport void senvgetpointdotsize(int *dotsize)
{
	*dotsize = npointdotsize;
}

DllExport void senvsetdrawdivbeze(DBL rag, DBL mini, DBL max)
{
	ddrawdivbezerag = rag;
	ddrawdivbezemini = mini;
	ddrawdivbezemax = max;
}

DllExport void senvsetdrawdivarc(DBL rag, DBL mini, DBL max)
{
	ddrawdivarcrag = rag;
	ddrawdivarcmini = mini;
	ddrawdivarcmax = max;
}

DllExport void senvgetdrawdivbeze(LPDBL rag, LPDBL mini, LPDBL max)
{
	*rag = ddrawdivbezerag;
	*mini = ddrawdivbezemini;
	*max = ddrawdivbezemax;
}

DllExport void senvgetdrawdivarc(LPDBL rag, LPDBL mini, LPDBL max)
{
	*rag = ddrawdivarcrag;
	*mini = ddrawdivarcmini;
	*max = ddrawdivarcmax;
}

DllExport void senvonflag(int flag)
{
	progflag |= flag; 
}

DllExport void senvoffflag(int flag)
{
	progflag &= ~flag; 
}

DllExport int senvchckflag(int flag)
{
	if(progflag & flag){
		return(ON);
	}
	else{
		return(OFF);
	}
}

DllExport int senvchckstopflag()
{
	int		chk;
	int		ret;

	chk = GetAsyncKeyState(VK_ESCAPE) & (~0x01);
	if(chk != 0){
		senvonflag(STOPFLAG);
	}
	ret = senvchckflag(STOPFLAG);
	return(ret);
}

DllExport int senvinitscrnclr()
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];

	sprintf(fname, "%s\\scrnclr.env", exepath);
	fp = fopen(fname, "r");
	if(fp == NULL){
		return(FALSE);
	}
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d %d %d", &bkrval, &bkgval, &bkbval);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d %d %d", &frrval, &frgval, &frbval);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d %d %d", &linerval, &linegval, &linebval);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d %d %d", &curvrval, &curvgval, &curvbval);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d %d %d", &atnrval, &atngval, &atnbval);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d %d %d", &slctrval, &slctgval, &slctbval);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d %d %d", &noslctrval, &noslctgval, &noslctbval);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d %d %d", &imagerval, &imagegval, &imagebval);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d %d %d", &lslctrval, &lslctgval, &lslctbval);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d %d %d", &sublrval, &sublgval, &sublbval);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d %d %d", &arearval, &areagval, &areabval);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d %d %d", &gridrval, &gridgval, &gridbval);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d %d %d", &scalrval, &scalgval, &scalbval);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d %d %d", &cntrrval, &cntrgval, &cntrbval);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d %d %d", &asstrval, &asstgval, &asstbval);
	if(fgets(buf, RECMAX, fp) == NULL){
		bezerval = curvrval; bezegval = curvgval; bezebval = curvbval;
	}
	else{
		sscanf(buf, "%d %d %d", &bezerval, &bezegval, &bezebval);
	}
	if(fgets(buf, RECMAX, fp) != NULL){
		sscanf(buf, "%d %d %d", &wakurval, &wakugval, &wakubval);
	}
	if(fgets(buf, RECMAX, fp) != NULL){
		sscanf(buf, "%d %d %d", &pgsprval, &pgspgval, &pgspbval);
	}
	fclose(fp);
	return(TRUE);
}

DllExport int senvendscrnclr()
{
	char	fname[FNAMEMAX];
	FILE	*fp;

	sprintf(fname, "%s\\scrnclr.env", exepath);
	fp = fopen(fname, "w");
	if(fp == NULL){
		return(FALSE);
	}
	fprintf(fp, "%03d %03d %03d\n", bkrval, bkgval, bkbval);
	fprintf(fp, "%03d %03d %03d\n", frrval, frgval, frbval);
	fprintf(fp, "%03d %03d %03d\n", linerval, linegval, linebval);
	fprintf(fp, "%03d %03d %03d\n", curvrval, curvgval, curvbval);
	fprintf(fp, "%03d %03d %03d\n", atnrval, atngval, atnbval);
	fprintf(fp, "%03d %03d %03d\n", slctrval, slctgval, slctbval);
	fprintf(fp, "%03d %03d %03d\n", noslctrval, noslctgval, noslctbval);
	fprintf(fp, "%03d %03d %03d\n", imagerval, imagegval, imagebval);
	fprintf(fp, "%03d %03d %03d\n", lslctrval, lslctgval, lslctbval);
	fprintf(fp, "%03d %03d %03d\n", sublrval, sublgval, sublbval);
	fprintf(fp, "%03d %03d %03d\n", arearval, areagval, areabval);
	fprintf(fp, "%03d %03d %03d\n", gridrval, gridgval, gridbval);
	fprintf(fp, "%03d %03d %03d\n", scalrval, scalgval, scalbval);
	fprintf(fp, "%03d %03d %03d\n", cntrrval, cntrgval, cntrbval);
	fprintf(fp, "%03d %03d %03d\n", asstrval, asstgval, asstbval);
	fprintf(fp, "%03d %03d %03d\n", bezerval, bezegval, bezebval);
	fprintf(fp, "%03d %03d %03d\n", wakurval, wakugval, wakubval);
	fprintf(fp, "%03d %03d %03d\n", pgsprval, pgspgval, pgspbval);
	fclose(fp);
	return(TRUE);
}

DllExport void senvsetbkclr(int rval, int gval, int bval)
{
	bkrval = rval;
	bkgval = gval;
	bkbval = bval;
}

DllExport void senvgetbkclr(int *rval, int *gval, int *bval)
{
	*rval = bkrval;
	*gval = bkgval;
	*bval = bkbval;
}

DllExport void senvsetfrclr(int rval, int gval, int bval)
{
	frrval = rval;
	frgval = gval;
	frbval = bval;
}

DllExport void senvgetfrclr(int *rval, int *gval, int *bval)
{
	*rval = frrval;
	*gval = frgval;
	*bval = frbval;
}

DllExport void senvsetlineclr(int rval, int gval, int bval)
{
	linerval = rval;
	linegval = gval;
	linebval = bval;
}

DllExport void senvgetlineclr(int *rval, int *gval, int *bval)
{
	*rval = linerval;
	*gval = linegval;
	*bval = linebval;
}

DllExport void senvsetcurvclr(int rval, int gval, int bval)
{
	curvrval = rval;
	curvgval = gval;
	curvbval = bval;
}

DllExport void senvgetcurvclr(int *rval, int *gval, int *bval)
{
	*rval = curvrval;
	*gval = curvgval;
	*bval = curvbval;
}

DllExport void senvsetatnclr(int rval, int gval, int bval)
{
	atnrval = rval;
	atngval = gval;
	atnbval = bval;
}

DllExport void senvgetatnclr(int *rval, int *gval, int *bval)
{
	*rval = atnrval;
	*gval = atngval;
	*bval = atnbval;
}

DllExport void senvsetslctclr(int rval, int gval, int bval)
{
	slctrval = rval;
	slctgval = gval;
	slctbval = bval;
}

DllExport void senvgetslctclr(int *rval, int *gval, int *bval)
{
	*rval = slctrval;
	*gval = slctgval;
	*bval = slctbval;
}

DllExport void senvsetnoslctclr(int rval, int gval, int bval)
{
	noslctrval = rval;
	noslctgval = gval;
	noslctbval = bval;
}

DllExport void senvgetnoslctclr(int *rval, int *gval, int *bval)
{
	*rval = noslctrval;
	*gval = noslctgval;
	*bval = noslctbval;
}

DllExport void senvsetimageclr(int rval, int gval, int bval)
{
	imagerval = rval;
	imagegval = gval;
	imagebval = bval;
}

DllExport void senvgetimageclr(int *rval, int *gval, int *bval)
{
	*rval = imagerval;
	*gval = imagegval;
	*bval = imagebval;
}

DllExport void senvsetlslctclr(int rval, int gval, int bval)
{
	lslctrval = rval;
	lslctgval = gval;
	lslctbval = bval;
}

DllExport void senvgetlslctclr(int *rval, int *gval, int *bval)
{
	*rval = lslctrval;
	*gval = lslctgval;
	*bval = lslctbval;
}

DllExport void senvsetsublclr(int rval, int gval, int bval)
{
	sublrval = rval;
	sublgval = gval;
	sublbval = bval;
}

DllExport void senvgetsublclr(int *rval, int *gval, int *bval)
{
	*rval = sublrval;
	*gval = sublgval;
	*bval = sublbval;
}

DllExport void senvsetareaclr(int rval, int gval, int bval)
{
	arearval = rval;
	areagval = gval;
	areabval = bval;
}

DllExport void senvgetareaclr(int *rval, int *gval, int *bval)
{
	*rval = arearval;
	*gval = areagval;
	*bval = areabval;
}

DllExport void senvsetgridclr(int rval, int gval, int bval)
{
	gridrval = rval;
	gridgval = gval;
	gridbval = bval;
}

DllExport void senvgetgridclr(int *rval, int *gval, int *bval)
{
	*rval = gridrval;
	*gval = gridgval;
	*bval = gridbval;
}

DllExport void senvsetscalclr(int rval, int gval, int bval)
{
	scalrval = rval;
	scalgval = gval;
	scalbval = bval;
}

DllExport void senvgetscalclr(int *rval, int *gval, int *bval)
{
	*rval = scalrval;
	*gval = scalgval;
	*bval = scalbval;
}

DllExport void senvsetcntrclr(int rval, int gval, int bval)
{
	cntrrval = rval;
	cntrgval = gval;
	cntrbval = bval;
}

DllExport void senvgetcntrclr(int *rval, int *gval, int *bval)
{
	*rval = cntrrval;
	*gval = cntrgval;
	*bval = cntrbval;
}

DllExport void senvsetasstclr(int rval, int gval, int bval)
{
	asstrval = rval;
	asstgval = gval;
	asstbval = bval;
}

DllExport void senvgetasstclr(int *rval, int *gval, int *bval)
{
	*rval = asstrval;
	*gval = asstgval;
	*bval = asstbval;
}

DllExport void senvsetbezeclr(int rval, int gval, int bval)
{
	bezerval = rval;
	bezegval = gval;
	bezebval = bval;
}

DllExport void senvgetbezeclr(int *rval, int *gval, int *bval)
{
	*rval = bezerval;
	*gval = bezegval;
	*bval = bezebval;
}

DllExport void senvsetwakuclr(int rval, int gval, int bval)
{
	wakurval = rval;
	wakugval = gval;
	wakubval = bval;
}

DllExport void senvgetwakuclr(int *rval, int *gval, int *bval)
{
	*rval = wakurval;
	*gval = wakugval;
	*bval = wakubval;
}

DllExport void senvsetpgspclr(int rval, int gval, int bval)
{
	pgsprval = rval;
	pgspgval = gval;
	pgspbval = bval;
}

DllExport void senvgetpgspclr(int *rval, int *gval, int *bval)
{
	*rval = pgsprval;
	*gval = pgspgval;
	*bval = pgspbval;
}

DllExport int senvgetcrtfillpenno()
{
	return(crtfillpenno);
}

DllExport void senvsetcrtfillpenno(int penno)
{
	crtfillpenno = penno;
}

DllExport int senvgetcrtlinepenno()
{
	return(crtlinepenno);
}

DllExport void senvsetcrtlinepenno(int penno)
{
	crtlinepenno = penno;
}

DllExport DBL senvgetcrtlinethick()
{
	return(dthickmm);
}

DllExport void senvsetcrtlinethick(DBL thick)
{
	dthickmm = thick;
}

DllExport void senvgetcrtpltname(LPSTR pltname)
{
	strcpy(pltname, pltfilename);
}

DllExport void senvsetcrtpltname(LPSTR pltname)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];

	strcpy(pltfilename, pltname);
	pltnomax = 0;

	sprintf(fname, "%s\\%s", exepath, pltfilename);
	fp = fopen(fname, "r+");
	if(fp == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		pltnomax++;
	}
	fclose(fp);
}

DllExport int senvgetpltmax()
{
	return(pltnomax);
}

DllExport int senvgetpenno(int rval, int gval, int bval)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	int		idx;
	char	buf[RECMAX];
	int		dr, dg, db;
	int		pr, pg, pb;

	if(rval == frrval && gval == frgval && bval == frbval){
		return(0);
	}
	sprintf(fname, "%s\\%s", exepath, pltfilename);
	fp = fopen(fname, "r+");
	if(fp == NULL){
		return(0);
	}
	idx = 1;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%d %d %d %d %d %d", &dr, &dg, &db, &pr, &pg, &pb);
		if(rval == dr && gval == dg && bval == db){
			fclose(fp);
			return(idx);
		}
		idx++;
	}
	dr = rval;
	dg = gval;
	db = bval;
	pr = rval;
	pg = gval;
	pb = bval;
	sprintf(buf, "%03d %03d %03d %03d %03d %03d\n", dr, dg, db, pr, pg, pb);
	fputs(buf, fp);
	fclose(fp);
	return(idx);
}

DllExport void senvsetpenrgb(int pno, int rval, int gval, int bval)
{
	char	fname[FNAMEMAX];
	char	bfname[FNAMEMAX];
	FILE	*fp;
	FILE	*bfp;
	int		i, len;
	char	buf[RECMAX];
	int		dr, dg, db;
	int		pr, pg, pb;

	dr = rval;
	dg = gval;
	db = bval;
	pr = rval;
	pg = gval;
	pb = bval;
	sprintf(fname, "%s\\%s", exepath, pltfilename);
	len = strlen(fname);
	strcpy(bfname, fname);
	bfname[len-4] = '\0';
	strcat(bfname, ".bak");

	fp = fopen(fname, "r+");
	if(fp == NULL){
		fp = fopen(fname, "w");
		if(fp == NULL){
			return;
		}
		for(i = 1; i < pno; i++){
			fprintf(fp, "%03d %03d %03d %03d %03d %03d\n", frrval, frgval, frbval, 0, 0, 0);
		}
		fprintf(fp, "%03d %03d %03d %03d %03d %03d\n", dr, dg, db, pr, pg, pb);
		fclose(fp);
		return;
	}
	bfp = fopen(bfname, "w");
	if(bfp == NULL){
		fclose(fp);
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		fputs(buf, bfp);
	}
	fclose(fp);
	fclose(bfp);

	bfp = fopen(bfname, "r");
	fp = fopen(fname, "w");
	i = 1;
	while(1){
		if(fgets(buf, RECMAX, bfp) == NULL){
			break;
		}
		if(i == pno){
			fprintf(fp, "%03d %03d %03d %03d %03d %03d\n", dr, dg, db, pr, pg, pb);
		}
		else{
			fputs(buf, fp);
		}
		i++;
	}
	fclose(fp);
	fclose(bfp);
}

DllExport void senvgetpenrgb(int pno, LPINT rval, LPINT gval, LPINT bval)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	int		i;
	char	buf[RECMAX];
	int		dr, dg, db;
	int		pr, pg, pb;
	int		mode;

	mode = senvcheckdrawmode();
	if(BLACKVIEW == drawmode){
		*rval = 0x00;
		*gval = 0x00;
		*bval = 0x00;
		return;
	}
	if(pno == 0){
		if(mode == PRINTVIEW || mode == DIRCTVIEW || mode == METAFVIEW){
			// âÊñ Ç™çïÇ≈µÃﬁºﬁ™∏ƒÇ™îíÇÃÇ∆Ç´àÛç¸Ç≈èoÇ»Ç¢
			*rval = 0;
			*gval = 0;
			*bval = 0;
		}
		else{
			*rval = frrval;
			*gval = frgval;
			*bval = frbval;
		}
	}

	sprintf(fname, "%s\\%s", exepath, pltfilename);
	fp = fopen(fname, "r+");
	if(fp == NULL){
		return;
	}
	i = 1;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(i == pno){
			sscanf(buf, "%d %d %d %d %d %d", &dr, &dg, &db, &pr, &pg, &pb);
			if(mode == PRINTVIEW || mode == DIRCTVIEW || mode == METAFVIEW){
				*rval = pr;
				*gval = pg;
				*bval = pb;
			}
			else{
				*rval = dr;
				*gval = dg;
				*bval = db;
			}
			break;
		}
		i++;
	}
	fclose(fp);
}

DllExport int senvgetpennoaddname(int rval, int gval, int bval)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	int		idx;
	char	buf[RECMAX];
	int		dr, dg, db;
	int		pr, pg, pb;
	char	name[QRECMAX];

	sprintf(fname, "%s\\%s", exepath, pltfilename);
	fp = fopen(fname, "r+");
	if(fp == NULL){
		return(0);
	}
	idx = 1;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sscanf(buf, "%d %d %d %d %d %d", &dr, &dg, &db, &pr, &pg, &pb);
		if(rval == dr && gval == dg && bval == db){
			fclose(fp);
			return(idx);
		}
		idx++;
	}
	dr = rval;
	dg = gval;
	db = bval;
	pr = rval;
	pg = gval;
	pb = bval;
	strcpy(name, "add");
	sprintf(buf, "%03d %03d %03d %03d %03d %03d %s\n", dr, dg, db, pr, pg, pb, name);
	fputs(buf, fp);
	fclose(fp);
	return(idx);
}

DllExport void senvsetpenrgbaddname(int pno, int rval, int gval, int bval)
{
	char	fname[FNAMEMAX];
	char	bfname[FNAMEMAX];
	FILE	*fp;
	FILE	*bfp;
	int		i, len;
	char	buf[RECMAX];
	int		tdr, tdg, tdb;
	int		tpr, tpg, tpb;
	int		dr, dg, db;
	int		pr, pg, pb;
	char	name[QRECMAX];

	dr = rval;
	dg = gval;
	db = bval;
	pr = rval;
	pg = gval;
	pb = bval;
	sprintf(fname, "%s\\%s", exepath, pltfilename);
	len = strlen(fname);
	strcpy(bfname, fname);
	bfname[len-4] = '\0';
	strcat(bfname, ".bak");

	fp = fopen(fname, "r+");
	if(fp == NULL){
		fp = fopen(fname, "w");
		if(fp == NULL){
			return;
		}
		for(i = 1; i < pno; i++){
			fprintf(fp, "000 000 000 000 000 000 non\n");
		}
		fprintf(fp, "%03d %03d %03d %03d %03d %03d add\n", dr, dg, db, pr, pg, pb);
		fclose(fp);
		return;
	}
	bfp = fopen(bfname, "w");
	if(bfp == NULL){
		fclose(fp);
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		fputs(buf, bfp);
	}
	fclose(fp);
	fclose(bfp);

	bfp = fopen(bfname, "r");
	fp = fopen(fname, "w");
	i = 1;
	while(1){
		if(fgets(buf, RECMAX, bfp) == NULL){
			break;
		}
		if(i == pno){
			strcpy(name, "");
			sscanf(buf, "%d %d %d %d %d %d %s", &tdr, &tdg, &tdb, &tpr, &tpg, &tpb, name);
			fprintf(fp, "%03d %03d %03d %03d %03d %03d %s\n", dr, dg, db, pr, pg, pb, name);
		}
		else{
			fputs(buf, fp);
		}
		i++;
	}
	fclose(fp);
	fclose(bfp);
}

DllExport void senvgetpenrgbaddname(int pno, LPINT rval, LPINT gval, LPINT bval, LPSTR name)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	int		i;
	char	buf[RECMAX];
	int		dr, dg, db;
	int		pr, pg, pb;
	int		mode;

	mode = senvcheckdrawmode();
	if(BLACKVIEW == drawmode){
		*rval = 0x00;
		*gval = 0x00;
		*bval = 0x00;
		strcpy(name, "");
		return;
	}

	if(bkrval == 0 && bkgval == 0 && bkbval == 0
	&& (mode == PRINTVIEW || mode == DIRCTVIEW || mode == METAFVIEW)){
		// àÛç¸Ç≈ÇOî‘ÉyÉìÇÃµÃﬁºﬁ™∏ƒÇ™îíÇÃÇ∆Ç´
		*rval = 0;
		*gval = 0;
		*bval = 0;
	}
	else{
		*rval = frrval;
		*gval = frgval;
		*bval = frbval;
	}
	strcpy(name, "");
	if(pno == 0){
		return;	
	}

	sprintf(fname, "%s\\%s", exepath, pltfilename);
	fp = fopen(fname, "r+");
	if(fp == NULL){
		return;
	}
	i = 1;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(i == pno){
			sscanf(buf, "%d %d %d %d %d %d %s", &dr, &dg, &db, &pr, &pg, &pb, name);
			if(mode == PRINTVIEW || mode == DIRCTVIEW || mode == METAFVIEW){
				*rval = pr;
				*gval = pg;
				*bval = pb;
			}
			else{
				*rval = dr;
				*gval = dg;
				*bval = db;
			}
			break;
		}
		i++;
	}
	fclose(fp);
}

DllExport void senvgetprnrgbaddname(int pno, LPINT rval, LPINT gval, LPINT bval, LPSTR name)
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	int		i;
	char	buf[RECMAX];
	int		dr, dg, db;
	int		pr, pg, pb;
	int		mode;

	mode = senvcheckdrawmode();
	if(frrval == 255 && frgval == 255 && frbval == 255
	&& (mode == PRINTVIEW || mode == DIRCTVIEW || mode == METAFVIEW)){
		// àÛç¸Ç≈ÇOî‘ÉyÉìÇÃµÃﬁºﬁ™∏ƒÇ™îíÇÃÇ∆Ç´
		*rval = 0;
		*gval = 0;
		*bval = 0;
	}
	else{
		*rval = frrval;
		*gval = frgval;
		*bval = frbval;
	}
	strcpy(name, "");
	if(pno == 0){
		return;	
	}

	sprintf(fname, "%s\\%s", exepath, pltfilename);
	fp = fopen(fname, "r+");
	if(fp == NULL){
		return;
	}
	i = 1;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(i == pno){
			sscanf(buf, "%d %d %d %d %d %d %s", &dr, &dg, &db, &pr, &pg, &pb, name);
			*rval = pr;
			*gval = pg;
			*bval = pb;
			break;
		}
		i++;
	}
	fclose(fp);
}

DllExport void scrnsetclrbrushandpen(HDC PaintDC, COLORREF fillclr, COLORREF lineclr, int nthick)
{
	hLinePen = NULL;
	hBezePen = NULL;
	hMainPen = CreatePen(PS_SOLID, nthick, lineclr);
	hSubPen = NULL;
	hConrPen = CreatePen(PS_SOLID, 1, lineclr);
	hMainBrush = CreateSolidBrush(fillclr);
	hConrBrush = CreateSolidBrush(lineclr);
	nPlgnThick = nthick;
	SetBkMode(PaintDC, TRANSPARENT);
	oldClr = SetTextColor(PaintDC, fillclr);
	oldhPen = (HPEN)SelectObject(PaintDC, hMainPen);
	oldhBrush = (HBRUSH)SelectObject(PaintDC, hMainBrush);
}

DllExport void scrnsetpreviewpen(HDC PaintDC, int r, int g, int b, int nthick)
{
	hLinePen = NULL;
	hBezePen = NULL;
	if(R2_XORPEN == GetROP2(PaintDC)){
		r ^= bkrval;
		g ^= bkgval;
		b ^= bkbval;
	}
	hMainPen = CreatePen(PS_SOLID, nthick, RGB(r, g, b));
	hSubPen = NULL;
	hConrPen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
	hMainBrush = CreateSolidBrush(RGB(r, g, b));
	hConrBrush = CreateSolidBrush(RGB(r, g, b));
	nPlgnThick = nthick;
	SetBkMode(PaintDC, TRANSPARENT);
	oldClr = SetTextColor(PaintDC, RGB(r, g, b));
	oldhPen = (HPEN)SelectObject(PaintDC, hMainPen);
	oldhBrush = (HBRUSH)SelectObject(PaintDC, hMainBrush);
}

DllExport void scrnsetwakupen(HDC PaintDC)
{
	int		r, g, b;

	if(R2_XORPEN == GetROP2(PaintDC)){
		r = wakurval ^ bkrval;
		g = wakugval ^ bkgval;
		b = wakubval ^ bkbval;
	}
	else{
		r = wakurval;
		g = wakugval;
		b = wakubval;
	}
	//hMainBrush = CreateSolidBrush(RGB(r, g, b));
	hMainBrush = NULL;
	hConrBrush = NULL;
	hLinePen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
	hBezePen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
	hMainPen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
	hSubPen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
	hConrPen = NULL;
	SetBkMode(PaintDC, TRANSPARENT);
	oldClr = SetTextColor(PaintDC, RGB(r, g, b));
	oldhPen = (HPEN)SelectObject(PaintDC, hMainPen);
	//oldhBrush = (HBRUSH)SelectObject(PaintDC, hMainBrush);
}

DllExport void scrnsetnoslctpen(HDC PaintDC)
{
	int		r, g, b;

	if(R2_XORPEN == GetROP2(PaintDC)){
		r = noslctrval ^ bkrval;
		g = noslctgval ^ bkgval;
		b = noslctbval ^ bkbval;
	}
	else{
		r = noslctrval;
		g = noslctgval;
		b = noslctbval;
	}
	//hMainBrush = CreateSolidBrush(RGB(r, g, b));
	hMainBrush = NULL;
	hConrBrush = NULL;
	hLinePen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
	hBezePen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
	hMainPen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
	hSubPen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
	hConrPen = NULL;
	SetBkMode(PaintDC, TRANSPARENT);
	oldClr = SetTextColor(PaintDC, RGB(r, g, b));
	oldhPen = (HPEN)SelectObject(PaintDC, hMainPen);
	//oldhBrush = (HBRUSH)SelectObject(PaintDC, hMainBrush);
}

DllExport void scrnsetfigupen(HDC PaintDC)
{
	hMainBrush = NULL;
	hConrBrush = NULL;
	if(R2_XORPEN == GetROP2(PaintDC)){
		hLinePen = CreatePen(PS_SOLID, 1, RGB(linerval^bkrval, linegval^bkgval, linebval^bkbval));;
		hBezePen = CreatePen(PS_SOLID, 1, RGB(bezerval^bkrval, bezegval^bkgval, bezebval^bkbval));
		hMainPen = CreatePen(PS_SOLID, 1, RGB(curvrval^bkrval, curvgval^bkgval, curvbval^bkbval));
		hSubPen = CreatePen(PS_SOLID, 1, RGB(sublrval^bkrval, sublgval^bkgval, sublbval^bkbval));
	}
	else{
		hLinePen = CreatePen(PS_SOLID, 1, RGB(linerval, linegval, linebval));;
		hBezePen = CreatePen(PS_SOLID, 1, RGB(bezerval, bezegval, bezebval));
		hMainPen = CreatePen(PS_SOLID, 1, RGB(curvrval, curvgval, curvbval));
		hSubPen = CreatePen(PS_SOLID, 1, RGB(sublrval, sublgval, sublbval));
	}
	hConrPen = NULL;
	SetBkMode(PaintDC, TRANSPARENT);
	if(R2_XORPEN == GetROP2(PaintDC)){
		oldClr = SetTextColor(PaintDC, RGB(linerval^bkrval, linegval^bkgval, linebval^bkbval));
	}
	else{
		oldClr = SetTextColor(PaintDC, RGB(linerval, linegval, linebval));
	}
	oldhPen = (HPEN)SelectObject(PaintDC, hMainPen);
}

DllExport void scrnsetpointpen(HDC PaintDC)
{
	int		r, g, b;

	if(R2_XORPEN == GetROP2(PaintDC)){
		r = slctrval ^ bkrval;
		g = slctgval ^ bkgval;
		b = slctbval ^ bkbval;
	}
	else{
		r = slctrval;
		g = slctgval;
		b = slctbval;
	}
	hMainBrush = NULL;
	hConrBrush = NULL;
	hLinePen = CreatePen(PS_SOLID, 1, RGB(r, g, b));;
	hBezePen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
	hMainPen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
	hSubPen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
	hConrPen = NULL;
	SetBkMode(PaintDC, TRANSPARENT);
	oldClr = SetTextColor(PaintDC, RGB(r, g, b));
	oldhPen = (HPEN)SelectObject(PaintDC, hMainPen);
}

DllExport void scrnsetdelpen(HDC PaintDC)
{
	int	width;

	width = 3;
	//width = 1;
	//hMainBrush = CreateSolidBrush(RGB(bkrval, bkgval, bkbval));
	hMainBrush = NULL;
	hConrBrush = NULL;
	if(senvcheckdrawmode() == POINTVIEW){
		hLinePen = CreatePen(PS_SOLID, width, RGB(bkrval, bkgval, bkbval));
		hBezePen = CreatePen(PS_SOLID, width, RGB(bkrval, bkgval, bkbval));
		hSubPen = CreatePen(PS_SOLID, width, RGB(bkrval, bkgval, bkbval));
	}
	else{
		hLinePen = NULL;
		hBezePen = NULL;
		hSubPen = NULL;
	}
	hMainPen = CreatePen(PS_SOLID, width, RGB(bkrval, bkgval, bkbval));
	hConrPen = NULL;
	oldhPen = (HPEN)SelectObject(PaintDC, hMainPen);
	SetBkMode(PaintDC, TRANSPARENT);
	oldClr = SetTextColor(PaintDC, RGB(bkrval, bkgval, bkbval));
	//oldhBrush = (HBRUSH)SelectObject(PaintDC, hMainBrush);
}

DllExport void scrnsetfrpen(HDC PaintDC)
{
	hMainBrush = NULL;
	hConrBrush = NULL;
	hLinePen = NULL;
	hBezePen = NULL;
	hMainPen = CreatePen(PS_SOLID, 1, RGB(frrval, frgval, frbval));
	hSubPen = NULL;
	hConrPen = NULL;
	oldhPen = (HPEN)SelectObject(PaintDC, hMainPen);
	SetBkMode(PaintDC, TRANSPARENT);
	oldClr = SetTextColor(PaintDC, RGB(frrval, frgval, frbval));
}

DllExport void scrnsetbkpen(HDC PaintDC)
{
	hMainBrush = NULL;
	hConrBrush = NULL;
	hLinePen = NULL;
	hBezePen = NULL;
	hMainPen = CreatePen(PS_SOLID, 1, RGB(bkrval, bkgval, bkbval));
	hSubPen = NULL;
	hConrPen = NULL;
	oldhPen = (HPEN)SelectObject(PaintDC, hMainPen);
	SetBkMode(PaintDC, TRANSPARENT);
	oldClr = SetTextColor(PaintDC, RGB(bkrval, bkgval, bkbval));
}

DllExport int scrnbrushchek()
{
	if(hMainBrush == NULL){
		return(OFF);
	}
	else{
		return(ON);
	}
}

DllExport void scrnsetslctpen(HDC PaintDC)
{
	int		r, g, b;

	if(drawmode == POINTVIEW){
		if(R2_XORPEN == GetROP2(PaintDC)){
			r = lslctrval ^ bkrval;
			g = lslctgval ^ bkgval;
			b = lslctbval ^ bkbval;
		}
		else{
			r = lslctrval;
			g = lslctgval;
			b = lslctbval;
		}
		hMainBrush = NULL;
		hConrBrush = NULL;
		hLinePen = CreatePen(PS_SOLID, 3, RGB(r, g, b));
		hBezePen = CreatePen(PS_SOLID, 3, RGB(r, g, b));
		hSubPen = CreatePen(PS_SOLID, 3, RGB(r, g, b));
		hMainPen = CreatePen(PS_SOLID, 3, RGB(r, g, b));
		hConrPen = NULL;
		oldhPen = (HPEN)SelectObject(PaintDC, hMainPen);
		SetBkMode(PaintDC, TRANSPARENT);
		oldClr = SetTextColor(PaintDC, RGB(r, g, b));
	}
	else{
		if(R2_XORPEN == GetROP2(PaintDC)){
			r = slctrval ^ bkrval;
			g = slctgval ^ bkgval;
			b = slctbval ^ bkbval;
		}
		else{
			r = slctrval;
			g = slctgval;
			b = slctbval;
		}
		hMainBrush = CreateSolidBrush(RGB(r, g, b));
		hConrBrush = NULL;
		hLinePen = NULL;
		hBezePen = NULL;
		hSubPen = NULL;
		hMainPen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
		hConrPen = NULL;
		oldhBrush = (HBRUSH)SelectObject(PaintDC, hMainBrush);
		oldhPen = (HPEN)SelectObject(PaintDC, hMainPen);
		SetBkMode(PaintDC, TRANSPARENT);
		oldClr = SetTextColor(PaintDC, RGB(r, g, b));
	}
}

DllExport void scrnsetslctdotpen(HDC PaintDC)
{
	int		r, g, b;

	if(R2_XORPEN == GetROP2(PaintDC)){
		r = slctrval ^ bkrval;
		g = slctgval ^ bkgval;
		b = slctbval ^ bkbval;
	}
	else{
		r = slctrval;
		g = slctgval;
		b = slctbval;
	}
	hMainBrush = NULL;
	hConrBrush = NULL;
	hLinePen = NULL;
	hBezePen = NULL;
	hMainPen = CreatePen(PS_DOT, 1, RGB(r, g, b));
	hSubPen = NULL;
	hConrPen = NULL;
	oldhPen = (HPEN)SelectObject(PaintDC, hMainPen);
	SetBkMode(PaintDC, TRANSPARENT);
	oldClr = SetTextColor(PaintDC, RGB(r, g, b));
}

DllExport void scrnsetatnpen(HDC PaintDC)
{
	int		r, g, b;

	if(R2_XORPEN == GetROP2(PaintDC)){
		r = atnrval ^ bkrval;
		g = atngval ^ bkgval;
		b = atnbval ^ bkbval;
	}
	else{
		r = atnrval;
		g = atngval;
		b = atnbval;
	}
	hMainBrush = NULL;
	hConrBrush = NULL;
	hLinePen = NULL;
	hBezePen = NULL;
	hMainPen = CreatePen(PS_SOLID, 3, RGB(r, g, b));
	hSubPen = NULL;
	hConrPen = NULL;
	oldhPen = (HPEN)SelectObject(PaintDC, hMainPen);
	SetBkMode(PaintDC, TRANSPARENT);
	oldClr = SetTextColor(PaintDC, RGB(r, g, b));
}

DllExport void scrnpeninit(HDC PaintDC, long lClr, long nthick)
{
	hLinePen = NULL;
	hBezePen = NULL;
	hMainPen = CreatePen(PS_SOLID, nthick, lClr);
	hSubPen = NULL;
	hConrPen = CreatePen(PS_SOLID, 1, lClr);
	hMainBrush = CreateSolidBrush(lClr);
	hConrBrush = CreateSolidBrush(lClr);
	nPlgnThick = nthick;
	oldhPen = (HPEN)SelectObject(PaintDC, hMainPen);
	oldhBrush = (HBRUSH)SelectObject(PaintDC, hMainBrush);
	SetBkMode(PaintDC, TRANSPARENT);
	oldClr = SetTextColor(PaintDC, lClr);
}

DllExport void scrnpenend(HDC PaintDC)
{
	SetTextColor(PaintDC, oldClr);
	if(hMainBrush != NULL){
		SelectObject(PaintDC, oldhBrush);
		DeleteObject(hMainBrush);
	}
	if(hConrBrush != NULL){
		DeleteObject(hConrBrush);
	}
	SelectObject(PaintDC, oldhPen);
	if(hSubPen != NULL){
		DeleteObject(hSubPen);
	}
	if(hMainPen != NULL){
		DeleteObject(hMainPen);
	}
	if(hLinePen != NULL){
		DeleteObject(hLinePen);
	}
	if(hBezePen != NULL){
		DeleteObject(hBezePen);
	}
	if(hConrPen != NULL){
		DeleteObject(hConrPen);
	}
	hConrPen = NULL;
	hLinePen = NULL;
	hBezePen = NULL;
	hMainPen = NULL;
	hSubPen = NULL;
	hMainBrush = NULL;
	hConrBrush = NULL;
	nPlgnThick = 0;
}

DllExport HPEN senvgetmainpen()
{
	return(hMainPen);
}

DllExport HPEN senvgetsubpen()
{
	return(hSubPen);
}

DllExport HPEN senvgetlinepen()
{
	return(hLinePen);
}

DllExport HPEN senvgetbezepen()
{
	return(hBezePen);
}

DllExport HPEN senvgetconrpen()
{
	return(hConrPen);
}

DllExport HBRUSH senvgetmainbrush()
{
	return(hMainBrush);
}

DllExport HBRUSH senvgetconrbrush(int *nthick)
{
	*nthick = nPlgnThick;
	return(hConrBrush);
}

DllExport int senvgetplgnthick()
{
	return(nPlgnThick);
}

DllExport void senvsetexepath(LPSTR path)
{
	char	defpath[FNAMEMAX+2];

	strcpy(exepath, path);
	wsprintf(defpath, "%s\\defenv", path);
	if(_access(defpath, 0) == -1){
		CreateDirectory(defpath, NULL);
	}
}

DllExport void senvgetexepath(LPSTR path)
{
	strcpy(path, exepath);
}

DllExport void senvgetwakpath(LPSTR path)
{
	strcpy(path, wakpath);
}

DllExport void senvsetwakpath(LPSTR path)
{
	strcpy(wakpath, path);
}

DllExport void senvgetkmnpath(LPSTR path)
{
	strcpy(path, kmnpath);
}

DllExport void senvsetkmnpath(LPSTR path)
{
	strcpy(kmnpath, path);
}

DllExport void senvgetdatpath(LPSTR path)
{
	strcpy(path, datpath);
}

DllExport void senvsetdatpath(LPSTR path)
{
	strcpy(datpath, path);
}

DllExport void senvgetprtpath(LPSTR path)
{
	strcpy(path, prtpath);
}

DllExport void senvsetprtpath(LPSTR path)
{
	strcpy(prtpath, path);
}

DllExport void senvgetoutpath(LPSTR path)
{
	strcpy(path, outpath);
}

DllExport void senvsetoutpath(LPSTR path)
{
	strcpy(outpath, path);
}

DllExport void senvgetpartpath(LPSTR path)
{
	strcpy(path, partpath);
}

DllExport void senvsetpartpath(LPSTR path)
{
	strcpy(partpath, path);
}

DllExport void senvgettmppath(LPSTR path)
{
	strcpy(path, tmppath);
}

DllExport void senvsettmppath(LPSTR path)
{
	strcpy(tmppath, path);
}

DllExport void senvgetbtppath(LPSTR path)
{
	strcpy(path, btppath);
}

DllExport void senvsetbtppath(LPSTR path)
{
	strcpy(btppath, path);
}

DllExport int senvcheckdrawmode()
{
	return(drawmode);
}

DllExport int senvsetdrawmode(int mode)
{
	int		retdrawmode;

	retdrawmode = drawmode;
	drawmode = mode;
	return(retdrawmode);
}

DllExport int senvcheckdrawgrid()
{
	return(drawgrid);
}

DllExport void senvondrawgrid()
{
	drawgrid = ON;
}

DllExport void senvoffdrawgrid()
{
	drawgrid = OFF;
}

DllExport int senvcheckdrawimage()
{
	return(drawimage);
}

DllExport void senvondrawimage()
{
	drawimage = ON;
}

DllExport void senvoffdrawimage()
{
	drawimage = OFF;
}

DllExport int senvcheckdrawpoint()
{
	return(drawpoint);
}

DllExport void senvondrawpoint()
{
	drawpoint = ON;
}

DllExport void senvoffdrawpoint()
{
	drawpoint = OFF;
}

DllExport int senvcheckdrawbeze()
{
	return(drawbeze);
}

DllExport void senvondrawbeze()
{
	drawbeze = ON;
}

DllExport void senvoffdrawbeze()
{
	drawbeze = OFF;
}

DllExport int senvcheckconrmode()
{
	return(nConrFlag);
}

DllExport void senvonconrmode()
{
	nConrFlag = ON;
}

DllExport void senvoffconrmode()
{
	nConrFlag = OFF;
}

