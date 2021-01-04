// smsoutDoc.h : CTonDoc クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include	"../../Object/Vect.h"
#include	"../../Object/DibCData.h"
#include	"../../Object/DataList.h"
#include	"../../Object/MojiVect.h"
#include	"../../Object/StrgVect.h"
#include	"../../Object/WakuVect.h"
#include	"../../Object/SWakVect.h"
#include	"../../Object/SPtnVect.h"
#include	"../../Object/DataGrup.h"
#include	"../../Object/DataLogo.h"
#include	"../../Object/DataFDib.h"
#include	"../../Object/MsurVect.h"
#include	"../../Object/ClipData.h"
#include	"../../Object/MetaData.h"

class CCrctWaku;

class CTonDoc : public CDocument
{
protected: // シリアライズ機能のみから作成します。
	CTonDoc();
	DECLARE_DYNCREATE(CTonDoc)
	virtual ~CTonDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// アトリビュート
public:
// オペレーション
public:

//オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTonDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	CTonView	*m_pcView;
	CSize	m_sizeDoc;
	DWORD	m_dwClr;
	int		m_nNoLoadDev;
	int		m_nActiveSave;
	int		m_nUndoNo;
	int		m_nError;
	int		m_nExitOK;
	int		m_nSaveWaku;
	CString	m_csGyousya;
	CString	m_csKouji;
	CString	m_csComent1;
	CString	m_csComent2;
	CString	m_csComent3;
	int		m_nNoMsgOut;

	TCHAR	m_szSystemName[RECMAX];
	TCHAR	m_szImageBmpFile[FNAMEMAX];
	CFSF1Font* m_pcFSF1Font;
	CESF0Font* m_pcESF0Font;
	CESF1Font* m_pcESF1Font;
	CFBU0Font* m_pcFBU0Font;
	CWinTTFont* m_pcWNTTFont;
	int		m_nTonnbo;
	DBL		m_dTopSpace;
	DBL		m_dBottomSpace;
	DBL		m_dLeftSpace;
	DBL		m_dRightSpace;

	int			m_nDBType;
	CString		m_csDBFileName;
	CString		m_csDBTableName;
	CTypedPtrList<CObList,CData*>     *m_pcObjDataList;
	CTypedPtrList<CObList,CData*>     m_tDataList;
	CTypedPtrList<CObList,CData*>     m_ObjDataList0;
	CTypedPtrList<CObList,CData*>     m_ObjDataList1;

	CList<CCrctWaku*, CCrctWaku*>     m_lstCWRead;
	CList<CCrctWaku*, CCrctWaku*>     m_lstCWSort;
	CList<CCrctWaku*, CCrctWaku*>     *m_pcListCW;
	int		m_nRecMax;


public:
	CSize GetDocSize();
	CData* SrchData(int flag, DBL x, DBL y);
	CData* SrchIdsData(int *nIDary, int nIDcnt, DBL x, DBL y);
	CData* SrchMoji(DBL x, DBL y, int* idx);
	void DataAllCls();
	BOOL DataActiveCls();
	CData *ConvertWakuToSWak(CWakuVect* pcWakuVect);
	CData *CheckSelectOne(int nSelectID);
	void DeleteWakuData();
	void SetCrtView(CTonView *pcView);
	void SaveFile(LPTSTR fname);
	void LoadFile(LPTSTR fname);
	void AreaInFlagOnOff(int flag, DBL sx, DBL sy, DBL ex, DBL ey);
	virtual void SetModifiedFlag();
	void SetFillClrFlagOn(int bitflag);
	void SetOutLineClrFlagOn(int bitflag);
	void LoadStringStoreFile(int id);
	void SaveStringStoreFile(int id);
	void WakuSave(TCHAR * wakufname);
	void WakuLoad(LPTSTR wakupath);
	void PointPicup();
	void PointYLegal(DBL yy);
	void PointXLegal(DBL xx);
	void AllOnPointFlag(int bitflag);
	void AllOffPointFlag(int bitflag);
	void LoadUndo();
	void SaveUndo();
	BOOL CheckUndo();
	void LoadRedo();
	void SaveRedo();
	BOOL CheckRedo();
	void OnFlagDataLink(int bitflag, CData *pData);
	void AreaInFlagOn(int flag, DBL sx, DBL sy, DBL ex, DBL ey);
	CDibCData* SrchDibData(DBL dx, DBL dy);
	void CopyData(DBL movex, DBL movey);
	CPoint GetTopPoint();
	BOOL CheckArchive();
	void LoadArchive();
	void SaveArchive();
	BOOL DataActiveCheck();
	CData* SrchDataPoint(DBL dx, DBL dy, int *retpp);
	CData* SrchDataOnLinePoint(DBL dx, DBL dy, DBL *retx, DBL *rety, int *pp);
	void MojiSaveAtnVect(LPTSTR fontname, UINT sjis, DBL sx, DBL sy, DBL size);
	void AtntionDraw(CDC *pcDC);
	void CheckFlagOutlineDraw(int flag, CDC *pcDC);
	void OffFlag(int bitflag);
	void OnFlag(int bitflag);
	void CheckOnFlag(int checkbitflag, int bitflag);
	void CheckOffFlag(int checkbitflag, int bitflag);
	void InAreaDataAtntionOn(DBL sx, DBL sy, DBL ex, DBL ey);
	void SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy);
	void GetDMiniMax(int flag, LPDBL minix, LPDBL miniy, LPDBL maxx, LPDBL maxy);
	void FitingData();
	CData* UseFitingVect(CVect *pcVect);
	void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	void MoveTrns(DBL movex, DBL movey);
	void RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	void MirrTrns(DBL a, DBL b, DBL t1,  DBL t2);
	void StrctBoxsTrns();
	void DataAddTail0(CData * pData);

	void DataAddTail(CData* pData);
	void DataAddHead(CData* pData);
	void ImageBmpFileCreate();

	void DeleteData(CData *pData);
	CData* NewData(int nItemID);

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CTonDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void InitSetList();
	void AddSortList(CCrctWaku* pcAddCW);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

