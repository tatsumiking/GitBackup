//
// Easy Scanner System Ver1.0
//
// ess  pout.c	プロッタ出力関数
//
//  Copyright (c) 1998 by THEON

#include	"stdafx.h"

#include	"Ploter.h"
#include	"Baselib.h"
#include	"Curvelib.h"
#include	"Senvlib.h"
#include	"Pdrwlib.h"
#include	"Plot.h"
#include	"Disp.h"
#include	"Pout.h"

#define PAINT	0
#define PLOT	1

#define	LIPSYMOVE	3.0
#define RPDLYMOVE	6.0

#define BEZNESTDEEP 20
//　poutdivmode 0 
#define BEZDIVMMLIMIT	0.05
#define	ARCMMLIMIT 0.1
#define ARCRAGLIMIT	1.0
//  poutdivmode 1
//#define BEZDIVMMLIMIT	0.5
//#define	ARCMMLIMIT 0.5
#define BEZRAGLIMIT	5.0
#define BEZMINDIV	3
#define BEZMAXDIV	20
//#define ARCRAGLIMIT	5.0
#define ARCMINDIV	3
#define ARCMAXDIV	20

#define	OVERLAPSETMM 1.0

extern DBL	pai, degree1, degree01, degree5;
extern int	senvpoutdispmode;

TCHAR	dbgbuf[RECMAX];

DBL		OverlapMM;
DBL		OverlapStep;
DBL		OverlapRest;
int		OverlapFlag;

DBL		rpdladdx, rpdladdy;
int		rpdlmovecntr;

DBL		poutaddx, poutaddy;

int		bezmindiv, bezmaxdiv;
DBL		bezdivstep;
DBL		bezraglimit;
int		arcmindiv, arcmaxdiv;
DBL		arclimitstep;
DBL		arcraglimit;
int		poutdivmode;

TCHAR	sportname[RECMAX];
TCHAR	splotername[RECMAX];
DBL		scrnsxmm, scrnsymm, scrnexmm, scrneymm;
DBL		poutsx, poutsy, poutex, poutey;
DBL		plotxmmsize, plotymmsize, poutstep;
int		pout3parcflag;
int		poutfillflag;
int		pouthomemode, poutlinemode, poutomitmode;
int		plotcmdtype, plotcntrkind;
int		poutmode;
int		poutfirstflag;

int		poutroundkind;		// 0 1 -1;

int	plotdeepcutflag = OFF;
DBL	plotdeepcutmm;
DBL	plotdeepcutstep;

DllExport void poutsetaddxy(DBL addx, DBL addy)
{
	poutaddx = addx;
	poutaddy = addy;
}

DllExport void poutsetoverlapflag(int flag)
{
	OverlapFlag = flag;
}

DllExport int poutcheckoverlapend()
{
	if(OverlapFlag == ON
	&& OverlapRest <= 0 ){
		return(ON);
	}
	return(OFF);
}

DllExport void poutsetdeepcutflag(int flag)
{
	plotdeepcutflag = flag;
}

DllExport int poutcheckdeepcutflag()
{
	return(plotdeepcutflag);
}

DllExport void poutsetdeepcut(DBL mm)
{
	plotdeepcutmm = mm;
}

DllExport void poutgetdeepcut(LPDBL mm)
{
	*mm = plotdeepcutmm;
}

DllExport void poutgetparaminit(LPDBL lmtstep, LPDBL limitr, LPDBL smm, LPDBL emm)
{
	plotgetparaminit(lmtstep, limitr, smm, emm);
}

DllExport void poutsetparaminit(DBL lmtstep, DBL limitr, DBL smm, DBL emm)
{
	plotsetparaminit(lmtstep, limitr, smm, emm);
}

DllExport void poutsmmemmset(DBL smm, DBL emm)
{
	plotsmmlegalset(smm);
	plotemmlegalset(emm);
}

DllExport void poutrpdladdset(DBL xadd, DBL yadd, int cntrflag)
{
	rpdladdx = xadd;
	rpdladdy = yadd;
	rpdlmovecntr = cntrflag;
}

DllExport void poutportnameset(LPTSTR port)
{
	_tcscpy_s(sportname, port);
}

DllExport void poutportnameget(LPTSTR port)
{
	int len = _tcslen(sportname) + 1;
	_tcscpy_s(port, len, sportname);
}

DllExport void poutploternameset(LPTSTR pltname)
{
	_tcscpy_s(splotername, pltname);
}

DllExport void poutploternameget(LPTSTR pltname)
{
	int len = _tcslen(splotername) + 1;
	_tcscpy_s(pltname, len, splotername);
}

DllExport void poutfillflagset(int flag)
{
	poutfillflag = flag;
}

DllExport void poutfillflagget(int *flag)
{
	*flag = poutfillflag;
}

DllExport void poutfillflagon()
{
	poutfillflag = ON;
}

DllExport void poutfillflagoff()
{
	poutfillflag = OFF;
}

DllExport int poutfillflagchk()
{
	return(poutfillflag);
}

DllExport void poutmodeget(int *homemode, int *linemode, int *omitmode)
{
	*homemode = pouthomemode;
	*linemode = poutlinemode;
	*omitmode = poutomitmode;
}

DllExport void poutmodeset(int homemode, int linemode, int omitmode)
{
	pouthomemode = homemode;
	poutlinemode = linemode;
	poutomitmode = omitmode;
}

DllExport int pouthomemodecheck()
{
	return(pouthomemode);
}

DllExport int poutlinemodecheck()
{
	return(poutlinemode);
}

//int TScrnEnv::PloterOmitModeCheck()
DllExport int poutomitmodecheck()
{
	return(poutomitmode);
}
DllExport void poutchkmove(DBL dx, DBL dy)
{
	plotmove(dx, dy);
	switch(plotcmdtype){
	case CJHP:
	case CJ2HP:
		plotdrawline(dx, dy, dx+1, dy+1);
		break;
	}
	plotmove(dx+1, dy+1);
	switch(plotcmdtype){
	case CJHP:
	case CJ2HP:
		plotdrawst();
		break;
	}
}

int poutareaget(DBL *sx, DBL *sy, DBL *ex, DBL *ey)
{
	*sx = poutsx;
	*sy = poutsy;
	*ex = poutex;
	*ey = poutey;
	return(0);
}

DllExport int poutareachk(DBL sx, DBL sy, DBL ex, DBL ey)
{
	DBL	tt;
	int	flagx1, flagy1, flagx2, flagy2;

	if(sx > ex){
		tt = sx;
		sx = ex;
		ex = tt;
	}
	if(sy > ey){
		tt = sy;
		sy = ey;
		ey = tt;
	}
	flagx1 = poutareasignget(sx, poutsx, poutex);
	flagy1 = poutareasignget(sy, poutsy, poutey);
	flagx2 = poutareasignget(ex, poutsx, poutex);
	flagy2 = poutareasignget(ey, poutsy, poutey);
	if(flagx1 == 0 && flagy1 == 0 && flagx2 == 0 && flagy2 == 0){
		return(INAREA);
	}
	if((flagx1 * flagx2) > 0 || (flagy1 * flagy2) > 0){
		return(OFF);
	}
	return(CROSS);
}

