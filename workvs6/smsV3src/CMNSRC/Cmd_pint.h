#ifndef __CMD_PINT_H
#define __CMD_PINT_H

//C 点操作関係コマンド処理クラス
class CCmdPoint : public CCmdBase
{
public:
	CVect*	m_pcVect;
	CVect*	m_pcVect2;
	CVect*	m_pcBackVect;
	int		m_nPP, m_nPP2;
	int		m_nAtr;

	CCmdPoint(CScrollView* pcview);

public:
	void SlopLegalMainShift(CVect *pcVect, int pp);
	void SlopLegalMain(CVect *pcVect, int pp);
	void Move2PointMain();
	UINT ExecYLegal(UINT event, DBL dx, DBL dy);
	UINT ExecXLegal(UINT event, DBL dx, DBL dy);
	UINT ExecSlopLegal(UINT event, DBL dx, DBL dy);
	UINT ExecPicup(UINT event, DBL dx, DBL dy);
	UINT Exec2Point(UINT event, DBL dx, DBL dy);
	UINT ExecPointMove(UINT event, DBL dx, DBL dy);
	UINT ExecPointDel(UINT event, DBL dx, DBL dy);
	UINT ExecPointAdd(UINT event, DBL dx, DBL dy);
	UINT ExecPointChngAtr(UINT event, DBL dx, DBL dy);
	UINT ExecAnyDel(UINT event, DBL dx, DBL dy);
	UINT ExecAnyMove(UINT event, DBL dx, DBL dy);
private:
	void StoreBackVect();
	void MainAnyPointMove(DBL movex, DBL movey);
	void MainAnyPointDelete();
	void RestoreBackVect();
	void PointAddMain(DBL dx, DBL dy);
	void PointDelMain();


};

#endif
