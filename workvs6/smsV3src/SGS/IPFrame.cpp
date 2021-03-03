// IpFrame.cpp : CInPlaceFrame クラスの動作の定義を行います。
//

#include "stdafx.h"

#include "IpFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame

IMPLEMENT_DYNCREATE(CInPlaceFrame, COleIPFrameWnd)

BEGIN_MESSAGE_MAP(CInPlaceFrame, COleIPFrameWnd)
	//{{AFX_MSG_MAP(CInPlaceFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame クラスの構築/消滅

CInPlaceFrame::CInPlaceFrame()
{
}

CInPlaceFrame::~CInPlaceFrame()
{
}

int CInPlaceFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleIPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// CResizeBar は埋め込み先でのサイズの変更を実現します。
	if (!m_wndResizeBar.Create(this))
	{
		TRACE0("Failed to create resize bar\n");
		return -1;      // 作成に失敗
	}

	// デフォルトで、ドロップ ターゲットをフレームウィンドウと共に登録し
	// ないのは、良い考えです。これは "falling through" ドロップからコ
	// ンテナのドラッグ アンド ドロップ サポートを保護します。
	m_dropTarget.Register(this);

	return 0;
}

// OnCreateControlBars はコンテナ アプリケーションのウィンドウ上にツールバーを作成するために、
// フレームワークから呼ばれます。pWndFrame はコンテナのトップ レベルのフレーム ウィンドウで、
// 常に NULL ではありません。pWndDoc はドキュメント レベルのフレーム ウィンドウでコンテナが SDI
// アプリケーションのときに NULL になります。サーバー アプリケーションは他のウィンドウに MFC の
// コントロール バーを配置できます。
BOOL CInPlaceFrame::OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc)
{
	// pWndDoc を使用する場会はこれを削除します。
	UNREFERENCED_PARAMETER(pWndDoc);

	// メッセージが正しいアプリケーションに渡されるように、このウィンドウにオーナーを設定します。
	m_wndToolBar.SetOwner(this);

	// クライアントのフレーム ウィンドウ上にツールバーを作成。
	if (!m_wndToolBar.Create(pWndFrame) ||
		!m_wndToolBar.LoadToolBar(IDR_SGSTYPE_SRVR_IP))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;
	}

	// TODO: ツール チップスが必要ない場合、ここを削除してください。
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: ツール バーをドッキング可能にしない場合は以下の３行を
	//       削除してください。
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	pWndFrame->EnableDocking(CBRS_ALIGN_ANY);
	pWndFrame->DockControlBar(&m_wndToolBar);

	return TRUE;
}

BOOL CInPlaceFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイル
	//       を修正してください。

	return COleIPFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame クラスの診断

#ifdef _DEBUG
void CInPlaceFrame::AssertValid() const
{
	COleIPFrameWnd::AssertValid();
}

void CInPlaceFrame::Dump(CDumpContext& dc) const
{
	COleIPFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame コマンド
