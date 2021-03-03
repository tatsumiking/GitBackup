
#include	<windows.h>

#include	<stdio.h>
#include	<math.h>

#include	"..\stdef.h"
#include	"dlldef.h"

#include	"baselib.fnc"
#include	"diza.fnc"
/*
int	dizaptn[16][16] = {
	{  0,128, 32,160,  8,136, 40,168,  2,130, 34,162, 10,138, 42,170},
	{192, 64,224, 96,200, 72,232,104,194, 66,226, 98,202, 74,234,106},
	{ 48,176, 16,144, 56,184, 24,152, 50,178, 18,146, 58,186, 26,154},
	{240,112,208, 80,248,120,216, 88,242,114,210, 82,250,122,218, 90},
	{ 12,140, 44,172,  4,132, 36,164, 14,142, 46,174,  6,134, 38,166},
	{204, 76,236,108,196, 68,228,100,206, 78,238,110,198, 70,230,102},
	{ 60,188, 28,156, 52,180, 20,148, 62,190, 30,158, 54,182, 22,150},
	{252,124,220, 92,244,116,212, 84,254,126,222, 94,246,118,214, 86},
	{  3,131, 35,163, 11,139, 43,171,  1,129, 33,161,  9,137, 41,169},
	{195, 67,227, 99,203, 75,235,107,193, 65,225, 97,201, 73,233,105},
	{ 51,179, 19,147, 59,187, 27,155, 49,177, 17,145, 57,185, 25,153},
	{243,115,211, 83,251,123,219, 91,241,113,209, 81,249,121,217, 89},
	{ 15,143, 47,175,  7,135, 39,167, 13,141, 45,173,  5,133, 37,165},
	{207, 79,239,111,199, 71,231,103,205, 77,237,109,197, 69,229,101},
	{ 63,191, 31,159, 55,183, 23,151, 61,189, 29,157, 53,181, 21,149},
	{255,127,223, 95,247,119,215, 87,253,125,221, 93,245,117,213, 85},
	};
*/
int	dizaptn[16][16] = {
	{253,245,228,240,198,183,212,182,211,197,181,210,233,227,250,254},
	{251,234,184,213,166,143,165,142,164,141,163,152,180,223,232,244},
	{241,221,199,144,124,100,123, 99,122, 98,113,131,162,196,222,226},
	{229,185,167,114,101, 84, 72, 71, 83, 70, 69, 97,112,140,179,239},
	{200,153,102,125, 73, 49, 48, 47, 58, 46, 45, 68, 96,132,161,195},
	{186,168,115, 74, 50, 34, 26, 25, 24, 23, 33, 44, 82,111,139,178},
	{201,145,103, 75, 51, 27, 12, 11, 10,  9, 22, 43, 67, 95,160,209},
	{214,169,126, 85, 59, 28, 13,  3,  2,  8, 21, 42, 66, 94,138,177},
	{187,146,104, 76, 52, 29, 14,  4,  0,  7, 20, 57, 88,121,159,219},
	{202,154,127, 77, 53, 30, 15, 16,  1,  6, 19, 41, 65, 93,151,194},
	{188,147,105, 86, 54, 35, 31, 32,  5, 18, 36, 40, 64,120,137,176},
	{203,155,128,106, 78, 55, 60, 56, 17, 38, 39, 81, 92,110,150,208},
	{235,215,148,116,129, 79, 80, 87, 37, 62, 63, 91,119,136,175,231},
	{242,189,204,170,107,117,108,118, 61, 90,109,130,135,207,218,225},
	{246,236,216,190,156,171,149,172, 89,157,134,158,174,193,238,249},
	{248,252,243,230,205,220,191,217,133,206,173,192,237,224,247,255},
	};

BYTE	rclrtbl[256+2];
BYTE	gclrtbl[256+2];
BYTE	bclrtbl[256+2];

