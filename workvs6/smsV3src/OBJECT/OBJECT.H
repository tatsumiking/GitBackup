// Object.h : OBJECT アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_OBJECT_H__AFCD19F1_B1E0_11D1_8006_0000E835797C__INCLUDED_)
#define AFX_OBJECT_H__AFCD19F1_B1E0_11D1_8006_0000E835797C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CObjectApp
// このクラスの動作の定義に関しては Object.cpp ファイルを参照してください。
//

class CObjectApp : public CWinApp
{
public:
	CObjectApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CObjectApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CObjectApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OBJECT_H__AFCD19F1_B1E0_11D1_8006_0000E835797C__INCLUDED_)
