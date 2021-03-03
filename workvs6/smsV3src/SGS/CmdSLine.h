#ifndef __CMDSLINE_H
#define __CMDSLINE_H

typedef struct {
	double	xx, yy;
	double	len;
} crosstype;

class CFigure;

//C 線操作関係コマンド処理クラス
class CCmdSLine : public CCmdBase
{
public:
	CCmdSLine(CScrollView* pcview);
public:
	crosstype m_crs[RECMAX];
	int		m_ncrs;
	CVect	*m_pcVect;
	CVect	*m_pcVect2;
	CTypedPtrList<CObList, CFigure*> m_cFigureList;
	CTypedPtrList<CObList, CFigure*> m_cAddFigureList;
	CTypedPtrList<CObList, CFigure*> m_cAFigureList;
	CTypedPtrList<CObList, CFigure*> m_cBFigureList;
	CTypedPtrList<CObList, CFigure*> *m_pcCrtList;
	dlinetype m_cutline;

public:
	UINT ExecCutJoin(UINT event, DBL dx, DBL dy);
public:
private:
	void DrawSELine(CDC *pcDC);
	void CutJoinMain();
	void LinkFigure();
	CVect *TraceVect(CFigure *pcTopFg);
	void RemoveListFigure(CFigure *pcSrcFg);
	int VectSetFigure(CVect *pcVect, CFigure *pcFg, int pp);
	int VectReverseSetFigure(CVect *pcVect, CFigure *pcFg, int pp);
	CFigure *SharchNextFigure(DBL x, DBL y);
	void SortCrossSetFigure(crosstype * crs, int crsmax);
	void DivideGroup(dlinetype cutline);
	int CheckNKind(dlinetype ln, CFigure *pcFg);
	void GetCrossPointLinekind(dlinetype bln, CFigure *pcFg);
	void GetCrossPointBezekind(dlinetype ln, CFigure *pcFg);
	CFigure *SrchEPPairFigure(DBL x1, DBL y1);
	CFigure *SrchSPPairFigure(DBL x4, DBL y4);
	void AddCrossPoint(DBL xx, DBL yy);
	void SortCrossPoint();
	void DivFiguPtrList();
	void DivFiguElementVect(CVect *pcVect);
	void ConvertFGArcToBeze(dfigutype *fg, CFigure *pcFg);
	void ConvertFGFreeToCFigure(dfigutype *fg, CFigure *pcFg);
};

#endif


