/*
	Easy Scanner システム共通ソース	1992.12.01	by stan
base		protect.c	プロテクト関係関数
	Copyright 株）ハリマ
*/

#include	<windows.h>
#include	<stdio.h>

#include	"..\stdef.h"
#include	"dlldef.h"

#include	"protect.fnc"

//int TScrnEnv::ChkCntinel()
DllExport int prtectchkcntinel()
{
	return(OFF);
}

//int TScrnEnv::ChkUserNo(LPSTR chkbuf)
DllExport int prtectchkuserno(LPSTR chkbuf)
{
	return(0);
}

//int TScrnEnv::ChkMentNo(LPSTR chkbuf)
DllExport int prtectchkmentno(LPSTR chkbuf)
{
	return(0);
}

//int TScrnEnv::ChkSralNo(LPSTR chkbuf)
DllExport int prtectchksralno(LPSTR chkbuf)
{
	return(0);
}

