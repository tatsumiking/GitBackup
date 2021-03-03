// smsDoc.cpp : CTonDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "io.h"
#include "..\ses\fileeps.h"

#include "smsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
	void scrnonundoflag();
	void scrnoffundoflag();
	int	 scrncheckundoflag();
	int senvsetverjion(int flag);
	void senvgetexepath(LPSTR path);
	void wakuenvloadoutputenv(LPSTR wakuloadfname);
	int basegetparentpath(LPSTR lpPath);
}
/////////////////////////////////////////////////////////////////////////////
// CTonDoc

IMPLEMENT_DYNCREATE(CTonDoc, CDocument)

BEGIN_MESSAGE_MAP(CTonDoc, CDocument)
	//{{AFX_MSG_MAP(CTonDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTonDoc クラスの構築/消滅

CTonDoc::CTonDoc()
{
	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	CMainFrame *pcWnd = (CMainFrame*)pcApp->GetMainWnd();
	m_nActiveSave = OFF;
	m_nTonnbo = FALSE;
	m_dTopSpace = 0.0;
	m_dBottomSpace = 0.0;
	m_dLeftSpace = 0.0;
	m_dRightSpace = 0.0;
	m_nDBType = 0;
	m_csDBFileName = _T("");
	m_csDBTableName = _T("");
	m_pcFSF1Font = pcWnd->m_pcFSF1Font;
	m_pcESF1Font = pcWnd->m_pcESF1Font;
	m_pcESF0Font = pcWnd->m_pcESF0Font;
	m_pcWNTTFont = pcWnd->m_pcWNTTFont;
	m_pcObjDataList = &m_ObjDataList1;
}

CTonDoc::~CTonDoc()
{
}

BOOL CTonDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTonDoc シリアライゼーション

void CTonDoc::Serialize(CArchive& ar)
{
	int			nID;
	CData*		pcData;
	POSITION	pos;
	CString		csCheckString;
	char		szCheckString1[] = "Easy Scanner 98 Ver 1.000.00";
	char		szCheckString2[] = "いちばん Ver 1.000.00";
	char		szCheckString3[] = "筆耕システム Ver 1.000.00";
	char		szCheckString4[] = "筆耕システム Ver 2.000.00";
	char		szCheckString5[] = "筆耕システム Ver 3.000.00";
	char		szCheckString6[] = "筆耕システム Ver 3.100.00";
	CTonView	*pcView;
	
	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	ar.m_pDocument = this;
	if (ar.IsStoring())
	{
		try
		{
			senvsetverjion(VER310);
			ar << CString(szCheckString6);
			ar << pcApp->m_clrBack;
			ar << m_dTopSpace;
			ar << m_dBottomSpace;
			ar << m_dLeftSpace;
			ar << m_dRightSpace;
			ar << m_nTonnbo;
			ar << m_nDBType;
			ar << m_csDBFileName;
			ar << m_csDBTableName;
			pos = m_pcObjDataList->GetHeadPosition();
			while (pos != NULL)
			{
				pcData = m_pcObjDataList->GetNext(pos);
				if(m_nActiveSave == OFF || pcData->CheckFlag(ACTIVEFLAG) == ON){
					nID = pcData->CheckID();
					ar << nID;
					pcData->Serialize(ar);
				}
			}
			nID = NOOPDATAID;
			ar << nID;
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
		}else{
			pos = GetFirstViewPosition();
			pcView = (CTonView *)(GetNextView(pos));
			pcView->MessageBox("ファイルが正しくありません", "読込エラー", MB_OK);
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
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTonDoc クラスの診断

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
// CTonDoc コマンド

CData* CTonDoc::NewData(int nItemID)
{
	CData*		pcData;

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
	}
	return(pcData);
}

void CTonDoc::WakuLoad(LPSTR wakufname)
{
	char	szExePath[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX8+16];
	CSWakVect*	cpSWakVect;
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
	    if((access(tmpfname, 0 )) == 0){
			pcView->PrintEnvLoad(tmpfname);
		}
		else{
			basegetparentpath(tmpfname);
			strcat(tmpfname, "\\defprnt.bin");
			pcView->PrintEnvLoad(tmpfname);
		}
	}

	sprintf(tmpfname, "%s.env", basefname);
    if((access(tmpfname, 0 )) == 0){
		wakuenvloadoutputenv(tmpfname);
	}

	sprintf(tmpfname, "%s.iti", basefname);
    if((access(tmpfname, 0 )) == 0){
		LoadFile(tmpfname);
		return;
	}

	// ファイルがダイレクトに保存されていない場合
	sprintf(tmpfname, "%s.esp", basefname);
	if(access(tmpfname, 0) == 0){
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

void CTonDoc::DataAddTail(CData *pcData)
{
	m_pcObjDataList->AddTail(pcData);
	// SetModifiedFlag();
}

void CTonDoc::DeleteData(CData *pData)
{
	POSITION	pos, tpos;
	CData	*crtpData;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL){
		tpos = pos;
		crtpData = (CData*)(m_pcObjDataList->GetNext(pos));
		if(crtpData == pData){
			m_pcObjDataList->RemoveAt(tpos);
		}
		else{
			crtpData->DeleteData(pData);
		}
	}
}

void CTonDoc::DataAllCls()
{
	CData*	pData;
	POSITION	pos, tpos;

	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL){
		tpos = pos;
		pData = m_pcObjDataList->GetNext(pos);
		m_pcObjDataList->RemoveAt(tpos);
		delete(pData);
	}
}

void CTonDoc::LoadFile(LPSTR fname)
{
	CFile file(fname, CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	Serialize(ar);
	ar.Close();
	file.Close();
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
		if(bitflag == 0 || pData->CheckFlag(bitflag) ==	ON){
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
		if(bitflag == 0 || pData->CheckFlag(bitflag) ==	ON){
			pData->SetDefFillClr();
		}
	}
}

CData* CTonDoc::SrchDataOnLinePoint(DBL dx, DBL dy, DBL *retx, DBL *rety, int *pp)
{
	return(NULL);
}

CData* CTonDoc::SrchIdsData(int *nIDary, int nIDcnt, DBL x, DBL y)
{
	return(NULL);
}

CData* CTonDoc::SrchDataPoint(DBL dx, DBL dy, int *retpp)
{
	return(NULL);
}

CData* CTonDoc::SrchData(int flag, DBL x, DBL y)
{
	return(NULL);
}

CData* CTonDoc::SrchMoji(DBL x, DBL y, int *idx)
{
	return(NULL);
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

void CTonDoc::SaveUndo()
{

}

void CTonDoc::LoadUndo()
{

}

CSize CTonDoc::GetDocSize()
{
	CSize	size;

	size = CSize(100, 100);
	return size;
}

CPoint CTonDoc::GetTopPoint()
{
	CPoint	point;

	point = CPoint(0, 0);
	return point;
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

	CTonApp *pcApp = (CTonApp*)::AfxGetApp();
	CSysEnv *pcSysEnv = pcApp->m_pcSysEnv;
	pcWakuVect->GetFontName(szFont);
	pcWakuVect->GetString(szStrg);
	nTateYoko = pcWakuVect->CheckTateYoko();
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
	pcSWakVect->SetFillPen(0);
	pcSWakVect->SetOutLinePen(0);
	pcSWakVect->SetFlatPar(MJFREEFLATPAR);
	pcSWakVect->SetSpacePar(0.0);
	pcSWakVect->SetKanaPar(100.0);
	pcSWakVect->SetOderKind(ODEREQUAL);
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
