/*
	Easy Scanner システム共通ソース	1992.12.01	by stan
base		wakuenv.c	スクリーン関係関数
	Copyright 株）テオン
*/

#include	"stdafx.h"

#include	"Senvlib.h"
#include	"Scrnlib.h"
#include	"Baselib.h"
#include	"Pout.h"
#include	"Wakuenv.h"

TCHAR	grupname[RECMAX];
TCHAR	papername[RECMAX];
DBL		paperxsize;
DBL		paperysize;
DBL		papersx;
DBL		papersy;
DBL		paperwidth;
DBL		paperhight;
int		flag1;
int		flag2;
TCHAR	kaisyaname[RECMAX];
TCHAR	paperpath[RECMAX];
TCHAR	dspdataname[RECMAX];
TCHAR	outdataname[RECMAX];
int		paperslctidx = 0;

DllExport void wakuenvinit(LPTSTR envfile)
{
	//papername[0] = _T('\0');
	//paperslctidx = 0;
}

DllExport void wakuenvsetkaisyaname(LPTSTR name)
{
	_tcscpy_s(kaisyaname, name);
}

DllExport void wakuenvgetpaperelement()
{
	TCHAR	wakpath[FNAMEMAX];
	TCHAR	paperlistfname[FNAMEMAX];
	TCHAR	buf[RECMAX];
	TCHAR	*sp;
	TCHAR	pname[HRECMAX];
	TCHAR	numstr[HRECMAX];
	FILE	*fp;

	scrngetmmsize(&paperxsize, &paperysize);
	papersx = 0;
	papersy = 0;
	paperwidth = paperxsize;
	paperhight = paperysize;
	flag1 = 0;
	flag2 = 0;
	/*
	if(papername[0] == _T('\0')){
		_tcscpy_s(papername, "defpaper");
		_tcscpy_s(paperpath, papername);
		wakuenvsetpaperelement();
	}
	*/
	senvgetwakpath(wakpath);
	_stprintf_s(paperlistfname, _T("%s\\paperlst.lst"), wakpath);
	_tfopen_s(&fp, paperlistfname, _T("r"));
	if(fp == NULL){
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, pname);
		if(_tcscmp(papername, pname) == 0){
			sp = basecsvstrdivcpy(sp, numstr);
			_stscanf_s(numstr, _T("%lf"), &paperxsize);
			sp = basecsvstrdivcpy(sp, numstr);
			_stscanf_s(numstr, _T("%lf"), &paperysize);
			sp = basecsvstrdivcpy(sp, numstr);
			_stscanf_s(numstr, _T("%lf"), &papersx);
			sp = basecsvstrdivcpy(sp, numstr);
			_stscanf_s(numstr, _T("%lf"), &papersy);
			sp = basecsvstrdivcpy(sp, numstr);
			_stscanf_s(numstr, _T("%lf"), &paperwidth);
			sp = basecsvstrdivcpy(sp, numstr);
			_stscanf_s(numstr, _T("%lf"), &paperhight);
			sp = basecsvstrdivcpy(sp, numstr);
			_stscanf_s(numstr, _T("%d"), &flag1);
			sp = basecsvstrdivcpy(sp, numstr);
			_stscanf_s(numstr, _T("%d"), &flag2);
			sp = basecsvstrdivcpy(sp, paperpath);
			break;
		}
	}
	fclose(fp);

	//_stprintf_s(dirname, "%s\\%s", wakpath, paperpath);
	//wakuenvloadoutputenv(dirname);
}

