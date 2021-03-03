// smsoutDoc.cpp : CTonDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "io.h"
#include "direct.h"
#include "..\ses\fileeps.h"
#include "..\cmnsrc\Cmd_base.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	UNDOMAX		32
#define NEARLIMIT	32

extern "C" {
	void basechangesafix(LPSTR FileName, LPSTR Safix);
	int senvsetverjion(int flag);
	void senvgetdatpath(LPSTR path);
	void senvgetexepath(LPSTR execpath);
	void scrnonundoflag();
	void scrnoffundoflag();
	int	 scrncheckundoflag();
	void scrngetrltvsize(DBL *pdRXsize, DBL *pdRYsize);
	void scrngetrltvtop(DBL *pdRX, DBL *pdRY);
	void scrndlenmmtorltv(LPDBL len);
	void mojienvgetfonttypename(LPSTR fontname, LPINT retftype, LPSTR fontfullname);
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
	void wakuenvloadoutputenv(LPSTR wakuloadfname);
	void wakuenvsaveoutputenv(LPSTR wakusavefname);
	int basegetparentpath(LPSTR lpPath);
	void basechangesafix(LPSTR FileName, LPSTR Safix);
}

/////////////////////////////////////////////////////////////////////////////
// CTonDoc

IMPLEMENT_DYNCREATE(CTonDoc, CDocument)

BEGIN_MESSAGE_MAP(CTonDoc, CDocument)
	//{{AFX_MSG_MAP(CTonDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTonDoc �N���X�̍\�z/����

CTonDoc::CTonDoc()
{
	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	CMainFrame *pcWnd = (CMainFrame*)pcApp->GetMainWnd();
	m_nActiveSave = OFF;
	m_nDBType = 0;
	m_csDBFileName = _T("");
	m_csDBTableName = _T("");
	m_pcFSF1Font = pcWnd->m_pcFSF1Font;
	m_pcESF1Font = pcWnd->m_pcESF1Font;
	m_pcESF0Font = pcWnd->m_pcESF0Font;
	m_pcWNTTFont = pcWnd->m_pcWNTTFont;
	m_pcObjDataList = &m_ObjDataList1;

	m_nTonnbo = FALSE;
	m_dTopSpace = 0.0;
	m_dBottomSpace = 0.0;
	m_dLeftSpace = 0.0;
	m_dRightSpace = 0.0;
}

