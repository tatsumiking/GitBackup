//
// Easy Scanner System Ver1.0
//
// ess  fontlib.c  ÉtÉHÉìÉgä÷åWä÷êî
//
//  Copyright (c) 1998 by THEON

#include	<windows.h>
#include	<stdio.h>
#include	<math.h>

#include	"..\stdef.h"
#include	"dlldef.h"
#include	"fsf1font.h"
#include	"esf0font.h"
#include	"esf1font.h"


#include	"fontlib.fnc"
#include	"baselib.fnc"
#include	"fsf1font.fnc"
#include	"esf0font.fnc"
#include	"esf1font.fnc"
#include	"ttfont.fnc"
#include	"smslib.fnc"

void fontlibInit()
{
	ttfontInit();
}

void fontlibEnd()
{
	ttfontEnd();
}

void fontlibFontNameAndType(LPSTR fontname, LPSTR fontfullname, int *retftype)
{
	char	exepath[FNAMEMAX];
	char	flstfname[FNAMEMAX];
	FILE	*fp;
	char	buf[HRECMAX+16];
	int		len;
	int		type;
	char	sfname[HRECMAX];
	char	ffname[HRECMAX];

	smslibGetExecPath(exepath);
	sprintf(flstfname, "%s\\fontlist.lst", exepath);
	fp = fopen(flstfname, "r+b");
	if(fp == NULL){
		*retftype = TT1FONT;
		strcpy(fontfullname, "ÇlÇr ñæí©");
		return;
	}
	while(1){
		len = fread(buf, 1, HRECMAX, fp);
		if(len != HRECMAX){
			break;
		}
		sscanf(buf, "%d", &type);
		strcpy(sfname, &(buf[3]));
		baselastspacedel(sfname);
		strcpy(ffname, &(buf[64]));
		baselastspacedel(ffname);
		if(strcmp(fontname, sfname) == 0){
			*retftype = type;
			strcpy(fontfullname, ffname);
			break;
		}
	}
	fclose(fp);
}

void fontlibTrnsTimeSet(DBL dsx, DBL dsy, DBL dwidth, DBL dhight)
{
	fsf1fontTrnsElementSet(dsx, dsy, dwidth, dhight);
}

void fontlibPlotOut(UINT sjiscode)
{
	int		nComID;		// dummy
	int		type;
	char	fontname[RECMAX];

	nComID = 1;
	type = smslibGetFontType();
	smslibGetFontName(fontname);
	switch(type){
	case ES0FONT:
		esf0fontFontNameSet(fontname);
		esf0fontSF1TrnsDataLoad(sjiscode);
		break;
	case ES1FONT:
		esf1fontFontNameSet(fontname);
		esf1fontDataLoad(sjiscode);
		break;
	case TT1FONT:
	case TT2FONT:
		ttfontFontNameSet(fontname);
		ttfontSF1TrnsDataLoad(sjiscode);
		break;
	}
	fsf1fontPlotOut(nComID);
}

void fontlibDispOut(HDC PaintDC, UINT sjiscode)
{
	int		type;
	char	fontname[RECMAX];

	type = smslibGetFontType();
	smslibGetFontName(fontname);
	switch(type){
	case ES0FONT:
		esf0fontFontNameSet(fontname);
		esf0fontSF1TrnsDataLoad(sjiscode);
		break;
	case ES1FONT:
		esf1fontFontNameSet(fontname);
		esf1fontDataLoad(sjiscode);
		break;
	case TT1FONT:
	case TT2FONT:
		ttfontFontNameSet(fontname);
		ttfontSF1TrnsDataLoad(sjiscode);
		break;
	}
	fsf1fontFillDraw(PaintDC);
}



