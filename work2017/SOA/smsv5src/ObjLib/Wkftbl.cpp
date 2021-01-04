/*
	枠順設定ファイルアクセス関数
*/

#include	"stdafx.h"

#include	"Wkftbl.h"
#include	"Baselib.h"


#define WAKUFRECLEN		80
#define WAKUFNAMEMAX	16

FILE	*wktblfp;

DllExport BOOL wkftblopen(LPTSTR wakupath)
{
	TCHAR	fname[FNAMEMAX];

	_stprintf_s(fname, _T("%s\\htjwakuf.tbl"), wakupath);
	_tfopen_s(&wktblfp, fname, _T("r+b"));
	if(wktblfp == NULL){
		wkftblinit(wakupath);
		_tfopen_s(&wktblfp, fname, _T("r+b"));
		if(wktblfp == NULL){
			return(FALSE);
		}
	}
	return(TRUE);
}

DllExport void wkftblclose()
{
	if(wktblfp != NULL){
		fclose(wktblfp);
	}
}

DllExport BOOL wkftblelementread(int idx, LPTSTR wakuname, DBL *rsx, DBL *rsy, DBL *rex, DBL *rey, int *rkind)
{
	long	seekp;
	TCHAR	buf[RECMAX];
	DBL		sx, sy, ex, ey, tt;
	int		ret, len;

	*rsx = 0; *rsy = 0;
	*rex = 1; *rey = 1;
	if(idx == -1){
		return(FALSE);
	}
	seekp = WAKUFRECLEN * (idx+1);
	fseek(wktblfp, seekp, 0);
	ret = fread(buf, 1, WAKUFRECLEN, wktblfp);
	if(ret != WAKUFRECLEN){
		return(FALSE);
	}
	_stscanf_s(buf, _T("%s"), wakuname, RECMAX);
	len = _tcslen(wakuname);
	wakuname[len-4] = _T('\0');
	_stscanf_s(&(buf[WAKUFNAMEMAX+1]), _T("%lf %lf %lf %lf"), &sx, &sy, &ex, &ey);
	if(ex < sx){
		tt = sx;
		sx = ex;
		ex = tt;
	}
	if(ey < sy){
		tt = sy;
		sy = ey;
		ey = tt;
	}
	*rsx = sx;
	*rsy = sy;
	*rex = ex;
	*rey = ey;
	*rkind = 0;
	return(TRUE);
}

DllExport BOOL wkftblelementwrite(int idx, LPTSTR wakuname, DBL sx, DBL sy, DBL ex, DBL ey, int kind)
{
	long	seekp;
	TCHAR	recbuf[RECMAX];
	int		ret;

	wkftblrecbufset(recbuf, wakuname, sx, sy, ex, ey, kind);
	seekp = WAKUFRECLEN * (idx+1);
	fseek(wktblfp, seekp, 0);
	ret = fwrite(recbuf, 1, WAKUFRECLEN, wktblfp);
	if(ret != WAKUFRECLEN){
		return(FALSE);
	}
	return(TRUE);
}

DllExport void wkftblbackup(LPTSTR wakupath)
{
	TCHAR	fname[FNAMEMAX];
	TCHAR	bakfname[FNAMEMAX];
	TCHAR	recbuf[RECMAX];
	FILE	*fp, *bakfp;
	int		ret;

	_stprintf_s(fname, _T("%s\\htjwakuf.tbl"), wakupath);
	_stprintf_s(bakfname, _T("%s\\htjwakuf.bak"), wakupath);
	_tfopen_s(&fp, fname, _T("rb"));
	if(fp == NULL){
		return;
	}
	_tfopen_s(&bakfp, bakfname, _T("wb"));
	if(bakfp == NULL){
		fclose(fp);
		return;
	}
	while(1){
		ret = fread(recbuf, 1, WAKUFRECLEN, fp);
		if(ret != WAKUFRECLEN){
			break;
		}
		fwrite(recbuf, 1, WAKUFRECLEN, bakfp);
	}
	fclose(bakfp);
	fclose(fp);
}

