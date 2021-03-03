//
// Easy Scanner System Ver1.0
//
// ess  kamonlib.c	ＥＳ家紋処理関数
//
//  Copyright (c) 1998 by THEON


#include	<windows.h>

#include	<stdio.h>
#include	<math.h>

#include	"..\stdef.h"
#include	"dlldef.h"

#include	"kamonlib.fnc"

extern	char	kmnpath[];

char	kmnwkfile[]="kmnwk";		//ds 家紋枠ファイル名
char	kamonfile[]="kamon";		//ds 家紋ファイル名
FILE	*kamonidxfp, *kamondatfp;	//ds 家紋ファイルハンドル

//p 家紋ファイルオープン関数
DllExport int kamonlibfileopen(char *basefname, char *mode)
{
	char	idxfname[FNAMEMAX], datfname[FNAMEMAX];
	int		len;

	sprintf(idxfname, "%s.tbl", basefname);
	sprintf(datfname, "%s.dat", basefname);

	if((kamonidxfp = fopen(idxfname, mode)) == NULL){
		len = KAMONDATAMAX*KAMONIDXSIZE;
		if(kamonlibfilenullset(len, idxfname) == FAIL){
			return(FAIL);
		}
		if((kamonidxfp = fopen(idxfname, mode)) == NULL){
			return(FAIL);
		}
	}
	if((kamondatfp = fopen(datfname, mode)) == NULL){
		if(kamonlibfilenullset(16, datfname) == FAIL){
			fclose(kamonidxfp);
			return(FAIL);
		}
		if((kamondatfp = fopen(datfname, mode)) == NULL){
			fclose(kamonidxfp);
			return(FAIL);
		}
	}
	return(0);
}


//p 家紋ファイルオープン関数
DllExport int kamonlibopen(char *mode)
{
	char	idxfname[FNAMEMAX], datfname[FNAMEMAX];
	int		len;

	sprintf(idxfname, "%s\\%s.tbl", kmnpath, kamonfile);
	sprintf(datfname, "%s\\%s.dat", kmnpath, kamonfile);

	if((kamonidxfp = fopen(idxfname, mode)) == NULL){
		len = KAMONDATAMAX*KAMONIDXSIZE;
		if(kamonlibfilenullset(len, idxfname) == FAIL){
			return(FAIL);
		}
		if((kamonidxfp = fopen(idxfname, mode)) == NULL){
			return(FAIL);
		}
	}
	if((kamondatfp = fopen(datfname, mode)) == NULL){
		if(kamonlibfilenullset(16, datfname) == FAIL){
			fclose(kamonidxfp);
			return(FAIL);
		}
		if((kamondatfp = fopen(datfname, mode)) == NULL){
			fclose(kamonidxfp);
			return(FAIL);
		}
	}
	return(0);
}

//p 家紋枠ファイルオープン関数
DllExport int kamonlibwakuopen(char *mode)
{
	char	idxfname[FNAMEMAX], datfname[FNAMEMAX];
	int	len;

	sprintf(idxfname, "%s\\%s.tbl", kmnpath, kmnwkfile);
	sprintf(datfname, "%s\\%s.dat", kmnpath, kmnwkfile);
	if((kamonidxfp = fopen(idxfname, mode)) == NULL){
		len = KAMONDATAMAX*KAMONIDXSIZE;
		if(kamonlibfilenullset(len, idxfname) == FAIL){
			return(FAIL);
		}
		if((kamonidxfp = fopen(idxfname, mode)) == NULL){
			return(FAIL);
		}
	}
	if((kamondatfp = fopen(datfname, mode)) == NULL){
		if(kamonlibfilenullset(16, datfname) == FAIL){
			fclose(kamonidxfp);
			return(FAIL);
		}
		if((kamondatfp = fopen(datfname, mode)) == NULL){
			fclose(kamonidxfp);
			return(FAIL);
		}
	}
	return(0);
}

//p 家紋ファイルクローズ関数
DllExport void kamonlibclose()
{
	fclose(kamonidxfp);
	fclose(kamondatfp);
}

