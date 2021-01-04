// es98Doc.cpp : CTonDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	UNDOMAX		32

//extern "C" {
	#include "../../ObjLib/Baselib.h"
	#include "../../ObjLib/Mojienv.h"
	#include "../../ObjLib/Senvlib.h"
	#include "../../ObjLib/Scrnlib.h"
//}

/////////////////////////////////////////////////////////////////////////////
// CTonDoc

IMPLEMENT_DYNCREATE(CTonDoc, CDocument)

BEGIN_MESSAGE_MAP(CTonDoc, CDocument)
	//{{AFX_MSG_MAP(CTonDoc)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTonDoc �N���X�̍\�z/����

CTonDoc::CTonDoc()
{
	CTonApp*	pcApp;
	CMainFrame*	pcWnd;
	CFileException ex;

	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
	pcApp = (CTonApp*)AfxGetApp();
	pcWnd = (CMainFrame*)pcApp->GetMainWnd();
	m_dwClr = pcWnd->m_dwClr;
	_tcscpy_s(m_szSystemName, _T("�M�k�V�X�e��"));
	m_pcFSF1Font = pcWnd->m_pcFSF1Font;
	m_pcESF1Font = pcWnd->m_pcESF1Font;
	m_pcESF0Font = pcWnd->m_pcESF0Font;
	m_pcFBU0Font = pcWnd->m_pcFBU0Font;
	m_pcWNTTFont = pcWnd->m_pcWNTTFont;
	m_pcARCFont = pcWnd->m_pcARCFont;

	m_pcDialogDBEdit = NULL;

	m_nTonnbo = FALSE;
	m_dTopSpace = 0.0;
	m_dBottomSpace = 0.0;
	m_dLeftSpace = 0.0;
	m_dRightSpace = 0.0;

	pcWnd->InitItemElement();
	pcWnd->SetItemElement();
	m_sizeDoc = CSize(1, 1);
	m_pcObjDataList = &m_ObjDataList1;
	m_nNoMsgOut = FALSE;
	m_nSaveWaku = OFF;
	m_nActiveSave = OFF;
	m_nUndoNo = 1;

	EnableAutomation();
	AfxOleLockApp();

}

CTonDoc::~CTonDoc()
{
	AfxOleUnlockApp();
}