DllExport BOOL wkftblinit(LPTSTR wakupath)
{
	TCHAR	fname[FNAMEMAX];
	TCHAR	srchfname[FNAMEMAX];
	TCHAR	wakuname[FNAMEMAX];
	TCHAR	recbuf[RECMAX];
	FILE	*fp;
	long	hndl;
	struct _tfinddata_t	c_file;
	DBL		sx, sy, ex, ey;
	int		kind;
	int		ret, len;

	_stprintf_s(fname, _T("%s\\htjwakuf.tbl"), wakupath);
	_tfopen_s(&fp, fname, _T("wb"));
	if(fp == NULL){
		return(FALSE);
	}
	memset(recbuf, _T(' '), WAKUFRECLEN);
	recbuf[WAKUFRECLEN-2] = CR;
	recbuf[WAKUFRECLEN-1] = LF;
	fwrite(recbuf, 1, WAKUFRECLEN, fp);

	_stprintf_s(srchfname, _T("%s\\*.wak"), wakupath);
	hndl = _tfindfirst(srchfname, &c_file);
	if(hndl == -1){
		fclose(fp);
		return(TRUE);
	}
	_tcscpy_s(wakuname, c_file.name);
	_stprintf_s(fname, _T("%s\\%s"), wakupath, wakuname);
	ret = wkftblwakminimaxget(fname, &sx, &sy, &ex, &ey, &kind);
	if(ret == TRUE){
		len = _tcslen(wakuname);
		wakuname[len-4] = _T('\0');
		wkftblrecbufset(recbuf, wakuname, sx, sy, ex, ey, kind);
		fwrite(recbuf, 1, WAKUFRECLEN, fp);
	}
	while(1){
		ret = _tfindnext(hndl, &c_file);
		if(ret != 0){
			break;
		}
		_tcscpy_s(wakuname, c_file.name);
		_stprintf_s(fname, _T("%s\\%s"), wakupath, wakuname);
		ret = wkftblwakminimaxget(fname, &sx, &sy, &ex, &ey, &kind);
		if(ret == TRUE){
			len = _tcslen(wakuname);
			wakuname[len-4] = _T('\0');
			wkftblrecbufset(recbuf, wakuname, sx, sy, ex, ey, kind);
			fwrite(recbuf, 1, WAKUFRECLEN, fp);

		}
	}
	_findclose(hndl);
	fclose(fp);
	return(TRUE);
}

DllExport void wkftblrecbufset(TCHAR *recbuf, LPTSTR wakuname, DBL sx, DBL sy, DBL ex, DBL ey, int kind)
{
	TCHAR	*sp;

	memset(recbuf, _T(' '), WAKUFRECLEN);
	_stprintf_s(recbuf, RECMAX, _T("%s.wak"), wakuname);
	sp = &(recbuf[WAKUFNAMEMAX+1]);
	_stprintf_s(sp, RECMAX, _T("%10.1lf %10.1lf %10.1lf %10.1lf"), sx, ey, ex, sy);
	recbuf[WAKUFRECLEN-2] = CR;
	recbuf[WAKUFRECLEN-1] = LF;
}

DllExport BOOL wkftblwakminimaxget(LPTSTR fname, DBL *dsx, DBL *dsy, DBL *dex, DBL *dey, int *kind)
{
	int		flag;
	FILE	*fp;
	TCHAR	buffer[RECMAX];
	int		len1, len2;
	TCHAR	temp[QRECMAX];
	int		atr;
	DBL		x, y;

	*dsx = INTMAX;
	*dsy = INTMAX;
	*dex = -INTMAX;
	*dey = -INTMAX;
	*kind = 0;
	flag = OFF;
	len1 = _tcslen(_T("scrnenv"));
	len2 = _tcslen(_T("wakuarea"));
	_tfopen_s(&fp, fname, _T("rb"));
	if(fp == NULL){
		while(1){
			if(baselibCrDelFgets(buffer, RECMAX, fp) == NULL){
				break;
			}
			if(_tcsnccmp(_T("scrnenv"), buffer, len1) == 0){
				flag = ON;
				_stscanf_s(buffer, _T("%s %lf %lf %d\n"), temp, QRECMAX, dex, dey, kind);
				*dsx = 0;
				*dsy = 0;
				break;
			}
			else if(_tcsnccmp(_T("wakuarea"), buffer, len2) == 0){
				flag = ON;
				_stscanf_s(buffer, _T("%s %lf %lf %lf %lf"), temp, QRECMAX, dsx, dey, dex, dsy);
				break;
			}
			else if(buffer[1]  = _T(' ')){
				flag = ON;
				_stscanf_s(buffer, _T("%x %lf %lf"), &atr, &x, &y);
				if(x < *dsx){
					*dsx = x;
				}
				if(y < *dsy){
					*dsy = y;
				}
				if(x > *dex){
					*dex = x;
				}
				if(y > *dey){
					*dey = y;
				}
			}
		}
	}
	fclose(fp);
	if(flag == OFF || dsx == dex || dsy == dey){
		return(FALSE);
	}
	return(TRUE);
}

