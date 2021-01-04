
#pragma once


class CCmdSWak;

//C 指定関係コマンド処理クラス
class CCmdSlct : public CCmdBase
{
public:
	CCmdSlct(CScrollView* pcview);
	UINT ExecObj(UINT event, DBL dx, DBL dy);
	UINT ExecArea(UINT event, DBL dx, DBL dy);
	UINT ExecObjLineUpSlct(UINT event, DBL dx, DBL dy);
	UINT ExecAreaSlctMove(UINT event, DBL dx, DBL dy);
	UINT ExecPart(UINT event, DBL dx, DBL dy);
	UINT ExecAll();
	UINT ExecChng();
	UINT ExecLock();
	UINT ExecUnlock();
public:
	int m_nBackMode;
	int	m_nLocalCommand;
	CCmdSWak	*m_pcCmdSWak;
	DBL m_dX0, m_dY0;
	DBL m_dRSX, m_dRSY;
	DBL	m_dREX, m_dREY;
	DBL m_dBaseRag;

public:
	void SetSWakLocate();
	void ResetSWakVectAtr();
	void SetSWakVectAtr();
	UINT ExecAreaTrns(UINT event, DBL dx, DBL dy);
	UINT ExecAreaSlct(UINT event, DBL dx, DBL dy);
	UINT ExecAreaMove(UINT event, DBL dx, DBL dy);
	UINT ExecAreaRound(UINT event, DBL dx, DBL dy);
	int ActiveOnOffObject(DBL dx, DBL dy);
	BOOL CheckNearPoint(DBL sx, DBL sy, DBL ex, DBL ey);
private:
	void SetCuserKind(DBL dx, DBL dy);
	void DispSelectDataSize();
	int SetLocalCommand(DBL dx, DBL dy);
	void MainSizeTrns();
	void SetSlctRoundAreaPoint();
	void DrawAtnRoundBoxs(CDC *pcDC);
	void RoundSlctArea(DBL dx, DBL dy);
	void MainRoundTrns();

};