BOOL CTonDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTonDoc �V���A���C�[�[�V����

void CTonDoc::Serialize(CArchive& ar)
{
	int			nID;
	CData*		pcData;
	POSITION	pos;
	POSITION	nextpos;
	CString		csCheckString;
	TCHAR		szCheckString1[] = _T("Easy Scanner 98 Ver 1.000.00");
	TCHAR		szCheckString2[] = _T("�����΂� Ver 1.000.00");
	TCHAR		szCheckString3[] = _T("�M�k�V�X�e�� Ver 1.000.00");
	TCHAR		szCheckString4[] = _T("�M�k�V�X�e�� Ver 2.000.00");
	TCHAR		szCheckString5[] = _T("�M�k�V�X�e�� Ver 3.000.00");
	TCHAR		szCheckString6[] = _T("�M�k�V�X�e�� Ver 3.100.00");
	TCHAR		szCheckString7[] = _T("�M�k�V�X�e�� Ver 3.500.00");
	CTonView	*pcView;
	
	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	ar.m_pDocument = this;
	if (ar.IsStoring())
	{
		try
		{
			senvsetverjion(VER310);
			ar << CString(szCheckString7);
			ar << pcApp->m_clrBack;
			ar << m_dTopSpace;
			ar << m_dBottomSpace;
			ar << m_dLeftSpace;
			ar << m_dRightSpace;
			ar << m_nTonnbo;
			ar << m_nDBType;
			ar << m_csDBFileName;
			ar << m_csDBTableName;
			// �w�i�̂��߂̃f�[�^�u���b�N
			pos = m_ObjDataList0.GetHeadPosition();
			while (pos != NULL) {
				pcData = m_ObjDataList0.GetNext(pos);
				if (m_nActiveSave == OFF || pcData->CheckFlag(ACTIVEFLAG) == ON) {
					nID = pcData->CheckID();
					ar << nID;
					pcData->Serialize(ar);
				}
			}
			nID = NOOPDATAID;
			ar << nID;

			pos = m_pcObjDataList->GetHeadPosition();
			while (pos != NULL)
			{
				nextpos = pos;
				pcData = m_pcObjDataList->GetNext(nextpos);
				if(m_nActiveSave == OFF || pcData->CheckFlag(ACTIVEFLAG) == ON){
					nID = pcData->CheckID();
					ar << nID;
					pcData->Serialize(ar);
				}
				pos = nextpos;
			}
			nID = NOOPDATAID;
			ar << nID;
			// TODO: ���̈ʒu�ɕۑ��p�̃R�[�h��ǉ����Ă��������B
		}catch(CFileException *pcFileException){
			pcFileException->Delete();
			m_nError = TRUE;
		}catch(CException *pcException){
			pcException->Delete();
			m_nError = TRUE;
		}
	}else{
		m_nTonnbo = FALSE;
		senvsetverjion(VER100);
		ar >> csCheckString;
		if(lstrcmp(szCheckString1, csCheckString) == 0){
			pcApp->m_clrBack = -1;
		}else if(lstrcmp(szCheckString2, csCheckString) == 0){
			ar >> pcApp->m_clrBack;
		}else if(lstrcmp(szCheckString3, csCheckString) == 0){
			ar >> pcApp->m_clrBack;
		}else if(lstrcmp(szCheckString4, csCheckString) == 0){
			ar >> pcApp->m_clrBack;
			ar >> m_dTopSpace;
			ar >> m_dBottomSpace;
			ar >> m_dLeftSpace;
			ar >> m_dRightSpace;
		}else if(lstrcmp(szCheckString5, csCheckString) == 0){
			ar >> pcApp->m_clrBack;
			ar >> m_dTopSpace;
			ar >> m_dBottomSpace;
			ar >> m_dLeftSpace;
			ar >> m_dRightSpace;
			ar >> m_nDBType;
			ar >> m_csDBFileName;
			ar >> m_csDBTableName;
			senvsetverjion(VER300);
		}else if(lstrcmp(szCheckString6, csCheckString) == 0){
			ar >> pcApp->m_clrBack;
			ar >> m_dTopSpace;
			ar >> m_dBottomSpace;
			ar >> m_dLeftSpace;
			ar >> m_dRightSpace;
			ar >> m_nTonnbo;
			ar >> m_nDBType;
			ar >> m_csDBFileName;
			ar >> m_csDBTableName;
			senvsetverjion(VER310);
		}
		else if (_tcscmp(szCheckString7, csCheckString) == 0) {
			ar >> pcApp->m_clrBack;
			ar >> m_dTopSpace;
			ar >> m_dBottomSpace;
			ar >> m_dLeftSpace;
			ar >> m_dRightSpace;
			ar >> m_nTonnbo;
			ar >> m_nDBType;
			ar >> m_csDBFileName;
			ar >> m_csDBTableName;
			senvsetverjion(VER350);
			while (1) {
				ar >> nID;
				if (nID == NOOPDATAID) {
					break;
				}
				pcData = NewData(nID);
				pcData->Serialize(ar);
				pcData->RenewMiniMax();
				DataAddTail0(pcData);
			}
		}else{
			pos = GetFirstViewPosition();
			pcView = (CTonView *)(GetNextView(pos));
			pcView->MessageBox(_T("�t�@�C��������������܂���"), _T("�Ǎ��G���["), MB_OK);
			return;
		}
		while(1){
			ar >> nID;
			if(nID == NOOPDATAID){
				break;
			}
			pcData = NewData(nID);
			pcData->Serialize(ar);
			pcData->RenewMiniMax();
			nID = pcData->CheckID();
			if(nID == WAKUVECTID){
				pcData = ConvertWakuToSWak((CWakuVect*)pcData);
			}
			DataAddTail(pcData);
			if(scrncheckundoflag() == OFF){
				pcData->OnFlag(ACTIVEFLAG);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTonDoc �N���X�̐f�f

#ifdef _DEBUG
void CTonDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTonDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTonDoc �R�}���h
CData* CTonDoc::NewData(int nItemID)
{
	CData*		pcData;
	//CTonView	*pcView;

	pcData = NULL;
	switch(nItemID){
	case DIBCDATAID:
		pcData = (CData *)(new CDibCData(nItemID));
		break;
	case STRGVECTID:
		pcData = (CData *)(new CStrgVect(nItemID));
		break;
	case WAKUVECTID:
		pcData = (CData *)(new CWakuVect(nItemID));
		break;
	case SWAKVECTID:
		pcData = (CData *)(new CSWakVect(nItemID));
		break;
	case SPTNVECTID:
		pcData = (CData *)(new CSPtnVect(nItemID));
		break;
	case DATALISTID:
		pcData = (CData *)(new CDataList(nItemID));
		break;
	case DATAGRUPID:
		pcData = (CData *)(new CDataGrup(nItemID));
		break;
	case DATAFDIBID:
		pcData = (CData *)(new CDataFDib(nItemID));
		break;
	case DATALOGOID:
		pcData = (CData *)(new CDataLogo(nItemID));
		break;
	case BASEVECTID:
		pcData = (CData *)(new CVect(nItemID));
		break;
	case MOJIVECTID:
		pcData = (CData *)(new CMojiVect(nItemID));
		break;
	case MSURVECTID:
		pcData = (CData *)(new CMsurVect(nItemID));
		break;
	case METADATAID:
		pcData = (CData *)(new CMetaData(nItemID));
		break;
	}
	if(pcData != NULL){
		pcData->BaseFontLink(m_pcFSF1Font);
		pcData->WNTTFontLink(m_pcWNTTFont);
		pcData->ESF0FontLink(m_pcESF0Font);
		pcData->ESF1FontLink(m_pcESF1Font);
		pcData->FBU0FontLink(m_pcFBU0Font);
		pcData->ARCFontLink(m_pcARCFont);
		//POSITION pos = GetFirstViewPosition();
		//pcView = (CTonView *)(GetNextView(pos));
	}
	return(pcData);
}

void CTonDoc::DeleteData(CData *pData)
{
	POSITION	crtPosition, nextPosition;
	CData	*crtpData;

	crtPosition = m_pcObjDataList->GetHeadPosition();
	while(1){
		nextPosition = crtPosition;
		crtpData = (CData*)(m_pcObjDataList->GetNext(nextPosition));
		if(crtpData == pData){
			m_pcObjDataList->RemoveAt(crtPosition);
		}
		else{
			crtpData->DeleteData(pData);
		}
		if(nextPosition == NULL){
			break;
		}
		crtPosition = nextPosition;
	}
}

CSize CTonDoc::GetDocSize()
{
	DBL		rltvxsize, rltvysize;

	scrngetrltvsize(&rltvxsize, &rltvysize);
	m_sizeDoc = CSize((int)rltvxsize, (int)rltvysize);
	return m_sizeDoc;
}

CData* CTonDoc::SrchData(int flag, DBL x, DBL y)
{
	CData* pData;
	CData* pretData;
	POSITION pos;
	DBL		len, tlen;

	pretData = NULL;
	len = INTMAX;
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData == NULL){
			break;
		}
		if(flag == 0){
			tlen = INTMAX;
			pData->SrchData(x, y, &tlen);
			if(tlen < len){
				len = tlen;
				pretData = pData;
			}
		}
		else if(pData->CheckFlag(flag) == ON){
			tlen = INTMAX;
			pData->SrchData(x, y, &tlen);
			if(tlen < len){
				len = tlen;
				pretData = pData;
			}
		}
	}
	scrndlenmmtorltv(&len);
	if(NEARLIMIT < len){
		return(NULL);
	}
	return(pretData);
}

CData* CTonDoc::SrchIdsData(int *nIDary, int nIDcnt, DBL x, DBL y)
{
	CData* pData;
	CData* ptData;
	CData* pretData;
	POSITION pos;
	DBL		len, tlen;
	int		i, nID;

	pretData = NULL;
	len = INTMAX;
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData == NULL){
			break;
		}
		if(pData->CheckFlag(ACTIVEFLAG) == 	ON){
			tlen = INTMAX;
			nID = pData->CheckID();
			for(i = 0; i < nIDcnt; i++){
				if(nID == nIDary[i]){
					ptData = pData->SrchData(x, y, &tlen);
					break;
				}
			}
			if(i == nIDcnt){
				ptData = pData->SrchIdsData(nIDary, nIDcnt, x, y, &tlen);
			}
			if(tlen < len){
				len = tlen;
				pretData = ptData;
			}
		}
	}
	return(pretData);
}

CData* CTonDoc::SrchMoji(DBL x, DBL y, int *retidx)
{
	CData* pcData;
	CData* ptData;
	CData* pretData;
	POSITION pos;
	DBL		len, tlen;
	int		tidx;
	int		checkid;

	pretData = NULL;
	len = INTMAX;
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_pcObjDataList->GetNext(pos);
		if(pcData == NULL){
			break;
		}
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			tlen = INTMAX;
			checkid = pcData->CheckID();
			ptData = pcData->SrchMoji(x, y, &tlen, &tidx);
			if(tlen < len){
				len = tlen;
				*retidx = tidx;
				pretData = ptData;
			}
		}
	}
	return(pretData);
}

