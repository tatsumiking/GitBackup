// UPTView.cpp : CTonView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "UsbPrtTest.h"

#include "UPTDoc.h"
#include "UPTView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTonView

IMPLEMENT_DYNCREATE(CTonView, CView)

BEGIN_MESSAGE_MAP(CTonView, CView)
	//{{AFX_MSG_MAP(CTonView)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//    この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTonView クラスの構築/消滅

CTonView::CTonView()
{
	// TODO: この場所に構築用のコードを追加してください。

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

/////////////////////////////////////////////////////////////////////////////
// CTonView クラスの描画

void CTonView::OnDraw(CDC* pDC)
{
	CTonDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}

/////////////////////////////////////////////////////////////////////////////
// CTonView クラスの印刷

BOOL CTonView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// デフォルトの印刷準備
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

/////////////////////////////////////////////////////////////////////////////
// CTonView クラスの診断

#ifdef _DEBUG
void CTonView::AssertValid() const
{
	CView::AssertValid();
}

void CTonView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTonDoc* CTonView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTonDoc)));
	return (CTonDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTonView クラスのメッセージ ハンドラ
