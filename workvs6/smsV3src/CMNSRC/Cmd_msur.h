#ifndef __CMD_MSUR_H
#define __CMD_MSUR_H

#include "..\object\msurvect.h"

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
	char m_szUnit[QRECMAX];
	int m_nFlotCount;
	DBL	m_dFontSize;
private:
	BOOL AtrSetMain();
	void MsurXAtrSet();
	void MsurYAtrSet();
};

#endif