DllExport void pouttypeset(int type)
{
	plotcmdtype = type;
	plotkindset(type);
}

DllExport void pouttypeget(int *type)
{
	*type = plotcmdtype;
}

DllExport void poutoutareaset(DBL sx, DBL sy, DBL ex, DBL ey)
{
	if(sx < ex){
		scrnsxmm = sx;
		scrnexmm = ex;
	}
	else{
		scrnsxmm = ex;
		scrnexmm = sx;
	}

	if(sy < ey){
		scrnsymm = sy;
		scrneymm = ey;
	}
	else{
		scrnsymm = ey;
		scrneymm = sy;
	}
	plotmmareaset(scrnsxmm, scrnsymm, scrnexmm, scrneymm);
}

DllExport void poutmmsizeset(DBL xsize, DBL ysize)
{
	plotxmmsize = xsize;
	plotymmsize = ysize;
	plotmmsizeset(xsize, ysize);
	//plotmmareaset(scrnsxmm, scrnsymm, scrnexmm, scrneymm);
}

DllExport void poutmmsizeget(DBL *xsize, DBL *ysize)
{
	*xsize = plotxmmsize;
	*ysize = plotymmsize;
}

DllExport void poutstepset(DBL step)
{
	poutstep = step;
	plotstepset(step);
}

DllExport void poutstepget(DBL *step)
{
	*step = poutstep;
}

DllExport void poutmmstartset(DBL sx, DBL sy)
{
	plotmmstartset(sx, sy);
}

DllExport void poutmmstartget(DBL *sx, DBL *sy)
{
	plotmmstartget(sx, sy);
}

DllExport void poutmmcntrset(DBL sx, DBL sy)
{
	plotmmcntrset(sx, sy);
}

DllExport void poutarcflagset(int flag)
{
	pout3parcflag = flag;
}

DllExport void poutarcflagget(int *flag)
{
	*flag = pout3parcflag;
}

DllExport void poutlinemodeset(int flag)
{
	poutlinemode = flag;
}

DllExport void poutlinemodeget(int *flag)
{
	*flag = poutlinemode;
}

DllExport void poutcntrkindset(int cntrkind)
{
	plotcntrkind = cntrkind;
	plotcntrkindset(cntrkind);
}

DllExport void poutcntrkindget(int *cntrkind)
{
	*cntrkind = plotcntrkind;
}

DllExport void poutelementinit()
{
	DBL		xtime, ytime;
	DBL		xmove, ymove;
	DBL		tt;

	OverlapFlag = OFF;
	OverlapMM = OVERLAPSETMM;
	OverlapStep = OverlapMM / poutstep;

	plotdeepcutstep = plotdeepcutmm / poutstep;

	pouttimeinit(&xtime, &ytime);
	plottimeset(xtime, ytime);
	poutmoveinit(&xmove, &ymove);
	plotmovexyset(xmove, ymove);

	poutprmload();

	poutsx = scrnsxmm;
	poutsy = scrnsymm;
	poutex = scrnexmm;
	poutey = scrneymm;
	plotdxymmtostep(&poutsx, &poutsy);
	plotdxymmtostep(&poutex, &poutey);
	//plotabsxyget(&poutsx, &poutsy);
	//plotabsxyget(&poutex, &poutey);
	if(poutsx > poutex){
		tt = poutsx;
		poutsx = poutex;
		poutex = tt;
	}
	if(poutsy > poutey){
		tt = poutsy;
		poutsy = poutey;
		poutey = tt;
	}
}

DllExport void poutstepoutareaset(DBL sx, DBL sy, DBL ex, DBL ey)
{
	poutsx = sx;
	poutsy = sy;
	poutex = ex;
	poutey = ey;
}

void poutprmload()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	DBL		rag, mm, mindiv, maxdiv;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\poutprm.env"), exepath);
	_tfopen_s(&fp, (LPTSTR)fname, _T("r"));
	if(fp == NULL){
		poutdivmode = 0;
		arcraglimit = ARCRAGLIMIT * degree1;
		arclimitstep = ARCMMLIMIT / poutstep;
		arcmindiv = ARCMINDIV;
		arcmaxdiv = ARCMAXDIV;

		bezraglimit = BEZRAGLIMIT * degree1;
		bezdivstep = BEZDIVMMLIMIT / poutstep;
		bezmindiv = BEZMINDIV;
		bezmaxdiv = BEZMAXDIV;
		return;
	}

	baselibCrDelFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%d"), &poutdivmode);

	baselibCrDelFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%lf %lf %lf %lf"), &rag, &mm, &mindiv, &maxdiv);
	arcraglimit = rag * degree1;
	arclimitstep = mm / poutstep;
	arcmindiv = (int)mindiv;
	arcmaxdiv = (int)maxdiv;

	baselibCrDelFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%lf %lf %lf %lf"), &rag, &mm, &mindiv, &maxdiv);
	bezraglimit = rag * degree1;
	bezdivstep = mm / poutstep;
	bezmindiv = (int)mindiv;
	bezmaxdiv = (int)maxdiv;

	fclose(fp);
}

void pouttimeinit(DBL *retxtime, DBL *retytime)
{
	DBL		xtime, ytime;
	DBL		xsize, ysize;

	xtime = 1.0;
	ytime = 1.0;
	xsize = scrnexmm - scrnsxmm;
	ysize = scrneymm - scrnsymm;
	switch(plotcmdtype){
	case RP93T:
	case RP9T:
	case RP6T:
		switch(plotcntrkind){
		case LEFTDOWN:
		case RIGHTUP:
			xtime = (int)(xsize / plotxmmsize);
			if(xtime * plotxmmsize < xsize){
				xtime += 1.0;
			}
			break;
		case LEFTUP:
		case RIGHTDOWN:
			xtime = (int)(ysize / plotxmmsize);
			if(xtime * plotxmmsize < ysize){
				xtime += 1.0;
			}
			break;
		}
		break;
	}
	*retxtime = xtime;
	*retytime = ytime;
}

