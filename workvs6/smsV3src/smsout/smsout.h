// smsout.h : SMSOUT アプリケーションのメイン ヘッダー ファイル
//

#if !defined(SMSOUT_H)
#define SMSOUT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

class CSysEnv;
/////////////////////////////////////////////////////////////////////////////
// CTonApp:
// このクラスの動作の定義に関しては smsout.cpp ファイルを参照してください。
//

class CTonApp : public CWinApp
{
public:
	CTonApp();
	~CTonApp();

public:
	CDC		m_dcScreen;
	CSysEnv	*m_pcSysEnv;
	COLORREF	m_clrBack;

public:
	void SetupPrinter();
	void AppExit();
	HANDLE GetDevMode();
	HANDLE GetDevNames();
	int CheckEnumPrinter(HANDLE hDevNames);
	void SetDevNamesElement(LPSTR lpDevice, LPSTR lpDriver, LPSTR lpOutput);
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTonApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CTonApp)
	afx_msg void OnAppAbout();
	afx_msg void OnAppExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(SMSOUT_H)
