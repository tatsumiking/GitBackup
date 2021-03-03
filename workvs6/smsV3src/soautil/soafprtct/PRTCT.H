// prtct.h : PRTCT アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_PRTCT_H__71320365_D9FE_11D3_9F64_0000E835797C__INCLUDED_)
#define AFX_PRTCT_H__71320365_D9FE_11D3_9F64_0000E835797C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CPrtctApp:
// このクラスの動作の定義に関しては prtct.cpp ファイルを参照してください。
//

class CPrtctApp : public CWinApp
{
public:
	CPrtctApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPrtctApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CPrtctApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PRTCT_H__71320365_D9FE_11D3_9F64_0000E835797C__INCLUDED_)
