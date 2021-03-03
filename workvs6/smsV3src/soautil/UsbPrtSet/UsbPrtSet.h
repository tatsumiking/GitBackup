// UsbPrtSet.h : USBPRTSET アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(USBPRTSET_H)
#define USBPRTSET_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CUsbPrtSetApp:
// このクラスの動作の定義に関しては UsbPrtSet.cpp ファイルを参照してください。
//

class CUsbPrtSetApp : public CWinApp
{
public:
	CUsbPrtSetApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CUsbPrtSetApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CUsbPrtSetApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(USBPRTSET_H)
