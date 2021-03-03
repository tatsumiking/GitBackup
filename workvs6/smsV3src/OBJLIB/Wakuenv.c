/*
	Easy Scanner システム共通ソース	1992.12.01	by stan
base		wakuenv.c	スクリーン関係関数
	Copyright 株）テオン
*/

#include	<windows.h>
#include	<stdio.h>
#include	<math.h>
#include	<direct.h>
#include	<io.h>
#include	"..\stdef.h"
#include	"dlldef.h"

#include	"senvlib.fnc"
#include	"scrnlib.fnc"
#include	"baselib.fnc"
#include	"pout.fnc"
#include	"wakuenv.fnc"

char	grupname[RECMAX];
char	papername[RECMAX];
DBL		paperxsize;
DBL		paperysize;
DBL		papersx;
DBL		papersy;
DBL		paperwidth;
DBL		paperhight;
int		flag1;
int		flag2;
char	kaisyaname[RECMAX];
char	paperpath[RECMAX] = "";
char	dspdataname[RECMAX];
char	outdataname[RECMAX];
int		paperslctidx = 0;

DllExport void wakuenvinit(LPSTR envfile)
{
	//papername[0] = '\0';
	//paperslctidx = 0;
}

DllExport void wakuenvsetkaisyaname(LPSTR name)
{
	strcpy(kaisyaname, name);
}

DllExport void wakuenvgetpaperelement()
{
	char	wakpath[FNAMEMAX];
	char	paperlistfname[FNAMEMAX];
	char	buf[RECMAX];
	char	*sp;
	char	pname[HRECMAX];
	char	numstr[HRECMAX];
	FILE	*fp;

	scrngetmmsize(&paperxsize, &paperysize);
	papersx = 0;
	papersy = 0;
	paperwidth = paperxsize;
	paperhight = paperysize;
	flag1 = 0;
	flag2 = 0;
	/*
	if(papername[0] == '\0'){
		strcpy(papername, "defpaper");
		strcpy(paperpath, papername);
		wakuenvsetpaperelement();
	}
	*/
	senvgetwakpath(wakpath);
	sprintf(paperlistfname, "%s\\paperlst.lst", wakpath);
	if((fp = fopen(paperlistfname, "r")) == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, pname);
		if(strcmp(papername, pname) == 0){
			sp = basecsvstrdivcpy(sp, numstr);
			sscanf(numstr, "%lf", &paperxsize);
			sp = basecsvstrdivcpy(sp, numstr);
			sscanf(numstr, "%lf", &paperysize);
			sp = basecsvstrdivcpy(sp, numstr);
			sscanf(numstr, "%lf", &papersx);
			sp = basecsvstrdivcpy(sp, numstr);
			sscanf(numstr, "%lf", &papersy);
			sp = basecsvstrdivcpy(sp, numstr);
			sscanf(numstr, "%lf", &paperwidth);
			sp = basecsvstrdivcpy(sp, numstr);
			sscanf(numstr, "%lf", &paperhight);
			sp = basecsvstrdivcpy(sp, numstr);
			sscanf(numstr, "%d", &flag1);
			sp = basecsvstrdivcpy(sp, numstr);
			sscanf(numstr, "%d", &flag2);
			sp = basecsvstrdivcpy(sp, paperpath);
			break;
		}
	}
	fclose(fp);

	//sprintf(dirname, "%s\\%s", wakpath, paperpath);
	//wakuenvloadoutputenv(dirname);
}

DllExport void wakuenvlistgetpapername(int idx, LPSTR papername)
{
	char	wakpath[FNAMEMAX];
	char	paperlistfname[FNAMEMAX];
	char	buf[RECMAX];
	char	*sp;
	int		tidx;
	FILE	*fp;

	papername[0] = '\0';
	senvgetwakpath(wakpath);
	sprintf(paperlistfname, "%s\\paperlst.lst", wakpath);
	if((fp = fopen(paperlistfname, "r")) == NULL){
		return;
	}
	tidx = 0;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(tidx == idx){
			sp = buf;
			sp = basecsvstrdivcpy(sp, papername);
			break;
		}
		tidx++;
	}
	fclose(fp);
}

