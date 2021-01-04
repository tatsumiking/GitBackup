//
// Easy Scanner System Ver1.0
//
// ess  plot.c	プロッターコマンド処理関数
//
//  Copyright (c) 1998 by THEON

#include	"stdafx.h"

#include	<winspool.h>

#include	"Ploter.h"
#include	"Plot.h"

#include	"Scrnlib.h"
#include	"Outlib.h"

#define	ESC		0x1b
#define	IS2		0x1e
#define	IS1		0x1f
#define	PAT		0x25
#define FF		0x0c
#define	ETX		0x03
#define	INCH	25.4

#define	NODBG	1

#define	RPDLPOINTMAX	350
#define PLOTPENMAX		10

DBL		plotstartx, plotstarty;
DBL		plotendx, plotendy;

int		plotdynamode;

DBL		plotsxmm, plotsymm, plotxmm, plotymm;
DBL		plotstep, plotxtime, plotytime;
int		plotkind, plotcntr;
int		plothomemode;
DBL		plotcntrxmm, plotcntrymm;
DBL		plotmovex, plotmovey;

int		plgncnt;
int		plotblockflag, plotrejionflag;
int		plotpaintmode, plotpaintfirst;
DBL		plotlimitstep, rlimitstep;
DBL		plotstartspace, plotendspace;
int		plotrejionfirst;
int		plotlinefirst;
DBL		plotcrtx, plotcrty;

DBL		rejionsx, rejionsy;
char	szfilename[FNAMEMAX];
char	szdataname[FNAMEMAX];
char	plotprebuf[RECMAX];
char	plotpostbuf[RECMAX];
int		plotpentbl[PLOTPENMAX] = {1, 3, 4, 5, 1, 1, 1, 1, 1, 1};

DllExport void plotpentblset(int idx, int val)
{
	plotpentbl[idx] = val;
}

DllExport void plotprebufset(LPSTR buf)
{
	sprintf_s(plotprebuf, RECMAX, "%s\n", buf);
}

DllExport void plotpostbufset(LPSTR buf)
{
	sprintf_s(plotpostbuf, RECMAX, "%s\n", buf);
}

void plotfilenameset(LPSTR filename)
{
	strcpy_s(szfilename, FNAMEMAX, filename);
}

void plotdatanameset(LPSTR dataname)
{
	strcpy_s(szdataname, RECMAX, dataname);
}

void plotsetparaminit(DBL plstep, DBL rlstep, DBL smm, DBL emm)
{
	plotlimitstep = plstep;
	rlimitstep = rlstep;
	plotstartspace = smm;
	plotendspace = emm;
}

void plotgetparaminit(LPDBL plstep, LPDBL rlstep, LPDBL smm, LPDBL emm)
{
	*plstep = plotlimitstep;
	*rlstep = rlimitstep;
	*smm = plotstartspace;
	*emm = plotendspace;
}

void plotkindset(int kind)
{
	plotkind = kind;
}

void plotmmsizeset(DBL xsize, DBL ysize)
{
	plotxmm = xsize;
	plotymm = ysize;
}

void plotstepset(DBL step)
{
	plotstep = step;
}

void plotmmstartset(DBL sx, DBL sy)
{
	plotsxmm = sx;
	plotsymm = sy;
}

void plotmmstartget(DBL *sx, DBL *sy)
{
	*sx = plotsxmm;
	*sy = plotsymm;
}

void plotmmcntrset(DBL cx, DBL cy)
{
	plotcntrxmm = cx;
	plotcntrymm = cy;
}

void plottimeset(DBL xtime, DBL ytime)
{
	plotxtime = xtime;
	plotytime = ytime;
}

void plotcntrkindset(int cntrkind)
{
	plotcntr = cntrkind;
}

DllExport void plotmovexyset(DBL xmove, DBL ymove)
{
	plotmovex = xmove;
	plotmovey = ymove;
}

void plotcomidset(int id)
{
}

DllExport void plotgetcrtxy(LPDBL crtx, LPDBL crty)
{
	*crtx = plotcrtx;
	*crty = plotcrty;
}

DllExport void plotmmareaset(DBL sx, DBL sy, DBL ex, DBL ey)
{
	plotstartx = sx;
	plotstarty = sy;
	plotendx = ex;
	plotendy = ey;
}

DllExport void plotdxymmtostep(LPDBL x, LPDBL y)
{
	DBL		mmx, mmy;

	//1998.07.17　プロッタ出力時に文字が逆になる現象の修正

	//switch(plotkind){
	//case RPDL:
	//case LIPS:
	//	break;
	//default:
		//scrngetmmsize(&mmx, &mmy);
		//*y = mmy - (*y); 
	//	break;
	//}
	switch(plotkind){
	case LASERPRO:
		scrngetmmsize(&mmx, &mmy);
		*y = mmy - (*y); 
		break;
	}
	switch(plotcntr){
	case LEFTDOWN:
		mmx = (*x) - plotstartx;
		mmy = (*y) - plotstarty;
		//mmy = plotendy - (*y);
		break;
	case LEFTUP:
		mmx = (*x) - plotstartx;
		mmy = plotendy - (*y);
		//mmy = (*y) - plotstarty;
		break;
	case RIGHTUP:
		mmx = plotendx - (*x);
		mmy = plotendy - (*y);
		//mmy = (*y) - plotstarty;
		break;
	case RIGHTDOWN:
		mmx = plotendx - (*x);
		mmy = (*y) - plotstarty;
		//mmy = plotendy - (*y);
		break;
	}

	plotabsxyget(&mmx, &mmy);
	plotabstimetrns(&mmx, &mmy);
	*x = mmx;
	*y = mmy;
}

DllExport void plotdxysteptomm(LPDBL x, LPDBL y)
{
	DBL		mmx, mmy;

	plotmmxyget(x, y);
	switch(plotcntr){
	case LEFTDOWN:
		(*x) = (*x) + plotstartx;
		(*y) = (*y) + plotstarty;
		break;
	case LEFTUP:
		(*x) = (*x) + plotstartx;
		(*y) = plotendy - (*y);
		break;
	case RIGHTUP:
		(*x) = plotendx - (*x);
		(*y) = plotendy - (*y);
		break;
	case RIGHTDOWN:
		(*x) = plotendx - (*x);
		(*y) = (*y) + plotstarty;
		break;
	}
	switch(plotkind){
	case LASERPRO:
		scrngetmmsize(&mmx, &mmy);
		*y = mmy - (*y); 
		break;
	}
}

DllExport void plotlenmmtostep(LPDBL len)
{
	DBL	steplen;

	steplen = *len;
	*len = steplen / plotstep;
}

void plotstartendmmget(DBL *smm, DBL *emm)
{
	switch(plotkind){
	case RP93T:
	case RP93N:
	case RP9T:
	case RP6T:
		*smm = plotstartspace;
		if(plotendspace == 0.0){
			*emm = 15.0;
		}
		else{
			*emm = plotendspace;
		}
		break;
	default:
		*smm = plotstartspace;
		*emm = plotendspace;
		break;
	}
}

