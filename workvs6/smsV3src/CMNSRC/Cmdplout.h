#ifndef __CMDPLOUT_H
#define __CMDPLOUT_H

#include	"DlDvPOut.h"

#define	DIVLINEMAX	50
#define	XMOVE	1
#define YMOVE	2

class CCmdPloter : public CCmdBase
{
public:
	CCmdPloter(CScrollView* pcview);
	~CCmdPloter();
public:
	UINT ExecDivPlotOut(UINT event, DBL dx, DBL dy);
	UINT ExecPlotOut(UINT event, DBL dx, DBL dy);
	UINT ExecPlotMove(UINT event, DBL dx, DBL dy);
	void PlotOutMain(DBL areasx, DBL areasy, DBL areaex, DBL areaey);
private:
	int PltMoveOutInit();
	void PltMoveOutEnd();
	void PlotMoveChk(int idx);
	void PlotOutAreaBoxs(int nComID, DBL areasx, DBL areasy, DBL areaex, DBL areaey);
	void PlotOutTomboLine(int nComID, DBL sx, DBL sy, DBL ex, DBL ey);
	void PlotOutTonnbo(int nComID);
	void PlotOutKadoTonnbo(int nComID, DBL cx, DBL cy, DBL signex, DBL signey);
	void PlotOutCntrTonnbo(int nComID, DBL cx, DBL cy, DBL signex, DBL signey);
	void LocateDivPlotDlg();
	void DivPloterOutLoop();
	BOOL DivPlotAreaInit();
	void DrawAreaLine();
	void DrawDivAreaLine(CDC *pcDC);
	void SlctCrtAreaLine(DBL xx, DBL yy);
	void SortAreaLine();
	void SetCrtAreaLine(DBL x, DBL y);
	void ResetCrtAreaLine(DBL x, DBL y);
	void DrawDivBoxs(CDC *pcDC, DBL sx, DBL sy, DBL ex, DBL ey);
	void DrawDivLine(CDC *pcDC, DBL sx, DBL sy, DBL ex, DBL ey);
	void DrawDivDotLine(CDC *pcDC, DBL sx, DBL sy, DBL ex, DBL ey);
	void DrawDivCrossLine(CDC *pcDC, DBL sx, DBL sy, DBL ex, DBL ey);

private:
	CDialogDivPlotOut* m_pcDialogDivPlotOut;
	int			m_nComID;
	int			m_nChkIdx;

	DBL			m_dNoriSiro;
	DBL			m_dPlotXsize, m_dPlotYsize;
	int			m_nCheckFlag;
	int			m_nAreaBoxsOutFlag;
	int			m_nFileNo;
	int			m_nDataNo;

	int			m_nCrtMode, m_nCrtLine;
	DBL			m_tXdiv[DIVLINEMAX+2];
	DBL			m_tYdiv[DIVLINEMAX+2];
	int			m_nDivXCnt, m_nDivYCnt;
	DBL			m_dDivBaseX, m_dDivBaseY;
	DBL			m_dDivTopX, m_dDivTopY;
	DBL			m_dDivEndX, m_dDivEndY;
	int			m_nOneXsize, m_nOneYsize;
	int			m_nBackX, m_nBackY;
};

#endif
