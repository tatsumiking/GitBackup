
// UtilView.cpp : CTonView クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "Util.h"
#endif

#include "UtilDoc.h"
#include "UtilView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTonView

IMPLEMENT_DYNCREATE(CTonView, CView)

BEGIN_MESSAGE_MAP(CTonView, CView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FONTCONVERT, &CTonView::OnFontconvert)
END_MESSAGE_MAP()

// CTonView コンストラクション/デストラクション

CTonView::CTonView() noexcept
{
	m_nFirstFlag = false;
	m_nCommand = 0;
	// TODO: 構築コードをここに追加します。

}

CTonView::~CTonView()
{
}

BOOL CTonView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CTonView 描画

void CTonView::OnDraw(CDC* pcDC)
{
	CTonDoc* pcDoc = GetDocument();
	ASSERT_VALID(pcDoc);
	if (!pcDoc) {
		return;
	}
	if (m_nFirstFlag == false) {
		pcDoc->SetCrtView(this);
	}
	if (m_nCommand == ID_FONTCONVERT) {
		pcDoc->FSF1FontDraw(pcDC);
	}
	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CTonView の印刷

BOOL CTonView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CTonView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CTonView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}


// CTonView の診断

#ifdef _DEBUG
void CTonView::AssertValid() const
{
	CView::AssertValid();
}

void CTonView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTonDoc* CTonView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTonDoc)));
	return (CTonDoc*)m_pDocument;
}
#endif //_DEBUG


void CTonView::OnFontconvert()
{
	CTonDoc* pcDoc = GetDocument();
	m_nCommand = ID_FONTCONVERT;
	pcDoc->FontConvertMain();

}