void plotsmmlegalset(DBL smm)
{
	int		ismm;

	plotmmcntrset(0.0, 0.0);
	switch(plotkind){
	case RP3L:
	case RP3LL:
	case RP35F:
		ismm = (int)(smm / 10.0);
		smm = ismm * 10.0;
		if(smm < 10.0){
			plotstartspace = 7.0;
		}
		else if(smm > 90.0){
			plotstartspace = 90.0;
		}
		else{
			plotstartspace = smm;
		}
		break;
	case RP93T:
	case RP93N:
		ismm = (int)(smm / 10.0);
		smm = ismm * 10.0;
		smm -= 80.0;
		plotmmcntrset(80.0, 0.0);
		if(smm < 15.0){
			plotstartspace = 15.0;
		}
		else if(smm > 990.0){
			plotstartspace = 990.0;
		}
		else{
			plotstartspace = smm;
		}
		break;
	case RP95T:
		ismm = (int)(smm / 10.0);
		smm = ismm * 10.0;
		smm -= 80.0;
		plotmmcntrset(80.0, 0.0);
		if(smm < 0.0){
			plotstartspace = 0.0;
		}
		else if(smm > 990.0){
			plotstartspace = 990.0;
		}
		else{
			plotstartspace = smm;
		}
		break;
	case RP9T:
	case RP6T:
		ismm = (int)(smm / 10.0);
		smm = ismm * 10.0;
		if(smm < 60.0){
			plotstartspace = 60.0;
		}
		else if(smm > 990.0){
			plotstartspace = 990.0;
		}
		else{
			plotstartspace = smm;
		}
		break;
	case RP1T:
		if(smm < 0.0){
			plotstartspace = 0.0;
		}
		else if(smm > 190.0){
			plotstartspace = 190.0;
		}
		else{
			plotstartspace = smm;
		}
		break;
	case TM13:
		if(smm < 0.0){
			plotstartspace = 0;
		}
		else{
			plotstartspace = smm;
		}
		break;
	case RPDL:
		if(smm < 5.0){
			plotstartspace = 5.0;
		}
		else{
			plotstartspace = smm;
		}
		break;
	default:
		if(smm < 0.0){
			plotstartspace = 0;
		}
		else{
			plotstartspace = smm;
		}
		break;
	}
}

int plotemmeffectchk()
{
	switch(plotkind){
	case RP93T:
	case RP93N:
	case RP95T:
	case RP9T:
	case RP6T:
	case TM13:
		return(ON);
		break;
	}
	return(OFF);
}

void plotemmlegalset(DBL emm)
{
	int		iemm;

	switch(plotkind){
	case RP93T:
	case RP93N:
		iemm = (int)(emm / 10.0);
		emm = iemm * 10.0;
		if(emm < 15.0){
			plotendspace = 15.0;
		}
		else if(emm > 990.0){
			plotendspace = 990.0;
		}
		else{
			plotendspace = emm;
		}
		break;
	case RP95T:
		iemm = (int)(emm / 10.0);
		emm = iemm * 10.0;
		if(emm < 0.0){
			plotendspace = 0.0;
		}
		else if(emm > 990.0){
			plotendspace = 990.0;
		}
		else{
			plotendspace = emm;
		}
		break;
	case RP9T:
	case RP6T:
		iemm = (int)(emm / 10.0);
		emm = iemm * 10.0;
		if(emm < 15.0){
			plotendspace = 15.0;
		}
		else if(emm > 990.0){
			plotendspace = 990.0;
		}
		else{
			plotendspace = emm;
		}
		break;
	case TM13:
		if(emm < 0.0){
			plotendspace = 0;
		}
		else{
			plotendspace = emm;
		}
		break;
	}
}

void plotsxchanelslct(int num)
{
	char	buf[RECMAX];

	sprintf_s(buf, RECMAX, "@bFOUT%d;", num);
	plotoutstring(buf);
}

int plotpausesend()
{
	char	buf[RECMAX];

	switch(plotkind){
	case GRPH:
		sprintf_s(buf, RECMAX, "F %c", ETX);
		break;
	default :
		return(0);
	}
	plotoutstring(buf);
	return(0);
}

void plotabsxyget(DBL *plotx, DBL *ploty)
{
	DBL	x, y;

	switch(plotcntr){
	case LEFTDOWN:
		x = *plotx;
		y = *ploty;
		break;
	case LEFTUP:
		x = *ploty;
		y = *plotx;
		break;
	case RIGHTUP:
		x = *plotx;
		y = *ploty;
		break;
	case RIGHTDOWN:
		x = *ploty;
		y = *plotx;
		break;
	}
	x += plotmovex;
	y += plotmovey;
	x += plotsxmm;
	y += plotsymm;
	x += plotcntrxmm;
	y += plotcntrymm;
	switch(plotkind){
	case RPDL:
	case LIPS:
		*plotx = y / plotstep;
		*ploty = x / plotstep;
		break;
	default:
		*plotx = x / plotstep;
		*ploty = y / plotstep;
		break;
	}
}

void plotmmxyget(DBL *mmx, DBL *mmy)
{
	DBL	x, y;

	switch(plotkind){
	case RPDL:
	case LIPS:
		x = (*mmy) * plotstep;
		y = (*mmx) * plotstep;
		break;
	default:
		x = (*mmx) * plotstep;
		y = (*mmy) * plotstep;
		break;
	}
	x -= plotmovex;
	y -= plotmovey;
	x -= plotsxmm;
	y -= plotsymm;
	x -= plotcntrxmm;
	y -= plotcntrymm;
	switch(plotcntr){
	case LEFTDOWN:
		*mmx = x;
		*mmy = y;
		break;
	case LEFTUP:
		*mmx = y;
		*mmy = x;
		break;
	case RIGHTUP:
		*mmx = x;
		*mmy = y;
		break;
	case RIGHTDOWN:
		*mmx = y;
		*mmy = x;
		break;
	}
}

void plotmmlenget(DBL *len)
{
	DBL		steplen;

	steplen = *len;
	*len = steplen / plotstep;
}

void plotabstimetrns(DBL *x, DBL *y)
{
	(*x) /= (DBL)plotxtime;
	(*y) /= (DBL)plotytime;
}

int plotenkolimitchk(DBL x0, DBL y0, DBL r)
{
	//if(x0 > plotlimitstep || y0 > plotlimitstep || r > rlimitstep){
	if(r > rlimitstep){
		return(ON);
	}
	return(OFF);
}