void poutmoveinit(DBL *retxmove, DBL *retymove)
{
	DBL		xmove, ymove;
	DBL		xsize, ysize;

	xmove = 0.0;
	ymove = 0.0;
	xsize = scrnexmm - scrnsxmm;
	ysize = scrneymm - scrnsymm;
	switch(plotcmdtype){
	case RP1T:
	case RP395T:
	case CH37:
	case RP3L:
	case RP3LL:
	case RP35F:
	case LIPS:
		switch(plotcntrkind){
		case LEFTDOWN:
		case RIGHTUP:
			ymove = (plotymmsize - ysize) / 2;
			break;
		case LEFTUP:
		case RIGHTDOWN:
			ymove = (plotymmsize - xsize) / 2;
			break;
		}
		break;
	case RPDL:
		if(rpdlmovecntr == 1){
			switch(plotcntrkind){
			case LEFTDOWN:
			case RIGHTUP:
				ymove = (plotymmsize - ysize) / 2;
				break;
			case LEFTUP:
			case RIGHTDOWN:
				ymove = (plotymmsize - xsize) / 2;
				break;
			}
		}
		break;
	}
	if(plotcmdtype == LIPS){
		ymove += LIPSYMOVE;
	}
	if(plotcmdtype == RPDL || plotcmdtype == CH37){
		xmove += rpdladdx;
		ymove += rpdladdy;
	}
	xmove += poutaddx;
	ymove += poutaddy;

	*retxmove = xmove;
	*retymove = ymove;
}

DllExport void poutbegin(int nComID)
{
	poutroundkind = 0;
	plotcomidset(nComID);
	plotoutlength(poutex, poutey);
	plotdrawinit();
}

DllExport void poutend(int nComID)
{
	plotdrawend();
}

DllExport void poutdynaclrnoset(int clrno)
{
	plotdynaclrnoset(clrno);
}

DllExport void poutdynapaintmodeset(int mode)
{
	plotdynapaintmodeset(mode);
}

DllExport void poutdynafillstart()
{
	plotdynafillstart();
}

DllExport void poutdynafillend()
{
	plotdynafillend();
}

DllExport void poutdynarulestart()
{
	plotdynarulestart();
}

DllExport void poutdynaruleend()
{
	plotdynaruleend();
}

DllExport void poutdynathickout(DBL thick)
{
	plotdynathickout(thick);
}

DllExport void poutpaintinit(int nComID)
{
	poutroundkind = 0;
	poutmode = PAINT;
	plotpaintinit();
}

DllExport void poutpaintend(int nComID)
{
	plotpaintend();
}

DllExport void poutpenoutinit(int pen)
{
	plotpenoutinit(pen);
}

DllExport void poutpenoutend()
{
	plotpenoutend();
}

DllExport void poutplotinit(int nComID)
{
	poutroundkind = 0;
	poutmode = PLOT;
	plotplotinit();
}

DllExport int poutpaintmodechk()
{
	if(poutmode == PAINT){
		return(ON);
	}
	return(OFF);
}

int poutplotmodechk()
{
	if(poutmode == PLOT){
		return(ON);
	}
	return(OFF);
}

DllExport void poutplotend(int nComID)
{
	plotplotend();
}

DllExport void poutblockstart(int nComID)
{
	poutroundkind = 0;
	plotoutblockstart();
}

DllExport void poutblockend(int nComID)
{
	plotoutblockend();
}

DllExport void poutroundkindset(int kind)
{
	poutroundkind = kind;
}

DllExport void poutrejionstart(int nComID)
{
	switch(plotcmdtype){
	case NCAMADA: 
	case RP93T:
	case RP93N:
	case RP9T:
	case RP6T:
	case RP1T:
	case RP395T:
	case CH37:
	case RP3L:
	case RP3LL:
	case RP35F:
	case LIPS:
	case RPDL:
	case TM13:
		OverlapRest = 0;
		break;
	default:
		OverlapRest = OverlapStep;
		break;
	}
	plotoutrejionstart();
}

DllExport void poutrejionend(int nComID)
{
	plotoutrejionend();
}

DllExport void poutpenchng(int nComID, int nPenno)
{
	//plotpenchng(nPenno);
}

DllExport void poutendhome()
{
	switch(plotcmdtype){
	case RP93T:
	case RP93N:
	case RP95T:
	case RP9T:
	case RP6T:
	case RP395T:
	case CH37:
	case TM13:
		poutlitleboxs(poutsx, poutsy, poutsx+1.0, poutsy+1.0);
		poutlitleboxs(poutsx, poutey, poutsx+1.0, poutey-1.0);
		poutlitleboxs(poutex, poutey, poutex-1.0, poutey-1.0);
		poutlitleboxs(poutex, poutsy, poutex-1.0, poutsy+1.0);
	}
	plotendhome();
}

DllExport void poutpenup()
{
	DBL		crtx, crty;

	plotgetcrtxy(&crtx, &crty);
	plotpenup(crtx + 5.0, crty + 5.0);
}

