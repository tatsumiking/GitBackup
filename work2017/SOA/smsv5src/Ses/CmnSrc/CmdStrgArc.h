
#pragma once


#include "DialogStrgArc.h"

class CCmdStrgArc : public CCmdSpclStrg
{
public:
	CCmdStrgArc(CScrollView* pcview);
	~CCmdStrgArc();
	UINT ExecStrgSetArc(UINT event, DBL dx, DBL dy);
	UINT ExecStrgFreeArc(UINT event, DBL dx, DBL dy);
	void LocateStrgArcDlg();
	void TrnsStrgSetArc();
	void TrnsStrgFreeArc();
	void CreateStrgArcSetVect();
	void SetArcPoint();
	BOOL ArcCntrRagSet();
public:
	DBL		m_dR;
	DBL		m_dSRag, m_dERag;
	DBL		m_dSX, m_dSY;
	DBL		m_dEX, m_dEY;
	CDialogStrgArc *m_pcDialogStrgArc;
	CDataList	*m_pcDataList;
private:

};

