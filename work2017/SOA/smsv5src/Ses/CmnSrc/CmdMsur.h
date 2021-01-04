
#pragma once


#include "../../Object/MsurVect.h"


//C ���@���֌W�R�}���h�����N���X
class CCmdMsur : public CCmdBase
{
public:
	CCmdMsur(CScrollView* pcview);
	~CCmdMsur();
	UINT ExecXCreate(UINT event, DBL dx, DBL dy);
	UINT ExecYCreate(UINT event, DBL dx, DBL dy);
	UINT ExecAtrSet(UINT event, DBL dx, DBL dy);
public:
	CMsurVect* m_pcMsurVect;
	TCHAR m_szUnit[QRECMAX];
	int m_nFlotCount;
	DBL	m_dFontSize;
private:
	BOOL AtrSetMain();
	void MsurXAtrSet();
	void MsurYAtrSet();
};

