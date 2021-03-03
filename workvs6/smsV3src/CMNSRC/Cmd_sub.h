#ifndef __CMD_SUB_H
#define __CMD_SUB_H

//C 補助関係コマンド処理クラス
class CCmdSub : public CCmdBase
{
public:
	CCmdSub(CScrollView* pcview);
	~CCmdSub();
public:
	UINT ExecSubGrid(UINT event, DBL dx, DBL dy);
	UINT ExecSubAnyWaku(UINT event, DBL dx, DBL dy);
	UINT ExecSubCntrline(UINT event, DBL dx, DBL dy);
	void ResetWakuData();
	UINT ExecSubWaku(UINT event, DBL dx, DBL dy);
	UINT ExecSubWakuDel(UINT event, DBL dx, DBL dy);
	UINT ExecScrnClrSet(UINT event, DBL dx, DBL dy);
	UINT ExecSubLineFree(UINT event, DBL dx, DBL dy);
	UINT ExecSubLineFreerag(UINT event, DBL dx, DBL dy);
	UINT ExecSubLineRag(UINT event, DBL dx, DBL dy);
	UINT ExecSubLineCood(UINT event, DBL dx, DBL dy);
	UINT ExecSubLineRltv(UINT event, DBL dx, DBL dy);
	UINT ExecSubLineDel(UINT event, DBL dx, DBL dy);
	UINT ExecCntrLineFree(UINT event, DBL dx, DBL dy);
	UINT ExecCntrLineCood(UINT event, DBL dx, DBL dy);
	UINT ExecCntrLineRltv(UINT event, DBL dx, DBL dy);

private:
	CDialogInputNum*	m_pcDialogInputNum;
	CDataList*	m_pcWakuDataList;
	CDataList*	m_pcDataList;
	DBL		m_dWakuSpace;
	DBL		m_dCntrMM;
private:
	void CreateWakuBoxs();
	void SetBoxsPoint(CVect* pcVect);
	void GetFlagOnDMiniMax(int flag, LPDBL minix, LPDBL miniy, LPDBL maxx, LPDBL maxy);
};

#endif