DllExport void wakuenvsetpaperelement()
{
	char	wakpath[FNAMEMAX];
	char	infname[FNAMEMAX];
	char	outfname[FNAMEMAX];
	char	envfname[FNAMEMAX];
	char	buf[RECMAX];
	char	*sp;
	char	pname[HRECMAX];
	FILE	*infp;
	FILE	*outfp;
	int		setflag;
	char	dirname[FNAMEMAX];

	senvgetwakpath(wakpath);
	sprintf(infname, "%s\\paperlst.lst", wakpath);
	sprintf(outfname, "%s\\paperlst.bak", wakpath);
	wakuenvfilebackup(infname, outfname);

	sprintf(infname, "%s\\paperlst.bak", wakpath);
	if((infp = fopen(infname, "r")) == NULL){
		return;
	}
	sprintf(outfname, "%s\\paperlst.lst", wakpath);
	if((outfp = fopen(outfname, "w")) == NULL){
		return;
	}
	setflag = OFF;
	while(1){
		if(fgets(buf, RECMAX, infp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, pname);
		if(strcmp(papername, pname) == 0){
			sprintf(buf, "\"%s\", %4.1lf, %4.1lf, %4.1lf, %4.1lf, %4.1lf, %4.1lf, %d, %d, \"%s\", \"\", \"\"\n",
				papername, paperxsize, paperysize, papersx, papersy, paperwidth, paperhight, flag1, flag2, paperpath);
			setflag = ON;
		}
		fputs(buf, outfp);
	}
	if(setflag == OFF){
		sprintf(buf, "\"%s\", %4.1lf, %4.1lf, %4.1lf, %4.1lf, %4.1lf, %4.1lf, %d, %d, \"%s\", \"\", \"\"\n",
			papername, paperxsize, paperysize, papersx, papersy, paperwidth, paperhight, flag1, flag2, paperpath);
		fputs(buf, outfp);
	}
	fclose(infp);
	fclose(outfp);

	sprintf(dirname, "%s\\%s", wakpath, paperpath);
	CreateDirectory(dirname, NULL);

	sprintf(envfname, "%s\\output.env", dirname);
	wakuenvsaveoutputenv(envfname);
}

DllExport void wakuenvdelpaper(LPSTR delpapername)
{
	char	wakpath[FNAMEMAX];
	char	infname[FNAMEMAX];
	char	outfname[FNAMEMAX];
	char	prebuf[RECMAX];
	char	buf[RECMAX];
	char	*sp;
	char	pname[HRECMAX];
	FILE	*infp;
	FILE	*outfp;
	char	numstr[HRECMAX];

	senvgetwakpath(wakpath);
	sprintf(infname, "%s\\paperlst.lst", wakpath);
	sprintf(outfname, "%s\\paperlst.bak", wakpath);
	wakuenvfilebackup(infname, outfname);

	sprintf(infname, "%s\\paperlst.bak", wakpath);
	if((infp = fopen(infname, "r")) == NULL){
		return;
	}
	sprintf(outfname, "%s\\paperlst.lst", wakpath);
	if((outfp = fopen(outfname, "w")) == NULL){
		return;
	}
	prebuf[0] = '\0';
	while(1){
		if(fgets(buf, RECMAX, infp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, pname);
		if(strcmp(delpapername, pname) == 0){
			wakuenvdeletefile(sp);			
		}
		else{
			fputs(buf, outfp);
			if(prebuf[0] == '\0'){
				strcpy(prebuf, buf);
			}
		}
	}
	fclose(infp);
	fclose(outfp);

	strcpy(buf, prebuf);
	sp = buf;
	sp = basecsvstrdivcpy(sp, papername);
	sp = basecsvstrdivcpy(sp, numstr);
	sscanf(numstr, "%lf", &paperxsize);
	sp = basecsvstrdivcpy(sp, numstr);
	sscanf(numstr, "%lf", &paperysize);
	sp = basecsvstrdivcpy(sp, numstr);
	sscanf(numstr, "%lf", &papersx);
	sp = basecsvstrdivcpy(sp, numstr);
	sscanf(numstr, "%lf", &papersy);
	sp = basecsvstrdivcpy(sp, numstr);
	sscanf(numstr, "%lf", &paperwidth);
	sp = basecsvstrdivcpy(sp, numstr);
	sscanf(numstr, "%lf", &paperhight);
	sp = basecsvstrdivcpy(sp, numstr);
	sscanf(numstr, "%d", &flag1);
	sp = basecsvstrdivcpy(sp, numstr);
	sscanf(numstr, "%d", &flag2);
	sp = basecsvstrdivcpy(sp, paperpath);
}

int wakuenvdeletefile(LPSTR sp)
{
	char	numstr[HRECMAX];
	char	path[FNAMEMAX];
	char	wakpath[FNAMEMAX];
	char	delname[FNAMEMAX];
	char	srchname[FNAMEMAX];
    struct _finddata_t c_file;
    long	hFile;

	//sp = basecsvstrdivcpy(sp, name); //papername
	sp = basecsvstrdivcpy(sp, numstr); //paperxsize
	sp = basecsvstrdivcpy(sp, numstr); //paperysize
	sp = basecsvstrdivcpy(sp, numstr); //papersx
	sp = basecsvstrdivcpy(sp, numstr); //papersy
	sp = basecsvstrdivcpy(sp, numstr); //paperwidth
	sp = basecsvstrdivcpy(sp, numstr); //paperhight
	sp = basecsvstrdivcpy(sp, numstr); //flag1
	sp = basecsvstrdivcpy(sp, numstr); //flag2
	sp = basecsvstrdivcpy(sp, path); //paperpath

	senvgetwakpath(wakpath);
	sprintf(delname, "%s\\%s", wakpath, path);

	wsprintf(srchname, "%s\\*.*", delname);
	hFile = _findfirst(srchname, &c_file );
	if(hFile == -1L){
		return(0);
	}
	while(1){
		wsprintf(srchname, "%s\\%s", delname, c_file.name);
		unlink(srchname);
		if(_findnext( hFile, &c_file ) == -1){
			break;
		}
	}
	_findclose( hFile );

	_rmdir(delname);

	return(0);
}

DllExport void wakuenvsaveoutputenv(LPSTR wakusavefname)
{
	FILE	*fp;
	int		ncntrkind;
	char	sportname[QRECMAX];
	char	splotname[QRECMAX];
	int		npcmdtype;
	DBL		dpxsize, dpysize;
	DBL		dpstep;
	DBL		dpsx, dpsy;
	int		nparcflag, npfillflag;
	int		nphomemode, nplinemode, npomitmode;
	DBL		xmmsize, ymmsize;

	poutcntrkindget(&ncntrkind);
	poutportnameget(sportname);
	poutploternameget(splotname);
	pouttypeget(&npcmdtype);
	poutmmsizeget(&dpxsize, &dpysize);
	poutstepget(&dpstep);
	poutmmstartget(&dpsx, &dpsy);
	poutarcflagget(&nparcflag);
	poutfillflagget(&npfillflag);
	poutmodeget(&nphomemode, &nplinemode, &npomitmode);
	scrngetmmsize(&xmmsize, &ymmsize);

	if((fp = fopen(wakusavefname, "w")) == NULL){
		return;
	}
	fprintf(fp, "%s\n", splotname);
	fprintf(fp, "%s\n", sportname);
	fprintf(fp, "%d\n", ncntrkind);
	fprintf(fp, "%d\n", npcmdtype);
	fprintf(fp, "%1.1lf %1.1lf\n", dpxsize, dpysize);
	fprintf(fp, "%lf\n", dpstep);
	fprintf(fp, "%1.1lf %1.1lf\n", dpsx, dpsy);
	fprintf(fp, "%d\n", nparcflag);
	fprintf(fp, "%d\n", npfillflag);
	fprintf(fp, "%d %d %d\n", nphomemode, nplinemode, npomitmode);
	fprintf(fp, "%1.1lf %1.1lf\n", xmmsize, ymmsize);
	fclose(fp);
}

DllExport void wakuenvloadoutputenv(LPSTR wakuloadfname)
{
	FILE	*fp;
	char	buf[RECMAX];
	int		ncntrkind;
	char	sportname[QRECMAX];
	char	splotname[QRECMAX];
	int		npcmdtype;
	DBL		dpxsize, dpysize;
	DBL		dpstep;
	DBL		dpsx, dpsy;
	int		nparcflag, npfillflag;
	int		nphomemode, nplinemode, npomitmode;
	DBL		xmmsize, ymmsize;
	char	*ret;

	if((fp = fopen(wakuloadfname, "r")) == NULL){
		return;
	}
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%s\n", splotname);
	fgets(sportname, RECMAX, fp);
	sportname[strlen(sportname)-1] = '\0';
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d\n", &ncntrkind);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d\n", &npcmdtype);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%lf %lf\n", &dpxsize, &dpysize);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%lf\n", &dpstep);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%lf %lf\n", &dpsx, &dpsy);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d\n", &nparcflag);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d\n", &npfillflag);
	fgets(buf, RECMAX, fp);
	sscanf(buf, "%d %d %d\n", &nphomemode, &nplinemode, &npomitmode);
	ret = fgets(buf, RECMAX, fp);
	if(ret != NULL){
		sscanf(buf, "%lf %lf\n", &xmmsize, &ymmsize);
		scrnsetmmsize(xmmsize, ymmsize);
	}
	fclose(fp);

	poutcntrkindset(ncntrkind);
	poutportnameset(sportname);
	poutploternameset(splotname);
	pouttypeset(npcmdtype);
	poutmmsizeset(dpxsize, dpysize);
	poutstepset(dpstep);
	poutmmstartset(dpsx, dpsy);
	poutarcflagset(nparcflag);
	poutfillflagset(npfillflag);
	poutmodeset(nphomemode, nplinemode, npomitmode);
}

