/*
	Easy Scanner �V�X�e�����ʃ\�[�X	1992.12.01	by stan
base		figuenv.c	�}�`�֌W�ϐ���`�֐�
	Copyright ���j�n���}
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