DllExport void wakuenvlistgetpapername(int idx, LPTSTR papername)
{
	TCHAR	wakpath[FNAMEMAX];
	TCHAR	paperlistfname[FNAMEMAX];
	TCHAR	buf[RECMAX];
	TCHAR	*sp;
	int		tidx;
	FILE	*fp;

	papername[0] = _T('\0');
	senvgetwakpath(wakpath);
	_stprintf_s(paperlistfname, _T("%s\\paperlst.lst"), wakpath);
	_tfopen_s(&fp, paperlistfname, _T("r"));
	if(fp == NULL){
		return;
	}
	tidx = 0;
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
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
	TCHAR	wakpath[FNAMEMAX];
	TCHAR	infname[FNAMEMAX];
	TCHAR	outfname[FNAMEMAX];
	TCHAR	envfname[FNAMEMAX];
	TCHAR	buf[RECMAX];
	TCHAR	*sp;
	TCHAR	pname[HRECMAX];
	FILE	*infp;
	FILE	*outfp;
	int		setflag;
	TCHAR	dirname[FNAMEMAX];

	senvgetwakpath(wakpath);
	_stprintf_s(infname, _T("%s\\paperlst.lst"), wakpath);
	_stprintf_s(outfname, _T("%s\\paperlst.bak"), wakpath);
	wakuenvfilebackup(infname, outfname);

	_stprintf_s(infname, _T("%s\\paperlst.bak"), wakpath);
	_tfopen_s(&infp, infname, _T("r"));
	if(infp == NULL){
		return;
	}
	_stprintf_s(outfname, _T("%s\\paperlst.lst"), wakpath);
	_tfopen_s(&outfp, outfname, _T("w"));
	if(outfp == NULL){
		return;
	}
	setflag = OFF;
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, infp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, pname);
		if(_tcscmp(papername, pname) == 0){
			_stprintf_s(buf, _T("\"%s\", %4.1lf, %4.1lf, %4.1lf, %4.1lf, %4.1lf, %4.1lf, %d, %d, \"%s\", \"\", \"\"\n"),
				papername, paperxsize, paperysize, papersx, papersy, paperwidth, paperhight, flag1, flag2, paperpath);
			setflag = ON;
		}
		baselibFputs(buf, outfp);
	}
	if(setflag == OFF){
		_stprintf_s(buf, _T("\"%s\", %4.1lf, %4.1lf, %4.1lf, %4.1lf, %4.1lf, %4.1lf, %d, %d, \"%s\", \"\", \"\"\n"),
			papername, paperxsize, paperysize, papersx, papersy, paperwidth, paperhight, flag1, flag2, paperpath);
		baselibFputs(buf, outfp);
	}
	fclose(infp);
	fclose(outfp);

	_stprintf_s(dirname, _T("%s\\%s"), wakpath, paperpath);
	CreateDirectory(dirname, NULL);

	_stprintf_s(envfname, _T("%s\\output.env"), dirname);
	wakuenvsaveoutputenv(envfname);
}

DllExport void wakuenvdelpaper(LPTSTR delpapername)
{
	TCHAR	wakpath[FNAMEMAX];
	TCHAR	infname[FNAMEMAX];
	TCHAR	outfname[FNAMEMAX];
	TCHAR	prebuf[RECMAX];
	TCHAR	buf[RECMAX];
	TCHAR	*sp;
	TCHAR	pname[HRECMAX];
	FILE	*infp;
	FILE	*outfp;
	TCHAR	numstr[HRECMAX];

	senvgetwakpath(wakpath);
	_stprintf_s(infname, _T("%s\\paperlst.lst"), wakpath);
	_stprintf_s(outfname, _T("%s\\paperlst.bak"), wakpath);
	wakuenvfilebackup(infname, outfname);

	_stprintf_s(infname, _T("%s\\paperlst.bak"), wakpath);
	_tfopen_s(&infp, infname, _T("r"));
	if(infp == NULL){
		return;
	}
	_stprintf_s(outfname, _T("%s\\paperlst.lst"), wakpath);
	_tfopen_s(&outfp, outfname, _T("w"));
	if(outfp == NULL){
		return;
	}
	prebuf[0] = _T('\0');
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, infp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, pname);
		if(_tcscmp(delpapername, pname) == 0){
			wakuenvdeletefile(sp);			
		}
		else{
			baselibFputs(buf, outfp);
			if(prebuf[0] == _T('\0')){
				_tcscpy_s(prebuf, buf);
			}
		}
	}
	fclose(infp);
	fclose(outfp);

	_tcscpy_s(buf, prebuf);
	sp = buf;
	sp = basecsvstrdivcpy(sp, papername);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &paperxsize);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &paperysize);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &papersx);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &papersy);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &paperwidth);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%lf"), &paperhight);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%d"), &flag1);
	sp = basecsvstrdivcpy(sp, numstr);
	_stscanf_s(numstr, _T("%d"), &flag2);
	sp = basecsvstrdivcpy(sp, paperpath);
}

