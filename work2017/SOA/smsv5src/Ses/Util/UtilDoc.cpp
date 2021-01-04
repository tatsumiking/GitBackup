
// UtilDoc.cpp : CTonDoc クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "Util.h"
#endif

#include "UtilDoc.h"

#include <propkey.h>

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Mojienv.h"
#include "../../ObjLib/Plgnfill.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTonDoc

IMPLEMENT_DYNCREATE(CTonDoc, CDocument)

BEGIN_MESSAGE_MAP(CTonDoc, CDocument)
END_MESSAGE_MAP()


// CTonDoc コンストラクション/デストラクション

CTonDoc::CTonDoc() noexcept
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。

}

CTonDoc::~CTonDoc()
{
}

BOOL CTonDoc::OnNewDocument()
{
	CTonApp*	pcApp;
	CMainFrame*	pcWnd;

	if (!CDocument::OnNewDocument())
		return FALSE;

	pcApp = (CTonApp*)::AfxGetApp();
	pcWnd = (CMainFrame*)::AfxGetMainWnd();
	m_pcFSF1Font = pcWnd->m_pcFSF1Font;
	m_pcESF1Font = pcWnd->m_pcESF1Font;
	m_pcESF0Font = pcWnd->m_pcESF0Font;
	m_pcFBU0Font = pcWnd->m_pcFBU0Font;
	m_pcWNTTFont = pcWnd->m_pcWNTTFont;

	return TRUE;
}




// CTonDoc のシリアル化

void CTonDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 格納するコードをここに追加してください。
	}
	else
	{
		// TODO: 読み込むコードをここに追加してください。
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

void CTonDoc::SetCrtView(CTonView *pcView)
{
	m_pcView = pcView;
}
CData* CTonDoc::NewData(int nItemID)
{
	CData*		pcData;
	//CTonView	*pcView;

	pcData = NULL;
	switch (nItemID) {
	case MOJIVECTID:
		pcData = (CData *)(new CMojiVect(nItemID));
		break;
	}
	if (pcData == NULL) {
		return(NULL);
	}
	pcData->BaseFontLink(m_pcFSF1Font);
	pcData->WNTTFontLink(m_pcWNTTFont);
	pcData->ESF0FontLink(m_pcESF0Font);
	pcData->ESF1FontLink(m_pcESF1Font);
	pcData->FBU0FontLink(m_pcFBU0Font);
	return(pcData);
}

// CTonDoc コマンド

void CTonDoc::FontConvertMain()
{
	TCHAR szExePath[FNAMEMAX];
	TCHAR szFilter[RECMAX];
	TCHAR szDefExt[QRECMAX];
	TCHAR szFileName[RECMAX];
	TCHAR szDataPath[FNAMEMAX];
	CFileDialog *pcFileDialog;
	CString csFileName = _T("");
	FILE	*fp;
	TCHAR	buf[RECMAX];
	int		tateyoko, skuten, ekuten;

	senvgetexepath(szExePath);
	_tcscpy_s(szFilter, _T("コードリストファイル(*.txt)|*.txt||"));
	_tcscpy_s(szDefExt, _T("txt"));
	_tcscpy_s(szFileName, _T("*.txt"));
	_tcscpy_s(szDataPath, szExePath);
	pcFileDialog = new CFileDialog(TRUE, szDefExt, szFileName, OFN_HIDEREADONLY, szFilter, m_pcView);
	pcFileDialog->m_ofn.lpstrInitialDir = szDataPath;
	if (pcFileDialog->DoModal() == IDOK) {
		csFileName = pcFileDialog->GetPathName();
	}
	delete(pcFileDialog);
	if (csFileName == _T("")) {
		return;
	}
	_tcscpy_s(szFileName, csFileName.GetBuffer(FNAMEMAX));
	_tfopen_s(&fp, szFileName, _T("r"));
	if (fp != NULL) {
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), m_szSrcFontName, QRECMAX);
		baselibCrDelFgets(buf, RECMAX, fp);
		_stscanf_s(buf, _T("%s"), m_szDstFontName, QRECMAX);
		FontInitCreate(m_szDstFontName);
		while (1) {
			if (baselibCrDelFgets(buf, RECMAX, fp) == NULL) {
				break;
			}
			if (buf[0] == _T('\0')) {
				break;
			}
			_stscanf_s(buf, _T("%d %d %d"), &m_nTateGaki, &skuten, &ekuten);
			FontConvertLoop(skuten, ekuten);
		}
		fclose(fp);
	}
}
void CTonDoc::FontConvertLoop(int skuten, int ekuten)
{
	CDC *pcDC;
	int kuten, ku, ten;
	int jis, sjis, u16;

	for (kuten = skuten; kuten <= ekuten; kuten++) {
		FontDataLoad(m_szSrcFontName, m_nTateGaki, kuten);
		FontDataSave(m_szDstFontName, m_nTateGaki, kuten);
		pcDC = m_pcView->GetDC();

		FSF1FontDraw(pcDC);
		m_pcView->ReleaseDC(pcDC);
		//m_pcView->Invalidate(TRUE);
		//baseDispatch();
		ten = kuten % 100;
		if (ten >= 95) {
			ten = 1;
			ku = kuten / 100;
			ku++;
			kuten = ku * 100 + ten;
		}
	}
}
void CTonDoc::FSF1FontDraw(CDC *pcDC)
{
	CRect	rect;
	int wd, hi;

	CBrush *pcBkBrush = new CBrush(RGB(255, 255, 255));
	CBrush *pcBrush = new CBrush(RGB(0, 0, 0));
	CPen *pcPen = new CPen(PS_SOLID, 3, RGB(0, 0, 0));
	CBrush *pcOldBrush = pcDC->SelectObject(pcBrush);
	CPen *pcOldPen = pcDC->SelectObject(pcPen);

	m_pcView->GetWindowRect(&rect);
	wd = rect.right - rect.left;
	hi = rect.bottom - rect.top;
	rect.left = 0; rect.right = wd;
	rect.top = 0; rect.bottom = hi;
	pcDC->FillRect(&rect, pcBkBrush);
	delete(pcBkBrush);

	plgnfillareaset(0, 0, 200, 200);
	plgnfillstart();
	m_pcFSF1Font->TrnsElementSet(10.0, 10.0, 100.0, 100.0);
	m_pcFSF1Font->FillDrawOut(pcDC);
	plgnfilldraw(pcDC->m_hDC);
	pcDC->SelectObject(pcOldBrush);
	pcDC->SelectObject(pcOldPen);
	delete(pcBrush);
	delete(pcPen);
}
void CTonDoc::FontInitCreate(TCHAR *lpFontName)
{
	int		nFontType;
	TCHAR	sFaceName[QRECMAX];

	mojienvflistopen(_T("rb"));
	mojienvgetfonttypename(lpFontName, &nFontType, sFaceName);
	mojienvflistclose();
	switch (nFontType) {
	case ES0FONT:
		m_pcESF0Font->FontNameSet(sFaceName);
		m_pcESF0Font->FontNullSet();
		break;
	case ES1FONT:
		m_pcESF1Font->FontNameSet(sFaceName);
		m_pcESF1Font->FontNullSet();
		break;
	case FB0FONT:
		m_pcFBU0Font->FontNameSet(sFaceName);
		m_pcFBU0Font->FontNullSet();
		break;
	}
}
void CTonDoc::FontDataLoad(TCHAR *lpFontName, int nTateGaki, int kuten)
{
	int		nFontType;
	TCHAR	sFaceName[QRECMAX];
	int		jis, sjis, u16;

	jis = basekutentojis(kuten);
	sjis = basejtosj(jis);
	u16 = baselibSjisToU16(sjis);

	mojienvflistopen(_T("rb"));
	mojienvgetfonttypename(lpFontName, &nFontType, sFaceName);
	mojienvflistclose();
	switch (nFontType) {
	case ES0FONT:
		m_pcESF0Font->FontNameSet(sFaceName);
		m_pcESF0Font->SetTateYoko(nTateGaki);
		m_pcESF0Font->SF1TrnsDataLoad(sjis);
		break;
	case ES1FONT:
		m_pcESF1Font->FontNameSet(sFaceName);
		m_pcESF1Font->SetTateYoko(nTateGaki);
		m_pcESF1Font->SF1TrnsDataLoad(sjis);
		break;
	case FB0FONT:
		m_pcFBU0Font->FontNameSet(sFaceName);
		m_pcFBU0Font->SetTateYoko(nTateGaki);
		m_pcFBU0Font->SF1TrnsDataLoad(u16);
		break;
	}
}
void CTonDoc::FontDataSave(TCHAR *lpFontName, int nTateGaki, int kuten)
{
	int		nFontType;
	TCHAR	sFaceName[QRECMAX];
	TCHAR	sGJFaceName[QRECMAX];
	TCHAR	sTategakiFaceName[QRECMAX];
	int		jis, sjis, u16;

	jis = basekutentojis(kuten);
	sjis = basejtosj(jis);
	u16 = baselibSjisToU16(sjis);

	mojienvflistopen(_T("rb"));
	mojienvgetfonttypename(lpFontName, &nFontType, sFaceName);
	mojienvflistclose();
	switch (nFontType) {
	case ES0FONT:
		m_pcESF0Font->FontNameSet(sFaceName);
		m_pcESF0Font->SetTateYoko(nTateGaki);
		m_pcESF0Font->SF1TrnsDataSave(sjis);
		break;
	case ES1FONT:
		m_pcESF1Font->FontNameSet(sFaceName);
		m_pcESF1Font->SetTateYoko(nTateGaki);
		m_pcESF1Font->SF1TrnsDataSave(sjis);
		break;
	case FB0FONT:
		m_pcFBU0Font->FontNameSet(sFaceName);
		m_pcFBU0Font->SetTateYoko(nTateGaki);
		m_pcFBU0Font->SF1TrnsDataReadSave(u16);
		break;
	}
}

