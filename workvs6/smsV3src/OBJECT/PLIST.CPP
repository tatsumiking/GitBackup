
//
// Easy Scanner System
//
// object plist.cpp  点ブロックリスト構造関係関数
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include "plist.h"

IMPLEMENT_SERIAL(CPList, CObject, 2)
CPList::CPList()
{
}

CPList::CPList(int nDummy)
{
	m_pNext = NULL;
	m_pPBlock = new CPBlock(nDummy);
}

CPList::~CPList()
{
	delete(m_pPBlock);
}

void CPList::Serialize(CArchive& ar)
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

CPList* CPList::GetNext()
{
	return(m_pNext);
}

void CPList::SetNext(CPList* pPList)
{
	m_pNext = pPList;
}

int CPList::GetCount()
{
	return(m_pPBlock->GetBlockCount());
}

int CPList::GetBlockCount()
{
	return(m_pPBlock->GetBlockCount());
}

void CPList::GetAtr(int idx, int *atr)
{
	m_pPBlock->GetAtr(idx, atr);
}

void CPList::SetAtr(int idx, int atr)
{
	m_pPBlock->SetAtr(idx, atr);
}

void CPList::GetAtrDXY(int idx, int *atr, DBL *x, DBL *y)
{
	m_pPBlock->GetAtrDXY(idx, atr, x, y);
}

void CPList::SetAtrDXY(int idx, int atr, DBL x, DBL y)
{
	m_pPBlock->SetAtrDXY(idx, atr, x, y);
}

void CPList::GetDXY(int idx, DBL *x, DBL *y)
{
	m_pPBlock->GetDXY(idx, x, y);
}

void CPList::SetDXY(int idx, DBL x, DBL y)
{
	m_pPBlock->SetDXY(idx, x, y);
}