DllExport void poutoutline(int nComID, DBL sx, DBL sy, DBL ex, DBL ey)
{
	int	flagx1, flagy1, flagx2, flagy2;
	int	x1, y1, x12, y12, x2, y2;
	DBL	ans1, ans2;

	baseDispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}

	if(poutfillflagchk() == OFF){
		poutfirstflag = ON;
		if(OverlapFlag == ON){
			OverlapRest = 0;
		}
		return;
	}

	flagx1 = poutareasignget(sx, poutsx, poutex);
	flagy1 = poutareasignget(sy, poutsy, poutey);
	flagx2 = poutareasignget(ex, poutsx, poutex);
	flagy2 = poutareasignget(ey, poutsy, poutey);

	if(flagx1 == 0){
		x1 = (int)sx;
	}
	else if(flagx1 < 0){
		x1 = (int)poutsx;
	}
	else{
		x1 = (int)poutex;
	}

	if(flagy1 == 0){
		y1 = (int)sy;
	}
	else if(flagy1 < 0){
		y1 = (int)poutsy;
	}
	else{
		y1 = (int)poutey;
	}

	if(flagx2 == 0){
		x2 = (int)ex;
	}
	else if(flagx2 < 0){
		x2 = (int)poutsx;
	}
	else{
		x2 = (int)poutex;
	}

	if(flagy2 == 0){
		y2 = (int)ey;
	}
	else if(flagy2 < 0){
		y2 = (int)poutsy;
	}
	else{
		y2 = (int)poutey;
	}

	if(flagx1 == flagx2 || flagy1 == flagy2){
		if(poutfirstflag == ON){
			if(poutroundkind != 0){
				plotmovekind(poutroundkind, x1, y1);
				if(senvcheckpoutdisp() == ON){
					dispmove(x1, y1);
				}
			}
			else{
				plotmove(x1, y1);
				if(senvcheckpoutdisp() == ON){
					dispmove(x1, y1);
				}
			}
			poutfirstflag = OFF;
		}
		plotdrawline(x1, y1, x2, y2);
		if(senvcheckpoutdisp() == ON){
			dispdrawline(x1, y1, x2, y2);
		}
	}
	else if(flagx1 == 0){
		if(flagx2 < 0){
			x12 = (int)poutsx;
		}
		else{
			x12 = (int)poutex;
		}
		if(flagy1 < 0){
			y12 = (int)poutsy;
		}
		else{
			y12 = (int)poutey;
		}
		if(poutfirstflag == ON){
			if(poutroundkind != 0){
				plotmovekind(poutroundkind, x1, y1);
				if(senvcheckpoutdisp() == ON){
					dispmove(x1, y1);
				}
			}
			else{
				plotmove(x1, y1);
				if(senvcheckpoutdisp() == ON){
					dispmove(x1, y1);
				}
			}
			poutfirstflag = OFF;
		}
		plotdrawline(x1, y1, x12, y12);
		if(senvcheckpoutdisp() == ON){
			dispdrawline(x1, y1, x12, y12);
		}
		plotdrawline(x12, y12, x2, y2);
		if(senvcheckpoutdisp() == ON){
			dispdrawline(x12, y12, x2, y2);
		}
	}
	else if(flagy1 == 0){
		if(flagy2 < 0){
			y12 = (int)poutsy;
		}
		else{
			y12 = (int)poutey;
		}
		if(flagx1 < 0){
			x12 = (int)poutsx;
		}
		else{
			x12 = (int)poutex;
		}
		if(poutfirstflag == ON){
			if(poutroundkind != 0){
				plotmovekind(poutroundkind, x1, y1);
				if(senvcheckpoutdisp() == ON){
					dispmove(x1, y1);
				}
			}
			else{
				plotmove(x1, y1);
				if(senvcheckpoutdisp() == ON){
					dispmove(x1, y1);
				}
			}
			poutfirstflag = OFF;
		}
		plotdrawline(x1, y1, x12, y12);
		if(senvcheckpoutdisp() == ON){
			dispdrawline(x1, y1, x12, y12);
		}
		plotdrawline(x12, y12, x2, y2);
		if(senvcheckpoutdisp() == ON){
			dispdrawline(x12, y12, x2, y2);
		}
	}
	else if(flagx1 == flagy1 && flagx2 == flagy2){
		ans1 = poutex * (sy - ey) + poutsy * (ex - sx) + sx * ey - ex * sy;
		ans2 = poutsx * (sy - ey) + poutey * (ex - sx) + sx * ey - ex * sy;
		if(fabs(ans1) < fabs(ans2)){
			x12 = (int)poutex;
			y12 = (int)poutsy;
		}
		else{
			x12 = (int)poutsx;
			y12 = (int)poutey;
		}
		if(poutfirstflag == ON){
			if(poutroundkind != 0){
				plotmovekind(poutroundkind, x1, y1);
				if(senvcheckpoutdisp() == ON){
					dispmove(x1, y1);
				}
			}
			else{
				plotmove(x1, y1);
				if(senvcheckpoutdisp() == ON){
					dispmove(x1, y1);
				}
			}
			poutfirstflag = OFF;
		}
		plotdrawline(x1, y1, x12, y12);
		if(senvcheckpoutdisp() == ON){
			dispdrawline(x1, y1, x12, y12);
		}
		plotdrawline(x12, y12, x2, y2);
		if(senvcheckpoutdisp() == ON){
			dispdrawline(x12, y12, x2, y2);
		}
	}
	else if(flagx1 != flagy1 && flagx2 != flagy2){
		ans1 = poutsx * (sy - ey) + poutsy * (ex - sx) + sx * ey - ex * sy;
		ans2 = poutex * (sy - ey) + poutey * (ex - sx) + sx * ey - ex * sy;
		if(fabs(ans1) < fabs(ans2)){
			x12 = (int)poutsx;
			y12 = (int)poutsx;
		}
		else{
			x12 = (int)poutex;
			y12 = (int)poutey;
		}
		if(poutfirstflag == ON){
			if(poutroundkind != 0){
				plotmovekind(poutroundkind, x1, y1);
				if(senvcheckpoutdisp() == ON){
					dispmove(x1, y1);
				}
			}
			else{
				plotmove(x1, y1);
				if(senvcheckpoutdisp() == ON){
					dispmove(x1, y1);
				}
			}
			poutfirstflag = OFF;
		}
		plotdrawline(x1, y1, x12, y12);
		if(senvcheckpoutdisp() == ON){
			dispdrawline(x1, y1, x12, y12);
		}
		plotdrawline(x12, y12, x2, y2);
		if(senvcheckpoutdisp() == ON){
			dispdrawline(x12, y12, x2, y2);
		}
	}
}

DllExport void poutmove(int nComID, DBL dx, DBL dy)
{
	poutfirstflag = ON;
}

DllExport void poutline(int nComID, DBL sx, DBL sy, DBL ex, DBL ey)
{
	int		stat, ret;
	DBL		x1, y1, x2, y2;
	DBL		len;

	baseDispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}
	stat = poutareachk(sx, sy, ex, ey);
	if(stat == OFF){
		poutoutline(nComID, sx, sy, ex, ey);
		return;
	}

	x1 = sx; y1 = sy;
	x2 = ex; y2 = ey;
	if(stat == CROSS){
		ret = poutarealegal(&sx, &sy, &ex, &ey);
		if(ret == OFF){
			poutoutline(nComID, x1, y1, x2, y2);
			return;
		}
	}

	if(OverlapFlag == ON){
		if(OverlapRest <= 0){
			return;
		}
		if(sx == ex && sy == ey){
			return;
		}
		len = hypot(ex - sx, ey - sy);
		if(OverlapRest < len){
			ex = (ex - sx) * OverlapRest / len + sx;
			ey = (ey - sy) * OverlapRest / len + sy;
		}
		plotdrawline(sx, sy, ex, ey);
		if(senvcheckpoutdisp() == ON){
			dispdrawline(sx, sy, ex, ey);
		}
		OverlapRest -= len;
		if(OverlapRest < 0){
			OverlapRest = 0;
		}
	}
	else{
		if(x1 != sx || y1 != sy){
			poutoutline(nComID, x1, y1, sx, sy);
		}

		if(poutfirstflag == ON){
			if(poutroundkind != 0){
				plotmovekind(poutroundkind, sx, sy);
				if(senvcheckpoutdisp() == ON){
					dispmove(sx, sy);
				}
			}
			else{
				plotmove(sx, sy);
				if(senvcheckpoutdisp() == ON){
					dispmove(sx, sy);
				}
			}
			poutfirstflag = OFF;
		}
		plotdrawline(sx, sy, ex, ey);
		if(senvcheckpoutdisp() == ON){
			dispdrawline(sx, sy, ex, ey);
		}

		if(x2 != ex || y2 != ey){
			poutoutline(nComID, ex, ey, x2, y2);
		}
	}
}

DllExport void poutbeze(int nComID, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	if(poutdivmode == 0){
		poutolddivbeze(nComID, x1, y1, x2, y2, x3, y3, x4, y4);
	}
	else{ //if(poutdivmode == 1)(
		poutnewdivbeze(nComID, x1, y1, x2, y2, x3, y3, x4, y4);
	}
}