DllExport void wakuenvfilebackup(LPSTR infname, LPSTR outfname)
{
	FILE	*infp;
	FILE	*outfp;
	char	buf[RECMAX];

	if((infp = fopen(infname, "r")) == NULL){
		if((infp = fopen(infname, "w")) == NULL){
			return;
		}
	}
	if((outfp = fopen(outfname, "w")) == NULL){
		fclose(infp);
		return;
	}

	while(1){
		if(fgets(buf, RECMAX, infp) == NULL){
			break;
		}
		fputs(buf, outfp);
	}
	fclose(infp);
	fclose(outfp);
}

DllExport void wakuenvend()
{
	char	exepath[FNAMEMAX];
	char	envfname[FNAMEMAX];
	FILE	*fp;

	senvgetexepath(exepath);
	sprintf(envfname, "%s\\waku.env", exepath);
	if((fp = fopen(envfname, "w")) == NULL){
		return;
	}
	fprintf(fp, "%s\n", papername);
	fclose(fp);
}

DllExport void wakuenvsetgrupname(LPSTR name)
{
	strcpy(grupname, name);
}

DllExport void wakuenvgetgrupname(LPSTR name)
{
	strcpy(name, grupname);
}

DllExport void wakuenvsetpapername(LPSTR name)
{
	strcpy(papername, name);
}

