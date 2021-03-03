// ah32.h : AH32 アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_AH32_H__657DBB45_862F_11D3_9DFC_00A0B0086478__INCLUDED_)
#define AFX_AH32_H__657DBB45_862F_11D3_9DFC_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CAh32App:
// このクラスの動作の定義に関しては ah32.cpp ファイルを参照してください。
//

class CAh32App : public CWinApp
{
public:
	CAh32App();
	~CAh32App();

public:
	CSysEnv	*m_pcSysEnv;
	CFuncLib	*m_pcFuncLib;
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAh32App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CAh32App)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_AH32_H__657DBB45_862F_11D3_9DFC_00A0B0086478__INCLUDED_)