DllExport void poutolddivbeze(int nComID, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	dbezetype	beze[BEZNESTDEEP+10];
	int			stp;
	double		sx, sy, ex, ey, bex, bey;

	baseDispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}
	sx = x1; sy = y1;
	bex = x4; bey = y4;
	stp = 1;
	beze[stp].x1 = x1; beze[stp].y1 = y1;
	beze[stp].x2 = x2; beze[stp].y2 = y2;
	beze[stp].x3 = x3; beze[stp].y3 = y3;
	beze[stp].x4 = x4; beze[stp].y4 = y4;
	while(1){
		if(stp == 0){
			break;
		}
		if(curvebezelinechk(beze[stp], bezdivstep) == ON
		|| stp >= BEZNESTDEEP){
			ex = beze[stp].x4;
			ey = beze[stp].y4;
			poutline(nComID, sx, sy, ex, ey);
			if(ex == bex && ey == bey){
				break;
			}
			sx = ex; sy = ey;
			stp--;
		}
		else{
			curvebezedivbeze(&beze[stp], &beze[stp+1], &beze[stp]);
			stp++;
			if(stp >= BEZNESTDEEP){
				break;
			}
		}
	}
}

DllExport void poutoldragdivbeze(int nComID, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL	rag1, rag2;
	DBL	subrag1, subrag2;
	DBL	sumrag;
	DBL	len;
	int cnt;
	DBL	stepcnt;

	baseDispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}
	if(x4 == x1 && y4 == y1){
		subrag1 = 0;
		subrag2 = pai;
	}
	else{
		if(x2 == x1 && y2 == y1){
			subrag1 = pai/2;
		}
		else{
			rag1 = atan2(x2-x1, y2-y1);
			rag2 = atan2(x4-x1, y4-y1);
			subrag1 = (rag1 - rag2);
		}
		if(x3 == x4 && y3 == y4){
			subrag2 = pai/2;
		}
		else{
			rag1 = atan2(x3-x4, y3-y4);
			rag2 = atan2(x1-x4, y1-y4);
			subrag2 = (rag1 - rag2);
		}
	}

	if(subrag1 > pai){
		subrag1 = subrag1 - pai*2;
	} 
	else if(subrag1 < -pai){
		subrag1 = subrag1 + pai*2;
	}

	if(subrag2 > pai){
		subrag2 = subrag2 - pai*2;
	} 
	else if(subrag2 < -pai){
		subrag2 = subrag2 + pai*2;
	}

	if((subrag1 < 0 && subrag2 > 0)
	|| (subrag2 < 0 && subrag1 > 0)){
		sumrag = fabs(subrag1) * 2 + fabs(subrag2) * 2;
	}
	else{
		sumrag = fabs(subrag1) + fabs(subrag2);
	}

	len = hypot(x2 - x1, y2 - y1);
	if(len > 32.0){
		cnt = (int)(sumrag / degree5);
	}
	else{
		cnt = (int)(sumrag / (degree5*2.0));
	}
	if(cnt <= 4){
		cnt = 5;
	}
	stepcnt = cnt;
	poutbezecountdiv(nComID, x1, y1, x2, y2, x3, y3, x4, y4, stepcnt);
}

DllExport void poutnewdivbeze(int nComID, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL		len1, len2, len3, len;
	DBL		rag1, rag2, subrag;
	DBL		stepcnt, stepcnt1, stepcnt2;

	baseDispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}

	// 長さ分割
	len1 = hypot(y1-y2, x1-x2);
	len2 = hypot(y2-y3, x2-x3);
	len3 = hypot(y3-y4, x3-x4);
	len = len1 + len2 + len3;
	stepcnt1 = len / bezdivstep;

	// 角度分割
	if((y2 == y1 && x2 == x1)
	|| (y3 == y2 && x3 == x2)
	|| (y4 == y3 && x4 == x3)){
		stepcnt2 = 0;
	}
	else{
		rag1 = fabs(atan2(y2 - y1, x2 - x1) - atan2(y3 - y2, x3 - x2));
		rag2 = fabs(atan2(y3 - y2, x3 - x2) - atan2(y4 - y3, x4 - x3));
		if(rag1 > pai){
			rag1 = pai*2 - rag1;
		}
		if(rag2 > pai){
			rag2 = pai*2 - rag2;
		}
		subrag = rag1 + rag2;
		stepcnt2 = (int)(subrag / bezraglimit);
	}
	if(stepcnt2 > stepcnt1){
		stepcnt = stepcnt1;
	}
	else{
		stepcnt = stepcnt2;
	}

	if(stepcnt < bezmindiv){
		stepcnt = bezmindiv;
	}
	if(stepcnt > bezmaxdiv){
		stepcnt = bezmaxdiv;
	}
	poutbezecountdiv(nComID, x1, y1, x2, y2, x3, y3, x4, y4, stepcnt);
}

void poutbezecountdiv(int nComID, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4, DBL stepcnt)
{
	DBL		stepadd;
	DBL		sx, sy, ex, ey;
	DBL		t, t1;
	DBL		b1, b2, b3, b4;

	stepadd = 1.0 / stepcnt;
	sx = x1; sy = y1;
	for(t = stepadd; t < 1.0; t += stepadd){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		ex = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		ey = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
	 	poutline(nComID, sx, sy, ex, ey);
		sx = ex; sy = ey;
	}
 	poutline(nComID, sx, sy, x4, y4);
}

