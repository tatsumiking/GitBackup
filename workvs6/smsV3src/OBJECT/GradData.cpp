
//
// Easy Scanner System
//
// object graddata.cpp  グラデーションデータ関係関数
//
//  Copyright (c) 1998 by THEON

#include "stdafx.h"
#include "math.h"

#include "graddata.h"

extern "C" {
}

IMPLEMENT_SERIAL(CGradData, CObject, GRADDATAID)
CGradData::CGradData()
{
}

CGradData::CGradData(int nDummy)
	: CDibCData(nDummy)
{

	m_nKind = 0;
	m_dSX = 0;
	m_dSY = 0;
	m_dLen = 1;
	m_dRag = 0;
	m_clrRGB[0] = RGB(0,0,0);
	m_clrRGB[1] = RGB(255,255,255);
	m_dClrCtrlPos[0] = 0.0;
	m_dClrPos[0] = 0.0;
	m_dClrCtrlPos[1] = 50.0;
	m_dClrPos[1] = 100.0;
	m_nClrTblCount = 2;
}

CGradData::~CGradData()
{
}

int CGradData::CheckID()
{
	return(GRADDATAID);
}

void CGradData::Serialize(CArchive& ar)
{
	int		i;

	if (ar.IsStoring()){
		ar << m_nKind;
		ar << m_dSX;
		ar << m_dSY;
		ar << m_dLen;
		ar << m_dRag;
		ar << m_nClrTblCount;
		for(i = 0; i < m_nClrTblCount; i++){
			ar << m_dClrPos[i];
			ar << m_dClrCtrlPos[i];
			ar << m_clrRGB[i];
		}
	}
	else{
		ar >> m_nKind;
		ar >> m_dSX;
		ar >> m_dSY;
		ar >> m_dLen;
		ar >> m_dRag;
		ar >> m_nClrTblCount;
		for(i = 0; i < m_nClrTblCount; i++){
			ar >> m_dClrPos[i];
			ar >> m_dClrCtrlPos[i];
			ar >> m_clrRGB[i];
		}
	}
	CDibCData::Serialize(ar);
}

