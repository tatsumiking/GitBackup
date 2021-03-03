// CntrItem.cpp : CEs98CntrItem クラスの動作の定義を行います。
//

#include "stdafx.h"

#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTonCntrItem インプリメンテーション

IMPLEMENT_SERIAL(CTonCntrItem, COleClientItem, 0)

CTonCntrItem::CTonCntrItem(CTonDoc* pContainer)
	: COleClientItem(pContainer)
{
	// TODO: この場所に構築処理用のコードを追加してください。

	m_rectPosition.SetRect(10, 10, 200, 200);
}

CTonCntrItem::~CTonCntrItem()
{
	// TODO: この場所に後処理用コードを追加してください。
	
}

void CTonCntrItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
	ASSERT_VALID(this);

	COleClientItem::OnChange(nCode, dwParam);

	// アイテムが編集されている時には (組み込み先でも 開いていても)
	// アイテムの状態またはその内容の画面表示の変更に対し、OnChange
	// 通知を送ります。

	// TODO: UpdateAllViews を呼び出してアイテムを無効にしてください。
	//  (アプリケーション特有のヒントを添えて)

	GetDocument()->UpdateAllViews(NULL);
		// すべてのビューをヒント情報なしでアップデートしました。
}

BOOL CTonCntrItem::OnChangeItemPosition(const CRect& rectPos)
{
	ASSERT_VALID(this);

	// 組み込み先が有効な間は CTonCntrItem::OnChangeItemPosition は組み込み先
	// ウィンドウの位置を変更するためにサーバーによって呼び出されます。通常、これは
	// サーバー ドキュメントのデータを変更したことによって範囲が変わったり、組み込み
	// 先のサイズを変更したことの結果です。
	//
	//アイテムを新しい位置に移動するために基本
	// クラスの COleClientItem::SetItemRects を呼び出します。
	//
	//

	if (!COleClientItem::OnChangeItemPosition(rectPos))
		return FALSE;

	// TODO: アイテムの矩形/範囲のキャッシュを更新してください。
	m_rectPosition = rectPos;
	return TRUE;
}

void CTonCntrItem::OnGetItemPosition(CRect& rPosition)
{
	ASSERT_VALID(this);
	rPosition = m_rectPosition;
	// 組み込み先が起動している間、CTonCntrItem::OnGetItemPosition はアイテムの
	// 位置を決定するために呼ばれます。AppWizard で作成されたデフォルトのコードは単純
	// にハードコーディングされた矩形を返します。
	// 通常、この矩形は使われているビューに関連するアイテムの現在の位置を反映します。
	// CTonCntrItem::GetActiveView を呼び出すことによってビューを得ることができ
	// ます。

	// TODO: rPosition にはピクセル単位で正しい矩形を返してください。

	rPosition.SetRect(10, 10, 210, 210);
}

void CTonCntrItem::OnActivate()
{
    // フレームごとに一つだけアイテムのビジュアル編集を可能にします。
    CTonView* pView = GetActiveView();
    ASSERT_VALID(pView);
    COleClientItem* pItem = GetDocument()->GetInPlaceActiveItem(pView);
    if (pItem != NULL && pItem != this)
        pItem->Close();
    
    COleClientItem::OnActivate();
}

void CTonCntrItem::OnDeactivateUI(BOOL bUndoable)
{
	COleClientItem::OnDeactivateUI(bUndoable);

    // オブジェクトがアウトサイド イン オブジェクトではない場合は非表示にします。
    DWORD dwMisc = 0;
    m_lpObject->GetMiscStatus(GetDrawAspect(), &dwMisc);
    if (dwMisc & OLEMISC_INSIDEOUT)
        DoVerb(OLEIVERB_HIDE, NULL);
}

void CTonCntrItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	// 最初の基本クラスの呼び出しは COleClientItem
	// データへ読み込まれます。CTonCntrItem::GetDocument
	// から返された m_pDocument ポインタを設定してから基本クラ
	// スを呼び出して最初のシリアライズを行ってください。
	COleClientItem::Serialize(ar);

	// CTonCntrItem に対してデータの保存/読み込みを行います。
	if (ar.IsStoring())
	{
		// TODO: この場所に保存用のコードを追加します。
		ar << m_rectPosition;
	}
	else
	{
		// TODO: この場所に読み込み用のコードを追加します。
		ar >> m_rectPosition;
	}
}

BOOL CTonCntrItem::CanActivate()
{
	// サーバーで編集されているときには組み込み先での編集は
	// サポートされていません。 この場合は組み込み先の起動  
	// を無効にしてください。
	CTonDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COleServerDoc)));
	if (pDoc->IsInPlaceActive())
		return FALSE;

	RefreshSize();
	// その他はデフォルトの処理に任せます。
	return COleClientItem::CanActivate();
}

void CTonCntrItem::RefreshSize()
{
	CSize size;
	if (GetCachedExtent(&size))
	{
		CClientDC dc(NULL);
		dc.HIMETRICtoDP(&size);

		m_rectPosition.bottom = m_rectPosition.top + size.cy;
		m_rectPosition.right = m_rectPosition.left + size.cx;

		CDocument* pDoc = GetDocument();
		ASSERT(pDoc != NULL);
		pDoc->SetModifiedFlag();
		pDoc->UpdateAllViews(NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTonCntrItem クラスの診断

#ifdef _DEBUG
void CTonCntrItem::AssertValid() const
{
	COleClientItem::AssertValid();
}

void CTonCntrItem::Dump(CDumpContext& dc) const
{
	COleClientItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
