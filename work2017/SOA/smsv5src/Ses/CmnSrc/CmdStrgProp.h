
#pragma once


#include "../CmnSrc/DibLib.h"
#include "../CmnSrc/DialogInputNum.h"

#include "../../Object/MojiVect.h"

#define	PREPOSTTBLSIZE	256
#define	BITMAPSIZE	256

class CCmdStrgProp : public CCmdBase
{
public:
	CCmdStrgProp(CScrollView* pcview);
	~CCmdStrgProp();
public:
	UINT ExecStrgProp(UINT event, DBL dx, DBL dy);
private:
	void StrgPropMain();
	void PropTableAlloc();
	void PropTableInit();
	void PropMoveMain(CMojiVect *pcMojiVect, DBL base, DBL dd);
	void PropTableCopy();
	void PropTableMiniMaxSet();
	void PropTableFree();
private:
	CStrgVect*	m_pcStrgVect;
	DBL			m_dSpace;
	int			m_nTateYoko;
	HANDLE		m_hPrePost;
	WORD FAR	*m_lpPrePost;
	WORD FAR	*m_lpPreTbl, *m_lpPostTbl, *m_lpBFTbl;
	CDialogInputNum *m_pcDialogInputNum;
	CDibLib		*m_pcDibLib;
};

