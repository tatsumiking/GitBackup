
#pragma once


#define	POINTSIZE	4
#define	POINTATR	0
#define	POINTX		1
#define	POINTY		2
#define	POINTBIT	3
#define	KAMONDEFSIZE	256
#define REJIONMASK	0x01
#define	KAMONREJION	0x10
#define	KAMONDATAMAX	1000

class CCmdKamon : public CCmdBase
{
public:
	CCmdKamon(CScrollView* pcview);
	UINT ExecEPSKamonLoad(UINT event, DBL dx, DBL dy);
	UINT ExecKamonLoad(UINT event, DBL dx, DBL dy);
	UINT ExecKamonSave(UINT event, DBL dx, DBL dy);
public:
	void EPSKamonLoadSetVect(LPTSTR filename);
	void ESKamonLoadSetVect(int slctno, DBL size);
	void ESKamonSave(int slctno);
private:

};