void CTonDoc::DataAllCls()
{
	CData*	pData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		m_pcObjDataList->RemoveAt(pos);
		delete(pData);
		pos = nextpos;
	}
}

BOOL CTonDoc::DataActiveCls()
{
	CData*		pData;
	POSITION	pos, nextpos;
	BOOL		ret;

	ret = TRUE;	//���ׂĂ��폜�ł����Ƃ�
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			m_pcObjDataList->RemoveAt(pos);
			delete(pData);
		}
		else{
			ret = FALSE;
		}
		pos = nextpos;
	}
	return(ret);
}

void CTonDoc::DataAddTail0(CData * pData)
{
}
void CTonDoc::DataAddTail(CData * pData)
{
	m_pcObjDataList->AddTail(pData);
	SetModifiedFlag();  // �t�@�C�������Ƃ��̊m�F�p�Ƀh�L�������g���X�V���ꂽ��
						// �ǂ����̃}�[�N��t���܂��B
}

void CTonDoc::DataAddHead(CData * pData)
{
	m_pcObjDataList->AddHead(pData);
	SetModifiedFlag();  // �t�@�C�������Ƃ��̊m�F�p�Ƀh�L�������g���X�V���ꂽ��
						// �ǂ����̃}�[�N��t���܂��B
}

void CTonDoc::ResetVectLink()
{
	POSITION	pos, delpos;
	CData		*pcData;
	int			nID;

	pos = m_pcObjDataList->GetHeadPosition();
	while(pos != NULL){
		delpos = pos;
		pcData = m_pcObjDataList->GetNext(pos);
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			nID = pcData->CheckID();
			switch(nID){
			case DATALISTID:
			case DATAGRUPID:
			case DATALOGOID:
				pcData->ResetVectLink();
				m_pcObjDataList->RemoveAt(delpos);
				delete(pcData);
				break;
			}
		}
	}
}

void CTonDoc::AddActiveBeforeLink(CData *pcAddData)
{
	POSITION	pos, tpos;
	CData		*pcData;

	pos = m_pcObjDataList->GetHeadPosition();
	while(pos != NULL){
		tpos = pos;
		pcData = m_pcObjDataList->GetNext(pos);
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			m_pcObjDataList->InsertBefore(tpos, pcAddData);
			break;
		}
	}
}