DllExport void wakuenvgetpapername(LPSTR name)
{
	strcpy(name, papername);
}

DllExport void wakuenvsetpapersize(DBL xsize, DBL ysize)
{
	paperxsize = xsize;
	paperysize = ysize;
}

DllExport void wakuenvgetpapersize(DBL *xsize, DBL *ysize)
{
	*xsize = paperxsize;
	*ysize = paperysize;
}

DllExport void wakuenvsetpaperstart(DBL sx, DBL sy)
{
	papersx = sx;
	papersy = sy;
}

DllExport void wakuenvgetpaperstart(DBL *sx, DBL *sy)
{
	*sx = papersx;
	*sy = papersy;
}

DllExport void wakuenvsetpaperarea(DBL width, DBL hight)
{
	paperwidth = width;
	paperhight = hight;
}

DllExport void wakuenvgetpaperarea(DBL *width, DBL *hight)
{
	*width = paperwidth;
	*hight = paperhight;
}

DllExport void wakuenvsetpaperpath(LPSTR path)
{
	strcpy(paperpath, path);
}

DllExport void wakuenvgetpaperpath(LPSTR path)
{
	strcpy(path, paperpath);
}

DllExport void wakuenvgetfullpaperpath(LPSTR retpath)
{
	char	wakpath[FNAMEMAX];

	senvgetwakpath(wakpath);
	if(paperpath[0] == '\0'){
		sprintf(retpath, "%s", wakpath);
	}
	else{
		sprintf(retpath, "%s\\%s", wakpath, paperpath);
	}
}

