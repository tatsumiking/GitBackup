#ifndef __CMD_EDIT_H
#define __CMD_EDIT_H

//G　概要
//　
//　機能を提供する
//

//C 編集（移動、複写）関係コマンド処理クラス
class CCmdEdit : public CCmdBaseBmp
{
public:
	CCmdEdit(CScrollView* pcview);
public:
	void MainMirrCopy();
	UINT ExecCopyRPich(UINT event, DBL dx, DBL dy);
	UINT ExecCopyPich(UINT event, DBL dx, DBL dy);
	UINT ExecMoveCood(UINT event, DBL dx, DBL dy);
	UINT ExecMoveRltv(UINT event, DBL dx, DBL dy);
	UINT ExecMove2PSlct(UINT event, DBL dx, DBL dy);
	UINT ExecCopyCood(UINT event, DBL dx, DBL dy);
	UINT ExecCopyMirror(UINT event, DBL dx, DBL dy);
	UINT ExecCopyRltv(UINT event, DBL dx, DBL dy);
	UINT ExecMoveCntr(UINT event, DBL dx, DBL dy);
	UINT ExecPMove(UINT event, DBL dx, DBL dy);
	UINT ExecCopy(UINT event, DBL dx, DBL dy);
	UINT ExecMove(UINT event, DBL dx, DBL dy);
	UINT ExecDelete();

private:
	int	m_nComID;
	int m_nFirstFlag;
	DBL		m_dRltvMoveX, m_dRltvMoveY;
	CData*	m_pcData;
	CVect*	m_pcVect;
	CVect*	m_pcVect2;
	int		m_nPP, m_nPP2;
private:
	virtual DBL CntrKindInit(DBL dx, DBL dy);
	virtual void CntrKindMove(DBL dx, DBL dy);
	void MoveTrnsMain();
	void Move2PointMain();
	int PltMoveOutInit();
	void PlotMoveChk(DBL dx, DBL dy);
	void PltMoveOutEnd();
	void DrawImage(CDC* pcDC);
	void BitmapCreate();
	void BitmapDelete();
};

#endif
