
#pragma once


class CMinMax;
class CSWakVect;
class CDialogWakuInput;
class CDialogSlctStrg;

class CCmdWaku : public CCmdBase
{
public:
	CCmdWaku(CScrollView* pcview);
public:
	UINT ExecWakuGrupSet(UINT event, DBL dx, DBL dy);
	UINT ExecWakuPaperDel(UINT event, DBL dx, DBL dy);
	UINT ExecWakuPaperSet(UINT event, DBL dx, DBL dy);
	UINT ExecWakuLoad(UINT event, DBL dx, DBL dy);
	UINT ExecWakuSave(UINT event, DBL dx, DBL dy);
	UINT ExecWakuSort(UINT event, DBL dx, DBL dy);
	UINT ExecWakuDelete(UINT event, DBL dx, DBL dy);
	UINT ExecWakuInsort(UINT event, DBL dx, DBL dy);
	UINT ExecWakuInput(UINT event, DBL dx, DBL dy);
	int m_nDlg;
	void InitWakuAreaLegal();
	void MoveWakuLink();
	void DelDrawWakuBoxs(CDC *pcDC);
	void LocateWakuStrDlg();
	void SWakVectStringReset();
	void SetBoxsElement();
	void SetMode(int mode);
	void MoveWakuData();
	void DrawStdLine(CDC *pcDC);
	void StdLineInit();
	void WakuBoxsElementSet();
	void CntrKindMove(DBL dx, DBL dy);
	virtual DBL CntrKindInit(DBL dx, DBL dy);
	void DrawWakuBoxs(CDC *pcDC);
	void LoadWakuString();
	void SaveWakuString();

	void LoadCrtSlctWaku();
	void SaveCrtSlctWaku();
private:
	CTypedPtrList<CObList,CMinMax*>     m_cSrtList;
	CSWakVect*			m_pSortSWakVect[300];
	int					m_nSort;
	CSWakVect*			m_pcSWakVect;
	int					m_nMode;
	int					m_nDialogID;
	CDialogWakuInput*	m_pcDialogWakuInput;
	CDialogSlctStrg*	m_pcDialogSlctStrg;
	int		m_nSelectIdx;
	int		m_nConerKind;
	int		m_nStartKind;
	int		m_nCntrKind;
};