void CTonDoc::CopyData(DBL movex, DBL movey)
{
	CData*		pData;
	CData*		pnewData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		if(pData->CheckFlag(COPYFLAG) == ON){
			pnewData = pData->CopyData(movex, movey);
			if(pnewData != NULL){
				DataAddTail(pnewData);
			}
		}
		pos = nextpos;
	}
	
}

void CTonDoc::MirrTrns(DBL a, DBL b, DBL t1, DBL t2)
{
	CData*		pData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			pData->MirrorTrns(a, b, t1, t2);
		}
		pos = nextpos;
	}
}

void CTonDoc::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	CData*		pData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			pData->RoundTrns(cntrx, cntry, tsin, tcos);
		}
		pos = nextpos;
	}
}

void CTonDoc::MoveTrns(DBL movex, DBL movey)
{
	CData*		pData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			pData->MoveTrns(movex, movey);
		}
		pos = nextpos;
	}
}

void CTonDoc::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	CData*		pData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			pData->SizeTrns(cntrx, cntry, timex, timey);
		}
		pos = nextpos;
	}
}

void CTonDoc::FitingData()
{
	CData*		pcData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pcData = m_pcObjDataList->GetNext(nextpos);
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			pcData->StrgFiting();
		}
		pos = nextpos;
	}
}

CData* CTonDoc::UseFitingVect(CVect *pcVect)
{
	CData*		pcData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_pcObjDataList->GetNext(pos);
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			if(pcVect == pcData->GetFitingVect()){
				return(pcData);
			}
		}
	}
	return(NULL);
}

void CTonDoc::StrctBoxsTrns()
{
	CData*		pData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			pData->StrctBoxsTrns();
		}
		pos = nextpos;
	}
}

void CTonDoc::GetDMiniMax(int flag, LPDBL minix, LPDBL miniy, LPDBL maxx, LPDBL maxy)
{
	CData*		pData;
	POSITION	pos, nextpos;
	DBL			tminix, tminiy, tmaxx, tmaxy;

	(*minix) = INTMAX;
	(*miniy) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		if(flag == 0 || pData->CheckFlag(flag) == ON){
			tminix = INTMAX; tminiy = INTMAX;
			tmaxx = -INTMAX; tmaxy = -INTMAX;
			pData->GetDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
			(*minix) = min((*minix), tminix);
			(*miniy) = min((*miniy), tminiy);
			(*maxx) = max((*maxx), tmaxx);
			(*maxy) = max((*maxy), tmaxy);
		}
		pos = nextpos;
	}
}

void CTonDoc::SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
{
	CData*		pData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			pData->SlopTrns(cntrx, cntry, ddx, ddy);
		}
		pos = nextpos;
	}
}

void CTonDoc::AllOffPointFlag(int bitflag)
{
	CData*		pData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		pData->AllOffPointFlag(bitflag);
		pos = nextpos;
	}
}

void CTonDoc::AllOnPointFlag(int bitflag)
{
	CData*		pData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		pData->AllOnPointFlag(bitflag);
		pos = nextpos;
	}
}

void CTonDoc::OffFlag(int bitflag)
{
	CData*		pData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		pData->OffFlag(bitflag);
		pos = nextpos;
	}
}

void CTonDoc::OnFlag(int bitflag)
{
	CData*		pData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		pData->OnFlag(bitflag);
		pos = nextpos;
	}
}

void CTonDoc::CheckOnFlag(int checkbitflag, int bitflag)
{
	CData*		pData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		if(pData->CheckFlag(checkbitflag) == ON){
			pData->OnFlag(bitflag);
		}
		pos = nextpos;
	}
}

void CTonDoc::CheckOffFlag(int checkbitflag, int bitflag)
{
	CData*		pData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		if(pData->CheckFlag(checkbitflag) == ON){
			pData->OffFlag(bitflag);
		}
		pos = nextpos;
	}
}

