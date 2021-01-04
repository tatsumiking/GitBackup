
#pragma once


#include "../../Object/Vect.h"
#include "../../Object/DataList.h"

class CCmdFigu : public CCmdBase
{
public:
	CCmdFigu(CScrollView* pcview);
	~CCmdFigu();

public:
	void CreatePoint(DBL dx, DBL dy);
	UINT ExecSizeList(UINT event, DBL dx, DBL dy);
	UINT ExecSizeCrcl(UINT event, DBL dx, DBL dy);
	UINT ExecSizeBoxs(UINT event, DBL dx, DBL dy);
	UINT ExecPoint(UINT event, DBL dx, DBL dy);
	UINT ExecLine(UINT event, DBL dx, DBL dy);
	UINT ExecCrcl(UINT event, DBL dx, DBL dy);
	UINT ExecBoxs(UINT event, DBL dx, DBL dy);
	UINT ExecPlgn(UINT event, DBL dx, DBL dy);
	UINT ExecFree(UINT event, DBL dx, DBL dy);
// 内部使用変数
private:
	DBL		m_dCntrX;
	DBL		m_dCntrY;
	DBL		m_dWidth;
	DBL		m_dHight;
	DBL		m_dXSize;
	DBL		m_dYSize;
	int		m_nXCount;
	int		m_nYCount;
	CDataList*	m_pcDataList;
// 内部使用関数
private:
	void CreateLine(DBL dx, DBL dy);
	void SetLineEndPoint(DBL dx, DBL dy);
	void CreateBoxs(DBL dx, DBL dy);
	void SetBoxsEndPoint(DBL dx, DBL dy);
	void SetBoxsPoint(CVect *pcVect);
	void CreateCrcl(DBL dx, DBL dy);
	void SetCrclEndPoint(DBL dx, DBL dy);
	void SetCrclPoint(CVect *pcVect);
	void CreatePlgn(DBL dx, DBL dy);
	void SetPlgnEndPoint(DBL dx, DBL dy);
	void SetPlgnPoint(CVect *pcVect);
	void CreateFree(DBL dx, DBL dy);
	void SetFreePoint(int pp, DBL dx, DBL dy);
	void SetFreeEndPoint(int pp);
	void CreateList(DBL xsize, DBL ysize, int xcount, int ycount);
};


