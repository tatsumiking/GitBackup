﻿
// SesDoc.cpp : CTonDoc クラスの実装
//

#include "stdafx.h"
#include <io.h>
#include <direct.h>
#include "FileEps.h"
#include "CmdBase.h"
#include "BmpFile.h"

#include <propkey.h>

//extern "C" {
	#include "../../ObjLib/BaseLib.h"
	#include "../../ObjLib/Senvlib.h"
	#include "../../ObjLib/Scrnlib.h"
	#include "../../ObjLib/Mojienv.h"
//}
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	UNDOMAX		32
#define NEARLIMIT	32

// CTonDoc

IMPLEMENT_DYNCREATE(CTonDoc, CDocument)

BEGIN_MESSAGE_MAP(CTonDoc, CDocument)
	//{{AFX_MSG_MAP(CTonDoc)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CTonDoc コンストラクション/デストラクション
CTonDoc::CTonDoc() noexcept
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。

}
BOOL CTonDoc::OnNewDocument()
{
	CTonApp*	pcApp;
	CMainFrame*	pcWnd;
	TCHAR	szTmpPath[FNAMEMAX];

	m_nSaveWaku = OFF;
	m_nActiveSave = OFF;

	m_nTonnbo = FALSE;
	m_dTopSpace = 0.0;
	m_dBottomSpace = 0.0;
	m_dLeftSpace = 0.0;
	m_dRightSpace = 0.0;

	m_pcCmdBase = NULL;

	m_nDBType = 0;
	m_csDBFileName = _T("");
	m_csDBTableName = _T("");
	m_pcDialogDBEdit = NULL;

	if (!CDocument::OnNewDocument())
		return FALSE;

	pcApp = (CTonApp*)::AfxGetApp();
	pcWnd = (CMainFrame*)::AfxGetMainWnd();
	m_dwClr = pcWnd->m_dwClr;
	m_pcFSF1Font = pcWnd->m_pcFSF1Font;
	m_pcESF1Font = pcWnd->m_pcESF1Font;
	m_pcESF0Font = pcWnd->m_pcESF0Font;
	m_pcFBU0Font = pcWnd->m_pcFBU0Font;
	m_pcWNTTFont = pcWnd->m_pcWNTTFont;
	senvgettmppath(szTmpPath);
	_stprintf_s(m_szImageBmpFile, _T("%s\\docimage.bmp"), szTmpPath);

	m_sizeDoc = CSize(1, 1);
	m_pcObjDataList = &m_ObjDataList1;
	m_nNoMsgOut = FALSE;
	m_nUndoNo = 1;
	m_szCrctFileName[0] = _T('\0');

	return TRUE;
}

CTonDoc::~CTonDoc()
{
	TCHAR	execpath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];

	senvgetexepath(execpath);
	for(m_nUndoNo = 0; m_nUndoNo < UNDOMAX; m_nUndoNo++){
		_stprintf_s(fname, _T("%s\\undo%04d.iti"), execpath, m_nUndoNo);
		_tremove(fname);
	}

	if(m_pcDialogDBEdit != NULL){
		m_pcDialogDBEdit->DestroyWindow();
		delete(m_pcDialogDBEdit);
		m_pcDialogDBEdit = NULL;
	}

	if(m_pcCmdBase != NULL){
		delete(m_pcCmdBase);
		m_pcCmdBase = NULL;
	}

}

// CTonDoc のシリアル化