void plotoutlength(DBL xlength, DBL ylength)
{
	char	buf[RECMAX];
	DBL		ylen;

	switch(plotkind){
	case RP395T:
		sprintf_s(buf, RECMAX, "%c%c-12345X", ESC, PAT);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "@PJL SET MACHINE=RP395P%c", LF);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "@PJL SET PAPERWIDTHMM=%4.0lf%c", ylength, LF);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "@PJL SET PAPERLENGTHMM=%4.0lf%c", xlength, LF);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "@PJL SET PAPERWIDTHPIX=%4.0lf%c", ylength*203/254, LF);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "@PJL SET PAPERLENGTHPIX=%4.0lf%c", xlength*203/254, LF);
		plotoutstring(buf);
		if(ylength > 3010){
			ylen = 2910;
		}else{
			ylen = ylength - 100;
		}
		sprintf_s(buf, RECMAX, "@PJL SET PRINTWIDTHMM=%4.0lf%c", ylen, LF);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "@PJL SET PRINTLENGTHMM=%4.0lf%c", xlength, LF);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "@PJL SET PRINTWIDTHPIX=%4.0lf%c", ylen*203/254, LF);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "@PJL SET PRINTLENGTHPIX=%4.0lf%c", xlength*203/254, LF);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "@PJL SET USESETUP=1%c", LF);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "@PJL SET AUTOCUT=1%c", LF);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "@PJL SET PRECUT=0%c", LF);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "@PJL SET PAPERSNSVOL=5%c", LF);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "@PJL SET PRNRESO=1%c", LF);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "@PJL SET COPIES=1%c", LF);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "@PJL ENTER LANGUAGE=PCL%c", LF);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "%cE", ESC);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "%c%c0F", ESC, PAT);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "Z %2.0lf,%2.0lf%c", xlength, ylength, ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J0%c", ETX);
		plotoutstring(buf);
		break;
	case RP93N:
	case RP95T:
	case CH37:
		sprintf_s(buf, RECMAX, "Z%2.0lf,%2.0lf%c", xlength, ylength, ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J0%c", ETX);
		plotoutstring(buf);
		break;
	}
}

void plotdrawinit()
{
	char	buf[RECMAX];
	int		i;

	switch(plotkind){
	case NCAMADA:
		sprintf_s(buf, RECMAX, "O200;\n");
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "M102(%s);\n", szfilename); 
		plotoutstring(buf);
		// G92 G90 X1000 Y1000;\n
		plotoutstring(plotprebuf);
		sprintf_s(buf, RECMAX, "G93 X0 Y0;\n");
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "M100;\n");
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "%s;\n", szdataname);
		plotoutstring(buf);
		break;
	case LASERPRO:
		sprintf_s(buf, RECMAX, "%c%cNMoveCheck", ESC, PAT);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "%cIN", ESC);
		plotoutstring(buf);
		for(i = 0; i < 16; i++){
			sprintf_s(buf, RECMAX, "VS500;FS50;");
			plotoutstring(buf);
		}
		sprintf_s(buf, RECMAX, "IP0,0,2032,2032;");
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "SC0,500,0,500;");
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "PR472,0;");
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "SP1;");
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "%c*t500R;", ESC);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "%c%cE", ESC, PAT);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "PR");
		plotoutstring(buf);
		plotcrtx = 0;
		plotcrty = 0;
		break;
	case HPGL:
		sprintf_s(buf, RECMAX, "IN;SP1;");
		plotoutstring(buf);
		break;
	case CMHP:
		sprintf_s(buf, RECMAX, "IN;PA0,0;SP1;");
		plotoutstring(buf);
		break;
	case PNCHP:
	case PCHP:
	case CM24HP:
		sprintf_s(buf, RECMAX, "IN;");
		plotoutstring(buf);
		break;
	case CJHP:
	case CJ2HP:
		sprintf_s(buf, RECMAX, "%cE", ESC);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "%c%c0B", ESC, PAT);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "BP5,1;");
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "IN;");
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "PU0,0;");
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "!ST1;");
		plotoutstring(buf);
		break;
	case DYNA:
		sprintf_s(buf, RECMAX,"F2,%c",ETX);
		plotoutstring(buf);
		break;
	default:
		break;
	}
}

void plotdrawend()
{
	char	buf[RECMAX];

	switch(plotkind){
	case NCAMADA:
		// G50;\n
		plotoutstring(plotpostbuf);
		sprintf_s(buf, RECMAX, "%c\n", PAT);
		plotoutstring(buf);
		break;
	case LASERPRO:
		sprintf_s(buf, RECMAX, ";PG;");
		plotoutstring(buf);
		break;
	case RP1T:
	case RP93T:
	case RP93N:
	case RP95T:
	case RP9T:
	case RP6T:
	case RP3L:
	case RP3LL:
	case RP35F:
		sprintf_s(buf, RECMAX, "F%c", ETX);
		plotoutstring(buf);
		break;
	case CH37:
		sprintf_s(buf, RECMAX, "J0%c< 0,0%cJ 1%cJ0%c", ETX, ETX, ETX, ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "F%c", ETX);
		plotoutstring(buf);
		break;
	case RP395T:
		sprintf_s(buf, RECMAX, "J0%c< 0,0%cJ 1%cJ0%c", ETX, ETX, ETX, ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "F%c", ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "%c%c0A", ESC, PAT);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "%cE", ESC);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "%c", FF);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "%c%c-12345X", ESC, PAT);
		plotoutstring(buf);
		break;
	case TM13:
		sprintf_s(buf, RECMAX, "F0%c", ETX);
		plotoutstring(buf);
		break;
	case HPGL:
	case PNCHP:
	case PCHP:
		sprintf_s(buf, RECMAX, "AF;");
		plotoutstring(buf);
		break;
	case CMHP:
		sprintf_s(buf, RECMAX, "PU;EC0;");
		plotoutstring(buf);
		break;
	case CM24HP:
		break;
	case CJHP:
		sprintf_s(buf, RECMAX, "!PG0;");
		plotoutstring(buf);
		break;
	case CJ2HP:
		sprintf_s(buf, RECMAX, "!PG0;");
		plotoutstring(buf);
		break;
	case DYNA:
		sprintf_s(buf, RECMAX, "F0,%c",ETX);
		plotoutstring(buf);
		break;
	default:
		break;
	}
}

