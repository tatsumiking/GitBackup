
#ifndef __RULEVECT_H
#define __RULEVECT_H

#include	"vect.h"

class AFX_CLASS_EXPORT CRuleVect : public CVect
{
public:
	int m_nStyle;	
protected:
	CRuleVect();
	DECLARE_SERIAL(CRuleVect)
public:
	CRuleVect(int nDummy);
	~CRuleVect();
public:
	void VectPlgnDraw(CDC *pcDC);
	virtual int CheckNullData();
	virtual int CheckID();
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual void OnDraw(CDC *pcDC);
	virtual void OutlineDraw(CDC *pcDC);
	virtual void PlgnDraw(CDC *pcDC);
	virtual void Serialize(CArchive& ar);
	virtual void SetStyle(int styleno);
	virtual int GetStyle();
private:
	void PlgnRejionDrawOut(CDC *pcDC);
	void PlgnNoRejionDrawOut(CDC *pcDC);
};

#endif