DllExport void poutarc(int nComID, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	int		round, stat, stat1, stat2;
	DBL		x0, y0, dr, rag01, rag03;

	baseDispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}
	stat1 = poutareachk(x1, y1, x2, y2);
	stat2 = poutareachk(x2, y2, x3, y3);
	if(stat1 == OFF && stat2 == OFF){
		poutline(nComID, x1, y1, x2, y2);
		poutline(nComID, x2, y2, x3, y3);
		return;
	}

	if(plotcmdtype != NCAMADA
	&& plotcmdtype != MIMK
	&& plotcmdtype != RLND
	&& plotcmdtype != HPGL
	&& plotcmdtype != CJHP
	&& plotcmdtype != CJ2HP
	&& plotcmdtype != PNCHP
	&& plotcmdtype != CMHP
	&& plotcmdtype != CM24HP
	&& plotcmdtype != PCHP
	&& OverlapFlag != ON
	&& poutlinemode != ON
	&& pout3parcflag == ON
	&& stat1 == INAREA && stat2 == INAREA){
		if(poutfirstflag == ON){
			if(poutroundkind != 0){
				plotmovekind(poutroundkind, x1, y1);
				if(senvcheckpoutdisp() == ON){
					dispmove(x1, y1);
				}
			}
			else{
				plotmove(x1, y1);
				if(senvcheckpoutdisp() == ON){
					dispmove(x1, y1);
				}
			}
			poutfirstflag = OFF;
		}
		plotdraw3penko(x1, y1, x2, y2, x3, y3);
		if(senvcheckpoutdisp() == ON){
			dispdraw3penko(x1, y1, x2, y2, x3, y3);
		}
		return;
	}
	stat = curve3pcenterget(x1, y1, x2, y2, x3, y3, &x0, &y0);
	if(stat == FAIL){
		poutline(nComID, x1, y1, x2, y2);
		poutline(nComID, x2, y2, x3, y3);
		return;
	}
	stat = curve3pragrget(x1, y1, x2, y2, x3, y3, x0, y0, &rag01, &rag03, &dr);
	if(stat == FAIL){
		poutline(nComID, x1, y1, x2, y2);
		poutline(nComID, x2, y2, x3, y3);
		return;
	}
	if((stat1 == CROSS || stat2 == CROSS)
	|| poutlinemode == ON
	|| plotenkolimitchk(x0, y0, dr) == ON
	|| OverlapFlag == ON){
		poutenkoline(nComID, x0, y0, x1, y1, x3, y3, dr, rag01, rag03);
	}
	else{
		if(plotcmdtype == MIMK && pout3parcflag == ON){
			if(rag01 > rag03){
				round = 1;
			}
			else{
				round = -1;
			}
			if(poutfirstflag == ON){
				if(poutroundkind != 0){
					plotmovekind(poutroundkind, x1, y1);
					if(senvcheckpoutdisp() == ON){
						dispmove(x1, y1);
					}
				}
				else{
					plotmove(x1, y1);
					if(senvcheckpoutdisp() == ON){
						dispmove(x1, y1);
					}
				}
				poutfirstflag = OFF;
			}
			plotdraw3pmenko(round, x1, y1, x3, y3, x0, y0);
			if(senvcheckpoutdisp() == ON){
				dispdrawenko(x0, y0, dr, rag01, rag03, x1, y1, x3, y3);
			}
		}
		else if(plotcmdtype == NCAMADA && pout3parcflag == ON){
			if(rag01 > rag03){
				round = 1;
			}
			else{
				round = -1;
			}
			if(poutfirstflag == ON){
				if(poutroundkind != 0){
					plotmovekind(poutroundkind, x1, y1);
					if(senvcheckpoutdisp() == ON){
						dispmove(x1, y1);
					}
				}
				else{
					plotmove(x1, y1);
					if(senvcheckpoutdisp() == ON){
						dispmove(x1, y1);
					}
				}
				poutfirstflag = OFF;
			}
			plotdraw3paenko(round, x1, y1, x3, y3, dr, x0, y0);
			if(senvcheckpoutdisp() == ON){
				dispdrawenko(x0, y0, dr, rag01, rag03, x1, y1, x3, y3);
			}
		}
		else{
			if(poutfirstflag == ON){
				if(poutroundkind != 0){
					plotmovekind(poutroundkind, x1, y1);
					if(senvcheckpoutdisp() == ON){
						dispmove(x1, y1);
					}
				}
				else{
					plotmove(x1, y1);
					if(senvcheckpoutdisp() == ON){
						dispmove(x1, y1);
					}
				}
				poutfirstflag = OFF;
			}
			plotdrawenko(x0, y0, dr, rag01/degree01, rag03/degree01, x1, y1, x3, y3);
			if(senvcheckpoutdisp() == ON){
				dispdrawenko(x0, y0, dr, rag01, rag03, x1, y1, x3, y3);
			}
		}
	}
}

DllExport void poutfreecurve(int nComID, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
{
	int		stat;
	DBL		rag1, rag2;
	int		rag1no, rag2no;
	DBL		xp1, yp1, xp2, yp2;

	baseDispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}
	stat = poutareachk(x1, y1, x2, y2);
	if(stat == OFF){
		poutoutline(nComID, x1, y1, x2, y2);
		return;
	}
	rag1 = pdrwstartragget(atr0, atr1, x0, y0, x1, y1, x2, y2);
	rag2 = pdrwendragget(atr1, atr2, x1, y1, x2, y2, x3, y3);

	if(rag1 == NORAG && rag2 == NORAG){
		poutline(nComID, x1, y1, x2, y2);
	}
	else{
		curvenoragtocurverag(&rag1, &rag2);
		rag1no = curveragnoget(rag1);
		rag2no = curveragnoget(rag2);

		if(rag1no == rag2no || rag1no == HALFPAI || rag2no == HALFPAI){
			curvebezecontrolpointget(x1, y1, x2, y2, rag1, &xp1, &yp1);
			curvebezecontrolpointget(x2, y2, x1, y1, rag2, &xp2, &yp2);
			poutbeze(nComID, x1, y1, xp1, yp1, xp2, yp2, x2, y2);
		}
		else{
			if(curvedivragchk(rag1, rag2)){
				poutragenko(nComID, rag1, x1, y1, x2, y2);
			}
			else{
				poutdivragenko(nComID, rag1, x1, y1, x2, y2, rag2);
			}
		}
	}
}

DllExport int poutchkdeepcut(DBL x0,DBL y0,DBL x1,DBL y1,DBL x2,DBL y2)
{
	DBL	rag01, rag12, subrag;

	if(plotdeepcutflag == OFF){
		return(OFF);
	}
	if(plotdeepcutstep <= 0){
		return(OFF);
	}
	if(x0 == x1 && y0 == y1){
		return(OFF);
	}
	if(x1 == x2 && y1 == y2){
		return(OFF);
	}
	rag01 = atan2(y1-y0, x1-x0);
	rag12 = atan2(y2-y1, x2-x1);
	subrag = rag01 - rag12;
	subrag = fabs(subrag);
	if(subrag > 2*pai){
		subrag -= 2*pai;
	}
	if(subrag > pai){
		subrag = 2*pai - subrag;
	}
	if(subrag > degree1 * 60){
		return(ON);
	}
	return(OFF);
}

DllExport void poutstartdeepcut(DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL	x0, y0, rag21, tsin, tcos;

	rag21 = atan2(y1-y2, x1-x2);
	tsin = sin(rag21);
	tcos = cos(rag21);
	x0 = tcos * plotdeepcutstep + x1;
	y0 = tsin * plotdeepcutstep + y1;
	poutmove(0, x0, y0);
	poutline(0, x0, y0, x1, y1);
}

DllExport void poutenddeepcut(DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL	x3, y3, rag12, tsin, tcos;

	rag12 = atan2(y2-y1, x2-x1);
	tsin = sin(rag12);
	tcos = cos(rag12);
	x3 = tcos * plotdeepcutstep + x2;
	y3 = tsin * plotdeepcutstep + y2;
	poutline(0, x2, y2, x3, y3);
}

void poutenkoline(int nComID, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL dr, DBL srag, DBL erag)
{
	if(poutdivmode == 0){
		poutoldhightenkoline(nComID, x0, y0, x1, y1, x2, y2, dr, srag, erag);
	}
	else{ //if(poutdivmode == 1)(
		poutnewenkoline(nComID, x0, y0, x1, y1, x2, y2, dr, srag, erag);
	}
}

