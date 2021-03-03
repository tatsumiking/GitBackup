#ifndef __CMD_SPTN_H
#define __CMD_SPTN_H

#include	"..\object\sptnvect.h"

#define	ONESPTNSIZE	256

class CTonView;
class CTonDoc;

class CCmdSPtn : public CCmdBase
{
public:
	CCmdSPtn(CScrollView* pcview);
	~CCmdSPtn();
public:
	int	m_nSlctidx;
	DBL		m_dBtime;
	DBL		m_dBxmm, m_dBymm;
	DBL		m_dBsx, m_dBsy; 
	HANDLE	m_hDIB;
	CDC		*m_pcMemDC;
	HBITMAP	m_hBMP;
	long	m_dwXByte;
public:
	void DataGrupSet();
	UINT ExecGrupSet(UINT event, DBL dx, DBL dy);
	UINT ExecSave(UINT event, DBL dx, DBL dy);
	UINT ExecLoad(UINT event, DBL dx, DBL dy);
	UINT ExecDelete(UINT event, DBL dx, DBL dy);
	void DelCrtSlctSPtn();
	void LoadCrtSlctSPtn();
	void SaveCrtSlctSPtn();
	void SmdFileCreate(LPSTR ptnfilename);
	void SmdFileLoad(LPSTR ptnfilename);
	void SmallFileCreate(LPSTR bmpfilename);
	void SmallDrawAreaInit();
	void SmallDrawMain(LPSTR bmpfilename);
	void SmallDrawAreaEnd();
	void BmpFileDataOut(LPSTR bmpfilename);
	void DCAreaCls(HDC hDC, int sx, int sy, int ex, int ey);
};

#endif
