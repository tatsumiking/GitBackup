// UPTDoc.cpp : CTonDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "UsbPrtTest.h"

#include "UPTDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

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
	if (ar.IsStoring())
	{
		// TODO: この位置に保存用のコードを追加してください。
	}
	else
	{
		// TODO: この位置に読み込み用のコードを追加してください。
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
