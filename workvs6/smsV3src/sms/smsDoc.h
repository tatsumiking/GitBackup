// smsDoc.h : CTonDoc �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(SMSDOC_H)
#define SMSDOC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

class CTonDoc : public CDocument
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	DECLARE_DYNCREATE(CTonDoc)
	CTonDoc();
public:
	virtual ~CTonDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

//�I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTonDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// �f�[�^
public:
	int		m_nError;

	int		m_nNoLoadDev;
	int		m_nActiveSave;
	CFSF1Font* m_pcFSF1Font;
	CESF0Font* m_pcESF0Font;
	CESF1Font* m_pcESF1Font;
	CWNTTFont* m_pcWNTTFont;
	CTypedPtrList<CObList,CData*>     *m_pcObjDataList;
	CTypedPtrList<CObList,CData*>     m_ObjDataList1;
	int		m_nTonnbo;
	DBL		m_dTopSpace;
	DBL		m_dBottomSpace;
	DBL		m_dLeftSpace;
	DBL		m_dRightSpace;
	int			m_nDBType;
	CString		m_csDBFileName;
	CString		m_csDBTableName;

// �C���v�������e�[�V����
public:
	void SetOutLineClrFlagOn(int bitflag);
	void SetFillClrFlagOn(int bitflag);
	CPoint GetTopPoint();
	CSize GetDocSize();
	void LoadUndo();
	void SaveUndo();
	void OnFlag(int flag);
	void OffFlag(int flag);
	CData* SrchMoji(DBL x, DBL y, int* idx);
	CData* SrchData(int flag, DBL x, DBL y);
	CData* SrchDataPoint(DBL dx, DBL dy, int *retpp);
	CData* SrchIdsData(int *nIDary, int nIDcnt, DBL x, DBL y);
	CData* SrchDataOnLinePoint(DBL dx, DBL dy, DBL *retx, DBL *rety, int *pp);
	void GetDMiniMax(int flag, LPDBL minix, LPDBL miniy, LPDBL maxx, LPDBL maxy);
	void LoadFile(LPSTR lpItiFileName);
	void DataAllCls();
	void DeleteData(CData *pData);
	void DataAddTail(CData *pcData);
	void WakuLoad(LPSTR wakufname);
	CData* NewData(int nItemID);
	CData *ConvertWakuToSWak(CWakuVect *pcWakuVect);

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CTonDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(SMSDOC_H)
