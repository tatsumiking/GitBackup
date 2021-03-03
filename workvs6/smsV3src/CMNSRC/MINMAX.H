#ifndef __MINMAX_H
#define __MINMAX_H

//C ソート用クラス
class CMinMax : public CObject
{
public:
	CMinMax();
public:
	DBL		m_dMin, m_dMax;
	CData	*m_pcData;
private:


};

#endif
