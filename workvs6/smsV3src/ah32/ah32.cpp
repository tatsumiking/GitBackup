// ah32.cpp : アプリケーション用クラスの定義を行います。
//

#include "stdafx.h"
#include "ah32.h"
#include "ah32Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAh32App

BEGIN_MESSAGE_MAP(CAh32App, CWinApp)
	//{{AFX_MSG_MAP(CAh32App)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAh32App クラスの構築

CAh32App::CAh32App()
{
	m_pcSysEnv = new CSysEnv();
	m_pcFuncLib = new CFuncLib();
}

CAh32App::~CAh32App()
{
	delete(m_pcFuncLib);
	delete(m_pcSysEnv);
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CAh32App オブジェクト

CAh32App theApp;

/////////////////////////////////////////////////////////////////////////////
// CAh32App クラスの初期化

BOOL CAh32App::InitInstance()
{
	AfxEnableControlContainer();

	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さくしたけ
	//  れば以下の特定の初期化ルーチンの中から不必要なものを削除して
	//  ください。

	CAh32Dlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//       記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <ｷｬﾝｾﾙ> で消された時のコードを
		//       記述してください。
	}

	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}