void poutoldhightenkoline(int nComID, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL dr, DBL srag, DBL erag)
{
	DBL 	s, divrag;
	DBL 	subrag, steprag, rag;
	DBL		tsin, tcos;
	DBL		sx, sy, ex, ey;
	int		i, cnt;

	baseDispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}

	subrag = erag - srag;
	if(dr < arclimitstep){
		poutline(nComID, x1, y1, x2, y2);
		return;
	}
	s = dr - arclimitstep;
	divrag = acos(s/dr);
	if(arcraglimit > divrag){
		divrag = arcraglimit;
	}

	cnt = (int)(fabs(subrag / divrag));
	if(cnt < 2){
		poutline(nComID, x1, y1, x2, y2);
		return;
	}

	steprag = subrag / cnt;

	sx = x1; sy = y1;
	for(i = 1; i < cnt; i++){
		rag = srag+((double)i)*steprag;
		tsin = sin(rag);
		tcos = cos(rag);
		ex = tcos*dr+x0;
		ey = tsin*dr+y0;
		poutline(nComID, sx, sy, ex, ey);
		sx = ex;
		sy = ey;
	}
	ex = x2; ey = y2;
	poutline(nComID, sx, sy, ex, ey);
}

void poutoldenkoline(int nComID, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL dr, DBL srag, DBL erag)
{
	DBL 	subrag, steprag, rag;
	DBL		tsin, tcos;
	DBL		sx, sy, ex, ey;
	int		i, cnt;

	subrag = erag - srag;
	cnt = (int)(fabs(subrag / (degree1)));
	if(cnt <= 2){
		cnt = 2;
	}
	steprag = subrag / (double)cnt;

	sx = x1; sy = y1;
	for(i = 1; i < cnt; i++){
		rag = srag+((double)i)*steprag;
		tsin = sin(rag);
		tcos = cos(rag);
		ex = tcos*dr+x0;
		ey = tsin*dr+y0;
		poutline(nComID, sx, sy, ex, ey);
		sx = ex;
		sy = ey;
	}
	ex = x2; ey = y2;
	poutline(nComID, sx, sy, ex, ey);
}

void poutnewenkoline(int nComID, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL dr, DBL srag, DBL erag)
{
	DBL		dsubrag;
	DBL		len;
	DBL		rag, steprag;
	DBL		sx, sy, ex, ey;
	int		stepcnt, stepcnt1, stepcnt2;
	int		idx;

	baseDispatch();
	if(senvchckflag(STOPFLAG) == ON){
		return;
	}

	dsubrag = fabs(erag - srag);
	if(dsubrag > pai){
		dsubrag = pai*2 - dsubrag;
	}

	len = 2.0 * dr * dsubrag;
	stepcnt1 = (int)(len / arclimitstep);
	stepcnt2 = (int)(dsubrag / arcraglimit);
	if(stepcnt2 > stepcnt1){
		stepcnt = stepcnt1;
	}
	else{
		stepcnt = stepcnt2;
	}

	if(stepcnt < arcmindiv){
		stepcnt = arcmindiv;
	}
	if(stepcnt > arcmaxdiv){
		stepcnt = arcmaxdiv;
	}

	steprag = (erag - srag) / stepcnt;
	sx = x1; sy = y1;
	for(idx = 1, rag = srag+steprag; idx < stepcnt; idx++, rag += steprag){
		ex = cos(rag)*dr+x0;
		ey = sin(rag)*dr+y0;
		poutline(nComID, sx, sy, ex, ey);
		sx = ex; sy = ey;
	}
	poutline(nComID, sx, sy, x2, y2);
}

void poutlitleboxs(DBL dsx, DBL dsy, DBL dex, DBL dey)
{
	int		sx, sy, ex, ey;

	sx = (int)dsx;
	sy = (int)dsy;
	ex = (int)dex;
	ey = (int)dey;
	plotoutrejionstart();
	plotmove(sx, sy);
	plotdrawline(sx, sy, sx, ey);
	plotdrawline(sx, ey, ex, ey);
	plotdrawline(ex, ey, ex, sy);
	plotdrawline(ex, sy, sx, sy);
	plotoutrejionend();
}

void poutragenko(int nComID, DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2)
{
	DBL		x0, y0, rag01, rag02, r;
	DBL		ragp, x12, y12;
	int		round;
	int		ret;

	if(curvecenterget(rag1, x1, y1, x2, y2, &x0, &y0) == FAIL){
		poutline(nComID, x1, y1, x2, y2);
		return;
	}
	if(curveragrget(x1, y1, x2, y2, x0, y0, &rag01, &rag02, &r) == FAIL){
		poutline(nComID, x1, y1, x2, y2);
		return;
	}
	ret = poutareachk(x1, y1, x2, y2);
	if(ret == OFF){
		poutline(nComID, x1, y1, x2, y2);
		return;
	}
	if(ret == CROSS
	|| poutlinemode == ON
	|| plotenkolimitchk(x0, y0, r) == ON
	|| OverlapFlag == ON){
		poutenkoline(nComID, x0, y0, x1, y1, x2, y2, r, rag01, rag02);
	}
	else{
		if(pout3parcflag == ON && plotcmdtype == MIMK){
			if(rag01 > rag02){
				round = 1;
			}
			else{
				round = -1;
			}
			if(poutfirstflag == ON){
				if(poutroundkind != 0){
					plotmovekind(poutroundkind, x1, y1);
					if(senvcheckpoutdisp() == ON){
						dispmove(x1, y1);
					}
				}
				else{
					plotmove(x1, y1);
					if(senvcheckpoutdisp() == ON){
						dispmove(x1, y1);
					}
				}
				poutfirstflag = OFF;
			}
			plotdraw3pmenko(round, x1, y1, x2, y2, x0, y0);
			if(senvcheckpoutdisp() == ON){
				dispdrawenko(x0, y0, r, rag01, rag02, x1, y1, x2, y2);
			}
		}
		else if(plotcmdtype == NCAMADA && pout3parcflag == ON){
			if(rag01 > rag02){
				round = 1;
			}
			else{
				round = -1;
			}
			if(poutfirstflag == ON){
				if(poutroundkind != 0){
					plotmovekind(poutroundkind, x1, y1);
					if(senvcheckpoutdisp() == ON){
						dispmove(x1, y1);
					}
				}
				else{
					plotmove(x1, y1);
					if(senvcheckpoutdisp() == ON){
						dispmove(x1, y1);
					}
				}
				poutfirstflag = OFF;
			}
			plotdraw3paenko(round, x1, y1, x2, y2, r, x0, y0);
			if(senvcheckpoutdisp() == ON){
				dispdrawenko(x0, y0, r, rag01, rag02, x1, y1, x2, y2);
			}
		}
		else if(pout3parcflag == ON
		&& plotcmdtype != HPGL
		&& plotcmdtype != CJHP
		&& plotcmdtype != CJ2HP
		&& plotcmdtype != PNCHP
		&& plotcmdtype != CMHP	
		&& plotcmdtype != CM24HP	
		&& plotcmdtype != PCHP
		&& plotcmdtype != RLND){
			ragp = (rag01 + rag02) / 2.0;
			x12 = cos(ragp)*r+x0;
			y12 = sin(ragp)*r+y0;
			if(poutfirstflag == ON){
				if(poutroundkind != 0){
					plotmovekind(poutroundkind, x1, y1);
					if(senvcheckpoutdisp() == ON){
						dispmove(x1, y1);
					}
				}
				else{
					plotmove(x1, y1);
					if(senvcheckpoutdisp() == ON){
						dispmove(x1, y1);
					}
				}
				poutfirstflag = OFF;
			}
			plotdraw3penko(x1, y1, x12, y12, x2, y2);
			if(senvcheckpoutdisp() == ON){
				dispdrawenko(x0, y0, r, rag01, rag02, x1, y1, x2, y2);
			}
		}
		else{
			if(poutfirstflag == ON){
				if(poutroundkind != 0){
					plotmovekind(poutroundkind, x1, y1);
					if(senvcheckpoutdisp() == ON){
						dispmove(x1, y1);
					}
				}
				else{
					plotmove(x1, y1);
					if(senvcheckpoutdisp() == ON){
						dispmove(x1, y1);
					}
				}
				poutfirstflag = OFF;
			}
			plotdrawenko(x0, y0, r, rag01/degree01, rag02/degree01, x1, y1, x2, y2);
			if(senvcheckpoutdisp() == ON){
				dispdrawenko(x0, y0, r, rag01, rag02, x1, y1, x2, y2);
			}
		}
	}
}