DllExport void wakuenvsetselectwakuidx(int idx)
{
	paperslctidx = idx;
}

DllExport void wakuenvgetselectwakuidx(int *idx)
{
	*idx = paperslctidx;
}

DllExport void wakuenvlistaddwakuname(int slctidx, LPSTR wakuname)
{
	char	paperpath[FNAMEMAX];
	char	infname[FNAMEMAX];
	char	outfname[FNAMEMAX];
	FILE	*infp;
	FILE	*outfp;
	char	buf[QRECMAX];
	char	tbuf[QRECMAX];
	int		idx;

	wakuenvgetfullpaperpath(paperpath);
	sprintf(infname, "%s\\wakuname.lst", paperpath);
	sprintf(outfname, "%s\\wakuname.bak", paperpath);
	wakuenvfilebackup(infname, outfname);

	sprintf(infname, "%s\\wakuname.bak", paperpath);
	sprintf(outfname, "%s\\wakuname.lst", paperpath);
	if((infp = fopen(infname, "r")) == NULL){
		return;
	}
	if((outfp = fopen(outfname, "w")) == NULL){
		fclose(infp);
		return;
	}
	idx = 0;
	while(1){
		if(fgets(buf, RECMAX, infp) == NULL){
			break;
		}
		if(idx == slctidx){
			sprintf(tbuf, "\"%s\"\n", wakuname);
			if(strcmp(buf, tbuf) != 0){
				fputs(tbuf, outfp);
			}
		}
		fputs(buf, outfp);
		idx++;
	}
	if(idx <= slctidx || slctidx == -1){
		sprintf(buf, "\"%s\"\n", wakuname);
		fputs(buf, outfp);
	}

	fclose(infp);
	fclose(outfp);
}

DllExport void wakuenvlistsort(int slctidx1, int slctidx2)
{
	char	paperpath[FNAMEMAX];
	char	infname[FNAMEMAX];
	char	outfname[FNAMEMAX];
	FILE	*infp;
	FILE	*outfp;
	char	buf[RECMAX];
	char	addbuf[RECMAX];
	char	wakufname[RECMAX];
	int		idx;
	char	*sp;

	if(slctidx1 == slctidx2){
		return;
	}
	wakuenvgetfullpaperpath(paperpath);
	sprintf(infname, "%s\\wakuname.lst", paperpath);
	sprintf(outfname, "%s\\wakuname.bak", paperpath);
	if((infp = fopen(infname, "r")) == NULL){
		return;
	}
	if((outfp = fopen(outfname, "w")) == NULL){
		fclose(infp);
		return;
	}
	idx = 0;
	while(1){
		if(fgets(buf, RECMAX, infp) == NULL){
			break;
		}
		if(idx == slctidx1){
			sp = buf;
			sp = basecsvstrdivcpy(sp, wakufname);
		}
		fputs(buf, outfp);
		idx++;
	}
	fclose(infp);
	fclose(outfp);

	sprintf(infname, "%s\\wakuname.bak", paperpath);
	sprintf(outfname, "%s\\wakuname.lst", paperpath);
	if((infp = fopen(infname, "r")) == NULL){
		return;
	}
	if((outfp = fopen(outfname, "w")) == NULL){
		fclose(infp);
		return;
	}
	idx = 0;
	while(1){
		if(fgets(buf, RECMAX, infp) == NULL){
			break;
		}
		if(idx == slctidx1){
			;	// リストから削除
		}
		else if(idx == slctidx2){
			sprintf(addbuf, "\"%s\"\n", wakufname);
			fputs(addbuf, outfp);
			fputs(buf, outfp);
		}
		else{
			fputs(buf, outfp);
		}
		idx++;
	}
	if(idx <= slctidx2){
		sprintf(addbuf, "\"%s\"\n", wakufname);
		fputs(addbuf, outfp);
	}
	fclose(infp);
	fclose(outfp);
}