void plotpaintinit()
{
	char	buf[RECMAX];
	char	txt1[10], txt2[10];
	int		num0, num1, num2;

	plotlinefirst = ON;
	switch(plotkind){
#ifdef	SOA
	case RP93T:
	case RP93N:
		num0 = (int)plotxtime;
		num1 = (int)(plotstartspace/10.0);
		num2 = (int)(plotendspace/10.0);
		sprintf_s(buf, RECMAX, "< %d%02d,-4%02d %c",
			num0, num1, num2, ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J1%c", ETX);
		plotoutstring(buf);
		break;
	case RP95T:
		num1 = (int)(plotstartspace/10.0);
		num2 = (int)(plotendspace/10.0);
		sprintf_s(buf, RECMAX, "< 0%02d,6%02d %c", num1, num2, ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J1%c", ETX);
		plotoutstring(buf);
		break;
#endif
	case RP9T:
	case RP6T:
		num0 = (int)plotxtime;
		num1 = (int)(plotstartspace/10.0);
		num2 = (int)(plotendspace/10.0);
		sprintf_s(buf, RECMAX, "< %d%02d,4%02d %c",
			num0, num1, num2, ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J1%c", ETX);
		plotoutstring(buf);
		break;
	case RP1T:
		num1 = (int)plotstartspace;
		sprintf_s(buf, RECMAX, "< %02d,0 %c", num1, ETX);
		plotoutstring(buf);
		break;
	case RP3L:
		sprintf_s(buf, RECMAX, "< 60,4 %c", ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J1%c", ETX);
		plotoutstring(buf);
		break;
	case RP3LL:
		sprintf_s(buf, RECMAX, "< 85,4 %c", ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J1%c", ETX);
		plotoutstring(buf);
		break;
	case RP35F:
		num1 = (int)(plotstartspace/10.0);
		sprintf_s(buf, RECMAX, "< 8%01d,4 %c", num1, ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J1%c", ETX);
		plotoutstring(buf);
		break;
	case RP395T:
	case CH37:
		sprintf_s(buf, RECMAX, "< 0,600%c", ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J 1%c", ETX);
		plotoutstring(buf);
		break;
	case TM13:
		plotpaintmode = ON;
		sprintf_s(buf, RECMAX, "F2%c", ETX);
		plotoutstring(buf);
		break;
	case MAXP:
		sprintf_s(buf, RECMAX, "< 0, 0 %c", ETX);
		plotoutstring(buf);
		break;
	case MAXC:
		num1 = (int)(plotstartspace);
		num2 = (int)(plotendspace);
		sprintf_s(buf, RECMAX, "<%d,%d%c", num1, num2, ETX);
		plotoutstring(buf);
		break;
	case RPDL:
		sprintf_s(buf, RECMAX, "%c3", ESC);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "IN;");
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "UN0;");
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "EM1,1;");
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "PM2,0;");
		plotoutstring(buf);
		break;
	case LIPS:
		plotpaintmode = ON;
		plotpaintfirst = ON;
		sprintf_s(buf, RECMAX, "%c%%@%cP31;300;1J%c\\%c<",
				ESC, ESC, ESC, ESC);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "%c[0p", ESC);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "%c[0&}", ESC);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "#%c", IS2);
		plotoutstring(buf);
		num1 = -2; num2 = 1;
		plotlipsnumtostrg(num1, txt1);
		plotlipsnumtostrg(num2, txt2);
		sprintf_s(buf, RECMAX, "!0%s%s%c", txt1, txt2, IS2);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "$%c", IS2);
		plotoutstring(buf);
		break;
	default:
		break;
	}
}

void plotpenoutinit(int penno)
{
#ifdef	SOA
	return;
#else if 
	int		plotpenno;
	char	buf[RECMAX];
	int		num0, num1, num2;

	if(penno < PLOTPENMAX){
		plotpenno = plotpentbl[penno];
	}
	else{
		plotpenno = 1;
	}
	switch(plotkind){
	case RP93T:
	case RP93N:
		num0 = (int)plotxtime;
		num1 = (int)(plotstartspace/10.0);
		num2 = (int)(plotendspace/10.0);
		sprintf_s(buf, RECMAX, "< %d%02d,-4%02d %c",
			num0, num1, num2, ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J%d%c", plotpenno, ETX);
		plotoutstring(buf);
		break;
	case RP95T:
		num1 = (int)(plotstartspace/10.0);
		num2 = (int)(plotendspace/10.0);
		sprintf_s(buf, RECMAX, "< 0%02d,6%02d %c", num1, num2, ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J%d%c", plotpenno, ETX);
		plotoutstring(buf);
		break;
	/*
	case CH37:
		sprintf_s(buf, RECMAX, "< 0,600 %c", ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J%d%c", plotpenno, ETX);
		plotoutstring(buf);
		break;
	*/
	}
#endif
}

void plotpaintend()
{
	char	buf[RECMAX];

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
	switch(plotkind){
#ifdef	SOA
	case RP93T:
	case RP93N:
	case RP95T:
#endif
	case RP9T:
	case RP6T:
	case RP3L:
	case RP3LL:
	case RP35F:
	case RP395T:
	case CH37:
		sprintf_s(buf, RECMAX, "J0%c", ETX);
		plotoutstring(buf);
		break;
	case TM13:
		plotpaintmode = OFF;
		break;
	case LIPS:
		plotpaintmode = OFF;
		break;
	case DYNA:
		if(plotdynamode == ON){
			sprintf_s(buf, RECMAX, "%c43,0,0,%c", PAT, ETX);
		}
		else{
			sprintf_s(buf, RECMAX, "%c42,0,0,%c", PAT, ETX);
		}
		plotoutstring(buf);
		break;
	default:
		break;
	}
}

void plotpenoutend()
{
#ifdef SOA
	return;
#else if
	char	buf[RECMAX];

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
	switch(plotkind){
	case RP93T:
	case RP93N:
	case RP95T:
	//case CH37:
		sprintf_s(buf, RECMAX, "J0%c", ETX);
		plotoutstring(buf);
		break;
	}
#endif
}

void plotplotinit()
{
	char	buf[RECMAX];
	int		num0, num1, num2;

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
	switch(plotkind){
	case RP93T:
	case RP93N:
	case RP95T:
		num0 = (int)plotxtime;
		num1 = (int)(plotstartspace/10.0);
		num2 = (int)(plotendspace/10.0);
		sprintf_s(buf, RECMAX, "< %d%02d,-3%02d %c",
			num0, num1, num2, ETX);
 		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J1%c", ETX);
		plotoutstring(buf);
		break;
	case RP9T:
	case RP6T:
		num0 = (int)plotxtime;
		num1 = (int)(plotstartspace/10.0);
		num2 = (int)(plotendspace/10.0);
		sprintf_s(buf, RECMAX, "< %d%02d,3%02d %c",
			num0, num1, num2, ETX);
 		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J1%c", ETX);
		plotoutstring(buf);
		break;
	case RP395T:
	case CH37:
		sprintf_s(buf, RECMAX, "< 0,300%c", ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J 1%c", ETX);
		plotoutstring(buf);
		break;
	case RP3L:
		sprintf_s(buf, RECMAX, "< 60,3 %c", ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J1%c", ETX);
		plotoutstring(buf);
		break;
	case RP3LL:
		sprintf_s(buf, RECMAX, "< 85,3 %c", ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J1%c", ETX);
		plotoutstring(buf);
		break;
	case RP35F:
		num1 = (int)(plotstartspace/10.0);
		sprintf_s(buf, RECMAX, "< 8%01d,3 %c", num1, ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "J1%c", ETX);
		plotoutstring(buf);
		break;
	case TM13:
		plotpaintmode = OFF;
		break;
	case RPDL:
		sprintf_s(buf, RECMAX, "IN;");
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "UN0;");
		plotoutstring(buf);
		break;
	case LIPS:
		plotpaintmode = OFF;
		break;
	default:
		break;
	}
}

void plotplotend()
{
	char	buf[RECMAX];

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
	switch(plotkind){
	case RP93T:
	case RP93N:
	case RP95T:
	case RP9T:
	case RP6T:
	case RP1T:
	case RP395T:
	case CH37:
	case RP3L:
	case RP3LL:
	case RP35F:
		sprintf_s(buf, RECMAX, "J0%c", ETX);
		plotoutstring(buf);
		break;
	case RPDL:
		sprintf_s(buf, RECMAX, "PG1;");
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "%c4", ESC);
		plotoutstring(buf);
		break;
	case LIPS:
		sprintf_s(buf, RECMAX, "%%%c", IS2);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "}p%c", IS2);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "%c[11h", ESC);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "%c%cP0J%c\\", FF, ESC, ESC);
		plotoutstring(buf);
		break;
	default:
		break;
	}
}

