// MyCmnLib.h : MYCMNLIB アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_MYCMNLIB_H__D2A3DDEB_2404_11D3_A712_00A0B0086478__INCLUDED_)
#define AFX_MYCMNLIB_H__D2A3DDEB_2404_11D3_A712_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CMyCmnLibApp
// このクラスの動作の定義に関しては MyCmnLib.cpp ファイルを参照してください。
//

class CMyCmnLibApp : public CWinApp
{
public:
	CMyCmnLibApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMyCmnLibApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMyCmnLibApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MYCMNLIB_H__D2A3DDEB_2404_11D3_A712_00A0B0086478__INCLUDED_)
