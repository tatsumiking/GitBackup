#ifndef __CMDDPRNT_H
#define __CMDDPRNT_H

//G　概要
//　プリンターへの分割出力の機能を提供する
//	このクラスの提供する機能により分割位置を
//　ある程度自由に設定することができる
//

#include	"DDivPrnt.h"

#define	DIVLINEMAX	50
#define	XMOVE	1
#define YMOVE	2

class CCmdDivPrint : public CCmdBase
{
public:
	CCmdDivPrint(CScrollView* pcview);
	~CCmdDivPrint();
public:
	UINT ExecDivPrint(UINT event, DBL dx, DBL dy);
private:
	void LocateDivPrintDlg();
	BOOL DivPrintAreaInit();
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
	CDialogDivPrint* m_pcDialogDivPrint;
	int			m_nComID;
	int			m_nChkIdx;

	DBL			m_dPrintXsize, m_dPrintYsize;
	DBL			m_dNoriSiro;
	DBL			m_dPrintMaxXsize, m_dPrintMaxYsize;
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
