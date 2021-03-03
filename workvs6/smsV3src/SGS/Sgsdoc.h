// sgsDoc.h : CSgsDoc �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(_SGSDOC_H_)
#define _SGSDOC_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include	"command.h"

#include	"..\object\vect.h"
#include	"..\object\dibcdata.h"
#include	"..\object\datalist.h"
#include	"..\object\mojivect.h"
#include	"..\object\strgvect.h"
#include	"..\object\wakuvect.h"
#include	"..\object\swakvect.h"
#include	"..\object\sptnvect.h"
#include	"..\object\mojitext.h"
#include	"..\object\strgtext.h"
#include	"..\object\swaktext.h"
#include	"..\object\sptntext.h"
#include	"..\object\datagrup.h"
#include	"..\object\datalogo.h"
#include	"..\object\rulevect.h"
#include	"..\object\datafdib.h"
#include	"..\object\msurvect.h"
#include	"..\object\clipdata.h"
#include	"..\object\namegrup.h"
#include	"..\object\metadata.h"
#include	"..\ses\DDBEdit.h"

class CTonSrvrItem;

class CTonDoc : public COleServerDoc
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CTonDoc();
	DECLARE_DYNCREATE(CTonDoc)

// �A�g���r���[�g
public:
	CTonSrvrItem* GetEmbeddedItem()
		{ return (CTonSrvrItem*)COleServerDoc::GetEmbeddedItem(); }

// �I�y���[�V����
public:

//�I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTonDoc)
	protected:
	virtual COleServerItem* OnGetEmbeddedItem();
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	CSize	m_sizeDoc;
	DWORD	m_dwClr;
	int		m_nActiveSave;
	int		m_nUndoNo;
	int		m_nError;
	int		m_nExitOK;
	CString	m_csGyousya;
	CString	m_csKouji;
	CString	m_csComent1;
	CString	m_csComent2;
	CString	m_csComent3;
	int		m_nNoMsgOut;
	int		m_nSaveWaku;
	int		m_nTonnbo;
	DBL		m_dTopSpace;
	DBL		m_dBottomSpace;
	DBL		m_dLeftSpace;
	DBL		m_dRightSpace;
	int			m_nDBType;
	CString		m_csDBFileName;
	CString		m_csDBTableName;
	CDialogDBEdit	*m_pcDialogDBEdit;

	char	m_szSystemName[RECMAX];
	CFSF1Font* m_pcFSF1Font;
	CESF0Font* m_pcESF0Font;
	CESF1Font* m_pcESF1Font;
	CWNTTFont* m_pcWNTTFont;
	CARCFont* m_pcARCFont;
public:
	void ResetVectLink();
	void UpdateDBObject();
	CData * ConvertWakuToSWak(CWakuVect *pcWakuVect);
	CData * CheckSelectOne(int nSelectID);
	void SaveFile(LPSTR fname);
	void LoadFile(LPSTR fname);
	void SetModifiedFlag();
	void SetFillClrFlagOn(int bitflag);
	void SetOutLineClrFlagOn(int bitflag);
	void LoadStringStoreFile(int id);
	void SaveStringStoreFile(int id);
	void WakuSave(char * wakufname);
	void WakuLoad(LPSTR wakupath);
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
	void AreaInFlagOnOff(int flag, DBL sx, DBL sy, DBL ex, DBL ey);
	CDibCData* SrchDibData(DBL dx, DBL dy);
	void CopyData(DBL movex, DBL movey);
	CPoint GetTopPoint();
	BOOL CheckArchive();
	void LoadArchive();
	void SaveArchive();
	BOOL DataActiveCheck();
	CData* SrchDataPoint(DBL dx, DBL dy, int *retpp);
	CData* SrchDataOnLinePoint(DBL dx, DBL dy, DBL *retx, DBL *rety, int *pp);
	void MojiSaveAtnVect(LPSTR fontname, UINT sjis, DBL sx, DBL sy, DBL size);
	void OffFlag(int bitflag);
	void OnFlag(int bitflag);
	void CheckOnFlag(int checkbitflag, int bitflag);
	void CheckOffFlag(int checkbitflag, int bitflag);
	void InAreaDataAtntionOn(DBL sx, DBL sy, DBL ex, DBL ey);
	void PointPicup();
	void PointYLegal(DBL yy);
	void PointXLegal(DBL xx);
	void SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy);
	void GetDMiniMax(int flag, LPDBL minix, LPDBL miniy, LPDBL maxx, LPDBL maxy);
	void FitingData();
	CData* UseFitingVect(CVect *pcVect);
	void SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey);
	void MoveTrns(DBL movex, DBL movey);
	void RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos);
	void MirrTrns(DBL a, DBL b, DBL t1,  DBL t2);
	void StrctBoxsTrns();
	void AddActiveBeforeLink(CData *pcAddData);
	void DataAddTail0(CData * pData);
	void DataAddTail(CData* pData);
	void DataAddHead(CData* pData);
	virtual ~CTonDoc();

	void DeleteData(CData *pData);
	CData* NewData(int nItemID);
	CTypedPtrList<CObList,CData*>     *m_pcObjDataList;
	CTypedPtrList<CObList,CData*>     m_tDataList;
	CTypedPtrList<CObList,CData*>     m_ObjDataList0;
	CTypedPtrList<CObList,CData*>     m_ObjDataList1;

	CSize GetDocSize();
	CData* SrchData(int flag, DBL x, DBL y);
	CData* SrchIdsData(int *nIDary, int nIDcnt, DBL x, DBL y);
	CData* SrchMoji(DBL x, DBL y, int* idx);
	void DataAllCls();
	BOOL DataActiveCls();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CTonDoc)
	afx_msg void OnFileClose();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// �������ꂽ OLE �f�B�X�p�b�` �}�b�v�֐�
	//{{AFX_DISPATCH(CTonDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(_SGSDOC_H_)