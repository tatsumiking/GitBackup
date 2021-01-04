
// SesDoc.h : CTonDoc クラスのインターフェイス
//


#pragma once

#include	"../Ses/Command.h"

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

#include	"../CmnSrc/DialogDBEdit.h"

class CCmdBase;

class CTonDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
	CTonDoc() noexcept;
	DECLARE_DYNCREATE(CTonDoc)
	DECLARE_MESSAGE_MAP()
public:
	virtual ~CTonDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// 属性
public:

// 操作
public:


// オーバーライド
	//{{AFX_VIRTUAL(CTonDoc)
protected:

public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS
	//}}AFX_VIRTUAL

// 実装
// インプリメンテーション
public:
	CTonView	*m_pcView;
	CCmdBase	*m_pcCmdBase;
	CSize	m_sizeDoc;
	DWORD	m_dwClr;
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
	TCHAR	m_szCrctFileName[FNAMEMAX];
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
	CDialogDBEdit	*m_pcDialogDBEdit;
	CTypedPtrList<CObList,CData*>     *m_pcObjDataList;
	CTypedPtrList<CObList,CData*>     m_tDataList;
	CTypedPtrList<CObList,CData*>     m_ObjDataList0;
	CTypedPtrList<CObList,CData*>     m_ObjDataList1;

	CSize GetDocSize();
	CData* SrchData(int flag, DBL x, DBL y);
	CData* SrchIdsData(int *nIDary, int nIDcnt, DBL x, DBL y);
	CData* SrchMoji(DBL x, DBL y, int* idx);
	void DataAllCls0();
	void DataAllCls();
	BOOL DataActiveCls();
	void ImageBmpFileCreate();

public:
	void CopyWakuFileBlock(LPTSTR lpSrcBaseName, LPTSTR lpDstBaseName);
	void UpdateDBObject();
	void ShowDialogDBEdit();
	void SetCrtView(CTonView *pcView);
	void DestroyDialogDBEdit();
	void LocateBottomFitDialog(CDialog *pcDialog);
	int CreateDialogDBEdit(CTonView *pcView);
	CData *ConvertWakuToSWak(CWakuVect* pcWakuVect);
	CData *CheckSelectOne(int nSelectID);
	void DeleteWakuData();
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
	void MojiSaveAtnVect(LPTSTR fontname, int code, DBL sx, DBL sy, DBL size);
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
	void SaveVectFile(LPTSTR lpVectFileName);

	void DeleteData(CData *pData);
	CData* NewData(int nItemID);

protected:

// 生成された、メッセージ割り当て関数
#ifdef SHARED_HANDLERS
	// 検索ハンドラーの検索コンテンツを設定するヘルパー関数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

protected:
	//{{AFX_MSG(CTonDoc)
	afx_msg void OnFileClose();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
};
