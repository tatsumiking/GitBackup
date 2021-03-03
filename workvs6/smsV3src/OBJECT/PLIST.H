
#ifndef __PLIST_H
#define __PLIST_H

#include "pblock.h"

// 内部使用予定クラス宣言
//class CPBlock;

class CPList : public CObject
{
private:
	CPBlock*	m_pPBlock;
	CPList*		m_pNext;

protected:
	CPList();
	DECLARE_SERIAL(CPList)
public:
	CPList(int nDummy);
	~CPList();
	virtual CPList*	GetNext();
	virtual void SetNext(CPList* pPList);
	virtual int GetCount();
	virtual int GetBlockCount();
	virtual void GetAtr(int idx, int *atr);
	virtual void SetAtr(int idx, int atr);
	virtual void GetAtrDXY(int idx, int *atr, DBL *x, DBL *y);
	virtual void SetAtrDXY(int idx, int atr, DBL x, DBL y);
	virtual void GetDXY(int idx, DBL *x, DBL *y);
	virtual void SetDXY(int idx, DBL x, DBL y);
public:
	virtual void Serialize(CArchive& ar);
};

#endif

