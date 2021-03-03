// UsbPrtTest.h : USBPRTTEST アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_USBPRTTEST_H__3681B068_9F3E_4861_B3A7_EF6DE88A12AD__INCLUDED_)
#define AFX_USBPRTTEST_H__3681B068_9F3E_4861_B3A7_EF6DE88A12AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CTonApp:
// このクラスの動作の定義に関しては UsbPrtTest.cpp ファイルを参照してください。
//

class CTonApp : public CWinApp
{
public:
	CTonApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTonApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CTonApp)
	afx_msg void OnAppAbout();
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_USBPRTTEST_H__3681B068_9F3E_4861_B3A7_EF6DE88A12AD__INCLUDED_)