int wakuenvdeletefile(LPTSTR sp)
{
	TCHAR	numstr[HRECMAX];
	TCHAR	path[FNAMEMAX];
	TCHAR	wakpath[FNAMEMAX];
	TCHAR	delname[FNAMEMAX];
	TCHAR	srchname[FNAMEMAX];
    struct _tfinddata_t c_file;
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
	_stprintf_s(delname, FNAMEMAX, _T("%s\\%s"), wakpath, path);
	_stprintf_s(srchname, FNAMEMAX, _T("%s\\*.*"), delname);
	hFile = _tfindfirst(srchname, &c_file );
	if(hFile == -1L){
		return(0);
	}
	while(1){
		_stprintf_s(srchname, FNAMEMAX, _T("%s\\%s"), delname, c_file.name);
		_tremove(srchname);
		if(_tfindnext( hFile, &c_file ) == -1){
			break;
		}
	}
	_findclose( hFile );

	_trmdir(delname);

	return(0);
}

DllExport void wakuenvsaveoutputenv(LPTSTR wakusavefname)
{
	FILE	*fp;
	TCHAR	buf[RECMAX];
	int		ncntrkind;
	TCHAR	sportname[QRECMAX];
	TCHAR	splotname[QRECMAX];
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

	_tfopen_s(&fp, wakusavefname, _T("w"));
	if(fp == NULL){
		return;
	}
	_stprintf_s(buf, _T("%s\n"), splotname);
	baselibFputs(buf, fp);
	_stprintf_s(buf, _T("%s\n"), sportname);
	baselibFputs(buf, fp);
	_stprintf_s(buf, _T("%d\n"), ncntrkind);
	baselibFputs(buf, fp);
	_stprintf_s(buf, _T("%d\n"), npcmdtype);
	baselibFputs(buf, fp);
	_stprintf_s(buf, _T("%1.1lf %1.1lf\n"), dpxsize, dpysize);
	baselibFputs(buf, fp);
	_stprintf_s(buf, _T("%lf\n"), dpstep);
	baselibFputs(buf, fp);
	_stprintf_s(buf, _T("%1.1lf %1.1lf\n"), dpsx, dpsy);
	baselibFputs(buf, fp);
	_stprintf_s(buf, _T("%d\n"), nparcflag);
	baselibFputs(buf, fp);
	_stprintf_s(buf, _T("%d\n"), npfillflag);
	baselibFputs(buf, fp);
	_stprintf_s(buf, _T("%d %d %d\n"), nphomemode, nplinemode, npomitmode);
	baselibFputs(buf, fp);
	_stprintf_s(buf, _T("%1.1lf %1.1lf\n"), xmmsize, ymmsize);
	baselibFputs(buf, fp);
	fclose(fp);
}

