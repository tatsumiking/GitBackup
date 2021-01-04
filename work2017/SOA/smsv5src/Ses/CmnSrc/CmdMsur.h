
#pragma once


#include "../../Object/MsurVect.h"


//C 寸法線関係コマンド処理クラス
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

