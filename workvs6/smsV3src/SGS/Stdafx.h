// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__D2ED8FA8_417C_11D2_99B2_004026171D2B__INCLUDED_)
#define AFX_STDAFX_H__D2ED8FA8_417C_11D2_99B2_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxole.h>         // MFC の OLE クラス
#include <afxodlgs.h>       // MFC の OLE ダイアログ クラス
#include <afxdisp.h>        // MFC の OLE オートメーション クラス
#include <afxdlgs.h>        // MFC の ダイアログクラス
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サ
#include <afxtempl.h>		// MFC のテンプレート
#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT
#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#include <imm.h>

#include "..\stdef.h"
#include "..\defenv.h"

#include "resource.h"
#include "sysenv.h"
#include "sgs.h"

#include "..\cmnsrc\mydialog.h"
#include "..\cmnsrc\mydlgbar.h"
#include "..\cmnsrc\myedit.h"
#include "..\cmnsrc\myfiledl.h"
#include "..\cmnsrc\mybutton.h"
#include "sgsDoc.h"
#include "sgsView.h"
#include "MainFrm.h"
#include "MainFrm.h"

// CG: 次の行は「IME サポート」コンポーネントにより追加されています。
#pragma comment(lib, "imm32.lib")
#endif // !defined(AFX_STDAFX_H__D2ED8FA8_417C_11D2_99B2_004026171D2B__INCLUDED_)
