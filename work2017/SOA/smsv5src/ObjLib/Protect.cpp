/*
	Easy Scanner システム共通ソース	1992.12.01	by stan
base		protect.c	プロテクト関係関数
	Copyright 株）ハリマ
*/

#include	"stdafx.h"

#include	"Protect.h"

//int TScrnEnv::ChkCntinel()
DllExport int prtectchkcntinel()
{
	return(OFF);
}

//int TScrnEnv::ChkUserNo(LPTSTR chkbuf)
DllExport int prtectchkuserno(LPTSTR chkbuf)
{
	return(0);
}

//int TScrnEnv::ChkMentNo(LPTSTR chkbuf)
DllExport int prtectchkmentno(LPTSTR chkbuf)
{
	return(0);
}

//int TScrnEnv::ChkSralNo(LPTSTR chkbuf)
DllExport int prtectchksralno(LPTSTR chkbuf)
{
	return(0);
}