int		maskbit[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

DBL		cntrstaa, cntrstbb;

DllExport void dizargbtblinit(LPSTR fname)
{
	FILE	*fp;
	char	buf[RECMAX];
	DBL		rnum, gnum, bnum, radd, gadd, badd;
	DBL		cntrst;

	rnum = 231.0; gnum = 239.0; bnum = 255.0;
	radd = 32.0; gadd = 32.0; badd = 32.0;
	cntrst = 1.0;
	dizacntrstset(cntrst);
	dizargbtbltrns(rnum, gnum, bnum, radd, gadd, badd);
	fp = fopen(fname, "r");
	if(fp != NULL){
		fread(buf, 1, RECMAX, fp);
		sscanf(buf, "%lf %lf %lf %lf %lf %lf %lf"
				, &rnum, &gnum, &bnum, &radd, &gadd, &badd, &cntrst);
		fclose(fp);
		dizacntrstset(cntrst);
		dizargbtbltrns(rnum, gnum, bnum, radd, gadd, badd);
	}
}

void dizargbtbltrns(DBL rnum, DBL gnum, DBL bnum, DBL radd, DBL gadd, DBL badd)
{
	DBL		x1, x4;
	DBL		b1, b2, b3, b4;
	DBL		t, t1, add;
	int		xx, yy;

	x1 = 0.0;
	x4 = 255.0-radd;
	rnum -= radd;
	add = 1.0 / 256.0;
	for(t = 0, yy = 0; yy < 256; t += add, yy++){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		xx = (int)(b1 * x1 + b2 * rnum + b3 * rnum + b4 * x4 + 0.5 + radd);
		if(xx < 0){
			xx = 0;
		}
		if(xx > 255){
			xx = 255;
		}
		rclrtbl[yy] = xx;
	}
	rclrtbl[255] = 255;
	x4 = 255.0-gadd;
	gnum -= gadd;
	for(t = 0, yy = 0; yy < 256; t += add, yy++){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		xx = (int)(b1 * x1 + b2 * gnum + b3 * gnum + b4 * x4 + 0.5 + gadd);
		if(xx < 0){
			xx = 0;
		}
		if(xx > 255){
			xx = 255;
		}
		gclrtbl[yy] = xx;
	}
	gclrtbl[255] = 255;
	x4 = 255.0-badd;
	bnum -= badd;
	for(t = 0, yy = 0; yy < 256; t += add, yy++){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		xx = (int)(b1 * x1 + b2 * bnum + b3 * bnum + b4 * x4 + 0.5 + badd);
		if(xx < 0){
			xx = 0;
		}
		if(xx > 255){
			xx = 255;
		}
		bclrtbl[yy] = xx;
	}
	bclrtbl[255] = 255;
}

void dizacntrstset(DBL cntrst)
{
	cntrstaa = cntrst;
	cntrstbb = (cntrst - 1.0) / 2.0;
}

DllExport void dizaclrchng(int *r, int *g, int *b)
{
	int		wr, wb, wg;
	DBL		twr, twb, twg;

	wr = (*r) & 0x00ff;
	wg = (*g) & 0x00ff;
	wb = (*b) & 0x00ff;
	if(cntrstaa != 1.0){
		twr = wr;
		twg = wg;
		twb = wb;
		wb = (int)(  cntrstaa * twb - cntrstbb * twr - cntrstbb * twg);
		wr = (int)(- cntrstbb * twb + cntrstaa * twr - cntrstbb * twg);
		wg = (int)(- cntrstbb * twb - cntrstbb * twr + cntrstaa * twg);
	}
	if(wr > 255){
		wr = 255;
	}
	else if(wr < 0){
		wr = 0;
	}
	if(wg > 255){
		wg = 255;
	}
	else if(wg < 0){
		wg = 0;
	}
	if(wb > 255){
		wb = 255;
	}
	else if(wb < 0){
		wb = 0;
	}

	*r = rclrtbl[wr];
	*g = gclrtbl[wg];
	*b = bclrtbl[wb];
}

DllExport void dizargbtocmy(int r, int g, int b, int *c, int *m, int *y, int *bk)
{
	int		wr, wb, wg;
	DBL		twr, twb, twg;

	wr = r & 0x00ff;
	wg = g & 0x00ff;
	wb = b & 0x00ff;

	if(wr == 0 && wg == 0 && wb == 0){
		*c = 255;
		*m = 255;
		*y = 255;
		*bk = 0;
	}
	else{
		if(cntrstaa != 1.0){
			twr = wr;
			twg = wg;
			twb = wb;
			wb = (int)(  cntrstaa * twb - cntrstbb * twr - cntrstbb * twg);
			wr = (int)(- cntrstbb * twb + cntrstaa * twr - cntrstbb * twg);
			wg = (int)(- cntrstbb * twb - cntrstbb * twr + cntrstaa * twg);
		}

		if(wr > 255){
			wr = 255;
		}
		else if(wr < 0){
			wr = 0;
		}
		if(wg > 255){
			wg = 255;
		}
		else if(wg < 0){
			wg = 0;
		}
		if(wb > 255){
			wb = 255;
		}
		else if(wb < 0){
			wb = 0;
		}

		*c = 255 - rclrtbl[wr];
		*m = 255 - gclrtbl[wg];
		*y = 255 - bclrtbl[wb];
		*bk = 0;
	}
}

void dizargbtotrgb(int r, int g, int b, int *tr, int *tg, int *tb)
{
	int		wr, wb, wg;

	wr = r & 0x00ff;
	wg = g & 0x00ff;
	wb = b & 0x00ff;

	*tr = rclrtbl[wr];
	*tg = gclrtbl[wg];
	*tb = bclrtbl[wb];

	if(*tr > 255){
		*tr = 255;
	}
	else if(*tr < 0){
		*tr = 0;
	}
	if(*tg > 255){
		*tg = 255;
	}
	else if(*tg < 0){
		*tg = 0;
	}
	if(*tb > 255){
		*tb = 255;
	}
	else if(*tb < 0){
		*tb = 0;
	}
}

int dizabitchk(int x, int y, int c)
{
	x &= 0x000f;
	y &= 0x000f;
	if(dizaptn[x][y] < c){
		return(ON);
	}
	return(OFF);
}

DllExport void dizabitcmychk(int x, int y, int *rc, int *rm, int *ry)
{
	x &= 0x000f;
	y &= 0x000f;
	if(dizaptn[x][y] >= *rc){
		*rc = 0;
	}
	if(dizaptn[x][y] >= *rm){
		*rm = 0;
	}
	if(dizaptn[x][y] >= *ry){
		*ry = 0;
	}
}

