// SrvrItem.cpp : CTonSrvrItem アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"

#include "SrvrItem.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTonSrvrItem インプリメンテーション

IMPLEMENT_DYNAMIC(CTonSrvrItem, COleServerItem)

CTonSrvrItem::CTonSrvrItem(CTonDoc* pContainerDoc)
	: COleServerItem(pContainerDoc, TRUE)
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
	//  (例えば、アイテム データ ソース用の特別なクリップボード形式の追加)
}

CTonSrvrItem::~CTonSrvrItem()
{
	// TODO: この場所に後処理を追加してください。
}

void CTonSrvrItem::Serialize(CArchive& ar)
{
	// CTonSrvrItem::Serialize はアイテムがクリップボードにコピーさ
	// れているときには、フレームワークによって呼ばれます。これは OLE コール
	// バック OnGetClipboardData を通り越して自動的に行わせることができます。
	// 埋め込みアイテムのデフォルトは簡単なドキュメントのシリアライズ関数の代
	// わりをします。
	// リンクをサポートしているときは、ドキュメントの一部をシリアライズします。

	if (!IsLinkedItem())
	{
		CTonDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pDoc->Serialize(ar);
	}
}

BOOL CTonSrvrItem::OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize)
{
	// 多くのアプリケーションでは、アイテムのコンテンツ アスペクトの描画だけをハンドルします。
	//  OnDrawEx がオーバーライドしている DVASPECT_THUMBNAIL で他のアスペクトもサポートする
	// 時には、OnGetExtent を追加するアスペクトもハンドルするように変更してください。
	//
	//

	if (dwDrawAspect != DVASPECT_CONTENT)
		return COleServerItem::OnGetExtent(dwDrawAspect, rSize);

	// CTonSrvrItem::OnGetExtent はアイテム全体の HIMETRIC 単位の範囲を得る
	// ために 呼ばれます。デフォルトは、単純にハードコーディングされた値を返します。
	//

	CTonDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: 適当なサイズに置き換えてください。

	rSize = CSize(3000, 3000);   // 3000 x 3000 HIMETRIC 単位

	return TRUE;
}

BOOL CTonSrvrItem::OnDraw(CDC* pDC, CSize& rSize)
{
	// rSize を使用する場合はここを削除してください
	UNREFERENCED_PARAMETER(rSize);

	CTonDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: マッピング モードと範囲の設定。
	//  (範囲は通常 OnGetExtent から返されたサイズと同じです)
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowOrg(0,0);
	pDC->SetWindowExt(3000, 3000);

	// TODO: この場所に描画用のコードを追加します。任意で HIMETRIC の範囲を塗りつぶしてください。
	//       メタファイル デバイス コンテキスト (pDC) のすべてを描画します。

	// TODO: CTonCntrItem の埋め込みオブジェクトも描画してください。

	// 以下に示すコードは最初のアイテムを適当な位置へ描画します。

	// TODO: このコードは実際の描画用のコードが確定したら削除してください。

	POSITION pos = pDoc->GetStartPosition();
	CTonCntrItem* pItem = (CTonCntrItem*)pDoc->GetNextClientItem(pos);
	if (pItem != NULL)
		pItem->Draw(pDC, CRect(10, 10, 1010, 1010));
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTonSrvrItem クラスの診断

#ifdef _DEBUG
void CTonSrvrItem::AssertValid() const
{
	COleServerItem::AssertValid();
}

void CTonSrvrItem::Dump(CDumpContext& dc) const
{
	COleServerItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