CTonDoc::~CTonDoc()
{
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
	CString		csCheckString;
	char		szCheckString1[] = "Easy Scanner 98 Ver 1.000.00";
	char		szCheckString2[] = "�����΂� Ver 1.000.00";
	char		szCheckString3[] = "�M�k�V�X�e�� Ver 1.000.00";
	char		szCheckString4[] = "�M�k�V�X�e�� Ver 2.000.00";
	char		szCheckString5[] = "�M�k�V�X�e�� Ver 3.000.00";
	char		szCheckString6[] = "�M�k�V�X�e�� Ver 3.100.00";
	char		szCheckString7[] = "�M�k�V�X�e�� Ver 3.500.00";
	CTonView	*pcView;
	
	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	ar.m_pDocument = this;
	if (ar.IsStoring())
	{
		try
		{
			senvsetverjion(VER350);
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
			while (pos != NULL){
				pcData = m_ObjDataList0.GetNext(pos);
				if(m_nActiveSave == OFF || pcData->CheckFlag(ACTIVEFLAG) == ON){
					nID = pcData->CheckID();
					ar << nID;
					pcData->Serialize(ar);
				}
			}
			nID = NOOPDATAID;
			ar << nID;

			pos = m_pcObjDataList->GetHeadPosition();
			while (pos != NULL){
				pcData = m_pcObjDataList->GetNext(pos);
				if(m_nActiveSave == OFF || pcData->CheckFlag(ACTIVEFLAG) == ON){
					nID = pcData->CheckID();
					ar << nID;
					pcData->Serialize(ar);
				}
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
		senvsetverjion(VER100);
		m_nTonnbo = FALSE;
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
		}else if(lstrcmp(szCheckString7, csCheckString) == 0){
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
			while(1){
				ar >> nID;
				if(nID == NOOPDATAID){
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
			pcView->MessageBox("�t�@�C��������������܂���", "�Ǎ��G���[", MB_OK);
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
	case CLIPDATAID:
		pcData = (CData *)(new CClipData(nItemID));
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
		if(pcData->CheckFlag(ACTIVEFLAG) == 	ON){
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
	CData*	pcData;
	POSITION	pos, delpos;

	pos = m_ObjDataList0.GetHeadPosition();
	while (pos != NULL)
	{
		delpos = pos;
		pcData = m_ObjDataList0.GetNext(pos);
		m_ObjDataList0.RemoveAt(delpos);
		delete(pcData);
	}

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		delpos = pos;
		pcData = m_pcObjDataList->GetNext(pos);
		m_pcObjDataList->RemoveAt(delpos);
		delete(pcData);
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
	m_ObjDataList0.AddTail(pData);
	SetModifiedFlag();  // �t�@�C�������Ƃ��̊m�F�p�Ƀh�L�������g���X�V���ꂽ��
						// �ǂ����̃}�[�N��t���܂��B
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

void CTonDoc::CopyData(DBL movex, DBL movey)
{
	CData*		pData;
	CData*		pnewData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData->CheckFlag(COPYFLAG) == ON){
			pnewData = pData->CopyData(movex, movey);
			if(pnewData != NULL){
				DataAddTail(pnewData);
			}
		}
	}
	
}

void CTonDoc::MirrTrns(DBL a, DBL b, DBL t1, DBL t2)
{
	CData*		pData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			pData->MirrorTrns(a, b, t1, t2);
		}
	}
}

void CTonDoc::RoundTrns(DBL cntrx, DBL cntry, DBL tsin, DBL tcos)
{
	CData*		pData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			pData->RoundTrns(cntrx, cntry, tsin, tcos);
		}
	}
}

void CTonDoc::MoveTrns(DBL movex, DBL movey)
{
	CData*		pData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			pData->MoveTrns(movex, movey);
		}
	}
}

void CTonDoc::SizeTrns(DBL cntrx, DBL cntry, DBL timex, DBL timey)
{
	CData*		pData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			pData->SizeTrns(cntrx, cntry, timex, timey);
		}
	}
}

void CTonDoc::FitingData()
{
	CData*		pcData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_pcObjDataList->GetNext(pos);
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			pcData->StrgFiting();
		}
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
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			pData->StrctBoxsTrns();
		}
	}
}

void CTonDoc::GetDMiniMax(int flag, LPDBL minix, LPDBL miniy, LPDBL maxx, LPDBL maxy)
{
	CData*		pData;
	POSITION	pos;
	DBL			tminix, tminiy, tmaxx, tmaxy;

	(*minix) = INTMAX;
	(*miniy) = INTMAX;
	(*maxx) = -INTMAX;
	(*maxy) = -INTMAX;
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(flag == 0 || pData->CheckFlag(flag) == ON){
			tminix = INTMAX; tminiy = INTMAX;
			tmaxx = -INTMAX; tmaxy = -INTMAX;
			pData->GetDMiniMax(&tminix, &tminiy, &tmaxx, &tmaxy);
			(*minix) = min((*minix), tminix);
			(*miniy) = min((*miniy), tminiy);
			(*maxx) = max((*maxx), tmaxx);
			(*maxy) = max((*maxy), tmaxy);
		}
	}
}

void CTonDoc::SlopTrns(DBL cntrx, DBL cntry, DBL ddx, DBL ddy)
{
	CData*		pData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData->CheckFlag(ACTIVEFLAG) == ON){
			pData->SlopTrns(cntrx, cntry, ddx, ddy);
		}
	}
}

void CTonDoc::AllOffPointFlag(int bitflag)
{
	CData*		pData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		pData->AllOffPointFlag(bitflag);
	}
}

