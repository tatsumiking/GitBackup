
#ifndef __OLDRVECT_H
#define __OLDRVECT_H

#include	"vect.h"

class AFX_CLASS_EXPORT COldRuleVect : public CVect
{
public:
	int m_nKind;	
protected:
	COldRuleVect();
	DECLARE_SERIAL(COldRuleVect)
private:

public:
	virtual int CheckNullData();
	virtual void Serialize(CArchive& ar);
	virtual void OnDraw(CDC *pcDC);
	virtual void OutlineDraw(CDC *pcDC);
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual int CheckID();
	COldRuleVect(int nDummy);
	~COldRuleVect();
};

#endif
