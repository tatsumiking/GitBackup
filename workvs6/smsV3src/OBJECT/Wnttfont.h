
#ifndef __WNTTFONT_H
#define __WNTTFONT_H               // Prevent multiple includes

#include	"fsf1font.h"

//#define	FTTFMESH	256
//#define MKAMONADDX	19

#define	FTTFMESH	8192
#define MKAMONADDX	608

class AFX_CLASS_EXPORT CWNTTFont : public CObject
{
protected:
	CWNTTFont();
	DECLARE_SERIAL(CWNTTFont)
public:
	void ParentSet(CWnd *pParent);
	virtual void Serialize(CArchive& ar);
public:
	HANDLE		m_hGlyphBuf;
	LPBYTE		m_lpGlyphBuf;
	long		m_lGlyphLen;

private:
	CWnd*		m_pcParent;
	CFSF1Font*	m_pcFSF1Font;
	int			m_nPointCount;
	long		m_lAllocGlyphLen;
	int			m_nTateYoko;
	char		m_sCrtFont[128];
	DBL			m_dCrtx, m_dCrty;
	DBL			m_dRsx, m_dRsy;
	DBL			m_dBezeaddtime;
	DBL			m_dXaddtime, m_dYaddtime;
	DBL			m_dXaddmove, m_dYaddmove;

public:
	CWNTTFont(int nDummy);
	~CWNTTFont();
	void FSF1ClassSet(CFSF1Font* pFSF1Font);
	void FontNameSet(LPSTR pFont);
	void BezeAddTimeSet(DBL time);
	void TrnsElementReset();
	void SetTateYoko(int flag);
	void SF1TrnsDataLoad(UINT sjis);
	void DataLoad(HDC hDC, UINT sjis);

	void GlyphToFSF1Trns();
	void PointSetStart();
	void PointSetEnd();
	void MovetoSet(LPPOINTFX lpcood);
	void PolygonSet(LPPOINTFX lpcood, WORD count);
	void ConicSet(LPPOINTFX lpcood, WORD count);
	void BezeSet(LPPOINTFX lpcood, WORD count);
	void PolyCurveSet(LPPOINTFX lpcood, WORD count);
	void B_splinePointSet(DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3);
	void LastjoinSet();
	void TTFcoodToSF1cood(DBL *retx, DBL *rety);
};

#endif

