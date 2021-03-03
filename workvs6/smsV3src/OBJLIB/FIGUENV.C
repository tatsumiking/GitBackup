/*
	Easy Scanner システム共通ソース	1992.12.01	by stan
base		figuenv.c	図形関係変数定義関数
	Copyright 株）ハリマ
*/

#include	<windows.h>
#include	<stdio.h>
#include	<math.h>
#include	"..\stdef.h"
#include	"dlldef.h"

#include	"figuenv.fnc"

int		ncntrkind;
int		nplgncount;

DllExport void figuenvinit()
{
	ncntrkind = CNTRCNTR;
	nplgncount = 5;
}

DllExport void figuenvgetcntrkind(LPINT cntrkind)
{
	*cntrkind = ncntrkind;
}

DllExport void figuenvsetcntrkind(int cntrkind)
{
	ncntrkind = cntrkind;
}

DllExport void figuenvgetplgncount(LPINT plgncount)
{
	*plgncount = nplgncount;
}

DllExport void figuenvsetplgncount(int plgncount)
{
	nplgncount = plgncount;
}

