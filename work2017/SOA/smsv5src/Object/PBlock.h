
#pragma once

#define ONEBLOCK		16

class CPBlock : public CObject
{
private:
	int	m_nPointAtr[ONEBLOCK];
	DBL	m_dPointX[ONEBLOCK];
	DBL	m_dPointY[ONEBLOCK];

protected:
	CPBlock();
	DECLARE_SERIAL(CPBlock)
public:
	CPBlock(int nDummy);
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