void CTonDoc::AllOnPointFlag(int bitflag)
{
	CData*		pData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		pData->AllOnPointFlag(bitflag);
	}
}

void CTonDoc::OffFlag(int bitflag)
{
	CData*		pData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		pData->OffFlag(bitflag);
	}
}

void CTonDoc::OnFlag(int bitflag)
{
	CData*		pData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		pData->OnFlag(bitflag);
	}
}

void CTonDoc::CheckOnFlag(int checkbitflag, int bitflag)
{
	CData*		pData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData->CheckFlag(checkbitflag) == ON){
			pData->OnFlag(bitflag);
		}
	}
}

void CTonDoc::CheckOffFlag(int checkbitflag, int bitflag)
{
	CData*		pData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData->CheckFlag(checkbitflag) == ON){
			pData->OffFlag(bitflag);
		}
	}
}

void CTonDoc::AtntionDraw(CDC *pcDC)
{
	CData*		pData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		pData->AtntionDraw(pcDC);
	}
}

void CTonDoc::CheckFlagDraw(int flag, CDC *pcDC)
{
	CData*		pData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData->CheckFlag(flag) == ON){
			pData->CheckFlagDraw(flag, pcDC);
		}
	}
}

void CTonDoc::CheckFlagDrawSetClr(int flag, CDC *pcDC, COLORREF clr1, COLORREF clr2, DBL thick)
{
	CData*		pData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData->CheckFlag(flag) == ON){
			pData->CheckFlagDrawSetClr(flag, pcDC, clr1, clr2, thick);
		}
	}
}

void CTonDoc::CheckFlagOutlineDraw(int flag, CDC *pcDC)
{
	CData*		pData;
	POSITION	pos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		if(pData->CheckFlag(flag) == ON){
			pData->CheckFlagOutlineDraw(flag, pcDC);
		}
	}
}

void CTonDoc::MojiSaveAtnVect(LPSTR fontname, UINT sjis, DBL sx, DBL sy, DBL size)
{
	CData*		pData;
	POSITION	pos;
	int			nFontType;
	char		sFaceName[FACEMAX+2];

	mojienvgetfonttypename(fontname, &nFontType, sFaceName);

	m_pcFSF1Font->PointSetStart();
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		m_pcFSF1Font->TrnsElementSet(sx, sy, size, size);
		pData->SetAtnVectFSF1fBuf(m_pcFSF1Font);
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
	case TT1FONT:
	case TT2FONT:
	case TT3FONT:
		strcpy(sFaceName, fontname);
		sFaceName[8] = '\0';
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

CData * CTonDoc::CheckSelectOne(int nSelectID)
{
	CData*	pcData;
	CData*	pcSelectData;
	int		nID;

	pcSelectData = NULL;
	POSITION pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_pcObjDataList->GetNext(pos);
		if(pcData == NULL){
			break;
		}
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			nID = pcData->CheckID();
			if(nSelectID == 0 || nSelectID == nID){
				if(pcSelectData == NULL){
					pcSelectData = pcData;
				}
				else{
					return(NULL);
				}
			}
		}
	}
	return(pcSelectData);
}

void CTonDoc::SaveArchive()
{
	char	execpath[FNAMEMAX];
	char	fname[FNAMEMAX];

	m_nActiveSave = ON;
	senvgetexepath(execpath);
	sprintf(fname, "%s\\archive.iti", execpath);
	CFile file(fname,  CFile::modeCreate|CFile::modeWrite);
	CArchive ar(&file, CArchive::store);
	Serialize(ar);
	m_nActiveSave = OFF;
}