void plotdynapaintmodeset(int mode)
{
	plotdynamode = mode;
}

void plotdynaclrnoset(int clrno)
{
	char	buf[RECMAX];

	switch(plotkind){
	case DYNA:
		sprintf_s(buf, RECMAX, "J%d,%c", clrno, ETX);
		plotoutstring(buf);
		break;
	default:
		break;
	}
}

void plotdynafillstart()
{
	char	buf[RECMAX];

	switch(plotkind){
	case DYNA:
		sprintf_s(buf, RECMAX, "%c30,%c", PAT, ETX);
		plotoutstring(buf);
		break;
	default:
		break;
	}
}

void plotdynafillend()
{
	char	buf[RECMAX];

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
	switch(plotkind){
	case DYNA:
		sprintf_s(buf, RECMAX, "%c32,%c", PAT, ETX);
		plotoutstring(buf);
		break;
	default:
		break;
	}
}

void plotdynarulestart()
{
}

void plotdynaruleend()
{
	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
}

void plotdynathickout(DBL thick)
{
	char	buf[RECMAX];

	switch(plotkind){
	case DYNA:
		sprintf_s(buf, RECMAX, "LA,0%c", ETX);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "LW%1.0lf%c", thick, ETX);
		plotoutstring(buf);
		break;
	default:
		break;
	}
}

void plotoutblockstart()
{
	char	buf[RECMAX];

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
	switch(plotkind){
	case TM13:
		sprintf_s(buf, RECMAX, "%c30, %c", PAT, ETX);
		plotoutstring(buf);
		break;
	case LIPS:
		if(plotpaintmode == ON){
			plotblockflag = ON;
			plotrejionflag = OFF;
		}
		break;
	default:
		break;
	}
}

void plotoutblockend()
{
	char	buf[RECMAX];

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
	switch(plotkind){
	case TM13:
		if(plotpaintmode == ON){
			sprintf_s(buf, RECMAX, "%c32, %c", PAT, ETX);
			plotoutstring(buf);
			sprintf_s(buf, RECMAX, "%c43,1,900, %c", PAT, ETX);
			plotoutstring(buf);
		}
		break;
	case LIPS:
		if(plotpaintmode == ON
		&& plotblockflag == OFF){
			sprintf_s(buf, RECMAX, "%c", IS2);
			plotoutstring(buf);
		}
		break;
	default:
		break;
	}
}

void plotoutrejionstart()
{
	char	buf[RECMAX];

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
	plotrejionfirst = ON;
	switch(plotkind){
	case TM13:
		if(plotpaintmode == ON){
			sprintf_s(buf, RECMAX, "%c31, %c", PAT, ETX);
			plotoutstring(buf);
		}
		break;
	case LIPS:
		if(plotpaintmode == ON){
			plotrejionflag = ON;
		}
		break;
	case RPDL:
		plotrejionflag = ON;
		plgncnt = 0;
		break;
	case DYNA:
		sprintf_s(buf, RECMAX, "%c31,%c", PAT, ETX);
		plotoutstring(buf);
		break;
	default:
		break;
	}
}

void plotoutrejionend()
{
	char	buf[RECMAX];

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
	switch(plotkind){
	case NCAMADA:
		if(plotrejionfirst == OFF){
			sprintf_s(buf, RECMAX, "M104;\n");
			plotoutstring(buf);
		}
		break;
	case RPDL:
		sprintf_s(buf, RECMAX, ";");
		plotoutstring(buf);
		plgncnt = 0;
		break;
	case LIPS:
		if(plotpaintmode == ON
		&& plotblockflag == OFF
		&& plotrejionflag == OFF){
			sprintf_s(buf, RECMAX, "%c", IS1);
			plotoutstring(buf);
		}
		break;
	}
}

void plotoutplgnend()
{
	char	buf[RECMAX];

	switch(plotkind){
	case LASERPRO:
		//sprintf_s(buf, RECMAX, ";");
		//plotoutstring(buf);
		break;
	case RP1T:
	case RP93T:
	case RP93N:
	case RP95T:
	case RP9T:
	case RP6T:
	//case RP395T:
	//case CH37:
	case RP3L:
	case RP3LL:
	case RP35F:
	case TM13:
	case MIMK:
	case GRPH:
	case MAXP:
	case MAXC:
		sprintf_s(buf, RECMAX, "%c", ETX);
		plotoutstring(buf);
		break;
	case RLND:
		sprintf_s(buf, RECMAX, "\n");
		plotoutstring(buf);
		break;
	case HPGL:
	case PNCHP:
	case PCHP:
	case CMHP:
	case CM24HP:
	case CJHP:
	case CJ2HP:
		sprintf_s(buf, RECMAX, ";");
		plotoutstring(buf);
		break;
	default:
		buf[0] = '\0';
		break;
	}
}

void plotmovekind(int kind, DBL movex, DBL movey)
{
	char	buf[RECMAX];

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
	switch(plotkind){
	case NCAMADA:
		rejionsx = movex;	// stan 94.12.07
		rejionsy = movey;	// stan 94.12.07
		if(kind == 1){
			sprintf_s(buf, RECMAX, "G00 G41 X%1.3lf Y%1.3lf;\n", movex, movey);
		}
		else{
			sprintf_s(buf, RECMAX, "G00 G42 X%1.3lf Y%1.3lf;\n", movex, movey);
		}
		plotoutstring(buf);
		plotcrtx = movex; plotcrty = movey;
		break;
	default:
		plotmove(movex, movey);
		break;
	}
}

void plotdrawst()
{
	char	buf[RECMAX];

	switch(plotkind){
	case CJ2HP:
		sprintf_s(buf, RECMAX, "!ST1;");
		plotoutstring(buf);
		break;
	}
}

