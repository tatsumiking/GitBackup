/*
	Easy Scanner システム共通ソース	1992.12.01	by stan
base		figuenv.c	図形関係変数定義関数
	Copyright 株）ハリマ
*/

#include	"stdafx.h"

#include	"Figuenv.h"

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