DllExport void wakuenvloadoutputenv(LPTSTR wakuloadfname)
{
	FILE	*fp;
	TCHAR	buf[RECMAX];
	int		ncntrkind;
	TCHAR	sportname[QRECMAX];
	TCHAR	splotname[QRECMAX];
	int		npcmdtype;
	DBL		dpxsize, dpysize;
	DBL		dpstep;
	DBL		dpsx, dpsy;
	int		nparcflag, npfillflag;
	int		nphomemode, nplinemode, npomitmode;
	DBL		xmmsize, ymmsize;
	TCHAR	*ret;

	_tfopen_s(&fp, wakuloadfname, _T("r"));
	if(fp == NULL){
		return;
	}
	baselibCrDelFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%s\n"), splotname, QRECMAX);
	baselibCrDelFgets(sportname, RECMAX, fp);
	sportname[_tcslen(sportname)-1] = _T('\0');
	baselibCrDelFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%d\n"), &ncntrkind);
	baselibCrDelFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%d\n"), &npcmdtype);
	baselibCrDelFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%lf %lf\n"), &dpxsize, &dpysize);
	baselibCrDelFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%lf\n"), &dpstep);
	baselibCrDelFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%lf %lf\n"), &dpsx, &dpsy);
	baselibCrDelFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%d\n"), &nparcflag);
	baselibCrDelFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%d\n"), &npfillflag);
	baselibCrDelFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%d %d %d\n"), &nphomemode, &nplinemode, &npomitmode);
	ret = baselibCrDelFgets(buf, RECMAX, fp);
	if(ret != NULL){
		_stscanf_s(buf, _T("%lf %lf\n"), &xmmsize, &ymmsize);
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

DllExport void wakuenvfilebackup(LPTSTR infname, LPTSTR outfname)
{
	FILE	*infp;
	FILE	*outfp;
	TCHAR	buf[RECMAX];

	_tfopen_s(&infp, infname, _T("r"));
	if(infp == NULL){
		_tfopen_s(&infp, infname, _T("w+"));
		if(infp == NULL){
			return;
		}
	}
	_tfopen_s(&outfp, outfname, _T("w"));
	if(outfp == NULL){
		fclose(infp);
		return;
	}

	while(1){
		if(baselibCrDelFgets(buf, RECMAX, infp) == NULL){
			break;
		}
		baselibFputs(buf, outfp);
	}
	fclose(infp);
	fclose(outfp);
}

DllExport void wakuenvend()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	envfname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	senvgetexepath(exepath);
	_stprintf_s(envfname, _T("%s\\waku.env"), exepath);
	_tfopen_s(&fp, envfname, _T("w"));
	if(fp == NULL){
		return;
	}
	_stprintf_s(buf, _T("%s\n"), papername);
	baselibFputs(buf, fp);
	fclose(fp);
}

DllExport void wakuenvsetgrupname(LPTSTR name)
{
	_tcscpy_s(grupname, name);
}

DllExport void wakuenvgetgrupname(LPTSTR name)
{
	int len = _tcslen(grupname) + 1;
	_tcscpy_s(name, len, grupname);
}

DllExport void wakuenvsetpapername(LPTSTR name)
{
	_tcscpy_s(papername, name);
}

DllExport void wakuenvgetpapername(LPTSTR name)
{
	int len = _tcslen(papername) + 1;
	_tcscpy_s(name, len, papername);
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

DllExport void wakuenvsetpaperpath(LPTSTR path)
{
	_tcscpy_s(paperpath, path);
}

DllExport void wakuenvgetpaperpath(LPTSTR path)
{
	int len = _tcslen(paperpath) + 1;
	_tcscpy_s(path, len, paperpath);
}

DllExport void wakuenvgetfullpaperpath(LPTSTR retpath)
{
	TCHAR	wakpath[FNAMEMAX];

	senvgetwakpath(wakpath);
	if(paperpath[0] == _T('\0')){
		_stprintf_s(retpath, FNAMEMAX, _T("%s"), wakpath);
	}
	else{
		_stprintf_s(retpath, FNAMEMAX, _T("%s\\%s"), wakpath, paperpath);
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

DllExport void wakuenvlistaddwakuname(int slctidx, LPTSTR wakuname)
{
	TCHAR	paperpath[FNAMEMAX];
	TCHAR	infname[FNAMEMAX];
	TCHAR	outfname[FNAMEMAX];
	FILE	*infp;
	FILE	*outfp;
	TCHAR	buf[QRECMAX];
	TCHAR	outbuf[QRECMAX];
	int		idx;

	wakuenvgetfullpaperpath(paperpath);
	_stprintf_s(infname, FNAMEMAX, _T("%s\\wakuname.lst"), paperpath);
	_stprintf_s(outfname, FNAMEMAX, _T("%s\\wakuname.bak"), paperpath);
	wakuenvfilebackup(infname, outfname);

	_stprintf_s(infname, FNAMEMAX, _T("%s\\wakuname.bak"), paperpath);
	_stprintf_s(outfname, FNAMEMAX, _T("%s\\wakuname.lst"), paperpath);
	_tfopen_s(&infp, infname, _T("r"));
	if(infp == NULL){
		return;
	}
	_tfopen_s(&outfp, outfname, _T("w"));
	if(outfp == NULL){
		fclose(infp);
		return;
	}
	idx = 0;
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, infp) == NULL){
			break;
		}
		if(idx == slctidx){
			_stprintf_s(outbuf, _T("\"%s\""), wakuname);
			if(_tcscmp(buf, outbuf) != 0){
				_stprintf_s(outbuf, _T("\"%s\"\n"), wakuname);
				baselibFputs(outbuf, outfp);
			}
		}
		_stprintf_s(outbuf, _T("%s\n"), buf);
		baselibFputs(outbuf, outfp);
		idx++;
	}
	if(idx <= slctidx || slctidx == -1){
		_stprintf_s(outbuf, _T("\"%s\"\n"), wakuname);
		baselibFputs(outbuf, outfp);
	}

	fclose(infp);
	fclose(outfp);
}

DllExport void wakuenvlistsort(int slctidx1, int slctidx2)
{
	TCHAR	paperpath[FNAMEMAX];
	TCHAR	infname[FNAMEMAX];
	TCHAR	outfname[FNAMEMAX];
	FILE	*infp;
	FILE	*outfp;
	TCHAR	buf[RECMAX];
	TCHAR	outbuf[RECMAX];
	TCHAR	addbuf[RECMAX];
	TCHAR	wakufname[RECMAX];
	int		idx;
	TCHAR	*sp;

	if(slctidx1 == slctidx2){
		return;
	}
	wakuenvgetfullpaperpath(paperpath);
	_stprintf_s(infname, _T("%s\\wakuname.lst"), paperpath);
	_stprintf_s(outfname, _T("%s\\wakuname.bak"), paperpath);
	_tfopen_s(&infp, infname, _T("r"));
	if(infp == NULL){
		return;
	}
	_tfopen_s(&outfp, outfname, _T("w"));
	if(outfp == NULL){
		fclose(infp);
		return;
	}
	idx = 0;
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, infp) == NULL){
			break;
		}
		if(idx == slctidx1){
			sp = buf;
			sp = basecsvstrdivcpy(sp, wakufname);
		}
		_stprintf_s(outbuf, _T("%s\n"), buf);
		baselibFputs(outbuf, outfp);
		idx++;
	}
	fclose(infp);
	fclose(outfp);

	_stprintf_s(infname, _T("%s\\wakuname.bak"), paperpath);
	_stprintf_s(outfname, _T("%s\\wakuname.lst"), paperpath);
	_tfopen_s(&infp, infname, _T("r"));
	if(infp == NULL){
		return;
	}
	_tfopen_s(&outfp, outfname, _T("w"));
	if(outfp == NULL){
		fclose(infp);
		return;
	}
	idx = 0;
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, infp) == NULL){
			break;
		}
		if(idx == slctidx1){
			;	// リストから削除
		}
		else if(idx == slctidx2){
			_stprintf_s(addbuf, _T("\"%s\"\n"), wakufname);
			baselibFputs(addbuf, outfp);
			_stprintf_s(outbuf, _T("%s\n"), buf);
			baselibFputs(outbuf, outfp);
		}
		else{
			_stprintf_s(outbuf, _T("%s\n"), buf);
			baselibFputs(outbuf, outfp);
		}
		idx++;
	}
	if(idx <= slctidx2){
		_stprintf_s(addbuf, _T("\"%s\"\n"), wakufname);
		baselibFputs(addbuf, outfp);
	}
	fclose(infp);
	fclose(outfp);
}

