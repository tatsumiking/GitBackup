#ifndef __CMDSLINE_H
#define __CMDSLINE_H

//C ���������i���s���Ȃǁj�֌W�R�}���h�����N���X
class CCmdSpclLine : public CCmdBase
{
public:
	CVect*	m_pcVect;
	CVect*	m_pcVect2;
	int		m_nPP, m_nPP2;

	CCmdSpclLine(CScrollView* pcview);
	UINT ExecPrepend(UINT event, DBL dx, DBL dy);
	UINT ExecParallel(UINT event, DBL dx, DBL dy);
public:
	void DrawLine(DBL sx, DBL sy, DBL ex, DBL ey);
	void ParallelMain(DBL dx, DBL dy);
	void PrependMain(DBL dx, DBL dy);
private:

};

#endif