void plotmove(DBL movex, DBL movey)
{
	char	buf[RECMAX];

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}

	rejionsx = movex;	// stan 94.12.07
	rejionsy = movey;	// stan 94.12.07

	switch(plotkind){
	case NCAMADA:
		sprintf_s(buf, RECMAX, "G00 G40 X%1.3lf Y%1.3lf;\n", movex, movey);
		plotoutstring(buf);
		plotcrtx = movex; plotcrty = movey;
		break;
	case LASERPRO:
		sprintf_s(buf, RECMAX, "PU%1.0lf,%1.0lf", movex-plotcrtx, movey-plotcrty);
		plotoutstring(buf);
		plotcrtx = movex; plotcrty = movey;
		break;
	case RP1T:
	case RP93T:
	case RP93N:
	case RP95T:
	case RP9T:
	case RP6T:
	case RP395T:
	case CH37:
	case RP3L:
	case RP3LL:
	case RP35F:
	case MAXP:
	case MAXC:
		plotcrtx = floor(movex * 10.0+0.5) / 10.0;
		plotcrty = floor(movey * 10.0+0.5) / 10.0;
		sprintf_s(buf, RECMAX, "M%1.1f,%1.1f%c", movex, movey, ETX);
		plotoutstring(buf);
		break;
	case TM13:
	case GRPH:
	case DYNA:
		sprintf_s(buf, RECMAX, "M%1.1f,%1.1f%c", movex, movey, ETX);
		plotoutstring(buf);
		plotcrtx = movex; plotcrty = movey;
		break;
	case MIMK:
		sprintf_s(buf, RECMAX, "M%1.1f,%1.1f%c", movex, movey, ETX);
		plotoutstring(buf);
		plotcrtx = movex; plotcrty = movey;
		break;
	case RLND:
		sprintf_s(buf, RECMAX, "M%1.0f,%1.0f\n", movex, movey);
		plotoutstring(buf);
		plotcrtx = movex; plotcrty = movey;
		break;
	case HPGL:
	case PNCHP:
	case PCHP:
	case CM24HP:
	case CJHP:
	case CJ2HP:
		sprintf_s(buf, RECMAX, "PU;PA%1.0f,%1.0f;", movex, movey);
		plotoutstring(buf);
		plotcrtx = movex; plotcrty = movey;
		break;
	case CMHP:
		sprintf_s(buf, RECMAX, "PU%1.0f,%1.0f;", movex, movey);
		plotoutstring(buf);
		plotcrtx = movex; plotcrty = movey;
		break;
	case MEHP:
	case MHP:
		sprintf_s(buf, RECMAX, "PU;PA%1.0f,%1.0f;", movex, movey);
		plotoutstring(buf);
		plotcrtx = movex; plotcrty = movey;
		break;
	case RPDL:
		sprintf_s(buf, RECMAX, "MA%1.0f,%1.0f;", movex, movey);
		plotoutstring(buf);
		plgncnt = 0;
		plotcrtx = movex; plotcrty = movey;
		break;
	case LIPS:
		plotcrtx = movex; plotcrty = movey;
		break;
	default:
		break;
	}
}

void plotrejionlastjoin()
{
	char	buf[RECMAX];
	DBL	x, y;

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}

	x = rejionsx; y = rejionsy;
	switch(plotkind){
	case RP1T:
	case RP93T:
	case RP93N:
	case RP95T:
	case RP9T:
	case RP6T:
	case RP395T:
	case CH37:
	case RP3L:
	case RP3LL:
	case RP35F:
	case MAXP:
	case MAXC:
		sprintf_s(buf, RECMAX, "D%1.1f,%1.1f %c",	x, y, ETX);
		plotoutstring(buf);
		plotcrtx = x; plotcrty = y;
		break;
	case TM13:
	case GRPH:
	case DYNA:
		sprintf_s(buf, RECMAX, "D%1.1f,%1.1f %c",	x, y, ETX);
		plotoutstring(buf);
		plotcrtx = x; plotcrty = y;
		break;
	case MEHP:
	case MHP:
		sprintf_s(buf, RECMAX, "PD;PA%1.0f,%1.0f;",x, y);
		plotoutstring(buf);
		plotcrtx = x; plotcrty = y;
		break;
	default :
		break;
	}
}

