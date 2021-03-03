//
// Easy Scanner System
//
// object oldrvect.cpp  旧罫線関係関数
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include <math.h>
#include "oldrvect.h"

extern "C" {
	int	senvcheckdrawmode();
}

IMPLEMENT_SERIAL(COldRuleVect, CObject, 3)
COldRuleVect::COldRuleVect()
{
}

COldRuleVect::COldRuleVect(int nDummy)
	: CVect(nDummy)
{
}

COldRuleVect::~COldRuleVect()
{
}

int COldRuleVect::CheckID()
{
	return(OLDRULEVECTID);
}

void COldRuleVect::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
	CVect::Serialize(ar);
	if (!(ar.IsStoring()))
	{
		RenewMiniMax();
	}
}

//p 内部データーが空の状態であれば削除しすべてが空のときTRUEを返す
int COldRuleVect::CheckNullData()
{
	if(CVect::CheckNullData() == TRUE){
		return(TRUE);
	}
	return(FALSE);
}

void COldRuleVect::GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy)
{
	DBL		width;

	CVect::GetDMiniMax(minix, miniy, maxx, maxy);
	width = GetOutLineWidth();
	*minix = *minix - width;
	*miniy = *miniy - width;
	*maxx = *maxx + width;
	*maxy = *maxy + width;
}

void COldRuleVect::OnDraw(CDC *pcDC)
{
	CVect::OnDraw(pcDC);
}

void COldRuleVect::OutlineDraw(CDC *pcDC)
{
	CVect::OutlineDraw(pcDC);
}

