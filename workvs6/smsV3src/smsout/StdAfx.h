// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__2D225333_631B_4625_85F5_6032071C2127__INCLUDED_)
#define AFX_STDAFX_H__2D225333_631B_4625_85F5_6032071C2127__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdisp.h>        // MFC のオートメーション クラス
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
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

#include <imm.h>

#include "..\stdef.h"
#include "..\defenv.h"

#include "Resource.h"

#include "smsout.h"
#include "sysenv.h"

#include "smsoutDoc.h"
#include "smsoutView.h"
#include "MainFrm.h"
// CG: 次の行は「IME サポート」コンポーネントにより追加されています。
#pragma comment(lib, "imm32.lib")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__2D225333_631B_4625_85F5_6032071C2127__INCLUDED_)