void CTonDoc::MojiSaveAtnVect(LPTSTR fontname, UINT code, DBL sx, DBL sy, DBL size)
{
	int			sjis;
	CData*		pData;
	POSITION	pos, nextpos;
	int			nFontType;
	TCHAR		sFaceName[FACEMAX+2];
	int			ret;

#ifdef UNICODE
	sjis = baselibU16ToSjis(code);
#else
	sjis = code;
#endif
	mojienvflistopen(_T("r+b"));
	mojienvgetfonttypename(fontname, &nFontType, sFaceName);
	mojienvflistclose();
	switch(nFontType){
	case ES0FONT:
		if(m_pcESF0Font->UserDataExist(sjis) != FAIL){
			ret = ::AfxMessageBox(_T("�㏑�����Ă�낵���ł���"), MB_OKCANCEL);
			if(ret == IDCANCEL){
				return;
			}
		}
		break;
	case ES1FONT:
		if(m_pcESF1Font->UserDataExist(sjis) != FAIL){
			ret = ::AfxMessageBox(_T("�㏑�����Ă�낵���ł���"), MB_OKCANCEL);
			if(ret == IDCANCEL){
				return;
			}
		}
		break;
	case FB0FONT:
		if(m_pcFBU0Font->UserDataExist(code) != FAIL){
			ret = ::AfxMessageBox(_T("�㏑�����Ă�낵���ł���"), MB_OKCANCEL);
			if(ret == IDCANCEL){
				return;
			}
		}
		break;
	case TT1FONT:
	case TT2FONT:
	case TT3FONT:
		_tcscpy_s(sFaceName, fontname);
		sFaceName[8] = '\0';
		m_pcESF1Font->FontNameSet(sFaceName);
		if(m_pcESF1Font->UserDataExist(sjis) != FAIL){
			ret = ::AfxMessageBox(_T("�㏑�����Ă�낵���ł���"), MB_OKCANCEL);
			if(ret == IDCANCEL){
				return;
			}
		}
		break;
	case ARCFONT:
	case MAXFONT:
		_tcscpy_s(sFaceName, fontname);
		sFaceName[8] = '\0';
		m_pcESF1Font->FontNameSet(sFaceName);
		if(m_pcESF1Font->UserDataExist(sjis) != FAIL){
			ret = ::AfxMessageBox(_T("�㏑�����Ă�낵���ł���"), MB_OKCANCEL);
			if(ret == IDCANCEL){
				return;
			}
		}
		break;
	}
	m_pcFSF1Font->PointSetStart();
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		m_pcFSF1Font->TrnsElementSet(sx, sy, size, size);
		pData->SetAtnVectFSF1fBuf(m_pcFSF1Font);
		pos = nextpos;
	}
	m_pcFSF1Font->PointSetEnd();

	switch(nFontType){
	case ES0FONT:
		m_pcESF0Font->FontNameSet(sFaceName);
		m_pcESF0Font->TrnsCopyFSF1FBuf();
		m_pcESF0Font->UserDataSave(sjis);
		break;
	case ES1FONT:
		m_pcESF1Font->FontNameSet(sFaceName);
		m_pcESF1Font->TrnsCopyFSF1FBuf();
		m_pcESF1Font->UserDataSave(sjis);
		break;
	case FB0FONT:
		m_pcFBU0Font->FontNameSet(sFaceName);
		m_pcFBU0Font->TrnsCopyFSF1FBuf();
		m_pcFBU0Font->UserDataSave(code);
		break;
	case TT1FONT:
	case TT2FONT:
	case TT3FONT:
		_tcscpy_s(sFaceName, fontname);
		sFaceName[8] = _T('\0');
		m_pcESF1Font->FontNameSet(sFaceName);
		m_pcESF1Font->TrnsCopyFSF1FBuf();
		m_pcESF1Font->UserDataSave(sjis);
		break;
	case ARCFONT:
	case MAXFONT:
		_tcscpy_s(sFaceName, fontname);
		sFaceName[8] = _T('\0');
		m_pcESF1Font->FontNameSet(sFaceName);
		m_pcESF1Font->TrnsCopyFSF1FBuf();
		m_pcESF1Font->UserDataSave(sjis);
		break;
	}
}

CData *CTonDoc::SrchDataOnLinePoint(DBL dx, DBL dy, DBL *xx, DBL *yy, int * pp)
{
	CData* pData;
	CData* pretData;
	CData* tpretData;
	POSITION pos;
	DBL		len, tlen;
	DBL		tx, ty;
	int		tpp;

	len = INTMAX;
	pretData = NULL;
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData == NULL){
			break;
		}
		if(pData->CheckFlag(ACTIVEFLAG) == 	ON){
			tlen = INTMAX;
			tpretData = pData->SrchNearOnLinePoint(dx, dy, &tlen, &tx, &ty, &tpp);
			if(tlen < len){
				len = tlen;
				*xx = tx;
				*yy = ty;
				*pp = tpp;
				pretData = tpretData;
			}
		}
	}
	return(pretData);
}

CData* CTonDoc::SrchDataPoint(DBL dx, DBL dy, int * retpp)
{
	CData* pData;
	CData* pretData;
	CData* tpretData;
	POSITION pos;
	DBL		len, tlen;
	int		tpp;

	pretData = NULL;
	len = INTMAX;
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData == NULL){
			break;
		}
		if(pData->CheckFlag(ACTIVEFLAG) == 	ON){
			tlen = INTMAX;
			tpretData = pData->SrchNearPoint(dx, dy, &tlen, &tpp);
			if(tlen < len){
				len = tlen;
				*retpp = tpp;
				pretData = tpretData;
			}
		}
	}
	return(pretData);
}

BOOL CTonDoc::DataActiveCheck()
{
	CData* pData;
	POSITION pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData == NULL){
			break;
		}
		if(pData->CheckFlag(ACTIVEFLAG) == 	ON){
			return(TRUE);
		}
	}
	return(FALSE);	
}

void CTonDoc::SaveArchive()
{
	TCHAR	execpath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];

	m_nActiveSave = ON;
	senvgetexepath(execpath);
	_stprintf_s(fname, _T("%s\\archive.ess"), execpath);
	CFile file(fname,  CFile::modeCreate|CFile::modeWrite);
	CArchive ar(&file, CArchive::store);
	Serialize(ar);
	m_nActiveSave = OFF;
}

void CTonDoc::LoadArchive()
{
	TCHAR	execpath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];

	senvgetexepath(execpath);
	_stprintf_s(fname, _T("%s\\archive.ess"), execpath);
	CFile file(fname, CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	Serialize(ar);
}

