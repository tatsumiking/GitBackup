// sgs.h : SGS アプリケーションのメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CTonApp:
// このクラスの動作の定義に関しては es98.cpp ファイルを参照してください。
//
class CCharLib;
class CSysEnv;

class CTonApp : public CWinApp
{
public:
	CTonApp();
	~CTonApp();

public:
	CCharLib *m_pcCharLib;
	CSysEnv	*m_pcSysEnv;

	HANDLE	m_hEvent;
	CDC m_dcScreen;
	int m_nDefFont;
	TCHAR	m_szArgv1[FNAMEMAX];
	TCHAR	m_szSavePath[FNAMEMAX];
	TCHAR	m_szSaveName[FNAMEMAX];
	TCHAR	m_szBackImagePath[FNAMEMAX];
	HANDLE	m_hTempStr;	// 作業文字列バッファハンドル
	LPTSTR	m_lpTempStr;
	COLORREF	m_clrBack;
	BOOL m_bWin4;
#ifndef _UNICODE
	BOOL m_bWin31;
#endif
	BOOL m_bLargeIcons;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTonApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	HANDLE GetDevMode();
	HANDLE GetDevNames();
	int CheckEnumPrinter(HANDLE hDevNames);
	void SetDevNamesElement(LPTSTR lpDevice, LPTSTR lpDriver, LPTSTR lpOutput);

	COleTemplateServer m_server;
		// ドキュメントを作成するためのサーバー オブジェクト

	//{{AFX_MSG(CTonApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFilePrintSetup();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。


