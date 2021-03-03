#ifndef __CROSPINT_H
#define __CROSPINT_H

//C 交点検出処理クラス
class CCrossPoint
{
public:
	CCrossPoint();
public:
	CVect	*m_pcVect;
	int		m_npp;
	DBL		m_dtt;
	DBL		m_dxx, m_dyy;
};

#endif
