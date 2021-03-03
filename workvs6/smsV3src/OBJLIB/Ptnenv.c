/*
	Easy Scanner システム共通ソース	1992.12.01	by stan
base		ptnenv.c	関係関数
	Copyright 株）テオン
*/

#include	<windows.h>
#include	<stdio.h>
#include	<math.h>
#include	<direct.h>
#include	"..\stdef.h"
#include	"dlldef.h"

#include	"senvlib.fnc"
#include	"baselib.fnc"
#include	"pout.fnc"
#include	"ptnenv.fnc"

char	grupname[RECMAX];
DBL		paperxsize;
DBL		paperysize;
char	gruppath[RECMAX];
int		grupslctidx;
int		nunitno;
int		noderno;
int		nsetmode;

DllExport void ptnenvsetmode(int nunit, int noder, int nset)
{
	nunitno = nunit;
	noderno = noder;
	nsetmode = nset;
}

DllExport void ptnenvgetmode(LPINT nunit, LPINT noder, LPINT nset)
{
	*nunit = nunitno;
	*noder = noderno;
	*nset = nsetmode;
}

DllExport void ptnenvgetpaperelement()
{
	char	wakpath[FNAMEMAX];
	char	paperlistfname[FNAMEMAX];
	char	buf[RECMAX];
	char	*sp;
	char	pname[HRECMAX];
	char	numstr[HRECMAX];
	FILE	*fp;

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
		if(strcmp(grupname, pname) == 0){
			sp = basecsvstrdivcpy(sp, numstr);
			sscanf(numstr, "%lf", &paperxsize);
			sp = basecsvstrdivcpy(sp, numstr);
			sscanf(numstr, "%lf", &paperysize);
			sprintf(gruppath, "%s\\%s", wakpath, grupname);
			break;
		}
	}
	fclose(fp);
}

DllExport void ptnenvsetpaperelement()
{
	char	wakpath[FNAMEMAX];
	char	infname[FNAMEMAX];
	char	outfname[FNAMEMAX];
	char	buf[RECMAX];
	char	*sp;
	char	pname[HRECMAX];
	FILE	*infp;
	FILE	*outfp;
	int		setflag;

	senvgetwakpath(wakpath);
	sprintf(infname, "%s\\paperlst.lst", wakpath);
	sprintf(outfname, "%s\\paperlst.bak", wakpath);

	ptnenvfilebackup(infname, outfname);

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
		if(strcmp(grupname, pname) == 0){
			sprintf(buf, "\"%s\", %4.1lf, %4.1lf\n",
				grupname, paperxsize, paperysize);
			setflag = ON;
		}
		fputs(buf, outfp);
	}
	if(setflag == OFF){
		sprintf(buf, "\"%s\", %4.1lf, %4.1lf\n",
			grupname, paperxsize, paperysize);
		fputs(buf, outfp);
	}
	fclose(infp);
	fclose(outfp);

	sprintf(gruppath, "%s\\%s", wakpath, grupname);
	CreateDirectory(gruppath, NULL);
}

DllExport void ptnenvfilebackup(LPSTR infname, LPSTR outfname)
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

DllExport void ptnenvsetgrupname(LPSTR name)
{
	strcpy(grupname, name);
}

DllExport void ptnenvgetgrupname(LPSTR name)
{
	strcpy(name, grupname);
}

DllExport void ptnenvsetpapersize(DBL xsize, DBL ysize)
{
	paperxsize = xsize;
	paperysize = ysize;
}

DllExport void ptnenvgetpapersize(DBL *xsize, DBL *ysize)
{
	*xsize = paperxsize;
	*ysize = paperysize;
}

DllExport void ptnenvgetgruppath(LPSTR path)
{
	strcpy(path, gruppath);
}

DllExport void ptnenvsetselectidx(int idx)
{
	grupslctidx = idx;
}

DllExport void ptnenvgetselectidx(int *idx)
{
	*idx = grupslctidx;
}

DllExport void ptnenvlistaddname(int slctidx, LPSTR ptnfname)
{
	char	infname[FNAMEMAX];
	char	outfname[FNAMEMAX];
	FILE	*infp;
	FILE	*outfp;
	char	buf[RECMAX];
	int		idx;

	sprintf(infname, "%s\\ptnname.lst", gruppath);
	sprintf(outfname, "%s\\ptnname.bak", gruppath);
	ptnenvfilebackup(infname, outfname);

	sprintf(infname, "%s\\ptnname.bak", gruppath);
	sprintf(outfname, "%s\\ptnname.lst", gruppath);
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
			sprintf(buf, "\"%s\"\n", ptnfname);
		}
		fputs(buf, outfp);
		idx++;
	}
	if(idx <= slctidx || slctidx == -1){
		sprintf(buf, "\"%s\"\n", ptnfname);
		fputs(buf, outfp);
	}
	fclose(infp);
	fclose(outfp);
}

DllExport void ptnenvlistchkaddname(LPSTR ptnfname)
{
	char	infname[FNAMEMAX];
	char	outfname[FNAMEMAX];
	FILE	*infp;
	FILE	*outfp;
	char	buf[RECMAX];
	int		idx, len, setflag;

	sprintf(infname, "%s\\ptnname.lst", gruppath);
	sprintf(outfname, "%s\\ptnname.bak", gruppath);
	ptnenvfilebackup(infname, outfname);

	sprintf(infname, "%s\\ptnname.bak", gruppath);
	sprintf(outfname, "%s\\ptnname.lst", gruppath);
	if((infp = fopen(infname, "r")) == NULL){
		return;
	}
	if((outfp = fopen(outfname, "w")) == NULL){
		fclose(infp);
		return;
	}
	idx = 0;
	len = strlen(ptnfname);
	setflag = TRUE;
	while(1){
		if(fgets(buf, RECMAX, infp) == NULL){
			break;
		}
		if(strncmp(&buf[1], ptnfname, len) == 0){
			setflag = FALSE;
		}
		fputs(buf, outfp);
		idx++;
	}
	if(setflag == TRUE){
		sprintf(buf, "\"%s\"\n", ptnfname);
		fputs(buf, outfp);
	}
	fclose(infp);
	fclose(outfp);
}

DllExport void ptnenvlistdelname(int slctidx, LPSTR ptnfname)
{
	char	infname[FNAMEMAX];
	char	outfname[FNAMEMAX];
	FILE	*infp;
	FILE	*outfp;
	char	buf[RECMAX];
	int		idx;

	sprintf(infname, "%s\\ptnname.lst", gruppath);
	sprintf(outfname, "%s\\ptnname.bak", gruppath);
	ptnenvfilebackup(infname, outfname);

	sprintf(infname, "%s\\ptnname.bak", gruppath);
	sprintf(outfname, "%s\\ptnname.lst", gruppath);
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

DllExport void ptnenvlistgetname(int slctidx, LPSTR ptnfname)
{
	char	listfname[FNAMEMAX];
	FILE	*fp;
	int		idx;
	char	buf[RECMAX];
	char	*sp;

	ptnfname[0] = '\0';
	sprintf(listfname, "%s\\ptnname.lst", gruppath);
	if((fp = fopen(listfname, "r")) == NULL){
		if((fp = fopen(listfname, "w+")) == NULL){
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
			sp = basecsvstrdivcpy(sp, ptnfname);
			break;	
		}
		idx++;
	}
	fclose(fp);
}

