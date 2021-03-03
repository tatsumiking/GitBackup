#ifndef __CMD_STRG_H
#define __CMD_STRG_H

#include "..\object\datagrup.h"
#include "DlgStrIn.h"
#include "DlStrIns.h"

//C 文字列処理関係コマンド処理クラス
class CCmdStrg : public CCmdBase
{
public:
	CCmdStrg(CScrollView* pcview);
	~CCmdStrg();
public:
	UINT ExecMojiSpace(UINT event, DBL dx, DBL dy);
	UINT ExecMojiInseart(UINT event, DBL dx, DBL dy);
	UINT ExecMojiDel(UINT event, DBL dx, DBL dy);
	UINT ExecStrgCreate(UINT event, DBL dx, DBL dy);
	UINT ExecStrgEdit(UINT event, DBL dx, DBL dy);
	UINT ExecStrgBoxsin(UINT event, DBL dx, DBL dy);
	UINT ExecStrgSlop(UINT event, DBL dx, DBL dy);
	UINT ExecStrgRound(UINT event, DBL dx, DBL dy);
	UINT ExecStrgOrder(UINT event, DBL dx, DBL dy);
	UINT ExecPlgning(UINT event, DBL dx, DBL dy);
	UINT ExecStrgSpace(UINT event, DBL dx, DBL dy);

public:
	void GetRealMojiSpace(int tategaki, int spacekind, DBL *mojispace);
	void MoveStringOder();
	void SetCntrXYCntrKind(DBL sx, DBL sy, DBL ex, DBL ey);
	void StrgSpaceMain(DBL space);
	BOOL ReadStrgSpace(LPDBL space);
	int MojiSpaceMain();
	void StrgInseartMain(LPSTR szInsStr, int mode);
	void StrgMojiInseart();
	void MojiDelMain();
	CData* StrgDataChange();
	void StrgElementReset();
	void StrgElementRestore();
	void AnyStrgCreate();
	void StrgBoxsDraw(CDC *pcDC);
	void LocateStrgDlg();
	void MoveStrgData();
	void DrawStdLine(CDC *pcDC);
	void StdLineInit();
	void StrgSlopMain();
	void StrgRoundMain();
	void StrgBoxsinMain();
private:
	int		m_nMode;
	CStrgVect*	m_pcStrgVect;
	CStrgVect*	m_pcStrgVect2;
	int		m_nMjidx;
	CVect*	m_pcVect;
	CDataGrup*	m_pcDataGrup;
	CDialogStrgInput*	m_pcDialogStrgInput;
	DBL		m_dStrgLen;
};

#endif