DllExport int kamonlibreadhead(int no, kamonheadtype *head)
{
	long	seekpoint;
	kamonidxtype	kamonidx;
	int		len;
	short	tlen;
	long	tseek;
	int		datapointcnt;

	seekpoint = (long)no * (long)KAMONIDXSIZE;
	fseek(kamonidxfp, seekpoint, 0);
	fread(&tlen, sizeof(short), 1, kamonidxfp);
	fread(&tseek, sizeof(long), 1, kamonidxfp);
	kamonidx.len = tlen;
	kamonidx.seekp = tseek;
	if(kamonidx.len <= 0){
		return(FAIL);
	}
	fseek(kamondatfp, kamonidx.seekp, 0);
	if(fread(head, 1, KAMONHEADSIZE, kamondatfp) == 0){
		return(FAIL);
	}
	len = kamonidx.len - KAMONHEADSIZE;
	datapointcnt = len / KAMONPOINTSIZE;
	return(datapointcnt);
}

DllExport int kamonlibwritehead(int no, kamonheadtype *head, int pcnt)
{
	long		seekpoint;
	kamonidxtype	kamonidx;
	int			len;
	short	tlen;
	long	tseek;

	seekpoint = (long)no * (long)KAMONIDXSIZE;
	fseek(kamonidxfp, seekpoint, 0);
	if(fread(&tlen, sizeof(short), 1, kamonidxfp) == 0){
		return(FAIL);
	}
	if(fread(&tseek, sizeof(long), 1, kamonidxfp) == 0){
		return(FAIL);
	}
	kamonidx.len = tlen;
	kamonidx.seekp = tseek;
	len = pcnt * KAMONPOINTSIZE + KAMONHEADSIZE;
	if(kamonidx.len < len){
		fseek(kamondatfp, 0L, 2);
		kamonidx.seekp = ftell(kamondatfp);
	}
	kamonidx.len = len;
	fseek(kamonidxfp, seekpoint, 0);
	fwrite(&(kamonidx.len), sizeof(short), 1, kamonidxfp);
	fwrite(&(kamonidx.seekp), sizeof(long), 1, kamonidxfp);

	fseek(kamondatfp, kamonidx.seekp, 0);
	if(fwrite(head, 1, KAMONHEADSIZE, kamondatfp) == 0){
		return(FAIL);
	}
	return(0);
}

DllExport int kamonlibreadatrxy(int *atr, DBL *dx, DBL *dy)
{
	int		xybit;
	int		tx, ty;
	DBL		x, y;
	UCHAR	pointdata[KAMONPOINTSIZE*4];

	if(fread(pointdata, KAMONPOINTSIZE, 1, kamondatfp) == 0){
		return(FAIL);
	}
	*atr = pointdata[KAMONPOINTATR];
	x = pointdata[KAMONPOINTX];
	y = pointdata[KAMONPOINTY];
	xybit = pointdata[KAMONPOINTBIT];
	tx = (xybit >> 4) & 0x0f;
	ty = xybit & 0x0f;
	*dx = x + ((DBL)tx / 16.0);
	*dy = y + ((DBL)ty / 16.0);
	return(0);
}

DllExport int kamonlibwriteatrxy(int atr, DBL dx, DBL dy)
{
	int		xybit;
	int		nx, ny;
	int		tx, ty;
	UCHAR	pointdata[KAMONPOINTSIZE*4];

	nx = (int)dx;
	ny = (int)dy;
	tx = (int)((dx - (DBL)nx) * 16.0);
	ty = (int)((dy - (DBL)ny) * 16.0);
	xybit = ((tx << 4) & 0xf0) | (ty & 0xf);
	pointdata[KAMONPOINTATR] = atr;
	pointdata[KAMONPOINTX] = nx;
	pointdata[KAMONPOINTY] = ny;
	pointdata[KAMONPOINTBIT] = xybit;
	if(fwrite(pointdata, KAMONPOINTSIZE, 1, kamondatfp) == 0){
		return(FAIL);
	}
	return(0);
}

int kamonlibfilenullset(int cnt, LPSTR fname)
{
	FILE	*fp;
	char	buf[16];

	if((fp = fopen(fname, "wb")) == NULL){
		return(FAIL);
	}
	memset(buf, 0, 16);
	while(cnt--){
		fwrite(buf, 1, 1, fp);
	}

	fclose(fp);
	return(TRUE);
}

