#ifndef __CMD_SLCT_H
#define __CMD_SLCT_H

//C 指定関係コマンド処理クラス
class CCmdSlct : public CCmdBase
{
public:
	CCmdSlct(CScrollView* pcview);
	UINT ExecObj(UINT event, DBL dx, DBL dy);
	UINT ExecArea(UINT event, DBL dx, DBL dy);
	UINT ExecAreaSlctMove(UINT event, DBL dx, DBL dy);
	UINT ExecPart(UINT event, DBL dx, DBL dy);
	UINT ExecAll();
	UINT ExecChng();
	UINT ExecLock();
	UINT ExecUnlock();
public:
	int ActiveOnOffObject(DBL dx, DBL dy);
	UINT ExecAreaTrns(UINT event, DBL dx, DBL dy);
	UINT ExecAreaSlct(UINT event, DBL dx, DBL dy);
	UINT ExecAreaMove(UINT event, DBL dx, DBL dy);
	int m_nBackMode;
	int	m_nLocalCommand;

private:
	void SetCuserKind(DBL dx, DBL dy);
	void DispSelectDataSize();
	int SetLocalCommand(DBL dx, DBL dy);
	void MainSizeTrns();
};

#endif
