// sgs.h : SGS アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_SGS_H__D2ED8FA6_417C_11D2_99B2_004026171D2B__INCLUDED_)
#define AFX_SGS_H__D2ED8FA6_417C_11D2_99B2_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CTonApp:
// このクラスの動作の定義に関しては es98.cpp ファイルを参照してください。
//

class CTonApp : public CWinApp
{
public:
	CTonApp();
	~CTonApp();

public:
	CSysEnv	*m_pcSysEnv;

	HANDLE	m_hEvent;
	CDC m_dcScreen;
	int m_nDefFont;
	char	m_szArgv1[FNAMEMAX];
	char	m_szSavePath[FNAMEMAX];
	char	m_szSaveName[FNAMEMAX];
	char	m_szBackImagePath[FNAMEMAX];
	HANDLE	m_hTempStr;	// 作業文字列バッファハンドル
	LPSTR	m_lpTempStr;
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
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	HANDLE GetDevMode();
	HANDLE GetDevNames();
	int CheckEnumPrinter(HANDLE hDevNames);
	void SetDevNamesElement(LPSTR lpDevice, LPSTR lpDriver, LPSTR lpOutput);

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

#endif // !defined(AFX_SGS_H__D2ED8FA6_417C_11D2_99B2_004026171D2B__INCLUDED_)