void plotdrawline(DBL x1, DBL y1, DBL x2, DBL y2)
{
	char	buf[RECMAX];
	DBL	dx, dy;

	if(!(EQUALPLT(plotcrtx, x1) && EQUALPLT(plotcrty, y1))){
		plotlinefirst = ON;
		plotoutplgnend();
		plotmove(x1, y1);
	}

	switch(plotkind){
	case NCAMADA:
		if(plotrejionfirst == ON){
			sprintf_s(buf, RECMAX, "M103;\n");
			plotoutstring(buf);
			plotrejionfirst = OFF;
		}
		sprintf_s(buf, RECMAX, "G01 X%1.3lf Y%1.3lf;\n", x2, y2);
		plotoutstring(buf);
		plotcrtx = x2; plotcrty = y2;
		break;
	case LASERPRO:
		if(plotlinefirst == ON){
			sprintf_s(buf, RECMAX, "PD%1.0lf,%1.0lf", x2-plotcrtx, x2-plotcrty);
			plotoutstring(buf);
			plotlinefirst = OFF;
		}
		else{
			sprintf_s(buf, RECMAX, ",%1.0lf,%1.0lf", x2-plotcrtx, x2-plotcrty);
			plotoutstring(buf);
		}
		plotcrtx = x2;
		plotcrty = y2;
		break;
	case RP1T:
	case RP93T:
	case RP93N:
	case RP95T:
	case RP9T:
	case RP6T:
	case RP3L:
	case RP3LL:
	case RP35F:
	case MAXP:
	case MAXC:
		dx = floor((x2 - plotcrtx)*10.0+0.5)/10.0;
		dy = floor((y2 - plotcrty)*10.0+0.5)/10.0;
		plotcrtx = plotcrtx + dx;
		plotcrty = plotcrty + dy;
		if(plotlinefirst == ON){
			plotlinefirst = OFF;
			sprintf_s(buf, RECMAX, "E%1.1f,%1.1f",	dx, dy);
		}
		else{
			sprintf_s(buf, RECMAX, ",%1.1f,%1.1f",	dx, dy);
		}
		plotoutstring(buf);
		break;
	case RP395T:
	case CH37:
	case TM13:
	case GRPH:
	case DYNA:
		sprintf_s(buf, RECMAX, "D%1.1f,%1.1f%c", x2, y2, ETX);
		plotoutstring(buf);
		plotcrtx = x2; plotcrty = y2;
		break;
	case MIMK:
		sprintf_s(buf, RECMAX, "D%1.1f,%1.1f,%1.1f,%1.1f%c", x1, y1, x2, y2, ETX);
		plotoutstring(buf);
		plotcrtx = x2; plotcrty = y2;
		break;
	case RLND:
		sprintf_s(buf, RECMAX, "D%1.0f,%1.0f,%1.0f,%1.0f\n", x1, y1, x2, y2);
		plotoutstring(buf);
		plotcrtx = x2; plotcrty = y2;
		break;
	case HPGL:
	case PNCHP:
	case PCHP:
	case CM24HP:
	case CJHP:
	case CJ2HP:
		if(plotlinefirst == ON){
			plotlinefirst = OFF;
			sprintf_s(buf, RECMAX, "PD;PA%1.0f,%1.0f,%1.0f,%1.0f",
				x1, y1, x2, y2);
		}
		else{
			sprintf_s(buf, RECMAX, ",%1.0f,%1.0f,%1.0f,%1.0f", x1, y1, x2, y2);
		}
		plotoutstring(buf);
		plotcrtx = x2; plotcrty = y2;
		break;
	case CMHP:
		if(plotlinefirst == ON){
			plotlinefirst = OFF;
			sprintf_s(buf, RECMAX, "PD%1.0f,%1.0f", x2, y2);
		}
		else{
			sprintf_s(buf, RECMAX, ",%1.0f,%1.0f", x2, y2);
		}
		plotoutstring(buf);
		plotcrtx = x2; plotcrty = y2;
		break;
	case MEHP:
	case MHP:
		sprintf_s(buf, RECMAX, "PD;PA%1.0f,%1.0f;PA%1.0f,%1.0f;",
			x1, y1, x2, y2);
		plotoutstring(buf);
		plotcrtx = x2; plotcrty = y2;
		break;
	case RPDL:
		if(plotrejionflag == ON){
			plotrejionflag = OFF;
			sprintf_s(buf, RECMAX, "GA%1.0f,%1.0f", x1, y1);
			plotoutstring(buf);
		}
		sprintf_s(buf, RECMAX, ",%1.0f,%1.0f", x2, y2);
		plotoutstring(buf);
		plgncnt++;
		if(plgncnt > RPDLPOINTMAX){
			sprintf_s(buf, RECMAX, ";GA%1.0f,%1.0f,%1.0f,%1.0f",
					plotcrtx, plotcrty, x2, y2);
			plotoutstring(buf);
			plgncnt = 0;
		}
		plotcrtx = x2; plotcrty = y2;
		break;
	case LIPS:
		if(plotpaintmode == ON){
			if(plotpaintfirst == ON){
				plotpaintfirst = OFF;
				sprintf_s(buf, RECMAX, "I11%c", IS2);
				plotoutstring(buf);
			}
			if(plotblockflag == ON){
				plotblockflag = OFF;
				sprintf_s(buf, RECMAX, "}2");
				plotoutstring(buf);
			}
			if(plotrejionflag == ON){
				plotrejionflag = OFF;
				dx = plotcrtx; dy = plotcrty;
				plotlipsnumtostrg(dx, buf);
				plotoutstring(buf);
				plotlipsnumtostrg(dy, buf);
				plotoutstring(buf);
			}
			dx = x2 - plotcrtx; dy = y2 - plotcrty;
			plotlipsnumtostrg(dx, buf);
			plotoutstring(buf);
			plotlipsnumtostrg(dy, buf);
			plotoutstring(buf);
			plotcrtx = x2; plotcrty = y2;
		}
		else{
			dx = plotcrtx; dy = plotcrty;
			sprintf_s(buf, RECMAX, "1");
			plotoutstring(buf);
			plotlipsnumtostrg(dx, buf);
			plotoutstring(buf);
			plotlipsnumtostrg(dy, buf);
			plotoutstring(buf);
			dx = x2 - plotcrtx; dy = y2 - plotcrty;
			plotlipsnumtostrg(dx, buf);
			plotoutstring(buf);
			plotlipsnumtostrg(dy, buf);
			plotoutstring(buf);
			sprintf_s(buf, RECMAX, "%c", IS2);
			plotoutstring(buf);
			plotcrtx = x2; plotcrty = y2;
		}
		break;
	default:
		break;
	}
}

void plotdrawenko(DBL x0, DBL y0, DBL r, DBL rag1, DBL rag2, DBL sx, DBL sy, DBL ex, DBL ey)
{
	DBL	subrag;
	char	buf[RECMAX];

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
	if(!(plotcrtx == sx && plotcrty == sy)){
		plotmove(sx, sy);
	}
	switch(plotkind){
	case RP1T:
	case RP93T:
	case RP93N:
	case RP95T:
	case RP9T:
	case RP6T:
	case RP395T:
	case CH37:
	case RP3L:
	case RP3LL:
	case RP35F:
	case MAXP:
	case MAXC:
		plotcrtx = floor(ex*10.0+0.5) / 10.0;
		plotcrty = floor(ey*10.0+0.5) / 10.0;
		sprintf_s(buf, RECMAX,"W%1.3f,%1.3f,%1.3f,%1.3f,%1.3f,%1.3f, 0 %c",
			x0, y0, r, r, rag1, rag2, ETX);
		plotoutstring(buf);
		break;
	case TM13:
	case GRPH:
	case DYNA:
		sprintf_s(buf, RECMAX,"W%1.3f,%1.3f,%1.3f,%1.3f,%1.3f,%1.3f, 0 %c",
			x0, y0, r, r, rag1, rag2, ETX);
		plotoutstring(buf);
		ex = floor(ex+0.5);
		ey = floor(ey+0.5);
		sprintf_s(buf, RECMAX, "D%1.1f,%1.1f%c", ex, ey, ETX);
		plotoutstring(buf);
		plotcrtx = ex;	plotcrty = ey;
		break;
	case MIMK:
		sprintf_s(buf, RECMAX,"W%1.1f,%1.1f,%1.1f,%1.1f,%1.1f,%1.1f, 0 %c",
			x0, y0, r, r, rag1, rag2, ETX);
		plotoutstring(buf);
		plotcrtx = ex;	plotcrty = ey;
		break;
	case RLND:
		sprintf_s(buf, RECMAX, "C%1.5f,%1.5f,%1.5f,%1.5f,%1.5f\n",
			x0, y0, r, rag1, rag2);
		plotoutstring(buf);
		plotcrtx = ex;	plotcrty = ey;
		break;
	case HPGL:
	case PNCHP:
	case PCHP:
	case CMHP:
	case CM24HP:
	case CJHP:
	case CJ2HP:
		subrag = (rag2 - rag1) / 10.0;
		sprintf_s(buf, RECMAX,"PD;AA%1.5f,%1.5f,%1.5f;",
			x0, y0, subrag);
		plotoutstring(buf);
		sprintf_s(buf, RECMAX, "PA%1.5f,%1.5f;", ex, ey);
		plotoutstring(buf);
		plotcrtx = ex; plotcrty = ey;
		break;
	case MEHP:
	case MHP:
		subrag = (rag2 - rag1) / 10.0;
		sprintf_s(buf, RECMAX,"PD;AA%1.5f,%1.5f,%1.5f;",
			x0, y0, subrag);
		plotoutstring(buf);
		plotcrtx = ex;	plotcrty = ey;
		break;
	default:
		break;
	}
}

