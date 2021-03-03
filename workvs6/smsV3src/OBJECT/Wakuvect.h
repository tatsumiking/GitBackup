
#ifndef __WAKUVECT_H
#define __WAKUVECT_H

#include	"strgvect.h"

class AFX_CLASS_EXPORT CWakuVect : public CStrgVect
{
protected:
	CWakuVect();
	DECLARE_SERIAL(CWakuVect)
public:
	virtual void Serialize(CArchive& ar);

private:

public:
	int		m_nWakuNo;
	char	m_szMatching[RECMAX];
	int		m_nFixFlag;

public:
	void DelBoxs(CDC *pcDC);
	void StockClrDraw(CDC *pcDC);
	CWakuVect(int nDummy);
	~CWakuVect();
	virtual int CheckNullData();
	virtual void OnDraw(CDC *pcDC);
	virtual void OutlineDraw(CDC *pcDC);
	virtual void NoSelectDraw(CDC *pcDC);
	virtual void SelectDraw(CDC *pcDC);
	virtual void GetDMiniMax(DBL *minix, DBL *miniy, DBL *maxx, DBL *maxy);
	virtual int DrawNo(CDC *pcDC, int noidx);
	virtual CData* CopyData(DBL movex, DBL movey);
	virtual void SaveStrgStoreFile(FILE *fp, int id);
	virtual void LoadStrgStoreFile(FILE *fp, int id);
	virtual void WakuLoad(FILE *fp, LPSTR buf);
	virtual void OldWakuLoad(FILE *fp, LPSTR buf);
	virtual void WakuSave(FILE *fp);
	virtual void AtntionDraw(CDC *pcDC);
	virtual void NoactDraw(CDC *pcDC);
	virtual void DelDraw(CDC *pcDC);
	virtual void WakuBoxsDraw(CDC *pcDC);
	virtual int CheckID();
	void GetMatchStrg(LPSTR mstrg);
	void SetMatchStrg(LPSTR mstrg);
	virtual void GetTrnsPar(LPDBL par);
	virtual void SetTrnsPar(DBL par);
	virtual void SetString(char *pstr);
	void SetFixFlag(int flag);
	int CheckFixFlag();
};

#endif