BOOL CTonDoc::CheckArchive()
{
	TCHAR	execpath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	CFileException ex;

	senvgetexepath(execpath);
	_stprintf_s(fname, _T("%s\\archive.ess"), execpath);
	CFile file;
	if(file.Open(fname, CFile::modeRead, &ex)){
		return(TRUE);
	}
	return(FALSE);
}

CPoint CTonDoc::GetTopPoint()
{
	DBL	rltvx, rltvy;
	CPoint	point;

	scrngetrltvtop(&rltvx, &rltvy);
	point = CPoint((int)rltvx, (int)rltvy);

	CTonApp*	theApp;
	CMainFrame*	theWnd;
	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	TCHAR	msg[64];
	_stprintf_s(msg, _T("%4.0lf %4.0lf"), rltvx, rltvy);
	theWnd->HelpTextOut(STATBARHELP, msg);

	return point;
}

CDibCData* CTonDoc::SrchDibData(DBL dx, DBL dy)
{
	CData* pData;
	CData* pretData;
	POSITION pos;
	DBL		len, tlen;

	pretData = NULL;
	len = INTMAX;
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData == NULL){
			break;
		}
		if(pData->CheckFlag(ACTIVEFLAG) == 	ON){
			if(pData->CheckID() == DIBCDATAID){
				pData->SrchData(dx, dy, &tlen);
				if(tlen < len){
					len = tlen;
					pretData = pData;
				}
			}
		}
	}
	return((CDibCData*)pretData);
}

void CTonDoc::AreaInFlagOn(int bitflag, DBL sx, DBL sy, DBL ex, DBL ey)
{
	CData*		pData;
	POSITION	pos, nextpos;
	DBL			minix, miniy, maxx, maxy;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		pData->GetDMiniMax(&minix, &miniy, &maxx, &maxy);
		if(sx < minix && maxx < ex
		&& ey < miniy && maxy < sy){
			pData->OnFlag(bitflag);
		}
		pos = nextpos;
	}
}

void CTonDoc::AreaInFlagOnOff(int bitflag, DBL sx, DBL sy, DBL ex, DBL ey)
{
	CData*		pcData;
	POSITION	pos, nextpos;
	DBL			minix, miniy, maxx, maxy;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pcData = m_pcObjDataList->GetNext(nextpos);
		pcData->GetDMiniMax(&minix, &miniy, &maxx, &maxy);
		if(sx < minix && maxx < ex
		&& ey < miniy && maxy < sy){
			if(pcData->CheckFlag(bitflag) == ON){
				pcData->OffFlag(bitflag);
			}
			else{
				pcData->OnFlag(bitflag);
			}
		}
		pos = nextpos;
	}
}

void CTonDoc::OnFlagDataLink(int bitflag, CData *pLinkData)
{
	CData*		pcData;
	CData*		pcSubData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pcData = m_pcObjDataList->GetNext(nextpos);
		if(pcData->CheckFlag(bitflag) == ON){
			m_pcObjDataList->RemoveAt(pos);
			if(pcData->CheckID() == DATAGRUPID){
				pcSubData = ((CDataGrup*)pcData)->m_pcDataTop;
				pLinkData->SetData(pcSubData);
				((CDataGrup*)pcData)->m_pcDataTop = NULL;
				delete(pcData);
			}
			else{
				pLinkData->SetData(pcData);
			}
		}
		pos = nextpos;
	}
}

void CTonDoc::SaveUndo()
{
	TCHAR	execpath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];

	senvgetexepath(execpath);
	_stprintf_s(fname, _T("%s\\undo%04d.sgs"), execpath, m_nUndoNo);
	CFile file(fname,  CFile::modeCreate|CFile::modeWrite);
	CArchive ar(&file, CArchive::store);
	scrnonundoflag();
	Serialize(ar);
	scrnoffundoflag();
	ar.Close();
	file.Close();
	m_nUndoNo++;
	if(m_nUndoNo > UNDOMAX){
		m_nUndoNo = 1;
	}
}

void CTonDoc::LoadUndo()
{
	TCHAR	execpath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];

	if(m_nUndoNo == 0){
		return;
	}
	DataAllCls();
	senvgetexepath(execpath);
	m_nUndoNo--;
	if(m_nUndoNo < 1){
		m_nUndoNo = UNDOMAX;
	}

	_stprintf_s(fname, _T("%s\\undo%04d.sgs"), execpath, m_nUndoNo);
	CFile file(fname, CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	scrnonundoflag();
	Serialize(ar);
	scrnoffundoflag();
	_tremove(fname);
	ar.Close();
	file.Close();
}

BOOL CTonDoc::CheckUndo()
{
	TCHAR	execpath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	CFileException ex;
	int		chkno;

	if(m_nUndoNo == 0){
		return(FALSE);
	}
	chkno = m_nUndoNo-1;
	if(chkno < 1){
		chkno = UNDOMAX;
	}

	senvgetexepath(execpath);
	_stprintf_s(fname, _T("%s\\undo%04d.sgs"), execpath, chkno);
	CFile file;
	if(file.Open(fname, CFile::modeRead, &ex)){
		file.Close();
		return(TRUE);
	}
	return(FALSE);
}

