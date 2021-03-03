#ifndef __CMD_EDIT_H
#define __CMD_EDIT_H

class CCmdEdit : public CCmdBase
{
public:
	CCmdEdit(CScrollView* pcview);
public:
	UINT ExecCopyRPich(UINT event, DBL dx, DBL dy);
	UINT ExecCopyPich(UINT event, DBL dx, DBL dy);
	UINT ExecCopyPichNoMove(UINT event, DBL dx, DBL dy);
	UINT ExecMoveCood(UINT event, DBL dx, DBL dy);
	UINT ExecMoveRltv(UINT event, DBL dx, DBL dy);
	UINT ExecCopyCood(UINT event, DBL dx, DBL dy);
	UINT ExecCopyRltv(UINT event, DBL dx, DBL dy);
	UINT ExecMoveCntr(UINT event, DBL dx, DBL dy);
	UINT ExecPMove(UINT event, DBL dx, DBL dy);
	UINT ExecCopy(UINT event, DBL dx, DBL dy);
	UINT ExecMove(UINT event, DBL dx, DBL dy);
	UINT ExecDelete();
	virtual DBL CntrKindInit(DBL dx, DBL dy);
	virtual void CntrKindMove(DBL dx, DBL dy);

private:
	int	m_nComID;
	int m_nFirstFlag;
	DBL		m_dBsx, m_dBsy;
	int		m_nXSize, m_nYSize;
	CDC		*m_pMemDC;
	CBitmap	*m_pBitmap, *m_oldBitmap;
private:
	void DrawImage(CDC* pcDC);
	void MoveTrnsMain();
	int PltMoveOutInit();
	void PlotMoveChk(DBL dx, DBL dy);
	void PltMoveOutEnd();
	void BitmapCreate();
	void BitmapDelete();
	void DrawAreaInit(DBL dminx, DBL dminy, DBL dmaxx, DBL dmaxy);
	void DrawAreaEnd();
	void MemoryInit();
	void MemoryEnd();
};

#endif
