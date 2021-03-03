/*
	枠順設定ファイルアクセス関数
*/

#include	<windows.h>
#include	<stdio.h>
#include	<io.h>

#include	"..\stdef.h"
#include	"dlldef.h"

#include	"wkftbl.fnc"

#define WAKUFRECLEN		80
#define WAKUFNAMEMAX	16

FILE	*wktblfp;

DllExport BOOL wkftblopen(LPSTR wakupath)
{
	char	fname[FNAMEMAX];

	sprintf(fname, "%s\\htjwakuf.tbl", wakupath);
	if((wktblfp = fopen(fname, "r+b")) == NULL){
		wkftblinit(wakupath);
		if((wktblfp = fopen(fname, "r+b")) == NULL){
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

DllExport BOOL wkftblelementread(int idx, LPSTR wakuname, DBL *rsx, DBL *rsy, DBL *rex, DBL *rey, int *rkind)
{
	long	seekp;
	char	buf[RECMAX];
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
	sscanf(buf, "%s", wakuname);
	len = strlen(wakuname);
	wakuname[len-4] = '\0';
	sscanf(&(buf[WAKUFNAMEMAX+1]), "%lf %lf %lf %lf", &sx, &sy, &ex, &ey);
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

DllExport BOOL wkftblelementwrite(int idx, LPSTR wakuname, DBL sx, DBL sy, DBL ex, DBL ey, int kind)
{
	long	seekp;
	char	recbuf[RECMAX];
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

DllExport void wkftblbackup(LPSTR wakupath)
{
	char	fname[FNAMEMAX];
	char	bakfname[FNAMEMAX];
	char	recbuf[RECMAX];
	FILE	*fp, *bakfp;
	int		ret;

	sprintf(fname, "%s\\htjwakuf.tbl", wakupath);
	if((fp = fopen(fname, "rb")) == NULL){
		return;
	}
	if((bakfp = fopen(bakfname, "wb")) == NULL){
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

DllExport BOOL wkftblinit(LPSTR wakupath)
{
	char	fname[FNAMEMAX];
	char	srchfname[FNAMEMAX];
	char	wakuname[FNAMEMAX];
	char	recbuf[RECMAX];
	FILE	*fp;
	long	hndl;
	struct _finddata_t	c_file;
	DBL		sx, sy, ex, ey;
	int		kind;
	int		ret, len;

	sprintf(fname, "%s\\htjwakuf.tbl", wakupath);
	if((fp = fopen(fname, "wb")) == NULL){
		return(FALSE);
	}
	memset(recbuf, ' ', WAKUFRECLEN);
	recbuf[WAKUFRECLEN-2] = CR;
	recbuf[WAKUFRECLEN-1] = LF;
	fwrite(recbuf, 1, WAKUFRECLEN, fp);

	sprintf(srchfname, "%s\\*.wak", wakupath);
	hndl = _findfirst(srchfname, &c_file);
	if(hndl == -1){
		fclose(fp);
		return(TRUE);
	}
	strcpy(wakuname, c_file.name);
	sprintf(fname, "%s\\%s", wakupath, wakuname);
	ret = wkftblwakminimaxget(fname, &sx, &sy, &ex, &ey, &kind);
	if(ret == TRUE){
		len = strlen(wakuname);
		wakuname[len-4] = '\0';
		wkftblrecbufset(recbuf, wakuname, sx, sy, ex, ey, kind);
		fwrite(recbuf, 1, WAKUFRECLEN, fp);
	}
	while(1){
		ret = _findnext(hndl, &c_file);
		if(ret != 0){
			break;
		}
		strcpy(wakuname, c_file.name);
		sprintf(fname, "%s\\%s", wakupath, wakuname);
		ret = wkftblwakminimaxget(fname, &sx, &sy, &ex, &ey, &kind);
		if(ret == TRUE){
			len = strlen(wakuname);
			wakuname[len-4] = '\0';
			wkftblrecbufset(recbuf, wakuname, sx, sy, ex, ey, kind);
			fwrite(recbuf, 1, WAKUFRECLEN, fp);

		}
	}
	_findclose(hndl);
	fclose(fp);
	return(TRUE);
}

DllExport void wkftblrecbufset(char *recbuf, LPSTR wakuname, DBL sx, DBL sy, DBL ex, DBL ey, int kind)
{
	char	*sp;

	memset(recbuf, ' ', WAKUFRECLEN);
	sprintf(recbuf, "%s.wak", wakuname);
	sp = &(recbuf[WAKUFNAMEMAX+1]);
	sprintf(sp, "%10.1lf %10.1lf %10.1lf %10.1lf", sx, ey, ex, sy);
	recbuf[WAKUFRECLEN-2] = CR;
	recbuf[WAKUFRECLEN-1] = LF;
}

DllExport BOOL wkftblwakminimaxget(LPSTR fname, DBL *dsx, DBL *dsy, DBL *dex, DBL *dey, int *kind)
{
	int		flag;
	FILE	*fp;
	char	buffer[RECMAX];
	char	temp[QRECMAX];
	int		atr;
	DBL		x, y;

	*dsx = INTMAX;
	*dsy = INTMAX;
	*dex = -INTMAX;
	*dey = -INTMAX;
	*kind = 0;
	flag = OFF;
	if((fp = fopen(fname, "rb")) != NULL){
		while(1){
			if(fgets(buffer, RECMAX, fp) == NULL){
				break;
			}
			if(strncmp("scrnenv", buffer, 7) == 0){
				flag = ON;
				sscanf(buffer, "%s %lf %lf %d\n", temp, dex, dey, &kind);
				*dsx = 0;
				*dsy = 0;
				break;
			}
			else if(strncmp("wakuarea", buffer, 8) == 0){
				flag = ON;
				sscanf(buffer, "%s %lf %lf %lf %lf", temp, dsx, dey, dex, dsy);
				break;
			}
			else if(buffer[1]  = ' '){
				flag = ON;
				sscanf(buffer, "%x %lf %lf", &atr, &x, &y);
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