DllExport void wakuenvlistdelwakuname(int slctidx)
{
	TCHAR	paperpath[FNAMEMAX];
	TCHAR	infname[FNAMEMAX];
	TCHAR	outfname[FNAMEMAX];
	FILE	*infp;
	FILE	*outfp;
	TCHAR	buf[RECMAX];
	TCHAR	outbuf[RECMAX];
	int		idx;

	wakuenvgetfullpaperpath(paperpath);
	_stprintf_s(infname, _T("%s\\wakuname.lst"), paperpath);
	_stprintf_s(outfname, _T("%s\\wakuname.bak"), paperpath);
	wakuenvfilebackup(infname, outfname);

	_stprintf_s(infname, _T("%s\\wakuname.bak"), paperpath);
	_stprintf_s(outfname, _T("%s\\wakuname.lst"), paperpath);
	_tfopen_s(&infp, infname, _T("r"));
	if(infp == NULL){
		return;
	}
	_tfopen_s(&outfp, outfname, _T("w"));
	if(outfp == NULL){
		fclose(infp);
		return;
	}
	idx = 0;
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, infp) == NULL){
			break;
		}
		if(idx != slctidx){
			_stprintf_s(outbuf, _T("%s\n"), buf);
			baselibFputs(outbuf, outfp);
		}
		idx++;
	}

	fclose(infp);
	fclose(outfp);
}

