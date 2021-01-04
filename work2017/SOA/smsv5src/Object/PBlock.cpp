
//
// Easy Scanner System
//
// object pblock.cpp  連続点ブロック構造関係関数
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"

#include "PBlock.h"

IMPLEMENT_SERIAL(CPBlock, CObject, 1)
CPBlock::CPBlock()
{
}

CPBlock::CPBlock(int nDummy)
{
	int	idx;

	for(idx = 0; idx < ONEBLOCK; idx++){
		m_nPointAtr[idx] = 0;
		m_dPointX[idx] = 0.0;
		m_dPointY[idx] = 0.0;
	}
}

void CPBlock::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// ar << 
	}
	else
	{
		// ar >>
	}
}

int CPBlock::GetBlockCount()
{
	return((int)ONEBLOCK);
}

void CPBlock::GetAtr(int idx, int *atr)
{
	if(idx < 0 || ONEBLOCK <= idx){
		*atr = 0;
		return;
	}
	*atr = m_nPointAtr[idx];
}

void CPBlock::SetAtr(int idx, int atr)
{
	if(idx < 0 || ONEBLOCK <= idx){
		return;
	}
	m_nPointAtr[idx] = atr;
}

void CPBlock::GetAtrDXY(int idx, int *atr, DBL *x, DBL *y)
{
	if(idx < 0 || ONEBLOCK <= idx){
		*atr = 0; *x = 0.0; *y = 0.0;
		return;
	}
	*atr = m_nPointAtr[idx];
	*x = m_dPointX[idx];
	*y = m_dPointY[idx];
}

void CPBlock::SetAtrDXY(int idx, int atr, DBL x, DBL y)
{
	if(idx < 0 || ONEBLOCK <= idx){
		return;
	}
	m_nPointAtr[idx] = atr;
	m_dPointX[idx] = x;
	m_dPointY[idx] = y;
}

void CPBlock::GetDXY(int idx, DBL *x, DBL *y)
{
	if(idx < 0 || ONEBLOCK <= idx){
		*x = 0; *y = 0;
		return;
	}
	*x = m_dPointX[idx];
	*y = m_dPointY[idx];
}

void CPBlock::SetDXY(int idx, DBL x, DBL y)
{
	if(idx < 0 || ONEBLOCK <= idx){
		return;
	}
	m_dPointX[idx] = x;
	m_dPointY[idx] = y;
}

