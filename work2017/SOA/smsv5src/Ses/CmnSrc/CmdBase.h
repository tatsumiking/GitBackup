
#pragma once


//G　概要
//	アプリケーションが提供する機能の親クラス
//　各機能共通の機能を提供する
//

class CData;
class CVect;
class CDataList;
class CMojiVect;
class CStrgVect;
class CWakuVect;
class CSPtnVect;
class CSWakVect;
class CDataFDib;
class CDataLogo;
class CDataGrup;

class CMainFrame;
class CTonView;
class CTonDoc;

#define	AREADOT 4 

class CDataStruct
{
public:
	DBL		m_dCX, m_dCY;
	CData	*m_pcData;
};

class CPosStruct
{
public:
	POSITION	m_posBase;
};

//C コマンド処理クラスの基本クラス
class CCmdBase
{
public:
	CCmdBase(CView* pcview);
public:
	CTonView*	m_pcView;
	CTonDoc*	m_pcDoc;
	CTonApp*	m_pcApp;
	CMainFrame*	m_pcWnd;
	CSysEnv*	m_pcSysEnv;
	CDC*	m_pcDC;
	CTypedPtrList<CObList,CData*>     *m_pcPtrList;
	CTypedPtrList<CObList,CData*>     m_cPtrList;
	CTypedPtrList<CObList,CData*>     m_cTmpPtrList;
	CData*	m_pcData;
	int 	m_nMouseSetCount;
	int		m_nIDary[10];
	int		m_nIDcnt;
	DBL		m_dBmvx, m_dBmvy;
	DBL		m_dBX, m_dBY;
	DBL		m_dMBX, m_dMBY;
	DBL		m_dCX, m_dCY;
	DBL		m_dSX, m_dSY;
	DBL		m_dEX, m_dEY;
	DBL		m_dBSX, m_dBSY;
	DBL		m_dBEX, m_dBEY;
	DBL		m_dDBSX, m_dDBSY;
	DBL		m_dDBEX, m_dDBEY;
	TCHAR	m_szMsgBuf[RECMAX];
	int		m_nCntrKind;
	DBL		m_dXYTime;
	DBL		m_dXMove, m_dYMove;
	DBL		m_dX1, m_dY1;
	DBL		m_dX2, m_dY2;
	DBL		m_dX3, m_dY3;
	DBL		m_dX4, m_dY4;

public:
	void SetMousePoint(DBL dx, DBL dy);
	void InitClass();
	void LocateRightOverFitDialog(CDialog * pcDialog);
	void MoveMousePoint(DBL sx, DBL sy, DBL dx, DBL dy);
	void ActiveDataChange(int nID);
	void CheckSetLimit(int *num, int min, int max);
	void CheckSetLimit(DBL *size, DBL min, DBL max);
	virtual void SaveUndo();
	virtual void LoadUndo();
	virtual void ScrnGetCoodMoveXY(LPDBL movex, LPDBL movey);
	virtual void ScrnCnvMMToCood(LPDBL coodx, LPDBL coody);
	virtual void ScrnCnvCoodToMM(LPDBL mmx, LPDBL mmy);
	int DataListGetDMiniMax(LPDBL minx, LPDBL miny, LPDBL maxx, LPDBL maxy);
	void ActiveDataAllOff();
	virtual void LocateRightFitDialog(CDialog *pcDialog);
	virtual void LocateRightBottomFitDialog(CDialog *pcDialog);
	DBL m_dTimexpy;
	virtual UINT Exec(UINT event, DBL dx, DBL dy);
	virtual void AddLinkDoc(CData* pcData);
	virtual CData* SrchMoji(DBL dx, DBL dy, int *idx);
	virtual CData* SrchData(int flag, DBL dx, DBL dy);
	virtual CData* SrchIdsData(int *nIDary, int nIDcnt, DBL dx, DBL dy);
	virtual CData* SrchDataPoint(DBL dx, DBL dy, int *retpp);
	virtual CData* SrchDataOnLinePoint(DBL dx, DBL dy, DBL *retx, DBL *rety, int *retpp);
	virtual CData* SrchDataFromPtrList(int flag, DBL dx, DBL dy);
	virtual void DrawModeSET();
	virtual void DrawModeXOR();
	virtual void EndDraw();
	virtual void BeginDraw();
	virtual void DispSelectDataSize();
	virtual void HlpMsgDsp(LPTSTR msg);
	virtual void HlpMsgDsp(int no, LPTSTR msg);
	virtual void InitToolBarElement();
	virtual void ShowToolBar();
	virtual void HideToolBar();
	virtual void SetToolBarElement();
	virtual void GetToolBarElement();
	virtual void DrawAtnLine(CDC *pcDC);
	virtual void DrawAtnBoxs(CDC *pcDC);
	virtual void DrawBoxs(CDC *pcDC, DBL sx, DBL sy, DBL ex, DBL ey);
	virtual void InitDataAreaBoxs();
	virtual void DrawAreaBoxs(CDC *pcDC);
	virtual DBL CntrKindInit(DBL dx, DBL dy);
	virtual void CntrKindMove(DBL dx, DBL dy);
	virtual DBL CntrKindInitNew(DBL dx, DBL dy);
	virtual void CntrKindMoveNew(DBL dx, DBL dy);
	virtual void GetDataCenter(DBL minix, DBL miniy, DBL maxx, DBL maxy, LPDBL cx, LPDBL cy);
	virtual void ScrnAreaRedraw(DBL minix, DBL miniy, DBL maxx, DBL maxy);
	virtual void ScrnDataRedraw(CData *pcData);
	virtual void ScrnRedraw();
	virtual void ScrnAtntionRedraw();
	virtual void ScrnAllRedraw();
	virtual void ScrnSizeReset();
	virtual int SetDataListSelectID(int nSelectID, int nCheckFlag);
	virtual int SetDataListDispath(int nSelectID, CData *pcSelectData);
	virtual int SetDataListForDataGrup(int nSelectID, CDataGrup *pcSelectData);
	virtual int SetDataListForDataList(int nSelectID, CDataList *pcSelectData);
	virtual int SetDataListForStrgVect(int nSelectID, CStrgVect *pcSelectData);
	virtual int SetDataListForWakuVect(int nSelectID, CWakuVect *pcSelectData);
	virtual int SetDataListForSPtnVect(int nSelectID, CSPtnVect *pcSelectData);
	virtual int SetDataListForSWakVect(int nSelectID, CSWakVect *pcSelectData);
	virtual int SetDataListForMojiVect(int nSelectID, CMojiVect *pcSelectData);
	virtual int SetDataListForDataFDib(int nSelectID, CDataFDib *pcSelectData);
	virtual int SetDataListForDataLogo(int nSelectID, CDataLogo *pcSelectData);
	virtual int AreaCopy(CVect * srcVect, int spp, int epp, CVect * dstVect, int dp);
	virtual CData* DataChange(CData *pcData);
	DBL GetSumLength(CVect *pcVect, int sp, int ep);
	void GetLengthPoint(CVect *pcVect, int sp, int ep, DBL len, DBL *x, DBL *y);
};