void CTonDoc::LoadArchive()
{
	char	execpath[FNAMEMAX];
	char	fname[FNAMEMAX];

	senvgetexepath(execpath);
	sprintf(fname, "%s\\archive.iti", execpath);
	CFile file(fname, CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	Serialize(ar);
}

BOOL CTonDoc::CheckArchive()
{
	char	execpath[FNAMEMAX];
	char	fname[FNAMEMAX];
	CFileException ex;

	senvgetexepath(execpath);
	sprintf(fname, "%s\\archive.iti", execpath);
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
	char	msg[64];
	sprintf(msg, "%4.0lf %4.0lf", rltvx, rltvy);
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
	char	execpath[FNAMEMAX];
	char	fname[FNAMEMAX];

	senvgetexepath(execpath);
	sprintf(fname, "%s\\undo%04d.iti", execpath, m_nUndoNo);
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
	char	execpath[FNAMEMAX];
	char	fname[FNAMEMAX];

	if(m_nUndoNo == 0){
		return;
	}
	DataAllCls();
	senvgetexepath(execpath);
	m_nUndoNo--;
	if(m_nUndoNo < 1){
		m_nUndoNo = UNDOMAX;
	}

	sprintf(fname, "%s\\undo%04d.iti", execpath, m_nUndoNo);
	CFile file(fname, CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	scrnonundoflag();
	Serialize(ar);
	scrnoffundoflag();
	unlink(fname);
	ar.Close();
	file.Close();
}

BOOL CTonDoc::CheckUndo()
{
	char	execpath[FNAMEMAX];
	char	fname[FNAMEMAX];
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
	sprintf(fname, "%s\\undo%04d.iti", execpath, chkno);
	CFile file;
	if(file.Open(fname, CFile::modeRead, &ex)){
		file.Close();
		return(TRUE);
	}
	return(FALSE);
}

void CTonDoc::SaveRedo()
{
	char	execpath[FNAMEMAX];
	char	fname[FNAMEMAX];

	senvgetexepath(execpath);
	sprintf(fname, "%s\\undo%04d.iti", execpath, m_nUndoNo);
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
	char	execpath[FNAMEMAX];
	char	fname[FNAMEMAX];

	m_nUndoNo++;
	if(m_nUndoNo > UNDOMAX){
		m_nUndoNo = 1;
	}
	DataAllCls();
	senvgetexepath(execpath);
	sprintf(fname, "%s\\undo%04d.iti", execpath, m_nUndoNo);
	CFile file(fname, CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	scrnonundoflag();
	Serialize(ar);
	scrnoffundoflag();
	unlink(fname);
	ar.Close();
	file.Close();
}

BOOL CTonDoc::CheckRedo()
{
	char	execpath[FNAMEMAX];
	char	fname[FNAMEMAX];
	CFileException ex;
	int		chkno;

	chkno = m_nUndoNo + 1;
	if(chkno > UNDOMAX){
		chkno = 1;
	}
	senvgetexepath(execpath);
	sprintf(fname, "%s\\undo%04d.iti", execpath, chkno);
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

void CTonDoc::WakuLoad(LPSTR wakufname)
{
	char	szExePath[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX8+16];
	CSWakVect	*pcSWakVect;
	CFileEps	*pcFileEps;
	char		basefname[FNAMEMAX];
	char		tmpfname[FNAMEMAX];
	int			len;

	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	pcApp->m_clrBack = -1;
	CMainFrame *pcWnd = (CMainFrame*)pcApp->GetMainWnd();
	CTonView *pcView = (CTonView*)pcWnd->m_pcCrtView;

	if(wakufname[1] != ':' && strncmp(wakufname, "\\\\", 2) != 0){
		senvgetexepath(szExePath);
		sprintf(basefname, "%s\\%s", szExePath, wakufname);
	}
	else{
		strcpy(basefname, wakufname);
	}
	len = strlen(basefname);
	basefname[len-4] = '\0';

	if(m_nNoLoadDev == FALSE){
		sprintf(tmpfname, "%s.bin", basefname);
	    if((_access(tmpfname, 0 )) == 0){
			pcView->PrintEnvLoad(tmpfname);
		}
		else{
			basegetparentpath(tmpfname);
			strcat(tmpfname, "\\defprnt.bin");
			pcView->PrintEnvLoad(tmpfname);
		}
	}
	else{
		sprintf(tmpfname, "%s.bin", basefname);
	    if((_access(tmpfname, 0 )) == 0){
			pcView->PrintEnvSetElementLoad(tmpfname);
		}
		else{
			basegetparentpath(tmpfname);
			strcat(tmpfname, "\\defprnt.bin");
			pcView->PrintEnvSetElementLoad(tmpfname);
		}
	}

	sprintf(tmpfname, "%s.env", basefname);
    if((_access(tmpfname, 0 )) == 0){
		wakuenvloadoutputenv(tmpfname);
	}

	sprintf(tmpfname, "%s.iti", basefname);
    if((_access(tmpfname, 0 )) == 0){
		LoadFile(tmpfname);
		return;
	}

	// �t�@�C�����_�C���N�g�ɕۑ�����Ă��Ȃ��ꍇ
	sprintf(tmpfname, "%s.esp", basefname);
	if(_access(tmpfname, 0) == 0){
		pcFileEps = new CFileEps(pcView);
		pcFileEps->LoadData(tmpfname);
		pcFileEps->GrupReset();
		delete(pcFileEps);
	}

	sprintf(tmpfname, "%s.wak", basefname);
	if((fp = fopen(tmpfname, "r")) == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX8, fp) == NULL){
			break;
		}
		pcSWakVect = (CSWakVect*)NewData(SWAKVECTID);
		if(pcSWakVect == NULL){
			return;
		}
		pcSWakVect->WakuLoad(fp, buf);
		DataAddTail((CData*)pcSWakVect);
	}
	fclose(fp);	

	return;
}

void CTonDoc::WakuSave(char *filename)
{
	CData*		pData;
	POSITION	pos;
	FILE		*fp;

	basechangesafix(filename, "iti");
	SaveFile(filename);
	m_nNoMsgOut = FALSE;
	basechangesafix(filename, "wak");
	if((fp = fopen(filename, "w")) == NULL){
		return;
	}
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		pData->WakuSave(fp);
	}
	fclose(fp);
	return;
}

void CTonDoc::SaveStringStoreFile(int id)
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	CData* pData;
	POSITION pos;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\string.lst", exepath);
	if((fp = fopen(fname, "wb")) == NULL){
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
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	CData* pData;
	POSITION pos;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\string.lst", exepath);
	if((fp = fopen(fname, "rb")) == NULL){
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

void CTonDoc::SetModifiedFlag()
{
}

void CTonDoc::LoadFile(LPSTR fname)
{
	char	szBuf[16];
	short		nBmpMark;
	long		lImageSize;

	CFile file(fname, CFile::modeRead);
	file.Read(szBuf, 2);
	file.Seek(0L, CFile::begin);
	CArchive ar(&file, CArchive::load);
	if(szBuf[0] == 0x42 && szBuf[1] == 0x4d){
		ar >> nBmpMark;
		ar >> lImageSize;
		lImageSize -= 6;
		while(lImageSize--){
			ar >> szBuf[0];
		}
	}
	Serialize(ar);
	ar.Close();
	file.Close();
}
void CTonDoc::SaveFile(LPSTR fname)
{
	char	msg[RECMAX];

	CFile file;

	file.Open(fname,  CFile::modeCreate|CFile::modeWrite, NULL);
	CArchive ar(&file, CArchive::store);

	Serialize(ar);

	ar.Close();
	file.Close();
}

void CTonDoc::DeleteWakuData()
{
	CData*		pcData;
	POSITION	pos, delpos;
	int			nID;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		delpos = pos;
		pcData = m_pcObjDataList->GetNext(pos);
		nID = pcData->CheckID();
		if(nID == SWAKVECTID){
			m_pcObjDataList->RemoveAt(delpos);
			delete(pcData);
		}
	}
}

CData *CTonDoc::ConvertWakuToSWak(CWakuVect *pcWakuVect)
{
	CSWakVect *pcSWakVect;
	CVect*		pcVect;
	char	szFont[RECMAX];
	char	szStrg[RECMAX];
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