DllExport void wakuenvlistdelwakuname(int slctidx)
{
	char	paperpath[FNAMEMAX];
	char	infname[FNAMEMAX];
	char	outfname[FNAMEMAX];
	FILE	*infp;
	FILE	*outfp;
	char	buf[RECMAX];
	int		idx;

	wakuenvgetfullpaperpath(paperpath);
	sprintf(infname, "%s\\wakuname.lst", paperpath);
	sprintf(outfname, "%s\\wakuname.bak", paperpath);
	wakuenvfilebackup(infname, outfname);

	sprintf(infname, "%s\\wakuname.bak", paperpath);
	sprintf(outfname, "%s\\wakuname.lst", paperpath);
	if((infp = fopen(infname, "r")) == NULL){
		return;
	}
	if((outfp = fopen(outfname, "w")) == NULL){
		fclose(infp);
		return;
	}
	idx = 0;
	while(1){
		if(fgets(buf, RECMAX, infp) == NULL){
			break;
		}
		if(idx != slctidx){
			fputs(buf, outfp);
		}
		idx++;
	}

	fclose(infp);
	fclose(outfp);
}

DllExport void wakuenvlistgetwakuname(int slctidx, LPSTR wakuname)
{
	char	paperpath[FNAMEMAX];
	char	wakulistfname[FNAMEMAX];
	FILE	*fp;
	int		idx;
	char	buf[RECMAX];
	char	*sp;

	wakuenvgetfullpaperpath(paperpath);
	wakuname[0] = '\0';
	sprintf(wakulistfname, "%s\\wakuname.lst", paperpath);
	if((fp = fopen(wakulistfname, "r")) == NULL){
		if((fp = fopen(wakulistfname, "w+")) == NULL){
			return;
		}
		fclose(fp);
		return;
	}
	idx = 0;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(idx == slctidx){
			sp = buf;
			sp = basecsvstrdivcpy(sp, wakuname);
			break;	
		}
		idx++;
	}
	fclose(fp);
}

DllExport int wakuenvlistsrchwakuname(LPSTR srchname)
{
	char	paperpath[FNAMEMAX];
	char	wakulistfname[FNAMEMAX];
	char	wakuname[QRECMAX];
	FILE	*fp;
	int		idx;
	char	buf[RECMAX];
	char	*sp;

	wakuenvgetfullpaperpath(paperpath);
	sprintf(wakulistfname, "%s\\wakuname.lst", paperpath);
	if((fp = fopen(wakulistfname, "r")) == NULL){
		if((fp = fopen(wakulistfname, "w+")) == NULL){
			return(-1);
		}
		fclose(fp);
		return(-1);
	}
	idx = 0;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			idx = -1;
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, wakuname);
		if(strcmp(srchname, wakuname) == 0){
			break;
		}
		idx++;
	}
	fclose(fp);
	return(idx);
}

int		nfixflag;
DBL		dtrnspar;
char	szmatching[RECMAX];

DllExport void wakuenvgetfixflag(int *flag)
{
	*flag = nfixflag;
}

DllExport void wakuenvsetfixflag(int flag)
{
	nfixflag = flag;
}

DllExport void wakuenvgettrnspar(DBL *par)
{
	*par = dtrnspar;
}

DllExport void wakuenvsettrnspar(DBL par)
{
	dtrnspar = par;
}

DllExport void wakuenvgetmatching(LPSTR mstr)
{
	strcpy(mstr, szmatching);
}

DllExport void wakuenvsetmatching(LPSTR mstr)
{
	strcpy(szmatching, mstr);
}