void CTonDoc::SaveRedo()
{
	TCHAR	execpath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];

	senvgetexepath(execpath);
	_stprintf_s(fname, _T("%s\\undo%04d.sgs"), execpath, m_nUndoNo);
	CFile file(fname,  CFile::modeCreate|CFile::modeWrite);
	CArchive ar(&file, CArchive::store);
	scrnonundoflag();
	Serialize(ar);
	scrnoffundoflag();
	ar.Close();
	file.Close();
}

void CTonDoc::LoadRedo()
{
	TCHAR	execpath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];

	m_nUndoNo++;
	if(m_nUndoNo > UNDOMAX){
		m_nUndoNo = 1;
	}
	DataAllCls();
	senvgetexepath(execpath);
	_stprintf_s(fname, _T("%s\\undo%04d.sgs"), execpath, m_nUndoNo);
	CFile file(fname, CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	scrnonundoflag();
	Serialize(ar);
	scrnoffundoflag();
	_tremove(fname);
	ar.Close();
	file.Close();
}

BOOL CTonDoc::CheckRedo()
{
	TCHAR	execpath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	CFileException ex;
	int		chkno;

	chkno = m_nUndoNo + 1;
	if(chkno > UNDOMAX){
		chkno = 1;
	}
	senvgetexepath(execpath);
	_stprintf_s(fname, _T("%s\\undo%04d.sgs"), execpath, chkno);
	CFile file;
	if(file.Open(fname, CFile::modeRead, &ex)){
		return(TRUE);
	}
	return(FALSE);
}

void CTonDoc::PointXLegal(DBL xx)
{
	CData*		pData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		pData->PointXLegal(xx);
		pos = nextpos;
	}
}

void CTonDoc::PointYLegal(DBL yy)
{
	CData*		pData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		pData->PointYLegal(yy);
		pos = nextpos;
	}

}

void CTonDoc::PointPicup()
{
	CData*		pData;
	POSITION	pos, nextpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		pData->PointPicup();
		pos = nextpos;
	}

}

void CTonDoc::WakuLoad(LPTSTR wakufname)
{
	FILE	*fp;
	TCHAR	buf[RECMAX4];
	CSWakVect*	cpSWakVect;

	_tfopen_s(&fp, wakufname, _T("r"));
	if (fp == NULL) {
		return;
	}
	while(1){
		if(_fgetts(buf, RECMAX4, fp) == NULL){
			break;
		}
		cpSWakVect = (CSWakVect*)NewData(SWAKVECTID);
		if(cpSWakVect == NULL){
			return;
		}
		cpSWakVect->WakuLoad(fp, buf);
		DataAddTail((CData*)cpSWakVect);
	}
	fclose(fp);	
	return;

}

void CTonDoc::WakuSave(TCHAR * wakufname)
{
	CData*		pData;
	POSITION	pos, nextpos;
	BOOL		ret;
	FILE		*fp;

	_tfopen_s(&fp, wakufname, _T("w"));
	if (fp == NULL) {
		return;
	}
	ret = TRUE;	//���ׂĂ��폜�ł����Ƃ�
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			pData->WakuSave(fp);
		}
		pos = nextpos;
	}
	fclose(fp);	

	return;
}

void CTonDoc::SaveStringStoreFile(int id)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	CData* pData;
	POSITION pos;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\string.lst"), exepath);
	_tfopen_s(&fp, fname, _T("wb"));
	if (fp == NULL) {
		return;
	}
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData == NULL){
			break;
		}
		if(pData->CheckFlag(ACTIVEFLAG) == 	ON){
			pData->SaveStrgStoreFile(fp, id);
		}
	}
	fclose(fp);
}

void CTonDoc::LoadStringStoreFile(int id)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	CData* pData;
	POSITION pos;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\string.lst"), exepath);
	_tfopen_s(&fp, fname, _T("rb"));
	if (fp == NULL) {
		return;
	}
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData == NULL){
			break;
		}
		if(pData->CheckFlag(ACTIVEFLAG) == 	ON){
			pData->LoadStrgStoreFile(fp, id);
		}
	}
	fclose(fp);
}

void CTonDoc::SetOutLineClrFlagOn(int bitflag)
{
	CData* pData;
	POSITION pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData == NULL){
			break;
		}
		if(pData->CheckFlag(bitflag) ==	ON){
			pData->SetDefOutLineClr();
		}
	}
}

void CTonDoc::SetFillClrFlagOn(int bitflag)
{
	CData* pData;
	POSITION pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData == NULL){
			break;
		}
		if(pData->CheckFlag(bitflag) ==	ON){
			pData->SetDefFillClr();
		}
	}
}

void CTonDoc::InAreaDataAtntionOn(DBL sx, DBL sy, DBL ex, DBL ey)
{
	CData*		pData;
	POSITION	pos, nextpos;
	BOOL		ret;

	ret = TRUE;	//���ׂĂ��폜�ł����Ƃ�
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			pData->InAreaDataAtntionOn(sx, sy, ex, ey);
		}
		pos = nextpos;
	}
}

void CTonDoc::OnFileClose() 
{
	TCHAR	datpath[FNAMEMAX];

	senvgetdatpath(datpath);
	SetCurrentDirectory(datpath);
	CDocument::OnFileClose();
}

