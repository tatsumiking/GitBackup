
// Ses.h : Ses アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CTonApp:
// このクラスの実装については、Ses.cpp を参照してください
//

class CCharLib;
class CSysEnv;

class CTonApp : public CWinApp
{
public:
	CTonApp() noexcept;
	~CTonApp();

public:
	CCharLib *m_pcCharLib;
	CSysEnv	*m_pcSysEnv;
	CDC m_dcScreen;
	int m_nDefFont;
	TCHAR	m_szArgv1[FNAMEMAX];
	HANDLE	m_hTempDevNames;
	HANDLE	m_hTempDevMode;
	TCHAR	m_szSavePath[FNAMEMAX];
	TCHAR	m_szSaveName[FNAMEMAX];
	int		m_nNoDispFlag;
	TCHAR	m_szBackImagePath[FNAMEMAX];
	TCHAR	m_szPhotoImagePath[FNAMEMAX];
	TCHAR	m_szTempImagePath[FNAMEMAX];
	TCHAR	m_szBackBmpName[FNAMEMAX];
	HANDLE	m_hTempStr;	// 作業文字列バッファハンドル
	LPTSTR	m_lpTempStr;
	COLORREF	m_clrBack;
	int		m_nMajor;
	int		m_nMinor;
	BOOL m_bWin4;
#ifndef _UNICODE
	BOOL m_bWin31;
#endif
	BOOL m_bLargeIcons;


// オーバーライド
public:
	void CreateWakuNameList(LPTSTR lpWakuPath);
	void AppExit();
	HANDLE GetDevMode();
	HANDLE GetDevNames();
	int CheckEnumPrinter(HANDLE hDevNames);
	int SetDevNamesElement(LPTSTR lpDevice, LPTSTR lpDriver, LPTSTR lpOutput);

	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	COleTemplateServer m_server;
		// ドキュメントを作成するためのサーバー オブジェクト
	afx_msg void OnAppAbout();
	afx_msg void OnFilePrintSetup();
	DECLARE_MESSAGE_MAP()
};

extern CTonApp theApp;