void plotdraw3penko(DBL x1, DBL y1, DBL x12, DBL y12, DBL x2, DBL y2)
{
	char	buf[RECMAX];

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
	if(!(plotcrtx == x1 && plotcrty == y1)){
		plotmove(x1, y1);
	}
	switch(plotkind){
	case TM13:
	case GRPH:
	case DYNA:
	    sprintf_s(buf, RECMAX,"WP 1,%1.1f,%1.1f,%1.1f,%1.1f,%1.1f,%1.1f %c",
			x1, y1, x12, y12, x2, y2, ETX);
		plotoutstring(buf);
		plotcrtx = x2; plotcrty = y2;
		break;
	case RP1T:
	case RP93T:
	case RP93N:
	case RP95T:
	case RP9T:
	case RP6T:
	case RP395T:
	case CH37:
	case RP3L:
	case RP3LL:
	case RP35F:
	case MAXP:
	case MAXC:
		plotcrtx = floor(x2*10.0+0.5) / 10.0;
		plotcrty = floor(y2*10.0+0.5) / 10.0;
	    sprintf_s(buf, RECMAX,"c 1,%1.1f,%1.1f,%1.1f,%1.1f,%1.1f,%1.1f %c",
			x1, y1, x12, y12, x2, y2, ETX);
		plotoutstring(buf);
		break;
	case MEHP:
	case MHP:
	    sprintf_s(buf, RECMAX,"PD;ZC %1.1f,%1.1f,%1.1f,%1.1f;",
			x12, y12, x2, y2);
		plotoutstring(buf);
		plotcrtx = x2; plotcrty = y2;
		break;
	default:
		break;
	}
}

void plotdraw3paenko(int round, DBL x1, DBL y1, DBL x3, DBL y3, DBL r, DBL x0, DBL y0)
{
	char	buf[RECMAX];

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
	if(!(plotcrtx == x1 && plotcrty == y1)){
		plotmove(x1, y1);
	}
	switch(plotkind){
	case NCAMADA:
		if(plotrejionfirst == ON){
			sprintf_s(buf, RECMAX, "M103;\n");
			plotoutstring(buf);
			plotrejionfirst = OFF;
		}
		if(round == 1){
			// 2001.05.30 修正
			sprintf_s(buf, RECMAX, "G03 X%1.3lf Y%1.3lf I%1.3lf J%1.3lf;\n",
				x3, y3, (plotcrtx-x0), (plotcrty-y0));
			//sprintf_s(buf, RECMAX, "G02 X%1.3lf Y%1.3lf I%1.3lf J%1.3lf;\n",
			//	x3, y3, (plotcrtx-x0), (plotcrty-y0));
			//sprintf_s(buf, RECMAX, "G02 X%1.3lf Y%1.3lf R%1.3lf;\n", x3, y3, r);
		}
		else{
			// 2001.05.30 修正
			sprintf_s(buf, RECMAX, "G02 X%1.3lf Y%1.3lf I%1.3lf J%1.3lf;\n",
				x3, y3, (plotcrtx-x0), (plotcrty-y0));
			//sprintf_s(buf, RECMAX, "G03 X%1.3lf Y%1.3lf I%1.3lf J%1.3lf;\n",
			//	x3, y3, (plotcrtx-x0), (plotcrty-y0));
			//sprintf_s(buf, RECMAX, "G03 X%1.3lf Y%1.3lf R%1.3lf;\n", x3, y3, r);
		}
		plotoutstring(buf);
		plotcrtx = x3; plotcrty = y3;
		break;
	default:
		break;
	}
}

void plotdraw3pmenko(int round, DBL x1, DBL y1, DBL x2, DBL y2, DBL x0, DBL y0)
{
	char	buf[RECMAX];

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
	if(!(plotcrtx == x1 && plotcrty == y1)){
		plotmove(x1, y1);
	}
	switch(plotkind){
	case MIMK:
	    sprintf_s(buf, RECMAX, "* %d,%1.1f,%1.1f,%1.1f,%1.1f,%1.1f,%1.1f %c",
			round, x1, y1, x2, y2, x0, y0, ETX);
		plotoutstring(buf);
		plotcrtx = x2; plotcrty = y2;
		break;
	default:
		break;
	}
}

void plotendhome()
{
	char	buf[RECMAX];
	DBL	x, y;

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
	if(plotkind == MEHP){
		sprintf_s(buf, RECMAX,"PU;!MC0;");
		plotoutstring(buf);
	}
	switch(plotkind){
	case MAXP:
	case MAXC:
	case GRPH:
	case MIMK:
	case RLND:
	case HPGL:
	case PNCHP:
	case PCHP:
	case CMHP:
	case CM24HP:
	case CJHP:
	case CJ2HP:
	case MEHP:
	case MHP:
		x = plotxmm - 10;
		y = plotymm - 10;
		x -= plotcntrxmm;
		y -= plotcntrymm;
		x /= plotstep;
		y /= plotstep;
		plotmove(x, y);
		break;
	}
}

void plotpenup(DBL x, DBL y)
{
	char	buf[RECMAX];

	if(plotlinefirst == OFF){
		plotlinefirst = ON;
		plotoutplgnend();
	}
	if(plotkind == MEHP){
		sprintf_s(buf, RECMAX,"PU;!MC0;");
		plotoutstring(buf);
	}
	switch(plotkind){
	case MAXP:
	case MAXC:
	case GRPH:
	case MIMK:
	case RLND:
	case HPGL:
	case PNCHP:
	case PCHP:
	case CMHP:
	case CM24HP:
	case CJHP:
	case CJ2HP:
	case MEHP:
	case MHP:
		plotmove(x, y);
		break;
	}
}

void plotlipsnumtostrg(DBL dnum, char txt[])
{
	int		num;
	char	tt[10];
	int		mask, i, byte;

	num = (int)dnum;
	if(0 <= num && num < 16){
		txt[0] = 0x30 | (num & 0x0f);
		txt[1] = '\0';
	}
	else if(0 > num && num > -16){
		num *= -1;
		txt[0] = 0x20 | (num & 0x0f);
		txt[1] = '\0';
	}
	else{
		if(0 < num){
			tt[0] = 0x30;
		}
		else{
			num *= -1;
			tt[0] = 0x20;
		}
		mask = 0x01;
		for(i = 0; i < 4; i++){
			if(num & 0x01){
				tt[0] |= mask;
			}
			mask <<= 1;
			num >>= 1;
		}
		byte = 1;
		tt[byte] = 0x40;
		mask = 0x01;
		while(1){
			if(num & 0x01){
				tt[byte] |= mask;
			}
			mask <<= 1;
			num >>= 1;
			if(num == 0){
				break;
			}
			if(mask == 0x40){
				byte++;
				tt[byte] = 0x40;
				mask = 0x01;
			}
		}
		byte++;
		for(i = 0; i < byte; i++){
			txt[i] = tt[byte - 1 - i];
		}
		txt[i] = '\0';
	}
}

void plotoutstring(LPSTR  buf)
{
//#ifdef NDEBUG
	outlibPutString(buf);
	//outputstring(buf);
//#endif
}
