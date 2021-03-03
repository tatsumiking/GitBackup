#ifndef __CMDSSTRG_H
#define __CMDSSTRG_H

#include "DlgStrVS.h"

class CCmdSpclStrg : public CCmdBase
{
public:
	CCmdSpclStrg(CScrollView* pcview);
	UINT ExecStrgLineStruct(UINT event, DBL dx, DBL dy);
public:
	BOOL m_bNoround;
	void LocateStrgDlg();
	void StrgLineStructMain();
	void SetLenStrgLineStructSpace();
	void SetLenStrgLineStructEqual();
	void StrgLineStructTrns();

	DBL GetVectLength();

	DBL GetVectPointLength(DBL xx, DBL yy);
	DBL GetRejionPointLength(int *flag, DBL xx, DBL yy);
	DBL GetNorejionPointLength(int *flag, DBL xx, DBL yy);
	DBL GetBezePointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	DBL GetArcPointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetFreeCurvePointLength(int *flag, DBL xx, DBL yy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetLinePointLength(int *flag, DBL xx, DBL yy, DBL x1, DBL y1, DBL x2, DBL y2);

	DBL GetVectPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy);
	DBL GetRejionPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy);
	DBL GetNorejionPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy);
	DBL GetBezePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
	DBL GetArcPointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetFreeCurvePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, int atr0, int atr1, int atr2, DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	DBL GetLinePointRagAndCood(LPDBL setlen, LPDBL dx, LPDBL dy, DBL x1, DBL y1, DBL x2, DBL y2);

	CVect *m_pcVect;
	CStrgVect *m_pcStrgVect;
	CDialogStrgVStruct *m_pcDialogStrgVStruct;

	int	m_nSetMode;
	BOOL m_bEqual;
	DBL	m_dLinelen;
	DBL	m_dStrglen;
	DBL	m_dStartlen;
	DBL	m_dTrnstime;
	DBL	m_dHalffontsize;
};

#endif