void CTonDoc::Serialize(CArchive& ar)
{
	int			nID;
	CData*		pcData;
	POSITION	pos;
	CString		csCheckString;
	TCHAR		szCheckString1[] = _T("Easy Scanner 98 Ver 1.000.00");
	TCHAR		szCheckString2[] = _T("いちばん Ver 1.000.00");
	TCHAR		szCheckString3[] = _T("筆耕システム Ver 1.000.00");
	TCHAR		szCheckString4[] = _T("筆耕システム Ver 2.000.00");
	TCHAR		szCheckString5[] = _T("筆耕システム Ver 3.000.00");
	TCHAR		szCheckString6[] = _T("筆耕システム Ver 3.100.00");
	TCHAR		szCheckString7[] = _T("筆耕システム Ver 3.500.00");
	CTonView	*pcView;
	FILE		*fp;
	int			len;
	TCHAR		szBuf[RECMAX];
	
	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	ar.m_pDocument = this;
	if (ar.IsStoring())
	{
		try
		{
			if(scrncheckundoflag() == OFF){ 
				ImageBmpFileCreate();
				_tfopen_s(&fp, m_szImageBmpFile, _T("rb"));
				if(fp == NULL){
					return;
				}
				while(1){
					len = fread(szBuf, 1, RECMAX, fp);
					ar.Write(szBuf, len);
					if(len != RECMAX){
						break;
					}
				}
				fclose(fp);
			}
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
			// 背景のためのデータブロック
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
			// TODO: この位置に保存用のコードを追加してください。
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
		if(_tcscmp(szCheckString1, csCheckString) == 0){
			pcApp->m_clrBack = -1;
		}else if(_tcscmp(szCheckString2, csCheckString) == 0){
			ar >> pcApp->m_clrBack;
		}else if(_tcscmp(szCheckString3, csCheckString) == 0){
			ar >> pcApp->m_clrBack;
		}else if(_tcscmp(szCheckString4, csCheckString) == 0){
			ar >> pcApp->m_clrBack;
			ar >> m_dTopSpace;
			ar >> m_dBottomSpace;
			ar >> m_dLeftSpace;
			ar >> m_dRightSpace;
		}else if(_tcscmp(szCheckString5, csCheckString) == 0){
			ar >> pcApp->m_clrBack;
			ar >> m_dTopSpace;
			ar >> m_dBottomSpace;
			ar >> m_dLeftSpace;
			ar >> m_dRightSpace;
			ar >> m_nDBType;
			ar >> m_csDBFileName;
			ar >> m_csDBTableName;
			senvsetverjion(VER300);
		}else if(_tcscmp(szCheckString6, csCheckString) == 0){
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
		}else if(_tcscmp(szCheckString7, csCheckString) == 0){
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
			pcView->MessageBox(_T("ファイルが正しくありません"), _T("読込エラー"), MB_OK);
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
	if(!ar.IsStoring()){
		if(scrncheckundoflag() == OFF){
			if(m_pcView != NULL
			&& m_csDBFileName.IsEmpty() == FALSE){
				CreateDialogDBEdit(m_pcView);	
			}
		}
		UpdateDBObject();
	}
}

#ifdef SHARED_HANDLERS

//縮小版のサポート
void CTonDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// このコードを変更してドキュメントのデータを描画します
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 検索ハンドラーのサポート
void CTonDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ドキュメントのデータから検索コンテンツを設定します。
	// コンテンツの各部分は ";" で区切る必要があります

	// 例:      strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CTonDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CTonDoc の診断

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


// CTonDoc コマンド
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
		pcData->FBU0FontLink(m_pcFBU0Font);
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
void CTonDoc::DataAllCls0()
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

	ret = TRUE;	//すべてが削除できたとき
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
	// 背景画像用データリンク
	DataAllCls0();
	m_ObjDataList0.AddTail(pData);
	SetModifiedFlag();
}
void CTonDoc::DataAddTail(CData * pData)
{
	m_pcObjDataList->AddTail(pData);
	SetModifiedFlag();  // ファイルを閉じるときの確認用にドキュメントが更新されたか
						// どうかのマークを付けます。
}

void CTonDoc::DataAddHead(CData * pData)
{
	m_pcObjDataList->AddHead(pData);
	SetModifiedFlag();  // ファイルを閉じるときの確認用にドキュメントが更新されたか
						// どうかのマークを付けます。
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

void CTonDoc::MojiSaveAtnVect(LPTSTR fontname, int code, DBL sx, DBL sy, DBL size)
{
	CData*		pData;
	POSITION	pos;
	int			nFontType;
	TCHAR		sFaceName[FACEMAX+2];
	int			sjis;

	mojienvflistopen(_T("rb"));
	mojienvgetfonttypename(fontname, &nFontType, sFaceName);
	mojienvflistclose();

	m_pcFSF1Font->PointSetStart();
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		pData = m_pcObjDataList->GetNext(pos);
		m_pcFSF1Font->TrnsElementSet(sx, sy, size, size);
		pData->SetAtnVectFSF1fBuf(m_pcFSF1Font);
	}
	m_pcFSF1Font->PointSetEnd();
#ifdef UNICODE
	sjis = baselibU16ToSjis(code);
#else
	sjis = code;
#endif
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
	TCHAR	execpath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];

	m_nActiveSave = ON;
	senvgetexepath(execpath);
	_stprintf_s(fname, _T("%s\\archive.iti"), execpath);
	CFile file(fname,  CFile::modeCreate|CFile::modeWrite);
	CArchive ar(&file, CArchive::store);
	scrnonundoflag();
	Serialize(ar);
	scrnoffundoflag();
	m_nActiveSave = OFF;
}

void CTonDoc::LoadArchive()
{
	TCHAR	execpath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];

	senvgetexepath(execpath);
	_stprintf_s(fname, _T("%s\\archive.iti"), execpath);
	CFile file(fname, CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	scrnonundoflag();
	Serialize(ar);
	scrnoffundoflag();
}

BOOL CTonDoc::CheckArchive()
{
	TCHAR	execpath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	CFileException ex;

	senvgetexepath(execpath);
	_stprintf_s(fname, _T("%s\\archive.iti"), execpath);
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
	_stprintf_s(fname, _T("%s\\undo%04d.iti"), execpath, m_nUndoNo);
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

	_stprintf_s(fname, _T("%s\\undo%04d.iti"), execpath, m_nUndoNo);
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
	_stprintf_s(fname, _T("%s\\undo%04d.iti"), execpath, chkno);
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
	_stprintf_s(fname, _T("%s\\undo%04d.iti"), execpath, m_nUndoNo);
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
	_stprintf_s(fname, _T("%s\\undo%04d.iti"), execpath, m_nUndoNo);
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
	_stprintf_s(fname, _T("%s\\undo%04d.iti"), execpath, chkno);
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

void CTonDoc::WakuLoad(LPTSTR filename)
{
	FILE	*fp;
	TCHAR	buf[RECMAX];
	CSWakVect*	cpSWakVect;
	CFileEps	*pcFileEps;

	CTonApp *pcApp = (CTonApp*)::AfxGetApp();
	CMainFrame *pcWnd = (CMainFrame*)pcApp->GetMainWnd();
	CTonView *pcview = (CTonView*)pcWnd->m_pcCrtView;

	basechangesafix(filename, _T("iti"));
	if(_taccess_s(filename, 0) == 0){
		LoadFile(filename);
		return;
		//DeleteWakuData();
	}
	else{
		basechangesafix(filename, _T("esp"));
		if(_taccess_s(filename, 0) == 0){
			pcFileEps = new CFileEps(pcview);
			pcFileEps->LoadData(filename);
			pcFileEps->GrupReset();
			delete(pcFileEps);
		}
	}
	basechangesafix(filename, _T("wak"));
	_tfopen_s(&fp, filename, _T("r"));
	if(fp == NULL){
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
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

void CTonDoc::WakuSave(TCHAR *filename)
{
	CData*		pData;
	POSITION	pos;
	FILE		*fp;

	basechangesafix(filename, _T("iti"));
	m_nNoMsgOut = TRUE;
	SaveFile(filename);
	m_nNoMsgOut = FALSE;
	basechangesafix(filename, _T("wak"));
	_tfopen_s(&fp, filename, _T("w"));
	if(fp == NULL){
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
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	CData* pData;
	POSITION pos;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\string.lst"), exepath);
	_tfopen_s(&fp, fname, _T("wb"));
	if(fp == NULL){
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
	if(fp == NULL){
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

	ret = TRUE;	//すべてが削除できたとき
	pos = m_pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		nextpos = pos;
		pData = m_pcObjDataList->GetNext(nextpos);
		if (pData->CheckFlag(ACTIVEFLAG) == ON) {
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
	BYTE	szBuf[16];
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

void CTonDoc::SaveFile(LPTSTR fname)
{
	TCHAR	msg[RECMAX];

	CFile file;
	if(m_nNoMsgOut == FALSE){
		if(file.Open(fname,  CFile::modeRead, NULL)){
			file.Close();
			_stprintf_s(msg, _T("%sに上書きしてもよろしいですか"), fname);
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
	TCHAR	szFont[RECMAX];
	TCHAR	szStrg[RECMAX];
	int		nTateYoko;
	DBL		dWidth, dHight;
	DBL		dStrLen;
	DBL		dStartX, dStartY;
	DBL		sx, sy, ex, ey;
	int		nSpaceKind;
	DBL		dTCHARCount;
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
	dTCHARCount = pcWakuVect->GetMaxCharCount();
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
	if(dTCHARCount <= 0){
		dTCHARCount = 0;
	}
	pcSWakVect->SetMaxCharCount(dTCHARCount);
	if(dTCHARCount != 0){
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
	szStrg[RECMAX] = _T('\0');
	pcSWakVect->RenewMiniMax();

	if(szStrg[0] != _T('\0')){
		pcSWakVect->StrgLineStruct();
		pcSWakVect->RenewMiniMax();
	}
	return(pcSWakVect);
}

int CTonDoc::CreateDialogDBEdit(CTonView *pcView)
{
	m_pcDialogDBEdit = new CDialogDBEdit(pcView);
	m_pcDialogDBEdit->SetModaless();
	if(m_pcDialogDBEdit->Create(IDD_DBEDIT, pcView)){
		m_pcDialogDBEdit->InitItemElement();
		if(m_pcDialogDBEdit->m_nFildMax > 251){
			::AfxMessageBox(_T("フィールド数が多すぎます"), MB_OK);
			DestroyDialogDBEdit();
			m_nDBType = 0;
			return(FALSE);
		}
		if(m_pcDialogDBEdit->m_nFildMax == 0){
			::AfxMessageBox(_T("フィールドが見つかりません"), MB_OK);
			DestroyDialogDBEdit();
			m_nDBType = 0;
			return(FALSE);
		}
		if(m_pcDialogDBEdit->m_nLineMax == 0){
			::AfxMessageBox(_T("レコードが見つかりません"), MB_OK);
			DestroyDialogDBEdit();
			m_nDBType = 0;
			return(FALSE);
		}
	}
	LocateBottomFitDialog(m_pcDialogDBEdit);
	return(TRUE);
}

void CTonDoc::LocateBottomFitDialog(CDialog *pcDialog)
{
	CRect	crect;
	CRect	prect;
	int		x, y, cx, cy;

	m_pcView->GetWindowRect(&prect);
	pcDialog->GetWindowRect(&crect);
	x = prect.left;
	y = prect.bottom - crect.Height();
	cx = crect.Width();
	cy = crect.Height();
	::SetWindowPos(pcDialog->m_hWnd, HWND_NOTOPMOST, x, y, cx, cy, 0);
}

void CTonDoc::ShowDialogDBEdit()
{
	m_pcDialogDBEdit->ShowWindow(SW_SHOWNORMAL);
}

void CTonDoc::DestroyDialogDBEdit()
{
	if(m_pcDialogDBEdit != NULL){
		m_pcDialogDBEdit->DestroyWindow();
		delete(m_pcDialogDBEdit);
		m_pcDialogDBEdit = NULL;
	}
}

void CTonDoc::SetCrtView(CTonView *pcView)
{
	m_pcView = pcView;
}

void CTonDoc::UpdateDBObject()
{
	POSITION	pos;
	TCHAR		szStr[RECMAX];
	CSWakVect	*pcSWakVect;
	TCHAR		szFildName[RECMAX];

	if(m_pcDialogDBEdit == NULL){
		return;
	}
	if(m_pcCmdBase == NULL){
		m_pcCmdBase = new CCmdBase(m_pcView);
		m_pcCmdBase->InitClass();
	}
	m_pcCmdBase->SetDataListSelectID(SWAKVECTID, 0);
	pos = m_pcCmdBase->m_pcPtrList->GetHeadPosition();
	while(pos != NULL){
		pcSWakVect = (CSWakVect*)m_pcCmdBase->m_pcPtrList->GetNext(pos);
		pcSWakVect->GetFildName(szFildName);
		if(szFildName[0] != _T('\0')){
			m_pcDialogDBEdit->GetFildElement(szFildName, szStr);
			pcSWakVect->SetString(szStr);
			pcSWakVect->StrgLineStruct();
			pcSWakVect->RenewMiniMax();
		}
	}
}
void CTonDoc::ImageBmpFileCreate()
{
	CBmpFile	*pcBmpFile;
	DBL		xsize, ysize;
	int		dotx, doty;
	int		srcwd, srchi;		

	CMainFrame *pcWnd = (CMainFrame*)AfxGetMainWnd();
	scrngetmmsize(&xsize, &ysize);
	if(xsize < ysize){
		doty = 512;
		dotx = (int)(xsize * (DBL)doty / ysize);
		srchi = 128;
		srcwd = (int)(xsize * (DBL)srchi / ysize);
	}
	else{
		dotx = 512;
		doty = (int)(ysize * (DBL)dotx / xsize);
		srcwd = 128;
		srchi = (int)(ysize * (DBL)srcwd / xsize);
	}
	pcBmpFile = new CBmpFile(m_pcView);
	pcBmpFile->CreateBmpInit(dotx, doty);
	pcBmpFile->CreateBmpImage(m_szImageBmpFile);
	delete(pcBmpFile);
}
void CTonDoc::SaveVectFile(LPTSTR lpFileName)
{
	CSWakVect	*pcSWakVect;
	FILE	*fp;
	POSITION	pos;
	DBL		sx, sy, ex, ey;
	TCHAR	buf[RECMAX];

	_tfopen_s(&fp, lpFileName, _T("w"));
	if(fp == NULL){
		return;
	}
	_stprintf_s(buf, _T("REM \n"));
	baselibFputs(buf, fp);
	if(m_pcCmdBase == NULL){
		m_pcCmdBase = new CCmdBase(m_pcView);
		m_pcCmdBase->InitClass();
	}
	m_pcCmdBase->SetDataListSelectID(SWAKVECTID, 0);
	pos = m_pcCmdBase->m_pcPtrList->GetHeadPosition();
	while(pos != NULL){
		pcSWakVect = (CSWakVect*)m_pcCmdBase->m_pcPtrList->GetNext(pos);
		pcSWakVect->GetFitingBoxs(&sx, &sy, &ex, &ey);
		_stprintf_s(buf,_T("boxs,%1.0lf,%1.0lf,%1.0lf,%1.0lf\n"), sx, sy, ex, ey);
		baselibFputs(buf, fp);
	}
	
	fclose(fp);
}
void CTonDoc::CopyWakuFileBlock(LPTSTR lpSrcBaseName, LPTSTR lpDstBaseName)
{
	TCHAR	szSrcFileName[FNAMEMAX];
	TCHAR	szDstFileName[FNAMEMAX];

	_stprintf_s(szSrcFileName, _T("%s.iti"), lpSrcBaseName);
	_stprintf_s(szDstFileName, _T("%s.iti"), lpDstBaseName);
	CopyFile(szSrcFileName, szDstFileName, FALSE);
	_stprintf_s(szSrcFileName, _T("%s.bin"), lpSrcBaseName);
	_stprintf_s(szDstFileName, _T("%s.bin"), lpDstBaseName);
	CopyFile(szSrcFileName, szDstFileName, FALSE);
	_stprintf_s(szSrcFileName, _T("%s.env"), lpSrcBaseName);
	_stprintf_s(szDstFileName, _T("%s.env"), lpDstBaseName);
	CopyFile(szSrcFileName, szDstFileName, FALSE);
	_stprintf_s(szSrcFileName, _T("%s.wak"), lpSrcBaseName);
	_stprintf_s(szDstFileName, _T("%s.wak"), lpDstBaseName);
	CopyFile(szSrcFileName, szDstFileName, FALSE);
}