void CTonDoc::OnFileSave() 
{
	TCHAR	datpath[FNAMEMAX];

	senvgetdatpath(datpath);
	SetCurrentDirectory(datpath);
	CDocument::OnFileSave();
}

void CTonDoc::OnFileSaveAs() 
{
	TCHAR	datpath[FNAMEMAX];

	senvgetdatpath(datpath);
	SetCurrentDirectory(datpath);
	CDocument::OnFileSaveAs();
}

void CTonDoc::SetModifiedFlag()
{

}

void CTonDoc::LoadFile(LPTSTR fname)
{
	CFile file(fname, CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	Serialize(ar);
	ar.Close();
	file.Close();
}

void CTonDoc::SaveFile(LPTSTR fname)
{
	TCHAR	msg[RECMAX];

	CFile file;
	if(m_nNoMsgOut == FALSE){
		if(file.Open(fname,  CFile::modeRead, NULL)){
			file.Close();
			_stprintf_s(msg, _T("%s�ɏ㏑�����Ă���낵���ł���"), fname);
			if(AfxMessageBox(msg, MB_OKCANCEL) == IDCANCEL){
				return;
			}
		}
	}
	file.Open(fname,  CFile::modeCreate|CFile::modeWrite, NULL);
	CArchive ar(&file, CArchive::store);

	Serialize(ar);

	ar.Close();
	file.Close();
}

CData * CTonDoc::CheckSelectOne(int nSelectID)
{
	return(NULL);
}

CData * CTonDoc::ConvertWakuToSWak(CWakuVect *pcWakuVect)
{
	CSWakVect *pcSWakVect;
	CVect*		pcVect;
	TCHAR	szFont[RECMAX];
	TCHAR	szStrg[RECMAX];
	int		nTateYoko;
	DBL		dWidth, dHight;
	DBL		dStrLen;
	DBL		dStartX, dStartY;
	DBL		sx, sy, ex, ey;
	int		nSpaceKind;
	DBL		dCharCount;
	int		nFillPenNo, nOutLinePen;
	DBL		dOutLineWidth;

	CTonApp *pcApp = (CTonApp*)::AfxGetApp();
	CSysEnv *pcSysEnv = pcApp->m_pcSysEnv;
	pcWakuVect->GetFontName(szFont);
	pcWakuVect->GetString(szStrg);
	nFillPenNo = pcWakuVect->m_nFillPen;
	nOutLinePen = pcWakuVect->m_nOutLinePen;
	dOutLineWidth = pcWakuVect->m_dOutLineWidth;
	nTateYoko = pcWakuVect->CheckTateYoko();
	dCharCount = pcWakuVect->GetMaxCharCount();
	pcWakuVect->GetFontSize(&dWidth, &dHight);
	pcWakuVect->GetStrLen(&dStrLen);
	pcWakuVect->GetStrStartXY(&dStartX, &dStartY);
	if(nTateYoko == TATEGAKI){
		sx = dStartX + dWidth / 2.0;
		sy = dStartY;
		ex = sx;
		ey = sy - dStrLen;
	}
	else{
		sx = dStartX;
		sy = dStartY - dHight / 2.0;
		ex = sx + dStrLen;
		ey = sy;
	}
	if(pcWakuVect->CheckSpaceKind() == ON){
		nSpaceKind = SPACEDATA;
	}
	else{
		nSpaceKind = SPACEWAKU;
	}

	pcSWakVect = (CSWakVect*)(NewData(SWAKVECTID));
	pcVect = (CVect*)(NewData(BASEVECTID));
	pcVect->SetAtrDXY(0, 0, sx, sy);
	pcVect->SetAtrDXY(1, 0, ex, ey);
	pcVect->SetPointCount(2);

	pcSWakVect->SetFitingVect(pcVect);
	pcSWakVect->SetFitingVect(pcVect);
	pcSWakVect->SetFillPen(nFillPenNo);
	pcSWakVect->SetOutLinePen(nOutLinePen);
	pcSWakVect->SetFlatPar(MJFREEFLATPAR);
	pcSWakVect->SetSpacePar(0.0);
	pcSWakVect->SetKanaPar(100.0);
	if(dCharCount <= 0){
		dCharCount = 0;
	}
	pcSWakVect->SetMaxCharCount(dCharCount);
	if(dCharCount != 0){
		pcSWakVect->SetOderKind(ODERLEFT);
	}else{
		pcSWakVect->SetOderKind(ODEREQUAL);
	}
	pcSWakVect->SetSpaceKind(nSpaceKind);
	pcSWakVect->SetTateYoko(nTateYoko);
	pcSWakVect->SetFontSize(dWidth, dHight);
	pcSWakVect->SetStrLen(0.0);
	pcSWakVect->SetStrStartXY(sx, sy);
	pcSWakVect->SetFontName(szFont);
	pcSWakVect->SetString(szStrg);
	szStrg[RECMAX] = '\0';
	pcSWakVect->RenewMiniMax();

	if(szStrg[0] != '\0'){
		pcSWakVect->StrgLineStruct();
		pcSWakVect->RenewMiniMax();
	}
	return(pcSWakVect);
}

void CTonDoc::UpdateDBObject()
{
}
void CTonDoc::CopyWakuFileBlock(LPTSTR lpSrcBaseName, LPTSTR lpDstBaseName)
{
}