void poutdivragenko(int nComID, DBL rag1, DBL x1, DBL y1, DBL x2, DBL y2, DBL rag2)
{
	DBL		rag, ragp, x12, y12;
	if(curvedivpointget(rag1, x1, y1, x2, y2, rag2, &x12, &y12) == FAIL){
		poutline(nComID, x1, y1, x2, y2);
		return;
	}
	ragp = atan2(y12 - y1, x12 - x1) - atan2(y2 - y1, x2 - x1);
	rag = rag1-ragp;
	if(x1 != x12 || y1 != y12){
		poutragenko(nComID, rag, x1, y1, x12, y12);
	}
	ragp = atan2(y12 - y2, x12 - x2) - atan2(y1 - y2, x1 - x2);
	rag = ragp-rag2;
	if(x2 != x12 || y2 != y12){
		poutragenko(nComID, rag, x12, y12, x2, y2);
	}
}

int poutareasignget(DBL num, DBL mini, DBL max)
{
	if(num < mini){
		return(-1);
	}
	else if(num > max){
		return(1);
	}
	else{
		return(0);
	}
}

int poutarealegal(DBL *sx, DBL *sy, DBL *ex, DBL *ey)
{
	DBL	x1, y1, x2, y2;
	DBL	sub1, sub2, sub3;

	x1 = *sx; y1 = *sy;
	x2 = *ex; y2 = *ey;
	if(x1 < x2){
		if(x2 < poutsx)	return(OFF);
		if(x1 > poutex)	return(OFF);
		if(x1 <= poutsx){
			sub3 = poutsx - x1;
			sub1 = x2 - x1;
			if(y1 < y2){
				sub2 = y2 - y1;
				y1 += ((sub2 * sub3) / sub1);
			}
			else if(y1 > y2){
				sub2 = y1 - y2;
				y1 -= ((sub2 * sub3) / sub1);
			}
			x1 = poutsx;
		}
		if(x2 >= poutex){
			sub3 = x2 - poutex;
			sub1 = x2 - x1;
			if(y2 < y1){
				sub2 = y1 - y2;
				y2 += ((sub2 * sub3) / sub1);
			}
			else if(y2 > y1){
				sub2 = y2 - y1;
				y2 -= ((sub2 * sub3) / sub1);
			}
			x2 = poutex;
		}
	}
	else if(x1 > x2){
		if(x1 < poutsx)	return(OFF);
		if(x2 > poutex)	return(OFF);
		if(x2 <= poutsx){
			sub3 = poutsx - x2;
			sub1 = x1 - x2;
			if(y2 < y1){
				sub2 = y1 - y2;
				y2 += ((sub2 * sub3) / sub1);
			}
			else if(y2 > y1){
				sub2 = y2 - y1;
				y2 -= ((sub2 * sub3) / sub1);
			}
			x2 = poutsx;
		}
		if(x1 >= poutex){
			sub3 = x1 - poutex;
			sub1 = x1 - x2;
			if(y1 < y2){
				sub2 = y2 - y1;
				y1 += ((sub2 * sub3) / sub1);
			}
			else if(y1 > y2){
				sub2 = y1 - y2;
				y1 -= ((sub2 * sub3) / sub1);
			}
			x1 = poutex;
		}
	}
	else if(x1 == x2){
		if(x1 > poutex) return(OFF);
		if(x1 < poutsx) return(OFF);
	}
	if(y1 < y2){
		if(y2 < poutsy)	return(OFF);
		if(y1 > poutey)	return(OFF);

		if(y1 <= poutsy){
			sub3 = poutsy - y1;
			sub1 = y2 - y1;
			if(x1 < x2){
				sub2 = x2 - x1;
				x1 += ((sub2 * sub3) / sub1);
			}
			else if(x1 > x2){
				sub2 = x1 - x2;
				x1 -= ((sub2 * sub3) / sub1);
			}
			y1 = poutsy;
		}
		if(y2 >= poutey){
			sub3 = y2 - poutey;
			sub1 = y2 - y1;
			if(x2 < x1){
				sub2 = x1 - x2;
				x2 += ((sub2 * sub3) / sub1);
			}
			else if(x2 > x1){
				sub2 = x2 - x1;
				x2 -= ((sub2 * sub3) / sub1);
			}
			y2 = poutey;
		}
	}
	else if(y1 > y2){
		if(y1 < poutsy)	return(OFF);
		if(y2 > poutey)	return(OFF);
		if(y2 <= poutsy){
			sub3 = poutsy - y2;
			sub1 = y1 - y2;
			if(x2 < x1){
				sub2 = x1 - x2;
				x2 += ((sub2 * sub3) / sub1);
			}
			else if(x2 > x1){
				sub2 = x2 - x1;
				x2 -= ((sub2 * sub3) / sub1);
			}
			y2 = poutsy;
		}
		if(y1 >= poutey){
			sub3 = y1 - poutey;
			sub1 = y1 - y2;
			if(x1 < x2){
				sub2 = x2 - x1;
				x1 += ((sub2 * sub3) / sub1);
			}
			else if(x1 > x2){
				sub2 = x1 - x2;
				x1 -= ((sub2 * sub3) / sub1);
			}
			y1 = poutey;
		}
	}
	else if(y1 == y2){
		if(y1 > poutey) return(OFF);
		if(y1 < poutsy) return(OFF);
	}
	*sx = x1; *sy = y1;
	*ex = x2; *ey = y2;
	return(ON);
}