DllExport void wakuenvlistgetwakuname(int slctidx, LPTSTR wakuname)
{
	TCHAR	paperpath[FNAMEMAX];
	TCHAR	wakulistfname[FNAMEMAX];
	FILE	*fp;
	int		idx;
	TCHAR	buf[RECMAX];
	TCHAR	*sp;

	wakuenvgetfullpaperpath(paperpath);
	wakuname[0] = _T('\0');
	_stprintf_s(wakulistfname, _T("%s\\wakuname.lst"), paperpath);
	_tfopen_s(&fp, wakulistfname, _T("r"));
	if(fp == NULL){
		_tfopen_s(&fp, wakulistfname, _T("w+"));
		if(fp == NULL){
			return;
		}
		fclose(fp);
		return;
	}
	idx = 0;
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
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

DllExport int wakuenvlistsrchwakuname(LPTSTR srchname)
{
	TCHAR	paperpath[FNAMEMAX];
	TCHAR	wakulistfname[FNAMEMAX];
	TCHAR	wakuname[QRECMAX];
	FILE	*fp;
	int		idx;
	TCHAR	buf[RECMAX];
	TCHAR	*sp;

	wakuenvgetfullpaperpath(paperpath);
	_stprintf_s(wakulistfname, _T("%s\\wakuname.lst"), paperpath);
	_tfopen_s(&fp, wakulistfname, _T("r"));
	if(fp == NULL){
		_tfopen_s(&fp, wakulistfname, _T("w+"));
		if(fp == NULL){
			return(-1);
		}
		fclose(fp);
		return(-1);
	}
	idx = 0;
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			idx = -1;
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, wakuname);
		if(_tcscmp(srchname, wakuname) == 0){
			break;
		}
		idx++;
	}
	fclose(fp);
	return(idx);
}

int		nfixflag;
DBL		dtrnspar;
TCHAR	szmatching[RECMAX];

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

DllExport void wakuenvgetmatching(LPTSTR mstr)
{
	int len = _tcslen(szmatching) + 1;
	_tcscpy_s(mstr, len, szmatching);
}

DllExport void wakuenvsetmatching(LPTSTR mstr)
{
	_tcscpy_s(szmatching, mstr);
}

